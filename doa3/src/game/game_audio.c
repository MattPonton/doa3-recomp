#include "game_audio.h"
#include "audio_settings.h"
#include "recomp/recomp_types.h"
#include "audio/dsound_hle.h"
#include "recomp/gen/xbe_layout.h"   /* DOA3_XBE_ID_JAPAN */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

/* DOA3 3.1 (NTSC-J) audio state.
 *
 *   0x59D094/95/96  Music / Sound effects / Voice percent (u8, 0-100,
 *                   default 80), inside the save block at 0x59D088 that
 *                   SaveGame (0x20C80) writes and the boot loader restores.
 *   0x2D3E9C        live music level the BGM code ramps and applies.
 *   0x5CF418        five BGM channels, 0x45A00 bytes apart: +0 ADXT handle,
 *                   +8 channel kind (3 = music).
 *   ADXT handle +1  ADXT status: 1 decinfo, 2 prep, 3 playing, 4 decode end
 *                   (still sounding), 0 stop, 5 play end.
 *
 * Every song starts through ADXT_StartAfs(adxt, 2, fid), partition 2 being
 * bgm.afs. The game's song list at 0x29CE48 (the Options sound test, test
 * number = entry + 1) pairs "nine lives" with 0x18, "amazing" with 0x16 and
 * "home tonight" with 0x17 in the low word; [BGM] log lines confirm the file
 * numbers at runtime. The credits play "Home Tonight" from the movie
 * mv_st.sfd instead, handled through the movie-start hook. */

#if defined(DOA3_XBE_ID_JAPAN)

#define GA_MUSIC_BYTE  0x59D094u
#define GA_SFX_BYTE    0x59D095u
#define GA_VOICE_BYTE  0x59D096u
#define GA_BGM_SLOTS   0x5CF418u
#define GA_BGM_STRIDE  0x45A00u
#define GA_BGM_COUNT   5u
#define GA_BGM_PART    2u
#define GA_MOVIE_HANDLE 0x73C308u   /* Sofdec player of the running movie; 0xA4C10 clears it */
#define GA_MOVIE_CREDITS 17u        /* d:\mv_st.sfd in the movie table at 0x2D4DD8 */

/* Aerosmith's songs: "nine lives" (sound test 01), "amazing" (23) and
 * "home tonight" (24). */
static int ga_licensed(uint32_t fid) { return fid == 0x18u || fid == 0x16u || fid == 0x17u; }

/* Which bgm.afs file each ADXT handle was last started with. */
static struct { uint32_t adxt, fid; } s_map[16];
static unsigned s_map_next;

static volatile LONG s_pending[3] = { -1, -1, -1 };
static volatile LONG s_muting;          /* read by the UI */
static int s_muted, s_saved_music;      /* game thread */
/* The credits movie: 0 none, 1 starting (player not created yet), 2 playing. */
static int s_movie_state;
static unsigned s_movie_wait;

int game_audio_available(void)
{
    extern unsigned g_doa3_frames_presented;
    return g_doa3_frames_presented > 0;
}

static uint32_t ga_byte(int channel)
{
    return channel == GAME_AUDIO_MUSIC ? GA_MUSIC_BYTE
         : channel == GAME_AUDIO_SFX ? GA_SFX_BYTE : GA_VOICE_BYTE;
}

int game_audio_get(int channel)
{
    int v;
    if (channel < 0 || channel > 2 || !game_audio_available()) return 80;
    if (channel == GAME_AUDIO_MUSIC && InterlockedCompareExchange(&s_muting, 0, 0))
        return s_saved_music;
    v = (int)(int8_t)MEM8(ga_byte(channel));
    return v < 0 ? 0 : v > 100 ? 100 : v;
}

void game_audio_request(int channel, int value)
{
    if (channel < 0 || channel > 2) return;
    value = (value + 2) / 5 * 5;                 /* the game's menu steps by 5 */
    if (value < 0) value = 0;
    if (value > 100) value = 100;
    InterlockedExchange(&s_pending[channel], value);
}

int game_audio_streamer_muting(void)
{
    return (int)InterlockedCompareExchange(&s_muting, 0, 0);
}

/* doa3.1-rollback diagnostic: each music channel's player handle, channel
 * state (+8; 3 = playing) and the ADXT state byte (+1: 0 stop, 1 decode
 * info, 2 prepare, 3 playing, 4 decode end, 5 play end, 6 error), logged on
 * every change with whether the rollback clock runs. */
