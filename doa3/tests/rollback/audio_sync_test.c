/* Actual lifted queue/callback router and HAL/RTL/bridge bodies, production
 * clock + replay guard. Voice effects and outer-loop peers are fixtures. */
#include <windows.h>
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_audio_service.h"
#include "kernel/kernel_replay.h"
#include "kernel/xbox_memory_layout.h"
#include "online/xbox_det.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define BYTES (16u*1024u*1024u)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
volatile int g_fib_slice_due;
static unsigned char *ram;
static uint32_t tick;
static int fiber;
static unsigned voice_calls,events;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)((uintptr_t)&tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));}
void *xbox_GetMemoryBase(void){return ram;}
size_t xbox_GetMemorySize(void){return BYTES;}
int xbox_fiber_current(void){return fiber;}
int xbox_fiber_is_primary(void){return !fiber;}
void xbox_fiber_wake(uint32_t key){CHECK(key==XBOX_DET_VBLANK_KEY);}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *v){(void)p;(void)v;CHECK(0);return 0;}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v){(void)p;(void)v;CHECK(0);return 0;}
DWORD xbox_InputHostMask(void){CHECK(0);return 0;}
DWORD xbox_InputTakeReconfiguredMask(void){CHECK(0);return 0;}
BYTE xbox_InputGuestSubtype(int p){(void)p;CHECK(0);return 0;}
typedef UCHAR KIRQL;
typedef void (*bridge_func_t)(void);
#define XBOX_LOG_WARN 1
#define XBOX_LOG_HAL 2
#define xbox_log(...) CHECK(0)
#define KERNEL_VA_BASE 0xFE000000u
#define XBOX_KERNEL_THUNK_TABLE_SIZE 4
#define KERNEL_VA_END (KERNEL_VA_BASE+16)
#define BRIDGE_MEM32(a) MEM32(a)
#define STACK_ARG(n) MEM32(g_esp+(n)*4)
#define XBOX_TO_NATIVE(a) ((a)?(void *)(ram+(a)):NULL)
static ULONG g_slot_ordinals[4]={277,294,160,161};
static int g_slot_arg_bytes[4]={4,4,0,0};
#include "audio_sync_endpoints.generated.inc"
static bridge_func_t g_slot_bridges[4]={bridge_RtlEnterCriticalSection,bridge_RtlLeaveCriticalSection,bridge_KfRaiseIrql,bridge_KfLowerIrql};
#include "kernel/kernel_audio_sync.inc"
typedef struct{uint32_t r[8];double fp[8];int top;} Registers;
static Registers regs(void){Registers r={0};uint32_t words[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};memcpy(r.r,words,sizeof words);memcpy(r.fp,g_fp_stack,sizeof r.fp);r.top=g_fp_top;return r;}
static void load(Registers r){g_eax=r.r[0];g_ecx=r.r[1];g_edx=r.r[2];g_esp=r.r[3];g_ebx=r.r[4];g_esi=r.r[5];g_edi=r.r[6];g_seh_ebp=r.r[7];memcpy(g_fp_stack,r.fp,sizeof r.fp);g_fp_top=r.top;}
static uint64_t hash(void){uint64_t h=14695981039346656037ull;for(size_t i=0;i<BYTES;++i)h=(h^ram[i])*1099511628211ull;return h;}
static void voice(uint32_t target){
    CHECK(xbox_current_irql()==0 && MEM8(0x24)==0 && g_ecx==0x330000);
    unsigned args=target==0x1F64A8 || target==0x1F66FE ? 4:0;
    CHECK(args || target==0x1F6403 || target==0x1F659E);
    if(args)CHECK(MEM32(g_esp+4)==99);
    ++voice_calls;MEM32(0x200)=MEM32(0x200)*7+target;g_eax=target;g_esp+=4+args;
}
static void reference_audio_1F688C(void);
static void reference_external(uint32_t target,uint32_t sp){
    ++events;
    if(target>=KERNEL_VA_BASE && target<KERNEL_VA_END){
        unsigned slot=(target-KERNEL_VA_BASE)/4;
        xbox_det_on_kernel_call(g_slot_ordinals[slot]);g_esp+=4;
        g_slot_bridges[slot]();g_esp+=g_slot_arg_bytes[slot];
    }else if(target==0x1F688C)reference_audio_1F688C();
    else voice(target);
    CHECK(g_esp==sp);
}
#include "audio_service_reference.generated.inc"
static Doa3LoopState pending(void){
    Doa3LoopState loop;doa3_loop_init(&loop);g_esp=0x100000;
    for(unsigned i=0;i<DOA3_LOOP_MAX_CALLS;++i){
        Doa3LoopAction a;CHECK(doa3_loop_next(&loop,&a));CHECK(a.kind==DOA3_LOOP_CALL);
        if(a.routine==0x1F1F01)return loop;
        g_esp+=4+(a.routine==0x1DF010 ? 24:a.routine==0x1E14D0 ? 4:0);CHECK(doa3_loop_complete(&loop,a.routine));
    }CHECK(0);return loop;
}
static void seed(void){
    memset(ram,0,BYTES);xbox_set_irql(0);
    MEM32(0x217AC8)=KERNEL_VA_BASE;MEM32(0x217AC4)=KERNEL_VA_BASE+4;
    MEM32(0x217B70)=KERNEL_VA_BASE+8;MEM32(0x217AFC)=KERNEL_VA_BASE+12;
    MEM32(0x20C4F8)=0x300000;MEM32(0x30000C)=0x310000;
    uint32_t head=0x3106EC;MEM32(head)=0x320000;MEM32(head+4)=0x320400;
    for(unsigned i=0;i<5;++i){
        uint32_t node=0x320000+i*0x100;
        MEM32(node)=i<4 ? node+0x100:head;MEM32(node+4)=i ? node-0x100:head;
        MEM32(node+8)=i%2 ? 6:2;MEM32(node+12)=0x330000;
        MEM32(node+16)=i;MEM32(node+20)=99;
    }
    MEM32(0x330000)=0x21811C;MEM32(0x218134)=0x1F688C;
}
static void no_sync(Doa3AudioService *s,Doa3InputTape *t,XboxAudioSync *scope){
    Registers a=regs();Doa3AudioService before=*s;XboxAudioSync sync=*scope;
    uint64_t memory=hash();unsigned calls=xbox_det_kcalls();KIRQL level=xbox_current_irql();
    CHECK(!doa3_audio_service_sync(s,t,scope));Registers b=regs();
    CHECK(!memcmp(&a,&b,sizeof a) && !memcmp(s,&before,sizeof before) && !memcmp(scope,&sync,sizeof sync));
    CHECK(hash()==memory && calls==xbox_det_kcalls() && level==xbox_current_irql());
}
static DWORD WINAPI foreign(void *p){CHECK(!xbox_audio_sync_quiesce(p));CHECK(!xbox_audio_sync_finish(p,1));return 0;}
int main(void){
    ram=guest_memory_fixture_alloc(BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    unsigned char *initial=malloc(BYTES),*final=malloc(BYTES);CHECK(initial && final);
    XboxAudioSync scope={0};CHECK(!xbox_audio_sync_begin(&scope));
    xbox_det_bases base={1,2,3,4};xbox_det_begin(&base);
    Doa3InputTape *t=doa3_input_tape_create(1);CHECK(t && doa3_input_tape_record(t,0) && doa3_input_tape_end(t) && doa3_input_tape_replay(t,0));
    seed();Doa3LoopState start_loop=pending();Registers start=regs();memcpy(initial,ram,BYTES);
    xbox_det_checkpoint clock0,clock1,clock2;CHECK(xbox_det_capture(&clock0));
    reference_audio_1F1F01();CHECK(voice_calls==4 && !xbox_current_irql());
    Registers end=regs();memcpy(final,ram,BYTES);CHECK(xbox_det_capture(&clock1));unsigned total=events;
    uint64_t old_pin=0;
    /* A full replay, then cancel before EVERY request, including while host
     * IRQL is raised. Recovery restores the complete fixture RAM/regs/clock. */
    for(unsigned stop=0;stop<=total;++stop){
        memcpy(ram,initial,BYTES);load(start);CHECK(xbox_det_restore(&clock0));
        Doa3LoopState loop=start_loop;Doa3AudioService s;CHECK(doa3_audio_service_begin(&s,&loop,t));
        CHECK(xbox_kernel_replay_begin() && xbox_audio_sync_begin(&scope));CHECK(!xbox_audio_sync_begin(&scope));
        CHECK(!xbox_kernel_replay_end(0) && !xbox_kernel_replay_end(1));
        XboxAudioSync other={0};CHECK(!xbox_audio_sync_begin(&other));
        CHECK(scope.guard_pin!=old_pin && !xbox_kernel_replay_pinned(old_pin));old_pin=scope.guard_pin;
        voice_calls=0;unsigned count=0;
        for(;;){
            Doa3AudioServiceCall call;int result=doa3_audio_service_next(&s,&loop,t,&call);
            if(result==2){Registers now=regs();CHECK(voice_calls==4 && !memcmp(&now,&end,sizeof end) && !memcmp(ram,final,BYTES));CHECK(xbox_det_capture(&clock2) && !memcmp(&clock1,&clock2,sizeof clock1));CHECK(xbox_audio_sync_finish(&scope,0) && xbox_kernel_replay_end(0));break;}
            CHECK(result==1);
            if(stop && ++count==stop){
                fiber=1;CHECK(!xbox_audio_sync_quiesce(&scope));fiber=0;
                HANDLE h=CreateThread(NULL,0,foreign,&scope,0,NULL);CHECK(h && WaitForSingleObject(h,5000)==WAIT_OBJECT_0);CloseHandle(h);
                CHECK(!xbox_kernel_replay_check(999,0));no_sync(&s,t,&scope);
                CHECK(!xbox_audio_sync_finish(&scope,0));CHECK(xbox_audio_sync_quiesce(&scope));
                CHECK(!xbox_current_irql() && MEM8(0x24)==0 && !xbox_kernel_replay_healthy());
                no_sync(&s,t,&scope);
                memcpy(ram,initial,BYTES);load(start);CHECK(xbox_det_restore(&clock0));
                CHECK(xbox_audio_sync_finish(&scope,1) && xbox_kernel_replay_end(1));break;
            }
            if(call.target>=KERNEL_VA_BASE){
                unsigned slot=(call.target-KERNEL_VA_BASE)/4;
                if(!stop){
                no_sync(&s,NULL,&scope);fiber=1;no_sync(&s,t,&scope);fiber=0;
                ++g_slot_arg_bytes[slot];no_sync(&s,t,&scope);--g_slot_arg_bytes[slot];
                bridge_func_t fn=g_slot_bridges[slot];g_slot_bridges[slot]=NULL;no_sync(&s,t,&scope);g_slot_bridges[slot]=fn;
                ++g_slot_ordinals[slot];no_sync(&s,t,&scope);--g_slot_ordinals[slot];
                ++s.pending;++s.frames[s.depth-1].call_target;no_sync(&s,t,&scope);--s.pending;--s.frames[s.depth-1].call_target;
                uint32_t saved_sp=g_esp;g_esp=BYTES;no_sync(&s,t,&scope);g_esp=saved_sp+1;no_sync(&s,t,&scope);g_esp=saved_sp;
                unsigned mirror=MEM8(0x24);MEM8(0x24)=7;no_sync(&s,t,&scope);MEM8(0x24)=(unsigned char)mirror;
                uint32_t iat=slot==0 ? 0x217AC8:slot==1 ? 0x217AC4:slot==2 ? 0x217B70:0x217AFC;
                uint32_t saved_iat=MEM32(iat);MEM32(iat)=1;no_sync(&s,t,&scope);MEM32(iat)=saved_iat;
                if(slot<2){uint32_t arg=MEM32(g_esp+4);MEM32(g_esp+4)=0;no_sync(&s,t,&scope);MEM32(g_esp+4)=arg;}
                else{uint32_t arg=g_ecx;g_ecx=7;no_sync(&s,t,&scope);g_ecx=arg;}
                }
                CHECK(doa3_audio_service_sync(&s,t,&scope)==1);
            }else if(call.target==0x1F688C){
                if(!stop)no_sync(&s,t,&scope);Doa3AudioService before=s;Registers a=regs();
                MEM32(0x218134)=1;CHECK(!doa3_audio_service_callback(&s,t));MEM32(0x218134)=0x1F688C;
                Registers b=regs();CHECK(!memcmp(&before,&s,sizeof s) && !memcmp(&a,&b,sizeof a));
                CHECK(doa3_audio_service_callback(&s,t));CHECK(!doa3_audio_service_callback(&s,t));
            }else{if(!stop)no_sync(&s,t,&scope);CHECK(!doa3_audio_service_callback(&s,t));voice(call.target);CHECK(doa3_audio_service_complete(&s,t,call.target)==1);}
        }
    }
    CHECK(doa3_input_tape_end(t));doa3_input_tape_destroy(t);xbox_det_end();
    free(initial);free(final);guest_memory_fixture_free(ram);
    puts("Actual audio synchronization, callback dispatch, full replay and every-request cancellation passed");return 0;
}
