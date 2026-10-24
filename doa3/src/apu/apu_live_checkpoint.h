#ifndef DOA3_APU_LIVE_CHECKPOINT_H
#define DOA3_APU_LIVE_CHECKPOINT_H
#include <stddef.h>
/* Process-local processing state for the current VP/passthrough backend.
 * Requires the deterministic APU owner, primary guest context and held APU pin.
 * Contains command/filter state, partial PCM, logical XA2 staging and chip time.
 * Guest RAM and interrupt delivery queues must be restored separately.
 * Rejects software voices, test tones, debug monitoring/muting, movie output,
 * waveOut and unsupported XA2 buffer sizes. Does not rewind device buffers.
 * Caller must exclude movie/mixer/configuration/device-lifecycle writers for
 * the entire trial, as well as snapshotting guest RAM and IRQ queues.
 * There is deliberately no live game caller yet. */
size_t mcpx_apu_processing_size(void);
int mcpx_apu_processing_capture(void *,size_t);
int mcpx_apu_processing_validate(const void *,size_t);
int mcpx_apu_processing_can_restore(void); /* transaction preflight under pin */
/* Restore additionally requires the local replay output gate below. */
int mcpx_apu_processing_restore(const void *,size_t);
/* Begin retains the current processing end-state. During replay the monitor
 * advances logical staging but never waits for/submits to a physical device.
 * End succeeds only after processing returns EXACTLY to that retained state.
 * cancel=1 permits closing a faulted gate after the caller restores its full
 * original checkpoint (including RAM). Failure leaves output gated and blocks
 * deterministic-session exit. Never clear it and continue partially restored.
 * Device playback can drain previously submitted buffers during the trial.
 * This supports unchanged-input round-trip checks, not corrected-audio output. */
int mcpx_apu_replay_begin(void);
int mcpx_apu_replay_end(int cancel);
#endif
