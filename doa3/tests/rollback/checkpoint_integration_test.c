/* Production checkpoint domains together, in a private owner-thread harness.
 * Task/stream service bodies are fixtures; scheduler, clock, APC, file cursor
 * and APU processing are real. This neither launches nor rewinds DOA3. */
#include "apu_chip.h"
#include "apu_replay.h"
#ifdef DOA3_CP_LIVE_AUDIO
#include "checkpoint_live_audio.h"
#include "game/rollback_trial.h"
#endif
#include "online/rollback_checkpoint.h"
#ifdef DOA3_CP_RENDERER
#include "checkpoint_renderer.h"
#endif
#include "online/rollback_tasks.h"
#include "online/rollback_workers.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_heap.h"
#include "kernel/kernel_apc.h"
#include "kernel/kernel_interrupt.h"
#include "nv2a/nv2a_frontend.h"
#include "nv2a/nv2a_queue.h"
#include "kernel/kernel_stream.h"
#include "game/rollback_input.h"
#include "game/rollback_input_tape.h"
#include "guest_memory_fixture.h"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
#define RAM_BYTES (128u*1024u*1024u)
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg;
int g_dbg_voice_monitor=-1;
unsigned g_doa3_frames_presented;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];int g_fp_top;
ptrdiff_t g_xbox_mem_offset;
volatile int g_doa3_post_movie=1;
static unsigned char *ram;
static unsigned host_reads,host_metadata,host_rumble;
static int host_disconnected;
static int mismatch_rumble;
DWORD xbox_InputGetState(DWORD port,XBOX_INPUT_STATE *out) {
    ++host_reads;if(host_disconnected)return ERROR_DEVICE_NOT_CONNECTED;
    memset(out,0,sizeof(*out));out->dwPacketNumber=host_reads;
    out->Gamepad.wButtons=(WORD)((host_reads*13+port)&0xFF);
    out->Gamepad.bAnalogButtons[port%8]=(BYTE)host_reads;
    out->Gamepad.sThumbLX=(SHORT)(host_reads*97);return 0;
}
DWORD xbox_InputHostMask(void) { ++host_metadata;return host_disconnected ? 0:15; }
DWORD xbox_InputTakeReconfiguredMask(void) { ++host_metadata;return host_disconnected ? 15:2; }
BYTE xbox_InputGuestSubtype(int port) { ++host_metadata;return (BYTE)(port+1); }
DWORD xbox_InputSetState(DWORD port,const XBOX_VIBRATION *v) { (void)port;(void)v;++host_rumble;return 0; }
uint32_t netplay_filter_pad_mask(uint32_t mask) { return mask; }
uint32_t netplay_forced_pads(void) { return 0; }
void doa3_perf_input_sample(unsigned port) { (void)port; }

/* Use the actual guest XPP overrides, including synthetic packet counters. */
#define guest ram
#define eax g_eax
#define esp g_esp
#define MEM8(a) (*(uint8_t *)(ram+(a)))
#define MEM16(a) (*(uint16_t *)(ram+(a)))
#define MEM32(a) (*(uint32_t *)(ram+(a)))
#include "input_bridge.generated.inc"
#undef guest
#undef eax
#undef esp
static void guest_inputs(unsigned frame) {
    uint32_t saved_sp=g_esp,saved_eax=g_eax;
    g_esp=0x610F00;sub_0021157D();MEM32(0x610000)=g_eax;
    g_esp=0x610F00;MEM32(g_esp+8)=0x610004;MEM32(g_esp+12)=0x610008;sub_0021159F();
    for(unsigned p=0;p<5;++p) {
        g_esp=0x610F00;MEM32(g_esp+4)=0x0AD00001+p%4;MEM32(g_esp+8)=0x610010+p*24;sub_00211A96();
        g_esp=0x610F00;MEM32(g_esp+4)=0x0AD00001+p%4;MEM32(g_esp+8)=0x610100+p*28;sub_002118A4();
    }
    g_esp=0x610F00;MEM32(g_esp+4)=0x0AD00001;MEM32(g_esp+8)=0x610200;
    MEM16(0x610242)=(uint16_t)(frame*1200+mismatch_rumble);MEM16(0x610244)=(uint16_t)(frame*1700);sub_00211B07();
    /* Guest input materially affects the fixture task and downstream workers. */
    for(unsigned p=0;p<5;++p)MEM32(0x600000)+=MEM32(0x610010+p*24)+MEM16(0x610014+p*24);
    g_esp=saved_sp;g_eax=saved_eax;
}

