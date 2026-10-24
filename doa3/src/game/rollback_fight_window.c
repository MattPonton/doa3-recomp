/* One real native dispatcher-to-dispatcher interval, then a rewind/return
 * check; the separate replay opt-in also re-executes the recorded interval. */
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
    if(auditing){XboxKernelReplayAudit audit;if(!xbox_kernel_replay_audit_end(&audit))fatal("kernel audit cleanup");auditing=0;}
    if(tape && (!doa3_input_tape_abort(tape) || !doa3_input_tape_destroy(tape)))fatal("input cleanup");
    tape=NULL;
    if(fight && !doa3_fight_checkpoint_destroy(fight))fatal("checkpoint cleanup");
    fight=NULL;start=NULL;recording=settling=enabled=0;
    if(g_apu_det)mcpx_apu_det_end();
    if(g_xbox_det_active)xbox_det_end();
    if(combat){
        fprintf(stderr,"[ROLLBACK-COMBAT] trial=%u frames=%u result=%s\n",trial,window,success ? ((render_ab || prep_ab || guest_ab) ? "state-matched (render A/B)":"matched"):"stopped");
        if(success && trial<6){enabled=1;warmup=0;}
        else fprintf(stderr,"[ROLLBACK-COMBAT] sequence %s after %u trials\n",success ? "passed":"stopped",trial);
    }
}
void doa3_fight_window_probe_boundary(void){
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
            if(result!=1 && !(render_probe && result==2))break;
        }
        fprintf(stderr,"[ROLLBACK-WINDOW] %s; %s\n",
            result>0 ? ((render_ab || prep_ab || guest_ab) ? "render A/B completed: CPU/logical state matched; skipped-frame pixels checked separately":"passed: checkpoint comparisons matched"):"refused or recovered to current endpoint",
            replay ? "recorded-input replay trial":"one native interval recorded, no frames replayed");
        fprintf(stderr,"[ROLLBACK-WINDOW] diagnostic_ms=%llu verify_ms=%llu frames=%u\n",
            (unsigned long long)(GetTickCount64()-capture_started),(unsigned long long)(GetTickCount64()-verify_started),window);
        finish(result==1 || ((render_ab || prep_ab || guest_ab) && result==2));return;
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
    recording=1;presented=g_doa3_frames_presented;
    fprintf(stderr,"[ROLLBACK-WINDOW] start captured; recording %u native intervals\n",window);
    /* Same fixed-rate clock/APU progression as the deterministic session. The
     * original native game runs once between this and the next A64B0 hook. */
    xbox_det_frame();mcpx_apu_det_frame();
}
