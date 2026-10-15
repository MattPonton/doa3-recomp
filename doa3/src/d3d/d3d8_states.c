/**
 * D3D8 Render State → D3D11 State Object Translation
 *
 * Converts D3D8 render state values into D3D11 state objects:
 *   - Blend state (alpha blending, color write mask)
 *   - Depth-stencil state (z-test, z-write, stencil)
 *   - Rasterizer state (cull mode, fill mode)
 *   - Sampler state (texture filtering, addressing)
 *
 * State objects are cached and recreated only when dirty.
 */

#include "d3d8_internal.h"
#include "d3d8_debug_views.h"
#include "d3d8_polygon_offset.h"
#include <math.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

/* ================================================================
 * Cached D3D11 state objects
 * ================================================================ */

static ID3D11BlendState        *g_blend_state = NULL;
static ID3D11DepthStencilState *g_ds_state = NULL;
static ID3D11RasterizerState   *g_raster_state = NULL;
static ID3D11SamplerState      *g_sampler_states[4] = { NULL, NULL, NULL, NULL };

/* State objects, kept by their full descriptor. The NV2A translator applies
 * state per draw and switches between a handful of combinations; this used
 * to release and recreate the object at every switch. Entries are owned
 * here; the g_*_state pointers above point into these caches. */
#define D3D8_STATECACHE_N 64
static struct { D3D11_BLEND_DESC desc; ID3D11BlendState *state; } g_blendcache[D3D8_STATECACHE_N];
static unsigned g_blendcache_n;
static struct { D3D11_DEPTH_STENCIL_DESC desc; ID3D11DepthStencilState *state; } g_dscache[D3D8_STATECACHE_N];
static unsigned g_dscache_n;
static D3D11_BLEND_DESC g_last_blend_desc;
static D3D11_DEPTH_STENCIL_DESC g_last_ds_desc;

/* What d3d8_states_apply last bound (see d3d8_BindInvalidate). */
static unsigned g_states_epoch;
static ID3D11BlendState *g_bound_blend;
static float g_bound_blend_factor[4];
static ID3D11DepthStencilState *g_bound_ds;
static UINT g_bound_stencil_ref;
static ID3D11RasterizerState *g_bound_raster;

static D3D11_RASTERIZER_DESC g_last_raster_desc;
static int g_polygon_bias;
static float g_polygon_slope;

void d3d8_SetPolygonOffset(int enabled, float slope, float bias,
                          int depth16, float raster_scale)
{
    double units;
    g_polygon_bias = 0;
    g_polygon_slope = 0.0f;
    if (!enabled) return;
    /* NV2A bias is in guest depth units; all host targets use D24_UNORM.
     * Scaling the slope preserves its size when rendering above native
     * resolution. The vertical scale is exact for uniform scaling; a single
     * D3D slope factor approximates anisotropic widescreen stretching. */
    if (isfinite(bias)) {
        units = (double)bias * (depth16 ? 16777215.0 / 65535.0 : 1.0);
        units = round(units);
        g_polygon_bias = units >= INT_MAX ? INT_MAX :
                         units <= INT_MIN ? INT_MIN : (int)units;
    }
    if (isfinite(slope) && isfinite(raster_scale) && raster_scale > 0.0f) {
        float scaled = slope * raster_scale;
        if (isfinite(scaled)) g_polygon_slope = scaled;
    }
}

/* ================================================================
 * D3D8 → D3D11 enum translation
 * ================================================================ */

