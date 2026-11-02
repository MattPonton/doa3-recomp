#include "doa3_ui.h"
#include "combat_overlay.h"
#include "combat_world.h"
#include "performance_overlay.h"
#include "pause_menu.h"
#include "../game/combat_debug.h"
#include "../game/training_dummy.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include <d3d11.h>
#include <xinput.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "pad_mapping.h"
extern "C" const char *ds4_model_name(int slot);   /* input/ds4_device.c */
#include "../game/video_settings.h"
#include "../game/audio_settings.h"
#include "../game/game_audio.h"
#include "../game/debug_settings.h"
#include "../game/debug_capture.h"
#include "../game/free_camera.h"
#include "../d3d/d3d8_debug_views.h"
#include "../d3d/d3d8_screenshot.h"
#include "../game/log_settings.h"
#include "../game/dip_switches.h"
#include "../game/update_check.h"
#include "../game/doa3_version.h"
#include <shellapi.h>
#define DOA3_VER_STR_UI DOA3_VER_FULL_STR
#include "../online/netplay_session.h"
#include "../online/netplay.h"
#include "../online/net_upnp.h"
ID3D11RenderTargetView *d3d8_GetPresentRTV(void);
UINT                    d3d8_GetPresentWidth(void);
UINT                    d3d8_GetPresentHeight(void);
ID3D11Device           *d3d8_GetD3D11Device(void);
ID3D11DeviceContext    *d3d8_GetD3D11Context(void);
ID3D11RenderTargetView *d3d8_GetDefaultRTV(void);
HWND                    d3d8_GetHWND(void);
UINT                    d3d8_GetBackbufferWidth(void);
UINT                    d3d8_GetBackbufferHeight(void);
void                    d3d8_RestoreDefaultTarget(void);
unsigned int            doa3_dbg_read32(unsigned int va);
unsigned int            doa3_dbg_read8(unsigned int va);
}

/* imgui_impl_win32's message handler, declared in its header. */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace {

bool g_ready   = false;
bool g_visible = false;
bool g_screenshot_listening = false;
/* 1.0.2: controller hotkeys switch tabs (L1 / R1); see PollControllerHotkeys. */
int g_tab_current = 0, g_tab_request = -1;
/* 1.0.4: the list menu (pause_menu.cpp). The screens not yet moved to it
 * open the tabbed window on their tab (SCR_CLASSIC); leaving it comes back
 * to the list. */
enum MenuScreen { SCR_MAIN, SCR_DISPLAY, SCR_SOUND, SCR_GAME, SCR_QUIT, SCR_CLASSIC,
                  SCR_CONTROLS, SCR_PLAYER, SCR_DEVICE, SCR_PHOTO, SCR_SPARRING };
MenuScreen g_screen = SCR_MAIN;
pm::State g_pm_main, g_pm_display, g_pm_sound, g_pm_game, g_pm_quit,
          g_pm_controls, g_pm_player, g_pm_device, g_pm_photo, g_pm_sparring;
/* 1.0.4: after a binding is captured, the list ignores input until the
 * captured key or control is let go (so a held d-pad or A does not also
 * move the selection or start listening again). */
PadBinding g_bind_release = { 0, 0 };
bool g_screen_dirty = false;      /* a setting on this screen changed: save on Back */
/* 1.0.4: "Pause the game while this menu is open" ([General] PauseInMenu),
 * through the frame-advance hold (recomp_manual.c). */
bool g_pause_in_menu = false;
bool g_menu_paused = false;
void MenuBack();
/* Cursor over the game while the menu is closed (doa3_ui_wndproc). */
bool g_cursor_shown = false;
ULONGLONG g_cursor_last_move, g_cursor_streak;
LPARAM g_cursor_last_pos = -1;

/* Rebind capture state, mirroring MapperPanel's listening_/armed_ pair. */
int  g_listening = -1;   /* row capturing a binding, or -1 */
bool g_armed     = false; /* skip the frame the click started on */
int  g_pending_vk = 0;    /* VK grabbed by the wndproc while listening */
int  g_input_player = 0;

char g_status[128] = { 0 };

/* 1.0.2: DOA3.exe's folder refused a test write at boot (main.c). */
extern "C" unsigned long g_doa3_folder_write_error;
const char *kFolderWarning =
    "Dead or Alive 3 cannot write to its folder, so settings and game saves will not be kept. "
    "Move the game's folder somewhere you own (not Program Files or a synced OneDrive folder), "
    "or allow DOA3.exe in Windows Security > Ransomware protection > Controlled folder access "
    "or in your antivirus.";
/* 1.0.2: Debug > Frame advance (recomp_manual.c). */
extern "C" volatile LONG g_doa3_frame_pause, g_doa3_frame_steps, g_doa3_frame_paused,
                         g_doa3_frame_step_trace, g_doa3_trace_frames, g_doa3_frame_slow_ms,
                         g_doa3_rthread_slow_ms;
extern "C" unsigned g_doa3_frames_presented;
void FramePauseToggle()
{
    InterlockedExchange(&g_doa3_frame_steps, 0);
    InterlockedExchange(&g_doa3_frame_pause, g_doa3_frame_pause ? 0 : 1);
}
void FrameStep()
{
    if (g_doa3_frame_pause) InterlockedIncrement(&g_doa3_frame_steps);
}
void DrawPausedBanner()
{
    if (!g_doa3_frame_paused || g_menu_paused) return;
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + 16.0f, vp->WorkPos.y + vp->WorkSize.y - 16.0f),
                            ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowBgAlpha(0.75f);
    if (ImGui::Begin("##paused_banner", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize))
        ImGui::Text("Paused after frame %u   F7 resume   F8 step%s", g_doa3_frames_presented,
                    g_doa3_frame_step_trace ? " (traced)" : "");
    ImGui::End();
}

void SetStatus(const char *s)
{
    /* A failed save points at the reason when it is known. */
    if (g_doa3_folder_write_error && strncmp(s, "Could not write", 15) == 0)
        _snprintf_s(g_status, sizeof(g_status), _TRUNCATE, "%s: the game's folder is protected (see Game)", s);
    else
        _snprintf_s(g_status, sizeof(g_status), _TRUNCATE, "%s", s);
}

/* 0.2.16: a "(?)" after the previous item; its text shows on hover, so the
 * long explanations no longer take up the tabs. */
