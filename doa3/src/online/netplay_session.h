/**
 * netplay_session.h - lockstep session core for online VS play.
 *
 * A session covers one stay in Versus mode (Single Battle or Tag Battle).
 * It is armed from the Online tab and starts at the first input frame on
 * the VS character select screen (the barrier). From there until the
 * players leave Versus:
 *
 *   - both players' pads come from the session's input exchange, written
 *     into guest ports 0 (host) and 1 (joiner) before the per-frame
 *     aggregate builder runs; ports 2 and 3 are closed;
 *   - guest clocks and worker scheduling follow the frame counter
 *     (kernel/xbox_det.c);
 *   - the RNG seed, the match settings (the in-memory save image, keeping
 *     the local volume bytes and the joiner's own button layout) and the
 *     fight-state regions come from the session start parameters;
 *   - a digest of the fight state is recorded every frame.
 *
 * Outside a session none of this is active and the game runs as before.
 */
#ifndef DOA3_NETPLAY_SESSION_H
#define DOA3_NETPLAY_SESSION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* One player's pad for one frame: the XINPUT_GAMEPAD part of the guest
 * state (buttons, 8 analog buttons, 4 thumb axes), 18 bytes on the wire. */
#pragma pack(push, 1)
typedef struct {
    uint16_t buttons;
    uint8_t  analog[8];
    int16_t  lx, ly, rx, ry;
} np_pad;
#pragma pack(pop)

enum { NP_ROLE_HOST = 0, NP_ROLE_JOINER = 1 };

enum {
    NP_STATE_OFF = 0,     /* not armed: the game runs as before */
    NP_STATE_ARMED,       /* waiting for VS character select */
    NP_STATE_ACTIVE       /* lockstep session running */
};

/* Input exchange for one frame. `local` is this machine's pad; the provider
 * fills out[0] (guest port 0, host) and out[1] (guest port 1, joiner).
 * Phase 2 has only the local provider (both players on this machine). */
typedef struct {
    int  state;           /* NP_STATE_* */
    int  role;            /* NP_ROLE_* */
    int  tag;             /* 0 Single Battle, 1 Tag Battle (at the barrier) */
    uint32_t frame;       /* frames since the barrier */
    uint64_t digest;      /* digest of the latest frame */
    uint32_t sessions;    /* sessions completed since launch */
    char last_log[260];   /* digest log written by the last session */
} np_status;

/* Online tab controls. `role` only matters for a local session: JOINER
 * exercises the joiner path (save redirect, settings blob, restore). */
void netplay_set_armed(int armed, int role);
int  netplay_armed(void);
/* Local sessions only: player 2 copies player 1's pad (one-controller testing). */
void netplay_set_mirror(int on);
int  netplay_mirror(void);
int  netplay_active(void);
void netplay_get_status(np_status *out);

/* Game hooks (recomp_manual.c). */
void     netplay_input_tick(void);                  /* before the aggregate builder */
uint32_t netplay_filter_pad_mask(uint32_t host_mask);  /* XGetDevices / XGetDeviceChanges */

#ifdef __cplusplus
}
#endif

#endif /* DOA3_NETPLAY_SESSION_H */
