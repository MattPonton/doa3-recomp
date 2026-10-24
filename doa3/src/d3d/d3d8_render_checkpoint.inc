/* Included after OffRT/FbCopy definitions. Also compiled by the WARP fixture.
 * Live capture/compare is opt-in. Renderer ownership and a complete restore
 * transaction must be established before enabling live restore. */
struct D3D8RenderCheckpoint {
    DWORD owner;
    D3D8ReplayImages *images;
    uint32_t keys[OFFRT_N];
    int off_cur, off_active, prevframe_valid;
    D3D11_VIEWPORT viewport;
};

static int render_checkpoint_inventory(ID3D11Texture2D **images)
{
    unsigned i;
    if (!g_device_state.default_tex || !g_device_state.default_depth ||
        !g_device_state.default_rtv || !g_device_state.default_dsv ||
        g_off_cur < -1 || g_off_cur >= OFFRT_N ||
        (g_off_active && (g_off_cur < 0 || !g_offrt[g_off_cur].tex)) ||
        (g_prevframe_valid && (!g_prevframe.tex || !g_prevframe.srv))) return 0;
    images[0] = g_device_state.default_tex;
    images[1] = g_device_state.default_depth;
    for (i = 0; i < OFFRT_N; i++) {
        if (!!g_offrt[i].tex != !!g_offrt[i].depth ||
            (g_offrt[i].tex && (!g_offrt[i].rtv || !g_offrt[i].dsv))) return 0;
        images[2 + i * 2] = g_offrt[i].tex;
        images[3 + i * 2] = g_offrt[i].depth;
    }
    images[18] = g_fbcopy.tex; images[19] = g_prevframe.tex;
    return 1;
}

int d3d8_render_checkpoint_destroy(D3D8RenderCheckpoint *s)
{
    if (!s) return 1;
    if (!d3d8_replay_images_destroy(s->images)) return 0;
    free(s);
    return 1;
}

D3D8RenderCheckpoint *d3d8_render_checkpoint_capture(uint64_t epoch)
{
    ID3D11Texture2D *images[D3D8_REPLAY_IMAGE_LIMIT];
    ID3D11RenderTargetView *rtv = NULL;
    ID3D11DepthStencilView *dsv = NULL;
    D3D8RenderCheckpoint *s;
    D3D11_VIEWPORT vp;
    UINT count = 1;
    unsigned i;
    int bound;
    if (!g_device_state.d3d11_context || !render_checkpoint_inventory(images)) return NULL;
    /* Refuse host composition and unexpected viewport layouts. */
    ID3D11DeviceContext_OMGetRenderTargets(g_device_state.d3d11_context, 1, &rtv, &dsv);
    bound = rtv == cur_rtv() && dsv == cur_dsv();
    if (rtv) ID3D11RenderTargetView_Release(rtv);
    if (dsv) ID3D11DepthStencilView_Release(dsv);
    ID3D11DeviceContext_RSGetViewports(g_device_state.d3d11_context, &count, &vp);
    if (!bound || count != 1) return NULL;
    s = (D3D8RenderCheckpoint *)calloc(1, sizeof(*s));
    if (!s) return NULL;
    s->owner = GetCurrentThreadId();
    s->images = d3d8_replay_images_capture(g_device_state.d3d11_device,
        g_device_state.d3d11_context, epoch, images, D3D8_REPLAY_IMAGE_LIMIT);
    if (!s->images) { free(s); return NULL; }
    for (i = 0; i < OFFRT_N; i++) s->keys[i] = g_offrt[i].key;
    s->off_cur = g_off_cur; s->off_active = g_off_active;
    s->prevframe_valid = g_prevframe_valid; s->viewport = vp;
    return s;
}

int d3d8_render_checkpoint_validate(const D3D8RenderCheckpoint *s, uint64_t epoch)
{
    ID3D11Texture2D *images[D3D8_REPLAY_IMAGE_LIMIT];
    unsigned i;
    if (!s || s->owner != GetCurrentThreadId() || !render_checkpoint_inventory(images)) return 0;
    for (i = 0; i < OFFRT_N; i++) if (s->keys[i] != g_offrt[i].key) return 0;
    return d3d8_replay_images_validate(s->images, g_device_state.d3d11_context,
        epoch, images, D3D8_REPLAY_IMAGE_LIMIT);
}

int d3d8_render_checkpoint_equal_state(const D3D8RenderCheckpoint *a,const D3D8RenderCheckpoint *b)
{
    return a && b && a->owner==GetCurrentThreadId() && b->owner==GetCurrentThreadId() &&
        !memcmp(a->keys,b->keys,sizeof(a->keys)) && a->off_cur==b->off_cur &&
        a->off_active==b->off_active && a->prevframe_valid==b->prevframe_valid &&
        !memcmp(&a->viewport,&b->viewport,sizeof(a->viewport));
}
int d3d8_render_checkpoint_equal(const D3D8RenderCheckpoint *a,const D3D8RenderCheckpoint *b)
{
    return d3d8_render_checkpoint_equal_state(a,b) &&
        d3d8_replay_images_equal(a->images,b->images);
}

int d3d8_render_checkpoint_restore(D3D8RenderCheckpoint *s, uint64_t epoch)
{
    ID3D11Texture2D *images[D3D8_REPLAY_IMAGE_LIMIT];
    ID3D11ShaderResourceView *nulls[4] = {0};
    ID3D11RenderTargetView *rtv;
    int result;
    if (!d3d8_render_checkpoint_validate(s, epoch)) return 0;
    if (!render_checkpoint_inventory(images)) return 0;
    result = d3d8_replay_images_restore(s->images, g_device_state.d3d11_context,
        epoch, images, D3D8_REPLAY_IMAGE_LIMIT);
    if (result != 1) return result;
    /* Remove sampled images from the future. PGRAPH must rebuild bindings
     * from its restored state before the next draw, including guest uploads. */
    ID3D11DeviceContext_PSSetShaderResources(g_device_state.d3d11_context, 0, 4, nulls);
    memset(g_cur_textures, 0, sizeof g_cur_textures);
    g_off_cur = s->off_cur; g_off_active = s->off_active;
    g_prevframe_valid = s->prevframe_valid;
    rtv = cur_rtv();
    ID3D11DeviceContext_OMSetRenderTargets(g_device_state.d3d11_context, 1, &rtv, cur_dsv());
    ID3D11DeviceContext_RSSetViewports(g_device_state.d3d11_context, 1, &s->viewport);
    return 1;
}