void HelpMarker(const char *text)
{
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

/* 0.2.16: menu size (General tab), [General] MenuScale= percent. The font is
 * rebuilt at the new size (sharp text) and every style size scaled, before
 * the next ImGui frame. */
int   g_menu_scale = 100;
int   g_menu_scale_built = 0;
bool  g_menu_resize = false;
bool  g_debug_tab = false;
ImGuiStyle g_base_style;
void ApplyMenuScale()
{
    if (g_menu_scale_built == g_menu_scale) return;
    float s = g_menu_scale / 100.0f;
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFontConfig cfg;
    cfg.SizePixels = 13.0f * s;
    io.Fonts->AddFontDefault(&cfg);
    pm::AddFonts(io.Fonts);   /* 1.0.4: the list menu's fonts */
    ImGui_ImplDX11_InvalidateDeviceObjects();   /* the font atlas is rebuilt on the next NewFrame */
    ImGui::GetStyle() = g_base_style;
    ImGui::GetStyle().ScaleAllSizes(s);
    g_menu_scale_built = g_menu_scale;
    g_menu_resize = true;
}

/* Use the same assigned player-1 device as gameplay, including native PS4.
 * The Win32 backend's hardcoded XInput-slot-0 navigation is disabled.
 * With "Capture gameplay input in Escape menu" off, the controller keeps
 * driving the game while the menu is open, so it must not also drive the
 * menu: report no gamepad (every button released) and leave the menu to
 * the mouse and keyboard. Rebinding reads the raw device separately. */
void UpdateGamepadNavigation()
{
    ImGuiIO &io = ImGui::GetIO();
    XINPUT_STATE state = {};
    bool connected = pad_mapping_capture_menu() != 0 &&
                     xbox_InputGetRawState(&state) == ERROR_SUCCESS;
    if (connected) io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    else io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    const struct { ImGuiKey key; WORD button; } buttons[] = {
        {ImGuiKey_GamepadStart, XINPUT_GAMEPAD_START}, {ImGuiKey_GamepadBack, XINPUT_GAMEPAD_BACK},
        {ImGuiKey_GamepadFaceDown, XINPUT_GAMEPAD_A}, {ImGuiKey_GamepadFaceRight, XINPUT_GAMEPAD_B},
        {ImGuiKey_GamepadFaceLeft, XINPUT_GAMEPAD_X}, {ImGuiKey_GamepadFaceUp, XINPUT_GAMEPAD_Y},
        {ImGuiKey_GamepadDpadUp, XINPUT_GAMEPAD_DPAD_UP}, {ImGuiKey_GamepadDpadDown, XINPUT_GAMEPAD_DPAD_DOWN},
        {ImGuiKey_GamepadDpadLeft, XINPUT_GAMEPAD_DPAD_LEFT}, {ImGuiKey_GamepadDpadRight, XINPUT_GAMEPAD_DPAD_RIGHT},
        {ImGuiKey_GamepadL1, XINPUT_GAMEPAD_LEFT_SHOULDER}, {ImGuiKey_GamepadR1, XINPUT_GAMEPAD_RIGHT_SHOULDER},
        {ImGuiKey_GamepadL3, XINPUT_GAMEPAD_LEFT_THUMB}, {ImGuiKey_GamepadR3, XINPUT_GAMEPAD_RIGHT_THUMB}
    };
    for (const auto &b : buttons) io.AddKeyEvent(b.key, connected && (state.Gamepad.wButtons & b.button) != 0);
    auto analog = [&](ImGuiKey key, int value, int dead, int end) {
        float amount = connected ? (float)(value - dead) / (float)(end - dead) : 0.0f;
        if (amount < 0.0f) amount = 0.0f;
        if (amount > 1.0f) amount = 1.0f;
        io.AddKeyAnalogEvent(key, amount > 0.1f, amount);
    };
    analog(ImGuiKey_GamepadL2, state.Gamepad.bLeftTrigger, 30, 255);
    analog(ImGuiKey_GamepadR2, state.Gamepad.bRightTrigger, 30, 255);
    analog(ImGuiKey_GamepadLStickLeft, state.Gamepad.sThumbLX, -7849, -32768);
    analog(ImGuiKey_GamepadLStickRight, state.Gamepad.sThumbLX, 7849, 32767);
    analog(ImGuiKey_GamepadLStickUp, state.Gamepad.sThumbLY, 7849, 32767);
    analog(ImGuiKey_GamepadLStickDown, state.Gamepad.sThumbLY, -7849, -32768);
    analog(ImGuiKey_GamepadRStickLeft, state.Gamepad.sThumbRX, -8689, -32768);
    analog(ImGuiKey_GamepadRStickRight, state.Gamepad.sThumbRX, 8689, 32767);
    analog(ImGuiKey_GamepadRStickUp, state.Gamepad.sThumbRY, 8689, 32767);
    analog(ImGuiKey_GamepadRStickDown, state.Gamepad.sThumbRY, -8689, -32768);
}

/* ── Capture ───────────────────────────────────────────────────────────────
 * Returns true when a source was pressed. A key arrives via the wndproc (so we
 * get a real VK rather than translating back out of ImGuiKey); controller
 * sources are polled here. */
bool CapturePad(PadBinding &out);
bool CaptureBinding(PadBinding &out)
{
    int device = pad_mapping_device(g_input_player);
    bool keyboard = device == PAD_DEVICE_KEYBOARD || (device == PAD_DEVICE_AUTO && g_input_player == 0);
    if (g_pending_vk && keyboard) {
        out.kind = PADSRC_KEY;
        out.code = g_pending_vk;
        g_pending_vk = 0;
        return true;
    }
    g_pending_vk = 0;
    return CapturePad(out);
}

/* The first control held on this player's controller, if any. */
bool CapturePad(PadBinding &out)
{
    XINPUT_STATE xi;
    int assigned = xbox_InputAssignedSlot(g_input_player);
    for (DWORD port = 0; port < XBOX_HOST_CONTROLLERS; ++port) {
        if ((int)port != assigned) continue;
        if (xbox_InputReadPhysical((int)port, &xi) != ERROR_SUCCESS) continue;

        static const int kButtons[] = {
            XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y,
            XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER,
            XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_BACK,
            XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB,
            XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_DOWN,
            XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT,
        };
        for (int b : kButtons) {
            if (xi.Gamepad.wButtons & b) {
                out.kind = PADSRC_PAD_BUTTON; out.code = b; return true;
            }
        }
        if (xi.Gamepad.bLeftTrigger  > 80) { out.kind = PADSRC_PAD_TRIGGER; out.code = 0; return true; }
        if (xi.Gamepad.bRightTrigger > 80) { out.kind = PADSRC_PAD_TRIGGER; out.code = 1; return true; }

        /* Sticks: well past the resting deadzone so a drifting stick does not
         * capture itself the moment a row starts listening. */
        const SHORT kDead = 16000;
        struct { SHORT v; int neg, pos; } ax[] = {
            { xi.Gamepad.sThumbLX, PADAXIS_LX_NEG, PADAXIS_LX_POS },
            { xi.Gamepad.sThumbLY, PADAXIS_LY_NEG, PADAXIS_LY_POS },
            { xi.Gamepad.sThumbRX, PADAXIS_RX_NEG, PADAXIS_RX_POS },
            { xi.Gamepad.sThumbRY, PADAXIS_RY_NEG, PADAXIS_RY_POS },
        };
        for (auto &a : ax) {
            if (a.v >  kDead) { out.kind = PADSRC_PAD_AXIS; out.code = a.pos; return true; }
            if (a.v < -kDead) { out.kind = PADSRC_PAD_AXIS; out.code = a.neg; return true; }
        }
    }
    return false;
}

/* ── Controls section ─────────────────────────────────────────────────────*/

/* "Reset to defaults" and "Save" at the right end of the row, the last
 * status message at the left. Returns 1 for reset, 2 for save. */
int ResetSaveRow()
{
    const ImGuiStyle &style = ImGui::GetStyle();
    float w = ImGui::CalcTextSize("Reset to defaults").x + ImGui::CalcTextSize("Save").x +
              style.FramePadding.x * 4.0f + style.ItemSpacing.x;
    float right = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x;
    int hit = 0;
    ImGui::AlignTextToFramePadding();
    if (g_status[0]) ImGui::TextDisabled("%s", g_status);
    else ImGui::TextUnformatted("");
    ImGui::SameLine(right - w);
    if (ImGui::Button("Reset to defaults")) hit = 1;
    ImGui::SameLine();
    if (ImGui::Button("Save")) hit = 2;
    return hit;
}

/* "Xbox controller N" / "PS4 controller N" / "PS5 controller N", numbered
 * per model among the connected pads (slot order); an unplugged slot has no
 * number. */
static void ControllerLabel(int slot, DWORD physical, char *out, size_t n)
{
    const char *model = slot < 4 ? "Xbox controller" : ds4_model_name(slot - 4);
    if (!(physical & (1u << slot))) {
        _snprintf_s(out, n, _TRUNCATE, "%s (disconnected)", model);
        return;
    }
    int num = 1;
    for (int j = 0; j < slot; ++j) {
        if (!(physical & (1u << j))) continue;
        const char *m = j < 4 ? "Xbox controller" : ds4_model_name(j - 4);
        if (strcmp(m, model) == 0) ++num;
    }
    _snprintf_s(out, n, _TRUNCATE, "%s %d", model, num);
}

static void DrawPlayerControls()
{
    static const char *types[] = { "Duke", "Controller S", "Fight Stick" };
    DWORD physical = xbox_InputPhysicalMask();
    PadMapping *m = pad_mapping_get_player(g_input_player);
    char device_labels[3 + XBOX_HOST_CONTROLLERS][64];
    strcpy_s(device_labels[0], "Automatic");
    strcpy_s(device_labels[1], "None");
    strcpy_s(device_labels[2], "Keyboard");
    for (int i = 0; i < XBOX_HOST_CONTROLLERS; ++i)
        ControllerLabel(i, physical, device_labels[i + 3], sizeof device_labels[i + 3]);
    const int devices[] = { PAD_DEVICE_AUTO, PAD_DEVICE_NONE, PAD_DEVICE_KEYBOARD, 0, 1, 2, 3, 4, 5, 6, 7 };
    int selected = 0;
    for (int i = 0; i < 3 + XBOX_HOST_CONTROLLERS; ++i) if (devices[i] == pad_mapping_device(g_input_player)) selected = i;
    ImGui::SetNextItemWidth(270.0f);
    if (ImGui::BeginCombo("Input device", device_labels[selected])) {
        for (int i = 0; i < 3 + XBOX_HOST_CONTROLLERS; ++i) {
            /* Only connected controllers are offered. The current choice stays
             * listed (as "disconnected") so an assignment to an unplugged
             * pad can still be seen and changed. */
            if (i >= 3 && !(physical & (1u << (i - 3))) && i != selected) continue;
            if (!ImGui::Selectable(device_labels[i], i == selected)) continue;
            pad_mapping_set_device(g_input_player, devices[i]);
            g_listening = -1; g_pending_vk = 0;
            SetStatus("Device changed; Save to keep settings");
        }
        if (!(physical & ((1u << XBOX_HOST_CONTROLLERS) - 1u)))
            ImGui::TextDisabled("No controllers connected");
        ImGui::EndCombo();
    }
    int type = pad_mapping_type(g_input_player);
    ImGui::SetNextItemWidth(270.0f);
    if (ImGui::Combo("Xbox controller type", &type, types, PAD_TYPE_COUNT)) {
        pad_mapping_set_type(g_input_player, type);
        g_listening = -1; g_pending_vk = 0;
        SetStatus("Controller type changed; Save to keep settings");
    }
    int slot = xbox_InputAssignedSlot(g_input_player);
    if (slot >= 0) {
        char using_label[64];
        ControllerLabel(slot, physical, using_label, sizeof using_label);
        ImGui::TextDisabled("Using %s", using_label);
    }
    else if (pad_mapping_device(g_input_player) == PAD_DEVICE_KEYBOARD)
        ImGui::TextDisabled("Using keyboard; bind keys below");
    else ImGui::TextDisabled("No physical controller assigned");
    ImGui::TextWrapped("Each player has separate bindings. Assigning a controller moves it from any other explicit assignment. "
                       "Automatic uses an available controller; Player 1 can also use bound keys.");

    switch (ResetSaveRow()) {
    case 1:
        pad_mapping_reset_player(g_input_player);
        g_listening = -1;
        SetStatus("This player's bindings reset to defaults");
        break;
    case 2:
        SetStatus(pad_mapping_save(NULL) ? "Saved doa3_input.ini"
                                         : "Could not write doa3_input.ini");
        break;
    }

    ImGui::Separator();

    if (!ImGui::BeginTable("##pad", 2,
                           ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                           ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp))
        return;

    ImGui::TableSetupColumn("Xbox control", ImGuiTableColumnFlags_WidthFixed, 140.0f);
    ImGui::TableSetupColumn("Bound to");
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    const char *group = nullptr;
    for (int row = 0; row < PADCTL_COUNT; ++row) {
        const int i = pad_ctl_display_order(row);
        const char *g = pad_ctl_group(i);
        if (!group || strcmp(g, group) != 0) {
            group = g;
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            ImGui::TableSetColumnIndex(0);
            ImGui::TextDisabled("%s", g);
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted("");
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted(pad_ctl_key(i));

        ImGui::TableSetColumnIndex(1);
        ImGui::PushID(i);
        const bool listening = (g_listening == i);
        char label[96];
        if (listening) _snprintf_s(label, sizeof(label), _TRUNCATE, "%s", "[ press an input ]");
        else           pad_binding_label(m->b[i], label, sizeof(label));

        if (listening) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.38f, 0.10f, 1.0f));
        if (ImGui::Button(label, ImVec2(-1.0f, 0.0f))) {
            g_listening  = listening ? -1 : i;
            g_armed      = false;
            g_pending_vk = 0;
        }
        if (listening) ImGui::PopStyleColor();

        if (ImGui::BeginPopupContextItem("##ctx")) {
            if (ImGui::MenuItem("Clear")) { m->b[i].kind = PADSRC_NONE; m->b[i].code = 0; }
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }
    ImGui::EndTable();

    /* Resolve a listening row. Skipping one frame keeps the initiating mouse
     * click (and a key still held from opening the menu) out of the capture. */
    if (g_listening >= 0 && g_listening < PADCTL_COUNT) {
        if (!g_armed) {
            g_armed = true;
        } else {
            PadBinding cap;
            if (CaptureBinding(cap)) {
                m->b[g_listening] = cap;
                char lbl[96];
                pad_binding_label(cap, lbl, sizeof(lbl));
                char msg[160];
                _snprintf_s(msg, sizeof(msg), _TRUNCATE, "%s bound to %s",
                            pad_ctl_key(g_listening), lbl);
                SetStatus(msg);
                g_listening = -1;
                g_armed = false;
            }
        }
    }


}

extern "C" volatile LONG g_doa3_trace_request;   /* recomp_manual.c */
extern "C" volatile LONG g_doa3_texdump_request;  /* nv2a_pgraph_d3d11.c */

void DrawControlsSection()
{
    static const char *players[] = { "Player 1", "Player 2", "Player 3", "Player 4" };
    bool focus_only = pad_mapping_focus_only() != 0;
    if (ImGui::Checkbox("Input only when focused", &focus_only)) {
        pad_mapping_set_focus_only(focus_only);
        SetStatus("Input focus setting changed; Save to keep settings");
    }
    ImGui::TextDisabled("Applies to keyboard and controllers for all players.");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Turn off to allow gameplay input while another app is active.");
    bool capture_menu = pad_mapping_capture_menu() != 0;
    if (ImGui::Checkbox("Capture gameplay input in Escape menu", &capture_menu)) {
        pad_mapping_set_capture_menu(capture_menu);
        SetStatus("Menu input setting changed; Save to keep settings");
    }
    ImGui::TextWrapped("On: while settings are open, controllers operate the menu without affecting the game. "
                       "Off: controllers keep playing the game and do not operate the menu (use the mouse "
                       "or keyboard). Applies to all players.");
    if (ImGui::BeginTabBar("Player controllers")) {
        for (int player = 0; player < 4; ++player) {
            if (!ImGui::BeginTabItem(players[player])) continue;
            if (g_input_player != player) {
                g_input_player = player;
                g_listening = -1; g_armed = false; g_pending_vk = 0;
                g_status[0] = 0;
            }
            ImGui::PushID(player);
            DrawPlayerControls();
            ImGui::PopID();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
/* ── Video section ────────────────────────────────────────────────────────*/

void DrawVideoSection()
{
    static const char *kWindow[] = { "Windowed", "Borderless fullscreen" };
    const char *kAspect[VIDEO_ASPECT_COUNT];
    for (int i = 0; i < VIDEO_ASPECT_COUNT; ++i) kAspect[i] = video_aspect_name(i);
    static const char *kScale[]  = { "1x (480p)", "2x (960p)", "3x (1440p)", "4x (1920p)" };

    switch (ResetSaveRow()) {
    case 1:
        video_set_window_mode(VIDEO_BORDERLESS);
        video_set_aspect(VIDEO_ASPECT_16_9);
        video_set_scale(3);
        g_menu_scale = 100;
        ResetPerformanceOverlay();
        SetStatus("Video settings reset to defaults");
        break;
    case 2: {
        int ok = video_settings_save();
        char v[16]; _snprintf_s(v, sizeof v, _TRUNCATE, "%d", g_menu_scale);
        ok = WritePrivateProfileStringA("General", "MenuScale", v, doa3_settings_ini_path()) && ok;
        SetStatus(ok ? "Saved doa3_settings.ini" : "Could not write doa3_settings.ini");
        break;
    }
    }

    ImGui::Separator();

    int wm = video_get_window_mode();
    if (ImGui::Combo("Window type", &wm, kWindow, IM_ARRAYSIZE(kWindow)) &&
        wm != video_get_window_mode())
        video_set_window_mode(wm);

    int ar = video_get_aspect();
    if (ImGui::Combo("Aspect ratio", &ar, kAspect, IM_ARRAYSIZE(kAspect)) &&
        ar != video_get_aspect())
        video_set_aspect(ar);

    int sc = video_get_scale() - 1;
    if (ImGui::Combo("Internal resolution", &sc, kScale, IM_ARRAYSIZE(kScale)) &&
        sc + 1 != video_get_scale())
        video_set_scale(sc + 1);

    /* 0.2.17: the Escape menu's own size (was in General in 0.2.16). */
    ImGui::Spacing();
    ImGui::SeparatorText("Menu");
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 12.0f);
    ImGui::SliderInt("Menu size", &g_menu_scale, 100, 250, "%d%%", ImGuiSliderFlags_AlwaysClamp);
    if (ImGui::IsItemDeactivatedAfterEdit()) g_menu_scale = (g_menu_scale + 12) / 25 * 25;   /* 25 % steps */
    HelpMarker("Size of this menu and its text, in 25% steps; applied when you let go of the slider.");

    ImGui::Spacing();
    ImGui::SeparatorText("Performance");
    DrawPerformanceDebugMenu();
}

/* Audio output settings. */

void DrawAudioSection()
{
    switch (ResetSaveRow()) {
    case 1:
        audio_set_volume(80);
        audio_streamer_mode_set(0);
        SetStatus("Audio settings reset to defaults");
        break;
    case 2:
        SetStatus(audio_settings_save() ? "Saved doa3_settings.ini"
                                       : "Could not write doa3_settings.ini");
        break;
    }

    ImGui::Separator();
    int volume = audio_get_volume();
    if (ImGui::SliderInt("Master volume", &volume, 0, 100, "%d%%",
                         ImGuiSliderFlags_AlwaysClamp))
        audio_set_volume(volume);
    ImGui::TextWrapped("Overall output level of the PC version, applied after the game's own mix "
                       "(music, voices, sound effects and movies). 100%% leaves the game unchanged; the default is 80%%. "
                       "Choose Save to keep it for next time.");

    ImGui::Spacing();
    ImGui::SeparatorText("Game volume");
    {
        static const char *const names[3] = { "Music", "Sound effects", "Voice" };
        const bool ready = game_audio_available() != 0;
        if (!ready) ImGui::BeginDisabled();
        for (int ch = 0; ch < 3; ++ch) {
            int v = game_audio_get(ch);
            if (ImGui::SliderInt(names[ch], &v, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp))
                game_audio_request(ch, v);
        }
        if (!ready) ImGui::EndDisabled();
        ImGui::TextWrapped("The same settings as the game's Options > Audio Setting, in its steps of 5. "
                           "They apply immediately and are stored in the game's save the next time the game "
                           "saves (leaving Options, or after the credits).");
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Streaming");
    {
        bool streamer = audio_streamer_mode_get() != 0;
        if (ImGui::Checkbox("Streamer mode", &streamer))
            audio_streamer_mode_set(streamer ? 1 : 0);
        ImGui::TextWrapped("Silences Aerosmith's licensed songs (\"Nine Lives\", \"Amazing\" and \"Home Tonight\") "
                           "wherever they play, including the opening, the credits and the sound test, by "
                           "setting the game's music volume to 0 while they play, then restoring your level. "
                           "Your saved music volume is never changed.");
        if (game_audio_streamer_muting())
            ImGui::TextDisabled("A licensed song is muted right now.");
    }
}

/* ── General section ──────────────────────────────────────────────────────*/

void DrawGeneralSection()
{
    switch (ResetSaveRow()) {
    case 1:
        doa3_log_set_enabled(0);
        update_check_set_enabled(1);
        dip_reset();
        SetStatus("General settings reset to defaults");
        break;
    case 2: {
        int ok = doa3_log_save();
        ok = update_check_save() && ok;
        ok = dip_save() && ok;
        SetStatus(ok ? "Saved doa3_settings.ini"
                     : "Could not write doa3_settings.ini");
        break;
    }
    }

    ImGui::Separator();

    if (g_doa3_folder_write_error) {
        ImGui::PushTextWrapPos(0.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.55f, 0.45f, 1.0f), "%s (Windows error %lu)", kFolderWarning,
                           g_doa3_folder_write_error);
        ImGui::PopTextWrapPos();
        ImGui::Separator();
    }

    /* 1.0.2: close the game from the menu (same as the window's close button
     * or Alt+F4). */
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.62f, 0.12f, 0.12f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.18f, 0.18f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.90f, 0.22f, 0.22f, 1.0f));
    if (ImGui::Button("Exit Game")) {
        HWND hwnd = d3d8_GetHWND();
        if (hwnd) PostMessageA(hwnd, WM_CLOSE, 0, 0);
    }
    ImGui::PopStyleColor(3);
    HelpMarker("Closes Dead or Alive 3. Settings you have not saved are not kept, and progress is "
               "kept only as far as the game itself last saved.");
    ImGui::Spacing();

    bool upd = update_check_enabled() != 0;
    if (ImGui::Checkbox("Check for updates at boot", &upd))
        update_check_set_enabled(upd ? 1 : 0);
    HelpMarker("At start-up, asks GitHub for the latest release of this port (one small request; "
               "nothing is downloaded or installed). A newer release is announced over the game "
               "and here.");
    {
        int st = update_check_state();
        if (st == 1) ImGui::TextDisabled("Checking for updates...");
        else if (st == 2) ImGui::TextDisabled("Up to date (version %s).", DOA3_VER_STR_UI);
        else if (st == 4) ImGui::TextDisabled("Could not reach GitHub to check for updates.");
        else if (st == 3) {
            ImGui::Text("Version %s is available (you have %s).", update_check_latest(), DOA3_VER_STR_UI);
            ImGui::SameLine();
            if (ImGui::SmallButton("Open release page"))
                ShellExecuteA(NULL, "open", update_check_url(), NULL, NULL, SW_SHOWNORMAL);
        }
        if (st != 1) {
            if (st) ImGui::SameLine();
            if (ImGui::SmallButton("Check now")) update_check_now();
        }
    }

    /* Dip switches: quality-of-life toggles for the game, all off by default
     * (off = the original game's behaviour). */
    ImGui::Spacing();
    ImGui::SeparatorText("Dip Switches");
    for (int i = 0; i < DIP_COUNT; i++) {
        if (dip_hidden((dip_switch)i)) continue;
        bool d = dip_get((dip_switch)i) != 0;
        if (ImGui::Checkbox(dip_label((dip_switch)i), &d))
            dip_set((dip_switch)i, d ? 1 : 0);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 28.0f);
            ImGui::TextUnformatted(dip_help((dip_switch)i));
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    /* 1.0.2: what a bug report needs, without the Debug tab. */
    ImGui::Spacing();
    ImGui::SeparatorText("Logging");
    bool on = doa3_log_enabled() != 0;
    if (ImGui::Checkbox("Write log files", &on))
        doa3_log_set_enabled(on ? 1 : 0);
    HelpMarker("Writes doa3_log.txt next to the game, for bug reports. Choose Save to keep this "
               "setting for next time.");
    if (ImGui::Button("Create a trace file")) {
        InterlockedExchange(&g_doa3_trace_request, 1);
        SetStatus("The next frame will be traced to the Diagnostics folder");
    }
    HelpMarker("Records the next frame's graphics commands (mtrace_manual_*.txt) and a screenshot "
               "(aa_*.bmp) in the Diagnostics folder next to the game. Use it while a visual bug is "
               "on screen; the game keeps running behind this menu. The folder is emptied at the next "
               "launch, so send the files before restarting the game.");
}

/* 0.2.16: a newer release: a notice at the top for 12 seconds, or until the
 * menu is opened (the General tab has the details and the link). */
ULONGLONG g_update_banner_until;
bool UpdateBannerPending()
{
    if (update_check_state() != 3) return false;
    if (!g_update_banner_until) g_update_banner_until = GetTickCount64() + 12000;
    return !g_visible && GetTickCount64() < g_update_banner_until;
}
void DrawUpdateBanner()
{
    if (!UpdateBannerPending()) return;
    char text[160];
    _snprintf_s(text, sizeof text, _TRUNCATE, "Dead or Alive 3 recomp %s is available. Esc > Game for the release page.",
                update_check_latest());
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f, vp->WorkPos.y + 24.0f),
                            ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.75f);
    if (ImGui::Begin("##update_banner", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize))
        ImGui::TextUnformatted(text);
    ImGui::End();
}

