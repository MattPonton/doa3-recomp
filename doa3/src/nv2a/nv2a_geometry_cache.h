#ifndef NV2A_GEOMETRY_CACHE_H
#define NV2A_GEOMETRY_CACHE_H
#include <stdlib.h>
#include <string.h>
#include "../d3d/d3d8_nv2aff.h"
#define XXH_INLINE_ALL
#include "../../third_party/xxhash.h"

/* Derived object-space data only. Never cache lighting, transforms, draw
 * state, textures or targets. Hashes select candidates; exact bytes decide
 * hits, including after a guest RAM restore. Bounded, owner-thread only. */
#define NV_GEOM_SLOTS 1024
#define NV_GEOM_WAYS 4
#define NV_GEOM_BUDGET (64u*1024u*1024u)
#define NV_GEOM_SOURCE_LIMIT (512u*1024u)
#ifndef NV_GEOM_ALLOC
#define NV_GEOM_ALLOC malloc
#endif
typedef struct {
    uint32_t count, prim, quads, mask, fp_control, fmt[5], off[5];
} NvGeometryKey;
typedef struct { uint32_t address, size; } NvGeometrySpan;
typedef struct {
    NvGeometryKey key;
    NvGeometrySpan spans[5];
    uint32_t span_count, source_size, nv, ni, all_zero, bytes;
    uint64_t hash, age;
    unsigned char *source;
    Nv2aFFVertex *verts;
    uint16_t *indices;
    Nv2aFFMesh *mesh;
} NvGeometryEntry;
typedef struct {
    NvGeometryEntry entries[NV_GEOM_SLOTS];
    uint64_t clock, hits, misses, vertices_reused;
    size_t bytes;
} NvGeometryCache;

static void nv_geometry_evict(NvGeometryCache *c,NvGeometryEntry *e)
{
    if(!e->source)return;
    d3d8_Nv2aFF_MeshDestroy(e->mesh);
    free(e->source);free(e->verts);free(e->indices);
    c->bytes-=e->bytes;memset(e,0,sizeof(*e));
}
static void nv_geometry_clear(NvGeometryCache *c)
{
    for(unsigned i=0;i<NV_GEOM_SLOTS;i++)nv_geometry_evict(c,&c->entries[i]);
}
static uint64_t nv_geometry_hash(const NvGeometryKey *k,const uint32_t *indices)
{
    return XXH3_64bits_withSeed(indices,(size_t)k->count*4,XXH3_64bits(k,sizeof(*k)));
}
static NvGeometryEntry *nv_geometry_find(NvGeometryCache *c,const NvGeometryKey *k,
    const NvGeometrySpan *spans,unsigned ns,const uint32_t *indices,const unsigned char *ram,uint64_t hash)
{
    unsigned base=(unsigned)(hash%(NV_GEOM_SLOTS/NV_GEOM_WAYS))*NV_GEOM_WAYS;
    for(unsigned i=0;i<NV_GEOM_WAYS;i++){
        NvGeometryEntry *e=&c->entries[base+i];
        if(!e->source || e->hash!=hash || memcmp(k,&e->key,sizeof(*k)) ||
           e->span_count!=ns || memcmp(spans,e->spans,ns*sizeof(*spans)) ||
           memcmp(indices,e->source,(size_t)k->count*4))continue;
        size_t offset=(size_t)k->count*4;unsigned s;
        for(s=0;s<ns;s++){
            if(memcmp(ram+spans[s].address,e->source+offset,spans[s].size))break;
            offset+=spans[s].size;
        }
        if(s!=ns)continue;
        e->age=++c->clock;++c->hits;c->vertices_reused+=e->nv;return e;
    }
    ++c->misses;return NULL;
}
static NvGeometryEntry *nv_geometry_store(NvGeometryCache *c,const NvGeometryKey *k,
    const NvGeometrySpan *spans,unsigned ns,const uint32_t *indices,const unsigned char *ram,uint64_t hash,
    const Nv2aFFVertex *verts,unsigned nv,const uint16_t *packed,unsigned ni,unsigned all_zero)
{
    size_t source=(size_t)k->count*4, vb=(size_t)nv*sizeof(*verts), ib=(size_t)ni*2;
    if(ns>5)return NULL;
    for(unsigned s=0;s<ns;s++)source+=spans[s].size;
    /* Reserve both CPU and eventual GPU copies in the budget. */
    size_t bytes=source+2*(vb+ib);
    if(!nv || !ni || source>NV_GEOM_SOURCE_LIMIT || bytes>NV_GEOM_BUDGET)return NULL;
    unsigned char *copy=NV_GEOM_ALLOC(source);
    Nv2aFFVertex *v=NV_GEOM_ALLOC(vb);uint16_t *p=NV_GEOM_ALLOC(ib);
    if(!copy || !v || !p){free(copy);free(v);free(p);return NULL;}
    memcpy(copy,indices,(size_t)k->count*4);
    size_t offset=(size_t)k->count*4;
    for(unsigned s=0;s<ns;s++){memcpy(copy+offset,ram+spans[s].address,spans[s].size);offset+=spans[s].size;}
    memcpy(v,verts,vb);memcpy(p,packed,ib);
    unsigned base=(unsigned)(hash%(NV_GEOM_SLOTS/NV_GEOM_WAYS))*NV_GEOM_WAYS;
    NvGeometryEntry *e=&c->entries[base];
    for(unsigned i=0;i<NV_GEOM_WAYS;i++){
        NvGeometryEntry *candidate=&c->entries[base+i];
        if(!candidate->source){e=candidate;break;}
        if(candidate->age<e->age)e=candidate;
    }
    nv_geometry_evict(c,e);
    while(c->bytes+bytes>NV_GEOM_BUDGET){
        NvGeometryEntry *old=NULL;
        for(unsigned i=0;i<NV_GEOM_SLOTS;i++)if(c->entries[i].source && (!old || c->entries[i].age<old->age))old=&c->entries[i];
        if(!old)break;
        nv_geometry_evict(c,old);
    }
    e->key=*k;memcpy(e->spans,spans,ns*sizeof(*spans));e->span_count=ns;
    e->source_size=(uint32_t)source;e->nv=nv;e->ni=ni;e->all_zero=all_zero;
    e->bytes=(uint32_t)bytes;e->hash=hash;e->age=++c->clock;
    e->source=copy;e->verts=v;e->indices=p;c->bytes+=bytes;
    return e;
}
#endif
