/* Esc menu list engine (1.0.4). See pause_menu.h. */
#include "pause_menu.h"
#include "menu_fonts.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace pm {
namespace {

ImFont *g_title_font = nullptr;   /* Poppins Bold Italic: the title and the screen name */
ImFont *g_label_font = nullptr;   /* Poppins Bold: row labels and values */
ImFont *g_row_font   = nullptr;   /* Poppins Medium: help, notes, hints */
ImFont *g_head_font  = nullptr;   /* Poppins Medium Italic: group headings */

/* Colours. Black gradient over the game, blue selection like the game's own
 * menus. */
const ImU32 kBgLeft     = IM_COL32(0, 0, 0, 236);
const ImU32 kBgMid      = IM_COL32(0, 0, 0, 176);
const ImU32 kBgRight    = IM_COL32(0, 0, 0, 72);
const ImU32 kSelLeft    = IM_COL32(28, 104, 236, 240);
const ImU32 kSelRight   = IM_COL32(8, 30, 110, 40);
const ImU32 kRuleLeft   = IM_COL32(52, 128, 255, 255);
const ImU32 kRuleRight  = IM_COL32(52, 128, 255, 0);
const ImU32 kTitle      = IM_COL32(255, 255, 255, 255);
const ImU32 kSection    = IM_COL32(196, 214, 244, 255);
const ImU32 kText       = IM_COL32(214, 220, 232, 255);
const ImU32 kTextSel    = IM_COL32(255, 255, 255, 255);
const ImU32 kValue      = IM_COL32(118, 170, 255, 255);
const ImU32 kValueSel   = IM_COL32(255, 255, 255, 255);
const ImU32 kDisabled   = IM_COL32(110, 116, 128, 255);
const ImU32 kHeading    = IM_COL32(120, 150, 200, 255);
const ImU32 kHelp       = IM_COL32(184, 192, 206, 255);
const ImU32 kHintKey    = IM_COL32(232, 236, 244, 255);
const ImU32 kHintAction = IM_COL32(128, 136, 152, 255);
const ImU32 kVersion    = IM_COL32(96, 104, 120, 255);
const ImU32 kStatus     = IM_COL32(150, 196, 255, 255);

struct Frame {
    State *st = nullptr;
    ImDrawList *dl = nullptr;
    ImFont *row = nullptr, *title = nullptr, *label = nullptr, *head = nullptr;
    float W = 0, H = 0, x0 = 0, colW = 0;
    float rowSize = 0, textSize = 0, titleSize = 0, rowH = 0;
    float listTop = 0, listBottom = 0;
    float cy = 0;                       /* content y of the next row (list space) */
    int   index = 0;                    /* selectable rows so far */
    std::vector<float> rowY;            /* content y of each selectable row */
    std::vector<bool>  rowEnabled;
    const char *help = nullptr;         /* the selected row's help */
    bool up = false, down = false, left = false, right = false, activate = false, back = false;
    bool mouseMoved = false, clicked = false;
    ImVec2 mouse;
} F;

bool Pressed(ImGuiKey a, ImGuiKey b = ImGuiKey_None, ImGuiKey c = ImGuiKey_None)
{
    return ImGui::IsKeyPressed(a, true) ||
           (b != ImGuiKey_None && ImGui::IsKeyPressed(b, true)) ||
           (c != ImGuiKey_None && ImGui::IsKeyPressed(c, true));
}

ImVec2 TextSize(ImFont *font, float size, const char *text, float wrap = 0.0f)
{
    return font->CalcTextSizeA(size, FLT_MAX, wrap, text);
}

/* A selectable row's frame: its screen rectangle, whether it is the selected
 * one, and the mouse. Returns false when the row is outside the list area
 * (it still counts and takes its place). */
struct Row {
    int   idx;
    bool  selected, enabled, hovered, visible;
    float y;           /* screen y of the top */
};

Row BeginRow(const char *help, bool enabled)
{
    Row r;
    r.idx = F.index++;
    r.enabled = enabled;
    F.rowY.push_back(F.cy);
    F.rowEnabled.push_back(enabled);
    r.y = F.listTop + F.cy - F.st->scroll;
    F.cy += F.rowH;
    r.visible = r.y + F.rowH > F.listTop && r.y < F.listBottom;
    r.hovered = r.visible && F.mouse.x >= F.x0 && F.mouse.x < F.x0 + F.colW &&
                F.mouse.y >= r.y && F.mouse.y < r.y + F.rowH &&
                F.mouse.y >= F.listTop && F.mouse.y < F.listBottom;
    if (r.hovered && F.mouseMoved && enabled) F.st->selected = r.idx;
    if (r.hovered && F.clicked && enabled) F.st->selected = r.idx;
    r.selected = F.st->selected == r.idx;
    if (r.selected) F.help = help;
    if (r.visible && r.selected) {
        F.dl->AddRectFilledMultiColor(ImVec2(F.x0, r.y), ImVec2(F.x0 + F.colW, r.y + F.rowH),
                                      kSelLeft, kSelRight, kSelRight, kSelLeft);
    }
    return r;
}

/* Headings and the screen name are drawn in capitals (ASCII letters; the
 * fonts are Latin subsets). */
void Upper(char *t)
{
    for (; *t; ++t) if (*t >= 'a' && *t <= 'z') *t = (char)(*t - 'a' + 'A');
}

/* The label. One too long for the room left by the value is
 * cut at the edge; while its row is selected it scrolls left like a
 * ticker (a pause at each end) to show the rest. */
void DrawLabel(const Row &r, const char *label, float valueW)
{
    if (!r.visible) return;
    ImU32 col = !r.enabled ? kDisabled : r.selected ? kTextSel : kText;
    float pad = F.rowSize * 0.6f;
    float room = F.colW - pad * 2.0f - (valueW > 0.0f ? valueW + pad : 0.0f);
    char text[200];
    _snprintf_s(text, sizeof text, _TRUNCATE, "%s", label);
    float width = TextSize(F.label, F.rowSize, text).x;
    float x = F.x0 + pad, y = r.y + (F.rowH - F.rowSize) * 0.5f - F.rowSize * 0.04f;
    if (width <= room) {
        F.dl->AddText(F.label, F.rowSize, ImVec2(x, y), col, text);
        return;
    }
    float shift = 0.0f;
    if (r.selected) {
        State &st = *F.st;
        double now = ImGui::GetTime();
        if (st.tickerRow != r.idx) { st.tickerRow = r.idx; st.tickerStart = now; }
        const double hold = 1.2;                                   /* seconds at each end */
        float over = width - room, speed = F.rowSize * 3.0f;      /* pixels per second */
        double run = over / speed, cycle = hold + run + hold;
        double t = fmod(now - st.tickerStart, cycle);
        shift = t < hold ? 0.0f : t < hold + run ? (float)((t - hold) * speed) : over;
    }
    F.dl->PushClipRect(ImVec2(x, r.y), ImVec2(x + room, r.y + F.rowH), true);
    F.dl->AddText(F.label, F.rowSize, ImVec2(x - shift, y), col, text);
    F.dl->PopClipRect();
}

/* The value text as drawn ("<   value   >" with arrows). */
void ValueText(char *out, size_t n, const char *value, bool arrows)
{
    if (!value) { out[0] = 0; return; }
    if (arrows) _snprintf_s(out, n, _TRUNCATE, "<   %s   >", value);
    else _snprintf_s(out, n, _TRUNCATE, "%s", value);
}
float ValueWidth(const char *value, bool arrows)
{
    char text[160];
    if (!value) return 0.0f;
    ValueText(text, sizeof text, value, arrows);
    return TextSize(F.label, F.rowSize, text).x;
}

/* "<  value  >" at the right end. Returns -1 / +1 when its left / right
 * half was clicked, else 0. */
int DrawValue(const Row &r, const char *value, bool arrows)
{
    if (!r.visible || !value) return 0;
    ImU32 col = !r.enabled ? kDisabled : r.selected ? kValueSel : kValue;
    char text[160];
    ValueText(text, sizeof text, value, arrows);
    ImVec2 sz = TextSize(F.label, F.rowSize, text);
    float pad = F.rowSize * 0.6f;
    float x = F.x0 + F.colW - pad - sz.x;
    F.dl->AddText(F.label, F.rowSize, ImVec2(x, r.y + (F.rowH - F.rowSize) * 0.5f - F.rowSize * 0.04f), col, text);
    if (arrows && r.enabled && F.clicked && r.hovered && F.mouse.x >= x - pad)
        return F.mouse.x < x + sz.x * 0.5f ? -1 : +1;
    return 0;
}

} /* namespace */