/* 1.0.2: DOA3.exe's folder refused a test write at boot (main.c): say so
 * over the game for 20 seconds, or until the menu is opened, and on the
 * General tab. */
ULONGLONG g_folder_banner_until;
bool FolderBannerPending()
{
    if (!g_doa3_folder_write_error) return false;
    if (!g_folder_banner_until) g_folder_banner_until = GetTickCount64() + 20000;
    return !g_visible && GetTickCount64() < g_folder_banner_until;
}
void DrawFolderBanner()
{
    if (!FolderBannerPending()) return;
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f, vp->WorkPos.y + 64.0f),
                            ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.85f);
    if (ImGui::Begin("##folder_banner", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 36.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.55f, 0.45f, 1.0f), "%s", kFolderWarning);
        ImGui::PopTextWrapPos();
    }
    ImGui::End();
}

/* 1.0.2: controller hotkeys, on any connected controller, while the game
 * window has focus and no binding is being captured:
 *   L3 + R3      open / close this menu
 *   L1 / R1      previous / next tab, while the menu is open
 *   L3 + R1      screenshot
 *   L3 + Y       performance overlay on / off
 * A combination fires once when it becomes held. The buttons still reach
 * the game as usual (R1 is Black by default), as on the console. */
extern "C" void doa3_ui_toggle(void);
WORD g_hotkey_prev[XBOX_HOST_CONTROLLERS];
int NextTab(int from, int dir)
{
    for (int k = 1; k <= 7; ++k) {
        int t = ((from + dir * k) % 7 + 7) % 7;
        if (t == 5 && !training_available()) continue;
        if (t == 6 && !g_debug_tab) continue;
        return t;
    }
    return from;
}
void PollControllerHotkeys()
{
    HWND hwnd = d3d8_GetHWND();
    if (!hwnd || GetForegroundWindow() != hwnd || g_listening >= 0 || g_screenshot_listening) {
        memset(g_hotkey_prev, 0, sizeof g_hotkey_prev);
        return;
    }
    DWORD mask = xbox_InputPhysicalMask();
    for (int slot = 0; slot < XBOX_HOST_CONTROLLERS; ++slot) {
        XINPUT_STATE st;
        if (!(mask & (1u << slot)) || xbox_InputReadPhysical(slot, &st) != ERROR_SUCCESS) {
            g_hotkey_prev[slot] = 0;
            continue;
        }
        WORD now = st.Gamepad.wButtons, before = g_hotkey_prev[slot];
        g_hotkey_prev[slot] = now;
        auto held = [](WORD b, WORD combo) { return (b & combo) == combo; };
        auto fired = [&](WORD combo) { return held(now, combo) && !held(before, combo); };
        const WORD L3 = XINPUT_GAMEPAD_LEFT_THUMB, R3 = XINPUT_GAMEPAD_RIGHT_THUMB;
        const WORD L1 = XINPUT_GAMEPAD_LEFT_SHOULDER, R1 = XINPUT_GAMEPAD_RIGHT_SHOULDER;
        if (fired(L3 | R3)) { doa3_ui_toggle(); continue; }
        if (fired(L3 | R1)) { d3d8_screenshot_request(); continue; }
        if (fired(L3 | XINPUT_GAMEPAD_Y)) { TogglePerformanceOverlay(); continue; }
        if (g_visible && g_screen == SCR_CLASSIC && !(now & L3) && !(before & L3)) {
            if (fired(L1)) g_tab_request = NextTab(g_tab_current, -1);
            else if (fired(R1)) g_tab_request = NextTab(g_tab_current, +1);
        }
    }
}

/* 1.0.2: a short notice when a screenshot has been written (or failed). It
 * appears after the frame was captured, so it is never in the picture. */
ULONGLONG g_shot_toast_until;
int g_shot_toast_seen = -1, g_shot_toast_ok;
bool ScreenshotToastPending()
{
    int ok = 0, n = d3d8_screenshot_finished(&ok);
    if (g_shot_toast_seen < 0) g_shot_toast_seen = n;   /* nothing from before this check */
    if (n != g_shot_toast_seen) {
        g_shot_toast_seen = n;
        g_shot_toast_ok = ok;
        g_shot_toast_until = GetTickCount64() + 2500;
    }
    return GetTickCount64() < g_shot_toast_until;
}
void DrawScreenshotToast()
{
    if (!ScreenshotToastPending()) return;
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x - 16.0f,
                                   vp->WorkPos.y + vp->WorkSize.y - 16.0f),
                            ImGuiCond_Always, ImVec2(1.0f, 1.0f));
    ImGui::SetNextWindowBgAlpha(0.75f);
    if (ImGui::Begin("##screenshot_toast", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize))
        ImGui::TextUnformatted(g_shot_toast_ok ? "Screenshot saved to the Screenshots folder"
                                               : "Screenshot failed (check disk space and folder permissions)");
    ImGui::End();
}

/* ── Online section ───────────────────────────────────────────────────────*/

/* Online play has something to show over the game (a wait, a disconnect, a
 * desync) even when the menu is closed. */
bool NetplayBannerPending()
{
    char b[8];
    return netplay_banner(b, sizeof(b)) != 0;
}

void DrawNetplayBanner()
{
    char text[200];
    if (!netplay_banner(text, sizeof(text))) return;
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImVec2 size = ImGui::CalcTextSize(text);
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f, vp->WorkPos.y + 24.0f),
                            ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(size.x + 32.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.75f);
    if (ImGui::Begin("##netplay_banner", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(text);
    }
    ImGui::End();
}

void DrawOnlineSection()
{
    static const char *kState[] = { "Off", "Armed - waiting for VS character select", "Session running" };
    static char s_join[128];
    static int  s_join_init = 0;
    static int  s_port = 0;
    np_status st;
    npl_status nl;
    netplay_get_status(&st);
    netplay_get(&nl);
    if (!s_join_init) {
        s_join_init = 1;
        s_join[0] = 0;   /* starts empty; nothing pre-filled */
    }
    if (!s_port) s_port = (int)netplay_port();

    bool online = nl.state != NPL_OFFLINE;
    bool connected = nl.state == NPL_CONNECTED || nl.state == NPL_INMATCH;

    /* status, with Save at the right end of the same line */
    {
        const ImGuiStyle &style = ImGui::GetStyle();
        float w = ImGui::CalcTextSize("Save").x + style.FramePadding.x * 2.0f;
        float right = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x;
        ImGui::AlignTextToFramePadding();
        ImGui::PushTextWrapPos(right - w - style.ItemSpacing.x);
        if (nl.state == NPL_INMATCH && nl.stalling)
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.3f, 1.0f), "%s", nl.status);
        else if (nl.desynced)
            ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.35f, 1.0f), "%s", nl.status);
        else
            ImGui::TextWrapped("%s", nl.status);
        ImGui::PopTextWrapPos();
        ImGui::SameLine(right - w);
        if (ImGui::Button("Save")) {
            netplay_set_port((unsigned short)s_port);
            SetStatus(netplay_settings_save() ? "Saved doa3_settings.ini" : "Could not write doa3_settings.ini");
        }
        if (g_status[0]) ImGui::TextDisabled("%s", g_status);
    }
    if (nl.event[0]) ImGui::TextWrapped("%s", nl.event);

    (void)connected;
    if (nl.is_host && online) {
        /* the host's code: the one thing the other player needs */
        ImGui::SeparatorText("Your code");
        if (nl.my_code[0]) {
            ImGui::Text("%s", nl.my_code);
            ImGui::SameLine();
            if (ImGui::SmallButton("Copy")) { ImGui::SetClipboardText(nl.my_code); SetStatus("Code copied"); }
            if (nl.my_code_note[0]) ImGui::TextDisabled("%s", nl.my_code_note);
        }
    } else if (!online) {
        ImGui::SeparatorText("Join");
        ImGui::SetNextItemWidth(220);
        bool enter = ImGui::InputText("##code", s_join, sizeof(s_join), ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine();
        if (ImGui::Button("Join") || enter) netplay_use_code(s_join);
        ImGui::SameLine();
        if (ImGui::Button("Host")) {
            netplay_set_port((unsigned short)s_port);
            netplay_host((unsigned short)s_port);
        }
    }
    if (online && ImGui::Button("Disconnect")) netplay_disconnect();
    if (nl.hint[0]) ImGui::TextWrapped("%s", nl.hint);

    /* delay */
    ImGui::SeparatorText("Input delay");
    {
        int automatic = netplay_delay_auto();
        int manual = netplay_delay_manual();
        bool in_match = nl.state == NPL_INMATCH;
        if (in_match) ImGui::BeginDisabled();
        if (ImGui::RadioButton("Auto (from the ping)", automatic != 0)) netplay_set_delay(1, manual);
        ImGui::SameLine();
        if (ImGui::RadioButton("Manual", automatic == 0)) netplay_set_delay(0, manual);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120);
        if (ImGui::SliderInt("frames", &manual, 1, 8)) netplay_set_delay(automatic, manual);
        if (in_match) ImGui::EndDisabled();
    }

    ImGui::Separator();
    if (nl.state == NPL_INMATCH)
        ImGui::Text("%s Battle online, %.1f frames ahead of the other side",
                    nl.match_tag ? "Tag" : "Single", nl.frames_ahead);

    ImGui::Separator();
    if (ImGui::CollapsingHeader("Developer Settings")) {
        static int role = NP_ROLE_HOST;
        bool armed = netplay_armed() != 0;
        bool running = st.state == NP_STATE_ACTIVE;

        if (running || online) ImGui::BeginDisabled();
        ImGui::RadioButton("Host path", &role, NP_ROLE_HOST);
        ImGui::SameLine();
        ImGui::RadioButton("Joiner path", &role, NP_ROLE_JOINER);
        bool mirror = netplay_mirror() != 0;
        if (ImGui::Checkbox("Player 2 mirrors Player 1 (one controller)", &mirror))
            netplay_set_mirror(mirror ? 1 : 0);
        if (ImGui::Checkbox("Arm local session", &armed))
            netplay_set_armed(armed ? 1 : 0, role);
        if (running || online) ImGui::EndDisabled();

        bool diag = netplay_diag() != 0;
        if (ImGui::Checkbox("Diagnostic logs", &diag))
            netplay_set_diag(diag ? 1 : 0);
        ImGui::Text("State: %s", kState[st.state < 3 ? st.state : 0]);
        if (running) {
            ImGui::Text("%s Battle, %s path, frame %u",
                        st.tag ? "Tag" : "Single",
                        st.role == NP_ROLE_JOINER ? "joiner" : "host", st.frame);
            ImGui::Text("Digest %016llX", (unsigned long long)st.digest);
        }
        ImGui::Text("Sessions completed: %u", st.sessions);
        if (st.last_log[0]) ImGui::TextDisabled("Last log: %s", st.last_log);

        /* Determinism check: record a session, then replay it (ideally in a
         * fresh launch) and see whether every frame's digest matches. */
        ImGui::Separator();
        bool record = netplay_record() != 0;
        if (ImGui::Checkbox("Record each session to a replay file", &record))
            netplay_set_record(record ? 1 : 0);

        static char s_files[32][128];
        static int s_nfiles = -1, s_pick = 0;
        if (s_nfiles < 0 || ImGui::Button("Refresh")) {
            s_nfiles = netplay_list_replays(s_files, 32);
            if (s_pick >= s_nfiles) s_pick = 0;
        }
        if (s_nfiles > 0) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(330);
            if (ImGui::BeginCombo("##replay", s_files[s_pick])) {
                for (int i = 0; i < s_nfiles; i++)
                    if (ImGui::Selectable(s_files[i], i == s_pick)) s_pick = i;
                ImGui::EndCombo();
            }
            if (running) ImGui::BeginDisabled();
            if (ImGui::Button("Replay selected")) netplay_request_replay(s_files[s_pick]);
            if (running) ImGui::EndDisabled();
        } else {
            ImGui::SameLine();
            ImGui::TextDisabled("No replay files yet");
        }
        if (running && st.replaying) {
            if (st.diverged_at == 0xFFFFFFFFu)
                ImGui::Text("Replay frame %u / %u - matching", st.frame, st.replay_frames);
            else
                ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.35f, 1.0f),
                                   "Replay frame %u / %u - diverged at frame %u",
                                   st.frame, st.replay_frames, st.diverged_at);
        }
        if (st.message[0]) ImGui::TextWrapped("%s", st.message);
    }
}

