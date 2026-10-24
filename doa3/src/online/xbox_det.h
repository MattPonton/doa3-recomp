/**
 * xbox_det.h - deterministic time for lockstep netplay sessions.
 *
 * While a session is active every guest-visible clock is derived from the
 * lockstep frame counter instead of the host, so two machines fed the same
 * inputs see the same time values at the same frames:
 *
 *   KeTickCount            base_ms  + frames * 1000 / 60
 *   KeQueryPerformanceCounter  base_qpc + frames * freq / 60  (+ that fiber's poll boost)
 *   KeQuerySystemTime      base_ft  + frames * 10000000 / 60
 *   XAPI CPU timestamp    base_cpu + frames * 733333333 / 60 (+ scaled fiber boost)
 *
 * Worker-fiber timeslicing is taken off the 4 ms host timer as well: at each
 * lockstep frame the workers get a fixed number of laps and then run until
 * all of them have blocked (xbox_fiber_timeslice); otherwise they only run
 * when a guest thread waits.
 *
 * Nothing here changes behaviour outside a session. When a session ends the
 * host clocks resume with an offset, so no guest clock ever runs backwards.
 */
#ifndef XBOX_DET_H
#define XBOX_DET_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XBOX_DET_LAPS_PER_FRAME  4     /* the host timer's 4 ms period at 60 Hz */
#define XBOX_DET_VBLANK_KEY      0xDE7FB1A0u   /* wait key for workers parked on the vblank in a session */

/* Session clock bases. The host of a session chooses them and sends them to
 * the other side; a local session takes the current host values. */
typedef struct {
    uint32_t tick_ms;     /* KeTickCount at frame 0 */
    uint64_t qpc;         /* guest QPC at frame 0 */
    uint64_t filetime;    /* KeQuerySystemTime at frame 0 (100 ns units) */
    uint64_t rdtsc;       /* XAPI CPU timestamp at frame 0 (733,333,333 Hz) */
} xbox_det_bases;

extern volatile int g_xbox_det_active;

/* Current guest-visible values (real or deterministic), for building bases. */
void     xbox_det_current_bases(xbox_det_bases *out);

void     xbox_det_begin(const xbox_det_bases *b);
void     xbox_det_end(void);

/* One lockstep frame: advance the clocks, publish KeTickCount, and schedule
 * this frame's worker laps. Called from the game thread only. */
void     xbox_det_frame(void);
uint64_t xbox_det_frames(void);

/* Local rollback checkpoint for the clock subsystem ONLY. This does not save
 * fiber continuations, APU state, guest memory or kernel handles. Capture and
 * restore on the game thread at a quiescent boundary, alongside those systems.
 * Valid only within the same active session; never send this struct to a peer.
 * Session bases/frequency are immutable, host clock offsets and the diagnostic
 * IO counter deliberately do not rewind. */
#define XBOX_DET_CHECKPOINT_FIBERS 64
#define XBOX_DET_CHECKPOINT_VERSION 2u
typedef struct {
    uint32_t consecutive,run;
    int64_t last_call;
} xbox_det_qpc_poll_state;
typedef struct {
    uint32_t version, epoch;
    uint64_t frames, qpc_boost;
    uint32_t kcalls;
    int32_t pending_laps, slice_due;
    uint32_t fiber_kcalls[XBOX_DET_CHECKPOINT_FIBERS];
    uint64_t fiber_boost[XBOX_DET_CHECKPOINT_FIBERS];
    xbox_det_qpc_poll_state fiber_polls[XBOX_DET_CHECKPOINT_FIBERS];
} xbox_det_checkpoint;
int xbox_det_capture(xbox_det_checkpoint *out);
int xbox_det_restore(const xbox_det_checkpoint *saved);
int xbox_det_validate(const xbox_det_checkpoint *saved); /* no mutation */
/* Nonblocking, nonrecursive pin for a multi-component transaction. Only the
 * session owner on primary fiber may acquire. Holds off both host timer
 * writers; capture/validate/restore may be called while pinned. No gameplay
 * stepping, yielding or begin/end until release. Bounded timestamp-bookkeeping
 * helpers may read the clock while pinned. This pins only clocks. */
int xbox_det_checkpoint_acquire(void);
int xbox_det_checkpoint_release(void);

/* Clock sources used by the kernel bridges. */
uint32_t xbox_det_tick_ms(void);
uint64_t xbox_det_qpc(void);
uint64_t xbox_det_filetime(void);
/* XAPI timestamp, including normal host-clock operation outside a session.
 * In-session reads are pure: exact 60 Hz frame time plus per-fiber QPC boost,
 * scaled to CPU cycles. No host-clock calls, poll counters or worker yields. */
uint64_t xbox_det_rdtsc(void);

/* Guest file opens/reads, counted so a netplay session can start only once
 * the game's loading has gone quiet. */
void     xbox_det_note_io(void);
uint32_t xbox_det_io_count(void);

/* Hot-poll time boost (kernel_bridge QPC) accumulated within the session. */
void     xbox_det_add_qpc_boost(uint64_t delta);
/* Record one QPC poll AFTER on_kernel_call. Applies the existing hot/fallback
 * boost and returns 1 when it fires. Only the primary bridge services workers.
 * Poll history is checkpointed per fiber. Owner-thread only, never while
 * pinned; inactive/wrong-owner/pinned calls leave state unchanged. */
int      xbox_det_qpc_poll(void);
/* Progress the reviewed CRI voice-stop timeout (199E20), which polls RDTSC
 * rather than QPC. Adds a fixed 100 microseconds to THIS fiber's existing
 * checkpointed boost per iteration, never a gameplay tick or worker yield.
 * Inactive, foreign-owner and pinned calls do nothing. */
int      xbox_det_audio_wait_poll(void);

/* Changes on every begin/end, so callers can reset per-session state. */
uint32_t xbox_det_epoch(void);

/* Kernel calls made by the calling fiber this session (hot-poll detection). */
uint32_t xbox_det_fiber_kcalls(void);

/* Diagnostics: guest kernel calls, and the game thread's hot-poll boost. */
uint32_t xbox_det_kcalls(void);
uint64_t xbox_det_boost(void);

/* Offsets applied to the host clocks after a session so they stay monotonic. */
uint32_t xbox_det_tick_offset(void);
uint64_t xbox_det_qpc_offset(void);

/* Host-side writers that must not race a session start. */
void     xbox_det_publish_host_tick(uint32_t host_ms);  /* 1 ms tick thread */
void     xbox_det_host_slice_timer(void);                /* 4 ms slice timer */

/* Called on every guest kernel call (game thread). */
void     xbox_det_on_kernel_call(unsigned ordinal);


/* Worker laps for the slice that is due now (deterministic in a session). */
int      xbox_det_slice_laps(void);

#ifdef __cplusplus
}
#endif

#endif /* XBOX_DET_H */
