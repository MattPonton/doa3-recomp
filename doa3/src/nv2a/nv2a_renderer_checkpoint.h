#ifndef NV2A_RENDERER_CHECKPOINT_H
#define NV2A_RENDERER_CHECKPOINT_H
#include "nv2a_pgraph_state.h"
#include "../online/rollback_checkpoint.h"
/* Private renderer adapter. The coordinator must own all renderer writers.
 * The source state and callback context must outlive the coordinator/images.
 * The live audit binds capture/compare only; no live restore. Includes D3D
 * logical pipeline state; native bindings are rebuilt on replay. Captures
 * require an ended draw batch and no bound guest D3D8 vertex/index buffers.
 * Native immutable caches and upload rings are retained, not rewound. */
typedef struct {
    Nv2aPgraphDrawState *draw;
    Nv2aPgraphAuxState *aux;
    float *model_view;
    void *context;
    uint64_t (*epoch)(void *);
    void (*invalidate_uploads)(void *);
} Nv2aRendererCheckpointSource;
Doa3CheckpointResource nv2a_renderer_checkpoint_resource(Nv2aRendererCheckpointSource *source);
/* Read-only diagnostic including exact GPU pixel comparison. Never restores. */
int nv2a_renderer_checkpoint_equal(Nv2aRendererCheckpointSource *,const void *,const void *);
/* Live owner-thread boundary binding for the opt-in capture audit only. */
int pgraph_d3d11_rollback_snapshot_source(Nv2aRendererCheckpointSource *);
#endif
