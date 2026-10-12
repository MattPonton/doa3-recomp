/* Bounded, opt-in geometry interpolation. Capture only draws to the guest
 * backbuffer. Render-to-texture passes retain their native 60 Hz cadence.
 * GPU constant buffers are copied at the draw, never read back to the CPU.
 * Replay runs on a deferred context into private color/depth targets; executing
 * its command list restores ALL immediate-context state. Guest render targets,
 * crossfade history, counters and simulation memory are never changed. */
#include "d3d8_internal.h"
#include "d3d8_interpolation.h"
#include "interpolation_math.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_COMMANDS 4096
#define MAX_GEOMETRY (32u * 1024u * 1024u)
#define MAX_TEXTURES 1024
#define MAX_TEXTURE_BYTES (128u * 1024u * 1024u)
#define RELEASE(p, type) do { if (p) type##_Release(p); (p) = NULL; } while (0)

typedef struct Draw {
    unsigned kind, count, stride, index_count, flags, stencil;
    uint32_t color;
    float z;
    unsigned char *vertices;
    uint16_t *indices;
    size_t capacity, index_capacity;
    int ff, depth_enabled;
    Nv2aFFConstants constants;
    ID3D11InputLayout *layout;
    ID3D11VertexShader *vs;
    ID3D11PixelShader *ps;
    ID3D11RasterizerState *rs;
    ID3D11BlendState *blend;
    ID3D11DepthStencilState *depth;
    ID3D11ShaderResourceView *srv[4];
    ID3D11ShaderResourceView *identity[4];
    ID3D11SamplerState *sampler[4];
    ID3D11Buffer *vscb[2], *pscb;
    unsigned char *vsdata[2], *psdata;
    unsigned vssize[2], pssize;
    uint64_t key;
    ID3D11Buffer *vb, *ib, *ffcb;
    unsigned vb_size, ib_size;
    float blend_factor[4];
    unsigned sample_mask, stencil_ref, nvp, nsc;
    D3D11_VIEWPORT vp[16];
    D3D11_RECT sc[16];
    D3D11_PRIMITIVE_TOPOLOGY topology;
} Draw;

typedef struct Texture {
    ID3D11ShaderResourceView *source, *copy_srv;
    ID3D11Resource *source_resource;
    ID3D11Texture2D *copy;
    D3D11_TEXTURE2D_DESC desc;
    D3D11_SHADER_RESOURCE_VIEW_DESC view;
    int stale;
} Texture;

typedef struct Frame {
    Draw *draw;
    unsigned count, bytes, scene;
    LONGLONG capture_ticks;
    int valid, cleared;
    Texture texture[MAX_TEXTURES];
    unsigned texture_count, texture_bytes;
} Frame;

static Frame frames[3];
static unsigned current;
static int enabled, ready;
static ID3D11DeviceContext *replay;
static ID3D11Texture2D *color_target, *depth_target;
static ID3D11RenderTargetView *rtv;
static ID3D11DepthStencilView *dsv;
static ID3D11ShaderResourceView *result;
static unsigned width, height;
static unsigned char *scratch;
static size_t scratch_size;
static uint64_t geometry_key;
typedef struct ConstantCache { ID3D11Buffer *buffer; unsigned size; unsigned char data[4096]; } ConstantCache;
static ConstantCache constants[8];
void d3d8_interpolation_key(uint64_t key) { geometry_key=key; }
void d3d8_interpolation_constants(ID3D11Buffer *buffer, const void *data, unsigned size)
{
    unsigned i;
    if (!enabled || size>sizeof constants[0].data) return;
    for(i=0;i<8;++i) if (!constants[i].buffer || constants[i].buffer==buffer) {
        constants[i].buffer=buffer; constants[i].size=size;
        memcpy(constants[i].data,data,size); return;
    }
}
unsigned g_doa3_interpolated_frames, g_doa3_interpolated_draws;
unsigned g_doa3_interpolation_commands, g_doa3_interpolation_status;
unsigned g_doa3_interpolation_capture_us, g_doa3_interpolation_replay_us;
unsigned g_doa3_interpolation_build_us, g_doa3_interpolation_execute_us;
unsigned g_doa3_interpolation_buffer_creates;
static LONGLONG frequency;

int d3d8_interpolation_ready(void) { return ready; }

void d3d8_interpolation_texture_changed(ID3D11Resource *resource)
{
    Frame *f=&frames[current];
    unsigned i;
    if (!enabled) return;
    for (i=0;i<f->texture_count;++i)
        if (f->texture[i].source_resource==resource) f->texture[i].stale=1;
}

