/* Exercise the actual frame-copy code on D3D11 WARP without a game window.
 * CMake extracts the implementation to reach its private snapshot without
 * unrelated game/UI dependencies. No frame-copy logic is duplicated here. */
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
static struct {
    ID3D11Device *d3d11_device;
    ID3D11DeviceContext *d3d11_context;
    ID3D11Texture2D *default_tex;
    ID3D11RenderTargetView *default_rtv;
} g_device_state;
static int g_off_active;
#include "frame_history_impl.h"

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #c); exit(1); } } while (0)

static int frame_is(BYTE r, BYTE g, BYTE b)
{
    ID3D11ShaderResourceView *srv = NULL;
    ID3D11Resource *resource = NULL;
    ID3D11Texture2D *readback = NULL;
    D3D11_TEXTURE2D_DESC td;
    D3D11_MAPPED_SUBRESOURCE mapped;
    int match;
    CHECK(d3d8_BindPrevFrame(0));
    ID3D11DeviceContext_PSGetShaderResources(g_device_state.d3d11_context, 0, 1, &srv);
    CHECK(srv != NULL);
    ID3D11ShaderResourceView_GetResource(srv, &resource);
    ID3D11Texture2D_GetDesc((ID3D11Texture2D *)resource, &td);
    td.Usage = D3D11_USAGE_STAGING;
    td.BindFlags = 0;
    td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device, &td, NULL, &readback)));
    ID3D11DeviceContext_CopyResource(g_device_state.d3d11_context, (ID3D11Resource *)readback, resource);
    CHECK(SUCCEEDED(ID3D11DeviceContext_Map(g_device_state.d3d11_context, (ID3D11Resource *)readback, 0, D3D11_MAP_READ, 0, &mapped)));
    BYTE *pixel = (BYTE *)mapped.pData;
    match = pixel[0] == b && pixel[1] == g && pixel[2] == r;
    if (!match) fprintf(stderr, "Expected RGB %u/%u/%u, got %u/%u/%u\n", r, g, b, pixel[2], pixel[1], pixel[0]);
    ID3D11DeviceContext_Unmap(g_device_state.d3d11_context, (ID3D11Resource *)readback, 0);
    ID3D11Texture2D_Release(readback);
    ID3D11Resource_Release(resource);
    ID3D11ShaderResourceView_Release(srv);
    return match;
}

/* Blend history onto the current scene on WARP. Repeated frames must retain
 * older after-images, not replace them with a clean pre-overlay scene. */
static void blend_history(void)
{
    static ID3D11VertexShader *vs;
    static ID3D11PixelShader *ps;
    static ID3D11BlendState *blend;
    static ID3D11RasterizerState *raster;
    ID3D11Device *device=g_device_state.d3d11_device;
    ID3D11DeviceContext *ctx=g_device_state.d3d11_context;
    D3D11_VIEWPORT viewport={0,0,16,16,0,1};
    if(!vs) {
        ID3DBlob *v=NULL,*p=NULL,*error=NULL;
        D3D11_BLEND_DESC bd={0};
        D3D11_RASTERIZER_DESC rd={0};
        const char *shader="Texture2D history:register(t0);"
            "float4 vs(uint id:SV_VertexID):SV_POSITION{"
            "return float4(id==2?3:-1,id==1?3:-1,0,1);}"
            "float4 ps(float4 p:SV_POSITION):SV_TARGET{"
            "return float4(history.Load(int3(p.xy,0)).rgb,0.75);}";
        CHECK(SUCCEEDED(D3DCompile(shader,strlen(shader),NULL,NULL,NULL,"vs","vs_4_0",0,0,&v,&error)));
        CHECK(SUCCEEDED(D3DCompile(shader,strlen(shader),NULL,NULL,NULL,"ps","ps_4_0",0,0,&p,&error)));
        CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(v),ID3D10Blob_GetBufferSize(v),NULL,&vs)));
        CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(device,ID3D10Blob_GetBufferPointer(p),ID3D10Blob_GetBufferSize(p),NULL,&ps)));
        ID3D10Blob_Release(v); ID3D10Blob_Release(p);
        bd.RenderTarget[0].BlendEnable=TRUE;
        bd.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
        bd.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
        bd.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
        bd.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
        bd.RenderTarget[0].BlendOp=bd.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
        bd.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
        CHECK(SUCCEEDED(ID3D11Device_CreateBlendState(device,&bd,&blend)));
        rd.FillMode=D3D11_FILL_SOLID; rd.CullMode=D3D11_CULL_NONE;
        CHECK(SUCCEEDED(ID3D11Device_CreateRasterizerState(device,&rd,&raster)));
    }
    CHECK(d3d8_BindPrevFrame(0));
    ID3D11DeviceContext_OMSetRenderTargets(ctx,1,&g_device_state.default_rtv,NULL);
    ID3D11DeviceContext_OMSetBlendState(ctx,blend,NULL,~0u);
    ID3D11DeviceContext_RSSetState(ctx,raster);
    ID3D11DeviceContext_RSSetViewports(ctx,1,&viewport);
    ID3D11DeviceContext_VSSetShader(ctx,vs,NULL,0);
    ID3D11DeviceContext_PSSetShader(ctx,ps,NULL,0);
    ID3D11DeviceContext_IASetInputLayout(ctx,NULL);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_Draw(ctx,3,0);
}

int main(void)
{
    D3D11_TEXTURE2D_DESC td = {0};
    const FLOAT red[4] = {1, 0, 0, 1}, green[4] = {0, 1, 0, 1}, blue[4] = {0, 0, 1, 1};
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0,
          D3D11_SDK_VERSION, &g_device_state.d3d11_device, NULL, &g_device_state.d3d11_context)));
    td.Width = td.Height = 16;
    td.MipLevels = td.ArraySize = td.SampleDesc.Count = 1;
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device, &td, NULL, &g_device_state.default_tex)));
    CHECK(SUCCEEDED(ID3D11Device_CreateRenderTargetView(g_device_state.d3d11_device,
          (ID3D11Resource *)g_device_state.default_tex, NULL, &g_device_state.default_rtv)));

    /* An effect samples a red frame, then is unused for longer than the old
     * 120-present retention window. The next reset must capture green. */
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, red);
    CHECK(d3d8_BindPrevFrame(0));
    for (int i = 0; i < 121; ++i) d3d8_keep_prev_frame();
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, green);
    d3d8_keep_prev_frame();
    CHECK(frame_is(0, 255, 0));

    /* A new frame has already been cleared: sampling must still return
     * the completed green frame, not the blue current render target. */
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, blue);
    CHECK(frame_is(0, 255, 0));

    /* Changes during the frame do not replace displayed history until the
     * native present. Retain the completed frame, including the effect. */
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, red);
    CHECK(frame_is(0, 255, 0));
    d3d8_keep_prev_frame();
    CHECK(frame_is(255, 0, 0));
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, blue);
    blend_history();
    CHECK(frame_is(255, 0, 0)); /* The draw cannot replace its own input. */
    d3d8_keep_prev_frame();
    CHECK(frame_is(191, 0, 64));
    {
        const FLOAT black[4]={0,0,0,1};
        ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, black);
        blend_history();
        d3d8_keep_prev_frame();
        CHECK(frame_is(143, 0, 48)); /* Red persists from two frames earlier. */
    }
    puts("Frame history stays fresh after inactivity, stays immutable during draws, and retains accumulated after-images.");
    return 0;
}