void AddFonts(ImFontAtlas *atlas)
{
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;   /* static arrays */
    _snprintf_s(cfg.Name, sizeof cfg.Name, _TRUNCATE, "Poppins Medium");
    g_row_font = atlas->AddFontFromMemoryTTF((void *)k_font_poppins_medium, (int)sizeof k_font_poppins_medium,
                                             24.0f, &cfg);
    _snprintf_s(cfg.Name, sizeof cfg.Name, _TRUNCATE, "Poppins Bold");
    g_label_font = atlas->AddFontFromMemoryTTF((void *)k_font_poppins_bold, (int)sizeof k_font_poppins_bold,
                                               24.0f, &cfg);
    _snprintf_s(cfg.Name, sizeof cfg.Name, _TRUNCATE, "Poppins Medium Italic");
    g_head_font = atlas->AddFontFromMemoryTTF((void *)k_font_poppins_medium_italic,
                                              (int)sizeof k_font_poppins_medium_italic, 24.0f, &cfg);
    _snprintf_s(cfg.Name, sizeof cfg.Name, _TRUNCATE, "Poppins Bold Italic");
    g_title_font = atlas->AddFontFromMemoryTTF((void *)k_font_poppins_bold_italic,
                                               (int)sizeof k_font_poppins_bold_italic, 48.0f, &cfg);
}

