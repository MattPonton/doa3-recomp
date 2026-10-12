/* Exercise production capture/replay against the WARP software GPU. No game,
 * window, or input device needed. Pixel checks prove geometric intermediate
 * frames rather than repeated presents or whole-image crossfades. */
#include "d3d8_internal.h"
#include "d3d8_interpolation.h"
#include "interpolation_math.h"
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static ID3D11Device *device;
static ID3D11DeviceContext *context;
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return context; }
UINT d3d8_GetBackbufferWidth(void) { return 64; }
UINT d3d8_GetBackbufferHeight(void) { return 64; }
typedef struct Vertex { float x,y,z,w; unsigned color; float uv[4]; } Vertex;

static void frame(float x, unsigned scene)
{
    Vertex v[3]={{x,24,.5f,1,0xffffffff,{0}}, {x+4,24,.5f,1,0xffffffff,{0}}, {x,40,.5f,1,0xffffffff,{0}}};
    d3d8_interpolation_clear(D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0xff000000,1,0);
    d3d8_interpolation_draw(v,3,sizeof(Vertex),NULL,0,NULL);
    d3d8_interpolation_finish(1,scene);
}

static int red_at(ID3D11ShaderResourceView *srv, unsigned x, unsigned y)
{
    ID3D11Resource *resource=NULL;
    ID3D11Texture2D *staging=NULL;
    D3D11_TEXTURE2D_DESC td={0};
    D3D11_MAPPED_SUBRESOURCE map;
    int red;
    ID3D11ShaderResourceView_GetResource(srv,&resource);
    td.Width=td.Height=64; td.ArraySize=td.MipLevels=td.SampleDesc.Count=1;
    td.Format=DXGI_FORMAT_R8G8B8A8_UNORM; td.Usage=D3D11_USAGE_STAGING;
    td.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(device,&td,NULL,&staging)));
    ID3D11DeviceContext_CopyResource(context,(ID3D11Resource *)staging,resource);
    CHECK(SUCCEEDED(ID3D11DeviceContext_Map(context,(ID3D11Resource *)staging,0,D3D11_MAP_READ,0,&map)));
    red=((unsigned char *)map.pData)[y*map.RowPitch+x*4]>200;
    ID3D11DeviceContext_Unmap(context,(ID3D11Resource *)staging,0);
    ID3D11Texture2D_Release(staging); ID3D11Resource_Release(resource);
    return red;
}

