/**
 * Video settings (see video_settings.h).
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "video_settings.h"
#include "xbe_layout.h"   /* DOA3_XBE_ID_<version> */

extern HWND d3d8_GetHWND(void);
extern ptrdiff_t g_xbox_mem_offset;

static int g_window_mode = VIDEO_BORDERLESS;
static int g_aspect      = VIDEO_ASPECT_16_9;
static int g_scale       = 3;

int video_get_window_mode(void) { return g_window_mode; }
int video_get_aspect(void)      { return g_aspect; }
int video_get_scale(void)       { return g_scale; }

static int valid_aspect(int aspect)
{
    return aspect >= 0 && aspect < VIDEO_ASPECT_COUNT ? aspect : VIDEO_ASPECT_16_9;
}

const char *video_aspect_name(int aspect)
{
    static const char *names[] = {
        "4:3 (Full Screen)", "4:3 (Anamorphic)", "16:9", "21:9", "32:9"
    };
    return names[valid_aspect(aspect)];
}

float video_display_aspect(int aspect)
{
    switch (valid_aspect(aspect)) {
    case VIDEO_ASPECT_4_3_FULL: case VIDEO_ASPECT_4_3_ANAMORPHIC: return 4.0f / 3.0f;
    case VIDEO_ASPECT_21_9: return 21.0f / 9.0f;
    case VIDEO_ASPECT_32_9: return 32.0f / 9.0f;
    default: return 16.0f / 9.0f;
    }
}

float video_projection_ratio(int aspect)
{
    switch (valid_aspect(aspect)) {
    case VIDEO_ASPECT_4_3_FULL: return 3.0f / 4.0f;
    case VIDEO_ASPECT_21_9: return 9.0f / 21.0f;
    case VIDEO_ASPECT_32_9: return 9.0f / 32.0f;
    default: return 9.0f / 16.0f;
    }
}

const char *doa3_settings_ini_path(void)
{
    static char path[MAX_PATH];
    char *slash;
    if (path[0]) return path;
    if (!GetModuleFileNameA(NULL, path, MAX_PATH)) {
        strcpy_s(path, sizeof(path), "doa3_settings.ini");
        return path;
    }
    slash = strrchr(path, '\\');
    if (slash) slash[1] = 0; else path[0] = 0;
    strcat_s(path, sizeof(path), "doa3_settings.ini");
    return path;
}

int video_settings_load(void)
{
    const char *p = doa3_settings_ini_path();
    int wm = (int)GetPrivateProfileIntA("Video", "WindowMode", VIDEO_BORDERLESS, p);
    int legacy = (int)GetPrivateProfileIntA("Video", "Widescreen", 1, p);
    int ar = (int)GetPrivateProfileIntA("Video", "AspectRatio",
        legacy == 0 ? VIDEO_ASPECT_4_3_ANAMORPHIC : VIDEO_ASPECT_16_9, p);
    int sc = (int)GetPrivateProfileIntA("Video", "InternalResolution", 3, p);
    g_window_mode = (wm == VIDEO_WINDOWED) ? VIDEO_WINDOWED : VIDEO_BORDERLESS;
    g_aspect      = valid_aspect(ar);
    g_scale       = (sc >= 1 && sc <= 3) ? sc : 3;
    return GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES;
}

int video_settings_save(void)
{
    const char *p = doa3_settings_ini_path();
    char aspect[16];
    _snprintf_s(aspect, sizeof(aspect), _TRUNCATE, "%d", g_aspect);
    /* WindowMode: 0 = windowed, 1 = borderless fullscreen.
     * AspectRatio: Full 4:3, Anamorphic 4:3, 16:9, 21:9, 32:9 (0..4).
     * Keep Widescreen for older builds: 0 = either 4:3 mode, 1 = wider.
     * InternalResolution: 1 = 480 lines, 2 = 960, 3 = 1440. */
    return WritePrivateProfileStringA("Video", "WindowMode",
                                      g_window_mode ? "1" : "0", p) &&
           WritePrivateProfileStringA("Video", "Widescreen",
                                      g_aspect <= VIDEO_ASPECT_4_3_ANAMORPHIC ? "0" : "1", p) &&
           WritePrivateProfileStringA("Video", "AspectRatio", aspect, p) &&
           WritePrivateProfileStringA("Video", "InternalResolution",
                                      g_scale == 3 ? "3" : g_scale == 2 ? "2" : "1", p);
}

/* 480 lines at 1x. The translator maps every guest vertex onto whatever size
 * this target is, so a larger target rasterises the scene at that resolution
 * rather than scaling up a 480-line picture. */
static void video_base_size(int aspect, unsigned *w, unsigned *h)
{
    *h = 480;
    switch (valid_aspect(aspect)) {
    case VIDEO_ASPECT_4_3_FULL: case VIDEO_ASPECT_4_3_ANAMORPHIC: *w = 640; break;
    case VIDEO_ASPECT_21_9: *w = 1120; break;
    case VIDEO_ASPECT_32_9: *w = 1707; break;
    default: *w = 854; break;
    }
}