void DrawDebugSection()
{
    switch (ResetSaveRow()) {
    case 1:
        doa3_auto_capture_set(0);
        combat_debug_set_flags(0);
        d3d8_debug_views_reset();
        SetStatus("Debug settings reset to defaults");
        break;
    case 2:
        SetStatus(debug_settings_save() ? "Saved doa3_settings.ini"
                                        : "Could not write doa3_settings.ini");
        break;
    }
    ImGui::Separator();
    DrawCombatDebugMenu();   /* hitboxes, hurtboxes, stage hazards */
    if (ImGui::CollapsingHeader("Frame advance")) {
        bool paused = g_doa3_frame_pause != 0;
        if (ImGui::Button(paused ? "Resume (F7)" : "Pause (F7)")) FramePauseToggle();
        ImGui::SameLine();
        ImGui::BeginDisabled(!paused);
        if (ImGui::Button("Step one frame (F8)")) FrameStep();
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::TextDisabled("frame %u", g_doa3_frames_presented);
        bool trace = g_doa3_frame_step_trace != 0;
        if (ImGui::Checkbox("Trace each stepped frame", &trace))
            InterlockedExchange(&g_doa3_frame_step_trace, trace ? 1 : 0);
        HelpMarker("Pause holds the game at the end of a frame, with the sound off; Step lets exactly one "
                   "more frame run (hold F8 to keep stepping). With the trace option, each stepped frame "
                   "writes mtrace_manual_*.txt and aa_*.bmp to the Diagnostics folder. F7 and F8 work with "
                   "this menu closed while the Debug tab is enabled. Not available during online play or "
                   "while a movie is playing.");
        ImGui::Spacing();
        static int s_trace_n = 10;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10.0f);
        ImGui::SliderInt("##trace_n", &s_trace_n, 2, 30, "%d frames");
        ImGui::SameLine();
        if (ImGui::Button("Trace the next frames")) {
            InterlockedExchange(&g_doa3_trace_frames, s_trace_n);
            SetStatus("The next frames will be traced to the Diagnostics folder");
        }
        HelpMarker("Writes a trace and a screenshot for each of the next frames, starting with the next "
                   "one. Each frame takes about 20 MB at 4x internal resolution.");
        if (g_doa3_trace_frames > 0) ImGui::TextDisabled("%ld frame(s) left to trace", g_doa3_trace_frames);
        ImGui::Spacing();
        int slow = (int)g_doa3_frame_slow_ms;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10.0f);
        if (ImGui::SliderInt("Simulate a slow PC", &slow, 0, 120, slow ? "+%d ms per frame" : "off"))
            InterlockedExchange(&g_doa3_frame_slow_ms, slow);
        HelpMarker("Makes every frame take this much longer, with the game's clocks running normally, to "
                   "reproduce what slower computers see (Omega's KO slowed to about 90 ms per frame on a "
                   "player's laptop). 80 ms is about 11 frames per second. Not saved; off at every launch.");
        int rslow = (int)g_doa3_rthread_slow_ms;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10.0f);
        if (ImGui::SliderInt("Simulate a slow GPU thread", &rslow, 0, 30, rslow ? "+%d ms per batch" : "off"))
            InterlockedExchange(&g_doa3_rthread_slow_ms, rslow);
        HelpMarker("Delays the thread that turns the game's graphics commands into PC graphics by this much "
                   "per batch of commands (several per frame), so it falls behind the game as it can on slower "
                   "computers. Not saved; off at "
                   "every launch.");
    }
    if (ImGui::CollapsingHeader("Vision modes")) {
        int view = d3d8_debug_view_get();
        if (ImGui::Combo("Render view", &view, "Normal\0Wireframe\0Vertex colors\0UV checker\0Distance\0"))
            d3d8_debug_view_set(view);
        const char *view_help[] = {
            "Original game rendering.",
            "Shows triangle edges to inspect meshes and intersecting surfaces.",
            "Shows vertex lighting/colors without textures or fog.",
            "Shows the first UV set: red = U, green = V, blue = checker squares.",
            "Shows distance along the view direction: black is near, white is far."
        };
        HelpMarker(view_help[d3d8_debug_view_get()]);
        if (view == D3D8_DEBUG_DISTANCE) {
            float range = d3d8_debug_distance_get();
            if (ImGui::SliderFloat("Distance range", &range, 10, 100000, "%.0f", ImGuiSliderFlags_Logarithmic))
                d3d8_debug_distance_set(range);
        }
        bool backfaces = d3d8_debug_backfaces_get() != 0;
        if (ImGui::Checkbox("Show backfaces", &backfaces)) d3d8_debug_backfaces_set(backfaces);
        if (ImGui::Button("Restore normal rendering")) d3d8_debug_views_reset();
        HelpMarker("Views apply to depth-tested geometry. Transparency and cutouts may appear solid. "
                   "Backfaces can only reveal geometry submitted by the game. These options are temporary.");
        if (d3d8_debug_view_error()[0]) ImGui::TextWrapped("%s", d3d8_debug_view_error());
    }
    if (ImGui::CollapsingHeader("Diagnostics")) {
        bool cap = doa3_auto_capture_enabled() != 0;
        char env_value[8];
        bool env_forced = GetEnvironmentVariableA("DOA3_AUTO_CAPTURE", env_value, sizeof env_value) > 0;
        if (env_forced) ImGui::BeginDisabled();
        if (ImGui::Checkbox("Automatic diagnostic captures", &cap))
            doa3_auto_capture_set(cap ? 1 : 0);
        if (env_forced) ImGui::EndDisabled();
        HelpMarker("Writes developer traces (mtrace_*.txt) and their screenshots (aa_*.bmp) to the "
                   "Diagnostics folder next to the game while you play, for bug reports. Leave off for "
                   "normal play. Choose Save to keep this setting for next time.");
        if (env_forced)
            ImGui::TextDisabled("Set by the DOA3_AUTO_CAPTURE environment variable for this launch.");
        ImGui::TextDisabled("Create a trace file: Game > Logging.");
        if (ImGui::Button("Save the next frame's textures")) {
            InterlockedExchange(&g_doa3_texdump_request, 1);
            SetStatus("The next frame's textures will be saved to the Textures folder");
        }
        HelpMarker("Saves every texture drawn in the next frame as a .dds file in a Textures folder next "
                   "to the game, named by its memory address and size. Files are kept between launches.");
        if (ImGui::Button("Save a game memory snapshot")) {
            unsigned fr = doa3_save_ram_snapshot();
            char msg[96];
            if (fr) _snprintf_s(msg, sizeof msg, _TRUNCATE, "Saved Snapshots\\ram_%06u.bin", fr);
            else strcpy_s(msg, "Could not write the snapshot");
            SetStatus(msg);
        }
        HelpMarker("Writes the game's 64 MB of memory to a Snapshots folder next to the game (kept between "
                   "launches). Take two a second apart while a logic bug is showing and two once it is gone, "
                   "for comparison.");
    }
}

/* 0.2.17: Photo Mode tab (from Debug): free camera, hiding the fighters and
 * the HUD, and screenshots. */
