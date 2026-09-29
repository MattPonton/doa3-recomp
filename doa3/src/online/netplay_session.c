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
void mcpx_apu_det_begin(uint64_t xgscnt_base); /* apu_core.c: chip stepped per frame */
uint64_t mcpx_apu_xgscnt_now(void);
void mcpx_apu_det_end(void);
void mcpx_apu_det_frame(void);

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
/* The camera setup (sub_00025690) copies the active camera into 0x4842A4,
 * 0x47E274 and 0x47E680 every frame; the camera objects animate on their own
 * clock in the menus, so those three floats are presentation and are left
 * out of both the sync and the digest. */
static const np_region k_sync[] = {
    { 0x00484C48u, 0x0D0u,        "side" },      /* 2 x 0x68 per-side state */
    { 0x004785D0u, 0x5C0u * 4u,   "fighter" },   /* 4 x 0x5C0 fighter records */
    { 0x004815A0u, 0x2D04u,       "b4815A0" },   /* ends before camera 0x4842A4 */
    { 0x0047E278u, 0x0408u,       "b47E278" },   /* after camera 0x47E274, to 0x47E680 */
    { 0x0047E684u, 0x0057u,       "b47E684" },
    { 0x00488AA8u, 0x00F7u,       "b488AA8" },
    /* The five camera objects and the active-camera index. A camera
     * script's remaining length is reloaded into 0x48E628 and counted down
     * every frame, and when it runs out a new camera is picked with rand();
     * the objects animate on the menus' own clock before the session, so
     * without this the pick landed a frame apart on two machines. Their
     * pointers are all into the game image (identical everywhere). */
    { 0x003C29C0u, 0x0CC0u * 5u,  "cameras" },
    { 0x003C7EE8u, 0x0008u,       "camidx" },
    { 0x0048E600u, 0x0080u,       "match" },     /* match config + camera countdown */
    /* Counters that run from boot or from screen entry, found by diffing two
     * replays' whole data/BSS mid-fight (same step, different start). The
     * fighter update reads 0x47AD30 and the timer block at 0x85B9D8 when it
     * decides on idle/blink motions, which call rand(); 0x4A0D70 is the
     * fight's frame counter (64 readers). 0x46C758.. are the counter fields
     * of four records whose other fields hold heap pointers, so only those
     * dwords are carried. */
    /* Five 0xB8-byte per-fighter records at 0x46C6A0 (the idle loop reads
     * their motion state for every player, tagged-out partners included).
     * +0x14, +0x98, +0x9C and +0xAC are heap pointers and stay local. */
    { 0x0046C6A0u, 0x0014u,       "rec0_00" },
    { 0x0046C6B8u, 0x0080u,       "rec0_18" },
    { 0x0046C740u, 0x000Cu,       "rec0_A0" },
    { 0x0046C750u, 0x0008u,       "rec0_B0" },
    { 0x0046C758u, 0x0014u,       "rec1_00" },
    { 0x0046C770u, 0x0080u,       "rec1_18" },
    { 0x0046C7F8u, 0x000Cu,       "rec1_A0" },
    { 0x0046C808u, 0x0008u,       "rec1_B0" },
    { 0x0046C810u, 0x0014u,       "rec2_00" },
    { 0x0046C828u, 0x0080u,       "rec2_18" },
    { 0x0046C8B0u, 0x000Cu,       "rec2_A0" },
    { 0x0046C8C0u, 0x0008u,       "rec2_B0" },
    { 0x0046C8C8u, 0x0014u,       "rec3_00" },
    { 0x0046C8E0u, 0x0080u,       "rec3_18" },
    { 0x0046C968u, 0x000Cu,       "rec3_A0" },
    { 0x0046C978u, 0x0008u,       "rec3_B0" },
    { 0x0046C980u, 0x0014u,       "rec4_00" },
    { 0x0046C998u, 0x0080u,       "rec4_18" },
    { 0x0046CA20u, 0x000Cu,       "rec4_A0" },
    { 0x0046CA30u, 0x0008u,       "rec4_B0" },
    { 0x0047AD20u, 0x0040u,       "b47AD20" },
    /* 0x85B9D8 and 0x868930 are left out: they hold the D3D fence id
     * (InsertFence, device+0x1C), a count of fences since boot. The fence
     * counter itself cannot be rewritten without breaking the GPU emulation,
     * and in this port every fence is complete as soon as it is inserted,
     * so the game's fence checks cannot take different paths. */
    { 0x0085B9C0u, 0x0018u,       "timer85B9C0" },
    { 0x0085B9DCu, 0x000Cu,       "timer85B9DC" },
    { 0x008688C0u, 0x0070u,       "b8688C0" },
    { 0x00868934u, 0x000Cu,       "b868934" },
    { 0x004914A0u, 0x0020u,       "b4914A0" },
    { 0x00491B70u, 0x0010u,       "b491B70" },
    { 0x004A0D60u, 0x0030u,       "b4A0D60" },
    { 0x004B8440u, 0x0008u,       "b4B8440" },
    { 0x003C23E0u, 0x0010u,       "b3C23E0" },
    /* Per-player idle/blink state of the fighter update (sub_0004C59C):
     * a countdown per player at 0x46C110 that re-rolls with rand() when it
     * runs out, and the records it fills at 0x46BA88 (stride 0x10). The
     * countdowns run from before the session, so they expired on different
     * frames on two machines. */
    { 0x0046BA80u, 0x0050u,       "idlerec" },
    { 0x0046C110u, 0x0010u,       "idletimer" },
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

static const char *region_name(int k)
{
    if (k < 0) return "?";
    if (k < (int)N_SYNC) return k_sync[k].name;
    if (k < (int)(N_SYNC + N_EXTRA)) return k_extra[k - N_SYNC].name;
    return "seedclock";
}

/* Session start parameters. The host chooses them; the joiner receives
 * them. A local session builds them from this machine. */
typedef struct {
    uint32_t       seed;
    xbox_det_bases bases;
    uint8_t        save[SAVE_LEN];
    uint8_t        joiner_layout[10];
    uint8_t        joiner_laysel;
    uint64_t       xgscnt;        /* APU sample counter (100 ns units) at frame 0 */
    uint8_t       *sync;          /* concatenated k_sync bytes */
    uint32_t       sync_len;
} np_params;

typedef struct {
    uint32_t frame;
    uint64_t digest;
    uint32_t region[N_REGION_HASH];
    np_pad   in[2];
    uint8_t  mode, scrst, gmode, pad_;
    uint32_t kcalls, boost;   /* diagnostics: see xbox_det_kcalls */
    uint16_t rand_game, rand_workers;   /* rand() calls this frame by fiber kind */
    uint8_t  active[2];                 /* each side's active character id (tag-ins) */
} np_frame_rec;

/* Replay files (netplay_replay_*.dnr): the start parameters, then per frame
 * both pads and the digest the recording saw, so a replay can say exactly
 * where it stopped matching. */
#define RP_MAGIC   "DOA3REP1"
#define RP_VERSION 2u
typedef struct {
    np_pad   in[2];
    uint64_t digest;
    uint32_t region[N_REGION_HASH];
} np_rp_frame;

typedef struct {
    int          loaded;
    char         name[260];
    uint32_t     build_stamp;
    int          role, tag;
    np_params    params;
    uint32_t     frames;
    np_rp_frame *f;
} np_replay;

/* Raw bytes of every digested region for the first STATE_FRAMES frames of a
 * recorded or replayed session (netplay_state_*.bin), so a divergence can
 * be traced to the bytes that differ. */
#define STATE_FRAMES 128u
static uint8_t  *s_state_buf;
static uint32_t  s_state_frames, s_state_stride;
static volatile uint32_t s_state_from;        /* first frame captured (harness) */

static volatile int s_record;                 /* write a replay for each session */
static np_replay s_rp;
static int       s_replaying;                 /* the active session is a replay */
static volatile LONG s_req_replay;            /* load s_req_path at the next frame */
static char      s_req_path[260];
static uint32_t  s_div_frame = 0xFFFFFFFFu;   /* first frame whose digest differed */
static int       s_div_region = -1;
static uint32_t  s_div_rec, s_div_now;
static char      s_msg[200];                  /* last replay / record message */
static char      s_last_replay[260];

static volatile int s_armed;
static volatile int s_mirror;
static uint32_t s_rand_game, s_rand_workers;  /* rand() calls since the last frame record */
static uint32_t s_rand_by_fiber[64];          /* rand() calls this session, per fiber */
/* Diagnostics: native call stacks of the first rand() calls in a session. */
#define RAND_BT_MAX   256
#define RAND_BT_DEPTH 10
static void    *s_rand_bt[RAND_BT_MAX][RAND_BT_DEPTH];
static uint32_t s_rand_bt_frame[RAND_BT_MAX];
static uint32_t s_rand_bt_n;
static uint8_t  s_rand_bt_fiber[RAND_BT_MAX];
static volatile uint32_t s_rand_bt_lo = 0, s_rand_bt_hi = 0;     /* frame window, off unless a harness sets it */
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

static int at_charsel(void)
{
    return MEM8(GAME_MODE_VA) == GAME_MODE_VS && MEM8(MODE_VA) == 0 &&
           MEM8(SCRST_VA) == SCRST_CHARSEL;
}

/* The session may only start once character select has settled. Opening
 * the screen starts loading both cursor characters' preview models, and
 * that load runs on wall-clock worker time until the session begins, so a
 * barrier on the first character-select frame catches it at a different
 * point on every machine (measured: previews completing 0-5 frames apart,
 * with the RNG and fight state diverging from there). Settled = both
 * previews posed (a preview fighter record holds an identity matrix until
 * its model is in) and no guest file open/read for BARRIER_QUIET_FRAMES. */
#define BARRIER_QUIET_FRAMES 10
#define FIGHTER_VA(k)        (0x004785D0u + 0x5C0u * (k))
static uint32_t s_quiet_frames, s_quiet_io;

static int at_barrier(void)
{
    uint32_t io = xbox_det_io_count();
    int posed = MEM32(FIGHTER_VA(0)) != 0x3F800000u && MEM32(FIGHTER_VA(1)) != 0x3F800000u;
    if (!at_charsel() || !posed || io != s_quiet_io) {
        s_quiet_io = io;
        s_quiet_frames = 0;
        return 0;
    }
    return ++s_quiet_frames >= BARRIER_QUIET_FRAMES;
}

/* The players left Versus. Going back to character select after a fight
 * passes through screen state 0 (the mode-select screen re-initialises)
 * before reaching 3, so state 0 alone is not an exit; backing out does
 * pass through the Single/Tag submenu (2), and QUIT goes through the title
 * (mode 2). The frame limit covers any other way out of the menus. */
#define SCRST_VS_SUBMENU    2
#define MENU_EXIT_FRAMES    600
static uint32_t s_menu_frames;
static uint8_t  s_last_mode, s_last_scrst;    /* reseed_on_transition */
static uint32_t s_transitions;

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
    p->xgscnt = mcpx_apu_xgscnt_now();
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
    if (s_replaying) {
        fprintf(f, "# replay of %s\n", s_rp.name);
        if (s_div_frame == 0xFFFFFFFFu)
            fprintf(f, "# replay matched the recording for all %u frames\n", s_rec_n);
        else
            fprintf(f, "# replay DIVERGED at frame %u in %s (recorded %08X, now %08X)\n",
                    s_div_frame, region_name(s_div_region), s_div_rec, s_div_now);
    }
    {   /* guest kernel calls by ordinal over the first frames (diagnostics) */
        const uint32_t *h = xbox_det_kcall_histogram();
        fprintf(f, "# kcalls-by-ordinal (first %u frames):", XBOX_DET_HIST_FRAMES);
        for (k = 0; k < XBOX_DET_HIST_SIZE; k++) if (h[k]) fprintf(f, " %u:%u", k, h[k]);
        fprintf(f, "\n");
    }
    fprintf(f, "# rand-by-fiber:");
    for (k = 0; k < 64; k++) if (s_rand_by_fiber[k]) fprintf(f, " %u:%u", k, s_rand_by_fiber[k]);
    fprintf(f, "\n");
    memset(s_rand_by_fiber, 0, sizeof(s_rand_by_fiber));
    for (i = 0; i < s_rand_bt_n; i++) {
        fprintf(f, "# rand-bt frame %u fiber %u:", s_rand_bt_frame[i], s_rand_bt_fiber[i]);
        for (k = 0; k < RAND_BT_DEPTH; k++) fprintf(f, " %p", s_rand_bt[i][k]);
        fprintf(f, "\n");
    }
    s_rand_bt_n = 0;
    fprintf(f, "# frame digest");
    for (k = 0; k < N_SYNC; k++) fprintf(f, " %s", k_sync[k].name);
    for (k = 0; k < N_EXTRA; k++) fprintf(f, " %s", k_extra[k].name);
    fprintf(f, " seedclock p1 p2 mode/scrst/gmode kcalls boost rand_game rand_workers active\n");
    for (i = 0; i < s_rec_n; i++) {
        const np_frame_rec *r = &s_rec[i];
        fprintf(f, "%u %016llX", r->frame, (unsigned long long)r->digest);
        for (k = 0; k < N_REGION_HASH; k++) fprintf(f, " %08X", r->region[k]);
        fprintf(f, " %04X %04X %u/%u/%u %u %u %u %u %u/%u\n", r->in[0].buttons, r->in[1].buttons,
                r->mode, r->scrst, r->gmode, r->kcalls, r->boost, r->rand_game, r->rand_workers,
                r->active[0], r->active[1]);
    }
    fclose(f);
}

