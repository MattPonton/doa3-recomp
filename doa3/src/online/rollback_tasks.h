#ifndef DOA3_ROLLBACK_TASKS_H
#define DOA3_ROLLBACK_TASKS_H
#include <stdint.h>
#include "rollback_nested.h"

/* Explicit C locals and resume point for the audited 3.1 fight coroutines.
 * Guest registers, x87 state and guest stack are owned by the fiber scheduler.
 * No native stack address is stored here. Schema changes invalidate snapshots. */
#define DOA3_RB_TASK_SCHEMA 2u
#define DOA3_RB_TASK_LOCALS 32
#define DOA3_RB_TASK_COUNT 15u
#define DOA3_RB_TASK_ROUTINES 0x87420,0x72070,0x54110,0xBE4A0,0x11110,0xB4720,0xA00D0,0x98DB0,0x7F470,0x235B0,0x1C60AB,0xE8030,0x49790,0x691F0,0xBB500
typedef struct {
    uint32_t resume;
    uint32_t yielded;
    uint32_t locals[DOA3_RB_TASK_LOCALS];
    Doa3NestedStack nested;
} Doa3RollbackTask;

/* Explicit opt-in at process start, before tasks are created. */
int doa3_rollback_tasks_enabled(void);
int doa3_rollback_task_supported(uint32_t routine);
int doa3_rollback_task_step(uint32_t routine, Doa3RollbackTask *task);
int doa3_rollback_task_checkpoint_valid(uint32_t routine,const Doa3RollbackTask *task);
void doa3_rollback_task_sleep(void);
#endif