void DrawPhotoModeSection()
{
    switch (ResetSaveRow()) {
    case 1:
        free_camera_set_enabled(0);
        free_camera_set_speed(10);
        free_camera_reset_fov();
        debug_hide_ui_set(0);
        debug_hide_player_set(0, 0);
        debug_hide_player_set(1, 0);
        debug_screenshot_key_set(VK_F12);
        g_screenshot_listening = false;
        SetStatus("Photo Mode settings reset to defaults");
        break;
    case 2:
        SetStatus(debug_settings_save() && debug_screenshot_key_save() ? "Saved doa3_settings.ini"
                                                                       : "Could not write doa3_settings.ini");
        break;
    }
    ImGui::SeparatorText("Free camera");
    {
        bool camera = free_camera_enabled() != 0;
        if (ImGui::Checkbox("Free camera", &camera)) {
            if (!free_camera_set_enabled(camera ? 1 : 0))
                SetStatus("Enter an offline 3D scene before enabling the camera.");
        }
        float speed = free_camera_speed();
        if (ImGui::SliderFloat("Camera speed", &speed, 1.0f, 20.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
            free_camera_set_speed(speed);
        if (ImGui::Button("Reset to game camera")) free_camera_reset();
        HelpMarker("Close this menu to fly. Keyboard: WASD move, mouse looks, E/Q up/down, Shift fast, "
                   "Ctrl slow. Controller: left stick moves, right stick looks, RT/LT up/down, LB fast, "
                   "RB slow. Esc releases the mouse. Untick Free camera to restore the game view. Fighter "
                   "controls are blocked while flying; the fight keeps running. Camera mode is temporary "
                   "and unavailable during online play.");
    }
    ImGui::SeparatorText("Hide characters and HUD");
    {
        ImGui::BeginDisabled(netplay_active() != 0);
        for (int side = 0; side < 2; ++side) {
            bool hide = debug_hide_player_get(side) != 0;
            if (ImGui::Checkbox(side ? "Hide Player 2" : "Hide Player 1", &hide))
                debug_hide_player_set(side, hide);
        }
        ImGui::EndDisabled();
        HelpMarker("Character visibility is session only. Gameplay and collision boxes stay active.");
        bool hidden = debug_hide_ui_get() != 0;
        if (ImGui::Checkbox("Hide UI", &hidden)) debug_hide_ui_set(hidden);
        HelpMarker("Hides game screen overlays, including HUD and menus. Some fades may also disappear. "
                   "Escape always opens these settings so you can restore the UI.");
        ImGui::SeparatorText("Screenshots");
        PadBinding key = { PADSRC_KEY, debug_screenshot_key_get() };
        char name[80];
        pad_binding_label(key, name, sizeof(name));
        ImGui::Text("Screenshot key: %s (change it in Controls)", name);
        if (ImGui::Button("Take screenshot")) d3d8_screenshot_request();
        ImGui::TextWrapped("%s", d3d8_screenshot_status());
        HelpMarker("Captures the game at its render resolution without this settings menu.");
    }
}

static int HumanSide()
{
    int port = training_human_port();
    if (port < 0) return -1;
    return training_human_side();
}

void DrawTrainingSection()
{
    static int slot=0;
    static bool loop=false;
    TrainingStatus status;
    training_status(&status);
    if (!training_available()) {
        ImGui::TextDisabled("Available only in offline Sparring.");
        return;
    }
    DrawMoveInfoToggle();   /* 0.2.18: frame data (was Debug > Fight information) */
    ImGui::SeparatorText("Dummy input recording");
    ImGui::TextUnformatted("Record inputs as the dummy to have it play them back.");
    HelpMarker("Record temporarily gives you control of the dummy; press Escape to finish recording "
               "or stop playback. Each slot holds up to 10 seconds. Directions replay relative to the "
               "dummy's facing, so forward stays forward when the fighters switch sides; positions are "
               "not reset. Recordings last for the current Sparring session; recording "
               "again replaces the selected slot. Guard, counter-action and counter-hit settings are in "
               "the game's own Sparring pause menu.");
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4.0f);
    ImGui::Combo("Slot", &slot, "1\0" "2\0" "3\0");
    ImGui::SameLine();
    ImGui::TextDisabled("%.1f / %.0f s", status.length[slot]/60.0f, TRAINING_MAX_FRAMES/60.0f);
    ImGui::SameLine();
    ImGui::Checkbox("Loop", &loop);
    bool ready=training_can_record()!=0;
    ImGui::BeginDisabled(!ready);
    if (ImGui::Button("Record dummy") && training_start(TRAINING_RECORD,slot,0)) {
        g_visible=false;
        pad_mapping_set_menu_open(0);
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(!status.length[slot]);
    if (ImGui::Button("Play") && training_start(TRAINING_PLAY,slot,loop)) {
        g_visible=false;
        pad_mapping_set_menu_open(0);
    }
    ImGui::EndDisabled();
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Clear slot")) training_clear(slot);
    if (!ready) {
        ImGui::TextDisabled("Not available right now");
        HelpMarker("Enter a solo Sparring fight, close the game's pause menu, and turn off Exercise "
                   "and free camera to record or play.");
    }

    /* Second controller drives the dummy (0.1.12). */
    ImGui::Spacing();
    ImGui::SeparatorText("Second controller");
    {
        int enabled = 0, port = 1;
        training_control_get(&enabled, &port);
        int human = training_human_port();
        bool tag = training_control_partner() != 0;
        const char *who = tag ? "Partner" : "Dummy";
        const char *cpu = tag ? "your controller" : "CPU";
        bool on = enabled != 0;
        if (ImGui::Checkbox(tag ? "Controller controls your tag partner###ctrl2" : "Controller controls the dummy###ctrl2", &on))
            training_control_set(on ? 1 : 0, port);
        HelpMarker(tag ?
            "Tag Sparring: the chosen controller plays your tag partner while it is on point, as when "
            "two players share a team in Tag Battle; you play the other member. Ports follow Controls > "
            "Player N. The partner goes back to your controller whenever that controller is unplugged." :
            "Ports follow Controls > Player N. The dummy goes back to the CPU whenever that controller is "
            "unplugged, and while a recording plays or records.");
        char preview[48];
        _snprintf_s(preview, sizeof preview, _TRUNCATE, "Player %d port%s", port + 1,
                    port == human ? " (yours)" : training_port_connected(port) ? "" : " (not connected)");
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15.0f);
        if (ImGui::BeginCombo("Second controller", preview)) {
            for (int p = 0; p < 4; ++p) {
                char item[48];
                _snprintf_s(item, sizeof item, _TRUNCATE, "Player %d port%s", p + 1,
                            p == human ? " (yours)" : training_port_connected(p) ? "" : " (not connected)");
                ImGui::BeginDisabled(p == human);
                if (ImGui::Selectable(item, p == port)) training_control_set(enabled, p);
                ImGui::EndDisabled();
            }
            ImGui::EndCombo();
        }
        if (!enabled) ImGui::TextDisabled("%s: %s", who, cpu);
        else if (training_control_active()) ImGui::TextDisabled("%s: Player %d controller", who, port + 1);
        else if (port == human) ImGui::TextDisabled("%s: %s (that is your own port)", who, cpu);
        else if (!training_port_connected(port)) ImGui::TextDisabled("%s: %s (no controller on that port)", who, cpu);
        else if (tag) ImGui::TextDisabled("Partner: your controller until the fight is running");
        else ImGui::TextDisabled("Dummy: CPU until the fight is running and this menu is closed");
    }

    /* Stagger Escape (0.1.12). */
    ImGui::Spacing();
    ImGui::SeparatorText("Stagger Escape");
    ImGui::TextUnformatted("Hold how quickly each fighter recovers from a critical stun.");
    HelpMarker("The critical-stun mash value: each input in a critical stun or guard break adds 3% (up to "
               "100%) and every idle frame takes 1% (down to 0%); it sets how fast the stun animation "
               "plays (0% = normal speed, 100% = double). Hold sets the value at the start of every frame, "
               "and the game applies that frame's input on top.");
    {
        int human = HumanSide();
        for (int side = 0; side < 2; ++side) {
            int hold = 0; float value = 1.0f;
            training_stagger_hold_get(side, &hold, &value);
            ImGui::PushID(side);
            /* Shown as a percentage: 1.00 (none) = 0 %, 2.00 (max) = 100 %. */
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Fighter %d%s: %3.0f%%", side + 1,
                        side == human ? " (you)  " : human >= 0 ? " (dummy)" : "",
                        (training_stagger_get(side) - 1.0f) * 100.0f);
            ImGui::SameLine();
            bool h = hold != 0;
            if (ImGui::Checkbox("Hold", &h)) training_stagger_hold(side, h ? 1 : 0, value);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 12.0f);
            float pct = (value - 1.0f) * 100.0f;
            if (ImGui::SliderFloat("##value", &pct, 0.0f, 100.0f, "%.0f%%"))
                training_stagger_hold(side, hold, 1.0f + pct / 100.0f);
            ImGui::PopID();
        }
    }
}

void DrawTrainingBanner()
{
    TrainingStatus status;
    training_status(&status);
    if (status.mode==TRAINING_IDLE) return;
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x*.5f,12),ImGuiCond_Always,ImVec2(.5f,0));
    ImGui::SetNextWindowBgAlpha(.8f);
    if (ImGui::Begin("##training_recording",nullptr,ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings))
        ImGui::Text("%s dummy | Slot %d | %.1fs | Escape: stop",
            status.mode==TRAINING_RECORD ? "Recording" : "Playing",status.slot+1,status.frame/60.0f);
    ImGui::End();
}

void DrawMenu()
{
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f,
                                   vp->WorkPos.y + vp->WorkSize.y * 0.5f),
                            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    {   float s = g_menu_scale_built ? g_menu_scale_built / 100.0f : 1.0f;
        ImGui::SetNextWindowSize(ImVec2(560 * s, 460 * s), g_menu_resize ? ImGuiCond_Always : ImGuiCond_Appearing);
        g_menu_resize = false; }

    bool open = true;
    bool debug_visible = false;
    if (ImGui::Begin("Dead or Alive 3", &open, ImGuiWindowFlags_NoCollapse)) {
        if (ImGui::BeginTabBar("##tabs")) {
            /* 0.2.16: General first; the Online tab is hidden until online
             * play works on this version of the game. */
            /* 1.0.2: tab index order (L1 / R1 move through the enabled ones). */
            auto tab = [&](int index, const char *name) {
                ImGuiTabItemFlags f = g_tab_request == index ? ImGuiTabItemFlags_SetSelected : 0;
                if (!ImGui::BeginTabItem(name, nullptr, f)) return false;
                g_tab_current = index;
                return true;
            };
            if (tab(0, "General")) {
                DrawGeneralSection();
                ImGui::EndTabItem();
            }
            if (tab(1, "Controls")) {
                DrawControlsSection();
                ImGui::EndTabItem();
            }
            if (tab(2, "Video")) {
                DrawVideoSection();
                ImGui::EndTabItem();
            }
            if (tab(3, "Audio")) {
                DrawAudioSection();
                ImGui::EndTabItem();
            }
            if (tab(4, "Photo Mode")) {
                debug_visible = true;    /* the screenshot key listener lives here now */
                DrawPhotoModeSection();
                ImGui::EndTabItem();
            }
            ImGui::BeginDisabled(!training_available());
            if (tab(5, "Sparring")) {
                DrawTrainingSection();
                ImGui::EndTabItem();
            }
            ImGui::EndDisabled();
            /* 0.2.17: Debug is hidden unless DOA3_DEBUG=1 is set, or toggled
             * for this session with Ctrl+Shift+D while this menu is open. */
            if (g_debug_tab && tab(6, "Debug")) {
                DrawDebugSection();
                ImGui::EndTabItem();
            }
            g_tab_request = -1;

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
    if (!debug_visible || !open) g_screenshot_listening = false;

    if (!open) MenuBack();
    /* B on a pad leaves the window too (not while a binding is listened for,
     * an item is being edited, or a popup is open). */
    else if (ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false) && g_listening < 0 &&
             !g_screenshot_listening && !ImGui::IsAnyItemActive() &&
             !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel))
        MenuBack();
}

/* ── List menu screens (1.0.4) ──────────────────────────────────────────
 * Main, Display, Sound, Game and Quit use the list engine (pause_menu.cpp).
 * A screen's settings are saved to doa3_settings.ini when it is left (Back,
 * B, Esc, or closing the menu) if any of them changed. */

const char *const kMenuTitle = "DEAD OR ALIVE 3";
const char *const kMenuVersion = "v" DOA3_VER_STR_UI;

void SetMenuPause(bool on)
{
    if (on && !g_menu_paused && !g_doa3_frame_pause && !netplay_active()) {
        InterlockedExchange(&g_doa3_frame_steps, 0);
        InterlockedExchange(&g_doa3_frame_pause, 2);   /* 2: the menu's hold (also over a movie) */
        g_menu_paused = true;
    } else if (!on && g_menu_paused) {
        InterlockedExchange(&g_doa3_frame_pause, 0);
        g_menu_paused = false;
    }
}

/* With the menu's pause on, a held frame is shown as it was drawn, so a new
 * picture shape or resolution would only stretch it. Let the game run two
 * more frames (the second shows the first finished with the new setting,
 * as the render thread works one frame behind) and hold again. */
void MenuPauseRefresh()
{
    if (g_menu_paused && g_doa3_frame_pause == 2) InterlockedExchange(&g_doa3_frame_steps, 2);
}

void SaveScreen(MenuScreen scr)
{
    int ok = 1;
    switch (scr) {
    case SCR_DISPLAY: {
        char v[16];
        ok = video_settings_save();
        _snprintf_s(v, sizeof v, _TRUNCATE, "%d", g_menu_scale);
        ok = WritePrivateProfileStringA("General", "MenuScale", v, doa3_settings_ini_path()) && ok;
        break;
    }
    case SCR_SOUND:
        ok = audio_settings_save();
        break;
    case SCR_CONTROLS: case SCR_PLAYER: case SCR_DEVICE:
        if (!pad_mapping_save(NULL)) { SetStatus("Could not write doa3_input.ini"); return; }
        break;
    case SCR_GAME:
        ok = doa3_log_save();
        ok = update_check_save() && ok;
        ok = dip_save() && ok;
        ok = WritePrivateProfileStringA("General", "PauseInMenu", g_pause_in_menu ? "1" : "0",
                                        doa3_settings_ini_path()) && ok;
        break;
    default:
        return;
    }
    SetStatus(ok ? "Settings saved" : "Could not write doa3_settings.ini");
}

/* Leave the current screen: a list screen saves and returns to the main
 * list; the main list closes the menu. */
void MenuBack()
{
    if (g_screen == SCR_MAIN) { doa3_ui_toggle(); return; }
    /* The device list goes back to its player's screen (saving later),
     * a player's screen to Controls, and the other screens to the main
     * list; changes are saved on leaving a screen for its parent. */
    if (g_screen == SCR_DEVICE) { g_screen = SCR_PLAYER; return; }
    if (g_screen_dirty) SaveScreen(g_screen);
    g_screen_dirty = false;
    g_screen = g_screen == SCR_PLAYER ? SCR_CONTROLS : SCR_MAIN;
    if (g_screen == SCR_CONTROLS) g_pm_controls.selected = 3 + g_input_player;   /* its Player row */
    g_listening = -1;
    g_armed = false;
    g_screenshot_listening = false;
}

void OpenScreen(MenuScreen scr, pm::State &st)
{
    g_screen = scr;
    g_screen_dirty = false;
    st.Reset();
}

void OpenClassic(int tab)
{
    g_screen = SCR_CLASSIC;
    g_screen_dirty = false;
    g_tab_request = tab;
    g_menu_resize = true;    /* centred again at its size */
}

void DrawMainScreen()
{
    pm::Begin(g_pm_main, kMenuTitle, nullptr, true);
    if (g_doa3_folder_write_error) pm::Note(kFolderWarning, IM_COL32(255, 140, 115, 255));
    if (pm::Action("Display", "Window type, picture shape, internal resolution and the performance overlay."))
        OpenScreen(SCR_DISPLAY, g_pm_display);
    if (pm::Action("Sound", "Overall volume, the game's music, effects and voice levels, and streamer mode."))
        OpenScreen(SCR_SOUND, g_pm_sound);
    if (pm::Action("Game", "Pausing, update checks, dip switches and log files."))
        OpenScreen(SCR_GAME, g_pm_game);
    if (pm::Action("Controls", "Each player's controller, keys and buttons."))
        OpenScreen(SCR_CONTROLS, g_pm_controls);
    if (pm::Action("Photo Mode", "Free camera, hiding the fighters and the game's screens, and screenshots."))
        OpenScreen(SCR_PHOTO, g_pm_photo);
    if (pm::Action("Sparring", training_available()
                   ? "Dummy recording and playback, and move information, for Sparring."
                   : "Dummy recording and playback, and move information. Available in an offline Sparring fight.",
                   training_available() != 0))
        OpenScreen(SCR_SPARRING, g_pm_sparring);
    if (g_debug_tab && pm::Action("Debug", "Developer tools."))
        OpenClassic(6);
    if (pm::Action("Quit", "Close Dead or Alive 3.")) {
        OpenScreen(SCR_QUIT, g_pm_quit);
    }
    if (pm::BackPressed()) doa3_ui_toggle();
    pm::End(g_status, kMenuVersion);
}

