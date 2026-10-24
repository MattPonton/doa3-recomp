/* Opt-in, read-only audit of complete dispatcher passes and the gap between
 * them. Never restores RAM or changes the clock, APU, inputs or task ordering. */
#include "online/rollback_frame.h"
#include "online/rollback_capture.h"
#include "online/xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "recomp/recomp_types.h"
#include "training_dummy.h"
#include "rollback_input_tape.h"
#include "rollback_fight_checkpoint.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned g_doa3_frames_presented;
extern volatile int g_apu_det;
void xbox_kernel_copy_counts(uint32_t counts[DOA3_RBF_KERNEL_COUNTS]);
static int enabled=-1, active, pending, failed;
static unsigned warmup, frame, slot, routine, task_count;
static int last_phase=-1;
static FILE *stream;
static unsigned char *previous, *sample;
static size_t memory_bytes;

static void write_bytes(const void *data, size_t bytes)
{
    if (!failed && fwrite(data,1,bytes,stream)!=bytes) failed=1;
}
static void finish(void)
{
    uint32_t footer[8]={0};
    if (stream) {
        if (!failed) write_bytes(footer,sizeof(footer));
        if (fclose(stream)) failed=1;
    }
    stream=NULL; free(previous); free(sample); previous=sample=NULL;
    enabled=active=pending=0;
    fprintf(stderr,"[ROLLBACK-FRAME] %s after %u dispatcher passes; audit only, no live restore\n",
            failed ? "failed" : "complete",frame);
}
static int eligible(void)
{
    TrainingStatus status;
    if (MEM8(0x5A2858)!=7 || MEM8(0x596A37)!=1 || MEM8(0x596AA5) ||
        g_xbox_det_active || g_apu_det) return 0;
    training_status(&status);
    return status.mode==TRAINING_IDLE;
}
static void record(unsigned kind)
{
    uint32_t counts[DOA3_RBF_KERNEL_COUNTS];
    uint32_t header[8]={kind,frame,slot,routine,0,g_doa3_frames_presented,
                       MEM8(0x5A2858),MEM8(0x596A37)};
    xbox_kernel_copy_counts(counts);
    memcpy(sample,xbox_GetMemoryBase(),memory_bytes);
    for (size_t p=0;p<memory_bytes;p+=DOA3_RBF_PAGE)
        if (memcmp(previous+p,sample+p,DOA3_RBF_PAGE)) ++header[4];
    write_bytes(header,sizeof(header)); write_bytes(counts,sizeof(counts));
    for (size_t p=0;p<memory_bytes;p+=DOA3_RBF_PAGE) {
        if (!memcmp(previous+p,sample+p,DOA3_RBF_PAGE)) continue;
        uint32_t index=(uint32_t)(p/DOA3_RBF_PAGE);
        write_bytes(&index,sizeof(index)); write_bytes(sample+p,DOA3_RBF_PAGE);
    }
    unsigned char *swap=previous; previous=sample; sample=swap;
}
void doa3_rollback_frame_phase(uint32_t address)
{
    if(address==0xA64B0){doa3_fight_checkpoint_probe_boundary();doa3_fight_window_probe_boundary();}
    static const uint32_t phases[]={DOA3_RBF_PHASES};
    /* No allocations or memory scan on the normal, unarmed path. Only the
     * single gap between the two captured dispatcher passes is observed. */
    if (enabled!=1 || !stream || active || !frame || frame>=DOA3_RBF_FRAMES) return;
    int index=-1;
    for (unsigned i=0;i<sizeof(phases)/sizeof(phases[0]);++i)
        if(phases[i]==address) { index=(int)i; break; }
    if(pending || index!=last_phase+1 || !eligible()) { failed=1; finish(); return; }
    last_phase=index; slot=UINT32_MAX; routine=address;
    record(DOA3_RBF_PHASE);
    if(failed) finish();
}
void doa3_rollback_frame_boundary(uint32_t address, int before)
{
    if (address==0x1C650C && before) doa3_rollback_ownership_boundary();
    if (address==0x1C650C && before) doa3_input_audit_boundary();
    if (address==0x1C650C && pgraph_d3d11_rollback_audit_enabled())
        pgraph_d3d11_rollback_audit_boundary(before,eligible());
    if (enabled<0) {
        const char *path=getenv("DOA3_ROLLBACK_FRAME_CAPTURE");
        const char *task_capture=getenv("DOA3_ROLLBACK_CAPTURE");
        enabled=path && *path && doa3_rollback_tasks_enabled();
        if (enabled && task_capture && *task_capture) {
            enabled=0;
            fprintf(stderr,"[ROLLBACK-FRAME] refused: task capture is also armed\n");
        }
        if (enabled) fprintf(stderr,"[ROLLBACK-FRAME] armed for two complete Practice dispatcher passes\n");
    }
    if (!enabled) return;
    if (address==0x1C650C && before) {
        if (active) { failed=1; finish(); return; }
        if(stream && frame && last_phase!=(int)DOA3_RBF_PHASE_COUNT-1) { failed=1; finish(); return; }
        if (!eligible()) {
            warmup=0;
            if (stream) { failed=1; finish(); }
            return;
        }
        if (!stream && ++warmup<120) return;
        if (!stream) {
            memory_bytes=xbox_GetMemorySize();
            if (memory_bytes!=128u*1024u*1024u) { failed=1; finish(); return; }
            previous=calloc(1,memory_bytes); sample=malloc(memory_bytes);
            if (previous && sample) stream=fopen(getenv("DOA3_ROLLBACK_FRAME_CAPTURE"),"wb");
            if (!stream) { failed=1; finish(); return; }
            uint32_t header[8]={DOA3_RBF_MAGIC,DOA3_RBF_VERSION,(uint32_t)memory_bytes,
                (uint32_t)g_xbox_mem_offset,DOA3_RBF_PAGE,DOA3_RBF_FRAMES,
                DOA3_RBF_KERNEL_COUNTS,0};
            write_bytes(header,sizeof(header)); write_bytes(doa3_rollback_source_digest,32);
        }
        active=1; pending=0; task_count=0; slot=UINT32_MAX; routine=0;
        record(DOA3_RBF_BEGIN);
    } else if (active && address==0x1C64C7) {
        if (before) {
            uint32_t record_va=MEM32(g_esp+4);
            uint32_t next=MEM32(0x73C644);
            if (pending || next>=18 || record_va!=0x73C400+next*32 ||
                (task_count && next<=slot)) { failed=1; finish(); return; }
            slot=next; routine=MEM32(record_va+12); pending=1; ++task_count;
            record(DOA3_RBF_TASK_BEGIN);
        } else {
            if (!pending) { failed=1; finish(); return; }
            record(DOA3_RBF_TASK_END); pending=0;
        }
    } else if (active && address==0x1C650C && !before) {
        if (pending || !task_count || !eligible()) { failed=1; finish(); return; }
        slot=UINT32_MAX; routine=0; record(DOA3_RBF_END); active=0; ++frame; last_phase=-1;
        if (frame==DOA3_RBF_FRAMES) { finish(); return; }
    }
    if (failed) finish();
}