void Begin(State &st, const char *title, const char *section)
{
    ImGuiIO &io = ImGui::GetIO();
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("##pause_menu", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav |
                 ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleVar(2);
    if (!ImGui::IsWindowFocused()) ImGui::SetWindowFocus();

    F = Frame();
    F.st = &st;
    F.dl = ImGui::GetWindowDrawList();
    F.row = g_row_font ? g_row_font : ImGui::GetFont();
    F.title = g_title_font ? g_title_font : F.row;
    F.label = g_label_font ? g_label_font : F.row;
    F.head = g_head_font ? g_head_font : F.row;
    F.W = vp->Size.x; F.H = vp->Size.y;
    ImVec2 o = vp->Pos;

    F.rowSize = F.H * 0.028f;
    if (F.rowSize < 15.0f) F.rowSize = 15.0f;
    if (F.rowSize > 40.0f) F.rowSize = 40.0f;
    F.textSize  = F.rowSize * 0.8f;
    F.titleSize = F.rowSize * 2.3f;
    F.rowH = F.rowSize * 1.7f;
    F.x0 = o.x + F.W * 0.07f;
    F.colW = F.W * 0.50f;
    if (F.colW < F.rowSize * 20.0f) F.colW = F.rowSize * 20.0f;
    if (F.colW > F.W - F.W * 0.14f) F.colW = F.W - F.W * 0.14f;

    /* Background: black, opaque on the left and fading toward the right so
     * the game shows through. */
    float mid = o.x + F.W * 0.55f;
    F.dl->AddRectFilledMultiColor(o, ImVec2(mid, o.y + F.H), kBgLeft, kBgMid, kBgMid, kBgLeft);
    F.dl->AddRectFilledMultiColor(ImVec2(mid, o.y), ImVec2(o.x + F.W, o.y + F.H), kBgMid, kBgRight, kBgRight, kBgMid);

    /* Title, its rule, and the section under it. */
    float y = o.y + F.H * 0.07f;
    F.dl->AddText(F.title, F.titleSize, ImVec2(F.x0, y), kTitle, title);
    y += F.titleSize * 1.12f;
    float rule = F.rowSize * 0.12f; if (rule < 2.0f) rule = 2.0f;
    F.dl->AddRectFilledMultiColor(ImVec2(F.x0, y), ImVec2(F.x0 + F.colW, y + rule),
                                  kRuleLeft, kRuleRight, kRuleRight, kRuleLeft);
    y += rule + F.textSize * 0.4f;
    if (section && section[0]) {
        /* the screen's name: capitals, bold italic, a little larger than
         * the rows */
        char name[64];
        _snprintf_s(name, sizeof name, _TRUNCATE, "%s", section);
        Upper(name);
        float size = F.rowSize * 1.2f;
        y += F.textSize * 0.2f;
        F.dl->AddText(F.title, size, ImVec2(F.x0, y), kSection, name);
        y += size * 1.45f;
    } else y += F.textSize * 0.4f;
    F.listTop = y;
    F.listBottom = o.y + F.H * 0.72f;
    /* a whole number of rows, so none shows cut in half at the bottom */
    if (F.listBottom - F.listTop > F.rowH)
        F.listBottom = F.listTop + floorf((F.listBottom - F.listTop) / F.rowH) * F.rowH;
    F.dl->PushClipRect(ImVec2(o.x, F.listTop), ImVec2(o.x + F.W, F.listBottom), true);

    /* Input. */
    F.up       = Pressed(ImGuiKey_UpArrow, ImGuiKey_GamepadDpadUp, ImGuiKey_GamepadLStickUp);
    F.down     = Pressed(ImGuiKey_DownArrow, ImGuiKey_GamepadDpadDown, ImGuiKey_GamepadLStickDown);
    F.left     = Pressed(ImGuiKey_LeftArrow, ImGuiKey_GamepadDpadLeft, ImGuiKey_GamepadLStickLeft);
    F.right    = Pressed(ImGuiKey_RightArrow, ImGuiKey_GamepadDpadRight, ImGuiKey_GamepadLStickRight);
    F.activate = ImGui::IsKeyPressed(ImGuiKey_Enter, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false) ||
                 ImGui::IsKeyPressed(ImGuiKey_Space, false) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown, false);
    F.back     = ImGui::IsKeyPressed(ImGuiKey_Backspace, false) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false);
    F.mouse = io.MousePos;
    F.mouseMoved = io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f;
    F.clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    if (io.MouseWheel != 0.0f && F.mouse.x < F.x0 + F.colW + F.rowSize)
        st.scroll -= io.MouseWheel * F.rowH * 2.0f;
}