static D3D11_BLEND d3d8_to_d3d11_blend(DWORD d3d8blend)
{
    switch (d3d8blend) {
    case D3DBLEND_ZERO:         return D3D11_BLEND_ZERO;
    case D3DBLEND_ONE:          return D3D11_BLEND_ONE;
    case D3DBLEND_SRCCOLOR:     return D3D11_BLEND_SRC_COLOR;
    case D3DBLEND_INVSRCCOLOR:  return D3D11_BLEND_INV_SRC_COLOR;
    case D3DBLEND_SRCALPHA:     return D3D11_BLEND_SRC_ALPHA;
    case D3DBLEND_INVSRCALPHA:  return D3D11_BLEND_INV_SRC_ALPHA;
    case D3DBLEND_DESTALPHA:    return D3D11_BLEND_DEST_ALPHA;
    case D3DBLEND_INVDESTALPHA: return D3D11_BLEND_INV_DEST_ALPHA;
    case D3DBLEND_DESTCOLOR:    return D3D11_BLEND_DEST_COLOR;
    case D3DBLEND_INVDESTCOLOR: return D3D11_BLEND_INV_DEST_COLOR;
    case D3DBLEND_SRCALPHASAT:  return D3D11_BLEND_SRC_ALPHA_SAT;
    case D3DBLEND_BLENDFACTOR:    return D3D11_BLEND_BLEND_FACTOR;
    case D3DBLEND_INVBLENDFACTOR: return D3D11_BLEND_INV_BLEND_FACTOR;
    default:                    return D3D11_BLEND_ONE;
    }
}

/* The constant for D3DBLEND_(INV)BLENDFACTOR, set by the pgraph translator
 * from NV097_SET_BLEND_COLOR. White = the old ONE/ZERO mapping. */
static float g_blend_factor[4] = { 1, 1, 1, 1 };
void d3d8_SetBlendFactor(float r, float g, float b, float a)
{
    g_blend_factor[0] = r; g_blend_factor[1] = g;
    g_blend_factor[2] = b; g_blend_factor[3] = a;
}

static D3D11_COMPARISON_FUNC d3d8_to_d3d11_cmp(DWORD d3d8cmp)
{
    switch (d3d8cmp) {
    case D3DCMP_NEVER:        return D3D11_COMPARISON_NEVER;
    case D3DCMP_LESS:         return D3D11_COMPARISON_LESS;
    case D3DCMP_EQUAL:        return D3D11_COMPARISON_EQUAL;
    case D3DCMP_LESSEQUAL:    return D3D11_COMPARISON_LESS_EQUAL;
    case D3DCMP_GREATER:      return D3D11_COMPARISON_GREATER;
    case D3DCMP_NOTEQUAL:     return D3D11_COMPARISON_NOT_EQUAL;
    case D3DCMP_GREATEREQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
    case D3DCMP_ALWAYS:       return D3D11_COMPARISON_ALWAYS;
    default:                  return D3D11_COMPARISON_LESS_EQUAL;
    }
}

static D3D11_STENCIL_OP d3d8_to_d3d11_stencilop(DWORD op)
{
    switch (op) {
    case 1: return D3D11_STENCIL_OP_KEEP;
    case 2: return D3D11_STENCIL_OP_ZERO;
    case 3: return D3D11_STENCIL_OP_REPLACE;
    case 4: return D3D11_STENCIL_OP_INCR_SAT;
    case 5: return D3D11_STENCIL_OP_DECR_SAT;
    case 6: return D3D11_STENCIL_OP_INVERT;
    case 7: return D3D11_STENCIL_OP_INCR;
    case 8: return D3D11_STENCIL_OP_DECR;
    default: return D3D11_STENCIL_OP_KEEP;
    }
}

static D3D11_BLEND_OP d3d8_to_d3d11_blendop(DWORD op)
{
    switch (op) {
    case 1: return D3D11_BLEND_OP_ADD;
    case 2: return D3D11_BLEND_OP_SUBTRACT;
    case 3: return D3D11_BLEND_OP_REV_SUBTRACT;
    case 4: return D3D11_BLEND_OP_MIN;
    case 5: return D3D11_BLEND_OP_MAX;
    default: return D3D11_BLEND_OP_ADD;
    }
}

/* ================================================================
 * State object creation
 * ================================================================ */

