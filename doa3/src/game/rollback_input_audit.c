/* Bounded input recording only. No live replay or game restore. */
#include "rollback_input_tape.h"
#include "online/rollback_tasks.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include "recomp/recomp_types.h"
#include "training_dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int enabled=-1;
static unsigned warmup,frame;
static Doa3InputTape *tape;
static int armed(const char *name)
{ const char *v=getenv(name);return v && *v && strcmp(v,"0"); }
static void finish(const char *reason)
{
    Doa3InputTapeStats stats={0};
    if(tape)doa3_input_tape_stats(tape,&stats);
    fprintf(stderr,"[ROLLBACK-INPUT] %s: frames=%u events=%u state_reads=%u ports=%X rumble_calls=%u failed=%d; record only, no replay\n",
        reason,stats.frames,stats.events,stats.state_reads,stats.ports,stats.rumble_calls,stats.failed);
    doa3_input_tape_destroy(tape);tape=NULL;enabled=0;
}
void doa3_input_audit_boundary(void)
{
    if(enabled<0) {
        const char *v=getenv("DOA3_ROLLBACK_INPUT_AUDIT");
        enabled=v && !strcmp(v,"1") && doa3_rollback_tasks_enabled();
        if(enabled && (armed("DOA3_ROLLBACK_OWNERSHIP_AUDIT") || armed("DOA3_ROLLBACK_CAPTURE") ||
            armed("DOA3_ROLLBACK_FRAME_CAPTURE") || armed("DOA3_ROLLBACK_RENDER_AUDIT"))) {
            enabled=0;fprintf(stderr,"[ROLLBACK-INPUT] refused: another audit is armed\n");
        }
    }
    if(!enabled)return;
    TrainingStatus status;training_status(&status);
    int eligible=xbox_fiber_is_primary() && !g_xbox_det_active && MEM8(0x5A2858)==7 &&
        MEM8(0x596A37)==1 && !MEM8(0x596AA5) && status.mode==TRAINING_IDLE;
    if(!eligible) {
        warmup=0;
        if(tape)finish("stopped: left eligible Practice");
        return;
    }
    if(!tape) {
        if(++warmup<120)return;
        tape=doa3_input_tape_create(DOA3_INPUT_TAPE_FRAMES);
        if(!tape){finish("allocation failed");return;}
        fprintf(stderr,"[ROLLBACK-INPUT] recording 600 dispatcher intervals; fresh controller reads retained\n");
    } else {
        if(!doa3_input_tape_end(tape)){finish("rejected");return;}
        if(++frame==DOA3_INPUT_TAPE_FRAMES){finish("complete");return;}
    }
    if(!doa3_input_tape_record(tape,frame))finish("frame begin rejected");
}
