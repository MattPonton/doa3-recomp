#define COBJMACROS
#include "d3d8_replay_images.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#ifdef DOA3_FULL_PIPELINE_FIXTURE
#include "d3d8_internal.h"
#include "pipeline.generated.inc"
#else
static struct {
    ID3D11Device *d3d11_device;
    ID3D11DeviceContext *d3d11_context;
    ID3D11Texture2D *default_tex, *default_depth;
    ID3D11RenderTargetView *default_rtv;
    ID3D11DepthStencilView *default_dsv;
    UINT width, height;
} g_device_state;
static int g_off_active;
static void *g_cur_textures[4];
#include "render_images.generated.inc"
#endif

static void fill(ID3D11RenderTargetView *rtv, ID3D11DepthStencilView *dsv, float v, UINT stencil)
{
    FLOAT color[4] = {v, v / 2, 1 - v, 1};
    ID3D11DeviceContext_ClearRenderTargetView(g_device_state.d3d11_context, rtv, color);
    ID3D11DeviceContext_ClearDepthStencilView(g_device_state.d3d11_context, dsv,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, v, (UINT8)stencil);
}
static uint64_t pixels(ID3D11Texture2D *texture)
{
    ID3D11Texture2D *staging = NULL;
    D3D11_TEXTURE2D_DESC d;
    D3D11_MAPPED_SUBRESOURCE map;
    UINT x,y;
    uint64_t hash = 14695981039346656037ull;
    if (!texture) return 0;
    ID3D11Texture2D_GetDesc(texture, &d);
    d.Usage = D3D11_USAGE_STAGING; d.BindFlags = 0; d.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    if (d.Format == DXGI_FORMAT_D24_UNORM_S8_UINT) d.Format = DXGI_FORMAT_R24G8_TYPELESS;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device,&d,NULL,&staging)));
    ID3D11DeviceContext_CopyResource(g_device_state.d3d11_context,(ID3D11Resource *)staging,(ID3D11Resource *)texture);
    CHECK(SUCCEEDED(ID3D11DeviceContext_Map(g_device_state.d3d11_context,(ID3D11Resource *)staging,0,D3D11_MAP_READ,0,&map)));
    for (y=0;y<d.Height;y++) for(x=0;x<d.Width*4;x++) {
        hash ^= ((unsigned char *)map.pData)[y*map.RowPitch+x]; hash *= 1099511628211ull;
    }
    ID3D11DeviceContext_Unmap(g_device_state.d3d11_context,(ID3D11Resource *)staging,0);
    ID3D11Texture2D_Release(staging);
    return hash;
}
static void hash_all(uint64_t *hashes)
{
    ID3D11Texture2D *images[20]; unsigned i;
    CHECK(render_checkpoint_inventory(images));
    for(i=0;i<20;i++) hashes[i]=pixels(images[i]);
}
