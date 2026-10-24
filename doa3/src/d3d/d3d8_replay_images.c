#define COBJMACROS
#include "d3d8_replay_images.h"
#include <stdlib.h>
#include <string.h>

struct D3D8ReplayImages {
    DWORD owner;
    uint64_t epoch;
    unsigned count;
    int poisoned;
    ID3D11Device *device;
    ID3D11DeviceContext *context;
    ID3D11Texture2D *original[D3D8_REPLAY_IMAGE_LIMIT];
    ID3D11Texture2D *copy[D3D8_REPLAY_IMAGE_LIMIT];
};

int d3d8_replay_images_destroy(D3D8ReplayImages *s)
{
    unsigned i;
    if (!s) return 1;
    if (s->owner != GetCurrentThreadId()) return 0;
    for (i = 0; i < s->count; i++) {
        if (s->copy[i]) ID3D11Texture2D_Release(s->copy[i]);
        if (s->original[i]) ID3D11Texture2D_Release(s->original[i]);
    }
    if (s->context) ID3D11DeviceContext_Release(s->context);
    if (s->device) ID3D11Device_Release(s->device);
    free(s);
    return 1;
}

D3D8ReplayImages *d3d8_replay_images_capture(ID3D11Device *dev,
    ID3D11DeviceContext *ctx, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count)
{
    D3D8ReplayImages *s;
    ID3D11Device *owner = NULL;
    unsigned i;
    if (!dev || !ctx || !epoch || !images || !count || count > D3D8_REPLAY_IMAGE_LIMIT ||
        ID3D11DeviceContext_GetType(ctx) != D3D11_DEVICE_CONTEXT_IMMEDIATE ||
        FAILED(ID3D11Device_GetDeviceRemovedReason(dev))) return NULL;
    ID3D11DeviceContext_GetDevice(ctx, &owner);
    if (owner) ID3D11Device_Release(owner);
    if (owner != dev) return NULL;
    s = (D3D8ReplayImages *)calloc(1, sizeof(*s));
    if (!s) return NULL;
    s->owner = GetCurrentThreadId(); s->epoch = epoch; s->count = count;
    s->device = dev; ID3D11Device_AddRef(dev);
    s->context = ctx; ID3D11DeviceContext_AddRef(ctx);
    /* Allocate/validate every copy before issuing any GPU operations. */
    for (i = 0; i < count; i++) {
        D3D11_TEXTURE2D_DESC d;
        if (!images[i]) continue;
        ID3D11Texture2D_GetDevice(images[i], &owner);
        if (owner) ID3D11Device_Release(owner);
        if (owner != dev) goto fail;
        ID3D11Texture2D_GetDesc(images[i], &d);
        if (d.Usage != D3D11_USAGE_DEFAULT || d.MipLevels != 1 ||
            d.ArraySize != 1 || d.SampleDesc.Count != 1 || d.CPUAccessFlags ||
            d.MiscFlags || !d.Width || !d.Height ||
            (d.Format != DXGI_FORMAT_B8G8R8A8_UNORM &&
             d.Format != DXGI_FORMAT_R8G8B8A8_UNORM &&
             d.Format != DXGI_FORMAT_D24_UNORM_S8_UINT &&
             d.Format != DXGI_FORMAT_D16_UNORM)) goto fail;
        if (FAILED(ID3D11Device_CreateTexture2D(dev, &d, NULL, &s->copy[i]))) goto fail;
        s->original[i] = images[i]; ID3D11Texture2D_AddRef(images[i]);
    }
    for (i = 0; i < count; i++) if (images[i])
        ID3D11DeviceContext_CopyResource(ctx, (ID3D11Resource *)s->copy[i], (ID3D11Resource *)images[i]);
    if (FAILED(ID3D11Device_GetDeviceRemovedReason(dev))) goto fail;
    return s;
fail:
    d3d8_replay_images_destroy(s);
    return NULL;
}