static uint32_t s_bgm_adxt;
/* doa3.1-rollback.7 diagnostic (DOA3_BGM_WATCH=1): while channel 0 has a
 * player, every recompiled-function wrapper (gen/recomp_probes.c) checks the
 * player's state byte around the call. The innermost function whose call
 * changed it is logged with the functions it was called from on that fiber.
 * A change no wrapper saw (host code or a hand-written function with no
 * wrapped caller) is reported by the per-frame watch below. */
volatile uint32_t g_bgm_watch_adxt;
#define BW_FIB   64
#define BW_DEPTH 64
static uint32_t s_bw_stk[BW_FIB][BW_DEPTH];
static int s_bw_depth[BW_FIB];
static unsigned s_bw_seen = 0xFFFFFFFFu, s_bw_lines;
static int bw_fiber(void)
{
    extern int xbox_fiber_current(void);
    int f = xbox_fiber_current();
    return (f < 0 || f >= BW_FIB) ? 0 : f;
}
/* ADXT_ExecErrChk (0x196300) fields: +0x38 server rate, +0x60 error
 * (0xFFFE no decode progress, 0xFFFF input starved), +0x64 last decoded
 * count, +0x68 no-progress server ticks (limit 5 x rate), +0x6A starved
 * ticks (5 x rate playing, 20 x rate preparing), +0x6D recovery mode
 * (1/2 stop), +0x72 paused; +4 decoder (+1 state, +0x2C decoded samples),
 * +8 stream (+1 state, 4 = error). 0x40C704 counts the ADX vblank worker's
 * server calls (0x1926F0). */
static void bgm_err_fields(const char *why, uint32_t a)
{
    extern unsigned g_doa3_frames_presented;
    uint32_t sjd = MEM32(a + 4), stm = MEM32(a + 8);
    fprintf(stderr, "[BGM-ERRCHK] frame %u %s: state %u rate %u err %04X last %u noprog %d starved %d mode %u paused %u"
            " | decoder %08X state %d decoded %u | stream %08X state %d | vblank-server calls %u\n",
            g_doa3_frames_presented, why, MEM8(a + 1), MEM32(a + 0x38), MEM16(a + 0x60), MEM32(a + 0x64),
            (int)(int16_t)MEM16(a + 0x68), (int)(int16_t)MEM16(a + 0x6A), MEM8(a + 0x6D), MEM8(a + 0x72),
            sjd, (sjd >= 0x1000 && sjd < 0x08000000u) ? (int)(int8_t)MEM8(sjd + 1) : -99,
            (sjd >= 0x1000 && sjd < 0x08000000u) ? MEM32(sjd + 0x2C) : 0,
            stm, (stm >= 0x1000 && stm < 0x08000000u) ? (int)(int8_t)MEM8(stm + 1) : -99, MEM32(0x40C704));
}
void game_audio_bgm_watch_enter(uint32_t va, int *depth)
{
    int f = bw_fiber(), d = s_bw_depth[f]++;
    if (d >= 0 && d < BW_DEPTH) s_bw_stk[f][d] = va;
    *depth = d;
}
void game_audio_bgm_watch_leave(uint32_t va, int depth, unsigned before)
{
    extern unsigned g_doa3_frames_presented;
    extern volatile int g_xbox_det_active;
    int f = bw_fiber(), i, lo;
    uint32_t a = g_bgm_watch_adxt;
    unsigned after;
    s_bw_depth[f] = depth;
    if (!a) return;
    after = MEM8(a + 1);
    if (after == before || after == s_bw_seen) return;
    s_bw_seen = after;
    if (s_bw_lines >= 60) return;
    s_bw_lines++;
    fprintf(stderr, "[BGM-WATCH] frame %u: player %08X state %u -> %u in %08X (fiber %d, rollback clock %s); called from:",
            g_doa3_frames_presented, a, before, after, va, f, (ds_det_active() || g_xbox_det_active) ? "ON" : "off");
    lo = depth > 24 ? depth - 24 : 0;
    for (i = (depth < BW_DEPTH ? depth : BW_DEPTH) - 1; i >= lo; i--) fprintf(stderr, " %08X", s_bw_stk[f][i]);
    fprintf(stderr, "\n");
    if (after == 0) bgm_err_fields("at stop", a);
    fflush(stderr);
}
/* doa3.1-rollback.13 diagnostic, DOA3_WATCH=addr[,addr...] (hex, up to
 * 24): during fights (mode 7), the innermost recompiled function whose call
 * changed a listed dword is logged with its callers on that fiber, the
 * first 3 times per address; changes made outside every wrapped function
 * are reported by the per-frame check. */
