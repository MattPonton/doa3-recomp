/* One real native dispatcher-to-dispatcher interval, then a rewind/return
 * check; the separate replay opt-in also re-executes the recorded interval. */
#include "kernel/xbox_ram_delta.h"
#include "rollback_fight_checkpoint.h"
#include "rollback_input_tape.h"
#include "rollback_native_frame.h"
#include "online/rollback_tasks.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include "kernel/kernel_replay.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include "recomp/recomp_types.h"
#include "training_dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern volatile int g_apu_det;
extern unsigned g_doa3_frames_presented;
void mcpx_apu_det_begin(uint64_t);
void mcpx_apu_det_end(void);
void mcpx_apu_det_frame(void);
uint64_t mcpx_apu_xgscnt_now(void);
static int enabled=-1,recording,settling,replay,auditing,combat,long_window;
static unsigned warmup,presented,settle_frames;
static unsigned trial,window=1,completed;
static unsigned repeats=1;
static int texture_ab,packet_ab,geometry_ab,render_ab,prep_ab,guest_ab;
/* rollback.12, DOA3_ROLLBACK_LOGIC_ONLY=1: on hidden replay frames skip the
 * render tasks too (one set per trial, then all of them), and accept the
 * replay when only render bookkeeping differs. Every trial runs. */
int g_doa3_logic_only,g_doa3_logic_only_arm;
static unsigned logic_results[7];
static int delta_test,delta_test_on;   /* rollback.17, DOA3_ROLLBACK_DELTA_TEST=1 */
static void logic_only_tasks(unsigned t)
{
    /* rollback.14: every task runs; DOA3_ROLLBACK_SKIP_TASKS=addr,... (hex)
     * still lets an experiment leave tasks out on hidden frames. */
    extern uint32_t g_doa3_skip_tasks[8];extern unsigned g_doa3_skip_task_count;
    const char *e=getenv("DOA3_ROLLBACK_SKIP_TASKS");
    g_doa3_skip_task_count=0;
    while(e && *e && g_doa3_skip_task_count<8){
        char *end;unsigned long v=strtoul(e,&end,16);
        if(end==e)break;
        if(v)g_doa3_skip_tasks[g_doa3_skip_task_count++]=(uint32_t)v;
        e=*end==',' ? end+1:end;
    }
    fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] trial %u: hidden frames skip the model and alpha-model command builders and deformation; tasks left out:",t);
    for(unsigned i=0;i<g_doa3_skip_task_count;++i)fprintf(stderr," %06X",g_doa3_skip_tasks[i]);
    fprintf(stderr,g_doa3_skip_task_count ? "\n":" none\n");
}
static ULONGLONG capture_started;
static uint32_t epoch;
static Doa3LoopState loop;
static Doa3FightCheckpoint *fight;
static Doa3CheckpointImage *start;
static Doa3InputTape *tape;
static int armed(const char *name){const char *v=getenv(name);return v && *v && strcmp(v,"0");}
static int attacking(void){
    for(unsigned side=0;side<2;++side){
        unsigned category=MEM8(0x59CF58+side*0x68+0x33);
        if(category==2 || category==3)return 1;
    }
    return 0;
}
static void combat_state(const char *when){
    if(!combat)return;
    for(unsigned side=0;side<2;++side){
        unsigned p=0x59CF58+side*0x68;
        fprintf(stderr,"[ROLLBACK-COMBAT] %s trial=%u frames=%u side=%u move=%u category=%u attack_frame=%u health=%u\n",
            when,trial,window,side+1,MEM16(p+0x2E),MEM8(p+0x33),MEM16(0x9B28A0+side*2),MEM16(p+0x20));
    }
}
static int eligible(void){
    TrainingStatus status;training_status(&status);
    return xbox_fiber_is_primary() && MEM8(0x5A2858)==7 && MEM8(0x596A37)==1 &&
        !MEM8(0x596AA5) && status.mode==TRAINING_IDLE;
}
static void fatal(const char *stage){
    fprintf(stderr,"[ROLLBACK-WINDOW] FATAL: %s; stopping instead of resuming uncertain state\n",stage);
    fflush(stderr);abort();
}
static void finish(int success){
    if(delta_test_on){xbox_ram_delta_end();delta_test_on=0;}
    if(auditing){XboxKernelReplayAudit audit;if(!xbox_kernel_replay_audit_end(&audit))fatal("kernel audit cleanup");auditing=0;}
    if(tape && (!doa3_input_tape_abort(tape) || !doa3_input_tape_destroy(tape)))fatal("input cleanup");
    tape=NULL;
    if(fight && !doa3_fight_checkpoint_destroy(fight))fatal("checkpoint cleanup");
    fight=NULL;start=NULL;recording=settling=enabled=0;
    if(g_apu_det)mcpx_apu_det_end();
    if(g_xbox_det_active)xbox_det_end();
    if(combat){
        fprintf(stderr,"[ROLLBACK-COMBAT] trial=%u frames=%u result=%s\n",trial,window,success ? ((render_ab || prep_ab || guest_ab) ? "state-matched (render A/B)":"matched"):"stopped");
        if(g_doa3_logic_only && trial<=6)logic_results[trial]=success ? 1:2;
        if((success || g_doa3_logic_only) && trial<6){enabled=1;warmup=0;}
        else if(g_doa3_logic_only){
            for(unsigned t=1;t<=6;++t)fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] summary trial %u: %s\n",t,
                logic_results[t]==1 ? "logic matched":logic_results[t]==2 ? "LOGIC DIFFERS or refused":"not run");
        }
        else fprintf(stderr,"[ROLLBACK-COMBAT] sequence %s after %u trials\n",success ? "passed":"stopped",trial);
    }
}
/* ---- rollback.18: sync test (DOA3_ROLLBACK_SYNC_TEST=depth) ----------------
 * GGPO-style: in solo Sparring, every frame (or every DOA3_ROLLBACK_SYNC_EVERY
 * frames) the game is rolled back `depth` frames and resimulated with the
 * recorded inputs, logic only (every resimulated frame hidden; the next
 * native frame renders). The resimulated boundary is compared with the one
 * the native frame reached, and play continues from the resimulated state.
 * State is saved every frame: RAM by delta records, the rest by lean saves. */
