#include "combat_world.h"
#include "../game/combat_debug.h"
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using Microsoft::WRL::ComPtr;
namespace {
struct Vertex { float clip[4]; unsigned color; };
struct Resources {
    ID3D11Device *owner = nullptr;
    ComPtr<ID3D11VertexShader> vs;
    ComPtr<ID3D11PixelShader> ps;
    ComPtr<ID3D11InputLayout> layout;
    ComPtr<ID3D11RasterizerState> raster;
    ComPtr<ID3D11BlendState> blend;
    ComPtr<ID3D11DepthStencilState> depth;
    ComPtr<ID3D11Buffer> vertices;
    ComPtr<ID3D11Texture2D> composite;
    ComPtr<ID3D11RenderTargetView> rtv;
    ComPtr<ID3D11ShaderResourceView> srv;
    UINT width = 0, height = 0, capacity = 0;
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    bool failed = false;
} gpu;
char error[128];
std::vector<Vertex> vertices;
constexpr unsigned color(unsigned r, unsigned g, unsigned b, unsigned a) {
    return r | (g<<8) | (b<<16) | (a<<24);
}
bool checked(HRESULT hr) {
    if (SUCCEEDED(hr)) return true;
    std::snprintf(error,sizeof(error),"Hazard rendering unavailable (0x%08lX)", (unsigned long)hr);
    std::fprintf(stderr,"[HAZARDS] %s\n",error);
    gpu.failed = true;
    return false;
}
bool prepare(ID3D11Device *device, const D3D11_TEXTURE2D_DESC &scene, UINT count) {
    if (gpu.owner != device) { gpu = Resources{}; gpu.owner = device; error[0] = 0; }
    if (gpu.failed) return false;
    if (!gpu.vs) {
        const char source[] =
            "struct V { float4 p:POSITION; float4 c:COLOR; };"
            "struct P { float4 p:SV_POSITION; float4 c:COLOR; };"
            "P vs(V v) { P o; o.p=v.p; o.c=v.c; return o; }"
            "float4 ps(P p):SV_TARGET { return p.c; }";
        ComPtr<ID3DBlob> vs, ps, messages;
        if (!checked(D3DCompile(source,sizeof(source)-1,"hazards",nullptr,nullptr,"vs","vs_4_0",0,0,&vs,&messages)) ||
            !checked(D3DCompile(source,sizeof(source)-1,"hazards",nullptr,nullptr,"ps","ps_4_0",0,0,&ps,&messages))) return false;
        if (!checked(device->CreateVertexShader(vs->GetBufferPointer(),vs->GetBufferSize(),nullptr,&gpu.vs)) ||
            !checked(device->CreatePixelShader(ps->GetBufferPointer(),ps->GetBufferSize(),nullptr,&gpu.ps))) return false;
        const D3D11_INPUT_ELEMENT_DESC elements[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,16,D3D11_INPUT_PER_VERTEX_DATA,0}
        };
        if (!checked(device->CreateInputLayout(elements,2,vs->GetBufferPointer(),vs->GetBufferSize(),&gpu.layout))) return false;
        D3D11_RASTERIZER_DESC raster = {};
        raster.FillMode = D3D11_FILL_SOLID; raster.CullMode = D3D11_CULL_NONE;
        raster.DepthClipEnable = TRUE;
        // Small toward-camera bias makes coplanar boundaries readable while
        // retaining occlusion by characters and foreground stage geometry.
        raster.DepthBias = -2;
        if (!checked(device->CreateRasterizerState(&raster,&gpu.raster))) return false;
        D3D11_DEPTH_STENCIL_DESC depth = {};
        depth.DepthEnable = TRUE; depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        if (!checked(device->CreateDepthStencilState(&depth,&gpu.depth))) return false;
        D3D11_BLEND_DESC blend = {};
        auto &b = blend.RenderTarget[0]; b.BlendEnable = TRUE;
        b.SrcBlend = D3D11_BLEND_SRC_ALPHA; b.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        b.BlendOp = b.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        b.SrcBlendAlpha = D3D11_BLEND_ONE; b.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        b.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        if (!checked(device->CreateBlendState(&blend,&gpu.blend))) return false;
    }
    if (!gpu.composite || gpu.width!=scene.Width || gpu.height!=scene.Height || gpu.format!=scene.Format) {
        gpu.srv.Reset(); gpu.rtv.Reset(); gpu.composite.Reset();
        D3D11_TEXTURE2D_DESC desc = scene;
        desc.Usage = D3D11_USAGE_DEFAULT; desc.CPUAccessFlags = desc.MiscFlags = 0;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        if (!checked(device->CreateTexture2D(&desc,nullptr,&gpu.composite)) ||
            !checked(device->CreateRenderTargetView(gpu.composite.Get(),nullptr,&gpu.rtv)) ||
            !checked(device->CreateShaderResourceView(gpu.composite.Get(),nullptr,&gpu.srv))) return false;
        gpu.width=scene.Width; gpu.height=scene.Height; gpu.format=scene.Format;
    }
    if (count>gpu.capacity) {
        gpu.vertices.Reset(); gpu.capacity=0;
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth=(count+1024)*sizeof(Vertex); desc.Usage=D3D11_USAGE_DYNAMIC;
        desc.BindFlags=D3D11_BIND_VERTEX_BUFFER; desc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
        if (!checked(device->CreateBuffer(&desc,nullptr,&gpu.vertices))) return false;
        gpu.capacity=count+1024;
    }
    return true;
}
struct Geometry {
    const float *matrix;
    void triangle(const float *a, const float *b, const float *c, unsigned tint) const {
        Vertex triangle[3]; const float *points[] = {a,b,c};
        for (unsigned i=0;i<3;++i) {
            for (unsigned j=0;j<4;++j) {
                triangle[i].clip[j]=points[i][0]*matrix[j]+points[i][1]*matrix[4+j]+
                    points[i][2]*matrix[8+j]+matrix[12+j];
                if (!std::isfinite(triangle[i].clip[j])) return;
            }
            triangle[i].color=tint;
        }
        // D3D clips homogeneous coordinates, including near-plane crossings.
        vertices.insert(vertices.end(),triangle,triangle+3);
    }
};
void geometry(const CombatSnapshot &snapshot, unsigned flags) {
    Geometry view = {snapshot.matrix};
    const unsigned orange=color(255,145,25,110);
    if (flags & COMBAT_SLIPPERY_GROUND) for (unsigned i=0;i<snapshot.floor_count;++i) {
        const auto &q=snapshot.floors[i];
        view.triangle(q.points[0],q.points[1],q.points[2],color(45,140,255,110));
        view.triangle(q.points[0],q.points[2],q.points[3],color(45,140,255,110));
    }
    for (unsigned i=0;i<snapshot.wall_count;++i) {
        const auto &q=snapshot.walls[i];
        unsigned kind=combat_debug_boundary_flag(q.type);
        if (!(flags & kind)) continue;
        unsigned tint=kind==COMBAT_SPECIAL_BOUNDARIES ? color(205,80,255,125) : orange;
        view.triangle(q.points[0],q.points[1],q.points[2],tint);
        view.triangle(q.points[0],q.points[2],q.points[3],tint);
    }
    constexpr unsigned slices=24;
    if (flags & COMBAT_STAGE_OBJECTS) for (unsigned i=0;i<snapshot.object_count;++i) {
        const auto &o=snapshot.objects[i];
        float bottom[slices][3],top[slices][3],center[3];
        for (unsigned k=0;k<3;++k) center[k]=o.base[k]+o.axis[k]*o.height/o.axis[1];
        for (unsigned j=0;j<slices;++j) {
            float angle=j*6.28318530718f/slices;
            bottom[j][0]=o.base[0]+std::cos(angle)*o.radius_x;
            bottom[j][1]=o.base[1];
            bottom[j][2]=o.base[2]+std::sin(angle)*o.radius_z;
            for (unsigned k=0;k<3;++k) top[j][k]=bottom[j][k]+center[k]-o.base[k];
        }
        for (unsigned j=0;j<slices;++j) {
            unsigned next=(j+1)%slices;
            view.triangle(bottom[j],bottom[next],top[next],orange);
            view.triangle(bottom[j],top[next],top[j],orange);
            view.triangle(top[j],top[next],center,orange);
            view.triangle(bottom[next],bottom[j],o.base,orange);
        }
    }
}
}