/* The executable's link timestamp: replays are only comparable on the build
 * that recorded them. */
static uint32_t build_stamp(void)
{
    const uint8_t *base = (const uint8_t *)GetModuleHandleW(NULL);
    const IMAGE_DOS_HEADER *dos = (const IMAGE_DOS_HEADER *)base;
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
    return nt->FileHeader.TimeDateStamp;
}

static void write_replay(void)
{
    SYSTEMTIME t;
    FILE *f;
    uint32_t i, v;
    GetLocalTime(&t);
    _snprintf(s_last_replay, sizeof(s_last_replay) - 1,
              "netplay_replay_%04u%02u%02u_%02u%02u%02u_%s.dnr",
              t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond,
              s_tag ? "tag" : "single");
    f = fopen(s_last_replay, "wb");
    if (!f) { s_last_replay[0] = 0; return; }
    fwrite(RP_MAGIC, 1, 8, f);
    v = RP_VERSION;            fwrite(&v, 4, 1, f);
    v = build_stamp();         fwrite(&v, 4, 1, f);
    v = (uint32_t)s_active_role; fwrite(&v, 4, 1, f);
    v = (uint32_t)s_tag;       fwrite(&v, 4, 1, f);
    fwrite(&s_params.seed, 4, 1, f);
    fwrite(&s_params.bases.tick_ms, 4, 1, f);
    fwrite(&s_params.bases.qpc, 8, 1, f);
    fwrite(&s_params.bases.filetime, 8, 1, f);
    fwrite(&s_params.xgscnt, 8, 1, f);
    fwrite(s_params.save, 1, SAVE_LEN, f);
    fwrite(s_params.joiner_layout, 1, 10, f);
    fwrite(&s_params.joiner_laysel, 1, 1, f);
    fwrite(&s_params.sync_len, 4, 1, f);
    fwrite(s_params.sync, 1, s_params.sync_len, f);
    fwrite(&s_rec_n, 4, 1, f);
    for (i = 0; i < s_rec_n; i++) {
        np_rp_frame fr;
        fr.in[0] = s_rec[i].in[0];
        fr.in[1] = s_rec[i].in[1];
        fr.digest = s_rec[i].digest;
        memcpy(fr.region, s_rec[i].region, sizeof(fr.region));
        fwrite(&fr, sizeof(fr), 1, f);
    }
    fclose(f);
}