/* Texture contents can change while the next simulation frame renders.
 * Keep the version that each captured draw actually sampled. Slots are reused
 * after three frames; no per-frame CreateTexture churn in a stable scene. */
static ID3D11ShaderResourceView *snapshot_texture(ID3D11ShaderResourceView *source)
{
    Frame *f=&frames[current];
    ID3D11Resource *resource=NULL;
    ID3D11Texture2D *texture=NULL;
    D3D11_TEXTURE2D_DESC desc;
    D3D11_SHADER_RESOURCE_VIEW_DESC view;
    Texture *slot;
    unsigned i;
    size_t bytes;
    if (!source) return NULL;
    for (i=0;i<f->texture_count;++i) if (f->texture[i].source==source && !f->texture[i].stale)
        return f->texture[i].copy_srv;
    if (f->texture_count==MAX_TEXTURES) goto fail;
    ID3D11ShaderResourceView_GetResource(source,&resource);
    if (FAILED(ID3D11Resource_QueryInterface(resource,&IID_ID3D11Texture2D,(void **)&texture))) goto fail;
    ID3D11Texture2D_GetDesc(texture,&desc);
    ID3D11ShaderResourceView_GetDesc(source,&view);
    bytes=(size_t)desc.Width*desc.Height*desc.ArraySize*8; /* conservative mip budget */
    if (bytes>MAX_TEXTURE_BYTES-f->texture_bytes) goto fail;
    desc.Usage=D3D11_USAGE_DEFAULT; desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags=0; desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
    slot=&f->texture[f->texture_count++];
    RELEASE(slot->source,ID3D11ShaderResourceView);
    RELEASE(slot->source_resource,ID3D11Resource);
    if (!slot->copy || memcmp(&desc,&slot->desc,sizeof desc) || memcmp(&view,&slot->view,sizeof view)) {
        RELEASE(slot->copy_srv,ID3D11ShaderResourceView); RELEASE(slot->copy,ID3D11Texture2D);
        if (FAILED(ID3D11Device_CreateTexture2D(d3d8_GetD3D11Device(),&desc,NULL,&slot->copy)) ||
            FAILED(ID3D11Device_CreateShaderResourceView(d3d8_GetD3D11Device(),(ID3D11Resource *)slot->copy,&view,&slot->copy_srv))) goto fail;
        slot->desc=desc; slot->view=view;
    }
    slot->source=source; ID3D11ShaderResourceView_AddRef(source);
    slot->source_resource=resource; resource=NULL;
    slot->stale=0;
    f->texture_bytes+=(unsigned)bytes;
    ID3D11DeviceContext_CopyResource(d3d8_GetD3D11Context(),(ID3D11Resource *)slot->copy,(ID3D11Resource *)texture);
    ID3D11Texture2D_Release(texture);
    return slot->copy_srv;
fail:
    RELEASE(resource,ID3D11Resource); RELEASE(texture,ID3D11Texture2D);
    f->valid=0;
    return NULL;
}

static void release_state(Draw *d)
{
    int i;
    RELEASE(d->layout, ID3D11InputLayout);
    RELEASE(d->vs, ID3D11VertexShader); RELEASE(d->ps, ID3D11PixelShader);
    RELEASE(d->rs, ID3D11RasterizerState); RELEASE(d->blend, ID3D11BlendState);
    RELEASE(d->depth, ID3D11DepthStencilState);
    for (i=0; i<4; ++i) {
        RELEASE(d->srv[i], ID3D11ShaderResourceView);
        RELEASE(d->sampler[i], ID3D11SamplerState);
    }
}

static int buffer(ID3D11Buffer **out, unsigned bytes, unsigned bind)
{
    D3D11_BUFFER_DESC bd;
    int geometry = bind == D3D11_BIND_VERTEX_BUFFER || bind == D3D11_BIND_INDEX_BUFFER;
    if (*out) {
        ID3D11Buffer_GetDesc(*out, &bd);
        if (bd.ByteWidth == bytes || (geometry && bd.ByteWidth >= bytes)) return 1;
        RELEASE(*out, ID3D11Buffer);
    }
    memset(&bd, 0, sizeof bd);
    bd.ByteWidth = bytes; bd.Usage = D3D11_USAGE_DEFAULT; bd.BindFlags = bind;
    if (geometry) {
        bd.ByteWidth=(bytes+255u)&~255u;
        bd.Usage=D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
    }
    if (FAILED(ID3D11Device_CreateBuffer(d3d8_GetD3D11Device(), &bd, NULL, out))) return 0;
    ++g_doa3_interpolation_buffer_creates;
    return 1;
}

/* Discard lets the driver rename storage still used by the GPU. Copy only
 * the live range: pooled geometry buffers can be larger than this draw. */
