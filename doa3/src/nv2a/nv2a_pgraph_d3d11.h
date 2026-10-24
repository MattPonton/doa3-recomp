/*
 * NV2A PGRAPH → D3D11 Translator
 *
 * Intercepts NV2A push buffer method calls and translates them into
 * D3D8→D3D11 rendering commands. This is the core of the GPU translation
 * layer for Xbox static recompilation.
 *
 * The push buffer contains NV2A Kelvin (NV097) methods:
 *   - Surface/viewport setup → D3D11 render target + viewport
 *   - Render state (blend, depth, cull) → D3D11 state objects
 *   - Begin/End draw + Inline vertex data → D3D11 DrawPrimitiveUP
 *   - Texture binding → D3D11 shader resource views
 *   - Clear commands → D3D11 ClearRenderTargetView
 *
 * Vertex formats observed in menus:
 *   5 dwords per vertex: float X, float Y, float U, float V, D3DCOLOR
 *   Drawn as TRIANGLE_STRIP (mode 6)
 *
 * This module is designed to be reusable across Xbox recompilation projects.
 * See: https://github.com/sp00nznet/xboxrecomp
 */

#ifndef NV2A_PGRAPH_D3D11_H
#define NV2A_PGRAPH_D3D11_H

#include <stdint.h>

/* Initialize the PGRAPH→D3D11 translator. Call after D3D11 device is created. */
void pgraph_d3d11_init(void);

/* Opt-in, bounded metadata audit at the actual dispatcher boundaries.
 * No snapshots, GPU copies, waits, restore, or timing/input changes. */
int pgraph_d3d11_rollback_audit_enabled(void);
void pgraph_d3d11_rollback_audit_boundary(int before, int eligible);

/* Shut down and release resources. */
void pgraph_d3d11_shutdown(void);

/* Experimental restore barrier: renderer owner only, with all writers stopped.
 * Call after RAM restore, before any replay draw. Preserves native allocations
 * but requires exact mip/palette source validation before reusing an upload.
 * Changed or unverified content uploads again, bypassing sampled fingerprints.
 * Does not restore GPU targets or logical draw state. */
void pgraph_d3d11_invalidate_guest_uploads(void);
/* Owner-thread A/B control: force uploads at restore barriers. Returns the
 * previous setting; caller must restore it even when verification fails. */
int pgraph_d3d11_force_guest_uploads(int force);

/* Process an NV2A PGRAPH method call. Called from push buffer parser.
 * Returns 1 if handled, 0 if unhandled (caller should log/ignore). */
int pgraph_d3d11_method(int subchannel, uint32_t method, uint32_t param);
/* `count` methods of one packet; same effect as calling pgraph_d3d11_method
 * for each, faster for the common register streams. */
void pgraph_d3d11_method_run(int subchannel, uint32_t method, const uint32_t *params,
                             uint32_t count, int incr);
/* Non-incrementing vertex/index payload only. Returns 1 when consumed, 0
 * without changes when the caller must use individual method calls. */
int pgraph_d3d11_data_packet(int subchannel, uint32_t method,
                           const uint32_t *words, uint32_t count);
/* Owner-thread A/B switch; returns the previous setting. */
int pgraph_d3d11_force_scalar_data(int force);
uint64_t pgraph_d3d11_bulk_words(void);
int pgraph_d3d11_force_geometry_rebuild(int force);
void pgraph_d3d11_geometry_stats(uint64_t *hits,uint64_t *misses,uint64_t *vertices,uint64_t *resident_draws);
/* Diagnostic-only owner-thread gate for intermediate replay frames. Keeps
 * command ingestion/targets/clears, skips END's geometry/material preparation.
 * Render the last frame normally and verify/recover before resuming play. */
int pgraph_d3d11_skip_preparation(int skip);
void pgraph_d3d11_preparation_stats(uint64_t *batches,uint64_t *indices,uint64_t *inline_words);

/* Flush any pending draw commands (call at end of frame). */
void pgraph_d3d11_flush(void);

/* Set chyron scroll: pass frame counter to animate, 0 to disable.
 * Applies horizontal scroll offset to vertices in the chyron Y band. */
void pgraph_d3d11_set_chyron_scroll(uint32_t frame);

/* Statistics */
typedef struct {
    uint32_t frames;
    uint32_t draw_calls;
    uint32_t vertices_submitted;
    uint32_t methods_handled;
    uint32_t methods_ignored;
    uint32_t clears;
} PgraphD3D11Stats;

void pgraph_d3d11_get_stats(PgraphD3D11Stats *out);

#endif /* NV2A_PGRAPH_D3D11_H */
