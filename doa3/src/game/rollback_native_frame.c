#include "rollback_native_frame.h"
#include "kernel/kernel_replay.h"
#include "kernel/xbox_fiber.h"
#include "online/xbox_det.h"
#include "recomp/recomp_types.h"
#include "d3d/d3d8_nv2aff.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include "audio/dsound_hle.h"
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
extern unsigned g_doa3_frames_presented;
static int active;
static int skip_guest_render;
static uint64_t guest_skipped;
static unsigned flips;
int doa3_native_frame_active(void){return active;}
/* doa3.1-rollback, DOA3_ROLLBACK_SKIP_MAP=1 (diagnostic only): also skip the
 * model and alpha-model command builders on hidden frames, to map which
 * memory a whole render skip leaves different. Not a valid replay mode. */
int g_doa3_skip_map;
/* DOA3_ROLLBACK_SKIP_HEAD=k (doa3.1-rollback): hide only the first k replay
 * frames and render the rest normally, so the endpoint shows whether the
 * game and renderer re-synchronise after skipped frames. 0 = Jay's rule
 * (every frame but the last is hidden). */
int g_doa3_skip_head;
/* rollback.12, DOA3_ROLLBACK_LOGIC_ONLY=1: game tasks not stepped on hidden
 * replay frames (set per trial by the fight window). */
uint32_t g_doa3_skip_tasks[8];unsigned g_doa3_skip_task_count;
static int hidden_now;
static uint64_t tasks_skipped;
/* rollback.14: DOA3_ROLLBACK_SKIP_FNS=addr,... -- wrapped functions not
 * called on hidden replay frames of the logic-only arm (void functions
 * only; the wrapper pops the return slot and its ret-N arguments).
 * DOA3_ROLLBACK_FN_PROFILE=1 -- fiber CPU time of every wrapped call on
 * hidden replay frames, top 40 printed after each replay. */