static int upload_geometry(ID3D11Buffer *buffer, const void *data, unsigned bytes)
{
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (FAILED(ID3D11DeviceContext_Map(replay,(ID3D11Resource *)buffer,0,
            D3D11_MAP_WRITE_DISCARD,0,&mapped))) return 0;
    memcpy(mapped.pData,data,bytes);
    ID3D11DeviceContext_Unmap(replay,(ID3D11Resource *)buffer,0);
    return 1;
}

static int copy_cb(ID3D11Buffer **out, unsigned char **data, unsigned *size, ID3D11Buffer *source)
{
    D3D11_BUFFER_DESC bd;
    unsigned i;
    if (!source) { RELEASE(*out, ID3D11Buffer); free(*data); *data=NULL; *size=0; return 1; }
    ID3D11Buffer_GetDesc(source, &bd);
    if (!buffer(out, bd.ByteWidth, D3D11_BIND_CONSTANT_BUFFER)) return 0;
    for(i=0;i<8;++i) if(constants[i].buffer==source && constants[i].size==bd.ByteWidth) {
        if (*size!=bd.ByteWidth) {
            void *p=realloc(*data,bd.ByteWidth);
            if (!p) return 0;
            *data=p;
        }
        *size=bd.ByteWidth;
        memcpy(*data,constants[i].data,*size);
        return 1;
    }
    free(*data); *data=NULL; *size=0;
    ID3D11DeviceContext_CopyResource(d3d8_GetD3D11Context(),
        (ID3D11Resource *)*out, (ID3D11Resource *)source);
    return 1;
}

static Draw *append(unsigned kind)
{
    Frame *f = &frames[current];
    Draw *d;
    if (!enabled || !f->valid) return NULL;
    if (f->count == MAX_COMMANDS) { f->valid = 0; return NULL; }
    if (!f->draw) f->draw = calloc(MAX_COMMANDS, sizeof(Draw));
    if (!f->draw) { f->valid = 0; return NULL; }
    d = &f->draw[f->count++];
    release_state(d);
    d->kind = kind;
    return d;
}

void d3d8_interpolation_invalidate(void)
{
    frames[current].valid = 0;
}

void d3d8_interpolation_clear(unsigned flags, uint32_t color, float z, unsigned stencil)
{
    Draw *d = append(0);
    if (!d) return;
    d->flags = flags; d->color = color; d->z = z; d->stencil = stencil;
    if (frames[current].count == 1 && (flags & (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER)) ==
            (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER)) frames[current].cleared = 1;
}

void d3d8_interpolation_draw(const void *vertices, unsigned count, unsigned stride,
    const uint16_t *indices, unsigned index_count, const Nv2aFFConstants *ff)
{
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    ID3D11Buffer *vs[2] = {0}, *ps = NULL;
    D3D11_DEPTH_STENCIL_DESC depth_desc;
    Frame *f = &frames[current];
    Draw *d;
    size_t size = (size_t)count * stride, isize = (size_t)index_count * 2;
    int i, ok = 1;
    LARGE_INTEGER capture_begin, capture_end;
    if (!enabled || !f->valid) return;
    QueryPerformanceCounter(&capture_begin);
    if (size + isize > MAX_GEOMETRY - f->bytes) { f->valid = 0; return; }
    d = append(1);
    if (!d) return;
    f->bytes += (unsigned)(size + isize);
    if (size > d->capacity) {
        void *p = realloc(d->vertices, size);
        if (!p) { f->valid = 0; return; }
        d->vertices = p; d->capacity = size;
    }
    if (isize > d->index_capacity) {
        void *p = realloc(d->indices, isize);
        if (!p) { f->valid = 0; return; }
        d->indices = p; d->index_capacity = isize;
    }
    memcpy(d->vertices, vertices, size);
    if (isize) memcpy(d->indices, indices, isize);
    d->count = count; d->stride = stride; d->index_count = index_count;
    d->key=geometry_key;
    d->ff = ff != NULL;
    if (ff) d->constants = *ff;
    ID3D11DeviceContext_IAGetInputLayout(ctx, &d->layout);
    ID3D11DeviceContext_IAGetPrimitiveTopology(ctx, &d->topology);
    ID3D11DeviceContext_VSGetShader(ctx, &d->vs, NULL, NULL);
    ID3D11DeviceContext_PSGetShader(ctx, &d->ps, NULL, NULL);
    ID3D11DeviceContext_RSGetState(ctx, &d->rs);
    ID3D11DeviceContext_OMGetBlendState(ctx, &d->blend, d->blend_factor, &d->sample_mask);
    ID3D11DeviceContext_OMGetDepthStencilState(ctx, &d->depth, &d->stencil_ref);
    d->depth_enabled = 1;
    if (d->depth) { ID3D11DepthStencilState_GetDesc(d->depth, &depth_desc); d->depth_enabled = depth_desc.DepthEnable; }
    ID3D11DeviceContext_PSGetShaderResources(ctx, 0, 4, d->srv);
    for (i=0;i<4;++i) {
        ID3D11ShaderResourceView *source=d->srv[i];
        d->identity[i]=source;
        d->srv[i]=snapshot_texture(source);
        if (d->srv[i]) ID3D11ShaderResourceView_AddRef(d->srv[i]);
        RELEASE(source,ID3D11ShaderResourceView);
    }
    ID3D11DeviceContext_PSGetSamplers(ctx, 0, 4, d->sampler);
    d->nvp = d->nsc = 16;
    ID3D11DeviceContext_RSGetViewports(ctx, &d->nvp, d->vp);
    ID3D11DeviceContext_RSGetScissorRects(ctx, &d->nsc, d->sc);
    ID3D11DeviceContext_VSGetConstantBuffers(ctx, 0, 2, vs);
    ID3D11DeviceContext_PSGetConstantBuffers(ctx, 0, 1, &ps);
    for (i=0; i<2; ++i) {
        ok &= copy_cb(&d->vscb[i], &d->vsdata[i], &d->vssize[i], vs[i]);
        RELEASE(vs[i], ID3D11Buffer);
    }
    ok &= copy_cb(&d->pscb, &d->psdata, &d->pssize, ps); RELEASE(ps, ID3D11Buffer);
    if (!ok) f->valid = 0;
    QueryPerformanceCounter(&capture_end);
    f->capture_ticks += capture_end.QuadPart-capture_begin.QuadPart;
}

