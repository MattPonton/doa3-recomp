#ifndef DOA3_ROLLBACK_TRIAL_H
#define DOA3_ROLLBACK_TRIAL_H
#include "online/rollback_checkpoint.h"
#include "rollback_input_tape.h"
#include "kernel/kernel_replay.h"

/* Bounded LOCAL unchanged-input validation, not network rollback. No live
 * caller is enabled. The caller supplies a complete parked frame advance and
 * independent expected-state comparisons, including renderer state if used. */
#define DOA3_TRIAL_MAX_FRAMES 16u
typedef struct {
    void *context;
    Doa3CheckpointSession *checkpoints;
    Doa3InputTape *inputs;
    /* Trial-wide exclusion of lifecycle/configuration and other external
     * writers. This is NOT the per-checkpoint clock/APU pin: advance must be
     * able to run. Enter/begin return 0 only if nothing was acquired/enabled. */
    int (*enter)(void *);
    void (*leave)(void *);
    /* Suppress irreversible output for the whole trial. A failed end must
     * retain the gate. cancel=1 is used only after verified recovery. */
    int (*output_begin)(void *);
    int (*output_end)(void *,int cancel);
    /* Advance one complete interval, without presenting, wall-clock pacing,
     * external writes, or input outside the recorded tape. Return at primary
     * boundary even on failure, with a bounded execution budget. Kernel
     * rejection does not unwind arbitrary guest/native stacks: advance must
     * return promptly when the kernel replay guard reports a fault.
     * Verify must be read-only and compare ALL
     * included state; it is also called for the original end-state. */
    int (*advance)(void *,uint64_t frame);
    int (*verify)(void *,uint64_t frame);
} Doa3TrialConfig;
enum {
    DOA3_TRIAL_OK, DOA3_TRIAL_PREFLIGHT, DOA3_TRIAL_ENTER,
    DOA3_TRIAL_VERIFY, DOA3_TRIAL_CAPTURE, DOA3_TRIAL_OUTPUT,
    DOA3_TRIAL_RESTORE, DOA3_TRIAL_INPUT, DOA3_TRIAL_ADVANCE,
    DOA3_TRIAL_EPOCH, DOA3_TRIAL_RECOVERY, DOA3_TRIAL_CLEANUP, DOA3_TRIAL_KERNEL
};
typedef struct {
    unsigned reason,advanced;uint64_t frame;int recovered,fatal;
    XboxKernelReplayReport kernel;
} Doa3TrialReport;
typedef struct Doa3ReplayTrial Doa3ReplayTrial;
Doa3ReplayTrial *doa3_trial_create(const Doa3TrialConfig *);
/* start is a retained checkpoint AFTER first; tape covers first+1..last.
 * Returns 1 verified, 0 untouched/recovered failure, -1 fatal: STOP the game.
 * Fatal retains the original image/exclusion and any still-active gates;
 * no subsequent trial/destroy is allowed. Process teardown owns them then.
 * Checkpoints/tape/config context must outlive the trial. No peer data. */
int doa3_trial_run(Doa3ReplayTrial *,const Doa3CheckpointImage *start,uint64_t first,uint64_t last);
int doa3_trial_report(const Doa3ReplayTrial *,Doa3TrialReport *);
int doa3_trial_destroy(Doa3ReplayTrial *);
#endif
