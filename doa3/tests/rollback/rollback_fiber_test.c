/* Production scheduler checkpoint/restore, using actual Windows fibers and
 * a small explicit task fixture. Does not claim to restore whole DOA3. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel/xbox_fiber.h"
#include "rollback_tasks.h"
#include "rollback_workers.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_seh_ebp;
double g_fp_stack[8]; int g_fp_top;
ptrdiff_t g_xbox_mem_offset;
volatile int g_doa3_post_movie = 1, g_xbox_det_active;
static unsigned char memory[8 * 1024 * 1024];
static uint32_t heap = 0x10000, observed[2];
static int nested;
static unsigned cancelled;
static unsigned worker_runs;
static uint32_t clock_epoch=1;
uint32_t xbox_det_epoch(void) { return clock_epoch; }
typedef void (*recomp_func_t)(void);
static void fixture_entry(void)
{
    /* Both fibers have one shared entry, just like DOA3's 1C6690 wrapper.
     * The body comes from its argument, not the fiber start address. */
    uint32_t routine = *(uint32_t *)(memory + g_esp + 4);
    CHECK(!xbox_fiber_run_restorable_task(0xBAD));
    (void)xbox_fiber_run_restorable_task(routine);
}
static void worker_entry(void)
{
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9002,0));
    for (;;) { ++worker_runs; xbox_fiber_block(0x1234); }
}
static void watchdog_entry(void) {
    uint32_t routine=xbox_fiber_current_ctx1();
    if(routine==0x1926B0) CHECK(xbox_fiber_run_restorable_watchdog());
    else CHECK(xbox_fiber_run_restorable_stream_worker(routine));
}
static int stream_nested,stream_exit;
static uint32_t stream_wait=0x5555;
int doa3_rollback_worker_supported(uint32_t routine) { return routine==0x1926F0 || routine==0x192760 || routine==0x1927D0; }
int doa3_rollback_worker_checkpoint_valid(uint32_t routine,const Doa3RollbackWorker *worker) {
    return doa3_rollback_worker_supported(routine) && worker->resume==1 && worker->yielded==1 && worker->wait_key==(routine==0x1927D0 ? 0xBEEF9004 : stream_wait);
}
int doa3_rollback_worker_step(uint32_t routine,Doa3RollbackWorker *worker) {
    CHECK(doa3_rollback_worker_supported(routine));
    CHECK(!xbox_fiber_replay_thread_allowed(xbox_fiber_current_xhandle(),0));
    if(g_xbox_det_active)CHECK(xbox_fiber_replay_thread_allowed(0xBEEF9999,0));
    if(stream_exit && routine==0x1927D0) xbox_fiber_exit();
    if(stream_nested) { stream_nested=0; xbox_fiber_yield(); }
    worker->resume=worker->yielded=1; worker->wait_key=routine==0x1927D0 ? xbox_fiber_current_xhandle() : stream_wait; worker->wait_sp=g_esp;
    worker->locals[0]=worker->locals[0]*13+1; g_eax=worker->locals[0];
    *(uint32_t *)(memory+0x500000+(routine==0x1927D0 ? 4 : 0))=g_eax;
    return 1;
}
recomp_func_t recomp_lookup(uint32_t va) {
    if(va==0x2000) return worker_entry;
    if(va==0x18C690) return watchdog_entry;
    CHECK(va == 0x1000); return fixture_entry;
}
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
uint32_t xbox_HeapAllocHigh(uint32_t bytes, uint32_t alignment)
{
    uint32_t result = heap;
    (void)alignment;
    heap += bytes;
    CHECK(heap < sizeof(memory));
    return result;
}
int doa3_workers_may_run(void) { return 0; }
unsigned char xbox_current_irql(void) { return 0; }
void xbox_det_host_slice_timer(void) { }
int xbox_det_slice_laps(void) { return 0; }
int doa3_rollback_tasks_enabled(void) { return 1; }
void doa3_rollback_capture_before(uint32_t routine, const Doa3RollbackTask *task) { (void)routine; (void)task; }
void doa3_rollback_capture_after(uint32_t routine, const Doa3RollbackTask *task) { (void)routine; (void)task; }
void doa3_rollback_capture_cancel(void) { ++cancelled; }
int doa3_rollback_task_checkpoint_valid(uint32_t routine,const Doa3RollbackTask *task) {
    return task->resume==1 && task->yielded==1 &&
        (routine!=2 || (task->nested.schema==DOA3_NESTED_SCHEMA && task->nested.depth==2));
}
int doa3_rollback_task_supported(uint32_t routine) { return routine == 1 || routine == 2; }
int doa3_rollback_task_step(uint32_t routine, Doa3RollbackTask *task)
{
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9002,1));
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9002,0));
    if (nested) {
        int kind = nested;
        nested = 0;
        if (kind == 2) xbox_fiber_switch_direct(0);
        else xbox_fiber_yield_back();
    }
    task->locals[0] = task->locals[0] * 33u + routine;
    g_eax = task->locals[0];
    if(routine==2) {
        task->nested.schema=DOA3_NESTED_SCHEMA;
        task->nested.depth=2; task->nested.status=DOA3_NESTED_YIELDED;
        task->nested.frames[0].routine=0x72070; task->nested.frames[0].resume=2;
        task->nested.frames[1].routine=0x85820; task->nested.frames[1].resume=1;
        task->nested.frames[1].locals[40]=task->nested.frames[1].locals[40]*17u+3;
        g_eax^=task->nested.frames[1].locals[40];
    }
    g_fp_stack[0] = (double)task->locals[0]; g_fp_top = 0;
    observed[routine - 1] = g_eax;
    task->resume = task->yielded = 1;
    return 1;
}
int main(void)
{
    _putenv_s("DOA3_ROLLBACK_WORKERS","1");
    _putenv_s("DOA3_ROLLBACK_STREAM_WORKERS","1");
    int ids[2], count;
    uint32_t expected[2];
    size_t bytes = xbox_fiber_task_snapshot_size();
    void *saved = malloc(bytes), *check = malloc(bytes);
    CHECK(saved && check);
    CHECK(!xbox_fiber_task_capture(saved, bytes));
    g_xbox_mem_offset = (ptrdiff_t)(uintptr_t)memory;
    xbox_fiber_init();
    ids[0] = xbox_fiber_create_dormant(0x1000, 1, 0x8000);
    ids[1] = xbox_fiber_create_dormant(0x1000, 2, 0x8000);
    CHECK(ids[0] > 0 && ids[1] > 0);
    CHECK(!xbox_fiber_task_capture(saved, bytes)); // not yet at a yielded boundary
    xbox_fiber_switch_direct(ids[0]); xbox_fiber_switch_direct(ids[1]);
    CHECK(xbox_fiber_task_capture(saved, bytes) == 2);
    for (count = 0; count < 25; ++count) {
        xbox_fiber_switch_direct(ids[0]); xbox_fiber_switch_direct(ids[1]);
    }
    memcpy(expected, observed, sizeof(expected));
    CHECK(!xbox_fiber_task_restore(saved, bytes - 1));
    CHECK(xbox_fiber_task_restore(saved, bytes) == 2);
    CHECK(xbox_fiber_task_capture(check, bytes) == 2);
    CHECK(!memcmp(saved, check, bytes));
    /* Restored tasks reproduce the same guest state across repeated steps. */
    for (count = 0; count < 25; ++count) {
        xbox_fiber_switch_direct(ids[0]); xbox_fiber_switch_direct(ids[1]);
    }
    CHECK(!memcmp(expected, observed, sizeof(expected)));
    // A task-slot reuse must invalidate old checkpoints even for same routine.
    xbox_fiber_destroy(ids[1]);
    ids[1] = xbox_fiber_create_dormant(0x1000, 2, 0x8000);
    xbox_fiber_switch_direct(ids[1]);
    CHECK(!xbox_fiber_task_restore(saved, bytes));
    CHECK(xbox_fiber_task_capture(saved, bytes) == 2);
    nested = 1;
    xbox_fiber_switch_direct(ids[0]);
    CHECK(!xbox_fiber_task_capture(check, bytes));
    xbox_fiber_switch_direct(ids[0]); // return from the unexpected nested yield
    CHECK(!xbox_fiber_task_restore(saved, bytes));
    CHECK(cancelled==1);
    CHECK(xbox_fiber_task_capture(check, bytes)==2); // fully unwound, new epoch
    xbox_fiber_destroy(ids[0]); xbox_fiber_destroy(ids[1]);
    // The tagged SwitchToFiber path must reject nested stacks too.
    ids[0] = xbox_fiber_create_dormant(0x1000, 1, 0x8000);
    xbox_fiber_switch_direct(ids[0]);
    CHECK(xbox_fiber_task_capture(saved, bytes) == 1);
    nested = 2;
    xbox_fiber_switch_direct(ids[0]);
    CHECK(!xbox_fiber_task_capture(check, bytes));
    xbox_fiber_switch_direct(ids[0]);
    CHECK(!xbox_fiber_task_restore(saved, bytes));
    CHECK(cancelled==2);
    xbox_fiber_destroy(ids[0]);
    /* A worker can advance and return to exactly the same native wait. Its
     * progress must still invalidate an otherwise valid task checkpoint. */
    ids[0]=xbox_fiber_create_dormant(0x1000,1,0x8000);
    xbox_fiber_switch_direct(ids[0]);
    CHECK(xbox_fiber_spawn(0x2000,0,0,0xBEEF9001,0));
    CHECK(xbox_fiber_run_thread(0xBEEF9001) && worker_runs==1);
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9001,1));
    CHECK(xbox_fiber_task_capture(saved,bytes)==1);
    CHECK(xbox_fiber_wake_count(0x1234)==1);
    CHECK(!xbox_fiber_task_restore(saved,bytes));
    CHECK(xbox_fiber_task_capture(saved,bytes)==1);
    CHECK(xbox_fiber_run_thread(0xBEEF9001) && worker_runs==2);
    CHECK(xbox_fiber_task_capture(check,bytes)==1);
    CHECK(!xbox_fiber_task_restore(saved,bytes));
    CHECK(xbox_fiber_task_capture(saved,bytes)==1 && !memcmp(saved,check,bytes));
    /* Task-only replay is still permitted while the worker stays parked. */
    xbox_fiber_switch_direct(ids[0]);
    CHECK(xbox_fiber_task_restore(saved,bytes)==1 && worker_runs==2);
    /* Joint task/watchdog restore: all dynamic state is guest RAM or saved
     * registers. The watchdog's parked native wrapper stays at one boundary. */
    CHECK(xbox_fiber_spawn(0x18C690,0x1926B0,0,0xBEEF9002,0));
    CHECK(xbox_fiber_run_thread(0xBEEF9002));
    CHECK(*(uint32_t *)(memory+0x40C700)==1);
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9002,1)); // no controlled clock
    CHECK(!xbox_fiber_converted_capture(saved,bytes)); // requires a clock session
    g_xbox_det_active=1;
    CHECK(xbox_fiber_replay_thread_allowed(0xBEEF9002,1));
    CHECK(xbox_fiber_replay_thread_allowed(0xBEEF9002,0)); // primary suspend is a no-op
    CHECK(!xbox_fiber_replay_thread_allowed(0,1));
    CHECK(!xbox_fiber_replay_thread_allowed(0xFFFFFFFEu,0));
    CHECK(!xbox_fiber_replay_thread_allowed(0xDEADBEEF,1));
    CHECK(!xbox_fiber_replay_thread_allowed(0x1234,1)); // would wake unconverted worker
    CHECK(bytes==xbox_fiber_converted_snapshot_size());
    CHECK(xbox_fiber_converted_capture(saved,bytes)==2);
    unsigned char *ram_snapshot=malloc(sizeof(memory)); CHECK(ram_snapshot);
    memcpy(ram_snapshot,memory,sizeof(memory));
    CHECK(xbox_fiber_task_capture(check,bytes)==1);
    for(int i=0;i<30;++i) {
        xbox_fiber_switch_direct(ids[0]); CHECK(xbox_fiber_run_thread(0xBEEF9002));
    }
    CHECK(!xbox_fiber_task_restore(check,bytes)); // task-only snapshots omit worker progress
    uint32_t task_after=observed[0],counter_after=*(uint32_t *)(memory+0x40C700);
    CHECK(counter_after==31);
    unsigned char *ram_after=malloc(sizeof(memory)); CHECK(ram_after);
    memcpy(ram_after,memory,sizeof(memory));
    CHECK(xbox_fiber_converted_capture(check,bytes)==2);
    CHECK(!xbox_fiber_converted_restore(saved,bytes-1));
    for(int replay=0;replay<2;++replay) {
        CHECK(xbox_fiber_converted_restore(saved,bytes)==2);
        memcpy(memory,ram_snapshot,sizeof(memory));
        for(int i=0;i<30;++i) {
            xbox_fiber_switch_direct(ids[0]); CHECK(xbox_fiber_run_thread(0xBEEF9002));
        }
        CHECK(observed[0]==task_after && *(uint32_t *)(memory+0x40C700)==counter_after);
        CHECK(!memcmp(memory,ram_after,sizeof(memory)));
        void *result=malloc(bytes); CHECK(result);
        CHECK(xbox_fiber_converted_capture(result,bytes)==2 && !memcmp(result,check,bytes));
        free(result);
    }
    ++clock_epoch; CHECK(!xbox_fiber_converted_restore(saved,bytes));
    CHECK(xbox_fiber_wake_count(0x1234)==1);
    CHECK(xbox_fiber_converted_capture(saved,bytes)==2);
    CHECK(xbox_fiber_run_thread(0xBEEF9001) && worker_runs==3);
    CHECK(!xbox_fiber_converted_restore(saved,bytes)); // other workers remain guarded
    CHECK(xbox_fiber_converted_capture(saved,bytes)==2);
    *(uint32_t *)(memory+0x40C718)=1; CHECK(xbox_fiber_run_thread(0xBEEF9002));
    CHECK(*(uint32_t *)(memory+0x40C71C)==1 && !xbox_fiber_thread_alive(0x1926B0));
    CHECK(!xbox_fiber_converted_restore(saved,bytes)); // cannot resurrect a finished worker
    CHECK(xbox_fiber_spawn(0x18C690,0x1926F0,0,0xBEEF9003,0));
    CHECK(xbox_fiber_run_thread(0xBEEF9003));
    CHECK(xbox_fiber_converted_capture(saved,bytes)==2);
    memcpy(ram_snapshot,memory,sizeof(memory));
    for(int replay=0;replay<3;++replay) {
        CHECK(xbox_fiber_converted_restore(saved,bytes)==2);
        memcpy(memory,ram_snapshot,sizeof(memory));
        CHECK(!xbox_fiber_run_thread(0xBEEF9003)); // restored WAITING, not silently READY
        for(int i=0;i<30;++i) {
            CHECK(xbox_fiber_wake_count(0x5555)==1);
            CHECK(xbox_fiber_run_thread(0xBEEF9003));
            xbox_fiber_switch_direct(ids[0]);
        }
        if(!replay) { memcpy(ram_after,memory,sizeof(memory)); CHECK(xbox_fiber_converted_capture(check,bytes)==2); }
        else {
            CHECK(!memcmp(ram_after,memory,sizeof(memory)));
            void *result=malloc(bytes); CHECK(result);
            CHECK(xbox_fiber_converted_capture(result,bytes)==2 && !memcmp(result,check,bytes)); free(result);
        }
    }
    CHECK(xbox_fiber_wake_count(0x5555)==1);
    CHECK(xbox_fiber_converted_restore(saved,bytes)==2); // wake state also rewound
    CHECK(!xbox_fiber_run_thread(0xBEEF9003));
    CHECK(xbox_fiber_wake_count(0x5555)==1);
    stream_nested=1;
    CHECK(xbox_fiber_run_thread(0xBEEF9003)); // now suspended inside its helper
    CHECK(!xbox_fiber_replay_thread_allowed(0xBEEF9003,1));
    CHECK(!xbox_fiber_converted_capture(check,bytes));
    CHECK(xbox_fiber_run_thread(0xBEEF9003)); // helper unwinds and reaches explicit wait
    CHECK(!xbox_fiber_converted_restore(saved,bytes));
    CHECK(xbox_fiber_converted_capture(check,bytes)==2);
    /* File worker parks on its own handle and wakes through directed resume. */
    CHECK(xbox_fiber_spawn(0x18C690,0x1927D0,0,0xBEEF9004,0));
    CHECK(xbox_fiber_run_thread(0xBEEF9004));
    CHECK(xbox_fiber_converted_capture(saved,bytes)==3);
    CHECK(xbox_fiber_replay_thread_allowed(0xBEEF9004,1));
    memcpy(ram_snapshot,memory,sizeof(memory));
    for(int replay=0;replay<3;++replay) {
        CHECK(xbox_fiber_converted_restore(saved,bytes)==3);
        memcpy(memory,ram_snapshot,sizeof(memory));
        for(int i=0;i<30;++i) {
            CHECK(xbox_fiber_run_thread(0xBEEF9004));
            CHECK(xbox_fiber_wake_count(0x5555)==1);
            CHECK(xbox_fiber_run_thread(0xBEEF9003));
            xbox_fiber_switch_direct(ids[0]);
        }
        if(!replay) { memcpy(ram_after,memory,sizeof(memory)); CHECK(xbox_fiber_converted_capture(check,bytes)==3); }
        else {
            CHECK(!memcmp(ram_after,memory,sizeof(memory)));
            void *result=malloc(bytes); CHECK(result);
            CHECK(xbox_fiber_converted_capture(result,bytes)==3 && !memcmp(result,check,bytes));free(result);
        }
    }
    CHECK(xbox_fiber_wake_count(0xBEEF9004)==1);
    CHECK(xbox_fiber_converted_restore(saved,bytes)==3);
    CHECK(xbox_fiber_wake_count(0xBEEF9004)==1); // restored its own WAITING handle
    stream_nested=1; CHECK(xbox_fiber_run_thread(0xBEEF9004));
    CHECK(!xbox_fiber_converted_capture(check,bytes));
    CHECK(xbox_fiber_run_thread(0xBEEF9004));
    CHECK(!xbox_fiber_converted_restore(saved,bytes));
    CHECK(xbox_fiber_converted_capture(saved,bytes)==3);
    stream_exit=1; CHECK(xbox_fiber_run_thread(0xBEEF9004));
    CHECK(!xbox_fiber_thread_alive(0x1927D0) && !xbox_fiber_converted_restore(saved,bytes));
    CHECK(!xbox_fiber_converted_clock_ready());
    CHECK(xbox_fiber_wake_count(0x5555)==1);
    CHECK(!xbox_fiber_converted_clock_ready()); // READY alone is not a new native park
    stream_wait=0;
    CHECK(xbox_fiber_run_thread(0xBEEF9003));
    CHECK(xbox_fiber_converted_clock_ready());
    g_xbox_det_active=0;CHECK(!xbox_fiber_converted_clock_ready());g_xbox_det_active=1;
    free(ram_snapshot);
    free(ram_after);
    xbox_fiber_destroy(ids[0]);
    free(saved); free(check);
    puts("Production task/watchdog checkpoints replay correctly; native-worker advancement, wake/session changes, shutdown and nested yields rejected.");
    return 0;
}
