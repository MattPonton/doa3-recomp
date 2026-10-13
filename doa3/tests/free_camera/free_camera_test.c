#include "free_camera.h"
#include "free_camera_math.h"
#include "input/xinput_xbox.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
#define CHECK_NEAR(a,b) CHECK(fabsf((a)-(b)) < 0.001f)
static int online, captured, builds, activations;
static float expected_fov = 1.0471975512f;
int netplay_active(void) { return online; }
void xbox_InputSetCaptured(BOOL value) { captured = value; }
DWORD xbox_InputGetRawState(XINPUT_STATE *state) { memset(state, 0, sizeof(*state)); return ERROR_DEVICE_NOT_CONNECTED; }

/* Guarded fake guest memory and calling convention, with real override code. */
static unsigned char ram[0x600000];
static uint32_t eax, ecx, edx, ebx, esi, edi, esp, g_seh_ebp;
static double g_fp_stack[8];
static int g_fp_top;
static uint16_t g_x87_cw;
#define XBOX_PTR(a) (ram + (a))
#define MEM8(a) (*(uint8_t *)XBOX_PTR(a))
#define MEMF(a) (*(float *)XBOX_PTR(a))
#define PUSH32(s,v) do { (s) -= 4; *(uint32_t *)XBOX_PTR(s) = (v); } while (0)
#include "camera_hook.inc"

