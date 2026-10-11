#include "pad_mapping.h"
#include "ds4_device.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static DWORD connected = 15, clock_ms = 100;
static XINPUT_STATE states[4];
static XINPUT_VIBRATION motors[4];
static int pressed_key;
extern DWORD ds4_test_mask;
extern XINPUT_STATE ds4_test_states[4];
extern XINPUT_VIBRATION ds4_test_motors[4];
DWORD input_test_clock(void) { return clock_ms; }
SHORT player_test_key(int key) { return key == pressed_key ? (SHORT)0x8000 : 0; }
DWORD WINAPI XInputGetState(DWORD p, XINPUT_STATE *state)
{
    *state = states[p];
    return connected & (1u << p) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI XInputGetCapabilities(DWORD p, DWORD flags, XINPUT_CAPABILITIES *caps)
{
    (void)flags;
    memset(caps,0,sizeof(*caps));
    return connected & (1u << p) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI XInputSetState(DWORD p, XINPUT_VIBRATION *vib)
{
    motors[p] = *vib;
    return connected & (1u << p) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;
}
static void write_ini(const char *text)
{
    FILE *f = NULL;
    CHECK(!fopen_s(&f,"player_mapping_test.ini","wb") && f);
    fputs(text,f); fclose(f);
}
int main(void)
{
    XBOX_INPUT_STATE guest;
    XBOX_INPUT_CAPABILITIES caps;
    XBOX_VIBRATION vib = {1234,5678};
    const WORD buttons[] = {XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_Y};
    pad_mapping_reset_defaults();
    for (int p=0;p<4;++p) { states[p].Gamepad.wButtons=buttons[p]; states[p].dwPacketNumber=100+p; }
    xbox_InputInit();
    CHECK(xbox_InputHostMask()==15);
    for (int p=0;p<4;++p) {
        CHECK(xbox_InputGetState(p,&guest)==ERROR_SUCCESS);
        CHECK(guest.Gamepad.bAnalogButtons[p]==255 && guest.dwPacketNumber==100+p);
    }
    /* Explicit slot reserves its device ahead of all automatic players. */
    pad_mapping_set_device(3,0);
    CHECK(xbox_InputAssignedSlot(3)==0 && xbox_InputAssignedSlot(0)==1);
    CHECK(xbox_InputGetState(3,&guest)==ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0]==255);
    CHECK(xbox_InputSetState(3,&vib)==ERROR_SUCCESS && motors[0].wLeftMotorSpeed==1234);
    pad_mapping_set_device(2,0);
    CHECK(pad_mapping_device(3)==PAD_DEVICE_NONE);
    CHECK(xbox_InputAssignedSlot(3)==-1 && xbox_InputAssignedSlot(2)==0);
    CHECK(motors[0].wLeftMotorSpeed==0);
    /* Four explicit slots do not slide when one is removed. */
    for (int p=0;p<4;++p) pad_mapping_set_device(p,p);
    connected &= ~2u;
    CHECK(xbox_InputGetState(1,&guest)==ERROR_DEVICE_NOT_CONNECTED);
    CHECK(xbox_InputHostMask()==13 && xbox_InputAssignedSlot(2)==2 && xbox_InputAssignedSlot(3)==3);
    connected |= 2u; clock_ms+=1000;
    CHECK(xbox_InputHostMask()==15 && xbox_InputAssignedSlot(1)==1);
    /* Separate bindings and keyboard ownership. */
    pad_mapping_get_player(1)->b[PADCTL_A]=(PadBinding){PADSRC_PAD_BUTTON,XINPUT_GAMEPAD_B};
    CHECK(xbox_InputGetState(1,&guest)==ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0]==255);
    CHECK(pad_mapping_get_player(0)->b[PADCTL_A].code==XINPUT_GAMEPAD_A);
    pad_mapping_set_device(1,PAD_DEVICE_KEYBOARD);
    pad_mapping_get_player(1)->b[PADCTL_A]=(PadBinding){PADSRC_KEY,'J'};
    pressed_key='J';
    CHECK(xbox_InputGetState(1,&guest)==ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0]==255);
    states[0].Gamepad.wButtons=0;
    CHECK(xbox_InputGetState(0,&guest)==ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0]==0);
    pad_mapping_set_menu_open(1);
    CHECK(xbox_InputGetState(1,&guest)==ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0]==0);
    pad_mapping_set_menu_open(0);
    pad_mapping_set_device(1,PAD_DEVICE_NONE);
    CHECK(xbox_InputGetState(1,&guest)==ERROR_DEVICE_NOT_CONNECTED);
    /* Guest subtype is independent of the physical host controller. */
    for (int t=0;t<PAD_TYPE_COUNT;++t) {
        const BYTE expected[]={1,2,0x20};
        pad_mapping_set_type(2,t);
        CHECK(xbox_InputGuestSubtype(2)==expected[t]);
        CHECK(xbox_InputGetCapabilities(2,0,&caps)==ERROR_SUCCESS && caps.SubType==expected[t]);
    }
    CHECK(xbox_InputTakeReconfiguredMask()!=0 && xbox_InputTakeReconfiguredMask()==0);
    /* All four mappings, assignments and types survive a save/load. */
    for (int p=0;p<4;++p) {
        pad_mapping_set_device(p,p);
        pad_mapping_set_type(p,p%3);
        pad_mapping_get_player(p)->b[PADCTL_START]=(PadBinding){PADSRC_KEY,'1'+p};
    }
    CHECK(pad_mapping_save("player_mapping_test.ini"));
    pad_mapping_reset_defaults();
    CHECK(pad_mapping_load("player_mapping_test.ini"));
    for (int p=0;p<4;++p) {
        CHECK(pad_mapping_device(p)==p && pad_mapping_type(p)==p%3);
        CHECK(pad_mapping_get_player(p)->b[PADCTL_START].code=='1'+p);
    }
    pad_mapping_reset_player(2);
    CHECK(pad_mapping_get_player(2)->b[PADCTL_START].kind==PADSRC_PAD_BUTTON);
    CHECK(pad_mapping_get_player(1)->b[PADCTL_START].kind==PADSRC_KEY);
    /* Older shared mappings migrate without discarding any player's binding. */
    write_ini("A=1:8192\n");
    CHECK(pad_mapping_load("player_mapping_test.ini"));
    for (int p=0;p<4;++p) CHECK(pad_mapping_get_player(p)->b[PADCTL_A].code==8192);
    write_ini("[Player1]\nDevice=2\nType=99\n[Player2]\nDevice=2\n[Player3]\nDevice=99\n[Player5]\nA=0:0\n");
    CHECK(pad_mapping_load("player_mapping_test.ini"));
    CHECK(pad_mapping_device(0)==2 && pad_mapping_device(1)==PAD_DEVICE_NONE);
    CHECK(pad_mapping_type(0)==PAD_TYPE_DUKE && pad_mapping_device(2)==PAD_DEVICE_AUTO);
    CHECK(pad_mapping_get_player(0)->b[PADCTL_A].kind==PADSRC_PAD_BUTTON);
    /* Native DS4 slots coexist with all four XInput slots, reserve normally,
     * survive settings round-trips, and release neutral input on unplug. */
    pad_mapping_reset_defaults();
    ds4_test_mask = 1;
    ds4_test_states[0].Gamepad.wButtons = XINPUT_GAMEPAD_A;
    ds4_test_states[0].Gamepad.bLeftTrigger = 190;
    clock_ms += 1000;
    CHECK(xbox_InputPhysicalMask() == 31);
    pad_mapping_set_device(0, 4);
    CHECK(xbox_InputAssignedSlot(0) == 4 && xbox_InputAssignedSlot(1) == 0);
    CHECK(xbox_InputGetState(0,&guest) == ERROR_SUCCESS && guest.Gamepad.bAnalogButtons[0] == 255);
    CHECK(guest.Gamepad.bAnalogButtons[XBOX_BUTTON_LTRIGGER] == 190);
    CHECK(xbox_InputGetCapabilities(0,0,&caps) == ERROR_SUCCESS);
    CHECK(caps.Flags & XINPUT_CAPS_FFB_SUPPORTED);
    CHECK(xbox_InputSetState(0,&vib) == ERROR_SUCCESS && ds4_test_motors[0].wLeftMotorSpeed == vib.wLeftMotorSpeed);
    xbox_InputSetActive(FALSE);
    CHECK(!ds4_test_motors[0].wLeftMotorSpeed && !ds4_test_motors[0].wRightMotorSpeed);
    CHECK(xbox_InputSetState(0,&vib) == ERROR_SUCCESS && !ds4_test_motors[0].wRightMotorSpeed);
    xbox_InputSetActive(TRUE);
    CHECK(xbox_InputSetState(0,&vib) == ERROR_SUCCESS && ds4_test_motors[0].wRightMotorSpeed == vib.wRightMotorSpeed);
    xbox_InputSetCaptured(TRUE);
    CHECK(!ds4_test_motors[0].wLeftMotorSpeed);
    xbox_InputSetCaptured(FALSE);
    CHECK(xbox_InputSetState(0,&vib) == ERROR_SUCCESS);
    pad_mapping_set_device(1,4);
    CHECK(!ds4_test_motors[0].wLeftMotorSpeed && pad_mapping_device(0) == PAD_DEVICE_NONE);
    pad_mapping_set_device(0,4);
    pad_mapping_set_device(1,PAD_DEVICE_AUTO);
    XINPUT_STATE raw;
    CHECK(xbox_InputReadPhysical(4,&raw) == ERROR_SUCCESS && raw.Gamepad.wButtons == XINPUT_GAMEPAD_A);
    CHECK(xbox_InputReadPhysical(8,&raw) == ERROR_DEVICE_NOT_CONNECTED);
    CHECK(pad_mapping_save("player_mapping_test.ini"));
    pad_mapping_reset_defaults();
    CHECK(pad_mapping_load("player_mapping_test.ini") && pad_mapping_device(0) == 4);
    ds4_test_mask = 0;
    CHECK(xbox_InputGetState(0,&guest) == ERROR_DEVICE_NOT_CONNECTED && xbox_InputAssignedSlot(0) == -1);
    /* A hotplug rescan refreshes the slot immediately, without waiting for
     * the disconnected-slot polling interval. */
    ds4_test_mask = 1; ds4_devices_changed();
    CHECK(xbox_InputHostMask() == 15 && xbox_InputAssignedSlot(0) == 4);
    connected = 0; clock_ms += 1000;
    pad_mapping_reset_defaults();
    CHECK(xbox_InputHostMask() == 1 && xbox_InputAssignedSlot(0) == 4);
    remove("player_mapping_test.ini");
    puts("Four-player routing, rumble, reconnect, bindings, keyboard, subtypes and persistence passed.");
    return 0;
}
