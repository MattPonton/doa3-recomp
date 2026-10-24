/* Production capture lifetime: every bounded window releases both clocks,
 * recording stays off outside solo Practice, and failure never owns a session. */
#include "rollback_capture.h"
#include "xbox_det.h"
#include "game/training_dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "guest_memory_fixture.h"
#include "nv2a/nv2a_queue.h"
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"line %d: %s\n",__LINE__,#v); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
uint32_t g_str_a,g_str_b,g_flg_a,g_flg_b;
int g_flg_w,g_flg_test,g_fp_top;
uint16_t g_x87_cw;
double g_fp_stack[8];
volatile int g_xbox_det_active,g_apu_det;
unsigned g_doa3_frames_presented;
const unsigned char doa3_rollback_source_digest[32]={0};
static unsigned char *memory;
static unsigned begins,ends,apu_begins,apu_ends;
static unsigned apu_exports;
static unsigned gpu_exports;
int doa3_targets_capture(Nv2aTargetsState *out) {
    memset(out,0,sizeof(*out)); out->count=2;
    out->offsets[0]=0x1000; out->offsets[1]=gpu_exports*4; return 1;
}
int doa3_submission_capture(Nv2aSubmissionState *out) {
    Nv2aSubmissionState s=NV2A_SUBMISSION_INITIAL;
    s.parsed=0x1000+gpu_exports*4; s.methods=gpu_exports;
    *out=s; return 1;
}
int nv2a_rollback_queue_capture(void *out,size_t size) {
    Nv2aQueueState queue={0};
    CHECK(g_xbox_det_active && size==sizeof(Nv2aQueueSnapshot));
    queue.regs[31][1]=++gpu_exports;
    queue.pfifo_pending=gpu_exports+7; queue.pfifo_enabled=3;
    queue.pfifo_regs[0x124C]=gpu_exports*11;
    queue.pfifo_regs[0x1FFF]=gpu_exports*13;
    return nv2a_queue_export(&queue,out,size);
}
size_t apu_rollback_size(void) { return 8; }
int mcpx_apu_rollback_capture(void *out,size_t size) {
    CHECK(g_apu_det && size==8); memset(out,0xA5,size); ++apu_exports; return 1;
}
int doa3_rollback_tasks_enabled(void) { return 1; }
int doa3_rollback_task_checkpoint_valid(uint32_t routine,const Doa3RollbackTask *task) {
    (void)routine; return task->resume==1 && task->yielded==1;
}
void training_status(TrainingStatus *status) { memset(status,0,sizeof(*status)); }
void *xbox_GetMemoryBase(void) { return memory; }
size_t xbox_GetMemorySize(void) { return 128u*1024u*1024u; }
void xbox_det_current_bases(xbox_det_bases *bases) { memset(bases,0,sizeof(*bases)); }
void xbox_det_begin(const xbox_det_bases *bases) { (void)bases; CHECK(!g_xbox_det_active); g_xbox_det_active=1; ++begins; }
void xbox_det_end(void) { CHECK(g_xbox_det_active); g_xbox_det_active=0; ++ends; }
uint64_t mcpx_apu_xgscnt_now(void) { return 1234; }
void mcpx_apu_det_begin(uint64_t base) { CHECK(base==1234 && !g_apu_det); g_apu_det=1; ++apu_begins; }
void mcpx_apu_det_end(void) { CHECK(g_apu_det); g_apu_det=0; ++apu_ends; }
int main(int argc,char **argv)
{
    CHECK(argc==2 || argc==3); CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE",argv[1]));
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE_CONTIGUOUS",argc==3 ? "1" : "0"));
    int gpu=argc==3 && !strcmp(argv[2],"gpu");
    int apu=gpu || (argc==3 && !strcmp(argv[2],"apu"));
    int cancel=argc==3 && !strcmp(argv[2],"cancel");
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE_APU",apu ? "1" : "0"));
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE_GPU",gpu ? "1" : "0"));
    memory=guest_memory_fixture_alloc(xbox_GetMemorySize()); CHECK(memory);
    g_xbox_mem_offset=(ptrdiff_t)memory;
    Doa3RollbackTask task={1,1,{0}};
    const uint32_t routines[]={DOA3_RB_TASK_ROUTINES};
    for(unsigned n=0;n<150;++n) {
        doa3_rollback_capture_before(routines[0],&task);
        doa3_rollback_capture_after(routines[0],&task);
    }
    CHECK(!begins); // outside Practice
    memory[0x5A2858]=7; memory[0x596A37]=1;
    g_xbox_det_active=1;
    for(unsigned n=0;n<150;++n) doa3_rollback_capture_before(routines[0],&task);
    CHECK(!begins); g_xbox_det_active=0; // don't take over a real netplay clock
    for(unsigned frame=0;frame<900;++frame) for(unsigned i=0;i<DOA3_RB_TASK_COUNT;++i) {
        g_doa3_frames_presented=frame;
        doa3_rollback_capture_before(routines[i],&task);
        CHECK(g_xbox_det_active==g_apu_det);
        memory[0x500000]++; // a real mutable guest byte, captured as a page delta
        if(cancel && g_xbox_det_active) doa3_rollback_capture_cancel();
        doa3_rollback_capture_after(routines[i],&task);
        CHECK(!g_xbox_det_active && !g_apu_det);
    }
    CHECK(begins==(cancel ? 1u : DOA3_RBC_PAIRS) && ends==begins && apu_begins==begins && apu_ends==begins);
    FILE *file=fopen(argv[1],"rb"); CHECK(file);
    uint32_t words[8],footer[4]; CHECK(fread(words,1,32,file)==32);
    CHECK(words[0]==DOA3_RBC_MAGIC && words[1]==5 && words[6]==sizeof(Doa3RollbackCaptureState)+(apu?8:0)+(gpu?sizeof(Nv2aQueueSnapshot):0));
    uint32_t payloads[2]; CHECK(!fseek(file,64,SEEK_SET));
    CHECK(fread(payloads,1,8,file)==8 && payloads[0]==(apu?8:0) && payloads[1]==(gpu?sizeof(Nv2aQueueSnapshot):0));
    if(cancel) {
        /* The cancelled pre-image has no success footer or post-image. */
        uint32_t record[4]; CHECK(!fseek(file,72,SEEK_SET));
        CHECK(fread(record,1,16,file)==16 && record[0]==1);
        CHECK(!fseek(file,(long)words[6]+(long)record[2]*(4+4096),SEEK_CUR));
        CHECK(fgetc(file)==EOF); fclose(file); remove(argv[1]); guest_memory_fixture_free(memory);
        puts("Cancelled capture released both clocks and cannot be accepted as complete."); return 0;
    }
    CHECK(apu_exports==(apu ? DOA3_RBC_PAIRS*2 : 0));
    CHECK(gpu_exports==(gpu ? DOA3_RBC_PAIRS*2 : 0));
    CHECK(!fseek(file,72,SEEK_SET));
    for(unsigned n=0;n<DOA3_RBC_PAIRS*2;++n) {
        uint32_t record[4]; Doa3RollbackCaptureState state;
        CHECK(fread(record,1,16,file)==16 && record[0]==1+n%2);
        CHECK(record[1]==routines[(n/2)%DOA3_RB_TASK_COUNT]);
        CHECK(fread(&state,1,sizeof(state),file)==sizeof(state));
        if(apu) { unsigned char payload[8]; CHECK(fread(payload,1,8,file)==8);
            for(unsigned b=0;b<8;++b) CHECK(payload[b]==0xA5); }
        if(gpu) { Nv2aQueueSnapshot payload; Nv2aQueueState queue;
            CHECK(fread(&payload,1,sizeof(payload),file)==sizeof(payload));
            CHECK(nv2a_queue_import(&queue,&payload,sizeof(payload)) && queue.regs[31][1]==n+1);
            CHECK(queue.pfifo_pending==n+8 && queue.pfifo_enabled==3);
            CHECK(queue.pfifo_regs[0x124C]==(n+1)*11 && queue.pfifo_regs[0x1FFF]==(n+1)*13);
            CHECK(queue.submission.parsed==0x1000+(n+1)*4 && queue.submission.methods==n+1);
            CHECK(queue.targets.count==2 && queue.targets.offsets[0]==0x1000 && queue.targets.offsets[1]==(n+1)*4); }
        CHECK(!fseek(file,(long)record[2]*(4+4096),SEEK_CUR));
    }
    CHECK(fread(footer,1,16,file)==16);
    CHECK(!(footer[0]|footer[1]|footer[2]|footer[3]) && fgetc(file)==EOF); fclose(file);
    remove(argv[1]); guest_memory_fixture_free(memory);
    puts("Bounded capture released all clock windows and preserved existing sessions.");
    return 0;
}