static void release_targets(void)
{
    RELEASE(result, ID3D11ShaderResourceView); RELEASE(rtv, ID3D11RenderTargetView);
    RELEASE(dsv, ID3D11DepthStencilView); RELEASE(color_target, ID3D11Texture2D);
    RELEASE(depth_target, ID3D11Texture2D);
    width = height = 0;
}

static int targets(void)
{
    ID3D11Device *dev = d3d8_GetD3D11Device();
    D3D11_TEXTURE2D_DESC td;
    unsigned w = d3d8_GetBackbufferWidth(), h = d3d8_GetBackbufferHeight();
    if (!replay && FAILED(ID3D11Device_CreateDeferredContext(dev, 0, &replay))) return 0;
    if (w == width && h == height && result && dsv) return 1;
    release_targets();
    memset(&td, 0, sizeof td);
    td.Width = w; td.Height = h; td.MipLevels = td.ArraySize = td.SampleDesc.Count = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    if (FAILED(ID3D11Device_CreateTexture2D(dev, &td, NULL, &color_target)) ||
        FAILED(ID3D11Device_CreateRenderTargetView(dev, (ID3D11Resource *)color_target, NULL, &rtv)) ||
        FAILED(ID3D11Device_CreateShaderResourceView(dev, (ID3D11Resource *)color_target, NULL, &result))) goto fail;
    td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    if (FAILED(ID3D11Device_CreateTexture2D(dev, &td, NULL, &depth_target)) ||
        FAILED(ID3D11Device_CreateDepthStencilView(dev, (ID3D11Resource *)depth_target, NULL, &dsv))) goto fail;
    width = w; height = h;
    return 1;
fail:
    release_targets(); return 0;
}

/* Draw order alone is insufficient: reject different topology, material,
 * shader, or index streams. Discontinuities use the current unmodified draw. */
static int compatible(const Draw *a, const Draw *b)
{
    return a->kind == 1 && b->kind == 1 && a->count == b->count &&
        a->stride == b->stride && a->key == b->key && a->ff == b->ff && a->vs == b->vs && a->ps == b->ps &&
        a->topology == b->topology && a->index_count == b->index_count &&
        a->depth == b->depth && a->blend == b->blend && a->rs == b->rs &&
        !memcmp(a->identity, b->identity, sizeof a->identity) &&
        !memcmp(a->vp, b->vp, sizeof(D3D11_VIEWPORT)*a->nvp) &&
        a->nvp == b->nvp && a->nsc == b->nsc &&
        !memcmp(a->sc, b->sc, sizeof(D3D11_RECT)*a->nsc) &&
        (!a->index_count || !memcmp(a->indices, b->indices, a->index_count*2));
}

/* Particle/effect draws can shift every subsequent command's ordinal. Match
 * keyed meshes independently of that order; reject ambiguous instances rather
 * than interpolating one fighter toward another sharing the same model. */