bool Action(const char *label, const char *help, bool enabled, const char *value)
{
    Row r = BeginRow(help, enabled);
    DrawLabel(r, label, ValueWidth(value, false));
    DrawValue(r, value, false);
    if (!enabled) return false;
    if (r.hovered && F.clicked) return true;
    return r.selected && F.activate;
}

bool Toggle(const char *label, bool &on, const char *help, bool enabled)
{
    Row r = BeginRow(help, enabled);
    DrawLabel(r, label, ValueWidth(on ? "On" : "Off", true));
    int click = DrawValue(r, on ? "On" : "Off", true);
    if (!enabled) return false;
    bool flip = click != 0 || (r.hovered && F.clicked) ||
                (r.selected && (F.left || F.right || F.activate));
    if (flip) on = !on;
    return flip;
}

bool Choice(const char *label, int &index, const char *const *names, int count,
            const char *help, bool enabled)
{
    Row r = BeginRow(help, enabled);
    if (count <= 0) { DrawLabel(r, label, 0.0f); return false; }
    if (index < 0 || index >= count) index = 0;
    DrawLabel(r, label, ValueWidth(names[index], true));
    int click = DrawValue(r, names[index], true);
    if (!enabled) return false;
    int d = click;
    if (!d && r.hovered && F.clicked) d = +1;
    if (!d && r.selected) d = F.left ? -1 : (F.right || F.activate) ? +1 : 0;
    if (!d) return false;
    index = (index + d + count) % count;
    return true;
}

bool Step(const char *label, int &value, int lo, int hi, int step, const char *fmt,
          const char *help, bool enabled)
{
    Row r = BeginRow(help, enabled);
    char text[64];
    _snprintf_s(text, sizeof text, _TRUNCATE, fmt ? fmt : "%d", value);
    DrawLabel(r, label, ValueWidth(text, true));
    int click = DrawValue(r, text, true);
    if (!enabled) return false;
    int d = click;
    if (!d && r.selected) d = F.left ? -1 : F.right ? +1 : 0;
    if (!d) return false;
    int v = value + d * step;
    /* land on the step grid (a value saved off it rounds the first time) */
    if (step > 1) v = (v - lo + (d < 0 ? step - 1 : 0)) / step * step + lo;
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    if (v == value) return false;
    value = v;
    return true;
}

void Heading(const char *text)
{
    /* capitals, medium italic, between the rows and the screen name in size */
    char t[96];
    _snprintf_s(t, sizeof t, _TRUNCATE, "%s", text);
    Upper(t);
    float size = F.rowSize * 1.1f;
    float h = F.rowH * 1.05f;
    float y = F.listTop + F.cy - F.st->scroll;
    if (y + h > F.listTop && y < F.listBottom)
        F.dl->AddText(F.head, size, ImVec2(F.x0, y + h - size * 1.3f), kHeading, t);
    F.cy += h;
}