volatile int g_mem_watch_on;
static uint32_t s_mw_addr[24], s_mw_seen[24];
static unsigned s_mw_hits[24], s_mw_n, s_mw_lines;
unsigned game_mem_watch_mix(void)
{
    unsigned h = 0, i;
    for (i = 0; i < s_mw_n; i++) h = h * 0x9E3779B1u + MEM32(s_mw_addr[i]);
    return h;
}
static void mem_watch_report(uint32_t va, int f, int depth, unsigned i, uint32_t now)
{
    extern unsigned g_doa3_frames_presented;
    int k, lo;
    if (s_mw_hits[i] >= 3 || s_mw_lines >= 200) return;
    s_mw_hits[i]++; s_mw_lines++;
    if (va) {
        fprintf(stderr, "[MEM-WATCH] frame %u: %08X %08X -> %08X in %08X (fiber %d); called from:",
                g_doa3_frames_presented, s_mw_addr[i], s_mw_seen[i], now, va, f);
        lo = depth > 28 ? depth - 28 : 0;
        for (k = (depth < BW_DEPTH ? depth : BW_DEPTH) - 1; k >= lo; k--) fprintf(stderr, " %08X", s_bw_stk[f][k]);
        fprintf(stderr, "\n");
    } else {
        fprintf(stderr, "[MEM-WATCH] frame %u: %08X %08X -> %08X outside every wrapped function\n",
                g_doa3_frames_presented, s_mw_addr[i], s_mw_seen[i], now);
    }
    fflush(stderr);
}
void game_mem_watch_leave(uint32_t va, int depth, unsigned before)
{
    int f = bw_fiber();
    unsigned i;
    s_bw_depth[f] = depth;
    if (game_mem_watch_mix() == before) return;
    for (i = 0; i < s_mw_n; i++) {
        uint32_t now = MEM32(s_mw_addr[i]);
        if (now != s_mw_seen[i]) { mem_watch_report(va, f, depth, i, now); s_mw_seen[i] = now; }
    }
}
static void mem_watch_frame(void)
{
    static int s_init;
    unsigned i;
    if (!s_init) {
        const char *e = getenv("DOA3_WATCH");
        s_init = 1;
        while (e && *e && s_mw_n < 24) {
            char *end; unsigned long v = strtoul(e, &end, 16);
            if (end == e) break;
            if (v >= 0x1000 && v < 0x08000000u - 4) s_mw_addr[s_mw_n++] = (uint32_t)v & ~3u;
            e = (*end == ',') ? end + 1 : end;
        }
        if (s_mw_n) fprintf(stderr, "[MEM-WATCH] watching %u dwords during fights\n", s_mw_n);
    }
    if (!s_mw_n) return;
    if (MEM8(0x5A2858) != 7) { g_mem_watch_on = 0; return; }
    for (i = 0; i < s_mw_n; i++) {
        uint32_t now = MEM32(s_mw_addr[i]);
        if (g_mem_watch_on && now != s_mw_seen[i]) mem_watch_report(0, 0, 0, i, now);
        s_mw_seen[i] = now;
    }
    g_mem_watch_on = 1;
}
void game_audio_bgm_watch(void)
{
    extern unsigned g_doa3_frames_presented;
    mem_watch_frame();
    extern volatile int g_xbox_det_active;
    static uint32_t s_last[GA_BGM_COUNT][3]; static int s_init; static unsigned s_lines;
    unsigned c;
    for (c = 0; c < GA_BGM_COUNT; c++) {
        uint32_t slot = GA_BGM_SLOTS + c * GA_BGM_STRIDE, h = MEM32(slot), ch = MEM32(slot + 8);
        uint32_t st = (h && h < 0x08000000u) ? MEM8(h + 1) : 0xFFu;
        if (s_init && (h != s_last[c][0] || ch != s_last[c][1] || st != s_last[c][2]) && s_lines < 80) {
            s_lines++;
            fprintf(stderr, "[BGM] frame %u: channel %u player %08X channel-state %u player-state %u (rollback clock %s)\n",
                    g_doa3_frames_presented, c, h, ch, st, (ds_det_active() || g_xbox_det_active) ? "ON" : "off");
            fflush(stderr);
        }
        s_last[c][0] = h; s_last[c][1] = ch; s_last[c][2] = st;
    }
    s_init = 1;
    {
        static int s_on = -1;
        uint32_t h0 = MEM32(GA_BGM_SLOTS);
        if (s_on < 0) { const char *e = getenv("DOA3_BGM_WATCH"); s_on = e && *e == '1'; }
        if (s_on && h0 && h0 < 0x08000000u) {
            unsigned st0 = MEM8(h0 + 1);
            if (g_bgm_watch_adxt == h0 && st0 != s_bw_seen && s_bw_lines < 60) {
                s_bw_lines++;
                fprintf(stderr, "[BGM-WATCH] frame %u: player %08X state now %u, changed outside every wrapped function\n",
                        g_doa3_frames_presented, h0, st0);
                fflush(stderr);
            }
            s_bw_seen = st0;
            g_bgm_watch_adxt = h0;
            {   /* every frame for 40 frames after the clock changes, else once a second */
                extern volatile int g_xbox_det_active;
                static int s_det = -1; static unsigned s_trace, s_n, s_errlines;
                int det = (ds_det_active() || g_xbox_det_active) ? 1 : 0;
                if (det != s_det) { s_trace = 40; s_det = det; }
                if ((s_trace || !(s_n % 60)) && s_errlines < 200) {
                    s_errlines++;
                    bgm_err_fields(det ? "clock ON" : "clock off", h0);
                }
                if (s_trace) s_trace--;
                s_n++;
            }
        }
    }
}
void game_audio_on_adxt_start(uint32_t adxt, uint32_t partition, uint32_t fid)
{
    unsigned i;
    uint32_t f = (partition == GA_BGM_PART) ? fid : 0xFFFFFFFFu;
    if (!adxt) return;
    if (partition == GA_BGM_PART) s_bgm_adxt = adxt;
    if (partition == GA_BGM_PART) {
        extern unsigned g_doa3_frames_presented;
        fprintf(stderr, "[BGM] frame %u: bgm.afs file %u started%s\n",
                g_doa3_frames_presented, fid, ga_licensed(fid) ? " (licensed song)" : "");
    }
    for (i = 0; i < 16; i++)
        if (s_map[i].adxt == adxt) { s_map[i].fid = f; break; }
    if (i == 16) {
        s_map[s_map_next].adxt = adxt; s_map[s_map_next].fid = f;
        s_map_next = (s_map_next + 1) & 15;
    }
    /* Mute at once rather than at the next frame, so the song's first
     * notes are not heard. */
    if (ga_licensed(f) && audio_streamer_mode_get()) game_audio_frame();
}

