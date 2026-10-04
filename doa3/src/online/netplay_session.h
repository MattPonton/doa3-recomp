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
    int  replaying;       /* a replay is loaded or running */
    uint32_t replay_frames;
    uint32_t diverged_at; /* first differing frame, 0xFFFFFFFF while matching */
    char message[200];    /* last record/replay message */
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

/* Determinism testing. Recording writes netplay_replay_*.dnr (start
 * parameters, both pads and the digest for every frame) when a session
 * ends. A replay starts at the same VS character select, plays the recorded
 * pads, and reports the first frame whose digest differs. */
void netplay_set_record(int on);
int  netplay_record(void);
void netplay_request_replay(const char *path);   /* loaded at the next game frame */
int  netplay_list_replays(char (*names)[128], int max);

/* Game hooks (recomp_manual.c). */
void     netplay_input_tick(void);                  /* before the aggregate builder */
void     netplay_note_rand(void);                   /* every CRT rand() call */
uint32_t netplay_filter_pad_mask(uint32_t host_mask);  /* XGetDevices / XGetDeviceChanges */
/* Ports the session forces present (0 outside a session/lobby). A forced
 * port the game has not opened yet must be reported as an insertion
 * whatever the change baseline says. */
uint32_t netplay_forced_pads(void);

/* ── Remote input provider (netplay.c) ─────────────────────────────────
 * With a provider set, an armed session is an online one: the provider
 * decides when it starts, supplies both players' pads every frame and is
 * told the per-frame digest and the end of the session. Everything is
 * called on the game thread from the input tick. */
typedef struct {
    /* Every input tick while armed and not yet running. `at_barrier` is set
     * once the local game has settled at VS character select; `tag` is the
     * battle type chosen there. Return 1 to begin the session (the start
     * parameters were loaded with netplay_params_load or captured with
     * netplay_params_capture), 0 to keep waiting, -1 to disarm. */
    int  (*barrier)(int at_barrier, int tag);
    /* One lockstep frame: this machine's pad in, both players' pads out
     * (0 = host, 1 = joiner). Returns 0 when the session must end. May
     * block until the other side's input is in. */
    int  (*exchange)(const np_pad *local, np_pad out[2]);
    /* The digest of the state entering the frame just exchanged. */
    void (*digest)(uint32_t frame, uint64_t digest);
    /* The session ended (the players left Versus, or exchange returned 0). */
    void (*end)(void);
    /* A screen transition inside the session (same frame on both sides).
     * The host captures its fight-state regions (netplay_sync_capture) and
     * sends them; the joiner waits for that transition's data and applies
     * it (netplay_sync_apply) before the frame continues, so load timing
     * before the transition cannot leave the two games apart. Return 0 to
     * end the session (no data arrived). */
    int  (*transition)(uint32_t index);
} np_provider;

/* The fight-state regions as one blob (the sync set), for mid-session
 * resyncs at screen transitions. Capture buffer is owned by this module. */
int  netplay_sync_capture(const uint8_t **blob, uint32_t *len);
int  netplay_sync_apply(const uint8_t *blob, uint32_t len);

void netplay_set_provider(const np_provider *p);   /* NULL: local sessions only */

/* Start parameters as one blob. The host captures them at the barrier (with
 * the joiner's own button layout, received over the network) and sends them;
 * the joiner loads what it received. The capture buffer belongs to this
 * module until the next capture. Both return non-zero on success. */
int  netplay_params_capture(const uint8_t **blob, uint32_t *len,
                            const uint8_t joiner_layout[10], uint8_t joiner_laysel);
int  netplay_params_load(const uint8_t *blob, uint32_t len);
/* This machine's port-0 button layout from the save image (a joiner sends
 * it to the host so the session maps its buttons the way it expects). */
void netplay_local_layout(uint8_t layout[10], uint8_t *laysel);
/* The executable's link timestamp: two machines must run the same build. */
uint32_t netplay_build_stamp(void);
/* Why the running session is ending (written into the digest log header). */
void netplay_session_set_end_note(const char *note);

/* The per-region hashes recorded for a frame of the running session, so the
 * two machines can tell each other which region went different on a
 * desync. Returns the number of regions written (0 if the frame is not
 * recorded); netplay_session_region_name names them. */
#define NP_MAX_REGIONS 64
int  netplay_session_frame_regions(uint32_t frame, uint32_t *out, int max);
int  netplay_session_region_count(void);
/* Raw guest bytes (for the desync report). */
void netplay_session_peek(uint32_t va, void *dst, uint32_t len);
const char *netplay_session_region_name(int k);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_NETPLAY_SESSION_H */
