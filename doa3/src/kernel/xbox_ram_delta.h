#ifndef XBOX_RAM_DELTA_H
#define XBOX_RAM_DELTA_H
/* doa3.1-rollback.17: delta snapshots of guest RAM.
 *
 * Instead of copying the whole guest space at every rollback boundary, the
 * pages the game writes are found by page protection: untouched pages are
 * read-only in the base view and every mirror view, the first write to one
 * faults, the vectored handler keeps its contents from before the write (a
 * pre-image, which is its contents at the last boundary) and makes the page
 * writable in that view. Written ("hot") pages are copied at every boundary;
 * a hot page unchanged for 16 boundaries is made read-only again.
 *
 * Record k describes guest RAM at boundary k: copies of the hot pages taken
 * at k, plus pre-images of pages first written after k. Restoring boundary t
 * applies the pre-images of records newest..t, then record t's copies.
 *
 * Memory that is reset every frame or is pure GPU output is never tracked
 * (xbox_ram_delta_untrack). A Win32 call that writes into guest RAM must call
 * xbox_ram_delta_prepare_write first: the kernel does not raise the fault, it
 * fails the call. */
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
int  xbox_ram_delta_begin(void);           /* protect; record 0 = now */
void xbox_ram_delta_untrack(uint32_t lo, uint32_t hi); /* before begin */
int  xbox_ram_delta_boundary(void);        /* close the record, open the next */
int  xbox_ram_delta_restore(unsigned back);/* to the boundary `back` records ago */
void xbox_ram_delta_end(void);             /* everything writable again */
int  xbox_ram_delta_active(void);
unsigned long long xbox_ram_delta_seq(void);   /* the open record's boundary number */
unsigned xbox_ram_delta_records(void);         /* boundaries that can be restored, counting the open one */
/* Hot pages: their numbers (up to max), for a comparison copy. */
unsigned xbox_ram_delta_hot_pages(uint32_t *pages, unsigned max);
void xbox_ram_delta_prepare_write(uint32_t va, uint32_t bytes);
int  xbox_ram_delta_tracked(uint32_t va);  /* page is tracked (not untracked) */
typedef struct {
    unsigned records, hot_pages, copied, cooled, faults, mirror_faults, pool_used, pool_pages, overflow;
    double boundary_ms;
} XboxRamDeltaStats;
void xbox_ram_delta_stats(XboxRamDeltaStats *out); /* faults since the last boundary */
#ifdef __cplusplus
}
#endif
#endif
