/* Compare native fibers with explicit continuations. External rendering and
 * scene helpers below are deterministic fixtures, NOT a rendering backend. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "rollback_tasks.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
static void *main_fiber;
static unsigned routine;
typedef struct { uint32_t v[8]; } Registers;
static Registers save_regs(void) {
    Registers r={{g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp}}; return r;
}
static void load_regs(Registers r) {
    g_eax=r.v[0]; g_ecx=r.v[1]; g_edx=r.v[2]; g_esp=r.v[3];
    g_ebx=r.v[4]; g_esi=r.v[5]; g_edi=r.v[6]; g_seh_ebp=r.v[7];
}
static void helper(unsigned id) {
    MEM32(0x60000+4*id)++;
    g_eax=MEM32(0x60000+4*id)*17+id;
    g_ecx^=g_eax; g_edx+=id+1; g_esp+=4;
}
void sub_000E8100(void);
void sub_000E8830(void);
void sub_000E8180(void) { helper(0); }
void sub_000E8290(void) { helper(1); }
void sub_000E8590(void) { helper(2); }
void sub_000B46E0(void) { unsigned side=g_edx; helper(3); g_eax=side+2; }
void sub_00054700(void) { helper(4); }
void sub_00180510(void) { helper(5); }
void sub_0006CAC0(void) { helper(6); }
void sub_000700E0(void) {
    /* Exercise caller-owned stack output and the optional follow-up call. */
    uint32_t out=MEM32(g_esp+4);
    CHECK(out==g_esp+16);
    MEM32(out)=MEM32(0x61000)*19+7;
    helper(7); g_eax=MEM32(0x61000)%2;
    g_esp+=4; /* stdcall one argument */
}
void sub_00068ED0(void) { MEM32(0x61004)=MEM32(g_esp+4); helper(8); }
void sub_00069600(void) { CHECK(MEM32(g_esp+4)==0x5A8A54); helper(9); }
void sub_000693F0(void) { CHECK(g_esi==0x5A8A54); helper(10); }
void sub_00069E30(void) { helper(11); }
void sub_00070450(void) { CHECK(g_ebx==0x5AA1A8); helper(12); }
void sub_0006BDD0(void) { helper(13); }
void sub_0006C520(void) { helper(14); }
void sub_0006C5A0(void) { helper(15); }
void sub_0006C900(void) { helper(16); }
void sub_000694B0(void) { CHECK(g_eax==0x5A8A54); helper(17); }
void sub_00069560(void) { CHECK(g_eax==0x5A8A54); helper(18); }
void sub_00069330(void) { helper(19); }
/* 49790's helpers remain fixture-only here; the live capture checks their
 * actual effects. Verify initialization is once and per-frame calls retain
 * their order, guest register effects and memory writes across restoration. */
