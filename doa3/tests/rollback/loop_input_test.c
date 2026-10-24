/* Real outer-loop poll/decode/feedback bodies and production XPP/tape code.
 * Other outer-loop helpers and physical controllers remain fixtures. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop_input.h"
#include "game/rollback_input.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define BYTES (16u*1024u*1024u)
#define FRAMES 48
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
unsigned g_doa3_frames_presented;
volatile int g_xbox_det_active=1;
static uint32_t epoch=1;
static int primary=1;
static unsigned char *ram;
static unsigned reads,writes,metadata,metrics,logs;
static uint32_t host_mask=15,reconfigured;
static unsigned input_frame;
uint32_t xbox_det_epoch(void){return epoch;}
int xbox_fiber_is_primary(void){return primary;}
void *xbox_GetMemoryBase(void){return ram;}
size_t xbox_GetMemorySize(void){return BYTES;}
DWORD xbox_InputHostMask(void){++metadata;return host_mask;}
DWORD xbox_InputTakeReconfiguredMask(void){++metadata;DWORD n=reconfigured;reconfigured=0;return n;}
BYTE xbox_InputGuestSubtype(int p){++metadata;return (BYTE)(p+1);}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v){(void)p;(void)v;++writes;return 0;}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *out){
    ++reads;if(!(host_mask&(1u<<p)))return ERROR_DEVICE_NOT_CONNECTED;
    memset(out,0,sizeof(*out));out->dwPacketNumber=reads;
    const BYTE pressure[]={0,31,32,127,128,255};
    const SHORT axis[]={-32768,-8193,-8192,-8191,0,8191,8192,8193,32767};
    out->Gamepad.wButtons=input_frame<7 ? (WORD)(reads*7):input_frame>=8 && input_frame<44 ? 1:0;
    for(unsigned i=0;i<8;++i)out->Gamepad.bAnalogButtons[i]=input_frame<7 ? pressure[(input_frame+p+i)%6]:0;
    out->Gamepad.sThumbLX=axis[input_frame%9];out->Gamepad.sThumbLY=axis[(input_frame+1)%9];
    out->Gamepad.sThumbRX=axis[(input_frame+2)%9];out->Gamepad.sThumbRY=axis[(input_frame+3)%9];return 0;
}
uint32_t netplay_filter_pad_mask(uint32_t m){return m;}
uint32_t netplay_forced_pads(void){return 0;}
void doa3_perf_input_sample(unsigned p){(void)p;++metrics;}
static int counted_fprintf(FILE *f,const char *format,...){(void)f;(void)format;++logs;return 0;}
static int counted_fflush(FILE *f){(void)f;++logs;return 0;}
#define fprintf counted_fprintf
#define fflush counted_fflush
#include "input_bridge.generated.inc"
#undef fprintf
#undef fflush
#include "loop_input_reference.generated.inc"
typedef struct {uint32_t r[8];double fp[8];int top;} Registers;
static Registers regs(void){
    Registers r={0};uint32_t words[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};
    memcpy(r.r,words,sizeof words);memcpy(r.fp,g_fp_stack,sizeof r.fp);r.top=g_fp_top;return r;
}
static void load(Registers r){
    g_eax=r.r[0];g_ecx=r.r[1];g_edx=r.r[2];g_esp=r.r[3];g_ebx=r.r[4];g_esi=r.r[5];g_edi=r.r[6];g_seh_ebp=r.r[7];
    memcpy(g_fp_stack,r.fp,sizeof r.fp);g_fp_top=r.top;
}
static uint64_t hash(const void *data,size_t n){
    const unsigned char *p=data;uint64_t h=14695981039346656037ull;
    for(size_t i=0;i<n;++i)h=(h^p[i])*1099511628211ull;return h;
}
static void prepare(unsigned frame,int record){
    input_frame=frame;
    if(record){const uint32_t masks[]={15,15,13,9,15,15};host_mask=frame<8 ? masks[frame%6]:15;reconfigured=frame==7 ? 8:0;}
    g_doa3_frames_presented=frame*300;
    for(unsigned p=0;p<4;++p){
        uint32_t pad=0x73C6C8+p*128;
        MEM32(pad+0x7C)=(frame+p)%3 ? 1:3;
        MEM32(pad+0x2F)=(frame+p)%4==3 ? 0x3E5:0;
        MEM16(pad+0x71)=(uint16_t)(500+frame*30+p);MEM16(pad+0x73)=(uint16_t)(900+frame*40+p);
    }
}
static void other_helper(uint32_t routine){
    /* Explicit test-only stand-ins; production adapter has NO fallback. */
    g_eax=g_eax*7+routine;g_esp+=4+(routine==0x1DF010 ? 24:routine==0x1E14D0 ? 4:0);
}
static void refused(Doa3LoopState *state,Doa3InputTape *t){
    uint64_t memory=hash(ram,BYTES);Doa3LoopState before=*state;Registers a=regs();
    unsigned hw=reads+writes+metadata+metrics+logs;
    CHECK(doa3_loop_input_run(state,t)==0);Registers b=regs();
    CHECK(!memcmp(&before,state,sizeof before) && !memcmp(&a,&b,sizeof a));
    CHECK(hash(ram,BYTES)==memory && hw==reads+writes+metadata+metrics+logs);
}
static void decoder_guards(Doa3LoopState *state,Doa3InputTape *t){
    const uint32_t tables[]={0x2D4C50,0x2D4C98,0x2D4CD0};const unsigned slots[]={9,7,3};
    for(unsigned j=0;j<3;++j){
        /* With the last sentinel removed, no loop may run off its table. */
        uint32_t end=tables[j]+(slots[j]-1)*8+4;MEM32(end)=1;refused(state,t);MEM32(end)=0;
        if(j){
            uint32_t saved=MEM32(tables[j]);
            MEM32(tables[j])=8;refused(state,t);
            MEM32(tables[j])=0xFFFFFFF0;refused(state,t);MEM32(tables[j])=saved;
        }
    }
    uint32_t sp=g_esp;
    const uint32_t alias[]={0x2D4C50,0x2D4CFC,0x73C6C8,0x73C9C0};
    for(unsigned i=0;i<4;++i){g_esp=alias[i];refused(state,t);}g_esp=sp;
    uint32_t pending=state->pending;
    state->pending=0xA4E99;refused(state,t);state->pending=0xA4EA0;refused(state,t);state->pending=pending;
}
static void seed_tables(void){
    const uint32_t digital[]={0x10,0x20,1,2,4,8,0x40,0x80};
    for(unsigned i=0;i<8;++i){MEM32(0x2D4C50+i*8)=digital[i];MEM32(0x2D4C54+i*8)=digital[i];}
    for(unsigned i=0;i<6;++i){MEM32(0x2D4C98+i*8)=i;MEM32(0x2D4C9C+i*8)=0x100u<<i;}
    for(unsigned i=0;i<2;++i){MEM32(0x2D4CD0+i*8)=6+i;MEM32(0x2D4CD4+i*8)=0x4000u<<i;}
}
static void decode_expected(uint32_t result[4][4]){
    for(unsigned p=0;p<4;++p){
        uint32_t pad=0x73C6C8+p*128,out=0x73C8D0+p*44,buttons=0;
        if(MEM32(0x73C8C8)&(1u<<p)){
            buttons=MEM16(pad+0x1D)&0xFF;
            for(unsigned i=0;i<8;++i)if(MEM8(pad+0x1F+i)>=(i<6 ? 32:128))buttons|=0x100u<<i;
        }
        uint32_t old=MEM32(out);
        result[p][0]=buttons;result[p][1]=old;result[p][2]=buttons&~old;result[p][3]=old&~buttons;
    }
}
static void decoded(const uint32_t expected[4][4]){
    uint32_t aggregate[5]={0};
    for(unsigned p=0;p<4;++p){
        uint32_t out=0x73C8D0+p*44,pad=0x73C6C8+p*128;
        for(unsigned i=0;i<4;++i){CHECK(MEM32(out+i*4)==expected[p][i]);aggregate[i]|=expected[p][i];}
        aggregate[4]|=MEM32(out+16);
        for(unsigned i=0;i<8;++i)CHECK(MEM8(out+20+i)==((MEM32(0x73C8C8)&(1u<<p)) ? MEM8(pad+0x1F+i):0));
        if(!(MEM32(0x73C8C8)&(1u<<p)))for(unsigned i=0;i<4;++i)CHECK(MEM16(out+28+i*2)==0);
        if(input_frame>=8){
            CHECK(MEM32(out+16)==(input_frame==8 || input_frame==38 || input_frame==43 ? 1u:0u));
            if(input_frame>=44)CHECK(MEM32(out+40)==0);
        }
    }
    for(unsigned i=0;i<5;++i)CHECK(MEM32(0x73C980+i*4)==aggregate[i]);
}
static int interval(Doa3LoopState *state,Doa3InputTape *t,int replay,int mismatch,int checks){
    for(unsigned step=0;step<DOA3_LOOP_MAX_CALLS;++step){
        Doa3LoopAction action;CHECK(doa3_loop_next(state,&action));
        if(action.kind==DOA3_LOOP_BOUNDARY)return 1;
        if(action.routine==0xA4DD0 || action.routine==0xA4E70 || action.routine==0xA50A0){
            uint32_t expected_decode[4][4];
            if(action.routine==0xA4E70)decode_expected(expected_decode);
            if(replay){
                if(checks && action.routine==0xA4DD0){
                    refused(state,NULL);
                    Doa3InputTape *other=doa3_input_tape_create(1);CHECK(other);
                    refused(state,other);CHECK(doa3_input_tape_destroy(other));
                    primary=0;refused(state,t);primary=1;
                    ++epoch;refused(state,t);--epoch;
                    uint32_t sp=g_esp;g_esp=4;refused(state,t);g_esp=sp;
                }
                if(checks && action.routine==0xA4E70)decoder_guards(state,t);
                if(mismatch && action.routine==0xA50A0)MEM16(0x73C739)++;
                int result=doa3_loop_input_run(state,t);
                if(result<0){CHECK(state->pending==action.routine);return -1;}
                CHECK(result==1 && !state->pending);
            }else{
                refused(state,t); /* recording is not replay permission */
                if(action.routine==0xA4DD0)reference_input_A4DD0();
                else if(action.routine==0xA4E70)reference_input_A4E70();else reference_input_A50A0();
                CHECK(doa3_loop_complete(state,action.routine));
            }
            if(action.routine==0xA4E70)decoded(expected_decode);
        }else{
            if(checks)refused(state,t); /* every unapproved helper rejects */
            other_helper(action.routine);CHECK(doa3_loop_complete(state,action.routine));
        }
    }
    CHECK(0);return 0;
}
int main(void){
    _putenv_s("DOA3_RUMBLE_TRACE","1");
    ram=guest_memory_fixture_alloc(BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    unsigned char *initial=malloc(BYTES),*final=malloc(BYTES);CHECK(initial && final);
    g_esp=0x100000;g_seh_ebp=123;g_fp_top=3;
    seed_tables();
    doa3_xpp_set_mask(0);
    Registers start=regs(),expected_regs[FRAMES];Doa3XppSnapshot start_xpp,expected_xpp[FRAMES];
    CHECK(doa3_xpp_capture(&start_xpp,sizeof start_xpp));memcpy(initial,ram,BYTES);
    Doa3LoopState loop;doa3_loop_init(&loop);Doa3LoopState expected_loop[FRAMES];uint64_t expected[FRAMES];
    Doa3InputTape *t=doa3_input_tape_create(FRAMES);CHECK(t);
    CHECK(!doa3_input_replay_active() && !doa3_input_tape_replay_ready(t));
    for(unsigned frame=0;frame<FRAMES;++frame){
        prepare(frame,1);CHECK(doa3_input_tape_record(t,frame));CHECK(interval(&loop,t,0,0,0)==1);CHECK(doa3_input_tape_end(t));
        expected[frame]=hash(ram,BYTES);expected_regs[frame]=regs();expected_loop[frame]=loop;
        CHECK(doa3_xpp_capture(&expected_xpp[frame],sizeof start_xpp));CHECK(g_esp==start.r[3]);
    }
    memcpy(final,ram,BYTES);unsigned hw=reads+writes+metadata,diag=metrics+logs;
    Doa3InputTapeStats stats;CHECK(doa3_input_tape_stats(t,&stats));
    CHECK(stats.ports==15 && stats.rumble_calls && stats.state_reads && logs && metrics);
    host_mask=0;reconfigured=15; /* real controllers differ during all replays */
    for(unsigned pass=0;pass<3;++pass){
        memcpy(ram,initial,BYTES);load(start);doa3_loop_init(&loop);CHECK(doa3_xpp_restore(&start_xpp,sizeof start_xpp));
        for(unsigned frame=0;frame<FRAMES;++frame){
            prepare(frame,0);CHECK(doa3_input_tape_replay(t,frame));CHECK(doa3_input_tape_replay_ready(t));
            CHECK(interval(&loop,t,1,0,pass==0 && frame==0)==1);CHECK(doa3_input_tape_end(t));
            Registers now=regs();Doa3XppSnapshot xpp;CHECK(doa3_xpp_capture(&xpp,sizeof xpp));
            CHECK(hash(ram,BYTES)==expected[frame] && !memcmp(&now,&expected_regs[frame],sizeof now));
            CHECK(!memcmp(&loop,&expected_loop[frame],sizeof loop) && !memcmp(&xpp,&expected_xpp[frame],sizeof xpp));
            CHECK(reads+writes+metadata==hw && metrics+logs==diag);
        }
        CHECK(!memcmp(final,ram,BYTES));
    }
    memcpy(ram,initial,BYTES);load(start);doa3_loop_init(&loop);CHECK(doa3_xpp_restore(&start_xpp,sizeof start_xpp));
    prepare(0,0);CHECK(doa3_input_tape_replay(t,0));CHECK(interval(&loop,t,1,1,0)==-1);
    CHECK(doa3_input_replay_active() && !doa3_input_tape_replay_ready(t));refused(&loop,t);
    CHECK(!doa3_input_tape_end(t));CHECK(reads+writes+metadata==hw && metrics+logs==diag);
    /* Test teardown stands in for the enclosing trial's full recovery. */
    CHECK(doa3_input_tape_abort(t));CHECK(!doa3_input_replay_active());CHECK(doa3_input_tape_destroy(t));
    free(initial);free(final);guest_memory_fixture_free(ram);
    puts("Outer-loop input helpers: three 48-interval replays match RAM/registers/XPP; four ports, reconnects, presses/releases, pressure thresholds, 30/5-tick repeat, pending/expiring rumble; zero replay controller, feedback, log or performance-observer calls. Malformed decoder tables/indices/stack overlaps refuse before mutation; rumble mismatch stays pending and gated.");
    return 0;
}
