#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static int alloc_fail,alloc_calls,destroyed;
static void *allocate(size_t n){return ++alloc_calls==alloc_fail ? NULL:malloc(n);}
#define NV_GEOM_ALLOC allocate
#include "nv2a/nv2a_geometry_cache.h"
void d3d8_Nv2aFF_MeshDestroy(Nv2aFFMesh *p){if(p){++destroyed;free(p);}}
static NvGeometryCache cache;
int main(void)
{
    unsigned char *ram=malloc(600000);CHECK(ram);memset(ram,0x35,600000);
    NvGeometryKey key={0};key.count=6;key.prim=4;key.mask=31;
    for(unsigned i=0;i<5;i++){key.fmt[i]=0x3032;key.off[i]=128+i*16;}
    NvGeometrySpan spans[2]={{128,128},{512,64}};
    uint32_t indices[]={0,2,1,1,2,0};uint16_t packed[]={0,1,2,2,1,0};
    Nv2aFFVertex verts[3];memset(verts,0x63,sizeof(verts));
    uint64_t hash=nv_geometry_hash(&key,indices);
    CHECK(!nv_geometry_find(&cache,&key,spans,2,indices,ram,hash));
    NvGeometryEntry *entry=nv_geometry_store(&cache,&key,spans,2,indices,ram,hash,verts,3,packed,6,1);
    CHECK(entry && !entry->mesh && entry->all_zero==1);
    CHECK(!memcmp(entry->verts,verts,sizeof(verts)) && !memcmp(entry->indices,packed,sizeof(packed)));
    CHECK(nv_geometry_find(&cache,&key,spans,2,indices,ram,hash)==entry);
    /* Every byte, not a sample/hash, is validated. A restored old source hits. */
    for(unsigned s=0;s<2;s++)for(unsigned b=0;b<spans[s].size;b++){
        ram[spans[s].address+b]^=1;
        CHECK(!nv_geometry_find(&cache,&key,spans,2,indices,ram,hash));
        ram[spans[s].address+b]^=1;
        CHECK(nv_geometry_find(&cache,&key,spans,2,indices,ram,hash)==entry);
    }
    /* Deliberate hash collision cannot hide changed indices or key fields. */
    for(unsigned b=0;b<sizeof(indices);b++){
        ((unsigned char *)indices)[b]^=1;CHECK(!nv_geometry_find(&cache,&key,spans,2,indices,ram,hash));
        ((unsigned char *)indices)[b]^=1;
    }
    for(unsigned b=0;b<sizeof(key);b++){
        ((unsigned char *)&key)[b]^=1;CHECK(!nv_geometry_find(&cache,&key,spans,2,indices,ram,hash));
        ((unsigned char *)&key)[b]^=1;
    }
    for(int failure=1;failure<=3;failure++){
        alloc_fail=alloc_calls+failure;size_t bytes=cache.bytes;
        CHECK(!nv_geometry_store(&cache,&key,spans,2,indices,ram,hash,verts,3,packed,6,0));
        CHECK(cache.bytes==bytes && nv_geometry_find(&cache,&key,spans,2,indices,ram,hash)==entry);
    }
    alloc_fail=0;entry->mesh=(Nv2aFFMesh *)malloc(1);CHECK(entry->mesh);
    /* Fill one set with different source versions and force LRU eviction. */
    for(unsigned i=1;i<=4;i++){
        ram[128]=(unsigned char)i;
        CHECK(nv_geometry_store(&cache,&key,spans,2,indices,ram,hash,verts,3,packed,6,0));
    }
    CHECK(destroyed==1);
    nv_geometry_clear(&cache);CHECK(cache.bytes==0);
    /* Whole-cache budget includes reserved native buffers. */
    spans[0].size=500000;
    for(unsigned i=0;i<200;i++){
        CHECK(nv_geometry_store(&cache,&key,spans,1,indices,ram,i,verts,3,packed,6,0));
        CHECK(cache.bytes<=NV_GEOM_BUDGET);
    }
    CHECK(!cache.entries[0].source); /* oldest entry evicted by byte budget */
    spans[0].size=NV_GEOM_SOURCE_LIMIT;
    CHECK(!nv_geometry_store(&cache,&key,spans,1,indices,ram,0,verts,3,packed,6,0));
    nv_geometry_clear(&cache);CHECK(cache.bytes==0);
    nv_geometry_clear(&cache);free(ram);
    puts("Exact geometry reuse: byte mutations, collisions, restore, allocation failure and bounded eviction passed");
    return 0;
}