static void free_replay(np_replay *r)
{
    free(r->params.sync);
    free(r->f);
    memset(r, 0, sizeof(*r));
}

/* Returns an error message, or NULL when the replay loaded. */
static const char *load_replay(const char *path, np_replay *r)
{
    FILE *f = fopen(path, "rb");
    char magic[8];
    uint32_t v, ver, sync_expect = 0, i;
    const char *err = NULL;
    free_replay(r);
    if (!f) return "cannot open the replay file";
    for (i = 0; i < N_SYNC; i++) sync_expect += k_sync[i].len;
#define RD(p, n) do { if (fread((p), 1, (n), f) != (size_t)(n)) { err = "replay file is truncated"; goto out; } } while (0)
    RD(magic, 8);
    if (memcmp(magic, RP_MAGIC, 8)) { err = "not a DOA3 replay file"; goto out; }
    RD(&ver, 4);
    if (ver != RP_VERSION) { err = "unsupported replay version"; goto out; }
    RD(&r->build_stamp, 4);
    RD(&v, 4); r->role = (int)v;
    RD(&v, 4); r->tag = (int)v;
    RD(&r->params.seed, 4);
    RD(&r->params.bases.tick_ms, 4);
    RD(&r->params.bases.qpc, 8);
    RD(&r->params.bases.filetime, 8);
    RD(&r->params.xgscnt, 8);
    RD(r->params.save, SAVE_LEN);
    RD(r->params.joiner_layout, 10);
    RD(&r->params.joiner_laysel, 1);
    RD(&r->params.sync_len, 4);
    if (r->params.sync_len != sync_expect) { err = "replay was recorded with different state regions"; goto out; }
    r->params.sync = (uint8_t *)malloc(r->params.sync_len);
    if (!r->params.sync) { err = "out of memory"; goto out; }
    RD(r->params.sync, r->params.sync_len);
    RD(&r->frames, 4);
    if (r->frames == 0 || r->frames > REC_MAX) { err = "replay has no frames"; goto out; }
    r->f = (np_rp_frame *)malloc((size_t)r->frames * sizeof(np_rp_frame));
    if (!r->f) { err = "out of memory"; goto out; }
    RD(r->f, (size_t)r->frames * sizeof(np_rp_frame));
#undef RD
    strncpy(r->name, path, sizeof(r->name) - 1);
    r->loaded = 1;
out:
    fclose(f);
    if (err) free_replay(r);
    return err;
}

