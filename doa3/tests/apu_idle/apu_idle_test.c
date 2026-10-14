#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "apu_regs.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); return 1; } } while(0)
typedef struct { uint32_t regs[0x20000]; } MCPXAPUState;
static MCPXAPUState state;
static const struct { unsigned top,current,next; } voice_list_regs[] = {
    {NV_PAPU_TVL2D,NV_PAPU_CVL2D,NV_PAPU_NVL2D},
    {NV_PAPU_TVL3D,NV_PAPU_CVL3D,NV_PAPU_NVL3D},
    {NV_PAPU_TVLMP,NV_PAPU_CVLMP,NV_PAPU_NVLMP}
};
static uint16_t links[MCPX_HW_MAX_VOICES];
static bool active[MCPX_HW_MAX_VOICES], locked[MCPX_HW_MAX_VOICES];
static unsigned trap_count, trapped_voice, reads;
static uint32_t voice_get_mask(MCPXAPUState *d,uint16_t v,unsigned off,uint32_t mask)
{
    (void)d;(void)mask; ++reads;
    return off==NV_PAVS_VOICE_TAR_PITCH_LINK ? links[v] : active[v];
}
static bool is_voice_locked(MCPXAPUState *d,uint16_t v) {(void)d;return locked[v];}
static void fe_method(MCPXAPUState *d,unsigned method,unsigned argument)
{
    if(method!=SE2FE_IDLE_VOICE) {fprintf(stderr,"unexpected FE method\n");return;}
    ++trap_count;trapped_voice=argument;
    d->regs[NV_PAPU_FECTL]=NV_PAPU_FECTL_FEMETHMODE_TRAPPED;
}
#include "idle_walk.h"
static void reset(void)
{
    memset(&state,0,sizeof(state)); memset(active,0,sizeof(active));memset(locked,0,sizeof(locked));
    for(int i=0;i<MCPX_HW_MAX_VOICES;++i)links[i]=0xFFFF;
    for(int i=0;i<3;++i)state.regs[voice_list_regs[i].top]=0xFFFF;
    state.regs[NV_PAPU_FETFORCE1]=NV_PAPU_FETFORCE1_SE2FE_IDLE_VOICE;
    trap_count=reads=0;trapped_voice=0xFFFF;
}
int main(void)
{
    reset(); CHECK(!mcpx_apu_vp_poll_idle(&state));
    /* An active voice ahead of two stopped voices is never retired. Each
     * call emits at most one trap, leaving all links/states for the guest. */
    state.regs[NV_PAPU_TVL2D]=4;active[4]=true;links[4]=8;links[8]=12;
    CHECK(mcpx_apu_vp_poll_idle(&state));CHECK(trap_count==1 && trapped_voice==8);
    CHECK(state.regs[NV_PAPU_CVL2D]==8 && state.regs[NV_PAPU_NVL2D]==12);
    CHECK(links[4]==8 && links[8]==12 && active[4] && !active[8]);
    CHECK(!mcpx_apu_vp_poll_idle(&state) && trap_count==1);
    /* Emulate guest acknowledgement/removal; the next idle voice is now eligible. */
    links[4]=12;state.regs[NV_PAPU_FECTL]=NV_PAPU_FECTL_FEMETHMODE_FREE_RUNNING;
    CHECK(mcpx_apu_vp_poll_idle(&state) && trapped_voice==12);
    for(int list=0;list<3;++list) {
        reset();state.regs[voice_list_regs[list].top]=5;
        CHECK(mcpx_apu_vp_poll_idle(&state) && trapped_voice==5);
        CHECK(state.regs[voice_list_regs[list].current]==5 && state.regs[voice_list_regs[list].next]==0xFFFF);
    }
    reset();state.regs[NV_PAPU_TVL2D]=1;locked[1]=true;
    CHECK(!mcpx_apu_vp_poll_idle(&state));locked[1]=false;
    state.regs[NV_PAPU_FECTL]=NV_PAPU_FECTL_FEMETHMODE_HALTED;
    CHECK(!mcpx_apu_vp_poll_idle(&state));state.regs[NV_PAPU_FECTL]=0;
    state.regs[NV_PAPU_FETFORCE1]=0;CHECK(!mcpx_apu_vp_poll_idle(&state));
    reset();state.regs[NV_PAPU_TVL2D]=MCPX_HW_MAX_VOICES;CHECK(!mcpx_apu_vp_poll_idle(&state) && reads==0);
    reset();state.regs[NV_PAPU_TVL2D]=1;active[1]=true;links[1]=1;
    CHECK(!mcpx_apu_vp_poll_idle(&state) && reads<=2);
    links[1]=2;links[2]=1;active[2]=true;reads=0;
    CHECK(!mcpx_apu_vp_poll_idle(&state) && reads<=2*MCPX_HW_MAX_VOICES);
    puts("Idle voice control-walk tests passed");return 0;
}
