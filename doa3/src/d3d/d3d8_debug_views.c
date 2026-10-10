#include "d3d8_internal.h"
#include "d3d8_debug_views.h"
#include <d3dcompiler.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Host-only draw overrides. Never edit guest states, textures, or offscreen
 * reflection/mask targets. The normal prepare-draw path binds its own PS and
 * states before every call, so a skipped draw or Normal restores them directly.
 */
static int s_mode, s_backfaces, s_failed;
static float s_distance = 2000;
static int s_uploaded_mode = -1;
static float s_uploaded_distance;
static ID3D11PixelShader *s_shader;
static ID3D11Buffer *s_constants;
static ID3D11BlendState *s_opaque[16];
static struct {
    D3D11_RASTERIZER_DESC desc;
    ID3D11RasterizerState *state;
} s_raster[32];
static unsigned s_next_raster;
static char s_error[128];

static const char s_source[] =
    "cbuffer DebugView : register(b7) { uint mode; float distanceRange; float2 pad; };\n"
    "struct Input { float4 pos:SV_POSITION; float4 color:COLOR0; float2 uv:TEXCOORD0; };\n"
    "float4 main(Input i):SV_TARGET {\n"
    "  if (mode == 1) return float4(0.85,1.0,0.9,1);\n"
    "  if (mode == 2) return float4(saturate(i.color.rgb),1);\n"
    "  if (mode == 3) {\n"
    "    float2 uv = frac(i.uv);\n"
    "    float check = fmod(floor(uv.x*8) + floor(uv.y*8),2);\n"
    "    return float4(uv,0.2 + check*0.6,1);\n"
    "  }\n"
    /* D3D11 PS SV_Position.w retains clip W (unlike GL's reciprocal W).
     * The native perspective projection makes this view-axis distance. */
    "  float d = saturate(abs(i.pos.w) / distanceRange);\n"
    "  return float4(d,d,d,1);\n"
    "}\n";

int d3d8_debug_view_get(void) { return s_mode; }
void d3d8_debug_view_set(int mode) { s_mode = mode >= 0 && mode < D3D8_DEBUG_VIEW_COUNT ? mode : 0; }
int d3d8_debug_backfaces_get(void) { return s_backfaces; }
void d3d8_debug_backfaces_set(int enabled) { s_backfaces = !!enabled; }
float d3d8_debug_distance_get(void) { return s_distance; }
void d3d8_debug_distance_set(float value) { if (isfinite(value)) s_distance = fmaxf(10, fminf(100000, value)); }
const char *d3d8_debug_view_error(void) { return s_error; }
void d3d8_debug_views_reset(void) { s_mode = s_backfaces = 0; s_distance = 2000; }

static void failed(HRESULT hr)
{
    snprintf(s_error, sizeof(s_error), "Debug rendering unavailable (0x%08lX).", (unsigned long)hr);
    fprintf(stderr, "[DEBUG VIEW] %s\n", s_error);
    s_failed = 1;
}

static int prepare_shader(ID3D11Device *device)
{
    ID3DBlob *code = NULL, *errors = NULL;
    HRESULT hr;
    D3D11_BUFFER_DESC bd = {0};
    if (s_shader && s_constants) return 1;
    hr = D3DCompile(s_source, sizeof(s_source)-1, "debug_views", NULL, NULL,
        "main", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &code, &errors);
    if (errors) ID3D10Blob_Release(errors);
    if (FAILED(hr)) { failed(hr); return 0; }
    hr = ID3D11Device_CreatePixelShader(device, ID3D10Blob_GetBufferPointer(code),
        ID3D10Blob_GetBufferSize(code), NULL, &s_shader);
    ID3D10Blob_Release(code);
    if (FAILED(hr)) { failed(hr); return 0; }
    bd.ByteWidth = 16; bd.Usage = D3D11_USAGE_DEFAULT; bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = ID3D11Device_CreateBuffer(device, &bd, NULL, &s_constants);
    if (FAILED(hr)) { failed(hr); return 0; }
    return 1;
}