int main(void)
{
    const char *hlsl="struct V{float4 p:POSITION;}; float4 vs(V v):SV_POSITION{return float4(v.p.x/32-1,1-v.p.y/32,v.p.z,1);} float4 ps():SV_TARGET{return float4(1,0,0,1);}";
    ID3DBlob *vblob=NULL,*pblob=NULL,*errors=NULL;
    ID3D11VertexShader *vs=NULL,*restored=NULL;
    ID3D11PixelShader *ps=NULL;
    ID3D11InputLayout *layout=NULL;
    ID3D11RasterizerState *raster=NULL;
    D3D11_INPUT_ELEMENT_DESC element={"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0};
    D3D11_RASTERIZER_DESC rd={0};
    D3D11_VIEWPORT vp={0,0,64,64,0,1};
    ID3D11ShaderResourceView *srv;
    float a[4]={0,0,.2f,1},b[4]={8,4,.8f,.5f},out[4];
    CHECK(interpolation_steps(60)==1 && interpolation_steps(120)==2 && interpolation_steps(240)==4);
    CHECK(interpolation_steps(144)==1);
    {
        InterpolationBudget budget={0}; unsigned i;
        for(i=0;i<120;++i) interpolation_budget_tick(&budget,12);
        CHECK(!budget.limited);
        for(i=0;i<8;++i) interpolation_budget_tick(&budget,17.5);
        CHECK(!budget.limited);
        for(i=0;i<16;++i) interpolation_budget_tick(&budget,12);
        /* A late tick followed by catch-up must not permanently disable
         * interpolation while the simulation is still keeping real time. */
        for(i=0;i<100;++i) {
            interpolation_budget_tick(&budget,24);
            interpolation_budget_tick(&budget,1000.0/30.0-24);
        }
        CHECK(!budget.limited);
        interpolation_budget_tick(&budget,50); interpolation_budget_tick(&budget,12);
        CHECK(!budget.limited);
        for(i=0;i<8;++i) interpolation_budget_tick(&budget,22);
        CHECK(budget.limited);
    }
    CHECK(interpolation_position(out,a,b,0) && fabsf(out[0])<.001f);
    CHECK(interpolation_position(out,a,b,1) && fabsf(out[0]-8)<.001f);
    CHECK(interpolation_position(out,a,b,.5f) && fabsf(out[0]-16.f/3)<.001f);
    b[3]=0; CHECK(!interpolation_position(out,a,b,.5f));
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,&context)));
    CHECK(SUCCEEDED(D3DCompile(hlsl,strlen(hlsl),NULL,NULL,NULL,"vs","vs_4_0",0,0,&vblob,&errors)));
    CHECK(SUCCEEDED(D3DCompile(hlsl,strlen(hlsl),NULL,NULL,NULL,"ps","ps_4_0",0,0,&pblob,&errors)));
    CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(vblob),ID3D10Blob_GetBufferSize(vblob),NULL,&vs)));
    CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(device,ID3D10Blob_GetBufferPointer(pblob),ID3D10Blob_GetBufferSize(pblob),NULL,&ps)));
    CHECK(SUCCEEDED(ID3D11Device_CreateInputLayout(device,&element,1,ID3D10Blob_GetBufferPointer(vblob),ID3D10Blob_GetBufferSize(vblob),&layout)));
    rd.FillMode=D3D11_FILL_SOLID; rd.CullMode=D3D11_CULL_NONE; rd.DepthClipEnable=TRUE;
    CHECK(SUCCEEDED(ID3D11Device_CreateRasterizerState(device,&rd,&raster)));
    ID3D11DeviceContext_IASetInputLayout(context,layout);
    ID3D11DeviceContext_IASetPrimitiveTopology(context,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_VSSetShader(context,vs,NULL,0);
    ID3D11DeviceContext_PSSetShader(context,ps,NULL,0);
    ID3D11DeviceContext_RSSetState(context,raster);
    ID3D11DeviceContext_RSSetViewports(context,1,&vp);
    d3d8_interpolation_finish(1,1);
    frame(16,1); CHECK(!d3d8_interpolation_render(.5f));
    frame(24,1);
    srv=d3d8_interpolation_render(.5f); CHECK(srv);
    CHECK(red_at(srv,21,27)); CHECK(!red_at(srv,17,27)); CHECK(!red_at(srv,25,27));
    ID3D11DeviceContext_VSGetShader(context,&restored,NULL,NULL);
    CHECK(restored==vs); ID3D11VertexShader_Release(restored);
    srv=d3d8_interpolation_render(0); CHECK(srv && red_at(srv,17,27));
    srv=d3d8_interpolation_render(1); CHECK(srv && red_at(srv,25,27));
    frame(28,2); CHECK(!d3d8_interpolation_render(.5f));
    frame(32,2); CHECK(!d3d8_interpolation_render(.5f));
    frame(36,2); CHECK(d3d8_interpolation_render(.5f));
    d3d8_interpolation_invalidate(); frame(36,2); CHECK(!d3d8_interpolation_render(.5f));
    d3d8_interpolation_finish(0,2); CHECK(!d3d8_interpolation_render(.5f));
    {
        /* Fixed-function camera transforms and a mutable sampled texture. */
        const char *ff_shader="cbuffer C:register(b0){row_major float4x4 m;} struct V{float4 p:POSITION;}; float4 vs(V v):SV_POSITION{return mul(m,v.p);} Texture2D tex:register(t0); float4 ps():SV_TARGET{return tex.Load(int3(0,0,0))*m[0][0];}";
        ID3DBlob *ffv=NULL,*ffp=NULL;
        ID3D11VertexShader *ffvs=NULL;
        ID3D11PixelShader *ffps=NULL;
        ID3D11Buffer *constant=NULL;
        ID3D11Buffer *pixel_constant=NULL;
        ID3D11Texture2D *tex=NULL;
        ID3D11ShaderResourceView *texsrv=NULL;
        D3D11_BUFFER_DESC bd={0};
        D3D11_TEXTURE2D_DESC td={0};
        Nv2aFFConstants cb={0};
        Nv2aFFVertex v[3]={0};
        uint16_t indices[3]={0,1,2};
        unsigned red=0xff0000ff, black=0xff000000, n;
        CHECK(SUCCEEDED(D3DCompile(ff_shader,strlen(ff_shader),NULL,NULL,NULL,"vs","vs_4_0",0,0,&ffv,&errors)));
        CHECK(SUCCEEDED(D3DCompile(ff_shader,strlen(ff_shader),NULL,NULL,NULL,"ps","ps_4_0",0,0,&ffp,&errors)));
        CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(ffv),ID3D10Blob_GetBufferSize(ffv),NULL,&ffvs)));
        CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(device,ID3D10Blob_GetBufferPointer(ffp),ID3D10Blob_GetBufferSize(ffp),NULL,&ffps)));
        bd.ByteWidth=sizeof cb; bd.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
        CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(device,&bd,NULL,&constant)));
        CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(device,&bd,NULL,&pixel_constant)));
        td.Width=td.Height=td.ArraySize=td.MipLevels=td.SampleDesc.Count=1;
        td.Format=DXGI_FORMAT_R8G8B8A8_UNORM; td.BindFlags=D3D11_BIND_SHADER_RESOURCE;
        CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(device,&td,NULL,&tex)));
        CHECK(SUCCEEDED(ID3D11Device_CreateShaderResourceView(device,(ID3D11Resource *)tex,NULL,&texsrv)));
        ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)tex,0,NULL,&red,4,4);
        ID3D11DeviceContext_VSSetShader(context,ffvs,NULL,0);
        ID3D11DeviceContext_PSSetShader(context,ffps,NULL,0);
        ID3D11DeviceContext_VSSetConstantBuffers(context,0,1,&constant);
        ID3D11DeviceContext_PSSetConstantBuffers(context,0,1,&pixel_constant);
        ID3D11DeviceContext_PSSetShaderResources(context,0,1,&texsrv);
        for(n=0;n<4;++n) cb.clip[n][n]=1;
        v[0].pos[0]=v[2].pos[0]=-.5f; v[1].pos[0]=-.375f;
        v[0].pos[1]=v[1].pos[1]=.25f; v[2].pos[1]=-.25f;
        for(n=0;n<3;++n) { v[n].pos[2]=.5f; v[n].pos[3]=1; }
        d3d8_interpolation_finish(1,3);
        for(n=0;n<2;++n) {
            Nv2aFFConstants shade={0};
            shade.clip[0][0]=1;
            ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)pixel_constant,0,NULL,&shade,0,0);
            d3d8_interpolation_constants(pixel_constant,&shade,sizeof shade);
            cb.clip[0][3]=n*.25f;
            ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)constant,0,NULL,&cb,0,0);
            d3d8_interpolation_constants(constant,&cb,sizeof cb);
            d3d8_interpolation_clear(7,0xff000000,1,0);
            /* A new preceding command must not break this mesh's history. */
            if(n) d3d8_interpolation_clear(0,0,1,0);
            d3d8_interpolation_key(123);
            d3d8_interpolation_draw(v,3,sizeof v[0],indices,3,&cb);
            /* Same pixel constants may share an upload across consecutive
             * draws. A later change must not reuse the old red value. */
            d3d8_interpolation_key(124);
            d3d8_interpolation_draw(v,3,sizeof v[0],indices,3,&cb);
            shade.clip[0][0]=0;
            ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)pixel_constant,0,NULL,&shade,0,0);
            d3d8_interpolation_constants(pixel_constant,&shade,sizeof shade);
            cb.clip[0][3]=.75f;
            ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)constant,0,NULL,&cb,0,0);
            d3d8_interpolation_constants(constant,&cb,sizeof cb);
            d3d8_interpolation_key(125);
            d3d8_interpolation_draw(v,3,sizeof v[0],indices,3,&cb);
            d3d8_interpolation_finish(1,3);
        }
        /* The next game frame overwrites these resources before an extra present. */
        memset(&cb,0,sizeof cb);
        ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)constant,0,NULL,&cb,0,0);
        d3d8_interpolation_texture_changed((ID3D11Resource *)tex);
        ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)tex,0,NULL,&black,4,4);
        srv=d3d8_interpolation_render(.5f);
        CHECK(srv && red_at(srv,21,27) && !red_at(srv,17,27) && !red_at(srv,25,27));
        srv=d3d8_interpolation_render(1);
        CHECK(srv && red_at(srv,25,27));
        CHECK(!red_at(srv,41,27));
        d3d8_interpolation_finish(0,3);
        ID3D11ShaderResourceView_Release(texsrv); ID3D11Texture2D_Release(tex);
        ID3D11Buffer_Release(constant); ID3D11VertexShader_Release(ffvs); ID3D11PixelShader_Release(ffps);
        ID3D11Buffer_Release(pixel_constant);
        ID3D10Blob_Release(ffv); ID3D10Blob_Release(ffp);
    }
    d3d8_interpolation_shutdown();
    ID3D11DeviceContext_ClearState(context); ID3D11DeviceContext_Flush(context);
    ID3D11RasterizerState_Release(raster); ID3D11InputLayout_Release(layout);
    ID3D11VertexShader_Release(vs); ID3D11PixelShader_Release(ps);
    ID3D10Blob_Release(vblob); ID3D10Blob_Release(pblob);
    ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    puts("PASS: CPU/GPU geometric midpoint, camera transforms, immutable texture/constant snapshots, perspective endpoints, state restore, scene cuts, invalidation, disable");
    return 0;
}
