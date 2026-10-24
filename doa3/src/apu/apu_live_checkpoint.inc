/* Included by apu_core.c; all live backend identity and ownership stays private. */
#include "apu_live_checkpoint.h"
#include <limits.h>
extern int xbox_fiber_is_primary(void);
typedef struct ApuProcessingImage {
    uint32_t magic,version,bytes,reserved;
    uint64_t epoch,device,ram;
    uint32_t owner,phase,pause,monitor,fill,buffer_size;
    uint32_t gp_regs[0x10000],ep_regs[0x10000];
    int16_t pcm[256][2],stage[1024][2];
    unsigned char command[];
} ApuProcessingImage;
static void *s_apu_replay_anchor,*s_apu_replay_scratch;
static int apu_live_supported(MCPXAPUState *d)
{
    int size=xa2_get_buffer_size();
    if(!d || !xa2_is_active() || xa2_movie_is_active() || g_waveout.initialized ||
       size<256 || size>1024 || size%256 || g_test_tone.active || g_audio_muted ||
       g_mixer_active_count || g_dbg_voice_monitor!=-1 || d->gp.realtime || d->ep.realtime ||
       (d->monitor.point!=MCPX_APU_DEBUG_MON_AC97 && d->monitor.point!=MCPX_APU_DEBUG_MON_EP) ||
       d->monitor.stream || d->ep_frame_div<0 || s_apu_fill<0 || s_apu_fill>=size || s_apu_fill%256)
        return 0;
    for(unsigned i=0;i<4;++i)if(g_dbg_muted_voices[i])return 0;
    if(g_mixer_initialized && !TryEnterCriticalSection(&g_mixer_cs))return 0;
    int empty=1;
    for(unsigned i=0;i<APU_MIXER_MAX_VOICES;++i)
        if(g_mixer_voices[i].active || g_mixer_voices[i].pcm_data || g_mixer_voices[i].pcm_bytes)empty=0;
    if(g_mixer_initialized)LeaveCriticalSection(&g_mixer_cs);
    return empty;
}
static int apu_processing_ready(void)
{
    DWORD owner=GetCurrentThreadId();MCPXAPUState *d=g_state;
    return d && d->ram_ptr && g_apu_det && s_apu_det_epoch && (DWORD)s_apu_pin_owner==owner &&
        (DWORD)s_apu_det_owner==owner && xbox_fiber_is_primary() && !d->exiting &&
        !d->rollback_frame_active && !d->rollback_det_pending && apu_live_supported(d);
}
size_t mcpx_apu_processing_size(void) { return sizeof(ApuProcessingImage)+apu_rollback_size(); }
int mcpx_apu_processing_capture(void *out,size_t bytes)
{
    if(!out || bytes!=mcpx_apu_processing_size() || !apu_processing_ready())return 0;
    MCPXAPUState *d=g_state;ApuProcessingImage *s=out;
    memset(s,0,bytes);s->magic=0x4C555041;s->version=1;s->bytes=(uint32_t)bytes;
    s->epoch=s_apu_det_epoch;s->device=(uintptr_t)d;s->ram=(uintptr_t)d->ram_ptr;
    s->owner=GetCurrentThreadId();s->phase=(uint32_t)d->ep_frame_div;
    s->pause=d->pause_requested;s->monitor=d->monitor.point;
    s->fill=(uint32_t)s_apu_fill;s->buffer_size=xa2_get_buffer_size();
    memcpy(s->gp_regs,d->gp.regs,sizeof(s->gp_regs));memcpy(s->ep_regs,d->ep.regs,sizeof(s->ep_regs));
    memcpy(s->pcm,d->monitor.frame_buf,sizeof(s->pcm));memcpy(s->stage,s_apu_stage,sizeof(s->stage));
    ApuRollbackClock clock={s_apu_det_xgs0,s_apu_det_chip,s_apu_det_debt,
        InterlockedCompareExchange(&g_apu_irq_pending,0,0)!=0};
    return apu_rollback_export(d,&clock,s->command,apu_rollback_size());
}
int mcpx_apu_processing_validate(const void *saved,size_t bytes)
{
    if(!saved || bytes!=mcpx_apu_processing_size() || !apu_processing_ready())return 0;
    const ApuProcessingImage *s=saved;ApuRollbackClock clock;
    return s->magic==0x4C555041 && s->version==1 && s->bytes==bytes && !s->reserved &&
        s->epoch==s_apu_det_epoch && s->device==(uintptr_t)g_state && s->ram==(uintptr_t)g_state->ram_ptr &&
        s->owner==GetCurrentThreadId() && s->phase<=INT_MAX && s->pause<=1 &&
        s->monitor==(uint32_t)g_state->monitor.point && s->buffer_size==(uint32_t)xa2_get_buffer_size() &&
        s->fill<s->buffer_size && !(s->fill%256) &&
        apu_rollback_validate(s->command,apu_rollback_size(),&clock) && clock.debt>=0 &&
        clock.base==s_apu_det_xgs0;
}
int mcpx_apu_processing_can_restore(void)
{ return s_apu_replay_active && apu_processing_ready(); }
int mcpx_apu_processing_restore(const void *saved,size_t bytes)
{
    if(!s_apu_replay_active || !mcpx_apu_processing_validate(saved,bytes))return 0;
    const ApuProcessingImage *s=saved;MCPXAPUState *d=g_state;ApuRollbackClock clock;
    if(!apu_rollback_import(d,&clock,s->command,apu_rollback_size()))return 0;
    s_apu_det_xgs0=clock.base;s_apu_det_chip=clock.chip;s_apu_det_debt=clock.debt;
    InterlockedExchange(&g_apu_irq_pending,(LONG)clock.irq_pending);
    d->ep_frame_div=(int)s->phase;d->pause_requested=s->pause!=0;s_apu_fill=(int)s->fill;
    memcpy(d->gp.regs,s->gp_regs,sizeof(s->gp_regs));memcpy(d->ep.regs,s->ep_regs,sizeof(s->ep_regs));
    memcpy(d->monitor.frame_buf,s->pcm,sizeof(s->pcm));memcpy(s_apu_stage,s->stage,sizeof(s->stage));
    return 1;
}
int mcpx_apu_replay_begin(void)
{
    if(s_apu_replay_active || !apu_processing_ready())return 0;
    size_t bytes=mcpx_apu_processing_size();
    void *anchor=malloc(bytes),*scratch=malloc(bytes);
    if(!anchor || !scratch || !mcpx_apu_processing_capture(anchor,bytes)){
        free(anchor);free(scratch);return 0;
    }
    s_apu_replay_anchor=anchor;s_apu_replay_scratch=scratch;
    s_apu_replay_buffer_size=xa2_get_buffer_size();s_apu_replay_fault=0;s_apu_replay_active=1;return 1;
}
int mcpx_apu_replay_end(int cancel)
{
    size_t bytes=mcpx_apu_processing_size();
    if(!s_apu_replay_active || (s_apu_replay_fault && !cancel) ||
       !mcpx_apu_processing_capture(s_apu_replay_scratch,bytes) ||
       memcmp(s_apu_replay_anchor,s_apu_replay_scratch,bytes))return 0;
    free(s_apu_replay_anchor);free(s_apu_replay_scratch);
    s_apu_replay_anchor=s_apu_replay_scratch=NULL;
    s_apu_replay_active=s_apu_replay_fault=s_apu_replay_buffer_size=0;return 1;
}
