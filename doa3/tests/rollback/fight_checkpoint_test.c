/* Production binding + coordinator + outer-loop state. Domain endpoints are
 * controlled fixtures here; real endpoints have their existing integration
 * suites. Verifies routing, complete primary CPU state and failed-step recovery. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_fight_checkpoint.h"
#include "game/rollback_input.h"
#include "online/xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_heap.h"
#include "nv2a/nv2a_renderer_checkpoint.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define BYTES (128u*1024u*1024u)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp,g_str_a,g_str_b,g_flg_a,g_flg_b;
double g_fp_stack[8];int g_fp_top,g_flg_w,g_flg_test;uint16_t g_x87_cw;
volatile int g_xbox_det_active=1;
static unsigned char *ram;
static DWORD owner;
static unsigned current_epoch=1,values[DOA3_CP_PARTS],captures,restores,resource_value=41,resources;
static int primary=1,clock_pin,audio_pin,files,gate,guard,deny_audio,deny_gate,deny_restore,deny_capture=-1;
static unsigned clock_acquisitions;
static int contend_after_capture;
static unsigned transient_audio_denials,audio_attempts;
static int change_on_audio_denial;
static unsigned reject_value,corrupt_value;
static uint32_t irql;
struct XboxHeapCheckpoint{int dummy;};static struct XboxHeapCheckpoint heap;
void *xbox_GetMemoryBase(void){return ram;}
size_t xbox_GetMemorySize(void){return BYTES;}
int xbox_fiber_is_primary(void){return primary;}
unsigned char xbox_current_irql(void){return (unsigned char)irql;}
uint32_t xbox_det_epoch(void){return current_epoch;}
int xbox_kernel_replay_owned(void){return guard && primary && GetCurrentThreadId()==owner;}
int xbox_kernel_replay_begin(void){
    if(guard || !xbox_det_checkpoint_acquire())return 0;
    guard=1;CHECK(xbox_det_checkpoint_release());return 1;
}
int xbox_kernel_replay_end(int cancelled){(void)cancelled;CHECK(guard && !gate);guard=0;return 1;}
int xbox_det_checkpoint_acquire(void){if(clock_pin || !primary)return 0;clock_pin=1;++clock_acquisitions;return 1;}
int xbox_det_checkpoint_release(void){CHECK(clock_pin);clock_pin=0;return 1;}
int mcpx_apu_rollback_acquire(void){
    ++audio_attempts;
    if(transient_audio_denials){
        --transient_audio_denials;
        if(change_on_audio_denial){++values[DOA3_CP_INPUT];change_on_audio_denial=0;}
        return 0;
    }
    if(!clock_pin || audio_pin || deny_audio)return 0;audio_pin=1;return 1;
}
int mcpx_apu_rollback_release(void){CHECK(audio_pin);audio_pin=0;return 1;}
int mcpx_apu_processing_can_restore(void){return audio_pin && gate;}
int mcpx_apu_replay_begin(void){CHECK(clock_pin && audio_pin && !gate);if(deny_gate)return 0;gate=1;return 1;}
int mcpx_apu_replay_end(int cancel){(void)cancel;CHECK(clock_pin && audio_pin && gate);gate=0;return 1;}
int xbox_stream_file_checkpoint_begin(void){if(files)return 0;files=1;return 1;}
void xbox_stream_file_checkpoint_end(void){CHECK(files);files=0;}
size_t xbox_stream_file_checkpoint_size(void){return 8;}
XboxHeapCheckpoint *xbox_heap_checkpoint_create(void){return &heap;}
void xbox_heap_checkpoint_destroy(XboxHeapCheckpoint *h){CHECK(h==&heap || !h);}
size_t xbox_heap_checkpoint_size(void){return 8;}
size_t xbox_fiber_converted_snapshot_size(void){return 8;}
size_t xbox_stream_completion_snapshot_size(void){return 8;}
size_t mcpx_apu_processing_size(void){return 8;}
static int capture(unsigned i,void *out,size_t n){
    CHECK(clock_pin && audio_pin && n>=4);++captures;
    /* Model another host thread winning every subsequent try-lock once the
     * first image is captured. A restore-only transaction must not drop pins. */
    if(contend_after_capture)deny_audio=1;
    if(deny_capture==(int)i)return 0;memset(out,0,n);memcpy(out,&values[i],4);return 1;
}
static int validate(const void *in,size_t n){
    CHECK(clock_pin && audio_pin);if(!in || n<4)return 0;
    unsigned value;memcpy(&value,in,4);return !reject_value || value!=reject_value;
}
static int restore(unsigned i,const void *in,size_t n){
    CHECK(guard && gate && validate(in,n));++restores;memcpy(&values[i],in,4);
    if(corrupt_value && values[i]==corrupt_value)++values[i];
    return deny_restore ? -1:1;
}
#define ENDPOINT(name,id) \
int name##_capture(void *p,size_t n){return capture(id,p,n);} \
int name##_validate(const void *p,size_t n){return validate(p,n);} \
int name##_restore(const void *p,size_t n){return restore(id,p,n);}
ENDPOINT(xbox_stream_file_checkpoint,DOA3_CP_FILES)
ENDPOINT(xbox_fiber_converted,DOA3_CP_FIBERS)
ENDPOINT(xbox_stream_completion,DOA3_CP_COMPLETIONS)
ENDPOINT(xbox_kernel_interrupt,DOA3_CP_INTERRUPTS)
ENDPOINT(doa3_gpu_frontend,DOA3_CP_GPU_FRONTEND)
ENDPOINT(mcpx_apu_processing,DOA3_CP_AUDIO)
ENDPOINT(doa3_xpp,DOA3_CP_INPUT)
int xbox_heap_checkpoint_capture(XboxHeapCheckpoint *h,void *p,size_t n){CHECK(h==&heap);return capture(DOA3_CP_HEAP,p,n);}
int xbox_heap_checkpoint_validate(XboxHeapCheckpoint *h,const void *p,size_t n){CHECK(h==&heap);return validate(p,n);}
int xbox_heap_checkpoint_restore(XboxHeapCheckpoint *h,const void *p,size_t n){CHECK(h==&heap);return restore(DOA3_CP_HEAP,p,n);}
int xbox_det_capture(xbox_det_checkpoint *p){return capture(DOA3_CP_CLOCK,p,sizeof(*p));}
int xbox_det_validate(const xbox_det_checkpoint *p){return validate(p,sizeof(*p));}
int xbox_det_restore(const xbox_det_checkpoint *p){return restore(DOA3_CP_CLOCK,p,sizeof(*p));}
int pgraph_d3d11_rollback_snapshot_source(Nv2aRendererCheckpointSource *s){CHECK(clock_pin && audio_pin);memset(s,0,sizeof(*s));return 1;}
static void *resource_capture(void *v){(void)v;CHECK(clock_pin && audio_pin);unsigned *p=malloc(4);CHECK(p);*p=resource_value;++resources;return p;}
static int resource_validate(void *v,const void *p){(void)v;return p!=NULL;}
static int resource_restore(void *v,const void *p){(void)v;CHECK(clock_pin && audio_pin && guard && gate);resource_value=*(const unsigned *)p;return 1;}
static void resource_destroy(void *v,void *p){(void)v;CHECK(clock_pin && audio_pin && resources);--resources;free(p);}
static int resource_equal(void *v,const void *a,const void *b){(void)v;return *(const unsigned *)a==*(const unsigned *)b;}
static int resource_state_equal(void *v,const void *a,const void *b){
    (void)v;return (*(const unsigned *)a/10)==(*(const unsigned *)b/10);
}
Doa3CheckpointResource nv2a_renderer_checkpoint_resource(Nv2aRendererCheckpointSource *s){
    return (Doa3CheckpointResource){s,resource_capture,resource_validate,resource_restore,resource_destroy,resource_equal,resource_state_equal};
}
static DWORD WINAPI foreign(void *v){CHECK(!doa3_fight_checkpoint_session(v));CHECK(!doa3_fight_checkpoint_destroy(v));return 0;}
static int replay_fixture(void *v){
    Doa3LoopState *loop=v;
    CHECK(guard && gate && !clock_pin && !audio_pin && loop->intervals==0);
    CHECK(g_eax==42 && MEM32(0x1234)==0xA7A7A7A7);
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]+=100;
    MEM32(0x1234)=0xBEEFFACE;g_eax=93;resource_value=116;loop->intervals=1;
    return 1;
}
static int replay_mismatch(void *v){replay_fixture(v);MEM32(0x1234)=7;return 1;}
static int replay_pixels_only(void *v){replay_fixture(v);resource_value=117;return 1;}
static int replay_renderer_state(void *v){replay_fixture(v);resource_value=127;return 1;}
static int replay_failure(void *v){replay_fixture(v);g_eax=7;return 0;}
static int replay_two(void *v){
    replay_fixture(v);
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]+=100;
    MEM32(0x1234)=0xACED1234;g_eax=94;resource_value=126;((Doa3LoopState *)v)->intervals=2;return 1;
}
int main(int argc,char **argv){
    (void)argv;
    owner=GetCurrentThreadId();ram=guest_memory_fixture_alloc(BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    memset(ram,0xA7,BYTES);ram[0x24]=0;Doa3LoopState loop;doa3_loop_init(&loop);
    g_esp=0x100000;g_eax=42;g_flg_w=4;g_fp_top=-1;g_x87_cw=0x027F;g_fp_stack[0]=1.25;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]=i+10;
    if(argc>1){
        deny_restore=1;Doa3CheckpointComparison diff;
        CHECK(doa3_fight_checkpoint_roundtrip(&loop,&diff)==-1);
        CHECK(guard && gate && files && resources && clock_pin && audio_pin);
        CHECK(doa3_fight_checkpoint_roundtrip(&loop,&diff)==-1);
        puts("Failed live checkpoint restore retains state/output guards and refuses continuation");return 0;
    }
    deny_audio=1;CHECK(!doa3_fight_checkpoint_create(&loop) && !files && !clock_pin && !audio_pin);deny_audio=0;
    Doa3FightCheckpoint *f=doa3_fight_checkpoint_create(&loop);CHECK(f && files);
    Doa3CheckpointSession *s=doa3_fight_checkpoint_session(f);CHECK(s);
    Doa3CheckpointImage *start=doa3_checkpoint_image_capture(s,0);CHECK(start && resources==1);
    Doa3CheckpointComparison diff;CHECK(doa3_checkpoint_image_matches(s,start,&diff)==1);
    unsigned before_restores=restores;
    CHECK(!doa3_checkpoint_image_restore(s,start,NULL) && restores==before_restores);
    guard=1;CHECK(!doa3_checkpoint_image_restore(s,start,NULL) && restores==before_restores);gate=1;
    /* Failed frame stopped at a pending helper, not at the next full boundary.
     * Compensation/anchor restore must still work from this parked state. */
    Doa3LoopAction action;CHECK(doa3_loop_next(&loop,&action));
    MEM32(0x1234)=0xDEADBEEF;g_eax=99;g_ecx=77;g_str_a=13;g_flg_a=20;g_flg_w=1;
    g_fp_top=4;g_fp_stack[0]=8.5;g_x87_cw=0x037F;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]+=100;
    resource_value=72;
    CHECK(doa3_checkpoint_image_matches(s,start,&diff)==0 && diff.different_parts==1023 && diff.renderer_different);
    uint64_t frame=99;CHECK(doa3_checkpoint_image_restore(s,start,&frame)==1 && frame==0);
    CHECK(doa3_checkpoint_image_matches(s,start,&diff)==1 && doa3_loop_at_boundary(&loop));
    CHECK(g_eax==42 && !g_ecx && !g_str_a && !g_flg_a && g_flg_w==4 && g_fp_top==-1 && g_fp_stack[0]==1.25 && g_x87_cw==0x027F);
    CHECK(MEM32(0x1234)==0xA7A7A7A7 && resource_value==41);
    HANDLE h=CreateThread(NULL,0,foreign,f,0,NULL);CHECK(h && WaitForSingleObject(h,5000)==WAIT_OBJECT_0);CloseHandle(h);
    primary=0;CHECK(!doa3_fight_checkpoint_session(f) && doa3_checkpoint_image_matches(s,start,&diff)==-1);primary=1;
    irql=2;CHECK(!doa3_fight_checkpoint_session(f));irql=0;
    ++current_epoch;CHECK(!doa3_fight_checkpoint_session(f));--current_epoch;
    deny_audio=1;CHECK(!doa3_fight_checkpoint_destroy(f) && resources==1 && files && !clock_pin);deny_audio=0;
    CHECK(doa3_fight_checkpoint_destroy(f) && !resources && !files && !clock_pin && !audio_pin);
    guard=gate=0;
    unsigned pins_before=clock_acquisitions;contend_after_capture=1;
    CHECK(doa3_fight_checkpoint_roundtrip(&loop,&diff)==1 && !guard && !gate && !files && !resources);
    CHECK(clock_acquisitions==pins_before+3 && !clock_pin && !audio_pin);
    contend_after_capture=deny_audio=0;
    deny_capture=DOA3_CP_AUDIO;CHECK(!doa3_fight_checkpoint_roundtrip(&loop,&diff));deny_capture=-1;
    deny_gate=1;CHECK(!doa3_fight_checkpoint_roundtrip(&loop,&diff));deny_gate=0;
    CHECK(!guard && !gate && !files && !resources && !clock_pin && !audio_pin);
    f=doa3_fight_checkpoint_create(&loop);CHECK(f);s=doa3_fight_checkpoint_session(f);
    start=doa3_checkpoint_image_capture(s,0);CHECK(start);
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]+=100;
    MEM32(0x1234)=0xBEEFFACE;g_eax=93;resource_value=116;loop.intervals=1;
    Doa3CheckpointImage *end=doa3_checkpoint_image_capture(s,1);CHECK(end);
    CHECK(doa3_fight_checkpoint_rewind_check(f,start,0,end,1,&diff)==1);
    CHECK(MEM32(0x1234)==0xBEEFFACE && g_eax==93 && resource_value==116 && loop.intervals==1);
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate && !clock_pin && !audio_pin);
    /* Persistent refusal must stop before capture/restore and release guard
     * and clock; transient contention must not require a whole new trial. */
    unsigned attempts_before=audio_attempts,restores_before=restores,captures_before=captures;
    deny_audio=1;
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_fixture,&loop,&diff)==0);
    CHECK(audio_attempts==attempts_before+8 && restores==restores_before && captures==captures_before);
    CHECK(!guard && !gate && !clock_pin && !audio_pin);deny_audio=0;
    transient_audio_denials=1;change_on_audio_denial=1;
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_fixture,&loop,&diff)==0);
    CHECK(restores==restores_before && diff.different_parts==(1u<<DOA3_CP_INPUT));
    CHECK(!guard && !gate && !clock_pin && !audio_pin);--values[DOA3_CP_INPUT];
    transient_audio_denials=3;
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_fixture,&loop,&diff)==1);
    CHECK(!transient_audio_denials);
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_mismatch,&loop,&diff)==0);
    CHECK(diff.different_parts==(1u<<DOA3_CP_RAM));
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_failure,&loop,&diff)==0);
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end,1,replay_pixels_only,&loop,&diff)==0);
    CHECK(doa3_fight_checkpoint_render_probe(f,start,0,end,1,replay_pixels_only,&loop,&diff)==2);
    CHECK(!diff.different_parts && diff.renderer_different && diff.renderer_state_equal);
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_render_probe(f,start,0,end,1,replay_renderer_state,&loop,&diff)==0);
    CHECK(diff.renderer_different && !diff.renderer_state_equal);
    CHECK(doa3_fight_checkpoint_render_probe(f,start,0,end,1,replay_mismatch,&loop,&diff)==0);
    CHECK(diff.different_parts==(1u<<DOA3_CP_RAM));
    CHECK(doa3_fight_checkpoint_render_probe(f,start,0,end,1,replay_failure,&loop,&diff)==0);
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1 && !guard && !gate);
    /* A stale earlier domain is rejected before mutation; retain current end. */
    reject_value=10;
    CHECK(doa3_fight_checkpoint_rewind_check(f,start,0,end,1,&diff)==0);reject_value=0;
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1);
    /* A restore reporting success but producing different earlier state must
     * recover the independently captured endpoint, never resume at start. */
    corrupt_value=10;
    CHECK(doa3_fight_checkpoint_rewind_check(f,start,0,end,1,&diff)==0);corrupt_value=0;
    CHECK(doa3_checkpoint_image_matches(s,end,&diff)==1);
    /* Two-frame success and a replay failing after frame one must both
     * recover the independently captured frame-two endpoint. */
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)values[i]+=100;
    MEM32(0x1234)=0xACED1234;g_eax=94;resource_value=126;loop.intervals=2;
    Doa3CheckpointImage *end2=doa3_checkpoint_image_capture(s,2);CHECK(end2);
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end2,2,replay_two,&loop,&diff)==1);
    CHECK(doa3_checkpoint_image_matches(s,end2,&diff)==1 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_replay_check(f,start,0,end2,2,replay_failure,&loop,&diff)==0);
    CHECK(doa3_checkpoint_image_matches(s,end2,&diff)==1 && loop.intervals==2 && !guard && !gate);
    CHECK(doa3_fight_checkpoint_destroy(f) && !guard && !gate && !files && !resources && !clock_pin && !audio_pin);
    guest_memory_fixture_free(ram);puts("Production fight checkpoint bindings: all domains, primary CPU/loop and failed-step recovery passed");return 0;
}
