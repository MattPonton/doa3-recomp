#include "debug_settings.h"
#include "debug_capture.h"
#include "../perf_capture.h"
#include "video_settings.h"
#include "recomp/recomp_types.h"
#include "recomp/gen/xbe_layout.h"
#include "online/netplay_session.h"
#include <windows.h>

static volatile LONG g_unlock_all;
static unsigned g_hide_players;
static int g_hide_ui, g_screenshot_key = VK_F12;
static volatile LONG g_auto_capture;

/* DOA3_AUTO_CAPTURE=1/0 in the environment wins over the settings file, so
 * a one-off developer launch does not need the ini edited. */
static int auto_capture_env(void)
{
    static int env = -2;
    if (env == -2) {
        const char *v = getenv("DOA3_AUTO_CAPTURE");
        env = (v && *v) ? (strcmp(v, "0") != 0) : -1;
    }
    return env;
}
int doa3_auto_capture_enabled(void)
{
    int env = auto_capture_env();
    if (env >= 0) return env;
    return (int)InterlockedCompareExchange(&g_auto_capture, 0, 0);
}
void doa3_auto_capture_set(int enabled)
{
    /* Diagnostics also drive the frame/draw timing (DOA3_PERF_CAPTURE), live.
     * A variable the user set before launch is left alone. */
    static int s_user_env = -1;
    InterlockedExchange(&g_auto_capture, enabled != 0);
    if (s_user_env < 0) s_user_env = GetEnvironmentVariableA("DOA3_PERF_CAPTURE", NULL, 0) != 0;
    if (!s_user_env) _putenv_s("DOA3_PERF_CAPTURE", doa3_auto_capture_enabled() ? "1" : "");
    doa3_perf_refresh();
}

int doa3_diag_clear(void)
{
    WCHAR dir[MAX_PATH], pattern[MAX_PATH], file[MAX_PATH], *slash;
    WIN32_FIND_DATAW fd;
    HANDLE h;
    DWORD n = GetModuleFileNameW(NULL, dir, MAX_PATH), attr;
    int deleted = 0;
    /* An absolute path built from the exe's own location, never the working
     * directory, so nothing outside <game folder>\Diagnostics is touched. */
    if (n == 0 || n >= MAX_PATH || !(slash = wcsrchr(dir, L'\\'))) return 0;
    *slash = 0;
    if (wcslen(dir) + 16 >= MAX_PATH) return 0;
    wcscat_s(dir, MAX_PATH, L"\\Diagnostics");
    attr = GetFileAttributesW(dir);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY) ||
        (attr & FILE_ATTRIBUTE_REPARSE_POINT))
        return 0;                       /* missing, not a folder, or a link elsewhere */
    swprintf_s(pattern, MAX_PATH, L"%s\\*", dir);
    h = FindFirstFileW(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (fd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT))
            continue;
        if (swprintf_s(file, MAX_PATH, L"%s\\%s", dir, fd.cFileName) <= 0) continue;
        if (DeleteFileW(file)) deleted++;
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    return deleted;
}

/* The working directory is the game's folder (main.c sets it at startup),
 * so diagnostics go to .\Diagnostics. */
