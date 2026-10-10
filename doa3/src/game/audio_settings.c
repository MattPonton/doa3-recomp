#include "audio_settings.h"
#include "video_settings.h"
#include <stdio.h>
#include <string.h>

/* The UI writes this while the APU and movie threads read it. */
static volatile LONG g_volume = 100;

int audio_get_volume(void)
{
    return (int)InterlockedCompareExchange(&g_volume, 0, 0);
}

void audio_set_volume(int percent)
{
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    InterlockedExchange(&g_volume, (LONG)percent);
}

int audio_settings_load(void)
{
    const char *path = doa3_settings_ini_path();
    audio_set_volume((int)GetPrivateProfileIntA("Audio", "MasterVolume", 100, path));
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

int audio_settings_save(void)
{
    char value[16];
    snprintf(value, sizeof(value), "%d", audio_get_volume());
    return WritePrivateProfileStringA("Audio", "MasterVolume", value,
                                      doa3_settings_ini_path()) != 0;
}

void audio_apply_volume(int16_t *samples, size_t count)
{
    int volume = audio_get_volume();
    if (volume == 100) return;
    if (volume == 0) {
        memset(samples, 0, count * sizeof(*samples));
        return;
    }
    for (size_t i = 0; i < count; ++i)
        samples[i] = (int16_t)((int)samples[i] * volume / 100);
}
