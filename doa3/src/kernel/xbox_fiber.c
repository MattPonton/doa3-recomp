/* Cooperative fiber scheduler — see xbox_fiber.h. */
#include "xbox_fiber.h"
#include "xbox_det.h"
#include "rollback_tasks.h"
#include "rollback_workers.h"
#include "rollback_capture.h"
#include "xbox_memory_layout.h"          /* xbox_HeapAlloc */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Recompiler symbols (declared locally to avoid pulling the heavy recomp
 * headers into the kernel library). */
extern uint32_t g_eax, g_ecx, g_edx, g_esp;
extern uint32_t g_ebx, g_esi, g_edi;
extern uint32_t g_seh_ebp;
extern ptrdiff_t g_xbox_mem_offset;

typedef void (*recomp_func_t)(void);
extern recomp_func_t recomp_lookup(uint32_t xbox_va);
extern recomp_func_t recomp_lookup_manual(uint32_t xbox_va);

#define FMEM32(va) (*(volatile uint32_t *)((uintptr_t)(va) + g_xbox_mem_offset))

#define MAX_FIBERS         64
#define WORKER_STACK_SIZE  0x40000      /* 256 KB Xbox stack per worker */
#define WORKER_NATIVE_STK  (8u << 20)   /* 8 MB native fiber stack */
#define CORO_STACK_SIZE    0x20000      /* fallback when the caller asks for nothing */
/* Floor for a game-task Xbox stack. DOA3 asks CreateFiber for 0x4000 (16 KB)
 * per task; this doubles that for headroom and is still 4x less than the
 * 128 KB we used to force. The old fixed 128 KB cost 2.6 MB across the 21
 * title-screen tasks and ran the 49 MB guest heap dry (97.9% used), which is
 * what killed the process after the movie. */
#define CORO_STACK_MIN     0x8000
#define CORO_NATIVE_STK    (24u << 20)  /* 24 MB native stack per coroutine (recomp
                                         * turns each Xbox call into a native call, so
                                         * the game's deep boot-screen call trees need
                                         * a large C stack — matches the main thread) */

typedef enum { FIB_FREE = 0, FIB_READY, FIB_RUNNING, FIB_WAITING, FIB_DONE,
               FIB_DORMANT /* direct-switched coroutine, parked */ } FibState;

typedef struct {
    LPVOID   handle;        /* Windows fiber */
    FibState state;
    uint32_t start_routine; /* Xbox VA (worker entry) */
    uint32_t ctx1, ctx2;
    uint32_t stack_top;     /* Xbox VA of this thread's stack top */
    uint32_t stack_size;    /* bytes allocated for it (recycle only if big enough) */
    uint32_t wait_event;    /* Xbox VA of the event this fiber is blocked on (FIB_WAITING) */
    uint32_t xhandle;       /* unique fake NT thread handle (0xBEEFxxxx) — waits/wakes
                             * on thread handles (join, suspend/resume) must target ONE
                             * thread; a shared handle deadlocked the Sofdec player. */
    int      is_coroutine;  /* direct-switched (XAPI fiber), never round-robin scheduled */
    int      resume_to;     /* fiber to park back into when this one finishes/yields */
    int      restorable_task;
    uint32_t task_routine;  /* actual body inside the shared guest task wrapper */
    int      task_in_step;
    unsigned task_nested_yields;
    int restorable_worker, worker_parked;
    unsigned worker_in_step,worker_nested_yields;
    Doa3RollbackWorker worker;
    Doa3RollbackTask task;
    /* saved recompiler register-globals */
    uint32_t r_eax, r_ecx, r_edx, r_esp, r_ebx, r_esi, r_edi, r_sehebp;
    /* saved x87 FPU state (g_fp_stack/g_fp_top are process globals) */
    double   r_fp_stack[8];
    int      r_fp_top;
} Fiber;

static Fiber g_fib[MAX_FIBERS];
static unsigned g_fib_runs[MAX_FIBERS];   /* times each fiber was resumed (diagnostic) */
static uint64_t g_fib_topology;
/* A task checkpoint cannot roll back a native worker's C continuation. Even
 * returning to the same wait key after one stream read invalidates it. */
static uint64_t g_external_epoch;
static uint64_t g_worker_epoch; /* invalidates task-only checkpoints */
static int g_worker_rr;
static unsigned g_idle_yields;
unsigned g_rollback_worker_steps; /* diagnostic only; never rewound */
unsigned g_rollback_stream_worker_steps[3],g_rollback_stream_worker_nested_yields[3];
/* Exported diagnostic counters, read-only to probes. No capture is automatic. */
unsigned g_rollback_task_steps[DOA3_RB_TASK_COUNT];
static int   g_nfib   = 0;
static int   g_cur    = 0;
static int   g_active = 0;
/* Directed handoff (xbox_fiber_run_thread): when >= 0, the next time the
 * running fiber parks (yield/block/exit) control returns STRAIGHT to this
 * fiber instead of round-robin. This gives NtResumeThread its hardware
 * semantics ("the resumed thread runs now") without letting every other
 * fiber take a turn mid-caller — general yields from the main fiber inside
 * the server pump re-entered CRI code and broke playback (bisected). */
static int   g_direct_return = -1;

static void note_external_change(int index)
{
    if (!index || g_fib[index].restorable_task) return;
    if (g_fib[index].restorable_worker) ++g_worker_epoch;
    else ++g_external_epoch;
}

extern double g_fp_stack[8];
extern int    g_fp_top;

