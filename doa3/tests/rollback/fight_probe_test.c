#include "game/rollback_fight_checkpoint.h"
#include "game/training_dummy.h"
#include "online/xbox_det.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
ptrdiff_t g_xbox_mem_offset;
volatile int g_xbox_det_active,g_apu_det;
static unsigned attempts,starts,ends;
static int primary=1,training=TRAINING_IDLE;
int xbox_fiber_is_primary(void){return primary;}
int doa3_rollback_tasks_enabled(void){return 1;}
void training_status(TrainingStatus *s){memset(s,0,sizeof(*s));s->mode=training;}
void xbox_det_current_bases(xbox_det_bases *b){memset(b,0,sizeof(*b));}
void xbox_det_begin(const xbox_det_bases *b){(void)b;CHECK(!g_xbox_det_active);++starts;g_xbox_det_active=1;}
void xbox_det_end(void){CHECK(g_xbox_det_active && !g_apu_det);++ends;g_xbox_det_active=0;}
uint64_t mcpx_apu_xgscnt_now(void){return 0;}
void mcpx_apu_det_begin(uint64_t base){(void)base;CHECK(g_xbox_det_active && !g_apu_det);g_apu_det=1;}
void mcpx_apu_det_end(void){CHECK(g_apu_det);g_apu_det=0;}
void doa3_loop_init(Doa3LoopState *s){memset(s,0,sizeof(*s));s->schema=DOA3_LOOP_SCHEMA;}
int doa3_fight_checkpoint_roundtrip(Doa3LoopState *s,Doa3CheckpointComparison *d){
    CHECK(s->schema==DOA3_LOOP_SCHEMA && !s->resume && primary && g_xbox_det_active && g_apu_det);
    memset(d,0,sizeof(*d));++attempts;return 1;
}
static void frames(unsigned n){while(n--)doa3_fight_checkpoint_probe_boundary();}
int main(int argc,char **argv){
    CHECK(argc==2);int enabled=!strcmp(argv[1],"enabled"),conflict=!strcmp(argv[1],"conflict");
    _putenv_s("DOA3_ROLLBACK_CHECKPOINT_TEST",enabled || conflict ? "1":"0");
    _putenv_s("DOA3_ROLLBACK_INPUT_AUDIT",conflict ? "1":"0");
    unsigned char *ram=guest_memory_fixture_alloc(16u*1024u*1024u);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    frames(130);CHECK(!attempts);ram[0x5A2858]=7;ram[0x596A37]=1;
    primary=0;frames(130);primary=1;CHECK(!attempts);
    frames(90);ram[0x596AA5]=1;frames(1);ram[0x596AA5]=0;
    frames(119);CHECK(!attempts);frames(1);CHECK(attempts==(unsigned)enabled);
    frames(300);CHECK(attempts==(unsigned)enabled && starts==attempts && ends==attempts && !g_xbox_det_active && !g_apu_det);
    guest_memory_fixture_free(ram);puts("Live checkpoint probe: opt-in, conflict/eligibility gates, warmup reset and exactly-once execution passed");return 0;
}
