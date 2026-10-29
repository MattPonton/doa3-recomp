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
    int ps5;   /* DualSense or DualSense Edge */
    /* 1.0.2: any other DirectInput gamepad or joystick (not one that Windows
     * also offers through XInput): arcade sticks, fight pads, older pads. */
    int generic;
    int axes;          /* bit n: axis n present (X Y Z Rx Ry Rz) */
    char name[48];
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

/* 1.0.2: a generic DirectInput pad. Most arcade sticks and fight pads in
 * DirectInput (PS3) mode report the PlayStation order: Square Cross Circle
 * Triangle L1 R1 L2 R2 Select Start L3 R3, hat = d-pad, so the same face
 * button mapping applies; L2 / R2 are buttons, pressed = full trigger. A
 * right stick is Z / Rz when the pad has them, otherwise Rx / Ry. Axes the
 * pad does not have stay centred. Everything can be rebound in Controls. */
static void generic_translate(const DIJOYSTATE2 *raw, int axes, XINPUT_GAMEPAD *pad)
{
    ds4_translate(raw, pad);
    pad->bLeftTrigger = (raw->rgbButtons[6] & 0x80) ? 255 : 0;
    pad->bRightTrigger = (raw->rgbButtons[7] & 0x80) ? 255 : 0;
    if (!(axes & 1)) pad->sThumbLX = 0;
    if (!(axes & 2)) pad->sThumbLY = 0;
    if ((axes & 4) && (axes & 32)) {
        /* Z / Rz: already the right stick */
    } else if ((axes & 8) && (axes & 16)) {
        pad->sThumbRX = stick(raw->lRx, 0);
        pad->sThumbRY = stick(raw->lRy, 1);
    } else {
        pad->sThumbRX = pad->sThumbRY = 0;
    }
}

typedef struct { int axes; } AxisPresence;
static BOOL CALLBACK note_axis(const DIDEVICEOBJECTINSTANCEA *object, void *context)
{
    AxisPresence *p = context;
    if (IsEqualGUID(&object->guidType, &GUID_XAxis))  p->axes |= 1;
    if (IsEqualGUID(&object->guidType, &GUID_YAxis))  p->axes |= 2;
    if (IsEqualGUID(&object->guidType, &GUID_ZAxis))  p->axes |= 4;
    if (IsEqualGUID(&object->guidType, &GUID_RxAxis)) p->axes |= 8;
    if (IsEqualGUID(&object->guidType, &GUID_RyAxis)) p->axes |= 16;
    if (IsEqualGUID(&object->guidType, &GUID_RzAxis)) p->axes |= 32;
    return DIENUM_CONTINUE;
}

/* Windows lists every XInput controller in DirectInput too; its device path
 * contains "IG_". Those are read through XInput. */
static int is_xinput_device(IDirectInputDevice8A *device)
{
    DIPROPGUIDANDPATH path = {0};
    path.diph.dwSize = sizeof(path);
    path.diph.dwHeaderSize = sizeof(path.diph);
    path.diph.dwHow = DIPH_DEVICE;
    if (FAILED(IDirectInputDevice8_GetProperty(device, DIPROP_GUIDANDPATH, &path.diph))) return 0;
    return wcsstr(path.wszPath, L"IG_") != NULL || wcsstr(path.wszPath, L"ig_") != NULL;
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
    int ps5 = product == 0x0ce6 || product == 0x0df2;   /* DualSense, DualSense Edge */
    /* Restrict this mapping to Sony DS4 and DualSense hardware; never
     * double-enumerate Xbox pads. Through DirectInput both report the same
     * layout: buttons Square Cross Circle Triangle L1 R1 L2 R2
     * Share/Create Options L3 R3, hat = d-pad, X/Y left stick, Z/Rz right
     * stick, Rx/Ry triggers. */
    int sony = vendor == 0x054c && (product == 0x05c4 || product == 0x09cc || product == 0x0ba0 || ps5);
    int kind = GET_DIDEVICE_TYPE(info->dwDevType);
    if (!sony && kind != DI8DEVTYPE_GAMEPAD && kind != DI8DEVTYPE_JOYSTICK)
        return DIENUM_CONTINUE;
    for (int i = 0; i < 4; ++i)
        if (s_pads[i].known && IsEqualGUID(&s_pads[i].id, &info->guidInstance)) { slot = i; break; }
    if (slot < 0) for (int i = 0; i < 4; ++i)
        if (!s_pads[i].known) { slot = i; break; }
    if (slot < 0) return DIENUM_CONTINUE;
    s_pads[slot].seen = 1;
    if (s_pads[slot].device) return DIENUM_CONTINUE;
    hr = IDirectInput8_CreateDevice(s_input, &info->guidInstance, &device, NULL);
    if (FAILED(hr)) return DIENUM_CONTINUE;
    if (!sony && is_xinput_device(device)) {
        /* read through XInput instead; forget the slot we reserved */
        IDirectInputDevice8_Release(device);
        s_pads[slot].seen = 0;
        return DIENUM_CONTINUE;
    }
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
    s_pads[slot].ps5 = ps5;
    s_pads[slot].generic = !sony;
    {   AxisPresence presence = {0};
        IDirectInputDevice8_EnumObjects(device, note_axis, &presence, DIDFT_AXIS);
        s_pads[slot].axes = presence.axes;
    }
    _snprintf_s(s_pads[slot].name, sizeof s_pads[slot].name, _TRUNCATE, "%s",
                info->tszProductName[0] ? info->tszProductName : "Controller");
    memset(&s_pads[slot].state, 0, sizeof(s_pads[slot].state));
    IDirectInputDevice8_Acquire(device);
    DIPROPGUIDANDPATH path = {0};
    path.diph.dwSize = sizeof(path);
    path.diph.dwHeaderSize = sizeof(path.diph);
    path.diph.dwHow = DIPH_DEVICE;
    if (sony && SUCCEEDED(IDirectInputDevice8_GetProperty(device, DIPROP_GUIDANDPATH, &path.diph)))
        s_pads[slot].rumble = ds4_rumble_open(path.wszPath);
    if (sony)
        fprintf(stderr, "[INPUT] %s %d connected: %s (USB rumble %s)\n",
            ps5 ? "PS5 controller" : "PS4 controller", slot + 1, info->tszProductName,
            s_pads[slot].rumble ? "available" : "unavailable");
    else
        fprintf(stderr, "[INPUT] DirectInput controller %d connected: %s (vendor %04X product %04X, axes %02X)\n",
            slot + 1, info->tszProductName, vendor, product, s_pads[slot].axes);
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
    if (s_pads[slot].generic) generic_translate(&raw, s_pads[slot].axes, &pad);
    else ds4_translate(&raw, &pad);
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

const char *ds4_model_name(int slot)
{
    if (slot < 0 || slot >= 4 || !s_pads[slot].device) return "PlayStation controller";
    if (s_pads[slot].generic) return s_pads[slot].name;
    return s_pads[slot].ps5 ? "PS5 controller" : "PS4 controller";
}
