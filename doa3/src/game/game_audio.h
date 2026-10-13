#ifndef DOA3_GAME_AUDIO_H
#define DOA3_GAME_AUDIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The game's own Music / Sound effects / Voice settings (Options > Audio
 * Setting), as stored in the save data, and streamer mode.
 *
 * The UI thread reads the values and queues changes; the game thread applies
 * them once per frame through the same routines the game's Options menu
 * uses. Values are 0-100 in steps of 5, like the game's menu. They live in
 * the game's save, not in doa3_settings.ini: the game writes them when it
 * saves (leaving Options, or after the credits). */
enum { GAME_AUDIO_MUSIC = 0, GAME_AUDIO_SFX = 1, GAME_AUDIO_VOICE = 2 };

int  game_audio_available(void);              /* save data loaded, 3.1 build */
int  game_audio_get(int channel);             /* 0-100; music reports the real level while streamer mode mutes it */
void game_audio_request(int channel, int value);
int  game_audio_streamer_muting(void);        /* a licensed song is silenced right now */

/* Game thread only (recomp_manual.c). */
void game_audio_frame(void);
void game_audio_on_adxt_start(uint32_t adxt, uint32_t partition, uint32_t fid);
void game_audio_on_movie_start(uint32_t index);
void game_audio_before_save(void);
void game_audio_after_save(void);

/* Guest calls, defined in recomp_manual.c (game thread only). */
void doa3_guest_set_music_volume(int percent);   /* 0xA61C0, the Options menu's music path */
void doa3_guest_apply_sfx_voice(void);           /* 0x8A3F0, the boot path for SFX and voice */

#ifdef __cplusplus
}
#endif
#endif