void sub_00027720_gen(void)
{
    ++builds;
    CHECK(eax == 0x4DAE90);
    CHECK_NEAR(MEMF(eax + 0xA8), 10);
    CHECK_NEAR(MEMF(eax + 0xBC), 20);
    CHECK_NEAR(MEMF(eax + 0xC0), free_camera_enabled() ? -70 : -10);
    CHECK_NEAR(MEMF(eax + 0xDC), free_camera_enabled() ? 0 : 0.2f);
    CHECK_NEAR(MEMF(eax + 0xD8), expected_fov);
    for (unsigned i = 0x150; i < 0x210; i += 4) MEMF(eax + i) = 42;
    eax = ecx = edx = ebx = esi = edi = g_seh_ebp = 999;
    g_fp_top = 3; g_x87_cw = 0;
    for (int i = 0; i < 8; ++i) g_fp_stack[i] = 999;
    esp += 4;
}
void sub_00029E30_gen(void)
{
    ++activations;
    CHECK(eax == 11 && ecx == 12 && edx == 13 && esi == 14 && edi == 15);
    CHECK(g_seh_ebp == 16 && g_fp_top == 7 && g_x87_cw == 0x27F);
    for (int i = 0; i < 8; ++i) CHECK(g_fp_stack[i] == i + 0.5);
    if (free_camera_enabled() && ebx == 0) {
        CHECK_NEAR(MEMF(0x4DAE90 + 0x150), 42);
        CHECK_NEAR(MEMF(0x4DAE90 + 0xC0), -70);
    }
    eax = 1; esp += 4;
}
static void registers(void)
{
    eax = 11; ecx = 12; edx = 13; esi = 14; edi = 15; g_seh_ebp = 16;
    ebx = 0; esp = 0x10000; g_fp_top = 7; g_x87_cw = 0x27F;
    for (int i = 0; i < 8; ++i) g_fp_stack[i] = i + 0.5;
}
int main(void)
{
    float eye[3] = {10,20,30}, target[3] = {10,20,-10}, f[3], x, y;
    FreeCameraPose p, a, b;
    CHECK(free_camera_pose_from_view(&p, eye, target));
    free_camera_forward(&p, f);
    CHECK_NEAR(f[0], 0); CHECK_NEAR(f[1], 0); CHECK_NEAR(f[2], -1);
    a = b = p;
    for (int i = 0; i < 60; ++i) free_camera_step(&a, 1.0f/60, 60, 0, 1, 0, 0, 0);
    for (int i = 0; i < 120; ++i) free_camera_step(&b, 1.0f/120, 60, 0, 1, 0, 0, 0);
    CHECK_NEAR(a.eye[2], -30); CHECK_NEAR(a.eye[2], b.eye[2]);
    a = p;
    free_camera_step(&a, 0.05f, 100, 1, 1, 0, 0, 0);
    CHECK_NEAR(hypotf(a.eye[0]-10, a.eye[2]-30), 5);
    CHECK(a.eye[0] > 10 && a.eye[2] < 30);
    a = p;
    free_camera_step(&a, 10, 100, 0, 0, 1, 0, 0);
    CHECK_NEAR(a.eye[1], 25); /* stalled frame capped */
    free_camera_step(&a, 0, 0, 0, 0, 0, 1000, 1000);
    CHECK(fabsf(a.yaw) <= 3.142f && a.pitch < 1.571f);
    free_camera_stick(1000, -1000, 0.24f, &x, &y);
    CHECK_NEAR(x, 0); CHECK_NEAR(y, 0);
    free_camera_stick(32767, 32767, 0.24f, &x, &y);
    CHECK_NEAR(hypotf(x,y), 1);
    free_camera_stick(-32768, 0, 0.24f, &x, &y);
    CHECK_NEAR(x, -1); CHECK_NEAR(y, 0);
    CHECK(!free_camera_pose_from_view(&a, eye, eye));
    target[0] = NAN;
    CHECK(!free_camera_pose_from_view(&a, eye, target));

    CHECK(!free_camera_set_enabled(1)); /* no valid game view yet */
    for (unsigned i = 0; i < 0xC80; ++i) ram[0x4DAE90 + i] = (unsigned char)i;
    MEM8(0x4DAE90 + 0xABC) = 1;
    MEMF(0x4DAE90 + 0xD8) = expected_fov;
    MEMF(0x4DAE90 + 0xDC) = 0.2f;
    for (int i = 0; i < 3; ++i) {
        MEMF(0x4DAE90 + 0xA8 + i*4) = eye[i];
        MEMF(0x4DAE90 + 0xB8 + i*4) = eye[i] - (i == 2 ? 40 : 0);
    }
    unsigned char original[0xC80];
    memcpy(original, ram + 0x4DAE90, sizeof(original));
    registers(); sub_00029E30();
    CHECK(builds == 0 && activations == 1 && esp == 0x10004);
    CHECK(free_camera_set_enabled(1) && captured);
    registers(); sub_00029E30();
    CHECK(builds == 1 && activations == 2 && esp == 0x10004 && eax == 1);
    CHECK(memcmp(original, ram + 0x4DAE90, sizeof(original)) == 0);
    registers(); ebx = 0x7FFFFFFF; sub_00029E30();
    CHECK(builds == 1 && esp == 0x10004); /* upload-current-view path untouched */
    registers(); ebx = 0xFFFFFFFF; sub_00029E30();
    CHECK(builds == 1 && esp == 0x10004); /* invalid index untouched */
    float out_eye[3], out_target[3];
    target[0] = 10;
    CHECK(!free_camera_view(1, eye, target, out_eye, out_target)); /* auxiliary camera */
    free_camera_set_enabled(0);
    CHECK(!captured);
    registers(); sub_00029E30();
    CHECK(builds == 1 && memcmp(original, ram + 0x4DAE90, sizeof(original)) == 0);
    /* FOV-only override rebuilds the native camera while preserving its pose,
     * scripted roll, source data and guest ABI. No input capture required. */
    CHECK_NEAR(free_camera_fov(), 60);
    free_camera_set_fov(90);
    expected_fov = 1.5707963268f;
    registers(); sub_00029E30();
    CHECK(builds == 2 && !captured && !free_camera_enabled() && esp == 0x10004);
    CHECK(memcmp(original, ram + 0x4DAE90, sizeof(original)) == 0);
    free_camera_reset_fov();
    registers(); sub_00029E30();
    CHECK(builds == 2); /* reset resumes original projection without rebuilding */
    CHECK_NEAR(free_camera_fov(), 60);
    free_camera_set_fov(0); CHECK_NEAR(free_camera_fov(), 15);
    free_camera_set_fov(180); CHECK_NEAR(free_camera_fov(), 120);
    free_camera_set_fov(NAN); CHECK_NEAR(free_camera_fov(), 120);
    CHECK_NEAR(free_camera_projection_fov(0, 1), 2.0943951024f);
    CHECK_NEAR(free_camera_projection_fov(0, 0), 0);
    free_camera_set_fov(90);
    CHECK(free_camera_set_enabled(1));
    registers(); sub_00029E30();
    CHECK(builds == 3); /* FOV also applies alongside the flying pose */
    CHECK(memcmp(original, ram + 0x4DAE90, sizeof(original)) == 0);
    CHECK_NEAR(free_camera_projection_fov(1, 1), 1); /* auxiliary camera untouched */
    eye[0] = 20;
    CHECK(free_camera_view(0, eye, target, out_eye, out_target));
    CHECK_NEAR(out_eye[0], 10); /* scripted camera motion does not move free camera */
    free_camera_reset();
    CHECK(free_camera_view(0, eye, target, out_eye, out_target));
    CHECK_NEAR(out_eye[0], 20);
    free_camera_set_speed(0); CHECK_NEAR(free_camera_speed(), 1);
    free_camera_set_speed(9000); CHECK_NEAR(free_camera_speed(), 20);
    online = 1;
    CHECK_NEAR(free_camera_projection_fov(0, 1), 1);
    CHECK(!free_camera_view(0, eye, target, out_eye, out_target));
    CHECK(!free_camera_enabled() && !captured && !free_camera_set_enabled(1));
    puts("Camera movement, independent FOV/reset, native hook/ABI restoration, auxiliary views, and online gating passed.");
    return 0;
}
