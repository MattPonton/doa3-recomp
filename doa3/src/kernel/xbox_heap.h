#ifndef DOA3_XBOX_HEAP_H
#define DOA3_XBOX_HEAP_H
#include <stddef.h>

/* Process-local allocator bookkeeping only; pair with a guest RAM checkpoint.
 * Owner-thread calls only, while ALL allocator users are quiescent. A caller
 * must separately validate fibers, GPU resources and every other host object
 * retaining heap addresses before restoring. This does not restore mappings,
 * VM protections, resource lifetimes or allocation contents. No live adapter.
 * Buffers belong outside guest RAM and must not alias session storage.
 * Never accept these blobs from disk or a network peer. */
typedef struct XboxHeapCheckpoint XboxHeapCheckpoint;
XboxHeapCheckpoint *xbox_heap_checkpoint_create(void);
void xbox_heap_checkpoint_destroy(XboxHeapCheckpoint *);
size_t xbox_heap_checkpoint_size(void);
int xbox_heap_checkpoint_capture(XboxHeapCheckpoint *,void *,size_t);
int xbox_heap_checkpoint_validate(XboxHeapCheckpoint *,const void *,size_t);
int xbox_heap_checkpoint_restore(XboxHeapCheckpoint *,const void *,size_t);
/* Memory-layout lifecycle notification; invalidates existing sessions even
 * if a replacement mapping happens to occupy the same host address. */
void xbox_heap_checkpoint_invalidate(void);
#endif
