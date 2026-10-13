#include "free_camera.h"
#include "free_camera_math.h"
#include "input/xinput_xbox.h"
#include "online/netplay_session.h"
#include <xinput.h>

static FreeCameraPose s_pose, s_normal;
static unsigned s_index, s_normal_index;
static int s_enabled, s_have_normal, s_captured, s_registered;
static float s_speed = 10.0f;     /* default; the slider spans 1-20 */
static float s_fov_degrees, s_normal_fov = 60.0f;
static LONG s_mouse_x, s_mouse_y;
static LARGE_INTEGER s_last, s_frequency;

int free_camera_enabled(void) { return s_enabled; }
float free_camera_speed(void) { return s_speed; }
void free_camera_set_speed(float speed) { s_speed = fmaxf(1, fminf(20, speed)); }
float free_camera_fov(void) { return s_fov_degrees > 0 ? s_fov_degrees : s_normal_fov; }
void free_camera_set_fov(float degrees)
{
    if (isfinite(degrees)) s_fov_degrees = fmaxf(15, fminf(120, degrees));
}
void free_camera_reset_fov(void) { s_fov_degrees = 0; }
float free_camera_projection_fov(unsigned index, float radians)
{
    const float to_degrees = 57.2957795131f;
    if (netplay_active() || !isfinite(radians) || radians <= 0 || radians >= 3.14159265f)
        return radians;
    /* Preserve auxiliary views while flying, just like the pose override. */
    if (s_enabled && index != s_index) return radians;
    s_normal_fov = radians * to_degrees;
    return s_fov_degrees > 0 ? s_fov_degrees / to_degrees : radians;
}

void free_camera_release_mouse(void)
{
    if (s_captured) {
        ClipCursor(NULL);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
    s_captured = 0;
    s_mouse_x = s_mouse_y = 0;
}

void free_camera_reset(void)
{
    if (s_have_normal) { s_pose = s_normal; s_index = s_normal_index; }
    s_mouse_x = s_mouse_y = 0;
}

int free_camera_set_enabled(int enabled)
{
    if (enabled && (!s_have_normal || netplay_active())) return 0;
    if (enabled && !s_enabled) free_camera_reset();
    s_enabled = !!enabled;
    xbox_InputSetCaptured(s_enabled);
    if (!s_enabled) free_camera_release_mouse();
    QueryPerformanceCounter(&s_last);
    return 1;
}

int free_camera_view(unsigned index, const float eye[3], const float target[3],
                     float out_eye[3], float out_target[3])
{
    FreeCameraPose normal;
    float forward[3];
    if (netplay_active()) {
        if (s_enabled) free_camera_set_enabled(0);
        return 0;
    }
    if (!free_camera_pose_from_view(&normal, eye, target)) return 0;
    /* Auxiliary camera slots must keep their own views. */
    if (s_enabled && index != s_index) return 0;
    s_normal = normal; s_normal_index = index; s_have_normal = 1;
    if (!s_enabled) return 0;
    free_camera_forward(&s_pose, forward);
    for (int i = 0; i < 3; ++i) {
        out_eye[i] = s_pose.eye[i];
        out_target[i] = s_pose.eye[i] + forward[i] * 100.0f;
    }
    return 1;
}

int free_camera_wndproc(HWND window, UINT message, WPARAM wp, LPARAM lp)
{
    (void)window;
    if ((message == WM_ACTIVATEAPP && !wp) || message == WM_KILLFOCUS ||
        message == WM_CLOSE || message == WM_DESTROY) free_camera_release_mouse();
    if (message == WM_SETCURSOR && s_captured && LOWORD(lp) == HTCLIENT) {
        SetCursor(NULL);
        return 1;
    }
    if (message == WM_INPUT && s_captured) {
        RAWINPUT input;
        UINT size = sizeof(input);
        if (GetRawInputData((HRAWINPUT)lp, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER)) != (UINT)-1 &&
            input.header.dwType == RIM_TYPEMOUSE && !(input.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)) {
            s_mouse_x += input.data.mouse.lLastX;
            s_mouse_y += input.data.mouse.lLastY;
        }
        /* DefWindowProc must still perform WM_INPUT foreground cleanup. */
    }
    return 0;
}

static float key(int code) { return (GetAsyncKeyState(code) & 0x8000) ? 1.0f : 0.0f; }

void free_camera_tick(HWND window, int menu_open)
{
    LARGE_INTEGER now;
    XINPUT_STATE pad = {0};
    float dt, lx = 0, ly = 0, rx = 0, ry = 0, vertical = 0, boost = 1;
    if (!s_frequency.QuadPart) QueryPerformanceFrequency(&s_frequency);
    QueryPerformanceCounter(&now);
    dt = s_last.QuadPart ? (float)((double)(now.QuadPart - s_last.QuadPart) / s_frequency.QuadPart) : 0;
    s_last = now;
    if (s_enabled && netplay_active()) free_camera_set_enabled(0);
    if (!s_enabled || menu_open || GetForegroundWindow() != window || IsIconic(window)) {
        free_camera_release_mouse();
        return;
    }
    if (!s_registered) {
        RAWINPUTDEVICE device = {1, 2, 0, window};
        s_registered = RegisterRawInputDevices(&device, 1, sizeof(device)) != 0;
    }
    {
        RECT rect;
        GetClientRect(window, &rect);
        MapWindowPoints(window, NULL, (POINT *)&rect, 2);
        ClipCursor(&rect);
        SetCursor(NULL);
    }
    if (!s_captured) { s_mouse_x = s_mouse_y = 0; dt = 0; }
    s_captured = 1;
    dt = fmaxf(0, fminf(dt, 0.05f));
    if (xbox_InputGetRawState(&pad) == ERROR_SUCCESS) {
        free_camera_stick(pad.Gamepad.sThumbLX, pad.Gamepad.sThumbLY, 0.24f, &lx, &ly);
        free_camera_stick(pad.Gamepad.sThumbRX, pad.Gamepad.sThumbRY, 0.24f, &rx, &ry);
        vertical = (fmaxf(0, pad.Gamepad.bRightTrigger - 30.0f) -
                    fmaxf(0, pad.Gamepad.bLeftTrigger - 30.0f)) / 225.0f;
        if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) boost = 4;
        if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) boost *= 0.25f;
    }
    if (key(VK_SHIFT)) boost *= 4;
    if (key(VK_CONTROL)) boost *= 0.25f;
    free_camera_step(&s_pose, dt, s_speed * boost,
        lx + key('D') - key('A'), ly + key('W') - key('S'), vertical + key('E') - key('Q'),
        s_mouse_x * 0.0025f + rx * 2.2f * dt, -s_mouse_y * 0.0025f + ry * 2.2f * dt);
    s_mouse_x = s_mouse_y = 0;
}
