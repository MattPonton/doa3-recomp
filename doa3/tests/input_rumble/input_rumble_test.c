#include "xinput_xbox.h"
#include <xinput.h>
#include <stdio.h>
#include <string.h>

static DWORD connected = (1u << 2) | (1u << 3);
static DWORD set_result = ERROR_SUCCESS, calls;
static XINPUT_VIBRATION motors[4];
static int keyboard;

DWORD WINAPI XInputGetState(DWORD slot, XINPUT_STATE *state)
{
    memset(state, 0, sizeof(*state));
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
    CHECK(xbox_InputHostMask() == 1);
    CHECK(motors[3].wLeftMotorSpeed == 0 && motors[3].wRightMotorSpeed == 0);
    CHECK(xbox_InputSetFeedback(0, packet) == ERROR_SUCCESS);
    CHECK(motors[3].wLeftMotorSpeed == 0x1234);
    xbox_InputStopRumble();
    connected = 0;
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
    puts("Rumble packet, motor routing, stop, focus, unplug, and error checks passed.");
    return 0;
}
