#include "debug_settings.h"
#include "recomp/recomp_types.h"
#include <windows.h>
#include <stdio.h>

ptrdiff_t g_xbox_mem_offset = 0x20000000;
static int online;
int netplay_active(void) { return online; }
static char ini_path[MAX_PATH];
const char *doa3_settings_ini_path(void) { return ini_path; }
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Failed line %d: %s\n", __LINE__, #c); result = 1; goto cleanup; } } while (0)

int main(void)
{
    char temp[MAX_PATH];
    int result = 0;
    const uint32_t flags[] = {0x59FB3C, 0x59FB3D, 0x5A0B47, 0x5A0C72};
    if (!GetTempPathA(sizeof(temp), temp) ||
        !GetTempFileNameA(temp, "dbg", 0, ini_path)) return 1;
    CHECK(debug_settings_load());
    CHECK(!debug_unlock_all_get());
    CHECK(VirtualAlloc((void *)g_xbox_mem_offset, 0x1000000, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE));
    MEM8(0x596A37)=1;
    MEM8(0x59CF5B)=0; MEM8(0x59CFC3)=1;
    MEM32(0x5A0CF0)=0x2774000; MEM32(0x5A0D0C)=0x2C0C000;
    CHECK(!debug_hide_player_vertices(0x2792800));
    debug_hide_player_set(0,1);
    CHECK(debug_hide_player_get(0) && !debug_hide_player_get(1));
    CHECK(debug_hide_player_vertices(0x2792800));
    CHECK(debug_hide_player_vertices(0x282A3E0)); /* shadow geometry */
    CHECK(!debug_hide_player_vertices(0x2C2A000));
    CHECK(!debug_hide_player_vertices(0x1DB1468)); /* stage */
    CHECK(!debug_hide_player_vertices(0x2773FFF));
    debug_hide_player_set(1,1);
    CHECK(debug_hide_player_vertices(0x2C2A000));
    debug_hide_player_set(0,0);
    CHECK(!debug_hide_player_vertices(0x2792800));
    online=1; CHECK(!debug_hide_player_vertices(0x2C2A000)); online=0;
    MEM8(0x596A37)=0; CHECK(!debug_hide_player_vertices(0x2C2A000)); MEM8(0x596A37)=1;
    MEM8(0x59CFC3)=4; CHECK(!debug_hide_player_vertices(0x2C2A000)); MEM8(0x59CFC3)=1;
    debug_hide_player_set(-1,1); debug_hide_player_set(2,1);
    CHECK(!debug_hide_player_get(-1) && !debug_hide_player_get(2));
    CHECK(debug_screenshot_key_get() == VK_F12);
    CHECK(!debug_hide_ui_get());
    debug_hide_ui_set(1); CHECK(debug_hide_ui_get());
    CHECK(!debug_screenshot_key_set(VK_ESCAPE));
    CHECK(!debug_screenshot_key_set(VK_CONTROL));
    CHECK(!debug_screenshot_key_set(9999));
    CHECK(debug_screenshot_key_get() == VK_F12);
    CHECK(debug_screenshot_key_set(VK_F9));
    CHECK(debug_screenshot_key_save());
    CHECK(doa3_debug_unlock_read8(0x59FB16, 2) == 2);
    CHECK(doa3_debug_unlock_read8(0x5A0B49, 0) == 0);

    debug_unlock_all_set(1);
    CHECK(doa3_debug_unlock_read8(0x59FB16, 2) == 4);
    CHECK(doa3_debug_unlock_read8(0x59FB1B, 2) == 5);
    CHECK(doa3_debug_unlock_read8(0x59FB16, 6) == 6);
    for (unsigned i = 0; i < sizeof(flags)/sizeof(flags[0]); ++i) {
        CHECK(doa3_debug_unlock_read8(flags[i], 0) == 1);
        CHECK(doa3_debug_unlock_read8(flags[i], 2) == 2);
    }
    for (unsigned i = 0; i < 19; ++i)
        CHECK(doa3_debug_unlock_read8(0x5A0B49 + i, 0) == 1);
    CHECK(doa3_debug_unlock_read8(0x59FB15, 0) == 0);
    CHECK(doa3_debug_unlock_read8(0x59FB29, 0) == 0);
    CHECK(doa3_debug_unlock_read8(0x5A0B5C, 0) == 0);
    CHECK(doa3_debug_unlock_read8(0x59D0D6, 0) == 0);

    CHECK(WritePrivateProfileStringA("Audio", "MasterVolume", "37", ini_path));
    CHECK(debug_settings_save());
    debug_unlock_all_set(0);
    CHECK(doa3_debug_unlock_read8(0x59FB16, 2) == 2);
    CHECK(doa3_debug_unlock_read8(0x5A0B49, 0) == 0);
    CHECK(debug_settings_load());
    CHECK(debug_unlock_all_get());
    CHECK(debug_screenshot_key_get() == VK_F9);
    CHECK(!debug_hide_ui_get()); /* Never persist invisible game menus. */
    CHECK(!debug_hide_player_get(0) && !debug_hide_player_get(1));
    CHECK(GetPrivateProfileIntA("Audio", "MasterVolume", 0, ini_path) == 37);
    debug_unlock_all_set(0);
    CHECK(debug_settings_save());
    debug_unlock_all_set(1);
    CHECK(debug_settings_load());
    CHECK(!debug_unlock_all_get());
cleanup:
    if (g_xbox_mem_offset) VirtualFree((void *)g_xbox_mem_offset,0,MEM_RELEASE);
    WritePrivateProfileStringA(NULL, NULL, NULL, ini_path);
    DeleteFileA(ini_path);
    if (!result) puts("Unlock override, reversal, bounds and persistence passed.");
    return result;
}
