#pragma once

/* Device-thread cache. The context retains bound SRVs, so these pointers do
 * not need another reference. Invalidate before changing render targets
 * (D3D11 can unbind conflicting SRVs) and use the shared binding epoch for
 * external draws. Callers must not sample the currently active target. */
typedef struct D3D8SrvBindings {
    ID3D11DeviceContext *context;
    ID3D11ShaderResourceView *views[4];
    unsigned valid, epoch;
} D3D8SrvBindings;

static inline int d3d8_srv_bind(D3D8SrvBindings *cache, ID3D11DeviceContext *context,
                              unsigned epoch, UINT stage, ID3D11ShaderResourceView *view)
{
    if (stage >= 4) {
        ID3D11DeviceContext_PSSetShaderResources(context, stage, 1, &view);
        return 1;
    }
    if (cache->context != context || cache->epoch != epoch) {
        cache->valid = 0;
        cache->context = context;
        cache->epoch = epoch;
    }
    if ((cache->valid & (1u << stage)) && cache->views[stage] == view) return 0;
    ID3D11DeviceContext_PSSetShaderResources(context, stage, 1, &view);
    cache->views[stage] = view;
    cache->valid |= 1u << stage;
    return 1;
}
