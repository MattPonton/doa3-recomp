#ifndef D3D8_REPLAY_IMAGES_H
#define D3D8_REPLAY_IMAGES_H
#include <stdint.h>
#include <d3d11.h>

#define D3D8_REPLAY_IMAGE_LIMIT 20
typedef struct D3D8ReplayImages D3D8ReplayImages;
/* Experimental, owner-thread GPU copies, not serialized snapshots. Caller
 * must stop all renderer writers, supply a nonzero session epoch, and keep
 * them stopped through validation/restore. Null slots are significant.
 * Capture/restore do not change bindings or present anything. Only DEFAULT,
 * single-sample, single-level 2D color/depth images are supported. */
D3D8ReplayImages *d3d8_replay_images_capture(ID3D11Device *device,
    ID3D11DeviceContext *context, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count);
int d3d8_replay_images_validate(const D3D8ReplayImages *snapshot,
    ID3D11DeviceContext *context, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count);
/* 1 success; 0 rejected before mutation; -1 device failure after copies may
 * have started. A negative result poisons this snapshot; stop replay. */
int d3d8_replay_images_restore(D3D8ReplayImages *snapshot,
    ID3D11DeviceContext *context, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count);
/* Owner thread only. Returns zero without destroying on a wrong thread. */
int d3d8_replay_images_destroy(D3D8ReplayImages *snapshot);
/* Diagnostic only: synchronous, row-by-row GPU readback of two captures.
 * Returns 1 only for equal contents and source identities on the owner thread.
 * Does not copy into live images or change bindings. May stall the GPU. */
int d3d8_replay_images_equal(const D3D8ReplayImages *, const D3D8ReplayImages *);

/* Production surface inventory adapter. The opt-in live audit captures and
 * compares only; no game path restores these yet.
 * Includes guest color/depth, every offscreen color/depth pair, framebuffer
 * feedback and previous-frame history. Excludes swap chain/host UI. Rejects
 * topology, key, device, session, or thread changes rather than recreate them.
 * Caller must also restore RAM/PGRAPH and invalidate guest texture uploads;
 * this is NOT a whole renderer checkpoint or permission to resume mid-draw. */
typedef struct D3D8RenderCheckpoint D3D8RenderCheckpoint;
D3D8RenderCheckpoint *d3d8_render_checkpoint_capture(uint64_t epoch);
int d3d8_render_checkpoint_validate(const D3D8RenderCheckpoint *snapshot, uint64_t epoch);
int d3d8_render_checkpoint_restore(D3D8RenderCheckpoint *snapshot, uint64_t epoch);
int d3d8_render_checkpoint_destroy(D3D8RenderCheckpoint *snapshot);
int d3d8_render_checkpoint_equal(const D3D8RenderCheckpoint *, const D3D8RenderCheckpoint *);
int d3d8_render_checkpoint_equal_state(const D3D8RenderCheckpoint *, const D3D8RenderCheckpoint *);
#endif
