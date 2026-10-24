#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d3d/d3d8_xbox.h"
#include "d3d/d3d8_swizzle.h"
#include "nv2a/nv2a_regs.h"
#include "nv2a/nv2a_pgraph_state.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_texfmt[64], g_texnull[4];
static unsigned creates, releases, uploads;
static int fail_lock=-1, fail_unlock=-1;
typedef struct {
    IDirect3DTexture8 iface;
    unsigned levels, pitch[12], bytes[12];
    unsigned char *data[12];
} MockTexture;
static IDirect3DTexture8Vtbl texture_vtable;
void d3d8_TextureSetLinearData(IDirect3DTexture8 *texture, BOOL value)
{ (void)texture; CHECK(value); }
static ULONG __stdcall release_texture(IDirect3DTexture8 *self)
{
    MockTexture *t=(MockTexture *)self; unsigned l;
    for(l=0;l<t->levels;l++) free(t->data[l]);
    free(t); releases++; return 0;
}
static HRESULT __stdcall lock_texture(IDirect3DTexture8 *self, UINT level,
    D3DLOCKED_RECT *rect, const RECT *area, DWORD flags)
{
    MockTexture *t=(MockTexture *)self; (void)area; (void)flags;
    CHECK(level<t->levels);
    if ((int)level==fail_lock) return E_FAIL;
    rect->Pitch=t->pitch[level]; rect->pBits=t->data[level]; return S_OK;
}
static HRESULT __stdcall unlock_texture(IDirect3DTexture8 *self, UINT level)
{ (void)self; uploads++; return (int)level==fail_unlock ? E_FAIL : S_OK; }
static HRESULT __stdcall create_texture(IDirect3DDevice8 *dev, UINT w, UINT h,
    UINT levels, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DTexture8 **out)
{
    MockTexture *t=(MockTexture *)calloc(1,sizeof(*t)); unsigned l;
    (void)dev; (void)usage; (void)pool; CHECK(t && levels<=12);
    t->iface.lpVtbl=&texture_vtable; t->levels=levels;
    for(l=0;l<levels;l++) {
        t->pitch[l]=fmt==D3DFMT_DXT1 ? ((w+3)/4)*8 : w*4;
        t->bytes[l]=t->pitch[l]*(fmt==D3DFMT_DXT1 ? (h+3)/4 : h);
        t->data[l]=(unsigned char *)calloc(1,t->bytes[l]); CHECK(t->data[l]);
        if(w>1)w/=2; if(h>1)h/=2;
    }
    creates++; *out=&t->iface; return S_OK;
}
static int fail_source_allocation;
static void *source_alloc(size_t n) { return fail_source_allocation ? NULL : malloc(n); }
#define malloc source_alloc
#include "texture_restore.generated.inc"
#undef malloc

