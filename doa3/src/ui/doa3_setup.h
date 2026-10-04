/**
 * First-run setup overlay (Dear ImGui).
 *
 * Shown before boot when the assets folder next to the exe is missing game
 * files: the user picks their Dead or Alive 3 disc image (.xiso, .xiso.iso
 * or the original .iso), it is validated and extracted into assets, and boot
 * then carries on in the same window.
 *
 * It owns its own D3D11 device and ImGui context for as long as it runs and
 * tears both down before returning, so the game's D3D8->D3D11 device and the
 * Esc overlay start exactly as they do on a normal launch.
 */

#ifndef DOA3_SETUP_H
#define DOA3_SETUP_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Run the setup screen on `hwnd` until the assets are installed (returns 1)
 * or the user quits / closes the window (returns 0). */
int doa3_setup_run(HWND hwnd, const WCHAR *assets_dir);

/* Window messages while setup is up. Returns non-zero when consumed. */
int doa3_setup_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_SETUP_H */