/* Movie start (0xA47E0, eax = movie number). The credits movie carries
 * "Home Tonight" in its own audio, and its player takes the music level
 * from 0x2D3E9C once, while it is created -- so the level must already be 0
 * when the start routine runs. */
void game_audio_on_movie_start(uint32_t index)
{
    extern unsigned g_doa3_frames_presented;
    fprintf(stderr, "[MOVIE] frame %u: movie %u starting%s\n", g_doa3_frames_presented,
            index, index == GA_MOVIE_CREDITS ? " (credits, licensed song)" : "");
    if (index == GA_MOVIE_CREDITS) {
        s_movie_state = 1;
        s_movie_wait = 0;
        if (audio_streamer_mode_get()) game_audio_frame();
    } else {
        s_movie_state = 0;
    }
}

static int ga_licensed_movie(void)
{
    if (s_movie_state == 1) {
        if (MEM32(GA_MOVIE_HANDLE) != 0) s_movie_state = 2;
        else if (++s_movie_wait > 600) s_movie_state = 0;   /* never started */
    } else if (s_movie_state == 2 && MEM32(GA_MOVIE_HANDLE) == 0) {
        s_movie_state = 0;                                  /* torn down: ended or skipped */
    }
    return s_movie_state != 0;
}

/* A licensed song is on a live music channel and still sounding. */
static int ga_licensed_playing(void)
{
    unsigned c, i;
    for (c = 0; c < GA_BGM_COUNT; c++) {
        uint32_t slot = GA_BGM_SLOTS + c * GA_BGM_STRIDE, h = MEM32(slot);
        uint8_t stat;
        if (!h || MEM32(slot + 8) != 3) continue;
        stat = MEM8(h + 1);
        if (stat < 1 || stat > 4) continue;
        for (i = 0; i < 16; i++)
            if (s_map[i].adxt == h && ga_licensed(s_map[i].fid)) return 1;
    }
    return 0;
}

