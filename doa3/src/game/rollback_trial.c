#include "rollback_trial.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include <stdlib.h>
#include <string.h>

struct Doa3ReplayTrial {
    Doa3TrialConfig c;
    DWORD owner;
    int running;
    Doa3CheckpointImage *original;
    Doa3TrialReport report;
};
static Doa3ReplayTrial *active_trial;
static int boundary(const Doa3ReplayTrial *t)
{ return t && t->owner==GetCurrentThreadId() && xbox_fiber_is_primary(); }
static int same_epoch(uint32_t epoch)
{ return g_xbox_det_active && xbox_det_epoch()==epoch && xbox_fiber_is_primary(); }
Doa3ReplayTrial *doa3_trial_create(const Doa3TrialConfig *c)
{
    if(!c || !c->checkpoints || !c->inputs || !c->enter || !c->leave ||
       !c->output_begin || !c->output_end || !c->advance || !c->verify ||
       !xbox_fiber_is_primary())return NULL;
    Doa3ReplayTrial *t=calloc(1,sizeof(*t));
    if(t){t->c=*c;t->owner=GetCurrentThreadId();}return t;
}
int doa3_trial_report(const Doa3ReplayTrial *t,Doa3TrialReport *out)
{ if(!t || t->owner!=GetCurrentThreadId() || !out)return 0;*out=t->report;return 1; }
int doa3_trial_destroy(Doa3ReplayTrial *t)
{
    if(!t)return 1;
    if(!boundary(t) || t->running || t->report.fatal || t->original)return 0;
    free(t);return 1;
}
int doa3_trial_run(Doa3ReplayTrial *t,const Doa3CheckpointImage *start,uint64_t first,uint64_t last)
{
    if(!boundary(t))return 0;
    if(t->report.fatal)return -1;
    if(active_trial && active_trial->report.fatal)return -1;
    if(t->running || active_trial)return 0;
    memset(&t->report,0,sizeof(t->report));t->report.reason=DOA3_TRIAL_PREFLIGHT;
    if(doa3_checkpoint_poisoned(t->c.checkpoints)) {
        t->report.fatal=1;active_trial=t;return -1;
    }
    if(!start || last<=first || last-first>DOA3_TRIAL_MAX_FRAMES ||
       !doa3_input_tape_can_replay(t->c.inputs,first+1,(unsigned)(last-first)))return 0;
    uint32_t epoch=xbox_det_epoch();if(!epoch)return 0;
    Doa3TrialConfig *c=&t->c;int result=0,entered=0,guarding=0;
    uint64_t restored=UINT64_MAX;
    t->running=1;active_trial=t;t->report.frame=last;
    t->report.reason=DOA3_TRIAL_ENTER;
    if(!c->enter(c->context))goto done;
    entered=1;t->report.reason=DOA3_TRIAL_VERIFY;
    if(!same_epoch(epoch) || !c->verify(c->context,last))goto done;
    t->report.reason=DOA3_TRIAL_CAPTURE;
    t->original=doa3_checkpoint_image_capture(c->checkpoints,last);
    if(!t->original)goto done;
    t->report.reason=DOA3_TRIAL_KERNEL;
    if(!xbox_kernel_replay_begin())goto done;
    guarding=1;
    t->report.reason=DOA3_TRIAL_OUTPUT;
    if(!c->output_begin(c->context))goto done;
    t->report.reason=DOA3_TRIAL_RESTORE;t->report.frame=first;
    if(!same_epoch(epoch) || doa3_checkpoint_image_restore(c->checkpoints,start,&restored)!=1 || restored!=first)goto recover;
    t->report.reason=DOA3_TRIAL_VERIFY;
    if(!c->verify(c->context,first))goto recover;
    for(uint64_t f=first+1;;++f) {
        t->report.frame=f;t->report.reason=DOA3_TRIAL_EPOCH;
        if(!same_epoch(epoch))goto recover;
        t->report.reason=DOA3_TRIAL_INPUT;
        if(!doa3_input_tape_replay(c->inputs,f))goto recover;
        t->report.reason=DOA3_TRIAL_ADVANCE;
        int advanced=c->advance(c->context,f);
        if(!xbox_kernel_replay_healthy()){t->report.reason=DOA3_TRIAL_KERNEL;goto recover;}
        if(!advanced)goto recover;
        ++t->report.advanced;t->report.reason=DOA3_TRIAL_INPUT;
        if(!doa3_input_tape_end(c->inputs))goto recover;
        t->report.reason=DOA3_TRIAL_EPOCH;
        if(!same_epoch(epoch))goto recover;
        t->report.reason=DOA3_TRIAL_VERIFY;
        if(!c->verify(c->context,f))goto recover;
        if(!xbox_kernel_replay_healthy()){t->report.reason=DOA3_TRIAL_KERNEL;goto recover;}
        t->report.reason=DOA3_TRIAL_EPOCH;
        if(!same_epoch(epoch))goto recover;
        if(f==last)break; /* no uint64 wrap at the final interval */
    }
    t->report.reason=DOA3_TRIAL_OUTPUT;
    if(!c->output_end(c->context,0))goto recover;
    t->report.reason=DOA3_TRIAL_OK;result=1;goto done;
recover:
    /* Never abort the tape or reopen output while partially restored. A
     * poisoned transaction/changed session cannot safely use the anchor. */
    if(!same_epoch(epoch) || doa3_checkpoint_poisoned(c->checkpoints) ||
       doa3_checkpoint_image_restore(c->checkpoints,t->original,&restored)!=1 || restored!=last ||
       !c->verify(c->context,last) || !same_epoch(epoch))goto fatal;
    if(!doa3_input_tape_abort(c->inputs) || !c->output_end(c->context,1))goto fatal;
    t->report.recovered=1;
done:
    if(t->original) {
        if(!doa3_checkpoint_image_discard(c->checkpoints,t->original)) {
            t->report.reason=DOA3_TRIAL_CLEANUP;goto fatal;
        }
        t->original=NULL;
    }
    if(guarding){
        xbox_kernel_replay_report(&t->report.kernel);
        if(!xbox_kernel_replay_end(t->report.recovered))goto fatal;
        guarding=0;
    }
    if(entered)c->leave(c->context);
    t->running=0;active_trial=NULL;return result;
fatal:
    if(guarding)xbox_kernel_replay_report(&t->report.kernel);
    /* State/gates intentionally retained; caller must stop. */
    if(t->report.reason!=DOA3_TRIAL_CLEANUP)t->report.reason=DOA3_TRIAL_RECOVERY;
    t->report.fatal=1;t->running=0;return -1;
}
