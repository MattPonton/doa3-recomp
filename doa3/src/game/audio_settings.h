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
int audio_settings_load(void);
int audio_settings_save(void);

/* Scale an output buffer, after mixing. Count is individual samples,
 * not stereo frames. Safe to call from either audio producer thread. */
void audio_apply_volume(int16_t *samples, size_t count);

#ifdef __cplusplus
}
#endif
#endif
