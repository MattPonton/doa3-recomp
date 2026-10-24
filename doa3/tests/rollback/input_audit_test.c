#include "game/rollback_input_tape.h"
#include "game/training_dummy.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);} } while(0)
ptrdiff_t g_xbox_mem_offset;
volatile int g_xbox_det_active;
static int training;
uint32_t xbox_det_epoch(void){return 1;}
int xbox_fiber_is_primary(void){return 1;}
int doa3_rollback_tasks_enabled(void){return 1;}
void training_status(TrainingStatus *s){memset(s,0,sizeof(*s));s->mode=training;}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *s){memset(s,0,sizeof(*s));s->Gamepad.wButtons=(WORD)(1u<<p);return 0;}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v){(void)p;(void)v;return 0;}
DWORD xbox_InputHostMask(void){return 15;}
DWORD xbox_InputTakeReconfiguredMask(void){return 0;}
BYTE xbox_InputGuestSubtype(int p){(void)p;return 1;}
int main(int argc,char **argv)
{
    CHECK(argc==2);unsigned char *memory=guest_memory_fixture_alloc(128u*1024u*1024u);CHECK(memory);
    g_xbox_mem_offset=(ptrdiff_t)memory;
    CHECK(!_putenv_s("DOA3_ROLLBACK_INPUT_AUDIT","1"));
    CHECK(!_putenv_s("DOA3_ROLLBACK_OWNERSHIP_AUDIT",!strcmp(argv[1],"conflict") ? "3":""));
    CHECK(!_putenv_s("DOA3_ROLLBACK_RENDER_AUDIT",""));
    CHECK(!_putenv_s("DOA3_ROLLBACK_FRAME_CAPTURE",""));CHECK(!_putenv_s("DOA3_ROLLBACK_CAPTURE",""));
    for(unsigned i=0;i<130;++i)doa3_input_audit_boundary(); /* menu is ineligible */
    memory[0x5A2858]=7;memory[0x596A37]=1;
    g_xbox_det_active=1;for(unsigned i=0;i<130;++i)doa3_input_audit_boundary();g_xbox_det_active=0;
    training=1;for(unsigned i=0;i<130;++i)doa3_input_audit_boundary();training=0;
    for(unsigned i=0;i<120;++i)doa3_input_audit_boundary();
    for(unsigned i=0;i<600;++i){
        XBOX_INPUT_STATE s;for(unsigned p=0;p<4;++p)CHECK(!doa3_input_state(p,&s));
        if(!strcmp(argv[1],"leave") && i==5)memory[0x5A2858]=0;
        doa3_input_audit_boundary();
    }
    /* Recorder is detached after completion/exit/refusal. */
    Doa3InputTape *t=doa3_input_tape_create(1);CHECK(t);CHECK(doa3_input_tape_record(t,0));
    CHECK(doa3_input_tape_end(t));CHECK(doa3_input_tape_destroy(t));
    guest_memory_fixture_free(memory);return 0;
}
