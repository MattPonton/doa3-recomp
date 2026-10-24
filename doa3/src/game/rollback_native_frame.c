#include "rollback_native_frame.h"
#include "kernel/kernel_replay.h"
#include "kernel/xbox_fiber.h"
#include "online/xbox_det.h"
#include "recomp/recomp_types.h"
#include "d3d/d3d8_nv2aff.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
/* Exact helper list from the source-pinned A6450 continuation. No address
 * lookup, external command, or peer-provided target is admitted here. */
#define HELPERS(X) X(000A65B0) X(000A4DD0) X(000A4E70) X(000A50A0) X(001F1F01) \
 X(001DC660) X(001DF010) X(000BE2D0) X(0017BAD0) X(00180490) X(0018BDD1) \
 X(001C650C) X(001DC740) X(000A5F90) X(001C76A0) X(001C6EF0) X(001C7700) \
 X(000A6680) X(000BE2E0) X(001E14D0) X(0017BCE0)
#define DECLARE(x) void sub_##x(void);
HELPERS(DECLARE)
#undef DECLARE
static LONGLONG ticks(void){LARGE_INTEGER t;QueryPerformanceCounter(&t);return t.QuadPart;}
extern volatile int g_apu_det;
extern unsigned g_doa3_frames_presented;
void mcpx_apu_det_frame(void);
static int active;
static int skip_guest_render;
static uint64_t guest_skipped;
static unsigned flips;
int doa3_native_frame_active(void){return active;}
int doa3_native_frame_skip_guest_render(unsigned routine){
    if(!active || !skip_guest_render)return 0;
    /* Material/command construction is stateful even with host draws off.
     * Never admit 17D900 or 17DF00 here: later frames depend on their state. */
    if(routine!=0x52046)return 0;
    ++guest_skipped;return 1;
}
void doa3_native_frame_flip(void){if(active)++flips;}
int doa3_native_frame_run(void *context){
    Doa3NativeFrame *f=context;
    unsigned count=f && f->frames ? f->frames:1;
    if(active || !f || !doa3_loop_at_boundary(f->loop) ||
       (f->suppress_host_draws && (f->skip_intermediate_preparation || f->skip_intermediate_guest_render)) ||
       count>DOA3_NATIVE_REPLAY_MAX_FRAMES ||
       !xbox_kernel_replay_owned() || !xbox_kernel_replay_healthy() ||
       !g_apu_det || !doa3_input_tape_can_replay(f->tape,1,count))return 0;
    uint64_t first=f->loop->intervals;
    if(first!=0 || !xbox_kernel_replay_frame_budget(count) || !doa3_input_tape_replay(f->tape,1))return 0;
    if(!xbox_kernel_replay_fail_stop(1)){doa3_input_tape_abort(f->tape);return 0;}
    /* One total timer; no per-helper/task instrumentation or trace I/O. */
    int previous_draw_mode=d3d8_replay_suppress_draws(f->suppress_host_draws);
    uint64_t skipped_before=d3d8_replay_skipped_draws();
    int previous_prep_mode=pgraph_d3d11_skip_preparation(0);
    uint64_t prep_before[3],prep_after[3];
    pgraph_d3d11_preparation_stats(&prep_before[0],&prep_before[1],&prep_before[2]);
    LONGLONG started=ticks();
    unsigned last_helper=0;
    guest_skipped=0;
    skip_guest_render=0;
    active=1;flips=0;
    unsigned visible_frame=g_doa3_frames_presented;
    g_doa3_frames_presented-=count; /* same frame index as the recorded batch */
    int result=0;
    for(unsigned frame=0;frame<count;++frame){
      skip_guest_render=f->skip_intermediate_guest_render && frame+1<count;
      pgraph_d3d11_skip_preparation((f->skip_intermediate_preparation || f->skip_intermediate_guest_render) && frame+1<count);
      /* Intermediate frames may modify texture RAM without binding it. Force
       * exact source validation before the final normally rendered frame. */
      if((f->skip_intermediate_preparation || f->skip_intermediate_guest_render) && frame && frame+1==count)
          pgraph_d3d11_invalidate_guest_uploads();
      if(frame && !doa3_input_tape_replay(f->tape,first+frame+1))goto done;
      xbox_det_frame();mcpx_apu_det_frame();
      unsigned previous_flips=flips;int completed=0;
      for(unsigned calls=0;calls<=DOA3_LOOP_MAX_CALLS;++calls){
        Doa3LoopAction action;
        if(!doa3_loop_next(f->loop,&action))break;
        if(action.kind==DOA3_LOOP_BOUNDARY){
            /* These are dead flag temporaries at A64B0 (source guard checks
             * the whole body). The native baseline has no continuation locals. */
            memset(f->loop->locals,0,sizeof(f->loop->locals));
            completed=f->loop->intervals==first+frame+1 && flips==previous_flips+1 &&
                doa3_input_tape_end(f->tape);break;
        }
        last_helper=action.routine;
        switch(action.routine){
#define RUN(x) case 0x##x:sub_##x();break;
        HELPERS(RUN)
#undef RUN
        default:goto done;
        }
        if(!xbox_fiber_is_primary() || !xbox_kernel_replay_healthy() ||
           !doa3_input_tape_replay_ready(f->tape) ||
           !doa3_loop_complete(f->loop,action.routine))break;
    }
      if(!completed)goto done;
    }
    result=1;
done:
    LONGLONG elapsed=ticks()-started;
    skip_guest_render=0;
    if(f->skip_intermediate_guest_render)
        fprintf(stderr,"[ROLLBACK-GUEST-RENDER-AB] model_commands=preserved vertex_deforms=%llu final_frame_rendered=%d\n",
            (unsigned long long)guest_skipped,result);
    pgraph_d3d11_skip_preparation(previous_prep_mode);
    if(f->skip_intermediate_preparation || f->skip_intermediate_guest_render){
        pgraph_d3d11_preparation_stats(&prep_after[0],&prep_after[1],&prep_after[2]);
        fprintf(stderr,"[ROLLBACK-PREP-AB] skipped_batches=%llu indices=%llu inline_words=%llu final_frame_rendered=%d\n",
            (unsigned long long)(prep_after[0]-prep_before[0]),
            (unsigned long long)(prep_after[1]-prep_before[1]),
            (unsigned long long)(prep_after[2]-prep_before[2]),result);
    }
    d3d8_replay_suppress_draws(previous_draw_mode);
    if(f->suppress_host_draws)fprintf(stderr,"[ROLLBACK-RENDER-AB] skipped_draws=%llu\n",
        (unsigned long long)(d3d8_replay_skipped_draws()-skipped_before));
    active=0;
    g_doa3_frames_presented=visible_frame;
    if(!xbox_kernel_replay_fail_stop(0))abort();
    fprintf(stderr,"[ROLLBACK-REPLAY] batch completed=%d requested=%u intervals=%llu flips=%u\n",result,count,
        (unsigned long long)(f->loop->intervals-first),flips);
    if(!result)fprintf(stderr,"[ROLLBACK-REPLAY] last helper=%08X\n",last_helper);
    LARGE_INTEGER frequency;QueryPerformanceFrequency(&frequency);
    fprintf(stderr,"[ROLLBACK-EXECUTION] frames=%u completed=%d execution_ms=%.3f\n",
        count,result,(double)elapsed*1000.0/(double)frequency.QuadPart);
    /* Leave a failed tape gated through checkpoint recovery; caller aborts it
     * only after the original endpoint has been restored and verified. */
    return result;
}