void Note(const char *text, ImU32 color)
{
    float wrap = F.colW - F.rowSize * 1.2f;
    ImVec2 sz = TextSize(F.row, F.textSize, text, wrap);
    float y = F.listTop + F.cy - F.st->scroll;
    if (y + sz.y > F.listTop && y < F.listBottom)
        F.dl->AddText(F.row, F.textSize, ImVec2(F.x0 + F.rowSize * 0.6f, y + F.textSize * 0.2f),
                      color ? color : kHelp, text, nullptr, wrap);
    F.cy += sz.y + F.textSize * 0.6f;
}

bool BackPressed() { return F.back; }

void End(const char *status, const char *version)
{
    State &st = *F.st;
    int n = F.index;
    F.dl->PopClipRect();

    /* Move the selection (wrapping, past disabled rows); keep it on an
     * enabled row. */
    if (n > 0) {
        int dir = F.down ? +1 : F.up ? -1 : 0;
        int s = st.selected;
        if (s < 0 || s >= n) s = 0;
        if (dir) {
            for (int k = 0; k < n; ++k) {
                s = (s + dir + n) % n;
                if (F.rowEnabled[s]) break;
            }
        } else if (!F.rowEnabled[s]) {
            for (int k = 0; k < n && !F.rowEnabled[s]; ++k) s = (s + 1) % n;
        }
        st.selected = s;

        /* Scroll so the selected row is in view (with a row of margin when
         * there is room), never past the content. */
        float view = F.listBottom - F.listTop;
        float total = F.cy;
        float y = F.rowY[s];
        if (dir || st.scroll < 0.0f || st.scroll > total) {
            if (y - F.rowH < st.scroll) st.scroll = y - F.rowH;
            if (y + F.rowH * 2.0f > st.scroll + view) st.scroll = y + F.rowH * 2.0f - view;
        }
        if (st.scroll > total - view) st.scroll = total - view;
        if (st.scroll < 0.0f) st.scroll = 0.0f;
        /* an ellipsis when more is above or below */
        if (st.scroll > 0.5f)
            F.dl->AddText(F.row, F.textSize, ImVec2(F.x0 + F.colW - F.rowSize, F.listTop - F.textSize * 1.2f),
                          kHeading, "...");
        if (total - st.scroll > view + 0.5f)
            F.dl->AddText(F.row, F.textSize, ImVec2(F.x0 + F.colW - F.rowSize, F.listBottom), kHeading, "...");
    }

    /* The selected row's explanation. */
    float y = F.listTop + F.cy - st.scroll;
    if (y > F.listBottom) y = F.listBottom;
    y += F.textSize * 0.9f;
    if (F.help && F.help[0]) {
        F.dl->AddText(F.row, F.textSize, ImVec2(F.x0, y), kHelp, F.help, nullptr, F.colW);
        y += TextSize(F.row, F.textSize, F.help, F.colW).y;
    }

    /* Status, button hints and the version, from the bottom up. */
    float by = F.H * 0.955f;
    if (version && version[0]) {
        float s = F.textSize * 0.82f;
        F.dl->AddText(F.row, s, ImVec2(F.x0, by - s), kVersion, version);
        by -= s * 1.5f;
    }
    {
        static const char *const hints[][2] = {
            { "Arrows / D-pad", "choose" }, { "Left / Right", "change" },
            { "Enter / A", "select" }, { "Esc / B", "back" },
        };
        float x = F.x0, s = F.textSize * 0.9f;
        for (const auto &h : hints) {
            F.dl->AddText(F.row, s, ImVec2(x, by - s), kHintKey, h[0]);
            x += TextSize(F.row, s, h[0]).x + s * 0.35f;
            F.dl->AddText(F.row, s, ImVec2(x, by - s), kHintAction, h[1]);
            x += TextSize(F.row, s, h[1]).x + s * 1.2f;
        }
        by -= s * 1.7f;
    }
    if (status && status[0] && by - F.textSize > y)
        F.dl->AddText(F.row, F.textSize, ImVec2(F.x0, by - F.textSize), kStatus, status);

    ImGui::End();
}

} /* namespace pm */
