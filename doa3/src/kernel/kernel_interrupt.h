#ifndef DOA3_KERNEL_INTERRUPT_H
#define DOA3_KERNEL_INTERRUPT_H
#include <stdint.h>
#include <stddef.h>
#define XBOX_ISR_CAPACITY 4u
#define XBOX_DPC_CAPACITY 16u
typedef struct { uint32_t obj,routine,ctx; } XboxIsrEntry;
typedef struct { uint32_t dpc,a1,a2; } XboxDpcEntry;
/* Guest-owner-thread only. Zero initialize, then reset before first use.
 * This is callback metadata, not guest continuations or the hardware IRQ latch. */
typedef struct {
    XboxIsrEntry isr[XBOX_ISR_CAPACITY];
    XboxDpcEntry dpc[XBOX_DPC_CAPACITY];
    uint32_t isr_count,dpc_count,delivering,tainted,owner;
    uint64_t generation;
} XboxInterruptState;
typedef struct {
    uint32_t version,isr_count,dpc_count,reserved;
    uint64_t generation,epoch;
    uintptr_t owner;
    XboxIsrEntry isr[XBOX_ISR_CAPACITY];
    XboxDpcEntry dpc[XBOX_DPC_CAPACITY];
} XboxInterruptSnapshot;
int xbox_interrupt_reset(XboxInterruptState *);
void xbox_interrupt_register(XboxInterruptState *,XboxIsrEntry);
/* Preserve existing guest results: duplicate=0; new=1, even if full. Full or
 * invalid work taints checkpointing, without stopping normal delivery. */
int xbox_interrupt_push(XboxInterruptState *,XboxDpcEntry);
int xbox_interrupt_get_isr(const XboxInterruptState *,unsigned,XboxIsrEntry *);
int xbox_interrupt_pop(XboxInterruptState *,XboxDpcEntry *);
int xbox_interrupt_begin(XboxInterruptState *);
void xbox_interrupt_end(XboxInterruptState *);
/* Require all writers pinned and no delivery in progress. No callbacks run.
 * Pair restore with RAM, fibers, APU and other host state. Local blobs only. */
int xbox_interrupt_capture(const XboxInterruptState *,uint64_t epoch,void *,size_t);
int xbox_interrupt_validate(const XboxInterruptState *,uint64_t epoch,const void *,size_t);
int xbox_interrupt_restore(XboxInterruptState *,uint64_t epoch,const void *,size_t);

/* Production bridge: deterministic clock, primary fiber and passive IRQL only. */
int xbox_kernel_interrupt_begin(void);
void xbox_kernel_interrupt_end(void);
int xbox_kernel_interrupt_capture(void *,size_t);
int xbox_kernel_interrupt_validate(const void *,size_t);
int xbox_kernel_interrupt_restore(const void *,size_t);
#endif
