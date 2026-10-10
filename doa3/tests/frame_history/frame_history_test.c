/* Exercise the actual frame-copy code on D3D11 WARP without a game window.
 * CMake extracts the implementation to reach its private snapshot without
 * unrelated game/UI dependencies. No frame-copy logic is duplicated here. */
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
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

    /* Preserve the existing after-image behavior: retain the red scene
     * captured before HUD/overlay draws even if presentation is blue. */
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, red);
    d3d8_SnapshotSceneForAfterImage();
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, g_device_state.default_rtv, blue);
    d3d8_keep_prev_frame();
    CHECK(frame_is(255, 0, 0));
    puts("Frame history stays current after inactivity; pre-overlay capture preserved.");
    return 0;
}
