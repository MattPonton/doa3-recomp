/* Isolated USER/PFIFO registers only. No D3D device, render thread, parser, window,
 * live memory, or GPU resource is created. Use the production MMIO decoder
 * and shared production queue operations against a private captured state. */
#include "nv2a_state.h"
#include "nv2a_queue.h"
#include "nv2a_pfifo_registers.h"
#include "nv2a_mmio_hook.h"
#include "shadow_gpu.h"
static Nv2aQueueState queue;
static NV2AState identity;
static unsigned char *mmio;
static int enabled;
static const char *error;
static uint32_t error_address;
NV2AState *nv2a_get_state(void) { return enabled ? &identity : NULL; }
size_t rb_gpu_size(void) { return sizeof(Nv2aQueueSnapshot); }
const char *rb_gpu_error(void) { return error; }
uint32_t rb_gpu_error_address(void) { return error_address; }
static int submission_method(void *opaque,uint32_t sub,uint32_t method,uint32_t value)
{
    (void)opaque; (void)sub; (void)value;
    /* The shared walker accounts for every ordered command in the captured
     * digest. This profile does not execute/restore the PGRAPH consumer. */
    if(method==0x130) { error="graphics_flip_requires_vblank_state"; error_address=method; return 0; }
    return 1;
}
int rb_gpu_submit(uint32_t dev,uint32_t (*read)(void *,uint32_t),void (*write)(void *,uint32_t,uint32_t),void *opaque)
{
    if(!enabled || error) return 0;
    if(!queue.submission.tracing) { error="submission_trace_not_captured"; return 0; }
    if(queue.submission.flip) { error="graphics_flip_requires_vblank_state"; return 0; }
    Nv2aSubmissionOps ops={read,write,submission_method,NULL,opaque,1};
    int result=nv2a_submission_kick(&queue.submission,&ops,dev);
    if(result!=1 && !error) error=result==-2 ? "graphics_uninitialized_channel" : "unsupported_submission_stream";
    if(queue.submission.flip && !error) error="graphics_flip_requires_vblank_state";
    return result==1 && !error;
}
int rb_gpu_targets(uint32_t dev,uint32_t arg,uint32_t (*read)(void *,uint32_t),void *opaque)
{
    if(!enabled || error) return 0;
    nv2a_targets_update(&queue.targets,dev,arg,read,opaque);
    return 1;
}
int rb_gpu_load(const void *snapshot,size_t bytes)
{
    enabled=0; error=NULL; error_address=0;
    if(!bytes) return 1;
    if(!nv2a_queue_import(&queue,snapshot,bytes)) { error="invalid_gpu_snapshot"; return 0; }
    if(!mmio) mmio=VirtualAlloc(NULL,0x1000000,MEM_RESERVE,PAGE_NOACCESS);
    if(!mmio) { error="gpu_allocation"; return 0; }
    enabled=1; return 1;
}
int rb_gpu_save(void *snapshot,size_t bytes)
{
    if(!bytes) return 1;
    return enabled && !error && nv2a_queue_export(&queue,snapshot,bytes);
}
uintptr_t rb_gpu_pointer(uint32_t address)
{
    return enabled && address>=0xFD000000u && address<0xFE000000u ?
        (uintptr_t)(mmio+(address-0xFD000000u)) : 0;
}
static int supported(hwaddr address,unsigned size)
{
    if(!enabled || error) return 0;
    error_address=0xFD000000u+(uint32_t)address;
    if(size!=4) { error="unsupported_gpu_width"; return 0; }
    if(address>=0x2000 && address<0x4000 && !(address&3)) return 1;
    if(address<0x800000 || address>=0x1000000 ||
       ((address&0xFFFF)!=0x40 && (address&0xFFFF)!=0x44)) {
        error="graphics_state_requires_snapshot"; return 0;
    }
    return 1;
}
uint64_t nv2a_mmio_read(NV2AState *d,hwaddr address,unsigned size)
{
    (void)d;
    if(!supported(address,size)) return 0;
    if(address<0x4000) return nv2a_pfifo_register_read(queue.pfifo_regs,
        queue.pfifo_pending,queue.pfifo_enabled,(uint32_t)address-0x2000);
    return nv2a_queue_read(&queue,(uint32_t)address);
}
void nv2a_mmio_write(NV2AState *d,hwaddr address,uint64_t value,unsigned size)
{
    (void)d;
    if(!supported(address,size)) return;
    if(address<0x4000) {
        uint32_t offset=(uint32_t)address-0x2000;
        if(offset==NV_PFIFO_INTR_0 || offset==NV_PFIFO_INTR_EN_0) {
            error="pfifo_interrupt_write_requires_service"; return;
        }
        nv2a_pfifo_register_write(queue.pfifo_regs,&queue.pfifo_pending,
            &queue.pfifo_enabled,offset,(uint32_t)value);
    } else nv2a_queue_write(&queue,(uint32_t)address,(uint32_t)value);
}
int rb_gpu_exception(EXCEPTION_POINTERS *e)
{
    EXCEPTION_RECORD *r=e->ExceptionRecord;
    if(!enabled || r->ExceptionCode!=EXCEPTION_ACCESS_VIOLATION || r->NumberParameters<2 ||
       r->ExceptionInformation[0]>1) return EXCEPTION_CONTINUE_SEARCH;
    uintptr_t address=r->ExceptionInformation[1], offset=address-(uintptr_t)mmio;
    if(offset>=0x1000000) return EXCEPTION_CONTINUE_SEARCH;
    error_address=0xFD000000u+(uint32_t)offset;
    if(!nv2a_hook_handle_mmio(e->ContextRecord,address,0xFD000000u+(uint32_t)offset,
                            (int)r->ExceptionInformation[0])) error="unsupported_gpu_instruction";
    return error ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_EXECUTION;
}
