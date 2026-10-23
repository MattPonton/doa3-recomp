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
/* Second-controller control of the dummy (0.1.12). */
static int s_ctrl_enabled, s_ctrl_port = 1, s_ctrl_modified, s_ctrl_active;
static unsigned s_ctrl_dummy;
static unsigned char s_ctrl_old_port, s_ctrl_old_human;
/* Held Stagger Escape values (0.1.12). */
static int s_stagger_hold[2];
static float s_stagger_value[2] = { 1.0f, 1.0f };

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

static void control_end(void)
{
    if (!s_ctrl_modified) return;
    MEM8(0x30E3F4+s_ctrl_dummy)=s_ctrl_old_port;
    MEM8(0x59CF58+s_ctrl_dummy*0x68)=s_ctrl_old_human;
    s_ctrl_modified=0;
}

void training_input_end(void)
{
    control_end();
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

int training_human_port(void)
{
    if (!training_available()) return -1;
    unsigned human=MEM8(0xC58E47);
    if (human>=2) return -1;
    unsigned port=MEM8(0x30E3F4+human);
    return port<4 ? (int)port : -1;
}

int training_human_side(void)
{
    if (!training_available()) return -1;
    unsigned human=MEM8(0xC58E47);
    return human<2 ? (int)human : -1;
}

int training_port_connected(int port)
{
    /* The game's open-device mask: set while a pad is plugged into the port. */
    return port>=0 && port<4 && (MEM32(0x73C8C8) & (1u<<port))!=0;
}

void training_control_set(int enabled, int port)
{
    s_ctrl_enabled=enabled!=0;
    if (port>=0 && port<4) s_ctrl_port=port;
}

void training_control_get(int *enabled, int *port)
{
    if (enabled) *enabled=s_ctrl_enabled;
    if (port) *port=s_ctrl_port;
}

int training_control_active(void) { return s_ctrl_active; }

float training_stagger_get(int side)
{
    return (side==0 || side==1) ? MEMF(0x4DA2D4+side*4) : 1.0f;
}

void training_stagger_hold(int side, int enabled, float value)
{
    if (side!=0 && side!=1) return;
    if (value<1.0f) value=1.0f;
    if (value>2.0f) value=2.0f;
    s_stagger_hold[side]=enabled!=0;
    s_stagger_value[side]=value;
}

void training_stagger_hold_get(int side, int *enabled, float *value)
{
    if (side!=0 && side!=1) return;
    if (enabled) *enabled=s_stagger_hold[side];
    if (value) *value=s_stagger_value[side];
}

/* Once per logic frame, before the fighters update: the stun handlers
 * (0x14D70, 0x16840, 0x1BAE0) add 0.03 per mash or take 0.01 per idle frame
 * from 0x4DA2D4[side], clamp to 1..2 and copy it to the fighter's animation
 * speed. Seeding the slot with the held value keeps that logic intact. */
static void stagger_apply(void)
{
    int side;
    if (!training_available()) return;
    for (side=0; side<2; ++side)
        if (s_stagger_hold[side]) MEMF(0x4DA2D4+side*4)=s_stagger_value[side];
}

/* The dummy is marked human and pointed at the chosen port for the input
 * window only (the game decodes its commands from that port's pad
 * aggregate, with that port's button bindings), then restored, the same
 * way recording hands the dummy to the player. */
static void control_begin(void)
{
    int port=s_ctrl_port;
    /* The overlay being open does not matter (0.1.14): only recording
     * stops with it, since the recorder reads the player's own pad. */
    int want=s_ctrl_enabled && s_mode==TRAINING_IDLE && training_can_record() &&
             port!=training_human_port() && training_port_connected(port);
    if (!want) { s_ctrl_active=0; return; }
    s_ctrl_dummy=MEM8(0xC58E47)^1;
    if (!s_ctrl_active) {
        /* Taking over from the CPU: drop its pending command history, as
         * recording does. */
        memset((void *)XBOX_PTR(0x4DA1D0+s_ctrl_dummy*0x82),0,0x82);
        MEM16(0x591F54+s_ctrl_dummy*2)=0;
        MEM16(0x5A27C4+s_ctrl_dummy*2)=0;
        MEM16(0x596A8C+s_ctrl_dummy*2)=0;
    }
    s_ctrl_old_port=MEM8(0x30E3F4+s_ctrl_dummy);
    s_ctrl_old_human=MEM8(0x59CF58+s_ctrl_dummy*0x68);
    MEM8(0x30E3F4+s_ctrl_dummy)=(unsigned char)port;
    MEM8(0x59CF58+s_ctrl_dummy*0x68)=1;
    s_ctrl_modified=1;
    s_ctrl_active=1;
}

void training_input_begin(void)
{
    training_input_end();
    stagger_apply();
    control_begin();
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

/* 0.1.14: the CPU's stun handler (0x16840) first resets the value to 1.00
 * when the fighter's +0x59 is 1 (a CPU fighter), then adds its random mash
 * or decays, and copies the result to the fighter's animation speed (+0x10)
 * on every path. Seeding the slot is therefore lost for a CPU dummy. Around
 * that handler: note the base it starts from, and afterwards put the held
 * value in its place with that frame's change on top, in both the slot and
 * the animation speed. */
static float s_cpu_base;
static int s_cpu_side = -1;
void training_stagger_cpu_before(void)
{
    unsigned side=MEM8(0x4DA2DD), fighter=MEM32(0x4DA1C4);
    s_cpu_side=-1;
    if (side>1 || !fighter || !s_stagger_hold[side] || !training_available()) return;
    s_cpu_side=(int)side;
    s_cpu_base=MEM8(fighter+0x59)==1 ? 1.0f : MEMF(0x4DA2D4+side*4);
}
void training_stagger_cpu_after(void)
{
    unsigned fighter;
    float v;
    if (s_cpu_side<0) return;
    fighter=MEM32(0x4DA1C4);
    v=s_stagger_value[s_cpu_side]+(MEMF(0x4DA2D4+s_cpu_side*4)-s_cpu_base);
    if (v<1.0f) v=1.0f;
    if (v>2.0f) v=2.0f;
    MEMF(0x4DA2D4+s_cpu_side*4)=v;
    if (fighter) MEMF(fighter+0x10)=v;
    s_cpu_side=-1;
}
