/* Compiled inside device.c and by the production draw fixture. These images
 * are owned/authenticated by the renderer checkpoint, not accepted from peers. */
#include "d3d8_pipeline_checkpoint.h"
void d3d8_pipeline_probe(DWORD expected_owner, D3D8BoundaryProbe *out)
{
    ID3D11Texture2D *images[D3D8_REPLAY_IMAGE_LIMIT];
    ID3D11RenderTargetView *rtv=NULL;
    ID3D11DepthStencilView *dsv=NULL;
    D3D11_VIEWPORT vp;
    UINT count=1;
    unsigned i;
    if (!out) return;
    memset(out,0,sizeof(*out));
    /* Do not even query the immediate context from another thread. */
    if (!expected_owner || expected_owner!=GetCurrentThreadId()) {
        out->flags=D3D8_PROBE_OWNER; return;
    }
    if (!g_device_state.d3d11_device || !g_device_state.d3d11_context ||
        FAILED(ID3D11Device_GetDeviceRemovedReason(g_device_state.d3d11_device))) {
        out->flags=D3D8_PROBE_DEVICE; return;
    }
    if (g_cur_vb || g_cur_ib) out->flags|=D3D8_PROBE_BUFFERS;
    if (!render_checkpoint_inventory(images)) {
        out->flags|=D3D8_PROBE_INVENTORY; return;
    }
    out->identity=14695981039346656037ull;
#define PROBE_HASH(value) out->identity=(out->identity^(uint64_t)(value))*1099511628211ull
    PROBE_HASH((uintptr_t)g_device_state.d3d11_device);
    PROBE_HASH((uintptr_t)g_device_state.d3d11_context);
    for (i=0;i<D3D8_REPLAY_IMAGE_LIMIT;i++) {
        D3D11_TEXTURE2D_DESC d;
        PROBE_HASH((uintptr_t)images[i]);
        if (!images[i]) continue;
        ++out->image_count;
        ID3D11Texture2D_GetDesc(images[i],&d);
        if (d.Usage!=D3D11_USAGE_DEFAULT || d.MipLevels!=1 || d.ArraySize!=1 ||
            d.SampleDesc.Count!=1 || d.CPUAccessFlags || d.MiscFlags || !d.Width || !d.Height ||
            (d.Format!=DXGI_FORMAT_B8G8R8A8_UNORM && d.Format!=DXGI_FORMAT_R8G8B8A8_UNORM &&
             d.Format!=DXGI_FORMAT_D24_UNORM_S8_UINT && d.Format!=DXGI_FORMAT_D16_UNORM))
            out->flags|=D3D8_PROBE_FORMAT;
        else out->image_bytes+=(uint64_t)d.Width*d.Height*(d.Format==DXGI_FORMAT_D16_UNORM ? 2u:4u);
    }
    for (i=0;i<OFFRT_N;i++) { PROBE_HASH(g_offrt[i].key); }
#undef PROBE_HASH
    ID3D11DeviceContext_OMGetRenderTargets(g_device_state.d3d11_context,1,&rtv,&dsv);
    if (rtv!=cur_rtv() || dsv!=cur_dsv()) out->flags|=D3D8_PROBE_BINDINGS;
    if (rtv) ID3D11RenderTargetView_Release(rtv);
    if (dsv) ID3D11DepthStencilView_Release(dsv);
    ID3D11DeviceContext_RSGetViewports(g_device_state.d3d11_context,&count,&vp);
    if (count!=1) out->flags|=D3D8_PROBE_VIEWPORT;
}
static int pipeline_checkpoint_ready(void)
{
    return g_device_state.d3d11_context && !g_cur_vb && !g_cur_ib;
}
#define PIPELINE_FIELDS(X) \
    X(render_states) X(tss) X(transforms) X(viewport) X(material) X(lights) \
    X(light_enable) X(in_scene) X(vertex_shader) X(pixel_shader)
int d3d8_pipeline_capture(D3D8PipelineImage *out)
{
    if (!out || !pipeline_checkpoint_ready()) return 0;
    memset(out, 0, sizeof(*out));out->version=1;out->bytes=sizeof(*out);
#define SAVE(name) memcpy(&out->name, &g_device_state.name, sizeof(out->name));
    PIPELINE_FIELDS(SAVE)
#undef SAVE
    out->scissor_count=D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    ID3D11DeviceContext_RSGetScissorRects(g_device_state.d3d11_context,&out->scissor_count,out->scissors);
    d3d8_states_capture_private(&out->raster);
    d3d8_combiners_capture_private(&out->combiners);
    d3d8_vsh_capture_private(&out->vsh);
    return d3d8_pipeline_validate(out);
}
int d3d8_pipeline_validate(const D3D8PipelineImage *s)
{
    return s && pipeline_checkpoint_ready() && s->version==1 && s->bytes==sizeof(*s) &&
        s->scissor_count<=D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE &&
        (s->combiners.dirty==FALSE || s->combiners.dirty==TRUE) &&
        (s->combiners.direct_on==FALSE || s->combiners.direct_on==TRUE) &&
        s->combiners.state.num_stages>=0 && s->combiners.state.num_stages<=NV2A_MAX_COMBINER_STAGES &&
        s->combiners.direct.num_stages>=0 && s->combiners.direct.num_stages<=NV2A_MAX_COMBINER_STAGES &&
        d3d8_vsh_validate_private(&s->vsh);
}
int d3d8_pipeline_restore(const D3D8PipelineImage *s)
{
    if (!d3d8_pipeline_validate(s)) return 0;
    ID3D11DeviceContext_ClearState(g_device_state.d3d11_context);
#define RESTORE(name) memcpy(&g_device_state.name, &s->name, sizeof(s->name));
    PIPELINE_FIELDS(RESTORE)
#undef RESTORE
    memset(g_cur_textures,0,sizeof(g_cur_textures));
    g_cur_vb_stride=0;g_cur_ib_base_vertex=0;
    ID3D11DeviceContext_RSSetScissorRects(g_device_state.d3d11_context,s->scissor_count,s->scissors);
    d3d8_states_restore_private(&s->raster);
    d3d8_combiners_restore_private(&s->combiners);
    d3d8_vsh_restore_private(&s->vsh);
    /* Upload rings keep advancing; rewinding offsets could overwrite work
     * still consumed by the GPU. Every replay draw uploads its own vertices. */
    return 1;
}
#undef PIPELINE_FIELDS
