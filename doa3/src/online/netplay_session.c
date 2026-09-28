/*
 * netplay_session.c - lockstep session core for online VS play (see header).
 *
 * Guest addresses come from the Phase 1 investigation (NOTES/plan):
 *   0x47E723  game mode (4 = Versus)       0x48E612  Tag match flag
 *   0x480B70  mode (0 menus, 1 fight, 2 attract/title)
 *   0x8612AD  mode-select screen state (3 = character select)
 *   0x484D78  in-memory save image (DOA3SAVE.DAT, 0x3C10 bytes)
 *   0x5E5CD0  pad structs (stride 0x80), 0x5E5ED8 per-frame aggregates
 *   [0x1C]+0x14  CRT rand() seed (the port's _getptd resolves through 0x1C)
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netplay_session.h"
#include "game/recomp/recomp_types.h"
#include "kernel/kernel.h"
#include "xbox_det.h"

void sub_000BB270(void);                       /* rebuilds the per-port button maps */
void doa3_netplay_force_pads(uint32_t mask);   /* recomp_manual.c */

#define GAME_MODE_VA   0x0047E723u
#define GAME_MODE_VS   4
#define TAG_FLAG_VA    0x0048E612u
#define MODE_VA        0x00480B70u
#define SCRST_VA       0x008612ADu
#define SCRST_CHARSEL  3

#define SAVE_VA        0x00484D78u
#define SAVE_LEN       0x3C10u
#define SAVE_VOL_MUSIC 0x48      /* audio settings: kept local */
#define SAVE_VOL_SE    0x49
#define SAVE_LAYOUT(p) (0x0Fu + 10u * (p))   /* custom button layout, 10 bytes */
#define SAVE_LAYSEL(p) (0x3BF5u + (p))       /* layout selector */

#define PAD_VA(p)      (0x005E5CD0u + 0x80u * (p))
#define PAD_RAW_A      0x19u     /* the slot the aggregate builder reads */
#define PAD_RAW_B      0x2Fu     /* the slot XInputGetState is handed */
#define AGG_VA         0x005E5ED8u
#define AGG_END        0x005E5FA4u
#define SESSION_PADS   0x3u      /* ports 0 (host) and 1 (joiner) */

/* Fight state carried in the session start parameters (so both machines
 * start from identical bytes, whatever each ran before) and digested every
 * frame. All pointer-free (Phase 1). */
typedef struct { uint32_t va, len; const char *name; } np_region;
static const np_region k_sync[] = {
    { 0x00484C48u, 0x0D0u,        "side" },      /* 2 x 0x68 per-side state */
    { 0x004785D0u, 0x5C0u * 4u,   "fighter" },   /* 4 x 0x5C0 fighter records */
    { 0x004815A0u, 0x2D07u,       "b4815A0" },
    { 0x0047E274u, 0x0467u,       "b47E274" },
    { 0x00488AA8u, 0x00F7u,       "b488AA8" },
};
#define N_SYNC (sizeof(k_sync) / sizeof(k_sync[0]))
static const np_region k_extra[] = {
    { AGG_VA,      AGG_END - AGG_VA, "pads" },
    { 0x0048E608u, 0x0Cu,         "players" },   /* human bytes + tag flag */
    { 0x0048A470u, 0x01u,         "pause" },
    { 0x0047E714u, 0x01u,         "pauser" },
    { GAME_MODE_VA, 0x01u,        "gmode" },
};
#define N_EXTRA (sizeof(k_extra) / sizeof(k_extra[0]))
#define N_REGION_HASH (N_SYNC + N_EXTRA + 1)     /* + seed/clock */

/* Session start parameters. The host chooses them; the joiner receives
 * them. A local session builds them from this machine. */
typedef struct {
    uint32_t       seed;
    xbox_det_bases bases;
    uint8_t        save[SAVE_LEN];
    uint8_t        joiner_layout[10];
    uint8_t        joiner_laysel;
    uint8_t       *sync;          /* concatenated k_sync bytes */
    uint32_t       sync_len;
} np_params;

typedef struct {
    uint32_t frame;
    uint64_t digest;
    uint32_t region[N_REGION_HASH];
    np_pad   in[2];
    uint8_t  mode, scrst, gmode, pad_;
} np_frame_rec;

