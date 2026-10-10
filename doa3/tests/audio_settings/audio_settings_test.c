#include "audio_settings.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

static char ini_path[MAX_PATH];
const char *doa3_settings_ini_path(void) { return ini_path; }

#define CHECK(condition) do { if (!(condition)) { \
    fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #condition); \
    result = 1; goto cleanup; } } while (0)

int main(void)
{
    char temp_path[MAX_PATH];
    const int16_t original[] = { -32768, 32767, -10000, 10000, 0, 1 };
    const int16_t half[] = { -16384, 16383, -5000, 5000, 0, 0 };
    int16_t samples[6];
    int result = 0;
    if (!GetTempPathA(sizeof(temp_path), temp_path) ||
        !GetTempFileNameA(temp_path, "dav", 0, ini_path)) return 1;

    /* A new installation keeps the original level. */
    CHECK(audio_settings_load());
    CHECK(audio_get_volume() == 100);
    memcpy(samples, original, sizeof(samples));
    audio_apply_volume(samples, 6);
    CHECK(memcmp(samples, original, sizeof(samples)) == 0);
    audio_set_volume(50);
    audio_apply_volume(samples, 6);
    CHECK(memcmp(samples, half, sizeof(samples)) == 0);
    audio_set_volume(0);
    audio_apply_volume(samples, 6);
    for (int i = 0; i < 6; ++i) CHECK(samples[i] == 0);
    audio_set_volume(-1);
    CHECK(audio_get_volume() == 0);
    audio_set_volume(101);
    CHECK(audio_get_volume() == 100);

    /* Round-trip mute and an intermediate value, preserving other settings. */
    CHECK(WritePrivateProfileStringA("Video", "InternalResolution", "2", ini_path));
    audio_set_volume(37);
    CHECK(audio_settings_save());
    audio_set_volume(100);
    CHECK(audio_settings_load());
    CHECK(audio_get_volume() == 37);
    CHECK(GetPrivateProfileIntA("Video", "InternalResolution", 0, ini_path) == 2);
    audio_set_volume(0);
    CHECK(audio_settings_save());
    audio_set_volume(100);
    CHECK(audio_settings_load());
    CHECK(audio_get_volume() == 0);
    CHECK(WritePrivateProfileStringA("Audio", "MasterVolume", "999", ini_path));
    CHECK(audio_settings_load());
    CHECK(audio_get_volume() == 100);
    CHECK(WritePrivateProfileStringA("Audio", "MasterVolume", "-1", ini_path));
    CHECK(audio_settings_load());
    CHECK(audio_get_volume() == 0);

cleanup:
    WritePrivateProfileStringA(NULL, NULL, NULL, ini_path);
    DeleteFileA(ini_path);
    if (!result) puts("Audio level, mute, bounds, and persistence checks passed.");
    return result;
}
