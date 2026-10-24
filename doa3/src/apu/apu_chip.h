#pragma once
#include "apu_state.h"

/* Shared guest-visible chip gating and IRQ latching. Host IRQ delivery and
 * audio-device submission stay with the caller. */
static inline bool apu_chip_running(const MCPXAPUState *d)
{
    return GET_MASK(qatomic_read(&d->regs[NV_PAPU_SECTL]), NV_PAPU_SECTL_XCNTMODE)
               != NV_PAPU_SECTL_XCNTMODE_OFF &&
        (qatomic_read(&d->regs[NV_PAPU_FECTL]) & NV_PAPU_FECTL_FEMETHMODE)
               != NV_PAPU_FECTL_FEMETHMODE_HALTED;
}

static inline bool apu_chip_update_irq(MCPXAPUState *d)
{
    if (d->regs[NV_PAPU_FECTL] & NV_PAPU_FECTL_FEMETHMODE_TRAPPED)
        qatomic_or(&d->regs[NV_PAPU_ISTS], NV_PAPU_ISTS_FETINTSTS);
    bool asserted = (d->regs[NV_PAPU_IEN] & NV_PAPU_ISTS_GINTSTS) &&
        ((d->regs[NV_PAPU_ISTS] & ~NV_PAPU_ISTS_GINTSTS) & d->regs[NV_PAPU_IEN]);
    if (asserted) qatomic_or(&d->regs[NV_PAPU_ISTS], NV_PAPU_ISTS_GINTSTS);
    else qatomic_and(&d->regs[NV_PAPU_ISTS], ~NV_PAPU_ISTS_GINTSTS);
    return asserted;
}