void sub_000497B0(void) { MEM32(0x6100C)=0; helper(21); }
void sub_000498E0(void) { MEM32(0x6100C)=MEM32(0x6100C)*33+MEM32(0x61000); helper(22); }
void sub_00049AE0(void) { MEM32(0x6100C)^=g_eax; helper(23); }
static void fixture_indirect(uint32_t va) {
    CHECK(va==0x12340 || va==0x12350);
    MEM32(0x61008)=MEM32(0x61008)*33+MEM32(g_esp+4)+va;
    helper(20);
}
#undef RECOMP_ICALL_SAFE
#define RECOMP_ICALL_SAFE(va,sp) fixture_indirect(va)
void doa3_rollback_task_sleep(void) {
    CHECK(MEM32(g_esp+4)==1);
    g_ecx=0x73C400; g_edx=MEM32(g_esp+4);
    MEM32(g_ecx)&=~0x10u; MEM32(g_ecx+4)=g_edx;
    g_eax=MEM32(0x73C640); g_esp+=4;
}
void sub_001C65D8(void) { doa3_rollback_task_sleep(); SwitchToFiber(main_fiber); }
#include "display.generated.inc"
static void CALLBACK original_thread(void *unused) {
    (void)unused;
    if(routine==0xE8030) original_000E8030();
    else if(routine==0x49790) original_00049790();
    else original_000691F0();
    CHECK(0);
}
static void advance(Doa3RollbackTask *task) {
    task->yielded=0;
    if(routine==0xE8030) step_000E8030(task);
    else if(routine==0x49790) step_00049790(task);
    else step_000691F0(task);
    CHECK(task->resume==1 && task->yielded==1);
}
static void input(unsigned frame) {
    MEM32(0x61000)=frame;
    MEM8(0xC59865)=frame%3;
    MEM8(0xC59863)=frame%2;
    for(unsigned i=0;i<3;++i) MEM8(0x59D084+i)=(unsigned char)(frame+i);
    MEM8(0x599878)=frame%7==0;
    MEM8(0x596AA5)=frame%2;
    MEM8(0x596A37)=frame%3==0 ? 4 : 1;
    MEM8(0x59CFAA)=frame%4!=0;
    MEM8(0x59D012)=frame%4<2;
    MEM8(0x5C93AA)=frame%2;
    MEM8(0x59CF34)=frame%5<2;
    MEM32(0x592FB0)=frame;
    MEM8(0x9BF68E)=0;
    /* Deliberately mix unchanged/changed cached stage and tick values. */
    if(frame%3==1) {
        MEM8(0x9BF678)=MEM8(0xC59865);
        MEM8(0x9BF679)=MEM8(0x59D084+MEM8(0xC59865));
    }
    MEM32(0x9BF674)=frame%2 ? frame : frame+1;
    MEM32(0x9BF684)=frame%2 ? 0x12340 : 0;
    MEM32(0x9BF688)=frame+3;
    MEM8(0x59D0D0)=0;
    MEM32(0x404AD0)=frame%2;
    MEM32(0x404AD4)=frame%3;
    MEM32(0x404ADC)=frame%2;
    MEM32(0x404AE0)=0x62000;
    MEM32(0x303DB0)=0x12340; MEM32(0x303DB4)=0x12350;
    for(unsigned i=0;i<3;++i) MEM32(0x62000+i*24+8)=frame%3!=0;
    MEM8(0x5A8A85)=frame%3==0;
    MEM8(0x59D080)=frame%2;
    MEM16(0x5A8D06)=(uint16_t)(frame*11);
}
int main(void) {
    const size_t bytes=128u*1024u*1024u;
    unsigned char *native=guest_memory_fixture_alloc(bytes);
    unsigned char *explicit=guest_memory_fixture_alloc(bytes), *checkpoint=malloc(bytes);
    CHECK(native && explicit && checkpoint);
    main_fiber=ConvertThreadToFiber(NULL); CHECK(main_fiber);
    const unsigned routines[]={0xE8030,0x49790,0x691F0};
    for(unsigned t=0;t<3;++t) {
        routine=routines[t]; memset(native,0,bytes); memset(explicit,0,bytes);
        Registers original={{0,0,0,0x100000,0,0,0,0xDEADBEEF}}, converted=original, saved;
        Doa3RollbackTask task={0}, saved_task;
        void *fiber=CreateFiber(0,original_thread,NULL); CHECK(fiber);
        for(unsigned frame=0;frame<60;++frame) {
            g_xbox_mem_offset=(ptrdiff_t)native; load_regs(original); input(frame);
            SwitchToFiber(fiber); original=save_regs();
            g_xbox_mem_offset=(ptrdiff_t)explicit; load_regs(converted); input(frame);
            advance(&task); converted=save_regs();
            CHECK(!memcmp(&original,&converted,sizeof(original)));
            CHECK(!memcmp(native,explicit,bytes));
            if(routine==0xE8030) {
                CHECK(MEM8(0x596AA5)==frame%2);
                CHECK(MEM8(0x9BF678)==MEM8(0xC59865));
                CHECK(MEM32(0x9BF674)==frame);
                CHECK(MEM32(0x9BF684)==0);
            } else if(routine==0x691F0) {
                CHECK(MEM16(0x5A8D04)==frame*11 && MEM16(0x5A8D06)==0);
                if(frame%2) CHECK(MEM32(0x61004)==frame*19+7);
            }
            if(frame==20) { memcpy(checkpoint,explicit,bytes); saved=converted; saved_task=task; }
        }
        DeleteFiber(fiber);
        if(routine==0xE8030) {
            CHECK(MEM32(0x60000)>0 && MEM32(0x60004)>0 && MEM32(0x60008)>0);
            CHECK(MEM32(0x6000C)>0 && MEM32(0x60050)>0);
        } else if(routine==0x691F0) {
            CHECK(MEM32(0x60010)==1 && MEM32(0x60014)==60 && MEM32(0x6001C)==60);
            CHECK(MEM32(0x60024)>0 && MEM32(0x60028)>0 && MEM32(0x6002C)>0);
            CHECK(MEM32(0x6004C)>0 && MEM32(0x60050)>0);
        } else {
            CHECK(MEM32(0x60054)==1 && MEM32(0x60058)==59 && MEM32(0x6005C)==59);
        }
        Doa3RollbackTask final_task=task;
        memcpy(explicit,checkpoint,bytes); load_regs(saved); task=saved_task;
        for(unsigned frame=21;frame<60;++frame) { input(frame); advance(&task); }
        converted=save_regs();
        CHECK(!memcmp(&original,&converted,sizeof(original)));
        CHECK(!memcmp(native,explicit,bytes));
        CHECK(!memcmp(&task,&final_task,sizeof(task)));
    }
    CHECK(ConvertFiberToThread());
    guest_memory_fixture_free(native); guest_memory_fixture_free(explicit); free(checkpoint);
    puts("Three actual display-task roots matched native fibers for 60 steps and 39 restored steps, including all RAM; external rendering remains fixture-only.");
    return 0;
}
