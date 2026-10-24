#ifndef DOA3_ROLLBACK_WORKERS_H
#define DOA3_ROLLBACK_WORKERS_H
#include <stdint.h>
typedef struct {
    uint32_t resume,yielded,wait_key,wait_object,wait_sp;
    uint32_t locals[32];
} Doa3RollbackWorker;
int doa3_rollback_worker_supported(uint32_t routine);
int doa3_rollback_worker_step(uint32_t routine,Doa3RollbackWorker *worker);
int doa3_rollback_worker_checkpoint_valid(uint32_t routine,const Doa3RollbackWorker *worker);
int doa3_rollback_worker_wait_prepare(Doa3RollbackWorker *worker);
int doa3_rollback_worker_wait_finish(Doa3RollbackWorker *worker);
int doa3_rollback_worker_suspend_prepare(Doa3RollbackWorker *worker);
int doa3_rollback_worker_suspend_finish(Doa3RollbackWorker *worker);
#endif
