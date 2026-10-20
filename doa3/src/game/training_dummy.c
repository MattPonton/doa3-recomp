#include "training_dummy.h"
#include "recomp/recomp_types.h"
#include "recomp/gen/xbe_layout.h"
#include "online/netplay_session.h"
#include "free_camera.h"
#include "ui/doa3_ui.h"
#include <string.h>

/* 3.1 Sparring already implements guard, counter-action and counter-hit
 * settings. Only input recording is added here. All access is on the game's
 * thread (including the UI), between input-fiber yields. */
typedef struct RecordedInput {
    unsigned short buttons;
    short sticks[4];
} RecordedInput;
typedef struct Recording {
    RecordedInput input[TRAINING_MAX_FRAMES];
    unsigned char bindings[20];
    int length;
} Recording;
static Recording s_slots[TRAINING_SLOTS];
static int s_mode, s_slot, s_frame, s_loop, s_modified, s_record_tick;
static unsigned s_human, s_dummy, s_port, s_virtual_port, s_character;
static unsigned short s_previous;
static unsigned char s_old_port, s_old_human, s_old_analog;
static unsigned char s_aggregate[0x2C], s_extra[8], s_bindings[20];
static unsigned char s_human_aggregate[0x2C], s_human_extra[8];
static unsigned char s_old_filter[16], s_filter[16], s_stick_extra[8];

int training_available(void)
{
#if defined(DOA3_XBE_ID_JAPAN)
    return !netplay_active() && (MEM8(0x5A2858)==7 || MEM8(0x5A2858)==11);
#else
    return 0;
#endif
}

int training_can_record(void)
{
    if (!training_available() || MEM8(0xC59866) || MEM8(0xC58E49) ||
        MEM8(0xC58E3A) || free_camera_enabled()) return 0;
    unsigned human=MEM8(0xC58E47), dummy=MEM8(0xC58E48);
    unsigned phase=MEM8(0x596A37);
    return (phase==1 || phase==2) && human<2 && dummy==(human^1) &&
        MEM8(0x59CF58+human*0x68)==1 &&
        MEM8(0x59CF58+dummy*0x68)==0 && MEM8(0x30E3F4+human)<4;
}

void training_input_end(void)
{
    if (!s_modified) return;
    memcpy(s_stick_extra,(void *)XBOX_PTR(0x4DA600+s_virtual_port*8),sizeof(s_stick_extra));
    memcpy(s_filter,(void *)XBOX_PTR(0x4DA5C0+s_virtual_port*16),sizeof(s_filter));
    memcpy((void *)XBOX_PTR(0x4DA5C0+s_virtual_port*16),s_old_filter,sizeof(s_old_filter));
    memcpy((void *)XBOX_PTR(0x73C8D0+s_virtual_port*0x2C),s_aggregate,sizeof(s_aggregate));
    memcpy((void *)XBOX_PTR(0x4DA600+s_virtual_port*8),s_extra,sizeof(s_extra));
    memcpy((void *)XBOX_PTR(0x3F6308+s_virtual_port*20),s_bindings,sizeof(s_bindings));
    MEM8(0x5A0B41+s_virtual_port)=s_old_analog;
    MEM8(0x30E3F4+s_dummy)=s_old_port;
    MEM8(0x59CF58+s_dummy*0x68)=s_old_human;
    if (s_record_tick) {
        memcpy((void *)XBOX_PTR(0x73C8D0+s_port*0x2C),s_human_aggregate,sizeof(s_human_aggregate));
        memcpy((void *)XBOX_PTR(0x4DA600+s_port*8),s_human_extra,sizeof(s_human_extra));
    }
    s_modified=0;
}

void training_stop(void)
{
    training_input_end();
    s_mode=TRAINING_IDLE;
    s_previous=0;
}

void training_reset(void)
{
    training_stop();
    memset(s_slots,0,sizeof(s_slots));
    s_frame=0;
}

int training_start(int mode, int slot, int loop)
{
    training_stop();
    if (!training_can_record() || slot<0 || slot>=TRAINING_SLOTS ||
        (mode!=TRAINING_RECORD && mode!=TRAINING_PLAY)) return 0;
    if (mode==TRAINING_PLAY && !s_slots[slot].length) return 0;
    s_human=MEM8(0xC58E47); s_dummy=s_human^1;
    s_character=MEM8(0x59CF59+s_dummy*0x68);
    s_port=MEM8(0x30E3F4+s_human);
    s_virtual_port=(s_port+1)%4;
    s_slot=slot; s_frame=0; s_loop=loop!=0; s_mode=mode;
    memset(s_filter,0,sizeof(s_filter));
    memset(s_stick_extra,0,sizeof(s_stick_extra));
    if (mode==TRAINING_RECORD) {
        s_slots[slot].length=0;
        memcpy(s_slots[slot].bindings,(void *)XBOX_PTR(0x3F6308+s_port*20),20);
    }
    /* Discard old command history when taking control; do not alter the native
     * training configuration or the characters' current moves/positions. */
    memset((void *)XBOX_PTR(0x4DA1D0+s_dummy*0x82),0,0x82);
    MEM16(0x591F54+s_dummy*2)=0;
    MEM16(0x5A27C4+s_dummy*2)=0;
    MEM16(0x596A8C+s_dummy*2)=0;
    return 1;
}

