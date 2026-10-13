/**
 * Video settings: window type and aspect ratio, persisted to the [Video]
 * section of doa3_settings.ini next to the executable. doa3_settings.ini is the
 * general settings file; controls stay in doa3_input.ini, which
 * pad_mapping_save rewrites wholesale.
 *
 * Display shape and guest projection are separate: 4:3 Anamorphic presents
 * the same projection as 16:9 in a 4:3 target. Other modes use their display
 * height/width as the guest projection ratio. Only mapped guest memory is
 * patched; the original XBE stays unchanged.
 */
#ifndef DOA3_VIDEO_SETTINGS_H
#define DOA3_VIDEO_SETTINGS_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VIDEO_WINDOWED    0
#define VIDEO_BORDERLESS  1

enum {
    VIDEO_ASPECT_4_3_FULL,
    VIDEO_ASPECT_4_3_ANAMORPHIC,
    VIDEO_ASPECT_16_9,
    VIDEO_ASPECT_21_9,
    VIDEO_ASPECT_32_9,
    VIDEO_ASPECT_COUNT
};

/* Full path of doa3_settings.ini next to the executable. */
const char *doa3_settings_ini_path(void);

int  video_get_window_mode(void);
int  video_get_aspect(void);
const char *video_aspect_name(int aspect);
float video_display_aspect(int aspect);
float video_projection_ratio(int aspect);
/* Horizontal HUD area inside the render target. Ultrawide uses a centered
 * 16:9 canvas; 4:3 and 16:9 retain their existing full-width scaling. */
void video_hud_region(unsigned target_width, float *x, float *width);

/* Load doa3_settings.ini (defaults when absent: borderless, 16:9, 3x).
 * Returns non-zero if the file exists. */
int  video_settings_load(void);
/* Returns non-zero on success. */
int  video_settings_save(void);

/* Change settings at runtime. Window mode restyles the window immediately;
 * aspect switches the guest target and the game's widescreen state at the
 * next frame boundary. */
void video_set_window_mode(int mode);
void video_set_aspect(int aspect);

/* Internal resolution: 1 = 480 lines, 2 = 960, 3 = 1440 (default). Applied at the next
 * frame boundary. */
int  video_get_scale(void);
void video_set_scale(int scale);

/* Host size of the guest render target for an aspect at the current internal
 * resolution: 480 lines at 1x, 960 at 2x, 1440 at 3x. */
void video_guest_target_size(int aspect, unsigned *w, unsigned *h);

/* XC_VIDEO (ExQueryNonVolatileSetting index 8) value for the current aspect. */
unsigned video_xc_video_value(void);

/* Restyle/resize the window for a window mode (also used at startup). */
void video_apply_window_mode(HWND hwnd, int mode);

/* Update mapped projection data and cached widescreen state at startup and
 * frame boundaries, including changes that keep the same target dimensions. */
void video_sync_guest_widescreen(void);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_VIDEO_SETTINGS_H */
