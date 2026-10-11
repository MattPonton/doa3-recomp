#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "nv2a_regs.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
#define HR(x) CHECK(SUCCEEDED(x))
enum { D3DRS_CULLMODE, D3DRS_FILLMODE };
enum { D3DFILL_POINT = 1, D3DFILL_WIREFRAME, D3DFILL_SOLID };
enum { D3DCULL_NONE = 1, D3DCULL_CW, D3DCULL_CCW };
static ID3D11Device *device;
static ID3D11DeviceContext *ctx;
static ID3D11RasterizerState *g_raster_state;
static ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
#include "offset.h"
#include "raster.h"

static struct { uint32_t surface_clip_v, surface_fmt, draw_mode; } g_pg;
static uint32_t regs[0x800];
#define NVREG(m) regs[(m) >> 2]
static unsigned height;
static unsigned d3d8_GetBackbufferHeight(void) { return height; }
static float u2f(uint32_t x) { float v; memcpy(&v, &x, 4); return v; }
static void reg_float(unsigned reg, float f) { memcpy(&NVREG(reg), &f, 4); }
#include "pgraph.h"

static ID3D11Buffer *constants;
static ID3D11VertexShader *vs;
static ID3D11PixelShader *ps;
static DWORD rs[2] = { D3DCULL_NONE, D3DFILL_SOLID };
static void draw(float delta, float gradient, int green)
{
    float values[8] = { delta, gradient, 0, 0, green ? 0.0f : 1.0f, green ? 1.0f : 0.0f, 0, 1 };
    ID3D11DeviceContext_UpdateSubresource(ctx, (ID3D11Resource *)constants, 0, NULL, values, 0, 0);
    update_rasterizer_state(rs);
    CHECK(g_raster_state);
    ID3D11DeviceContext_RSSetState(ctx, g_raster_state);
    ID3D11DeviceContext_Draw(ctx, 3, 0);
}

/* Actual D24 rasterization of two almost-coplanar surfaces. This catches
 * bias sign, units, scale compensation, state invalidation and disable. */
static void render_case(unsigned scale, float slope, float bias, float gap, float gradient, int expected_green)
{
    unsigned size = 64 * scale;
    D3D11_TEXTURE2D_DESC td = {0};
    ID3D11Texture2D *color, *depth, *readback;
    ID3D11RenderTargetView *rt;
    ID3D11DepthStencilView *ds;
    D3D11_VIEWPORT vp = {0, 0, (float)size, (float)size, 0, 1};
    D3D11_RECT scissor = {0, 0, (LONG)size, (LONG)size};
    D3D11_MAPPED_SUBRESOURCE map;
    float black[4] = {0,0,0,1};
    td.Width = td.Height = size; td.MipLevels = td.ArraySize = 1;
    td.SampleDesc.Count = 1; td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET;
    HR(ID3D11Device_CreateTexture2D(device, &td, NULL, &color));
    HR(ID3D11Device_CreateRenderTargetView(device, (ID3D11Resource *)color, NULL, &rt));
    td.BindFlags = 0; td.Usage = D3D11_USAGE_STAGING; td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    HR(ID3D11Device_CreateTexture2D(device, &td, NULL, &readback));
    td.Usage = D3D11_USAGE_DEFAULT; td.CPUAccessFlags = 0;
    td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    HR(ID3D11Device_CreateTexture2D(device, &td, NULL, &depth));
    HR(ID3D11Device_CreateDepthStencilView(device, (ID3D11Resource *)depth, NULL, &ds));
    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rt, ds);
    ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
    ID3D11DeviceContext_RSSetScissorRects(ctx, 1, &scissor);
    ID3D11DeviceContext_ClearRenderTargetView(ctx, rt, black);
    ID3D11DeviceContext_ClearDepthStencilView(ctx, ds, D3D11_CLEAR_DEPTH, 1, 0);
    height = size; g_pg.surface_clip_v = 64 << 16; g_pg.surface_fmt = 0x20; g_pg.draw_mode = 6;
    NVREG(NV097_SET_POLY_OFFSET_FILL_ENABLE) = 0;
    nv_apply_polygon_offset();
    draw(0, gradient, 0);
    NVREG(NV097_SET_POLY_OFFSET_FILL_ENABLE) = 1;
    reg_float(NV097_SET_POLYGON_OFFSET_SCALE_FACTOR, slope);
    reg_float(NV097_SET_POLYGON_OFFSET_BIAS, bias);
    nv_apply_polygon_offset();
    draw(gap, gradient, 1);
    /* A subsequent unbiased draw must not inherit the previous offset. */
    NVREG(NV097_SET_POLY_OFFSET_FILL_ENABLE) = 0;
    nv_apply_polygon_offset();
    draw(gap + 0.00001f, gradient, 0);
    ID3D11DeviceContext_CopyResource(ctx, (ID3D11Resource *)readback, (ID3D11Resource *)color);
    HR(ID3D11DeviceContext_Map(ctx, (ID3D11Resource *)readback, 0, D3D11_MAP_READ, 0, &map));
    for (unsigned y = size/4; y < 3*size/4; y++) {
        unsigned char *row = (unsigned char *)map.pData + y * map.RowPitch;
        for (unsigned x = size/4; x < 3*size/4; x++) {
            CHECK(row[x*4 + (expected_green ? 1 : 0)] == 255);
            CHECK(row[x*4 + (expected_green ? 0 : 1)] == 0);
        }
    }
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource *)readback, 0);
    ID3D11DeviceContext_OMSetRenderTargets(ctx, 0, NULL, NULL);
    ID3D11DepthStencilView_Release(ds); ID3D11RenderTargetView_Release(rt);
    ID3D11Texture2D_Release(depth); ID3D11Texture2D_Release(color); ID3D11Texture2D_Release(readback);
}

