#ifndef XBOX_KERNEL_REPLAY_H
#define XBOX_KERNEL_REPLAY_H
#include <stdint.h>
/* Local trial guard, not a snapshot or a host-thread exclusion lock. The
 * caller must exclude external writers before begin, retain that exclusion
 * through end, and stop simulation if verified recovery is impossible. */
enum { XBOX_REPLAY_KERNEL_OK, XBOX_REPLAY_KERNEL_UNSUPPORTED,
       XBOX_REPLAY_KERNEL_OWNER, XBOX_REPLAY_KERNEL_EPOCH,
       XBOX_REPLAY_KERNEL_BUDGET };
typedef struct { uint32_t reason,ordinal,accepted,rejected; } XboxKernelReplayReport;
#define XBOX_REPLAY_KERNEL_CALL_LIMIT 100000u
#define XBOX_REPLAY_KERNEL_MAX_FRAMES 8u
int xbox_kernel_replay_begin(void);
/* Set the bounded total call allowance once, before any replay calls. Only
 * the owner/primary may do so, outside pins and before fail-stop is enabled. */
int xbox_kernel_replay_frame_budget(unsigned frames);
/* supported comes only from the audited bridge binding policy. During replay
 * unknown calls fail closed, including native fallback/missing bridges. */
int xbox_kernel_replay_check(uint32_t ordinal,int supported);
int xbox_kernel_replay_healthy(void); /* also valid on owner-thread workers */
/* Native diagnostic helpers cannot unwind at an arbitrary rejected bridge.
 * Opt-in owner/primary mode terminates immediately on a guard rejection. */
int xbox_kernel_replay_fail_stop(int enabled);
/* Read-only admission audit of the ORIGINAL interval. Calls execute normally;
 * unsupported ordinals are counted so the caller can refuse before rewinding.
 * This is preflight evidence, not permission to bypass the replay guard. */
typedef struct { uint32_t accepted,rejected,invalid,unknown,unsupported[400]; } XboxKernelReplayAudit;
int xbox_kernel_replay_audit_begin(void);
int xbox_kernel_replay_audit_end(XboxKernelReplayAudit *);
/* Active owner on the primary fiber in the same epoch; includes a faulted
 * guard so recovery can quiesce host state without releasing exclusion. */
int xbox_kernel_replay_owned(void);
/* One local recovery scope may pin the guard. End cannot release the guard
 * while pinned, including cancellation. Tokens are never reused. */
uint64_t xbox_kernel_replay_pin(void);
int xbox_kernel_replay_pinned(uint64_t token);
int xbox_kernel_replay_unpin(uint64_t token);
void xbox_kernel_replay_report(XboxKernelReplayReport *);
/* cancelled=1 only AFTER the full original state has been restored/verified.
 * Wrong owner, changed session or non-primary end never releases the guard. */
int xbox_kernel_replay_end(int cancelled);
#endif
