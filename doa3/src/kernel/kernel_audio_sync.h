#ifndef XBOX_KERNEL_AUDIO_SYNC_H
#define XBOX_KERNEL_AUDIO_SYNC_H
#include <stdint.h>
/* Local primary-thread audio-service scope. Zero initialize before begin;
 * never deserialize from a peer. No mid-scope checkpoint restore yet. */
typedef struct {
    uint32_t phase,owner,epoch,irql,locks;
    uint64_t guard_pin;
} XboxAudioSync;
int xbox_audio_sync_begin(XboxAudioSync *);
/* Executes only exact bound 3.1 synchronization bridges, with validated ABI.
 * No scheduler/trace/native fallback. The caller owns writer exclusion and
 * must account for scheduling separately before enabling live replay. */
int xbox_audio_sync_call(XboxAudioSync *,uint32_t target,uint32_t ordinal,uint32_t expected_sp);
/* On failure, quiesce FIRST, then restore/verify the full checkpoint, then
 * finish(cancelled=1). Quiescing alone does not restore RAM/clock/registers
 * and does not release the kernel or audio output replay guards. */
int xbox_audio_sync_quiesce(XboxAudioSync *);
int xbox_audio_sync_finish(XboxAudioSync *,int cancelled);
#endif
