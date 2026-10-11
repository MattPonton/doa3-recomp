/**
 * Xbox Input -> XInput Compatibility Layer
 *
 * Translates Xbox controller API calls to Windows XInput.
 *
 * Host XInput user slots 0..3 are packed onto guest ports 0..n-1 in order, so
 * one controller is always player 1 whatever index it enumerates at (a
 * wireless receiver or a virtual pad can leave slot 0 empty). Port 0 can also
 * be driven from the keyboard through the mapping (pad_mapping.c): it reports
 * present with no controller attached once any key is bound, and the keyboard
 * overlays the controller on port 0 when both exist.
 */

#include "xinput_xbox.h"
#include "pad_mapping.h"
#include <xinput.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma comment(lib, "xinput.lib")

/* Guest reads always fetch fresh input. Device discovery can reuse connection
 * information from those reads instead of polling the same pad a second time.
 * Idle connected slots are checked every 250ms, empty slots every second. */
static struct { BOOL connected, probed; DWORD last_probe; } s_slot[XBOX_MAX_CONTROLLERS];
/* Guest port -> host slot, -1 = no controller. */
static int   s_port_slot[XBOX_MAX_CONTROLLERS] = { -1, -1, -1, -1 };
static DWORD s_kb_packet;
static BOOL s_active = TRUE;
static BOOL s_captured;
static BOOL s_rumbling[XBOX_MAX_CONTROLLERS];

static void stop_slot(int slot)
{
    XINPUT_VIBRATION off = { 0, 0 };
    if (slot >= 0 && s_rumbling[slot]) {
        XInputSetState((DWORD)slot, &off);
        s_rumbling[slot] = FALSE;
    }
}

void xbox_InputStopRumble(void)
{
    for (int slot = 0; slot < XBOX_MAX_CONTROLLERS; ++slot) stop_slot(slot);
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
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++) {
        XINPUT_STATE st;
        DWORD interval = s_slot[i].connected ? 250 : 1000;
        if (s_slot[i].probed && (now - s_slot[i].last_probe) < interval)
            continue;
        s_slot[i].probed = TRUE;
        s_slot[i].last_probe = now;
        s_slot[i].connected = (XInputGetState(i, &st) == ERROR_SUCCESS);
    }
}

static DWORD read_slot(int slot, XINPUT_STATE *state)
{
    DWORD result = XInputGetState((DWORD)slot, state);
    s_slot[slot].connected = result == ERROR_SUCCESS;
    s_slot[slot].probed = TRUE;
    s_slot[slot].last_probe = GetTickCount();
    return result;
}

static void rebuild_map(void)
{
    int p = 0, i;
    int previous[XBOX_MAX_CONTROLLERS];
    memcpy(previous, s_port_slot, sizeof(previous));
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++) s_port_slot[i] = -1;
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++)
        if (s_slot[i].connected) s_port_slot[p++] = i;
    /* A connected pad can move to a different guest port after hot-unplug.
     * Do not leave it playing the old player's effect. */
    for (i = 0; i < XBOX_MAX_CONTROLLERS; i++)
        if (previous[i] != s_port_slot[i]) stop_slot(previous[i]);
}

static int port_has_keyboard(DWORD dwPort)
{
    return dwPort == 0 && pad_mapping_has_key_binding();
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
        pad_mapping_apply_port(&xi_state, &pState->Gamepad, dwPort == 0);
        if (s_captured) memset(&pState->Gamepad, 0, sizeof(pState->Gamepad));
        return ERROR_SUCCESS;
    }
    if (slot >= 0) {
        /* Unplugged since the last poll: drop it now, XGetDeviceChanges
         * reports the removal on its next call. */
        rebuild_map();
    }

    /* No controller on this port. Port 0 can still be the keyboard, but ONLY
     * once the user has bound a key: with nothing bound we report the slot
     * empty rather than a pad that is not there. */
    if (!port_has_keyboard(dwPort))
        return ERROR_DEVICE_NOT_CONNECTED;

    memset(pState, 0, sizeof(XBOX_INPUT_STATE));
    pState->dwPacketNumber = ++s_kb_packet;
    pad_mapping_apply_port(NULL, &pState->Gamepad, 1);
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

    result = XInputSetState((DWORD)slot, &xi_vib);
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
        pCaps->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;
        return ERROR_SUCCESS;
    }

    result = XInputGetCapabilities((DWORD)slot, dwFlags, &xi_caps);
    if (result != ERROR_SUCCESS) return result;

    pCaps->Type = xi_caps.Type;
    pCaps->SubType = xi_caps.SubType;
    pCaps->Flags = xi_caps.Flags;

    return ERROR_SUCCESS;
}
