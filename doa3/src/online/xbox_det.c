/*
 * xbox_det.c - deterministic time for lockstep netplay sessions (see xbox_det.h).
 */
#include "kernel/kernel.h"
#include "xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_fiber.h"

volatile int g_xbox_det_active = 0;
extern volatile int g_fib_slice_due;   /* xbox_fiber.c */

/* One lock orders session start/end against the two host threads that
 * would otherwise write guest-visible time at arbitrary moments. */
static SRWLOCK s_lock = SRWLOCK_INIT;
static DWORD s_session_owner;
static volatile LONG s_pin_owner;
static int clock_pinned_here(void)
{
    return (DWORD)InterlockedCompareExchange(&s_pin_owner,0,0)==GetCurrentThreadId();
}
int xbox_det_checkpoint_acquire(void)
{
    if (InterlockedCompareExchange(&s_pin_owner,0,0) || !TryAcquireSRWLockExclusive(&s_lock)) return 0;
    if (!g_xbox_det_active || s_session_owner!=GetCurrentThreadId() || xbox_fiber_current()!=0) {
        ReleaseSRWLockExclusive(&s_lock); return 0;
    }
    InterlockedExchange(&s_pin_owner,(LONG)GetCurrentThreadId());
    return 1;
}
int xbox_det_checkpoint_release(void)
{
    if (!clock_pinned_here()) return 0;
    InterlockedExchange(&s_pin_owner,0);
    ReleaseSRWLockExclusive(&s_lock);
    return 1;
}
static void checkpoint_lock(void)
{
    if (!clock_pinned_here()) AcquireSRWLockExclusive(&s_lock);
}
static void checkpoint_unlock(void)
{
    if (!clock_pinned_here()) ReleaseSRWLockExclusive(&s_lock);
}

static xbox_det_bases s_base;
static uint64_t s_frames;
static uint64_t s_qpf;
static uint64_t s_qpc_boost;        /* hot-poll boost accumulated this session */
static uint32_t s_kcalls;           /* guest kernel calls since session start */
static int      s_pending_laps;
/* Per fiber: kernel calls and hot-poll QPC boost. The audio workers' polling
 * depends on how much streaming is pending, so neither may move the time or
 * the poll accounting another fiber (above all the game thread) sees. */
#define DET_FIBERS XBOX_DET_CHECKPOINT_FIBERS
static uint32_t s_fkcalls[DET_FIBERS];
static uint64_t s_fboost[DET_FIBERS];
static xbox_det_qpc_poll_state s_fpolls[DET_FIBERS];
static int fib_index(void)
{
    int f = xbox_fiber_current();
    return (f >= 0 && f < DET_FIBERS) ? f : DET_FIBERS - 1;
}
static volatile uint32_t s_epoch;   /* bumped on begin and end */

static uint32_t s_tick_offset;      /* added to the host tick after a session */
static uint64_t s_qpc_offset;       /* added to the host QPC after a session */
static uint64_t s_rdtsc_offset;     /* keep CPU timestamps monotonic on exit */
static volatile uint32_t s_last_host_raw_ms;

static volatile uint32_t *tick_export(void)
{
    return (volatile uint32_t *)((uintptr_t)(XBOX_KERNEL_DATA_BASE + KDATA_TICK_COUNT) +
                                 (uintptr_t)xbox_GetMemoryOffset());
}

static uint64_t host_qpc(void)
{
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (uint64_t)c.QuadPart;
}

#define CPU_HZ UINT64_C(733333333)
static INIT_ONCE s_cpu_init=INIT_ONCE_STATIC_INIT;
static LARGE_INTEGER s_cpu_frequency,s_cpu_origin;
static BOOL CALLBACK init_cpu_clock(PINIT_ONCE once,PVOID parameter,PVOID *context)
{
    (void)once;(void)parameter;(void)context;
    QueryPerformanceFrequency(&s_cpu_frequency);
    QueryPerformanceCounter(&s_cpu_origin);
    return TRUE;
}
static uint64_t cpu_scale(uint64_t ticks,uint64_t frequency)
{
    return (ticks/frequency)*CPU_HZ+(ticks%frequency)*CPU_HZ/frequency;
}
static uint64_t host_rdtsc(void)
{
    LARGE_INTEGER now;
    InitOnceExecuteOnce(&s_cpu_init,init_cpu_clock,NULL,NULL);
    QueryPerformanceCounter(&now);
    return cpu_scale((uint64_t)(now.QuadPart-s_cpu_origin.QuadPart),(uint64_t)s_cpu_frequency.QuadPart);
}
uint64_t xbox_det_rdtsc(void)
{
    if(g_xbox_det_active)
        return s_base.rdtsc+cpu_scale(s_frames,60)+cpu_scale(s_fboost[fib_index()],s_qpf);
    return host_rdtsc()+s_rdtsc_offset;
}

