#ifndef DOA3_NV2A_PFIFO_REGISTERS_H
#define DOA3_NV2A_PFIFO_REGISTERS_H
#include <stdint.h>
#include "nv2a_regs.h"

/* Register behavior of this synchronous port, shared with isolated replay.
 * Callers dispatch only offsets within the 0x2000-byte PFIFO block. The
 * production array uses byte offsets as indices; retain that representation. */
static uint32_t nv2a_pfifo_register_read(const uint32_t *regs, uint32_t pending,
                                      uint32_t enabled, uint32_t address)
{
    switch(address) {
    case NV_PFIFO_INTR_0: return pending;
    case NV_PFIFO_INTR_EN_0: return enabled;
    case NV_PFIFO_CACHE1_STATUS: return NV_PFIFO_CACHE1_STATUS_LOW_MARK;
    case NV_PFIFO_CACHE1_DMA_PUSH:
        return regs[address] & ~(uint32_t)(NV_PFIFO_CACHE1_DMA_PUSH_STATE | NV_PFIFO_CACHE1_DMA_PUSH_STATUS);
    case NV_PFIFO_RUNOUT_STATUS: return NV_PFIFO_RUNOUT_STATUS_LOW_MARK;
    case NV_PFIFO_CACHE1_PULL0: return regs[address] & ~(uint32_t)NV_PFIFO_CACHE1_PULL0_BUSY;
    case NV_PFIFO_CACHES: return regs[address] & ~(uint32_t)NV_PFIFO_CACHES_DMA_SUSPEND;
    case NV_PFIFO_CACHE1_DMA_GET: return regs[NV_PFIFO_CACHE1_DMA_PUT];
    default: return regs[address];
    }
}
/* Return nonzero when the production caller must update interrupt delivery.
 * Replay rejects those writes until the interrupt controller is captured. */
static int nv2a_pfifo_register_write(uint32_t *regs, uint32_t *pending,
                                    uint32_t *enabled, uint32_t address, uint32_t value)
{
    switch(address) {
    case NV_PFIFO_INTR_0: *pending &= ~value; return 1;
    case NV_PFIFO_INTR_EN_0: *enabled=value; return 1;
    default: regs[address]=value; return 0;
    }
}
#endif
