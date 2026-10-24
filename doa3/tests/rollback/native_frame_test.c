#include "game/rollback_native_frame.h"
#include "game/recomp/recomp_types.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
volatile int g_apu_det=1;
unsigned g_doa3_frames_presented=101;
static int tape,healthy=1,owned=1,primary=1,fail_stop,deny_input,bad_flip;
static unsigned calls,clock_steps,apu_steps;
static unsigned expected_frames=1,replayed,ended;
static unsigned fail_frame;
static int suppressed,expected_suppressed;
static int prep_suppressed,expected_prep;
static int expected_guest;
static unsigned original_models[3];
void sub_0017D900_gen(void){++original_models[0];g_esp+=4;}
void sub_0017DF00_gen(void){++original_models[1];g_esp+=8;}
void sub_00052046_gen(void){++original_models[2];g_esp+=4;}
#include "game/rollback_guest_render.inc"
static void models(int skip){
    unsigned old[3];memcpy(old,original_models,sizeof old);
    uint32_t stack=g_esp,regs[]={g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi,g_seh_ebp};
    g_esp-=4;sub_0017D900();CHECK(g_esp==stack);
    g_esp-=8;sub_0017DF00();CHECK(g_esp==stack);
    g_esp-=4;sub_00052046();CHECK(g_esp==stack);
    uint32_t after[]={g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi,g_seh_ebp};
    CHECK(!memcmp(regs,after,sizeof regs));
    for(unsigned i=0;i<3;++i)CHECK(original_models[i]==old[i]+!(i==2 && skip));
    CHECK(!doa3_native_frame_skip_guest_render(0x17D900));
    CHECK(!doa3_native_frame_skip_guest_render(0x17DF00));
    CHECK(!doa3_native_frame_skip_guest_render(0x4CD00)); /* animation is never admitted */
    CHECK(!doa3_native_frame_skip_guest_render(0xB4720)); /* combat likewise */
}
static unsigned upload_invalidations;
int pgraph_d3d11_skip_preparation(int on){int old=prep_suppressed;prep_suppressed=on;return old;}
void pgraph_d3d11_preparation_stats(uint64_t *b,uint64_t *i,uint64_t *w){*b=*i=*w=0;}
void pgraph_d3d11_invalidate_guest_uploads(void){CHECK((expected_prep || expected_guest) && !prep_suppressed && replayed==expected_frames-1);++upload_invalidations;}
int d3d8_replay_suppress_draws(int on){int old=suppressed;suppressed=on;return old;}
uint64_t d3d8_replay_skipped_draws(void){return 0;}
int xbox_kernel_replay_owned(void){return owned;}
int xbox_kernel_replay_healthy(void){return healthy;}
int xbox_kernel_replay_fail_stop(int on){fail_stop=on;return owned;}
int xbox_kernel_replay_frame_budget(unsigned n){CHECK(n==expected_frames && !tape && !fail_stop);return owned;}
int xbox_fiber_is_primary(void){return primary;}
int doa3_input_tape_can_replay(const Doa3InputTape *p,uint64_t f,unsigned n){return p && !tape && f==1 && n==expected_frames;}
int doa3_input_tape_replay(Doa3InputTape *p,uint64_t f){CHECK(p && f==++replayed && !tape);tape=1;return 1;}
int doa3_input_tape_replay_ready(const Doa3InputTape *p){return p && tape && !deny_input && replayed!=fail_frame;}
int doa3_input_tape_end(Doa3InputTape *p){CHECK(p && tape);tape=0;++ended;return 1;}
int doa3_input_tape_abort(Doa3InputTape *p){CHECK(p);tape=0;return 1;}
void xbox_det_frame(void){CHECK(tape && fail_stop && doa3_native_frame_active());++clock_steps;}
void mcpx_apu_det_frame(void){++apu_steps;}
static void helper(unsigned routine){
    CHECK(suppressed==expected_suppressed);
    CHECK(prep_suppressed==((expected_prep || expected_guest) && replayed<expected_frames));
    models(expected_guest && replayed<expected_frames);
    CHECK(doa3_native_frame_active() && tape && fail_stop && MEM32(g_esp)==0);++calls;
    if(calls==1)CHECK(routine==0x1C650C);
    if(routine==0x1C650C)CHECK(g_doa3_frames_presented==101-expected_frames+replayed-1);
    if(routine==0x1E14D0 && !bad_flip){++g_doa3_frames_presented;doa3_native_frame_flip();}
    g_esp+=4+(routine==0x1DF010 ? 24:routine==0x1E14D0 ? 4:0);
    g_eax=routine;
}
#define H(x) void sub_##x(void){helper(0x##x);}
H(000A65B0) H(000A4DD0) H(000A4E70) H(000A50A0) H(001F1F01)
H(001DC660) H(001DF010) H(000BE2D0) H(0017BAD0) H(00180490) H(0018BDD1)
H(001C650C) H(001DC740) H(000A5F90) H(001C76A0) H(001C6EF0) H(001C7700)
H(000A6680) H(000BE2E0) H(001E14D0) H(0017BCE0)
int main(void){
    unsigned char *ram=guest_memory_fixture_alloc(16u*1024u*1024u);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    Doa3LoopState loop;Doa3NativeFrame frame={&loop,(Doa3InputTape *)&frame};
    for(unsigned mode=0;mode<3;++mode){
        doa3_loop_init(&loop);g_esp=0x100000;calls=tape=replayed=ended=0;
        deny_input=mode==1;bad_flip=mode==2;
        CHECK(doa3_native_frame_run(&frame)==(mode==0));
        CHECK(!doa3_native_frame_active() && !fail_stop && g_doa3_frames_presented==101);
        if(!mode){CHECK(loop.intervals==1 && g_esp==0x100000 && calls>=18 && !tape);
            for(unsigned i=0;i<5;++i)CHECK(!loop.locals[i]);}
        else CHECK(tape); /* failure keeps input gate through recovery */
        doa3_input_tape_abort(frame.tape);
    }
    CHECK(clock_steps==3 && apu_steps==3);
    deny_input=bad_flip=0;expected_frames=frame.frames=2;
    for(unsigned mode=0;mode<2;++mode){
        doa3_loop_init(&loop);g_esp=0x100000;calls=tape=replayed=ended=0;fail_frame=mode ? 2:0;
        CHECK(doa3_native_frame_run(&frame)==!mode);
        CHECK(replayed==2 && ended==(mode ? 1:2) && g_doa3_frames_presented==101);
        CHECK(loop.intervals==(mode ? 1:2) && !doa3_native_frame_active() && !fail_stop);
        CHECK(tape==(int)mode);doa3_input_tape_abort(frame.tape);
    }
    CHECK(clock_steps==7 && apu_steps==7);
    for(unsigned n=4;n<=8;n*=2){
      expected_suppressed=frame.suppress_host_draws=1;
      expected_frames=frame.frames=n;
      for(unsigned mode=0;mode<2;++mode){
        doa3_loop_init(&loop);g_esp=0x100000;calls=tape=replayed=ended=0;fail_frame=mode ? n:0;
        unsigned before=clock_steps;
        CHECK(doa3_native_frame_run(&frame)==!mode);
        CHECK(clock_steps==before+n && apu_steps==clock_steps && replayed==n && ended==n-mode);
        CHECK(loop.intervals==n-mode && !doa3_native_frame_active() && !fail_stop && g_doa3_frames_presented==101);
        CHECK(tape==(int)mode && !suppressed);doa3_input_tape_abort(frame.tape);
      }
    }
    CHECK(clock_steps==31);
    doa3_loop_init(&loop);frame.frames=9;CHECK(!doa3_native_frame_run(&frame));frame.frames=2;
    doa3_loop_init(&loop);owned=0;CHECK(!doa3_native_frame_run(&frame));
    CHECK(clock_steps==31 && !tape);
    owned=1;expected_suppressed=frame.suppress_host_draws=0;
    expected_prep=frame.skip_intermediate_preparation=1;
    for(unsigned n=1;n<=8;n*=2){
        expected_frames=frame.frames=n;
        for(unsigned mode=0;mode<3;++mode){
            doa3_loop_init(&loop);g_esp=0x100000;calls=tape=replayed=ended=0;
            fail_frame=mode==1 ? 1:mode==2 ? n:0;
            unsigned invalidated=upload_invalidations;
            CHECK(doa3_native_frame_run(&frame)==!mode);
            CHECK(!suppressed && !prep_suppressed && !fail_stop && !doa3_native_frame_active());
            CHECK(g_doa3_frames_presented==101);
            CHECK(upload_invalidations==invalidated+(n>1 && mode!=1));
            doa3_input_tape_abort(frame.tape);
        }
    }
    doa3_loop_init(&loop);frame.suppress_host_draws=1;
    CHECK(!doa3_native_frame_run(&frame) && !suppressed && !prep_suppressed);
    frame.suppress_host_draws=0;
    expected_prep=frame.skip_intermediate_preparation=0;
    expected_guest=frame.skip_intermediate_guest_render=1;
    for(unsigned n=1;n<=8;n*=2){
        expected_frames=frame.frames=n;
        for(unsigned mode=0;mode<3;++mode){
            doa3_loop_init(&loop);g_esp=0x100000;calls=tape=replayed=ended=0;
            fail_frame=mode==1 ? 1:mode==2 ? n:0;
            models(0); /* never skip outside replay, even with experiment armed */
            unsigned invalidated=upload_invalidations;
            CHECK(doa3_native_frame_run(&frame)==!mode);
            CHECK(!suppressed && !prep_suppressed && !fail_stop && !doa3_native_frame_active());
            CHECK(g_doa3_frames_presented==101);
            CHECK(upload_invalidations==invalidated+(n>1 && mode!=1));
            models(0); /* gate cleared on both successful and failed execution */
            doa3_input_tape_abort(frame.tape);
        }
    }
    doa3_loop_init(&loop);frame.suppress_host_draws=1;
    CHECK(!doa3_native_frame_run(&frame));models(0);
    guest_memory_fixture_free(ram);puts("Native interval driver: source-pinned loop, one tick, helper dispatch, input/flip failures and output gates passed");return 0;
}
