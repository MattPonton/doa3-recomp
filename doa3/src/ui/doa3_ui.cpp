#include "doa3_ui.h"
#include "combat_overlay.h"
#include "combat_world.h"
#include "performance_overlay.h"
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
#include "../game/video_settings.h"
#include "../game/audio_settings.h"
#include "../game/game_audio.h"
#include "../game/debug_settings.h"
#include "../game/debug_capture.h"
#include "../game/free_camera.h"
#include "../d3d/d3d8_debug_views.h"
#include "../d3d/d3d8_screenshot.h"
#include "../game/log_settings.h"
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

/* Rebind capture state, mirroring MapperPanel's listening_/armed_ pair. */
int  g_listening = -1;   /* row capturing a binding, or -1 */
bool g_armed     = false; /* skip the frame the click started on */
int  g_pending_vk = 0;    /* VK grabbed by the wndproc while listening */
int  g_input_player = 0;

char g_status[128] = { 0 };

void SetStatus(const char *s) { _snprintf_s(g_status, sizeof(g_status), _TRUNCATE, "%s", s); }

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
        _snprintf_s(device_labels[i + 3], sizeof device_labels[i + 3], _TRUNCATE,
            "%s %d (%s)", i < 4 ? "Xbox controller" : "PS4 controller", i % 4 + 1,
            physical & (1u << i) ? "connected" : "disconnected");
    const int devices[] = { PAD_DEVICE_AUTO, PAD_DEVICE_NONE, PAD_DEVICE_KEYBOARD, 0, 1, 2, 3, 4, 5, 6, 7 };
    int selected = 0;
    for (int i = 0; i < 3 + XBOX_HOST_CONTROLLERS; ++i) if (devices[i] == pad_mapping_device(g_input_player)) selected = i;
    ImGui::SetNextItemWidth(270.0f);
    if (ImGui::BeginCombo("Input device", device_labels[selected])) {
        for (int i = 0; i < 3 + XBOX_HOST_CONTROLLERS; ++i) if (ImGui::Selectable(device_labels[i], i == selected)) {
            pad_mapping_set_device(g_input_player, devices[i]);
            g_listening = -1; g_pending_vk = 0;
            SetStatus("Device changed; Save to keep settings");
        }
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
    if (slot >= 0) ImGui::TextDisabled("Using %s %d", slot < 4 ? "Xbox controller" : "PS4 controller", slot % 4 + 1);
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
    for (int i = 0; i < PADCTL_COUNT; ++i) {
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
    static const char *kScale[]  = { "1x (480p)", "2x (960p)", "3x (1440p)" };

    switch (ResetSaveRow()) {
    case 1:
        video_set_window_mode(VIDEO_BORDERLESS);
        video_set_aspect(VIDEO_ASPECT_16_9);
        video_set_scale(3);
        SetStatus("Video settings reset to defaults");
        break;
    case 2:
        SetStatus(video_settings_save() ? "Saved doa3_settings.ini"
                                        : "Could not write doa3_settings.ini");
        break;
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
}

/* Audio output settings. */

void DrawAudioSection()
{
    switch (ResetSaveRow()) {
    case 1:
        audio_set_volume(100);
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
                       "(music, voices, sound effects and movies). 100%% leaves the game unchanged. "
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
        SetStatus("General settings reset to defaults");
        break;
    case 2:
        SetStatus(doa3_log_save() ? "Saved doa3_settings.ini"
                                  : "Could not write doa3_settings.ini");
        break;
    }

    ImGui::Separator();

    bool on = doa3_log_enabled() != 0;
    if (ImGui::Checkbox("Write log files", &on))
        doa3_log_set_enabled(on ? 1 : 0);
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
        debug_unlock_all_set(0);
        doa3_auto_capture_set(0);
        combat_debug_set_flags(0);
        ResetPerformanceOverlay();
        free_camera_set_enabled(0);
        free_camera_set_speed(10);
        free_camera_reset_fov();
        d3d8_debug_views_reset();
        debug_hide_ui_set(0);
        debug_hide_player_set(0, 0);
        debug_hide_player_set(1, 0);
        debug_screenshot_key_set(VK_F12);
        g_screenshot_listening = false;
        SetStatus("Debug settings reset to defaults");
        break;
    case 2:
        SetStatus(debug_settings_save() ? "Saved doa3_settings.ini"
                                        : "Could not write doa3_settings.ini");
        break;
    }
    ImGui::Separator();
    DrawCombatDebugMenu();
    DrawPerformanceDebugMenu();
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
        ImGui::TextWrapped("%s", view_help[d3d8_debug_view_get()]);
        if (view == D3D8_DEBUG_DISTANCE) {
            float range = d3d8_debug_distance_get();
            if (ImGui::SliderFloat("Distance range", &range, 10, 100000, "%.0f", ImGuiSliderFlags_Logarithmic))
                d3d8_debug_distance_set(range);
        }
        bool backfaces = d3d8_debug_backfaces_get() != 0;
        if (ImGui::Checkbox("Show backfaces", &backfaces)) d3d8_debug_backfaces_set(backfaces);
        if (ImGui::Button("Restore normal rendering")) d3d8_debug_views_reset();
        ImGui::TextWrapped("Views apply to depth-tested geometry. Transparency and cutouts may appear solid. "
                           "Backfaces can only reveal geometry submitted by the game. These options are temporary.");
        if (d3d8_debug_view_error()[0]) ImGui::TextWrapped("%s", d3d8_debug_view_error());
    }
    if (ImGui::CollapsingHeader("Unlockables")) {
        bool unlock = debug_unlock_all_get() != 0;
        if (ImGui::Checkbox("Unlock all unlockables", &unlock))
            debug_unlock_all_set(unlock ? 1 : 0);
        ImGui::TextWrapped("Unlocks characters, costumes and Theater movies. "
                           "Re-enter character select or Theater to refresh the list.");
        ImGui::Spacing();
        ImGui::TextDisabled("Your earned unlocks and save data are preserved.");
        ImGui::TextWrapped("Untick to use your normal unlocks. Choose Save to keep "
                           "this setting for next time.");
    }
    if (ImGui::CollapsingHeader("Diagnostics")) {
        bool cap = doa3_auto_capture_enabled() != 0;
        char env_value[8];
        bool env_forced = GetEnvironmentVariableA("DOA3_AUTO_CAPTURE", env_value, sizeof env_value) > 0;
        if (env_forced) ImGui::BeginDisabled();
        if (ImGui::Checkbox("Automatic diagnostic captures", &cap))
            doa3_auto_capture_set(cap ? 1 : 0);
        if (env_forced) ImGui::EndDisabled();
        ImGui::TextWrapped("Writes developer traces (mtrace_*.txt) and their screenshots (aa_*.bmp) "
                           "to the Diagnostics folder next to the game while you play, for bug reports. "
                           "Leave off for normal play. "
                           "Choose Save to keep this setting for next time.");
        if (env_forced)
            ImGui::TextDisabled("Set by the DOA3_AUTO_CAPTURE environment variable for this launch.");
        if (ImGui::Button("Trace the next frame")) {
            InterlockedExchange(&g_doa3_trace_request, 1);
            SetStatus("The next frame will be traced to the Diagnostics folder");
        }
        ImGui::TextWrapped("Records the next frame's graphics commands (mtrace_manual_*.txt) and a "
                           "screenshot (aa_*.bmp) in the Diagnostics folder. Use it while a visual bug "
                           "is on screen; the game keeps running behind this menu.");
        if (ImGui::Button("Save the next frame's textures")) {
            InterlockedExchange(&g_doa3_texdump_request, 1);
            SetStatus("The next frame's textures will be saved to the Textures folder");
        }
        ImGui::TextWrapped("Saves every texture drawn in the next frame as a .dds file in a Textures "
                           "folder next to the game, named by its memory address and size. Files are "
                           "kept between launches.");
    }
    if (ImGui::CollapsingHeader("Camera controls")) {
        float fov = free_camera_fov();
        if (ImGui::SliderFloat("Field of view", &fov, 15.0f, 120.0f, "%.1f degrees", ImGuiSliderFlags_AlwaysClamp))
            free_camera_set_fov(fov);
        if (ImGui::Button("Reset field of view")) free_camera_reset_fov();
        ImGui::TextWrapped("Vertical field of view. Works with the normal camera or free camera. "
                           "Temporary and unavailable during online play. Reset follows the game's camera zoom.");
        ImGui::Separator();
        bool camera = free_camera_enabled() != 0;
        if (ImGui::Checkbox("Free camera", &camera)) {
            if (!free_camera_set_enabled(camera ? 1 : 0))
                SetStatus("Enter an offline 3D scene before enabling the camera.");
        }
        float speed = free_camera_speed();
        if (ImGui::SliderFloat("Camera speed", &speed, 1.0f, 20.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
            free_camera_set_speed(speed);
        if (ImGui::Button("Reset to game camera")) free_camera_reset();
        ImGui::TextWrapped("Close this menu to fly. WASD: move; mouse: look; E/Q: up/down. "
                           "Shift: fast; Ctrl: slow.");
        ImGui::TextWrapped("Controller: left stick moves, right stick looks, RT/LT moves up/down. "
                           "LB: fast; RB: slow.");
        ImGui::TextWrapped("Esc releases the mouse. Untick Free camera to restore the game view. "
                           "Fighter controls are blocked while flying; the fight keeps running. "
                           "Camera mode is temporary and unavailable during online play.");
    }
    if (ImGui::CollapsingHeader("Screenshots and UI")) {
        ImGui::BeginDisabled(netplay_active() != 0);
        for (int side = 0; side < 2; ++side) {
            bool hide = debug_hide_player_get(side) != 0;
            if (ImGui::Checkbox(side ? "Hide Player 2" : "Hide Player 1", &hide))
                debug_hide_player_set(side, hide);
        }
        ImGui::EndDisabled();
        ImGui::TextDisabled("Character visibility is session only. Gameplay and collision boxes stay active.");
        bool hidden = debug_hide_ui_get() != 0;
        if (ImGui::Checkbox("Hide UI", &hidden)) debug_hide_ui_set(hidden);
        ImGui::TextWrapped("Hides game screen overlays, including HUD and menus. Some fades may also disappear. "
                           "Escape always opens these settings so you can restore the UI.");
        PadBinding key = { PADSRC_KEY, debug_screenshot_key_get() };
        char name[80];
        pad_binding_label(key, name, sizeof(name));
        ImGui::Text("Screenshot hotkey: %s", name);
        if (ImGui::Button(g_screenshot_listening ? "Press a key (Esc cancels)" : "Assign screenshot hotkey")) {
            g_screenshot_listening = !g_screenshot_listening;
            g_listening = -1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Default: F12")) {
            debug_screenshot_key_set(VK_F12); g_screenshot_listening = false;
            SetStatus(debug_screenshot_key_save() ? "Screenshot key saved" : "Key changed; could not save settings");
        }
        if (ImGui::Button("Take screenshot")) d3d8_screenshot_request();
        ImGui::TextWrapped("%s", d3d8_screenshot_status());
        ImGui::TextWrapped("Captures the game at its render resolution without this settings menu. "
                           "Use a key that is not assigned to fighter or camera controls. Hotkey changes save automatically.");
    } else {
        g_screenshot_listening = false;
    }
}

void DrawTrainingSection()
{
    static int slot=0;
    static bool loop=false;
    TrainingStatus status;
    training_status(&status);
    ImGui::TextWrapped("Guard, counter-action and counter-hit settings are available in the game's Sparring pause menu.");
    ImGui::Separator();
    if (!training_available()) {
        ImGui::TextDisabled("Available only in offline Sparring.");
        return;
    }
    ImGui::TextUnformatted("Dummy input recording");
    ImGui::Combo("Slot", &slot, "1\0" "2\0" "3\0");
    ImGui::Text("Recorded: %d / %d frames (%.1f seconds)", status.length[slot],
        TRAINING_MAX_FRAMES, status.length[slot]/60.0f);
    ImGui::Checkbox("Loop playback", &loop);
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
    ImGui::TextWrapped("Record temporarily gives you control of the dummy. Press Escape to finish recording or stop playback. Each slot holds up to 10 seconds. Directions replay as recorded; positions are not reset.");
    ImGui::TextWrapped("Recordings last for the current Sparring session. Recording again replaces the selected slot.");
    if (!ready)
        ImGui::TextWrapped("Enter a solo Sparring fight, close the game's pause menu, and turn off Exercise and free camera to record or play.");
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
    ImGui::SetNextWindowSize(ImVec2(560, 460), ImGuiCond_Appearing);

    bool open = true;
    bool debug_visible = false;
    if (ImGui::Begin("Dead or Alive 3", &open, ImGuiWindowFlags_NoCollapse)) {
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("Online")) {
                DrawOnlineSection();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Controls")) {
                DrawControlsSection();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Video")) {
                DrawVideoSection();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Audio")) {
                DrawAudioSection();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("General")) {
                DrawGeneralSection();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug")) {
                debug_visible = true;
                DrawDebugSection();
                ImGui::EndTabItem();
            }
            ImGui::BeginDisabled(!training_available());
            if (ImGui::BeginTabItem("Training")) {
                DrawTrainingSection();
                ImGui::EndTabItem();
            }
            ImGui::EndDisabled();

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
    if (!debug_visible || !open) g_screenshot_listening = false;

    if (!open) {
        g_visible = false;
        pad_mapping_set_menu_open(0);
    }
}

} /* namespace */

/* ── C API ─────────────────────────────────────────────────────────────── */

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

extern "C" void doa3_ui_toggle(void)
{
    g_visible = !g_visible;
    if (g_visible) training_stop();
    g_listening = -1;
    g_armed = false;
    g_pending_vk = 0;
    g_screenshot_listening = false;
    /* The input mapping applies the configured menu capture policy. */
    pad_mapping_set_menu_open(g_visible ? 1 : 0);
    if (g_visible) free_camera_release_mouse();
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
        } else {
            doa3_ui_toggle();
        }
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
    if (!g_visible && !NetplayBannerPending() && !combat_debug_flags() && !PerformanceOverlayEnabled() && training.mode==TRAINING_IDLE) return;
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

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    UpdateGamepadNavigation();
    ImGui::NewFrame();

    DrawCombatDebugOverlay();
    DrawPerformanceOverlay();
    DrawTrainingBanner();
    if (g_visible) DrawMenu();
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
    /* Hand the output merger back with the depth buffer attached. The bind
     * above passes a null depth-stencil view; leaving that bound means every
     * guest draw afterwards runs with no depth test and no depth writes. */
    d3d8_RestoreDefaultTarget();
    if (prev_nvp) ctx->RSSetViewports(prev_nvp, prev_vp);
}
