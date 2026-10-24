#ifndef DOA3_APU_OFFLINE
#error This backend must only be built in a private offline executable.
#endif
#include "apu_chip.h"
#include "apu_replay.h"
#include <limits.h>

extern uint8_t *g_apu_ram_ptr;
struct ApuReplay {
    MCPXAPUState device;
    ApuRollbackClock clock;
    int seeded;
};
typedef struct ReplayImage {
    uint32_t magic, version, bytes, phase;
    int16_t pcm[256][2];
    unsigned char command[];
} ReplayImage;

ApuReplay *apu_replay_create(void *ram, size_t bytes)
{
    if (!ram || bytes!=128u*1024u*1024u) return NULL;
    ApuReplay *r=calloc(1,sizeof(*r));
    if (!r) return NULL;
    r->device.ram_ptr=ram;
    qemu_mutex_init(&r->device.lock); qemu_cond_init(&r->device.cond);
    return r;
}
void apu_replay_destroy(ApuReplay *r)
{
    if (!r) return;
    qemu_mutex_destroy(&r->device.lock); free(r);
}
int apu_replay_seed(ApuReplay *r,const void *command,size_t bytes)
{
    if (!r) return 0;
    /* Validate the clock before importing so a rejected seed is atomic. */
    ApuRollbackClock clock;
    int ok=apu_rollback_validate(command,bytes,&clock) && clock.debt>=0;
    if (!ok || !apu_rollback_import(&r->device,&r->clock,command,bytes)) return 0;
    r->device.ep_frame_div=0;
    r->device.monitor.point=MCPX_APU_DEBUG_MON_EP;
    memset(r->device.monitor.frame_buf,0,sizeof(r->device.monitor.frame_buf));
    r->seeded=1;
    return 1;
}
size_t apu_replay_snapshot_size(void) { return sizeof(ReplayImage)+apu_rollback_size(); }
int apu_replay_capture(const ApuReplay *r,void *out,size_t bytes)
{
    if (!r || !r->seeded || !out || bytes!=apu_replay_snapshot_size()) return 0;
    ReplayImage *s=out;
    memset(s,0,bytes);
    s->magic=0x52555041; s->version=1; s->bytes=(uint32_t)bytes;
    s->phase=(uint32_t)r->device.ep_frame_div;
    memcpy(s->pcm,r->device.monitor.frame_buf,sizeof(s->pcm));
    return apu_rollback_export(&r->device,&r->clock,s->command,apu_rollback_size());
}
int apu_replay_validate(const ApuReplay *r,const void *saved,size_t bytes)
{
    if (!r || !saved || bytes!=apu_replay_snapshot_size()) return 0;
    const ReplayImage *s=saved;
    if (s->magic!=0x52555041 || s->version!=1 || s->bytes!=bytes || s->phase>=8) return 0;
    ApuRollbackClock clock;
    return apu_rollback_validate(s->command,apu_rollback_size(),&clock) && clock.debt>=0;
}
int apu_replay_restore(ApuReplay *r,const void *saved,size_t bytes)
{
    if (!apu_replay_validate(r,saved,bytes)) return 0;
    const ReplayImage *s=saved;
    if (!apu_replay_seed(r,s->command,apu_rollback_size())) return 0;
    r->device.ep_frame_div=(int)s->phase;
    memcpy(r->device.monitor.frame_buf,s->pcm,sizeof(s->pcm));
    return 1;
}
static int run(ApuReplay *r,unsigned ticks,ApuReplayOutput *out)
{
    if (!r || !r->seeded || !out || ticks>25 || g_dbg_voice_monitor!=-1 ||
        r->clock.chip>UINT64_MAX-ticks) return 0;
    memset(out,0,sizeof(*out));
    MCPXAPUState *d=&r->device;
    uint8_t *previous_ram=g_apu_ram_ptr;
    g_apu_ram_ptr=d->ram_ptr;
    for (unsigned i=0;i<ticks;++i) {
        ++r->clock.chip;
        if (apu_chip_running(d)) {
            float bins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME]={0};
            mcpx_apu_vp_frame(d,bins);
            mcpx_apu_dsp_frame(d,bins);
        }
        if (d->ep_frame_div==7) {
            memcpy(out->pcm+out->samples,d->monitor.frame_buf,sizeof(d->monitor.frame_buf));
            out->samples+=256;
            memset(d->monitor.frame_buf,0,sizeof(d->monitor.frame_buf));
        }
        d->ep_frame_div=(d->ep_frame_div+1)%8;
        if (d->set_irq) {
            d->set_irq=false;
            if (apu_chip_update_irq(d)) r->clock.irq_pending=1;
        }
    }
    g_apu_ram_ptr=previous_ram;
    return 1;
}
int apu_replay_frame(ApuReplay *r,ApuReplayOutput *out)
{
    if (!r || !r->seeded) return 0;
    int debt=r->clock.debt;
    unsigned ticks=debt>=25 ? 0 : 25-(unsigned)debt;
    if (!run(r,ticks,out)) return 0;
    r->clock.debt=debt>=25 ? debt-25 : 0;
    return 1;
}
int apu_replay_wait(ApuReplay *r,ApuReplayOutput *out)
{
    if (!r || r->clock.debt==INT_MAX || !run(r,1,out)) return 0;
    ++r->clock.debt;
    return 1;
}
int apu_replay_take_irq(ApuReplay *r)
{
    if (!r || !r->seeded) return 0;
    int pending=r->clock.irq_pending!=0; r->clock.irq_pending=0; return pending;
}
const ApuRollbackClock *apu_replay_clock(const ApuReplay *r)
{ return r && r->seeded ? &r->clock : NULL; }
