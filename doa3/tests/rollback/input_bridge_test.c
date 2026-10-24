#include "game/rollback_input.h"
#include "game/rollback_input_tape.h"
#include "input/xinput_xbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
volatile int g_xbox_det_active=1;
uint32_t xbox_det_epoch(void) { return 1; }
int xbox_fiber_is_primary(void) { return 1; }
static unsigned char *guest;
static uint32_t eax,esp,host_mask=1,reconfigured;
static unsigned reads,writes,scalars;
static int changing,disconnected;
unsigned g_doa3_frames_presented;
#define MEM8(a) (*(uint8_t *)(guest+(a)))
#define MEM16(a) (*(uint16_t *)(guest+(a)))
#define MEM32(a) (*(uint32_t *)(guest+(a)))
DWORD xbox_InputHostMask(void) { ++scalars;return host_mask; }
DWORD xbox_InputTakeReconfiguredMask(void) { ++scalars;DWORD n=reconfigured;reconfigured=0;return n; }
BYTE xbox_InputGuestSubtype(int port) { ++scalars;return (BYTE)(port+1); }
DWORD xbox_InputSetState(DWORD port,const XBOX_VIBRATION *v) { (void)port;(void)v;++writes;return 0; }
uint32_t netplay_filter_pad_mask(uint32_t n) { return n; }
uint32_t netplay_forced_pads(void) { return 0; }
void doa3_perf_input_sample(unsigned port) { (void)port; }
DWORD xbox_InputGetState(DWORD port,XBOX_INPUT_STATE *out)
{
    ++reads;if(disconnected)return ERROR_DEVICE_NOT_CONNECTED;
    memset(out,0,sizeof(*out));out->dwPacketNumber=900+port;
    out->Gamepad.wButtons=(WORD)(1u<<port);out->Gamepad.bAnalogButtons[port]=128;
    out->Gamepad.sThumbLX=-1234;out->Gamepad.sThumbRY=2345;
    if(changing)out->Gamepad.sThumbLX=(SHORT)reads;
    return 0;
}
#include "input_bridge.generated.inc"
static void input_sequence(void)
{
    esp=0x100;sub_0021157D();CHECK(eax==15);
    for(unsigned p=0;p<5;++p){
        esp=0x100;MEM32(esp+4)=0x0AD00001+p%4;MEM32(esp+8)=0x2000+p*24;sub_00211A96();CHECK(!eax);
    }
    esp=0x100;MEM32(esp+8)=0x2080;MEM32(esp+12)=0x2084;sub_0021159F();
    for(unsigned p=0;p<4;++p){
        esp=0x100;MEM32(esp+4)=0x0AD00001+p;MEM32(esp+8)=0x2090+p*28;sub_002118A4();CHECK(!eax);
    }
    esp=0x100;MEM32(esp+4)=0x0AD00001;MEM32(esp+8)=0x2100;
    MEM16(0x2142)=1234;MEM16(0x2144)=5678;sub_00211B07();CHECK(!eax);
}
int main(void)
{
    guest=calloc(1,0x800000);CHECK(guest);
    Doa3XppSnapshot before,after;
    unsigned char expected[128],actual[128];
    esp=0x100;sub_0021157D();CHECK(eax==1 && esp==0x108);
    CHECK(doa3_xpp_capture(&before,sizeof(before)));
    for (unsigned repeat=0;repeat<3;++repeat) {
        CHECK(doa3_xpp_restore(&before,sizeof(before)));
        memset(guest+0x1000,0,128);
        for (unsigned p=0;p<4;++p) {
            esp=0x100;MEM32(esp+4)=0x0AD00001u+p;MEM32(esp+8)=0x1000+p*24;
            sub_00211A96();CHECK(!eax && esp==0x10C);
            CHECK(MEM32(0x1000+p*24)==0x1001 && MEM16(0x1004+p*24)==(1u<<p));
            CHECK(MEM16(0x100E+p*24)==(uint16_t)-1234);
        }
        host_mask=2;esp=0x100;MEM32(esp+8)=0x1060;MEM32(esp+12)=0x1064;
        sub_0021159F();CHECK(eax==1 && esp==0x110);
        CHECK(MEM32(0x1060)==2 && MEM32(0x1064)==1);
        memcpy(actual,guest+0x1000,128);
        if (!repeat) memcpy(expected,actual,128);else CHECK(!memcmp(expected,actual,128));
    }
    CHECK(doa3_xpp_capture(&before,sizeof(before)));
    esp=0x100;MEM32(esp+4)=0x0AD00001;MEM32(esp+8)=0;sub_00211A96();
    CHECK(doa3_xpp_capture(&after,sizeof(after)) && !memcmp(&before,&after,sizeof(before)));
    doa3_netplay_force_pads(3);CHECK(doa3_xpp_capture(&after,sizeof(after)) && after.previous_mask==3);
    host_mask=3;reconfigured=2;esp=0x100;MEM32(esp+8)=0x1060;MEM32(esp+12)=0x1064;
    sub_0021159F();CHECK(MEM32(0x1060)==0 && MEM32(0x1064)==2);
    /* Replay the actual guest overrides with devices changed/unplugged. */
    Doa3InputTape *t=doa3_input_tape_create(1);CHECK(t);
    unsigned char recorded[0x200];Doa3XppSnapshot recorded_xpp;
    CHECK(doa3_xpp_capture(&before,sizeof(before)));
    changing=1;host_mask=15;reconfigured=4;
    CHECK(doa3_input_tape_record(t,100));input_sequence();CHECK(doa3_input_tape_end(t));
    memcpy(recorded,guest+0x2000,sizeof(recorded));CHECK(doa3_xpp_capture(&recorded_xpp,sizeof(recorded_xpp)));
    unsigned r=reads,w=writes,c=scalars;host_mask=0;reconfigured=15;disconnected=1;
    for(unsigned repeat=0;repeat<3;++repeat){
        CHECK(doa3_xpp_restore(&before,sizeof(before)));memset(guest+0x2000,0,sizeof(recorded));
        CHECK(doa3_input_tape_replay(t,100));input_sequence();CHECK(doa3_input_tape_end(t));
        CHECK(!memcmp(recorded,guest+0x2000,sizeof(recorded)));
        CHECK(doa3_xpp_capture(&after,sizeof(after)) && !memcmp(&after,&recorded_xpp,sizeof(after)));
        CHECK(reads==r && writes==w && scalars==c);
    }
    CHECK(doa3_input_tape_destroy(t));
    free(guest);
    puts("Actual 3.1 XPP overrides: identical guest state/XPP after input replay with unplugged devices and no hardware polling/rumble");
    return 0;
}
