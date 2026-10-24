/* Actual bridge dispatch/policy and clock/guard; fake bridge endpoints count
 * calls. No files, kernel handles or guest worker bodies are executed. */
#include "kernel/kernel_replay.h"
#include "kernel/xbox_memory_layout.h"
#include "online/xbox_det.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static uint32_t tick,g_eax,g_edx,g_esp,ram[1024];
static unsigned effects,slices;
static int fiber,fault_in_slice,coroutine;
static unsigned thread_wakes,thread_runs;
static int thread_allowed=1;
int xbox_fiber_replay_thread_allowed(uint32_t handle,int resume){return thread_allowed && handle==0xBEEF1234 && (resume==0 || resume==1);}
uint32_t xbox_fiber_current_xhandle(void){return 0xBEEF5678;}
int xbox_fiber_run_thread(uint32_t handle){CHECK(handle==0xBEEF1234);++thread_runs;return 1;}
size_t xbox_GetMemorySize(void){return sizeof ram;}
int xbox_fiber_is_coroutine(void){return coroutine;}
volatile int g_fib_slice_due;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)((uintptr_t)&tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));}
int xbox_fiber_current(void){return fiber;}
int xbox_fiber_is_primary(void){return fiber==0;}
void xbox_fiber_wake(uint32_t key){if(key==0xBEEF1234)++thread_wakes;else CHECK(key==XBOX_DET_VBLANK_KEY);}
void xbox_fiber_timeslice(void){++slices;if(fault_in_slice)CHECK(!xbox_kernel_replay_check(236,0));}
typedef void (*bridge_func_t)(void);
#undef XBOX_KERNEL_THUNK_TABLE_SIZE
#define XBOX_KERNEL_THUNK_TABLE_SIZE 4
#define BRIDGE_MEM32(a) ram[(a)/4]
static int g_kernel_dispatch_slot;
static ULONG g_slot_ordinals[4];
static bridge_func_t g_slot_bridges[4];
static int g_slot_arg_bytes[4];
static unsigned long long g_kernel_call_count;
static unsigned g_kcount[400],g_ktrace_pos;
static struct {unsigned long long n;uint32_t esp,arg0;uint16_t ordinal;int16_t fiber;} g_ktrace_ring[32];
/* Keep endpoints distinct under MSVC identical-code folding: binding identity
 * is part of this test, so identical empty/counting stubs are insufficient. */