void video_guest_target_size(int aspect, unsigned *w, unsigned *h)
{
    video_base_size(aspect, w, h);
    *w *= (unsigned)g_scale;
    *h *= (unsigned)g_scale;
}

void video_set_scale(int scale)
{
    /* Applied at the next frame boundary, like the aspect. */
    g_scale = (scale >= 1 && scale <= 3) ? scale : 3;
}

/* XC_VIDEO dword: XGetVideoFlags returns (value >> 16) & 0x5F, and
 * XC_VIDEO_FLAGS_WIDESCREEN is bit 0 of that. */
unsigned video_xc_video_value(void)
{
    return g_aspect != VIDEO_ASPECT_4_3_FULL ? 0x00010000u : 0u;
}

void video_set_aspect(int aspect)
{
    /* The d3d8 layer notices the change at the next frame boundary and
     * rebuilds the guest target (d3d8_device.c, present path). */
    g_aspect = valid_aspect(aspect);
}

/* Apply projection data before guest startup and at native frame boundaries.
 * D3D init (3.1 sub_0017B8D0; 3.0 sub_001539C0) caches XC_VIDEO in three
 * places: a projection flag, present flags, and raw video flags. Before init
 * has set the guest present width to 720, leave those caches alone; the
 * kernel query returns the requested setting directly. */
void video_sync_guest_widescreen(void)
{
#if defined(DOA3_XBE_ID_3_1)
    volatile uint8_t *m = (volatile uint8_t *)(uintptr_t)g_xbox_mem_offset;
    volatile uint32_t *pp_w, *pp_fl, *vflag, *ws;
    if (!g_xbox_mem_offset) return;
    /* Retail Japanese 3.1: file 0x244C20 = VA 0x251620 in .rdata.
     * 00 00 10 3F is the complete little-endian float 9/16; write all
     * four bytes so non-exact ratios such as 9/21 retain their mantissa.
     * Projection routines (e.g. sub_00027720) read this through MEMF. */
    *(volatile float *)(m + 0x251620) = video_projection_ratio(g_aspect);
    /* sub_0017B8D0 caches XC_VIDEO here. Preserve all unrelated flag bits. */
    pp_w = (volatile uint32_t *)(m + 0xA48C30);
    pp_fl = (volatile uint32_t *)(m + 0xA48C58);
    vflag = (volatile uint32_t *)(m + 0xA48C78);
    ws = (volatile uint32_t *)(m + 0xA48CDC);
#elif defined(DOA3_XBE_ID_3_0)
    volatile uint8_t *m = (volatile uint8_t *)(uintptr_t)g_xbox_mem_offset;
    volatile uint32_t *pp_w  = (volatile uint32_t *)(m + 0x90F4A8);
    volatile uint32_t *pp_fl = (volatile uint32_t *)(m + 0x90F4D0);
    volatile uint32_t *vflag = (volatile uint32_t *)(m + 0x90F4E0);
    volatile uint32_t *ws    = (volatile uint32_t *)(m + 0x90F544);
#else
    return;
#endif
#if defined(DOA3_XBE_ID_3_1) || defined(DOA3_XBE_ID_3_0)
    int on = g_aspect != VIDEO_ASPECT_4_3_FULL;
    if (!g_xbox_mem_offset || *pp_w != 0x2D0u) return;
    if (on) { *ws |= 1u;  *pp_fl |= 0x10u;  *vflag |= 1u; }
    else    { *ws &= ~1u; *pp_fl &= ~0x10u; *vflag &= ~1u; }
#endif
}

void video_apply_window_mode(HWND hwnd, int mode)
{
    MONITORINFO mi;
    HMONITOR mon;
    if (!hwnd) return;
    mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
    memset(&mi, 0, sizeof(mi));
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoA(mon, &mi)) return;

    if (mode == VIDEO_BORDERLESS) {
        RECT r = mi.rcMonitor;
        SetWindowLongPtrA(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(hwnd, HWND_TOP, r.left, r.top, r.right - r.left, r.bottom - r.top,
                     SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    } else {
        unsigned cw, ch;
        RECT r, wa = mi.rcWork;
        video_base_size(g_aspect, &cw, &ch);
        r.left = 0; r.top = 0; r.right = (LONG)cw; r.bottom = (LONG)ch;
        AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
        SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(hwnd, HWND_NOTOPMOST,
                     wa.left + ((wa.right - wa.left) - (r.right - r.left)) / 2,
                     wa.top + ((wa.bottom - wa.top) - (r.bottom - r.top)) / 2,
                     r.right - r.left, r.bottom - r.top,
                     SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
}

void video_set_window_mode(int mode)
{
    g_window_mode = (mode == VIDEO_WINDOWED) ? VIDEO_WINDOWED : VIDEO_BORDERLESS;
    video_apply_window_mode(d3d8_GetHWND(), g_window_mode);
}
