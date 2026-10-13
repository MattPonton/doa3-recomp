#include "game_audio.h"
#include "audio_settings.h"
#include "recomp/recomp_types.h"
#include "recomp/gen/xbe_layout.h"   /* DOA3_XBE_ID_3_1 */
#include <windows.h>
#include <stdio.h>

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
 * bgm.afs. The game's song list at 0x29CE48 pairs "nine lives" with 0x18 and
 * "home tonight" with 0x17 in the low word; [BGM] log lines confirm the file
 * numbers at runtime. */

#if defined(DOA3_XBE_ID_3_1)

#define GA_MUSIC_BYTE  0x59D094u
#define GA_SFX_BYTE    0x59D095u
#define GA_VOICE_BYTE  0x59D096u
#define GA_BGM_SLOTS   0x5CF418u
#define GA_BGM_STRIDE  0x45A00u
#define GA_BGM_COUNT   5u
#define GA_BGM_PART    2u

static int ga_licensed(uint32_t fid) { return fid == 0x18u || fid == 0x17u; }

/* Which bgm.afs file each ADXT handle was last started with. */
static struct { uint32_t adxt, fid; } s_map[16];
static unsigned s_map_next;

static volatile LONG s_pending[3] = { -1, -1, -1 };
static volatile LONG s_muting;          /* read by the UI */
static int s_muted, s_saved_music;      /* game thread */

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

void game_audio_on_adxt_start(uint32_t adxt, uint32_t partition, uint32_t fid)
{
    unsigned i;
    uint32_t f = (partition == GA_BGM_PART) ? fid : 0xFFFFFFFFu;
    if (!adxt) return;
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
    want = audio_streamer_mode_get() && ga_licensed_playing();
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
void game_audio_before_save(void) {}
void game_audio_after_save(void) {}

#endif
