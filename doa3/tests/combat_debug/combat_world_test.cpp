#include "../../src/ui/combat_world.h"
#include "../../src/game/combat_debug.h"
#include <wrl/client.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
using Microsoft::WRL::ComPtr;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"Failed line %d: %s (%s)\n",__LINE__,#x,combat_world_error()); std::exit(1); } } while(0)
static CombatSnapshot snapshot;
static unsigned flags=COMBAT_SLIPPERY_GROUND;
extern "C" unsigned combat_debug_flags(void) { return flags; }
extern "C" int combat_debug_snapshot(CombatSnapshot *out) { *out=snapshot; return 1; }
extern "C" unsigned combat_debug_boundary_flag(unsigned) { return COMBAT_WALLS; }
static void quad(unsigned index,float x0,float x1,float z) {
    const float points[4][3]={{x0,-1,z},{x1,-1,z},{x1,1,z},{x0,1,z}};
    std::memcpy(snapshot.floors[index].points,points,sizeof(points));
}
static unsigned pixel(ID3D11Device *dev,ID3D11DeviceContext *ctx,ID3D11Resource *source,UINT x,UINT y) {
    ComPtr<ID3D11Texture2D> texture; CHECK(SUCCEEDED(source->QueryInterface(IID_PPV_ARGS(&texture))));
    D3D11_TEXTURE2D_DESC desc; texture->GetDesc(&desc);
    desc.Usage=D3D11_USAGE_STAGING; desc.BindFlags=0; desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    ComPtr<ID3D11Texture2D> staging; CHECK(SUCCEEDED(dev->CreateTexture2D(&desc,nullptr,&staging)));
    ctx->CopyResource(staging.Get(),source);
    D3D11_MAPPED_SUBRESOURCE mapped; CHECK(SUCCEEDED(ctx->Map(staging.Get(),0,D3D11_MAP_READ,0,&mapped)));
    unsigned value=*(unsigned *)((char *)mapped.pData+y*mapped.RowPitch+x*4);
    ctx->Unmap(staging.Get(),0); return value;
}
int main() {
    ComPtr<ID3D11Device> dev; ComPtr<ID3D11DeviceContext> ctx;
    CHECK(SUCCEEDED(D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_WARP,nullptr,0,nullptr,0,D3D11_SDK_VERSION,&dev,nullptr,&ctx)));
    snapshot.matrix[0]=snapshot.matrix[5]=snapshot.matrix[10]=snapshot.matrix[15]=1;
    for (UINT size: {8u,16u}) {
        D3D11_TEXTURE2D_DESC desc={}; desc.Width=desc.Height=size; desc.ArraySize=desc.MipLevels=1;
        desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM; desc.SampleDesc.Count=1;
        desc.BindFlags=D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
        ComPtr<ID3D11Texture2D> scene,zbuffer; ComPtr<ID3D11RenderTargetView> rtv;
        ComPtr<ID3D11DepthStencilView> dsv;
        CHECK(SUCCEEDED(dev->CreateTexture2D(&desc,nullptr,&scene)));
        CHECK(SUCCEEDED(dev->CreateRenderTargetView(scene.Get(),nullptr,&rtv)));
        desc.Format=DXGI_FORMAT_D24_UNORM_S8_UINT; desc.BindFlags=D3D11_BIND_DEPTH_STENCIL;
        CHECK(SUCCEEDED(dev->CreateTexture2D(&desc,nullptr,&zbuffer)));
        CHECK(SUCCEEDED(dev->CreateDepthStencilView(zbuffer.Get(),nullptr,&dsv)));
        FLOAT black[4]={0,0,0,1}; ctx->ClearRenderTargetView(rtv.Get(),black);
        ctx->ClearDepthStencilView(dsv.Get(),D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,.5f,37);
        UINT original_depth=pixel(dev.Get(),ctx.Get(),zbuffer.Get(),1,1);
        ID3D11RenderTargetView *target=rtv.Get(); ctx->OMSetRenderTargets(1,&target,dsv.Get());
        D3D11_VIEWPORT vp={1,2,3,4,.1f,.9f}; ctx->RSSetViewports(1,&vp);
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        snapshot.floor_count=2; quad(0,-1,0,.75f); quad(1,0,1,.25f);
        auto *result=combat_world_render(dev.Get(),ctx.Get(),scene.Get(),dsv.Get()); CHECK(result);
        ComPtr<ID3D11Resource> output; result->GetResource(&output);
        CHECK(pixel(dev.Get(),ctx.Get(),output.Get(),1,size/2)==0xFF000000); // behind scene
        UINT visible=pixel(dev.Get(),ctx.Get(),output.Get(),size-2,size/2);
        CHECK((visible&0xFFFFFF)!=0); // in front of scene
        CHECK(pixel(dev.Get(),ctx.Get(),scene.Get(),size-2,size/2)==0xFF000000); // source untouched
        CHECK(pixel(dev.Get(),ctx.Get(),zbuffer.Get(),size-2,size/2)==original_depth); // no depth/stencil writes
        ComPtr<ID3D11RenderTargetView> restored; ComPtr<ID3D11DepthStencilView> restored_depth;
        ctx->OMGetRenderTargets(1,&restored,&restored_depth);
        CHECK(restored.Get()==rtv.Get() && restored_depth.Get()==dsv.Get());
        D3D11_VIEWPORT after; UINT count=1; ctx->RSGetViewports(&count,&after);
        CHECK(count==1 && std::memcmp(&vp,&after,sizeof(vp))==0);
        D3D11_PRIMITIVE_TOPOLOGY topology; ctx->IAGetPrimitiveTopology(&topology);
        CHECK(topology==D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        result=combat_world_render(dev.Get(),ctx.Get(),scene.Get(),dsv.Get()); CHECK(result);
        CHECK(pixel(dev.Get(),ctx.Get(),output.Get(),size-2,size/2)==visible); // no repeated tint accumulation
        flags=0; CHECK(!combat_world_render(dev.Get(),ctx.Get(),scene.Get(),dsv.Get()));
        flags=COMBAT_SLIPPERY_GROUND;
        CHECK(!combat_world_render(dev.Get(),ctx.Get(),scene.Get(),nullptr));
        // D3D clips a wholly behind-camera surface rather than projecting it across the screen.
        snapshot.floor_count=1; quad(0,-1,1,-.5f);
        result=combat_world_render(dev.Get(),ctx.Get(),scene.Get(),dsv.Get()); CHECK(result);
        CHECK(pixel(dev.Get(),ctx.Get(),output.Get(),size-2,size/2)==0xFF000000);
        ctx->OMSetRenderTargets(0,nullptr,nullptr);
    }
    combat_world_shutdown();
    std::puts("Hazard depth occlusion, read-only scene buffers, state restoration, clipping, resize and repeated presentation passed.");
}
