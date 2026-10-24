#include "apu_state.h"
#include "apu_rollback.h"
#include "shadow_apu.h"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
void mcpx_apu_vp_write(void *,hwaddr,uint64_t,unsigned);
static uint32_t write_ports(int faulting,MCPXAPUState *d)
{
    const uint32_t ports[]={NV1BA0_PIO_SET_CURRENT_VOICE,NV1BA0_PIO_SET_ANTECEDENT_VOICE,
        NV1BA0_PIO_SET_VOICE_CFG_FMT,NV1BA0_PIO_VOICE_ON,NV1BA0_PIO_VOICE_OFF};
    const uint32_t values[]={5,1u<<16,0,5,5};
    uint32_t free_slots=0;
    __try {
        if(faulting) free_slots=*(volatile uint32_t *)rb_apu_pointer(0xFE820010);
        else free_slots=0x80;
        for(unsigned i=0;i<5;++i) {
            if(faulting) *(volatile uint32_t *)rb_apu_pointer(0xFE820000+ports[i])=values[i];
            else mcpx_apu_vp_write(d,ports[i],values[i],4);
        }
    } __except(rb_apu_exception(GetExceptionInformation())) {
        fprintf(stderr,"Unexpected MMIO error: %s\n",rb_apu_error()); exit(1);
    }
    return free_slots;
}
int main(int argc,char **argv)
{
    size_t size=rb_apu_size(), bytes=128u*1024u*1024u;
    void *initial=malloc(size),*expected=malloc(size),*actual=malloc(size);
    unsigned char *ram=calloc(1,bytes), *expected_ram=malloc(bytes);
    MCPXAPUState *d=calloc(1,sizeof(*d)); CHECK(d && ram && initial && expected && actual && expected_ram);
    extern uint8_t *g_apu_ram_ptr; g_apu_ram_ptr=ram;
    qemu_mutex_init(&d->lock); qemu_cond_init(&d->cond);
    d->regs[NV_PAPU_VPVADDR]=0x100000; d->regs[NV_PAPU_FENADDR]=0x40000;
    d->regs[NV_PAPU_TVL2D]=d->regs[NV_PAPU_TVL3D]=d->regs[NV_PAPU_TVLMP]=0xFFFF;
    ApuRollbackClock clock={900,4,0,0};
    CHECK(apu_rollback_export(d,&clock,initial,size));
    if(argc==3 && !strcmp(argv[1],"--snapshot")) {
        FILE *f=fopen(argv[2],"wb"); CHECK(f && fwrite(initial,1,size,f)==size); CHECK(!fclose(f));
    } else CHECK(argc==1);
    CHECK(write_ports(0,d)==0x80);
    CHECK(apu_rollback_export(d,&clock,expected,size)); memcpy(expected_ram,ram,bytes);
    for(unsigned i=0;i<2;++i) {
        memset(ram,0,bytes); CHECK(rb_apu_load(ram,initial,size));
        CHECK(write_ports(1,NULL)==0x80); CHECK(rb_apu_save(actual,size));
        CHECK(!memcmp(expected,actual,size) && !memcmp(expected_ram,ram,bytes));
    }
    int rejected=0;
    __try { *(volatile uint32_t *)rb_apu_pointer(0xFE800000+NV_PAPU_FECTL)=0; }
    __except(rb_apu_exception(GetExceptionInformation())) { rejected=1; }
    CHECK(rejected && !strcmp(rb_apu_error(),"apu_control_write_requires_service"));
    CHECK(!rb_apu_save(actual,size));
    CHECK(rb_apu_load(ram,NULL,0) && !rb_apu_pointer(0xFE820010));
    free(initial);free(expected);free(actual);free(ram);free(expected_ram);
    qemu_mutex_destroy(&d->lock);free(d);
    puts("Isolated production MMIO decoder replays real VP commands; unsupported worker control is rejected.");
}
