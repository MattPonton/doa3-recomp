#include "training_dummy.h"
#include "recomp/recomp_types.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset=0x20000000;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
void sub_00011000_gen(void);
void sub_0001EF20_gen(void);
static void stick_tick(unsigned port)
{
    g_esp=0x100000;
    MEM32(g_esp)=0; MEM32(g_esp+4)=port;
    sub_0001EF20_gen();
    CHECK(g_esp==0x100004);
}
static int online, camera, menu;
int netplay_active(void) { return online; }
int free_camera_enabled(void) { return camera; }
int doa3_ui_visible(void) { return menu; }

static void setup(void)
{
    training_reset();
    memset((void *)XBOX_PTR(0x10000),0,0xC60000);
    MEM8(0x5A2858)=7; MEM8(0x596A37)=1;
    MEM8(0xC58E47)=0; MEM8(0xC58E48)=1;
    MEM8(0x59CF58)=1; MEM8(0x59CFC1)=4;
    MEM8(0x30E3F5)=2;
    MEM8(0xC58E42)=6; MEM8(0xC58E1B)=2; MEM8(0xC58E43)=1;
}
int main(void)
{
    CHECK(VirtualAlloc((void *)(g_xbox_mem_offset+0x10000),0xC60000,
        MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));
    setup();
    CHECK(training_available() && training_can_record());
    CHECK(!training_start(TRAINING_PLAY,0,0));
    CHECK(!training_start(TRAINING_RECORD,-1,0));
    unsigned char saved[0x2C], mapping[20];
    memset((void *)XBOX_PTR(0x73C8FC),0xA5,0x2C);
    memset((void *)XBOX_PTR(0x3F631C),0x5A,20);
    memcpy(saved,(void *)XBOX_PTR(0x73C8FC),sizeof(saved));
    memcpy(mapping,(void *)XBOX_PTR(0x3F631C),sizeof(mapping));
    MEM8(0x5A0B42)=1;
    CHECK(training_start(TRAINING_RECORD,0,0));
    MEM16(0x73C8EC)=32767;
    MEM16(0x73C8D0)=0x8138; /* include Start/Back to verify removal */
    training_input_begin();
    CHECK(MEM8(0x59CFC0)==1 && MEM8(0x30E3F5)==1);
    CHECK(MEM16(0x73C8FC)==0x8108 && MEM16(0x73C904)==0x8108);
    CHECK(MEM16(0x73C8D0)==0 && MEM8(0x5A0B42)==0);
    CHECK(MEM16(0x73C918)==32767);
    training_input_end();
    CHECK(MEM8(0x59CFC0)==0 && MEM8(0x30E3F5)==2);
    CHECK(MEM16(0x73C8D0)==0x8138 && MEM8(0x5A0B42)==1);
    CHECK(!memcmp(saved,(void *)XBOX_PTR(0x73C8FC),sizeof(saved)));
    CHECK(!memcmp(mapping,(void *)XBOX_PTR(0x3F631C),sizeof(mapping)));
    training_input_begin();
    CHECK(MEM16(0x73C900)==0x8108 && MEM16(0x73C904)==0);
    training_input_end();
    training_stop();
    TrainingStatus s; training_status(&s);
    CHECK(s.mode==TRAINING_IDLE && s.length[0]==2);
    CHECK(MEM8(0xC58E42)==6 && MEM8(0xC58E1B)==2 && MEM8(0xC58E43)==1);
    CHECK(training_start(TRAINING_PLAY,0,0));
    MEM16(0x73C8D0)=0x100;
    training_input_begin();
    CHECK(MEM16(0x73C8FC)==0x8108 && MEM16(0x73C8D0)==0x100);
    CHECK(MEM16(0x73C918)==32767);
    training_input_end(); training_input_begin(); training_input_end();
    training_input_begin(); training_status(&s); CHECK(s.mode==TRAINING_IDLE);
    CHECK(training_start(TRAINING_PLAY,0,1));
    for (int i=0;i<7;++i) { training_input_begin(); training_input_end(); }
    training_status(&s); CHECK(s.mode==TRAINING_PLAY && s.frame==1);
    menu=1; training_input_begin(); training_status(&s); CHECK(s.mode==TRAINING_IDLE); menu=0;
    CHECK(training_start(TRAINING_RECORD,1,0));
    for (int i=0;i<TRAINING_MAX_FRAMES+2;++i) { training_input_begin(); training_input_end(); }
    training_status(&s); CHECK(s.length[1]==TRAINING_MAX_FRAMES && s.mode==TRAINING_IDLE);
    online=1; CHECK(!training_available() && !training_start(TRAINING_RECORD,0,0)); online=0;
    camera=1; CHECK(!training_can_record()); camera=0;
    MEM8(0xC58E49)=1; CHECK(!training_can_record()); MEM8(0xC58E49)=0;
    MEM8(0xC59866)=1; CHECK(!training_can_record()); MEM8(0xC59866)=0;
    CHECK(training_start(TRAINING_PLAY,0,1));
    MEM8(0x5A2858)=2; training_input_begin(); training_status(&s);
    CHECK(!training_available() && s.mode==TRAINING_IDLE && MEM8(0x59CFC0)==0);
    setup();
    MEM8(0xC58E47)=1; MEM8(0xC58E48)=0;
    MEM8(0x59CF58)=0; MEM8(0x59CFC0)=1; MEM8(0x30E3F5)=3;
    CHECK(training_start(TRAINING_RECORD,2,0));
    MEM16(0x73C954)=0x200;
    training_input_begin(); CHECK(MEM16(0x73C8D0)==0x200 && MEM8(0x30E3F4)==0);
    training_stop(); CHECK(MEM8(0x59CF58)==0 && MEM16(0x73C954)==0x200);
    training_reset(); training_status(&s);
    CHECK(!s.length[0] && !s.length[1] && !s.length[2]);
    /* Feed recorded analog input through the game's real converter and prove
     * that playback reproduces it, including its held-input edge history. */
    setup();
    MEM16(0x73C8EC)=32767;
    MEM16(0x3F6308)=1; /* physical high button -> logical H */
    CHECK(training_start(TRAINING_RECORD,0,0));
    unsigned short logical[2], edges[2];
    for (int i=0;i<2;++i) {
        training_input_begin(); stick_tick(1);
        logical[i]=MEM16(0x4DA608); edges[i]=MEM16(0x4DA60A);
        CHECK(logical[i]==0x40);
        training_input_end();
    }
    CHECK(edges[0]==0x40 && edges[1]==0);
    training_stop(); CHECK(training_start(TRAINING_PLAY,0,0));
    MEM16(0x73C8EC)=0;
    for (int i=0;i<2;++i) {
        training_input_begin(); stick_tick(1);
        CHECK(MEM16(0x4DA608)==logical[i] && MEM16(0x4DA60A)==edges[i]);
        g_ecx=1; g_ebx=0x8000; g_esp=0x100000;
        sub_00011000_gen(); CHECK((g_eax&0xFFFF)==1);
        training_input_end();
    }
    training_stop();
    puts("training dummy: routing, edges, restoration, loop, bounds and mode gates passed");
    return 0;
}
