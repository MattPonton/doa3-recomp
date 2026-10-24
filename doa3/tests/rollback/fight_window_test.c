#include "game/rollback_fight_checkpoint.h"
#include "game/rollback_input_tape.h"
#include "game/training_dummy.h"
#include "online/xbox_det.h"
#include "kernel/kernel_replay.h"
#include "game/rollback_native_frame.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
ptrdiff_t g_xbox_mem_offset;
volatile int g_xbox_det_active,g_apu_det;
unsigned g_doa3_frames_presented;
static unsigned begins,ends,advances,apu_advances,captures,checks,recorded;
static int primary=1,training=TRAINING_IDLE,active_tape,fail_end,deny_endpoint;
static unsigned ready_after=1;
static unsigned expected_window=1;
static int compare_failure;
static unsigned fail_on_check;
static int texture_ab,forced_uploads;
static int packet_ab,forced_scalar;
static int geometry_ab,forced_geometry;
static int render_ab,prep_ab,guest_ab,probe_call;
int pgraph_d3d11_force_geometry_rebuild(int force){int old=forced_geometry;forced_geometry=force;return old;}
void pgraph_d3d11_geometry_stats(uint64_t *h,uint64_t *m,uint64_t *v,uint64_t *r){*h=*m=*v=*r=0;}
int pgraph_d3d11_force_guest_uploads(int force){int old=forced_uploads;forced_uploads=force;return old;}
int pgraph_d3d11_force_scalar_data(int force){int old=forced_scalar;forced_scalar=force;return old;}
uint64_t pgraph_d3d11_bulk_words(void){return 0;}
static int audit_active,audit_reject,audit_invalid;
int xbox_kernel_replay_audit_begin(void){CHECK(!audit_active && active_tape && captures==1);audit_active=1;return 1;}
int xbox_kernel_replay_audit_end(XboxKernelReplayAudit *a){
    CHECK(audit_active);audit_active=0;memset(a,0,sizeof(*a));a->accepted=4;
    a->rejected=audit_reject;a->unsupported[236]=audit_reject;a->invalid=audit_invalid;return 1;
}
int xbox_fiber_converted_clock_ready(void){return advances>=ready_after;}
static Doa3LoopState *token;
struct Doa3FightCheckpoint{int used;};static struct Doa3FightCheckpoint fixture;
struct Doa3InputTape{int used;};static struct Doa3InputTape input;
struct Doa3CheckpointImage{uint64_t frame;};static struct Doa3CheckpointImage images[2];
int doa3_native_frame_run(void *v){(void)v;CHECK(0);return 0;}
int doa3_fight_checkpoint_replay_check(Doa3FightCheckpoint *s,const Doa3CheckpointImage *a,uint64_t first,
    const Doa3CheckpointImage *b,uint64_t last,int (*run)(void *),void *v,Doa3CheckpointComparison *d){
    CHECK(run==doa3_native_frame_run && v && !audit_active);
    CHECK(((Doa3NativeFrame *)v)->frames==expected_window);
    CHECK(((Doa3NativeFrame *)v)->tape==&input && captures==2);
    if(render_ab || prep_ab || guest_ab){
        unsigned sample=checks%4;
        int skip=(sample==0 || sample==3)!=(begins%2!=0);
        CHECK(((Doa3NativeFrame *)v)->suppress_host_draws==(render_ab && skip) && probe_call==(guest_ab || skip));
        CHECK(((Doa3NativeFrame *)v)->skip_intermediate_preparation==(guest_ab || (prep_ab && skip)));
        CHECK(((Doa3NativeFrame *)v)->skip_intermediate_guest_render==(guest_ab && skip));
    }else CHECK(!probe_call && !((Doa3NativeFrame *)v)->suppress_host_draws && !((Doa3NativeFrame *)v)->skip_intermediate_preparation);
    if(texture_ab) {
        unsigned sample=checks%4;
        CHECK(forced_uploads==((sample==0 || sample==3)==(begins%2!=0)));
    }
    if(packet_ab) {
        unsigned sample=checks%4;
        CHECK(forced_scalar==((sample==0 || sample==3)==(begins%2!=0)) && !forced_uploads);
    }
    if(geometry_ab){
        unsigned sample=checks%4;
        CHECK(forced_geometry==((sample==0 || sample==3)==(begins%2!=0)) && !forced_uploads && !forced_scalar);
    }
    return doa3_fight_checkpoint_rewind_check(s,a,first,b,last,d);
}
int doa3_fight_checkpoint_render_probe(Doa3FightCheckpoint *s,const Doa3CheckpointImage *a,uint64_t first,
    const Doa3CheckpointImage *b,uint64_t last,int (*run)(void *),void *v,Doa3CheckpointComparison *d){
    CHECK((render_ab || prep_ab || guest_ab) && !probe_call);probe_call=1;
    int result=doa3_fight_checkpoint_replay_check(s,a,first,b,last,run,v,d);
    probe_call=0;return result ? 2:0;
}
int xbox_fiber_is_primary(void){return primary;}
int doa3_rollback_tasks_enabled(void){return 1;}
void training_status(TrainingStatus *s){memset(s,0,sizeof(*s));s->mode=training;}
void xbox_det_current_bases(xbox_det_bases *b){memset(b,0,sizeof(*b));}
void xbox_det_begin(const xbox_det_bases *b){(void)b;CHECK(!g_xbox_det_active);++begins;g_xbox_det_active=1;advances=apu_advances=captures=recorded=0;}
uint32_t xbox_det_epoch(void){return 7;}
void xbox_det_end(void){CHECK(g_xbox_det_active && !g_apu_det && !fixture.used && !input.used);++ends;g_xbox_det_active=0;}
void xbox_det_frame(void){CHECK(g_xbox_det_active && ((!active_tape && !captures) || (advances==ready_after+recorded && active_tape && captures==1)));++advances;}
void mcpx_apu_det_frame(void){CHECK(g_apu_det && advances==apu_advances+1);++apu_advances;}
uint64_t mcpx_apu_xgscnt_now(void){return 0;}
void mcpx_apu_det_begin(uint64_t base){(void)base;CHECK(g_xbox_det_active && !g_apu_det);g_apu_det=1;}
void mcpx_apu_det_end(void){CHECK(g_apu_det);g_apu_det=0;}
void doa3_loop_init(Doa3LoopState *s){memset(s,0,sizeof(*s));s->schema=DOA3_LOOP_SCHEMA;}
Doa3FightCheckpoint *doa3_fight_checkpoint_create(Doa3LoopState *s){
    CHECK(g_xbox_det_active && g_apu_det && !fixture.used && s->schema==DOA3_LOOP_SCHEMA);
    token=s;fixture.used=1;return &fixture;
}
Doa3CheckpointSession *doa3_fight_checkpoint_session(Doa3FightCheckpoint *s){CHECK(s==&fixture && s->used);return (void *)s;}
int doa3_fight_checkpoint_destroy(Doa3FightCheckpoint *s){CHECK(s==&fixture && s->used);s->used=0;return 1;}
Doa3CheckpointImage *doa3_checkpoint_image_capture(Doa3CheckpointSession *s,uint64_t frame){
    CHECK(s==(void *)&fixture && (frame==0 || frame==expected_window) && token->intervals==frame);++captures;
    if(frame && deny_endpoint)return NULL;
    images[frame!=0].frame=frame;return &images[frame!=0];
}
int doa3_fight_checkpoint_rewind_check(Doa3FightCheckpoint *s,const Doa3CheckpointImage *a,uint64_t first,
    const Doa3CheckpointImage *b,uint64_t last,Doa3CheckpointComparison *d){
    CHECK(s==&fixture && a==&images[0] && b==&images[1] && first==0 && last==expected_window);
    CHECK(!active_tape && recorded==expected_window && token->intervals==expected_window && advances==ready_after+expected_window && apu_advances==advances);
    memset(d,0,sizeof(*d));++checks;return !compare_failure && checks!=fail_on_check;
}
Doa3InputTape *doa3_input_tape_create(unsigned n){CHECK(n==expected_window && captures==1);input.used=1;return &input;}
int doa3_input_tape_record(Doa3InputTape *t,uint64_t frame){CHECK(t==&input && t->used && frame==recorded+1 && frame<=expected_window && !active_tape);active_tape=1;return 1;}
int doa3_input_tape_end(Doa3InputTape *t){CHECK(t==&input && active_tape);if(fail_end)return 0;active_tape=0;++recorded;return 1;}
int doa3_input_tape_stats(const Doa3InputTape *t,Doa3InputTapeStats *s){CHECK(t==&input);memset(s,0,sizeof(*s));s->frames=recorded;s->state_reads=4;s->events=8;s->failed=fail_end;return 1;}
int doa3_input_tape_abort(Doa3InputTape *t){CHECK(t==&input && t->used);active_tape=0;return 1;}
int doa3_input_tape_destroy(Doa3InputTape *t){CHECK(t==&input && t->used && !active_tape);t->used=0;return 1;}
static void frames(unsigned n){while(n--)doa3_fight_window_probe_boundary();}
int main(int argc,char **argv){
    CHECK(argc==2);int disabled=!strcmp(argv[1],"disabled"),conflict=!strcmp(argv[1],"conflict");
    fail_end=!strcmp(argv[1],"input-failure");deny_endpoint=!strcmp(argv[1],"endpoint-failure");
    int transition=!strcmp(argv[1],"transition"),bad_present=!strcmp(argv[1],"presentation");
    int timeout=!strcmp(argv[1],"settle-timeout");
    if(!strcmp(argv[1],"late-worker"))ready_after=4;
    if(timeout)ready_after=999;
    _putenv_s("DOA3_ROLLBACK_WINDOW_TEST",disabled ? "0":"1");
    audit_reject=!strcmp(argv[1],"kernel-refusal");audit_invalid=!strcmp(argv[1],"kernel-invalid");
    _putenv_s("DOA3_ROLLBACK_REPLAY_TEST",audit_reject || audit_invalid || !strcmp(argv[1],"replay") ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_CHECKPOINT_TEST",conflict ? "1":"0");
    int long_window=!strncmp(argv[1],"long-",5);
    int combat=long_window || !strncmp(argv[1],"combat-",7);
    texture_ab=strstr(argv[1],"texture-ab")!=NULL;
    packet_ab=strstr(argv[1],"packet-ab")!=NULL;
    geometry_ab=strstr(argv[1],"geometry-ab")!=NULL;
    render_ab=strstr(argv[1],"render-ab")!=NULL;
    prep_ab=strstr(argv[1],"prep-ab")!=NULL;
    guest_ab=strstr(argv[1],"guest-ab")!=NULL;
    int repeat=strstr(argv[1],"repeat")!=NULL || texture_ab || packet_ab || geometry_ab || render_ab || prep_ab || guest_ab;
    unsigned repetitions=texture_ab || packet_ab || geometry_ab || render_ab || prep_ab || guest_ab ? 4:3;
    _putenv_s("DOA3_ROLLBACK_GUEST_RENDER_AB_TEST",guest_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_PREP_AB_TEST",prep_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_RENDER_AB_TEST",render_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_GEOMETRY_AB_TEST",geometry_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_PACKET_AB_TEST",packet_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_TEXTURE_AB_TEST",texture_ab ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_REPEAT_TEST",repeat ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_LONG_TEST",long_window ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_COMBAT_TEST",combat ? "1":"0");
    if(combat)_putenv_s("DOA3_ROLLBACK_REPLAY_TEST","1");
    unsigned char *ram=guest_memory_fixture_alloc(16u*1024u*1024u);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    if(combat){
        ram[0x5A2858]=7;ram[0x596A37]=1;
        frames(600);CHECK(!begins); // idle does not count as combat coverage
        int mismatch=strstr(argv[1],"mismatch")!=NULL,transition=strstr(argv[1],"transition")!=NULL,input_fail=strstr(argv[1],"input-failure")!=NULL;
        unsigned trials=mismatch ? 2:(transition || input_fail) ? 4:6;
        ram[0x59CF58+0x33]=2;
        for(unsigned trial=1;trial<=trials;++trial){
            expected_window=long_window ? (trial>3 ? 8:4):(trial>3 ? 2:1);
            compare_failure=!repeat && mismatch && trial==trials;
            if(repeat && mismatch && trial==trials)fail_on_check=checks+2;
            unsigned waiting=0;
            while(begins<trial && waiting++<500)frames(1);
            CHECK(begins==trial);frames(1);CHECK(active_tape);
            for(unsigned frame=0;frame<expected_window;++frame){
                if(trial==trials && frame==expected_window-1){
                    if(transition)ram[0x5A2858]=2;
                    if(input_fail)fail_end=1;
                }
                ++g_doa3_frames_presented;frames(1);
            }
            CHECK(ends==trial && !active_tape && !audit_active && !g_xbox_det_active && !g_apu_det && !fixture.used);
            CHECK(!forced_uploads && !forced_scalar && !forced_geometry);
        }
        CHECK(checks==(repeat ? (mismatch ? (trials-1)*repetitions+2:trials*repetitions):trials-(transition || input_fail)));
        ram[0x5A2858]=7;fail_end=0;frames(1000);CHECK(begins==trials);
        guest_memory_fixture_free(ram);puts("Combat sequence: attack gate, three singles before doubles, bounded repetition and stop-on-failure passed");return 0;
    }
    frames(130);CHECK(!begins);ram[0x5A2858]=7;ram[0x596A37]=1;
    primary=0;frames(130);primary=1;CHECK(!begins);
    frames(90);ram[0x596AA5]=1;frames(1);ram[0x596AA5]=0;
    frames(119);CHECK(!begins);frames(1);
    if(disabled || conflict){CHECK(!begins && !advances);puts("Window probe disabled/conflict gate passed");return 0;}
    CHECK(begins==1 && !ends && advances==1 && apu_advances==1 && !active_tape && !captures && !checks);
    if(timeout){frames(120);CHECK(ends==1 && advances==120 && !captures && !checks && !g_xbox_det_active);return 0;}
    for(unsigned i=1;i<ready_after;++i){frames(1);CHECK(!captures && !active_tape && !checks);}
    frames(1);
    CHECK(advances==ready_after+1 && apu_advances==advances && active_tape && captures==1);
    if(transition)ram[0x5A2858]=2;
    if(!bad_present)++g_doa3_frames_presented;
    frames(1);
    CHECK(checks==(!fail_end && !deny_endpoint && !transition && !bad_present && !audit_reject && !audit_invalid));
    CHECK(!audit_active);if(audit_reject || audit_invalid)CHECK(captures==1);
    CHECK(ends==1 && !g_apu_det && !g_xbox_det_active && !active_tape && !fixture.used && !input.used);
    frames(300);CHECK(begins==1 && advances==ready_after+1);
    guest_memory_fixture_free(ram);puts("Window probe: two boundaries, one clock tick, input order, eligibility and cleanup passed");return 0;
}
