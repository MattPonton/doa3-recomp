#include "apu_state.h"
#include "apu_chip.h"
#include "apu_rollback.h"
#include "apu.h"
#include "online/xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "online/rollback_frame.h"
#include "game/training_dummy.h"
#include "guest_memory_fixture.h"
#include "game/rollback_state_audit.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
MCPXAPUState *g_state;
volatile LONG g_apu_irq_pending;
static struct { int active; } g_test_tone;
static int g_mixer_active_count;
static APUMixerVoice g_mixer_voices[APU_MIXER_MAX_VOICES];
static CRITICAL_SECTION g_mixer_cs;
static int g_mixer_initialized,g_audio_muted;
int g_dbg_voice_monitor=-1;
uint64_t g_dbg_muted_voices[4];
static struct { int initialized; } g_waveout;
static int16_t s_apu_stage[1024][2];
static int s_apu_fill,s_apu_replay_active,s_apu_replay_fault,s_apu_replay_buffer_size;
int xa2_get_buffer_size(void){return 1024;}
int xa2_is_active(void){return 1;}
int xa2_movie_is_active(void){return 0;}
static HANDLE in_output_wait,finish_output;
static int emulate_wait,completed_frames;
static uint32_t exported_tick;
volatile int g_fib_slice_due;
ptrdiff_t g_xbox_mem_offset;
static int audit_fiber_captures;
/* Mode 3 tests orchestration using real clock/APU ownership; GPU contents are
 * exercised separately with the actual renderer in the WARP fixtures. */
static int render_mode,render_captures,render_destroys,state_sampled,state_token,render_tokens[2];
extern volatile int g_apu_det;
static void audit_pins_held(void)
{
    CHECK(g_xbox_det_active && g_apu_det);
    CHECK(!xbox_det_checkpoint_acquire() && !mcpx_apu_rollback_acquire());
}
Doa3StateAudit *doa3_state_audit_create(void) { CHECK(render_mode);return (Doa3StateAudit *)&state_token; }
int doa3_state_audit_begin_files(Doa3StateAudit *s) { CHECK(s==(Doa3StateAudit *)&state_token);return 1; }
int doa3_state_audit_sample(Doa3StateAudit *s)
{ CHECK(s==(Doa3StateAudit *)&state_token && render_captures==1);audit_pins_held();state_sampled=1;return 1; }
unsigned doa3_state_audit_stable_mask(const Doa3StateAudit *s)
{ CHECK(s==(Doa3StateAudit *)&state_token && state_sampled);return 127; }
void doa3_state_audit_destroy(Doa3StateAudit *s) { CHECK(render_mode ? s==(Doa3StateAudit *)&state_token:!s); }
#include "nv2a/nv2a_renderer_checkpoint.h"
int pgraph_d3d11_rollback_snapshot_source(Nv2aRendererCheckpointSource *s)
{ CHECK(render_mode && s);audit_pins_held();return 1; }
static void *audit_render_capture(void *v)
{ CHECK(v && render_captures<2);audit_pins_held();CHECK(!render_captures || state_sampled);return &render_tokens[render_captures++]; }
static void audit_render_destroy(void *v,void *p)
{ CHECK(v && (p==&render_tokens[0] || p==&render_tokens[1]));CHECK(!g_xbox_det_active && !g_apu_det);++render_destroys; }
Doa3CheckpointResource nv2a_renderer_checkpoint_resource(Nv2aRendererCheckpointSource *s)
{ CHECK(render_mode);return (Doa3CheckpointResource){s,audit_render_capture,NULL,NULL,audit_render_destroy}; }
int nv2a_renderer_checkpoint_equal(Nv2aRendererCheckpointSource *s,const void *a,const void *b)
{ CHECK(s && a==&render_tokens[0] && b==&render_tokens[1] && render_captures==2 && state_sampled);audit_pins_held();return render_mode==1; }
int doa3_rollback_tasks_enabled(void) { return 1; }
int xbox_fiber_is_primary(void) { return 1; }
void training_status(TrainingStatus *out) { memset(out,0,sizeof(*out)); }
/* Parked-fiber fixture only: the test below uses real clock/APU ownership,
 * while the real scheduler's snapshot rules have their separate tests. */