static D3D11_BLEND d3d11_blend_alpha_equiv(D3D11_BLEND f)
{
    switch (f) {
    case D3D11_BLEND_SRC_COLOR:      return D3D11_BLEND_SRC_ALPHA;
    case D3D11_BLEND_INV_SRC_COLOR:  return D3D11_BLEND_INV_SRC_ALPHA;
    case D3D11_BLEND_DEST_COLOR:     return D3D11_BLEND_DEST_ALPHA;
    case D3D11_BLEND_INV_DEST_COLOR: return D3D11_BLEND_INV_DEST_ALPHA;
    case D3D11_BLEND_SRC1_COLOR:     return D3D11_BLEND_SRC1_ALPHA;
    case D3D11_BLEND_INV_SRC1_COLOR: return D3D11_BLEND_INV_SRC1_ALPHA;
    default: return f;
    }
}

static void update_blend_state(const DWORD *rs)
{
    D3D11_BLEND_DESC bd;
    HRESULT hr;
    unsigned i;

    memset(&bd, 0, sizeof(bd));
    bd.RenderTarget[0].BlendEnable = rs[D3DRS_ALPHABLENDENABLE] ? TRUE : FALSE;
    bd.RenderTarget[0].SrcBlend = d3d8_to_d3d11_blend(rs[D3DRS_SRCBLEND]);
    bd.RenderTarget[0].DestBlend = d3d8_to_d3d11_blend(rs[D3DRS_DESTBLEND]);
    bd.RenderTarget[0].BlendOp = d3d8_to_d3d11_blendop(rs[D3DRS_BLENDOP] ? rs[D3DRS_BLENDOP] : 1);
    /* D3D11 rejects the colour-only factors (SRC_COLOR, INV_SRC_COLOR,
     * DEST_COLOR, INV_DEST_COLOR) in the alpha slots: CreateBlendState
     * returned E_INVALIDARG, the state stayed NULL and the draw went out
     * unblended. DOA3's round-start/KO messages blend with
     * INVDESTCOLOR/INVSRCALPHA and came out as solid white. Use the alpha
     * counterpart of each colour factor, which is what D3D8/D3D9 do. */
    bd.RenderTarget[0].SrcBlendAlpha = d3d11_blend_alpha_equiv(bd.RenderTarget[0].SrcBlend);
    bd.RenderTarget[0].DestBlendAlpha = d3d11_blend_alpha_equiv(bd.RenderTarget[0].DestBlend);
    bd.RenderTarget[0].BlendOpAlpha = bd.RenderTarget[0].BlendOp;
    bd.RenderTarget[0].RenderTargetWriteMask = (UINT8)(rs[D3DRS_COLORWRITEENABLE] & 0x0F);

    if (g_blend_state && !memcmp(&bd, &g_last_blend_desc, sizeof bd)) return;
    for (i = 0; i < g_blendcache_n; i++)
        if (!memcmp(&bd, &g_blendcache[i].desc, sizeof bd)) {
            g_blend_state = g_blendcache[i].state;
            g_last_blend_desc = bd;
            return;
        }
    if (g_blendcache_n == D3D8_STATECACHE_N) {     /* full: start over */
        for (i = 0; i < g_blendcache_n; i++) ID3D11BlendState_Release(g_blendcache[i].state);
        g_blendcache_n = 0;
    }
    g_blend_state = NULL;
    hr = ID3D11Device_CreateBlendState(d3d8_GetD3D11Device(), &bd, &g_blend_state);
    if (FAILED(hr) || !g_blend_state) { g_blend_state = NULL; return; }
    g_blendcache[g_blendcache_n].desc = bd;
    g_blendcache[g_blendcache_n].state = g_blend_state;
    g_blendcache_n++;
    g_last_blend_desc = bd;
}

