/* Explicitly armed restore-only test at A64B0, before the dispatcher call.
 * No frame advance, native-loop replacement, automatic capture or networking. */
#include "rollback_fight_checkpoint.h"
#include "online/rollback_tasks.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include "recomp/recomp_types.h"
#include "training_dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern volatile int g_apu_det;
void mcpx_apu_det_begin(uint64_t);
void mcpx_apu_det_end(void);
uint64_t mcpx_apu_xgscnt_now(void);
static int armed(const char *name){const char *v=getenv(name);return v && *v && strcmp(v,"0");}
void doa3_fight_checkpoint_probe_boundary(void){
    static int enabled=-1;static unsigned warmup;
    if(enabled<0){
        const char *v=getenv("DOA3_ROLLBACK_CHECKPOINT_TEST");
        enabled=v && !strcmp(v,"1") && doa3_rollback_tasks_enabled();
        if(enabled && (armed("DOA3_ROLLBACK_CAPTURE") || armed("DOA3_ROLLBACK_FRAME_CAPTURE") ||
            armed("DOA3_ROLLBACK_OWNERSHIP_AUDIT") || armed("DOA3_ROLLBACK_INPUT_AUDIT") ||
            armed("DOA3_ROLLBACK_RENDER_AUDIT") || armed("DOA3_ROLLBACK_WINDOW_TEST"))){
            enabled=0;fprintf(stderr,"[ROLLBACK-CHECKPOINT] refused: another diagnostic is armed\n");
        }
    }
    if(!enabled)return;
    TrainingStatus training;training_status(&training);
    if(!xbox_fiber_is_primary() || g_xbox_det_active || g_apu_det || MEM8(0x5A2858)!=7 ||
       MEM8(0x596A37)!=1 || MEM8(0x596AA5) || training.mode!=TRAINING_IDLE){warmup=0;return;}
    if(++warmup<120)return;enabled=0;
    /* Native caller locals remain on the same stack and are never advanced;
     * a fresh outer-loop token is valid only for this identity restore. */
    Doa3LoopState loop;doa3_loop_init(&loop);
    xbox_det_bases bases;xbox_det_current_bases(&bases);xbox_det_begin(&bases);
    mcpx_apu_det_begin(mcpx_apu_xgscnt_now());
    Doa3CheckpointComparison difference={0};
    int result=g_apu_det ? doa3_fight_checkpoint_roundtrip(&loop,&difference):0;
    if(result<0){
        fprintf(stderr,"[ROLLBACK-CHECKPOINT] FATAL: restore could not be verified; stopping with output gated\n");
        fflush(stderr);abort();
    }
    if(g_apu_det)mcpx_apu_det_end();xbox_det_end();
    fprintf(stderr,"[ROLLBACK-CHECKPOINT] %s; same-state restore, no frames replayed\n",
        result==1 ? "passed: RAM, CPU, continuations, clock, files, input, audio and renderer match":"refused before mutation or safely compensated");
}
