/* Exercise the production resource implementation and read every mip back
 * from D3D11. --hardware also measures paired scene-sized creation batches. */
#include "d3d8_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
#define HR(x) CHECK(SUCCEEDED(x))
static ID3D11Device *device;
static ID3D11DeviceContext *context;
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return context; }
IDirect3DDevice8 *xbox_GetD3DDevice(void) { return NULL; }

static BYTE value(UINT level, UINT row, UINT byte, UINT seed)
{ return (BYTE)(level * 39 + row * 17 + byte * 13 + seed); }
static void fill(IDirect3DTexture8 *iface, UINT l, UINT seed)
{
    D3D8Texture *t = (D3D8Texture *)iface;
    D3DLOCKED_RECT lock;
    UINT rows = d3d8_format_is_compressed(t->d3d8_format) ? (t->level_h[l]+3)/4 : t->level_h[l];
    HR(iface->lpVtbl->LockRect(iface, l, &lock, NULL, 0));
    for (UINT r=0; r<rows; ++r)
        for (UINT b=0; b<t->level_pitch[l]; ++b)
            ((BYTE *)lock.pBits)[r*lock.Pitch+b] = value(l,r,b,seed);
    HR(iface->lpVtbl->UnlockRect(iface,l));
}
static void verify(IDirect3DTexture8 *iface, UINT seed, int changed_level)
{
    D3D8Texture *t=(D3D8Texture *)iface;
    D3D11_TEXTURE2D_DESC desc;
    ID3D11Texture2D *readback;
    CHECK(t->d3d11_texture && t->srv);
    ID3D11Texture2D_GetDesc(t->d3d11_texture,&desc);
    desc.Usage=D3D11_USAGE_STAGING; desc.BindFlags=0; desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    HR(ID3D11Device_CreateTexture2D(device,&desc,NULL,&readback));
    ID3D11DeviceContext_CopyResource(context,(ID3D11Resource *)readback,(ID3D11Resource *)t->d3d11_texture);
    for (UINT l=0; l<t->levels; ++l) {
        D3D11_MAPPED_SUBRESOURCE mapped;
        UINT rows=d3d8_format_is_compressed(t->d3d8_format) ? (t->level_h[l]+3)/4 : t->level_h[l];
        HR(ID3D11DeviceContext_Map(context,(ID3D11Resource *)readback,l,D3D11_MAP_READ,0,&mapped));
        for (UINT r=0; r<rows; ++r)
            for (UINT b=0; b<t->level_pitch[l]; ++b)
                CHECK(((BYTE *)mapped.pData)[r*mapped.RowPitch+b] == value(l,r,b,seed + ((int)l==changed_level ? 1 : 0)));
        ID3D11DeviceContext_Unmap(context,(ID3D11Resource *)readback,l);
    }
    ID3D11Texture2D_Release(readback);
}
static void check_format(D3DFORMAT format)
{
    for (int deferred=0; deferred<2; ++deferred) {
        IDirect3DTexture8 *t;
        if (deferred) HR(d3d8_CreateUploadTexture(32,16,6,format,&t));
        else HR(d3d8_CreateTextureImpl(32,16,6,0,format,&t));
        d3d8_TextureSetLinearData(t,TRUE);
        if (deferred) {
            CHECK(!((D3D8Texture *)t)->d3d11_texture);
            CHECK(FAILED(d3d8_TextureFinishUpload(t)));
        }
        for (UINT l=0; l<6; ++l) fill(t,l,11);
        HR(d3d8_TextureFinishUpload(t));
        HR(d3d8_TextureFinishUpload(t)); /* idempotent, never recreates */
        verify(t,11,-1);
        ID3D11Texture2D *original=((D3D8Texture *)t)->d3d11_texture;
        fill(t,2,12);
        HR(d3d8_TextureFinishUpload(t));
        CHECK(((D3D8Texture *)t)->d3d11_texture==original);
        verify(t,11,2); /* an update must preserve every other mip */
        t->lpVtbl->Release(t);
    }
}
static double batch(int deferred)
{
    IDirect3DTexture8 *textures[64];
    ID3D11Query *query;
    D3D11_QUERY_DESC qd={D3D11_QUERY_EVENT,0};
    LARGE_INTEGER start,end,freq;
    HR(ID3D11Device_CreateQuery(device,&qd,&query));
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (UINT i=0;i<64;++i) {
        IDirect3DTexture8 *t;
        D3DFORMAT fmt=i%2 ? D3DFMT_DXT1 : D3DFMT_A8R8G8B8;
        if (deferred) HR(d3d8_CreateUploadTexture(128,128,8,fmt,&t));
        else HR(d3d8_CreateTextureImpl(128,128,8,0,fmt,&t));
        d3d8_TextureSetLinearData(t,TRUE);
        for (UINT l=0;l<8;++l) fill(t,l,i);
        HR(d3d8_TextureFinishUpload(t));
        textures[i]=t;
    }
    QueryPerformanceCounter(&end);
    ID3D11DeviceContext_End(context,(ID3D11Asynchronous *)query);
    ID3D11DeviceContext_Flush(context);
    while (ID3D11DeviceContext_GetData(context,(ID3D11Asynchronous *)query,NULL,0,0)==S_FALSE) Sleep(0);
    ID3D11Query_Release(query);
    for (UINT i=0;i<64;++i) textures[i]->lpVtbl->Release(textures[i]);
    return (end.QuadPart-start.QuadPart)*1000.0/freq.QuadPart;
}
int main(int argc, char **argv)
{
    int hardware=argc>1 && strcmp(argv[1],"--hardware")==0;
    HR(D3D11CreateDevice(NULL,hardware ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_WARP,
        NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,&context));
    check_format(D3DFMT_A8R8G8B8); check_format(D3DFMT_R5G6B5);
    check_format(D3DFMT_A8); check_format(D3DFMT_DXT1);
    check_format(D3DFMT_DXT3); check_format(D3DFMT_DXT5);
    IDirect3DTexture8 *unused;
    HR(d3d8_CreateUploadTexture(32,32,6,D3DFMT_A8R8G8B8,&unused));
    unused->lpVtbl->Release(unused); /* release before publication */
    if (hardware) {
        batch(0); batch(1);
        for (int i=0;i<8;++i) {
            double a,b;
            if (i%2) { b=batch(1); a=batch(0); } else { a=batch(0); b=batch(1); }
            printf("batch %d: separate %.3f ms, initial data %.3f ms\n",i,a,b);
        }
    }
    ID3D11DeviceContext_ClearState(context);
    ID3D11DeviceContext_Release(context);
    ID3D11Device_Release(device);
    puts("Texture upload mip readback and update tests passed.");
    return 0;
}
