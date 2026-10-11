/**
 * Xbox Input -> Windows Controller Compatibility Layer
 *
 * Translates Xbox controller API calls to Windows XInput or native PS4 input.
 *
 * Each guest port selects Automatic, None, Keyboard, or an explicit host
 * slot. Explicit devices are reserved before automatic ports are packed.
 * Player 1's Automatic mode retains the legacy keyboard overlay/fallback.
 */

#include "xinput_xbox.h"
#include "pad_mapping.h"
#include "ds4_device.h"
#include <xinput.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma comment(lib, "xinput.lib")

/* Guest reads always fetch fresh input. Device discovery can reuse connection
 * information from those reads instead of polling the same pad a second time.
 * Idle connected slots are checked every 250ms, empty slots every second. */
static struct { BOOL connected, probed; DWORD last_probe; } s_slot[XBOX_HOST_CONTROLLERS];
/* Guest port -> host slot, -1 = no controller. */
static int   s_port_slot[XBOX_MAX_CONTROLLERS] = { -1, -1, -1, -1 };
static DWORD s_kb_packet;
static BOOL s_active = TRUE;
static BOOL s_captured;
static BOOL s_rumbling[XBOX_HOST_CONTROLLERS];
static DWORD s_reconfigured;

static void stop_slot(int slot)
{
    XINPUT_VIBRATION off = { 0, 0 };
    /* DS4 writes are asynchronous: stop even if the last request reported
     * an earlier write failure and the worker has since recovered. */
    if (slot >= 0 && slot < XBOX_HOST_CONTROLLERS && (s_rumbling[slot] || slot >= 4)) {
        if (slot < 4) XInputSetState((DWORD)slot, &off);
        else ds4_set_rumble(slot - 4, &off);
        s_rumbling[slot] = FALSE;
    }
}

void xbox_InputStopRumble(void)
{
    for (int slot = 0; slot < XBOX_HOST_CONTROLLERS; ++slot) stop_slot(slot);
}

void xbox_InputSetActive(BOOL active)
{
    s_active = active;
    if (!active) xbox_InputStopRumble();
}

static void probe_slots(void)
{
    DWORD now = GetTickCount();
    DWORD i;
    if (ds4_probe())
        for (i = 4; i < XBOX_HOST_CONTROLLERS; ++i) s_slot[i].probed = FALSE;
    for (i = 0; i < XBOX_HOST_CONTROLLERS; i++) {
        XINPUT_STATE st;
        DWORD interval = s_slot[i].connected ? 250 : 1000;
        if (s_slot[i].probed && (now - s_slot[i].last_probe) < interval)
            continue;
        s_slot[i].probed = TRUE;
        s_slot[i].last_probe = now;
        s_slot[i].connected = ((i < 4 ? XInputGetState(i, &st) : ds4_read((int)i - 4, &st)) == ERROR_SUCCESS);
    }
}

static DWORD read_slot(int slot, XINPUT_STATE *state)
{
    DWORD result = slot < 4 ? XInputGetState((DWORD)slot, state) : ds4_read(slot - 4, state);
    s_slot[slot].connected = result == ERROR_SUCCESS;
    s_slot[slot].probed = TRUE;
    s_slot[slot].last_probe = GetTickCount();
    return result;
}

DWORD xbox_InputReadPhysical(int slot, XINPUT_STATE *state)
{
    if (!state || slot < 0 || slot >= XBOX_HOST_CONTROLLERS) return ERROR_DEVICE_NOT_CONNECTED;
    return read_slot(slot, state);
}

static void rebuild_map(void)
{
    int p, i;
    DWORD reserved = 0;
    int previous[XBOX_MAX_CONTROLLERS];
    memcpy(previous, s_port_slot, sizeof(previous));
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++) s_port_slot[i] = -1;
    for (p = 0; p < XBOX_MAX_CONTROLLERS; ++p) {
        i = pad_mapping_device(p);
        if (i >= 0 && i < XBOX_HOST_CONTROLLERS && !(reserved & (1u << i))) {
            reserved |= 1u << i; /* Reserve even while disconnected. */
            if (s_slot[i].connected) s_port_slot[p] = i;
        }
    }
    for (p = 0; p < XBOX_MAX_CONTROLLERS; ++p) {
        if (pad_mapping_device(p) != PAD_DEVICE_AUTO) continue;
        for (i = 0; i < XBOX_HOST_CONTROLLERS; ++i) {
            if (!s_slot[i].connected || (reserved & (1u << i))) continue;
            s_port_slot[p] = i;
            reserved |= 1u << i;
            break;
        }
    }
    /* A connected pad can move to a different guest port after hot-unplug.
     * Do not leave it playing the old player's effect. */
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++)
        if (previous[i] != s_port_slot[i]) stop_slot(previous[i]);
}

