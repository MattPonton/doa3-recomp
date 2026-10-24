/* Real transaction and input tape; tiny mutable state and output endpoint.
 * Fatal cases run in separate processes because their gate must remain held. */
#include "game/rollback_trial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
volatile int g_xbox_det_active=1;
static uint32_t current_epoch=1;
static int primary=1;
static unsigned reads,writes;
static DWORD clock_owner;
int xbox_det_checkpoint_acquire(void){return g_xbox_det_active && primary && GetCurrentThreadId()==clock_owner;}
int xbox_det_checkpoint_release(void){return GetCurrentThreadId()==clock_owner;}
uint32_t xbox_det_epoch(void){return current_epoch;}
int xbox_fiber_is_primary(void){return primary;}
DWORD xbox_InputGetState(DWORD port,XBOX_INPUT_STATE *out){
    CHECK(port==0 && out);memset(out,0,sizeof(*out));out->Gamepad.wButtons=(WORD)++reads;return 0;
}
DWORD xbox_InputSetState(DWORD port,const XBOX_VIBRATION *v){CHECK(port==0 && v);++writes;return 0;}
DWORD xbox_InputHostMask(void){CHECK(0);return 0;}
DWORD xbox_InputTakeReconfiguredMask(void){CHECK(0);return 0;}
BYTE xbox_InputGuestSubtype(int port){(void)port;CHECK(0);return 0;}
enum { NONE,ENTER,INITIAL,CAPTURE,BEGIN,RESTORE,START,ADVANCE,BAD_INPUT,UNDER,VERIFY,END,
       RECOVERY,RECOVERY_VERIFY,CANCEL,EPOCH,CLEANUP,KERNEL };
