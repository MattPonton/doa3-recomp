#ifndef DOA3_ROLLBACK_CHECKPOINT_H
#define DOA3_ROLLBACK_CHECKPOINT_H
#include <stdint.h>
#include <stddef.h>

/* LOCAL transaction for ten checkpoint domains, not a complete game snapshot.
 * Fixtures cover private offline and production audio processing. No live
 * whole-fight adapter is enabled.
 * Files commit first; RAM precedes clock (which republishes its exported tick).
 * The optional renderer sidecar owns private PGRAPH/backend/GPU images;
 * XPP packet/device bookkeeping is mandatory, even with recorded pad reads.
 * Whole-fight writer exclusion and kernel objects still need separate work. */
enum { DOA3_CP_FILES, DOA3_CP_RAM, DOA3_CP_HEAP, DOA3_CP_FIBERS, DOA3_CP_CLOCK,
       DOA3_CP_COMPLETIONS, DOA3_CP_INTERRUPTS, DOA3_CP_GPU_FRONTEND, DOA3_CP_AUDIO,
       DOA3_CP_INPUT, DOA3_CP_PARTS };
typedef struct {
    void *context;
    size_t bytes;
    int (*capture)(void *,void *,size_t);
    int (*validate)(void *,const void *,size_t); /* read-only, >0 valid */
    /* >0 success, 0 failure (possibly partially applied), <0 irrecoverable.
     * Must also accept the captured before-image during compensation. */
    int (*restore)(void *,const void *,size_t);
    /* Optional read-only execution preflight, checked before ANY mutation.
     * Separate from image validation: capture may be valid while restore is
     * forbidden (for example, until an audio output gate has been opened). */
    int (*can_restore)(void *);
} Doa3CheckpointPart;

/* Optional nonserializable renderer sidecar. Capture returns an owned image;
 * destroy must accept any successfully captured image, including stale epochs.
 * All callbacks run under the same acquire/release fence as the byte domains.
 * Restore follows the byte domains and must support compensation. */
typedef struct {
    void *context;
    void *(*capture)(void *);
    int (*validate)(void *,const void *);
    int (*restore)(void *,const void *);
    void (*destroy)(void *,void *);
    /* Optional exact, read-only comparison of two captured resources. */
    int (*equal)(void *,const void *,const void *);
    /* Optional diagnostic comparison excluding GPU pixels ONLY. Never used
     * to authorize restoration or an ordinary exact replay match. */
    int (*equal_state)(void *,const void *,const void *);
} Doa3CheckpointResource;
typedef struct {
    void *context;
    /* Owner-thread-only, nonrecursive. Acquire must pin ALL component writers,
     * at a primary dispatcher boundary, including audio and host clock writers.
     * Callbacks cannot yield, deliver completions, present, or submit audio. */
    int (*acquire)(void *);
    void (*release)(void *);
    uint64_t (*epoch)(void *); /* nonzero, immutable while acquired */
    Doa3CheckpointPart parts[DOA3_CP_PARTS];
    Doa3CheckpointResource renderer;
} Doa3CheckpointConfig;
typedef struct Doa3CheckpointSession Doa3CheckpointSession;
Doa3CheckpointSession *doa3_checkpoint_create(const Doa3CheckpointConfig *);
void doa3_checkpoint_destroy(Doa3CheckpointSession *);
/* Checked destroy for adapters whose callback context must outlive cleanup. */
int doa3_checkpoint_dispose(Doa3CheckpointSession *);
size_t doa3_checkpoint_size(const Doa3CheckpointSession *);
int doa3_checkpoint_poisoned(const Doa3CheckpointSession *);
/* Buffers must be caller-owned, outside every component's mutable state.
 * Capture publishes only on success; rejected restore leaves frame untouched.
 * Integrity hash detects accidents, not hostile data. Never accept peer blobs.
 * Restore: 1 committed, 0 rejected/compensated, -1 poisoned (stop simulation).
 * A poisoned session refuses further work; recreating it does not repair state. */
int doa3_checkpoint_capture(Doa3CheckpointSession *,uint64_t frame,void *,size_t);
int doa3_checkpoint_restore(Doa3CheckpointSession *,const void *,size_t,uint64_t *frame);

/* Owned local checkpoints are required when renderer is configured; raw byte
 * APIs then refuse operation. Handles belong to this session and cannot be
 * copied/serialized. Discard each when retired; session destruction retires
 * all remaining handles. Cleanup still works after poison/epoch changes, but
 * requires the owner fence (destroy leaves session alive if acquire refuses).
 * No live game adapter is enabled by these APIs. */
typedef struct Doa3CheckpointImage Doa3CheckpointImage;
Doa3CheckpointImage *doa3_checkpoint_image_capture(Doa3CheckpointSession *,uint64_t frame);
int doa3_checkpoint_image_restore(Doa3CheckpointSession *,const Doa3CheckpointImage *,uint64_t *frame);
int doa3_checkpoint_image_discard(Doa3CheckpointSession *,Doa3CheckpointImage *);
typedef struct {
    uint32_t different_parts;
    size_t first_byte[DOA3_CP_PARTS]; /* SIZE_MAX when that domain matches */
    int renderer_different;
    int renderer_state_equal; /* explicitly verified, false when unavailable */
} Doa3CheckpointComparison;
/* Capture/compare current state under the same fence; NEVER restore it.
 * 1 exact match, 0 mismatch, -1 invalid image/context or capture failure.
 * Resources must supply equal(). Comparison is published only on 0 or 1. */
int doa3_checkpoint_image_matches(Doa3CheckpointSession *,const Doa3CheckpointImage *,Doa3CheckpointComparison *);
#endif