uint32_t xbox_det_epoch(void) { return s_epoch; }

static volatile uint32_t s_io;
void     xbox_det_note_io(void)  { s_io++; }
uint32_t xbox_det_io_count(void) { return s_io; }

void xbox_det_current_bases(xbox_det_bases *out)
{
    FILETIME ft;
    if (!out) return;
    if (g_xbox_det_active) {
        out->tick_ms = xbox_det_tick_ms();
        out->qpc = xbox_det_qpc();
        out->filetime = xbox_det_filetime();
        out->rdtsc = xbox_det_rdtsc();
        return;
    }
    out->tick_ms = *tick_export();
    out->qpc = host_qpc() + s_qpc_offset;
    GetSystemTimeAsFileTime(&ft);
    out->filetime = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    out->rdtsc = xbox_det_rdtsc();
}

void xbox_det_begin(const xbox_det_bases *b)
{
    if (clock_pinned_here()) return;
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    AcquireSRWLockExclusive(&s_lock);
    s_session_owner=GetCurrentThreadId();
    s_base = *b;
    s_frames = 0;
    s_qpf = (uint64_t)f.QuadPart;
    s_qpc_boost = 0;
    s_kcalls = 0;
    memset(s_fkcalls, 0, sizeof(s_fkcalls));
    memset(s_fboost, 0, sizeof(s_fboost));
    memset(s_fpolls, 0, sizeof(s_fpolls));
    for (unsigned i=0;i<DET_FIBERS;++i) s_fpolls[i].last_call=-2;
    s_pending_laps = 0;
    s_epoch++;
    g_fib_slice_due = 0;
    *tick_export() = s_base.tick_ms;
    g_xbox_det_active = 1;
    ReleaseSRWLockExclusive(&s_lock);
}

void xbox_det_end(void)
{
    if (clock_pinned_here()) return;
    AcquireSRWLockExclusive(&s_lock);
    if (g_xbox_det_active) {
        uint32_t det_ms = xbox_det_tick_ms();
        uint64_t det_q = xbox_det_qpc();
        uint64_t now_q = host_qpc();
        uint64_t det_cpu=xbox_det_rdtsc(),now_cpu=host_rdtsc();
        if ((int32_t)(det_ms - (s_last_host_raw_ms + s_tick_offset)) > 0)
            s_tick_offset = det_ms - s_last_host_raw_ms;
        if (det_q > now_q + s_qpc_offset)
            s_qpc_offset = det_q - now_q;
        if(det_cpu>now_cpu+s_rdtsc_offset)s_rdtsc_offset=det_cpu-now_cpu;
        g_xbox_det_active = 0;
        s_session_owner=0;
        s_pending_laps = 0;
        s_epoch++;
    }
    ReleaseSRWLockExclusive(&s_lock);
}

void xbox_det_frame(void)
{
    if (InterlockedCompareExchange(&s_pin_owner,0,0)) return;
    if (!g_xbox_det_active) return;
    s_frames++;
    *tick_export() = xbox_det_tick_ms();
    s_pending_laps += XBOX_DET_LAPS_PER_FRAME;
    g_fib_slice_due = 1;
    xbox_fiber_wake(XBOX_DET_VBLANK_KEY);
}

uint64_t xbox_det_frames(void) { return s_frames; }

int xbox_det_capture(xbox_det_checkpoint *out)
{
    if (!out) return 0;
    checkpoint_lock();
    if (!g_xbox_det_active) {
        checkpoint_unlock();
        return 0;
    }
    memset(out, 0, sizeof(*out));
    out->version = XBOX_DET_CHECKPOINT_VERSION;
    out->epoch = s_epoch;
    out->frames = s_frames;
    out->qpc_boost = s_qpc_boost;
    out->kcalls = s_kcalls;
    out->pending_laps = s_pending_laps;
    out->slice_due = g_fib_slice_due;
    memcpy(out->fiber_kcalls, s_fkcalls, sizeof(s_fkcalls));
    memcpy(out->fiber_boost, s_fboost, sizeof(s_fboost));
    memcpy(out->fiber_polls, s_fpolls, sizeof(s_fpolls));
    checkpoint_unlock();
    return 1;
}

