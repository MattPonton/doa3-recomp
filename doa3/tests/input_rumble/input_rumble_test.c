#include "xinput_xbox.h"
#include <xinput.h>
#include <stdio.h>
#include <string.h>

static DWORD connected = (1u << 2) | (1u << 3);
static DWORD set_result = ERROR_SUCCESS, calls;
static XINPUT_VIBRATION motors[4];
static int keyboard;
static DWORD clock_ms = 100, get_calls[4], packet_number;
DWORD input_test_clock(void) { return clock_ms; }

DWORD WINAPI XInputGetState(DWORD slot, XINPUT_STATE *state)
{
    memset(state, 0, sizeof(*state));
    ++get_calls[slot];
    state->dwPacketNumber = ++packet_number;
    state->Gamepad.sThumbLX = 12345;
    return (connected & (1u << slot)) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI XInputGetCapabilities(DWORD slot, DWORD flags, XINPUT_CAPABILITIES *caps)
{
    (void)slot; (void)flags;
    memset(caps, 0, sizeof(*caps));
    return ERROR_SUCCESS;
}
DWORD WINAPI XInputSetState(DWORD slot, XINPUT_VIBRATION *vibration)
{
    ++calls;
    if (set_result == ERROR_SUCCESS) motors[slot] = *vibration;
    return set_result;
}
int pad_mapping_has_key_binding(void) { return keyboard; }
void pad_mapping_apply_port(const void *state, XBOX_GAMEPAD *pad, int use_keyboard)
{
    (void)state; (void)use_keyboard;
    memset(pad, 0, sizeof(*pad));
    pad->sThumbLX = 12345;
}

#define CHECK(condition) do { if (!(condition)) { \
    fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #condition); return 1; \
} } while (0)

