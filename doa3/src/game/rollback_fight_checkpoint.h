#ifndef DOA3_FIGHT_CHECKPOINT_H
#define DOA3_FIGHT_CHECKPOINT_H
#include "rollback_loop.h"
#include "online/rollback_checkpoint.h"
/* Production binding for the first loaded-fight local trial. Caller owns the
 * deterministic clock/APU sessions, parked outer loop, and exclusion of UI,
 * device/configuration, movie, topology and other external writers throughout
 * the window. This object does not provide frame advancement or enable replay.
 * Create/destroy at the same passive primary boundary in the same session.
 * All retained images and trials must be retired before destroying it. */
typedef struct Doa3FightCheckpoint Doa3FightCheckpoint;
Doa3FightCheckpoint *doa3_fight_checkpoint_create(Doa3LoopState *);
Doa3CheckpointSession *doa3_fight_checkpoint_session(Doa3FightCheckpoint *);
int doa3_fight_checkpoint_destroy(Doa3FightCheckpoint *);
/* Restore-only smoke test: captures/restores the SAME parked state, with
 * audio output gated. Does not advance/replay a frame. Caller owns active
 * deterministic sessions. 1 verified, 0 safe refusal, -1 fatal: stop process;
 * failed restoration retains resources and gates rather than resuming. */
int doa3_fight_checkpoint_roundtrip(Doa3LoopState *,Doa3CheckpointComparison *);
void doa3_fight_checkpoint_probe_boundary(void); /* explicit opt-in, once */
/* Visit a retained earlier checkpoint, verify it, then restore/verify the
 * current endpoint before returning. No guest instructions run at the earlier
 * state. Images remain owned by the caller. 1 verified rewind/return, 0 safe
 * refusal/recovery, -1 fatal with guards retained: stop the process. */
int doa3_fight_checkpoint_rewind_check(Doa3FightCheckpoint *,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,Doa3CheckpointComparison *);
void doa3_fight_window_probe_boundary(void); /* one native interval, opt-in */
/* Experimental synchronous native interval. The callback owns its execution
 * bounds/output suppression; physical pins are released only while it runs.
 * Always restore/verify end before returning, including after a matching replay.
 * 1 means callback completed and its entire endpoint matched; 0 recovered;
 * -1 retains gates and requires process termination. */
int doa3_fight_checkpoint_replay_check(Doa3FightCheckpoint *,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,
    int (*advance)(void *),void *context,Doa3CheckpointComparison *);
/* Diagnostic ONLY: additionally returns 2 for an exact CPU/logical renderer
 * endpoint with differing GPU pixels. Still performs a full comparison and
 * restores/verifies the original rendered endpoint. Never resume that replay. */
int doa3_fight_checkpoint_render_probe(Doa3FightCheckpoint *,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,
    int (*advance)(void *),void *context,Doa3CheckpointComparison *);
#endif
