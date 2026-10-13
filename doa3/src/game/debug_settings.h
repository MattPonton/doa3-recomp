#ifndef DOA3_DEBUG_SETTINGS_H
#define DOA3_DEBUG_SETTINGS_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
int debug_unlock_all_get(void);
void debug_unlock_all_set(int enabled);
int debug_hide_ui_get(void);
void debug_hide_ui_set(int enabled);
int debug_hide_player_get(int side);
void debug_hide_player_set(int side, int enabled);
/* Render-only filtering of a fighter-owned vertex stream. */
int debug_hide_player_vertices(uint32_t address);
int debug_screenshot_key_get(void);
int debug_screenshot_key_set(int key);
int debug_screenshot_key_save(void);
int debug_settings_load(void);
int debug_settings_save(void);
/* Only menu/selection reads use this; the save and reward code read real data. */
uint8_t doa3_debug_unlock_read8(uint32_t address, uint8_t saved_value);
#ifdef __cplusplus
}
#endif
#endif
