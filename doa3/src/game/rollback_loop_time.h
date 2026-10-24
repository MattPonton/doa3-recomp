#ifndef DOA3_ROLLBACK_LOOP_TIME_H
#define DOA3_ROLLBACK_LOOP_TIME_H
#include "rollback_loop.h"
#include "rollback_input_tape.h"
/* Bounded A65B0/A6680 timestamp bookkeeping only. Requires the exact pending
 * request, healthy owner/primary replay tape and active deterministic clock.
 * Pins the clock during read-only time sampling. Caller excludes other RAM
 * writers and checkpoints registers, RAM, loop, input and clock together.
 * 1 acknowledges success; 0 refuses before mutation; -1 leaves the request
 * pending after partial execution: restore the full original checkpoint.
 * No live caller, pacing, presentation or game-frame advancement. */
int doa3_loop_time_run(Doa3LoopState *,Doa3InputTape *);
#endif