static volatile unsigned endpoint_hits[23];
#define ENDPOINT(name,id) static void name(void){++endpoint_hits[id];++effects;g_eax=42;}
ENDPOINT(kx_ExAllocatePool,0) ENDPOINT(kx_ExAllocatePoolWithTag,1) ENDPOINT(kx_ExFreePool,2)
ENDPOINT(kx_ExQueryPoolBlockSize,3) ENDPOINT(bridge_KeInitializeDpc,4)
ENDPOINT(bridge_KeInitializeInterrupt,5) ENDPOINT(bridge_KeInsertQueueDpc,6)
ENDPOINT(bridge_KeQueryPerformanceCounter,7) ENDPOINT(bridge_KeQueryPerformanceFrequency,8)
ENDPOINT(bridge_KeQuerySystemTime,9) ENDPOINT(bridge_KeSetEvent,10)
ENDPOINT(kx_MmQueryAddressProtect,11) ENDPOINT(kx_MmQueryAllocationSize,12) ENDPOINT(unsafe_bridge,13)
ENDPOINT(bridge_KfRaiseIrql,14) ENDPOINT(bridge_KfLowerIrql,15)
ENDPOINT(bridge_RtlEnterCriticalSection,16) ENDPOINT(bridge_RtlLeaveCriticalSection,17)
ENDPOINT(bridge_ObReferenceObjectByHandle,18) ENDPOINT(bridge_ObfDereferenceObject,19)
ENDPOINT(bridge_KeWaitForSingleObject,20)
ENDPOINT(bridge_NtResumeThread,21) ENDPOINT(bridge_NtSuspendThread,22)
#define STACK_ARG(n) BRIDGE_MEM32(g_esp+(n)*4)
#define BRIDGE_MEM8(a) (((unsigned char *)ram)[a])
#define XBOX_FIB_VBLANK_KEY 0xB1A4B1A4u
static int g_kwait_log;
int xbox_fiber_active(void){return 1;}
uint32_t doa3_vblank_event_va(void){return 64;}
uint32_t doa3_vblank_count_va(void){CHECK(0);return 0;}
void xbox_fiber_block(uint32_t key){(void)key;CHECK(0);}
void xbox_fiber_run_workers_idle(unsigned n){(void)n;CHECK(0);}
void xbox_fiber_yield(void){CHECK(0);}
void doa3_pump_cri_servers(void){CHECK(0);}
typedef void (*recomp_func_t)(void);
void sub_001F3FFA(void),sub_001F8C51(void);
static int callback_remap;
recomp_func_t recomp_lookup_manual(uint32_t va){(void)va;return callback_remap ? unsafe_bridge:NULL;}
recomp_func_t recomp_lookup(uint32_t va){return va==0x1F3FFA ? sub_001F3FFA:va==0x1F8C51 ? sub_001F8C51:NULL;}
static uint32_t g_ecx,g_esi,g_seh_ebp;
#define eax g_eax
#define edx g_edx
#define ecx g_ecx
#define esi g_esi
#define esp g_esp
#define MEM32(a) BRIDGE_MEM32(a)
#define MEM8(a) BRIDGE_MEM8(a)
#define PUSH32(sp,v) do{uint32_t push_value=(v);(sp)-=4;MEM32(sp)=push_value;}while(0)
#define POP32(sp,v) do{(v)=MEM32(sp);(sp)+=4;}while(0)
#define LO8(v) ((uint8_t)(v))
#define SET_LO8(v,b) ((v)=((v)&0xFFFFFF00u)|(uint8_t)(b))
#include "kernel_dispatch.generated.inc"
#undef esp
#undef eax
#undef edx
#undef ecx
#undef esi
static void dispatch(unsigned ordinal,bridge_func_t fn,int args){
    g_kernel_dispatch_slot=0;g_slot_ordinals[0]=ordinal;g_slot_bridges[0]=fn;g_slot_arg_bytes[0]=args;
    g_esp=256;g_eax=g_edx=0xDEADBEEF;kernel_thunk_dispatch();CHECK(g_esp==260+(uint32_t)args);
}
static DWORD WINAPI foreign(void *unused){
    (void)unused;CHECK(!xbox_kernel_replay_begin());
    CHECK(!xbox_kernel_replay_frame_budget(8));
    CHECK(!xbox_kernel_replay_check(126,1));CHECK(!xbox_kernel_replay_end(1));return 0;
}
static DWORD WINAPI foreign_audit(void *unused){
    XboxKernelReplayAudit a;(void)unused;
    CHECK(!xbox_kernel_replay_audit_begin());CHECK(!xbox_kernel_replay_audit_end(&a));
    CHECK(xbox_kernel_replay_check(236,0));return 0;
}
static void dispatch_benchmark(void){
    double elapsed[2][5];LARGE_INTEGER frequency;QueryPerformanceFrequency(&frequency);
    /* Same ordinal/binding and guard budget in alternating order. No timers
     * or output inside dispatch; 300k approximates an eight-frame fight run. */
    for(unsigned sample=0;sample<5;++sample)for(unsigned order=0;order<2;++order){
        unsigned reference=(sample+order)&1;
        CHECK(xbox_kernel_replay_begin() && xbox_kernel_replay_frame_budget(8));
        g_kernel_dispatch_slot=0;g_slot_ordinals[0]=277;
        g_slot_bridges[0]=bridge_RtlEnterCriticalSection;g_slot_arg_bytes[0]=4;
        LARGE_INTEGER start,end;unsigned before=effects;QueryPerformanceCounter(&start);
        for(unsigned i=0;i<300000;++i){
            g_esp=256;
            if(reference)kernel_thunk_dispatch_reference();else kernel_thunk_dispatch();
        }
        QueryPerformanceCounter(&end);
        elapsed[reference][sample]=(end.QuadPart-start.QuadPart)*1000.0/frequency.QuadPart;
        XboxKernelReplayReport report;xbox_kernel_replay_report(&report);
        CHECK(effects==before+300000 && report.accepted==300000 && !report.reason && g_esp==264);
        CHECK(xbox_kernel_replay_end(0));
    }
    for(unsigned i=0;i<5;++i)printf("dispatch benchmark sample=%u reference_ms=%.3f optimized_ms=%.3f\n",i+1,elapsed[1][i],elapsed[0][i]);
}
int main(int argc,char **argv){
    CHECK(!_putenv_s("DOA3_KERNEL_DIAGNOSTICS","0"));
    _putenv_s("DOA3_KTRACE","0");
    CHECK(!xbox_kernel_replay_begin());
    dispatch(236,unsafe_bridge,32);CHECK(effects==1 && g_eax==42); /* unchanged outside a trial */
    xbox_det_bases base={1,2,3};xbox_det_begin(&base);
    if(argc==2 && !strcmp(argv[1],"--bench")){dispatch_benchmark();xbox_det_end();return 0;}
    if(argc==2 && !strcmp(argv[1],"fail-stop")){
        SetErrorMode(SEM_NOGPFAULTERRORBOX);
        _set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
        CHECK(xbox_kernel_replay_begin() && xbox_kernel_replay_fail_stop(1));
        dispatch(236,unsafe_bridge,32);
        puts("UNSAFE CONTINUATION");return 0;
    }
    const unsigned denied[]={187,189,190,193,219,222,225,236,182,184,199,255,258,125,159,0,999};
    for(unsigned i=0;i<sizeof(denied)/sizeof(*denied);++i){
        CHECK(xbox_kernel_replay_begin());CHECK(!xbox_kernel_replay_begin());
        unsigned calls=effects,work=slices,kcalls=xbox_det_kcalls();
        dispatch(denied[i],unsafe_bridge,32);
        CHECK(effects==calls && slices==work && xbox_det_kcalls()==kcalls && !g_eax && !g_edx);
        CHECK(!xbox_kernel_replay_healthy() && !xbox_kernel_replay_end(0));
        dispatch(126,bridge_KeQueryPerformanceCounter,0);CHECK(effects==calls);
        XboxKernelReplayReport r;xbox_kernel_replay_report(&r);
        CHECK(r.reason==XBOX_REPLAY_KERNEL_UNSUPPORTED && r.ordinal==denied[i] && r.rejected==2 && !r.accepted);
        CHECK(xbox_kernel_replay_end(1));
    }
    const struct {unsigned ordinal;bridge_func_t fn;int args;} allowed[]={
        {14,kx_ExAllocatePool,4},{15,kx_ExAllocatePoolWithTag,8},{17,kx_ExFreePool,4},
        {23,kx_ExQueryPoolBlockSize,4},{107,bridge_KeInitializeDpc,12},
        {109,bridge_KeInitializeInterrupt,28},{119,bridge_KeInsertQueueDpc,12},
        {126,bridge_KeQueryPerformanceCounter,0},{127,bridge_KeQueryPerformanceFrequency,0},
        {128,bridge_KeQuerySystemTime,4},{145,bridge_KeSetEvent,12},
        {179,kx_MmQueryAddressProtect,4},{180,kx_MmQueryAllocationSize,4},
        {160,bridge_KfRaiseIrql,0},{161,bridge_KfLowerIrql,0},
        {277,bridge_RtlEnterCriticalSection,4},{294,bridge_RtlLeaveCriticalSection,4},
        {246,bridge_ObReferenceObjectByHandle,12},{250,bridge_ObfDereferenceObject,0},
        {159,bridge_KeWaitForSingleObject,20}};
    for(unsigned i=0;i<sizeof(allowed)/sizeof(*allowed);++i){
        CHECK(xbox_kernel_replay_begin());unsigned before=effects;
        dispatch(allowed[i].ordinal,allowed[i].fn,allowed[i].args);
        CHECK(effects==before+1 && g_eax==42 && xbox_kernel_replay_healthy());CHECK(xbox_kernel_replay_end(0));
        CHECK(xbox_kernel_replay_begin());dispatch(allowed[i].ordinal,unsafe_bridge,allowed[i].args);
        CHECK(effects==before+1 && !xbox_kernel_replay_healthy());CHECK(xbox_kernel_replay_end(1));
    }
    /* Argument admission uses the pre-pop stack, rejects real waits and bad
     * pointers, and never admits the coroutine's yielding reference path. */
    g_esp=256;ram[268/4]=sizeof ram-4;
    CHECK(kernel_replay_supported(246,bridge_ObReferenceObjectByHandle));
    ram[268/4]=sizeof ram-3;CHECK(!kernel_replay_supported(246,bridge_ObReferenceObjectByHandle));
    ram[268/4]=0;coroutine=1;CHECK(!kernel_replay_supported(246,bridge_ObReferenceObjectByHandle));coroutine=0;
    ram[260/4]=1;CHECK(!kernel_replay_supported(159,bridge_KeWaitForSingleObject));ram[260/4]=0;
    ram[276/4]=sizeof ram-8;CHECK(kernel_replay_supported(159,bridge_KeWaitForSingleObject));
    ram[276/4]=sizeof ram-7;CHECK(!kernel_replay_supported(159,bridge_KeWaitForSingleObject));ram[276/4]=0;
    g_esp=UINT32_MAX;CHECK(!kernel_replay_supported(246,bridge_ObReferenceObjectByHandle));
    CHECK(!kernel_replay_supported(159,bridge_KeWaitForSingleObject));
    /* Execute the actual three production bridges: reference writes only its
     * guest result, null waits don't yield/block, dereference has no effects. */
    g_esp=260;ram[268/4]=512;ram[512/4]=0x12345678;
    uint32_t expected[1024];memcpy(expected,ram,sizeof ram);expected[512/4]=0;
    actual_ObReferenceObjectByHandle();CHECK(!g_eax && !memcmp(expected,ram,sizeof ram));
    actual_ObfDereferenceObject();CHECK(!g_eax && !memcmp(expected,ram,sizeof ram));
    ram[276/4]=sizeof ram-8;memcpy(expected,ram,sizeof ram);
    actual_KeWaitForSingleObject();CHECK(!g_eax && !memcmp(expected,ram,sizeof ram));
    ram[276/4]=0;ram[268/4]=0;memcpy(expected,ram,sizeof ram);
    actual_KeWaitForSingleObject();actual_ObReferenceObjectByHandle();
    CHECK(!g_eax && !memcmp(expected,ram,sizeof ram));
    /* The actual thread bridges write the suspend count and only primary
     * resume performs a directed handoff. Worker resume wakes without yield. */
    g_esp=260;ram[260/4]=0xBEEF1234;ram[264/4]=512;ram[512/4]=123;
    actual_NtResumeThread();CHECK(!g_eax && !ram[512/4] && thread_wakes==1 && thread_runs==1);
    fiber=2;actual_NtResumeThread();CHECK(thread_wakes==2 && thread_runs==1);
    actual_NtSuspendThread();CHECK(!g_eax);fiber=0;actual_NtSuspendThread();CHECK(!g_eax);
    for(unsigned ord=224;ord<=231;ord+=7){
        bridge_func_t fn=ord==224 ? bridge_NtResumeThread:bridge_NtSuspendThread;
        CHECK(xbox_kernel_replay_begin());unsigned n=effects;dispatch(ord,fn,8);
        CHECK(effects==n+1 && xbox_kernel_replay_end(0));
        thread_allowed=0;CHECK(xbox_kernel_replay_begin());dispatch(ord,fn,8);
        CHECK(effects==n+1 && xbox_kernel_replay_end(1));thread_allowed=1;
        g_esp=256;ram[264/4]=sizeof ram-3;CHECK(!kernel_replay_supported(ord,fn));ram[264/4]=512;
        CHECK(!kernel_replay_supported(ord,unsafe_bridge));
    }
    memset(ram,0,sizeof ram);
    const unsigned stub_ordinals[]={124,143,175};
    for(unsigned i=0;i<3;++i){
        unsigned ord=stub_ordinals[i];
        uint32_t va=1;bridge_func_t fn=NULL;int args=0;
        CHECK(kx_resolve(ord,&va,&fn,&args) && !va && fn==bridge_generic_stub && args==(ord==124?4:ord==143?8:12));
        unsigned n=effects;CHECK(xbox_kernel_replay_begin());dispatch(ord,fn,args);
        CHECK(!g_eax && effects==n && xbox_kernel_replay_end(0));
        CHECK(xbox_kernel_replay_begin());dispatch(ord,unsafe_bridge,8);
        CHECK(effects==n && xbox_kernel_replay_end(1));
        CHECK(!kernel_replay_supported(ord,NULL));
    }
    uint32_t phys_va=1;bridge_func_t phys_fn=NULL;int phys_args=0;
    CHECK(kx_resolve(173,&phys_va,&phys_fn,&phys_args) && !phys_va && phys_fn==bridge_MmGetPhysicalAddress && phys_args==4);
    const uint32_t addresses[]={0,0x12345678,0xFFFFFFFF};
    for(unsigned i=0;i<3;++i){
        ram[260/4]=addresses[i];memcpy(expected,ram,sizeof ram);
        CHECK(xbox_kernel_replay_begin());dispatch(173,phys_fn,phys_args);
        CHECK(g_eax==addresses[i] && !memcmp(expected,ram,sizeof ram) && xbox_kernel_replay_end(0));
    }
    CHECK(!kernel_replay_supported(173,unsafe_bridge));
    g_esp=UINT32_MAX;CHECK(!kernel_replay_supported(173,phys_fn));memset(ram,0,sizeof ram);
    /* Actual synchronization bridge + lifted callbacks: completion bits are
     * consumed once; the byte-pair callback performs its two RAM updates. */
    g_esp=256;ram[264/4]=0x1F3FFA;ram[268/4]=512;
    ram[(512+0x6F4)/4]=0x12;ram[(512+0x6FC)/4]=0x80;g_esi=0x1234;
    CHECK(xbox_kernel_replay_begin());dispatch(153,bridge_KeSynchronizeExecution,12);
    CHECK(g_eax==1 && g_esi==0x1234 && ram[(512+0x6F4)/4]==0 && ram[(512+0x6FC)/4]==0x92);
    CHECK(xbox_kernel_replay_end(0));
    ram[264/4]=0x1F8C51;MEM8(512+0x38)=3;MEM8(512+0x39)=7;MEM8(512+0x3A)=4;MEM8(512+0x3B)=9;
    CHECK(xbox_kernel_replay_begin());dispatch(153,bridge_KeSynchronizeExecution,12);
    CHECK(g_eax==1 && !MEM8(512+0x38) && MEM8(512+0x39)==10 && !MEM8(512+0x3A) && MEM8(512+0x3B)==13);
    CHECK(xbox_kernel_replay_end(0));
    g_esp=256;callback_remap=1;CHECK(!kernel_replay_supported(153,bridge_KeSynchronizeExecution));callback_remap=0;
    ram[264/4]=0x1234;CHECK(!kernel_replay_supported(153,bridge_KeSynchronizeExecution));
    ram[264/4]=0x1F3FFA;ram[268/4]=sizeof ram-0x6FF;CHECK(!kernel_replay_supported(153,bridge_KeSynchronizeExecution));
    ram[264/4]=0x1F8C51;ram[268/4]=sizeof ram-0x3B;CHECK(!kernel_replay_supported(153,bridge_KeSynchronizeExecution));
    ram[268/4]=512;g_esp=8;CHECK(!kernel_replay_supported(153,bridge_KeSynchronizeExecution));
    memset(ram,0,sizeof ram);
    /* Original-interval audit records all unsupported calls without stopping
     * normal dispatch or allowing an enforcing replay guard to overlap. */
    XboxKernelReplayAudit a;unsigned audit_effects=effects;
    CHECK(xbox_kernel_replay_audit_begin());CHECK(!xbox_kernel_replay_audit_begin());
    CHECK(!xbox_kernel_replay_begin());CHECK(!xbox_kernel_replay_audit_end(NULL));
    dispatch(236,unsafe_bridge,32);dispatch(236,unsafe_bridge,32);dispatch(126,bridge_KeQueryPerformanceCounter,0);
    CHECK(xbox_kernel_replay_check(UINT32_MAX,0));
    CHECK(effects==audit_effects+3 && xbox_kernel_replay_healthy());
    CHECK(xbox_kernel_replay_audit_end(&a));
    CHECK(a.accepted==1 && a.rejected==3 && a.unsupported[236]==2 && a.unknown==1 && !a.invalid);
    CHECK(!xbox_kernel_replay_audit_end(&a));
    CHECK(xbox_kernel_replay_audit_begin());
    HANDLE at=CreateThread(NULL,0,foreign_audit,NULL,0,NULL);CHECK(at);
    CHECK(WaitForSingleObject(at,5000)==WAIT_OBJECT_0);CloseHandle(at);
    CHECK(xbox_kernel_replay_audit_end(&a) && a.invalid && a.rejected==1);
    CHECK(xbox_kernel_replay_audit_begin());xbox_det_end();xbox_det_begin(&base);
    CHECK(xbox_kernel_replay_audit_end(&a) && a.invalid);
    CHECK(xbox_kernel_replay_begin());CHECK(!xbox_kernel_replay_audit_begin());
    dispatch(126,NULL,0);CHECK(!xbox_kernel_replay_end(0));CHECK(xbox_kernel_replay_end(1));
    CHECK(xbox_kernel_replay_begin());g_kernel_dispatch_slot=-1;g_esp=256;kernel_thunk_dispatch();
    CHECK(g_esp==260 && !xbox_kernel_replay_healthy());CHECK(xbox_kernel_replay_end(1));
    /* If an allowed call schedules a worker that faults, its bridge is skipped. */
    CHECK(xbox_kernel_replay_begin());fault_in_slice=1;g_fib_slice_due=1;unsigned before=effects;
    dispatch(126,bridge_KeQueryPerformanceCounter,0);CHECK(effects==before && !g_eax);
    fault_in_slice=0;g_fib_slice_due=0;CHECK(xbox_kernel_replay_end(1));
    CHECK(xbox_kernel_replay_begin());
    HANDLE th=CreateThread(NULL,0,foreign,NULL,0,NULL);CHECK(th);CHECK(WaitForSingleObject(th,5000)==WAIT_OBJECT_0);CloseHandle(th);
    XboxKernelReplayReport r;xbox_kernel_replay_report(&r);CHECK(r.reason==XBOX_REPLAY_KERNEL_OWNER);
    CHECK(xbox_kernel_replay_end(1));
    CHECK(xbox_kernel_replay_begin());fiber=3;CHECK(xbox_kernel_replay_check(126,1));CHECK(!xbox_kernel_replay_end(0));fiber=0;
    CHECK(xbox_kernel_replay_fail_stop(1));CHECK(xbox_kernel_replay_fail_stop(0));
    CHECK(xbox_kernel_replay_end(0));
    CHECK(!xbox_kernel_replay_frame_budget(8));
    for(unsigned frames=4;frames<=8;frames*=2){
        CHECK(xbox_kernel_replay_begin());
        CHECK(!xbox_kernel_replay_frame_budget(0) && !xbox_kernel_replay_frame_budget(9));
        fiber=3;CHECK(!xbox_kernel_replay_frame_budget(frames));fiber=0;
        uint64_t pin=xbox_kernel_replay_pin();CHECK(pin && !xbox_kernel_replay_frame_budget(frames));
        CHECK(xbox_kernel_replay_unpin(pin));
        CHECK(xbox_kernel_replay_fail_stop(1));CHECK(!xbox_kernel_replay_frame_budget(frames));CHECK(xbox_kernel_replay_fail_stop(0));
        CHECK(xbox_kernel_replay_frame_budget(frames));CHECK(!xbox_kernel_replay_frame_budget(frames));
        for(unsigned i=0;i<XBOX_REPLAY_KERNEL_CALL_LIMIT*frames;++i)CHECK(xbox_kernel_replay_check(126,1));
        CHECK(!xbox_kernel_replay_check(126,1));CHECK(!xbox_kernel_replay_frame_budget(8));
        XboxKernelReplayReport budget;xbox_kernel_replay_report(&budget);
        CHECK(budget.reason==XBOX_REPLAY_KERNEL_BUDGET && budget.accepted==XBOX_REPLAY_KERNEL_CALL_LIMIT*frames);
        CHECK(xbox_kernel_replay_end(1));
    }
    CHECK(xbox_kernel_replay_begin());CHECK(xbox_kernel_replay_check(126,1));
    CHECK(!xbox_kernel_replay_frame_budget(8));CHECK(xbox_kernel_replay_end(0));
    CHECK(xbox_kernel_replay_begin());
    for(unsigned i=0;i<XBOX_REPLAY_KERNEL_CALL_LIMIT;++i)CHECK(xbox_kernel_replay_check(126,1));
    CHECK(!xbox_kernel_replay_check(126,1));xbox_kernel_replay_report(&r);
    CHECK(r.reason==XBOX_REPLAY_KERNEL_BUDGET && r.accepted==XBOX_REPLAY_KERNEL_CALL_LIMIT);CHECK(xbox_kernel_replay_end(1));
    CHECK(xbox_kernel_replay_begin());xbox_det_end();xbox_det_begin(&base);
    CHECK(!xbox_kernel_replay_healthy() && !xbox_kernel_replay_end(1));
    xbox_kernel_replay_report(&r);CHECK(r.reason==XBOX_REPLAY_KERNEL_EPOCH);
    CHECK(!g_ktrace_pos); /* Optional flight recorder did no work. */
    puts("Kernel guard: actual dispatch rejects unsupported/remapped calls before effects, preserves stack cleanup, handles worker faults, owner/epoch/budget guards and sticky cancellation.");return 0;
}
