#pragma once

#include <windows.h>
#include <stdio.h>

/* Opt-in wall-clock attribution. No timers or output on ordinary launches.
 * Report the ending QPC timestamp so a slow operation can be correlated with
 * the frame capture without depending on game symbols in renderer tests. */
static inline LONGLONG doa3_perf_begin(void)
{
    /* Re-read the switch about once a second, so turning diagnostics on or
     * off in the Debug menu (which sets/clears DOA3_PERF_CAPTURE) takes
     * effect without a relaunch. */
    static volatile LONG enabled = -1;
    static volatile ULONGLONG recheck;
    LARGE_INTEGER now;
    LONG on = InterlockedCompareExchange(&enabled, -1, -1);
    ULONGLONG tick = GetTickCount64();
    if (on < 0 || tick >= recheck) {
        on = GetEnvironmentVariableA("DOA3_PERF_CAPTURE", NULL, 0) != 0;
        InterlockedExchange(&enabled, on);
        recheck = tick + 1000;
    }
    if (!on) return 0;
    QueryPerformanceCounter(&now);
    return now.QuadPart;
}

static inline void doa3_perf_report(const char *operation, LONGLONG begin,
                                  LONGLONG end)
{
    LARGE_INTEGER frequency;
    double ms;
    if (!begin) return;
    QueryPerformanceFrequency(&frequency);
    ms = 1000.0 * (end - begin) / frequency.QuadPart;
    if (ms >= 1.0)
        fprintf(stderr, "[PERF-OP] %s %.3f ms end-qpc %lld\n",
                operation, ms, (long long)end);
}

static inline void doa3_perf_end(const char *operation, LONGLONG begin)
{
    LARGE_INTEGER end;
    if (!begin) return;
    QueryPerformanceCounter(&end);
    doa3_perf_report(operation, begin, end.QuadPart);
}
