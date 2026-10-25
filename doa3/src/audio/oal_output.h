/**
 * OpenAL Soft output device: plays the finished DirectSound mix
 */
#ifndef OAL_OUTPUT_H
#define OAL_OUTPUT_H

#include <stdint.h>

/* Open the default device and make its context current on the calling
 * thread (the mixer thread; the game's sounds live on another context).
 * Returns 1 on success, 0 on failure. */
int oal_init(void);

/* Close the device and release all resources. */
void oal_shutdown(void);

/* Returns 1 if the output is active. */
int oal_is_active(void);

/* Buffers queued and not yet played (-1 when inactive). */
int oal_queued(void);

/* Submit interleaved stereo 16-bit samples. Returns 1 if accepted. */
int oal_submit_samples(const int16_t *samples, int num_samples);

/* Get the preferred buffer size in samples. */
int oal_get_buffer_size(void);

/* Write the last 30 s of the mix to Diagnostics\audio_mix_<tag>.wav
 * (automatic diagnostic captures only). */
void audio_mix_dump(const char *tag);

#endif /* OAL_OUTPUT_H */