size_t xbox_fiber_converted_snapshot_size(void) { return 32; }
int xbox_fiber_converted_capture(void *out,size_t bytes)
{
    CHECK(bytes==32 && g_xbox_det_active);
    CHECK(!xbox_det_checkpoint_acquire() && !mcpx_apu_rollback_acquire());
    memset(out,0xAB,bytes);++audit_fiber_captures;return 1;
}
int xbox_fiber_converted_validate(const void *p,size_t bytes) { return p && bytes==32; }
ptrdiff_t xbox_GetMemoryOffset(void)
{
    return (ptrdiff_t)((uintptr_t)&exported_tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));
}
int xbox_fiber_current(void) { return 0; }
void xbox_fiber_wake(uint32_t event) { (void)event; }
static void se_frame(MCPXAPUState *d)
{
    if (emulate_wait) {
        /* Same critical ownership window as monitor_frame: the lock is
         * temporarily released, but the chip's frame is not complete. */
        qemu_mutex_unlock(&d->lock);
        CHECK(SetEvent(in_output_wait));
        CHECK(WaitForSingleObject(finish_output,5000)==WAIT_OBJECT_0);
        qemu_mutex_lock(&d->lock);
    }
    ++completed_frames;
    ++d->ep_frame_div;
}
static void mcpx_apu_monitor_frame(MCPXAPUState *d) { se_frame(d); }
static void update_irq(MCPXAPUState *d) { (void)d; }
#include "apu_ownership.generated.inc"

