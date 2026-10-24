#include "apu_state.h"
#include "apu_chip.h"
#include "apu.h"
#include "apu_rollback.h"
#include "apu_live_checkpoint.h"
#include "guest_memory_fixture.h"
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);} } while(0)
MCPXAPUState *g_state;
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg,g_dbg_cache;
int g_dbg_voice_monitor=-1;
uint64_t g_dbg_muted_voices[4];
volatile LONG g_apu_irq_pending;
unsigned g_doa3_frames_presented;
static unsigned g_apu_frames_total;
static struct { int active;double phase,phase_inc;int16_t amplitude; } g_test_tone;
static APUMixerVoice g_mixer_voices[APU_MIXER_MAX_VOICES];
static int g_mixer_active_count,g_mixer_initialized,g_audio_muted,primary=1;
static CRITICAL_SECTION g_mixer_cs;
static unsigned submissions,queue_queries,mix_calls;
static int backend=1,movie,buffer_size=1024;
static int16_t output[32768][2];
static unsigned samples;
int xbox_fiber_is_primary(void){return primary;}
int xa2_is_active(void){return backend;}
int xa2_movie_is_active(void){return movie;}
int xa2_get_buffer_size(void){return buffer_size;}
int xa2_queued(void){++queue_queries;return 0;}
int xa2_submit_samples(const int16_t *pcm,int count){
    CHECK(count>0 && samples+(unsigned)count<=32768);
    memcpy(output+samples,pcm,count*4);samples+=count;++submissions;return 1;
}
static void mixer_render(int16_t (*pcm)[2],int count){(void)pcm;(void)count;++mix_calls;}
void audio_apply_volume(int16_t *p,size_t n){(void)p;(void)n;}
void mcpx_debug_begin_frame(void){}
void mcpx_debug_end_frame(void){}
#include "apu_live.generated.inc"
static void pin(void){CHECK(mcpx_apu_rollback_acquire());}
static void unpin(void){CHECK(mcpx_apu_rollback_release());}
static void read_seed(const char *path,void *out,size_t n){
    FILE *f=fopen(path,"rb");CHECK(f && fread(out,1,n,f)==n && fgetc(f)==EOF);fclose(f);
}
static DWORD WINAPI wrong_owner(void *unused){
    (void)unused;void *p=malloc(mcpx_apu_processing_size());CHECK(p);
    CHECK(!mcpx_apu_processing_capture(p,mcpx_apu_processing_size()));
    CHECK(!mcpx_apu_replay_begin() && !mcpx_apu_replay_end(1));free(p);return 0;
}
int main(int argc,char **argv){
    CHECK(argc==1 || argc==3);
    g_state=calloc(1,sizeof(*g_state));CHECK(g_state);
    g_apu_ram_ptr=guest_memory_fixture_alloc(128u*1024u*1024u);CHECK(g_apu_ram_ptr);g_state->ram_ptr=g_apu_ram_ptr;
    qemu_mutex_init(&g_state->lock);qemu_cond_init(&g_state->cond);
    InitializeCriticalSection(&g_mixer_cs);g_mixer_initialized=1;
    g_state->regs[NV_PAPU_TVL2D]=g_state->regs[NV_PAPU_TVL3D]=g_state->regs[NV_PAPU_TVLMP]=0xFFFF;
    g_state->regs[NV_PAPU_SECTL]=NV_PAPU_SECTL_XCNTMODE;
    for(unsigned i=0;i<MCPX_HW_MAX_VOICES;++i)g_state->vp.filters[i].voice=(uint16_t)i;
    if(argc==3){
        void *command=malloc(apu_rollback_size());CHECK(command);ApuRollbackClock clock;
        read_seed(argv[1],g_apu_ram_ptr,128u*1024u*1024u);read_seed(argv[2],command,apu_rollback_size());
        CHECK(apu_rollback_import(g_state,&clock,command,apu_rollback_size()));free(command);
    }
    size_t n=mcpx_apu_processing_size();void *before=malloc(n),*after=malloc(n),*actual=malloc(n),*bad=malloc(n);
    unsigned char *ram_before=malloc(128u*1024u*1024u),*ram_after=malloc(128u*1024u*1024u);
    CHECK(before && after && actual && bad && ram_before && ram_after);
    CHECK(!mcpx_apu_processing_capture(before,n));mcpx_apu_det_begin(1234);
    for(unsigned i=0;i<5;++i)mcpx_apu_det_step(); /* partial PCM and debt */
    s_apu_fill=256;memset(s_apu_stage,0x12,256*4); /* partial device staging */
    pin();CHECK(mcpx_apu_processing_capture(before,n));CHECK(!mcpx_apu_processing_restore(before,n));
    memcpy(ram_before,g_apu_ram_ptr,128u*1024u*1024u);unpin();
    for(unsigned i=0;i<6;++i)mcpx_apu_det_frame();
    pin();CHECK(mcpx_apu_processing_capture(after,n));memcpy(ram_after,g_apu_ram_ptr,128u*1024u*1024u);
    unsigned sent=submissions,queries=queue_queries,mixed=mix_calls;CHECK(sent && samples);
    for(unsigned repeat=0;repeat<3;++repeat){
        CHECK(mcpx_apu_replay_begin());CHECK(!mcpx_apu_replay_begin());
        CHECK(mcpx_apu_processing_restore(before,n));memcpy(g_apu_ram_ptr,ram_before,128u*1024u*1024u);
        CHECK(!mcpx_apu_replay_end(0)); /* cannot unmute an earlier frame */
        unpin();mcpx_apu_det_end();CHECK(g_apu_det); /* gated session cannot escape */
        for(unsigned i=0;i<6;++i)mcpx_apu_det_frame();
        pin();CHECK(mcpx_apu_processing_capture(actual,n));CHECK(!memcmp(after,actual,n));
        CHECK(!memcmp(ram_after,g_apu_ram_ptr,128u*1024u*1024u));CHECK(mcpx_apu_replay_end(0));
        CHECK(submissions==sent && queue_queries==queries && mix_calls==mixed);
    }
    /* Full validation precedes mutation. */
    CHECK(mcpx_apu_replay_begin());
    for(unsigned failure=0;failure<6;++failure){
        memcpy(bad,before,n);ApuProcessingImage *s=bad;
        switch(failure){case 0:++s->version;break;case 1:++s->epoch;break;case 2:s->phase=UINT32_MAX;break;
            case 3:s->fill=s->buffer_size;break;case 4:++s->owner;break;case 5:s->reserved=1;break;}
        CHECK(!mcpx_apu_processing_restore(bad,n));CHECK(mcpx_apu_processing_capture(actual,n));CHECK(!memcmp(after,actual,n));
    }
    HANDLE thread=CreateThread(NULL,0,wrong_owner,NULL,0,NULL);CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);CloseHandle(thread);
    primary=0;CHECK(!mcpx_apu_processing_capture(actual,n));primary=1;
    CHECK(mcpx_apu_replay_end(0));
    /* Unsupported paths reject before opening a replay gate. */
    movie=1;CHECK(!mcpx_apu_replay_begin());movie=0;
    g_mixer_voices[0].pcm_data=(void *)1;CHECK(!mcpx_apu_replay_begin());g_mixer_voices[0].pcm_data=NULL;
    g_test_tone.active=1;CHECK(!mcpx_apu_replay_begin());g_test_tone.active=0;
    g_dbg_voice_monitor=1;CHECK(!mcpx_apu_replay_begin());g_dbg_voice_monitor=-1;
    g_waveout.initialized=1;CHECK(!mcpx_apu_replay_begin());g_waveout.initialized=0;
    buffer_size=300;CHECK(!mcpx_apu_replay_begin());buffer_size=1024;
    /* Backend loss inside replay cannot fall through to waveOut or hardware. */
    CHECK(mcpx_apu_replay_begin());unpin();backend=0;mcpx_apu_det_frame();backend=1;pin();
    CHECK(!mcpx_apu_replay_end(0));CHECK(mcpx_apu_processing_restore(after,n));
    memcpy(g_apu_ram_ptr,ram_after,128u*1024u*1024u);
    CHECK(!mcpx_apu_replay_end(0));CHECK(mcpx_apu_replay_end(1));
    CHECK(submissions==sent && queue_queries==queries && mix_calls==mixed);
    unpin();mcpx_apu_det_frame();CHECK(submissions>sent && queue_queries>queries && mix_calls>mixed);
    mcpx_apu_det_end();mcpx_apu_det_begin(1234);pin();CHECK(!mcpx_apu_processing_validate(after,n));unpin();mcpx_apu_det_end();
    unsigned nonzero=0;for(unsigned i=256;i<samples;++i)nonzero+=output[i][0]!=0 || output[i][1]!=0;
    if(argc==3)CHECK(nonzero);
    printf("Live audio source fixture: three six-frame round trips match processing/RAM; partial PCM/staging retained; no replay submits, waits or mixer calls; fault recovery and stale/owner guards pass (%u nonzero output samples).\n",nonzero);
    free(before);free(after);free(actual);free(bad);free(ram_before);free(ram_after);
    guest_memory_fixture_free(g_apu_ram_ptr);qemu_mutex_destroy(&g_state->lock);free(g_state);DeleteCriticalSection(&g_mixer_cs);return 0;
}
