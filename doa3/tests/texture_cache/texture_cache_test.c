/* Production lookup, deletion and allocation policy with counted resources. */
#include "d3d8_xbox.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache_limits.h"
/* Production defines TEXCACHE_BUDGET as a runtime variable (an ini override). */
#define TEXCACHE_BUDGET TEXCACHE_BUDGET_DEFAULT
static int g_texcache_recycle = 1;
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static struct {
    struct {
        IDirect3DTexture8 *tex;
        uint32_t off,w,h,fmt,levels,pal;
        int uploaded;
        uint32_t sig,last_frame,bytes;
    } texcache[TEXCACHE_N];
    int16_t texhash[TEXCACHE_HASH_N];
    uint32_t texcache_bytes,frame;
} g_pg;
static IDirect3DTexture8 textures[TEXCACHE_N];
static unsigned releases;
static ULONG __stdcall release(IDirect3DTexture8 *t) { CHECK(t); releases++; return 0; }
static IDirect3DTexture8Vtbl vt;
#include "cache_impl.h"
static void reset(void)
{
    memset(&g_pg,0,sizeof g_pg); releases=0; g_pg.frame=100;
    vt.Release=release;
    for(int i=0;i<TEXCACHE_N;i++) textures[i].lpVtbl=&vt;
}
static void put(int i,uint32_t address,uint32_t bytes,uint32_t levels,uint32_t frame)
{
    g_pg.texcache[i].tex=&textures[i]; g_pg.texcache[i].off=address;
    g_pg.texcache[i].w=g_pg.texcache[i].h=128;
    g_pg.texcache[i].fmt=12; g_pg.texcache[i].levels=levels;
    g_pg.texcache[i].bytes=bytes; g_pg.texcache[i].last_frame=frame;
    g_pg.texcache_bytes+=bytes; texcache_hash_insert(i);
}
int main(void)
{
    int slot;
    reset(); put(0,32,1024,1,1);
    CHECK(texcache_make_room(1024,128,128,12,1)==1);
    CHECK(releases==0 && g_pg.texcache_bytes==1024);
    /* Entry pressure at a small fraction of the byte limit: preserve a
     * currently used resource, reuse the oldest compatible previous one. */
    reset();
    for(int i=0;i<TEXCACHE_N;i++) put(i,32u*(i+1),1024,1,i==0 ? 100 : 1);
    CHECK(g_pg.texcache_bytes<TEXCACHE_BUDGET);
    for(int i=0;i<TEXCACHE_N;i++) CHECK(texcache_find(32u*(i+1),128,128,12,0)==i);
    slot=texcache_make_room(1024,128,128,12,1);
    CHECK(slot==1 && releases==0 && g_pg.texcache[slot].tex==&textures[1]);
    CHECK(texcache_find(64,128,128,12,0)==-1);
    CHECK(g_pg.texcache_bytes==(TEXCACHE_N-1)*1024u);
    put(slot,0x7000000,1024,1,100);
    CHECK(texcache_find(0x7000000,128,128,12,0)==slot);
    for(int i=2;i<TEXCACHE_N;i++) CHECK(texcache_find(32u*(i+1),128,128,12,0)==i);
    /* Mip mismatch must release, rather than recycling an incompatible GPU
     * allocation. This also exercises hash deletion amid a full table. */
    slot=texcache_make_room(1024,128,128,12,2);
    CHECK(slot==2 && releases==1 && !g_pg.texcache[slot].tex);
    for(int i=3;i<TEXCACHE_N;i++) CHECK(texcache_find(32u*(i+1),128,128,12,0)==i);
    /* Budget pressure with plenty of free slots also permits same-size reuse. */
    reset(); put(0,32,TEXCACHE_BUDGET/2,1,1); put(1,64,TEXCACHE_BUDGET/2,1,2);
    CHECK(texcache_make_room(TEXCACHE_BUDGET/2,128,128,12,1)==0);
    CHECK(releases==0 && g_pg.texcache_bytes==TEXCACHE_BUDGET/2);
    /* Different format/size may not reuse; normal eviction still frees space. */
    reset(); put(0,32,TEXCACHE_BUDGET/2,1,1); put(1,64,TEXCACHE_BUDGET/2,1,2);
    CHECK(texcache_make_room(TEXCACHE_BUDGET/2,256,128,15,1)==0);
    CHECK(releases==1 && !g_pg.texcache[0].tex);
    CHECK(texcache_find(64,128,128,12,0)==1);
    puts("Texture cache lookup, reuse, current-frame protection and budget tests pass");
    return 0;
}