static void save_regs(Fiber *f)
{
    if (f->restorable_worker && f->worker_in_step && !f->worker_nested_yields) {
        f->worker_nested_yields=1;
        ++g_fib_topology;
        unsigned slot=f->ctx1==0x1927D0 ? 2u : f->ctx1==0x192760 ? 1u : 0u;
        if (++g_rollback_stream_worker_nested_yields[slot]<=4)
            fprintf(stderr,"[ROLLBACK] worker %08X yielded inside a native helper; prior checkpoints invalidated\n",f->ctx1);
        doa3_rollback_capture_cancel();
    }
    /* Catch every scheduler handoff, including direct switches and waits,
     * not just the expected frame-yield API. A nested C stack cannot rewind. */
    if (f->restorable_task && f->task_in_step && !f->task_nested_yields) {
        f->task_nested_yields = 1;
        ++g_fib_topology; /* invalidate checkpoints made before this native handoff */
        doa3_rollback_capture_cancel();
        fprintf(stderr, "[ROLLBACK] task %08X yielded from a nested native call; prior checkpoints invalidated, capture cancelled if active; fresh checkpoints require native step return\n", f->task_routine);
    }
    f->r_eax = g_eax; f->r_ecx = g_ecx; f->r_edx = g_edx; f->r_esp = g_esp;
    f->r_ebx = g_ebx; f->r_esi = g_esi; f->r_edi = g_edi; f->r_sehebp = g_seh_ebp;
    memcpy(f->r_fp_stack, g_fp_stack, sizeof(g_fp_stack));
    f->r_fp_top = g_fp_top;
}
static void load_regs(Fiber *f)
{
    note_external_change((int)(f-g_fib));
    g_eax = f->r_eax; g_ecx = f->r_ecx; g_edx = f->r_edx; g_esp = f->r_esp;
    g_ebx = f->r_ebx; g_esi = f->r_esi; g_edi = f->r_edi; g_seh_ebp = f->r_sehebp;
    memcpy(g_fp_stack, f->r_fp_stack, sizeof(g_fp_stack));
    g_fp_top = f->r_fp_top;
}

/* Next runnable (READY) fiber after `from`, round-robin. -1 if none. */
/* Post-movie only: the movie phase relies on the older round-robin resumption of
 * game tasks (a task waits for the movie by yielding, and the primary must run
 * in between); its verified timing is left as it was. */
extern volatile int g_doa3_post_movie;
static int pick_next(int from)
{
    for (int k = 1; k <= g_nfib; k++) {
        int i = (from + k) % g_nfib;
        /* XAPI fibers (game tasks) resume only when the game switches to them,
         * never by round-robin: two runnable tasks were being resumed in index
         * order after a vblank wait, so the render record builder and the
         * walker interleaved in a way the game never does and the walker spun
         * on a half-rebuilt list (the intro-scene freeze). */
        if (g_fib[i].is_coroutine && g_doa3_post_movie) continue;
        if (g_fib[i].state == FIB_READY) return i;
    }
    return -1;
}

static void CALLBACK fiber_trampoline(LPVOID p)
{
    Fiber *f = (Fiber *)p;
    {   /* Each fiber has its own stack guarantee (saved per fiber), and new
         * fibers start with none: keep 64 KB so a stack overflow on a worker
         * or coroutine reaches the exception handlers and gets logged. */
        ULONG guarantee = 64 * 1024;
        SetThreadStackGuarantee(&guarantee);
    }
    /* Set up the Xbox stack exactly like PsCreateSystemThreadEx does for the
     * main thread: push ctx2, ctx1, and a dummy return address, then call the
     * start routine. */
    g_esp = f->stack_top;
    g_esp -= 4; FMEM32(g_esp) = f->ctx2;
    g_esp -= 4; FMEM32(g_esp) = f->ctx1;
    g_esp -= 4; FMEM32(g_esp) = 0;            /* dummy return address */

    recomp_func_t fn = recomp_lookup_manual(f->start_routine);
    if (!fn) fn = recomp_lookup(f->start_routine);
    if (fn) fn();

    /* The start routine returned → this fiber has exited. */
    f->state = FIB_DONE;

    if (f->is_coroutine) {
        /* Direct-switched coroutine: hand control back to whoever ran us. */
        for (;;) {
            int r = f->resume_to;
            if (r < 0 || r >= g_nfib || g_fib[r].state == FIB_FREE || g_fib[r].state == FIB_DONE)
                r = 0;
            g_cur = r;
            if (g_fib[r].state != FIB_WAITING) g_fib[r].state = FIB_RUNNING;
            load_regs(&g_fib[r]);
            SwitchToFiber(g_fib[r].handle);
        }
    }

    fprintf(stderr, "[FIBER] thread 0x%08X exited\n", f->start_routine);
    fflush(stderr);

    /* Hand control to another runnable fiber (or the primary). We never resume. */
    for (;;) {
        int n = pick_next(g_cur);
        if (n < 0) n = 0;                     /* fall back to the primary fiber */
        g_cur = n; g_fib[n].state = FIB_RUNNING; load_regs(&g_fib[n]);
        SwitchToFiber(g_fib[n].handle);
    }
}

void xbox_fiber_init(void)
{
    if (g_active) return;
    LPVOID prim = ConvertThreadToFiber(NULL);
    if (!prim) prim = GetCurrentFiber();      /* already a fiber */
    memset(g_fib, 0, sizeof(g_fib));
    g_fib[0].handle = prim;
    g_fib[0].state  = FIB_RUNNING;
    g_nfib = 1;
    g_cur  = 0;
    g_active = 1;
    xbox_fiber_timeslice();      /* starts the 4 ms slice timer (nothing is due yet) */
    fprintf(stderr, "[FIBER] primary fiber initialized\n");
    fflush(stderr);
}

int xbox_fiber_active(void) { return g_active; }

/* Run the workers until none is ready (at most `cap` laps). Netplay
 * sessions use this wherever the game gives its workers time, so how much
 * work they finish never depends on how much was queued. */
void xbox_fiber_run_workers_idle(int cap)
{
    extern int xbox_fiber_workers_ready(void);
    xbox_fiber_yield();
    while (--cap > 0 && xbox_fiber_workers_ready()) xbox_fiber_yield();
}

/* Any worker (not the primary, not a game-task coroutine) ready to run. */
int xbox_fiber_workers_ready(void)
{
    int i;
    for (i = 1; i < g_nfib; i++) {
        if (g_fib[i].is_coroutine && g_doa3_post_movie) continue;
        /* The CRI watchdog (sub_0016A530) is a pure yield loop and is always
         * READY; counting it meant "until idle" always ran to its cap, and
         * within that budget the file server's progress on a load depended
         * on how much music streaming was queued ahead of it. */
#if defined(DOA3_XBE_ID_3_0)
        if (g_fib[i].ctx1 == 0x0016A530u) continue;
#elif defined(DOA3_XBE_ID_3_1)
        if (g_fib[i].ctx1 == 0x001926B0u) continue;
#endif
        if (g_fib[i].state == FIB_READY) return 1;
    }
    return 0;
}

