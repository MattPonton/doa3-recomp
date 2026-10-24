/* Small deterministic oracle fixture for the production chain/checkpoint
 * checker. Actual translated-task validation remains a separate live test. */
#include "shadow_types.h"
const unsigned char rb_source_digest[32]={0};
const uint32_t rb_kernel_count=9, rb_kernel_ordinals[]={277,294,291,128,160,161,129,175,173};
void rb_dispatch(uint32_t va) {
    if(va==0x6DF00) { g_eax=0xABC; g_esp+=4; return; }
    if(va==0x1DC8E0) {
        /* ABI oracle: changes the device fallback surface during the guest
         * body, clobbers registers, and consumes both stdcall arguments. */
        MEM32(0x10000+0x2070)=MEM32(g_esp+8);
        g_eax=0x1234; g_ecx=0; g_esp+=12; return;
    }
    rb_abort("unsupported_call",va);
}
void rb_step(uint32_t va,Doa3RollbackTask *task)
{
    if(task->locals[31]==0x4B45524Eu) {
        if(task->locals[30]==0xFE820010 || task->locals[30]==0xFD800044)
            (void)rb_ptr(task->locals[30],4);
        rb_call(task->locals[30]);
        task->resume=task->yielded=1;
        return;
    }
    task->locals[0]+=MEM32(0x60000)+va;
#ifdef DOA3_SHADOW_GPU
    /* Private FIFO read + write, with automatic production GET advancement. */
    MEM32(0xFD800040)=MEM32(0xFD800044)+task->locals[0];
    /* Force a standalone MMIO load, like the captured guest path. A folded
     * IMUL memory operand is outside the production decoder's instruction set. */
    volatile uint32_t cursor=MEM32(0xFD00324C);
    MEM32(0xFD00324C)=cursor*17u+task->locals[0];
#endif
    if(task->nested.schema==DOA3_NESTED_SCHEMA)
        task->nested.frames[1].locals[40]=task->locals[0];
    MEM32(0x50000)=MEM32(0x50000)*33u+task->locals[0];
    g_eax=task->locals[0]; g_fp_stack[0]=(double)g_eax;
    task->resume=task->yielded=1;
}
