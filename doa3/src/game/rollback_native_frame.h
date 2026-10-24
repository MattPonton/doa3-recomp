#ifndef DOA3_ROLLBACK_NATIVE_FRAME_H
#define DOA3_ROLLBACK_NATIVE_FRAME_H
#include "rollback_loop.h"
#include "rollback_input_tape.h"
/* Bounded opt-in LOCAL native intervals. Uses existing converted tasks/workers
 * and source-pinned outer-loop continuation. Native helpers are not generally
 * preemptible: an unsupported kernel call must stop the diagnostic process,
 * never return synthetic success into unbounded native code. */
#define DOA3_NATIVE_REPLAY_MAX_FRAMES 8u
/* frames=0 preserves the original single-frame caller contract. */
typedef struct { Doa3LoopState *loop;Doa3InputTape *tape;unsigned frames;
    int suppress_host_draws; /* diagnostic only, always recover original end */
    int skip_intermediate_preparation; /* render last frame normally */
    int skip_intermediate_guest_render; /* opt-in vertex deformation skip; commands/state retained */
} Doa3NativeFrame;
int doa3_native_frame_run(void *context);
int doa3_native_frame_active(void);
/* Audited helper allowlist; false outside intermediate replay execution. */
int doa3_native_frame_skip_guest_render(unsigned routine);
void doa3_native_frame_flip(void);
#endif