volatile int g_fn_hidden_diag;
static uint32_t skip_fns[32];static unsigned skip_fn_count;static uint64_t skip_fn_calls;
static int fn_profile=-1,fn_skip_now;
#define PROF_SLOTS 16384u
static struct { uint32_t va; unsigned calls; unsigned long long ticks; } prof[PROF_SLOTS];
static unsigned long long prof_hidden_ticks;
static void fn_diag_init(void){
    if(fn_profile>=0)return;
    const char *e=getenv("DOA3_ROLLBACK_FN_PROFILE");fn_profile=e && *e=='1';
    e=getenv("DOA3_ROLLBACK_SKIP_FNS");
    {   /* rollback.15: the logic-only default (verified by Matt's .14 runs):
         * the renderer's other command builders, the 2D draw-list flush and
         * the D3D draw call. DOA3_ROLLBACK_SKIP_FNS replaces it ("0" = none). */
        extern int g_doa3_logic_only;
        /* rollback.20: 180510 (2D draw-list flush) left out of the list: it
         * also advances the 2D manager's list state (0x5AA1EC / 0x5AA224);
         * its drawing child 180730 stays skipped. */
        if(!e && g_doa3_logic_only)e="17EA20,17ECF0,17F0C0,17F1A0,17F400,17F6A0,180730,12FC74,1DF690";
    }
    while(e && *e && skip_fn_count<32){
        char *end;unsigned long v=strtoul(e,&end,16);
        if(end==e)break;
        if(v)skip_fns[skip_fn_count++]=(uint32_t)v;
        e=*end==',' ? end+1:end;
    }
    if(skip_fn_count){
        fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] hidden frames also skip:");
        for(unsigned i=0;i<skip_fn_count;++i)fprintf(stderr," %06X",skip_fns[i]);
        fprintf(stderr,"\n");
    }
}
int doa3_fn_hidden_skip(uint32_t va){
    if(!fn_skip_now)return 0;
    for(unsigned i=0;i<skip_fn_count;++i)if(skip_fns[i]==va){++skip_fn_calls;return 1;}
    return 0;
}
unsigned long long doa3_fn_prof_enter(void){return fn_profile>0 ? xbox_fiber_cpu_ticks():0;}
void doa3_fn_prof_leave(uint32_t va,unsigned long long t0){
    if(fn_profile<=0)return;
    unsigned long long d=xbox_fiber_cpu_ticks()-t0;
    unsigned h=(va*2654435761u)>>18;
    for(unsigned k=0;k<PROF_SLOTS;++k,h=(h+1)&(PROF_SLOTS-1)){
        if(prof[h].va==va || !prof[h].va){prof[h].va=va;prof[h].calls++;prof[h].ticks+=d;return;}
    }
}
static void prof_report(unsigned hidden_frames){
    LARGE_INTEGER f;QueryPerformanceFrequency(&f);
    double ms=1000.0/(double)f.QuadPart;
    fprintf(stderr,"[ROLLBACK-FN-PROFILE] %u hidden frames, %.2f ms; inclusive fiber CPU time per wrapped function (nested calls counted in their callers too):\n",
        hidden_frames,(double)prof_hidden_ticks*ms);
    for(unsigned n=0;n<40;++n){
        unsigned best=PROF_SLOTS;
        for(unsigned i=0;i<PROF_SLOTS;++i)if(prof[i].va && prof[i].calls && (best==PROF_SLOTS || prof[i].ticks>prof[best].ticks))best=i;
        if(best==PROF_SLOTS)break;
        fprintf(stderr,"[ROLLBACK-FN-PROFILE]   %06X %8.3f ms %7u calls\n",prof[best].va,(double)prof[best].ticks*ms,prof[best].calls);
        prof[best].calls=0;
    }
    memset(prof,0,sizeof(prof));prof_hidden_ticks=0;
}
int doa3_native_frame_skip_task(uint32_t routine){
    if(!active || !hidden_now || !skip_guest_render)return 0;
    for(unsigned i=0;i<g_doa3_skip_task_count;++i)if(g_doa3_skip_tasks[i]==routine){++tasks_skipped;return 1;}
    return 0;
}
int doa3_native_frame_skip_guest_render(unsigned routine){
    if(!active || !skip_guest_render)return 0;
    /* Material/command construction is stateful even with host draws off.
     * Never admit 17D900 or 17DF00 here: later frames depend on their state
     * (except in the skip-map diagnostic). */
    if(routine!=0x52046 && !(g_doa3_skip_map && (routine==0x17D900 || routine==0x17DF00)))return 0;
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
       !ds_det_active())return 0;
    uint64_t first=f->loop->intervals;
    if((first!=0 && !f->on_frame) || !doa3_input_tape_can_replay(f->tape,first+1,count) ||
       !xbox_kernel_replay_frame_budget(count) || !doa3_input_tape_replay(f->tape,first+1))return 0;
    if(!xbox_kernel_replay_fail_stop(1)){doa3_input_tape_abort(f->tape);return 0;}
    /* One total timer; no per-helper/task instrumentation or trace I/O. */
    int previous_draw_mode=d3d8_replay_suppress_draws(f->suppress_host_draws);
    uint64_t skipped_before=d3d8_replay_skipped_draws();
    int previous_prep_mode=pgraph_d3d11_skip_preparation(0);
    uint64_t prep_before[3],prep_after[3];
    pgraph_d3d11_preparation_stats(&prep_before[0],&prep_before[1],&prep_before[2]);
    LONGLONG started=ticks();
    unsigned last_helper=0;
    guest_skipped=0;tasks_skipped=0;
    skip_guest_render=0;
    active=1;flips=0;
    unsigned visible_frame=g_doa3_frames_presented;
    g_doa3_frames_presented-=count; /* same frame index as the recorded batch */
    int result=0;
    int previous_hidden=0;
    for(unsigned frame=0;frame<count;++frame){
      int hidden=f->all_hidden || (frame+1<count && (!g_doa3_skip_head || frame<(unsigned)g_doa3_skip_head));
      skip_guest_render=f->skip_intermediate_guest_render && hidden;
      hidden_now=hidden;
      fn_diag_init();
      fn_skip_now=skip_guest_render && skip_fn_count;
      g_fn_hidden_diag=hidden && (fn_skip_now || fn_profile>0);
      pgraph_d3d11_skip_preparation((f->skip_intermediate_preparation || f->skip_intermediate_guest_render) && hidden);
      /* Intermediate frames may modify texture RAM without binding it. Force
       * exact source validation before the first normally rendered frame. */
      if((f->skip_intermediate_preparation || f->skip_intermediate_guest_render) && previous_hidden && !hidden)
          pgraph_d3d11_invalidate_guest_uploads();
      previous_hidden=hidden;
      if(frame && !doa3_input_tape_replay(f->tape,first+frame+1))goto done;
      xbox_det_frame();ds_det_frame();
      LONGLONG frame_started=ticks();
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
      if(hidden)prof_hidden_ticks+=(unsigned long long)(ticks()-frame_started);
      g_fn_hidden_diag=0;fn_skip_now=0;
      if(!completed)goto done;
      if(f->on_frame){
          /* the boundary save must not see replay-time skipping */
          int skip_was=skip_guest_render;skip_guest_render=0;hidden_now=0;
          pgraph_d3d11_skip_preparation(0);
          f->on_frame(f->on_frame_ctx,frame);
          skip_guest_render=skip_was;
      }
    }
    result=1;
done:
    LONGLONG elapsed=ticks()-started;
    skip_guest_render=0;hidden_now=0;g_fn_hidden_diag=0;fn_skip_now=0;
    if(fn_profile>0 && count>1)prof_report(count-1);
    if(skip_fn_count && f->skip_intermediate_guest_render)
        fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] listed function calls skipped on hidden frames: %llu\n",(unsigned long long)skip_fn_calls);
    skip_fn_calls=0;
    if(g_doa3_skip_task_count && f->skip_intermediate_guest_render)
        fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] task steps skipped on hidden frames: %llu\n",(unsigned long long)tasks_skipped);
    if(f->skip_intermediate_guest_render)
        fprintf(stderr,"[ROLLBACK-GUEST-RENDER-AB] model_commands=%s skipped_calls=%llu hidden_frames=%u final_frame_rendered=%d\n",
            g_doa3_skip_map ? "skipped":"preserved",(unsigned long long)guest_skipped,
            g_doa3_skip_head && (unsigned)g_doa3_skip_head<count ? (unsigned)g_doa3_skip_head:count-1,result);
    pgraph_d3d11_skip_preparation(previous_prep_mode);
    /* Texture RAM may have changed under hidden frames and the restore:
     * the next rendered frame validates every guest upload again. */
    if(f->all_hidden)pgraph_d3d11_invalidate_guest_uploads();
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
