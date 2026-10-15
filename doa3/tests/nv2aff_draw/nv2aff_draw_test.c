/* Render through the production NV2A draw and both shader paths on WARP.
 * Alternating fallback/combiner draws catches stale bindings; changing colors
 * and transforms catches constants that were accidentally supplied by the
 * generic preparation removed from the fast path. */
#include "d3d8_internal.h"
#include "d3d8_nv2aff.h"
#include "d3d8_combiners.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#define HR(x) CHECK(SUCCEEDED(x))
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static struct { ID3D11Device *d3d11_device; ID3D11DeviceContext *d3d11_context; } g_device_state;
static DWORD rs[256], tss[4][32];
static D3DMATRIX identity;
#define NV2AFF_RING_SIZE 4096
static ID3D11Buffer *g_nv2aff_ring;
static UINT g_nv2aff_ring_off;
static ID3D11Buffer *g_nv2aff_ia_ring;
static unsigned g_nv2aff_ia_epoch;
static int g_nv2aff_ia_topology = -1;
static IDirect3DVertexBuffer8 *g_cur_vb;
static IDirect3DIndexBuffer8 *g_cur_ib;
static UINT g_cur_vb_stride;
static unsigned g_d3d_draw_count, g_d3d_draw_off;
static int g_off_active;
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return context; }
const DWORD *d3d8_GetRenderStates(void) { return rs; }
const DWORD *d3d8_GetTSS(DWORD stage) { return tss[stage]; }
const D3DMATRIX *d3d8_GetTransform(D3DTRANSFORMSTATETYPE type) { (void)type; return &identity; }
const D3DMATERIAL8 *d3d8_GetMaterial(void) { return NULL; }
const D3DLIGHT8 *d3d8_GetLight(DWORD index) { (void)index; return NULL; }
BOOL d3d8_GetLightEnable(DWORD index) { (void)index; return FALSE; }
UINT d3d8_GetNumLights(void) { return 0; }
UINT d3d8_GetBackbufferWidth(void) { return 16; }
UINT d3d8_GetBackbufferHeight(void) { return 16; }
void d3d8_states_apply(void) {}
/* Exercise the production uploader with a small ring to force wraps. */
#include "draw_impl.h"
static ID3D11Texture2D *target, *readback;
static Nv2aFFVertex vertices[3];
static Nv2aFFConstants constants;
static void draw_check(const NV2ACombinerState *combiner, unsigned expected)
{
    static const uint16_t indices[] = {0,1,2};
    const float clear[] = {0,0,0,0};
    ID3D11RenderTargetView *rt;
    D3D11_MAPPED_SUBRESOURCE map;
    unsigned char *pixel;
    HR(ID3D11Device_CreateRenderTargetView(device,(ID3D11Resource *)target,NULL,&rt));
    ID3D11DeviceContext_OMSetRenderTargets(context,1,&rt,NULL);
    ID3D11DeviceContext_ClearRenderTargetView(context,rt,clear);
    d3d8_combiners_set_direct(combiner);
    HR(d3d8_DrawNv2aFF(vertices,3,indices,3,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,&constants));
    ID3D11DeviceContext_CopyResource(context,(ID3D11Resource *)readback,(ID3D11Resource *)target);
    HR(ID3D11DeviceContext_Map(context,(ID3D11Resource *)readback,0,D3D11_MAP_READ,0,&map));
    pixel=(unsigned char *)map.pData+8*map.RowPitch+8*4;
    CHECK(abs((int)pixel[0]-(int)((expected>>16)&255))<=1);
    CHECK(abs((int)pixel[1]-(int)((expected>>8)&255))<=1);
    CHECK(abs((int)pixel[2]-(int)(expected&255))<=1);
    ID3D11DeviceContext_Unmap(context,(ID3D11Resource *)readback,0);
    ID3D11RenderTargetView_Release(rt);
}
int main(void)
{
    D3D11_TEXTURE2D_DESC td={0}; D3D11_VIEWPORT vp={0};
    D3D11_RASTERIZER_DESC rd={0}; ID3D11RasterizerState *raster;
    NV2ACombinerState combiner={0};
    HR(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,&context));
    g_device_state.d3d11_device=device;
    g_device_state.d3d11_context=context;
    for(int i=0;i<4;i++) { ((float *)&identity)[i*5]=1; constants.clip[i][i]=1; combiner.tex_mode[i]=NV2A_TEXMODE_NONE; }
    constants.flags=NV2AFF_FLAG_HAS_COLOR;
    for(int i=0;i<3;i++) { vertices[i].pos[2]=0.5f; vertices[i].pos[3]=1; vertices[i].color=0xff204080; }
    vertices[0].pos[0]=-1; vertices[0].pos[1]=-1;
    vertices[1].pos[0]=-1; vertices[1].pos[1]=3;
    vertices[2].pos[0]=3; vertices[2].pos[1]=-1;
    tss[0][D3DTSS_COLOROP]=D3DTOP_SELECTARG1;
    tss[0][D3DTSS_COLORARG1]=D3DTA_DIFFUSE;
    tss[0][D3DTSS_ALPHAOP]=D3DTOP_SELECTARG1;
    tss[0][D3DTSS_ALPHAARG1]=D3DTA_DIFFUSE;
    for(int i=1;i<4;i++) tss[i][D3DTSS_COLOROP]=D3DTOP_DISABLE;
    HR(d3d8_shaders_init()); HR(d3d8_Nv2aFF_Init()); HR(d3d8_combiners_init());
    td.Width=td.Height=16; td.MipLevels=td.ArraySize=1; td.SampleDesc.Count=1;
    td.Format=DXGI_FORMAT_R8G8B8A8_UNORM; td.BindFlags=D3D11_BIND_RENDER_TARGET;
    HR(ID3D11Device_CreateTexture2D(device,&td,NULL,&target));
    td.BindFlags=0; td.Usage=D3D11_USAGE_STAGING; td.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    HR(ID3D11Device_CreateTexture2D(device,&td,NULL,&readback));
    vp.Width=vp.Height=16; vp.MaxDepth=1; ID3D11DeviceContext_RSSetViewports(context,1,&vp);
    rd.FillMode=D3D11_FILL_SOLID; rd.CullMode=D3D11_CULL_NONE; rd.DepthClipEnable=TRUE;
    HR(ID3D11Device_CreateRasterizerState(device,&rd,&raster)); ID3D11DeviceContext_RSSetState(context,raster);
    combiner.final_input[3].reg=NV2A_REG_C0;
    combiner.final_input[6].reg=NV2A_REG_C0; combiner.final_input[6].alpha_rep=1;
    combiner.final_c0=0xffc03010; draw_check(&combiner,0xc03010);
    draw_check(NULL,0x204080);
    combiner.final_c0=0xff10c030; draw_check(&combiner,0x10c030);
    constants.clip[0][3]=8; draw_check(&combiner,0);
    constants.clip[0][3]=0; draw_check(NULL,0x204080);
    for(int i=0;i<3;i++) vertices[i].color=0xff907020;
    draw_check(NULL,0x907020); draw_check(&combiner,0x10c030);
    /* Appends must preserve base/index offsets, DISCARD wraps must retain
     * the ring binding, and externally replaced IA state must be rebound. */
    for (int i=0;i<80;i++) {
        if (i%7==0) {
            ID3D11Buffer *null_buffer=NULL; UINT zero=0;
            ID3D11DeviceContext_IASetVertexBuffers(context,0,1,&null_buffer,&zero,&zero);
            ID3D11DeviceContext_IASetIndexBuffer(context,NULL,DXGI_FORMAT_R16_UINT,0);
            d3d8_BindInvalidate();
        }
        draw_check(i%2 ? &combiner : NULL,i%2 ? 0x10c030 : 0x907020);
    }
    ID3D11DeviceContext_ClearState(context);
    d3d8_combiners_shutdown(); d3d8_Nv2aFF_Shutdown(); d3d8_shaders_shutdown();
    ID3D11Buffer_Release(g_nv2aff_ring);
    ID3D11RasterizerState_Release(raster); ID3D11Texture2D_Release(target); ID3D11Texture2D_Release(readback);
    ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    puts("NV2A combiner/fallback transitions, changing colors and transforms pass");
    return 0;
}
