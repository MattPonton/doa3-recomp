#ifndef DOA3_ROLLBACK_CAPTURE_H
#define DOA3_ROLLBACK_CAPTURE_H
#include "rollback_tasks.h"
/* Local diagnostic format, little endian. Not a portable save or net snapshot.
 * Every byte of the base guest mapping is included through 4 KB page deltas. */
#define DOA3_RBC_MAGIC 0x31434252u
#define DOA3_RBC_VERSION 5u
/* v5 follows the source digest with APU/GPU payload sizes (two u32s).
 * Records contain task state, optional APU, optional USER queue, then pages. */
/* v4 appends the explicit nested call stack to each task-state record. */
/* v2 record word 3 is the presentation counter at the task boundary. */
#define DOA3_RBC_PAGE 4096u
#define DOA3_RBC_PAIRS (DOA3_RB_TASK_COUNT * 6u)
extern const unsigned char doa3_rollback_source_digest[32];
typedef struct {
    /* eax ecx edx esp ebx esi edi seh, string operands, flag operands,
     * flag width/test, fp top, x87 control word. */
    uint32_t regs[16];
    double fp[8];
    Doa3RollbackTask task;
} Doa3RollbackCaptureState;
void doa3_rollback_capture_before(uint32_t routine, const Doa3RollbackTask *task);
void doa3_rollback_capture_after(uint32_t routine, const Doa3RollbackTask *task);
void doa3_rollback_capture_cancel(void);
int doa3_rollback_capture_is_armed(void);
#endif
