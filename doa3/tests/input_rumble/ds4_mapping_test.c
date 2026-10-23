#include "ds4_device.h"
#include "ds4_rumble.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
int main(void)
{
    DIJOYSTATE2 raw = {0};
    XINPUT_GAMEPAD pad;
    const WORD expected[] = {XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_Y,
        XINPUT_GAMEPAD_LEFT_SHOULDER,XINPUT_GAMEPAD_RIGHT_SHOULDER,0,0,
        XINPUT_GAMEPAD_BACK,XINPUT_GAMEPAD_START,XINPUT_GAMEPAD_LEFT_THUMB,XINPUT_GAMEPAD_RIGHT_THUMB};
    raw.lX = raw.lY = raw.lZ = raw.lRz = 32768;
    raw.rgdwPOV[0] = 0xFFFFFFFF;
    ds4_translate(&raw, &pad);
    CHECK(!pad.wButtons && !pad.bLeftTrigger && !pad.bRightTrigger);
    CHECK(pad.sThumbLX == 0 && pad.sThumbLY == -1 && pad.sThumbRX == 0 && pad.sThumbRY == -1);
    for (int i = 0; i < 12; ++i) {
        raw.rgbButtons[i] = 0x80;
        ds4_translate(&raw, &pad);
        CHECK(pad.wButtons == expected[i]);
        raw.rgbButtons[i] = 0;
    }
    const WORD hats[] = {1,9,8,10,2,6,4,5};
    for (int i = 0; i < 8; ++i) {
        raw.rgdwPOV[0] = i * 4500;
        ds4_translate(&raw, &pad);
        CHECK(pad.wButtons == hats[i]);
    }
    raw.rgdwPOV[0] = 0xFFFFFFFF;
    raw.lX = raw.lY = 0; raw.lZ = raw.lRz = 65535;
    raw.lRx = 65535; raw.lRy = 32768;
    ds4_translate(&raw, &pad);
    CHECK(pad.sThumbLX == -32768 && pad.sThumbLY == 32767);
    CHECK(pad.sThumbRX == 32767 && pad.sThumbRY == -32768);
    CHECK(pad.bLeftTrigger == 255 && pad.bRightTrigger == 127);
    BYTE report[32];
    ds4_rumble_report(report, 65535, 32768);
    CHECK(report[0] == 5 && report[1] == 1 && report[4] == 128 && report[5] == 255);
    for (int i = 2; i < 32; ++i) if (i != 4 && i != 5) CHECK(report[i] == 0);
    ds4_rumble_report(report, 0, 0);
    CHECK(report[4] == 0 && report[5] == 0 && report[1] == 1);
    BYTE r5[48];
    ds5_rumble_report(r5, 65535, 32768);
    CHECK(r5[0] == 2 && r5[1] == 3 && r5[3] == 128 && r5[4] == 255 && r5[39] == 4);
    for (int i = 2; i < 48; ++i) if (i != 3 && i != 4 && i != 39) CHECK(r5[i] == 0);
    ds5_rumble_report(r5, 0, 0);
    CHECK(r5[3] == 0 && r5[4] == 0 && r5[1] == 3);
    puts("DS4/DualSense buttons, diagonal d-pad, sticks, Y inversion, analog triggers and rumble reports passed.");
    return 0;
}
