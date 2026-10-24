/* Control-flow equivalence only: native IO/audio/model helpers are fixtures. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "rollback_nested.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#undef RECOMP_ITAIL
#define RECOMP_ITAIL(va) CHECK(0) /* native fixture only uses enumerated table targets */
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8]; int g_fp_top;
static void *main_fiber;
typedef struct { uint32_t regs[8]; double fp[8]; int top; } Registers;
static Registers save_regs(void) {
    Registers r; memset(&r,0,sizeof(r));
    const uint32_t v[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};
    memcpy(r.regs,v,sizeof(v)); memcpy(r.fp,g_fp_stack,sizeof(r.fp)); r.top=g_fp_top;
    return r;
}
static void load_regs(Registers r) {
    g_eax=r.regs[0]; g_ecx=r.regs[1]; g_edx=r.regs[2]; g_esp=r.regs[3];
    g_ebx=r.regs[4]; g_esi=r.regs[5]; g_edi=r.regs[6]; g_seh_ebp=r.regs[7];
    memcpy(g_fp_stack,r.fp,sizeof(r.fp)); g_fp_top=r.top;
}
static void fixture_helper(uint32_t routine) {
    MEM32(0x1000000+routine*4)++;
    if(routine==0x85C40) g_eax=0; /* choose the pending resource */
    if(routine==0x1907A0) g_eax=MEM32(0x62000) ? 3 : 1;
    if(routine==0x8ABF0) g_fp_stack[--g_fp_top & 7]=1.0;
    g_esp+=4;
}
void doa3_rollback_task_sleep(void) {
    CHECK(MEM32(g_esp+4)==1);
    g_eax=0xF1BE0000; g_ecx=0x73C400; g_edx=1;
    MEM32(0x73C404)=1; g_esp+=4;
}
void sub_001C65D8(void) { doa3_rollback_task_sleep(); SwitchToFiber(main_fiber); }
#include "flow.generated.inc"
static void CALLBACK original(void *unused) { (void)unused; sub_00087420(); CHECK(0); }
static void initialize(void) {
    const uint32_t targets[]={0x875AB,0x875B2,0x875B9,0x875DC,0x875E3};
    for(unsigned i=0;i<5;++i) MEM32(0x8760C+4*i)=targets[i];
    MEM32(0x5B2110)=0x55000; MEM32(0xA48CD0)=60; MEM32(0xA48C2C)=60;
    MEMF(0x2FFFE0)=1; MEMF(0x2FFFE4)=0;
}
static void input(unsigned frame) {
    MEM32(0x62000)=frame>=12; /* release the nested resource wait */
    MEM8(0x598E80)=(unsigned char)(frame%5);
    MEM32(0x5C931C)=frame%3;
    MEM32(0x5C9448)=frame*3;
    MEM8(0x5C93AA)=frame%2; MEM8(0x5C93D1)=frame%3;
    MEM8(0x59CF58)=(unsigned char)frame; MEM8(0x59CFC0)=(unsigned char)(frame+1);
}
int main(void) {
    const size_t bytes=128u*1024u*1024u;
    unsigned char *native=guest_memory_fixture_alloc(bytes), *converted=guest_memory_fixture_alloc(bytes);
    unsigned char *checkpoint=malloc(bytes); CHECK(native && converted && checkpoint);
    memset(native,0,bytes); memset(converted,0,bytes);
    g_xbox_mem_offset=(ptrdiff_t)native; initialize();
    g_xbox_mem_offset=(ptrdiff_t)converted; initialize();
    Registers nr={0}, cr, saved_regs;
    nr.regs[3]=0x100000; nr.regs[7]=123; nr.top=4; cr=nr;
    Doa3NestedStack stack,saved,final;
    CHECK(doa3_nested_init(&stack,0x87420,nested_validate));
    main_fiber=ConvertThreadToFiber(NULL); CHECK(main_fiber);
    void *fiber=CreateFiber(0,original,NULL); CHECK(fiber);
    unsigned depth_three=0, root_yields=0;
    for(unsigned frame=0;frame<60;++frame) {
        g_xbox_mem_offset=(ptrdiff_t)native; load_regs(nr); input(frame);
        SwitchToFiber(fiber); nr=save_regs();
        g_xbox_mem_offset=(ptrdiff_t)converted; load_regs(cr); input(frame);
        CHECK(doa3_nested_advance(&stack,nested_dispatch,nested_validate,100)==DOA3_NESTED_YIELDED);
        cr=save_regs();
        CHECK(!memcmp(&nr,&cr,sizeof(nr)) && !memcmp(native,converted,bytes));
        if(stack.depth==3) ++depth_three;
        if(stack.depth==1) ++root_yields;
        if(frame==5) {
            CHECK(stack.depth==3 && stack.frames[1].routine==0x87620 && stack.frames[2].routine==0x873A0);
            CHECK(doa3_nested_capture(&stack,&saved,sizeof(saved),nested_validate));
            memcpy(checkpoint,converted,bytes); saved_regs=cr;
        }
    }
    DeleteFiber(fiber); CHECK(depth_three==12 && root_yields==48);
    const uint32_t branches[]={0x87D00,0x87EB0,0x88000,0x88570,0x885E0,0x89240};
    for(unsigned i=0;i<6;++i) CHECK(MEM32(0x1000000+branches[i]*4)>0);
    final=stack;
    CHECK(doa3_nested_restore(&stack,&saved,sizeof(saved),nested_validate));
    memcpy(converted,checkpoint,bytes); load_regs(saved_regs);
    for(unsigned frame=6;frame<60;++frame) {
        input(frame); CHECK(doa3_nested_advance(&stack,nested_dispatch,nested_validate,100)==DOA3_NESTED_YIELDED);
    }
    cr=save_regs();
    CHECK(!memcmp(&nr,&cr,sizeof(nr)) && !memcmp(native,converted,bytes));
    CHECK(!memcmp(&stack,&final,sizeof(stack)));
    /* A corrupted local switch target cannot escape into a native tail call. */
    MEM8(0x598E80)=0; MEM32(0x8760C)=0xDEAD;
    CHECK(doa3_nested_advance(&stack,nested_dispatch,nested_validate,100)==DOA3_NESTED_FAILED);
    CHECK(!doa3_nested_capture(&stack,&saved,sizeof(saved),nested_validate));
    CHECK(ConvertFiberToThread());
    guest_memory_fixture_free(native); guest_memory_fixture_free(converted); free(checkpoint);
    puts("Actual main-flow/setup/wait bodies match 60 native yields; depth-three restore reproduces 54 steps; all five local switch cases covered, unknown target rejected.");
    return 0;
}
