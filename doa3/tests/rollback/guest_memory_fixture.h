#ifndef ROLLBACK_GUEST_MEMORY_FIXTURE_H
#define ROLLBACK_GUEST_MEMORY_FIXTURE_H
#include <windows.h>
#include <stdint.h>
/* The production resolver supports 32-bit native pointers in its mirrored
 * mapping. An arbitrary malloc's low 32 bits can alias that range and make a
 * guest VA look native. Use a 4 GiB-aligned test mapping (offset32 == 0), so
 * the fixture always takes the guest-VA path while retaining the full pointer. */
static unsigned char *guest_memory_fixture_alloc(size_t bytes)
{
    for (uintptr_t i=1;i<=16;++i) {
        void *base=VirtualAlloc((void *)(i*UINT64_C(0x100000000)),bytes,
                               MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
        if (base) return (unsigned char *)base;
    }
    return NULL;
}
static void guest_memory_fixture_free(void *base)
{
    if (base) VirtualFree(base,0,MEM_RELEASE);
}
#endif
