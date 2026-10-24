#include "apu_chip.h"
#include "apu_replay.h"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg;
int g_dbg_voice_monitor=-1;
unsigned g_doa3_frames_presented;
static void read_file(const char *path,void *out,size_t n)
{
    FILE *f=fopen(path,"rb"); CHECK(f); CHECK(fread(out,1,n,f)==n);
    CHECK(fgetc(f)==EOF); fclose(f);
}
static void sequence(ApuReplay *r,ApuReplayOutput *output)
{
    ApuRollbackClock start=*apu_replay_clock(r);
    CHECK(start.debt>=0 && start.debt<21);
    for (int i=0;i<40;++i) {
        /* Cross both an audio-period boundary and the next frame's debt. */
        if(i<29) CHECK(apu_replay_wait(r,&output[i]));
        else CHECK(apu_replay_frame(r,&output[i]));
        if(i==29) {
            CHECK(apu_replay_clock(r)->chip==start.chip+29);
            CHECK(apu_replay_clock(r)->debt==start.debt+4 && output[i].samples==0);
        }
    }
    CHECK(apu_replay_clock(r)->debt==0);
    CHECK(apu_replay_clock(r)->chip==start.chip+11*25-start.debt);
}
int main(int argc,char **argv)
{
    SetEnvironmentVariableA("DOA3_MIX_TRACE",NULL);
    const size_t ram_bytes=128u*1024u*1024u;
    size_t cmd_n=apu_rollback_size(),n=apu_replay_snapshot_size();
    unsigned char *ram=calloc(1,ram_bytes),*saved_ram=malloc(ram_bytes),*expected_ram=malloc(ram_bytes);
    void *cmd=malloc(cmd_n),*saved=malloc(n),*expected=malloc(n),*actual=malloc(n);
    ApuReplayOutput *a=calloc(40,sizeof(*a)),*b=calloc(40,sizeof(*b)),out;
    CHECK(ram && saved_ram && expected_ram && cmd && saved && expected && actual && a && b);
    MCPXAPUState *device=calloc(1,sizeof(*device)); CHECK(device);
    device->regs[NV_PAPU_VPVADDR]=0x100000;
    device->regs[NV_PAPU_FENADDR]=0x40000;
    device->regs[NV_PAPU_TVL2D]=5;
    device->regs[NV_PAPU_TVL3D]=device->regs[NV_PAPU_TVLMP]=0xFFFF;
    device->regs[NV_PAPU_SECTL]=NV_PAPU_SECTL_XCNTMODE;
    device->regs[NV_PAPU_IEN]=NV_PAPU_ISTS_GINTSTS|NV_PAPU_ISTS_FETINTSTS;
    device->regs[NV_PAPU_FETFORCE1]=NV_PAPU_FETFORCE1_SE2FE_IDLE_VOICE;
    *(uint32_t *)(ram+0x100000+5*NV_PAVS_SIZE+NV_PAVS_VOICE_TAR_PITCH_LINK)=0xFFFF;
    for (unsigned v=0;v<MCPX_HW_MAX_VOICES;++v) device->vp.filters[v].voice=(uint16_t)v;
    ApuRollbackClock clock={1234,0,0,0};
    CHECK(apu_rollback_export(device,&clock,cmd,cmd_n));
    if(argc==3) { read_file(argv[1],ram,ram_bytes); read_file(argv[2],cmd,cmd_n); }
    else CHECK(argc==1);
    ApuReplay *r=apu_replay_create(ram,ram_bytes); CHECK(r);
    CHECK(!apu_replay_frame(r,&out)); CHECK(!apu_replay_create(ram,ram_bytes-1));
    CHECK(apu_replay_seed(r,cmd,cmd_n));
    uint64_t before=apu_replay_clock(r)->chip;
    int old_debt=apu_replay_clock(r)->debt;
    CHECK(apu_replay_wait(r,&out));
    CHECK(apu_replay_clock(r)->chip==before+1 && apu_replay_clock(r)->debt==old_debt+1);
    if(argc==1) {
        CHECK(apu_replay_take_irq(r)); CHECK(!apu_replay_take_irq(r));
        CHECK(apu_replay_frame(r,&out)); CHECK(apu_replay_clock(r)->chip==25);
        CHECK(apu_replay_clock(r)->debt==0 && out.samples==768);
    }
    /* The checkpoint is deliberately inside a partially filled PCM period. */
    CHECK(apu_replay_capture(r,saved,n)); memcpy(saved_ram,ram,ram_bytes);
    sequence(r,a); CHECK(apu_replay_capture(r,expected,n)); memcpy(expected_ram,ram,ram_bytes);
    size_t changed=0,nonzero=0;
    for(size_t i=0;i<ram_bytes;++i) changed+=saved_ram[i]!=expected_ram[i];
    for(int i=0;i<40;++i) for(unsigned j=0;j<a[i].samples;++j)
        nonzero+=a[i].pcm[j][0]!=0 || a[i].pcm[j][1]!=0;
    for(int replay=0;replay<2;++replay) {
        memcpy(ram,saved_ram,ram_bytes); CHECK(apu_replay_restore(r,saved,n));
        sequence(r,b); CHECK(apu_replay_capture(r,actual,n));
        CHECK(!memcmp(a,b,40*sizeof(*a)) && !memcmp(actual,expected,n));
        CHECK(!memcmp(ram,expected_ram,ram_bytes));
    }
    CHECK(!apu_replay_restore(r,saved,n-1));
    ((uint32_t *)saved)[3]=8; CHECK(!apu_replay_restore(r,saved,n));
    CHECK(apu_replay_capture(r,actual,n) && !memcmp(actual,expected,n));
    g_dbg_voice_monitor=1; CHECK(!apu_replay_frame(r,&out)); g_dbg_voice_monitor=-1;
    CHECK(apu_replay_capture(r,actual,n) && !memcmp(actual,expected,n));
    CHECK(g_apu_ram_ptr==NULL);
    if(argc==3) CHECK(changed && nonzero);
    printf("APU processing replay: two repeated restores match state, all RAM and PCM; changed RAM=%zu, nonzero stereo samples=%zu.\n",changed,nonzero);
    apu_replay_destroy(r); free(device); free(ram); free(saved_ram); free(expected_ram);
    free(cmd); free(saved); free(expected); free(actual); free(a); free(b);
    return 0;
}
