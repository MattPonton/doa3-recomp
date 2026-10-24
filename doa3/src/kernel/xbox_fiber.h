/*
 * Cooperative fiber scheduler for Xbox threads.
 *
 * The recompiler is single-threaded: the x86 registers are global variables
 * (g_eax, g_esp, ...). DOA3's CRI ADXF middleware, however, is built around
 * worker threads (it spawns I/O server threads that block on events and process
 * a file-load queue). To run that faithfully we give each Xbox thread its own
 * Windows fiber (own native C stack) and round-robin between them cooperatively.
 *
 * Each fiber owns the recompiler register-globals while it runs; the scheduler
 * saves/restores them across switches. Yield points are the Xbox blocking
 * primitives (KeWaitForSingleObject) and the game's busy-wait spins. Because the
 * recomp is otherwise single-threaded there are no data races to lock against —
 * only one fiber runs at a time and it yields at well-defined points.
 */
#ifndef XBOX_FIBER_H
#define XBOX_FIBER_H

#include <stdint.h>
#include <stddef.h>

/* LOCAL checkpoints of the opt-in explicit fight tasks. These include saved
 * guest registers and generated continuation locals, NOT guest RAM/stacks,
 * ordinary Windows fibers, worker state or external resources. Capture/restore
 * only from primary at a quiescent dispatcher boundary. A topology change or
 * a nested native yield, native worker/coroutine execution, or a worker wake
 * invalidates restoration. This guard detects advancement; it does not save
 * worker continuations or suppress workers. Whole-game rollback must
 * checkpoint the other state separately; these APIs cannot rewind a match alone. */
size_t xbox_fiber_task_snapshot_size(void);
int xbox_fiber_task_capture(void *out, size_t bytes);
int xbox_fiber_task_restore(const void *saved, size_t bytes);

/* Combined LOCAL checkpoint of converted tasks and opt-in 3.1 workers:
 * watchdog, two vblank workers and the file worker at its self-suspend wait.
 * Requires deterministic-clock ownership and a primary dispatcher boundary.
 * Returns total converted fibers. Includes worker rotation/idle scheduling;
 * caller still owns RAM, clocks, kernel objects, APU and file state. Unconverted
 * worker execution/wakes, nested native worker waits and topology changes
 * invalidate the whole checkpoint.
 * Validates everything before changing any fiber. Never serializes a C stack. */
size_t xbox_fiber_converted_snapshot_size(void);
int xbox_fiber_converted_capture(void *out,size_t bytes);
int xbox_fiber_converted_restore(const void *saved,size_t bytes);
int xbox_fiber_converted_validate(const void *saved,size_t bytes); /* no mutation */
int xbox_fiber_run_restorable_watchdog(void); /* DOA3_ROLLBACK_WORKERS=1 opt-in */
int xbox_fiber_run_restorable_stream_worker(uint32_t routine); /* DOA3_ROLLBACK_STREAM_WORKERS=1 */
/* Read-only handoff check: live vblank workers must be converted and parked
 * at their deterministic yield, not their former wall-clock vblank block. */
int xbox_fiber_converted_clock_ready(void);
/* Admission only: named thread operations may affect converted workers;
 * native self-suspend and coroutine yield paths are excluded. */
int xbox_fiber_replay_thread_allowed(uint32_t handle,int resume);

/* Convert the current (main) OS thread into the primary fiber. Call once at
 * startup, before running the recompiled entry point. */
void xbox_fiber_init(void);

/* True once xbox_fiber_init() has run (so callers can no-op before then). */
int  xbox_fiber_active(void);

/* Create a cooperative fiber for an Xbox worker thread. When first scheduled it
 * runs start_routine(ctx1, ctx2) on its own freshly allocated Xbox stack. Does
 * NOT run immediately. xhandle is the unique fake NT thread handle for this
 * thread (join/suspend/resume target it); if suspended, the fiber starts
 * parked on its own handle until NtResumeThread wakes it.
 * Returns 1 on success, 0 on failure. */