void DrawDisplayScreen()
{
    static const char *const kWindow[] = { "Windowed", "Borderless fullscreen" };
    static const char *const kScale[]  = { "1x (480p)", "2x (960p)", "3x (1440p)", "4x (1920p)" };
    const char *kAspect[VIDEO_ASPECT_COUNT];
    for (int i = 0; i < VIDEO_ASPECT_COUNT; ++i) kAspect[i] = video_aspect_name(i);

    pm::Begin(g_pm_display, kMenuTitle, "Display");
    int wm = video_get_window_mode();
    if (pm::Choice("Window type", wm, kWindow, 2,
                   "A normal window, or borderless fullscreen covering the screen the window is on.")) {
        video_set_window_mode(wm);
        MenuPauseRefresh();
        g_screen_dirty = true;
    }
    int ar = video_get_aspect();
    if (pm::Choice("Aspect ratio", ar, kAspect, VIDEO_ASPECT_COUNT,
                   "The shape of the picture. 4:3 is the original; the wide shapes show more of each scene.")) {
        video_set_aspect(ar);
        MenuPauseRefresh();
        g_screen_dirty = true;
    }
    int sc = video_get_scale() - 1;
    if (pm::Choice("Internal resolution", sc, kScale, 4,
                   "How sharp the 3D is. 1x is the Xbox's own resolution; higher is sharper and needs a "
                   "faster graphics card.")) {
        video_set_scale(sc + 1);
        MenuPauseRefresh();
        g_screen_dirty = true;
    }
    bool perf = PerformanceOverlayEnabled();
    if (pm::Toggle("Performance overlay", perf,
                   "Frames per second and frame times in the corner (also L3 + Y on a controller). "
                   "For this session only."))
        TogglePerformanceOverlay();
    int ms = g_menu_scale;
    /* 1.0.4.26: only the Debug tab still uses the classic window */
    if (g_debug_tab && pm::Step("Classic menu size", ms, 100, 250, 25, "%d%%",
                 "The size of the classic window (the Debug tab) and its text.")) {
        g_menu_scale = ms;
        g_screen_dirty = true;
    }
    if (pm::Action("Reset to defaults", "Borderless fullscreen, 16:9, 3x.")) {
        video_set_window_mode(VIDEO_BORDERLESS);
        video_set_aspect(VIDEO_ASPECT_16_9);
        video_set_scale(3);
        MenuPauseRefresh();
        g_menu_scale = 100;
        ResetPerformanceOverlay();
        g_screen_dirty = true;
        SetStatus("Display settings reset to defaults");
    }
    if (pm::Action("Back", "Keeps your changes and goes back.") || pm::BackPressed()) MenuBack();
    pm::End(g_status, kMenuVersion);
}

void DrawSoundScreen()
{
    static const char *const kChannel[3] = { "Music", "Sound effects", "Voice" };
    pm::Begin(g_pm_sound, kMenuTitle, "Sound");
    int volume = audio_get_volume();
    if (pm::Step("Master volume", volume, 0, 100, 5, "%d%%",
                 "Overall level of the PC version, after the game's own mix (music, voices, effects and "
                 "movies). 100% leaves the game unchanged; the default is 80%.")) {
        audio_set_volume(volume);
        g_screen_dirty = true;
    }
    const bool ready = game_audio_available() != 0;
    for (int ch = 0; ch < 3; ++ch) {
        int v = game_audio_get(ch);
        if (pm::Step(kChannel[ch], v, 0, 100, 5, "%d%%",
                     ready ? "The same setting as the game's Options > Audio Setting. It is kept in the game's "
                             "save the next time the game saves (leaving Options, or after the credits)."
                           : "Available once the game has loaded its save.",
                     ready))
            game_audio_request(ch, v);
    }
    bool streamer = audio_streamer_mode_get() != 0;
    if (pm::Toggle("Streamer mode", streamer,
                   "Silences Aerosmith's licensed songs (\"Nine Lives\", \"Amazing\" and \"Home Tonight\") "
                   "wherever they play, including the opening, the credits and the sound test. Your saved "
                   "music volume is never changed.")) {
        audio_streamer_mode_set(streamer ? 1 : 0);
        g_screen_dirty = true;
    }
    if (game_audio_streamer_muting()) pm::Note("A licensed song is muted right now.");
    if (pm::Action("Reset to defaults", "Master volume 80%, streamer mode off. The game's own levels are not changed.")) {
        audio_set_volume(80);
        audio_streamer_mode_set(0);
        g_screen_dirty = true;
        SetStatus("Sound settings reset to defaults");
    }
    if (pm::Action("Back", "Keeps your changes and goes back.") || pm::BackPressed()) MenuBack();
    pm::End(g_status, kMenuVersion);
}

void DrawGameScreen()
{
    pm::Begin(g_pm_game, kMenuTitle, "Game");
    if (g_doa3_folder_write_error) pm::Note(kFolderWarning, IM_COL32(255, 140, 115, 255));
    if (pm::Toggle("Pause while this menu is open", g_pause_in_menu,
                   "Holds the game (and its sound) while this menu is open, movies included. Never "
                   "during online play.")) {
        SetMenuPause(g_pause_in_menu);
        g_screen_dirty = true;
    }

    pm::Heading("Updates");
    bool upd = update_check_enabled() != 0;
    if (pm::Toggle("Check for updates at boot", upd,
                   "At start-up, asks GitHub for the latest release of this port (one small request; "
                   "nothing is downloaded or installed).")) {
        update_check_set_enabled(upd ? 1 : 0);
        g_screen_dirty = true;
    }
    {
        int st = update_check_state();
        char latest[64];
        const char *value = st == 1 ? "Checking..." : st == 2 ? "Up to date" :
                            st == 4 ? "Could not reach GitHub" : "";
        if (st == 3) {
            _snprintf_s(latest, sizeof latest, _TRUNCATE, "%s available", update_check_latest());
            value = latest;
        }
        if (pm::Action("Check for updates now", "Asks GitHub for the latest release now.", st != 1, value))
            update_check_now();
        if (st == 3 && pm::Action("Open the release page", "Opens the new release's page in your browser."))
            ShellExecuteA(NULL, "open", update_check_url(), NULL, NULL, SW_SHOWNORMAL);
    }

    pm::Heading("Dip switches");
    for (int i = 0; i < DIP_COUNT; i++) {
        if (dip_hidden((dip_switch)i)) continue;
        bool d = dip_get((dip_switch)i) != 0;
        if (pm::Toggle(dip_label((dip_switch)i), d, dip_help((dip_switch)i))) {
            dip_set((dip_switch)i, d ? 1 : 0);
            g_screen_dirty = true;
        }
    }

    pm::Heading("Logging");
    bool on = doa3_log_enabled() != 0;
    if (pm::Toggle("Write log files", on,
                   "Writes doa3_log.txt next to the game, for bug reports. Takes effect at the next launch.")) {
        doa3_log_set_enabled(on ? 1 : 0);
        g_screen_dirty = true;
    }
    if (pm::Action("Create a trace file",
                   "Records the next frame's graphics commands (mtrace_manual_*.txt) and a screenshot "
                   "(aa_*.bmp) in the Diagnostics folder next to the game, for a visual bug on screen now. "
                   "The folder is emptied at the next launch, so send the files before restarting.")) {
        InterlockedExchange(&g_doa3_trace_request, 1);
        SetStatus("The next frame will be traced to the Diagnostics folder");
    }

    if (pm::Action("Reset to defaults", "Pausing off, update checks on, every dip switch off, log files off.")) {
        g_pause_in_menu = false;
        SetMenuPause(false);
        doa3_log_set_enabled(0);
        update_check_set_enabled(1);
        dip_reset();
        g_screen_dirty = true;
        SetStatus("Game settings reset to defaults");
    }
    if (pm::Action("Back", "Keeps your changes and goes back.") || pm::BackPressed()) MenuBack();
    pm::End(g_status, kMenuVersion);
}

void DrawQuitScreen()
{
    pm::Begin(g_pm_quit, kMenuTitle, "Quit");
    pm::Note("Close Dead or Alive 3? Progress is kept only as far as the game itself last saved.");
    if (pm::Action("No", "Back to the menu.") || pm::BackPressed()) MenuBack();
    if (pm::Action("Yes, quit", "Closes the game.")) {
        HWND hwnd = d3d8_GetHWND();
        if (hwnd) PostMessageA(hwnd, WM_CLOSE, 0, 0);
    }
    pm::End(g_status, kMenuVersion);
}


/* ── Controls (1.0.4) ─────────────────────────────────────────────────────
 * Controls: the two input policies, then one row per player that opens the
 * player's screen (its value is the device the player uses). The player's
 * screen: input device (opens a list, so looking through the devices never
 * takes one from another player), Xbox controller type, one row per
 * control, Reset to defaults and Back. Changes apply at once and are saved
 * to doa3_input.ini on leaving, like the other screens. */

/* The device a player uses, as a label: "Keyboard", "None", "PS5 controller 1",
 * or "Automatic (Xbox controller 1)". */
static void PlayerDeviceLabel(int player, char *out, size_t n)
{
    DWORD physical = xbox_InputPhysicalMask();
    int dev = pad_mapping_device(player), slot = xbox_InputAssignedSlot(player);
    char pad[64] = "";
    if (slot >= 0) ControllerLabel(slot, physical, pad, sizeof pad);
    if (dev == PAD_DEVICE_KEYBOARD) _snprintf_s(out, n, _TRUNCATE, "Keyboard");
    else if (dev == PAD_DEVICE_NONE) _snprintf_s(out, n, _TRUNCATE, "None");
    else if (dev == PAD_DEVICE_AUTO) {
        if (slot >= 0) _snprintf_s(out, n, _TRUNCATE, "Automatic (%s)", pad);
        else if (player == 0) _snprintf_s(out, n, _TRUNCATE, "Automatic (keyboard)");
        else _snprintf_s(out, n, _TRUNCATE, "Automatic (none connected)");
    } else {
        ControllerLabel(dev, physical, pad, sizeof pad);
        _snprintf_s(out, n, _TRUNCATE, "%s", pad);
    }
}

/* A control's name on the player screen (the .ini keys are terse). */
static const char *CtlName(int ctl)
{
    static const char *const names[][2] = {
        { "DPadUp", "Up" }, { "DPadDown", "Down" }, { "DPadLeft", "Left" }, { "DPadRight", "Right" },
        { "LThumb", "Left stick press" }, { "RThumb", "Right stick press" },
        { "LT", "Left trigger" }, { "RT", "Right trigger" },
        { "LX-", "Left stick left" }, { "LX+", "Left stick right" },
        { "LY-", "Left stick down" }, { "LY+", "Left stick up" },
        { "RX-", "Right stick left" }, { "RX+", "Right stick right" },
        { "RY-", "Right stick down" }, { "RY+", "Right stick up" },
    };
    const char *key = pad_ctl_key(ctl);
    for (const auto &n : names) if (strcmp(n[0], key) == 0) return n[1];
    return key;
}

void OpenPlayerScreen(int player)
{
    g_input_player = player;
    g_listening = -1; g_armed = false; g_pending_vk = 0;
    g_bind_release.kind = PADSRC_NONE;
    g_screen = SCR_PLAYER;
    g_pm_player.Reset();
}

void DrawControlsScreen()
{
    /* the screenshot key is captured by the window procedure; meanwhile
     * the list ignores the controller and mouse */
    pm::SetBlocked(g_screenshot_listening);
    pm::Begin(g_pm_controls, kMenuTitle, "Controls");
    bool focus_only = pad_mapping_focus_only() != 0;
    if (pm::Toggle("Input only when focused", focus_only,
                   "On: the game ignores the keyboard and controllers while another app is in front. "
                   "Off: it keeps reading them. For all players.")) {
        pad_mapping_set_focus_only(focus_only);
        g_screen_dirty = true;
    }
    bool capture_menu = pad_mapping_capture_menu() != 0;
    if (pm::Toggle("Capture gameplay input in menu", capture_menu,
                   "On: while this menu is open, controllers operate it and the game doesn't see them. "
                   "Off: controllers keep playing the game and don't operate the menu (use the mouse or "
                   "keyboard). For all players.")) {
        pad_mapping_set_capture_menu(capture_menu);
        g_screen_dirty = true;
    }
    {   /* 1.0.4: moved here from Photo Mode */
        PadBinding key = { PADSRC_KEY, debug_screenshot_key_get() };
        char name[80];
        if (g_screenshot_listening) _snprintf_s(name, sizeof name, _TRUNCATE, "Press a key");
        else pad_binding_label(key, name, sizeof name);
        bool clear = false;
        if (pm::Binding("Screenshot key", name, g_screenshot_listening,
                        "Saves a picture of the game (without this menu) to the Screenshots folder. Enter / A, "
                        "then press the key; Esc cancels. Delete / X puts it back to F12. On a controller, "
                        "L3 + RB takes one too.", clear)) {
            g_screenshot_listening = true;
            g_listening = -1;
            SetStatus("Press a key for screenshots (Esc cancels)");
        }
        if (clear) {
            debug_screenshot_key_set(VK_F12);
            SetStatus(debug_screenshot_key_save() ? "Screenshot key saved" : "Key changed; could not save settings");
        }
    }
    pm::Heading("Players");
    for (int p = 0; p < 4; ++p) {
        char label[32], value[96];
        _snprintf_s(label, sizeof label, _TRUNCATE, "Player %d", p + 1);
        PlayerDeviceLabel(p, value, sizeof value);
        if (pm::Action(label, "The device this player uses, its controller type, and its keys and buttons.",
                       true, value))
            OpenPlayerScreen(p);
    }
    if (pm::Action("Back", "Keeps your changes and goes back.") || pm::BackPressed()) MenuBack();
    pm::End(g_status, kMenuVersion);
}

