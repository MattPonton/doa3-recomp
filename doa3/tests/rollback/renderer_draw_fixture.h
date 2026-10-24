/* Shared headless WARP scene. Real uploads, render targets and snapshots;
 * the tiny shader/pipeline is a fixture, not a DOA3 guest draw stream. */
#define DOA3_FULL_PIPELINE_FIXTURE
#include "render_fixture.h"
#include "d3d8_internal.h"
#include "d3d8_swizzle.h"
#include "nv2a/nv2a_renderer_checkpoint.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include "nv2a/nv2a_regs.h"
#include <d3dcompiler.h>
extern ptrdiff_t g_xbox_mem_offset;
uint32_t g_texfmt[64],g_texnull[4];
#include "texture_restore.generated.inc"
static unsigned char *draw_ram;
static uint32_t draw_offset;
static float model_view[16];
static IDirect3DDevice8 upload_device;
void d3d8_debug_views_apply(void) {}
void d3d8_debug_views_shutdown(void) {}
IDirect3DDevice8 *xbox_GetD3DDevice(void) { return &upload_device; }
static HRESULT __stdcall create_texture(IDirect3DDevice8 *dev,UINT w,UINT h,UINT levels,
    DWORD usage,D3DFORMAT format,D3DPOOL pool,IDirect3DTexture8 **out)
{ (void)dev;(void)pool;return d3d8_CreateTextureImpl(w,h,levels,usage,format,out); }
static ID3D11VertexShader *vs;
static ID3D11PixelShader *ps;
static ID3D11Buffer *constant;
static ID3D11SamplerState *sampler;
static ID3D11RasterizerState *raster;
static void init_shaders(void) {
    const char *source=
        "cbuffer C:register(b0){float4 tint;} Texture2D a:register(t0);Texture2D b:register(t1);SamplerState s:register(s0);"
        "float4 vs(uint i:SV_VertexID):SV_Position{float2 p=float2((i<<1)&2,i&2);return float4(p*float2(2,-2)+float2(-1,1),0.5,1);}"
        "float4 ps(float4 p:SV_Position):SV_Target{return a.SampleLevel(s,p.xy/32,1)*tint+b.SampleLevel(s,p.xy/32,0)*0.2;}";
    ID3DBlob *vb=NULL,*pb=NULL;
    D3D11_BUFFER_DESC bd={0};D3D11_SAMPLER_DESC sd={0};D3D11_RASTERIZER_DESC rd={0};
    CHECK(SUCCEEDED(D3DCompile(source,strlen(source),NULL,NULL,NULL,"vs","vs_5_0",0,0,&vb,NULL)));
    CHECK(SUCCEEDED(D3DCompile(source,strlen(source),NULL,NULL,NULL,"ps","ps_5_0",0,0,&pb,NULL)));
    CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(d3d8_GetD3D11Device(),ID3D10Blob_GetBufferPointer(vb),ID3D10Blob_GetBufferSize(vb),NULL,&vs)));
    CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(d3d8_GetD3D11Device(),ID3D10Blob_GetBufferPointer(pb),ID3D10Blob_GetBufferSize(pb),NULL,&ps)));
    ID3D10Blob_Release(vb);ID3D10Blob_Release(pb);
    bd.ByteWidth=16;bd.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
    CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(d3d8_GetD3D11Device(),&bd,NULL,&constant)));
    sd.Filter=D3D11_FILTER_MIN_MAG_MIP_POINT;sd.AddressU=sd.AddressV=sd.AddressW=D3D11_TEXTURE_ADDRESS_CLAMP;sd.MaxLOD=D3D11_FLOAT32_MAX;
    CHECK(SUCCEEDED(ID3D11Device_CreateSamplerState(d3d8_GetD3D11Device(),&sd,&sampler)));
    rd.FillMode=D3D11_FILL_SOLID;rd.CullMode=D3D11_CULL_NONE;rd.DepthClipEnable=TRUE;
    CHECK(SUCCEEDED(ID3D11Device_CreateRasterizerState(d3d8_GetD3D11Device(),&rd,&raster)));
}
static void draw_now(void) {
    ID3D11DeviceContext *ctx=d3d8_GetD3D11Context();
    D3D8Texture *texture=(D3D8Texture *)get_dynamic_texture(&upload_device);CHECK(texture);
    /* Fixture rebuilds its entire pipeline; this is not PGRAPH submit_draw. */
    ID3D11DeviceContext_IASetInputLayout(ctx,NULL);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_VSSetShader(ctx,vs,NULL,0);ID3D11DeviceContext_PSSetShader(ctx,ps,NULL,0);
    ID3D11DeviceContext_RSSetState(ctx,raster);ID3D11DeviceContext_OMSetBlendState(ctx,NULL,NULL,~0u);
    ID3D11DeviceContext_OMSetDepthStencilState(ctx,NULL,0);
    ID3D11DeviceContext_UpdateSubresource(ctx,(ID3D11Resource *)constant,0,NULL,g_pg.vp.consts[0],0,0);
    ID3D11DeviceContext_PSSetConstantBuffers(ctx,0,1,&constant);ID3D11DeviceContext_PSSetSamplers(ctx,0,1,&sampler);
    ID3D11DeviceContext_PSSetShaderResources(ctx,0,1,&texture->srv);
    CHECK(d3d8_SetOffscreenTarget(0x2000,16,16));
    fill(cur_rtv(),cur_dsv(),model_view[0],17);
    CHECK(d3d8_BindPrevFrame(1));ID3D11DeviceContext_Draw(ctx,3,0);
    d3d8_RestoreDefaultTarget();fill(cur_rtv(),cur_dsv(),model_view[0],25);
    CHECK(d3d8_BindOffscreenTexture(0x2000,1));ID3D11DeviceContext_Draw(ctx,3,0);
    CHECK(d3d8_BindFramebufferCopy(2));d3d8_keep_prev_frame();
}
static void change_input(unsigned n) {
    memset(draw_ram+draw_offset+4096,(int)(30+n*35),1024); /* lower mip only */
    g_pg.vp.consts[0][0]=.3f+n*.08f;g_pg.vp.consts[0][1]=.7f;g_pg.vp.consts[0][2]=.6f;g_pg.vp.consts[0][3]=1;
    model_view[0]=.9f;g_pg_aux.blend_color=n; /* clear depth behind triangle */
    pgraph_d3d11_invalidate_guest_uploads(); /* explicit fixture upload event */
}
static void init_draw_fixture(unsigned char *memory,uint32_t offset) {
    static IDirect3DDevice8Vtbl upload_vt;
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,
        &g_device_state.d3d11_device,NULL,&g_device_state.d3d11_context)));
    CHECK(d3d8_SetOffscreenTarget(0x1000,32,16));
    g_device_state.default_tex=g_offrt[0].tex;g_device_state.default_depth=g_offrt[0].depth;
    g_device_state.default_rtv=g_offrt[0].rtv;g_device_state.default_dsv=g_offrt[0].dsv;
    memset(&g_offrt[0],0,sizeof g_offrt[0]);g_device_state.width=32;g_device_state.height=16;
    d3d8_RestoreDefaultTarget();fill(cur_rtv(),cur_dsv(),.9f,1);
    CHECK(d3d8_BindFramebufferCopy(2));d3d8_keep_prev_frame();
    CHECK(d3d8_SetOffscreenTarget(0x2000,16,16));fill(cur_rtv(),cur_dsv(),.9f,2);d3d8_RestoreDefaultTarget();
    init_shaders();upload_vt.CreateTexture=create_texture;upload_device.lpVtbl=&upload_vt;
    draw_ram=memory;draw_offset=offset;memset(draw_ram+draw_offset,0x11,4096);
    g_pg.tex[0].offset=offset;g_pg.tex[0].format=(5u<<24)|(5u<<20)|(2u<<16)|(6u<<8);
}
static void close_draw_fixture(void) {
    unsigned i;
    ID3D11DeviceContext_ClearState(d3d8_GetD3D11Context());
    for(i=0;i<TEXCACHE_N;i++)if(g_pg_host.texcache[i].tex)texcache_evict((int)i);
    for(i=0;i<8;i++)offrt_free(&g_offrt[i]);
    ID3D11VertexShader_Release(vs);ID3D11PixelShader_Release(ps);ID3D11Buffer_Release(constant);
    ID3D11SamplerState_Release(sampler);ID3D11RasterizerState_Release(raster);
    ID3D11ShaderResourceView_Release(g_fbcopy.srv);ID3D11Texture2D_Release(g_fbcopy.tex);
    ID3D11ShaderResourceView_Release(g_prevframe.srv);ID3D11Texture2D_Release(g_prevframe.tex);
    ID3D11RenderTargetView_Release(g_device_state.default_rtv);ID3D11DepthStencilView_Release(g_device_state.default_dsv);
    ID3D11Texture2D_Release(g_device_state.default_tex);ID3D11Texture2D_Release(g_device_state.default_depth);
    ID3D11DeviceContext_Release(g_device_state.d3d11_context);CHECK(ID3D11Device_Release(g_device_state.d3d11_device)==0);
}
