#include "rollback_frame.h"
#include "rollback_capture.h"
#include "game/training_dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "guest_memory_fixture.h"
#define CHECK(v) do { if (!(v)) { fprintf(stderr,"line %d: %s\n",__LINE__,#v); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_esp;
unsigned g_doa3_frames_presented;
volatile int g_xbox_det_active,g_apu_det;
const unsigned char doa3_rollback_source_digest[32]={0};
static unsigned char *memory;
static unsigned counts[400];
#define U32(va) (*(uint32_t *)(memory+(va)))
int doa3_rollback_tasks_enabled(void) { return 1; }
void doa3_rollback_ownership_boundary(void) {}
void doa3_input_audit_boundary(void) {}
void doa3_fight_checkpoint_probe_boundary(void) {}
void doa3_fight_window_probe_boundary(void) {}
int pgraph_d3d11_rollback_audit_enabled(void) { return 0; }
void pgraph_d3d11_rollback_audit_boundary(int before,int eligible) { (void)before; (void)eligible; }
void training_status(TrainingStatus *status) { memset(status,0,sizeof(*status)); }
void *xbox_GetMemoryBase(void) { return memory; }
size_t xbox_GetMemorySize(void) { return 128u*1024u*1024u; }
void xbox_kernel_copy_counts(uint32_t *out) { memcpy(out,counts,sizeof(counts)); }
int main(int argc,char **argv)
{
    CHECK(argc==3);
    CHECK(!_putenv_s("DOA3_ROLLBACK_FRAME_CAPTURE",argv[1]));
    CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE",""));
    memory=guest_memory_fixture_alloc(xbox_GetMemorySize()); CHECK(memory);
    g_xbox_mem_offset=(ptrdiff_t)memory; g_esp=0x10000;
    for (unsigned n=0;n<150;++n) {
        doa3_rollback_frame_boundary(0x1C650C,1);
        doa3_rollback_frame_boundary(0x1C650C,0);
    }
    FILE *file=fopen(argv[1],"rb"); CHECK(!file); /* gated outside Practice */
    memory[0x5A2858]=7; memory[0x596A37]=1;
    g_xbox_det_active=1;
    for (unsigned n=0;n<150;++n) doa3_rollback_frame_boundary(0x1C650C,1);
    file=fopen(argv[1],"rb"); CHECK(!file); g_xbox_det_active=0;
    for (unsigned n=0;n<119;++n) {
        doa3_rollback_frame_boundary(0x1C650C,1);
        doa3_rollback_frame_boundary(0x1C650C,0);
    }
    const unsigned slots[]={6,11}, routines[]={0x11110,0x49790};
    const unsigned phases[]={DOA3_RBF_PHASES};
    for (unsigned n=0;n<2;++n) {
        if (n) {
            for(unsigned i=0;i<DOA3_RBF_PHASE_COUNT;++i) {
                memory[0x500003]++; counts[277]++;
                uint32_t address=phases[i];
                if(!strcmp(argv[2],"bad-phase") && i==3) address=0xDEAD;
                if(!strcmp(argv[2],"skip-phase") && i==3) continue;
                doa3_rollback_frame_phase(address);
            }
            memory[0x500002]++; /* final outside-dispatcher write */
        }
        doa3_rollback_frame_boundary(0x1C650C,1);
        for (unsigned k=0;k<2;++k) {
            U32(0x73C644)=slots[k]; U32(g_esp+4)=0x73C400+slots[k]*32;
            U32(0x73C400+slots[k]*32+12)=routines[k];
            if (!strcmp(argv[2],"bad-slot")) U32(g_esp+4)++;
            doa3_rollback_frame_boundary(0x1C64C7,1);
            memory[0x500000+k]++; counts[246]+=k+1;
            if (k) ++g_doa3_frames_presented;
            doa3_rollback_frame_boundary(0x1C64C7,0);
        }
        doa3_rollback_frame_boundary(0x1C650C,0);
    }
    CHECK(!g_xbox_det_active && !g_apu_det);
    file=fopen(argv[1],"rb"); CHECK(file); fclose(file);
    guest_memory_fixture_free(memory);
    return 0;
}