int xbox_fiber_spawn(uint32_t start_routine, uint32_t ctx1, uint32_t ctx2,
                     uint32_t xhandle, int suspended)
{
    if (!g_active || g_nfib >= MAX_FIBERS) return 0;
    int i = g_nfib;
    Fiber *f = &g_fib[i];
    memset(f, 0, sizeof(*f));
    f->handle = CreateFiber(WORKER_NATIVE_STK, fiber_trampoline, f);
    if (!f->handle) {
        fprintf(stderr, "[FIBER] CreateFiber failed for 0x%08X\n", start_routine);
        return 0;
    }
    uint32_t base = xbox_HeapAllocHigh(WORKER_STACK_SIZE, 16); /* DOA3: CPU-only, keep the GPU-addressable low heap for the game */
    f->start_routine = start_routine;
    f->ctx1 = ctx1; f->ctx2 = ctx2;
    f->stack_top = base + WORKER_STACK_SIZE - 16;
    f->stack_size = WORKER_STACK_SIZE;
    f->xhandle = xhandle;
    /* NOTE: CreateSuspended is intentionally IGNORED (fiber starts READY).
     * Honoring it parked the CRI ADX workers and the whole boot pipeline
     * (partition loads) deadlocked behind their handshake; the workers
     * self-pace via NtSuspendThread once running, which works. */
    (void)suspended;
    f->state = FIB_READY;
    g_nfib++;
    ++g_fib_topology;
    fprintf(stderr, "[FIBER] spawned #%d routine=0x%08X ctx1=0x%08X stack_top=0x%08X handle=0x%08X%s\n",
            i, start_routine, ctx1, f->stack_top, xhandle, suspended ? " (suspended)" : "");
    fflush(stderr);
    return 1;
}

/* Hardware-paced vertical blank for worker threads (non-3.0 builds; see the
 * vblank branch of bridge_KeWaitForSingleObject). Worker fibers that wait for
 * the D3D vblank event park on XBOX_FIB_VBLANK_KEY; this releases them once
 * per 1/60 s of wall time. It runs at every scheduling decision, so it needs
 * no thread of its own. */
void xbox_fiber_vblank_tick(void)
{
    static LONGLONG s_qpf, s_next;
    LARGE_INTEGER now;
    if (!s_qpf) { LARGE_INTEGER f; QueryPerformanceFrequency(&f); s_qpf = f.QuadPart; }
    QueryPerformanceCounter(&now);
    if (now.QuadPart < s_next) return;
    if (!s_next || now.QuadPart - s_next > s_qpf / 10) s_next = now.QuadPart;
    s_next += s_qpf * 1001 / 60000;                 /* 59.94 Hz */
    xbox_fiber_wake(XBOX_FIB_VBLANK_KEY);
}

void xbox_fiber_yield(void)
{
    if (!g_active) return;
    xbox_fiber_vblank_tick();
    int me = g_cur;
    int n = -1;
    if (g_direct_return >= 0 && g_direct_return != me) {
        n = g_direct_return;                  /* directed handoff: return to the caller */
        g_direct_return = -1;
        g_fib[n].state = FIB_READY;
    } else if (g_fib[me].is_coroutine && g_doa3_post_movie) {
        /* Rotate through the workers. Scanning from `me` always lands on the
         * first READY worker after the task -- the CRI watchdog (#1) -- so a
         * task spinning on a CRI handshake (sub_0016A4C0, waiting for the
         * server #4 to clear 0xB24D3C) ping-ponged with the watchdog forever
         * and the server never ran: the story-mode pre-FMV freeze (hang dump
         * 2026-10-01). */
        n = pick_next(g_worker_rr);
        if (n >= 0) g_worker_rr = n;
    } else {
        n = pick_next(me);
    }
    if (g_fib[me].is_coroutine && g_doa3_post_movie) {
        /* A game task yielding (vblank wait, sleep): let one worker run and
         * come straight back here -- the task is not schedulable by anyone
         * else (see pick_next). */
        if (n < 0) return;
        g_direct_return = me;
    }
    if (n < 0) {
        /* Nobody else is READY.
         *
         * If fibers are parked on events this is a circular wait: whoever
         * would signal them cannot run, because we are the only runnable
         * fiber and we merely spin. xbox_fiber_block() already breaks that
         * by releasing every waiter to re-check its own condition, but a
         * pure yield-loop never reaches that path -- and the CRI watchdog
         * override (sub_0016A530) is exactly such a loop. Post-movie the
         * whole game parked behind it: the primary fiber and two CRI
         * workers sat on event 0x260830 while the watchdog span forever.
         *
         * Apply the same release, but only after enough fruitless yields
         * that this cannot be ordinary scheduling. */
        int waiters = 0, i;
        for (i = 0; i < g_nfib; i++)
            if (g_fib[i].state == FIB_WAITING) waiters++;
        if (!waiters) { g_idle_yields = 0; return; }
        if (++g_idle_yields < 200000u) return;
        g_idle_yields = 0;
        for (i = 0; i < g_nfib; i++)
            if (g_fib[i].state == FIB_WAITING) {
                note_external_change(i);
                g_fib[i].state = FIB_READY; g_fib[i].wait_event = 0;
            }
        {   static int logged;
            if (logged < 8) { logged++;
                fprintf(stderr, "[FIBER] yield-spin deadlock: released %d waiter(s)\n",
                        waiters);
                fflush(stderr); } }
        n = pick_next(me);
        if (n < 0) return;
    }

    save_regs(&g_fib[me]);
    if (g_fib[me].state == FIB_RUNNING) g_fib[me].state = FIB_READY;
    g_cur = n; g_fib[n].state = FIB_RUNNING; load_regs(&g_fib[n]);
    SwitchToFiber(g_fib[n].handle);
    /* Resumed: the switcher already restored our register-globals. */
    g_cur = me; g_fib[me].state = FIB_RUNNING; g_fib_runs[g_cur]++;
}

/* Time-based scheduling point for the worker threads.
 *
 * The scheduler is cooperative, so the CRI worker threads (vblank-paced ADX
 * and file servers, and the I/O completion worker) only run when the game
 * thread parks. After the movie the game loop never blocks in the kernel,
 * and while it loads a scene it does not present either, so the workers ran
 * only when a deadlock breaker fired: the music stream got one file read
 * every few seconds and the voice replayed its 341 ms ring buffer in between
 * (measured: prim_yields=1..3 per 2 s, writes in 2 s bursts). On hardware
 * those threads are woken by the vblank interrupt regardless of what the
 * game thread is doing. Model that with a periodic lap through the READY
 * fibers from the primary, taken from the kernel-call path (which the game
 * thread hits constantly) but never while it is inside the CRI server pump
 * or holds the CRI lock, so no server is re-entered. */
