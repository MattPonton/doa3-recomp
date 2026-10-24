/* Offline only: no live game, input, networking, audio output or renderer. */
#include "shadow_types.h"
#include "shadow_apu.h"
#include "shadow_gpu.h"
#include "../../src/game/online_menu.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
uint32_t g_str_a,g_str_b,g_flg_a,g_flg_b;
int g_flg_w,g_flg_test,g_fp_top;
uint16_t g_x87_cw;
double g_fp_stack[8];
int rb_native_camera;
static int rb_submission_stream;
static unsigned char *memory;
static size_t memory_bytes, image_bytes, apu_bytes, gpu_bytes, capture_state_bytes;
static jmp_buf escape;
static const char *failure;
static uint32_t failure_value, budget;
static uint32_t call_trace[16], trace_count;
static uint32_t guest_site;
static uint32_t capture_version;
extern const unsigned char rb_source_digest[32];
extern const uint32_t rb_kernel_count, rb_kernel_ordinals[];

__declspec(noreturn) void rb_abort(const char *reason, uint32_t value)
{ failure=reason; failure_value=value; longjmp(escape,1); }
int doa3_save_accept_compatible(const void *save,size_t size)
{ (void)save; rb_abort("save_compat_requires_host_state",(uint32_t)size); }
void rb_budget(void) { if (!budget--) rb_abort("execution_budget",0); }
void rb_site(uint32_t va) { guest_site=va; rb_budget(); }
static uint32_t normalize(uint32_t address)
{
    uint32_t base=(uint32_t)g_xbox_mem_offset, relative=address-base;
    if (base && relative<0x74000000u)
        return relative<0x08000000u ? relative : relative%0x04000000u;
    return address;
}
uintptr_t rb_ptr(uint32_t address, size_t bytes)
{
    uint32_t va=normalize(address);
    if (va>=0xFD000000u && va<0xFE000000u) {
        uintptr_t mapped=rb_gpu_pointer(va);
        if(mapped) return mapped;
        rb_abort("graphics_state_requires_snapshot",address);
    }
    if (va>=0xFE800000u && va<0xFE880000u) {
        uintptr_t mapped=rb_apu_pointer(va);
        if(mapped) return mapped;
        rb_abort("apu_mmio_requires_snapshot",address);
    }
    if (bytes>memory_bytes || va>memory_bytes-bytes) rb_abort("outside_guest_memory",address);
    return (uintptr_t)(memory+va);
}
static void check_pointer(const void *ptr, size_t bytes)
{
    uintptr_t delta=(uintptr_t)ptr-(uintptr_t)memory;
    if (bytes>memory_bytes || delta>memory_bytes-bytes) rb_abort("outside_guest_bulk",(uint32_t)delta);
}
void *rb_memcpy(void *dst,const void *src,size_t bytes)
{ check_pointer(dst,bytes); check_pointer(src,bytes); return memcpy(dst,src,bytes); }
void *rb_memmove(void *dst,const void *src,size_t bytes)
{ check_pointer(dst,bytes); check_pointer(src,bytes); return memmove(dst,src,bytes); }
void *rb_memset(void *dst,int value,size_t bytes)
{ check_pointer(dst,bytes); return memset(dst,value,bytes); }
static uint32_t submission_read(void *opaque,uint32_t address)
{ (void)opaque; return *(volatile uint32_t *)rb_ptr(address,4); }
static void submission_write(void *opaque,uint32_t address,uint32_t value)
{ (void)opaque; *(volatile uint32_t *)rb_ptr(address,4)=value; }
void rb_call(uint32_t va)
{
    rb_budget(); va=normalize(va);
    call_trace[trace_count++%16]=va;
    if(va==0x6DF00) {
        uint32_t id=submission_read(NULL,g_eax);
        if(id==ONLINE_MENU_ICON || id==ONLINE_MENU_LABEL)
            rb_abort("online_sprite_requires_host_state",id);
        rb_dispatch(va); return;
    }
    if(va==0x1DC8E0) {
        if(!rb_submission_stream) rb_abort("render_target_requires_replay_profile",va);
        if(!gpu_bytes) rb_abort("render_target_requires_snapshot",va);
        uint32_t arg=submission_read(NULL,g_esp+4),dev=submission_read(NULL,0x1EDE80);
        rb_dispatch(va); /* Actual translated SetRenderTarget; preserves its ABI. */
        if(!rb_gpu_targets(dev,arg,submission_read,NULL))
            rb_abort(rb_gpu_error() ? rb_gpu_error() : "render_target_requires_snapshot",va);
        return;
    }
    if(va==0x1E27C0) {
        if(!rb_submission_stream) rb_abort("graphics_submission_requires_replay_profile",va);
        (void)rb_ptr(g_esp,4);
        if(!rb_gpu_submit(g_ecx,submission_read,submission_write,NULL))
            rb_abort(rb_gpu_error() ? rb_gpu_error() : "graphics_submission_requires_snapshot",va);
        g_esp+=4; return;
    }
    /* Exact inactive path of the manual voice-retirement wait. An active
     * retirement requires APU IRQ/service state, which is not captured yet:
     * fail there rather than fabricate completion or touch live audio. */
    if (va==0x1F5157) {
        uint16_t flags;
        memcpy(&flags,(const void *)rb_ptr(g_ecx+0x12,2),2);
        if ((flags&0x8001u)==0x8001u) rb_abort("audio_retirement_requires_apu",g_ecx);
        (void)rb_ptr(g_esp,4); g_esp+=4; return;
    }
    if ((va & 0xFF000000u)==0xFE000000u) {
        uint32_t offset=va-0xFE000000u, ordinal;
        if ((offset&3u) || offset/4>=rb_kernel_count)
            rb_abort("invalid_kernel_thunk",va);
        ordinal=rb_kernel_ordinals[offset/4];
        /* Audited current kernel_rtl.c + kernel_bridge.c behavior: these
         * three bridges read one argument, do not dereference/mutate the
         * critical section, return EAX=0 and pop return+stdcall argument.
         * No live kernel dispatch, worker scheduling or audio is invoked.
         * Captured guest effects still have to match byte-for-byte. */
        if (ordinal==277 || ordinal==294 || ordinal==291) {
            (void)rb_ptr(g_esp,8);
            g_eax=0; g_esp+=8; return;
        }
        /* The current 3.1 kernel table explicitly implements ordinal 175
         * with a NULL bridge and 12 argument bytes: EAX=0, no page locking
         * or pointed-to memory access. Mirror that existing guest ABI only.
         * Ordinal 173's bridge returns the argument unchanged (Xbox identity
         * mapping), not a native pointer. Neither calls the live kernel. */
        if (ordinal==175) {
            (void)rb_ptr(g_esp,16);
            g_eax=0; g_esp+=16; return;
        }
        if (ordinal==173) {
            const void *arg=(const void *)rb_ptr(g_esp,8);
            memcpy(&g_eax,(const unsigned char *)arg+4,4);
            g_esp+=8; return;
        }
        /* kernel_hal.c mirrors EVERY IRQL setter to guest KPCR byte 0x24.
         * The isolated mirror is the state here; it is restored with RAM.
         * These fastcall/no-argument bridges pop only the return address. */
        if (ordinal==160 || ordinal==161 || ordinal==129) {
            unsigned char *irql=(unsigned char *)rb_ptr(0x24,1);
            (void)rb_ptr(g_esp,4);
            g_eax=ordinal==161 ? 0u : *irql;
            *irql=ordinal==129 ? 2u : (unsigned char)g_ecx;
            g_esp+=4; return;
        }
        rb_abort("unsupported_kernel_ordinal",ordinal);
    }
    /* Exact guest ABI of recomp_manual.c's CRT tail-copy fix. The checked
     * pointers can access only the replay buffer, never live guest memory. */
    if (va==0x1B73D0 || va==0x1B7CB0) {
        uint32_t dst=MEM32(g_esp+4), src=MEM32(g_esp+8), n=MEM32(g_esp+12);
        if (n) memmove((void *)rb_ptr(dst,n),(void *)rb_ptr(src,n),n);
        g_eax=dst; g_esp+=4; return;
    }
    rb_dispatch(va);
}
uint64_t doa3_guest_rdtsc(void) { rb_abort("guest_clock",0); }
void doa3_rollback_task_sleep(void)
{
    uint32_t slot=MEM32(0x73C644u);
    if (slot>=18) rb_abort("task_slot",slot);
    g_ecx=0x73C400u+slot*32; g_edx=MEM32(g_esp+4);
    MEM32(g_ecx)&=~0x10u; MEM32(g_ecx+4)=g_edx;
    g_eax=MEM32(0x73C640); g_esp+=4;
}
static void load_state(const Doa3RollbackCaptureState *s)
{
    g_eax=s->regs[0]; g_ecx=s->regs[1]; g_edx=s->regs[2]; g_esp=s->regs[3];
    g_ebx=s->regs[4]; g_esi=s->regs[5]; g_edi=s->regs[6]; g_seh_ebp=s->regs[7];
    g_str_a=s->regs[8]; g_str_b=s->regs[9]; g_flg_a=s->regs[10]; g_flg_b=s->regs[11];
    g_flg_w=(int)s->regs[12]; g_flg_test=(int)s->regs[13]; g_fp_top=(int)s->regs[14];
    g_x87_cw=(uint16_t)s->regs[15]; memcpy(g_fp_stack,s->fp,sizeof(g_fp_stack));
}
static void save_state(Doa3RollbackCaptureState *s)
{
    const uint32_t r[]={g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp,
        g_str_a,g_str_b,g_flg_a,g_flg_b,(uint32_t)g_flg_w,(uint32_t)g_flg_test,
        (uint32_t)g_fp_top,g_x87_cw};
    memcpy(s->regs,r,sizeof(r)); memcpy(s->fp,g_fp_stack,sizeof(g_fp_stack));
}
static int replay_exception(EXCEPTION_POINTERS *e)
{
    int gpu=rb_gpu_exception(e);
    return gpu!=EXCEPTION_CONTINUE_SEARCH ? gpu : rb_apu_exception(e);
}
static int execute(uint32_t routine,Doa3RollbackCaptureState *state)
{
    failure=NULL; failure_value=0; budget=2000000; trace_count=0; guest_site=0;
    load_state(state);
    if(!rb_apu_load(memory,memory+memory_bytes,apu_bytes)) { failure="invalid_apu_snapshot"; return 0; }
    if(!rb_gpu_load(memory+memory_bytes+apu_bytes,gpu_bytes)) { failure="invalid_gpu_snapshot"; return 0; }
    if (setjmp(escape)) return 0;
    __try { rb_step(routine,&state->task); }
    __except(replay_exception(GetExceptionInformation())) {
        failure=rb_gpu_error() ? rb_gpu_error() : rb_apu_error() ? rb_apu_error() : "native_exception";
        failure_value=rb_gpu_error() ? rb_gpu_error_address() : GetExceptionCode(); return 0;
    }
    if (state->task.resume!=1 || state->task.yielded!=1) { failure="missing_yield"; return 0; }
    if(!rb_apu_save(memory+memory_bytes,apu_bytes)) { failure="apu_export_failed"; return 0; }
    if(!rb_gpu_save(memory+memory_bytes+apu_bytes,gpu_bytes)) { failure="gpu_export_failed"; return 0; }
    save_state(state); return 1;
}
static int read_exact(FILE *f,void *out,size_t bytes) { return fread(out,1,bytes,f)==bytes; }
static int read_record(FILE *f,uint32_t h[4],Doa3RollbackCaptureState *state,unsigned char *image)
{
    if (!read_exact(f,h,16)) return -1;
    if (!h[0]) return (h[1]||h[2]||h[3]||fgetc(f)!=EOF) ? -1 : 0;
    memset(state,0,sizeof(*state));
    if (h[0]>2 || h[2]>memory_bytes/4096 || (capture_version==1 && h[3]) || !read_exact(f,state,capture_state_bytes)) return -1;
    if(apu_bytes && !read_exact(f,image+memory_bytes,apu_bytes)) return -1;
    if(gpu_bytes && !read_exact(f,image+memory_bytes+apu_bytes,gpu_bytes)) return -1;
    uint32_t last=0;
    for (uint32_t i=0;i<h[2];++i) {
        uint32_t page;
        if (!read_exact(f,&page,4) || page>=memory_bytes/4096 || (i && page<=last) ||
            !read_exact(f,image+(size_t)page*4096,4096)) return -1;
        last=page;
    }
    return 1;
}
/* Carry task-owned registers/continuations and RAM between samples. Changes
 * made by OTHER tasks/workers are a recorded external-write journal, never
 * mislabeled as simulated work. Rewind restores the complete isolated model,
 * then consumes that same journal again. This is not whole-game rollback. */
