#include "rollback_fight_checkpoint.h"
#include "rollback_input.h"
#include "recomp/recomp_types.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_fiber.h"
#include "kernel/xbox_heap.h"
#include "kernel/kernel_stream.h"
#include "kernel/kernel_apc.h"
#include "kernel/kernel_interrupt.h"
#include "kernel/kernel_replay.h"
#include "online/xbox_det.h"
#include "apu/apu_rollback.h"
#include "apu/apu_live_checkpoint.h"
#include "nv2a/nv2a_queue.h"
#include "nv2a/nv2a_renderer_checkpoint.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern unsigned char xbox_current_irql(void);
/* Primary registers were NOT included by the converted-fiber snapshot. */
typedef struct {
    uint32_t schema,regs[16];double fp[8];Doa3LoopState loop;
} FightCpu;
typedef struct {struct Doa3FightCheckpoint *owner;unsigned id;} Binding;
static const char *part_names[]={"files","RAM/CPU","heap","fibers","clock","completions","interrupts","GPU frontend","audio","input"};
struct Doa3FightCheckpoint {
    DWORD owner;uint32_t epoch;int held,files,roundtrip_pinned;
    void *ram;size_t ram_bytes;
    Doa3LoopState *loop;
    XboxHeapCheckpoint *heap;
    Nv2aRendererCheckpointSource renderer;
    Binding bindings[DOA3_CP_PARTS];
    Doa3CheckpointSession *session;
};
static int context(const Doa3FightCheckpoint *s){
    return s && s->owner==GetCurrentThreadId() && xbox_fiber_is_primary() &&
        g_xbox_det_active && s->epoch==xbox_det_epoch() && !xbox_current_irql() &&
        s->ram && s->ram==xbox_GetMemoryBase() && s->ram_bytes==xbox_GetMemorySize() &&
        (ptrdiff_t)s->ram==g_xbox_mem_offset && !((unsigned char *)s->ram)[0x24] && doa3_loop_valid(s->loop);
}
static int acquire_impl(void *v,int report){
    Doa3FightCheckpoint *s=v;
    if(!context(s) || s->held)return 0;
    /* The restore-only probe executes no guest work. Keep its physical pins
     * for the entire transaction, while retaining nonrecursive callback entry.
     * Releasing/reacquiring try-locks between restore and verification can
     * otherwise turn ordinary host-thread contention into a fatal test. */
    if(s->roundtrip_pinned){s->held=1;return 1;}
    if(!xbox_det_checkpoint_acquire()){
        if(report)fprintf(stderr,"[ROLLBACK-CHECKPOINT] acquire refused: clock pin\n");return 0;
    }
    if(!mcpx_apu_rollback_acquire()){
        if(report)fprintf(stderr,"[ROLLBACK-CHECKPOINT] acquire refused: audio pin\n");
        xbox_det_checkpoint_release();return 0;
    }
    s->held=1;return 1;
}
static int acquire(void *v){return acquire_impl(v,1);}
/* Only before the first restore: allow a background audio thread to finish
 * holding its lock. Every failed attempt releases the clock pin. No guest
 * instructions, restores or gate changes run while waiting. The full saved
 * endpoint is still independently compared after acquiring both pins. */
