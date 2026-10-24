/* One opt-in live check of the clock/APU pin and parked continuations.
 * Does not restore anything or claim ownership of other snapshot domains. */
#include "online/rollback_frame.h"
#include "online/rollback_tasks.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include "apu/apu_rollback.h"
#include "apu/apu_live_checkpoint.h"
#include "recomp/recomp_types.h"
#include "training_dummy.h"
#include "rollback_state_audit.h"
#include "nv2a/nv2a_renderer_checkpoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

extern volatile int g_apu_det;
void mcpx_apu_det_begin(uint64_t);
void mcpx_apu_det_end(void);
uint64_t mcpx_apu_xgscnt_now(void);

void doa3_rollback_ownership_boundary(void)
{
    static int enabled=-1;
    static unsigned warmup;
    TrainingStatus training;
    if (enabled<0) {
        const char *v=getenv("DOA3_ROLLBACK_OWNERSHIP_AUDIT");
        const char *capture=getenv("DOA3_ROLLBACK_CAPTURE");
        const char *frame=getenv("DOA3_ROLLBACK_FRAME_CAPTURE");
        enabled=v && doa3_rollback_tasks_enabled() ?
            (!strcmp(v,"4") ? 4:!strcmp(v,"3") ? 3:!strcmp(v,"2") ? 2:!strcmp(v,"1")):0;
        if (enabled && ((capture && *capture) || (frame && *frame))) {
            enabled=0;
            fprintf(stderr,"[ROLLBACK-OWNERSHIP] refused: memory capture also armed\n");
        }
    }
    if (!enabled) return;
    if (MEM8(0x5A2858)!=7 || MEM8(0x596A37)!=1 || MEM8(0x596AA5) ||
        g_xbox_det_active || g_apu_det || !xbox_fiber_is_primary()) { warmup=0;return; }
    training_status(&training);
    if (training.mode!=TRAINING_IDLE) { warmup=0;return; }
    if (++warmup<120) return;
    int extended=enabled==2 || enabled==3,render=enabled==3,processing=enabled==4;
    enabled=0; /* exactly one attempt, including allocation/acquisition failures */
    size_t audio_bytes=processing ? mcpx_apu_processing_size():apu_rollback_size();
    size_t fiber_bytes=xbox_fiber_converted_snapshot_size();
    void *a=malloc(audio_bytes),*b=malloc(audio_bytes);
    void *f=malloc(fiber_bytes),*g=malloc(fiber_bytes);
    Doa3StateAudit *state=extended ? doa3_state_audit_create():NULL;
    int state_ok=!extended,files_ready=0;
    unsigned state_mask=0;
    Nv2aRendererCheckpointSource renderer_source={0};
    Doa3CheckpointResource renderer={0};
    void *renderer_first=NULL,*renderer_second=NULL;
    int renderer_ready=0,renderer_ok=!render;
    xbox_det_bases bases;
    xbox_det_checkpoint first,last;
    int clock_pin=0,audio_pin=0,clock_started=0,audio_started=0;
    int audio_ok=0,clock_ok=0,fiber_count=0,fiber_ok=0;
    LARGE_INTEGER start,end,hz;
    QueryPerformanceCounter(&start);QueryPerformanceFrequency(&hz);
    if (!a || !b || !f || !g || (extended && !state)) goto done;
    xbox_det_current_bases(&bases);xbox_det_begin(&bases);clock_started=1;
    if (state) files_ready=doa3_state_audit_begin_files(state);
    mcpx_apu_det_begin(mcpx_apu_xgscnt_now());audio_started=g_apu_det!=0;
    if (!audio_started) goto done;
    clock_pin=xbox_det_checkpoint_acquire();if (!clock_pin) goto done;
    audio_pin=mcpx_apu_rollback_acquire();if (!audio_pin) goto done;
    clock_ok=xbox_det_capture(&first);
    audio_ok=processing ? mcpx_apu_processing_capture(a,audio_bytes):mcpx_apu_rollback_capture(a,audio_bytes);
    fiber_count=xbox_fiber_converted_capture(f,fiber_bytes);
    if (render) {
        renderer_ready=pgraph_d3d11_rollback_snapshot_source(&renderer_source);
        if (renderer_ready) {
            renderer=nv2a_renderer_checkpoint_resource(&renderer_source);
            renderer_first=renderer.capture(renderer.context);
        }
    }
    if (state) {
        state_ok=doa3_state_audit_sample(state) && files_ready;
        state_mask=doa3_state_audit_stable_mask(state);
    }
    if (render && renderer_first) {
        renderer_second=renderer.capture(renderer.context);
        renderer_ok=nv2a_renderer_checkpoint_equal(&renderer_source,renderer_first,renderer_second);
    }
    audio_ok=audio_ok && (processing ? mcpx_apu_processing_capture(b,audio_bytes):mcpx_apu_rollback_capture(b,audio_bytes)) &&
        (processing ? mcpx_apu_processing_validate(a,audio_bytes):apu_rollback_validate(a,audio_bytes,NULL)) && !memcmp(a,b,audio_bytes);
    fiber_ok=fiber_count>0 && xbox_fiber_converted_validate(f,fiber_bytes)==fiber_count &&
        xbox_fiber_converted_capture(g,fiber_bytes)==fiber_count && !memcmp(f,g,fiber_bytes);
    clock_ok=clock_ok && xbox_det_validate(&first) && xbox_det_capture(&last) &&
        !memcmp(&first,&last,sizeof first);
done:
    if (audio_pin) mcpx_apu_rollback_release();
    if (clock_pin) xbox_det_checkpoint_release();
    if (audio_started) mcpx_apu_det_end();
    if (clock_started) xbox_det_end();
    QueryPerformanceCounter(&end);
    fprintf(stderr,"[ROLLBACK-OWNERSHIP] %s: clock_pin=%d audio_pin=%d clock_stable=%d audio_stable=%d fibers=%d fibers_stable=%d audio_bytes=%zu fiber_bytes=%zu elapsed_ms=%.3f; no restore\n",
        clock_pin && audio_pin && clock_ok && audio_ok && fiber_ok && state_ok && renderer_ok ? "passed":"rejected",
        clock_pin,audio_pin,clock_ok,audio_ok,fiber_count,fiber_ok,audio_bytes,fiber_bytes,
        1000.0*(double)(end.QuadPart-start.QuadPart)/(double)hz.QuadPart);
    if (extended) fprintf(stderr,"[ROLLBACK-STATE] %s: files_ready=%d ram=%u heap=%u files=%u completions=%u interrupts=%u gpu_queue=%u xpp=%u mask=%02X; two captures, no restore\n",
        state_ok ? "passed":"rejected",files_ready,state_mask&1u,(state_mask>>1)&1u,
        (state_mask>>2)&1u,(state_mask>>3)&1u,(state_mask>>4)&1u,(state_mask>>5)&1u,(state_mask>>6)&1u,state_mask);
    if (render) fprintf(stderr,"[ROLLBACK-RENDER] %s: ready=%d first=%d second=%d equal=%d; logical/pipeline/targets and exact pixels, no restore\n",
        renderer_ok ? "passed":"rejected",renderer_ready,renderer_first!=NULL,renderer_second!=NULL,renderer_ok);
    if (processing) fprintf(stderr,"[ROLLBACK-AUDIO-PROCESSING] %s: stable=%d bytes=%zu; command, partial PCM and output staging; capture only, no restore\n",
        audio_ok ? "passed":"rejected",audio_ok,audio_bytes);
    if (renderer.destroy) {
        renderer.destroy(renderer.context,renderer_second);
        renderer.destroy(renderer.context,renderer_first);
    }
    doa3_state_audit_destroy(state);
    free(a);free(b);free(f);free(g);
}
