/* Compare the previous disabled hot path with the shared switch. */
#include "perf_capture.h"
#include <stdlib.h>
static __declspec(noinline) LONGLONG old_begin(void)
{
    static volatile LONG enabled = -1;
    static volatile ULONGLONG recheck;
    LONG on = InterlockedCompareExchange(&enabled,-1,-1);
    ULONGLONG tick = GetTickCount64();
    if (on<0 || tick>=recheck) {
        on=GetEnvironmentVariableA("DOA3_PERF_CAPTURE",NULL,0)!=0;
        InterlockedExchange(&enabled,on); recheck=tick+1000;
    }
    return on ? doa3_perf_now() : 0;
}
static __declspec(noinline) LONGLONG new_begin(void) { return doa3_perf_begin(); }
static double measure(LONGLONG (*fn)(void))
{
    LARGE_INTEGER frequency;
    volatile LONGLONG sum=0;
    LONGLONG start=doa3_perf_now();
    for(unsigned i=0;i<2000000;i++) sum+=fn();
    QueryPerformanceFrequency(&frequency);
    return 1000.0*(doa3_perf_now()-start)/frequency.QuadPart;
}
int main(void)
{
    _putenv_s("DOA3_PERF_CAPTURE",""); doa3_perf_refresh(); old_begin();
    for(int i=0;i<7;i++) {
        double old_ms, new_ms;
        if(i%2) { new_ms=measure(new_begin); old_ms=measure(old_begin); }
        else { old_ms=measure(old_begin); new_ms=measure(new_begin); }
        printf("disabled timing: old %.3f ms, new %.3f ms / 2000000 calls\n",old_ms,new_ms);
    }
    return 0;
}
