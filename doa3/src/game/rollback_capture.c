/* Explicitly armed, bounded capture of real fight task boundaries with the
 * clock/APU pinned per sample. No live restore, screenshots, or networking. */
#include "online/rollback_capture.h"
#include "recomp/recomp_types.h"
#include "kernel/xbox_memory_layout.h"
#include "training_dummy.h"
#include "online/xbox_det.h"
#include "apu/apu_rollback.h"
#include "nv2a/nv2a_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *s_file;
static unsigned char *s_previous;
static size_t s_bytes;
static int s_enabled = -1, s_recording, s_failed;
static unsigned s_wait, s_pairs, s_index;
static int s_clock_window;
static int s_contiguous;
static size_t s_apu_bytes;
static void *s_apu_image;
static size_t s_gpu_bytes;
static Nv2aQueueSnapshot s_gpu_image;
static unsigned s_frame;
int doa3_rollback_capture_is_armed(void) { return s_enabled>0; }
extern unsigned g_doa3_frames_presented;
extern volatile int g_apu_det;
void mcpx_apu_det_begin(uint64_t base);
void mcpx_apu_det_end(void);
uint64_t mcpx_apu_xgscnt_now(void);
static const uint32_t routines[DOA3_RB_TASK_COUNT] = {DOA3_RB_TASK_ROUTINES};