static int port_has_keyboard(DWORD dwPort)
{
    int device = pad_mapping_device((int)dwPort);
    return device == PAD_DEVICE_KEYBOARD ||
        (device == PAD_DEVICE_AUTO && dwPort == 0 && pad_mapping_player_has_key(0));
}

void xbox_InputConfigurationChanged(void)
{
    static int devices[4] = {-1,-1,-1,-1}, types[4];
    int previous[4];
    memcpy(previous, s_port_slot, sizeof previous);
    rebuild_map();
    for (int p = 0; p < 4; ++p) {
        int device = pad_mapping_device(p), type = pad_mapping_type(p);
        if (devices[p] != device || types[p] != type || previous[p] != s_port_slot[p]) {
            s_reconfigured |= 1u << p;
            stop_slot(previous[p]);
        }
        devices[p] = device;
        types[p] = type;
    }
}
DWORD xbox_InputTakeReconfiguredMask(void)
{
    DWORD mask = s_reconfigured;
    s_reconfigured = 0;
    return mask;
}
DWORD xbox_InputPhysicalMask(void)
{
    DWORD mask = 0;
    probe_slots();
    rebuild_map();
    for (int i = 0; i < XBOX_HOST_CONTROLLERS; ++i) if (s_slot[i].connected) mask |= 1u << i;
    return mask;
}
int xbox_InputAssignedSlot(int player)
{
    return player >= 0 && player < 4 ? s_port_slot[player] : -1;
}
BYTE xbox_InputGuestSubtype(int player)
{
    /* Original Xbox XID subtypes, not Windows XInput subtype values. */
    static const BYTE subtypes[] = { 0x01, 0x02, 0x20 };
    int type = pad_mapping_type(player);
    return subtypes[type >= 0 && type < PAD_TYPE_COUNT ? type : 0];
}

void xbox_InputInit(void)
{
    probe_slots();
    rebuild_map();
}

DWORD xbox_InputHostMask(void)
{
    DWORD mask = 0, i;
    probe_slots();
    rebuild_map();
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++)
        if (s_port_slot[i] >= 0 || port_has_keyboard(i)) mask |= (1u << i);
    return mask;
}

void xbox_InputSetCaptured(BOOL captured)
{
    s_captured = captured;
    if (captured) xbox_InputStopRumble();
}

DWORD xbox_InputGetRawState(XINPUT_STATE *state)
{
    probe_slots();
    rebuild_map();
    memset(state, 0, sizeof(*state));
    if (!s_active || s_port_slot[0] < 0) return ERROR_DEVICE_NOT_CONNECTED;
    DWORD result = read_slot(s_port_slot[0], state);
    if (result != ERROR_SUCCESS) rebuild_map();
    return result;
}

DWORD xbox_InputGetState(DWORD dwPort, XBOX_INPUT_STATE *pState)
{
    XINPUT_STATE xi_state;
    int slot;

    if (dwPort >= XBOX_MAX_CONTROLLERS || !pState) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    slot = s_port_slot[dwPort];
    if (slot >= 0 && read_slot(slot, &xi_state) == ERROR_SUCCESS) {
        memset(pState, 0, sizeof(XBOX_INPUT_STATE));
        pState->dwPacketNumber = xi_state.dwPacketNumber;
        pad_mapping_apply_player((int)dwPort, &xi_state, &pState->Gamepad, port_has_keyboard(dwPort));
        if (s_captured) memset(&pState->Gamepad, 0, sizeof(pState->Gamepad));
        return ERROR_SUCCESS;
    }
    if (slot >= 0) {
        /* Unplugged since the last poll: drop it now, XGetDeviceChanges
         * reports the removal on its next call. */
        rebuild_map();
    }

    /* Explicit Keyboard remains present for binding; Automatic only exposes
     * a keyboard fallback on player 1 when keys are bound. */
    if (!port_has_keyboard(dwPort))
        return ERROR_DEVICE_NOT_CONNECTED;

    memset(pState, 0, sizeof(XBOX_INPUT_STATE));
    pState->dwPacketNumber = ++s_kb_packet;
    pad_mapping_apply_player((int)dwPort, NULL, &pState->Gamepad, 1);
    if (s_captured) memset(&pState->Gamepad, 0, sizeof(pState->Gamepad));
    return ERROR_SUCCESS;
}

