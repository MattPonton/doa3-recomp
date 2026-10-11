/* Read-only hardware check; no rumble, settings changes or generated input. */
#include "ds4_device.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    int samples = argc > 1 ? atoi(argv[1]) : 1;
    if (samples < 1 || samples > 300) return 1;
    for (int n = 0; n < samples; ++n) {
        /* This console-only diagnostic has no window to receive hotplug. */
        if (n % 10 == 0) ds4_devices_changed();
        ds4_probe();
        for (int i = 0; i < 4; ++i) {
            XINPUT_STATE state;
            DWORD result = ds4_read(i, &state);
            if (!n || result == ERROR_SUCCESS)
                printf("sample=%d ps4=%d result=%lu buttons=%04X sticks=%d,%d,%d,%d triggers=%u,%u\n",
                    n, i + 1, result, result ? 0 : state.Gamepad.wButtons,
                    result ? 0 : state.Gamepad.sThumbLX, result ? 0 : state.Gamepad.sThumbLY,
                    result ? 0 : state.Gamepad.sThumbRX, result ? 0 : state.Gamepad.sThumbRY,
                    result ? 0 : state.Gamepad.bLeftTrigger, result ? 0 : state.Gamepad.bRightTrigger);
        }
        fflush(stdout);
        if (n + 1 < samples) Sleep(100);
    }
    return 0;
}