int d3d8_replay_images_validate(const D3D8ReplayImages *s,
    ID3D11DeviceContext *ctx, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count)
{
    unsigned i;
    if (!s || !images || s->poisoned || s->owner != GetCurrentThreadId() ||
        ctx != s->context || epoch != s->epoch || count != s->count ||
        FAILED(ID3D11Device_GetDeviceRemovedReason(s->device))) return 0;
    for (i = 0; i < count; i++) if (images[i] != s->original[i]) return 0;
    return 1;
}

int d3d8_replay_images_restore(D3D8ReplayImages *s,
    ID3D11DeviceContext *ctx, uint64_t epoch,
    ID3D11Texture2D *const *images, unsigned count)
{
    unsigned i;
    if (!d3d8_replay_images_validate(s, ctx, epoch, images, count)) return 0;
    for (i = 0; i < count; i++) if (images[i])
        ID3D11DeviceContext_CopyResource(ctx, (ID3D11Resource *)images[i], (ID3D11Resource *)s->copy[i]);
    if (FAILED(ID3D11Device_GetDeviceRemovedReason(s->device))) { s->poisoned = 1; return -1; }
    return 1;
}

int d3d8_replay_images_equal(const D3D8ReplayImages *a,const D3D8ReplayImages *b)
{
    if (!a || !b || a->owner!=GetCurrentThreadId() || b->owner!=GetCurrentThreadId() ||
        a->poisoned || b->poisoned || a->device!=b->device || a->context!=b->context ||
        a->epoch!=b->epoch || a->count!=b->count ||
        FAILED(ID3D11Device_GetDeviceRemovedReason(a->device))) return 0;
    for (unsigned i=0;i<a->count;++i) if (a->original[i]!=b->original[i]) return 0;
    for (unsigned i=0;i<a->count;++i) {
        ID3D11Texture2D *staging[2]={0};
        D3D11_MAPPED_SUBRESOURCE maps[2]={{0}};
        D3D11_TEXTURE2D_DESC d;
        int mapped[2]={0},ok=0;
        if (!a->copy[i] && !b->copy[i]) continue;
        if (!a->copy[i] || !b->copy[i]) return 0;
        ID3D11Texture2D_GetDesc(a->copy[i],&d);
        unsigned row_bytes=d.Width*(d.Format==DXGI_FORMAT_D16_UNORM ? 2u:4u);
        d.Usage=D3D11_USAGE_STAGING;d.BindFlags=0;d.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
        if (d.Format==DXGI_FORMAT_D24_UNORM_S8_UINT) d.Format=DXGI_FORMAT_R24G8_TYPELESS;
        if (d.Format==DXGI_FORMAT_D16_UNORM) d.Format=DXGI_FORMAT_R16_TYPELESS;
        for (unsigned p=0;p<2;++p) {
            if (FAILED(ID3D11Device_CreateTexture2D(a->device,&d,NULL,&staging[p]))) goto cleanup;
            ID3D11DeviceContext_CopyResource(a->context,(ID3D11Resource *)staging[p],
                (ID3D11Resource *)(p ? b->copy[i]:a->copy[i]));
        }
        for (unsigned p=0;p<2;++p) {
            if (FAILED(ID3D11DeviceContext_Map(a->context,(ID3D11Resource *)staging[p],0,
                D3D11_MAP_READ,0,&maps[p]))) goto cleanup;
            mapped[p]=1;
            if (!maps[p].pData || maps[p].RowPitch<row_bytes) goto cleanup;
        }
        for (unsigned y=0;y<d.Height;++y)
            if (memcmp((const unsigned char *)maps[0].pData+(size_t)y*maps[0].RowPitch,
                       (const unsigned char *)maps[1].pData+(size_t)y*maps[1].RowPitch,row_bytes)) goto cleanup;
        ok=SUCCEEDED(ID3D11Device_GetDeviceRemovedReason(a->device));
cleanup:
        for (unsigned p=0;p<2;++p) {
            if (mapped[p]) ID3D11DeviceContext_Unmap(a->context,(ID3D11Resource *)staging[p],0);
            if (staging[p]) ID3D11Texture2D_Release(staging[p]);
        }
        if (!ok) return 0;
    }
    return 1;
}
