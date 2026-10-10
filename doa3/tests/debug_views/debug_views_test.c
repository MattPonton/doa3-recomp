#include "d3d8_internal.h"
#include "d3d8_debug_views.h"
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
#define HR(c) CHECK(SUCCEEDED(c))
static ID3D11Device *device;
static ID3D11DeviceContext *ctx;
static ID3D11PixelShader *normal_ps;
static ID3D11RasterizerState *normal_rs;
static ID3D11RenderTargetView *rtv;
static ID3D11Texture2D *target, *readback;
static DWORD states[256];
static int offscreen;
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return ctx; }
const DWORD *d3d8_GetRenderStates(void) { return states; }
int d3d8_OffscreenTargetActive(void) { return offscreen; }

static ID3DBlob *compile(const char *text, const char *profile)
{
    ID3DBlob *code = NULL, *errors = NULL;
    HRESULT hr = D3DCompile(text, strlen(text), "test", NULL, NULL, "main", profile, 0, 0, &code, &errors);
    if (errors) {
        if (FAILED(hr)) fprintf(stderr, "%s\n", (char *)ID3D10Blob_GetBufferPointer(errors));
        ID3D10Blob_Release(errors);
    }
    CHECK(SUCCEEDED(hr)); return code;
}

static int draw(unsigned char pixel[4])
{
    const float black[4] = {0,0,0,0};
    D3D11_MAPPED_SUBRESOURCE mapped;
    int covered = 0;
    ID3D11DeviceContext_ClearRenderTargetView(ctx, rtv, black);
    /* Same per-draw rebinding contract as the game's prepare-draw path. */
    ID3D11DeviceContext_PSSetShader(ctx, normal_ps, NULL, 0);
    ID3D11DeviceContext_RSSetState(ctx, normal_rs);
    ID3D11DeviceContext_OMSetBlendState(ctx, NULL, NULL, 0xFFFFFFFF);
    d3d8_debug_views_apply();
    CHECK(!d3d8_debug_view_error()[0]);
    ID3D11DeviceContext_Draw(ctx, 3, 0);
    ID3D11DeviceContext_CopyResource(ctx, (ID3D11Resource *)readback, (ID3D11Resource *)target);
    HR(ID3D11DeviceContext_Map(ctx, (ID3D11Resource *)readback, 0, D3D11_MAP_READ, 0, &mapped));
    memcpy(pixel, (char *)mapped.pData + 20*mapped.RowPitch + 8*4, 4);
    for (int y = 0; y < 32; ++y) for (int x = 0; x < 32; ++x) {
        unsigned char *p = (unsigned char *)mapped.pData + y*mapped.RowPitch + x*4;
        if (p[0] || p[1] || p[2]) ++covered;
    }
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource *)readback, 0);
    return covered;
}