int doa3_fight_lean_begin(Doa3FightCheckpoint *);
int doa3_fight_lean_save(Doa3FightCheckpoint *,int);
int doa3_fight_lean_can_rollback(unsigned);
int doa3_fight_lean_rollback(Doa3FightCheckpoint *,unsigned,int (*)(void *),void *,int,size_t *,double *,double *);
void doa3_fight_lean_stats(double *,unsigned *,unsigned *);
void doa3_fight_lean_end(void);
#define SYNC_RING 16u
static int sync_mode=-1,sync_state,sync_auditing;
static unsigned sync_compare=30,sync_compares;   /* compare every n-th rollback (0 = never) */
static double sync_frame_ms_sum;static LONGLONG sync_last_qpc;
static unsigned sync_depth,sync_every=1,sync_warm,sync_settle,sync_frames;
static unsigned sync_rollbacks,sync_skipped,sync_mismatches,sync_unadmitted;
static double sync_restore_sum,sync_replay_sum,sync_worst;
static unsigned long long sync_total_rollbacks,sync_total_mismatches;
static unsigned char sync_frame_ok[SYNC_RING];   /* frame id % ring: kernel audit admitted it */
static Doa3FightCheckpoint *sync_fight;
static Doa3InputTape *sync_tape;
static uint32_t sync_epoch;
static void sync_on_frame(void *ctx,unsigned frame){(void)frame;doa3_fight_lean_save((Doa3FightCheckpoint *)ctx,1);}
static void sync_stop(const char *why){
    if(sync_auditing){XboxKernelReplayAudit a;xbox_kernel_replay_audit_end(&a);sync_auditing=0;}
    if(sync_tape){doa3_input_tape_abort(sync_tape);doa3_input_tape_destroy(sync_tape);sync_tape=NULL;}
    if(sync_state==2)doa3_fight_lean_end();
    if(sync_fight && !doa3_fight_checkpoint_destroy(sync_fight))fatal("sync checkpoint cleanup");
    sync_fight=NULL;
    if(g_apu_det)mcpx_apu_det_end();
    if(g_xbox_det_active)xbox_det_end();
    fprintf(stderr,"[ROLLBACK-SYNC] stopped (%s) after %u frames: %llu rollbacks, %llu with differences\n",
        why,sync_frames,sync_total_rollbacks,sync_total_mismatches);
    fflush(stderr);
    sync_state=0;sync_warm=0;
}
static void sync_boundary(void){
    if(doa3_native_frame_active())return;   /* never inside a resimulation */
    if(sync_state==0){
        if(!eligible() || g_xbox_det_active || g_apu_det){sync_warm=0;return;}
        if(++sync_warm<120)return;
        xbox_det_bases bases;xbox_det_current_bases(&bases);xbox_det_begin(&bases);
        sync_epoch=xbox_det_epoch();mcpx_apu_det_begin(mcpx_apu_xgscnt_now());
        if(!g_apu_det){fprintf(stderr,"[ROLLBACK-SYNC] refused: audio clock ownership\n");xbox_det_end();sync_mode=0;return;}
        sync_state=1;sync_settle=0;
        fprintf(stderr,"[ROLLBACK-SYNC] settling workers into controlled-clock waits\n");
        xbox_det_frame();mcpx_apu_det_frame();return;
    }
    if(!g_xbox_det_active || !g_apu_det || xbox_det_epoch()!=sync_epoch || !xbox_fiber_is_primary())fatal("sync clock ownership changed");
    if(sync_state==1){
        if(!eligible()){sync_stop("mode changed while settling");return;}
        if(!xbox_fiber_converted_clock_ready()){
            if(++sync_settle>=240){sync_stop("workers did not settle");sync_mode=0;return;}
            xbox_det_frame();mcpx_apu_det_frame();return;
        }
        doa3_loop_init(&loop);
        sync_fight=doa3_fight_checkpoint_create(&loop);
        if(!sync_fight){sync_stop("checkpoint create refused");return;}
        if(!doa3_fight_lean_begin(sync_fight)){sync_stop("delta tracking refused");return;}
        sync_state=2;
        loop.intervals=0;
        if(!doa3_fight_lean_save(sync_fight,0)){sync_stop("first save refused");return;}
        sync_tape=doa3_input_tape_create(DOA3_INPUT_TAPE_FRAMES);
        memset(sync_frame_ok,0,sizeof sync_frame_ok);
        sync_auditing=xbox_kernel_replay_audit_begin();
        if(!sync_tape || !sync_auditing || !doa3_input_tape_record(sync_tape,1)){sync_stop("input/audit start refused");return;}
        sync_frames=0;sync_rollbacks=sync_skipped=sync_mismatches=sync_unadmitted=0;
        sync_restore_sum=sync_replay_sum=sync_worst=0;sync_total_rollbacks=sync_total_mismatches=0;
        fprintf(stderr,"[ROLLBACK-SYNC] running: rollback %u frame(s) every %u frame(s), logic-only resimulation; "
            "every %u-th rollback renders its last frame and is compared with the native state\n",sync_depth,sync_every,sync_compare);
        sync_compares=0;sync_frame_ms_sum=0;sync_last_qpc=0;
        xbox_det_frame();mcpx_apu_det_frame();return;
    }
    /* sync_state 2: one native frame (id loop.intervals+1) just ran. */
    if(!eligible()){sync_stop("left solo Sparring");return;}
    uint64_t id=loop.intervals+1;
    if(!doa3_input_tape_end(sync_tape)){sync_stop("input recording failed");return;}
    {   XboxKernelReplayAudit a;
        int ok=sync_auditing && xbox_kernel_replay_audit_end(&a);sync_auditing=0;
        sync_frame_ok[id%SYNC_RING]=ok && !a.rejected && !a.invalid && a.accepted<=XBOX_REPLAY_KERNEL_CALL_LIMIT;
        if(!sync_frame_ok[id%SYNC_RING])++sync_unadmitted;
    }
    loop.intervals=id;
    memset(loop.locals,0,sizeof(loop.locals));
    if(!doa3_fight_lean_save(sync_fight,1)){/* this boundary cannot be a rollback target */}
    ++sync_frames;
    if(sync_depth && sync_frames%sync_every==0){
        int ok=doa3_fight_lean_can_rollback(sync_depth) && id>sync_depth &&
            doa3_input_tape_can_replay(sync_tape,id-sync_depth+1,sync_depth);
        for(unsigned k=0;ok && k<sync_depth;++k)ok=sync_frame_ok[(id-k)%SYNC_RING];
        if(ok){
            /* A compared rollback renders its last frame like Jay's replay,
             * so render output matches too; the others are all hidden. */
            int compare=sync_compare && (sync_total_rollbacks%sync_compare)==0;
            Doa3NativeFrame nf={&loop,sync_tape,sync_depth,0,1,1,!compare,sync_on_frame,sync_fight};
            size_t odd=0;double mr=0,mp=0;
            int r=doa3_fight_lean_rollback(sync_fight,sync_depth,doa3_native_frame_run,&nf,compare,&odd,&mr,&mp);
            if(r>0 && compare)++sync_compares;
            if(r<0)fatal("sync rollback");
            if(r>0){
                if(loop.intervals!=id)fatal("sync resimulation ended at the wrong interval");
                ++sync_rollbacks;++sync_total_rollbacks;sync_restore_sum+=mr;sync_replay_sum+=mp;
                if(mr+mp>sync_worst)sync_worst=mr+mp;
                if(odd){++sync_mismatches;++sync_total_mismatches;
                    fprintf(stderr,"[ROLLBACK-SYNC] frame %llu: resimulated state differs from native (%zu unexplained bytes)\n",(unsigned long long)id,odd);}
            }else ++sync_skipped;
        }else ++sync_skipped;
    }
    {   LARGE_INTEGER q,fq;QueryPerformanceCounter(&q);QueryPerformanceFrequency(&fq);
        if(sync_last_qpc)sync_frame_ms_sum+=1000.0*(double)(q.QuadPart-sync_last_qpc)/(double)fq.QuadPart;
        sync_last_qpc=q.QuadPart; }
    if(sync_frames%300==0){
        double save_ms;unsigned saves,refused;doa3_fight_lean_stats(&save_ms,&saves,&refused);
        fprintf(stderr,"[ROLLBACK-SYNC] last 300 frames (%.2f ms a frame): %u rollbacks of %u frames (restore %.2f ms, resimulation %.2f ms "
            "including its saves, average; worst %.2f ms), %u skipped, %u frames not admitted for replay; %u compared, %u with differences; "
            "saves %u (%.3f ms average, %u refused)\n",
            sync_frame_ms_sum/300.0,sync_rollbacks,sync_depth,sync_rollbacks ? sync_restore_sum/sync_rollbacks:0,sync_rollbacks ? sync_replay_sum/sync_rollbacks:0,
            sync_worst,sync_skipped,sync_unadmitted,sync_compares,sync_mismatches,saves,save_ms,refused);
        sync_rollbacks=sync_skipped=sync_mismatches=sync_unadmitted=sync_compares=0;sync_restore_sum=sync_replay_sum=sync_worst=0;
        sync_frame_ms_sum=0;
    }
    {   /* the tape holds 600 frames: start a new one well before it fills */
        Doa3InputTapeStats st={0};doa3_input_tape_stats(sync_tape,&st);
        if(st.frames>=DOA3_INPUT_TAPE_FRAMES-8){
            doa3_input_tape_destroy(sync_tape);sync_tape=doa3_input_tape_create(DOA3_INPUT_TAPE_FRAMES);
            if(!sync_tape){sync_stop("input tape renewal failed");return;}
        }
    }
    sync_auditing=xbox_kernel_replay_audit_begin();
    if(!sync_auditing || !doa3_input_tape_record(sync_tape,id+1)){sync_stop("input/audit restart refused");return;}
    xbox_det_frame();mcpx_apu_det_frame();
}
void doa3_fight_window_probe_boundary(void){
    if(sync_mode<0){
        const char *v=getenv("DOA3_ROLLBACK_SYNC_TEST");
        sync_mode=v && *v && strcmp(v,"0") && doa3_rollback_tasks_enabled();
        if(sync_mode){
            extern int g_doa3_logic_only,g_doa3_skip_map;
            sync_depth=(unsigned)atoi(v);
            if(sync_depth<1)sync_depth=1;
            if(sync_depth>DOA3_NATIVE_REPLAY_MAX_FRAMES-1)sync_depth=DOA3_NATIVE_REPLAY_MAX_FRAMES-1;
            v=getenv("DOA3_ROLLBACK_SYNC_EVERY");if(v && atoi(v)>0)sync_every=(unsigned)atoi(v);
            v=getenv("DOA3_ROLLBACK_SYNC_COMPARE");if(v && *v)sync_compare=(unsigned)atoi(v);
            g_doa3_logic_only=1;g_doa3_skip_map=1;
            enabled=0;   /* the window test does not run alongside */
        }
    }
    if(sync_mode){sync_boundary();return;}
    if(enabled<0){
        const char *v=getenv("DOA3_ROLLBACK_WINDOW_TEST");
        enabled=v && !strcmp(v,"1") && doa3_rollback_tasks_enabled();
        v=getenv("DOA3_ROLLBACK_REPLAY_TEST");
        replay=v && !strcmp(v,"1");
        combat=replay && armed("DOA3_ROLLBACK_COMBAT_TEST");
        long_window=combat && armed("DOA3_ROLLBACK_LONG_TEST");
        /* Bounded repeated measurements of one saved state/input tape. No
         * detailed profiling; every repetition must verify and recover. */
        repeats=replay && armed("DOA3_ROLLBACK_REPEAT_TEST") ? 3:1;
        texture_ab=replay && armed("DOA3_ROLLBACK_TEXTURE_AB_TEST");
        packet_ab=replay && armed("DOA3_ROLLBACK_PACKET_AB_TEST");
        geometry_ab=replay && armed("DOA3_ROLLBACK_GEOMETRY_AB_TEST");
        render_ab=replay && armed("DOA3_ROLLBACK_RENDER_AB_TEST");
        prep_ab=replay && armed("DOA3_ROLLBACK_PREP_AB_TEST");
        guest_ab=replay && armed("DOA3_ROLLBACK_GUEST_RENDER_AB_TEST");
        delta_test=replay && armed("DOA3_ROLLBACK_DELTA_TEST");
        {   /* doa3.1-rollback diagnostics: mismatch map; whole-render skip map */
            extern int g_doa3_skip_map;
            extern void doa3_fight_checkpoint_diff_map_enable(void);
            extern int g_doa3_skip_head;
            if(replay && armed("DOA3_ROLLBACK_SKIP_MAP")){g_doa3_skip_map=1;guest_ab=1;}
            if(replay && combat && armed("DOA3_ROLLBACK_LOGIC_ONLY")){g_doa3_logic_only=1;g_doa3_skip_map=1;guest_ab=1;}
            {   const char *h=getenv("DOA3_ROLLBACK_SKIP_HEAD");
                g_doa3_skip_head=h ? atoi(h):0;
                if(g_doa3_skip_head<0)g_doa3_skip_head=0;
                if(g_doa3_skip_head)fprintf(stderr,"[ROLLBACK-SKIPMAP] only the first %d replay frame(s) hidden; the rest render\n",g_doa3_skip_head); }
            if(g_doa3_skip_map || armed("DOA3_ROLLBACK_DIFF_MAP"))doa3_fight_checkpoint_diff_map_enable();
        }
        if(texture_ab+packet_ab+geometry_ab+render_ab+prep_ab+guest_ab>1){
            enabled=0;
            fprintf(stderr,"[ROLLBACK-WINDOW] refused: choose one A/B optimization\n");
        }
        if(texture_ab || packet_ab || geometry_ab || render_ab || prep_ab || guest_ab)repeats=4;
        const char *conflicts[]={"DOA3_ROLLBACK_CHECKPOINT_TEST","DOA3_ROLLBACK_CAPTURE",
            "DOA3_ROLLBACK_FRAME_CAPTURE","DOA3_ROLLBACK_OWNERSHIP_AUDIT",
            "DOA3_ROLLBACK_INPUT_AUDIT","DOA3_ROLLBACK_RENDER_AUDIT"};
        for(unsigned i=0;enabled && i<sizeof(conflicts)/sizeof(conflicts[0]);++i)
            if(armed(conflicts[i])){enabled=0;fprintf(stderr,"[ROLLBACK-WINDOW] refused: conflicting diagnostic %s\n",conflicts[i]);}
    }
    if(!enabled)return;
    if(settling){
        if(!g_xbox_det_active || !g_apu_det || xbox_det_epoch()!=epoch || !xbox_fiber_is_primary())fatal("settling ownership changed");
        if(!eligible()){fprintf(stderr,"[ROLLBACK-WINDOW] refused: mode changed during clock handoff\n");finish(0);return;}
        {   /* doa3.1-rollback, DOA3_ROLLBACK_CLOCK_ONLY=1: five seconds under the
             * controlled clock/APU stepping, then back, with no checkpoint,
             * restore or replay -- isolates the music cut-out. Runs once. */
            static int clock_only=-1;
            if(clock_only<0)clock_only=armed("DOA3_ROLLBACK_CLOCK_ONLY");
            if(clock_only){
                if(++settle_frames<300){xbox_det_frame();mcpx_apu_det_frame();return;}
                fprintf(stderr,"[ROLLBACK-CLOCK] 300 frames under the controlled clock, no checkpoint or replay; back to the normal clock\n");
                combat=0;finish(1);return;
            }
        }
        if(!xbox_fiber_converted_clock_ready() || (combat && !attacking())){
            if(++settle_frames>=120){
                fprintf(stderr,"[ROLLBACK-WINDOW] refused: worker/combat readiness exceeded 120 intervals\n");finish(0);return;
            }
            xbox_det_frame();mcpx_apu_det_frame();return;
        }
        settling=0;
        fprintf(stderr,"[ROLLBACK-WINDOW] deterministic worker parks verified after %u intervals\n",settle_frames+1);
        goto capture_start;
    }
    if(recording){
        if(!g_xbox_det_active || !g_apu_det || xbox_det_epoch()!=epoch || !xbox_fiber_is_primary())fatal("session ownership changed");
        int input_ok=doa3_input_tape_end(tape);
        ++completed;
        if(!input_ok || !eligible() || g_doa3_frames_presented-presented!=completed){
            fprintf(stderr,"[ROLLBACK-WINDOW] refused before rewind: interval/input eligibility\n");finish(0);return;
        }
        loop.intervals=completed;
        if(delta_test_on){extern void doa3_fight_checkpoint_delta_boundary(unsigned);doa3_fight_checkpoint_delta_boundary(completed);}
        if(completed<window){
            if(!doa3_input_tape_record(tape,completed+1)){finish(0);return;}
            xbox_det_frame();mcpx_apu_det_frame();return;
        }
        if(auditing){
            XboxKernelReplayAudit audit;
            if(!xbox_kernel_replay_audit_end(&audit))fatal("kernel audit end");
            auditing=0;
            fprintf(stderr,"[ROLLBACK-REPLAY] original kernel audit: accepted=%u unsupported=%u invalid=%u unknown=%u\n",
                audit.accepted,audit.rejected,audit.invalid,audit.unknown);
            for(unsigned i=0;i<400;++i)if(audit.unsupported[i])
                fprintf(stderr,"[ROLLBACK-REPLAY] unsupported original ordinal=%u calls=%u\n",i,audit.unsupported[i]);
            if(audit.rejected || audit.invalid || audit.accepted>XBOX_REPLAY_KERNEL_CALL_LIMIT*window){
                fprintf(stderr,"[ROLLBACK-REPLAY] refused before rewind: original batch exceeds kernel admission/budget\n");finish(0);return;
            }
        }
        Doa3InputTapeStats stats={0};doa3_input_tape_stats(tape,&stats);
        fprintf(stderr,"[ROLLBACK-WINDOW] native interval complete: presentations=%u input_frames=%u events=%u reads=%u ports=%X rumble=%u failed=%d\n",
            g_doa3_frames_presented-presented,stats.frames,stats.events,stats.state_reads,stats.ports,stats.rumble_calls,stats.failed);
        /* Token marks which parked boundary the image belongs to. We never
         * execute with the older token/native caller locals: return to end
         * is mandatory. A replay runner must supply its actual continuation. */
        combat_state("endpoint");
        Doa3CheckpointSession *session=doa3_fight_checkpoint_session(fight);
        Doa3CheckpointImage *end=doa3_checkpoint_image_capture(session,window);
        if(!end){fprintf(stderr,"[ROLLBACK-WINDOW] refused before rewind: endpoint capture\n");finish(0);return;}
        {   extern void doa3_fight_checkpoint_delta_report(Doa3FightCheckpoint *,const Doa3CheckpointImage *,
                const Doa3CheckpointImage *,unsigned);
            doa3_fight_checkpoint_delta_report(fight,start,end,window); }
        if(delta_test_on){
            extern int doa3_fight_checkpoint_delta_verify(Doa3FightCheckpoint *,const Doa3CheckpointImage *,
                const Doa3CheckpointImage *,unsigned);
            doa3_fight_checkpoint_delta_verify(fight,start,end,window);delta_test_on=0;
        }
        Doa3CheckpointComparison difference={0};
        Doa3NativeFrame native={&loop,tape,window};
        ULONGLONG verify_started=GetTickCount64();
        int result=0;
        for(unsigned repeat=0;repeat<repeats;++repeat){
            if(repeats>1)fprintf(stderr,"[ROLLBACK-REPEAT] trial=%u sample=%u/%u frames=%u\n",trial,repeat+1,repeats,window);
            int previous_upload_mode=0;
            if(texture_ab){
                /* ABBA / BAAB balances ordering without changing the saved
                 * input tape, resolution or endpoint verification. */
                int force=(repeat==0 || repeat==3) == (trial%2!=0);
                previous_upload_mode=pgraph_d3d11_force_guest_uploads(force);
                fprintf(stderr,"[ROLLBACK-TEXTURE-AB] mode=%s\n",force ? "force-upload":"exact-reuse");
            }
            int previous_packet_mode=0;
            uint64_t bulk_before=0;
            if(packet_ab){
                int scalar=(repeat==0 || repeat==3) == (trial%2!=0);
                previous_packet_mode=pgraph_d3d11_force_scalar_data(scalar);
                bulk_before=pgraph_d3d11_bulk_words();
                fprintf(stderr,"[ROLLBACK-PACKET-AB] mode=%s\n",scalar ? "scalar":"bulk");
            }
            int previous_geometry_mode=0;
            uint64_t geometry_before[4]={0},geometry_after[4]={0};
            if(geometry_ab){
                int rebuild=(repeat==0 || repeat==3)==(trial%2!=0);
                previous_geometry_mode=pgraph_d3d11_force_geometry_rebuild(rebuild);
                pgraph_d3d11_geometry_stats(&geometry_before[0],&geometry_before[1],&geometry_before[2],&geometry_before[3]);
                fprintf(stderr,"[ROLLBACK-GEOMETRY-AB] mode=%s\n",rebuild ? "rebuild":"reuse");
            }
            if(render_ab){
                /* Same recorded input/state, ABBA then BAAB on alternating
                 * trials. The gate is scoped to execution, never recovery. */
                native.suppress_host_draws=(repeat==0 || repeat==3)!=(trial%2!=0);
                fprintf(stderr,"[ROLLBACK-RENDER-AB] mode=%s\n",native.suppress_host_draws ? "host-draws-off":"full-render");
            }
            if(prep_ab){
                native.skip_intermediate_preparation=(repeat==0 || repeat==3)!=(trial%2!=0);
                fprintf(stderr,"[ROLLBACK-PREP-AB] mode=%s\n",native.skip_intermediate_preparation ? "skip-intermediate-preparation":"full-render");
            }
            if(guest_ab){
                /* Both arms use the verified host-preparation skip. Measure
                 * the additional guest-side saving, not the old gain twice. */
                native.skip_intermediate_preparation=1;
                native.skip_intermediate_guest_render=(repeat==0 || repeat==3)!=(trial%2!=0);
                g_doa3_logic_only_arm=native.skip_intermediate_guest_render;
                if(g_doa3_logic_only && !repeat)logic_only_tasks(trial);
                fprintf(stderr,"[ROLLBACK-GUEST-RENDER-AB] mode=%s\n",
                    native.skip_intermediate_guest_render ? "preserve-commands-skip-deformation":"host-preparation-only");
            }
            int render_probe=native.suppress_host_draws || native.skip_intermediate_preparation || native.skip_intermediate_guest_render;
            result=render_probe ? doa3_fight_checkpoint_render_probe(fight,start,0,end,window,
                doa3_native_frame_run,&native,&difference):
                replay ? doa3_fight_checkpoint_replay_check(fight,start,0,end,window,
                doa3_native_frame_run,&native,&difference):
                doa3_fight_checkpoint_rewind_check(fight,start,0,end,window,&difference);
            if(texture_ab)pgraph_d3d11_force_guest_uploads(previous_upload_mode);
            if(geometry_ab){
                pgraph_d3d11_force_geometry_rebuild(previous_geometry_mode);
                pgraph_d3d11_geometry_stats(&geometry_after[0],&geometry_after[1],&geometry_after[2],&geometry_after[3]);
                fprintf(stderr,"[ROLLBACK-GEOMETRY-AB] hits=%llu misses=%llu vertices=%llu resident_draws=%llu\n",
                    (unsigned long long)(geometry_after[0]-geometry_before[0]),
                    (unsigned long long)(geometry_after[1]-geometry_before[1]),
                    (unsigned long long)(geometry_after[2]-geometry_before[2]),
                    (unsigned long long)(geometry_after[3]-geometry_before[3]));
            }
            if(packet_ab){
                pgraph_d3d11_force_scalar_data(previous_packet_mode);
                fprintf(stderr,"[ROLLBACK-PACKET-AB] bulk_words=%llu\n",
                    (unsigned long long)(pgraph_d3d11_bulk_words()-bulk_before));
            }
            if(result<0)fatal("rewind/return verification");
            if(result!=1 && !(render_probe && (result==2 || result==3)))break;
        }
        fprintf(stderr,"[ROLLBACK-WINDOW] %s; %s\n",
            result>0 ? ((render_ab || prep_ab || guest_ab) ? "render A/B completed: CPU/logical state matched; skipped-frame pixels checked separately":"passed: checkpoint comparisons matched"):"refused or recovered to current endpoint",
            replay ? "recorded-input replay trial":"one native interval recorded, no frames replayed");
        fprintf(stderr,"[ROLLBACK-WINDOW] diagnostic_ms=%llu verify_ms=%llu frames=%u\n",
            (unsigned long long)(GetTickCount64()-capture_started),(unsigned long long)(GetTickCount64()-verify_started),window);
        g_doa3_logic_only_arm=0;
        finish(result==1 || ((render_ab || prep_ab || guest_ab) && (result==2 || result==3)));return;
    }
    if(!eligible() || g_xbox_det_active || g_apu_det){warmup=0;return;}
    if(warmup<(combat ? 480u:120u))++warmup;
    if(warmup<(combat ? 480u:120u) || (combat && !attacking()))return;
    ++trial;window=long_window ? (trial>3 ? 8:4):(combat && trial>3 ? 2:1);completed=0;
    doa3_loop_init(&loop);
    xbox_det_bases bases;xbox_det_current_bases(&bases);xbox_det_begin(&bases);
    epoch=xbox_det_epoch();mcpx_apu_det_begin(mcpx_apu_xgscnt_now());
    if(!g_apu_det){fprintf(stderr,"[ROLLBACK-WINDOW] refused: audio clock ownership\n");finish(0);return;}
    /* Vblank workers enter a different native park when the clock changes:
     * ordinary play blocks on B1A4B1A4, controlled time cooperatively yields.
     * Let that handoff finish BEFORE keeping an image. The scheduler must
     * continue rejecting snapshots across different native park sites. */
    settling=1;settle_frames=0;
    fprintf(stderr,"[ROLLBACK-WINDOW] settling native workers into controlled-clock waits\n");
    xbox_det_frame();mcpx_apu_det_frame();return;
capture_start:
    capture_started=GetTickCount64();combat_state("start");
    fight=doa3_fight_checkpoint_create(&loop);
    if(fight)start=doa3_checkpoint_image_capture(doa3_fight_checkpoint_session(fight),0);
    if(start)tape=doa3_input_tape_create(window);
    if(!tape || !doa3_input_tape_record(tape,1)){
        fprintf(stderr,"[ROLLBACK-WINDOW] refused before advance: start capture/input recording\n");finish(0);return;
    }
    if(replay && !(auditing=xbox_kernel_replay_audit_begin())){
        fprintf(stderr,"[ROLLBACK-REPLAY] refused before advance: kernel audit ownership\n");finish(0);return;
    }
    {   /* rollback.17 delta snapshot test */
        extern int doa3_fight_checkpoint_delta_begin(void);
        if(delta_test && !doa3_fight_checkpoint_delta_begin())delta_test_on=0; else delta_test_on=delta_test;
    }
    recording=1;presented=g_doa3_frames_presented;
    fprintf(stderr,"[ROLLBACK-WINDOW] start captured; recording %u native intervals\n",window);
    /* Same fixed-rate clock/APU progression as the deterministic session. The
     * original native game runs once between this and the next A64B0 hook. */
    xbox_det_frame();mcpx_apu_det_frame();
}
