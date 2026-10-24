/* Private command backend: no device initialization, output stream, audio
 * worker, mixer or live game memory. Actual production VP and MMIO decoder. */
#include "apu_state.h"
#include "apu_rollback.h"
#include "shadow_apu.h"
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg;
int g_dbg_voice_monitor=-1;
extern MCPXAPUState *g_apu_state; /* private decoder instance in this executable */
static MCPXAPUState *device;
static ApuRollbackClock clock_state;
static unsigned char *mmio;
static int enabled;
static const char *error;
uint64_t mcpx_apu_vp_read(void *,hwaddr,unsigned);
void mcpx_apu_vp_write(void *,hwaddr,uint64_t,unsigned);
bool apu_hook_handle_mmio(PCONTEXT,uintptr_t,uint32_t,int);
size_t rb_apu_size(void) { return apu_rollback_size(); }
const char *rb_apu_error(void) { return error; }
int rb_apu_load(void *ram,const void *snapshot,size_t bytes)
{
    enabled=0; error=NULL;
    if(!bytes) return 1;
    if(!device) {
        device=calloc(1,sizeof(*device));
        mmio=VirtualAlloc(NULL,0x80000,MEM_RESERVE,PAGE_NOACCESS);
        if(!device || !mmio) {
            free(device); device=NULL;
            if(mmio) VirtualFree(mmio,0,MEM_RELEASE);
            mmio=NULL; error="apu_allocation"; return 0;
        }
        qemu_mutex_init(&device->lock); qemu_cond_init(&device->cond);
        g_apu_state=device;
    }
    if(!apu_rollback_import(device,&clock_state,snapshot,bytes)) { error="invalid_apu_snapshot"; return 0; }
    g_apu_ram_ptr=ram; enabled=1; return 1;
}
int rb_apu_save(void *snapshot,size_t bytes)
{
    if(!bytes) return 1;
    return enabled && !error && apu_rollback_export(device,&clock_state,snapshot,bytes);
}
uintptr_t rb_apu_pointer(uint32_t address)
{
    return enabled && address>=0xFE800000u && address<0xFE880000u ?
        (uintptr_t)(mmio+(address-0xFE800000u)) : 0;
}
uint64_t mcpx_apu_mmio_read(MCPXAPUState *d,uint64_t addr,unsigned size)
{
    if(!enabled || error) return 0;
    if(size!=1 && size!=2 && size!=4) { error="unsupported_apu_width"; return 0; }
    if(addr>=0x20000 && addr<0x30000) return mcpx_apu_vp_read(d,addr-0x20000,size);
    if(addr==NV_PAPU_XGSCNT) return clock_state.base+(clock_state.chip*20000u)/3u;
    if(addr<0x20000) return d->regs[addr];
    error="unsupported_apu_read"; return 0;
}
void mcpx_apu_mmio_write(MCPXAPUState *d,uint64_t addr,uint64_t value,unsigned size)
{
    if(!enabled || error) return;
    if(size!=1 && size!=2 && size!=4) { error="unsupported_apu_width"; return; }
    if(addr>=0x20000 && addr<0x30000) {
        mcpx_apu_vp_write(d,addr-0x20000,value,size); return;
    }
    /* Main-register writes can wake workers/deliver IRQs. They are a separate
     * replay integration step, never silently accepted as inert stores. */
    error="apu_control_write_requires_service";
}
int rb_apu_exception(EXCEPTION_POINTERS *e)
{
    EXCEPTION_RECORD *r=e->ExceptionRecord;
    if(!enabled || r->ExceptionCode!=EXCEPTION_ACCESS_VIOLATION || r->NumberParameters<2 ||
       r->ExceptionInformation[0]>1) return EXCEPTION_EXECUTE_HANDLER;
    uintptr_t address=r->ExceptionInformation[1], offset=address-(uintptr_t)mmio;
    if(offset>=0x80000) return EXCEPTION_EXECUTE_HANDLER;
    if(!apu_hook_handle_mmio(e->ContextRecord,address,0xFE800000u+(uint32_t)offset,
                           (int)r->ExceptionInformation[0])) error="unsupported_apu_instruction";
    return error ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_EXECUTION;
}