static void update_depth_stencil_state(const DWORD *rs)
{
    D3D11_DEPTH_STENCIL_DESC dsd;
    HRESULT hr;
    unsigned i;

    memset(&dsd, 0, sizeof(dsd));
    dsd.DepthEnable = rs[D3DRS_ZENABLE] ? TRUE : FALSE;
    dsd.DepthWriteMask = rs[D3DRS_ZWRITEENABLE] ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc = d3d8_to_d3d11_cmp(rs[D3DRS_ZFUNC]);

    dsd.StencilEnable = rs[D3DRS_STENCILENABLE] ? TRUE : FALSE;
    dsd.StencilReadMask = (UINT8)(rs[D3DRS_STENCILMASK] & 0xFF);
    dsd.StencilWriteMask = (UINT8)(rs[D3DRS_STENCILWRITEMASK] & 0xFF);

    dsd.FrontFace.StencilFunc = d3d8_to_d3d11_cmp(rs[D3DRS_STENCILFUNC]);
    dsd.FrontFace.StencilFailOp = d3d8_to_d3d11_stencilop(rs[D3DRS_STENCILFAIL]);
    dsd.FrontFace.StencilDepthFailOp = d3d8_to_d3d11_stencilop(rs[D3DRS_STENCILZFAIL]);
    dsd.FrontFace.StencilPassOp = d3d8_to_d3d11_stencilop(rs[D3DRS_STENCILPASS]);
    dsd.BackFace = dsd.FrontFace;

    if (g_ds_state && !memcmp(&dsd, &g_last_ds_desc, sizeof dsd)) return;
    for (i = 0; i < g_dscache_n; i++)
        if (!memcmp(&dsd, &g_dscache[i].desc, sizeof dsd)) {
            g_ds_state = g_dscache[i].state;
            g_last_ds_desc = dsd;
            return;
        }
    if (g_dscache_n == D3D8_STATECACHE_N) {        /* full: start over */
        for (i = 0; i < g_dscache_n; i++) ID3D11DepthStencilState_Release(g_dscache[i].state);
        g_dscache_n = 0;
    }
    g_ds_state = NULL;
    hr = ID3D11Device_CreateDepthStencilState(d3d8_GetD3D11Device(), &dsd, &g_ds_state);
    if (FAILED(hr) || !g_ds_state) { g_ds_state = NULL; return; }
    g_dscache[g_dscache_n].desc = dsd;
    g_dscache[g_dscache_n].state = g_ds_state;
    g_dscache_n++;
    g_last_ds_desc = dsd;
}

