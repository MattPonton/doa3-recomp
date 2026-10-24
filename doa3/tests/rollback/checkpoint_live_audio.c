/* Implements the existing fixture audio interface using production core code.
 * Only physical transport and empty host mixer endpoints are replaced. */
#include "apu_state.h"
#include "apu_chip.h"
#include "apu.h"
#include "apu_replay.h"
#include "apu_live_checkpoint.h"
#include "online/xbox_det.h"
#include "checkpoint_live_audio.h"
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"audio bridge line %d: %s\n",__LINE__,#x);exit(1);} } while(0)
MCPXAPUState *g_state;
extern uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg_cache;
uint64_t g_dbg_muted_voices[4];
volatile LONG g_apu_irq_pending;
static unsigned g_apu_frames_total,submissions,queue_queries;
static struct { int active;double phase,phase_inc;int16_t amplitude; } g_test_tone;
static APUMixerVoice g_mixer_voices[APU_MIXER_MAX_VOICES];
static int g_mixer_active_count,g_mixer_initialized,g_audio_muted,deny_pin;
static CRITICAL_SECTION g_mixer_cs;
static ApuReplayOutput *observed;
int xa2_is_active(void){return 1;}
int xa2_movie_is_active(void){return 0;}
int xa2_get_buffer_size(void){return 1024;}
int xa2_queued(void){++queue_queries;return 0;}
int xa2_submit_samples(const int16_t *pcm,int count){CHECK(pcm && count==1024);++submissions;return 1;}
static void mixer_render(int16_t (*p)[2],int n){(void)p;(void)n;}
void audio_apply_volume(int16_t *p,size_t n){(void)p;(void)n;}
void mcpx_debug_begin_frame(void){}
void mcpx_debug_end_frame(void){}
static void fixture_observe_pcm(const int16_t (*p)[2]){
    CHECK(observed && observed->samples+256<=1024);
    memcpy(observed->pcm+observed->samples,p,256*4);observed->samples+=256;
}
#define APU_FIXTURE_OBSERVE_PCM
#include "apu_live.generated.inc"
struct ApuReplay { MCPXAPUState device; };
ApuReplay *apu_replay_create(void *ram,size_t bytes){
    CHECK(!g_state && ram && bytes==128u*1024u*1024u);
    ApuReplay *r=calloc(1,sizeof(*r));CHECK(r);g_state=&r->device;
    g_apu_ram_ptr=ram;g_state->ram_ptr=ram;
    qemu_mutex_init(&g_state->lock);qemu_cond_init(&g_state->cond);return r;
}
int apu_replay_seed(ApuReplay *r,const void *saved,size_t bytes){
    ApuRollbackClock clock;CHECK(r && &r->device==g_state);
    if(!apu_rollback_import(g_state,&clock,saved,bytes))return 0;
    mcpx_apu_det_begin(clock.base);return g_apu_det!=0;
}
void apu_replay_destroy(ApuReplay *r){
    CHECK(r && !s_apu_replay_active);mcpx_apu_det_end();CHECK(!g_apu_det);
    qemu_mutex_destroy(&g_state->lock);g_state=NULL;free(r);
}
size_t apu_replay_snapshot_size(void){return mcpx_apu_processing_size();}
int apu_replay_capture(const ApuReplay *r,void *p,size_t n){CHECK(r);return mcpx_apu_processing_capture(p,n);}
int apu_replay_validate(const ApuReplay *r,const void *p,size_t n){CHECK(r);return mcpx_apu_processing_validate(p,n);}
int apu_replay_restore(ApuReplay *r,const void *p,size_t n){CHECK(r);return mcpx_apu_processing_restore(p,n);}
int apu_replay_frame(ApuReplay *r,ApuReplayOutput *out){
    CHECK(r && out && !observed && !s_apu_pin_owner);memset(out,0,sizeof(*out));observed=out;
    mcpx_apu_det_frame();observed=NULL;return !s_apu_replay_fault;
}
int apu_replay_wait(ApuReplay *r,ApuReplayOutput *out){
    CHECK(r && out && !observed && !s_apu_pin_owner);memset(out,0,sizeof(*out));observed=out;
    mcpx_apu_det_step();observed=NULL;return !s_apu_replay_fault;
}
int apu_replay_take_irq(ApuReplay *r){CHECK(r);return InterlockedExchange(&g_apu_irq_pending,0)!=0;}
int fixture_live_acquire(void){
    if(!xbox_det_checkpoint_acquire())return 0;
    if(deny_pin || !mcpx_apu_rollback_acquire()){CHECK(xbox_det_checkpoint_release());return 0;}
    return 1;
}
void fixture_live_release(void){CHECK(mcpx_apu_rollback_release());CHECK(xbox_det_checkpoint_release());}
int fixture_live_can_restore(void *p){(void)p;return mcpx_apu_processing_can_restore();}
void fixture_live_begin(void){CHECK(fixture_live_acquire());CHECK(mcpx_apu_replay_begin());fixture_live_release();}
void fixture_live_end(int cancel){CHECK(fixture_live_acquire());CHECK(mcpx_apu_replay_end(cancel));fixture_live_release();}
unsigned fixture_live_submissions(void){return submissions;}
unsigned fixture_live_queue_queries(void){return queue_queries;}
void fixture_live_deny_pin(int deny){deny_pin=deny;}
