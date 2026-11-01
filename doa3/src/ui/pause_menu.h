/* Esc menu list engine (1.0.4): a full-screen black gradient over the game
 * and one column of rows on the left, driven by the arrow keys / d-pad,
 * Left / Right, Enter / A and Esc / B, or the mouse. Immediate mode: each
 * frame a screen calls Begin, one function per row, then End.
 *
 * Rows that can be selected: Action, Toggle, Choice, Step. Heading and Note
 * are text between them. The selected row's help text is shown under the
 * list. Up / Down move the selection (applied at End); Left / Right change
 * the selected row's value; Enter / A / a click activates it (a click on the
 * value's left or right half steps it down or up). */
#pragma once
#include "imgui.h"

namespace pm {

/* Add the menu's fonts to the atlas (after the default font, which the
 * other ImGui windows keep using). Call whenever the atlas is rebuilt. */
void AddFonts(ImFontAtlas *atlas);

struct State {
    int    selected = 0;
    float  scroll   = 0.0f;
    int    tickerRow = -1;      /* the selected row the ticker is timing */
    double tickerStart = 0.0;   /* when it was selected */
    void  Reset() { selected = 0; scroll = 0.0f; tickerRow = -1; }
};

/* title: large heading ("MENU"); section: the line under it ("Display"). */
/* main: the top list's style (labels in capitals, bold italic) rather
 * than a settings screen's (normal case, bold). */
void Begin(State &st, const char *title, const char *section, bool main = false);
bool Action(const char *label, const char *help, bool enabled = true, const char *value = nullptr);
bool Toggle(const char *label, bool &on, const char *help, bool enabled = true);
bool Choice(const char *label, int &index, const char *const *names, int count,
            const char *help, bool enabled = true);
bool Step(const char *label, int &value, int lo, int hi, int step, const char *fmt,
          const char *help, bool enabled = true);
/* A control's binding: the value is drawn without arrows (in the status
 * color while listening). Returns true when activated; clear is set when
 * Delete / X is pressed on it or it is right-clicked. */
bool Binding(const char *label, const char *value, bool listening, const char *help, bool &clear);
/* While on (a binding is being captured), the list ignores the keyboard,
 * controller and mouse. Call before Begin; it stays until turned off. */
void SetBlocked(bool on);
/* One more button hint after the usual ones, for this frame (after Begin). */
void Hint(const char *key, const char *action);
void Heading(const char *text);
/* A thin rule between groups of rows (e.g. before Reset to defaults). */
void Separator();
void Note(const char *text, ImU32 color = 0);
/* B / Backspace was pressed this frame (Esc is handled by the window
 * procedure). */
bool BackPressed();
/* status: a short message over the button hints (or null); version: the
 * small line at the bottom. */
void End(const char *status, const char *version);

} /* namespace pm */
