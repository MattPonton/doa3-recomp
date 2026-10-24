"""Generate CRI outer continuations at audited vblank and self-suspend waits.

Other helper calls remain native. The scheduler invalidates old checkpoints
if they suspend; this generator does not claim to convert their deep stacks.
"""
ROOTS=(0x1926F0,0x192760,0x1927D0)
WAIT="sub_001DC660(); /* call 0x001DC660 */"

def transform_worker(va,body,transform):
    suspend=va==0x1927D0
    wait="sub_0018C522(); /* call 0x0018C522 */" if suspend else WAIT
    adapter="suspend" if suspend else "wait"
    if body.count(wait)!=1:
        raise ValueError(f"{va:08X}: worker wait changed")
    converted=transform(va,body.replace(wait,"sub_001C65D8(); /* call 0x001C65D8 */"))
    converted=converted.replace("Doa3RollbackTask", "Doa3RollbackWorker")
    converted=converted.replace(f"step_{va:08X}", f"worker_step_{va:08X}")
    prepare=f"if (!doa3_rollback_worker_{adapter}_prepare(task)) return;"
    if suspend:
        # Suspending a different thread is an immediate no-op in the bridge.
        # Do not introduce an extra scheduling boundary on that path.
        prepare+="\n    if (!task->wait_key) goto rollback_resume;"
    converted=converted.replace("doa3_rollback_task_sleep();", prepare)
    converted=converted.replace("rollback_resume: ;", f"rollback_resume: ;\n    if (!doa3_rollback_worker_{adapter}_finish(task)) return;")
    return converted

def generate_workers(bodies,transform):
    import hashlib
    if hashlib.sha256(bodies[0x1DC660].encode()).hexdigest()!="016e8601dd378d4c6279994d086ff0e869122789d18624154ed2ab8053db7114":
        raise ValueError("D3D worker wait changed; reaudit prepare/finish adapter")
    if hashlib.sha256(bodies[0x18C522].encode()).hexdigest()!="201765710ffa0d2c4f1c6a4301c73298469940b3921cb9f301960389af1e8636":
        raise ValueError("Worker suspend changed; reaudit prepare/finish adapter")
    result='#include "rollback_workers.h"\n'
    result+='\n'.join(transform_worker(va,bodies[va],transform) for va in ROOTS)
    result+='''
int doa3_rollback_worker_supported(uint32_t routine) { return routine==0x1926F0 || routine==0x192760 || routine==0x1927D0; }
int doa3_rollback_worker_step(uint32_t routine,Doa3RollbackWorker *worker) {
    if (!worker || worker->resume>1) return 0;
    worker->yielded=0;
    switch(routine) {
    case 0x1926F0: worker_step_001926F0(worker); break;
    case 0x192760: worker_step_00192760(worker); break;
    case 0x1927D0: worker_step_001927D0(worker); break;
    default: return 0;
    }
    return worker->yielded==1;
}
int doa3_rollback_worker_checkpoint_valid(uint32_t routine,const Doa3RollbackWorker *worker) {
    return worker && doa3_rollback_worker_supported(routine) && worker->resume==1 && worker->yielded==1 &&
        worker->wait_object && worker->wait_object<0x08000000u-8u &&
        worker->wait_sp>=24u && worker->wait_sp<0x08000000u-24u &&
        (routine==0x1927D0 ? (worker->wait_key && worker->wait_object==worker->wait_sp+16u) :
         (worker->wait_key==0 || worker->wait_key==0xB1A4B1A4u));
}
'''
    return result