static int acquire_before_rewind(Doa3FightCheckpoint *s){
    for(unsigned attempt=0;attempt<8;++attempt){
        if(!context(s) || s->held || s->roundtrip_pinned)return 0;
        if(acquire_impl(s,attempt==7)){
            if(attempt)fprintf(stderr,"[ROLLBACK-WINDOW] physical pins acquired after %u retries\n",attempt);
            return 1;
        }
        if(attempt<7)Sleep(1);
    }
    return 0;
}
static void release(void *v){
    Doa3FightCheckpoint *s=v;s->held=0;
    if(s->roundtrip_pinned)return;
    mcpx_apu_rollback_release();xbox_det_checkpoint_release();
}
static uint64_t epoch(void *v){Doa3FightCheckpoint *s=v;return context(s)?s->epoch:0;}
static void cpu_capture(FightCpu *p,Doa3LoopState *loop){
    uint32_t r[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp,g_str_a,g_str_b,
        g_flg_a,g_flg_b,(uint32_t)g_flg_w,(uint32_t)g_flg_test,(uint32_t)g_fp_top,g_x87_cw};
    memset(p,0,sizeof(*p));p->schema=1;memcpy(p->regs,r,sizeof r);memcpy(p->fp,g_fp_stack,sizeof p->fp);p->loop=*loop;
}
static void cpu_restore(const FightCpu *p,Doa3LoopState *loop){
    const uint32_t *r=p->regs;
    g_eax=r[0];g_ecx=r[1];g_edx=r[2];g_esp=r[3];g_ebx=r[4];g_esi=r[5];g_edi=r[6];g_seh_ebp=r[7];
    g_str_a=r[8];g_str_b=r[9];g_flg_a=r[10];g_flg_b=r[11];g_flg_w=(int)r[12];g_flg_test=(int)r[13];
    g_fp_top=(int)r[14];g_x87_cw=(uint16_t)r[15];memcpy(g_fp_stack,p->fp,sizeof p->fp);*loop=p->loop;
}
static int capture_part(void *v,void *out,size_t n){
    Binding *b=v;Doa3FightCheckpoint *s=b->owner;if(!s->held)return 0;
    switch(b->id){
    case DOA3_CP_RAM:{FightCpu cpu;cpu_capture(&cpu,s->loop);memcpy(out,&cpu,sizeof cpu);memcpy((char *)out+sizeof cpu,s->ram,s->ram_bytes);return 1;}
    case DOA3_CP_FILES:return xbox_stream_file_checkpoint_capture(out,n);
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_capture(s->heap,out,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_capture(out,n);
    case DOA3_CP_CLOCK:return xbox_det_capture(out);
    case DOA3_CP_COMPLETIONS:return xbox_stream_completion_capture(out,n);
    case DOA3_CP_INTERRUPTS:return xbox_kernel_interrupt_capture(out,n);
    case DOA3_CP_GPU_FRONTEND:return doa3_gpu_frontend_capture(out,n);
    case DOA3_CP_AUDIO:return mcpx_apu_processing_capture(out,n);
    case DOA3_CP_INPUT:return doa3_xpp_capture(out,n);
    default:return 0;
    }
}
static int capture(void *v,void *out,size_t n){
    int result=capture_part(v,out,n);
    if(result<=0)fprintf(stderr,"[ROLLBACK-CHECKPOINT] capture refused: %s\n",part_names[((Binding *)v)->id]);
    return result;
}
static int validate_part(void *v,const void *in,size_t n){
    Binding *b=v;Doa3FightCheckpoint *s=b->owner;if(!s->held)return 0;
    switch(b->id){
    case DOA3_CP_RAM:{const FightCpu *p=in;return n==sizeof(*p)+s->ram_bytes && p->schema==1 &&
        p->regs[15]<=UINT16_MAX && doa3_loop_valid(&p->loop);}
    case DOA3_CP_FILES:return xbox_stream_file_checkpoint_validate(in,n);
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_validate(s->heap,in,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_validate(in,n);
    case DOA3_CP_CLOCK:return xbox_det_validate(in);
    case DOA3_CP_COMPLETIONS:return xbox_stream_completion_validate(in,n);
    case DOA3_CP_INTERRUPTS:return xbox_kernel_interrupt_validate(in,n);
    case DOA3_CP_GPU_FRONTEND:return doa3_gpu_frontend_validate(in,n);
    case DOA3_CP_AUDIO:return mcpx_apu_processing_validate(in,n);
    case DOA3_CP_INPUT:return doa3_xpp_validate(in,n);
    default:return 0;
    }
}
static int validate(void *v,const void *in,size_t n){
    int result=validate_part(v,in,n);
    if(result<=0)fprintf(stderr,"[ROLLBACK-CHECKPOINT] validation refused: %s\n",part_names[((Binding *)v)->id]);
    return result;
}
static int can_restore(void *v){
    Binding *b=v;return b->owner->held && xbox_kernel_replay_owned() && mcpx_apu_processing_can_restore();
}
static int restore(void *v,const void *in,size_t n){
    Binding *b=v;Doa3FightCheckpoint *s=b->owner;if(!can_restore(v))return 0;
    switch(b->id){
    case DOA3_CP_RAM:memcpy(s->ram,(const char *)in+sizeof(FightCpu),s->ram_bytes);cpu_restore(in,s->loop);return 1;
    case DOA3_CP_FILES:return xbox_stream_file_checkpoint_restore(in,n);
    case DOA3_CP_HEAP:return xbox_heap_checkpoint_restore(s->heap,in,n);
    case DOA3_CP_FIBERS:return xbox_fiber_converted_restore(in,n);
    case DOA3_CP_CLOCK:return xbox_det_restore(in);
    case DOA3_CP_COMPLETIONS:return xbox_stream_completion_restore(in,n);
    case DOA3_CP_INTERRUPTS:return xbox_kernel_interrupt_restore(in,n);
    case DOA3_CP_GPU_FRONTEND:return doa3_gpu_frontend_restore(in,n);
    case DOA3_CP_AUDIO:return mcpx_apu_processing_restore(in,n);
    case DOA3_CP_INPUT:return doa3_xpp_restore(in,n);
    default:return 0;
    }
}
Doa3FightCheckpoint *doa3_fight_checkpoint_create(Doa3LoopState *loop){
    Doa3FightCheckpoint *s=calloc(1,sizeof(*s));if(!s)return NULL;
    s->owner=GetCurrentThreadId();s->epoch=xbox_det_epoch();s->loop=loop;
    s->ram=xbox_GetMemoryBase();s->ram_bytes=xbox_GetMemorySize();
    if(s->ram_bytes!=128u*1024u*1024u || !context(s) || !doa3_loop_at_boundary(loop))goto fail;
    s->heap=xbox_heap_checkpoint_create();if(!s->heap)goto fail;
    if(!xbox_stream_file_checkpoint_begin())goto fail;s->files=1;
    if(!acquire(s))goto fail;
    int renderer_ok=pgraph_d3d11_rollback_snapshot_source(&s->renderer);release(s);
    if(!renderer_ok)goto fail;
    Doa3CheckpointConfig c={0};c.context=s;c.acquire=acquire;c.release=release;c.epoch=epoch;
    size_t sizes[]={xbox_stream_file_checkpoint_size(),sizeof(FightCpu)+s->ram_bytes,
        xbox_heap_checkpoint_size(),xbox_fiber_converted_snapshot_size(),sizeof(xbox_det_checkpoint),
        xbox_stream_completion_snapshot_size(),sizeof(XboxInterruptSnapshot),sizeof(Nv2aQueueSnapshot),
        mcpx_apu_processing_size(),sizeof(Doa3XppSnapshot)};
    for(unsigned i=0;i<DOA3_CP_PARTS;++i){s->bindings[i]=(Binding){s,i};c.parts[i]=(Doa3CheckpointPart){&s->bindings[i],sizes[i],capture,validate,restore,can_restore};}
    c.renderer=nv2a_renderer_checkpoint_resource(&s->renderer);
    s->session=doa3_checkpoint_create(&c);if(!s->session)goto fail;
    return s;
fail:
    if(s->files)xbox_stream_file_checkpoint_end();
    xbox_heap_checkpoint_destroy(s->heap);free(s);return NULL;
}
Doa3CheckpointSession *doa3_fight_checkpoint_session(Doa3FightCheckpoint *s)
{return context(s)?s->session:NULL;}
int doa3_fight_checkpoint_destroy(Doa3FightCheckpoint *s){
    if(!s)return 1;
    if(!context(s) || s->held || s->roundtrip_pinned || !doa3_checkpoint_dispose(s->session))return 0;
    xbox_stream_file_checkpoint_end();xbox_heap_checkpoint_destroy(s->heap);free(s);return 1;
}
int doa3_fight_checkpoint_roundtrip(Doa3LoopState *loop,Doa3CheckpointComparison *comparison){
    static Doa3FightCheckpoint *fatal_state;
    if(fatal_state)return -1;
    if(!comparison)return 0;
    Doa3FightCheckpoint *s=doa3_fight_checkpoint_create(loop);if(!s)return 0;
    /* The kernel guard itself briefly takes the clock pin; open it before
     * retaining the transaction's nonrecursive clock/APU pins. */
    if(!xbox_kernel_replay_begin()){
        if(!doa3_fight_checkpoint_destroy(s)){fatal_state=s;return -1;}
        return 0;
    }
    /* Only this no-advance identity test owns a continuous pin. Frame replay
     * must use the ordinary per-operation pins so simulation can execute. */
    if(!acquire(s)){
        if(!xbox_kernel_replay_end(1)){fatal_state=s;return -1;}
        if(!doa3_fight_checkpoint_destroy(s)){fatal_state=s;return -1;}
        return 0;
    }
    s->roundtrip_pinned=1;release(s);
    Doa3CheckpointImage *image=doa3_checkpoint_image_capture(s->session,0);
    int guarding=1,gated=0,result=0;
    const char *stage="capture";
    if(!image)goto done;
    stage="audio gate acquire";
    if(!acquire(s))goto done;
    stage="audio gate begin";
    gated=mcpx_apu_replay_begin();release(s);if(!gated)goto done;
    uint64_t frame=UINT64_MAX;
    int restored=doa3_checkpoint_image_restore(s->session,image,&frame);
    /* Even a compensated refusal must be independently verified before the
     * output gate closes. No guest instruction executes during this test. */
    stage="restore";
    fprintf(stderr,"[ROLLBACK-CHECKPOINT] restore result=%d frame=%llu poisoned=%d\n",
        restored,(unsigned long long)frame,doa3_checkpoint_poisoned(s->session));
    if(restored<0)goto fatal;
    stage="compare";
    int matches=doa3_checkpoint_image_matches(s->session,image,comparison);
    fprintf(stderr,"[ROLLBACK-CHECKPOINT] compare result=%d\n",matches);
    if(matches==0){
        fprintf(stderr,"[ROLLBACK-CHECKPOINT] differences mask=%03X renderer=%d\n",
            comparison->different_parts,comparison->renderer_different);
        for(unsigned i=0;i<DOA3_CP_PARTS;++i)if(comparison->different_parts&(1u<<i)){
            size_t offset=comparison->first_byte[i];
            fprintf(stderr,"[ROLLBACK-CHECKPOINT] %s first byte=0x%llX\n",part_names[i],(unsigned long long)offset);
            if(i==DOA3_CP_RAM && offset>=sizeof(FightCpu))
                fprintf(stderr,"[ROLLBACK-CHECKPOINT] guest RAM address=0x%08llX\n",(unsigned long long)(offset-sizeof(FightCpu)));
        }
    }
    if(matches!=1)goto fatal;
    result=restored==1 && frame==0;
    stage="audio gate end acquire";
    if(!acquire(s))goto fatal;
    stage="audio gate end";
    int ended=mcpx_apu_replay_end(!result);release(s);if(!ended)goto fatal;gated=0;
done:
    if(!result)fprintf(stderr,"[ROLLBACK-CHECKPOINT] safely refused at %s\n",stage);
    stage="image discard";
    if(image && !doa3_checkpoint_image_discard(s->session,image))goto fatal;
    stage="kernel guard end";
    if(guarding && !xbox_kernel_replay_end(!result))goto fatal;
    s->roundtrip_pinned=0;
    mcpx_apu_rollback_release();xbox_det_checkpoint_release();
    stage="binding destroy";
    if(!doa3_fight_checkpoint_destroy(s))goto fatal;
    return result;
fatal:
    fprintf(stderr,"[ROLLBACK-CHECKPOINT] fatal stage=%s\n",stage);fflush(stderr);
    (void)gated;fatal_state=s;return -1;
}

/* ---- doa3.1-rollback: render-skip memory map (diagnostic) ----------------
 * Every differing guest RAM range (coalesced across gaps under 64 bytes),
 * labelled with the push-buffer ring and the D3D device block. The aim is to
 * learn which memory a skipped render leaves different, so it can be shown to
 * be render-only. Nothing is excluded from the comparison here. */
extern volatile uint32_t g_doa3_pb_dev;   /* recomp_manual.c, last KickOff device */
/* Known render-only memory (labels only; nothing is excluded):
 *   push-buffer ring       device +0x24 .. +0x28 (GPU commands)
 *   D3D device             device block (write cursor, fences, state shadow)
 *   deform scratch         0x587FF0..0x588010, used only by 0x52046
 *   fence 0x9B23A0         D3D fence from 0x1DC740 (0x1E28B0) each frame
 *   render arena           0xAD8A78..0xB58A78 bump allocator (0x180490 resets
 *                          it every frame; 0x17DD20 / 0x17E1D0 allocate) and the
 *                          block from 0xB58A88 it points the renderer at */
static const char *diff_label(uint32_t a)
{
    uint32_t dev=g_doa3_pb_dev;
    if(dev && dev<0x08000000u){
        uint32_t lo=MEM32(dev+0x24),hi=MEM32(dev+0x28)+4;
        if(a>=lo && a<hi)return "push-buffer ring";
        if(a>=dev && a<dev+0x2400)return "D3D device";
    }
    if(a>=0x587FF0u && a<0x588010u)return "deform scratch";
    if(a>=0x9B23A0u && a<0x9B23A4u)return "fence 0x9B23A0";
    if(a>=0xAD8A78u && a<0xB5A000u)return "render arena";
    return "";
}
/* rollback.7: what the differing dwords of an unlabelled range look like.
 * A fence stamp is a value at or below its image's GPU reference counter
 * and within 0x8000 of it (D3D writes the counter into a resource's Lock
 * word each time a draw uses it, so skipped draws leave older stamps). A
 * push-buffer pointer points into the ring. Stack ranges name the fiber and
 * whether the bytes are below its saved stack pointer (dead). */
/* rollback.12: totals of the last RAM diff map, for the logic-only test. */
size_t g_doa3_diff_unexplained_bytes,g_doa3_diff_unexplained_ranges;
static uint32_t rd32(const unsigned char *p,size_t n,size_t off){uint32_t v=0;if(off+4<=n)memcpy(&v,p+off,4);return v;}
static void diff_map(unsigned part,const unsigned char *a,const unsigned char *b,size_t n)
{
    static int lines;
    size_t i=0,ranges=0,bytes=0,other_ranges=0,other_bytes=0,unexplained_ranges=0,unexplained_bytes=0;
    const size_t hdr=sizeof(FightCpu);
    uint32_t dev=g_doa3_pb_dev,lo=0,hi=0,ctr_at=0,ca=0,cb=0;
    if(dev && dev<0x08000000u){lo=MEM32(dev+0x24);hi=MEM32(dev+0x28)+4;}
    fprintf(stderr,"[ROLLBACK-DIFFMAP] part=%s bytes=%zu\n",part_names[part],n);
    if(part==DOA3_CP_RAM && lo>=0x1000u){
        ctr_at=lo-0x1000u;ca=rd32(a,n,hdr+ctr_at);cb=rd32(b,n,hdr+ctr_at);
        fprintf(stderr,"[ROLLBACK-DIFFMAP] GPU reference counter (ring - 0x1000) @%08X: reference %X replay %X\n",ctr_at,ca,cb);
    }
    while(i<n){
        if(a[i]==b[i]){++i;continue;}
        size_t start=i,end=i+1,gap=0;
        for(size_t j=i+1;j<n && gap<64;++j){ if(a[j]!=b[j]){end=j+1;gap=0;} else ++gap; }
        size_t diff=0;for(size_t j=start;j<end;++j)diff+=a[j]!=b[j];
        ++ranges;bytes+=diff;
        if(part==DOA3_CP_RAM && start>=hdr){
            uint32_t ga=(uint32_t)(start-hdr);
            const char *lab=diff_label(ga);
            char what[160]="";int fib=-1;uint32_t routine=0;int stk=0,explained=0;
            if(!*lab){
                unsigned dw=0,fence=0,ptr=0;
                for(size_t j=start&~(size_t)3;j<end;j+=4){
                    uint32_t va=rd32(a,n,j),vb=rd32(b,n,j);
                    if(va==vb)continue;
                    ++dw;
                    if(ca && va<=ca && va+0x8000u>ca && vb<=cb && vb+0x8000u>cb)++fence;
                    else if(lo && va>=lo && va<hi && vb>=lo && vb<hi)++ptr;
                }
                stk=xbox_fiber_stack_of(ga,&fib,&routine);
                if(stk)snprintf(what,sizeof what,"fiber %d (routine %08X) %s stack; ",fib,routine,stk==1 ? "dead":"live");
                snprintf(what+strlen(what),sizeof what-strlen(what),"%u differing dwords: %u fence stamps, %u push-buffer pointers",dw,fence,ptr);
                explained=stk==1 || (dw && fence+ptr==dw);
                other_ranges++;other_bytes+=diff;
                if(!explained){unexplained_ranges++;unexplained_bytes+=diff;}
            }
            if(lines<600){
                ++lines;
                fprintf(stderr,"[ROLLBACK-DIFFMAP]   guest %08X-%08X (%zu bytes differ) %s%s%s\n",
                        ga,(uint32_t)(end-hdr)-1,diff,lab,*what ? what:"",(*what && !explained) ? " -- UNEXPLAINED":"");
                if(!*lab && !explained){   /* both values, dword-aligned, first 32 bytes */
                    size_t s0=start&~(size_t)3,s1=s0+32<end ? s0+32:((end+3)&~(size_t)3);
                    char la[200],lb[200];int pa=0,pb=0;
                    for(size_t j=s0;j<s1 && j+4<=n;j+=4){
                        uint32_t va,vb;memcpy(&va,a+j,4);memcpy(&vb,b+j,4);
                        pa+=snprintf(la+pa,sizeof la-pa," %08X",va);pb+=snprintf(lb+pb,sizeof lb-pb," %08X",vb);
                    }
                    fprintf(stderr,"[ROLLBACK-DIFFMAP]     reference @%08X:%s\n",(uint32_t)(s0-hdr),la);
                    fprintf(stderr,"[ROLLBACK-DIFFMAP]     replay    @%08X:%s\n",(uint32_t)(s0-hdr),lb);
                }
            }
        }else if(lines<600){
            ++lines;
            fprintf(stderr,"[ROLLBACK-DIFFMAP]   offset %zX-%zX (%zu bytes differ)%s\n",
                    start,end-1,diff,part==DOA3_CP_RAM ? " CPU/loop header":"");
        }
        i=end;
    }
    fprintf(stderr,"[ROLLBACK-DIFFMAP] part=%s ranges=%zu differing_bytes=%zu\n",part_names[part],ranges,bytes);
    if(part==DOA3_CP_RAM){
        fprintf(stderr,"[ROLLBACK-DIFFMAP] outside the labelled render-only memory: ranges=%zu differing_bytes=%zu\n",other_ranges,other_bytes);
        fprintf(stderr,"[ROLLBACK-DIFFMAP] not fence stamps, push-buffer pointers or dead stack: ranges=%zu differing_bytes=%zu\n",unexplained_ranges,unexplained_bytes);
        g_doa3_diff_unexplained_bytes=unexplained_bytes;g_doa3_diff_unexplained_ranges=unexplained_ranges;
    }
    fflush(stderr);
}
void doa3_fight_checkpoint_diff_map_enable(void)
{
    extern void (*g_doa3_checkpoint_diff_hook)(unsigned,const unsigned char *,const unsigned char *,size_t);
    g_doa3_checkpoint_diff_hook=diff_map;
}

static double diagnostic_ms(void){
    LARGE_INTEGER count,frequency;QueryPerformanceCounter(&count);QueryPerformanceFrequency(&frequency);
    return (double)count.QuadPart*1000.0/(double)frequency.QuadPart;
}
static int visit(Doa3FightCheckpoint *s,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,Doa3CheckpointComparison *difference,
    int (*advance)(void *),void *advance_context,int pixel_probe){
    if(!context(s) || s->held || s->roundtrip_pinned || !start || !end ||
       !difference || first>=last || doa3_checkpoint_poisoned(s->session)){
        fprintf(stderr,"[ROLLBACK-WINDOW] refused before rewind: context/arguments\n");return 0;
    }
    if(!xbox_kernel_replay_begin()){
        fprintf(stderr,"[ROLLBACK-WINDOW] refused before rewind: kernel guard/clock pin\n");return 0;
    }
    if(!acquire_before_rewind(s)){
        fprintf(stderr,"[ROLLBACK-WINDOW] refused before rewind: physical pins\n");
        return xbox_kernel_replay_end(1) ? 0:-1;
    }
    s->roundtrip_pinned=1;release(s);
    const char *stage="endpoint preflight";
    int result=0,gated=0;
    double restore_ms=0,replay_ms=0,compare_ms=0,recovery_ms=0,t=0;
    uint64_t frame=UINT64_MAX;
    if(doa3_checkpoint_image_matches(s->session,end,difference)!=1)goto done;
    stage="audio gate";
    if(!acquire(s))goto done;
    gated=mcpx_apu_replay_begin();release(s);if(!gated)goto done;
    stage="earlier restore";
    t=diagnostic_ms();
    int restored=doa3_checkpoint_image_restore(s->session,start,&frame);
    fprintf(stderr,"[ROLLBACK-WINDOW] earlier restore=%d frame=%llu\n",restored,(unsigned long long)frame);
    if(restored<0)goto fatal;
    if(restored==1 && frame==first){
        int equal=doa3_checkpoint_image_matches(s->session,start,difference);
        fprintf(stderr,"[ROLLBACK-WINDOW] earlier compare=%d mask=%03X renderer=%d\n",
            equal,difference->different_parts,difference->renderer_different);
        result=equal==1;
    }
    restore_ms=diagnostic_ms()-t;
    if(result && advance){
        /* Workers and APU stepping require unpinned clock/audio state. The
         * output gate and kernel guard remain owned throughout execution. */
        s->roundtrip_pinned=0;mcpx_apu_rollback_release();xbox_det_checkpoint_release();
        stage="native interval";
        t=diagnostic_ms();
        result=advance(advance_context);
        replay_ms=diagnostic_ms()-t;t=diagnostic_ms();
        stage="replay verification pin";
        if(!acquire(s))goto fatal;
        s->roundtrip_pinned=1;release(s);
        if(result){
            int equal=doa3_checkpoint_image_matches(s->session,end,difference);
            fprintf(stderr,"[ROLLBACK-REPLAY] endpoint compare=%d mask=%03X renderer=%d\n",
                equal,difference->different_parts,difference->renderer_different);
            for(unsigned i=0;i<DOA3_CP_PARTS;++i)if(difference->different_parts&(1u<<i))
                fprintf(stderr,"[ROLLBACK-REPLAY] %s first byte=0x%llX\n",part_names[i],
                    (unsigned long long)difference->first_byte[i]);
            result=equal==1;
            if(pixel_probe && equal==0 && !difference->different_parts &&
               difference->renderer_different && difference->renderer_state_equal){
                result=2;
                fprintf(stderr,"[ROLLBACK-RENDER-AB] CPU and logical renderer state matched; GPU pixels differ (diagnostic only)\n");
            }
            {   /* rollback.12, logic-only resimulation test: the replay skipped
                 * the render tasks on hidden frames. Accept it as a logic match
                 * when only RAM and the GPU front end differ, and every
                 * differing RAM byte is render bookkeeping (labelled render
                 * memory, fence stamps, push-buffer pointers or dead stack). */
                extern int g_doa3_logic_only,g_doa3_logic_only_arm;
                extern size_t g_doa3_diff_unexplained_bytes;
                const unsigned render_parts=(1u<<DOA3_CP_RAM)|(1u<<DOA3_CP_GPU_FRONTEND);
                if(pixel_probe && g_doa3_logic_only && g_doa3_logic_only_arm && equal==0){
                    int logic=!(difference->different_parts&~render_parts) &&
                        (!(difference->different_parts&(1u<<DOA3_CP_RAM)) || !g_doa3_diff_unexplained_bytes);
                    fprintf(stderr,"[ROLLBACK-LOGIC-ONLY] %s: differing parts %03X, unexplained RAM bytes %zu, renderer logical state %s\n",
                        logic ? "LOGIC MATCHED (only render bookkeeping differs)":"LOGIC DIFFERS",
                        difference->different_parts,g_doa3_diff_unexplained_bytes,
                        difference->renderer_state_equal ? "same":"differs (render-only)");
                    if(logic)result=3;
                }
            }
            if(pixel_probe)fprintf(stderr,"[ROLLBACK-RENDER-AB] logical_renderer_equal=%d\n",difference->renderer_state_equal);
        }
        compare_ms=diagnostic_ms()-t;
    }
    /* Even a rejected earlier image must return through the independently
     * verified current endpoint. Never resume native caller locals at start. */
    stage="endpoint recovery";
    t=diagnostic_ms();
    Doa3CheckpointComparison recovery={0};
    if(doa3_checkpoint_image_restore(s->session,end,&frame)!=1 || frame!=last ||
       doa3_checkpoint_image_matches(s->session,end,&recovery)!=1)goto fatal;
    fprintf(stderr,"[ROLLBACK-WINDOW] current endpoint restored and matched frame=%llu\n",(unsigned long long)frame);
    recovery_ms=diagnostic_ms()-t;
    stage="audio gate end";
    if(!acquire(s))goto fatal;
    int ended=mcpx_apu_replay_end(!result);release(s);if(!ended)goto fatal;gated=0;
done:
    if(!result)fprintf(stderr,"[ROLLBACK-WINDOW] refused or recovered at stage=%s\n",stage);
    if(advance)fprintf(stderr,"[ROLLBACK-TIMING] frames=%llu restore_start_ms=%.3f replay_ms=%.3f compare_end_ms=%.3f recover_end_ms=%.3f\n",
        (unsigned long long)(last-first),restore_ms,replay_ms,compare_ms,recovery_ms);
    if(!xbox_kernel_replay_end(!result))goto fatal;
    s->roundtrip_pinned=0;mcpx_apu_rollback_release();xbox_det_checkpoint_release();
    return result;
fatal:
    fprintf(stderr,"[ROLLBACK-WINDOW] fatal stage=%s audio_gate=%d\n",stage,gated);fflush(stderr);
    return -1;
}
int doa3_fight_checkpoint_rewind_check(Doa3FightCheckpoint *s,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,Doa3CheckpointComparison *difference){
    return visit(s,start,first,end,last,difference,NULL,NULL,0);
}
int doa3_fight_checkpoint_replay_check(Doa3FightCheckpoint *s,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,
    int (*advance)(void *),void *advance_context,Doa3CheckpointComparison *difference){
    if(!advance)return 0;
    return visit(s,start,first,end,last,difference,advance,advance_context,0);
}
int doa3_fight_checkpoint_render_probe(Doa3FightCheckpoint *s,
    const Doa3CheckpointImage *start,uint64_t first,
    const Doa3CheckpointImage *end,uint64_t last,
    int (*advance)(void *),void *advance_context,Doa3CheckpointComparison *difference){
    if(!advance)return 0;
    return visit(s,start,first,end,last,difference,advance,advance_context,1);
}
