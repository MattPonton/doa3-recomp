/**
 * Xbox DirectSound, high level, on OpenAL Soft
 *
 * The game's DirectSound entry points (recomp_dsound.c) land here instead of
 * the recompiled DSOUND library, which drove the MCPX APU. Each sound
 * buffer is an OpenAL source whose samples come from a buffer callback that
 * reads the buffer's guest memory as the hardware voice did, so play
 * cursors, loop regions and buffers the game rewrites while they play all
 * behave as on the console.
 *
 * Every function here is called from the game thread and takes the engine
 * lock; the mixer thread renders under the same lock.
 */
#ifndef DSOUND_HLE_H
#define DSOUND_HLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Xbox DirectSound constants the game uses */
#define DSBCAPS_CTRL3D                0x00000010u
#define DSBCAPS_MIXIN                 0x00002000u
#define DSBCAPS_FXIN                  0x00080000u
#define DSBPLAY_LOOPING               0x00000001u
#define DSBPLAY_FROMSTART             0x00000002u
#define DSBSTOPEX_ENVELOPE            0x00000001u
#define DSBSTOPEX_RELEASEWAVEFORM     0x00000002u
#define DSBSTATUS_PLAYING             0x00000001u
#define DSBSTATUS_LOOPING             0x00000004u
#define DSBLOCK_FROMWRITECURSOR       0x00000001u
#define DSBLOCK_ENTIREBUFFER          0x00000002u
#define DSERR_INVALIDCALL             0x88780032u
#define DSERR_OUTOFMEMORY             0x8007000Eu
#define DSERR_GENERIC                 0x80004005u
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM               0x0001u
#endif
#define WAVE_FORMAT_XBOX_ADPCM        0x0069u

/* WAVEFORMATEX fields the voices need */
typedef struct DsFormat {
    uint16_t tag;
    uint16_t channels;
    uint32_t rate;
    uint16_t block_align;
    uint16_t bits;
} DsFormat;

/* `mem_base` is where guest address 0 is mapped in the host. Opens the
 * loopback device the game's sounds play on and starts the mixer thread,
 * which owns the real output device (oal_output.c). Returns 1 on success. */
int  ds_init(uint8_t *mem_base);
void ds_shutdown(void);

/* Buffers are small positive handles (0 = none). A buffer created with a
 * size owns guest memory at `data_va` (the caller allocates it); one
 * created without gets its data from ds_buffer_set_data. */
int      ds_buffer_create(uint32_t flags, const DsFormat *fmt, uint32_t data_va, uint32_t data_bytes);
void     ds_buffer_destroy(int h);
void     ds_buffer_data(int h, uint32_t *data_va, uint32_t *data_bytes);
void     ds_buffer_set_data(int h, uint32_t data_va, uint32_t data_bytes);
uint32_t ds_buffer_set_loop_region(int h, uint32_t start_bytes, uint32_t length_bytes);
void     ds_buffer_play(int h, uint32_t flags);
void     ds_buffer_stop(int h);
void     ds_buffer_stop_ex(int h, uint32_t flags);
uint32_t ds_buffer_status(int h);
void     ds_buffer_position(int h, uint32_t *play_bytes, uint32_t *write_bytes);
void     ds_buffer_set_position(int h, uint32_t play_bytes);
void     ds_buffer_set_volume(int h, int32_t volume_mb);
void     ds_buffer_set_headroom(int h, uint32_t headroom_mb);
void     ds_buffer_set_frequency(int h, uint32_t hz);
/* `pairs` is count (mixbin, volume in mB) pairs. SetMixBins replaces the
 * buffer's bins; SetMixBinVolumes only changes the volumes of bins it has. */
void     ds_buffer_set_mixbins(int h, const uint32_t *pairs, int count);
void     ds_buffer_set_mixbin_volumes(int h, const uint32_t *pairs, int count);
void     ds_buffer_set_3d_position(int h, float x, float y, float z);
void     ds_buffer_set_3d_velocity(int h, float x, float y, float z);
void     ds_buffer_set_min_distance(int h, float distance);

void     ds_listener_set_position(float x, float y, float z);
void     ds_listener_set_velocity(float x, float y, float z);
/* The twelve DSI3DL2LISTENER words, as the guest stores them. */
void     ds_set_i3dl2(const uint32_t raw[12]);

/* Online sessions (netplay_session.c): what the game sees of its buffers
 * (status, play cursor) then advances by exactly 800 samples per lockstep
 * frame instead of with the output device, so both machines see the same. */
void     ds_det_begin(void);
void     ds_det_end(void);
void     ds_det_frame(void);

/* Host-decoded movie audio (movie_present.c): one 48 kHz stereo stream. */
int      audio_movie_start(void);
int      audio_movie_submit(const int16_t *samples, int num_samples, int end_of_stream);
uint64_t audio_movie_samples_played(void);
void     audio_movie_stop(void);

#ifdef __cplusplus
}
#endif
#endif /* DSOUND_HLE_H */
