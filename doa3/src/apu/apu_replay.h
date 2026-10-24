#pragma once
#include "apu_rollback.h"

/* Private, single-threaded offline processing backend. Never link into a live
 * game: the production VP's physical-memory shim uses a process-global RAM
 * binding. This exercises the current VP + passthrough DSP, not an Xbox DSP
 * interpreter, CRI worker, DirectSound driver or host mixer/device rewind. */
typedef struct ApuReplay ApuReplay;
typedef struct ApuReplayOutput {
    uint32_t samples;                 /* stereo frames in completed periods */
    int16_t pcm[1024][2];             /* at most four 256-sample periods */
} ApuReplayOutput;

/* Caller owns a private 128 MiB RAM image and snapshots/restores it separately. */
ApuReplay *apu_replay_create(void *ram, size_t bytes);
void apu_replay_destroy(ApuReplay *r);
/* Seed command/filter state from an existing command capture. Output starts
 * at a fresh period; this is explicitly NOT a live processing checkpoint. */
int apu_replay_seed(ApuReplay *r, const void *command, size_t bytes);
size_t apu_replay_snapshot_size(void);
int apu_replay_capture(const ApuReplay *r, void *out, size_t bytes);
int apu_replay_restore(ApuReplay *r, const void *saved, size_t bytes);
int apu_replay_validate(const ApuReplay *r, const void *saved, size_t bytes);
/* Each game frame budgets 25 chip ticks (800 samples). Retire-wait ticks
 * advance immediately and are repaid from later game-frame budgets. Output
 * is returned as data only; no device is opened or samples submitted. */
int apu_replay_frame(ApuReplay *r, ApuReplayOutput *out);
int apu_replay_wait(ApuReplay *r, ApuReplayOutput *out);
int apu_replay_take_irq(ApuReplay *r);
const ApuRollbackClock *apu_replay_clock(const ApuReplay *r);
