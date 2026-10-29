#ifndef DOA3_DS4_DEVICE_H
#define DOA3_DS4_DEVICE_H
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>
#include <xinput.h>

/* Four native DirectInput slots, independent of Windows' four XInput slots:
 * PlayStation controllers (DualShock 4, DualSense, DualSense Edge) and,
 * since 1.0.2, any other DirectInput gamepad or joystick that Windows does
 * not also offer through XInput (arcade sticks, fight pads, older pads). */
/* Scan at startup or after a device notification; return 1 after a scan. */
int ds4_probe(void);
void ds4_devices_changed(void);
DWORD ds4_read(int slot, XINPUT_STATE *state);
DWORD ds4_set_rumble(int slot, const XINPUT_VIBRATION *motors);
int ds4_has_rumble(int slot);
/* "PS4 controller", "PS5 controller", or a DirectInput pad's product name. */
const char *ds4_model_name(int slot);
void ds4_translate(const DIJOYSTATE2 *raw, XINPUT_GAMEPAD *pad);
#endif