volatile int g_fib_slice_due = 0;
static DWORD WINAPI fib_slice_timer(LPVOID p)
{
    (void)p;
    /* No effect while a netplay session schedules the slices (xbox_det.c). */
    for (;;) { Sleep(4); xbox_det_host_slice_timer(); }
}
void xbox_fiber_timeslice(void)
{
    extern int doa3_workers_may_run(void);
    static HANDLE s_timer;
    extern unsigned char xbox_current_irql(void);
    if (!g_active || g_cur != 0) return;
    if (!s_timer) s_timer = CreateThread(NULL, 0, fib_slice_timer, NULL, 0, NULL);
    if (!g_fib_slice_due) return;
    if (xbox_current_irql() >= 2) return;   /* no thread switch at DISPATCH_LEVEL */
    if (!doa3_workers_may_run()) return;
    g_fib_slice_due = 0;
    {   /* One lap per 4 ms tick that elapsed since the last slice, capped.
         * While a scene loads the game thread sits in host D3D11 calls for
         * tens of ms at a time (no hook can run there); a timer-driven
         * thread would have run once per tick in the meantime, so catch up
         * on the missed ticks instead of granting a single lap. */
        static LARGE_INTEGER s_last, s_freq;
        LARGE_INTEGER now; int laps = 1;
        if (!s_freq.QuadPart) QueryPerformanceFrequency(&s_freq);
        QueryPerformanceCounter(&now);
        if (s_last.QuadPart) {
            long long ticks /* ms; a worker iteration is ~4 laps */ = (now.QuadPart - s_last.QuadPart) * 1000 / s_freq.QuadPart;
            if (ticks > laps) laps = (int)(ticks > 64 ? 64 : ticks);
        }
        s_last = now;
        /* Netplay session: a fixed lap budget per lockstep frame, never the
         * wall clock, so both machines run their workers identically. */
        if (g_xbox_det_active) {
            /* Then keep going until every worker has blocked: whatever file
             * or decode work is pending finishes inside this frame, so a
             * load completes on the same frame however much music streaming
             * was queued ahead of it (that amount depends on where the menu
             * music was when the session started). */
            laps = xbox_det_slice_laps();
            while (laps-- > 0) xbox_fiber_yield();
            xbox_fiber_run_workers_idle(1024);
            laps = 0;
        }
        while (laps-- > 0) xbox_fiber_yield();
    }
}

void xbox_fiber_block(uint32_t event_va)
{
    if (!g_active) return;
    xbox_fiber_vblank_tick();
    int me = g_cur;
    g_fib[me].wait_event = event_va;
    g_fib[me].state = FIB_WAITING;
    save_regs(&g_fib[me]);

    int n = -1;
    if (g_direct_return >= 0 && g_direct_return != me) {
        n = g_direct_return;                  /* directed handoff: return to the caller */
        g_direct_return = -1;
        g_fib[n].state = FIB_READY;
    } else {
        n = pick_next(me);
    }
    if (n < 0 && g_xbox_det_active) { xbox_fiber_wake(XBOX_DET_VBLANK_KEY); n = pick_next(me); }
    if (n < 0) {
        /* Nobody runnable. Wake all waiters to avoid a hard deadlock; they will
         * re-check their own condition and block again if still unsatisfied. */
        for (int i = 0; i < g_nfib; i++)
            if (g_fib[i].state == FIB_WAITING) {
                note_external_change(i);
                g_fib[i].state = FIB_READY; g_fib[i].wait_event = 0;
            }
        n = pick_next(me);
        if (n < 0) { g_fib[me].state = FIB_RUNNING; g_fib[me].wait_event = 0; return; }
    }
    g_cur = n; g_fib[n].state = FIB_RUNNING; load_regs(&g_fib[n]);
    SwitchToFiber(g_fib[n].handle);
    /* Woken: the switcher restored our register-globals. */
    g_cur = me; g_fib[me].state = FIB_RUNNING; g_fib[me].wait_event = 0; g_fib_runs[g_cur]++;
}

void xbox_fiber_wake(uint32_t event_va)
{
    (void)xbox_fiber_wake_count(event_va);
}

/* Like xbox_fiber_wake, but reports how many fibers were actually woken (the
 * NtResumeThread bridge uses 0 == "this resume would be lost" — see
 * xbox_fiber_credit_resume). */
int xbox_fiber_wake_count(uint32_t event_va)
{
    int n = 0;
    if (!g_active) return 0;
    for (int i = 0; i < g_nfib; i++)
        if (g_fib[i].state == FIB_WAITING && (event_va == 0 || g_fib[i].wait_event == event_va)) {
            note_external_change(i);
            g_fib[i].state = FIB_READY;
            g_fib[i].wait_event = 0;
            n++;
        }
    return n;
}

void xbox_fiber_exit(void)
{
    if (!g_active) return;
    int me = g_cur;
    if (me == 0) return;                      /* primary thread: let it return normally */
    g_fib[me].state = FIB_DONE;
    fprintf(stderr, "[FIBER] fiber #%d (0x%08X) terminated\n", me, g_fib[me].start_routine);
    fflush(stderr);
    /* Signal the thread object: wake any fiber joined on our handle. */
    if (g_fib[me].xhandle)
        xbox_fiber_wake(g_fib[me].xhandle);
    int n = -1;
    if (g_direct_return >= 0 && g_direct_return != me) {
        n = g_direct_return;
        g_direct_return = -1;
        g_fib[n].state = FIB_READY;
    } else {
        n = pick_next(me);
    }
    if (n < 0) n = 0;                         /* fall back to the primary fiber */
    g_cur = n; g_fib[n].state = FIB_RUNNING; load_regs(&g_fib[n]);
    SwitchToFiber(g_fib[n].handle);
    /* never reached */
}

