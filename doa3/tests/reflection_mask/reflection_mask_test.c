/* Real renderer shader sources and NV register decoding, exercised on WARP. */
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "d3d8_combiners.h"
#include "vertices.h"
#include "clip.h"
#include "vertex_shader.h"
#include "pixel_generator.h"
static uint32_t regs[0x2000/4];
#define NVREG(x) regs[(x)/4]
static struct {uint32_t shader_prog; struct {int enabled;} tex[4];} g_pg;
#include "registers.h"
#define CHECK(c) do { if (!(c)) {fprintf(stderr,"line %d: %s\n",__LINE__,#c);exit(1);} } while(0)
static ID3D11Device *dev;
static ID3D11DeviceContext *ctx;
static ID3DBlob *compile(const char *code,const char *target) {
 ID3DBlob *b=NULL,*e=NULL; HRESULT hr=D3DCompile(code,strlen(code),NULL,NULL,NULL,"main",target,0,0,&b,&e);
 if(FAILED(hr)&&e) fputs((char*)ID3D10Blob_GetBufferPointer(e),stderr);
 CHECK(SUCCEEDED(hr));if(e)ID3D10Blob_Release(e);return b;
}
static ID3D11Buffer *buffer(const void *data,UINT bytes,UINT bind) {
 D3D11_BUFFER_DESC d={0}; D3D11_SUBRESOURCE_DATA s={0}; ID3D11Buffer *b;
 d.ByteWidth=bytes;d.Usage=D3D11_USAGE_DEFAULT;d.BindFlags=bind;s.pSysMem=data;
 CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(dev,&d,&s,&b)));return b;
}
static ID3D11ShaderResourceView *texture(const BYTE *pixels,UINT width) {
 D3D11_TEXTURE2D_DESC d={0};D3D11_SUBRESOURCE_DATA s={0}; ID3D11Texture2D *t;ID3D11ShaderResourceView *v;
 d.Width=width;d.Height=d.MipLevels=d.ArraySize=d.SampleDesc.Count=1;d.Format=DXGI_FORMAT_R8G8B8A8_UNORM;d.BindFlags=D3D11_BIND_SHADER_RESOURCE;
 s.pSysMem=pixels;s.SysMemPitch=width*4;CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(dev,&d,&s,&t)));
 CHECK(SUCCEEDED(ID3D11Device_CreateShaderResourceView(dev,(ID3D11Resource*)t,NULL,&v)));ID3D11Texture2D_Release(t);return v;
}
int main(void) {
 ID3DBlob *vb,*pb;ID3D11VertexShader *vs;ID3D11PixelShader *ps;ID3D11InputLayout *layout;
 ID3D11Texture2D *target,*readback;ID3D11RenderTargetView *rtv;ID3D11SamplerState *sampler;
 D3D11_TEXTURE2D_DESC td={0};D3D11_SAMPLER_DESC sd={0};D3D11_VIEWPORT vp={0,0,4,4,0,1};
 D3D11_MAPPED_SUBRESOURCE map;NV2ACombinerState comb;NV2APSConstants pc={0};char hlsl[65536];
 float vc[60]={0};UINT stride=sizeof(OutputVertex),off=0;ID3D11Buffer *vbuf,*vconst,*pconst;
 OutputVertex verts[4]={{0,0,.5f,1,0xffffffff,.5f,.5f,0,0},{4,0,.5f,1,0xffffffff,.5f,.5f,1,0},{0,4,.5f,1,0xffffffff,.5f,.5f,0,1},{4,4,.5f,1,0xffffffff,.5f,.5f,1,1}};
 D3D11_INPUT_ELEMENT_DESC elems[]={
 {"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
 {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
 {"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
 {"COLOR",1,DXGI_FORMAT_R8G8B8A8_UNORM,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
 {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(OutputVertex,u),D3D11_INPUT_PER_VERTEX_DATA,0},
 {"TEXCOORD",1,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(OutputVertex,u1),D3D11_INPUT_PER_VERTEX_DATA,0},
 {"TEXCOORD",2,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(OutputVertex,u),D3D11_INPUT_PER_VERTEX_DATA,0},
 {"TEXCOORD",3,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(OutputVertex,u),D3D11_INPUT_PER_VERTEX_DATA,0}};
 BYTE reflection[]={64,128,192,204},mask[]={255,0,0,0, 0,255,0,64, 0,0,255,128, 0,0,0,255};
 ID3D11ShaderResourceView *textures[2];
 /* Captured Beach reflection combiner registers: RGB=T0, alpha=T0.a*T1.a. */
 NVREG(0x1e60)=2;NVREG(0xac0)=0x08200000;NVREG(0xac4)=0x0c200000;
 NVREG(0x260)=0x18200000;NVREG(0x264)=0x1c190000;
 NVREG(0x1e40)=NVREG(0x1e44)=NVREG(0xaa0)=NVREG(0xaa4)=0xc00;
 NVREG(0x288)=0xc;NVREG(0x28c)=0x1c80;g_pg.shader_prog=0x21;g_pg.tex[0].enabled=g_pg.tex[1].enabled=1;
 nv_build_combiner_state(&comb);CHECK(d3d8_combiners_generate_hlsl(&comb,hlsl,sizeof hlsl)>0);
 CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,&dev,NULL,&ctx)));
 vb=compile(g_vs_source,"vs_5_0");pb=compile(hlsl,"ps_5_0");
 CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(dev,ID3D10Blob_GetBufferPointer(vb),ID3D10Blob_GetBufferSize(vb),NULL,&vs)));
 CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(dev,ID3D10Blob_GetBufferPointer(pb),ID3D10Blob_GetBufferSize(pb),NULL,&ps)));
 CHECK(SUCCEEDED(ID3D11Device_CreateInputLayout(dev,elems,8,ID3D10Blob_GetBufferPointer(vb),ID3D10Blob_GetBufferSize(vb),&layout)));
 vc[48]=vc[49]=4;((UINT*)vc)[50]=1|2|(2<<8);
 vconst=buffer(vc,sizeof vc,D3D11_BIND_CONSTANT_BUFFER);pconst=buffer(&pc,sizeof pc,D3D11_BIND_CONSTANT_BUFFER);vbuf=buffer(verts,sizeof verts,D3D11_BIND_VERTEX_BUFFER);
 td.Width=td.Height=4;td.ArraySize=td.MipLevels=td.SampleDesc.Count=1;td.Format=DXGI_FORMAT_R8G8B8A8_UNORM;td.BindFlags=D3D11_BIND_RENDER_TARGET;
 CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(dev,&td,NULL,&target)));CHECK(SUCCEEDED(ID3D11Device_CreateRenderTargetView(dev,(ID3D11Resource*)target,NULL,&rtv)));
 td.BindFlags=0;td.Usage=D3D11_USAGE_STAGING;td.CPUAccessFlags=D3D11_CPU_ACCESS_READ;CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(dev,&td,NULL,&readback)));
 sd.Filter=D3D11_FILTER_MIN_MAG_MIP_POINT;sd.AddressU=sd.AddressV=sd.AddressW=D3D11_TEXTURE_ADDRESS_CLAMP;sd.MaxLOD=D3D11_FLOAT32_MAX;
 CHECK(SUCCEEDED(ID3D11Device_CreateSamplerState(dev,&sd,&sampler)));textures[0]=texture(reflection,1);textures[1]=texture(mask,4);
 ID3D11DeviceContext_OMSetRenderTargets(ctx,1,&rtv,NULL);ID3D11DeviceContext_RSSetViewports(ctx,1,&vp);
 ID3D11DeviceContext_IASetInputLayout(ctx,layout);ID3D11DeviceContext_IASetVertexBuffers(ctx,0,1,&vbuf,&stride,&off);ID3D11DeviceContext_IASetPrimitiveTopology(ctx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
 ID3D11DeviceContext_VSSetShader(ctx,vs,NULL,0);ID3D11DeviceContext_PSSetShader(ctx,ps,NULL,0);ID3D11DeviceContext_VSSetConstantBuffers(ctx,0,1,&vconst);ID3D11DeviceContext_PSSetConstantBuffers(ctx,0,1,&pconst);
 ID3D11DeviceContext_PSSetShaderResources(ctx,0,2,textures);ID3D11DeviceContext_PSSetSamplers(ctx,0,1,&sampler);ID3D11DeviceContext_PSSetSamplers(ctx,1,1,&sampler);
 ID3D11DeviceContext_Draw(ctx,4,0);ID3D11DeviceContext_CopyResource(ctx,(ID3D11Resource*)readback,(ID3D11Resource*)target);
 CHECK(SUCCEEDED(ID3D11DeviceContext_Map(ctx,(ID3D11Resource*)readback,0,D3D11_MAP_READ,0,&map)));
 for(int x=0;x<4;x++){BYTE *p=(BYTE*)map.pData+map.RowPitch+x*4;int expected=(204*mask[x*4+3]+127)/255;CHECK(p[0]==64&&p[1]==128&&p[2]==192);CHECK(abs(p[3]-expected)<=1);}
 ID3D11DeviceContext_Unmap(ctx,(ID3D11Resource*)readback,0);
 /* Clipping must interpolate the mask's coordinates independently of T0. */
 {ClipVert a,b,c;OutputVertex out;nv_clip_from_out(&verts[0],&a);nv_clip_from_out(&verts[3],&b);nv_clip_lerp(&a,&b,.25f,&c);nv_clip_to_out(&c,&out);CHECK(fabsf(out.u-.5f)<1e-6f&&fabsf(out.v-.5f)<1e-6f);CHECK(fabsf(out.u1-.25f)<1e-6f&&fabsf(out.v1-.25f)<1e-6f);}
 puts("PASS: reflection RGB preserved; independent mask UVs produce alpha 0/51/102/204; clipping preserves both UV sets.");return 0;
}