static volatile int s_armed;
static volatile int s_mirror;
static volatile int s_role = NP_ROLE_HOST;
static int      s_state = NP_STATE_OFF;
static int      s_active_role;
static int      s_tag;
static uint32_t s_frame;
static uint64_t s_digest;
static uint32_t s_sessions;
static char     s_last_log[260];
static np_params s_params;
static uint8_t  s_save_backup[SAVE_LEN];      /* joiner: restored at the end */
static np_frame_rec *s_rec;
static uint32_t s_rec_n, s_rec_cap;
#define REC_MAX (60u * 60u * 60u)             /* one hour of frames */

/* ── guest memory helpers ──────────────────────────────────────────── */

static void guest_read(uint32_t va, void *dst, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++) ((uint8_t *)dst)[i] = MEM8(va + i);
}

static void guest_write(uint32_t va, const void *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++) MEM8(va + i) = ((const uint8_t *)src)[i];
}

static uint32_t rng_seed_va(void)
{
    uint32_t ptd = MEM32(0x1Cu);
    return ptd ? ptd + 0x14u : 0;
}

static uint32_t fnv32(uint32_t h, uint32_t va, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++) { h ^= MEM8(va + i); h *= 16777619u; }
    return h;
}

static void pad_from_guest(uint32_t port, np_pad *p)
{
    uint32_t s = PAD_VA(port) + PAD_RAW_A;
    int i;
    p->buttons = MEM16(s + 4);
    for (i = 0; i < 8; i++) p->analog[i] = MEM8(s + 6 + i);
    p->lx = (int16_t)MEM16(s + 14); p->ly = (int16_t)MEM16(s + 16);
    p->rx = (int16_t)MEM16(s + 18); p->ry = (int16_t)MEM16(s + 20);
}

static void pad_to_guest(uint32_t port, const np_pad *p, uint32_t packet)
{
    uint32_t slots[2] = { PAD_VA(port) + PAD_RAW_A, PAD_VA(port) + PAD_RAW_B };
    int k, i;
    for (k = 0; k < 2; k++) {
        uint32_t s = slots[k];
        MEM32(s) = packet;
        MEM16(s + 4) = p->buttons;
        for (i = 0; i < 8; i++) MEM8(s + 6 + i) = p->analog[i];
        MEM16(s + 14) = (uint16_t)p->lx; MEM16(s + 16) = (uint16_t)p->ly;
        MEM16(s + 18) = (uint16_t)p->rx; MEM16(s + 20) = (uint16_t)p->ry;
    }
}

/* Rebuild the live button maps from the save image. A guest call: push the
 * dummy return slot the lifted `ret` pops, and keep the caller's registers. */
static void rebuild_button_maps(void)
{
    uint32_t sv[7] = { g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp };
    PUSH32(g_esp, 0);
    sub_000BB270();
    g_eax = sv[0]; g_ecx = sv[1]; g_edx = sv[2];
    g_ebx = sv[3]; g_esi = sv[4]; g_edi = sv[5]; g_esp = sv[6];
}

/* ── session start / end ───────────────────────────────────────────── */

static int at_barrier(void)
{
    return MEM8(GAME_MODE_VA) == GAME_MODE_VS && MEM8(MODE_VA) == 0 &&
           MEM8(SCRST_VA) == SCRST_CHARSEL;
}

/* The players left Versus. Going back to character select after a fight
 * passes through screen state 0 (the mode-select screen re-initialises)
 * before reaching 3, so state 0 alone is not an exit; backing out does
 * pass through the Single/Tag submenu (2), and QUIT goes through the title
 * (mode 2). The frame limit covers any other way out of the menus. */
#define SCRST_VS_SUBMENU    2
#define MENU_EXIT_FRAMES    600
static uint32_t s_menu_frames;

static int left_versus(void)
{
    uint8_t mode = MEM8(MODE_VA), scrst = MEM8(SCRST_VA);
    if (MEM8(GAME_MODE_VA) != GAME_MODE_VS) return 1;
    if (mode == 2) return 1;                              /* title / attract */
    if (mode == 0 && scrst == SCRST_VS_SUBMENU) return 1;
    if (mode == 0 && scrst < SCRST_CHARSEL) {
        if (++s_menu_frames > MENU_EXIT_FRAMES) return 1;
    } else {
        s_menu_frames = 0;
    }
    return 0;
}