void training_clear(int slot)
{
    if (slot<0 || slot>=TRAINING_SLOTS) return;
    training_stop();
    memset(&s_slots[slot],0,sizeof(s_slots[slot]));
}

void training_status(TrainingStatus *out)
{
    out->mode=s_mode; out->slot=s_slot; out->frame=s_frame;
    for (int i=0;i<TRAINING_SLOTS;++i) out->length[i]=s_slots[i].length;
}

void training_input_begin(void)
{
    training_input_end();
    if (s_mode==TRAINING_IDLE) return;
    if (!training_can_record() || doa3_ui_visible() ||
        MEM8(0xC58E47)!=s_human || MEM8(0x30E3F4+s_human)!=s_port ||
        MEM8(0x59CF59+s_dummy*0x68)!=s_character) {
        training_stop(); return;
    }
    Recording *clip=&s_slots[s_slot];
    RecordedInput input;
    s_record_tick=s_mode==TRAINING_RECORD;
    if (s_record_tick) {
        if (clip->length==TRAINING_MAX_FRAMES) { training_stop(); return; }
        /* Start and Back operate native menus/reset; never record them. */
        input.buttons=MEM16(0x73C8D0+s_port*0x2C)&0xFFCF;
        memcpy(input.sticks,(void *)XBOX_PTR(0x73C8EC+s_port*0x2C),sizeof(input.sticks));
        clip->input[clip->length++]=input;
        s_frame=clip->length;
    } else {
        if (s_frame==clip->length) {
            if (!s_loop) { training_stop(); return; }
            s_frame=0;
        }
        input=clip->input[s_frame++];
    }
    unsigned agg=0x73C8D0+s_virtual_port*0x2C;
    unsigned short buttons=input.buttons;
    memcpy(s_aggregate,(void *)XBOX_PTR(agg),sizeof(s_aggregate));
    memcpy(s_extra,(void *)XBOX_PTR(0x4DA600+s_virtual_port*8),sizeof(s_extra));
    memcpy(s_bindings,(void *)XBOX_PTR(0x3F6308+s_virtual_port*20),sizeof(s_bindings));
    memcpy(s_old_filter,(void *)XBOX_PTR(0x4DA5C0+s_virtual_port*16),sizeof(s_old_filter));
    s_old_analog=MEM8(0x5A0B41+s_virtual_port);
    s_old_port=MEM8(0x30E3F4+s_dummy);
    s_old_human=MEM8(0x59CF58+s_dummy*0x68);
    if (s_record_tick) {
        memcpy(s_human_aggregate,(void *)XBOX_PTR(0x73C8D0+s_port*0x2C),sizeof(s_human_aggregate));
        memcpy(s_human_extra,(void *)XBOX_PTR(0x4DA600+s_port*8),sizeof(s_human_extra));
        memset((void *)XBOX_PTR(0x73C8D0+s_port*0x2C),0,sizeof(s_human_aggregate));
        memset((void *)XBOX_PTR(0x4DA600+s_port*8),0,sizeof(s_human_extra));
    }
    memset((void *)XBOX_PTR(agg),0,sizeof(s_aggregate));
    MEM32(agg)=buttons;
    MEM32(agg+4)=s_previous;
    MEM32(agg+8)=buttons & ~s_previous;
    MEM32(agg+12)=s_previous & ~buttons;
    /* 1EF20 converts these stick axes to native movement, including its
     * hysteresis and pressed/held/released edges. Keep that history per clip. */
    memcpy((void *)XBOX_PTR(agg+0x1C),input.sticks,sizeof(input.sticks));
    memcpy((void *)XBOX_PTR(0x4DA600+s_virtual_port*8),s_stick_extra,sizeof(s_stick_extra));
    memcpy((void *)XBOX_PTR(0x4DA5C0+s_virtual_port*16),s_filter,sizeof(s_filter));
    memcpy((void *)XBOX_PTR(0x3F6308+s_virtual_port*20),clip->bindings,20);
    MEM8(0x5A0B41+s_virtual_port)=0;
    MEM8(0x30E3F4+s_dummy)=(unsigned char)s_virtual_port;
    MEM8(0x59CF58+s_dummy*0x68)=1;
    s_previous=buttons;
    s_modified=1;
}