int  xbox_fiber_spawn(uint32_t start_routine, uint32_t ctx1, uint32_t ctx2,
                      uint32_t xhandle, int suspended);

/* Cooperative scheduling point: save our register-globals, switch to the next
 * runnable fiber, and resume here when scheduled again. No-op if no other
 * runnable fiber exists. */
void xbox_fiber_yield(void);

/* Block the current fiber until xbox_fiber_wake(event_va) is called. Marks it
 * WAITING on event_va, switches to another runnable fiber, and resumes here only
 * after it is woken. Models the real game's KeWaitForSingleObject on a CRI event. */
void xbox_fiber_block(uint32_t event_va);
/* Key worker fibers park on to wait for the next (59.94 Hz) vertical blank. */
#define XBOX_FIB_VBLANK_KEY 0xB1A4B1A4u
void xbox_fiber_vblank_tick(void);
void xbox_fiber_vblank_wait(void);   /* worker fibers: one pass per elapsed vblank */

/* Wake every fiber WAITING on event_va (mark them runnable). event_va == 0 wakes
 * all waiters. Models KeSetEvent / NtSetEvent. */
void xbox_fiber_wake(uint32_t event_va);
/* Same, but returns how many fibers were woken (0 = the wake found no waiter). */
int  xbox_fiber_wake_count(uint32_t event_va);

/* End the currently running worker fiber (Xbox thread terminated). Marks it done
 * and switches to another runnable fiber; never returns. No-op (returns normally)
 * if called on the primary fiber or before fibers are active. */
void xbox_fiber_exit(void);

/* True if the currently running fiber is the primary (the game's main thread). */
int  xbox_fiber_is_primary(void);
int  xbox_fiber_is_coroutine(void);   /* running fiber is a game-task coroutine */
int  xbox_fiber_run_thread(uint32_t xhandle);
int  xbox_fiber_thread_alive(uint32_t ctx1);
void xbox_fiber_timeslice(void);
int  xbox_fiber_workers_ready(void);   /* a worker fiber is READY */
void xbox_fiber_run_workers_idle(int cap);  /* yield until no worker is READY */
extern volatile int g_fib_slice_due;   /* set every 4 ms by a host timer; consumed by xbox_fiber_timeslice */
int  xbox_fiber_any_ready(void);
void xbox_fiber_dump_states(void);


/* ── Direct-switched (XAPI CreateFiber/SwitchToFiber) coroutine support ──
 * DOA3's game tasks are XAPI fibers switched explicitly (symmetric coroutines),
 * not scheduled threads. Dormant fibers are excluded from the round-robin
 * scheduler; they run only via xbox_fiber_switch_direct and park again when
 * they switch away. */

/* Create a dormant fiber that will run routine_va(param) when first switched to.
 * Returns the fiber index (>0) or -1. */
int  xbox_fiber_create_dormant(uint32_t routine_va, uint32_t param,
                               uint32_t stack_size);

/* Destroy a dormant fiber created by xbox_fiber_create_dormant. */
void xbox_fiber_destroy(int idx);

/* Switch directly to the given fiber (parking the current one) and resume here
 * when something switches back. */
void xbox_fiber_switch_direct(int idx);

/* From a coroutine: switch back to whoever direct-switched into it. */
void xbox_fiber_yield_back(void);

/* Called at a supported guest task body, inside its normal entry wrapper.
 * Returns zero when disabled; enabled infinite tasks park inside this call. */
int xbox_fiber_run_restorable_task(uint32_t routine);

/* Index of the currently running fiber (0 = primary). */
int  xbox_fiber_current(void);
uint32_t xbox_fiber_current_ctx1(void);
uint32_t xbox_fiber_current_xhandle(void); /* the running fiber's NT thread handle (0 = none) */   /* start context of the running fiber (0 = primary) */

/* Number of live (non-finished) fibers, including the primary. */
int  xbox_fiber_count(void);

#endif /* XBOX_FIBER_H */