void DrawPlayerScreen()
{
    static const char *const kTypes[] = { "Duke", "Controller S", "Fight Stick" };
    PadMapping *m = pad_mapping_get_player(g_input_player);
    char section[48];
    _snprintf_s(section, sizeof section, _TRUNCATE, "Controls > Player %d", g_input_player + 1);

    /* Capturing: arm once everything that started it is let go (the A or
     * Enter that selected the row would otherwise bind itself), then take
     * the first key or control. Afterwards, wait for it to be let go. */
    if (g_listening >= 0) {
        PadBinding held;
        if (!g_armed) {
            if (!CapturePad(held) && !(GetAsyncKeyState(VK_RETURN) & 0x8000) &&
                !(GetAsyncKeyState(VK_SPACE) & 0x8000) && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
                g_armed = true;
                g_pending_vk = 0;
            }
        } else if (CaptureBinding(held)) {
            m->b[g_listening] = held;
            char lbl[96], msg[160];
            pad_binding_label(held, lbl, sizeof lbl);
            _snprintf_s(msg, sizeof msg, _TRUNCATE, "%s bound to %s", CtlName(g_listening), lbl);
            SetStatus(msg);
            g_listening = -1;
            g_armed = false;
            g_bind_release = held;
            g_screen_dirty = true;
        }
    }
    if (g_bind_release.kind != PADSRC_NONE) {
        PadBinding held;
        bool down = g_bind_release.kind == PADSRC_KEY ? (GetAsyncKeyState(g_bind_release.code) & 0x8000) != 0
                                                      : CapturePad(held);
        if (!down) g_bind_release.kind = PADSRC_NONE;
    }
    pm::SetBlocked(g_listening >= 0 || g_bind_release.kind != PADSRC_NONE);

    pm::Begin(g_pm_player, kMenuTitle, section);
    char device[96];
    PlayerDeviceLabel(g_input_player, device, sizeof device);
    if (pm::Action("Input device", "Automatic uses a free controller (and, for Player 1, the keyboard). Opens "
                   "the list of devices; choosing one another player has moves it to this player.",
                   true, device)) {
        g_screen = SCR_DEVICE;
        g_pm_device.Reset();
    }
    int type = pad_mapping_type(g_input_player);
    if (pm::Choice("Xbox controller type", type, kTypes, PAD_TYPE_COUNT,
                   "The kind of Xbox controller the game sees plugged in for this player.")) {
        pad_mapping_set_type(g_input_player, type);
        g_screen_dirty = true;
    }
    const char *group = nullptr;
    for (int row = 0; row < PADCTL_COUNT; ++row) {
        const int i = pad_ctl_display_order(row);
        const char *g = pad_ctl_group(i);
        if (!group || strcmp(g, group) != 0) { group = g; pm::Heading(g); }
        char value[96];
        const bool listening = g_listening == i;
        if (listening) _snprintf_s(value, sizeof value, _TRUNCATE, "%s", g_armed ? "Press an input" : "...");
        else pad_binding_label(m->b[i], value, sizeof value);
        bool clear = false;
        if (pm::Binding(CtlName(i), value, listening,
                        "Enter / A, then press the key or control to use. Delete / X clears it; Esc cancels.",
                        clear)) {
            g_listening = i;
            g_armed = false;
            g_pending_vk = 0;
            char msg[96];
            _snprintf_s(msg, sizeof msg, _TRUNCATE, "Press an input for %s (Esc cancels)", CtlName(i));
            SetStatus(msg);
        }
        if (clear) {
            m->b[i].kind = PADSRC_NONE; m->b[i].code = 0;
            g_screen_dirty = true;
        }
    }
    pm::Separator();
    if (pm::Action("Reset to defaults", "This player's keys and buttons go back to the defaults. The input "
                   "device and controller type stay as they are.")) {
        pad_mapping_reset_player(g_input_player);
        g_listening = -1;
        g_screen_dirty = true;
        SetStatus("This player's bindings reset to defaults");
    }
    if (pm::Action("Save", "Saves this player's controls and goes back to Controls.")) {
        g_screen_dirty = true;   /* write the file even when nothing changed */
        MenuBack();
    } else if (pm::BackPressed()) MenuBack();
    pm::Hint("Delete / X", "clear");
    pm::End(g_status, kMenuVersion);
}

void DrawDeviceScreen()
{
    DWORD physical = xbox_InputPhysicalMask();
    char section[48];
    _snprintf_s(section, sizeof section, _TRUNCATE, "Controls > Player %d > Input device", g_input_player + 1);
    pm::Begin(g_pm_device, kMenuTitle, section);
    const int cur = pad_mapping_device(g_input_player);
    auto pick = [&](int dev, const char *label, const char *help) {
        /* the value: this player's current choice, or who else has it */
        char value[48] = "";
        if (dev == cur) _snprintf_s(value, sizeof value, _TRUNCATE, "Current");
        else if (dev >= 0)
            for (int p = 0; p < 4; ++p) {
                if (p == g_input_player) continue;
                if (pad_mapping_device(p) == dev) {
                    _snprintf_s(value, sizeof value, _TRUNCATE, "Player %d", p + 1);
                    break;
                }
                if (pad_mapping_device(p) == PAD_DEVICE_AUTO && xbox_InputAssignedSlot(p) == dev) {
                    _snprintf_s(value, sizeof value, _TRUNCATE, "Player %d (automatic)", p + 1);
                    break;
                }
            }
        if (!pm::Action(label, help, true, value[0] ? value : nullptr)) return;
        if (dev != cur) {
            pad_mapping_set_device(g_input_player, dev);
            g_screen_dirty = true;
            SetStatus("Input device changed");
        }
        g_screen = SCR_PLAYER;
    };
    pick(PAD_DEVICE_AUTO, "Automatic", "A controller no other player has chosen (and, for Player 1, the keyboard).");
    pick(PAD_DEVICE_NONE, "None", "This player has no input.");
    pick(PAD_DEVICE_KEYBOARD, "Keyboard", "The keyboard keys bound on this player's screen.");
    int shown = 0;
    for (int i = 0; i < XBOX_HOST_CONTROLLERS; ++i) {
        /* connected controllers, and this player's choice even when unplugged */
        if (!(physical & (1u << i)) && i != cur) continue;
        char label[64];
        ControllerLabel(i, physical, label, sizeof label);
        pick(i, label, "Choosing a controller another player chose moves it to this player, and that "
                       "player's device becomes None.");
        ++shown;
    }
    if (!shown) pm::Note("No controllers connected.");
    if (pm::Action("Back", "Keeps the current device and goes back.") || pm::BackPressed()) MenuBack();
    pm::End(g_status, kMenuVersion);
}

/* ── Photo Mode (1.0.4) ─────────────────────────────────────────────────
 * Everything here lasts for the session only (as on the classic tab), so
 * the screen saves nothing. */
void DrawPhotoScreen()
{
    pm::Begin(g_pm_photo, kMenuTitle, "Photo Mode");
    const bool online = netplay_active() != 0;
    pm::Heading("Free camera");
    bool camera = free_camera_enabled() != 0;
    if (pm::Toggle("Free camera", camera,
                   "Close this menu to fly. Keyboard: WASD move, the mouse looks, E / Q up and down, Shift "
                   "fast, Ctrl slow. Controller: left stick moves, right stick looks, RT / LT up and down, "
                   "LB fast, RB slow. Fighter controls are blocked while flying; the fight keeps running. "
                   "Not available online.", !online)) {
        if (!free_camera_set_enabled(camera ? 1 : 0))
            SetStatus("Enter an offline 3D scene before turning on the camera");
    }
    int speed = (int)(free_camera_speed() + 0.5f);
    if (pm::Step("Camera speed", speed, 1, 20, 1, "%d", "How fast the free camera moves. The default is 10.",
                 !online))
        free_camera_set_speed((float)speed);
    if (pm::Action("Reset to game camera", "Puts the free camera back where the game's camera is.", !online))
        free_camera_reset();
    pm::Heading("Hide");
    for (int side = 0; side < 2; ++side) {
        bool hide = debug_hide_player_get(side) != 0;
        if (pm::Toggle(side ? "Hide Player 2" : "Hide Player 1", hide,
                       "Hides the fighter. Gameplay and collision stay as they are. Not available online.",
                       !online))
            debug_hide_player_set(side, hide);
    }
    bool hidden = debug_hide_ui_get() != 0;
    if (pm::Toggle("Hide UI", hidden,
                   "Hides the game's screen overlays, the HUD and menus included (some fades may go too). "
                   "Escape always opens this menu, so you can bring them back."))
        debug_hide_ui_set(hidden);
    pm::Heading("Screenshots");
    {
        PadBinding key = { PADSRC_KEY, debug_screenshot_key_get() };
        char name[80];
        pad_binding_label(key, name, sizeof name);
        if (pm::Action("Take screenshot", "Saves the game at its render resolution, without this menu, to the "
                       "Screenshots folder. The screenshot key (set in Controls) does the same.", true, name))
            d3d8_screenshot_request();
        const char *shot = d3d8_screenshot_status();
        if (shot && shot[0]) pm::Note(shot);
    }
    pm::Separator();
    if (pm::Action("Reset to defaults", "Free camera off at speed 10, nothing hidden.")) {
        free_camera_set_enabled(0);
        free_camera_set_speed(10);
        free_camera_reset_fov();
        debug_hide_ui_set(0);
        debug_hide_player_set(0, 0);
        debug_hide_player_set(1, 0);
        SetStatus("Photo Mode reset to defaults");
    }
    if (pm::Action("Back", "Goes back. These settings last until the game is closed.") || pm::BackPressed())
        MenuBack();
    pm::End(g_status, kMenuVersion);
}

/* ── Sparring (1.0.4) ───────────────────────────────────────────────────
 * The classic tab's rows, one per line; everything lasts for the session. */
void DrawSparringScreen()
{
    static int slot = 0;
    static bool loop = false;
    pm::Begin(g_pm_sparring, kMenuTitle, "Sparring");
    if (!training_available()) {
        pm::Note("Available only in an offline Sparring fight.");
        if (pm::Action("Back", "Goes back.") || pm::BackPressed()) MenuBack();
        pm::End(g_status, kMenuVersion);
        return;
    }
    const bool online = netplay_active() != 0;
    {
        unsigned flags = combat_debug_flags();
        bool frames = (flags & COMBAT_FRAMES) != 0;
        if (pm::Toggle("Show Move Info", frames,
                       "Each attack's base startup (first active frame), active duration and recovery, beside "
                       "the observed strike frames: blue startup, red active, yellow recovery, gray missing "
                       "samples. Cancels shorten the observation, not the base timing. Hit/block advantage is "
                       "not measured yet.", !online))
            combat_debug_set_flags(frames ? (flags | COMBAT_FRAMES) : (flags & ~COMBAT_FRAMES));
    }

    pm::Heading("Dummy recording");
    TrainingStatus status;
    training_status(&status);
    char slots[3][32];
    const char *slot_names[3];
    for (int i = 0; i < 3; ++i) {
        _snprintf_s(slots[i], sizeof slots[i], _TRUNCATE, "%d   (%.1f / %.0f s)", i + 1,
                    status.length[i] / 60.0f, TRAINING_MAX_FRAMES / 60.0f);
        slot_names[i] = slots[i];
    }
    pm::Choice("Slot", slot, slot_names, 3,
               "Each slot holds up to 10 seconds, for this Sparring session. Recording again replaces it.");
    pm::Toggle("Loop playback", loop, "Play the slot over and over until Escape.");
    const bool ready = training_can_record() != 0;
    const char *why = "Enter a solo Sparring fight, close the game's pause menu, and turn off Exercise and the "
                      "free camera to record or play.";
    if (pm::Action("Record dummy", ready ? "You control the dummy until Escape; its inputs go into the slot. "
                   "Directions replay relative to the dummy's facing; positions are not reset." : why, ready) &&
        training_start(TRAINING_RECORD, slot, 0))
        doa3_ui_toggle();
    if (pm::Action("Play", ready ? "The dummy plays the slot back (Escape stops it)." : why,
                   ready && status.length[slot] != 0) &&
        training_start(TRAINING_PLAY, slot, loop))
        doa3_ui_toggle();
    if (pm::Action("Clear slot", "Empties the slot.", status.length[slot] != 0)) training_clear(slot);
    if (!ready) pm::Note("Not available right now.");

    {   /* Second controller drives the dummy (0.1.12) */
        int enabled = 0, port = 1;
        training_control_get(&enabled, &port);
        int human = training_human_port();
        bool tag = training_control_partner() != 0;
        const char *who = tag ? "Partner" : "Dummy";
        const char *cpu = tag ? "your controller" : "CPU";
        pm::Heading("Second controller");
        bool on = enabled != 0;
        if (pm::Toggle(tag ? "Controller controls your tag partner" : "Controller controls the dummy", on,
                       tag ? "Tag Sparring: the chosen controller plays your tag partner while it is on point; "
                             "you play the other member. It goes back to your controller when that controller "
                             "is unplugged."
                           : "The chosen controller plays the dummy. It goes back to the CPU when that "
                             "controller is unplugged, and while a recording plays or records."))
            training_control_set(on ? 1 : 0, port);
        char ports[4][48];
        const char *port_names[4];
        for (int p = 0; p < 4; ++p) {
            _snprintf_s(ports[p], sizeof ports[p], _TRUNCATE, "Player %d%s", p + 1,
                        p == human ? " (yours)" : training_port_connected(p) ? "" : " (not connected)");
            port_names[p] = ports[p];
        }
        int pick = port;
        if (pm::Choice("Controller", pick, port_names, 4,
                       "Which player's controller (as set in Controls). Your own is skipped.")) {
            if (pick == human) pick = (pick + (pick == (port + 1) % 4 ? 1 : 3)) % 4;
            training_control_set(enabled, pick);
        }
        char line[96];
        if (!enabled) _snprintf_s(line, sizeof line, _TRUNCATE, "%s: %s", who, cpu);
        else if (training_control_active()) _snprintf_s(line, sizeof line, _TRUNCATE, "%s: Player %d controller", who, port + 1);
        else if (port == human) _snprintf_s(line, sizeof line, _TRUNCATE, "%s: %s (that is your own controller)", who, cpu);
        else if (!training_port_connected(port)) _snprintf_s(line, sizeof line, _TRUNCATE, "%s: %s (no controller there)", who, cpu);
        else if (tag) _snprintf_s(line, sizeof line, _TRUNCATE, "Partner: your controller until the fight is running");
        else _snprintf_s(line, sizeof line, _TRUNCATE, "Dummy: CPU until the fight is running and this menu is closed");
        pm::Note(line);
    }

    pm::Heading("Stagger Escape");
    {
        static const char *kStaggerHelp =
            "Holds how quickly the fighter recovers from a critical stun. Each input in a critical stun or "
            "guard break adds 3% (up to 100%) and every idle frame takes 1%; 0% plays the stun at normal "
            "speed, 100% at double. Hold sets the value at the start of every frame, and that frame's input "
            "applies on top.";
        int human = HumanSide();
        char now[96];
        _snprintf_s(now, sizeof now, _TRUNCATE, "Now: Fighter 1 %.0f%%, Fighter 2 %.0f%%",
                    (training_stagger_get(0) - 1.0f) * 100.0f, (training_stagger_get(1) - 1.0f) * 100.0f);
        for (int side = 0; side < 2; ++side) {
            int hold = 0; float value = 1.0f;
            training_stagger_hold_get(side, &hold, &value);
            const char *tagname = side == human ? " (you)" : human >= 0 ? " (dummy)" : "";
            char label[48];
            _snprintf_s(label, sizeof label, _TRUNCATE, "Hold Fighter %d%s", side + 1, tagname);
            bool h = hold != 0;
            if (pm::Toggle(label, h, kStaggerHelp)) training_stagger_hold(side, h ? 1 : 0, value);
            _snprintf_s(label, sizeof label, _TRUNCATE, "Fighter %d%s value", side + 1, tagname);
            int pct = (int)((value - 1.0f) * 100.0f + 0.5f);
            if (pm::Step(label, pct, 0, 100, 5, "%d%%", kStaggerHelp))
                training_stagger_hold(side, hold, 1.0f + pct / 100.0f);
        }
        pm::Note(now);
    }
    pm::Separator();
    if (pm::Action("Back", "Goes back. These settings last for this Sparring session.") || pm::BackPressed())
        MenuBack();
    pm::End(g_status, kMenuVersion);
}

