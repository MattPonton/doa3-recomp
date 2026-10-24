/* Actual lifted timestamp helpers + production deterministic clock. Other
 * outer-loop helpers, worker scheduler and hardware controllers are fixtures. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static unsigned host_clock_reads;
static BOOL counted_counter(LARGE_INTEGER *v){++host_clock_reads;return QueryPerformanceCounter(v);}
static BOOL counted_frequency(LARGE_INTEGER *v){++host_clock_reads;return QueryPerformanceFrequency(v);}
#define QueryPerformanceCounter counted_counter
#define QueryPerformanceFrequency counted_frequency
#include "../../src/online/xbox_det.c"
#undef QueryPerformanceCounter
#undef QueryPerformanceFrequency
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop_time.h"
#include "guest_memory_fixture.h"
#include "loop_time_reference.generated.inc"
#define BYTES (16u*1024u*1024u)
#define FRAMES 72
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
volatile int g_fib_slice_due;
static unsigned char *ram;
static uint32_t tick;
static int fiber,wakes;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)((uintptr_t)&tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));}
void *xbox_GetMemoryBase(void){return ram;}
size_t xbox_GetMemorySize(void){return BYTES;}
int xbox_fiber_current(void){return fiber;}
int xbox_fiber_is_primary(void){return fiber==0;}
void xbox_fiber_wake(uint32_t event){CHECK(event==XBOX_DET_VBLANK_KEY);++wakes;}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *out){(void)p;(void)out;CHECK(0);return 0;}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v){(void)p;(void)v;CHECK(0);return 0;}
DWORD xbox_InputHostMask(void){CHECK(0);return 0;}
DWORD xbox_InputTakeReconfiguredMask(void){CHECK(0);return 0;}
BYTE xbox_InputGuestSubtype(int p){(void)p;CHECK(0);return 0;}
typedef struct{uint32_t r[8];double fp[8];int top;} Registers;
static Registers regs(void){
    Registers r={0};uint32_t words[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp};
    memcpy(r.r,words,sizeof words);memcpy(r.fp,g_fp_stack,sizeof r.fp);r.top=g_fp_top;return r;
}
static void load(Registers r){
    g_eax=r.r[0];g_ecx=r.r[1];g_edx=r.r[2];g_esp=r.r[3];g_ebx=r.r[4];g_esi=r.r[5];g_edi=r.r[6];g_seh_ebp=r.r[7];
    memcpy(g_fp_stack,r.fp,sizeof r.fp);g_fp_top=r.top;
}
static uint64_t hash(void){uint64_t h=14695981039346656037ull;for(size_t i=0;i<BYTES;++i)h=(h^ram[i])*1099511628211ull;return h;}
static void refused(Doa3LoopState *s,Doa3InputTape *t){
    Registers a=regs();Doa3LoopState state=*s;xbox_det_checkpoint before,after;
    CHECK(xbox_det_capture(&before));uint64_t memory=hash();unsigned host=host_clock_reads;int wake=wakes;
    CHECK(!doa3_loop_time_run(s,t));Registers b=regs();CHECK(xbox_det_capture(&after));
    CHECK(!memcmp(&a,&b,sizeof a) && !memcmp(&state,s,sizeof state) && hash()==memory);
    CHECK(!memcmp(&before,&after,sizeof before) && host_clock_reads==host && wakes==wake);
}
static DWORD WINAPI wrong_thread(void *p){CHECK(!doa3_loop_time_run(((void **)p)[0],((void **)p)[1]));return 0;}
static void guards(Doa3LoopState *s,Doa3InputTape *t){
    refused(s,NULL);fiber=1;refused(s,t);fiber=0;
    void *args[]={s,t};HANDLE h=CreateThread(NULL,0,wrong_thread,args,0,NULL);CHECK(h);
    CHECK(WaitForSingleObject(h,5000)==WAIT_OBJECT_0);CloseHandle(h);
    CHECK(xbox_det_checkpoint_acquire());refused(s,t);CHECK(xbox_det_checkpoint_release());
    uint32_t sp=g_esp;const uint32_t bad[]={4,BYTES,0x100001,0x9B2340,0x9B23C4,0x250EF4,0x2511DC};
    for(unsigned i=0;i<sizeof bad/sizeof bad[0];++i){g_esp=bad[i];refused(s,t);}g_esp=sp;
    int top=g_fp_top;g_fp_top=INT_MAX;refused(s,t);g_fp_top=INT_MIN;refused(s,t);g_fp_top=top;
    if(s->pending==0xA6680){uint32_t index=MEM32(0x9B238C);MEM32(0x9B238C)=8;refused(s,t);MEM32(0x9B238C)=UINT32_MAX;refused(s,t);MEM32(0x9B238C)=index;}
}
static void interval(Doa3LoopState *s,Doa3InputTape *t,int replay,int check){
    for(unsigned i=0;i<DOA3_LOOP_MAX_CALLS;++i){
        Doa3LoopAction a;CHECK(doa3_loop_next(s,&a));if(a.kind==DOA3_LOOP_BOUNDARY)return;
        if(a.routine==0xA65B0 || a.routine==0xA6680){
            xbox_det_checkpoint before,after;CHECK(xbox_det_capture(&before));int wake=wakes;unsigned host=host_clock_reads;
            if(replay){if(check)guards(s,t);CHECK(doa3_loop_time_run(s,t)==1);}
            else{refused(s,t);if(a.routine==0xA65B0)reference_time_000A65B0();else reference_time_000A6680();CHECK(doa3_loop_complete(s,a.routine));}
            CHECK(xbox_det_capture(&after));CHECK(!memcmp(&before,&after,sizeof before));
            CHECK(host_clock_reads==host && wakes==wake);
        }else{
            if(check)refused(s,t);
            g_eax=g_eax*7+a.routine;g_esp+=4+(a.routine==0x1DF010 ? 24:a.routine==0x1E14D0 ? 4:0);
            CHECK(doa3_loop_complete(s,a.routine));
        }
    }
    CHECK(0);
}
static void prepare(unsigned frame){
    xbox_det_frame();
    if(frame%9==3)xbox_det_add_qpc_boost(733333);
    fiber=2;xbox_det_add_qpc_boost(900);fiber=0;
    MEM32(0x9B23AC)=frame==1 ? UINT32_MAX:frame==2 ? 1:0;
    MEM32(0x9B23A8)=frame==0 ? 0:733333333;
}
int main(void){
    ram=guest_memory_fixture_alloc(BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    unsigned char *initial=malloc(BYTES),*final=malloc(BYTES);CHECK(initial && final);
    uint64_t host0=xbox_det_rdtsc();Sleep(2);CHECK(xbox_det_rdtsc()>=host0);
    xbox_det_bases base;xbox_det_current_bases(&base);
    base.rdtsc=UINT64_C(0xFFFFFFF0);xbox_det_begin(&base);
    CHECK(xbox_det_rdtsc()==base.rdtsc);unsigned host=host_clock_reads;
    xbox_det_checkpoint clock0;CHECK(xbox_det_capture(&clock0));
    for(unsigned i=1;i<=60;++i){xbox_det_frame();CHECK(xbox_det_rdtsc()==base.rdtsc+(UINT64_C(733333333)*i)/60);}
    CHECK(host_clock_reads==host);CHECK(xbox_det_restore(&clock0));
    LARGE_INTEGER frequency;CHECK(QueryPerformanceFrequency(&frequency));
    xbox_det_add_qpc_boost((uint64_t)frequency.QuadPart);
    CHECK(xbox_det_rdtsc()==base.rdtsc+UINT64_C(733333333));
    fiber=2;CHECK(xbox_det_rdtsc()==base.rdtsc);
    xbox_det_add_qpc_boost((uint64_t)frequency.QuadPart*2);
    CHECK(xbox_det_rdtsc()==base.rdtsc+UINT64_C(1466666666));fiber=0;
    CHECK(xbox_det_rdtsc()==base.rdtsc+UINT64_C(733333333));
    xbox_det_bases current;xbox_det_current_bases(&current);CHECK(current.rdtsc==xbox_det_rdtsc());
    CHECK(xbox_det_restore(&clock0) && xbox_det_rdtsc()==base.rdtsc);
    g_esp=0x100000;g_fp_top=3;MEMF(0x250EF4)=1.0f;MEMF(0x2511DC)=1000.0f;
    MEM32(0x9B2350)=(uint32_t)base.rdtsc;MEM32(0x9B2354)=(uint32_t)(base.rdtsc>>32);
    MEM32(0x9B2398)=(uint32_t)base.rdtsc;MEM32(0x9B239C)=(uint32_t)(base.rdtsc>>32);
    Registers start=regs(),expected_regs[FRAMES];Doa3LoopState s,expected_s[FRAMES];
    uint64_t expected_hash[FRAMES],expected_cpu[FRAMES];xbox_det_checkpoint expected_clock[FRAMES];
    memcpy(initial,ram,BYTES);doa3_loop_init(&s);
    Doa3InputTape *t=doa3_input_tape_create(FRAMES);CHECK(t);
    for(unsigned i=0;i<FRAMES;++i){
        prepare(i);CHECK(doa3_input_tape_record(t,i));interval(&s,t,0,0);CHECK(doa3_input_tape_end(t));
        expected_hash[i]=hash();expected_regs[i]=regs();expected_s[i]=s;expected_cpu[i]=xbox_det_rdtsc();
        CHECK(xbox_det_capture(&expected_clock[i]));CHECK(g_esp==start.r[3] && g_fp_top==start.top);
    }
    memcpy(final,ram,BYTES);Sleep(3);
    for(unsigned pass=0;pass<2;++pass){
        memcpy(ram,initial,BYTES);load(start);doa3_loop_init(&s);CHECK(xbox_det_restore(&clock0));
        for(unsigned i=0;i<FRAMES;++i){
            prepare(i);CHECK(doa3_input_tape_replay(t,i));interval(&s,t,1,pass==0 && i==0);CHECK(doa3_input_tape_end(t));
            Registers now=regs();xbox_det_checkpoint clock;CHECK(xbox_det_capture(&clock));
            CHECK(hash()==expected_hash[i] && !memcmp(&now,&expected_regs[i],sizeof now) && !memcmp(&s,&expected_s[i],sizeof s));
            CHECK(!memcmp(&clock,&expected_clock[i],sizeof clock) && xbox_det_rdtsc()==expected_cpu[i]);
        }
        CHECK(!memcmp(final,ram,BYTES));
    }
    CHECK(host_clock_reads==host);uint64_t end=xbox_det_rdtsc();xbox_det_end();CHECK(xbox_det_rdtsc()>=end);
    xbox_det_current_bases(&base);xbox_det_begin(&base);CHECK(xbox_det_rdtsc()==base.rdtsc);xbox_det_end();
    CHECK(doa3_input_tape_destroy(t));free(initial);free(final);guest_memory_fixture_free(ram);
    puts("Actual timing helpers: two 72-interval replays match RAM/registers/x87/clock/loop. Zero host-clock reads or worker yields in helpers; 60 Hz CPU cycles, low-word rollover, repeat restore and guarded rejection pass.");
    return 0;
}