typedef struct World World;
typedef struct {World *w;unsigned index;} Part;
struct World {
    unsigned values[DOA3_CP_PARTS],expected[4][DOA3_CP_PARTS];
    Part parts[DOA3_CP_PARTS];
    Doa3CheckpointSession *session;Doa3CheckpointImage *start;
    Doa3InputTape *tape;Doa3ReplayTrial *trial;
    unsigned steps,restores,leaves,ends,enters;
    int fence,held,gate,fault,triggered,discard_fail;
};
static int acquire(void *v){World *w=v;if(w->fence || w->discard_fail)return 0;w->fence=1;return 1;}
static void release(void *v){World *w=v;CHECK(w->fence);w->fence=0;}
static uint64_t epoch(void *v){(void)v;return current_epoch;}
static int capture(void *v,void *out,size_t n){
    Part *p=v;World *w=p->w;CHECK(w->fence && n==sizeof(unsigned));
    if(w->fault==CAPTURE)return 0;memcpy(out,&w->values[p->index],n);return 1;
}
static int validate(void *v,const void *p,size_t n){CHECK(((Part *)v)->w->fence);return p && n==sizeof(unsigned);}
static int can_restore(void *v){World *w=((Part *)v)->w;CHECK(w->fence);return w->gate;}
static int restore(void *v,const void *in,size_t n){
    Part *p=v;World *w=p->w;CHECK(w->fence && w->gate);memcpy(&w->values[p->index],in,n);
    if(!p->index){
        ++w->restores;
        if(w->fault==RESTORE && !w->triggered){w->triggered=1;return 0;}
        if(w->fault==RECOVERY && w->restores>1)return 0;
    }return 1;
}
static int enter(void *v){World *w=v;CHECK(!w->held);++w->enters;if(w->fault==ENTER)return 0;w->held=1;return 1;}
static void leave(void *v){World *w=v;CHECK(w->held && !w->gate);w->held=0;++w->leaves;}
static int begin(void *v){World *w=v;CHECK(w->held && !w->gate);if(w->fault==BEGIN)return 0;w->gate=1;return 1;}
static int end(void *v,int cancel){
    World *w=v;CHECK(w->held && w->gate);
    CHECK(!memcmp(w->values,w->expected[3],sizeof(w->values)));
    if(w->fault==END && !cancel)return 0;
    if(w->fault==CANCEL && cancel)return 0;
    w->gate=0;++w->ends;if(w->fault==CLEANUP)w->discard_fail=1;return 1;
}
static void step(World *w,unsigned frame,int replay){
    XBOX_INPUT_STATE input={0};XBOX_VIBRATION vibration={(WORD)frame,(WORD)(frame*2)};
    if(replay && w->fault==BAD_INPUT){CHECK(doa3_input_state(1,&input)==ERROR_INVALID_DATA);}
    else CHECK(!doa3_input_state(0,&input));
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)w->values[i]=w->values[i]*13+input.Gamepad.wButtons+frame+i;
    if(replay && w->fault==UNDER)return;
    DWORD status=doa3_input_rumble(0,&vibration);
    CHECK(status==(DWORD)(replay && w->fault==BAD_INPUT ? ERROR_INVALID_DATA:0));
}
static int advance(void *v,uint64_t frame){
    World *w=v;CHECK(w->held && w->gate && !w->fence);++w->steps;
    /* A callback cannot nest the runner or destroy it while in flight. */
    CHECK(!doa3_trial_run(w->trial,w->start,0,3));CHECK(!doa3_trial_destroy(w->trial));
    step(w,(unsigned)frame,1);
    if(w->fault==KERNEL)CHECK(!xbox_kernel_replay_check(236,0));
    if(w->fault==EPOCH)++current_epoch;
    return w->fault!=ADVANCE;
}
static int verify(void *v,uint64_t frame){
    World *w=v;CHECK(w->held && frame<4);
    if(w->fault==INITIAL)return 0;
    if(w->fault==START && frame==0)return 0;
    if(w->fault==RECOVERY_VERIFY && w->steps && frame==3)return 0;
    if((w->fault==VERIFY || w->fault==RECOVERY || w->fault==RECOVERY_VERIFY || w->fault==CANCEL) && frame==1)return 0;
    return !memcmp(w->values,w->expected[frame],sizeof(w->values));
}
static void setup(World *w){
    memset(w,0,sizeof(*w));Doa3CheckpointConfig cp={0};cp.context=w;cp.acquire=acquire;cp.release=release;cp.epoch=epoch;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i){w->parts[i]=(Part){w,i};cp.parts[i]=(Doa3CheckpointPart){&w->parts[i],sizeof(unsigned),capture,validate,restore,can_restore};}
    w->session=doa3_checkpoint_create(&cp);CHECK(w->session);
    w->start=doa3_checkpoint_image_capture(w->session,0);CHECK(w->start);
    w->tape=doa3_input_tape_create(3);CHECK(w->tape);
    for(unsigned f=1;f<=3;++f){CHECK(doa3_input_tape_record(w->tape,f));step(w,f,0);CHECK(doa3_input_tape_end(w->tape));memcpy(w->expected[f],w->values,sizeof(w->values));}
    Doa3TrialConfig c={w,w->session,w->tape,enter,leave,begin,end,advance,verify};
    w->trial=doa3_trial_create(&c);CHECK(w->trial);
}
static DWORD WINAPI wrong_thread(void *v){World *w=v;CHECK(!doa3_trial_run(w->trial,w->start,0,3));CHECK(!doa3_trial_destroy(w->trial));return 0;}
static void test(int fault){
    World w;setup(&w);unsigned r=reads,s=writes;w.fault=fault;
    CHECK(!doa3_trial_run(w.trial,w.start,3,3));CHECK(!doa3_trial_run(w.trial,w.start,UINT64_MAX,0));
    CHECK(!doa3_trial_run(w.trial,w.start,0,17));CHECK(!doa3_trial_run(w.trial,w.start,0,4));CHECK(!w.enters);
    primary=0;CHECK(!doa3_trial_run(w.trial,w.start,0,3));primary=1;
    HANDLE thread=CreateThread(NULL,0,wrong_thread,&w,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);CloseHandle(thread);
    int result=doa3_trial_run(w.trial,w.start,0,3);Doa3TrialReport report;
    CHECK(doa3_trial_report(w.trial,&report));CHECK(reads==r && writes==s);
    int fatal=fault>=RECOVERY && fault<=CLEANUP;
    CHECK(result==(fatal ? -1:fault==NONE ? 1:0));CHECK(report.fatal==fatal);
    if(fatal){
        CHECK(w.held && !w.leaves && !doa3_trial_destroy(w.trial));
        CHECK(doa3_trial_run(w.trial,w.start,0,3)==-1);
        Doa3TrialConfig other_config={&w,w.session,w.tape,enter,leave,begin,end,advance,verify};
        Doa3ReplayTrial *other=doa3_trial_create(&other_config);CHECK(other);
        CHECK(doa3_trial_run(other,w.start,0,3)==-1);CHECK(doa3_trial_destroy(other));
        if(fault!=CLEANUP)CHECK(w.gate);
        if(fault==EPOCH){XBOX_INPUT_STATE out;CHECK(doa3_input_state(0,&out)==ERROR_INVALID_DATA);CHECK(reads==r);}
        printf("Fatal trial %d: retained exclusion/gates; no resume or destroy.\n",fault);return;
    }
    CHECK(!w.held && !w.gate && !w.fence && !memcmp(w.values,w.expected[3],sizeof(w.values)));
    CHECK(report.recovered==(fault>=RESTORE));
    if(fault==KERNEL)CHECK(report.reason==DOA3_TRIAL_KERNEL && report.kernel.ordinal==236 && report.kernel.reason==XBOX_REPLAY_KERNEL_UNSUPPORTED);
    CHECK(doa3_trial_destroy(w.trial));CHECK(doa3_checkpoint_image_discard(w.session,w.start));
    doa3_checkpoint_destroy(w.session);CHECK(doa3_input_tape_destroy(w.tape));
}
int main(int argc,char **argv){
    clock_owner=GetCurrentThreadId();
    if(argc==2 && !strcmp(argv[1],"poison")){
        World w;setup(&w);w.fault=RECOVERY;w.restores=1;w.gate=1;
        CHECK(doa3_checkpoint_image_restore(w.session,w.start,NULL)==-1);w.gate=0;
        CHECK(doa3_trial_run(w.trial,w.start,0,3)==-1 && !w.enters);
        Doa3TrialReport report;CHECK(doa3_trial_report(w.trial,&report) && report.fatal);
        CHECK(!doa3_trial_destroy(w.trial));
    }
    else if(argc==2){int fault=atoi(argv[1]);CHECK(fault>=RECOVERY && fault<=CLEANUP);test(fault);}
    else{CHECK(argc==1);for(int f=NONE;f<=END;++f)test(f);test(KERNEL);}
    puts("Trial controller: bounded replay, owner/reentry guards, no duplicate input/rumble, checked recovery and fail-closed paths pass.");return 0;
}