/* NOTE (item 100): "resume credits" (mark a READY/RUNNING fiber so its next
 * self-suspend re-checks instead of parking, closing the cooperative model's
 * enlarged resume-vs-park race) were implemented here TWICE (unconditional,
 * then only-when-wake-found-nobody) and BOTH collapsed playback: the ADXM
 * protocol legitimately retries resumes against a running server hundreds of
 * times per frame, so any credit keeps the sfdec servers from ever parking
 * and one worker monopolizes the scheduler. Lost resumes are healed by those
 * same protocol retries; the one case where retries stop (main parked in the
 * mount poll) is covered by the gated [MOUNTNUDGE] spurious wake there. */

/* Directed resume: run the parked fiber(s) matching this thread handle NOW,
 * returning to the caller as soon as the target parks again. Falls back to a
 * plain wake when no matching fiber is parked. Returns 1 if a switch
 * happened. Only intended for the primary fiber (NtResumeThread bridge). */
/* Is a spawned (non-coroutine) thread whose start context is ctx1 still
 * running? DOA3 joins exactly one such thread: its cache-install worker
 * (routine 0x0009D440), see bridge_NtWaitForSingleObject. */
int xbox_fiber_thread_alive(uint32_t ctx1)
{
    for (int i = 0; i < g_nfib; i++) {
        Fiber *f = &g_fib[i];
        if (f->is_coroutine) continue;
        if (f->ctx1 != ctx1) continue;
        if (f->state == FIB_FREE || f->state == FIB_DONE) continue;
        return 1;
    }
    return 0;
}

int xbox_fiber_run_thread(uint32_t xhandle)
{
    if (!g_active) return 0;
    int me = g_cur;
    for (int i = 0; i < g_nfib; i++) {
        if (i == me) continue;
        Fiber *f = &g_fib[i];
        if (f->is_coroutine) continue;
        if (f->state != FIB_READY &&
            !(f->state == FIB_WAITING &&
              (f->wait_event == xhandle || xhandle == 0)))
            continue;
        if (xhandle && f->xhandle != xhandle && f->wait_event != xhandle)
            continue;
        f->state = FIB_READY;
        f->wait_event = 0;
        save_regs(&g_fib[me]);
        if (g_fib[me].state == FIB_RUNNING) g_fib[me].state = FIB_READY;
        g_direct_return = me;
        g_cur = i; f->state = FIB_RUNNING; load_regs(f);
        SwitchToFiber(f->handle);
        /* target parked; the direct-return path switched back to us */
        g_cur = me; g_fib[me].state = FIB_RUNNING; g_fib_runs[g_cur]++;
        g_direct_return = -1;
        return 1;
    }
    return 0;
}

/* Diagnostic: one-line-per-fiber state dump (for lock-spin forensics). */
void xbox_fiber_dump_states(void)
{
    fprintf(stderr, "[FIBSTATE] cur=%d n=%d:", g_cur, g_nfib);
    for (int i = 0; i < g_nfib; i++)
        fprintf(stderr, " #%d{st=%d rt=%X wait=%X co=%d}",
                i, (int)g_fib[i].state, g_fib[i].start_routine,
                g_fib[i].wait_event, g_fib[i].is_coroutine);
    fprintf(stderr, "%c", 10);
    fflush(stderr);
}

/* Any non-coroutine fiber ready to run? (decode-budget loop exit test) */
int xbox_fiber_any_ready(void)
{
    if (!g_active) return 0;
    for (int i = 0; i < g_nfib; i++)
        if (i != g_cur && g_fib[i].state == FIB_READY && !g_fib[i].is_coroutine)
            return 1;
    return 0;
}

int xbox_fiber_is_primary(void)
{
    return g_active && g_cur == 0;
}

uint32_t xbox_fiber_current_ctx1(void) { return g_active ? g_fib[g_cur].ctx1 : 0; }
uint32_t xbox_fiber_current_xhandle(void) { return g_active ? g_fib[g_cur].xhandle : 0; }

int xbox_fiber_current(void)
{
    return g_cur;
}

/* True if the current fiber is a direct-switched coroutine (XAPI game task). */
int xbox_fiber_is_coroutine(void)
{
    return g_active && g_fib[g_cur].is_coroutine;
}

/* ── Direct-switched coroutines (XAPI CreateFiber/SwitchToFiber backing) ── */

int xbox_fiber_create_dormant(uint32_t routine_va, uint32_t param,
                              uint32_t stack_size)
{
    if (!g_active) return -1;
    int i;
    /* Reuse a free/finished slot (game tasks are created and deleted per screen). */
    for (i = 1; i < MAX_FIBERS; i++)
        if (i >= g_nfib || g_fib[i].state == FIB_FREE) break;
    if (i >= MAX_FIBERS) {
        fprintf(stderr, "[FIBER] coroutine pool exhausted\n");
        return -1;
    }
    Fiber *f = &g_fib[i];
    if (f->handle) { DeleteFiber(f->handle); f->handle = NULL; }
    uint32_t keep_stack = f->stack_top;    /* reuse the Xbox stack of a recycled slot */
    uint32_t keep_size  = f->stack_size;
    if (!stack_size) stack_size = CORO_STACK_SIZE;
    if (stack_size < CORO_STACK_MIN) stack_size = CORO_STACK_MIN;
    stack_size = (stack_size + 15u) & ~15u;
    /* A recycled slot's stack is only reusable if it is big enough. */
    if (keep_stack && keep_size < stack_size) { keep_stack = 0; keep_size = 0; }
    memset(f, 0, sizeof(*f));
    f->handle = CreateFiber(CORO_NATIVE_STK, fiber_trampoline, f);
    if (!f->handle) return -1;
    if (!keep_stack) {
        /* On failure xbox_HeapAlloc returns 0. The old code still computed
         * base + size - 16 from it, handing the fiber a stack pointer at
         * 0x1FFF0 -- inside the XBE image -- so the task silently shredded
         * the game's own code and the process died with no exception to
         * catch. Fail the create instead, loudly. */
        uint32_t base = xbox_HeapAllocHigh(stack_size, 16); /* DOA3: CPU-only stack -> high heap */
        if (!base) {
            fprintf(stderr, "[FIBER] OUT OF GUEST HEAP: cannot allocate %u-byte "
                            "Xbox stack for task routine 0x%08X (slot %d)\n",
                    stack_size, routine_va, i);
            fflush(stderr);
            DeleteFiber(f->handle); f->handle = NULL;
            f->state = FIB_FREE;
            return -1;
        }
        keep_stack = base + stack_size - 16;
        keep_size  = stack_size;
    }
    f->stack_top  = keep_stack;
    f->stack_size = keep_size;
    f->start_routine = routine_va;
    f->ctx1 = param;
    f->ctx2 = 0;
    f->is_coroutine = 1;
    f->resume_to = 0;
    f->state = FIB_DORMANT;
    ++g_fib_topology;
    if (i >= g_nfib) g_nfib = i + 1;
    return i;
}

