/* Actual lifted loop versus generated continuation. External helpers are
 * counted deterministic fixtures; this is not a fight or pacing validation. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define BYTES (16u*1024u*1024u)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
typedef struct {uint32_t r[8];double fp[8];int top;} Registers;
static Registers regs(void){
    Registers r={0};const uint32_t words[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};
    memcpy(r.r,words,sizeof words);memcpy(r.fp,g_fp_stack,sizeof r.fp);r.top=g_fp_top;return r;
}
static void load(Registers r,unsigned char *ram){
    g_xbox_mem_offset=(ptrdiff_t)ram;g_eax=r.r[0];g_ecx=r.r[1];g_edx=r.r[2];g_esp=r.r[3];
    g_ebx=r.r[4];g_esi=r.r[5];g_edi=r.r[6];g_seh_ebp=r.r[7];
    memcpy(g_fp_stack,r.fp,sizeof r.fp);g_fp_top=r.top;
}
static void *primary;
static int native_mode;
static Doa3LoopAction observed;
static unsigned calls;
void doa3_rollback_frame_phase(uint32_t address){
    if(address==0xA64B0){observed=(Doa3LoopAction){DOA3_LOOP_BOUNDARY,0};SwitchToFiber(primary);}
}
static void fixture_helper(uint32_t routine){
    volatile uint32_t *counter=&MEM32(0xD00000+(routine&0xFFFF)*4);
    CHECK(MEM32(g_esp)==0);++*counter;++calls;
    /* Model real caller/callee stack cleanup and make arguments observable. */
    unsigned args=routine==0x1DF010 ? 6:routine==0x1E14D0 ? 1:0;
    uint32_t sum=0;for(unsigned i=0;i<args;++i)sum+=MEM32(g_esp+4+i*4);
    if(routine==0x1C76A0 || routine==0x1C6EF0)sum+=MEM32(g_esp+4);
    g_eax=g_eax*17+routine+*counter+sum;g_ecx^=g_eax;g_edx+=routine;
    MEM32(0xE00000)=MEM32(0xE00000)*31+g_eax;
    g_fp_stack[g_fp_top&7]+=(double)(routine&255)/256;
    g_esp+=4+args*4;
    if(native_mode){observed=(Doa3LoopAction){DOA3_LOOP_CALL,routine};SwitchToFiber(primary);}
}
#include "loop_native.generated.inc"
static void CALLBACK native_entry(void *v){(void)v;native_loop();CHECK(0);}
static void input(unsigned char *ram,unsigned n){
    g_xbox_mem_offset=(ptrdiff_t)ram;MEM32(0x73C384)=n%2;
    MEM32(0x7FE2CC)=n%3;MEMF(0x7FE2D0)=4.5f;MEMF(0x7FD1C0)=.25f;
    MEM32(0x7FE2C8)=17+n;MEM32(0xA48CE0)=n*31;
}
static uint64_t hash(const void *data,size_t n){
    const unsigned char *p=data;uint64_t h=14695981039346656037ull;
    for(size_t i=0;i<n;++i)h=(h^p[i])*1099511628211ull;return h;
}
static void rejection(Doa3LoopState bad){
    Doa3LoopState before=bad;Doa3LoopAction action={77,88},old=action;
    Registers a=regs();unsigned before_calls=calls;
    CHECK(!doa3_loop_next(&bad,&action));Registers b=regs();
    CHECK(!memcmp(&bad,&before,sizeof bad) && !memcmp(&action,&old,sizeof action));
    CHECK(!memcmp(&a,&b,sizeof a) && calls==before_calls);
}
int main(void){
    unsigned char *native=guest_memory_fixture_alloc(BYTES),*converted=guest_memory_fixture_alloc(BYTES),*saved=malloc(BYTES);
    CHECK(native && converted && saved);
    Registers nr={0},cr,saved_regs;nr.r[3]=0x100000;nr.r[7]=123;nr.top=3;cr=nr;
    Doa3LoopState state,snapshot;doa3_loop_init(&state);CHECK(doa3_loop_at_boundary(&state));
    primary=ConvertThreadToFiber(NULL);CHECK(primary);void *fiber=CreateFiber(0,native_entry,NULL);CHECK(fiber);
    load(nr,native);native_mode=1;SwitchToFiber(fiber);nr=regs();CHECK(observed.kind==DOA3_LOOP_BOUNDARY);
    uint64_t expected[20];Registers expected_regs[20];Doa3LoopState expected_state[20];
    unsigned optional=0,fade=0,fade_end=0;
    for(unsigned frame=0;frame<20;++frame){
        input(native,frame);input(converted,frame);
        for(unsigned step=0;;++step){
            CHECK(step<DOA3_LOOP_MAX_CALLS);
            load(nr,native);native_mode=1;SwitchToFiber(fiber);nr=regs();
            load(cr,converted);native_mode=0;Doa3LoopAction action;
            CHECK(doa3_loop_next(&state,&action));CHECK(!memcmp(&action,&observed,sizeof action));
            if(action.kind==DOA3_LOOP_CALL){
                CHECK(!doa3_loop_at_boundary(&state));rejection(state); /* cannot skip a pending call */
                Doa3LoopState before=state;CHECK(!doa3_loop_complete(&state,action.routine+1));CHECK(!memcmp(&before,&state,sizeof state));
                fixture_helper(action.routine);CHECK(doa3_loop_complete(&state,action.routine));
                CHECK(!doa3_loop_complete(&state,action.routine));
                optional+=action.routine==0x1DC660;fade+=action.routine==0x1C76A0;fade_end+=action.routine==0x1C6EF0;
            }
            cr=regs();CHECK(!memcmp(&nr,&cr,sizeof nr));CHECK(!memcmp(native,converted,BYTES));
            if(action.kind==DOA3_LOOP_BOUNDARY)break;
        }
        CHECK(doa3_loop_at_boundary(&state) && state.intervals==frame+1 && g_esp==0x100000);
        expected[frame]=hash(converted,BYTES);expected_regs[frame]=cr;expected_state[frame]=state;
        if(frame==4){memcpy(saved,converted,BYTES);snapshot=state;saved_regs=cr;}
    }
    CHECK(optional==10 && fade==13 && fade_end==7);
    for(unsigned replay=0;replay<3;++replay){
        memcpy(converted,saved,BYTES);state=snapshot;load(saved_regs,converted);native_mode=0;
        for(unsigned frame=5;frame<20;++frame){
            input(converted,frame);
            for(unsigned step=0;;++step){
                CHECK(step<DOA3_LOOP_MAX_CALLS);Doa3LoopAction action;CHECK(doa3_loop_next(&state,&action));
                if(action.kind==DOA3_LOOP_BOUNDARY)break;
                fixture_helper(action.routine);CHECK(doa3_loop_complete(&state,action.routine));
            }
            Registers actual=regs();CHECK(hash(converted,BYTES)==expected[frame]);
            CHECK(!memcmp(&actual,&expected_regs[frame],sizeof actual));CHECK(!memcmp(&state,&expected_state[frame],sizeof state));
        }
    }
    Doa3LoopState bad=state;bad.schema++;rejection(bad);bad=state;bad.reserved=1;rejection(bad);
    bad=state;bad.resume=99;rejection(bad);bad=state;bad.pending=0x123;rejection(bad);
    bad=state;bad.intervals=UINT64_MAX;rejection(bad);
    bad=state;bad.resume=1;bad.calls=DOA3_LOOP_MAX_CALLS;rejection(bad);
    CHECK(!doa3_loop_next(NULL,NULL));CHECK(!doa3_loop_complete(NULL,1));
    DeleteFiber(fiber);CHECK(ConvertFiberToThread());guest_memory_fixture_free(native);guest_memory_fixture_free(converted);free(saved);
    puts("3.1 outer loop: 20 native/continuation intervals match after EVERY helper; optional/fade branches, guest stack, x87 and three restored replays match. Helpers remain fixtures; no live game advance.");
    return 0;
}
