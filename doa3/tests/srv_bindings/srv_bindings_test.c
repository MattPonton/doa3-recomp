#include "d3d8_internal.h"
#include "d3d8_srv_bindings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
#define HR(x) CHECK(SUCCEEDED(x))
static D3D8SrvBindings g_srv_bindings;
unsigned g_d3d8_bind_epoch = 1;
static unsigned g_d3d_settexture_count;
static IDirect3DBaseTexture8 *g_cur_textures[4];
static struct { ID3D11DeviceContext *d3d11_context; DWORD tss[4][32]; } g_device_state;
#include "binding_impl.h"

static void expect(unsigned stage, ID3D11ShaderResourceView *view)
{
    ID3D11ShaderResourceView *bound=NULL;
    ID3D11DeviceContext_PSGetShaderResources(g_device_state.d3d11_context,stage,1,&bound);
    CHECK(bound==view);
    if(bound) ID3D11ShaderResourceView_Release(bound);
}

static double measure(ID3D11DeviceContext *ctx,ID3D11ShaderResourceView *view,int cached,int alternating)
{
    LARGE_INTEGER first,last,frequency;
    QueryPerformanceFrequency(&frequency); QueryPerformanceCounter(&first);
    for(unsigned i=0;i<200000;i++) {
        ID3D11ShaderResourceView *current=(alternating && (i&1)) ? NULL : view;
        if(cached) d3d8_srv_bind(&g_srv_bindings,ctx,g_d3d8_bind_epoch,0,current);
        else ID3D11DeviceContext_PSSetShaderResources(ctx,0,1,&current);
    }
    ID3D11DeviceContext_Flush(ctx);
    QueryPerformanceCounter(&last);
    return 1000.0*(last.QuadPart-first.QuadPart)/frequency.QuadPart;
}

int main(int argc,char **argv)
{
    int benchmark=argc>1 && !strcmp(argv[1],"--benchmark");
    ID3D11Device *device;
    ID3D11DeviceContext *ctx;
    ID3D11Texture2D *resource;
    ID3D11RenderTargetView *target;
    D3D8Texture texture={0};
    D3D11_TEXTURE2D_DESC desc={0};
    HR(D3D11CreateDevice(NULL,benchmark ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_WARP,
        NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,&ctx));
    g_device_state.d3d11_context=ctx;
    if(benchmark) {
        IDXGIDevice *dxgi; IDXGIAdapter *adapter; DXGI_ADAPTER_DESC info;
        HR(ID3D11Device_QueryInterface(device,&IID_IDXGIDevice,(void **)&dxgi));
        HR(IDXGIDevice_GetAdapter(dxgi,&adapter)); HR(IDXGIAdapter_GetDesc(adapter,&info));
        printf("Benchmark adapter: %ls\n",info.Description);
        IDXGIAdapter_Release(adapter); IDXGIDevice_Release(dxgi);
    }
    desc.Width=desc.Height=16; desc.MipLevels=desc.ArraySize=desc.SampleDesc.Count=1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BindFlags=D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
    HR(ID3D11Device_CreateTexture2D(device,&desc,NULL,&resource));
    HR(ID3D11Device_CreateShaderResourceView(device,(ID3D11Resource *)resource,NULL,&texture.srv));
    HR(ID3D11Device_CreateRenderTargetView(device,(ID3D11Resource *)resource,NULL,&target));
    for(unsigned stage=0;stage<4;stage++) {
        g_device_state.tss[stage][D3DTSS_COLOROP]=D3DTOP_DISABLE;
        HR(dev_SetTexture(NULL,stage,(IDirect3DBaseTexture8 *)&texture)); expect(stage,texture.srv);
        CHECK(g_device_state.tss[stage][D3DTSS_COLOROP]==D3DTOP_MODULATE);
        CHECK(!d3d8_srv_bind(&g_srv_bindings,ctx,g_d3d8_bind_epoch,stage,texture.srv));
        g_device_state.tss[stage][D3DTSS_COLOROP]=D3DTOP_SELECTARG1;
        HR(dev_SetTexture(NULL,stage,(IDirect3DBaseTexture8 *)&texture));
        CHECK(g_device_state.tss[stage][D3DTSS_COLOROP]==D3DTOP_SELECTARG1);
        HR(dev_SetTexture(NULL,stage,NULL)); expect(stage,NULL);
        CHECK(g_device_state.tss[stage][D3DTSS_COLOROP]==D3DTOP_DISABLE);
        CHECK(!d3d8_srv_bind(&g_srv_bindings,ctx,g_d3d8_bind_epoch,stage,NULL));
    }
    CHECK(dev_SetTexture(NULL,4,NULL)==E_INVALIDARG);
    HR(dev_SetTexture(NULL,0,(IDirect3DBaseTexture8 *)&texture));
    { ID3D11ShaderResourceView *none=NULL;
      ID3D11DeviceContext_PSSetShaderResources(ctx,0,1,&none); }
    ++g_d3d8_bind_epoch; /* movie, composition or external renderer */
    HR(dev_SetTexture(NULL,0,(IDirect3DBaseTexture8 *)&texture)); expect(0,texture.srv);
    g_srv_bindings.valid=0;
    ID3D11DeviceContext_OMSetRenderTargets(ctx,1,&target,NULL); expect(0,NULL);
    g_srv_bindings.valid=0;
    ID3D11DeviceContext_OMSetRenderTargets(ctx,0,NULL,NULL);
    HR(dev_SetTexture(NULL,0,(IDirect3DBaseTexture8 *)&texture)); expect(0,texture.srv);
    if(benchmark) {
        for(int alternating=0;alternating<2;alternating++) for(int i=-2;i<7;i++) {
            double old_ms,new_ms;
            if(i&1) { g_srv_bindings.valid=0; new_ms=measure(ctx,texture.srv,1,alternating); old_ms=measure(ctx,texture.srv,0,alternating); }
            else { old_ms=measure(ctx,texture.srv,0,alternating); g_srv_bindings.valid=0; new_ms=measure(ctx,texture.srv,1,alternating); }
            if(i>=0) printf("SRV %s: old %.3f ms, new %.3f ms / 200000 binds\n",alternating ? "alternating" : "unchanged",old_ms,new_ms);
        }
    }
    ID3D11DeviceContext_ClearState(ctx);
    ID3D11RenderTargetView_Release(target); ID3D11ShaderResourceView_Release(texture.srv);
    ID3D11Texture2D_Release(resource); ID3D11DeviceContext_Release(ctx); ID3D11Device_Release(device);
    puts("SRV cache hits, stage state, external binding and render-target hazard recovery pass");
    return 0;
}
