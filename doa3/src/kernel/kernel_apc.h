#ifndef DOA3_KERNEL_APC_H
#define DOA3_KERNEL_APC_H
#include <stdint.h>
#include <stddef.h>

#define XBOX_APC_CAPACITY 16u
typedef struct { uint32_t ctx, iosb, bytes; } XboxApcEntry;
/* Zero-initialize before use. Access is confined to the guest/fiber thread. */
typedef struct {
    XboxApcEntry entries[XBOX_APC_CAPACITY];
    uint32_t count, delivering, tainted;
    uint64_t epoch;
} XboxApcQueue;
typedef struct {
    uint32_t version, count;
    uint64_t epoch;
    uintptr_t owner;
    XboxApcEntry entries[XBOX_APC_CAPACITY];
} XboxApcSnapshot;

void xbox_apc_reset(XboxApcQueue *queue);
int xbox_apc_push(XboxApcQueue *queue, XboxApcEntry entry);
/* A batch is detached before guest callbacks run. Callbacks may enqueue or
 * deliver new work without overwriting the remaining entries of this batch.
 * Pair each nonzero begin with end, including a batch with unknown callbacks. */
unsigned xbox_apc_begin(XboxApcQueue *queue, XboxApcEntry batch[XBOX_APC_CAPACITY]);
void xbox_apc_end(XboxApcQueue *queue);
int xbox_apc_capture(const XboxApcQueue *queue, void *out, size_t bytes);
int xbox_apc_restore(XboxApcQueue *queue, const void *saved, size_t bytes);
int xbox_apc_validate(const XboxApcQueue *queue, const void *saved, size_t bytes);

/* Production queue only; primary fiber + deterministic session required.
 * This is process-local completion metadata, NOT a complete streaming snapshot.
 * Caller must separately restore guest RAM/registers, workers, file cursors,
 * kernel objects and clocks. Refuses capture/restore during guest callbacks.
 * No callback executes and no host file is touched by these functions. */
size_t xbox_stream_completion_snapshot_size(void);
int xbox_stream_completion_capture(void *out, size_t bytes);
int xbox_stream_completion_validate(const void *saved, size_t bytes);
int xbox_stream_completion_restore(const void *saved, size_t bytes);
#endif