void DrawPauseMenu()
{
    pm::SetBlocked(false);   /* only Controls and the player screen block, while capturing */
    switch (g_screen) {
    case SCR_MAIN:    DrawMainScreen(); break;
    case SCR_DISPLAY: DrawDisplayScreen(); break;
    case SCR_SOUND:   DrawSoundScreen(); break;
    case SCR_GAME:    DrawGameScreen(); break;
    case SCR_QUIT:    DrawQuitScreen(); break;
    case SCR_CONTROLS: DrawControlsScreen(); break;
    case SCR_PLAYER:  DrawPlayerScreen(); break;
    case SCR_DEVICE:  DrawDeviceScreen(); break;
    case SCR_PHOTO:   DrawPhotoScreen(); break;
    case SCR_SPARRING: DrawSparringScreen(); break;
    case SCR_CLASSIC: DrawMenu(); break;
    }
}

} /* namespace */

/* ── C API ─────────────────────────────────────────────────────────────── */

static void LoadMenuSettings(void);
extern "C" int doa3_ui_init(void)
{
    if (g_ready) return 1;

    HWND hwnd = d3d8_GetHWND();
    ID3D11Device *dev = d3d8_GetD3D11Device();
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    if (!hwnd || !dev || !ctx) return 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    /* No imgui.ini: the overlay is one centred window, and writing a settings
     * file into the game's working directory is just clutter. */
    io.IniFilename = nullptr;
    /* Navigation and binding capture use the game's unified controller path. */
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    g_base_style = ImGui::GetStyle();
    {   char v[8];
        g_debug_tab = GetEnvironmentVariableA("DOA3_DEBUG", v, sizeof v) > 0 && v[0] == '1'; }
    LoadMenuSettings();

    if (!ImGui_ImplWin32_Init(hwnd)) { ImGui::DestroyContext(); return 0; }
    if (!ImGui_ImplDX11_Init(dev, ctx)) {
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        return 0;
    }

    g_ready = true;
    return 1;
}

extern "C" void doa3_ui_shutdown(void)
{
    combat_world_shutdown();
    if (!g_ready) return;
    g_ready = false;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

extern "C" int doa3_ui_visible(void) { return g_visible ? 1 : 0; }

/* [General] PauseInMenu and MenuScale, read once. The first Esc press
 * toggles the menu before doa3_ui_init has run (that waits for the first
 * frame with something to draw), so the toggle reads them too: until 1.0.4.12
 * the first opening never paused. */
static void LoadMenuSettings(void)
{
    static bool s_loaded;
    if (s_loaded) return;
    s_loaded = true;
    g_pause_in_menu = GetPrivateProfileIntA("General", "PauseInMenu", 0, doa3_settings_ini_path()) != 0;
    g_menu_scale = (int)GetPrivateProfileIntA("General", "MenuScale", 100, doa3_settings_ini_path());
    if (g_menu_scale < 100 || g_menu_scale > 250) g_menu_scale = 100;
}

extern "C" void doa3_ui_toggle(void)
{
    LoadMenuSettings();
    if (g_visible && g_screen_dirty) SaveScreen(g_screen);   /* closed from a list screen */
    g_screen_dirty = false;
    g_visible = !g_visible;
    /* 1.0.4: always opens on the main list, at its first row. */
    g_screen = SCR_MAIN;
    g_pm_main.Reset();
    SetMenuPause(g_visible && g_pause_in_menu);
    if (g_visible) g_status[0] = 0;   /* no message left over from last time */
    if (g_visible) training_stop();
    g_listening = -1;
    g_armed = false;
    g_pending_vk = 0;
    g_screenshot_listening = false;
    /* The input mapping applies the configured menu capture policy. */
    pad_mapping_set_menu_open(g_visible ? 1 : 0);
    if (g_visible) free_camera_release_mouse();
    else { g_cursor_shown = true; g_cursor_last_move = GetTickCount64(); }   /* hides after 1 s idle */
    /* Show or hide the cursor now, without waiting for the mouse to move
     * (the window's thread answers WM_SETCURSOR; this may be the pad's). */
    {
        HWND hwnd = d3d8_GetHWND();
        POINT pt;
        RECT rc;
        if (hwnd && GetCursorPos(&pt) && GetWindowRect(hwnd, &rc) && PtInRect(&rc, pt) &&
            WindowFromPoint(pt) == hwnd)
            PostMessageA(hwnd, WM_SETCURSOR, (WPARAM)hwnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
    }
}

extern "C" int doa3_ui_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg == WM_KILLFOCUS || (msg == WM_ACTIVATEAPP && !wp)) {
        g_screenshot_listening = false;
        g_listening = -1;
        g_armed = false;
        g_pending_vk = 0;
    }
    /* Escape: cancel a listening row first, otherwise toggle the overlay. It is
     * handled here rather than as a bindable key so the menu can always be
     * closed, whatever the mapping says. */
    if (msg == WM_KEYDOWN && wp == VK_ESCAPE && !(lp & (1 << 30))) {
        if (g_visible && g_screenshot_listening) {
            g_screenshot_listening = false;
            SetStatus("Screenshot binding cancelled");
        } else if (g_visible && g_listening >= 0) {
            g_listening = -1;
            g_armed = false;
            SetStatus("Rebind cancelled");
        } else if (g_visible && g_screen != SCR_MAIN) {
            MenuBack();   /* 1.0.4: Esc goes back a screen */
        } else {
            doa3_ui_toggle();
        }
        return 1;
    }

    /* 1.0.2: Frame advance hotkeys, only while the Debug tab is enabled.
     * F8 repeats when held, stepping continuously. */
    if (g_debug_tab && msg == WM_KEYDOWN && !(g_visible && (g_listening >= 0 || g_screenshot_listening))) {
        if (wp == VK_F7 && !(lp & (1 << 30))) { FramePauseToggle(); return 1; }
        if (wp == VK_F8) { FrameStep(); return 1; }
    }

    if (msg == WM_KEYDOWN && wp == 'D' && g_visible && !(lp & (1 << 30)) &&
        (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
        g_debug_tab = !g_debug_tab;
        SetStatus(g_debug_tab ? "Debug tab shown for this session" : "Debug tab hidden");
        return 1;
    }
    if ((msg == WM_KEYDOWN && wp != VK_SNAPSHOT && !(lp & (1 << 30))) ||
        (msg == WM_KEYUP && wp == VK_SNAPSHOT)) {
        if (g_visible && g_screenshot_listening) {
            if (debug_screenshot_key_set((int)wp)) {
                g_screenshot_listening = false;
                SetStatus(debug_screenshot_key_save() ? "Screenshot key saved" : "Key changed; could not save settings");
            } else SetStatus("Choose a single key other than Escape or a modifier");
            return 1;
        }
        if (!g_visible && (int)wp == debug_screenshot_key_get() &&
            !(GetAsyncKeyState(VK_CONTROL) & 0x8000) && !(GetAsyncKeyState(VK_MENU) & 0x8000) &&
            !(GetAsyncKeyState(VK_LWIN) & 0x8000) && !(GetAsyncKeyState(VK_RWIN) & 0x8000)) {
            d3d8_screenshot_request();
            return 1;
        }
    }

    /* 1.0.2: the game has no use for the mouse, so while this menu is closed
     * the cursor over the game hides after a second without movement and
     * comes back after a quarter second of moving it (a bumped mouse does not
     * bring it back). The menu and the first-run setup always show it. */
    if (msg == WM_MOUSEMOVE && !g_visible) {
        ULONGLONG now = GetTickCount64();
        if (lp != g_cursor_last_pos) {             /* Windows repeats moves on the spot */
            g_cursor_last_pos = lp;
            if (now - g_cursor_last_move > 200) g_cursor_streak = now;   /* a new movement */
            g_cursor_last_move = now;
            if (!g_cursor_shown && now - g_cursor_streak >= 250) {
                g_cursor_shown = true;
                SetCursor(LoadCursor(NULL, IDC_ARROW));
            }
        }
    }
    if (msg == WM_SETCURSOR && !g_visible && LOWORD(lp) == HTCLIENT) {
        SetCursor(g_cursor_shown ? LoadCursor(NULL, IDC_ARROW) : NULL);
        return 1;
    }

    if (!g_ready || !g_visible) return 0;

    /* Grab the raw VK for a listening row before ImGui swallows it. */
    if (msg == WM_KEYDOWN && g_listening >= 0 && g_armed && wp != VK_ESCAPE)
        g_pending_vk = (int)wp;

    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wp, lp);

    /* Keep mouse/keyboard out of the game while the overlay has them. */
    ImGuiIO &io = ImGui::GetIO();
    switch (msg) {
    case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP: case WM_CHAR:
        return io.WantCaptureKeyboard ? 1 : 0;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDBLCLK:
    case WM_MOUSEWHEEL:  case WM_MOUSEHWHEEL:
        return io.WantCaptureMouse ? 1 : 0;
    default:
        return 0;
    }
}

extern "C" void doa3_ui_render(void)
{
    PollControllerHotkeys();   /* 1.0.2 */
    /* 1.0.2: hide the cursor after a second without mouse movement. */
    if (g_cursor_shown && !g_visible && GetTickCount64() - g_cursor_last_move > 1000) {
        g_cursor_shown = false;
        HWND hwnd = d3d8_GetHWND();
        POINT pt;
        if (hwnd && GetCursorPos(&pt) && WindowFromPoint(pt) == hwnd)
            PostMessageA(hwnd, WM_SETCURSOR, (WPARAM)hwnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
    }
    TickPerformanceOverlay();
    free_camera_tick(d3d8_GetHWND(), g_visible ? 1 : 0);
    TrainingStatus training;
    training_status(&training);
    if (!training_available() && training.mode!=TRAINING_IDLE) {
        training_stop();
        training_status(&training);
    }
    /* Nothing to do on a normal frame unless the menu is open or online play
     * has something to say over the game. */
    if (!g_visible && !NetplayBannerPending() && !combat_debug_flags() && !PerformanceOverlayEnabled() && training.mode==TRAINING_IDLE &&
        !UpdateBannerPending() && !ScreenshotToastPending() && !FolderBannerPending() &&
        !g_doa3_frame_paused) return;
    if (!g_ready && !doa3_ui_init()) return;

    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    /* Drawn after the guest frame has been scaled into the window, so the
     * overlay is at window resolution and lines up with the mouse. */
    ID3D11RenderTargetView *rtv = d3d8_GetPresentRTV();
    if (!ctx || !rtv) return;

    /* The overlay replaces the output-merger targets and the viewport below.
     * Save the viewport: the guest only re-sets its own when it calls
     * SetViewport, not once a frame. */
    D3D11_VIEWPORT prev_vp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    UINT prev_nvp = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    ctx->RSGetViewports(&prev_nvp, prev_vp);
    /* ...and the output-merger targets (0.0.177). This used to end with
     * d3d8_RestoreDefaultTarget(), which marks the guest frame as bound; when
     * the frame was presented with an offscreen target bound (Zack costume
     * 3's environment-map pass), the presenter then put that offscreen
     * target back while the layer believed the guest frame was bound, so
     * the next frame's switch to the frame buffer was skipped and every
     * draw went into the 256x256 target: the black Zack cutscenes, with the
     * performance overlay or the menu up. Changing the internal resolution
     * rebinds the guest frame, which is why that cleared it. */
    ID3D11RenderTargetView *prev_rtv = nullptr;
    ID3D11DepthStencilView *prev_dsv = nullptr;
    ctx->OMGetRenderTargets(1, &prev_rtv, &prev_dsv);

    if (!ImGui::IsAnyItemActive()) ApplyMenuScale();   /* not while the slider is held */
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    UpdateGamepadNavigation();
    ImGui::NewFrame();

    DrawCombatDebugOverlay();
    DrawPerformanceOverlay();
    DrawTrainingBanner();
    DrawUpdateBanner();
    DrawScreenshotToast();
    DrawFolderBanner();
    DrawPausedBanner();
    if (g_visible) DrawPauseMenu();
    DrawNetplayBanner();

    ImGui::Render();

    /* The pgraph translator and the movie presenter each bind their own target
     * and viewport; bind the backbuffer explicitly rather than inheriting
     * whatever the last draw left set. */
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f; vp.TopLeftY = 0.0f;
    vp.Width  = (FLOAT)d3d8_GetPresentWidth();
    vp.Height = (FLOAT)d3d8_GetPresentHeight();
    vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
    ctx->RSSetViewports(1, &vp);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    /* Hand the output merger back as it was, depth buffer included. The bind
     * above passes a null depth-stencil view; leaving that bound means every
     * guest draw afterwards runs with no depth test and no depth writes. */
    ctx->OMSetRenderTargets(1, &prev_rtv, prev_dsv);
    if (prev_rtv) prev_rtv->Release();
    if (prev_dsv) prev_dsv->Release();
    if (prev_nvp) ctx->RSSetViewports(prev_nvp, prev_vp);
}
