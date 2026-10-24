#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static uint32_t eax=1,ecx=2,edx=3,ebx=4,esi=5,edi=6,esp=100,g_seh_ebp=7;
static int replay,flushed,irq,history,flip,present,pace,capture,monitor,trace,adx,profile,callback;
unsigned g_doa3_frames_presented;
#define PUSH32(sp,value) ((sp)-=4)
int doa3_native_frame_active(void){return replay;}
void pgraph_d3d11_flush(void){++flushed;}
void doa3_native_frame_flip(void){CHECK(flushed && !present);++flip;}
void doa3_apu_deliver_irq(void){CHECK(flip);++irq;eax=77;}
void d3d8_ReplayFrameComplete(void){CHECK(irq);++history;}
void doa3_present_frame(void){++present;}
void doa3_frame_monitor(unsigned f){(void)f;++monitor;}
void doa3_adx_frame(unsigned f){(void)f;++adx;}
void doa3_fn_profile_tick(void){++profile;}
static void d3d4134_capture_frame(unsigned f){(void)f;++capture;}
static void doa3_mtrace_frame(unsigned f){(void)f;++trace;}
static void d3d4134_pace(void){++pace;}
static void sub_001E4AA0(void){CHECK(ecx==0x2268+0x1000 && esp==96);++callback;esp+=4;eax=99;}
#include "native_present.generated.inc"
int main(void){
    replay=1;d3d4134_frame_done(0x1000);
    CHECK(flushed==1 && flip==1 && irq==1 && history==1 && callback==1 && g_doa3_frames_presented==1);
    CHECK(!present && !pace && !capture && !monitor && !trace && !adx && !profile);
    CHECK(eax==1 && ecx==2 && edx==3 && ebx==4 && esi==5 && edi==6 && esp==100 && g_seh_ebp==7);
    replay=0;d3d4134_frame_done(0x1000);
    CHECK(flushed==2 && flip==1 && history==1 && callback==2 && g_doa3_frames_presented==2);
    CHECK(present==1 && pace==1 && capture==1 && monitor==1 && trace==1 && adx==1 && profile==1);
    puts("Actual flip helper: replay preserves IRQ/history/guest callback and registers; skips presentation, pacing and captures");return 0;
}
