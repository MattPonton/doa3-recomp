#include "kernel_apc.h"
#include <string.h>

static int entry_valid(XboxApcEntry entry) { return entry.ctx && entry.iosb; }
static int queue_ready(const XboxApcQueue *queue)
{
    if (!queue || queue->delivering || queue->tainted || queue->count>XBOX_APC_CAPACITY)
        return 0;
    for (unsigned i=0;i<queue->count;++i)
        if (!entry_valid(queue->entries[i])) return 0;
    return 1;
}
void xbox_apc_reset(XboxApcQueue *queue)
{
    /* Resetting under a live callback would discard its in-flight batch. */
    if (!queue || queue->delivering) return;
    uint64_t epoch=queue->epoch+1;
    memset(queue,0,sizeof(*queue));
    queue->epoch=epoch;
}
int xbox_apc_push(XboxApcQueue *queue,XboxApcEntry entry)
{
    if (!queue) return 0;
    if (queue->count>=XBOX_APC_CAPACITY || !entry_valid(entry)) {
        /* Never certify a checkpoint after losing a completion. Preserve the
         * existing bounded queue's behavior, but make the loss observable. */
        queue->tainted=1;
        return 0;
    }
    queue->entries[queue->count++]=entry;
    return 1;
}
unsigned xbox_apc_begin(XboxApcQueue *queue,XboxApcEntry batch[XBOX_APC_CAPACITY])
{
    if (!queue || !batch || !queue->count) return 0;
    if (queue->count>XBOX_APC_CAPACITY || queue->delivering==UINT32_MAX) {
        queue->tainted=1;
        return 0;
    }
    unsigned count=queue->count;
    memcpy(batch,queue->entries,count*sizeof(*batch));
    memset(queue->entries,0,sizeof(queue->entries));
    queue->count=0;
    ++queue->delivering;
    return count;
}
void xbox_apc_end(XboxApcQueue *queue)
{
    if (!queue) return;
    if (!queue->delivering) { queue->tainted=1; return; }
    --queue->delivering;
}
int xbox_apc_capture(const XboxApcQueue *queue,void *out,size_t bytes)
{
    if (!out || bytes!=sizeof(XboxApcSnapshot) || !queue_ready(queue)) return 0;
    XboxApcSnapshot snapshot={0};
    snapshot.version=1;
    snapshot.count=queue->count;
    snapshot.epoch=queue->epoch;
    snapshot.owner=(uintptr_t)queue;
    memcpy(snapshot.entries,queue->entries,queue->count*sizeof(XboxApcEntry));
    memcpy(out,&snapshot,sizeof(snapshot));
    return 1;
}
int xbox_apc_validate(const XboxApcQueue *queue,const void *saved,size_t bytes)
{
    if (!saved || bytes!=sizeof(XboxApcSnapshot) || !queue_ready(queue)) return 0;
    XboxApcSnapshot snapshot;
    memcpy(&snapshot,saved,sizeof(snapshot));
    if (snapshot.version!=1 || snapshot.owner!=(uintptr_t)queue ||
        snapshot.epoch!=queue->epoch || snapshot.count>XBOX_APC_CAPACITY) return 0;
    for (unsigned i=0;i<XBOX_APC_CAPACITY;++i) {
        XboxApcEntry e=snapshot.entries[i];
        if (i<snapshot.count ? !entry_valid(e) : (e.ctx || e.iosb || e.bytes)) return 0;
    }
    return 1;
}
int xbox_apc_restore(XboxApcQueue *queue,const void *saved,size_t bytes)
{
    if (!xbox_apc_validate(queue,saved,bytes)) return 0;
    XboxApcSnapshot snapshot; memcpy(&snapshot,saved,sizeof(snapshot));
    /* All rejection paths precede mutation. The in-flight batch is never a
     * serialized C continuation: queue_ready excludes it entirely. */
    memcpy(queue->entries,snapshot.entries,sizeof(queue->entries));
    queue->count=snapshot.count;
    return 1;
}
