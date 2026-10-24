/* Production kernel QPC bridge + clock; scheduler endpoints are counters. */
#include "xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static uint32_t tick,g_eax,g_edx;
static int fiber;
static unsigned worker_runs,host_reads;
static unsigned long long g_kernel_call_count;
volatile int g_fib_slice_due;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)((uintptr_t)&tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));}
int xbox_fiber_current(void){return fiber;}
int xbox_fiber_is_primary(void){return fiber==0;}
int xbox_fiber_active(void){return 1;}
void xbox_fiber_wake(uint32_t key){CHECK(key==XBOX_DET_VBLANK_KEY);}
void xbox_fiber_run_workers_idle(int cap){CHECK(fiber==0 && cap==1024);++worker_runs;}
void xbox_fiber_yield(void){CHECK(!g_xbox_det_active);}
LARGE_INTEGER xbox_KeQueryPerformanceCounter(void){LARGE_INTEGER v;v.QuadPart=123456;++host_reads;return v;}
#include "qpc_bridge.generated.inc"
typedef struct { uint64_t qpc;unsigned workers,reserved; } Sample;
static Sample poll(unsigned gaps){
    for(unsigned i=0;i<gaps;++i){++g_kernel_call_count;xbox_det_on_kernel_call(127);}
    ++g_kernel_call_count;xbox_det_on_kernel_call(126);unsigned before=worker_runs;
    bridge_KeQueryPerformanceCounter();
    Sample s={(uint64_t)g_eax|((uint64_t)g_edx<<32),worker_runs-before,0};return s;
}
static void exercise(unsigned warm,unsigned gaps,int selected){
    xbox_det_bases base={100,200,300};xbox_det_checkpoint saved,expected,actual;
    Sample a[16],b[16];fiber=0;xbox_det_begin(&base);fiber=selected;
    for(unsigned i=0;i<warm;++i)CHECK(!poll(gaps).workers);
    CHECK(xbox_det_capture(&saved));
    for(unsigned i=0;i<16;++i)a[i]=poll(gaps);
    CHECK(xbox_det_capture(&expected));
    for(unsigned i=0;i<300;++i)(void)poll(0);
    unsigned host=host_reads;
    for(unsigned replay=0;replay<3;++replay){
        CHECK(xbox_det_restore(&saved));
        for(unsigned i=0;i<16;++i){b[i]=poll(gaps);CHECK(!memcmp(&a[i],&b[i],sizeof(Sample)));}
        CHECK(xbox_det_capture(&actual) && !memcmp(&actual,&expected,sizeof(actual)));
    }
    CHECK(host_reads==host);
    if(gaps){CHECK(a[0].qpc==base.qpc+733333);CHECK(a[1].qpc==a[0].qpc);}
    else {CHECK(a[0].qpc==base.qpc);CHECK(a[1].qpc==base.qpc+733333);}
    if(selected)CHECK(!a[0].workers && !a[1].workers);
    else CHECK(gaps ? a[0].workers==1:a[1].workers==1);
    fiber=0;xbox_det_end();
}
static DWORD WINAPI wrong_owner(void *unused){(void)unused;CHECK(!xbox_det_qpc_poll());return 0;}
static void isolation(void){
    xbox_det_bases base={1,2,3};xbox_det_checkpoint before,after;
    fiber=0;xbox_det_begin(&base);
    for(unsigned i=0;i<150;++i)(void)poll(0);
    fiber=3;for(unsigned i=0;i<170;++i)CHECK(!poll(0).workers);
    fiber=0;CHECK(poll(0).qpc==2);CHECK(poll(0).qpc==2+733333);
    CHECK(xbox_det_capture(&before));
    HANDLE th=CreateThread(NULL,0,wrong_owner,NULL,0,NULL);CHECK(th);
    CHECK(WaitForSingleObject(th,5000)==WAIT_OBJECT_0);CloseHandle(th);
    CHECK(xbox_det_capture(&after) && !memcmp(&before,&after,sizeof(before)));
    CHECK(xbox_det_checkpoint_acquire());
    CHECK(!xbox_det_qpc_poll());CHECK(xbox_det_capture(&after));
    CHECK(!memcmp(&before,&after,sizeof(before)));CHECK(xbox_det_checkpoint_release());
    xbox_det_end();CHECK(!xbox_det_qpc_poll());
    xbox_det_begin(&base);CHECK(xbox_det_capture(&after));
    for(unsigned i=0;i<XBOX_DET_CHECKPOINT_FIBERS;++i)
        CHECK(!after.fiber_polls[i].consecutive && !after.fiber_polls[i].run && after.fiber_polls[i].last_call==-2);
    xbox_det_end();
}
int main(void){
    exercise(150,0,0);exercise(2047,2,0);exercise(150,0,3);exercise(2047,2,3);
    isolation();
    puts("Actual kernel QPC bridge: hot/fallback threshold rewinds reproduce every returned time and worker-run decision for primary and worker fibers.");return 0;
}
