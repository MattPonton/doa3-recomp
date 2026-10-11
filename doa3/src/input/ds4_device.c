#define COBJMACROS
#include "ds4_device.h"
#include "ds4_rumble.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static IDirectInput8A *s_input;
static volatile LONG s_discovery_pending = 1;
static struct {
    GUID id;
    int known, seen;
    IDirectInputDevice8A *device;
    XINPUT_STATE state;
    Ds4Rumble *rumble;
} s_pads[4];

static SHORT stick(LONG value, int invert)
{
    LONG v = value < 0 ? 0 : value > 65535 ? 65535 : value;
    return (SHORT)(invert ? 32767 - v : v - 32768);
}

void ds4_translate(const DIJOYSTATE2 *raw, XINPUT_GAMEPAD *pad)
{
    static const WORD buttons[12] = {
        XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_Y,
        XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, 0, 0,
        XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START,
        XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB
    };
    static const WORD hats[8] = {1, 9, 8, 10, 2, 6, 4, 5};
    memset(pad, 0, sizeof(*pad));
    for (int i = 0; i < 12; ++i)
        if (raw->rgbButtons[i] & 0x80) pad->wButtons |= buttons[i];
    if (raw->rgdwPOV[0] < 36000) pad->wButtons |= hats[((raw->rgdwPOV[0] + 2250) / 4500) % 8];
    pad->sThumbLX = stick(raw->lX, 0);
    pad->sThumbLY = stick(raw->lY, 1);
    pad->sThumbRX = stick(raw->lZ, 0);
    pad->sThumbRY = stick(raw->lRz, 1);
    pad->bLeftTrigger = (BYTE)((raw->lRx < 0 ? 0 : raw->lRx > 65535 ? 65535 : raw->lRx) / 257);
    pad->bRightTrigger = (BYTE)((raw->lRy < 0 ? 0 : raw->lRy > 65535 ? 65535 : raw->lRy) / 257);
}

static void close_pad(int slot)
{
    ds4_rumble_close(s_pads[slot].rumble);
    s_pads[slot].rumble = NULL;
    if (s_pads[slot].device) {
        IDirectInputDevice8_Unacquire(s_pads[slot].device);
        IDirectInputDevice8_Release(s_pads[slot].device);
        s_pads[slot].device = NULL;
    }
}

static void shutdown_ds4(void)
{
    for (int i = 0; i < 4; ++i) close_pad(i);
    if (s_input) { IDirectInput8_Release(s_input); s_input = NULL; }
}

typedef struct { IDirectInputDevice8A *device; HRESULT result; } AxisContext;
static BOOL CALLBACK set_axis_range(const DIDEVICEOBJECTINSTANCEA *object, void *context)
{
    DIPROPRANGE range = {{sizeof(range), sizeof(range.diph), object->dwType, DIPH_BYID}, 0, 65535};
    AxisContext *axes = context;
    HRESULT hr = IDirectInputDevice8_SetProperty(axes->device, DIPROP_RANGE, &range.diph);
    if (FAILED(hr)) axes->result = hr;
    return DIENUM_CONTINUE;
}

