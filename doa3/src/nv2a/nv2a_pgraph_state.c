#include "nv2a_pgraph_state.h"
#include <string.h>

static int valid_state(const Nv2aPgraphDrawState *draw, const Nv2aPgraphAuxState *aux)
{
    /* Raw register values (including NaNs and out-of-range VP load pointers)
     * are legal state: handlers already bounds-check their array writes.
     * Validate accumulator bounds, not assumptions about a particular game. */
    return draw && aux && (draw->in_draw == 0 || draw->in_draw == 1) &&
        draw->inline_count <= MAX_INLINE_VERTS * INLINE_VERT_DWORDS &&
        draw->idx_count <= MAX_IDX && draw->vp.instr_count <= NV2A_VP_MAX_INSTR &&
        aux->framebuffer_count >= 0 && aux->framebuffer_count <= 4;
}

int nv2a_pgraph_logical_capture(Nv2aPgraphLogicalImage *image,
    const Nv2aPgraphDrawState *draw, const Nv2aPgraphAuxState *aux,
    const float model_view[16])
{
    if (!image || !model_view || !valid_state(draw, aux)) return 0;
    image->version = 1;
    image->size = (uint32_t)sizeof(*image);
    memcpy(&image->draw, draw, sizeof(*draw));
    memcpy(&image->aux, aux, sizeof(*aux));
    memcpy(image->model_view, model_view, sizeof(image->model_view));
    return 1;
}

int nv2a_pgraph_logical_validate(const Nv2aPgraphLogicalImage *image)
{
    return image && image->version == 1 && image->size == sizeof(*image) &&
        valid_state(&image->draw, &image->aux);
}

int nv2a_pgraph_logical_restore_private(const Nv2aPgraphLogicalImage *image,
    Nv2aPgraphDrawState *draw, Nv2aPgraphAuxState *aux, float model_view[16])
{
    if (!draw || !aux || !model_view || !nv2a_pgraph_logical_validate(image)) return 0;
    memcpy(draw, &image->draw, sizeof(*draw));
    memcpy(aux, &image->aux, sizeof(*aux));
    memcpy(model_view, image->model_view, sizeof(image->model_view));
    return 1;
}
