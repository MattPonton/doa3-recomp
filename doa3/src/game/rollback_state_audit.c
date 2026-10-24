#include "rollback_state_audit.h"
#include "rollback_input.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_heap.h"
#include "kernel/kernel_stream.h"
#include "kernel/kernel_apc.h"
#include "kernel/kernel_interrupt.h"
#include "nv2a/nv2a_queue.h"
#include <stdlib.h>
#include <string.h>

enum { RAM,HEAP,FILES,APC,IRQ,GPU,XPP,PARTS };
struct Doa3StateAudit {
    DWORD owner;
    void *ram;
    XboxHeapCheckpoint *heap;
    size_t bytes[PARTS];
    void *image[2][PARTS];
    int files;
    unsigned stable;
};
static int allocate(Doa3StateAudit *s,unsigned part)
{
    if (!s->bytes[part]) return 0;
    for (unsigned pass=0;pass<2;pass++) {
        s->image[pass][part]=malloc(s->bytes[part]);
        if (!s->image[pass][part]) return 0;
        memset(s->image[pass][part],0,s->bytes[part]);
    }
    return 1;
}
void doa3_state_audit_destroy(Doa3StateAudit *s)
{
    if (!s || s->owner!=GetCurrentThreadId()) return;
    if (s->files) xbox_stream_file_checkpoint_end();
    for (unsigned p=0;p<2;p++) for (unsigned i=0;i<PARTS;i++) free(s->image[p][i]);
    xbox_heap_checkpoint_destroy(s->heap);free(s);
}
Doa3StateAudit *doa3_state_audit_create(void)
{
    Doa3StateAudit *s=calloc(1,sizeof(*s));if (!s) return NULL;
    s->owner=GetCurrentThreadId();s->ram=xbox_GetMemoryBase();
    s->bytes[RAM]=xbox_GetMemorySize();
    if (!s->ram || s->bytes[RAM]!=128u*1024u*1024u) goto fail;
    s->heap=xbox_heap_checkpoint_create();if (!s->heap) goto fail;
    s->bytes[HEAP]=xbox_heap_checkpoint_size();
    s->bytes[APC]=xbox_stream_completion_snapshot_size();
    s->bytes[IRQ]=sizeof(XboxInterruptSnapshot);s->bytes[GPU]=sizeof(Nv2aQueueSnapshot);
    s->bytes[XPP]=sizeof(Doa3XppSnapshot);
    for (unsigned i=0;i<PARTS;i++) if (i!=FILES && !allocate(s,i)) goto fail;
    return s;
fail:
    doa3_state_audit_destroy(s);return NULL;
}
int doa3_state_audit_begin_files(Doa3StateAudit *s)
{
    if (!s || s->owner!=GetCurrentThreadId() || s->files) return 0;
    if (!xbox_stream_file_checkpoint_begin()) return 0;
    s->files=1;s->bytes[FILES]=xbox_stream_file_checkpoint_size();
    return allocate(s,FILES);
}
static int capture(Doa3StateAudit *s,unsigned p,unsigned i)
{
    void *out=s->image[p][i];size_t bytes=s->bytes[i];
    if (!out || !bytes) return 0;
    switch(i) {
    case RAM:
        if (s->ram!=xbox_GetMemoryBase() || bytes!=xbox_GetMemorySize()) return 0;
        memcpy(out,s->ram,bytes);return 1;
    case HEAP:return xbox_heap_checkpoint_capture(s->heap,out,bytes);
    case FILES:return s->files && xbox_stream_file_checkpoint_capture(out,bytes);
    case APC:return xbox_stream_completion_capture(out,bytes);
    case IRQ:return xbox_kernel_interrupt_capture(out,bytes);
    case GPU:return nv2a_rollback_queue_capture(out,bytes);
    case XPP:return doa3_xpp_capture(out,bytes);
    default:return 0;
    }
}
static int validate(Doa3StateAudit *s,unsigned p,unsigned i)
{
    const void *image=s->image[p][i];size_t bytes=s->bytes[i];
    switch(i) {
    case RAM:return s->ram==xbox_GetMemoryBase() && bytes==xbox_GetMemorySize();
    case HEAP:return xbox_heap_checkpoint_validate(s->heap,image,bytes);
    case FILES:return xbox_stream_file_checkpoint_validate(image,bytes);
    case APC:return xbox_stream_completion_validate(image,bytes);
    case IRQ:return xbox_kernel_interrupt_validate(image,bytes);
    case GPU:return nv2a_queue_validate(image,bytes);
    case XPP:return doa3_xpp_validate(image,bytes);
    default:return 0;
    }
}
int doa3_state_audit_sample(Doa3StateAudit *s)
{
    int ok[2][PARTS]={{0}};
    if (!s || s->owner!=GetCurrentThreadId()) return 0;
    s->stable=0;
    /* Full passes span the rest of the capture work. No guest callbacks. */
    for (unsigned p=0;p<2;p++) for (unsigned i=0;i<PARTS;i++) ok[p][i]=capture(s,p,i);
    for (unsigned i=0;i<PARTS;i++) if (ok[0][i] && ok[1][i] &&
        validate(s,0,i) && validate(s,1,i) && !memcmp(s->image[0][i],s->image[1][i],s->bytes[i]))
        s->stable|=1u<<i;
    return s->stable==127u;
}
unsigned doa3_state_audit_stable_mask(const Doa3StateAudit *s)
{
    return s && s->owner==GetCurrentThreadId() ? s->stable:0;
}
