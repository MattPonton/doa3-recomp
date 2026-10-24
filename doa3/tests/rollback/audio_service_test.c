#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_audio_service.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define BYTES (16u*1024u*1024u)
enum{ENTER=0x100001,LEAVE=0x100002,RAISE=0x100003,LOWER=0x100004,AUDIO_CALLBACK=0x100005};
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
volatile int g_xbox_det_active=1;
static uint32_t epoch=1;
static int primary=1;
static unsigned char *ram;
static unsigned events,callbacks,reference_count;
static unsigned callback_irql;
/* This older control-flow fixture deliberately admits no real bridges. */
int xbox_audio_sync_call(XboxAudioSync *s,uint32_t target,uint32_t ordinal,uint32_t sp){
    (void)s;(void)target;(void)ordinal;(void)sp;return 0;
}
uint32_t xbox_det_epoch(void){return epoch;}
int xbox_fiber_is_primary(void){return primary;}
void *xbox_GetMemoryBase(void){return ram;}
size_t xbox_GetMemorySize(void){return BYTES;}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *v){(void)p;(void)v;CHECK(0);return 0;}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v){(void)p;(void)v;CHECK(0);return 0;}
DWORD xbox_InputHostMask(void){CHECK(0);return 0;}
DWORD xbox_InputTakeReconfiguredMask(void){CHECK(0);return 0;}
BYTE xbox_InputGuestSubtype(int p){(void)p;CHECK(0);return 0;}
typedef struct{uint32_t r[8];double fp[8];int top;} Registers;
static Registers regs(void){Registers r={0};uint32_t words[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};memcpy(r.r,words,sizeof words);memcpy(r.fp,g_fp_stack,sizeof r.fp);r.top=g_fp_top;return r;}
static void load(Registers r){g_eax=r.r[0];g_ecx=r.r[1];g_edx=r.r[2];g_esp=r.r[3];g_ebx=r.r[4];g_esi=r.r[5];g_edi=r.r[6];g_seh_ebp=r.r[7];memcpy(g_fp_stack,r.fp,sizeof r.fp);g_fp_top=r.top;}
static uint64_t hash(void){uint64_t h=14695981039346656037ull;for(size_t i=0;i<BYTES;++i)h=(h^ram[i])*1099511628211ull;return h;}
typedef struct{uint32_t target,sp;Registers registers;uint64_t memory;} Event;
static Event expected[256];
static void external(uint32_t target){
    ++events;CHECK(MEM32(g_esp)==0);
    switch(target){
    case ENTER:CHECK(MEM32(g_esp+4)==0x20BE58);++MEM32(0x200);g_eax=0;g_esp+=8;break;
    case LEAVE:CHECK(MEM32(g_esp+4)==0x20BE58 && MEM32(0x200));--MEM32(0x200);g_eax=0;g_esp+=8;break;
    case RAISE:g_eax=MEM8(0x24);MEM8(0x24)=2;g_esp+=4;break;
    case LOWER:MEM8(0x24)=LO8(g_ecx);g_esp+=4;break;
    case AUDIO_CALLBACK:{
        CHECK(g_ecx>=0x330000 && g_ecx<0x331000 && MEM8(0x24)==callback_irql);
        unsigned index=(g_ecx-0x330000)/0x100;
        CHECK(MEM32(g_esp+4)==10+index && MEM32(g_esp+8)==20+index);
        ++callbacks;MEM32(0x204)+=index+1;g_eax=0xAB00+index;g_esp+=12;break;
    }
    default:CHECK(0);
    }
}
static void reference_external(uint32_t target,uint32_t sp){
    CHECK(reference_count<256);expected[reference_count++]=(Event){target,sp,regs(),hash()};external(target);CHECK(g_esp==sp);
}
#include "audio_service_reference.generated.inc"
static Doa3LoopState pending(void){
    Doa3LoopState loop;doa3_loop_init(&loop);g_esp=0x100000;
    for(unsigned i=0;i<DOA3_LOOP_MAX_CALLS;++i){
        Doa3LoopAction a;CHECK(doa3_loop_next(&loop,&a));CHECK(a.kind==DOA3_LOOP_CALL);
        if(a.routine==0x1F1F01)return loop;
        g_esp+=4+(a.routine==0x1DF010 ? 24:a.routine==0x1E14D0 ? 4:0);CHECK(doa3_loop_complete(&loop,a.routine));
    }
    CHECK(0);return loop;
}
static void seed(unsigned mode,unsigned nodes){
    callback_irql=mode==1 ? 2:0;
    memset(ram,0,BYTES);MEM32(0x217AC8)=ENTER;MEM32(0x217AC4)=LEAVE;
    MEM32(0x217B70)=RAISE;MEM32(0x217AFC)=LOWER;
    MEM8(0x24)=mode==1 ? 2:0;MEM32(0x20BE54)=mode==2;
    MEM32(0x20C4F8)=mode==3 ? 0:0x300000;MEM32(0x30000C)=0x310000;
    uint32_t head=0x3106EC;MEM32(head)=nodes ? 0x320000:head;MEM32(head+4)=nodes ? 0x320000+(nodes-1)*0x100:head;
    for(unsigned i=0;i<nodes;++i){
        uint32_t node=0x320000+i*0x100;
        MEM32(node)=i+1<nodes ? node+0x100:head;MEM32(node+4)=i ? node-0x100:head;
        MEM32(node+8)=i%2 ? 6:2;MEM32(node+12)=0x330000+i*0x100;
        MEM32(node+16)=10+i;MEM32(node+20)=20+i;MEM32(0x330000+i*0x100)=0x340000;
    }
    MEM32(0x340018)=AUDIO_CALLBACK;
}
static void refuse(Doa3AudioService *s,Doa3LoopState *loop,Doa3InputTape *t){
    Doa3AudioService before=*s;Doa3LoopState original=*loop;Registers a=regs();uint64_t memory=hash();unsigned count=events;
    Doa3AudioServiceCall call={0};CHECK(!doa3_audio_service_next(s,loop,t,&call));Registers b=regs();
    CHECK(!memcmp(s,&before,sizeof before) && !memcmp(loop,&original,sizeof original) && !memcmp(&a,&b,sizeof a) && hash()==memory && events==count);
}
static int advance(Doa3AudioService *s,Doa3LoopState *loop,Doa3InputTape *t,unsigned *position,int compare){
    for(unsigned i=0;i<256;++i){
        Doa3AudioServiceCall call;unsigned count=events;int result=doa3_audio_service_next(s,loop,t,&call);
        CHECK(events==count);if(result!=1)return result;
        CHECK(loop->pending==0x1F1F01);refuse(s,loop,t);
        Doa3AudioService before=*s;CHECK(!doa3_audio_service_complete(s,t,call.target+10));CHECK(!memcmp(s,&before,sizeof before));
        if(compare){CHECK(*position<reference_count);Event *e=&expected[*position];Registers now=regs();CHECK(call.target==e->target && call.expected_sp==e->sp && hash()==e->memory && !memcmp(&now,&e->registers,sizeof now));}
        ++*position;external(call.target);CHECK(doa3_audio_service_complete(s,t,call.target)==1);
    }
    CHECK(0);return 0;
}
int main(void){
    ram=guest_memory_fixture_alloc(BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    unsigned char *initial=malloc(BYTES),*final=malloc(BYTES),*mid=malloc(BYTES);CHECK(initial && final && mid);
    Doa3InputTape *t=doa3_input_tape_create(1);CHECK(t);CHECK(doa3_input_tape_record(t,0));CHECK(doa3_input_tape_end(t));
    CHECK(doa3_input_tape_replay(t,0));
    for(unsigned mode=0;mode<5;++mode){
        unsigned nodes=mode==0 ? 6:mode==1 ? 3:0;
        seed(mode,nodes);Doa3LoopState loop=pending(),start_loop=loop;Registers start=regs();memcpy(initial,ram,BYTES);
        events=callbacks=reference_count=0;reference_audio_1F1F01();Registers end=regs();memcpy(final,ram,BYTES);
        CHECK(!MEM32(0x200));unsigned expected_callbacks=callbacks;CHECK(expected_callbacks==nodes);
        for(unsigned pass=0;pass<3;++pass){
            memcpy(ram,initial,BYTES);load(start);loop=start_loop;events=callbacks=0;
            Doa3AudioService service;CHECK(doa3_audio_service_begin(&service,&loop,t));
            if(!pass){
                refuse(&service,&loop,NULL);primary=0;refuse(&service,&loop,t);primary=1;
                ++epoch;refuse(&service,&loop,t);--epoch;
                ++loop.intervals;refuse(&service,&loop,t);--loop.intervals;
                Doa3AudioService valid=service;
                service.depth=DOA3_AUDIO_SERVICE_DEPTH+1;refuse(&service,&loop,t);service=valid;
                service.frames[0].resume=999;refuse(&service,&loop,t);service=valid;
                service.expected_sp=4;refuse(&service,&loop,t);service=valid;
            }
            unsigned position=0;
            if(mode==0){
                Doa3AudioServiceCall call;
                for(unsigned i=0;;++i){
                    CHECK(i<256 && doa3_audio_service_next(&service,&loop,t,&call)==1);
                    Registers now=regs();Event *e=&expected[position];
                    CHECK(call.target==e->target && call.expected_sp==e->sp && hash()==e->memory && !memcmp(&now,&e->registers,sizeof now));
                    if(call.target==AUDIO_CALLBACK)break;
                    external(call.target);CHECK(doa3_audio_service_complete(&service,t,call.target)==1);++position;
                }
                memcpy(mid,ram,BYTES);Registers saved=regs();Doa3AudioService checkpoint=service;
                unsigned saved_position=position,saved_events=events,saved_callbacks=callbacks;
                external(call.target);CHECK(doa3_audio_service_complete(&service,t,call.target)==1);
                ++position;CHECK(advance(&service,&loop,t,&position,1)==2);
                /* Restore across the pending external call, including every
                 * surviving guest local and private continuation frame. */
                memcpy(ram,mid,BYTES);load(saved);service=checkpoint;loop=start_loop;
                events=saved_events;callbacks=saved_callbacks;position=saved_position;
                CHECK(!doa3_audio_service_complete(&service,t,call.target+10));
                external(call.target);CHECK(doa3_audio_service_complete(&service,t,call.target)==1);++position;
            }
            CHECK(advance(&service,&loop,t,&position,1)==2);Registers now=regs();
            CHECK(position==reference_count && callbacks==expected_callbacks && !loop.pending && service.done);
            CHECK(!memcmp(&now,&end,sizeof now) && !memcmp(ram,final,BYTES));
        }
    }
    /* Real corrupted lists stay within memory but would never finish natively. */
    seed(0,1);MEM32(0x320000)=0x320000;MEM32(0x320004)=0x320000;
    Doa3LoopState loop=pending();Doa3AudioService service;CHECK(doa3_audio_service_begin(&service,&loop,t));unsigned position=0;
    CHECK(advance(&service,&loop,t,&position,0)==-1 && service.fault==DOA3_AUDIO_SERVICE_EXHAUSTED && loop.pending);
    Doa3AudioServiceCall call;CHECK(doa3_audio_service_next(&service,&loop,t,&call)==-1);
    seed(0,1);MEM32(0x30000C)=BYTES;loop=pending();CHECK(doa3_audio_service_begin(&service,&loop,t));position=0;
    CHECK(advance(&service,&loop,t,&position,0)==-1 && service.fault==DOA3_AUDIO_SERVICE_MEMORY && loop.pending);
    seed(0,1);loop=pending();CHECK(doa3_audio_service_begin(&service,&loop,t));CHECK(doa3_audio_service_next(&service,&loop,t,&call)==1);
    CHECK(doa3_audio_service_complete(&service,t,call.target)==-1 && service.fault==DOA3_AUDIO_SERVICE_ABI && service.pending && loop.pending);
    CHECK(doa3_input_tape_abort(t));CHECK(doa3_input_tape_destroy(t));
    free(initial);free(final);free(mid);guest_memory_fixture_free(ram);
    puts("Audio service: real eight-function queue flow matches native RAM/registers at external calls and return, with pending-call restore, empty/disabled/no-device/IRQL paths, AUDIO_CALLBACK retain/remove, bounded corrupt-list failure, checked memory and ABI refusal. External calls are fixtures, never automatic dispatch.");return 0;
}
