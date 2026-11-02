/* The live ownership audit (rollback_ownership.c) with the real clock: its
 * gating, the pins held while every domain is sampled, and balanced cleanup.
 * Audio is the DirectSound lockstep clock and pin, stubbed here. */
#include "online/xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "online/rollback_frame.h"
#include "game/training_dummy.h"
#include "guest_memory_fixture.h"
#include "game/rollback_state_audit.h"
#include "audio/dsound_hle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
volatile int g_fib_slice_due;
ptrdiff_t g_xbox_mem_offset;
static uint32_t exported_tick;
static int audit_fiber_captures,audio_det,audio_pin,audio_captures;
/* Mode 3 tests orchestration using real clock ownership; GPU contents are
 * exercised separately with the actual renderer in the WARP fixtures. */
static int render_mode,render_captures,render_destroys,state_sampled,state_token,render_tokens[2];
int ds_det_begin(void) { if (audio_det) return 0; audio_det=1; return 1; }
void ds_det_end(void) { CHECK(audio_det && !audio_pin); audio_det=0; }
int ds_det_active(void) { return audio_det; }
size_t ds_rollback_size(void) { return 16; }
int ds_rollback_acquire(void) { if (!audio_det || audio_pin) return 0; audio_pin=1; return 1; }
int ds_rollback_release(void) { if (!audio_pin) return 0; audio_pin=0; return 1; }
int ds_rollback_capture(void *out,size_t bytes)
{ CHECK(audio_pin && bytes==16); memset(out,0x5A,bytes); ++audio_captures; return 1; }
int ds_rollback_validate(const void *in,size_t bytes) { return audio_pin && in && bytes==16; }
static void audit_pins_held(void)
{
    CHECK(g_xbox_det_active && audio_det && audio_pin);
    CHECK(!xbox_det_checkpoint_acquire() && !ds_rollback_acquire());
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
{ CHECK(v && (p==&render_tokens[0] || p==&render_tokens[1]));CHECK(!g_xbox_det_active && !audio_det);++render_destroys; }
Doa3CheckpointResource nv2a_renderer_checkpoint_resource(Nv2aRendererCheckpointSource *s)
{ CHECK(render_mode);return (Doa3CheckpointResource){s,audit_render_capture,NULL,NULL,audit_render_destroy}; }
int nv2a_renderer_checkpoint_equal(Nv2aRendererCheckpointSource *s,const void *a,const void *b)
{ CHECK(s && a==&render_tokens[0] && b==&render_tokens[1] && render_captures==2 && state_sampled);audit_pins_held();return render_mode==1; }
int doa3_rollback_tasks_enabled(void) { return 1; }
int xbox_fiber_is_primary(void) { return 1; }
void training_status(TrainingStatus *out) { memset(out,0,sizeof(*out)); }
/* Parked-fiber fixture only; the real scheduler's snapshot rules have their
 * separate tests. */
size_t xbox_fiber_converted_snapshot_size(void) { return 32; }
int xbox_fiber_converted_capture(void *out,size_t bytes)
{
    CHECK(bytes==32 && g_xbox_det_active);audit_pins_held();
    memset(out,0xAB,bytes);++audit_fiber_captures;return 1;
}
int xbox_fiber_converted_validate(const void *p,size_t bytes) { return p && bytes==32; }
ptrdiff_t xbox_GetMemoryOffset(void)
{
    return (ptrdiff_t)((uintptr_t)&exported_tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));
}
int xbox_fiber_current(void) { return 0; }
void xbox_fiber_wake(uint32_t event) { (void)event; }
int main(int argc,char **argv)
{
    CHECK(argc==1 || (argc==2 && (!strcmp(argv[1],"renderer") || !strcmp(argv[1],"renderer-mismatch"))));
    render_mode=argc==1 ? 0:!strcmp(argv[1],"renderer") ? 1:2;
    unsigned char *guest=guest_memory_fixture_alloc(0x600000);CHECK(guest);g_xbox_mem_offset=(ptrdiff_t)guest;
    CHECK(!_putenv_s("DOA3_ROLLBACK_OWNERSHIP_AUDIT",render_mode ? "3":"1"));
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE","") && !_putenv_s("DOA3_ROLLBACK_FRAME_CAPTURE",""));
    /* Not in a solo Practice fight: nothing is sampled. */
    for(unsigned i=0;i<125;i++)doa3_rollback_ownership_boundary();
    CHECK(!audit_fiber_captures);
    guest[0x5A2858]=7;guest[0x596A37]=1;
    for(unsigned i=0;i<119;i++)doa3_rollback_ownership_boundary();
    CHECK(!audit_fiber_captures);
    doa3_rollback_ownership_boundary();
    CHECK(audit_fiber_captures==2 && audio_captures==2 && !audio_det && !audio_pin && !g_xbox_det_active);
    CHECK(!xbox_det_checkpoint_release());
    doa3_rollback_ownership_boundary();CHECK(audit_fiber_captures==2);   /* one attempt only */
    CHECK(render_captures==(render_mode ? 2:0) && render_destroys==render_captures);
    guest_memory_fixture_free(guest);g_xbox_mem_offset=0;
    puts("Ownership audit: gating, pins held across every sample, stable captures and balanced cleanup passed");
    return 0;
}
