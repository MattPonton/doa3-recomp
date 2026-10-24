#ifndef DOA3_NV2A_TARGETS_H
#define DOA3_NV2A_TARGETS_H
#include <stdint.h>

/* Host routing metadata only: no textures, D3D objects or pixel contents. */
typedef struct {
    uint32_t count, reserved;
    uint32_t offsets[64];
} Nv2aTargetsState;
extern Nv2aTargetsState g_doa3_targets;
#define g_doa3_offrt_n (g_doa3_targets.count)
#define g_doa3_offrt_offs (g_doa3_targets.offsets)

/* Called AFTER the guest SetRenderTarget body, using its original arguments.
 * Return the resolved surface only when a new offset was added (for logging).
 * Preserve inactive slots too, matching the original swap-removal behavior. */
static uint32_t nv2a_targets_update(Nv2aTargetsState *s, uint32_t dev,
    uint32_t arg, uint32_t (*read)(void *,uint32_t), void *opaque)
{
    if (!arg && dev) arg=read(opaque,dev+0x2070);
    if (dev) {
        for (unsigned b=0;b<3;++b) {
            uint32_t fs=read(opaque,dev+0x207C+4*b);
            if (fs<0x1000 || fs>=0x08000000u) continue;
            uint32_t fd=read(opaque,fs+4)&0x03FFFFFFu;
            for (unsigned k=0;k<s->count;) {
                if (s->offsets[k]==fd) s->offsets[k]=s->offsets[--s->count];
                else ++k;
            }
        }
    }
    if (dev && arg>=0x1000 && arg<0x08000000u &&
        arg!=read(opaque,dev+0x207C) && arg!=read(opaque,dev+0x2080) &&
        arg!=read(opaque,dev+0x2084)) {
        uint32_t data=read(opaque,arg+4)&0x03FFFFFFu;
        for (unsigned k=0;k<s->count;++k) if (s->offsets[k]==data) return 0;
        if (data && s->count<64) { s->offsets[s->count++]=data; return arg; }
    }
    return 0;
}
int doa3_targets_capture(Nv2aTargetsState *out);
#endif