FILE *doa3_diag_fopen(const char *name, const char *mode)
{
    static volatile LONG s_made;
    char path[MAX_PATH];
    if (!name || !*name) return NULL;
    if (!InterlockedCompareExchange(&s_made, 0, 0)) {
        CreateDirectoryA("Diagnostics", NULL);     /* fails harmlessly if it exists */
        InterlockedExchange(&s_made, 1);
    }
    wsprintfA(path, "Diagnostics\\%s", name);
    return fopen(path, mode);
}
int debug_hide_ui_get(void) { return g_hide_ui; }
void debug_hide_ui_set(int enabled) { g_hide_ui = !!enabled; }
int debug_hide_player_get(int side)
{
    return side >= 0 && side < 2 && (g_hide_players & (1u << side)) != 0;
}
void debug_hide_player_set(int side, int enabled)
{
    if (side < 0 || side >= 2) return;
    if (enabled) g_hide_players |= 1u << side;
    else g_hide_players &= ~(1u << side);
}
int debug_hide_player_vertices(uint32_t address)
{
#if defined(DOA3_XBE_ID_3_1)
    if (!g_hide_players || netplay_active() || MEM8(0x596A37) != 1) return 0;
    /* 8AA50 allocates one 0x498000-byte model/animation/vertex arena per
     * resource slot. Player +3 selects that slot (including tag swaps).
     * Filtering the position stream also covers model shadows/reflections,
     * without changing simulation, collision volumes, or shared textures. */
    for (unsigned side = 0; side < 2; ++side) {
        if (!(g_hide_players & (1u << side))) continue;
        unsigned slot = MEM8(0x59CF5B + side * 0x68);
        if (slot >= 4) continue;
        uint32_t base = MEM32(0x5A0CF0 + slot * 0x1C);
        if (base >= 0x10000 && base <= 0x08000000 - 0x498000 &&
            address >= base && address - base < 0x498000) return 1;
    }
#endif
    return 0;
}
int debug_screenshot_key_get(void) { return g_screenshot_key; }
int debug_screenshot_key_set(int key)
{
    if (key < VK_BACK || key > 0xFE || key == VK_ESCAPE ||
        key == VK_SHIFT || key == VK_CONTROL || key == VK_MENU ||
        (key >= VK_LSHIFT && key <= VK_RMENU) || key == VK_LWIN || key == VK_RWIN) return 0;
    g_screenshot_key = key;
    return 1;
}
static int g_screenshot_scale;
int debug_screenshot_scale_get(void) { return g_screenshot_scale; }
void debug_screenshot_scale_set(int scale) { g_screenshot_scale = (scale >= 1 && scale <= 5) ? scale : 0; }
int debug_screenshot_scale_save(void)
{
    char value[16];
    wsprintfA(value, "%d", g_screenshot_scale);
    return WritePrivateProfileStringA("Debug", "ScreenshotScale", value, doa3_settings_ini_path()) != 0;
}
int debug_screenshot_key_save(void)
{
    char value[16];
    wsprintfA(value, "%d", g_screenshot_key);
    return WritePrivateProfileStringA("Debug", "ScreenshotKey", value, doa3_settings_ini_path()) != 0;
}

int debug_unlock_all_get(void)
{
    return (int)InterlockedCompareExchange(&g_unlock_all, 0, 0);
}

void debug_unlock_all_set(int enabled)
{
    InterlockedExchange(&g_unlock_all, enabled != 0);
}

int debug_settings_load(void)
{
    const char *path = doa3_settings_ini_path();
    debug_unlock_all_set(GetPrivateProfileIntA("Debug", "UnlockAll", 0, path) != 0);
    doa3_auto_capture_set(GetPrivateProfileIntA("Debug", "AutoCapture", 0, path) != 0);
    g_screenshot_key = VK_F12;
    debug_screenshot_key_set(GetPrivateProfileIntA("Debug", "ScreenshotKey", VK_F12, path));
    debug_screenshot_scale_set(GetPrivateProfileIntA("Debug", "ScreenshotScale", 0, path));
    g_hide_ui = 0;
    g_hide_players = 0;
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

int debug_settings_save(void)
{
    int result = WritePrivateProfileStringA("Debug", "UnlockAll",
        debug_unlock_all_get() ? "1" : "0", doa3_settings_ini_path()) != 0;
    result = (WritePrivateProfileStringA("Debug", "AutoCapture",
        InterlockedCompareExchange(&g_auto_capture, 0, 0) ? "1" : "0",
        doa3_settings_ini_path()) != 0) && result;
    return debug_screenshot_key_save() && result;
}

uint8_t doa3_debug_unlock_read8(uint32_t address, uint8_t saved_value)
{
    /* DOA3 3.1 costume limits at 0x29C70C, mapped to save indices by
     * 0x2FF868. Unused model slots retain the save's default count of 2.
     * Do not invent extra costume numbers: they index real asset tables. */
    static const uint8_t costume_counts[19] = {
        4, 3, 3, 3, 3, 5, 3, 3, 3, 3, 4, 3, 2, 3, 3, 4, 2, 3, 3
    };
    if (!debug_unlock_all_get()) return saved_value;
    if (address >= 0x59FB16u && address < 0x59FB29u) {
        uint8_t count = costume_counts[address - 0x59FB16u];
        return saved_value > count ? saved_value : count;
    }
    /* Ein/access prerequisites, endings in Theater, and its menu gate. */
    if (address == 0x59FB3Cu || address == 0x59FB3Du ||
        address == 0x5A0B47u || address == 0x5A0C72u ||
        (address >= 0x5A0B49u && address < 0x5A0B5Cu))
        return saved_value ? saved_value : 1;
    return saved_value;
}
