/* Actual lifted CRI stop/status/deadline loop and XAPI RDTSC helper, with
 * production deterministic clock. Voice endpoints are controlled fixtures. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "online/xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp,g_str_a,g_str_b,g_flg_a,g_flg_b;
double g_fp_stack[8];int g_fp_top,g_flg_w,g_flg_test;uint16_t g_x87_cw;
volatile int g_fib_slice_due;
static uint32_t exported_tick;
static int fiber=2;
static unsigned polls,stop_after;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)((uintptr_t)&exported_tick-(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT));}
int xbox_fiber_current(void){return fiber;}
void xbox_fiber_wake(uint32_t key){(void)key;CHECK(0);}
uint64_t doa3_guest_rdtsc(void){return xbox_det_rdtsc();}
void sub_001F1E1D(void){CHECK(MEM32(esp+4)==0x20000);eax=0;esp+=8;}
void sub_001F1E79(void){
    CHECK(MEM32(esp+4)==0x20000);CHECK(++polls<2000);
    MEM32(MEM32(esp+8))=stop_after && polls>=stop_after ? 0:1;
    eax=0;esp+=12;
}
void sub_0019C1E0(void){CHECK(0);}
#include "audio_wait.generated.inc"
static void run(void){
    g_esp=0x30000;g_esi=0xBEEF;MEM32(g_esp+4)=0x10000;
    MEM32(0x10004)=1;MEM32(0x10008)=0x20000;
    MEM32(0xC75620)=73333333;MEM32(0xC75624)=0;polls=0;
    sub_00199DB0();CHECK(g_esp==0x30004 && g_esi==0xBEEF);
}
static DWORD WINAPI foreign(void *v){(void)v;CHECK(!xbox_det_audio_wait_poll());return 0;}
int main(void){
    unsigned char *ram=guest_memory_fixture_alloc(16u*1024u*1024u);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    CHECK(!xbox_det_audio_wait_poll());
    xbox_det_bases bases={1,2,3,100};xbox_det_checkpoint start,expected,actual;
    fiber=0;xbox_det_begin(&bases);uint64_t primary_clock=xbox_det_rdtsc();
    CHECK(xbox_det_checkpoint_acquire());CHECK(!xbox_det_audio_wait_poll());CHECK(xbox_det_checkpoint_release());
    HANDLE thread=CreateThread(NULL,0,foreign,NULL,0,NULL);CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);CloseHandle(thread);
    fiber=2;CHECK(xbox_det_capture(&start));run();
    unsigned count=polls;uint64_t end=xbox_det_rdtsc();
    CHECK(count>900 && count<1100 && end-bases.rdtsc>73333333 && xbox_det_frames()==0);
    CHECK(xbox_det_capture(&expected));
    for(unsigned i=0;i<3;++i){
        CHECK(xbox_det_restore(&start));run();CHECK(polls==count && xbox_det_rdtsc()==end);
        CHECK(xbox_det_capture(&actual) && !memcmp(&expected,&actual,sizeof(actual)));
    }
    fiber=0;CHECK(xbox_det_rdtsc()==primary_clock && !xbox_det_boost());
    fiber=2;CHECK(xbox_det_restore(&start));stop_after=3;run();CHECK(polls==3);
    CHECK(xbox_det_rdtsc()-bases.rdtsc<73333333); /* native status exit still wins */
    fiber=0;xbox_det_end();guest_memory_fixture_free(ram);
    puts("Native CRI RDTSC wait: bounded timeout, exact restored timing, early voice stop and fiber isolation passed");return 0;
}