int main(void)
{
    ID3DBlob *code;
    ID3D11VertexShader *vs;
    D3D11_TEXTURE2D_DESC td = {0};
    D3D11_RASTERIZER_DESC rd = {0};
    D3D11_VIEWPORT vp = {0,0,32,32,0,1};
    D3D11_RECT rect = {0,0,32,32};
    unsigned char p[4];
    HR(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device, NULL, &ctx));
    td.Width = td.Height = 32; td.MipLevels = td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; td.SampleDesc.Count = 1;
    td.BindFlags = D3D11_BIND_RENDER_TARGET;
    HR(ID3D11Device_CreateTexture2D(device, &td, NULL, &target));
    HR(ID3D11Device_CreateRenderTargetView(device, (ID3D11Resource *)target, NULL, &rtv));
    td.BindFlags = 0; td.Usage = D3D11_USAGE_STAGING; td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    HR(ID3D11Device_CreateTexture2D(device, &td, NULL, &readback));
    code = compile("struct O { float4 pos:SV_POSITION; float4 color:COLOR0; float2 uv:TEXCOORD0; };"
        "O main(uint id:SV_VertexID) { O o; float2 xy[3]={float2(-.9,-.9),float2(-.9,.9),float2(.9,-.9)};"
        "o.pos=float4(xy[id]*10,5,10); o.color=float4(.25,.5,.75,.1); o.uv=float2(.3,.6); return o; }", "vs_4_0");
    HR(ID3D11Device_CreateVertexShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &vs));
    ID3D10Blob_Release(code);
    code = compile("float4 main():SV_TARGET { return float4(1,0,1,1); }", "ps_4_0");
    HR(ID3D11Device_CreatePixelShader(device, ID3D10Blob_GetBufferPointer(code), ID3D10Blob_GetBufferSize(code), NULL, &normal_ps));
    ID3D10Blob_Release(code);
    rd.FillMode = D3D11_FILL_SOLID; rd.CullMode = D3D11_CULL_NONE;
    rd.DepthClipEnable = rd.ScissorEnable = TRUE;
    HR(ID3D11Device_CreateRasterizerState(device, &rd, &normal_rs));
    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rtv, NULL);
    ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
    ID3D11DeviceContext_RSSetScissorRects(ctx, 1, &rect);
    ID3D11DeviceContext_VSSetShader(ctx, vs, NULL, 0);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    states[D3DRS_ZENABLE] = 1; states[D3DRS_COLORWRITEENABLE] = 15;
    int solid = draw(p);
    CHECK(solid > 300 && p[0] == 255 && p[1] == 0 && p[2] == 255);
    d3d8_debug_view_set(D3D8_DEBUG_VERTEX_COLOR); draw(p);
    CHECK(abs(p[0]-64) <= 1 && abs(p[1]-128) <= 1 && abs(p[2]-191) <= 1 && p[3] == 255);
    d3d8_debug_view_set(D3D8_DEBUG_UV); draw(p);
    CHECK(abs(p[0]-77) <= 1 && abs(p[1]-153) <= 1 && abs(p[2]-51) <= 1);
    d3d8_debug_distance_set(20); d3d8_debug_view_set(D3D8_DEBUG_DISTANCE); draw(p);
    CHECK(abs(p[0]-128) <= 1 && p[0] == p[1] && p[1] == p[2]);
    d3d8_debug_distance_set(40); draw(p);
    CHECK(abs(p[0]-64) <= 1);
    draw(p); CHECK(abs(p[0]-64) <= 1); /* unchanged private CB remains valid */
    d3d8_debug_views_shutdown(); draw(p);
    CHECK(abs(p[0]-64) <= 1); /* same settings, newly created private CB */
    d3d8_debug_view_set(D3D8_DEBUG_WIREFRAME);
    int edges = draw(p); CHECK(edges > 30 && edges < solid / 2);
    offscreen = 1; draw(p); CHECK(p[0] == 255 && p[1] == 0 && p[2] == 255);
    offscreen = 0; states[D3DRS_ZENABLE] = 0; draw(p); CHECK(p[0] == 255 && p[1] == 0);
    states[D3DRS_ZENABLE] = 1; states[D3DRS_COLORWRITEENABLE] = 8;
    draw(p); CHECK(p[0] == 255 && p[1] == 0);
    states[D3DRS_COLORWRITEENABLE] = 15;
    d3d8_debug_views_reset(); draw(p); CHECK(p[0] == 255 && p[1] == 0);
    CHECK(d3d8_debug_view_get() == 0 && d3d8_debug_distance_get() == 2000);
    ID3D11RasterizerState_Release(normal_rs);
    rd.CullMode = D3D11_CULL_BACK;
    HR(ID3D11Device_CreateRasterizerState(device, &rd, &normal_rs));
    d3d8_debug_backfaces_set(1); draw(p);
    ID3D11RasterizerState *active;
    ID3D11DeviceContext_RSGetState(ctx, &active);
    ID3D11RasterizerState_GetDesc(active, &rd); ID3D11RasterizerState_Release(active);
    CHECK(rd.CullMode == D3D11_CULL_NONE && rd.FillMode == D3D11_FILL_SOLID && rd.ScissorEnable && rd.DepthClipEnable);
    d3d8_debug_views_reset(); draw(p);
    ID3D11DeviceContext_RSGetState(ctx, &active);
    ID3D11RasterizerState_GetDesc(active, &rd); ID3D11RasterizerState_Release(active);
    CHECK(rd.CullMode == D3D11_CULL_BACK);
    d3d8_debug_views_shutdown(); ID3D11DeviceContext_ClearState(ctx);
    ID3D11VertexShader_Release(vs); ID3D11PixelShader_Release(normal_ps);
    ID3D11RasterizerState_Release(normal_rs); ID3D11RenderTargetView_Release(rtv);
    ID3D11Texture2D_Release(target); ID3D11Texture2D_Release(readback);
    ID3D11DeviceContext_Release(ctx); ID3D11Device_Release(device);
    puts("WARP: wireframe coverage, vertex colors, UVs, view distance, pass exclusions, backfaces, and reset passed.");
    return 0;
}