extern "C" const char *combat_world_error(void) { return error; }
extern "C" void combat_world_shutdown(void) {
    gpu=Resources{}; vertices.clear(); error[0]=0;
}
extern "C" ID3D11ShaderResourceView *combat_world_render(ID3D11Device *device,
    ID3D11DeviceContext *ctx, ID3D11Texture2D *scene, ID3D11DepthStencilView *depth) {
    unsigned flags=combat_debug_flags();
    if (!(flags & COMBAT_HAZARDS) || !device || !ctx || !scene || !depth) return nullptr;
    CombatSnapshot snapshot;
    if (!combat_debug_snapshot(&snapshot)) return nullptr;
    vertices.clear(); geometry(snapshot,flags);
    if (vertices.empty()) return nullptr;
    D3D11_TEXTURE2D_DESC desc; scene->GetDesc(&desc);
    if (desc.SampleDesc.Count!=1 || !prepare(device,desc,(UINT)vertices.size())) return nullptr;
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (!checked(ctx->Map(gpu.vertices.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&mapped))) return nullptr;
    std::memcpy(mapped.pData,vertices.data(),vertices.size()*sizeof(Vertex));
    ctx->Unmap(gpu.vertices.Get(),0);

    // Save every binding changed below. Guest state caches assume rendering
    // helpers leave the context exactly as they found it.
    ID3D11RenderTargetView *targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]={};
    ComPtr<ID3D11DepthStencilView> old_depth;
    ComPtr<ID3D11InputLayout> layout; ComPtr<ID3D11Buffer> vb;
    D3D11_PRIMITIVE_TOPOLOGY topology; UINT stride,offset;
    ComPtr<ID3D11VertexShader> vs; ComPtr<ID3D11PixelShader> ps;
    ComPtr<ID3D11GeometryShader> gs; ComPtr<ID3D11HullShader> hs; ComPtr<ID3D11DomainShader> ds;
    ComPtr<ID3D11RasterizerState> raster; ComPtr<ID3D11BlendState> blend;
    ComPtr<ID3D11DepthStencilState> depth_state; FLOAT factor[4]; UINT mask,ref;
    D3D11_VIEWPORT vps[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    UINT nvp=D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    ctx->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,targets,&old_depth);
    ctx->IAGetInputLayout(&layout); ctx->IAGetPrimitiveTopology(&topology);
    ctx->IAGetVertexBuffers(0,1,&vb,&stride,&offset);
    ctx->VSGetShader(&vs,nullptr,nullptr); ctx->PSGetShader(&ps,nullptr,nullptr);
    ctx->GSGetShader(&gs,nullptr,nullptr); ctx->HSGetShader(&hs,nullptr,nullptr); ctx->DSGetShader(&ds,nullptr,nullptr);
    ctx->RSGetState(&raster); ctx->RSGetViewports(&nvp,vps);
    ctx->OMGetBlendState(&blend,factor,&mask); ctx->OMGetDepthStencilState(&depth_state,&ref);

    ctx->OMSetRenderTargets(0,nullptr,nullptr);
    // A fresh copy avoids changing reflection/crossfade history, screenshots,
    // or accumulating translucent tint during repeated paused presentations.
    ctx->CopyResource(gpu.composite.Get(),scene);
    ID3D11RenderTargetView *target=gpu.rtv.Get();
    ctx->OMSetRenderTargets(1,&target,depth);
    D3D11_VIEWPORT viewport={0,0,(FLOAT)desc.Width,(FLOAT)desc.Height,0,1};
    ctx->RSSetViewports(1,&viewport); ctx->RSSetState(gpu.raster.Get());
    ctx->OMSetBlendState(gpu.blend.Get(),nullptr,0xFFFFFFFF);
    ctx->OMSetDepthStencilState(gpu.depth.Get(),0);
    UINT vertex_stride=sizeof(Vertex),zero=0; ID3D11Buffer *buffer=gpu.vertices.Get();
    ctx->IASetVertexBuffers(0,1,&buffer,&vertex_stride,&zero);
    ctx->IASetInputLayout(gpu.layout.Get()); ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->VSSetShader(gpu.vs.Get(),nullptr,0); ctx->PSSetShader(gpu.ps.Get(),nullptr,0);
    ctx->GSSetShader(nullptr,nullptr,0); ctx->HSSetShader(nullptr,nullptr,0); ctx->DSSetShader(nullptr,nullptr,0);
    ctx->Draw((UINT)vertices.size(),0);

    ctx->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,targets,old_depth.Get());
    for (auto *t:targets) if (t) t->Release();
    ctx->IASetInputLayout(layout.Get()); ctx->IASetPrimitiveTopology(topology);
    buffer=vb.Get(); ctx->IASetVertexBuffers(0,1,&buffer,&stride,&offset);
    ctx->VSSetShader(vs.Get(),nullptr,0); ctx->PSSetShader(ps.Get(),nullptr,0);
    ctx->GSSetShader(gs.Get(),nullptr,0); ctx->HSSetShader(hs.Get(),nullptr,0); ctx->DSSetShader(ds.Get(),nullptr,0);
    ctx->RSSetState(raster.Get()); ctx->RSSetViewports(nvp,vps);
    ctx->OMSetBlendState(blend.Get(),factor,mask); ctx->OMSetDepthStencilState(depth_state.Get(),ref);
    return gpu.srv.Get();
}
