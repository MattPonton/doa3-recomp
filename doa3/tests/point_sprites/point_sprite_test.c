/* Captured 3.1 castle registers through the production point draw and shader
 * translator, rendered on WARP. Texture RGB deliberately differs from the
 * particle: this catches replacing its tint/alpha with the sprite texture. */
#include "d3d8_internal.h"
#include "d3d8_combiners.h"
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#define HR(x) CHECK(SUCCEEDED(x))
static ID3D11Device *device;
static ID3D11DeviceContext *context;
unsigned g_d3d8_bind_epoch = 1;
static DWORD render_states[256];
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return context; }
const DWORD *d3d8_GetRenderStates(void) { return render_states; }
#include "vertex.h"
static struct {
    struct { uint32_t offset, enabled; } tex[4];
    uint32_t point_smooth, shader_prog, comb_control;
    uint32_t tss3_color, tss3_alpha;
    int tss3_valid;
} g_pg;
static uint32_t regs[0x800];
#define NVREG(m) regs[(m)>>2]
#include "texop.h"
#include "decode.h"
static int applied_direct, expect_direct, missing_texture;
static DWORD fvf;
static ID3D11ShaderResourceView *sprite_srv;
static ID3D11VertexShader *vs;
static ID3D11InputLayout *layout;
static unsigned binds, unbinds, draws;
static void nv_apply_draw_state_cpu(IDirect3DDevice8 *dev, OutputVertex *v, uint32_t n, int direct)
{ (void)dev; (void)v; (void)n; applied_direct=direct; CHECK(g_pg.tex[0].offset==g_pg.tex[3].offset || !g_pg.point_smooth); }
static IDirect3DTexture8 *nv_stage_texture(IDirect3DDevice8 *dev, int stage)
{ (void)dev; CHECK(stage==3); return missing_texture ? NULL : (IDirect3DTexture8 *)sprite_srv; }
static void nv_apply_tex_address(IDirect3DDevice8 *dev, int stage)
{ (void)dev; CHECK(stage==3); }
static void nv_mtrace_draw(const char *path, const OutputVertex *v, uint32_t n)
{ (void)path; (void)v; (void)n; }
#include "points.h"
static HRESULT __stdcall set_texture(IDirect3DDevice8 *dev, DWORD stage, IDirect3DBaseTexture8 *t)
{
    ID3D11ShaderResourceView *srv=(ID3D11ShaderResourceView *)t;
    (void)dev; CHECK(stage==3);
    if(t) binds++; else unbinds++;
    ID3D11DeviceContext_PSSetShaderResources(context,stage,1,&srv);
    return S_OK;
}
static HRESULT __stdcall set_tss(IDirect3DDevice8 *dev, DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
{ (void)dev; (void)stage; (void)type; (void)value; return S_OK; }
static HRESULT __stdcall set_vs(IDirect3DDevice8 *dev, DWORD value)
{ (void)dev; fvf=value; return S_OK; }
static HRESULT __stdcall draw_up(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE prim, UINT count, const void *data, UINT stride)
{
    ID3D11Buffer *buffer;
    D3D11_BUFFER_DESC bd={0}; D3D11_SUBRESOURCE_DATA sd={0};
    UINT offset=0;
    (void)dev; CHECK(prim==D3DPT_TRIANGLELIST && count==2); draws++;
    CHECK(!!d3d8_combiners_prepare_draw()==expect_direct);
    if(!expect_direct) { CHECK(stride==sizeof(OutputVertex)); CHECK((fvf & 0xf00)==D3DFVF_TEX1); return S_OK; }
    CHECK(stride==52 && (fvf & 0xf00)==D3DFVF_TEX4);
    bd.ByteWidth=count*3*stride; bd.Usage=D3D11_USAGE_IMMUTABLE; bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
    sd.pSysMem=data; HR(ID3D11Device_CreateBuffer(device,&bd,&sd,&buffer));
    ID3D11DeviceContext_IASetVertexBuffers(context,0,1,&buffer,&stride,&offset);
    ID3D11DeviceContext_IASetInputLayout(context,layout);
    ID3D11DeviceContext_IASetPrimitiveTopology(context,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_VSSetShader(context,vs,NULL,0);
    ID3D11DeviceContext_Draw(context,count*3,0);
    ID3D11Buffer_Release(buffer); return S_OK;
}
static IDirect3DDevice8Vtbl vtable;
static IDirect3DDevice8 dev;
static ID3D11Texture2D *target,*readback;
static void check_pixel(unsigned x,unsigned y,unsigned r,unsigned g,unsigned b)
{
    D3D11_MAPPED_SUBRESOURCE map;
    const unsigned char *p;
    ID3D11DeviceContext_CopyResource(context,(ID3D11Resource *)readback,(ID3D11Resource *)target);
    HR(ID3D11DeviceContext_Map(context,(ID3D11Resource *)readback,0,D3D11_MAP_READ,0,&map));
    p=(const unsigned char *)map.pData+y*map.RowPitch+x*4;
    if(abs((int)p[0]-(int)r)>1 || abs((int)p[1]-(int)g)>1 || abs((int)p[2]-(int)b)>1)
        fprintf(stderr,"pixel %u,%u got %u,%u,%u expected %u,%u,%u\n",x,y,p[0],p[1],p[2],r,g,b);
    CHECK(abs((int)p[0]-(int)r)<=1 && abs((int)p[1]-(int)g)<=1 && abs((int)p[2]-(int)b)<=1);
    ID3D11DeviceContext_Unmap(context,(ID3D11Resource *)readback,0);
}
static ID3D11RenderTargetView *rt;
static void render(unsigned color)
{
    float black[4]={0};
    OutputVertex quad[6]={{0,0,.5f,1,0,0,0},{64,0,.5f,1,0,1,0},{0,64,.5f,1,0,0,1},
                          {0,64,.5f,1,0,0,1},{64,0,.5f,1,0,1,0},{64,64,.5f,1,0,1,1}};
    int direct; unsigned i;
    for(i=0;i<6;i++) quad[i].color=color;
    ID3D11DeviceContext_ClearRenderTargetView(context,rt,black);
    direct=nv_apply_point_state(&dev,quad,6);
    CHECK(direct==expect_direct && applied_direct==expect_direct);
    CHECK(g_pg.tex[0].offset==123); /* Temporary texture substitution restored. */
    nv_draw_point_sprites(&dev,quad,6,direct);
    CHECK(!d3d8_combiners_prepare_draw()); /* No shader leaks into next draw. */
}
int main(void)
{
    ID3DBlob *blob=NULL,*errors=NULL;
    ID3D11Texture2D *texture;
    ID3D11SamplerState *sampler;
    ID3D11BlendState *blend;
    ID3D11RasterizerState *raster;
    D3D11_TEXTURE2D_DESC td={0}; D3D11_SUBRESOURCE_DATA sd={0};
    D3D11_SAMPLER_DESC ss={0}; D3D11_BLEND_DESC bs={0}; D3D11_RASTERIZER_DESC rs={0};
    D3D11_VIEWPORT viewport={0,0,64,64,0,1};
    const unsigned char texels[16]={0,255,0,0, 0,255,0,128, 0,255,0,255, 0,255,0,64};
    const D3D11_INPUT_ELEMENT_DESC elements[]={
        {"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",3,DXGI_FORMAT_R32G32_FLOAT,0,44,D3D11_INPUT_PER_VERTEX_DATA,0}};
    const char *source="struct V{float4 p:POSITION;float4 c:COLOR0;float2 uv:TEXCOORD3;};"
        "struct O{float4 p:SV_POSITION;float4 c:COLOR0;float4 s:COLOR1;float2 t0:TEXCOORD0;float2 t1:TEXCOORD1;float2 t2:TEXCOORD2;float2 t3:TEXCOORD3;float f:TEXCOORD4;};"
        "O main(V v){O o=(O)0;o.p=float4(v.p.x/32-1,1-v.p.y/32,v.p.z,1);o.c=v.c.bgra;o.t3=v.uv;o.f=1;return o;}";
    HR(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,&context));
    HR(d3d8_combiners_init());
    HR(D3DCompile(source,strlen(source),NULL,NULL,NULL,"main","vs_4_0",0,0,&blob,&errors));
    HR(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(blob),ID3D10Blob_GetBufferSize(blob),NULL,&vs));
    HR(ID3D11Device_CreateInputLayout(device,elements,3,ID3D10Blob_GetBufferPointer(blob),ID3D10Blob_GetBufferSize(blob),&layout));
    ID3D10Blob_Release(blob);
    td.Width=td.Height=64; td.MipLevels=td.ArraySize=td.SampleDesc.Count=1;
    td.Format=DXGI_FORMAT_R8G8B8A8_UNORM; td.BindFlags=D3D11_BIND_RENDER_TARGET;
    HR(ID3D11Device_CreateTexture2D(device,&td,NULL,&target));
    HR(ID3D11Device_CreateRenderTargetView(device,(ID3D11Resource *)target,NULL,&rt));
    td.BindFlags=0; td.Usage=D3D11_USAGE_STAGING; td.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    HR(ID3D11Device_CreateTexture2D(device,&td,NULL,&readback));
    td.Width=td.Height=2; td.BindFlags=D3D11_BIND_SHADER_RESOURCE; td.Usage=D3D11_USAGE_IMMUTABLE; td.CPUAccessFlags=0;
    sd.pSysMem=texels; sd.SysMemPitch=8;
    HR(ID3D11Device_CreateTexture2D(device,&td,&sd,&texture));
    HR(ID3D11Device_CreateShaderResourceView(device,(ID3D11Resource *)texture,NULL,&sprite_srv));
    ss.Filter=D3D11_FILTER_MIN_MAG_MIP_POINT; ss.AddressU=ss.AddressV=ss.AddressW=D3D11_TEXTURE_ADDRESS_CLAMP; ss.MaxLOD=D3D11_FLOAT32_MAX;
    HR(ID3D11Device_CreateSamplerState(device,&ss,&sampler));
    ID3D11DeviceContext_PSSetSamplers(context,3,1,&sampler);
    bs.RenderTarget[0].BlendEnable=TRUE; bs.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA; bs.RenderTarget[0].DestBlend=D3D11_BLEND_ONE;
    bs.RenderTarget[0].BlendOp=bs.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
    bs.RenderTarget[0].SrcBlendAlpha=bs.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ONE; bs.RenderTarget[0].RenderTargetWriteMask=15;
    HR(ID3D11Device_CreateBlendState(device,&bs,&blend));
    ID3D11DeviceContext_OMSetBlendState(context,blend,NULL,~0u);
    rs.FillMode=D3D11_FILL_SOLID; rs.CullMode=D3D11_CULL_NONE; rs.DepthClipEnable=TRUE;
    HR(ID3D11Device_CreateRasterizerState(device,&rs,&raster));
    ID3D11DeviceContext_RSSetState(context,raster); ID3D11DeviceContext_RSSetViewports(context,1,&viewport);
    ID3D11DeviceContext_OMSetRenderTargets(context,1,&rt,NULL);
    vtable.SetTexture=set_texture; vtable.SetTextureStageState=set_tss;
    vtable.SetVertexShader=set_vs; vtable.DrawPrimitiveUP=draw_up; dev.lpVtbl=&vtable;
    g_pg.tex[0].offset=123; g_pg.tex[3].offset=456; g_pg.point_smooth=1;
    g_pg.shader_prog=0x8000; g_pg.comb_control=NVREG(0x1E60)=1;
    NVREG(0x0AC0)=0x04200000; NVREG(0x0260)=0x1B140000;
    NVREG(0x1E40)=NVREG(0x0AA0)=0xC00;
    NVREG(0x0288)=0x0000000C; NVREG(0x028C)=0x00001C80;
    expect_direct=1;
    render(0x33FF6D02); /* 20% particle opacity, orange RGB, green mask texture. */
    check_pixel(16,16,0,0,0); check_pixel(48,16,26,11,0);
    check_pixel(16,48,51,22,0); check_pixel(48,48,13,5,0);
    render(0x00FF6D02); check_pixel(16,48,0,0,0);
    render(0xFFFF6D02); check_pixel(16,48,255,109,2);
    /* A texture-colored particle must still follow its own combiner. */
    NVREG(0x0AC0)=0x0B200000;
    render(0xFFFF6D02); check_pixel(16,48,0,255,0);
    missing_texture=1; render(0xFFFF6D02); check_pixel(16,48,0,0,0); missing_texture=0;
    CHECK(binds==4 && unbinds==6);
    expect_direct=0; g_pg.tss3_valid=1; render(0xFFFFFFFF);
    g_pg.tss3_valid=0; g_pg.shader_prog=0x8001; render(0xFFFFFFFF);
    g_pg.shader_prog=0x8000; g_pg.point_smooth=0; render(0xFFFFFFFF);
    g_pg.point_smooth=1; g_pg.comb_control=0; render(0xFFFFFFFF);
    CHECK(draws==9);
    d3d8_combiners_shutdown(); ID3D11DeviceContext_ClearState(context);
    ID3D11ShaderResourceView_Release(sprite_srv); ID3D11Texture2D_Release(texture);
    ID3D11Texture2D_Release(target); ID3D11Texture2D_Release(readback); ID3D11RenderTargetView_Release(rt);
    ID3D11SamplerState_Release(sampler); ID3D11BlendState_Release(blend); ID3D11RasterizerState_Release(raster);
    ID3D11InputLayout_Release(layout); ID3D11VertexShader_Release(vs);
    ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    puts("PASS: castle ember tint, particle fade, T3 mask/coordinates, texture-colored particles, missing texture, state cleanup and legacy gates");
    return 0;
}