static void update_rasterizer_state(const DWORD *rs)
{
    /* Kept by descriptor like the blend and depth states; rs == NULL
     * releases them all (shutdown). */
    static struct { D3D11_RASTERIZER_DESC desc; ID3D11RasterizerState *state; } cache[64];
    static unsigned cache_n;
    D3D11_RASTERIZER_DESC rd;
    HRESULT hr;
    unsigned i;

    if (!rs) {
        for (i = 0; i < cache_n; i++) ID3D11RasterizerState_Release(cache[i].state);
        cache_n = 0;
        g_raster_state = NULL;
        memset(&g_last_raster_desc, 0, sizeof g_last_raster_desc);
        return;
    }
    memset(&rd, 0, sizeof(rd));

    switch (rs[D3DRS_FILLMODE]) {
    case D3DFILL_POINT:     rd.FillMode = D3D11_FILL_WIREFRAME; break;  /* D3D11 has no point fill */
    case D3DFILL_WIREFRAME: rd.FillMode = D3D11_FILL_WIREFRAME; break;
    default:                rd.FillMode = D3D11_FILL_SOLID; break;
    }

    switch (rs[D3DRS_CULLMODE]) {
    case D3DCULL_NONE: rd.CullMode = D3D11_CULL_NONE; break;
    case D3DCULL_CW:   rd.CullMode = D3D11_CULL_FRONT; break;  /* D3D8 CW = cull front in D3D11 convention */
    case D3DCULL_CCW:  rd.CullMode = D3D11_CULL_BACK; break;
    default:           rd.CullMode = D3D11_CULL_BACK; break;
    }

    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    /* On, so the pgraph can honour NV097_SET_WINDOW_CLIP (the hardware
     * scissor that confines the character-select portrait to its window).
     * The rect is set to the whole target at device creation and restored
     * after every clipped draw, so nothing outside the pgraph sees it. */
    rd.ScissorEnable = TRUE;
    rd.MultisampleEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.DepthBias = g_polygon_bias;
    rd.SlopeScaledDepthBias = g_polygon_slope;

    /* Compare the complete descriptor: changing only polygon offset must
     * invalidate the rasterizer, including the next unbiased draw. */
    if (g_raster_state && !memcmp(&rd, &g_last_raster_desc, sizeof(rd))) return;
    for (i = 0; i < cache_n; i++)
        if (!memcmp(&rd, &cache[i].desc, sizeof rd)) {
            g_raster_state = cache[i].state;
            g_last_raster_desc = rd;
            return;
        }
    if (cache_n == sizeof cache / sizeof cache[0]) {   /* full: start over */
        for (i = 0; i < cache_n; i++) ID3D11RasterizerState_Release(cache[i].state);
        cache_n = 0;
    }
    g_raster_state = NULL;
    hr = ID3D11Device_CreateRasterizerState(d3d8_GetD3D11Device(), &rd, &g_raster_state);
    if (FAILED(hr) || !g_raster_state) { g_raster_state = NULL; return; }
    cache[cache_n].desc = rd;
    cache[cache_n].state = g_raster_state;
    cache_n++;
    g_last_raster_desc = rd;
}

/* ================================================================
 * Sampler state
 * ================================================================ */

static D3D11_TEXTURE_ADDRESS_MODE d3d8_to_d3d11_address(DWORD mode)
{
    switch (mode) {
    case D3DTADDRESS_WRAP:       return D3D11_TEXTURE_ADDRESS_WRAP;
    case D3DTADDRESS_MIRROR:     return D3D11_TEXTURE_ADDRESS_MIRROR;
    case D3DTADDRESS_CLAMP:      return D3D11_TEXTURE_ADDRESS_CLAMP;
    case D3DTADDRESS_BORDER:     return D3D11_TEXTURE_ADDRESS_BORDER;
    case D3DTADDRESS_MIRRORONCE: return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
    default:                     return D3D11_TEXTURE_ADDRESS_WRAP;
    }
}

static D3D11_FILTER d3d8_to_d3d11_filter(DWORD mag, DWORD min, DWORD mip)
{
    /* Simplified filter mapping */
    BOOL mag_linear = (mag == D3DTEXF_LINEAR || mag == D3DTEXF_ANISOTROPIC);
    BOOL min_linear = (min == D3DTEXF_LINEAR || min == D3DTEXF_ANISOTROPIC);
    BOOL mip_linear = (mip == D3DTEXF_LINEAR);

    if (mag == D3DTEXF_ANISOTROPIC || min == D3DTEXF_ANISOTROPIC)
        return D3D11_FILTER_ANISOTROPIC;

    if (min_linear && mag_linear && mip_linear)
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    if (min_linear && mag_linear)
        return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    if (min_linear)
        return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    if (mag_linear)
        return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

    return D3D11_FILTER_MIN_MAG_MIP_POINT;
}

/* Sampler cache.
 *
 * This used to release and recreate a sampler state for all four stages on
 * every state apply -- and the NV2A translator applies state per draw, so on
 * the title stage that was ~3500 CreateSamplerState calls a frame for what is
 * in practice a handful of distinct samplers. Keep them keyed by the six
 * stage states that feed the descriptor. */
#define D3D8_SAMPCACHE_N 32
static struct {
    uint64_t key;
    ID3D11SamplerState *state;
} g_sampcache[D3D8_SAMPCACHE_N];
static unsigned g_sampcache_n;
static uint64_t g_sampler_key[4] = { ~0ull, ~0ull, ~0ull, ~0ull };