static int owned_state_equal(const Doa3RollbackCaptureState *a,const Doa3RollbackCaptureState *b)
{
    return !memcmp(a->regs,b->regs,8*sizeof(uint32_t)) && a->regs[14]==b->regs[14] &&
           !memcmp(a->fp,b->fp,sizeof(a->fp)) && !memcmp(&a->task,&b->task,sizeof(a->task));
}
static int check_chain(FILE *f,unsigned limit)
{
    const uint32_t routines[DOA3_RB_TASK_COUNT]={DOA3_RB_TASK_ROUTINES};
    Doa3RollbackCaptureState tasks[DOA3_RB_TASK_COUNT]={0}, saved_tasks[DOA3_RB_TASK_COUNT], final_tasks[DOA3_RB_TASK_COUNT];
    Doa3RollbackCaptureState pre,post,actual;
    unsigned char *recorded=calloc(1,image_bytes), *previous_live=calloc(1,image_bytes);
    unsigned char *checkpoint=malloc(image_bytes), *final_image=malloc(image_bytes);
    unsigned pairs=0, checkpoint_pairs=0, replayed=0, frame=0, checkpoint_frame=0;
    uint64_t external_bytes=0;
    int rewinding=0, pending=0, status, result=1;
    __int64 checkpoint_position=0;
    uint32_t h[4];
    memory=malloc(image_bytes);
    if (!recorded || !previous_live || !checkpoint || !final_image || !memory) {
        fprintf(stderr,"Out of memory\n"); result=2; goto done;
    }
    if (capture_version<2 || limit<DOA3_RB_TASK_COUNT*2 || limit%DOA3_RB_TASK_COUNT) {
        fprintf(stderr,"Chain check requires v2 complete consecutive task cycles\n"); result=2; goto done;
    }
    puts("CHAIN external-journal model: unconverted work is recorded input, not replayed simulation.");
read_pass:
    while ((status=read_record(f,h,&post,recorded))>0) {
        unsigned index=pairs%DOA3_RB_TASK_COUNT;
        if (pairs>=limit || h[1]!=routines[index]) { status=-1; break; }
        if (h[0]==1) {
            if (pending || post.task.resume!=1 || post.task.yielded!=1 ||
                (pairs && h[3]!=frame+(index==0 ? 1u : 0u))) { status=-1; break; }
            frame=h[3]; pre=post; pending=1;
            if (!pairs) memcpy(memory,recorded,image_bytes);
            else {
                /* Apply only actual between-sample writes. Never replace the
                 * whole replay image with the next recorded pre-state. */
                for (size_t i=0;i<image_bytes;++i) if(previous_live[i]!=recorded[i]) {
                    memory[i]=recorded[i]; if(!rewinding) ++external_bytes;
                }
                if (memcmp(memory,recorded,image_bytes)) {
                    fprintf(stderr,"CHAIN pre-state mismatch at pair %u\n",pairs+1); goto done;
                }
            }
            if (pairs>=DOA3_RB_TASK_COUNT && !owned_state_equal(&tasks[index],&pre)) {
                fprintf(stderr,"CHAIN task continuation gap at pair %u task %08X\n",pairs+1,h[1]); goto done;
            }
            actual=pre;
            if(pairs>=DOA3_RB_TASK_COUNT) {
                /* String/flag operands and x87 control are shared globals;
                 * dispatcher/other tasks supply their recorded boundary state. */
                memcpy(actual.regs,tasks[index].regs,8*sizeof(uint32_t));
                actual.regs[14]=tasks[index].regs[14];
                memcpy(actual.fp,tasks[index].fp,sizeof(actual.fp));
                actual.task=tasks[index].task;
            }
            continue;
        }
        if(!pending || h[3]!=frame) { status=-1; break; }
        pending=0;
        if(!execute(h[1],&actual)) {
            fprintf(stderr,"CHAIN BLOCKED pair=%u task=%08X %s value=%08X site=%08X\n",pairs+1,h[1],failure,failure_value,guest_site); goto done;
        }
        if(memcmp(memory,recorded,image_bytes) || memcmp(&actual,&post,sizeof(actual))) {
            fprintf(stderr,"CHAIN DIFFERENT pair=%u task=%08X\n",pairs+1,h[1]); goto done;
        }
        tasks[index]=actual; memcpy(previous_live,recorded,image_bytes); ++pairs;
        if(rewinding) ++replayed;
        if(!rewinding && pairs==(limit/DOA3_RB_TASK_COUNT/2)*DOA3_RB_TASK_COUNT) {
            memcpy(checkpoint,memory,image_bytes); memcpy(saved_tasks,tasks,sizeof(tasks));
            checkpoint_pairs=pairs; checkpoint_frame=frame; checkpoint_position=_ftelli64(f);
            if(checkpoint_position<0) { result=2; goto done; }
        }
    }
    if(status<0 || pending || pairs!=limit) { fprintf(stderr,"Invalid or nonconsecutive chain capture\n"); result=2; goto done; }
    if(!rewinding) {
        if(!checkpoint_pairs) { result=2; goto done; }
        memcpy(final_image,memory,image_bytes); memcpy(final_tasks,tasks,sizeof(tasks));
        memcpy(memory,checkpoint,image_bytes); memcpy(recorded,checkpoint,image_bytes);
        memcpy(previous_live,checkpoint,image_bytes); memcpy(tasks,saved_tasks,sizeof(tasks));
        pairs=checkpoint_pairs; frame=checkpoint_frame; rewinding=1;
        if(_fseeki64(f,checkpoint_position,SEEK_SET)) { result=2; goto done; }
        goto read_pass;
    }
    if(memcmp(memory,final_image,image_bytes) || memcmp(tasks,final_tasks,sizeof(tasks))) {
        fprintf(stderr,"CHAIN restored replay differs from forward result\n"); goto done;
    }
    printf("CHAIN MATCH cycles=%u tasks=%u checkpoint_after=%u restored_steps=%u external_bytes=%llu; isolated task group, not whole-fight rollback\n",
           limit/DOA3_RB_TASK_COUNT,limit,checkpoint_pairs,replayed,(unsigned long long)external_bytes);
    result=0;
done:
    free(recorded); free(previous_live); free(checkpoint); free(final_image); free(memory); memory=NULL;
    return result;
}
int main(int argc,char **argv)
{
    int chain=0;
    if(argc<2 || argc>5) { fprintf(stderr,"Usage: rollback_shadow capture.rbc [--native-camera] [--chain] [--submission-stream]\n"); return 2; }
    for(int i=2;i<argc;++i) {
        if(!strcmp(argv[i],"--native-camera") && !rb_native_camera) rb_native_camera=1;
        else if(!strcmp(argv[i],"--submission-stream") && !rb_submission_stream) rb_submission_stream=1;
        else if(!strcmp(argv[i],"--chain") && !chain) chain=1;
        else { fprintf(stderr,"Unknown or duplicate option\n"); return 2; }
    }
    if (rb_native_camera) puts("PROFILE native-camera: free-camera/FOV overrides are not reproduced; all guest bytes and task state remain compared.");
    if(rb_submission_stream) puts("PROFILE submission-stream: compare guest synchronization, target routing and ordered method digests; PGRAPH/resources/pixels are NOT reproduced; flip/vblank blocked.");
    FILE *f=fopen(argv[1],"rb"); uint32_t header[8];
    if (!f || !read_exact(f,header,sizeof(header)) || header[0]!=DOA3_RBC_MAGIC ||
        (header[1]<1 || header[1]>DOA3_RBC_VERSION) || header[2]!=128u*1024u*1024u ||
        !header[7] || header[7]>DOA3_RBC_PAIRS) {
        fprintf(stderr,"Invalid capture header\n"); if(f)fclose(f); return 2;
    }
    capture_state_bytes=header[1]>=4 ? sizeof(Doa3RollbackCaptureState) : 264u;
    if(header[1]<5 && header[6]!=capture_state_bytes && (header[1]<3 || !rb_apu_size() ||
       header[6]!=capture_state_bytes+rb_apu_size())) {
        fprintf(stderr,"Invalid capture header\n"); fclose(f); return 2;
    }
    unsigned char digest[32];
    if (!read_exact(f,digest,32) || memcmp(digest,rb_source_digest,32)) {
        fprintf(stderr,"Capture and replay sources differ; rebuild the checker for this capture\n"); fclose(f); return 2;
    }
    if(header[1]>=5) {
        uint32_t payloads[2];
        if(!read_exact(f,payloads,sizeof(payloads)) ||
           (payloads[0] && (!rb_apu_size() || payloads[0]!=rb_apu_size())) ||
           (payloads[1] && (!rb_gpu_size() || payloads[1]!=rb_gpu_size())) ||
           header[6]!=capture_state_bytes+(size_t)payloads[0]+payloads[1]) {
            fprintf(stderr,"Invalid capture payload sizes\n"); fclose(f); return 2;
        }
        apu_bytes=payloads[0]; gpu_bytes=payloads[1];
    } else apu_bytes=header[6]-capture_state_bytes;
    memory_bytes=header[2];
    image_bytes=memory_bytes+apu_bytes+gpu_bytes; g_xbox_mem_offset=header[3]; capture_version=header[1];
    if(chain) { int result=check_chain(f,header[7]); fclose(f); return result; }
    unsigned char *recorded=calloc(1,image_bytes), *before=malloc(image_bytes), *first=malloc(image_bytes);
    memory=malloc(image_bytes);
    if (!recorded || !before || !first || !memory) { fprintf(stderr,"Out of memory\n"); return 2; }
    unsigned pairs=0,matched=0,blocked=0,diverged=0; int status;
    uint32_t h[4],routine=0,frame=0; Doa3RollbackCaptureState pre,post;
    while ((status=read_record(f,h,&post,recorded))>0) {
        if (h[0]==1) {
            if (routine || post.task.resume!=1 || !post.task.yielded) { status=-1; break; }
            routine=h[1]; frame=h[3]; pre=post; memcpy(before,recorded,image_bytes); continue;
        }
        if (!routine || routine!=h[1] || h[3]!=frame || pairs>=header[7]) { status=-1; break; }
        ++pairs; memcpy(memory,before,image_bytes); Doa3RollbackCaptureState actual=pre;
        if (!execute(routine,&actual)) {
            ++blocked; printf("pair=%u task=%08X BLOCKED %s value=%08X site=%08X\n",pairs,routine,failure,failure_value,guest_site);
            printf("  calls:"); for(unsigned n=trace_count>16?trace_count-16:0;n<trace_count;++n)
                printf(" %08X",call_trace[n%16]); puts("");
        } else {
            memcpy(first,memory,image_bytes);
            memcpy(memory,before,image_bytes); Doa3RollbackCaptureState repeated=pre;
            int again=execute(routine,&repeated);
            int stable=again && !memcmp(first,memory,image_bytes) && !memcmp(&actual,&repeated,sizeof(actual));
            unsigned differences=0; uint32_t first_difference=0;
            for (size_t i=0;i<image_bytes;++i) if (first[i]!=recorded[i]) {
                if (differences<24) printf("  byte %08X replay=%02X live=%02X\n",(uint32_t)i,first[i],recorded[i]);
                if (!differences) first_difference=(uint32_t)i; ++differences;
            }
            int states=!memcmp(&actual,&post,sizeof(actual));
            if (stable && states && !differences) ++matched; else ++diverged;
            printf("pair=%u task=%08X %s repeat=%s state=%s memory_bytes=%u first=%08X\n",pairs,routine,
                stable&&states&&!differences?"MATCH":"DIFFERENT",stable?"same":"different",states?"same":"different",
                differences,first_difference);
            if (!states) {
                for (unsigned i=0;i<16;++i) if(actual.regs[i]!=post.regs[i])
                    printf("  register[%u] replay=%08X live=%08X\n",i,actual.regs[i],post.regs[i]);
                if(memcmp(&actual.task,&post.task,sizeof(actual.task))) puts("  continuation differs");
            }
        }
        routine=0;
    }
    fclose(f); free(recorded); free(before); free(first); free(memory);
    if (status<0 || routine || pairs!=header[7]) { fprintf(stderr,"Truncated or invalid capture stream\n"); return 2; }
    printf("SUMMARY pairs=%u matched=%u blocked=%u diverged=%u; task checks only, not whole-fight rollback\n",pairs,matched,blocked,diverged);
    return blocked||diverged ? 1 : 0;
}