void xbox_fiber_destroy(int idx)
{
    if (idx <= 0 || idx >= g_nfib) return;
    Fiber *f = &g_fib[idx];
    if (!f->is_coroutine) return;             /* never destroy scheduler threads */
    ++g_fib_topology;
    if (f->handle) { DeleteFiber(f->handle); f->handle = NULL; }
    f->state = FIB_FREE;                       /* stack_top kept for reuse */
    f->is_coroutine = 0;
}

/* A coroutine yielding back to whoever direct-switched into it (its resume_to).
 * No-op on the primary/scheduler fibers. */
void xbox_fiber_yield_back(void)
{
    if (!g_active) return;
    Fiber *me = &g_fib[g_cur];
    if (!me->is_coroutine) return;
    int r = me->resume_to;
    if (r < 0 || r >= g_nfib || !g_fib[r].handle ||
        g_fib[r].state == FIB_FREE || g_fib[r].state == FIB_DONE)
        r = 0;
    int self = g_cur;
    me->state = FIB_DORMANT;
    save_regs(me);
    g_cur = r;
    g_fib[r].state = FIB_RUNNING;
    load_regs(&g_fib[r]);
    SwitchToFiber(g_fib[r].handle);
    /* Resumed (switched back into this task). */
    g_cur = self; g_fib_runs[g_cur]++;
    g_fib[self].state = FIB_RUNNING;
}

void xbox_fiber_switch_direct(int idx)
{
    if (!g_active || idx < 0 || idx >= g_nfib || idx == g_cur) return;
    Fiber *t = &g_fib[idx];
    if (t->state == FIB_FREE || t->state == FIB_DONE || !t->handle) return;
    int me = g_cur;
    g_fib[me].state = FIB_DORMANT;             /* park; only a direct switch resumes us */
    save_regs(&g_fib[me]);
    t->resume_to = me;
    g_cur = idx;
    t->state = FIB_RUNNING;
    load_regs(t);
    SwitchToFiber(t->handle);
    /* Switched back: our registers were restored by whoever resumed us. */
    g_cur = me; g_fib_runs[g_cur]++;
    g_fib[me].state = FIB_RUNNING;
}

int xbox_fiber_run_restorable_task(uint32_t routine)
{
    if (!g_active || !g_fib[g_cur].is_coroutine ||
        !doa3_rollback_tasks_enabled() || !doa3_rollback_task_supported(routine)) return 0;
    Fiber *f = &g_fib[g_cur];
    f->restorable_task = 1;
    f->task_routine = routine;
    ++g_fib_topology;
    fprintf(stderr, "[ROLLBACK] task %08X uses an explicit continuation (fiber %d)\n", routine, g_cur);
    /* Enter through the actual task body, preserving the game's shared entry
     * wrapper. Every live body local is saved before parking here. */
    for (;;) {
        doa3_rollback_capture_before(routine, &f->task);
        f->task_in_step = 1;
        if (!doa3_rollback_task_step(routine, &f->task)) {
            fprintf(stderr, "[ROLLBACK] restorable task %08X did not yield normally\n", routine);
            abort();
        }

        f->task_in_step = 0;
        /* The generated step has now returned: no nested native stack remains.
         * Old checkpoints stay invalid through the topology epoch above. */
        f->task_nested_yields = 0;
        doa3_rollback_capture_after(routine, &f->task);
        { static const uint32_t routines[DOA3_RB_TASK_COUNT] = {DOA3_RB_TASK_ROUTINES};
          for (unsigned k = 0; k < DOA3_RB_TASK_COUNT; ++k)
              if (routine == routines[k]) ++g_rollback_task_steps[k]; }
        xbox_fiber_yield_back();
    }
}

int xbox_fiber_run_restorable_stream_worker(uint32_t routine)
{
    static int enabled=-1;
    if (enabled<0) {
        const char *value=getenv("DOA3_ROLLBACK_STREAM_WORKERS");
        enabled=value && !strcmp(value,"1") && doa3_rollback_tasks_enabled();
    }
    if (!enabled || !g_active || g_cur<=0 || !doa3_rollback_worker_supported(routine)) return 0;
    Fiber *f=&g_fib[g_cur];
    if (f->is_coroutine || f->start_routine!=0x18C690 || f->ctx1!=routine) return 0;
    f->restorable_worker=1; ++g_fib_topology;
    fprintf(stderr,"[ROLLBACK] streaming worker %08X uses an explicit outer wait (fiber %d)\n",routine,g_cur);
    for (;;) {
        f->worker_in_step=1;
        if (!doa3_rollback_worker_step(routine,&f->worker)) {
            fprintf(stderr,"[ROLLBACK] streaming worker %08X failed at its explicit wait\n",routine);
            abort();
        }
        f->worker_in_step=f->worker_nested_yields=0;
        ++g_rollback_stream_worker_steps[routine==0x1927D0 ? 2 : routine==0x192760 ? 1 : 0];
        f->worker_parked=1;
        if (f->worker.wait_key) xbox_fiber_block(f->worker.wait_key);
        else xbox_fiber_yield();
        f->worker_parked=0;
    }
}

int xbox_fiber_run_restorable_watchdog(void)
{
    static int enabled=-1;
    if (enabled<0) {
        const char *value=getenv("DOA3_ROLLBACK_WORKERS");
        enabled=value && !strcmp(value,"1") && doa3_rollback_tasks_enabled();
    }
    if (!enabled || !g_active || g_cur<=0) return 0;
    Fiber *f=&g_fib[g_cur];
    /* Only the audited 3.1 idle worker, inside its unchanged XAPI startup
     * wrapper. No native helper runs between its explicit yield boundaries. */
    if (f->is_coroutine || f->start_routine!=0x18C690 || f->ctx1!=0x1926B0) return 0;
    f->restorable_worker=1;
    ++g_fib_topology;
    fprintf(stderr,"[ROLLBACK] watchdog 001926B0 uses an explicit worker boundary (fiber %d)\n",g_cur);
    while (!FMEM32(0x40C718)) {
        FMEM32(0x40C700)=FMEM32(0x40C700)+1;
        ++g_rollback_worker_steps;
        /* The only C local that survives this park is f, whose immutable
         * identity is checked by restore. yield's post-switch path uses only
         * the immutable fiber index before returning here. */
        f->worker_parked=1;
        xbox_fiber_yield();
        f->worker_parked=0;
    }
    f->restorable_worker=0;
    ++g_fib_topology; /* the enclosing native startup/exit path may now unwind */
    FMEM32(0x40C71C)=1;
    g_esp+=4;
    return 1;
}

