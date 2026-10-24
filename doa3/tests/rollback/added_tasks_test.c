/* Root conversion equivalence, not a replacement for live helper/APU replay. */
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
/* These fixtures exercise root branching, register clobbers and call-stack
 * cleanup; production helper results are checked by the separate shadow run. */
static void helper(unsigned id) {
    MEM32(0x60000+4*id)++;
    g_eax=MEM32(0x60000+4*id)*17+id;
    g_ecx^=g_eax; g_edx+=id+1; g_esp+=4;
}
void sub_0012E27E(void) { helper(0); }
void sub_000BE2F0(void) { helper(1); }
void sub_000BE4E0(void) { helper(2); }
void sub_000BF3B0(void) { helper(3); }
void sub_000BB5F0(void) { helper(4); }
void sub_001C1B63(void) { helper(5); }
void sub_000BB660(void) { helper(6); }
void sub_001C1705(void);
void sub_001C5F54(void);
void sub_001C1D0F(void) {
    /* Explicit stand-in for model/render updates. Consume the actual helper's
     * three arguments so single/tag argument order affects compared memory. */
    MEM32(0x60020)=MEM32(0x60020)*33+MEM32(g_esp+4);
    MEM32(0x60024)=MEM32(0x60024)*33+MEM32(g_esp+8);
    MEM32(0x60028)=MEM32(0x60028)*33+MEM32(g_esp+12);
    helper(7);
}
void doa3_rollback_task_sleep(void) {
    CHECK(MEM32(g_esp+4)==1);
    g_ecx=0x73C400; g_edx=MEM32(g_esp+4);
    MEM32(g_ecx)&=~0x10u; MEM32(g_ecx+4)=g_edx;
    g_eax=MEM32(0x73C640); g_esp+=4;
}
void sub_001C65D8(void) { doa3_rollback_task_sleep(); SwitchToFiber(main_fiber); }
#include "added_tasks.generated.inc"
static void CALLBACK original_thread(void *unused) {
    (void)unused;
    if(routine==0x54110) original_00054110();
    else if(routine==0xBE4A0) original_000BE4A0();
    else original_001C60AB();
    CHECK(0);
}
static void advance(Doa3RollbackTask *task) {
    task->yielded=0;
    if(routine==0x54110) step_00054110(task);
    else if(routine==0xBE4A0) step_000BE4A0(task);
    else step_001C60AB(task);
    CHECK(task->resume==1 && task->yielded==1);
}
static void input(unsigned frame) {
    MEM8(0xC59865)=2;
    MEM8(0x59D086)=(frame%3==0 ? 0x35 : 0x12);
    MEM8(0x591F50)=(frame%2);
    MEM32(0x26CCBC)=frame*31;
    MEM8(0x5A2858)=(frame%3==0 ? 7 : 5);
    MEM8(0x596A37)=(frame%2 ? 4 : 1);
    MEM8(0x5B1400)=(frame%4<2);
    MEM8(0xC59866)=frame%2; /* single/tag helper paths */
    MEM8(0x5C93AA)=frame%3==0;
    MEM8(0x59CFAA)=frame%4!=0; /* history copy enabled/disabled */
    MEM8(0x5A2815)=(unsigned char)(frame*3);
    MEM32(0x5B1D2C)=frame*19;
    for(unsigned player=0;player<2;++player) {
        for(unsigned field=0;field<3;++field)
            MEM8(0x59CF59+player*0x68+field)=(unsigned char)(frame+player*7+field);
        for(unsigned slot=0;slot<2;++slot) {
            MEM8(0x5A2620+player*5+slot)=(unsigned char)(frame+player+slot);
            MEM8(0x59CF44+player*5+slot)=(unsigned char)(frame*3+player+slot);
        }
    }
}
int main(void) {
    const size_t bytes=128u*1024u*1024u;
    unsigned char *native=guest_memory_fixture_alloc(bytes);
    unsigned char *explicit=guest_memory_fixture_alloc(bytes), *checkpoint=malloc(bytes);
    CHECK(native && explicit && checkpoint);
    main_fiber=ConvertThreadToFiber(NULL); CHECK(main_fiber);
    const unsigned routines[]={0x54110,0xBE4A0,0x1C60AB};
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
            if(routine==0x1C60AB) {
                CHECK(MEM32(0x9B36A4)==0x599078);
                CHECK(MEM8(0x9B36A1)==MEM8(0x5A2815));
                CHECK(MEM8(0x9B3710)==MEM8(0x5C93AA));
                CHECK(MEM8(0x9B36A0)==MEM8(0xC59865));
                if(frame%4!=0)
                    for(unsigned player=0;player<2;++player)
                        for(unsigned field=0;field<3;++field)
                            CHECK(MEM8(0x9B3714+player*4+field)==MEM8(0x59CF59+player*0x68+field));
                if(frame%3==0) CHECK(MEM32(0x59C5C0)==MEM32(0x5B1D2C));
            }
            if(frame==20) { memcpy(checkpoint,explicit,bytes); saved=converted; saved_task=task; }
        }
        DeleteFiber(fiber);
        if(t==0) { CHECK(MEM32(0x592FB0)==60 && MEM32(0x60000)==10); }
        else if(t==1) { CHECK(MEM32(0x60004)==1 && MEM32(0x60008)==59 && MEM32(0x6000C)>0); }
        else { CHECK(MEM32(0x60010)==1 && MEM32(0x60014)==1 && MEM32(0x60018)==60 && MEM32(0x6001C)==180); }
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
    puts("Three actual task roots matched native fibers for 60 steps and 39 restored steps, including all RAM.");
    return 0;
}
