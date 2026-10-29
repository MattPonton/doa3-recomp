#include "audio_settings.h"
#include "video_settings.h"
#include <stdio.h>
#include <string.h>

/* The UI writes this while the APU and movie threads read it. */
static volatile LONG g_volume = 80;   /* 1.0.2: 80% on a first boot (100% was loud) */
/* Streamer mode: the game thread (game_audio.c) reads it every frame. */
static volatile LONG g_streamer_mode = 0;

int audio_streamer_mode_get(void)
{
    return (int)InterlockedCompareExchange(&g_streamer_mode, 0, 0);
}

void audio_streamer_mode_set(int enabled)
{
    InterlockedExchange(&g_streamer_mode, enabled != 0);
}

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
    audio_set_volume((int)GetPrivateProfileIntA("Audio", "MasterVolume", 80, path));
    audio_streamer_mode_set(GetPrivateProfileIntA("Audio", "StreamerMode", 0, path) != 0);
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

int audio_settings_save(void)
{
    char value[16];
    int ok;
    snprintf(value, sizeof(value), "%d", audio_get_volume());
    ok = WritePrivateProfileStringA("Audio", "MasterVolume", value,
                                    doa3_settings_ini_path()) != 0;
    return (WritePrivateProfileStringA("Audio", "StreamerMode",
                audio_streamer_mode_get() ? "1" : "0", doa3_settings_ini_path()) != 0) && ok;
}

void audio_apply_volume(int16_t *samples, size_t count)
{
    int volume = audio_get_volume();
    {   /* 1.0.2: silent while the game is held by Debug > Frame advance. */
        extern volatile LONG g_doa3_frame_paused;
        if (g_doa3_frame_paused) volume = 0;
    }
    if (volume == 100) return;
    if (volume == 0) {
        memset(samples, 0, count * sizeof(*samples));
        return;
    }
    for (size_t i = 0; i < count; ++i)
        samples[i] = (int16_t)((int)samples[i] * volume / 100);
}