/* Parameters for a session where both players are on this machine. */
static void make_local_params(np_params *p)
{
    LARGE_INTEGER q;
    uint32_t i, off = 0;
    QueryPerformanceCounter(&q);
    p->seed = (uint32_t)q.QuadPart ^ (uint32_t)(q.QuadPart >> 32) ^ GetTickCount();
    xbox_det_current_bases(&p->bases);
    guest_read(SAVE_VA, p->save, SAVE_LEN);
    guest_read(SAVE_VA + SAVE_LAYOUT(1), p->joiner_layout, 10);
    p->joiner_laysel = MEM8(SAVE_VA + SAVE_LAYSEL(1));
    p->sync_len = 0;
    for (i = 0; i < N_SYNC; i++) p->sync_len += k_sync[i].len;
    free(p->sync);
    p->sync = (uint8_t *)malloc(p->sync_len);
    for (i = 0; i < N_SYNC && p->sync; i++) {
        guest_read(k_sync[i].va, p->sync + off, k_sync[i].len);
        off += k_sync[i].len;
    }
}

static void apply_params(const np_params *p, int role)
{
    uint32_t i, off = 0, sva;
    uint8_t vol[2];

    /* Match settings: the whole save image except this machine's volumes,
     * with port 1 carrying the joiner's own button layout. */
    vol[0] = MEM8(SAVE_VA + SAVE_VOL_MUSIC);
    vol[1] = MEM8(SAVE_VA + SAVE_VOL_SE);
    if (role == NP_ROLE_JOINER) {
        guest_read(SAVE_VA, s_save_backup, SAVE_LEN);
        xbox_path_set_title_redirect(1);
    }
    guest_write(SAVE_VA, p->save, SAVE_LEN);
    MEM8(SAVE_VA + SAVE_VOL_MUSIC) = vol[0];
    MEM8(SAVE_VA + SAVE_VOL_SE) = vol[1];
    guest_write(SAVE_VA + SAVE_LAYOUT(1), p->joiner_layout, 10);
    MEM8(SAVE_VA + SAVE_LAYSEL(1)) = p->joiner_laysel;
    rebuild_button_maps();

    /* Fight state: identical starting bytes on both machines. */
    for (i = 0; i < N_SYNC && p->sync; i++) {
        guest_write(k_sync[i].va, p->sync + off, k_sync[i].len);
        off += k_sync[i].len;
    }

    sva = rng_seed_va();
    if (sva) MEM32(sva) = p->seed;

    /* Pads: ports 0 and 1 open, 2 and 3 closed, no carried-over history. */
    doa3_netplay_force_pads(SESSION_PADS);
}

