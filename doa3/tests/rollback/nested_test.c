/* Real translated 72FE0 -> 85820 nested wait, driven by fixture side effects.
 * Tests native-stack removal, not the omitted rendering/audio helpers. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "recomp_types.h"
#include "rollback_nested.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
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
    MEM32(0x62000+(routine%256)*4)++;
    /* This helper returns an x87 value consumed by the real caller. */
    if(routine==0x8ABF0) g_fp_stack[--g_fp_top & 7]=1.0;
    g_esp+=4;
}
void doa3_rollback_task_sleep(void) {
    CHECK(MEM32(g_esp+4)==1);
    g_eax=0xF1BE0000; g_ecx=0x73C400; g_edx=1;
    MEM32(0x73C404)=1; g_esp+=4;
}
void sub_001C65D8(void) { doa3_rollback_task_sleep(); SwitchToFiber(main_fiber); }
#include "nested.generated.inc"
static void CALLBACK original(void *unused) { (void)unused; sub_00001000(); CHECK(0); }
static void input(unsigned frame) {
    MEM8(0x5B20B0)=(frame%7<3);
    MEM8(0x5A2858)=7; MEM8(0x596A74)=1; MEM8(0x598E80)=2;
    MEM8(0x5B1340)=1; MEM32(0x5B1424)=1;
}
static int synthetic_validate(uint32_t routine,uint32_t resume) {
    if(routine!=1) return 0;
    return resume==0 ? DOA3_NESTED_ENTRY : resume==1 ? DOA3_NESTED_CALL_POINT : 0;
}
static Doa3NestedAction recurse(Doa3NestedFrame *frame) {
    frame->resume=1; return (Doa3NestedAction){DOA3_NESTED_CALL,1};
}
static Doa3NestedAction tail_loop(Doa3NestedFrame *frame) {
    (void)frame; return (Doa3NestedAction){DOA3_NESTED_TAIL,1};
}
static void rejection_tests(Doa3NestedStack valid) {
    Doa3NestedStack target=valid, bad, before;
    for(unsigned i=0;i<7;++i) {
        bad=valid;
        if(i==0) bad.schema++;
        if(i==1) bad.depth=DOA3_NESTED_DEPTH+1;
        if(i==2) bad.frames[bad.depth-1].resume=99999;
        if(i==3) bad.frames[0].resume=0; /* waiting caller cannot be at entry */
        if(i==4) bad.frames[bad.depth].routine=1;
        if(i==5) bad.status=DOA3_NESTED_READY;
        if(i==6) bad.reserved=1;
        before=target;
        CHECK(!doa3_nested_restore(&target,&bad,sizeof(bad),nested_validate));
        CHECK(!memcmp(&target,&before,sizeof(target)));
    }
    CHECK(!doa3_nested_restore(&target,&valid,sizeof(valid)-1,nested_validate));
    CHECK(doa3_nested_init(&target,1,synthetic_validate));
    CHECK(doa3_nested_advance(&target,recurse,synthetic_validate,100)==DOA3_NESTED_FAILED);
    before=target;
    CHECK(doa3_nested_advance(&target,recurse,synthetic_validate,100)==DOA3_NESTED_FAILED);
    CHECK(!memcmp(&target,&before,sizeof(target)));
    CHECK(doa3_nested_init(&target,1,synthetic_validate));
    CHECK(doa3_nested_advance(&target,tail_loop,synthetic_validate,100)==DOA3_NESTED_FAILED);
    CHECK(target.depth==1); /* tail calls do not consume stack capacity */
}
int main(void) {
    const size_t bytes=128u*1024u*1024u;
    unsigned char *native=guest_memory_fixture_alloc(bytes), *converted=guest_memory_fixture_alloc(bytes);
    unsigned char *memory_checkpoint=malloc(bytes); CHECK(native && converted && memory_checkpoint);
    memset(native,0,bytes); memset(converted,0,bytes);
    Registers native_regs={0}, converted_regs, saved_regs;
    native_regs.regs[3]=0x100000; native_regs.regs[7]=123; native_regs.top=4; converted_regs=native_regs;
    Doa3NestedStack stack,saved,final;
    CHECK(doa3_nested_init(&stack,0x1000,nested_validate));
    CHECK(!doa3_nested_capture(&stack,&saved,sizeof(saved),nested_validate));
    main_fiber=ConvertThreadToFiber(NULL); CHECK(main_fiber);
    void *fiber=CreateFiber(0,original,NULL); CHECK(fiber);
    unsigned checkpoint_frame=0, deepest=0, returns_to_root=0;
    for(unsigned frame=0;frame<60;++frame) {
        g_xbox_mem_offset=(ptrdiff_t)native; load_regs(native_regs); input(frame);
        SwitchToFiber(fiber); native_regs=save_regs();
        g_xbox_mem_offset=(ptrdiff_t)converted; load_regs(converted_regs); input(frame);
        CHECK(doa3_nested_advance(&stack,nested_dispatch,nested_validate,100)==DOA3_NESTED_YIELDED);
        converted_regs=save_regs();
        CHECK(!memcmp(&native_regs,&converted_regs,sizeof(native_regs)));
        CHECK(!memcmp(native,converted,bytes));
        if(stack.depth>deepest) deepest=stack.depth;
        if(stack.depth==1) ++returns_to_root;
        if(frame>8 && !checkpoint_frame && stack.depth==3) {
            CHECK(doa3_nested_capture(&stack,&saved,sizeof(saved),nested_validate));
            memcpy(memory_checkpoint,converted,bytes); saved_regs=converted_regs; checkpoint_frame=frame;
        }
    }
    DeleteFiber(fiber); CHECK(deepest==3 && returns_to_root>0 && checkpoint_frame>8);
    final=stack; rejection_tests(saved);
    CHECK(doa3_nested_restore(&stack,&saved,sizeof(saved),nested_validate));
    memcpy(converted,memory_checkpoint,bytes); load_regs(saved_regs);
    for(unsigned frame=checkpoint_frame+1;frame<60;++frame) {
        input(frame); CHECK(doa3_nested_advance(&stack,nested_dispatch,nested_validate,100)==DOA3_NESTED_YIELDED);
    }
    converted_regs=save_regs(); CHECK(!memcmp(&native_regs,&converted_regs,sizeof(native_regs)));
    CHECK(!memcmp(native,converted,bytes) && !memcmp(&stack,&final,sizeof(stack)));
    CHECK(ConvertFiberToThread());
    guest_memory_fixture_free(native); guest_memory_fixture_free(converted); free(memory_checkpoint);
    printf("Native/nested stacks matched 60 yields, max depth %u, and %u restored steps. Corrupt/overflow/budget states rejected.\n",deepest,59-checkpoint_frame);
    return 0;
}