void d3d8_debug_views_apply(void)
{
    const DWORD *rs;
    ID3D11Device *device;
    ID3D11DeviceContext *ctx;
    ID3D11RasterizerState *original = NULL, *raster = NULL;
    D3D11_RASTERIZER_DESC desc = {0};
    HRESULT hr;
    unsigned slot;
    if ((!s_mode && !s_backfaces) || s_failed || d3d8_OffscreenTargetActive()) return;
    rs = d3d8_GetRenderStates();
    /* Exclude 2D overlays and depth/stencil/alpha-only mask passes. */
    if (!rs || !rs[D3DRS_ZENABLE] || !(rs[D3DRS_COLORWRITEENABLE] & 7)) return;
    device = d3d8_GetD3D11Device(); ctx = d3d8_GetD3D11Context();
    if (!device || !ctx) return;
    if (s_mode && !prepare_shader(device)) return;
    /* Color/UV/distance views keep the normal rasterizer unchanged. */
    if (s_mode == D3D8_DEBUG_WIREFRAME || s_backfaces) {
        ID3D11DeviceContext_RSGetState(ctx, &original);
        if (!original) return; /* Normal state preparation always supplies this. */
        ID3D11RasterizerState_GetDesc(original, &desc);
        ID3D11RasterizerState_Release(original);
        if (s_mode == D3D8_DEBUG_WIREFRAME) desc.FillMode = D3D11_FILL_WIREFRAME;
        if (s_backfaces) desc.CullMode = D3D11_CULL_NONE;
        for (slot = 0; slot < 32; ++slot) {
            if (s_raster[slot].state && !memcmp(&s_raster[slot].desc, &desc, sizeof(desc))) {
                raster = s_raster[slot].state; break;
            }
        }
        if (!raster) {
            slot = s_next_raster++ % 32;
            hr = ID3D11Device_CreateRasterizerState(device, &desc, &raster);
            if (FAILED(hr)) { failed(hr); return; }
            if (s_raster[slot].state) ID3D11RasterizerState_Release(s_raster[slot].state);
            s_raster[slot].desc = desc; s_raster[slot].state = raster;
        }
    }
    if (s_mode) {
        unsigned mask = rs[D3DRS_COLORWRITEENABLE] & 15;
        struct { unsigned mode; float range, pad[2]; } constants = {(unsigned)s_mode, s_distance, {0,0}};
        if (!s_opaque[mask]) {
            D3D11_BLEND_DESC blend = {0};
            blend.RenderTarget[0].RenderTargetWriteMask = (UINT8)mask;
            hr = ID3D11Device_CreateBlendState(device, &blend, &s_opaque[mask]);
            if (FAILED(hr)) { failed(hr); return; }
        }
        /* This private buffer changes only when the user changes a setting.
         * Re-uploading for every surface can add in-flight resource copies. */
        if (s_uploaded_mode != s_mode || s_uploaded_distance != s_distance) {
            ID3D11DeviceContext_UpdateSubresource(ctx, (ID3D11Resource *)s_constants, 0, NULL, &constants, 0, 0);
            s_uploaded_mode = s_mode; s_uploaded_distance = s_distance;
        }
        ID3D11DeviceContext_PSSetConstantBuffers(ctx, 7, 1, &s_constants);
        ID3D11DeviceContext_PSSetShader(ctx, s_shader, NULL, 0);
        ID3D11DeviceContext_OMSetBlendState(ctx, s_opaque[mask], NULL, 0xFFFFFFFF);
    }
    if (raster) ID3D11DeviceContext_RSSetState(ctx, raster);
}

void d3d8_debug_views_shutdown(void)
{
    if (s_shader) { ID3D11PixelShader_Release(s_shader); s_shader = NULL; }
    if (s_constants) { ID3D11Buffer_Release(s_constants); s_constants = NULL; }
    for (int i = 0; i < 16; ++i) if (s_opaque[i]) {
        ID3D11BlendState_Release(s_opaque[i]); s_opaque[i] = NULL;
    }
    for (int i = 0; i < 32; ++i) if (s_raster[i].state) {
        ID3D11RasterizerState_Release(s_raster[i].state); s_raster[i].state = NULL;
    }
    s_next_raster = 0; s_failed = 0; s_error[0] = 0;
    s_uploaded_mode = -1;
}