static const Draw *previous_draw(const Frame *old, const Draw *d, unsigned ordinal)
{
    const Draw *match=NULL;
    unsigned i,j;
    if (!d->ff || !d->key)
        return ordinal<old->count && compatible(&old->draw[ordinal],d) ? &old->draw[ordinal] : NULL;
    for(i=0;i<old->count;++i) {
        const Draw *candidate=&old->draw[i];
        if (!compatible(candidate,d)) continue;
        if (d->stride!=sizeof(Nv2aFFVertex)) continue;
        for(j=0;j<d->count;++j) {
            const Nv2aFFVertex *a=(const Nv2aFFVertex *)candidate->vertices+j;
            const Nv2aFFVertex *b=(const Nv2aFFVertex *)d->vertices+j;
            if (memcmp(a->uv,b->uv,sizeof a->uv) || memcmp(a->uv1,b->uv1,sizeof a->uv1)) break;
        }
        if(j!=d->count) continue;
        if(match) return NULL;
        match=candidate;
    }
    return match;
}

static int interpolate(Draw *d, const Draw *old, float t, Nv2aFFConstants *cb)
{
    unsigned i, j;
    size_t size = (size_t)d->count*d->stride;
    if (scratch_size < size) {
        void *p = realloc(scratch, size);
        if (!p) return 0;
        scratch = p; scratch_size = size;
    }
    memcpy(scratch, d->vertices, size);
    if (d->ff) {
        float *dst;
        const float *old_clip, *new_clip;
        if (d->stride != sizeof(Nv2aFFVertex) || d->constants.flags != old->constants.flags) return 0;
        *cb = d->constants;
        /* Interpolate geometry only. Lighting and generated reflection UVs use
         * the current normals/model-view, matching the CPU vertex path's
         * current diffuse/specular colors. This avoids mixing different
         * lighting times across CPU and GPU material passes. */
        dst = &cb->clip[0][0]; old_clip = &old->constants.clip[0][0]; new_clip = &d->constants.clip[0][0];
        for (j=0; j<16; ++j) {
            if (!isfinite(old_clip[j]) || !isfinite(new_clip[j])) return 0;
            dst[j] = old_clip[j] + (new_clip[j]-old_clip[j])*t;
        }
        for (i=0; i<d->count; ++i) {
            Nv2aFFVertex *v = (Nv2aFFVertex *)scratch+i;
            const Nv2aFFVertex *a = (const Nv2aFFVertex *)old->vertices+i;
            const Nv2aFFVertex *b = (const Nv2aFFVertex *)d->vertices+i;
            if (memcmp(a->uv,b->uv,sizeof a->uv) || memcmp(a->uv1,b->uv1,sizeof a->uv1)) return 0;
            float ca[4]={0}, cbv[4]={0};
            unsigned row, col;
            for (row=0; row<4; ++row) for (col=0; col<4; ++col) {
                ca[row] += old->constants.clip[row][col]*a->pos[col];
                cbv[row] += d->constants.clip[row][col]*b->pos[col];
            }
            if (ca[3] > 0.01f && cbv[3] > 0.01f &&
                (fabsf(ca[0]/ca[3]-cbv[0]/cbv[3]) > 0.3f ||
                 fabsf(ca[1]/ca[3]-cbv[1]/cbv[3]) > 0.3f)) return 0;
            /* Reject teleports/reused dynamic geometry instead of drawing stretched limbs. */
            for (j=0; j<3; ++j) {
                if (!isfinite(a->pos[j]) || !isfinite(b->pos[j]) || fabsf(a->pos[j]-b->pos[j])>0.75f) return 0;
                v->pos[j] = a->pos[j]+(b->pos[j]-a->pos[j])*t;
            }
        }
    } else {
        /* Only the NV2A output format, with depth enabled. HUD stays exact. */
        if (d->stride != 36 || !d->depth_enabled) return 0;
        for (i=0; i<d->count; ++i) {
            const float *a=(const float *)(old->vertices+i*d->stride);
            const float *b=(const float *)(d->vertices+i*d->stride);
            float *v=(float *)(scratch+i*d->stride);
            if (fabsf(a[0]-b[0])>width*0.15f || fabsf(a[1]-b[1])>height*0.15f ||
                !interpolation_position(v,a,b,t)) return 0;
        }
    }
    return 1;
}