static int checkpoint_valid(const xbox_det_checkpoint *saved)
{
    if(!saved || !g_xbox_det_active || saved->version!=XBOX_DET_CHECKPOINT_VERSION ||
       saved->epoch!=s_epoch || saved->pending_laps<0 || saved->slice_due<0 || saved->slice_due>1)return 0;
    for(unsigned i=0;i<DET_FIBERS;++i) {
        const xbox_det_qpc_poll_state *p=&saved->fiber_polls[i];
        if(p->run>=2048 || (p->last_call!=-2 && (p->last_call<0 || p->last_call>UINT32_MAX)))return 0;
    }
    return 1;
}
int xbox_det_validate(const xbox_det_checkpoint *saved)
{
    checkpoint_lock();
    int valid=checkpoint_valid(saved);
    checkpoint_unlock();
    return valid;
}
int xbox_det_restore(const xbox_det_checkpoint *saved)
{
    if (!saved) return 0;
    checkpoint_lock();
    if (!checkpoint_valid(saved)) {
        checkpoint_unlock();
        return 0;
    }
    s_frames = saved->frames;
    s_qpc_boost = saved->qpc_boost;
    s_kcalls = saved->kcalls;
    s_pending_laps = saved->pending_laps;
    g_fib_slice_due = saved->slice_due;
    memcpy(s_fkcalls, saved->fiber_kcalls, sizeof(s_fkcalls));
    memcpy(s_fboost, saved->fiber_boost, sizeof(s_fboost));
    memcpy(s_fpolls, saved->fiber_polls, sizeof(s_fpolls));
    *tick_export() = xbox_det_tick_ms();
    /* Do not wake workers here. Their scheduler state must be restored by the
     * simulation adapter, without executing work during snapshot restoration. */
    checkpoint_unlock();
    return 1;
}

uint32_t xbox_det_tick_ms(void)
{
    return s_base.tick_ms + (uint32_t)((s_frames * 1000u) / 60u);
}

/* The calling fiber's view of the session QPC. */
uint64_t xbox_det_qpc(void)
{
    return s_base.qpc + (s_frames * s_qpf) / 60u + s_fboost[fib_index()];
}

void xbox_det_add_qpc_boost(uint64_t delta)
{
    if (InterlockedCompareExchange(&s_pin_owner,0,0)) return;
    s_fboost[fib_index()] += delta;
    s_qpc_boost += delta;
}

int xbox_det_qpc_poll(void)
{
    if(!g_xbox_det_active || s_session_owner!=GetCurrentThreadId() ||
       InterlockedCompareExchange(&s_pin_owner,0,0))return 0;
    int f=fib_index(),hot=0;
    xbox_det_qpc_poll_state *p=&s_fpolls[f];
    int64_t calls=s_fkcalls[f];
    if(calls<=p->last_call+2) {
        if(++p->consecutive>150)hot=1;
    } else p->consecutive=0;
    p->last_call=calls;
    if(hot || ++p->run>=2048) {
        xbox_det_add_qpc_boost(733333);
        p->run=0;return 1;
    }
    return 0;
}

int xbox_det_audio_wait_poll(void)
{
    if(!g_xbox_det_active || s_session_owner!=GetCurrentThreadId() ||
       InterlockedCompareExchange(&s_pin_owner,0,0))return 0;
    /* The native wait's deadline can never expire on a frozen per-frame CPU
     * timestamp. Fixed per-iteration progress is independent of host speed;
     * its boost is already captured/restored and isolated from other fibers.
     * Do not bypass voice status or force a successful stop. The original
     * guest status/deadline branches still decide when this loop returns. */
    xbox_det_add_qpc_boost((s_qpf+9999u)/10000u);
    return 1;
}

uint32_t xbox_det_fiber_kcalls(void) { return s_fkcalls[fib_index()]; }
uint32_t xbox_det_kcalls(void) { return s_kcalls; }
uint64_t xbox_det_boost(void) { return s_fboost[0]; }   /* the game thread's */

uint64_t xbox_det_filetime(void)
{
    return s_base.filetime + (s_frames * 10000000u) / 60u;
}

uint32_t xbox_det_tick_offset(void) { return s_tick_offset; }
uint64_t xbox_det_qpc_offset(void)  { return s_qpc_offset; }

void xbox_det_publish_host_tick(uint32_t host_ms)
{
    AcquireSRWLockShared(&s_lock);
    s_last_host_raw_ms = host_ms;
    if (!g_xbox_det_active) *tick_export() = host_ms + s_tick_offset;
    ReleaseSRWLockShared(&s_lock);
}

void xbox_det_host_slice_timer(void)
{
    AcquireSRWLockShared(&s_lock);
    if (!g_xbox_det_active) g_fib_slice_due = 1;
    ReleaseSRWLockShared(&s_lock);
}


void xbox_det_on_kernel_call(unsigned ordinal)
{
    if (InterlockedCompareExchange(&s_pin_owner,0,0)) return;
    if (!g_xbox_det_active) return;
    s_kcalls++;
    s_fkcalls[fib_index()]++;
    /* No kernel-call-driven slices in a session: where they fell depended
     * on how many calls the audio code had made, so a load could finish
     * just before or just after the game's per-frame check. Workers run
     * at the lockstep frame (until idle) and whenever a guest thread
     * waits. */
}

int xbox_det_slice_laps(void)
{
    if (InterlockedCompareExchange(&s_pin_owner,0,0)) return 0;
    int n = s_pending_laps;
    s_pending_laps = 0;
    return n > 0 ? n : 1;
}