static DWORD WINAPI worker_frame(void *unused)
{
    (void)unused;
    qemu_mutex_lock(&g_state->lock);
    apu_frame_body(g_state);
    qemu_mutex_unlock(&g_state->lock);
    return 0;
}
static DWORD WINAPI handoff_observer(void *unused)
{
    ULONGLONG deadline=GetTickCount64()+5000;
    (void)unused;
    for (;;) {
        int pending;
        qemu_mutex_lock(&g_state->lock);
        pending=g_state->rollback_det_pending;
        if (pending) {
            CHECK(g_state->rollback_frame_active && !g_apu_det && !completed_frames);
            CHECK(!mcpx_apu_rollback_acquire());
        }
        qemu_mutex_unlock(&g_state->lock);
        if (pending) break;
        CHECK(GetTickCount64()<deadline);Sleep(1);
    }
    CHECK(SetEvent(finish_output));
    return 0;
}
static DWORD WINAPI wrong_owner(void *unused)
{
    (void)unused;
    CHECK(!mcpx_apu_rollback_acquire());
    CHECK(!mcpx_apu_rollback_release());
    mcpx_apu_det_frame();mcpx_apu_det_step();mcpx_apu_det_end();
    /* Prove the pin also excludes a writer that only knows the APU lock. */
    CHECK(!TryEnterCriticalSection(&g_state->lock.cs));
    return 0;
}
static DWORD WINAPI hold_lock(void *unused)
{
    (void)unused;
    qemu_mutex_lock(&g_state->lock);
    CHECK(SetEvent(in_output_wait));
    CHECK(WaitForSingleObject(finish_output,5000)==WAIT_OBJECT_0);
    qemu_mutex_unlock(&g_state->lock);
    return 0;
}
static void join(HANDLE thread)
{
    DWORD result=1;
    CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
    CHECK(GetExitCodeThread(thread,&result) && !result);CloseHandle(thread);
}
int main(int argc,char **argv)
{
    CHECK(argc==1 || (argc==2 && (!strcmp(argv[1],"renderer") || !strcmp(argv[1],"renderer-mismatch") || !strcmp(argv[1],"processing"))));
    int processing=argc==2 && !strcmp(argv[1],"processing");
    render_mode=argc==1 || processing ? 0:!strcmp(argv[1],"renderer") ? 1:2;
    HANDLE worker,observer;
    void *a=malloc(apu_rollback_size()),*b=malloc(apu_rollback_size());
    CHECK(a && b);
    g_state=calloc(1,sizeof(*g_state));CHECK(g_state);
    qemu_mutex_init(&g_state->lock);qemu_cond_init(&g_state->cond);
    in_output_wait=CreateEvent(NULL,TRUE,FALSE,NULL);finish_output=CreateEvent(NULL,TRUE,FALSE,NULL);
    CHECK(in_output_wait && finish_output);
    CHECK(!mcpx_apu_rollback_acquire() && !mcpx_apu_rollback_release());
    emulate_wait=1;
    worker=CreateThread(NULL,0,worker_frame,NULL,0,NULL);CHECK(worker);
    CHECK(WaitForSingleObject(in_output_wait,5000)==WAIT_OBJECT_0);
    observer=CreateThread(NULL,0,handoff_observer,NULL,0,NULL);CHECK(observer);
    mcpx_apu_det_begin(1234); /* Must wait for the worker's whole frame. */
    join(worker);join(observer);emulate_wait=0;
    CHECK(g_apu_det && completed_frames==1 && !g_state->rollback_frame_active && !g_state->rollback_det_pending);
    xbox_det_bases bases={100,200,300};xbox_det_checkpoint clock_before,clock_after;
    xbox_det_begin(&bases);
    CHECK(xbox_det_checkpoint_acquire()); /* fixed acquisition order: clock then APU */
    CHECK(mcpx_apu_rollback_acquire());
    CHECK(xbox_det_capture(&clock_before));
    CHECK(!mcpx_apu_rollback_acquire()); /* nonrecursive despite recursive CS */
    CHECK(mcpx_apu_rollback_capture(a,apu_rollback_size()));
    worker=CreateThread(NULL,0,wrong_owner,NULL,0,NULL);join(worker);
    mcpx_apu_det_frame();mcpx_apu_det_step();mcpx_apu_det_end();
    CHECK(g_apu_det && completed_frames==1 && s_apu_det_chip==0);
    CHECK(mcpx_apu_rollback_capture(b,apu_rollback_size()) && !memcmp(a,b,apu_rollback_size()));
    CHECK(xbox_det_capture(&clock_after) && !memcmp(&clock_before,&clock_after,sizeof clock_before));
    CHECK(mcpx_apu_rollback_release() && !mcpx_apu_rollback_release());
    CHECK(xbox_det_checkpoint_release());
    /* A busy backend is rejected immediately, without waiting on its lock. */
    ResetEvent(in_output_wait);ResetEvent(finish_output);
    worker=CreateThread(NULL,0,hold_lock,NULL,0,NULL);CHECK(worker);
    CHECK(WaitForSingleObject(in_output_wait,5000)==WAIT_OBJECT_0);
    CHECK(xbox_det_checkpoint_acquire());
    CHECK(!mcpx_apu_rollback_acquire());
    CHECK(xbox_det_checkpoint_release()); /* failed second acquisition unwinds first */
    SetEvent(finish_output);join(worker);
    g_state->rollback_frame_active=true;
    CHECK(!mcpx_apu_rollback_acquire() && !mcpx_apu_rollback_capture(b,apu_rollback_size()));
    g_state->rollback_frame_active=false;
    mcpx_apu_det_step();CHECK(s_apu_det_chip==1 && completed_frames==2);
    mcpx_apu_det_end();CHECK(!g_apu_det && !mcpx_apu_rollback_acquire());
    xbox_det_end();
    /* Exercise the live audit's gating and balanced ownership cleanup with
     * actual clock/APU functions and the parked-fiber fixture above. */
    unsigned char *guest=guest_memory_fixture_alloc(0x600000);CHECK(guest);g_xbox_mem_offset=(ptrdiff_t)guest;
    g_state->ram_ptr=guest;
    CHECK(!_putenv_s("DOA3_ROLLBACK_OWNERSHIP_AUDIT",processing ? "4":render_mode ? "3":"1"));
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE","") && !_putenv_s("DOA3_ROLLBACK_FRAME_CAPTURE",""));
    for(unsigned i=0;i<125;i++)doa3_rollback_ownership_boundary();
    CHECK(!audit_fiber_captures);
    guest[0x5A2858]=7;guest[0x596A37]=1;
    for(unsigned i=0;i<119;i++)doa3_rollback_ownership_boundary();
    CHECK(!audit_fiber_captures);
    doa3_rollback_ownership_boundary();
    CHECK(audit_fiber_captures==2 && !g_apu_det && !g_xbox_det_active);
    CHECK(!mcpx_apu_rollback_release() && !xbox_det_checkpoint_release());
    doa3_rollback_ownership_boundary();CHECK(audit_fiber_captures==2);
    CHECK(render_captures==(render_mode ? 2:0) && render_destroys==render_captures);
    guest_memory_fixture_free(guest);g_xbox_mem_offset=0;
    qemu_mutex_destroy(&g_state->lock);free(g_state);g_state=NULL;
    CHECK(!mcpx_apu_rollback_acquire() && !mcpx_apu_rollback_release());
    CloseHandle(in_output_wait);CloseHandle(finish_output);free(a);free(b);
    puts("APU ownership: in-flight handoff, nonblocking pin, writer exclusion, stable capture and resume passed");
    return 0;
}
