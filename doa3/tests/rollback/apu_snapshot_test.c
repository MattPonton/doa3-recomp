#include "apu_state.h"
#include "apu_rollback.h"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg;
int g_dbg_voice_monitor=-1;
void mcpx_apu_vp_write(void *,hwaddr,uint64_t,unsigned);
static void commands(MCPXAPUState *d)
{
    mcpx_apu_vp_write(d,NV1BA0_PIO_SET_CURRENT_VOICE,5,4);
    mcpx_apu_vp_write(d,NV1BA0_PIO_SET_ANTECEDENT_VOICE,1u<<16,4);
    mcpx_apu_vp_write(d,NV1BA0_PIO_SET_VOICE_CFG_FMT,0,4);
    mcpx_apu_vp_write(d,NV1BA0_PIO_VOICE_ON,5,4);
    CHECK(d->regs[NV_PAPU_TVL2D]==5);
    mcpx_apu_vp_write(d,NV1BA0_PIO_VOICE_OFF,5,4);
    CHECK(d->set_irq);
}
int main(void)
{
    size_t n=apu_rollback_size(), ram_bytes=128u*1024u*1024u;
    MCPXAPUState *a=calloc(1,sizeof(*a)),*b=calloc(1,sizeof(*b));
    void *initial=malloc(n),*expected=malloc(n),*actual=malloc(n);
    unsigned char *ram=calloc(1,ram_bytes),*saved=malloc(ram_bytes);
    CHECK(a && b && initial && expected && actual && ram && saved); g_apu_ram_ptr=ram;
    qemu_mutex_init(&a->lock); qemu_cond_init(&a->cond);
    qemu_mutex_init(&b->lock); qemu_cond_init(&b->cond);
    a->regs[NV_PAPU_VPVADDR]=0x100000; a->regs[NV_PAPU_FENADDR]=0x40000;
    a->regs[NV_PAPU_TVL2D]=a->regs[NV_PAPU_TVL3D]=a->regs[NV_PAPU_TVLMP]=0xFFFF;
    for(unsigned v=0;v<MCPX_HW_MAX_VOICES;++v) {
        a->vp.filters[v].voice=(uint16_t)v;
        a->vp.filters[v].rs_frac=0.125;
        setup_svf(&a->vp.filters[v].svf[0],0.3f,0.7f,F_LP);
        a->vp.filters[v].resampler=src_callback_new(NULL,0,2,NULL,NULL);
    }
    ApuRollbackClock clock={12345,8,2,1},restored={0};
    CHECK(apu_rollback_export(a,&clock,initial,n));
    CHECK(apu_rollback_import(b,&restored,initial,n));
    CHECK(!memcmp(&clock,&restored,sizeof(clock)));
    CHECK(b->vp.filters[7].svf[0].op==&b->vp.filters[7].svf[0].l);
    CHECK(b->vp.filters[7].svf[0].op!=a->vp.filters[7].svf[0].op);
    commands(a); CHECK(apu_rollback_export(a,&clock,expected,n)); memcpy(saved,ram,ram_bytes);
    for(unsigned replay=0;replay<2;++replay) {
        memset(ram,0,ram_bytes); CHECK(apu_rollback_import(b,&restored,initial,n));
        commands(b); CHECK(apu_rollback_export(b,&restored,actual,n));
        CHECK(!memcmp(actual,expected,n) && !memcmp(saved,ram,ram_bytes));
    }
    ((uint32_t *)initial)[1]^=1;
    CHECK(!apu_rollback_import(b,&restored,initial,n));
    CHECK(!apu_rollback_import(b,&restored,expected,n-1));
    CHECK(apu_rollback_export(b,&restored,actual,n) && !memcmp(actual,expected,n));
    puts("APU command snapshot: relocated filters, voice on/off, notifier writes and repeated restore/replay match.");
    free(initial); free(expected); free(actual); free(ram); free(saved);
    qemu_mutex_destroy(&a->lock); qemu_mutex_destroy(&b->lock); free(a); free(b);
}