static void write_log(void)
{
    SYSTEMTIME t;
    FILE *f;
    uint32_t i, k;
    GetLocalTime(&t);
    _snprintf(s_last_log, sizeof(s_last_log) - 1,
              "netplay_digest_%04u%02u%02u_%02u%02u%02u.txt",
              t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    f = fopen(s_last_log, "w");
    if (!f) { s_last_log[0] = 0; return; }
    fprintf(f, "# DOA3 netplay session digest\n");
    fprintf(f, "# role=%s battle=%s frames=%u seed=%08X tick_base=%u qpc_base=%llu ft_base=%llu\n",
            s_active_role == NP_ROLE_JOINER ? "joiner" : "host", s_tag ? "tag" : "single",
            s_rec_n, s_params.seed, s_params.bases.tick_ms,
            (unsigned long long)s_params.bases.qpc, (unsigned long long)s_params.bases.filetime);
    fprintf(f, "# frame digest");
    for (k = 0; k < N_SYNC; k++) fprintf(f, " %s", k_sync[k].name);
    for (k = 0; k < N_EXTRA; k++) fprintf(f, " %s", k_extra[k].name);
    fprintf(f, " seedclock p1 p2 mode/scrst/gmode\n");
    for (i = 0; i < s_rec_n; i++) {
        const np_frame_rec *r = &s_rec[i];
        fprintf(f, "%u %016llX", r->frame, (unsigned long long)r->digest);
        for (k = 0; k < N_REGION_HASH; k++) fprintf(f, " %08X", r->region[k]);
        fprintf(f, " %04X %04X %u/%u/%u\n", r->in[0].buttons, r->in[1].buttons,
                r->mode, r->scrst, r->gmode);
    }
    fclose(f);
}

static void end_session(void)
{
    xbox_det_end();
    if (s_active_role == NP_ROLE_JOINER) {
        guest_write(SAVE_VA, s_save_backup, SAVE_LEN);
        rebuild_button_maps();
        xbox_path_set_title_redirect(0);
    }
    write_log();
    s_sessions++;
    s_rec_n = 0;
    s_state = s_armed ? NP_STATE_ARMED : NP_STATE_OFF;
}

static void begin_session(void)
{
    s_active_role = s_role;
    make_local_params(&s_params);
    apply_params(&s_params, s_active_role);
    s_tag = MEM8(TAG_FLAG_VA) ? 1 : 0;
    s_frame = 0;
    s_rec_n = 0;
    s_menu_frames = 0;
    xbox_det_begin(&s_params.bases);
    s_state = NP_STATE_ACTIVE;
}

/* ── per-frame ─────────────────────────────────────────────────────── */

static void record_frame(const np_pad in[2])
{
    np_frame_rec r;
    uint32_t k, sva = rng_seed_va();
    uint64_t total = 1469598103934665603ull;
    r.frame = s_frame;
    for (k = 0; k < N_SYNC; k++) r.region[k] = fnv32(2166136261u, k_sync[k].va, k_sync[k].len);
    for (k = 0; k < N_EXTRA; k++) r.region[N_SYNC + k] = fnv32(2166136261u, k_extra[k].va, k_extra[k].len);
    {
        uint32_t h = 2166136261u, v[3];
        v[0] = sva ? MEM32(sva) : 0;
        v[1] = xbox_det_tick_ms();
        v[2] = (uint32_t)xbox_det_filetime();
        for (k = 0; k < sizeof(v); k++) { h ^= ((uint8_t *)v)[k]; h *= 16777619u; }
        r.region[N_REGION_HASH - 1] = h;
    }
    for (k = 0; k < N_REGION_HASH; k++) { total ^= r.region[k]; total *= 1099511628211ull; }
    r.digest = total;
    r.in[0] = in[0];
    r.in[1] = in[1];
    r.mode = MEM8(MODE_VA);
    r.scrst = MEM8(SCRST_VA);
    r.gmode = MEM8(GAME_MODE_VA);
    r.pad_ = 0;
    s_digest = total;
    if (s_rec_n == s_rec_cap && s_rec_cap < REC_MAX) {
        uint32_t cap = s_rec_cap ? s_rec_cap * 2 : 4096;
        np_frame_rec *n = (np_frame_rec *)realloc(s_rec, cap * sizeof(*n));
        if (n) { s_rec = n; s_rec_cap = cap; }
    }
    if (s_rec_n < s_rec_cap) s_rec[s_rec_n++] = r;
}

void netplay_input_tick(void)
{
    np_pad in[2];
    uint32_t p;

    if (s_state == NP_STATE_OFF) {
        if (!s_armed) return;
        s_state = NP_STATE_ARMED;
    }
    if (s_state == NP_STATE_ARMED) {
        if (!s_armed) { s_state = NP_STATE_OFF; return; }
        if (!at_barrier()) return;
        begin_session();
    } else if (!s_armed || left_versus()) {
        static const np_pad none[2];
        record_frame(none);         /* the state that ended the session */
        end_session();
        return;
    }

    /* Local input exchange: both players on this machine (Phase 2). The
     * raw slots still hold what XInputGetState read from the host pads. */
    pad_from_guest(0, &in[0]);
    if (s_mirror) in[1] = in[0];
    else pad_from_guest(1, &in[1]);

    record_frame(in);               /* state entering this frame */
    for (p = 0; p < 2; p++) pad_to_guest(p, &in[p], 0x1000u + s_frame);
    for (p = 2; p < 4; p++) {
        static const np_pad zero;
        pad_to_guest(p, &zero, 0);
    }
    xbox_det_frame();
    s_frame++;
}

uint32_t netplay_filter_pad_mask(uint32_t host_mask)
{
    return s_state == NP_STATE_ACTIVE ? SESSION_PADS : host_mask;
}

/* ── UI ────────────────────────────────────────────────────────────── */

void netplay_set_armed(int armed, int role)
{
    if (s_state != NP_STATE_ACTIVE) s_role = role;
    s_armed = armed ? 1 : 0;
}

int netplay_armed(void)  { return s_armed; }
void netplay_set_mirror(int on) { s_mirror = on ? 1 : 0; }
int netplay_mirror(void) { return s_mirror; }
int netplay_active(void) { return s_state == NP_STATE_ACTIVE; }

void netplay_get_status(np_status *out)
{
    if (!out) return;
    out->state = s_state;
    out->role = s_state == NP_STATE_ACTIVE ? s_active_role : s_role;
    out->tag = s_tag;
    out->frame = s_frame;
    out->digest = s_digest;
    out->sessions = s_sessions;
    memcpy(out->last_log, s_last_log, sizeof(out->last_log));
}