static void ga_mute(void)
{
    s_saved_music = (int)(int8_t)MEM8(GA_MUSIC_BYTE);
    if (s_saved_music < 0 || s_saved_music > 100) s_saved_music = 80;
    s_muted = 1;
    InterlockedExchange(&s_muting, 1);
    MEM8(GA_MUSIC_BYTE) = 0;
    doa3_guest_set_music_volume(0);
    fprintf(stderr, "[BGM] streamer mode: licensed song muted (music %d restored afterwards)\n", s_saved_music);
}

static void ga_unmute(void)
{
    s_muted = 0;
    MEM8(GA_MUSIC_BYTE) = (uint8_t)s_saved_music;
    doa3_guest_set_music_volume(s_saved_music);
    InterlockedExchange(&s_muting, 0);
    fprintf(stderr, "[BGM] streamer mode: music restored to %d\n", s_saved_music);
}

void game_audio_frame(void)
{
    LONG v;
    int want;
    if (!game_audio_available()) return;

    /* Queued changes from our Audio tab. */
    v = InterlockedExchange(&s_pending[GAME_AUDIO_MUSIC], -1);
    if (v >= 0) {
        if (s_muted) s_saved_music = (int)v;    /* takes effect when the song ends */
        else { MEM8(GA_MUSIC_BYTE) = (uint8_t)v; doa3_guest_set_music_volume((int)v); }
    }
    {   LONG sfx = InterlockedExchange(&s_pending[GAME_AUDIO_SFX], -1);
        LONG voice = InterlockedExchange(&s_pending[GAME_AUDIO_VOICE], -1);
        if (sfx >= 0) MEM8(GA_SFX_BYTE) = (uint8_t)sfx;
        if (voice >= 0) MEM8(GA_VOICE_BYTE) = (uint8_t)voice;
        if (sfx >= 0 || voice >= 0) doa3_guest_apply_sfx_voice();
    }

    /* Streamer mode. While muted, a non-zero music byte means the player
     * changed music in the game's own Options: adopt it as the level to
     * restore and keep the song silent. */
    if (s_muted && MEM8(GA_MUSIC_BYTE) != 0) {
        s_saved_music = (int)(int8_t)MEM8(GA_MUSIC_BYTE);
        MEM8(GA_MUSIC_BYTE) = 0;
        doa3_guest_set_music_volume(0);
    }
    {   int movie = ga_licensed_movie();      /* always tracked, even with the option off */
        want = audio_streamer_mode_get() && (ga_licensed_playing() || movie);
    }
    if (want && !s_muted) ga_mute();
    else if (!want && s_muted) ga_unmute();
}

/* SaveGame copies the save block, music byte included: never let it store
 * the temporary 0. */
void game_audio_before_save(void)
{
    if (s_muted) MEM8(GA_MUSIC_BYTE) = (uint8_t)s_saved_music;
}

void game_audio_after_save(void)
{
    if (s_muted) MEM8(GA_MUSIC_BYTE) = 0;
}

#else   /* other XBE versions: addresses not mapped yet */

int  game_audio_available(void) { return 0; }
int  game_audio_get(int channel) { (void)channel; return 80; }
void game_audio_request(int channel, int value) { (void)channel; (void)value; }
int  game_audio_streamer_muting(void) { return 0; }
void game_audio_frame(void) {}
void game_audio_on_adxt_start(uint32_t a, uint32_t p, uint32_t f) { (void)a; (void)p; (void)f; }
void game_audio_bgm_watch(void) {}
volatile uint32_t g_bgm_watch_adxt;
volatile int g_mem_watch_on;
unsigned game_mem_watch_mix(void) { return 0; }
void game_mem_watch_leave(uint32_t va, int depth, unsigned before) { (void)va; (void)depth; (void)before; }
void game_audio_bgm_watch_enter(uint32_t va, int *depth) { (void)va; *depth = 0; }
void game_audio_bgm_watch_leave(uint32_t va, int depth, unsigned before) { (void)va; (void)depth; (void)before; }
void game_audio_on_movie_start(uint32_t index) { (void)index; }
void game_audio_before_save(void) {}
void game_audio_after_save(void) {}

#endif