int main(void)
{
    IDirect3DDevice8Vtbl vt={0}; IDirect3DDevice8 dev;
    unsigned char *ram=(unsigned char *)calloc(1,0x08000000);
    MockTexture *t; unsigned i,old_uploads,old_creates; int slot;
    CHECK(ram); g_xbox_mem_offset=(ptrdiff_t)ram;
    texture_vtable.Release=release_texture; texture_vtable.LockRect=lock_texture; texture_vtable.UnlockRect=unlock_texture;
    vt.CreateTexture=create_texture; dev.lpVtbl=&vt;
    /* 8x8 DXT1, four levels; level zero is unchanged by the rewind. */
    g_pg.tex[0].offset=0x1000; g_pg.tex[0].format=(3u<<24)|(3u<<20)|(4u<<16)|(0x0Cu<<8);
    memset(ram+0x1000,0x11,32); memset(ram+0x1020,0x22,24);
    t=(MockTexture *)get_dynamic_texture(&dev); CHECK(t && uploads==4 && creates==1);
    CHECK(t->data[1][0]==0x22);
    /* Identical full chains survive arbitrarily many restore barriers without
     * any decode/LockRect/UnlockRect. Every source byte is still checked. */
    for(i=0;i<8;i++) {
        pgraph_d3d11_invalidate_guest_uploads();
        pgraph_d3d11_invalidate_guest_uploads();
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==4 && creates==1);
    }
    CHECK(!pgraph_d3d11_force_guest_uploads(1));
    old_uploads=uploads;pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+4);
    CHECK(pgraph_d3d11_force_guest_uploads(0));
    /* Reset counters so the following older regression retains exact counts. */
    uploads=4;
    memset(ram+0x1020,0x33,24);
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==4); /* base-level cache shortcut */
    old_creates=creates; pgraph_d3d11_invalidate_guest_uploads();
    CHECK(!g_pg_host.dyn_tex && !g_pg_host.dyn_src_valid);
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==8 && creates==old_creates);
    for(i=1;i<4;i++) CHECK(t->data[i][0]==0x33);
    /* Failed upload cannot mark partially updated content cache-valid. */
    slot=texcache_find(0x1000,8,8,D3DFMT_DXT1,0); CHECK(slot>=0);
    for(i=0;i<2;i++) {
        ram[0x1020]^=1; /* force a real upload, not an exact-content reuse */
        pgraph_d3d11_invalidate_guest_uploads();
        if(i==0) fail_lock=2; else fail_unlock=1;
        CHECK(get_dynamic_texture(&dev)==NULL && !g_pg_host.texcache[slot].uploaded);
        fail_lock=fail_unlock=-1; old_uploads=uploads;
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+4);
        CHECK(g_pg_host.texcache[slot].uploaded);
    }
    /* Every byte of every mip participates, including those a hash misses. */
    for(i=0;i<56;i++) {
        old_uploads=uploads;ram[0x1000+i]^=1;
        pgraph_d3d11_invalidate_guest_uploads();
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+4);
        old_uploads=uploads;pgraph_d3d11_invalidate_guest_uploads();
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads);
    }
    /* Source-copy allocation is optional. Failure keeps the texture usable
     * but can never authorize reuse after RAM restoration. */
    free(g_pg_host.texcache[slot].source);g_pg_host.texcache[slot].source=NULL;
    fail_source_allocation=1;
    for(i=0;i<2;i++) {
        old_uploads=uploads;pgraph_d3d11_invalidate_guest_uploads();
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+4);
        CHECK(!g_pg_host.texcache[slot].source);
    }
    fail_source_allocation=0;pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && g_pg_host.texcache[slot].source);
    /* Different mip count at the same guest address must replace allocation. */
    g_pg.tex[0].format=(g_pg.tex[0].format&~0xF0000u)|(2u<<16);
    t=(MockTexture *)get_dynamic_texture(&dev);
    CHECK(t && t->levels==2 && creates==2 && releases==1);
    /* Large swizzled image: byte 4 is not in its sampled fingerprint. */
    g_pg.tex[0].offset=0x10000; g_pg.tex[0].format=(7u<<24)|(7u<<20)|(1u<<16)|(6u<<8);
    memset(ram+0x10000,0x44,128*128*4);
    t=(MockTexture *)get_dynamic_texture(&dev); CHECK(t && t->data[0][4]==0x44);
    ram[0x10004]=0x77; old_uploads=uploads;
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads);
    pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && t->data[0][4]==0x77 && uploads==old_uploads+1);
    /* Palettised uploads expand restored colors through the production path. */
    g_pg.tex[0].offset=0x30000; g_pg.tex[0].palette=0x40000;
    g_pg.tex[0].format=(2u<<24)|(2u<<20)|(1u<<16)|(0x0Bu<<8);
    memset(ram+0x30000,1,16); ((uint32_t *)(ram+0x40000))[1]=0xFF123456;
    t=(MockTexture *)get_dynamic_texture(&dev); CHECK(t && ((uint32_t *)t->data[0])[0]==0xFF123456);
    ((uint32_t *)(ram+0x40000))[1]=0xFFABCDEF;
    pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && ((uint32_t *)t->data[0])[0]==0xFFABCDEF);
    /* Palette animation must invalidate a warm cache without a rewind. */
    old_uploads=uploads;((uint32_t *)(ram+0x40000))[1]=0xFF556677;
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+1 &&
          ((uint32_t *)t->data[0])[0]==0xFF556677);
    old_uploads=uploads;
    pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads);
    for(i=0;i<1024;i++) {
        old_uploads=uploads;ram[0x40000+i]^=1;
        pgraph_d3d11_invalidate_guest_uploads();
        CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+1);
    }
    /* 64x64 RGBA: any small-source byte change must refresh the same cached
     * texture, without discarding the allocation or explicit invalidation. */
    g_pg.tex[0].offset=0x50000;g_pg.tex[0].format=(6u<<24)|(6u<<20)|(1u<<16)|(6u<<8);
    memset(ram+0x50000,0x33,16384);t=(MockTexture *)get_dynamic_texture(&dev);CHECK(t);
    old_creates=creates;old_uploads=uploads;
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads);
    ram[0x50000+16383]^=1;
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+1 && creates==old_creates);
    /* Linear/movie images must retain their unconditional upload behavior. */
    g_pg.tex[0].offset=0x60000;g_pg.tex[0].format=(1u<<16)|(0x12u<<8);
    g_pg.tex[0].image_rect=(8u<<16)|8u;g_pg.tex[0].control1=32u<<16;
    t=(MockTexture *)get_dynamic_texture(&dev);CHECK(t);old_uploads=uploads;
    pgraph_d3d11_invalidate_guest_uploads();
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+1);
    CHECK(get_dynamic_texture(&dev)==&t->iface && uploads==old_uploads+2);
    uint32_t budget=0;
    for(i=0;i<TEXCACHE_N;i++)if(g_pg_host.texcache[i].tex)budget+=g_pg_host.texcache[i].bytes;
    CHECK(budget==g_pg_host.texcache_bytes && budget<=TEXCACHE_BUDGET);
    for(i=0;i<TEXCACHE_N;i++) if(g_pg_host.texcache[i].tex) texcache_evict((int)i);
    CHECK(creates==releases && g_pg_host.texcache_bytes==0);
    free(ram);
    puts("Production guest uploads: restored mips, sampled-hash misses, palettes, allocation changes and failure retries passed");
    return 0;
}
