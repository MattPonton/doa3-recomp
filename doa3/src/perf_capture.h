#pragma once

#include <windows.h>
#include <stdio.h>

/* One process-wide switch, updated when the Debug setting changes. Windows
 * provides atomic aligned LONG loads; the disabled draw path is just a load
 * and branch, with no clock, environment lookup or locked instruction. */
extern volatile LONG doa3_perf_enabled;
LONG doa3_perf_refresh(void);

static inline LONGLONG doa3_perf_now(void)
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return now.QuadPart;
}

static inline LONGLONG doa3_perf_begin(void)
{
    LONG on = doa3_perf_enabled;
    if (on < 0) on = doa3_perf_refresh();
    return on ? doa3_perf_now() : 0;
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