void d3d8_states_release_samplers(void)
{
    unsigned i;
    for (i = 0; i < D3D8_SAMPCACHE_N; i++) {
        if (g_sampcache[i].state) ID3D11SamplerState_Release(g_sampcache[i].state);
        g_sampcache[i].state = NULL;
        g_sampcache[i].key = 0;
    }
    g_sampcache_n = 0;
    for (i = 0; i < 4; i++) {
        g_sampler_states[i] = NULL;
        g_sampler_key[i] = ~0ull;
    }
}

void d3d8_states_apply_sampler(DWORD stage)
{
    const DWORD *tss;
    D3D11_SAMPLER_DESC sd;
    HRESULT hr;
    uint64_t key;
    unsigned i;
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();

    if (stage >= 4) return;
    tss = d3d8_GetTSS(stage);
    if (!tss) return;

    /* MIPMAPLODBIAS (float bits) and MAXMIPLEVEL (-> MinLOD) are sampler state too. */
    key = (uint64_t)((tss[D3DTSS_MAGFILTER] & 7u)
        | ((tss[D3DTSS_MINFILTER] & 7u) << 3)
        | ((tss[D3DTSS_MIPFILTER] & 7u) << 6)
        | ((tss[D3DTSS_ADDRESSU] & 7u) << 9)
        | ((tss[D3DTSS_ADDRESSV] & 7u) << 12)
        | ((tss[D3DTSS_MAXANISOTROPY] & 0xFFu) << 15)
        | ((tss[D3DTSS_MAXMIPLEVEL] & 0xFu) << 23))
        | ((uint64_t)tss[D3DTSS_MIPMAPLODBIAS] << 32);

    if (key == g_sampler_key[stage] && g_sampler_states[stage])
        return;                                  /* already bound on this stage */

    for (i = 0; i < g_sampcache_n; i++) {
        if (g_sampcache[i].key == key && g_sampcache[i].state) {
            g_sampler_states[stage] = g_sampcache[i].state;
            g_sampler_key[stage] = key;
            ID3D11DeviceContext_PSSetSamplers(ctx, stage, 1, &g_sampler_states[stage]);
            return;
        }
    }

    memset(&sd, 0, sizeof(sd));
    sd.Filter = d3d8_to_d3d11_filter(
        tss[D3DTSS_MAGFILTER],
        tss[D3DTSS_MINFILTER],
        tss[D3DTSS_MIPFILTER]);
    sd.AddressU = d3d8_to_d3d11_address(tss[D3DTSS_ADDRESSU] ? tss[D3DTSS_ADDRESSU] : D3DTADDRESS_WRAP);
    sd.AddressV = d3d8_to_d3d11_address(tss[D3DTSS_ADDRESSV] ? tss[D3DTSS_ADDRESSV] : D3DTADDRESS_WRAP);
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.MaxAnisotropy = tss[D3DTSS_MAXANISOTROPY] ? tss[D3DTSS_MAXANISOTROPY] : 1;
    sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.MaxLOD = D3D11_FLOAT32_MAX;
    {   DWORD bb = tss[D3DTSS_MIPMAPLODBIAS]; float bias;
        memcpy(&bias, &bb, sizeof bias);
        if (!(bias > -16.0f && bias < 16.0f)) bias = 0.0f;
        sd.MipLODBias = bias;
        sd.MinLOD = (float)(tss[D3DTSS_MAXMIPLEVEL] & 0xFu);
    }

    {
        ID3D11SamplerState *ss = NULL;
        hr = ID3D11Device_CreateSamplerState(d3d8_GetD3D11Device(), &sd, &ss);
        if (FAILED(hr) || !ss) return;
        if (g_sampcache_n < D3D8_SAMPCACHE_N) {
            g_sampcache[g_sampcache_n].key = key;
            g_sampcache[g_sampcache_n].state = ss;
            g_sampcache_n++;
        } else {
            /* Cache full: replace slot 0 rather than leaking. */
            if (g_sampcache[0].state) ID3D11SamplerState_Release(g_sampcache[0].state);
            g_sampcache[0].key = key;
            g_sampcache[0].state = ss;
            {   unsigned k;
                for (k = 0; k < 4; k++) g_sampler_key[k] = 0xFFFFFFFFu; }
        }
        g_sampler_states[stage] = ss;
        g_sampler_key[stage] = key;
        ID3D11DeviceContext_PSSetSamplers(ctx, stage, 1, &ss);
    }
}

