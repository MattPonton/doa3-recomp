/* Rollback event driver. Not yet connected to the DOA3 3.1 simulation.
 * See ROLLBACK.md before implementing the game callbacks. */
#ifndef DOA3_ROLLBACK_H
#define DOA3_ROLLBACK_H

#include <stdint.h>
#include "gekkonet.h"
#include "netplay_session.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NP_ROLLBACK_MAX_FRAMES 16
#define NP_ROLLBACK_STATE_HEADER 20u

typedef struct np_rollback_callbacks {
    /* Serialize all mutable simulation state, including RNG and guest time.
     * Never include uninitialized padding in the canonical checksum. Local
     * snapshots may contain pointers; the checksum must exclude their addresses.
     * Return 0 on failure. Do not retain the supplied buffers. */
    int (*save)(void *user, void *dst, uint32_t capacity,
                uint32_t *written, uint32_t *canonical_checksum);
    /* Validate the payload before modifying the game. Return 0 on failure. */
    int (*load)(void *user, const void *src, uint32_t bytes);
    /* Run exactly one 1/60-second simulation step, without polling live input,
     * presenting, sleeping, or advancing host wall time. For replay != 0 also
     * suppress irreversible effects (audio, rumble, file/network writes).
     * Both pads are immutable and ordered host, joiner. Return 0 on failure. */
    int (*advance)(void *user, int frame, const np_pad pads[2], int replay);
    /* Optional: present the final corrected state once per successful batch.
     * Called only after ALL save/load/advance events have completed. */
    void (*present)(void *user, int frame);
} np_rollback_callbacks;

typedef struct np_rollback {
    np_rollback_callbacks callbacks;
    void *user;
    uint32_t state_capacity;
    unsigned prediction_frames;
    int frame;                 /* state AFTER this frame; initial state is -1 */
    int latest_frame;          /* high-water mark, does not rewind */
    uint64_t forward_frames, replayed_frames, restores;
    unsigned max_rewind;
    int failed;
    char error[160];
} np_rollback;

/* All calls belong to the game thread at the same quiescent frame boundary.
 * A failed driver must be discarded along with its Gekko session; it cannot
 * fall back to local play with potentially partially advanced/restored state. */
int np_rollback_init(np_rollback *rb, const np_rollback_callbacks *callbacks,
                     void *user, uint32_t state_capacity, unsigned prediction_frames);
/* Produces a TWO-player rollback config, not the old 8-byte digest-only config.
 * Limited saving and runahead are deliberately unsupported. */
int np_rollback_config(const np_rollback *rb, GekkoConfig *config);
/* Consume Gekko's whole event batch synchronously before the next update.
 * Save buffers/checksum pointers are owned by Gekko and must not be deferred.
 * Returns 1 on success (including a stall with zero events), 0 on fatal error. */
int np_rollback_process(np_rollback *rb, GekkoGameEvent *const *events, int count);
/* Call BEFORE process() for the events returned by that same Gekko update. */
int np_rollback_session_events(np_rollback *rb, GekkoSessionEvent *const *events, int count);

#ifdef __cplusplus
}
#endif
#endif
