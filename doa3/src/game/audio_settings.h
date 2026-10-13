#ifndef DOA3_AUDIO_SETTINGS_H
#define DOA3_AUDIO_SETTINGS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Master output level, 0 (silent) through 100 (unchanged). */
int audio_get_volume(void);
void audio_set_volume(int percent);
/* Streamer mode: silence the game's licensed songs (Nine Lives, Home
 * Tonight) through the game's own music volume while they play. */
int audio_streamer_mode_get(void);
void audio_streamer_mode_set(int enabled);
int audio_settings_load(void);
int audio_settings_save(void);

/* Scale an output buffer, after mixing. Count is individual samples,
 * not stereo frames. Safe to call from either audio producer thread. */
void audio_apply_volume(int16_t *samples, size_t count);

#ifdef __cplusplus
}
#endif
#endif