#define WORD(va) (*(uint32_t *)(ram+(va)))
ptrdiff_t xbox_GetMemoryOffset(void) { return (ptrdiff_t)ram; }
int doa3_workers_may_run(void) { return 0; }
unsigned char xbox_current_irql(void) { return 0; }
int doa3_rollback_tasks_enabled(void) { return 1; }
void doa3_rollback_capture_before(uint32_t r,const Doa3RollbackTask *t) { (void)r;(void)t; }
void doa3_rollback_capture_after(uint32_t r,const Doa3RollbackTask *t) { (void)r;(void)t; }
void doa3_rollback_capture_cancel(void) { CHECK(0); }
int doa3_rollback_task_supported(uint32_t r) { return r==1; }
int doa3_rollback_task_checkpoint_valid(uint32_t r,const Doa3RollbackTask *t) { return r==1 && t->resume==1 && t->yielded==1; }
int doa3_rollback_task_step(uint32_t r,Doa3RollbackTask *t) {
    CHECK(r==1);t->locals[0]=t->locals[0]*33+WORD(0x600000);
    WORD(0x600010)=g_eax=t->locals[0];t->resume=t->yielded=1;return 1;
}
int doa3_rollback_worker_supported(uint32_t r) { return r==0x1926F0 || r==0x192760 || r==0x1927D0; }
int doa3_rollback_worker_checkpoint_valid(uint32_t r,const Doa3RollbackWorker *w) {
    return doa3_rollback_worker_supported(r) && w->resume==1 && w->yielded==1 &&
        w->wait_key==(r==0x1927D0 ? 0xBEEF9004u : 0);
}
int doa3_rollback_worker_step(uint32_t r,Doa3RollbackWorker *w) {
    CHECK(doa3_rollback_worker_supported(r));unsigned slot=r==0x1927D0 ? 2 : r==0x192760 ? 1 : 0;
    w->locals[0]=w->locals[0]*13+WORD(0x600010)+slot;
    WORD(0x600020+slot*4)=g_eax=w->locals[0];w->wait_sp=g_esp;
    w->resume=w->yielded=1;w->wait_key=r==0x1927D0 ? xbox_fiber_current_xhandle() : 0;return 1;
}
static void task_entry(void) { (void)xbox_fiber_run_restorable_task(1);CHECK(0); }
static void worker_entry(void) {
    if(xbox_fiber_current_ctx1()==0x1926B0) (void)xbox_fiber_run_restorable_watchdog();
    else (void)xbox_fiber_run_restorable_stream_worker(xbox_fiber_current_ctx1());
    CHECK(0);
}
typedef void (*recomp_func_t)(void);
recomp_func_t recomp_lookup(uint32_t va) { if(va==0x18C690) return worker_entry;CHECK(va==0x1000);return task_entry; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va;return NULL; }
typedef struct World World;
typedef struct { World *world;unsigned id; } Part;
struct World {
    XboxApcQueue apc;
    XboxInterruptState interrupts;
    XboxHeapCheckpoint *heap;
    XboxStreamFiles *files;
    ApuReplay *audio;
    Nv2aFrontend *gpu;
    HANDLE file;
    Part parts[DOA3_CP_PARTS];
    int held,task,fail_restore;
};
static int acquire(void *v) {
    World *w=v;if(w->held || !g_xbox_det_active || !xbox_fiber_is_primary()) return 0;
#ifdef DOA3_CP_LIVE_AUDIO
    if(!fixture_live_acquire())return 0;
#endif
    w->held=1;return 1;
}
static void release(void *v) {
    World *w=v;CHECK(w->held);
#ifdef DOA3_CP_LIVE_AUDIO
    fixture_live_release();
#endif
    w->held=0;
}
static uint64_t epoch(void *v) { (void)v;return xbox_det_epoch(); }
static int capture(void *v,void *out,size_t n) {
    Part *p=v;World *w=p->world;CHECK(w->held);
    switch(p->id) {
    case DOA3_CP_FILES:return xbox_stream_files_capture(w->files,1,out,n);
    case DOA3_CP_RAM:memcpy(out,ram,n);return 1;
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_capture(w->heap,out,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_capture(out,n);
    case DOA3_CP_CLOCK:return xbox_det_capture(out);
    case DOA3_CP_COMPLETIONS:return xbox_apc_capture(&w->apc,out,n);
    case DOA3_CP_INTERRUPTS:return xbox_interrupt_capture(&w->interrupts,xbox_det_epoch(),out,n);
    case DOA3_CP_GPU_FRONTEND:return nv2a_frontend_capture(w->gpu,xbox_det_epoch(),out,n);
    case DOA3_CP_AUDIO:return apu_replay_capture(w->audio,out,n);
    case DOA3_CP_INPUT:return doa3_xpp_capture(out,n);
    default:return 0;
    }
}
static int validate(void *v,const void *saved,size_t n) {
    Part *p=v;World *w=p->world;CHECK(w->held);
    switch(p->id) {
    case DOA3_CP_FILES:return xbox_stream_files_validate(w->files,1,saved,n);
    case DOA3_CP_RAM:return n==RAM_BYTES;
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_validate(w->heap,saved,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_validate(saved,n);
    case DOA3_CP_CLOCK:return n==sizeof(xbox_det_checkpoint) && xbox_det_validate(saved);
    case DOA3_CP_COMPLETIONS:return xbox_apc_validate(&w->apc,saved,n);
    case DOA3_CP_INTERRUPTS:return xbox_interrupt_validate(&w->interrupts,xbox_det_epoch(),saved,n);
    case DOA3_CP_GPU_FRONTEND:return nv2a_frontend_validate(w->gpu,xbox_det_epoch(),saved,n);
    case DOA3_CP_AUDIO:return apu_replay_validate(w->audio,saved,n);
    case DOA3_CP_INPUT:return doa3_xpp_validate(saved,n);
    default:return 0;
    }
}
static int restore_domain(void *v,const void *saved,size_t n) {
    Part *p=v;World *w=p->world;CHECK(w->held);
    switch(p->id) {
    case DOA3_CP_FILES:return xbox_stream_files_restore(w->files,1,saved,n);
    case DOA3_CP_RAM:memcpy(ram,saved,n);return 1;
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_restore(w->heap,saved,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_restore(saved,n);
    case DOA3_CP_CLOCK:return xbox_det_restore(saved);
    case DOA3_CP_COMPLETIONS:return xbox_apc_restore(&w->apc,saved,n);
    case DOA3_CP_INTERRUPTS:return xbox_interrupt_restore(&w->interrupts,xbox_det_epoch(),saved,n);
    case DOA3_CP_GPU_FRONTEND:return nv2a_frontend_restore(w->gpu,xbox_det_epoch(),saved,n);
    case DOA3_CP_AUDIO:return apu_replay_restore(w->audio,saved,n);
    case DOA3_CP_INPUT:return doa3_xpp_restore(saved,n);
    default:return 0;
    }
}
static int restore(void *v,const void *saved,size_t n) {
    Part *p=v;int result=restore_domain(v,saved,n);
    if(result>0 && p->world->fail_restore==(int)p->id) {
        p->world->fail_restore=-1;return 0; // injected failure AFTER actual restore
    }
    return result;
}
static uint32_t gpu_read(void *v,uint32_t address) { (void)v;CHECK(address<=RAM_BYTES-4 && !(address&3));return WORD(address); }
static void gpu_write(void *v,uint32_t address,uint32_t value) { (void)v;CHECK(address<=RAM_BYTES-4 && !(address&3));WORD(address)=value; }
static void advance(World *w,unsigned frame,ApuReplayOutput *out) {
    CHECK(!w->held);xbox_det_frame();xbox_det_on_kernel_call(219);
    /* Cross the real QPC hot-poll threshold inside the recorded window. */
    for(unsigned poll=0;poll<12;++poll){
        xbox_det_on_kernel_call(126);(void)xbox_det_qpc_poll();
        WORD(0x600038)+=(uint32_t)xbox_det_qpc();
    }
    guest_inputs(frame);
    /* Retain allocation references in guest RAM, just like replayed code. */
    unsigned slot=frame%4;uint32_t *block=&WORD(0x600040+slot*4);
    xbox_HeapFree(*block);*block=xbox_HeapAlloc(65+frame*29,frame%3 ? 16 : 4096);CHECK(*block);
    memset(ram+*block,(int)frame,xbox_HeapBlockSize(*block));
    WORD(0x600050)=xbox_HeapAllocHigh(97,64);CHECK(WORD(0x600050));
    WORD(0x600054)=xbox_HeapReserveTop(64,64);CHECK(WORD(0x600054));
    memset(ram+WORD(0x600050),(int)frame,97);memset(ram+WORD(0x600054),(int)frame,64);
    DWORD got;CHECK(ReadFile(w->file,ram+0x601000,16,&got,NULL) && got==16);
    WORD(0x600000)=WORD(0x600000)*17+WORD(0x601000)+frame;
    CHECK(xbox_apc_push(&w->apc,(XboxApcEntry){0x600004,0x600008,got}));
    if(frame%3==0) {
        XboxApcEntry batch[XBOX_APC_CAPACITY];unsigned count=xbox_apc_begin(&w->apc,batch);CHECK(count);
        for(unsigned i=0;i<count;++i) { WORD(batch[i].ctx)+=batch[i].bytes;WORD(batch[i].iosb)=frame; }
        xbox_apc_end(&w->apc);
    }
    CHECK(xbox_interrupt_push(&w->interrupts,(XboxDpcEntry){0x602000+frame*32,frame,WORD(0x600000)}));
    if(frame%3==0) {
        CHECK(xbox_interrupt_begin(&w->interrupts));
        XboxDpcEntry dpc;XboxIsrEntry isr;CHECK(xbox_interrupt_get_isr(&w->interrupts,0,&isr));
        while(xbox_interrupt_pop(&w->interrupts,&dpc)) WORD(isr.ctx)+=dpc.a1+dpc.a2;
        xbox_interrupt_end(&w->interrupts);
    }
    /* Submit two commands per frame, wrapping the ring every eight frames.
     * USER MMIO progress and the guest semaphore are both replayed. */
    uint32_t pos=0x608000+((frame-1)%8)*16;
    WORD(pos)=(1u<<18)|0x200;WORD(pos+4)=WORD(0x600000);
    WORD(pos+8)=(1u<<18)|0x204;WORD(pos+12)=frame;
    WORD(0x604000)=pos+16;WORD(0x604030)=frame+2;
    CHECK(nv2a_frontend_submit(w->gpu,0x604000,gpu_read,gpu_write,NULL));
    uint32_t get;CHECK(nv2a_frontend_read(w->gpu,0xFD9F0044,4,&get) && get==pos+16);
    CHECK(WORD(0x603080)==frame);
    WORD(0x603004)=0x100000+frame*0x1000;
    CHECK(nv2a_frontend_targets(w->gpu,0x604000,0x603000,gpu_read,NULL));
    xbox_fiber_switch_direct(w->task);
    for(unsigned i=0;i<4;++i) CHECK(xbox_fiber_run_thread(0xBEEF9001+i));
    if(frame%2) { ApuReplayOutput wait;CHECK(apu_replay_wait(w->audio,&wait));WORD(0x600030)+=wait.samples; }
    CHECK(apu_replay_frame(w->audio,out));WORD(0x600034)+=apu_replay_take_irq(w->audio);
#ifdef DOA3_CP_RENDERER
    fixture_renderer_advance(frame,WORD(0x600010));
#endif
}
static void read_seed(const char *path,void *out,size_t n) {
    FILE *f=fopen(path,"rb");CHECK(f && fread(out,1,n,f)==n && fgetc(f)==EOF);fclose(f);
}
static uint64_t checkpoint_hash(const void *image,size_t bytes) {
    const unsigned char *p=image;uint64_t h=UINT64_C(14695981039346656037);
    for(size_t i=0;i<bytes;++i)h=(h^p[i])*UINT64_C(1099511628211);
    return h;
}
/* Observation is read-only and independent of restore. With a renderer,
 * serialize each production byte domain and pixel hashes under their common
 * fence. The coordinator deliberately forbids its raw APIs in this case;
 * every restore still goes through its owned-image transaction. */
static int observe_state(World *w,Doa3CheckpointSession *session,uint64_t frame,void *out,size_t n) {
#ifdef DOA3_CP_RENDERER
    unsigned char *p=out;size_t used=sizeof(frame);int ok=1;
    if(!acquire(w))return 0;
    memset(out,0,n);memcpy(p,&frame,sizeof(frame));
    const size_t sizes[DOA3_CP_PARTS]={xbox_stream_files_size(w->files),RAM_BYTES,xbox_heap_checkpoint_size(),xbox_fiber_converted_snapshot_size(),sizeof(xbox_det_checkpoint),sizeof(XboxApcSnapshot),sizeof(XboxInterruptSnapshot),nv2a_frontend_checkpoint_size(),apu_replay_snapshot_size(),sizeof(Doa3XppSnapshot)};
    for(unsigned i=0;i<DOA3_CP_PARTS && ok;++i){
        CHECK(used+sizes[i]<=n);ok=capture(&w->parts[i],p+used,sizes[i]);used+=sizes[i];
    }
    uint64_t hashes[20];CHECK(used+sizeof(hashes)<=n);
    if(ok){fixture_renderer_observe(hashes);memcpy(p+used,hashes,sizeof(hashes));}
    release(w);return ok;
#else
    (void)w;return doa3_checkpoint_capture(session,frame,out,n);
#endif
}
static int restore_state(Doa3CheckpointSession *session,const void *raw,size_t n,
    const Doa3CheckpointImage *owned,uint64_t *frame) {
#ifdef DOA3_CP_RENDERER
    (void)raw;(void)n;return doa3_checkpoint_image_restore(session,owned,frame);
#else
    (void)owned;return doa3_checkpoint_restore(session,raw,n,frame);
#endif
}
#ifdef DOA3_CP_LIVE_AUDIO
typedef struct {
    World *world;Doa3CheckpointSession *session;size_t bytes;void *scratch;
    uint64_t first_hash,*hashes;ApuReplayOutput *out;int held,fail_verify,corrupt_pixels;
} TrialFixture;
static int trial_enter(void *v){TrialFixture *f=v;CHECK(!f->held);f->held=1;return 1;}
static void trial_leave(void *v){TrialFixture *f=v;CHECK(f->held);f->held=0;}
static int trial_begin(void *v){CHECK(((TrialFixture *)v)->held);fixture_live_begin();return 1;}
static int trial_end(void *v,int cancel){CHECK(((TrialFixture *)v)->held);fixture_live_end(cancel);return 1;}
static int trial_advance(void *v,uint64_t frame){
    TrialFixture *f=v;CHECK(f->held && frame>=3 && frame<=18);
    advance(f->world,(unsigned)frame,&f->out[frame-3]);return 1;
}
static int trial_verify(void *v,uint64_t frame){
    TrialFixture *f=v;CHECK(f->held && frame>=2 && frame<=18);
    if(f->fail_verify==(int)frame){f->fail_verify=0;return 0;}
#ifdef DOA3_CP_RENDERER
    if(f->corrupt_pixels==(int)frame){f->corrupt_pixels=0;fixture_renderer_corrupt();}
#endif
    return observe_state(f->world,f->session,frame,f->scratch,f->bytes) &&
        checkpoint_hash(f->scratch,f->bytes)==(frame==2 ? f->first_hash:f->hashes[frame-3]);
}
#endif
int main(int argc,char **argv) {
    CHECK(argc==1 || argc==3);
    SetEnvironmentVariableA("DOA3_MIX_TRACE",NULL);
    _putenv_s("DOA3_ROLLBACK_WORKERS","1");_putenv_s("DOA3_ROLLBACK_STREAM_WORKERS","1");
    _putenv_s("DOA3_RUMBLE_TRACE","");
    ram=guest_memory_fixture_alloc(RAM_BYTES);CHECK(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
    World w={0};w.fail_restore=-1;xbox_apc_reset(&w.apc);
    CHECK(xbox_interrupt_reset(&w.interrupts));
    xbox_interrupt_register(&w.interrupts,(XboxIsrEntry){0x600060,0x1000,0x600064});
    MCPXAPUState *d=calloc(1,sizeof(*d));CHECK(d);
    d->regs[NV_PAPU_VPVADDR]=0x100000;d->regs[NV_PAPU_FENADDR]=0x40000;
    d->regs[NV_PAPU_TVL2D]=5;d->regs[NV_PAPU_TVL3D]=d->regs[NV_PAPU_TVLMP]=0xFFFF;
    d->regs[NV_PAPU_SECTL]=NV_PAPU_SECTL_XCNTMODE;
    d->regs[NV_PAPU_IEN]=NV_PAPU_ISTS_GINTSTS|NV_PAPU_ISTS_FETINTSTS;
    d->regs[NV_PAPU_FETFORCE1]=NV_PAPU_FETFORCE1_SE2FE_IDLE_VOICE;
    WORD(0x100000+5*NV_PAVS_SIZE+NV_PAVS_VOICE_TAR_PITCH_LINK)=0xFFFF;
    for(unsigned i=0;i<MCPX_HW_MAX_VOICES;++i) d->vp.filters[i].voice=(uint16_t)i;
    size_t command_n=apu_rollback_size();void *command=malloc(command_n);CHECK(command);
    ApuRollbackClock apu_clock={1234,0,0,0};CHECK(apu_rollback_export(d,&apu_clock,command,command_n));
    if(argc==3) { read_seed(argv[1],ram,RAM_BYTES);read_seed(argv[2],command,command_n); }
    w.audio=apu_replay_create(ram,RAM_BYTES);CHECK(w.audio && apu_replay_seed(w.audio,command,command_n));
    free(command);free(d);WORD(0x40C718)=0;
    memset(ram+0x600040,0,24);w.heap=xbox_heap_checkpoint_create();CHECK(w.heap);
    Nv2aQueueState gpu_seed={0};gpu_seed.submission=(Nv2aSubmissionState)NV2A_SUBMISSION_INITIAL;
    Nv2aQueueSnapshot gpu_image;CHECK(nv2a_queue_export(&gpu_seed,&gpu_image,sizeof(gpu_image)));
    w.gpu=nv2a_frontend_create(&gpu_image,sizeof(gpu_image));CHECK(w.gpu);
    memset(ram+0x603000,0,0x6000);
    WORD(0x604024)=0x608000;WORD(0x604028)=0x608080;WORD(0x608080)=0x608001;
    WORD(0x606264)=0xFD9F0000;WORD(0x604034)=0x603080;
    WCHAR temp[MAX_PATH],path[MAX_PATH];CHECK(GetTempPathW(MAX_PATH,temp) && GetTempFileNameW(temp,L"rbc",0,path));
    HANDLE writer=CreateFileW(path,GENERIC_WRITE,0,NULL,TRUNCATE_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(writer!=INVALID_HANDLE_VALUE);
    unsigned char data[4096];for(unsigned i=0;i<sizeof(data);++i) data[i]=(unsigned char)(i*37);
    DWORD written;CHECK(WriteFile(writer,data,sizeof(data),&written,NULL) && written==sizeof(data));CloseHandle(writer);
    w.file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(w.file!=INVALID_HANDLE_VALUE);
    w.files=xbox_stream_files_create();CHECK(w.files);
    XboxStreamFileBinding binding={1,0,w.file,path};CHECK(xbox_stream_files_begin(w.files,&binding,1,1));
    xbox_det_bases base={100,200,300};xbox_det_begin(&base);xbox_fiber_init();
    w.task=xbox_fiber_create_dormant(0x1000,0,0x8000);CHECK(w.task>0);xbox_fiber_switch_direct(w.task);
    const uint32_t routines[]={0x1926B0,0x1926F0,0x192760,0x1927D0};
    for(unsigned i=0;i<4;++i) { CHECK(xbox_fiber_spawn(0x18C690,routines[i],0,0xBEEF9001+i,0));CHECK(xbox_fiber_run_thread(0xBEEF9001+i)); }
    Doa3CheckpointConfig config={0};config.context=&w;config.acquire=acquire;config.release=release;config.epoch=epoch;
    size_t sizes[DOA3_CP_PARTS]={xbox_stream_files_size(w.files),RAM_BYTES,xbox_heap_checkpoint_size(),xbox_fiber_converted_snapshot_size(),sizeof(xbox_det_checkpoint),sizeof(XboxApcSnapshot),sizeof(XboxInterruptSnapshot),nv2a_frontend_checkpoint_size(),apu_replay_snapshot_size(),sizeof(Doa3XppSnapshot)};
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) { w.parts[i]=(Part){&w,i};config.parts[i]=(Doa3CheckpointPart){&w.parts[i],sizes[i],capture,validate,restore}; }
    Doa3CheckpointPart input_part=config.parts[DOA3_CP_INPUT];
    memset(&config.parts[DOA3_CP_INPUT],0,sizeof(input_part));
    CHECK(!doa3_checkpoint_create(&config)); /* input bookkeeping cannot be omitted */
    config.parts[DOA3_CP_INPUT]=input_part;
#ifdef DOA3_CP_LIVE_AUDIO
    config.parts[DOA3_CP_AUDIO].can_restore=fixture_live_can_restore;
#endif
#ifdef DOA3_CP_RENDERER
    uint32_t texture=xbox_HeapAlloc(8192,128);CHECK(texture);
    config.renderer=fixture_renderer_init(ram,texture,&w.held,&w,epoch);
#endif
    Doa3CheckpointSession *session=doa3_checkpoint_create(&config);CHECK(session);
    size_t n=doa3_checkpoint_size(session);
#ifdef DOA3_CP_RENDERER
    n+=20*sizeof(uint64_t); /* observation appends pixel hashes */
#endif
    void *saved=malloc(n),*expected=malloc(n),*actual=malloc(n);CHECK(saved && expected && actual);
    ApuReplayOutput warm,a[16],b[16];advance(&w,1,&warm);advance(&w,2,&warm);
    CHECK(w.apc.count==2 && w.interrupts.dpc_count==2 && observe_state(&w,session,2,saved,n));
#ifdef DOA3_CP_LIVE_AUDIO
    Doa3CheckpointImage *start=doa3_checkpoint_image_capture(session,2);CHECK(start);
#else
    Doa3CheckpointImage *start=NULL;
#endif
    Doa3CheckpointImage *finish=NULL;
    Doa3InputTape *tape=doa3_input_tape_create(16);CHECK(tape);
    uint64_t expected_hashes[16];
    for(unsigned i=0;i<16;++i) {
        CHECK(doa3_input_tape_record(tape,i+3));advance(&w,i+3,&a[i]);CHECK(doa3_input_tape_end(tape));
        CHECK(observe_state(&w,session,i+3,expected,n));expected_hashes[i]=checkpoint_hash(expected,n);
    }
#ifdef DOA3_CP_RENDERER
    finish=doa3_checkpoint_image_capture(session,18);CHECK(finish);
    Doa3CheckpointComparison exact;
    CHECK(doa3_checkpoint_image_matches(session,finish,&exact)==1);
    CHECK(!doa3_checkpoint_capture(session,18,actual,n));
    CHECK(!doa3_checkpoint_restore(session,saved,n,NULL));
    CHECK(fixture_renderer_images()==2);
#endif
    unsigned reads=host_reads,metadata=host_metadata,rumble=host_rumble;host_disconnected=1;
#ifdef DOA3_CP_LIVE_AUDIO
    unsigned sent=fixture_live_submissions(),queries=fixture_live_queue_queries();CHECK(sent);
    uint64_t rejected_frame=999;
    CHECK(!restore_state(session,saved,n,start,&rejected_frame) && rejected_frame==999);
    CHECK(!doa3_checkpoint_poisoned(session)); /* output gate preflight rejects before any restore */
    CHECK(observe_state(&w,session,18,actual,n) && !memcmp(expected,actual,n));
    fixture_live_deny_pin(1);CHECK(!restore_state(session,saved,n,start,NULL));fixture_live_deny_pin(0);
    CHECK(fixture_live_acquire());fixture_live_release(); /* clock pin unwound when audio pin failed */
    TrialFixture trial_fixture={&w,session,n,actual,checkpoint_hash(saved,n),expected_hashes,b,0,0};
    Doa3TrialConfig trial_config={&trial_fixture,session,tape,trial_enter,trial_leave,trial_begin,trial_end,trial_advance,trial_verify};
    Doa3ReplayTrial *trial=doa3_trial_create(&trial_config);CHECK(trial);
#endif
    for(unsigned replay=0;replay<3;++replay) {
#ifdef DOA3_CP_LIVE_AUDIO
        CHECK(doa3_trial_run(trial,start,2,18)==1);
        Doa3TrialReport report;CHECK(doa3_trial_report(trial,&report));
        CHECK(report.advanced==16 && report.reason==DOA3_TRIAL_OK && !report.fatal && !report.recovered);
#else
        uint64_t frame=999;CHECK(restore_state(session,saved,n,start,&frame)==1 && frame==2 && w.apc.count==2 && w.interrupts.dpc_count==2);
        CHECK(xbox_det_frames()==2 && WORD(XBOX_KERNEL_DATA_BASE+KDATA_TICK_COUNT)==133);
        for(unsigned i=0;i<16;++i) {
            CHECK(doa3_input_tape_replay(tape,i+3));advance(&w,i+3,&b[i]);CHECK(doa3_input_tape_end(tape));
            CHECK(observe_state(&w,session,i+3,actual,n));CHECK(checkpoint_hash(actual,n)==expected_hashes[i]);
        }
#endif
        CHECK(!memcmp(a,b,sizeof(a)) && !memcmp(expected,actual,n));
        CHECK(host_reads==reads && host_metadata==metadata && host_rumble==rumble);
#ifdef DOA3_CP_RENDERER
        CHECK(doa3_checkpoint_image_matches(session,finish,&exact)==1);
#endif
#ifdef DOA3_CP_LIVE_AUDIO
        CHECK(fixture_live_submissions()==sent && fixture_live_queue_queries()==queries);
#endif
    }
#ifdef DOA3_CP_LIVE_AUDIO
    trial_fixture.fail_verify=5;CHECK(doa3_trial_run(trial,start,2,18)==0);
    Doa3TrialReport recovery;CHECK(doa3_trial_report(trial,&recovery));
    CHECK(recovery.recovered && !recovery.fatal && recovery.advanced==3 && recovery.reason==DOA3_TRIAL_VERIFY);
    CHECK(!memcmp(expected,actual,n) && !trial_fixture.held);
    CHECK(fixture_live_submissions()==sent && fixture_live_queue_queries()==queries);
    CHECK(host_reads==reads && host_metadata==metadata && host_rumble==rumble);
#ifdef DOA3_CP_RENDERER
    /* Corrupt only GPU pixels. The ordinary verifier must detect it, then the
     * controller must restore both CPU/audio and GPU end-state together. */
    trial_fixture.corrupt_pixels=5;CHECK(doa3_trial_run(trial,start,2,18)==0);
    CHECK(doa3_trial_report(trial,&recovery));
    CHECK(recovery.recovered && !recovery.fatal && recovery.advanced==3 && recovery.reason==DOA3_TRIAL_VERIFY);
    CHECK(!memcmp(expected,actual,n) && !trial_fixture.held && fixture_renderer_images()==2);
    /* Fail after renderer restore has actually mutated GPU images. */
    fixture_renderer_fail_restore();CHECK(doa3_trial_run(trial,start,2,18)==0);
    CHECK(doa3_trial_report(trial,&recovery));
    CHECK(recovery.recovered && !recovery.fatal && recovery.advanced==0 && recovery.reason==DOA3_TRIAL_RESTORE);
    CHECK(!memcmp(expected,actual,n) && fixture_renderer_images()==2);
    CHECK(fixture_live_submissions()==sent && fixture_live_queue_queries()==queries);
    CHECK(host_reads==reads && host_metadata==metadata && host_rumble==rumble);
#endif
    CHECK(doa3_trial_destroy(trial));
#endif
    /* Failure after each real domain applies must recover the entire before-image. */
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
#ifdef DOA3_CP_LIVE_AUDIO
        fixture_live_begin();
#endif
        uint64_t frame=999;w.fail_restore=(int)i;
        CHECK(!restore_state(session,saved,n,start,&frame) && frame==999);
        CHECK(!doa3_checkpoint_poisoned(session));
        CHECK(observe_state(&w,session,18,actual,n) && !memcmp(expected,actual,n));
#ifdef DOA3_CP_LIVE_AUDIO
        fixture_live_end(1);
#endif
    }
    /* A bad recorded-input call after mutation must remain gated, then allow
     * restoration of the original end-state without touching physical input. */
#ifdef DOA3_CP_LIVE_AUDIO
    fixture_live_begin();
#endif
    CHECK(restore_state(session,saved,n,start,NULL)==1);
    CHECK(doa3_input_tape_replay(tape,3));mismatch_rumble=1;advance(&w,3,&warm);
    CHECK(!doa3_input_tape_end(tape));
    XBOX_INPUT_STATE denied;CHECK(doa3_input_state(0,&denied)==ERROR_INVALID_DATA);
    CHECK(restore_state(session,expected,n,finish,NULL)==1);
    CHECK(doa3_input_tape_abort(tape));mismatch_rumble=0;
    CHECK(observe_state(&w,session,18,actual,n) && !memcmp(expected,actual,n));
    CHECK(host_reads==reads && host_metadata==metadata && host_rumble==rumble);
#ifdef DOA3_CP_LIVE_AUDIO
    fixture_live_end(1);CHECK(fixture_live_submissions()==sent && fixture_live_queue_queries()==queries);
#endif
    /* In-flight interrupt delivery forbids the entire transaction. */
#ifdef DOA3_CP_LIVE_AUDIO
    fixture_live_begin();
#endif
    CHECK(xbox_interrupt_begin(&w.interrupts));
    CHECK(!restore_state(session,saved,n,start,NULL));
    xbox_interrupt_end(&w.interrupts);
    CHECK(observe_state(&w,session,18,actual,n) && !memcmp(expected,actual,n));
    /* A queue reset invalidates the bundle before file positions/RAM move. */
    xbox_apc_reset(&w.apc);CHECK(observe_state(&w,session,18,expected,n));
    CHECK(!restore_state(session,saved,n,start,NULL));
    CHECK(observe_state(&w,session,18,actual,n) && !memcmp(expected,actual,n));
    unsigned nonzero=0;for(unsigned i=0;i<16;++i) for(unsigned j=0;j<a[i].samples;++j) nonzero+=a[i].pcm[j][0]!=0 || a[i].pcm[j][1]!=0;
#ifdef DOA3_CP_LIVE_AUDIO
    fixture_live_end(1);
#endif
    if(argc==3) CHECK(nonzero);
    printf("Coordinated replay: three 16-frame replays match every frame's complete checkpoint hash and final bytes: 128 MiB RAM, allocator, five fibers, clock, file cursor, completions/interrupts, GPU frontend, private APU/PCM (%u nonzero samples), XPP. Actual guest input overrides make zero replay hardware calls. A mismatched-input replay recovers the original end-state.\n",nonzero);
#ifdef DOA3_CP_LIVE_AUDIO
    puts("Production audio variant: real clock/APU fences, output-gate preflight, staged PCM restore, all-domain compensation; zero replay submissions or queue waits. Transport and game task bodies remain fixtures.");
#endif
#ifdef DOA3_CP_RENDERER
    puts("Combined CPU/audio/WARP trial: every frame matches state and GPU pixel hashes; pixel divergence and post-renderer-restore failure recover the original full end-state. No live fight is replayed.");
#endif
    if(start)CHECK(doa3_checkpoint_image_discard(session,start));
    if(finish)CHECK(doa3_checkpoint_image_discard(session,finish));
    CHECK(doa3_input_tape_destroy(tape));
    doa3_checkpoint_destroy(session);free(saved);free(expected);free(actual);
#ifdef DOA3_CP_RENDERER
    fixture_renderer_shutdown();
#endif
    nv2a_frontend_destroy(w.gpu);xbox_heap_checkpoint_destroy(w.heap);xbox_stream_files_destroy(w.files);CloseHandle(w.file);CHECK(DeleteFileW(path));apu_replay_destroy(w.audio);
    /* Fibers still point into private RAM; process exit owns their teardown. */
    return 0;
}