int main(void)
{
    /* Deliberately unaligned, like DOA3's feedback at pad+0x2F. */
    BYTE storage[0x48], before[0x48], *packet = storage + 1;
    DWORD status, previous_calls;
    XBOX_VIBRATION off = { 0, 0 };
    memset(storage, 0xA5, sizeof(storage));
    packet[0x42] = 0x34; packet[0x43] = 0x12;
    packet[0x44] = 0xCD; packet[0x45] = 0xAB;
    memcpy(before, storage, sizeof(storage));
    xbox_InputInit();
    /* Even during the first second of system uptime, initialization probes
     * every slot. Repeated enumeration must not resample a connected pad. */
    for (unsigned i=0; i<4; ++i) CHECK(get_calls[i]==1);
    for (unsigned i=0; i<1000; ++i) CHECK(xbox_InputHostMask()==3);
    for (unsigned i=0; i<4; ++i) CHECK(get_calls[i]==1);
    {
        XBOX_INPUT_STATE first, second;
        CHECK(xbox_InputGetState(0,&first)==ERROR_SUCCESS);
        CHECK(xbox_InputGetState(0,&second)==ERROR_SUCCESS);
        CHECK(second.dwPacketNumber==first.dwPacketNumber+1);
        CHECK(get_calls[2]==3); /* Input is fresh even within the same ms. */
        CHECK(xbox_InputHostMask()==3 && get_calls[2]==3);
        clock_ms+=249;
        CHECK(xbox_InputHostMask()==3 && get_calls[2]==3 && get_calls[3]==1);
        ++clock_ms;
        CHECK(xbox_InputHostMask()==3 && get_calls[2]==4 && get_calls[3]==2);
        CHECK(get_calls[0]==1 && get_calls[1]==1);
    }
    CHECK(xbox_InputHostMask() == 3);
    {
        XBOX_INPUT_STATE guest;
        XINPUT_STATE raw;
        CHECK(xbox_InputGetState(0, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 12345);
        xbox_InputSetCaptured(TRUE);
        CHECK(xbox_InputGetState(0, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 0);
        CHECK(xbox_InputGetRawState(&raw) == ERROR_SUCCESS && raw.Gamepad.sThumbLX == 12345);
        CHECK(xbox_InputGetState(1, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 0);
        CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS && motors[2].wLeftMotorSpeed == 0);
        xbox_InputSetCaptured(FALSE);
        CHECK(xbox_InputGetState(0, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 12345);
    }
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(motors[2].wLeftMotorSpeed == 0x1234 && motors[2].wRightMotorSpeed == 0xABCD);
    CHECK(motors[3].wLeftMotorSpeed == 0);
    memcpy(&status, packet, sizeof(status));
    CHECK(status == ERROR_SUCCESS);
    CHECK(storage[0] == before[0] && memcmp(storage + 5, before + 5, sizeof(storage) - 5) == 0);
    CHECK(xbox_InputSetFeedback(1, packet) == ERROR_SUCCESS);
    CHECK(motors[3].wRightMotorSpeed == 0xABCD);
    CHECK(xbox_InputSetState(0, &off) == ERROR_SUCCESS);
    CHECK(motors[2].wLeftMotorSpeed == 0 && motors[2].wRightMotorSpeed == 0);

    xbox_InputSetActive(FALSE);
    CHECK(motors[3].wLeftMotorSpeed == 0 && motors[3].wRightMotorSpeed == 0);
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(motors[2].wLeftMotorSpeed == 0 && motors[2].wRightMotorSpeed == 0);
    xbox_InputSetActive(TRUE);
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(motors[2].wLeftMotorSpeed == 0x1234);
    xbox_InputStopRumble();
    CHECK(motors[2].wLeftMotorSpeed == 0 && motors[2].wRightMotorSpeed == 0);

    set_result = ERROR_DEVICE_NOT_CONNECTED;
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_DEVICE_NOT_CONNECTED);
    memcpy(&status, packet, sizeof(status));
    CHECK(status == ERROR_DEVICE_NOT_CONNECTED);
    set_result = ERROR_SUCCESS;
    previous_calls = calls;
    CHECK(xbox_InputSetFeedback(4, packet) == ERROR_DEVICE_NOT_CONNECTED);
    CHECK(xbox_InputSetFeedback(0, NULL) == ERROR_INVALID_PARAMETER);
    CHECK(calls == previous_calls);

    CHECK(xbox_InputSetFeedback(1, packet) == ERROR_SUCCESS);
    connected = 1u << 3;
    {
        XBOX_INPUT_STATE guest;
        CHECK(xbox_InputGetState(0,&guest)==ERROR_DEVICE_NOT_CONNECTED);
    }
    CHECK(xbox_InputHostMask() == 1);
    CHECK(motors[3].wLeftMotorSpeed == 0 && motors[3].wRightMotorSpeed == 0);
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(motors[3].wLeftMotorSpeed == 0x1234);
    xbox_InputStopRumble();
    connected = 0;
    clock_ms+=250; /* Idle discovery also detects an unplug. */
    CHECK(xbox_InputHostMask() == 0);
    keyboard = 1;
    {
        XBOX_INPUT_STATE guest;
        XINPUT_STATE raw;
        xbox_InputSetCaptured(TRUE);
        CHECK(xbox_InputGetState(0, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 0);
        CHECK(xbox_InputGetRawState(&raw) == ERROR_DEVICE_NOT_CONNECTED);
        xbox_InputSetCaptured(FALSE);
        CHECK(xbox_InputGetState(0, &guest) == ERROR_SUCCESS && guest.Gamepad.sThumbLX == 12345);
    }
    previous_calls = calls;
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(calls == previous_calls);
    /* Reconnect is discovered without caching button state or spinning on
     * empty slots, and free-camera raw reads update disconnect status too. */
    keyboard=0;
    connected=1;
    CHECK(xbox_InputHostMask()==0);
    clock_ms+=1000;
    CHECK(xbox_InputHostMask()==1);
    {
        XINPUT_STATE raw;
        CHECK(xbox_InputGetRawState(&raw)==ERROR_SUCCESS);
        connected=0;
        CHECK(xbox_InputGetRawState(&raw)==ERROR_DEVICE_NOT_CONNECTED);
        CHECK(xbox_InputHostMask()==0);
    }
    clock_ms=0xFFFFFFF0u;
    xbox_InputHostMask();
    DWORD prior=get_calls[0];
    clock_ms+=32; /* DWORD uptime wrap must preserve the probe interval. */
    xbox_InputHostMask(); CHECK(get_calls[0]==prior);
    clock_ms+=1000;
    xbox_InputHostMask(); CHECK(get_calls[0]==prior+1);
    puts("Fresh polling, discovery throttle, reconnect, clock wrap, rumble, routing, focus and capture passed.");
    return 0;
}