void d3d8_interpolation_finish(int active, unsigned scene)
{
    Frame *done = &frames[current], *previous = &frames[(current+2)%3];
    if (!frequency) { LARGE_INTEGER f; QueryPerformanceFrequency(&f); frequency=f.QuadPart; }
    if (!active) {
        if (enabled) d3d8_interpolation_shutdown();
        return;
    }
    ready = enabled && active && done->valid && done->cleared && previous->valid &&
        previous->cleared && previous->scene == scene && done->scene == scene;
    g_doa3_interpolation_commands=done->count;
    g_doa3_interpolation_capture_us=(unsigned)(done->capture_ticks*1000000/frequency);
    g_doa3_interpolation_status=ready?3:!done->valid?2:1;
    current = (current+1)%3;
    frames[current].count = frames[current].bytes = 0;
    frames[current].capture_ticks=0;
    frames[current].cleared = 0;
    frames[current].valid = active;
    frames[current].scene = scene;
    frames[current].texture_count = frames[current].texture_bytes = 0;
    enabled = active;
}

typedef struct ReplayConstant {
    ID3D11Buffer *buffer;
    const unsigned char *data;
    unsigned size;
} ReplayConstant;

static ID3D11Buffer *replay_constant(ReplayConstant *last, ID3D11Buffer *buffer,
    const unsigned char *data, unsigned size)
{
    if (data && last->data && size==last->size && !memcmp(data,last->data,size))
        return last->buffer;
    if (data) ID3D11DeviceContext_UpdateSubresource(replay,(ID3D11Resource *)buffer,0,NULL,data,0,0);
    last->buffer=buffer; last->data=data; last->size=size;
    return buffer;
}

