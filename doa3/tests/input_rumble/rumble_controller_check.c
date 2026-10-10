#include "xinput_xbox.h"
#include <xinput.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    BYTE feedback[0x46] = {0};
    DWORD left, right;
    for (DWORD slot = 0; slot < 4; ++slot) {
        XINPUT_CAPABILITIES caps;
        DWORD result = XInputGetCapabilities(slot, 0, &caps);
        if (result == ERROR_SUCCESS)
            printf("Connected host slot %lu: type=%u subtype=%u flags=%04X motors=%u/%u\n",
                   slot, caps.Type, caps.SubType, caps.Flags,
                   caps.Vibration.wLeftMotorSpeed, caps.Vibration.wRightMotorSpeed);
    }
    atexit(xbox_InputStopRumble);
    xbox_InputInit();
    if (!(xbox_InputHostMask() & 1)) {
        puts("No XInput controller connected; hardware check skipped.");
        return 2;
    }
    /* 50% strength, each motor for one second, through the guest packet path. */
    feedback[0x43] = 0x80;
    left = xbox_InputSetFeedback(0, feedback);
    Sleep(1000);
    xbox_InputStopRumble();
    Sleep(150);
    feedback[0x43] = 0;
    feedback[0x45] = 0x80;
    right = xbox_InputSetFeedback(0, feedback);
    Sleep(1000);
    xbox_InputStopRumble();
    printf("Controller motor command results: left=%lu right=%lu (0=success).\n", left, right);
    return left || right ? 1 : 0;
}