static void capture_state(Doa3RollbackCaptureState *out, const Doa3RollbackTask *task)
{
    const uint32_t regs[16] = {g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp,
        g_str_a,g_str_b,g_flg_a,g_flg_b,(uint32_t)g_flg_w,(uint32_t)g_flg_test,
        (uint32_t)g_fp_top,g_x87_cw};
    memset(out,0,sizeof(*out));
    memcpy(out->regs,regs,sizeof(regs));
    memcpy(out->fp,g_fp_stack,sizeof(out->fp));
    out->task = *task;
}
static void write_data(const void *data, size_t bytes)
{
    if (!s_failed && fwrite(data,1,bytes,s_file) != bytes) s_failed = 1;
}
static void record(unsigned kind, uint32_t routine, const Doa3RollbackTask *task)
{
    const unsigned char *memory = xbox_GetMemoryBase();
    Doa3RollbackCaptureState state;
    uint32_t header[4] = {kind,routine,0,g_doa3_frames_presented};
    capture_state(&state,task);
    /* Stage the entire sample before doing file IO. Workers are cooperative;
     * clock/APU writers are pinned, and every guest-memory byte is compared. */
    unsigned char *sample = malloc(s_bytes);
    if (!sample) { s_failed = 1; return; }
    memcpy(sample,memory,s_bytes);
    if(s_apu_bytes && !mcpx_apu_rollback_capture(s_apu_image,s_apu_bytes)) {
        free(sample); s_failed=1; return;
    }
    if(s_gpu_bytes && !nv2a_rollback_queue_capture(&s_gpu_image,s_gpu_bytes)) {
        free(sample); s_failed=1; return;
    }
    if(s_gpu_bytes && (!doa3_submission_capture(&s_gpu_image.queue.submission) ||
        !doa3_targets_capture(&s_gpu_image.queue.targets))) {
        free(sample); s_failed=1; return;
    }
    for (size_t p=0;p<s_bytes;p+=DOA3_RBC_PAGE)
        if (memcmp(sample+p,s_previous+p,DOA3_RBC_PAGE)) ++header[2];
    write_data(header,sizeof(header)); write_data(&state,sizeof(state));
    if(s_apu_bytes) write_data(s_apu_image,s_apu_bytes);
    if(s_gpu_bytes) write_data(&s_gpu_image,s_gpu_bytes);
    for (size_t p=0;p<s_bytes;p+=DOA3_RBC_PAGE) {
        if (!memcmp(sample+p,s_previous+p,DOA3_RBC_PAGE)) continue;
        uint32_t page = (uint32_t)(p / DOA3_RBC_PAGE);
        write_data(&page,sizeof(page)); write_data(sample+p,DOA3_RBC_PAGE);
    }
    memcpy(s_previous,sample,s_bytes);
    free(sample);
}
static void finish(void)
{
    if (s_clock_window) { mcpx_apu_det_end(); xbox_det_end(); s_clock_window=0; }
    uint32_t end[4] = {0,0,0,0};
    if (s_file) {
        if (!s_failed) write_data(end,sizeof(end));
        if (fclose(s_file)) s_failed = 1;
    }
    s_file = NULL; free(s_previous); s_previous = NULL;
    free(s_apu_image); s_apu_image=NULL;
    s_enabled = 0; s_recording = 0;
    fprintf(stderr,"[ROLLBACK-CAPTURE] %s after %u task pairs; no live state restored\n",
            s_failed ? "failed" : "complete",s_pairs);
}
void doa3_rollback_capture_before(uint32_t routine, const Doa3RollbackTask *task)
{
    if (s_recording) { s_failed=1; return; }
    if (s_enabled < 0) {
        const char *path = getenv("DOA3_ROLLBACK_CAPTURE");
        const char *continuous = getenv("DOA3_ROLLBACK_CAPTURE_CONTIGUOUS");
        s_contiguous = continuous && !strcmp(continuous,"1");
        const char *apu=getenv("DOA3_ROLLBACK_CAPTURE_APU");
        if(apu && !strcmp(apu,"1")) s_apu_bytes=apu_rollback_size();
        const char *gpu=getenv("DOA3_ROLLBACK_CAPTURE_GPU");
        if(gpu && !strcmp(gpu,"1")) s_gpu_bytes=sizeof(s_gpu_image);
        s_enabled = path && *path && doa3_rollback_tasks_enabled();
        if (s_enabled) fprintf(stderr,"[ROLLBACK-CAPTURE] armed for %u task pairs in solo Practice\n",DOA3_RBC_PAIRS);
    }
    if (!s_enabled) return;
    if (s_contiguous && s_file && routine!=routines[s_index]) { s_failed=1; finish(); return; }
    if (routine != routines[s_index] || !doa3_rollback_task_checkpoint_valid(routine,task) ||
        MEM8(0x5A2858)!=7 || MEM8(0x596A37)!=1 || MEM8(0x596AA5) ||
        g_xbox_det_active || g_apu_det) {
        if (s_contiguous && s_file) { s_failed=1; finish(); }
        return;
    }
    TrainingStatus status; training_status(&status);
    if (status.mode != TRAINING_IDLE) {
        if (s_contiguous && s_file) { s_failed=1; finish(); }
        return;
    }
    if (!s_index && (!s_contiguous || !s_file) && ++s_wait < 120) return;
    if (s_contiguous && s_file && g_doa3_frames_presented != s_frame + (s_index==0 ? 1u : 0u)) {
        s_failed=1; finish(); return;
    }
    if (!s_index) s_frame=g_doa3_frames_presented;
    if (!s_file) {
        s_bytes = xbox_GetMemorySize();
        if (!s_bytes || s_bytes > 128u*1024u*1024u || s_bytes % DOA3_RBC_PAGE) {
            s_failed=1; finish(); return;
        }
        s_previous = calloc(1,s_bytes);
        if(s_apu_bytes) {
            s_apu_image=malloc(s_apu_bytes);
            if(!s_apu_image) { s_failed=1; finish(); return; }
        }
        if (s_previous) s_file = fopen(getenv("DOA3_ROLLBACK_CAPTURE"),"wb");
        if (!s_file) { s_failed=1; finish(); return; }
        uint32_t header[8] = {DOA3_RBC_MAGIC,DOA3_RBC_VERSION,(uint32_t)s_bytes,
            (uint32_t)g_xbox_mem_offset,(uint32_t)DOA3_XBE_FNV64,
            (uint32_t)(DOA3_XBE_FNV64>>32),(uint32_t)(sizeof(Doa3RollbackCaptureState)+s_apu_bytes+s_gpu_bytes),DOA3_RBC_PAIRS};
        write_data(header,sizeof(header));
        write_data(doa3_rollback_source_digest,32);
        uint32_t payloads[2]={(uint32_t)s_apu_bytes,(uint32_t)s_gpu_bytes};
        write_data(payloads,sizeof(payloads));
    }
    /* Pin asynchronous clock/APU writers only for this bounded task sample.
     * Reuse the existing deterministic-clock APIs; never take over a session.
     * This is controlled diagnostic sampling, not an APU rollback solution. */
    xbox_det_bases bases; xbox_det_current_bases(&bases);
    xbox_det_begin(&bases); mcpx_apu_det_begin(mcpx_apu_xgscnt_now()); s_clock_window=1;
    record(1,routine,task); s_recording=1;
}
void doa3_rollback_capture_after(uint32_t routine, const Doa3RollbackTask *task)
{
    if (!s_recording) return;
    if (routine!=routines[s_index] || !doa3_rollback_task_checkpoint_valid(routine,task)) { s_failed=1; finish(); return; }
    if (s_contiguous && g_doa3_frames_presented!=s_frame) { s_failed=1; finish(); return; }
    record(2,routine,task); s_recording=0; ++s_pairs;
    mcpx_apu_det_end(); xbox_det_end(); s_clock_window=0;
    s_index=(s_index+1)%DOA3_RB_TASK_COUNT; if (!s_index) s_wait=0;
    if (s_failed || s_pairs==DOA3_RBC_PAIRS) finish();
}
void doa3_rollback_capture_cancel(void)
{
    /* Startup/loading handoffs before the first sample do not spoil a future
     * capture at a fully unwound boundary. Any handoff in a capture does. */
    if (!s_file) return;
    s_failed=1;
    finish();
}