static BOOL CALLBACK found_pad(const DIDEVICEINSTANCEA *info, void *context)
{
    WORD vendor = LOWORD(info->guidProduct.Data1), product = HIWORD(info->guidProduct.Data1);
    int slot = -1;
    IDirectInputDevice8A *device = NULL;
    HRESULT hr;
    (void)context;
    /* Restrict this mapping to Sony DS4 hardware; never double-enumerate Xbox pads. */
    if (vendor != 0x054c || (product != 0x05c4 && product != 0x09cc && product != 0x0ba0)) return DIENUM_CONTINUE;
    for (int i = 0; i < 4; ++i)
        if (s_pads[i].known && IsEqualGUID(&s_pads[i].id, &info->guidInstance)) { slot = i; break; }
    if (slot < 0) for (int i = 0; i < 4; ++i)
        if (!s_pads[i].known) { slot = i; break; }
    if (slot < 0) return DIENUM_CONTINUE;
    s_pads[slot].seen = 1;
    if (s_pads[slot].device) return DIENUM_CONTINUE;
    hr = IDirectInput8_CreateDevice(s_input, &info->guidInstance, &device, NULL);
    if (FAILED(hr)) return DIENUM_CONTINUE;
    hr = IDirectInputDevice8_SetDataFormat(device, &c_dfDIJoystick2);
    if (SUCCEEDED(hr)) hr = IDirectInputDevice8_SetCooperativeLevel(device, NULL, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    if (SUCCEEDED(hr)) {
        AxisContext axes = {device, S_OK};
        hr = IDirectInputDevice8_EnumObjects(device, set_axis_range, &axes, DIDFT_AXIS);
        if (SUCCEEDED(hr)) hr = axes.result;
    }
    if (FAILED(hr)) { IDirectInputDevice8_Release(device); return DIENUM_CONTINUE; }
    s_pads[slot].id = info->guidInstance;
    s_pads[slot].known = 1;
    s_pads[slot].device = device;
    memset(&s_pads[slot].state, 0, sizeof(s_pads[slot].state));
    IDirectInputDevice8_Acquire(device);
    DIPROPGUIDANDPATH path = {0};
    path.diph.dwSize = sizeof(path);
    path.diph.dwHeaderSize = sizeof(path.diph);
    path.diph.dwHow = DIPH_DEVICE;
    if (SUCCEEDED(IDirectInputDevice8_GetProperty(device, DIPROP_GUIDANDPATH, &path.diph)))
        s_pads[slot].rumble = ds4_rumble_open(path.wszPath);
    fprintf(stderr, "[INPUT] PS4 controller %d connected: %s (USB rumble %s)\n",
        slot + 1, info->tszProductName, s_pads[slot].rumble ? "available" : "unavailable");
    return DIENUM_CONTINUE;
}

void ds4_devices_changed(void)
{
    /* The window callback only queues discovery; never enumerate inside it. */
    InterlockedExchange(&s_discovery_pending, 1);
}

int ds4_probe(void)
{
    static int initialized, probed;
    static DWORD last;
    DWORD now = GetTickCount();
    if (!initialized) {
        initialized = 1;
        if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
            &IID_IDirectInput8A, (void **)&s_input, NULL))) return 0;
        atexit(shutdown_ds4);
    }
    /* EnumDevices can block for several frames even with no DS4 attached.
     * Coalesce hotplug bursts, but never rescan merely because time passed. */
    if (!s_input || (probed && now - last < 250)) return 0;
    if (!InterlockedExchange(&s_discovery_pending, 0)) return 0;
    probed = 1; last = now;
    for (int i = 0; i < 4; ++i) s_pads[i].seen = 0;
    if (SUCCEEDED(IDirectInput8_EnumDevices(s_input, DI8DEVCLASS_GAMECTRL, found_pad, NULL, DIEDFL_ATTACHEDONLY)))
        for (int i = 0; i < 4; ++i) if (!s_pads[i].seen) close_pad(i);
    return 1;
}

DWORD ds4_read(int slot, XINPUT_STATE *state)
{
    DIJOYSTATE2 raw;
    XINPUT_GAMEPAD pad;
    HRESULT hr;
    if (slot < 0 || slot >= 4 || !state || !s_pads[slot].device) return ERROR_DEVICE_NOT_CONNECTED;
    hr = IDirectInputDevice8_Poll(s_pads[slot].device);
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
        IDirectInputDevice8_Acquire(s_pads[slot].device);
        hr = IDirectInputDevice8_Poll(s_pads[slot].device);
    }
    if (SUCCEEDED(hr)) hr = IDirectInputDevice8_GetDeviceState(s_pads[slot].device, sizeof(raw), &raw);
    if (FAILED(hr)) {
        if (hr == DIERR_UNPLUGGED || hr == DIERR_DEVICENOTREG) close_pad(slot);
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    ds4_translate(&raw, &pad);
    if (memcmp(&pad, &s_pads[slot].state.Gamepad, sizeof(pad))) ++s_pads[slot].state.dwPacketNumber;
    s_pads[slot].state.Gamepad = pad;
    *state = s_pads[slot].state;
    return ERROR_SUCCESS;
}

int ds4_has_rumble(int slot)
{
    return slot >= 0 && slot < 4 && s_pads[slot].rumble != NULL;
}

DWORD ds4_set_rumble(int slot, const XINPUT_VIBRATION *motors)
{
    if (slot < 0 || slot >= 4 || !s_pads[slot].device) return ERROR_DEVICE_NOT_CONNECTED;
    if (!motors) return ERROR_INVALID_PARAMETER;
    return ds4_rumble_set(s_pads[slot].rumble, motors->wLeftMotorSpeed, motors->wRightMotorSpeed);
}
