/**
 * D3D8 Resource Management - Vertex Buffers, Index Buffers, Textures
 *
 * Implements Xbox D3D8 resource creation and Lock/Unlock using D3D11.
 * Resources use system memory staging with UpdateSubresource on Unlock
 * for maximum compatibility with D3D8 Lock semantics.
 */

#include "d3d8_internal.h"
#include "d3d8_swizzle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ================================================================
 * Format conversion: Xbox D3DFORMAT → DXGI_FORMAT
 * ================================================================ */

DXGI_FORMAT d3d8_to_dxgi_format(D3DFORMAT fmt)
{
    switch (fmt) {
    case D3DFMT_A8R8G8B8:       return DXGI_FORMAT_B8G8R8A8_UNORM;
    case D3DFMT_X8R8G8B8:       return DXGI_FORMAT_B8G8R8X8_UNORM;
    case D3DFMT_R5G6B5:         return DXGI_FORMAT_B5G6R5_UNORM;
    case D3DFMT_A1R5G5B5:       return DXGI_FORMAT_B5G5R5A1_UNORM;
    case D3DFMT_DXT1:           return DXGI_FORMAT_BC1_UNORM;
    case D3DFMT_DXT3:           return DXGI_FORMAT_BC2_UNORM;
    case D3DFMT_DXT5:           return DXGI_FORMAT_BC3_UNORM;
    case D3DFMT_A8:             return DXGI_FORMAT_A8_UNORM;
    case D3DFMT_L8:             return DXGI_FORMAT_R8_UNORM;
    case D3DFMT_D24S8:          return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case D3DFMT_D16:            return DXGI_FORMAT_D16_UNORM;
    case D3DFMT_LIN_A8R8G8B8:   return DXGI_FORMAT_B8G8R8A8_UNORM;
    case D3DFMT_LIN_X8R8G8B8:   return DXGI_FORMAT_B8G8R8X8_UNORM;
    case D3DFMT_LIN_R5G6B5:     return DXGI_FORMAT_B5G6R5_UNORM;
    case D3DFMT_LIN_A1R5G5B5:   return DXGI_FORMAT_B5G5R5A1_UNORM;
    case D3DFMT_A4R4G4B4:       return DXGI_FORMAT_B4G4R4A4_UNORM;
    case D3DFMT_LIN_A4R4G4B4:   return DXGI_FORMAT_B4G4R4A4_UNORM;
    case D3DFMT_A8L8:           return DXGI_FORMAT_R8G8_UNORM;
    case D3DFMT_P8:             return DXGI_FORMAT_R8_UNORM;  /* palette handled separately */
    case D3DFMT_INDEX16:        return DXGI_FORMAT_R16_UINT;
    case D3DFMT_INDEX32:        return DXGI_FORMAT_R32_UINT;
    default:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}

UINT d3d8_format_bpp(D3DFORMAT fmt)
{
    switch (fmt) {
    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
    case D3DFMT_LIN_A8R8G8B8:
    case D3DFMT_LIN_X8R8G8B8:
    case D3DFMT_INDEX32:
        return 32;
    case D3DFMT_R5G6B5:
    case D3DFMT_A1R5G5B5:
    case D3DFMT_A4R4G4B4:
    case D3DFMT_LIN_R5G6B5:
    case D3DFMT_LIN_A1R5G5B5:
    case D3DFMT_LIN_A4R4G4B4:
    case D3DFMT_D16:
    case D3DFMT_INDEX16:
        return 16;
    case D3DFMT_A8:
    case D3DFMT_L8:
    case D3DFMT_P8:
        return 8;
    case D3DFMT_DXT1:  return 4;   /* 4 bits per pixel (BC1) */
    case D3DFMT_DXT3:
    case D3DFMT_DXT5:  return 8;   /* 8 bits per pixel (BC2/BC3) */
    case D3DFMT_D24S8: return 32;
    default: return 32;
    }
}

BOOL d3d8_format_is_compressed(D3DFORMAT fmt)
{
    return fmt == D3DFMT_DXT1 || fmt == D3DFMT_DXT3 || fmt == D3DFMT_DXT5;
}

UINT d3d8_row_pitch(D3DFORMAT fmt, UINT width)
{
    if (d3d8_format_is_compressed(fmt)) {
        UINT block_width = (width + 3) / 4;
        UINT block_bytes = (fmt == D3DFMT_DXT1) ? 8 : 16;
        return block_width * block_bytes;
    }
    return (width * d3d8_format_bpp(fmt)) / 8;
}

/* ================================================================
 * Vertex Buffer Implementation
 * ================================================================ */

static D3D8VertexBuffer *vb_from_iface(IDirect3DVertexBuffer8 *iface)
{
    return (D3D8VertexBuffer *)iface;
}

static HRESULT __stdcall vb_QueryInterface(IDirect3DVertexBuffer8 *self, const IID *riid, void **ppv)
{
    (void)self; (void)riid; (void)ppv;
    return E_NOINTERFACE;
}

static ULONG __stdcall vb_AddRef(IDirect3DVertexBuffer8 *self)
{
    D3D8VertexBuffer *vb = vb_from_iface(self);
    return (ULONG)InterlockedIncrement(&vb->ref_count);
}

static ULONG __stdcall vb_Release(IDirect3DVertexBuffer8 *self)
{
    D3D8VertexBuffer *vb = vb_from_iface(self);
    LONG ref = InterlockedDecrement(&vb->ref_count);
    if (ref <= 0) {
        if (vb->d3d11_buffer) ID3D11Buffer_Release(vb->d3d11_buffer);
        free(vb->sys_mem);
        free(vb);
    }
    return (ULONG)ref;
}

static HRESULT __stdcall vb_GetDevice(IDirect3DVertexBuffer8 *self, IDirect3DDevice8 **ppDevice)
{
    (void)self;
    *ppDevice = xbox_GetD3DDevice();
    return S_OK;
}

static DWORD __stdcall vb_SetPriority(IDirect3DVertexBuffer8 *self, DWORD Priority)
{
    (void)self; (void)Priority;
    return 0;
}

static DWORD __stdcall vb_GetPriority(IDirect3DVertexBuffer8 *self)
{
    (void)self;
    return 0;
}

static void __stdcall vb_PreLoad(IDirect3DVertexBuffer8 *self)
{
    (void)self;
}

static DWORD __stdcall vb_GetType(IDirect3DVertexBuffer8 *self)
{
    (void)self;
    return 3; /* D3DRTYPE_VERTEXBUFFER */
}

static HRESULT __stdcall vb_Lock(IDirect3DVertexBuffer8 *self, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags)
{
    D3D8VertexBuffer *vb = vb_from_iface(self);
    (void)SizeToLock; (void)Flags;

    if (!ppbData) return E_INVALIDARG;
    if (vb->locked) return E_FAIL;

    *ppbData = vb->sys_mem + OffsetToLock;
    vb->locked = TRUE;
    return S_OK;
}

static HRESULT __stdcall vb_Unlock(IDirect3DVertexBuffer8 *self)
{
    D3D8VertexBuffer *vb = vb_from_iface(self);
    if (!vb->locked) return E_FAIL;

    vb->locked = FALSE;
    vb->dirty = TRUE;

    /* Upload to GPU */
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    if (ctx && vb->d3d11_buffer) {
        ID3D11DeviceContext_UpdateSubresource(ctx,
            (ID3D11Resource *)vb->d3d11_buffer,
            0, NULL, vb->sys_mem, vb->size, 0);
        vb->dirty = FALSE;
    }
    return S_OK;
}

static HRESULT __stdcall vb_GetDesc(IDirect3DVertexBuffer8 *self, void *pDesc)
{
    (void)self; (void)pDesc;
    return E_NOTIMPL;
}

static const IDirect3DVertexBuffer8Vtbl g_vb_vtbl = {
    vb_QueryInterface,
    vb_AddRef,
    vb_Release,
    vb_GetDevice,
    vb_SetPriority,
    vb_GetPriority,
    vb_PreLoad,
    vb_GetType,
    vb_Lock,
    vb_Unlock,
    vb_GetDesc,
};

HRESULT d3d8_CreateVertexBufferImpl(UINT Length, DWORD Usage, DWORD FVF, IDirect3DVertexBuffer8 **ppVB)
{
    D3D8VertexBuffer *vb;
    D3D11_BUFFER_DESC bd;
    HRESULT hr;

    if (!ppVB) return E_INVALIDARG;

    vb = (D3D8VertexBuffer *)calloc(1, sizeof(*vb));
    if (!vb) return E_OUTOFMEMORY;

    vb->sys_mem = (BYTE *)calloc(1, Length);
    if (!vb->sys_mem) { free(vb); return E_OUTOFMEMORY; }

    /* Create D3D11 buffer */
    memset(&bd, 0, sizeof(bd));
    bd.ByteWidth = Length;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    hr = ID3D11Device_CreateBuffer(d3d8_GetD3D11Device(), &bd, NULL, &vb->d3d11_buffer);
    if (FAILED(hr)) {
        free(vb->sys_mem);
        free(vb);
        return hr;
    }

    vb->iface.lpVtbl = &g_vb_vtbl;
    vb->ref_count = 1;
    vb->size = Length;
    vb->fvf = FVF;
    vb->usage = Usage;

    *ppVB = &vb->iface;
    return S_OK;
}

/* ================================================================
 * Index Buffer Implementation
 * ================================================================ */

static D3D8IndexBuffer *ib_from_iface(IDirect3DIndexBuffer8 *iface)
{
    return (D3D8IndexBuffer *)iface;
}

static HRESULT __stdcall ib_QueryInterface(IDirect3DIndexBuffer8 *self, const IID *riid, void **ppv)
{
    (void)self; (void)riid; (void)ppv;
    return E_NOINTERFACE;
}

static ULONG __stdcall ib_AddRef(IDirect3DIndexBuffer8 *self)
{
    return (ULONG)InterlockedIncrement(&ib_from_iface(self)->ref_count);
}

static ULONG __stdcall ib_Release(IDirect3DIndexBuffer8 *self)
{
    D3D8IndexBuffer *ib = ib_from_iface(self);
    LONG ref = InterlockedDecrement(&ib->ref_count);
    if (ref <= 0) {
        if (ib->d3d11_buffer) ID3D11Buffer_Release(ib->d3d11_buffer);
        free(ib->sys_mem);
        free(ib);
    }
    return (ULONG)ref;
}

static HRESULT __stdcall ib_GetDevice(IDirect3DIndexBuffer8 *self, IDirect3DDevice8 **ppDevice)
{
    (void)self;
    *ppDevice = xbox_GetD3DDevice();
    return S_OK;
}

static DWORD __stdcall ib_SetPriority(IDirect3DIndexBuffer8 *self, DWORD Priority) { (void)self; (void)Priority; return 0; }
static DWORD __stdcall ib_GetPriority(IDirect3DIndexBuffer8 *self) { (void)self; return 0; }
static void  __stdcall ib_PreLoad(IDirect3DIndexBuffer8 *self) { (void)self; }
static DWORD __stdcall ib_GetType(IDirect3DIndexBuffer8 *self) { (void)self; return 4; /* D3DRTYPE_INDEXBUFFER */ }

static HRESULT __stdcall ib_Lock(IDirect3DIndexBuffer8 *self, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags)
{
    D3D8IndexBuffer *ib = ib_from_iface(self);
    (void)SizeToLock; (void)Flags;
    if (!ppbData) return E_INVALIDARG;
    if (ib->locked) return E_FAIL;
    *ppbData = ib->sys_mem + OffsetToLock;
    ib->locked = TRUE;
    return S_OK;
}

static HRESULT __stdcall ib_Unlock(IDirect3DIndexBuffer8 *self)
{
    D3D8IndexBuffer *ib = ib_from_iface(self);
    if (!ib->locked) return E_FAIL;
    ib->locked = FALSE;
    ib->dirty = TRUE;

    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    if (ctx && ib->d3d11_buffer) {
        ID3D11DeviceContext_UpdateSubresource(ctx,
            (ID3D11Resource *)ib->d3d11_buffer,
            0, NULL, ib->sys_mem, ib->size, 0);
        ib->dirty = FALSE;
    }
    return S_OK;
}

static HRESULT __stdcall ib_GetDesc(IDirect3DIndexBuffer8 *self, void *pDesc)
{
    (void)self; (void)pDesc;
    return E_NOTIMPL;
}

static const IDirect3DIndexBuffer8Vtbl g_ib_vtbl = {
    ib_QueryInterface, ib_AddRef, ib_Release,
    ib_GetDevice, ib_SetPriority, ib_GetPriority, ib_PreLoad, ib_GetType,
    ib_Lock, ib_Unlock, ib_GetDesc,
};

HRESULT d3d8_CreateIndexBufferImpl(UINT Length, DWORD Usage, D3DFORMAT Format, IDirect3DIndexBuffer8 **ppIB)
{
    D3D8IndexBuffer *ib;
    D3D11_BUFFER_DESC bd;
    HRESULT hr;

    if (!ppIB) return E_INVALIDARG;

    ib = (D3D8IndexBuffer *)calloc(1, sizeof(*ib));
    if (!ib) return E_OUTOFMEMORY;

    ib->sys_mem = (BYTE *)calloc(1, Length);
    if (!ib->sys_mem) { free(ib); return E_OUTOFMEMORY; }

    memset(&bd, 0, sizeof(bd));
    bd.ByteWidth = Length;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    hr = ID3D11Device_CreateBuffer(d3d8_GetD3D11Device(), &bd, NULL, &ib->d3d11_buffer);
    if (FAILED(hr)) { free(ib->sys_mem); free(ib); return hr; }

    ib->iface.lpVtbl = &g_ib_vtbl;
    ib->ref_count = 1;
    ib->size = Length;
    ib->format = Format;
    ib->usage = Usage;

    *ppIB = &ib->iface;
    return S_OK;
}

/* ================================================================
 * Texture Implementation
 * ================================================================ */

static D3D8Texture *tex_from_iface(IDirect3DTexture8 *iface)
{
    return (D3D8Texture *)iface;
}

/* Mark a texture as already-linear: its LockRect data is row-major and
 * UnlockRect must upload it verbatim. */
void d3d8_TextureSetLinearData(IDirect3DTexture8 *tex, BOOL linear)
{
    if (tex) tex_from_iface(tex)->linear_data = linear;
}


static HRESULT __stdcall tex_QueryInterface(IDirect3DTexture8 *self, const IID *riid, void **ppv)
{
    (void)self; (void)riid; (void)ppv;
    return E_NOINTERFACE;
}

/* Every mip is a slice of the same staging allocation. */
static void d3d8_tex_free_levels(D3D8Texture *tex)
{
    UINT l;
    free(tex->sys_mem);
    for (l = 0; l < D3D8_MAX_LEVELS; l++) {
        tex->level_mem[l] = NULL;
    }
    tex->sys_mem = NULL;
}

static ULONG __stdcall tex_AddRef(IDirect3DTexture8 *self)
{
    return (ULONG)InterlockedIncrement(&tex_from_iface(self)->ref_count);
}

static ULONG __stdcall tex_Release(IDirect3DTexture8 *self)
{
    D3D8Texture *tex = tex_from_iface(self);
    LONG ref = InterlockedDecrement(&tex->ref_count);
    if (ref <= 0) {
        if (tex->srv) ID3D11ShaderResourceView_Release(tex->srv);
        if (tex->d3d11_texture) ID3D11Texture2D_Release(tex->d3d11_texture);
        d3d8_tex_free_levels(tex);
        free(tex);
    }
    return (ULONG)ref;
}

static HRESULT __stdcall tex_GetDevice(IDirect3DTexture8 *self, IDirect3DDevice8 **ppDevice)
{
    (void)self;
    *ppDevice = xbox_GetD3DDevice();
    return S_OK;
}

static DWORD __stdcall tex_SetPriority(IDirect3DTexture8 *self, DWORD Priority) { (void)self; (void)Priority; return 0; }
static DWORD __stdcall tex_GetPriority(IDirect3DTexture8 *self) { (void)self; return 0; }
static void  __stdcall tex_PreLoad(IDirect3DTexture8 *self) { (void)self; }
static DWORD __stdcall tex_GetType(IDirect3DTexture8 *self) { (void)self; return 5; /* D3DRTYPE_TEXTURE */ }

static DWORD __stdcall tex_GetLevelCount(IDirect3DTexture8 *self)
{
    return tex_from_iface(self)->levels;
}

static HRESULT __stdcall tex_GetLevelDesc(IDirect3DTexture8 *self, UINT Level, D3DSURFACE_DESC *pDesc)
{
    D3D8Texture *tex = tex_from_iface(self);
    if (!pDesc || Level >= tex->levels) return E_INVALIDARG;
    pDesc->Format = tex->d3d8_format;
    pDesc->Width = tex->width >> Level;
    pDesc->Height = tex->height >> Level;
    if (pDesc->Width < 1) pDesc->Width = 1;
    if (pDesc->Height < 1) pDesc->Height = 1;
    pDesc->Pool = D3DPOOL_DEFAULT;
    return S_OK;
}

static HRESULT __stdcall tex_GetSurfaceLevel(IDirect3DTexture8 *self, UINT Level, IDirect3DSurface8 **ppSurface)
{
    (void)self; (void)Level; (void)ppSurface;
    return E_NOTIMPL;
}

static HRESULT __stdcall tex_LockRect(IDirect3DTexture8 *self, UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags)
{
    D3D8Texture *tex = tex_from_iface(self);
    (void)pRect; (void)Flags;

    if (!pLockedRect || Level >= tex->levels || Level >= D3D8_MAX_LEVELS)
        return E_INVALIDARG;
    if (tex->locked) return E_FAIL;
    if (!tex->level_mem[Level]) return E_FAIL;

    pLockedRect->Pitch = (INT)tex->level_pitch[Level];
    pLockedRect->pBits = tex->level_mem[Level];
    tex->locked = TRUE;
    tex->locked_level = Level;
    return S_OK;
}

static HRESULT __stdcall tex_UnlockRect(IDirect3DTexture8 *self, UINT Level)
{
    D3D8Texture *tex = tex_from_iface(self);
    if (!tex->locked || Level != tex->locked_level) return E_FAIL;
    if (Level >= tex->levels || Level >= D3D8_MAX_LEVELS) return E_FAIL;

    tex->locked = FALSE;
    tex->dirty = TRUE;
    tex->initialized_levels |= 1u << Level;

    /* Upload this level to its own subresource. */
    {
        ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
        if (ctx && tex->d3d11_texture) {
            UINT lw = tex->level_w[Level], lh = tex->level_h[Level];
            UINT lp = tex->level_pitch[Level];
            UINT rows = d3d8_format_is_compressed(tex->d3d8_format)
                      ? (lh + 3) / 4 : lh;
            BYTE *upload_data = tex->level_mem[Level];
            BYTE *unswizzled = NULL;

            /* Unswizzle if the format is a swizzled Xbox format, unless the
             * caller already handed us linear rows (see linear_data). */
            if (!tex->linear_data &&
                !d3d8_format_is_compressed(tex->d3d8_format) &&
                d3d8_format_is_swizzled(tex->d3d8_format))
            {
                UINT bpp = d3d8_format_bpp(tex->d3d8_format) / 8;
                unswizzled = (BYTE *)malloc((size_t)lw * lh * bpp);
                if (unswizzled) {
                    xbox_unswizzle_rect(unswizzled, tex->level_mem[Level],
                                        lw, lh, bpp);
                    upload_data = unswizzled;
                }
            }

            ID3D11DeviceContext_UpdateSubresource(ctx,
                (ID3D11Resource *)tex->d3d11_texture,
                Level, NULL, upload_data, lp, lp * rows);
            tex->dirty = FALSE;

            if (unswizzled) free(unswizzled);
        }
    }
    return S_OK;
}

static const IDirect3DTexture8Vtbl g_tex_vtbl = {
    tex_QueryInterface, tex_AddRef, tex_Release,
    tex_GetDevice, tex_SetPriority, tex_GetPriority, tex_PreLoad, tex_GetType,
    tex_GetLevelCount,
    tex_GetLevelDesc, tex_GetSurfaceLevel, tex_LockRect, tex_UnlockRect,
};

static HRESULT d3d8_tex_create_gpu(D3D8Texture *tex, BOOL initial_data)
{
    D3D11_TEXTURE2D_DESC td = {0};
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {0};
    D3D11_SUBRESOURCE_DATA data[D3D8_MAX_LEVELS] = {0};
    HRESULT hr;
    td.Width = tex->width;
    td.Height = tex->height;
    td.MipLevels = tex->levels;
    td.ArraySize = 1;
    td.Format = tex->dxgi_format;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    if (initial_data) {
        for (UINT l = 0; l < tex->levels; ++l) {
            UINT rows = d3d8_format_is_compressed(tex->d3d8_format)
                      ? (tex->level_h[l] + 3) / 4 : tex->level_h[l];
            data[l].pSysMem = tex->level_mem[l];
            data[l].SysMemPitch = tex->level_pitch[l];
            data[l].SysMemSlicePitch = tex->level_pitch[l] * rows;
        }
    }
    hr = ID3D11Device_CreateTexture2D(d3d8_GetD3D11Device(), &td,
                                    initial_data ? data : NULL, &tex->d3d11_texture);
    if (FAILED(hr)) return hr;
    srvd.Format = tex->dxgi_format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = tex->levels;
    hr = ID3D11Device_CreateShaderResourceView(d3d8_GetD3D11Device(),
        (ID3D11Resource *)tex->d3d11_texture, &srvd, &tex->srv);
    if (FAILED(hr)) {
        ID3D11Texture2D_Release(tex->d3d11_texture);
        tex->d3d11_texture = NULL;
        return hr;
    }
    if (initial_data) tex->dirty = FALSE;
    return S_OK;
}

/* Write the texture's staged levels as a .dds file (Debug > Diagnostics >
 * "Save the next frame's textures"). The staging copy holds every uploaded
 * level as host rows -- unswizzled, palettes expanded to A8R8G8B8, DXT blocks
 * as they are -- which is exactly a DDS body. Returns the levels written, 0
 * for a format with no DDS equivalent here. */
int d3d8_TextureWriteDDS(IDirect3DTexture8 *iface, const char *path)
{
    D3D8Texture *t;
    uint32_t h[32];
    FILE *f;
    UINT levels = 0, l;
    uint32_t *pf;
    if (!iface || !path) return 0;
    t = tex_from_iface(iface);
    while (levels < t->levels && (t->initialized_levels & (1u << levels)) && t->level_mem[levels]) levels++;
    if (!levels) return 0;
    memset(h, 0, sizeof h);
    h[0] = 0x20534444u;                  /* "DDS " */
    h[1] = 124;                          /* header size */
    h[2] = 0x1u | 0x2u | 0x4u | 0x1000u | (levels > 1 ? 0x20000u : 0);  /* CAPS HEIGHT WIDTH PIXELFORMAT MIPMAPCOUNT */
    h[3] = t->height;
    h[4] = t->width;
    h[5] = t->level_pitch[0];
    h[7] = levels;
    pf = &h[19];                         /* DDS_PIXELFORMAT */
    pf[0] = 32;
    switch (t->dxgi_format) {
    case DXGI_FORMAT_BC1_UNORM: pf[1] = 0x4; pf[2] = 0x31545844u; break;   /* DXT1 */
    case DXGI_FORMAT_BC2_UNORM: pf[1] = 0x4; pf[2] = 0x33545844u; break;   /* DXT3 */
    case DXGI_FORMAT_BC3_UNORM: pf[1] = 0x4; pf[2] = 0x35545844u; break;   /* DXT5 */
    case DXGI_FORMAT_B8G8R8A8_UNORM: pf[1] = 0x41; pf[3] = 32; pf[4] = 0x00FF0000u; pf[5] = 0x0000FF00u; pf[6] = 0x000000FFu; pf[7] = 0xFF000000u; break;
    case DXGI_FORMAT_B8G8R8X8_UNORM: pf[1] = 0x40; pf[3] = 32; pf[4] = 0x00FF0000u; pf[5] = 0x0000FF00u; pf[6] = 0x000000FFu; break;
    case DXGI_FORMAT_B5G6R5_UNORM:   pf[1] = 0x40; pf[3] = 16; pf[4] = 0xF800u; pf[5] = 0x07E0u; pf[6] = 0x001Fu; break;
    case DXGI_FORMAT_B5G5R5A1_UNORM: pf[1] = 0x41; pf[3] = 16; pf[4] = 0x7C00u; pf[5] = 0x03E0u; pf[6] = 0x001Fu; pf[7] = 0x8000u; break;
    case DXGI_FORMAT_B4G4R4A4_UNORM: pf[1] = 0x41; pf[3] = 16; pf[4] = 0x0F00u; pf[5] = 0x00F0u; pf[6] = 0x000Fu; pf[7] = 0xF000u; break;
    case DXGI_FORMAT_R8_UNORM:       pf[1] = 0x20000; pf[3] = 8; pf[4] = 0xFFu; break;               /* luminance */
    case DXGI_FORMAT_A8_UNORM:       pf[1] = 0x2; pf[3] = 8; pf[7] = 0xFFu; break;                   /* alpha */
    case DXGI_FORMAT_R8G8_UNORM:     pf[1] = 0x20001; pf[3] = 16; pf[4] = 0xFFu; pf[7] = 0xFF00u; break; /* luminance + alpha */
    default: return 0;
    }
    h[2] |= (pf[1] & 0x4) ? 0x80000u : 0x8u;                      /* LINEARSIZE or PITCH */
    if (pf[1] & 0x4) h[5] = t->level_pitch[0] * ((t->level_h[0] + 3) / 4);
    h[27] = 0x1000u | (levels > 1 ? 0x400008u : 0);              /* caps: TEXTURE (+ MIPMAP COMPLEX) */
    f = fopen(path, "wb");
    if (!f) return 0;
    fwrite(h, 4, 32, f);
    for (l = 0; l < levels; l++) {
        UINT rows = (pf[1] & 0x4) ? (t->level_h[l] + 3) / 4 : t->level_h[l];
        fwrite(t->level_mem[l], 1, (size_t)t->level_pitch[l] * rows, f);
    }
    fclose(f);
    return (int)levels;
}

HRESULT d3d8_TextureFinishUpload(IDirect3DTexture8 *iface)
{
    if (!iface) return E_INVALIDARG;
    D3D8Texture *tex = tex_from_iface(iface);
    if (tex->d3d11_texture) return S_OK;
    if (tex->locked || !tex->linear_data ||
        tex->initialized_levels != ((1u << tex->levels) - 1u)) return E_FAIL;
    return d3d8_tex_create_gpu(tex, TRUE);
}

static HRESULT d3d8_create_texture(UINT Width, UINT Height, UINT Levels, D3DFORMAT Format, BOOL deferred, IDirect3DTexture8 **ppTex)
{
    D3D8Texture *tex;
    HRESULT hr;

    if (!ppTex) return E_INVALIDARG;
    *ppTex = NULL;

    tex = (D3D8Texture *)calloc(1, sizeof(*tex));
    if (!tex) return E_OUTOFMEMORY;

    tex->d3d8_format = Format;
    tex->dxgi_format = d3d8_to_dxgi_format(Format);
    tex->width = Width;
    tex->height = Height;
    tex->levels = Levels ? Levels : 1;
    if (tex->levels > D3D8_MAX_LEVELS) tex->levels = D3D8_MAX_LEVELS;
    {   /* One full mip chain cannot be longer than log2(max dim) + 1. */
        UINT mx = (Width > Height) ? Width : Height, cap = 1;
        while ((mx >> (cap - 1)) > 1) cap++;
        if (tex->levels > cap) tex->levels = cap;
    }
    tex->pitch = d3d8_row_pitch(Format, Width);
    tex->linear_data = deferred;

    /* One staging allocation for the complete chain. Scene startup can load
     * hundreds of mips; separate heap allocations are unnecessary. */
    {
        UINT l, lw = Width, lh = Height;
        size_t offsets[D3D8_MAX_LEVELS], total = 0;
        for (l = 0; l < tex->levels; l++) {
            UINT lp = d3d8_row_pitch(Format, lw);
            UINT sz = d3d8_format_is_compressed(Format)
                    ? lp * ((lh + 3) / 4) : lp * lh;
            tex->level_w[l] = lw;
            tex->level_h[l] = lh;
            tex->level_pitch[l] = lp;
            offsets[l] = total;
            /* Retain alignment for typed 16/32-bit texture conversion. */
            total += ((size_t)(sz ? sz : 1) + 15u) & ~(size_t)15u;
            lw = (lw > 1) ? lw / 2 : 1;
            lh = (lh > 1) ? lh / 2 : 1;
        }
        tex->sys_mem = (BYTE *)calloc(1, total);
        if (!tex->sys_mem) { free(tex); return E_OUTOFMEMORY; }
        for (l = 0; l < tex->levels; l++)
            tex->level_mem[l] = tex->sys_mem + offsets[l];
    }
    if (!deferred) {
        hr = d3d8_tex_create_gpu(tex, FALSE);
        if (FAILED(hr)) {
            d3d8_tex_free_levels(tex);
            free(tex);
            return hr;
        }
    }

    tex->iface.lpVtbl = &g_tex_vtbl;
    tex->ref_count = 1;

    *ppTex = &tex->iface;
    return S_OK;
}

HRESULT d3d8_CreateTextureImpl(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, IDirect3DTexture8 **ppTex)
{
    (void)Usage;
    return d3d8_create_texture(Width, Height, Levels, Format, FALSE, ppTex);
}

HRESULT d3d8_CreateUploadTexture(UINT Width, UINT Height, UINT Levels, D3DFORMAT Format, IDirect3DTexture8 **ppTex)
{
    return d3d8_create_texture(Width, Height, Levels, Format, TRUE, ppTex);
}
