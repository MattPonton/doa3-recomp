#ifndef NV2A_TEXTURE_SIGNATURE_H
#define NV2A_TEXTURE_SIGNATURE_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#define XXH_INLINE_ALL
#include "../../third_party/xxhash.h"

/* Host-only cache fingerprint, never guest state. Small sources are checked
 * in full on EVERY lookup, including partial in-place animation updates.
 * Keep the established sparse policy for larger sources; rollback invalidates
 * uploads independently, including mip levels and unsampled changed bytes. */
static uint32_t nv_tex_signature(const uint8_t *p,size_t n)
{
    if(n<=16384)return (uint32_t)XXH3_64bits(p,n);
    uint32_t h=2166136261u^(uint32_t)n;
    size_t step=(n/256)&~(size_t)3;
    if(step<4)step=4;
    for(size_t i=0;i+4<=n;i+=step){
        uint32_t v;memcpy(&v,p+i,4);h=(h^v)*16777619u;
    }
    return h;
}
#endif
