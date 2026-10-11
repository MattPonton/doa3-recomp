#include "ds4_device.h"
DWORD ds4_test_mask;
XINPUT_STATE ds4_test_states[4];
XINPUT_VIBRATION ds4_test_motors[4];
void ds4_probe(void) {}
DWORD ds4_read(int slot, XINPUT_STATE *state)
{
    if (slot < 0 || slot >= 4 || !(ds4_test_mask & (1u << slot))) return ERROR_DEVICE_NOT_CONNECTED;
    *state = ds4_test_states[slot];
    return ERROR_SUCCESS;
}
int ds4_has_rumble(int slot) { return slot >= 0 && slot < 4 && (ds4_test_mask & (1u << slot)) != 0; }
DWORD ds4_set_rumble(int slot, const XINPUT_VIBRATION *motors)
{
    if (!ds4_has_rumble(slot)) return ERROR_DEVICE_NOT_CONNECTED;
    ds4_test_motors[slot] = *motors;
    return ERROR_SUCCESS;
}