ID3D11ShaderResourceView *d3d8_interpolation_render(float alpha)
{
    Frame *f = &frames[(current+2)%3], *old = &frames[(current+1)%3];
    ID3D11CommandList *list = NULL;
    ReplayConstant constant[3]={{0}};
    const Draw *last=NULL;
    unsigned i;
    LARGE_INTEGER begin, submitted, end;
    if (!ready || alpha > 1 || alpha < 0 || !targets()) return NULL;
    QueryPerformanceCounter(&begin);
    g_doa3_interpolation_buffer_creates=0;
    g_doa3_interpolated_draws=0;
    ID3D11DeviceContext_ClearState(replay);
    ID3D11DeviceContext_OMSetRenderTargets(replay, 1, &rtv, dsv);
    for (i=0; i<f->count; ++i) {
        Draw *d = &f->draw[i];
        if (!d->kind) {
            float c[4] = { ((d->color>>16)&255)/255.0f, ((d->color>>8)&255)/255.0f,
                (d->color&255)/255.0f, (d->color>>24)/255.0f };
            unsigned flags = ((d->flags&D3DCLEAR_ZBUFFER)?D3D11_CLEAR_DEPTH:0) |
                ((d->flags&D3DCLEAR_STENCIL)?D3D11_CLEAR_STENCIL:0);
            if (d->flags&D3DCLEAR_TARGET) ID3D11DeviceContext_ClearRenderTargetView(replay, rtv, c);
            if (flags) ID3D11DeviceContext_ClearDepthStencilView(replay, dsv, flags, d->z, (UINT8)d->stencil);
        } else {
            unsigned zero = 0, size=d->count*d->stride;
            const void *data=d->vertices;
            ID3D11Buffer *vscb[2]={d->vscb[0],d->vscb[1]};
            ID3D11Buffer *pscb;
            Nv2aFFConstants cb;
            const Draw *prior=alpha<1 ? previous_draw(old,d,i) : NULL;
            int ff_interpolated=0;
            if (prior && interpolate(d,prior,alpha,&cb)) {
                ++g_doa3_interpolated_draws;
                data=scratch;
                if (d->ff) {
                    if (!buffer(&d->ffcb,sizeof cb,D3D11_BIND_CONSTANT_BUFFER)) goto fail;
                    ID3D11DeviceContext_UpdateSubresource(replay,(ID3D11Resource *)d->ffcb,0,NULL,&cb,0,0);
                    vscb[0]=d->ffcb;
                    ff_interpolated=1;
                }
            }
            if (!ff_interpolated)
                vscb[0]=replay_constant(&constant[0],d->vscb[0],d->vsdata[0],d->vssize[0]);
            vscb[1]=replay_constant(&constant[1],d->vscb[1],d->vsdata[1],d->vssize[1]);
            pscb=replay_constant(&constant[2],d->pscb,d->psdata,d->pssize);
            if (!buffer(&d->vb,size,D3D11_BIND_VERTEX_BUFFER)) goto fail;
            if (!upload_geometry(d->vb,data,size)) goto fail;
            ID3D11DeviceContext_IASetVertexBuffers(replay,0,1,&d->vb,&d->stride,&zero);
            if (!last || last->layout!=d->layout) ID3D11DeviceContext_IASetInputLayout(replay,d->layout);
            if (!last || last->topology!=d->topology) ID3D11DeviceContext_IASetPrimitiveTopology(replay,d->topology);
            if (!last || last->vs!=d->vs) ID3D11DeviceContext_VSSetShader(replay,d->vs,NULL,0);
            if (!last || last->ps!=d->ps) ID3D11DeviceContext_PSSetShader(replay,d->ps,NULL,0);
            ID3D11DeviceContext_VSSetConstantBuffers(replay,0,2,vscb);
            ID3D11DeviceContext_PSSetConstantBuffers(replay,0,1,&pscb);
            if (!last || memcmp(last->srv,d->srv,sizeof d->srv)) ID3D11DeviceContext_PSSetShaderResources(replay,0,4,d->srv);
            if (!last || memcmp(last->sampler,d->sampler,sizeof d->sampler)) ID3D11DeviceContext_PSSetSamplers(replay,0,4,d->sampler);
            if (!last || last->rs!=d->rs) ID3D11DeviceContext_RSSetState(replay,d->rs);
            if (!last || last->nvp!=d->nvp || memcmp(last->vp,d->vp,d->nvp*sizeof *d->vp))
                ID3D11DeviceContext_RSSetViewports(replay,d->nvp,d->vp);
            if (!last || last->nsc!=d->nsc || memcmp(last->sc,d->sc,d->nsc*sizeof *d->sc))
                ID3D11DeviceContext_RSSetScissorRects(replay,d->nsc,d->sc);
            if (!last || last->blend!=d->blend || last->sample_mask!=d->sample_mask ||
                    memcmp(last->blend_factor,d->blend_factor,sizeof d->blend_factor))
                ID3D11DeviceContext_OMSetBlendState(replay,d->blend,d->blend_factor,d->sample_mask);
            if (!last || last->depth!=d->depth || last->stencil_ref!=d->stencil_ref)
                ID3D11DeviceContext_OMSetDepthStencilState(replay,d->depth,d->stencil_ref);
            if (d->index_count) {
                if (!buffer(&d->ib,d->index_count*2,D3D11_BIND_INDEX_BUFFER)) goto fail;
                if (!upload_geometry(d->ib,d->indices,d->index_count*2)) goto fail;
                ID3D11DeviceContext_IASetIndexBuffer(replay,d->ib,DXGI_FORMAT_R16_UINT,0);
                ID3D11DeviceContext_DrawIndexed(replay,d->index_count,0,0);
            } else ID3D11DeviceContext_Draw(replay,d->count,0);
            last=d;
        }
    }
    if (FAILED(ID3D11DeviceContext_FinishCommandList(replay,FALSE,&list))) return NULL;
    QueryPerformanceCounter(&submitted);
    ID3D11DeviceContext_ExecuteCommandList(d3d8_GetD3D11Context(),list,TRUE);
    ID3D11CommandList_Release(list);
    ++g_doa3_interpolated_frames;
    QueryPerformanceCounter(&end);
    g_doa3_interpolation_replay_us=(unsigned)((end.QuadPart-begin.QuadPart)*1000000/frequency);
    g_doa3_interpolation_build_us=(unsigned)((submitted.QuadPart-begin.QuadPart)*1000000/frequency);
    g_doa3_interpolation_execute_us=(unsigned)((end.QuadPart-submitted.QuadPart)*1000000/frequency);
    return result;
fail:
    /* Discard a partial replay, preserving the complete native frame. */
    ID3D11DeviceContext_FinishCommandList(replay,FALSE,&list);
    RELEASE(list,ID3D11CommandList);
    return NULL;
}

/* Explicit developer opt-in only: validate the replay at alpha=1 against the
 * native frame before diagnosing interpolation. Never run during normal play. */