/* The opaque snapshot is process-local. Native fiber handles are identity
 * checks only, never restored or used as a serialized execution context. The
 * native stack is always parked in the same trampoline for these tasks. */
typedef struct {
    uint32_t version, count, workers, idle_yields, clock_epoch;
    int worker_rr;
    uint64_t topology, mask, external_epoch, worker_epoch;
    Fiber tasks[MAX_FIBERS];
} TaskSnapshot;

size_t xbox_fiber_task_snapshot_size(void) { return sizeof(TaskSnapshot); }
size_t xbox_fiber_converted_snapshot_size(void) { return sizeof(TaskSnapshot); }

static int checkpoint_ready(const Fiber *f)
{
    if (f->restorable_worker) {
        if (f->is_coroutine || f->restorable_task || f->worker_parked!=1 ||
            f->worker_in_step || f->worker_nested_yields || f->start_routine!=0x18C690) return 0;
        if (f->ctx1==0x1926B0) return f->state==FIB_READY && !f->wait_event;
        if (f->ctx1==0x1927D0 && f->worker.wait_key!=f->xhandle) return 0;
        return doa3_rollback_worker_checkpoint_valid(f->ctx1,&f->worker) && f->r_esp==f->worker.wait_sp &&
            ((f->state==FIB_READY && !f->wait_event) ||
             (f->state==FIB_WAITING && f->worker.wait_key && f->wait_event==f->worker.wait_key));
    }
    return f->restorable_task && f->state==FIB_DORMANT &&
        !f->task_in_step && !f->task_nested_yields &&
        doa3_rollback_task_checkpoint_valid(f->task_routine,&f->task);
}

static int task_snapshot_mask(uint64_t *mask,int workers)
{
    int count = 0;
    *mask = 0;
    if (!g_active || g_cur != 0) return 0;
    if (workers && (!g_xbox_det_active || g_direct_return!=-1)) return 0;
    for (int i = 1; i < g_nfib; ++i) {
        Fiber *f = &g_fib[i];
        if ((!f->restorable_task && !(workers && f->restorable_worker)) ||
            f->state == FIB_FREE || f->state == FIB_DONE) continue;
        if (!checkpoint_ready(f)) return 0;
        *mask |= UINT64_C(1) << i;
        ++count;
    }
    return count;
}

int xbox_fiber_converted_clock_ready(void)
{
    uint64_t mask;
    if(!task_snapshot_mask(&mask,1))return 0;
    unsigned vblank_workers=0;
    for(int i=1;i<g_nfib;++i){
        const Fiber *f=&g_fib[i];
        if(f->state==FIB_FREE || f->state==FIB_DONE || f->is_coroutine)continue;
        if(f->ctx1!=0x1926F0 && f->ctx1!=0x192760)continue;
        if(!f->restorable_worker || !checkpoint_ready(f) || f->worker.wait_key ||
           f->state!=FIB_READY || f->wait_event)return 0;
        ++vblank_workers;
    }
    return vblank_workers!=0;
}

int xbox_fiber_replay_thread_allowed(uint32_t handle,int resume)
{
    if(!g_active || !g_xbox_det_active || !handle || handle==UINT32_MAX ||
       handle==0xFFFFFFFEu || g_fib[g_cur].is_coroutine)return 0;
    if(g_cur && !g_fib[g_cur].restorable_worker)return 0;
    /* A worker self-suspend would park inside the native bridge rather than
     * its converted trampoline, so it cannot be rewound there. */
    if(!resume)return !g_cur || handle!=g_fib[g_cur].xhandle;
    int named=0;
    for(int i=1;i<g_nfib;++i){
        const Fiber *f=&g_fib[i];
        if(f->state==FIB_FREE || f->state==FIB_DONE)continue;
        if(f->xhandle==handle)named=1;
        /* Wake touches every matching wait; directed primary resume can
         * also run a READY worker with the named handle. Check both sets. */
        if((f->state==FIB_WAITING && f->wait_event==handle) ||
           (!g_cur && !f->is_coroutine &&
            (f->state==FIB_READY || f->state==FIB_WAITING) && f->xhandle==handle)){
            if(!f->restorable_worker || !checkpoint_ready(f))return 0;
        }
    }
    return named;
}

static int checkpoint_capture(void *out,size_t bytes,int workers)
{
    uint64_t mask;
    int count = task_snapshot_mask(&mask,workers);
    TaskSnapshot *saved = (TaskSnapshot *)out;
    if (!out || bytes != sizeof(*saved) || !count) return 0;
    memset(saved, 0, sizeof(*saved));
    saved->version = DOA3_RB_TASK_SCHEMA;
    saved->count = (uint32_t)count;
    saved->topology = g_fib_topology;
    saved->external_epoch = g_external_epoch;
    saved->workers=(uint32_t)workers;
    saved->worker_epoch=workers ? 0 : g_worker_epoch;
    if (workers) {
        saved->worker_rr=g_worker_rr; saved->idle_yields=g_idle_yields;
        saved->clock_epoch=xbox_det_epoch();
    }
    saved->mask = mask;
    for (int i = 1; i < g_nfib; ++i)
        if (mask & (UINT64_C(1) << i)) saved->tasks[i] = g_fib[i];
    return count;
}