static void write_state(void)
{
    SYSTEMTIME t;
    char name[260];
    FILE *f;
    uint32_t k, n = (uint32_t)(N_SYNC + N_EXTRA);
    if (!s_state_buf || !s_state_frames) return;
    GetLocalTime(&t);
    _snprintf(name, sizeof(name) - 1, "netplay_state_%04u%02u%02u_%02u%02u%02u.bin",
              t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    name[sizeof(name) - 1] = 0;
    f = fopen(name, "wb");
    if (!f) return;
    fwrite("DOA3STA1", 1, 8, f);
    fwrite(&n, 4, 1, f);
    for (k = 0; k < n; k++) {
        const np_region *r = k < N_SYNC ? &k_sync[k] : &k_extra[k - N_SYNC];
        char nm[16] = {0};
        strncpy(nm, r->name, 15);
        fwrite(&r->va, 4, 1, f);
        fwrite(&r->len, 4, 1, f);
        fwrite(nm, 1, 16, f);
    }
    fwrite(&s_state_frames, 4, 1, f);
    fwrite(s_state_buf, s_state_stride, s_state_frames, f);
    fclose(f);
}

/* Whole game data/BSS (image data through the end of BSS) at a few early
 * frames of a recorded or replayed session: netplay_full_<frame>_<n>.bin.
 * Diffing two runs shows every byte the session does not yet sync. */
static volatile int s_full_dumps;   /* off unless a test harness sets it */
static volatile uint32_t s_full_f0 = 0, s_full_f1 = 7;   /* frames to dump */
#define FULL_LO 0x00219000u
#define FULL_HI 0x00C40000u
static void dump_full(uint32_t frame)
{
    static uint32_t s_run_id;
    char name[128];
    FILE *f;
    uint8_t *buf;
    if (!s_run_id || frame == s_full_f0) s_run_id = GetTickCount();
    buf = (uint8_t *)malloc(FULL_HI - FULL_LO);
    if (!buf) return;
    guest_read(FULL_LO, buf, FULL_HI - FULL_LO);
    _snprintf(name, sizeof(name) - 1, "netplay_full_f%u_%u.bin", frame, s_run_id);
    name[sizeof(name) - 1] = 0;
    f = fopen(name, "wb");
    if (f) { fwrite(buf, 1, FULL_HI - FULL_LO, f); fclose(f); }
    free(buf);
}

static void capture_state(void)
{
    uint32_t k, off = 0, sva;
    if (!(s_record || s_replaying)) return;
    if (s_full_dumps && (s_frame == s_full_f0 || s_frame == s_full_f1)) dump_full(s_frame);
    if (s_frame < s_state_from || s_state_frames >= STATE_FRAMES) return;
    if (!s_state_buf) {
        s_state_stride = 4;
        for (k = 0; k < N_SYNC; k++) s_state_stride += k_sync[k].len;
        for (k = 0; k < N_EXTRA; k++) s_state_stride += k_extra[k].len;
        s_state_buf = (uint8_t *)malloc((size_t)s_state_stride * STATE_FRAMES);
        if (!s_state_buf) return;
    }
    {
        uint8_t *d = s_state_buf + (size_t)s_state_frames * s_state_stride;
        for (k = 0; k < N_SYNC; k++) { guest_read(k_sync[k].va, d + off, k_sync[k].len); off += k_sync[k].len; }
        for (k = 0; k < N_EXTRA; k++) { guest_read(k_extra[k].va, d + off, k_extra[k].len); off += k_extra[k].len; }
        sva = rng_seed_va();
        k = sva ? MEM32(sva) : 0;
        memcpy(d + off, &k, 4);
    }
    s_state_frames++;
}

static void end_session(void)
{
    mcpx_apu_det_end();
    write_state();
    s_state_frames = 0;
    xbox_det_end();
    if (s_active_role == NP_ROLE_JOINER) {
        guest_write(SAVE_VA, s_save_backup, SAVE_LEN);
        rebuild_button_maps();
        xbox_path_set_title_redirect(0);
    }
    write_log();
    if (s_replaying) {
        if (s_div_frame == 0xFFFFFFFFu)
            _snprintf(s_msg, sizeof(s_msg) - 1, "Replay matched the recording for all %u frames",
                      s_rec_n);
        else
            _snprintf(s_msg, sizeof(s_msg) - 1,
                      "Replay DIVERGED at frame %u (%s: recorded %08X, now %08X)",
                      s_div_frame, region_name(s_div_region), s_div_rec, s_div_now);
        s_replaying = 0;
        s_armed = 0;                /* a replay runs once */
        free_replay(&s_rp);
    } else if (s_record) {
        write_replay();
        _snprintf(s_msg, sizeof(s_msg) - 1, "Recorded %s", s_last_replay[0] ? s_last_replay : "(write failed)");
    }
    s_sessions++;
    s_rec_n = 0;
    s_state = s_armed ? NP_STATE_ARMED : NP_STATE_OFF;
}

static void copy_params(np_params *dst, const np_params *src)
{
    uint8_t *keep = dst->sync;
    *dst = *src;
    dst->sync = (uint8_t *)realloc(keep, src->sync_len);
    if (dst->sync) memcpy(dst->sync, src->sync, src->sync_len);
}

static void begin_session(void)
{
    s_div_frame = 0xFFFFFFFFu;
    s_div_region = -1;
    if (s_rp.loaded) {
        s_replaying = 1;
        s_active_role = s_rp.role;
        copy_params(&s_params, &s_rp.params);
        _snprintf(s_msg, sizeof(s_msg) - 1, "Replaying %s", s_rp.name);
    } else {
        s_replaying = 0;
        s_active_role = s_role;
        make_local_params(&s_params);
    }
    apply_params(&s_params, s_active_role);
    s_tag = MEM8(TAG_FLAG_VA) ? 1 : 0;
    s_frame = 0;
    s_rec_n = 0;
    s_menu_frames = 0;
    s_last_mode = MEM8(MODE_VA);
    s_last_scrst = MEM8(SCRST_VA);
    s_transitions = 0;
    xbox_det_begin(&s_params.bases);
    mcpx_apu_det_begin(s_params.xgscnt);
    s_state = NP_STATE_ACTIVE;
}

/* ── per-frame ─────────────────────────────────────────────────────── */

/* Menu presentation (camera cuts, background effects) calls rand() on
 * timers that started before the session, so by the time a fight loads the
 * two machines' seeds would differ. Every screen change inside the session
 * happens on the same lockstep frame on both, so reset the seed there from
 * the session seed and the number of changes so far. */
static void reseed_on_transition(void)
{
    uint8_t mode = MEM8(MODE_VA), scrst = MEM8(SCRST_VA);
    uint32_t sva;
    if (mode == s_last_mode && scrst == s_last_scrst) return;
    s_last_mode = mode;
    s_last_scrst = scrst;
    s_transitions++;
    sva = rng_seed_va();
    if (sva) MEM32(sva) = s_params.seed ^ (s_transitions * 0x9E3779B1u);
}

static void record_frame(const np_pad in[2])
{
    np_frame_rec r;
    uint32_t k, sva = rng_seed_va();
    uint64_t total = 1469598103934665603ull;
    int in_fight = MEM8(MODE_VA) == 1;
    r.frame = s_frame;
    capture_state();
    /* In a fight everything in the sync set is gameplay state. In the menus
     * the fighter records and the other blocks also hold the character
     * select's animated previews and background, which run on their own
     * clocks from before the session, so there only each side's selection
     * (active character, costume, side: +1..+3) is digested; the seed is
     * reset at every screen change (reseed_on_transition). */
    for (k = 0; k < N_SYNC; k++) {
        if (in_fight)
            r.region[k] = fnv32(2166136261u, k_sync[k].va, k_sync[k].len);
        else if (k == 0)
            r.region[k] = fnv32(fnv32(2166136261u, 0x00484C49u, 3), 0x00484CB1u, 3);
        else
            r.region[k] = 0;
    }
    for (k = 0; k < N_EXTRA; k++) r.region[N_SYNC + k] = fnv32(2166136261u, k_extra[k].va, k_extra[k].len);
    {
        uint32_t h = 2166136261u, v[3];
        v[0] = (sva && in_fight) ? MEM32(sva) : 0;
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
    r.kcalls = xbox_det_kcalls();
    r.active[0] = MEM8(0x00484C49u);
    r.active[1] = MEM8(0x00484CB1u);
    r.rand_game = (uint16_t)s_rand_game;
    r.rand_workers = (uint16_t)s_rand_workers;
    s_rand_game = s_rand_workers = 0;
    r.boost = (uint32_t)(xbox_det_boost() / 733333u);
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

    if (s_req_replay && s_state != NP_STATE_ACTIVE) {
        /* Loaded here, on the game thread, whoever asked for it. */
        const char *err = load_replay(s_req_path, &s_rp);
        InterlockedExchange(&s_req_replay, 0);
        if (err) {
            _snprintf(s_msg, sizeof(s_msg) - 1, "Replay not loaded: %s", err);
        } else {
            _snprintf(s_msg, sizeof(s_msg) - 1,
                      "Replay ready (%s Battle, %u frames%s) - go to that VS character select",
                      s_rp.tag ? "Tag" : "Single", s_rp.frames,
                      s_rp.build_stamp == build_stamp() ? "" : ", recorded on a DIFFERENT build");
            s_armed = 1;
        }
    }

    if (s_state == NP_STATE_OFF) {
        if (!s_armed) return;
        s_state = NP_STATE_ARMED;
    }
    if (s_state == NP_STATE_ARMED) {
        if (!s_armed) { s_state = NP_STATE_OFF; free_replay(&s_rp); return; }
        if (!at_barrier()) return;
        s_quiet_frames = 0;
        if (s_rp.loaded && (MEM8(TAG_FLAG_VA) ? 1 : 0) != s_rp.tag) {
            _snprintf(s_msg, sizeof(s_msg) - 1, "This replay is a %s Battle recording - pick %s Battle",
                      s_rp.tag ? "Tag" : "Single", s_rp.tag ? "Tag" : "Single");
            return;
        }
        begin_session();
    } else if (!s_armed || left_versus() || (s_replaying && s_frame >= s_rp.frames)) {
        static const np_pad none[2];
        record_frame(none);         /* the state that ended the session */
        end_session();
        return;
    }

    reseed_on_transition();

    if (s_replaying) {
        in[0] = s_rp.f[s_frame].in[0];
        in[1] = s_rp.f[s_frame].in[1];
    } else {
        /* Local input exchange: both players on this machine. The raw
         * slots still hold what XInputGetState read from the host pads. */
        pad_from_guest(0, &in[0]);
        if (s_mirror) in[1] = in[0];
        else pad_from_guest(1, &in[1]);
    }

    record_frame(in);               /* state entering this frame */
    if (s_replaying && s_div_frame == 0xFFFFFFFFu && s_rec_n &&
        s_rec[s_rec_n - 1].digest != s_rp.f[s_frame].digest) {
        uint32_t k;
        s_div_frame = s_frame;
        for (k = 0; k < N_REGION_HASH; k++)
            if (s_rec[s_rec_n - 1].region[k] != s_rp.f[s_frame].region[k]) {
                s_div_region = (int)k;
                s_div_rec = s_rp.f[s_frame].region[k];
                s_div_now = s_rec[s_rec_n - 1].region[k];
                break;
            }
    }
    for (p = 0; p < 2; p++) pad_to_guest(p, &in[p], 0x1000u + s_frame);
    for (p = 2; p < 4; p++) {
        static const np_pad zero;
        pad_to_guest(p, &zero, 0);
    }
    xbox_det_frame();
    mcpx_apu_det_frame();
    s_frame++;
}

void netplay_note_rand(void)
{
    extern int xbox_fiber_current(void);
    if (s_state != NP_STATE_ACTIVE) return;
    {
        int f = xbox_fiber_current();
        s_rand_by_fiber[(f >= 0 && f < 64) ? f : 63]++;
        if (s_rand_bt_n < RAND_BT_MAX && s_frame >= s_rand_bt_lo && s_frame < s_rand_bt_hi) {
            CaptureStackBackTrace(1, RAND_BT_DEPTH, s_rand_bt[s_rand_bt_n], NULL);
            s_rand_bt_fiber[s_rand_bt_n] = (uint8_t)f;
            s_rand_bt_frame[s_rand_bt_n++] = s_frame;
        }
        if (f == 0) s_rand_game++;
        else s_rand_workers++;
    }
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
void netplay_set_record(int on) { s_record = on ? 1 : 0; }
int netplay_record(void) { return s_record; }

void netplay_request_replay(const char *path)
{
    if (!path || s_state == NP_STATE_ACTIVE) return;
    strncpy(s_req_path, path, sizeof(s_req_path) - 1);
    s_req_path[sizeof(s_req_path) - 1] = 0;
    InterlockedExchange(&s_req_replay, 1);
}

int netplay_list_replays(char (*names)[128], int max)
{
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA("netplay_replay_*.dnr", &fd);
    int n = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (n < max) {
            strncpy(names[n], fd.cFileName, 127);
            names[n][127] = 0;
            n++;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return n;
}

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
    out->replaying = s_replaying || s_rp.loaded;
    out->replay_frames = s_rp.loaded ? s_rp.frames : 0;
    out->diverged_at = s_div_frame;
    memcpy(out->message, s_msg, sizeof(out->message));
}