/* ================================================================
 * Apply all states before draw call
 * ================================================================ */

HRESULT d3d8_states_init(void)
{
    /* States are created on first apply */
    return S_OK;
}

void d3d8_states_shutdown(void)
{
    unsigned i;
    d3d8_debug_views_shutdown();
    for (i = 0; i < g_blendcache_n; i++) ID3D11BlendState_Release(g_blendcache[i].state);
    for (i = 0; i < g_dscache_n; i++) ID3D11DepthStencilState_Release(g_dscache[i].state);
    g_blendcache_n = g_dscache_n = 0;
    g_blend_state = NULL;
    g_ds_state = NULL;
    update_rasterizer_state(NULL);
    for (i = 0; i < 4; i++)
        g_sampler_states[i] = NULL;   /* owned by the sampler cache */
    memset(&g_last_blend_desc, 0, sizeof g_last_blend_desc);
    memset(&g_last_ds_desc, 0, sizeof g_last_ds_desc);
    g_states_epoch = 0;
    g_bound_blend = NULL; g_bound_ds = NULL; g_bound_raster = NULL;
    d3d8_SetPolygonOffset(0, 0, 0, 0, 1);
}

void d3d8_states_apply(void)
{
    const DWORD *rs = d3d8_GetRenderStates();
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    const float *blend_factor = g_blend_factor;
    BOOL rebind;

    if (!rs || !ctx) return;

    update_blend_state(rs);
    update_depth_stencil_state(rs);
    update_rasterizer_state(rs);

    /* Bind only what changed since the last apply, unless other code bound
     * its own state in between. */
    rebind = g_states_epoch != g_d3d8_bind_epoch;
    if (g_blend_state && (rebind || g_blend_state != g_bound_blend ||
                          memcmp(blend_factor, g_bound_blend_factor, sizeof g_bound_blend_factor))) {
        ID3D11DeviceContext_OMSetBlendState(ctx, g_blend_state, blend_factor, 0xFFFFFFFF);
        g_bound_blend = g_blend_state;
        memcpy(g_bound_blend_factor, blend_factor, sizeof g_bound_blend_factor);
    }
    if (g_ds_state && (rebind || g_ds_state != g_bound_ds ||
                       (UINT)rs[D3DRS_STENCILREF] != g_bound_stencil_ref)) {
        ID3D11DeviceContext_OMSetDepthStencilState(ctx, g_ds_state, rs[D3DRS_STENCILREF]);
        g_bound_ds = g_ds_state;
        g_bound_stencil_ref = (UINT)rs[D3DRS_STENCILREF];
    }
    if (g_raster_state && (rebind || g_raster_state != g_bound_raster)) {
        ID3D11DeviceContext_RSSetState(ctx, g_raster_state);
        g_bound_raster = g_raster_state;
    }
    if (rebind) {
        unsigned k;
        for (k = 0; k < 4; k++) g_sampler_key[k] = ~0ull;   /* rebind the samplers too */
    }
    g_states_epoch = g_d3d8_bind_epoch;

    /* Apply samplers for all 4 texture stages */
    {
        DWORD s;
        for (s = 0; s < 4; s++)
            d3d8_states_apply_sampler(s);
    }
    d3d8_debug_views_apply();
}
