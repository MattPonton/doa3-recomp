#ifndef D3D8_PIPELINE_CHECKPOINT_H
#define D3D8_PIPELINE_CHECKPOINT_H
#include "d3d8_internal.h"

/* Read-only boundary observations. Zero flags means the observed conditions
 * passed, NOT that asynchronous writers are fenced or restore is safe.
 * Identity is a process-local diagnostic fingerprint, never a restore token. */
enum {
    D3D8_PROBE_OWNER=1, D3D8_PROBE_DEVICE=2, D3D8_PROBE_BUFFERS=4,
    D3D8_PROBE_INVENTORY=8, D3D8_PROBE_BINDINGS=16,
    D3D8_PROBE_VIEWPORT=32, D3D8_PROBE_FORMAT=64,
    D3D8_PROBE_BATCH=128
};
typedef struct {
    uint32_t flags, image_count;
    uint64_t identity, image_bytes;
} D3D8BoundaryProbe;
void d3d8_pipeline_probe(DWORD expected_owner, D3D8BoundaryProbe *out);

/* Private, pointer-free backend state. Only valid at an owner-fenced PGRAPH
 * boundary with no guest D3D8 VB/IB bindings. Texture bindings are rebuilt
 * from restored PGRAPH/RAM, not from potentially evicted host pointers. */
typedef struct {
    int polygon_bias;
    float polygon_slope, blend_factor[4];
} D3D8RasterImage;
typedef struct {
    DWORD token;
    BOOL dirty, direct_on;
    NV2ACombinerState state, direct;
} D3D8CombinerImage;
typedef struct {
    NV2AVshSlot slots[NV2A_VS_MAX_SLOTS];
    NV2AVSConstants constants;
    int count;
} D3D8VshImage;
typedef struct {
    uint32_t version, bytes;
    DWORD render_states[256], tss[4][32];
    D3DMATRIX transforms[512];
    D3DVIEWPORT8 viewport;
    D3DMATERIAL8 material;
    D3DLIGHT8 lights[8];
    BOOL light_enable[8], in_scene;
    DWORD vertex_shader, pixel_shader;
    UINT scissor_count;
    D3D11_RECT scissors[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    D3D8RasterImage raster;
    D3D8CombinerImage combiners;
    D3D8VshImage vsh;
} D3D8PipelineImage;

int d3d8_pipeline_capture(D3D8PipelineImage *);
int d3d8_pipeline_validate(const D3D8PipelineImage *);
/* Clears native bindings; caller must restore guest OM targets/viewport,
 * then PGRAPH state/upload validity before drawing. No live caller. */
int d3d8_pipeline_restore(const D3D8PipelineImage *);
void d3d8_states_capture_private(D3D8RasterImage *);
void d3d8_states_restore_private(const D3D8RasterImage *);
void d3d8_combiners_capture_private(D3D8CombinerImage *);
void d3d8_combiners_restore_private(const D3D8CombinerImage *);
void d3d8_vsh_capture_private(D3D8VshImage *);
int d3d8_vsh_validate_private(const D3D8VshImage *);
void d3d8_vsh_restore_private(const D3D8VshImage *);
#endif