int main(void)
{
    ID3DBlob *code = NULL, *errors = NULL;
    const char *shader =
        "cbuffer C:register(b0){float4 geom;float4 color;}"
        "float4 VS(uint id:SV_VertexID):SV_Position {"
        "float2 p=id==0?float2(-1,-1):id==1?float2(-1,3):float2(3,-1);"
        "return float4(p,0.4+geom.x+geom.y*p.x,1);}"
        "float4 PS():SV_Target{return color;}";
    D3D11_BUFFER_DESC bd = {0};
    HR(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device, NULL, &ctx));
    HR(D3DCompile(shader, strlen(shader), NULL, NULL, NULL, "VS", "vs_4_0", 0, 0, &code, &errors));
    HR(ID3D11Device_CreateVertexShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &vs));
    ID3D10Blob_Release(code);
    HR(D3DCompile(shader, strlen(shader), NULL, NULL, NULL, "PS", "ps_4_0", 0, 0, &code, &errors));
    HR(ID3D11Device_CreatePixelShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &ps));
    ID3D10Blob_Release(code);
    bd.ByteWidth = 32; bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HR(ID3D11Device_CreateBuffer(device, &bd, NULL, &constants));
    ID3D11DeviceContext_VSSetShader(ctx, vs, NULL, 0); ID3D11DeviceContext_PSSetShader(ctx, ps, NULL, 0);
    ID3D11DeviceContext_VSSetConstantBuffers(ctx, 0, 1, &constants);
    ID3D11DeviceContext_PSSetConstantBuffers(ctx, 0, 1, &constants);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    for (unsigned scale = 1; scale <= 3; scale += 2) {
        render_case(scale, 0, 0, 1.0f/16777215.0f, 0, 0);
        render_case(scale, 0, -2, 1.0f/16777215.0f, 0, 1);
        render_case(scale, 0, 2, -1.0f/16777215.0f, 0, 0);
        render_case(scale, -0.5f, -2, 0.001f, 0.1f, 1); /* Tao settings */
        render_case(scale, 0.5f, 2, -0.001f, 0.1f, 0);
    }
    d3d8_SetPolygonOffset(1, -0.5f, -2, 1, 3);
    CHECK(g_polygon_bias == -512 && g_polygon_slope == -1.5f);
    d3d8_SetPolygonOffset(1, NAN, INFINITY, 0, 1);
    CHECK(g_polygon_bias == 0 && g_polygon_slope == 0);
    NVREG(NV097_SET_POLY_OFFSET_FILL_ENABLE) = 1;
    for (unsigned mode = 1; mode <= 4; mode++) {
        g_pg.draw_mode = mode; nv_apply_polygon_offset();
        CHECK(g_polygon_bias == 0 && g_polygon_slope == 0);
    }
    g_pg.draw_mode = 6;
    NVREG(NV097_SET_POLY_OFFSET_FILL_ENABLE) = 0;
    NVREG(NV097_SET_POLY_OFFSET_POINT_ENABLE) = 1;
    NVREG(NV097_SET_POLY_OFFSET_LINE_ENABLE) = 1;
    nv_apply_polygon_offset();
    CHECK(g_polygon_bias == 0 && g_polygon_slope == 0);
    ID3D11DeviceContext_ClearState(ctx);
    ID3D11RasterizerState_Release(g_raster_state);
    ID3D11Buffer_Release(constants); ID3D11VertexShader_Release(vs); ID3D11PixelShader_Release(ps);
    ID3D11DeviceContext_Release(ctx); ID3D11Device_Release(device);
    puts("Polygon offset: D24 overlap, slope, native/3x, disable, D16 conversion and primitive gating passed.");
    return 0;
}
