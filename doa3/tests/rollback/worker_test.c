/* Real generated roots + D3D wait, production explicit adapter. Service calls
 * are deterministic fixtures; this does not certify their live external IO. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "rollback_workers.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
volatile int g_xbox_det_active,g_fib_slice_due;
volatile uint32_t g_icall_trace[16],g_icall_trace_idx;
volatile uint64_t g_icall_count;
static void *primary;
static unsigned routine;
static unsigned kernel_calls;
void xbox_det_on_kernel_call(unsigned ordinal) { CHECK(ordinal==159 || ordinal==231); if(g_xbox_det_active) ++kernel_calls; }
uint32_t xbox_fiber_current_xhandle(void) { return 0xBEEF9004; }
typedef struct { uint32_t v[8]; } Registers;
static Registers save(void) { Registers r={{g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp}}; return r; }
static void load(Registers r) {
    g_eax=r.v[0];g_ecx=r.v[1];g_edx=r.v[2];g_esp=r.v[3];
    g_ebx=r.v[4];g_esi=r.v[5];g_edi=r.v[6];g_seh_ebp=r.v[7];
}
void xbox_fiber_timeslice(void) { CHECK(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va; CHECK(0); }
static void native_kernel_wait(void) {
    xbox_det_on_kernel_call(159);
    g_esp+=4; // thunk pops return before entering the bridge
    uint32_t object=MEM32(g_esp);
    CHECK(object==0x702430 && MEM32(g_esp+4)==6 && MEM32(g_esp+8)==1 && !MEM32(g_esp+12) && !MEM32(g_esp+16));
    SwitchToFiber(primary);
    MEM32(object+4)=0; MEM32(0x70245C)++; g_eax=0; g_esp+=20;
}
static void callback(void) { MEM32(0x60008)=MEM32(0x60008)*17+MEM32(g_esp+4); g_eax^=71; g_esp+=4; }
static void native_kernel_suspend(void) {
    xbox_det_on_kernel_call(231); g_esp+=4;
    uint32_t handle=MEM32(g_esp),prev=MEM32(g_esp+4);
    CHECK(prev==g_esp+16); MEM32(prev)=0;
    if(!handle || handle==0xFFFFFFFEu) handle=xbox_fiber_current_xhandle();
    if(handle==xbox_fiber_current_xhandle()) SwitchToFiber(primary);
    g_eax=0;g_esp+=8;
}
recomp_func_t recomp_lookup(uint32_t va) { if(va==0x123456) return callback; if(va==0xFE000000) return native_kernel_wait; if(va==0xFE000004) return native_kernel_suspend; return NULL; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va;return NULL; }
static void service(unsigned id) { MEM32(0x60000+4*id)++; g_eax=MEM32(0x60000+4*id)*13;g_ecx^=g_eax;g_edx+=id+2;g_esp+=4; }
void sub_0019A640(void) { service(0); }
void sub_0019A660(void) { service(1); }
void sub_0019A690(void) {
    MEM32(0x60010)++;g_eax=MEM32(0x60010)%3==1;
    if(MEM32(0xC80078)==0xBEEF9999) {
        /* Exercise a non-self suspend, then a true park in the same step. */
        MEM32(0x60018)++;g_eax=0;
        if(!(MEM32(0x60018)%2)) MEM32(0xC80078)=xbox_fiber_current_xhandle();
    }
    g_ecx^=g_eax;g_esp+=4;
}
void sub_0018C436(void) { MEM32(0x60014)+=MEM32(g_esp+8);g_eax=0;g_esp+=12; }
void sub_0019A560(void) { CHECK(0); }
void sub_0018CA9E(void) { CHECK(0); }
void sub_0018C548(void) { MEM32(0x6000C)+=MEM32(g_esp+4);g_eax=0;g_esp+=8; }
void sub_0018C5D7(void) { CHECK(0); }
void sub_001DC660(void);
void sub_0018C522(void);
#include "workers.generated.inc"
static void CALLBACK original(void *unused) {
    (void)unused;
    if(routine==0x1926F0) original_001926F0(); else if(routine==0x192760) original_00192760(); else original_001927D0();
    CHECK(0);
}
static void input(unsigned frame) {
    MEM32(0x40C710)=frame%3 ? 0x123456 : 0;
    MEM32(0x40C714)=frame*19;
    MEM32(0xC80078)=routine==0x1927D0 ? (frame%4==0 ? 0 : frame%4==1 ? 0xFFFFFFFEu : frame%4==2 ? xbox_fiber_current_xhandle() : 0xBEEF9999) : frame+100;
    MEM32(0x40C6FC)=frame%2;
    MEM32(0x40C6F4)=frame+9;
}
int main(void) {
    const size_t bytes=128u*1024u*1024u;
    unsigned char *native=guest_memory_fixture_alloc(bytes),*converted=guest_memory_fixture_alloc(bytes);
    unsigned char *checkpoint=malloc(bytes); CHECK(native && converted && checkpoint);
    primary=ConvertThreadToFiber(NULL); CHECK(primary);
    for(unsigned mode=0;mode<2;++mode) for(unsigned which=0;which<3;++which) {
        routine=which==2 ? 0x1927D0 : which ? 0x192760 : 0x1926F0; g_xbox_det_active=(int)mode;
        memset(native,0,bytes); memset(converted,0,bytes);
        *(uint32_t *)(native+0x1EDE80)=*(uint32_t *)(converted+0x1EDE80)=0x700000;
        *(uint32_t *)(native+0x217B10)=*(uint32_t *)(converted+0x217B10)=0xFE000000;
        *(uint32_t *)(native+0x217AE4)=*(uint32_t *)(converted+0x217AE4)=0xFE000004;
        Registers n={{0,0,0,0x100000,0,0,0,0xAABBCCDD}},c=n,saved;
        Doa3RollbackWorker worker={0},saved_worker;
        unsigned native_calls=0,converted_calls=0,saved_calls=0;
        void *fiber=CreateFiber(0,original,NULL); CHECK(fiber);
        for(unsigned frame=0;frame<40;++frame) {
            kernel_calls=native_calls;
            g_xbox_mem_offset=(ptrdiff_t)native; load(n);input(frame);SwitchToFiber(fiber);n=save();native_calls=kernel_calls;
            kernel_calls=converted_calls;
            g_xbox_mem_offset=(ptrdiff_t)converted;load(c);input(frame);
            CHECK(doa3_rollback_worker_step(routine,&worker));c=save();
            converted_calls=kernel_calls; CHECK(native_calls==converted_calls);
            CHECK(doa3_rollback_worker_checkpoint_valid(routine,&worker));
            CHECK(worker.wait_key==(which==2 ? xbox_fiber_current_xhandle() : mode?0:0xB1A4B1A4u));
            CHECK(!memcmp(&n,&c,sizeof(n)) && !memcmp(native,converted,bytes));
            if(frame==19) { memcpy(checkpoint,converted,bytes); saved=c;saved_worker=worker;saved_calls=kernel_calls; }
        }
        for(unsigned replay=0;replay<2;++replay) {
            memcpy(converted,checkpoint,bytes);load(saved);worker=saved_worker;
            kernel_calls=saved_calls;
            for(unsigned frame=20;frame<40;++frame) { input(frame);CHECK(doa3_rollback_worker_step(routine,&worker)); }
            CHECK(kernel_calls==native_calls);
            Registers result=save(); CHECK(!memcmp(&result,&n,sizeof(n)) && !memcmp(native,converted,bytes));
        }
        DeleteFiber(fiber);
    }
    /* Suspending another thread returns immediately, without a new park. */
    memset(native,0,bytes);memset(converted,0,bytes);
    *(uint32_t *)(native+0x217AE4)=*(uint32_t *)(converted+0x217AE4)=0xFE000004;
    *(uint32_t *)(native+0x100004)=*(uint32_t *)(converted+0x100004)=0xBEEF9999;
    Registers initial={{11,22,33,0x100000,44,55,66,0xABCDEF}};
    g_xbox_mem_offset=(ptrdiff_t)native;load(initial);kernel_calls=0;sub_0018C522();Registers expected=save();
    unsigned expected_calls=kernel_calls;
    g_xbox_mem_offset=(ptrdiff_t)converted;load(initial);kernel_calls=0;Doa3RollbackWorker immediate={0};
    CHECK(doa3_rollback_worker_suspend_prepare(&immediate) && !immediate.wait_key);
    CHECK(!doa3_rollback_worker_checkpoint_valid(0x1927D0,&immediate));
    CHECK(doa3_rollback_worker_suspend_finish(&immediate));Registers actual=save();
    CHECK(kernel_calls==expected_calls && !memcmp(&actual,&expected,sizeof(actual)) && !memcmp(native,converted,bytes));
    puts("Three streaming roots match original Windows-fiber waits in both clock modes; midpoint replay, kernel accounting and non-self suspend match.");
    free(checkpoint);guest_memory_fixture_free(native);guest_memory_fixture_free(converted);
    return 0;
}
