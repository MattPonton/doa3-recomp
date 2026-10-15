#include "perf_capture.h"

volatile LONG doa3_perf_enabled = -1;

LONG doa3_perf_refresh(void)
{
    LONG on = GetEnvironmentVariableA("DOA3_PERF_CAPTURE", NULL, 0) != 0;
    InterlockedExchange(&doa3_perf_enabled, on);
    return on;
}
