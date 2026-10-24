#include "kernel_interrupt.h"
#include "xbe_layout.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
typedef void (*recomp_func_t)(void);
static XboxInterruptState state;
static uint32_t ram[0x1000],esp=0x3000;
static unsigned char irql;
static int pending,callback_count;
static uint32_t order[16];
unsigned g_doa3_frames_presented;
#define MEM32(va) ram[(va)/4]
#define PUSH32(sp,value) do { (sp)-=4;MEM32(sp)=(value); } while(0)
int xbox_kernel_interrupt_begin(void) { return xbox_interrupt_begin(&state); }
void xbox_kernel_interrupt_end(void) { xbox_interrupt_end(&state); }
int xbox_kernel_get_isr(int i,uint32_t *obj,uint32_t *routine,uint32_t *ctx) {
    XboxIsrEntry e;if(i<0 || !xbox_interrupt_get_isr(&state,(unsigned)i,&e)) return 0;
    *obj=e.obj;*routine=e.routine;*ctx=e.ctx;return 1;
}
int xbox_kernel_pop_dpc(uint32_t *dpc,uint32_t *a1,uint32_t *a2) {
    XboxDpcEntry e;if(!xbox_interrupt_pop(&state,&e)) return 0;
    *dpc=e.dpc;*a1=e.a1;*a2=e.a2;return 1;
}
unsigned char xbox_current_irql(void) { return irql; }
unsigned char xbox_set_irql(unsigned char v) { unsigned char old=irql;irql=v;return old; }
int mcpx_apu_take_irq(void) { int old=pending;pending=0;return old; }
void doa3_apu_deliver_irq(void);
static void callback_guard(void) {
    XboxInterruptSnapshot p;CHECK(!xbox_interrupt_capture(&state,1,&p,sizeof(p)));
    CHECK(!xbox_interrupt_reset(&state) && !xbox_interrupt_begin(&state));
    CHECK(state.delivering);doa3_apu_deliver_irq();CHECK(state.delivering);
}
static void isr_callback(void) {
    callback_guard();CHECK(irql==0x0B && MEM32(esp+4)==0x1100 && MEM32(esp+8)==0x1200);
    CHECK(callback_count<16);order[callback_count++]=1;
    CHECK(xbox_interrupt_push(&state,(XboxDpcEntry){0x1000,17,23}));
}
static void dpc_callback(void) {
    callback_guard();CHECK(irql==2 && MEM32(esp+8)==0x1300);
    uint32_t dpc=MEM32(esp+4);CHECK(callback_count<16);order[callback_count++]=dpc;
    if(dpc==0x1000) {
        CHECK(MEM32(esp+12)==17 && MEM32(esp+16)==23);
        CHECK(xbox_interrupt_push(&state,(XboxDpcEntry){0x1020,31,37}));
    } else CHECK(dpc==0x1020 && MEM32(esp+12)==31 && MEM32(esp+16)==37);
}
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup(uint32_t va) {
    if(va==DOA3_SEC_DSOUND_VA+4) return isr_callback;
    if(va==DOA3_SEC_DSOUND_VA+8) return dpc_callback;
    return NULL;
}
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4210) /* Existing production function-local externs. */
#endif
#include "interrupt.generated.inc"
#ifdef _MSC_VER
#pragma warning(pop)
#endif
static DWORD WINAPI wrong_thread(void *unused) {
    (void)unused;XboxInterruptSnapshot p;
    CHECK(!xbox_interrupt_reset(&state) && !xbox_interrupt_begin(&state));
    CHECK(!xbox_interrupt_capture(&state,1,&p,sizeof(p)));
    CHECK(!xbox_interrupt_push(&state,(XboxDpcEntry){4,5,6}));return 0;
}
int main(void) {
    CHECK(xbox_interrupt_reset(&state));
    xbox_interrupt_register(&state,(XboxIsrEntry){0x1100,DOA3_SEC_DSOUND_VA+4,0x1200});
    MEM32(0x100C)=MEM32(0x102C)=DOA3_SEC_DSOUND_VA+8;MEM32(0x1010)=MEM32(0x1030)=0x1300;
    XboxInterruptSnapshot saved,actual,bad;CHECK(xbox_interrupt_capture(&state,1,&saved,sizeof(saved)));
    /* Original delivery body: masked IRQ stays pending, reentry denied, ISR
     * precedes FIFO DPCs, and callbacks can enqueue more work for this drain. */
    pending=1;irql=2;doa3_apu_deliver_irq();CHECK(pending && !callback_count);
    irql=0;doa3_apu_deliver_irq();CHECK(!pending && irql==0 && esp==0x3000 && callback_count==3);
    CHECK(order[0]==1 && order[1]==0x1000 && order[2]==0x1020 && !state.delivering);
    CHECK(xbox_interrupt_capture(&state,1,&actual,sizeof(actual)) && !memcmp(&saved,&actual,sizeof(saved)));
    doa3_apu_deliver_irq();CHECK(callback_count==3 && !state.delivering);
    CHECK(xbox_interrupt_push(&state,(XboxDpcEntry){0x1000,17,23}));
    CHECK(!xbox_interrupt_push(&state,(XboxDpcEntry){0x1000,999,999}));
    CHECK(xbox_interrupt_push(&state,(XboxDpcEntry){0x1020,31,37}));
    CHECK(xbox_interrupt_capture(&state,1,&saved,sizeof(saved)));
    for(unsigned replay=0;replay<3;++replay) {
        CHECK(xbox_interrupt_restore(&state,1,&saved,sizeof(saved)));
        XboxDpcEntry e;CHECK(xbox_interrupt_pop(&state,&e) && e.dpc==0x1000 && e.a1==17);
        CHECK(xbox_interrupt_pop(&state,&e) && e.dpc==0x1020 && e.a2==37);
        CHECK(!xbox_interrupt_pop(&state,&e));
    }
    CHECK(xbox_interrupt_capture(&state,1,&actual,sizeof(actual)));
    for(unsigned fault=0;fault<8;++fault) {
        bad=saved;
        switch(fault) {
        case 0:bad.version++;break;case 1:bad.epoch++;break;case 2:bad.generation++;break;
        case 3:bad.isr_count=5;break;case 4:bad.dpc_count=17;break;
        case 5:bad.dpc[1].dpc=bad.dpc[0].dpc;break;
        case 6:bad.isr[0].routine=0;break;case 7:bad.dpc[15].a1=1;break;
        }
        CHECK(!xbox_interrupt_validate(&state,1,&bad,sizeof(bad)));
        CHECK(!xbox_interrupt_restore(&state,1,&bad,sizeof(bad)));
        XboxInterruptSnapshot after;CHECK(xbox_interrupt_capture(&state,1,&after,sizeof(after)));
        CHECK(!memcmp(&actual,&after,sizeof(actual)));
    }
    CHECK(!xbox_interrupt_restore(&state,1,&saved,sizeof(saved)-1));
    CHECK(xbox_interrupt_begin(&state));CHECK(!xbox_interrupt_restore(&state,1,&saved,sizeof(saved)));xbox_interrupt_end(&state);
    HANDLE thread=CreateThread(NULL,0,wrong_thread,NULL,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,INFINITE)==WAIT_OBJECT_0);CloseHandle(thread);
    CHECK(xbox_interrupt_reset(&state));CHECK(!xbox_interrupt_restore(&state,1,&saved,sizeof(saved)));
    for(unsigned i=0;i<17;++i) CHECK(xbox_interrupt_push(&state,(XboxDpcEntry){i+1,2,3}));
    memset(&actual,0xA5,sizeof(actual));CHECK(state.tainted && !xbox_interrupt_capture(&state,1,&actual,sizeof(actual)));
    for(unsigned i=0;i<sizeof(actual);++i) CHECK(((unsigned char *)&actual)[i]==0xA5);
    CHECK(!xbox_interrupt_restore(&state,1,&saved,sizeof(saved)));
    CHECK(xbox_interrupt_reset(&state));
    for(unsigned i=0;i<5;++i) xbox_interrupt_register(&state,(XboxIsrEntry){i+1,2,3});
    CHECK(state.tainted && !xbox_interrupt_capture(&state,1,&actual,sizeof(actual)));
    puts("Interrupt checkpoint: callback order/reentry/masking verified with production delivery body; replay, malformed/stale images and overflow guards pass.");return 0;
}
