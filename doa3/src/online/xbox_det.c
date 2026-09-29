/*
 * xbox_det.c - deterministic time for lockstep netplay sessions (see xbox_det.h).
 */
#include "kernel/kernel.h"
#include "xbox_det.h"
#include "kernel/xbox_memory_layout.h"

volatile int g_xbox_det_active = 0;
extern volatile int g_fib_slice_due;   /* xbox_fiber.c */

/* One lock orders session start/end against the two host threads that
 * would otherwise write guest-visible time at arbitrary moments. */
static SRWLOCK s_lock = SRWLOCK_INIT;

static xbox_det_bases s_base;
static uint64_t s_frames;
static uint64_t s_qpf;
static uint64_t s_qpc_boost;        /* hot-poll boost accumulated this session */
static uint32_t s_kcalls;           /* guest kernel calls since session start */
static int      s_pending_laps;
static volatile uint32_t s_epoch;   /* bumped on begin and end */

static uint32_t s_tick_offset;      /* added to the host tick after a session */
static uint64_t s_qpc_offset;       /* added to the host QPC after a session */
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

uint32_t xbox_det_epoch(void) { return s_epoch; }

void xbox_det_current_bases(xbox_det_bases *out)
{
    FILETIME ft;
    if (!out) return;
    if (g_xbox_det_active) {
        out->tick_ms = xbox_det_tick_ms();
        out->qpc = xbox_det_qpc();
        out->filetime = xbox_det_filetime();
        return;
    }
    out->tick_ms = *tick_export();
    out->qpc = host_qpc() + s_qpc_offset;
    GetSystemTimeAsFileTime(&ft);
    out->filetime = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
}

void xbox_det_begin(const xbox_det_bases *b)
{
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    AcquireSRWLockExclusive(&s_lock);
    s_base = *b;
    s_frames = 0;
    s_qpf = (uint64_t)f.QuadPart;
    s_qpc_boost = 0;
    s_kcalls = 0;
    s_pending_laps = 0;
    s_epoch++;
    g_fib_slice_due = 0;
    *tick_export() = s_base.tick_ms;
    g_xbox_det_active = 1;
    ReleaseSRWLockExclusive(&s_lock);
}

void xbox_det_end(void)
{
    AcquireSRWLockExclusive(&s_lock);
    if (g_xbox_det_active) {
        uint32_t det_ms = xbox_det_tick_ms();
        uint64_t det_q = xbox_det_qpc();
        uint64_t now_q = host_qpc();
        if ((int32_t)(det_ms - (s_last_host_raw_ms + s_tick_offset)) > 0)
            s_tick_offset = det_ms - s_last_host_raw_ms;
        if (det_q > now_q + s_qpc_offset)
            s_qpc_offset = det_q - now_q;
        g_xbox_det_active = 0;
        s_pending_laps = 0;
        s_epoch++;
    }
    ReleaseSRWLockExclusive(&s_lock);
}

void xbox_det_frame(void)
{
    if (!g_xbox_det_active) return;
    s_frames++;
    *tick_export() = xbox_det_tick_ms();
    s_pending_laps += XBOX_DET_LAPS_PER_FRAME;
    g_fib_slice_due = 1;
}

uint64_t xbox_det_frames(void) { return s_frames; }

uint32_t xbox_det_tick_ms(void)
{
    return s_base.tick_ms + (uint32_t)((s_frames * 1000u) / 60u);
}

uint64_t xbox_det_qpc(void)
{
    return s_base.qpc + (s_frames * s_qpf) / 60u + s_qpc_boost;
}

void xbox_det_add_qpc_boost(uint64_t delta) { s_qpc_boost += delta; }

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

void xbox_det_on_kernel_call(void)
{
    if (!g_xbox_det_active) return;
    if (++s_kcalls % XBOX_DET_KCALL_PERIOD == 0) {
        s_pending_laps += 1;
        g_fib_slice_due = 1;
    }
}

int xbox_det_slice_laps(void)
{
    int n = s_pending_laps;
    s_pending_laps = 0;
    return n > 0 ? n : 1;
}