DWORD xbox_InputSetState(DWORD dwPort, const XBOX_VIBRATION *pVibration)
{
    XINPUT_VIBRATION xi_vib;
    DWORD result;
    int slot;

    if (dwPort >= XBOX_MAX_CONTROLLERS || !pVibration) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    slot = s_port_slot[dwPort];
    if (slot < 0)
        return port_has_keyboard(dwPort) ? ERROR_SUCCESS : ERROR_DEVICE_NOT_CONNECTED;

    xi_vib.wLeftMotorSpeed = s_active && !s_captured ? pVibration->wLeftMotorSpeed : 0;
    xi_vib.wRightMotorSpeed = s_active && !s_captured ? pVibration->wRightMotorSpeed : 0;

    result = slot < 4 ? XInputSetState((DWORD)slot, &xi_vib) : ds4_set_rumble(slot - 4, &xi_vib);
    {
        static int trace = -1, count;
        static XINPUT_VIBRATION previous[4];
        static int seen[4], previous_active[4];
        if (trace < 0) trace = getenv("DOA3_RUMBLE_TRACE") != NULL;
        if (trace && count < 120 && (!seen[dwPort] ||
            previous[dwPort].wLeftMotorSpeed != pVibration->wLeftMotorSpeed ||
            previous[dwPort].wRightMotorSpeed != pVibration->wRightMotorSpeed ||
            previous_active[dwPort] != s_active)) {
            fprintf(stderr, "[RUMBLE] port=%lu slot=%d active=%d requested=%u/%u sent=%u/%u result=%lu\n",
                    dwPort, slot, s_active, pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed,
                    xi_vib.wLeftMotorSpeed, xi_vib.wRightMotorSpeed, result);
            fflush(stderr);
            previous[dwPort].wLeftMotorSpeed = pVibration->wLeftMotorSpeed;
            previous[dwPort].wRightMotorSpeed = pVibration->wRightMotorSpeed;
            previous_active[dwPort] = s_active;
            seen[dwPort] = 1;
            ++count;
        }
    }
    if (result == ERROR_SUCCESS)
        s_rumbling[slot] = xi_vib.wLeftMotorSpeed || xi_vib.wRightMotorSpeed;
    return result;
}

DWORD xbox_InputSetFeedback(DWORD dwPort, void *feedback)
{
    XBOX_VIBRATION vibration;
    DWORD result;
    BYTE *packet = (BYTE *)feedback;
    if (!packet) return ERROR_INVALID_PARAMETER;
    /* The guest packet is packed and often unaligned. Its 64-byte header
     * precedes a two-byte device report header and the two 16-bit motors.
     * DOA3 3.1's sub_000A50A0 passes pad+0x2F and writes pad+0x71/+0x73. */
    memcpy(&vibration.wLeftMotorSpeed, packet + 0x42, sizeof(WORD));
    memcpy(&vibration.wRightMotorSpeed, packet + 0x44, sizeof(WORD));
    result = xbox_InputSetState(dwPort, &vibration);
    memcpy(packet, &result, sizeof(result));
    return result;
}

BOOL xbox_InputIsConnected(DWORD dwPort)
{
    if (dwPort >= XBOX_MAX_CONTROLLERS) return FALSE;
    return s_port_slot[dwPort] >= 0 || port_has_keyboard(dwPort);
}

DWORD xbox_InputGetCapabilities(DWORD dwPort, DWORD dwFlags, XBOX_INPUT_CAPABILITIES *pCaps)
{
    XINPUT_CAPABILITIES xi_caps;
    DWORD result;
    int slot;

    if (dwPort >= XBOX_MAX_CONTROLLERS || !pCaps) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    slot = s_port_slot[dwPort];
    memset(pCaps, 0, sizeof(XBOX_INPUT_CAPABILITIES));
    if (slot < 0) {
        if (!port_has_keyboard(dwPort)) return ERROR_DEVICE_NOT_CONNECTED;
        pCaps->SubType = xbox_InputGuestSubtype((int)dwPort);
        return ERROR_SUCCESS;
    }

    if (slot >= 4) {
        if (!s_slot[slot].connected) return ERROR_DEVICE_NOT_CONNECTED;
        pCaps->Type = XINPUT_DEVTYPE_GAMEPAD;
        pCaps->SubType = xbox_InputGuestSubtype((int)dwPort);
        pCaps->Flags = ds4_has_rumble(slot - 4) ? XINPUT_CAPS_FFB_SUPPORTED : 0;
        return ERROR_SUCCESS;
    }
    result = XInputGetCapabilities((DWORD)slot, dwFlags, &xi_caps);
    if (result != ERROR_SUCCESS) return result;

    pCaps->Type = xi_caps.Type;
    pCaps->SubType = xbox_InputGuestSubtype((int)dwPort);
    pCaps->Flags = xi_caps.Flags;

    return ERROR_SUCCESS;
}