static unsigned char *diagnostic_pixels(ID3D11Texture2D *texture, const char *path)
{
    D3D11_TEXTURE2D_DESC desc;
    D3D11_MAPPED_SUBRESOURCE mapped;
    ID3D11Texture2D *staging=NULL;
    unsigned char *pixels=NULL;
    unsigned row;
    ID3D11Texture2D_GetDesc(texture,&desc);
    desc.Usage=D3D11_USAGE_STAGING; desc.BindFlags=0;
    desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ; desc.MiscFlags=0;
    if(FAILED(ID3D11Device_CreateTexture2D(d3d8_GetD3D11Device(),&desc,NULL,&staging))) return NULL;
    ID3D11DeviceContext_CopyResource(d3d8_GetD3D11Context(),(ID3D11Resource *)staging,(ID3D11Resource *)texture);
    if(SUCCEEDED(ID3D11DeviceContext_Map(d3d8_GetD3D11Context(),(ID3D11Resource *)staging,0,D3D11_MAP_READ,0,&mapped))) {
        pixels=malloc((size_t)desc.Width*desc.Height*4);
        if(pixels) for(row=0;row<desc.Height;++row)
            memcpy(pixels+(size_t)row*desc.Width*4,(unsigned char *)mapped.pData+(size_t)row*mapped.RowPitch,desc.Width*4);
        ID3D11DeviceContext_Unmap(d3d8_GetD3D11Context(),(ID3D11Resource *)staging,0);
    }
    ID3D11Texture2D_Release(staging);
    if(pixels) {
        FILE *file=fopen(path,"wb");
        if(file) {
            unsigned char header[54]={0}, *bgra=malloc((size_t)desc.Width*desc.Height*4);
            header[0]='B'; header[1]='M';
            *(uint32_t *)(header+2)=54+desc.Width*desc.Height*4;
            *(uint32_t *)(header+10)=54; *(uint32_t *)(header+14)=40;
            *(int32_t *)(header+18)=(int32_t)desc.Width; *(int32_t *)(header+22)=-(int32_t)desc.Height;
            *(uint16_t *)(header+26)=1; *(uint16_t *)(header+28)=32;
            if(bgra) {
                for(row=0;row<desc.Width*desc.Height;++row) {
                    bgra[row*4]=pixels[row*4+2]; bgra[row*4+1]=pixels[row*4+1];
                    bgra[row*4+2]=pixels[row*4]; bgra[row*4+3]=pixels[row*4+3];
                }
                fwrite(header,1,54,file); fwrite(bgra,4,(size_t)desc.Width*desc.Height,file); free(bgra);
            }
            fclose(file);
        }
    }
    return pixels;
}

void d3d8_interpolation_validate(ID3D11Texture2D *native)
{
    static int opt_in=-1, count, done;
    unsigned char *a,*b,*mid;
    unsigned i,j, different=0, max_difference=0;
    FILE *file;
    if(opt_in<0) { const char *e=getenv("DOA3_INTERPOLATION_CHECK"); opt_in=e && *e=='1'; }
    if(!opt_in || done || !ready || frames[(current+2)%3].count<100 || ++count<20) return;
    done=1;
    if(!d3d8_interpolation_render(1)) return;
    a=diagnostic_pixels(native,"interpolation_native.bmp");
    b=diagnostic_pixels(color_target,"interpolation_replay.bmp");
    if(a && b) {
        for(i=0;i<width*height;++i) {
            unsigned worst=0;
            for(j=0;j<3;++j) {
                unsigned d=(unsigned)abs((int)a[i*4+j]-(int)b[i*4+j]);
                if(d>worst) worst=d;
            }
            if(worst>2) ++different;
            if(worst>max_difference) max_difference=worst;
        }
        file=fopen("interpolation_validation.txt","w");
        if(file) { fprintf(file,"Native versus alpha=1 replay: %u/%u pixels differ by >2; max channel delta %u\n",different,width*height,max_difference); fclose(file); }
    }
    free(a); free(b);
    if(d3d8_interpolation_render(.5f)) { mid=diagnostic_pixels(color_target,"interpolation_midpoint.bmp"); free(mid); }
}

void d3d8_interpolation_shutdown(void)
{
    unsigned f,i;
    for (f=0; f<3; ++f) {
        if (frames[f].draw) for (i=0; i<MAX_COMMANDS; ++i) {
            Draw *d=&frames[f].draw[i];
            release_state(d);
            RELEASE(d->vscb[0],ID3D11Buffer); RELEASE(d->vscb[1],ID3D11Buffer);
            RELEASE(d->pscb,ID3D11Buffer); RELEASE(d->ffcb,ID3D11Buffer);
            RELEASE(d->vb,ID3D11Buffer); RELEASE(d->ib,ID3D11Buffer);
            free(d->vertices); free(d->indices);
            free(d->vsdata[0]); free(d->vsdata[1]); free(d->psdata);
        }
        free(frames[f].draw);
        for (i=0;i<MAX_TEXTURES;++i) {
            Texture *t=&frames[f].texture[i];
            RELEASE(t->source,ID3D11ShaderResourceView); RELEASE(t->source_resource,ID3D11Resource);
            RELEASE(t->copy_srv,ID3D11ShaderResourceView); RELEASE(t->copy,ID3D11Texture2D);
        }
    }
    memset(frames,0,sizeof frames);
    release_targets(); RELEASE(replay,ID3D11DeviceContext);
    free(scratch); scratch=NULL; scratch_size=0;
    enabled=ready=0; current=0;
    memset(constants,0,sizeof constants);
    g_doa3_interpolation_status=g_doa3_interpolation_commands=g_doa3_interpolated_draws=0;
}