static int checkpoint_restore(const void *data,size_t bytes,int workers,int apply)
{
    const TaskSnapshot *saved = (const TaskSnapshot *)data;
    uint64_t mask;
    int count = task_snapshot_mask(&mask,workers);
    if (!saved || bytes != sizeof(*saved) || !count || saved->version != DOA3_RB_TASK_SCHEMA ||
        saved->topology != g_fib_topology || saved->external_epoch != g_external_epoch ||
        saved->workers!=(uint32_t)workers || (!workers && saved->worker_epoch!=g_worker_epoch) ||
        (workers && (saved->clock_epoch!=xbox_det_epoch() || saved->worker_rr<0 ||
            saved->worker_rr>=g_nfib || saved->idle_yields>=200000u)) ||
        saved->count != (uint32_t)count || saved->mask != mask) {
        if(!apply && saved && bytes==sizeof(*saved))
            fprintf(stderr,"[ROLLBACK-FIBERS] header refusal: count=%u/%d topology=%llu/%llu external=%llu/%llu workers=%u/%d worker_epoch=%llu/%llu clock=%u/%u rr=%d idle=%u mask=%llX/%llX\n",
                saved->count,count,(unsigned long long)saved->topology,(unsigned long long)g_fib_topology,
                (unsigned long long)saved->external_epoch,(unsigned long long)g_external_epoch,saved->workers,workers,
                (unsigned long long)saved->worker_epoch,(unsigned long long)g_worker_epoch,saved->clock_epoch,xbox_det_epoch(),
                saved->worker_rr,saved->idle_yields,(unsigned long long)saved->mask,(unsigned long long)mask);
        return 0;
    }
    /* Validate every record before restoring any record. */
    for (int i = 1; i < g_nfib; ++i) {
        const Fiber *old = &saved->tasks[i], *now = &g_fib[i];
        if (!(mask & (UINT64_C(1) << i))) continue;
        if (old->handle != now->handle || old->start_routine != now->start_routine ||
            old->task_routine != now->task_routine ||
            old->restorable_worker != now->restorable_worker || old->restorable_task != now->restorable_task ||
            old->is_coroutine != now->is_coroutine || old->xhandle != now->xhandle ||
            old->stack_top != now->stack_top || old->stack_size != now->stack_size ||
            old->ctx1 != now->ctx1 || old->ctx2 != now->ctx2 ||
            old->resume_to != now->resume_to || !checkpoint_ready(old) ||
            (old->restorable_worker && old->worker.wait_key!=now->worker.wait_key) ||
            old->r_esp < now->stack_top + 16u - now->stack_size || old->r_esp > now->stack_top) {
            if(!apply)fprintf(stderr,"[ROLLBACK-FIBERS] fiber=%d routine=%08X task=%08X ready=%d return=%d/%d wait_key=%08X/%08X esp=%08X range=%08X..%08X identity=%d\n",
                i,old->ctx1,old->task_routine,checkpoint_ready(old),old->resume_to,now->resume_to,
                old->worker.wait_key,now->worker.wait_key,old->r_esp,now->stack_top+16u-now->stack_size,now->stack_top,
                old->handle==now->handle && old->start_routine==now->start_routine && old->task_routine==now->task_routine &&
                old->restorable_worker==now->restorable_worker && old->restorable_task==now->restorable_task &&
                old->is_coroutine==now->is_coroutine && old->xhandle==now->xhandle && old->stack_top==now->stack_top &&
                old->stack_size==now->stack_size && old->ctx1==now->ctx1 && old->ctx2==now->ctx2);
            return 0;
        }
    }
    if (!apply) return count;
    for (int i = 1; i < g_nfib; ++i) {
        const Fiber *old = &saved->tasks[i];
        Fiber *now = &g_fib[i];
        if (!(mask & (UINT64_C(1) << i))) continue;
        now->task = old->task;
        now->worker=old->worker;
        if (now->restorable_worker) { now->state=old->state; now->wait_event=old->wait_event; }
        now->r_eax = old->r_eax; now->r_ecx = old->r_ecx; now->r_edx = old->r_edx;
        now->r_esp = old->r_esp; now->r_ebx = old->r_ebx; now->r_esi = old->r_esi;
        now->r_edi = old->r_edi; now->r_sehebp = old->r_sehebp;
        memcpy(now->r_fp_stack, old->r_fp_stack, sizeof(now->r_fp_stack));
        now->r_fp_top = old->r_fp_top;
    }
    if (workers) { g_worker_rr=saved->worker_rr; g_idle_yields=saved->idle_yields; }
    return count;
}
int xbox_fiber_task_capture(void *out,size_t bytes) { return checkpoint_capture(out,bytes,0); }
int xbox_fiber_task_restore(const void *saved,size_t bytes) { return checkpoint_restore(saved,bytes,0,1); }
int xbox_fiber_converted_capture(void *out,size_t bytes) { return checkpoint_capture(out,bytes,1); }
int xbox_fiber_converted_restore(const void *saved,size_t bytes) { return checkpoint_restore(saved,bytes,1,1); }
int xbox_fiber_converted_validate(const void *saved,size_t bytes) { return checkpoint_restore(saved,bytes,1,0); }

int xbox_fiber_count(void)
{
    int c = 0;
    for (int i = 0; i < g_nfib; i++)
        if (g_fib[i].state == FIB_READY || g_fib[i].state == FIB_RUNNING) c++;
    return c;
}

/* One line: per fiber state, wait key and resumes since the last call. */
void xbox_fiber_dump_runs(void)
{
    static const char *S[] = { "FREE", "READY", "RUN", "WAIT", "DONE", "DORM" };
    static unsigned s_last[MAX_FIBERS];
    int i;
    fprintf(stderr, "[FIBRUNS] cur=%d", g_cur);
    for (i = 0; i < g_nfib; i++) {
        fprintf(stderr, " #%d:%X:%s", i, g_fib[i].ctx1 ? g_fib[i].ctx1 : g_fib[i].start_routine,
                S[g_fib[i].state]);
        if (g_fib[i].state == FIB_WAITING) fprintf(stderr, "(%X)", g_fib[i].wait_event);
        fprintf(stderr, "=%u", g_fib_runs[i] - s_last[i]);
        s_last[i] = g_fib_runs[i];
    }
    fprintf(stderr, "\n");
}

void xbox_fiber_dump(void)
{
    static const char *S[] = { "FREE", "READY", "RUNNING", "WAITING", "DONE", "DORMANT" };
    fprintf(stderr, "[FIBER-DUMP] %d fibers, cur=%d:\n", g_nfib, g_cur);
    for (int i = 0; i < g_nfib; i++)
        fprintf(stderr, "   #%d routine=0x%08X state=%s wait_event=0x%08X\n",
                i, g_fib[i].start_routine, S[g_fib[i].state], g_fib[i].wait_event);
    fflush(stderr);
}
