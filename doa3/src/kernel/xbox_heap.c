#include "xbox_memory_layout.h"
#include "xbox_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { uint32_t va,size; } HeapBlock;

/* ── Dynamic heap allocator ────────────────────────────────
 *
 * Best-fit free-list and bump allocator for guest memory.
 * Returns Xbox VAs within the mapped region so MEM32() works correctly.
 */
static uint32_t g_heap_next = XBOX_HEAP_BASE;
/* Low-heap ceiling. Blocks reserved with xbox_HeapReserveTop() are carved
 * off the top and this drops to match, so the bump allocator below never
 * hands the same memory out. Reserving from the top rather than the front
 * leaves every game allocation at the address it had before. */
static uint32_t g_heap_limit = XBOX_HEAP_BASE + XBOX_HEAP_SIZE;

static int g_heap_alloc_count = 0;

/* Block bookkeeping for the low heap.
 *
 * Live blocks sit in an open-addressing hash (VA -> size) and free blocks in
 * a list kept sorted by address, so a free merges with both neighbours in
 * one step and a free block that ends at the bump pointer hands its space
 * straight back to it.
 *
 * The old tracker was a flat 512-entry table. Every allocation past the
 * 512th went untracked, so its free was silently ignored; and ExFreePool was
 * a no-op. Together those leaked DirectSound's per-stream pool blocks
 * (0x10004 / 0x8004 bytes, one set per ADX stream) until the heap ran dry
 * after a few modes, at which point ADXT_StartAfs failed with "can't open"
 * and the BGM and voices went silent while one-shot effects still played.
 * No headers: Xbox code receives raw aligned VAs. */
#define HEAP_LIVE_CAP  65536u                 /* power of two */
#define HEAP_FREE_CAP  16384
static HeapBlock g_heap_live[HEAP_LIVE_CAP];   /* va 0 = empty, 1 = tombstone */
static uint32_t g_heap_live_n = 0, g_heap_live_used = 0;
static HeapBlock g_heap_free[HEAP_FREE_CAP];   /* sorted by va */
static int g_heap_free_n = 0;
static uint32_t g_heap_free_bytes = 0, g_heap_live_bytes = 0;
static uint32_t g_heap_untracked = 0, g_heap_oom = 0;

static uint32_t heap_hash(uint32_t va) { return (va >> 4) * 2654435761u; }

static void heap_live_put(uint32_t va, uint32_t size)
{
    uint32_t i, tomb = 0xFFFFFFFFu;
    if (g_heap_live_used >= HEAP_LIVE_CAP - HEAP_LIVE_CAP / 8) {   /* keep probes short */
        g_heap_untracked++;
        return;
    }
    for (i = heap_hash(va) & (HEAP_LIVE_CAP - 1);; i = (i + 1) & (HEAP_LIVE_CAP - 1)) {
        if (g_heap_live[i].va == 0) break;
        if (g_heap_live[i].va == 1) { if (tomb == 0xFFFFFFFFu) tomb = i; continue; }
        if (g_heap_live[i].va == va) {
            g_heap_live_bytes += size - g_heap_live[i].size;
            g_heap_live[i].size = size;
            return;
        }
    }
    if (tomb != 0xFFFFFFFFu) i = tomb; else g_heap_live_used++;
    g_heap_live[i].va = va; g_heap_live[i].size = size;
    g_heap_live_n++; g_heap_live_bytes += size;
}

/* Removes va from the live set; returns its size, or 0 if it was not live. */
static uint32_t heap_live_take(uint32_t va)
{
    for (uint32_t i = heap_hash(va) & (HEAP_LIVE_CAP - 1);; i = (i + 1) & (HEAP_LIVE_CAP - 1)) {
        if (g_heap_live[i].va == 0) return 0;
        if (g_heap_live[i].va == va) {
            uint32_t s = g_heap_live[i].size;
            g_heap_live[i].va = 1; g_heap_live[i].size = 0;
            g_heap_live_n--; g_heap_live_bytes -= s;
            return s;
        }
    }
}

uint32_t xbox_HeapBlockSize(uint32_t va)
{
    if (va < 2) return 0;
    for (uint32_t i = heap_hash(va) & (HEAP_LIVE_CAP - 1);; i = (i + 1) & (HEAP_LIVE_CAP - 1)) {
        if (g_heap_live[i].va == 0) return 0;
        if (g_heap_live[i].va == va) return g_heap_live[i].size;
    }
}

static void heap_free_remove(int k)
{
    g_heap_free_bytes -= g_heap_free[k].size;
    memmove(&g_heap_free[k], &g_heap_free[k + 1],
            (size_t)(g_heap_free_n - k - 1) * sizeof g_heap_free[0]);
    g_heap_free_n--;
}

/* Inserts [va, va+size) into the free list, merging with its neighbours and
 * giving a block that ends at the bump pointer back to the bump region.
 * Returns 0 only when the list is full and nothing could be merged. */
static int heap_free_insert(uint32_t va, uint32_t size)
{
    int lo = 0, hi = g_heap_free_n;
    if (!size) return 1;
    while (lo < hi) { int m = (lo + hi) / 2; if (g_heap_free[m].va < va) lo = m + 1; else hi = m; }
    /* lo = first free block at or above va */
    if (lo > 0 && g_heap_free[lo - 1].va + g_heap_free[lo - 1].size == va) {
        va = g_heap_free[lo - 1].va; size += g_heap_free[lo - 1].size;
        heap_free_remove(--lo);
    }
    if (lo < g_heap_free_n && va + size == g_heap_free[lo].va) {
        size += g_heap_free[lo].size;
        heap_free_remove(lo);
    }
    if (va + size == g_heap_next) {          /* top of the bump region: give it back */
        g_heap_next = va;
        return 1;
    }
    if (g_heap_free_n >= HEAP_FREE_CAP) return 0;
    memmove(&g_heap_free[lo + 1], &g_heap_free[lo],
            (size_t)(g_heap_free_n - lo) * sizeof g_heap_free[0]);
    g_heap_free[lo].va = va; g_heap_free[lo].size = size;
    g_heap_free_n++; g_heap_free_bytes += size;
    return 1;
}

void xbox_HeapStats(uint32_t *bump_used, uint32_t *limit, uint32_t *live_n, uint32_t *live_bytes,
                    uint32_t *free_n, uint32_t *free_bytes, uint32_t *largest_free, uint32_t *oom)
{
    uint32_t big = 0;
    for (int i = 0; i < g_heap_free_n; i++) if (g_heap_free[i].size > big) big = g_heap_free[i].size;
    if (g_heap_limit - g_heap_next > big) big = g_heap_limit - g_heap_next;
    *bump_used = g_heap_next - XBOX_HEAP_BASE; *limit = g_heap_limit - XBOX_HEAP_BASE;
    *live_n = g_heap_live_n; *live_bytes = g_heap_live_bytes;
    *free_n = (uint32_t)g_heap_free_n; *free_bytes = g_heap_free_bytes;
    *largest_free = big; *oom = g_heap_oom;
}

uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment)
{
    uint32_t result, gran;

    if (alignment < 16) alignment = 16;

    /* Zero-size requests must still get their own block: the Xbox D3D8 code
     * computes some resource sizes from GPU capabilities that read back 0
     * here, and with a bump allocator those would all share one address and
     * overlap. */
    if (size == 0) size = 4096;
    else if (size < 64) size = 64;
    /* Small blocks round to 64 bytes, page-sized ones to a page, so split
     * remainders stay reusable. Page-aligned requests (the Mm* and Nt*
     * allocators) always cover whole pages, as on the console: DirectSound
     * relies on that. It asks for 4- and 16-byte blocks with page alignment
     * and hands the rest of the page to its "slop" heap; with a 64-byte
     * block the next allocation landed inside that slop, and the slop list
     * links were overwritten (an endless walk in sub_001F6D91 at boot). */
    gran = (size >= 4096 || alignment >= 4096) ? 4096u : 64u;
    size = (size + gran - 1) & ~(gran - 1);

    /* Best fit over the free list. A block whose start is not aligned can
     * still hold an aligned sub-range: align up inside it, give the skipped
     * head back, and split off any tail. */
    {
        int best = -1;
        uint32_t best_va = 0;
        for (int i = 0; i < g_heap_free_n; i++) {
            uint32_t va = (g_heap_free[i].va + alignment - 1) & ~(alignment - 1);
            uint32_t end = g_heap_free[i].va + g_heap_free[i].size;
            if (va < g_heap_free[i].va || va > end || end - va < size) continue;
            if (best < 0 || g_heap_free[i].size < g_heap_free[best].size) {
                best = i; best_va = va;
                if (g_heap_free[i].size == size) break;
            }
        }
        if (best >= 0) {
            uint32_t bva = g_heap_free[best].va, bend = bva + g_heap_free[best].size;
            heap_free_remove(best);
            if (bend - (best_va + size) >= 64) heap_free_insert(best_va + size, bend - (best_va + size));
            else size = bend - best_va;            /* sliver: keep it with the block */
            if (best_va > bva) heap_free_insert(bva, best_va - bva);
            memset((void *)((uintptr_t)best_va + xbox_GetMemoryOffset()), 0, size);
            heap_live_put(best_va, size);
            g_heap_alloc_count++;
            return best_va;
        }
    }

    result = (g_heap_next + alignment - 1) & ~(alignment - 1);
    if (result < g_heap_next || result + size < result || result + size > g_heap_limit) {
        g_heap_oom++;
        fprintf(stderr, "xbox_HeapAlloc: out of memory (requested %u, bump %u/%u, "
                "live %u blocks %u bytes, free %d blocks %u bytes)\n",
                size, g_heap_next - XBOX_HEAP_BASE, g_heap_limit - XBOX_HEAP_BASE,
                g_heap_live_n, g_heap_live_bytes, g_heap_free_n, g_heap_free_bytes);
        return 0;
    }
    if (result > g_heap_next) {
        /* Alignment gap: claim the block first so the gap cannot merge back
         * into the bump region, then keep the gap on the free list. */
        uint32_t gap_va = g_heap_next, gap = result - g_heap_next;
        g_heap_next = result + size;
        heap_free_insert(gap_va, gap);
    } else {
        g_heap_next = result + size;
    }

    /* Zero-fill the allocated block (Xbox memory is always zeroed) */
    memset((void *)((uintptr_t)result + xbox_GetMemoryOffset()), 0, size);
    heap_live_put(result, size);
    g_heap_alloc_count++;
    return result;
}

void xbox_HeapFree(uint32_t xbox_va)
{
    uint32_t size;
    if (xbox_va < 2) return;
    size = heap_live_take(xbox_va);
    if (!size) return;                    /* unknown or double free: ignore */
    if (!heap_free_insert(xbox_va, size))
        fprintf(stderr, "xbox_HeapFree: free list full, %u bytes at %08X lost\n", size, xbox_va);
}

/* High heap (above the console's 64 MB): CPU-only allocations — see the
 * header comment. Same bump model as xbox_HeapAlloc. */
/* Reserve a block at the top of the low heap.
 *
 * Needed for anything the *guest* converts from a virtual to a physical
 * address. Xbox RAM is 64 MB and VA == PA there, so the console D3D8
 * library performs that conversion with a bare 26-bit mask (& 0x03FFFFFF;
 * see recomp_0010.c). A buffer placed above 64 MB therefore aliases onto
 * low RAM as soon as the guest masks its address: the D3D8 push buffer at
 * 0x04000000-0x04400000 aliased onto 0x00000000-0x00400000, so its vertex
 * writes landed in .data and corrupted the 175-object array at 0x370C48
 * that sub_000E8BB0 dispatches through -- vtable pointers overwritten with
 * vertex data, then called. Keeping such buffers under 64 MB makes the
 * guest mask the identity it is on hardware. */
uint32_t xbox_HeapReserveTop(uint32_t size, uint32_t alignment)
{
    uint32_t base;

    if (alignment < 4) alignment = 4;
    if (size > g_heap_limit - g_heap_next) {
        fprintf(stderr, "xbox_HeapReserveTop: no room (requested %u, free %u)\n",
                size, g_heap_limit - g_heap_next);
        return 0;
    }
    base = (g_heap_limit - size) & ~(alignment - 1);
    g_heap_limit = base;
    memset((void *)((uintptr_t)base + xbox_GetMemoryOffset()), 0, size);
    return base;
}

static uint32_t g_high_next = XBOX_HIGH_BASE;

uint32_t xbox_HeapAllocHigh(uint32_t size, uint32_t alignment)
{
    uint32_t result;

    if (alignment < 4) alignment = 4;
    result = (g_high_next + alignment - 1) & ~(alignment - 1);

    if (result + size > XBOX_HIGH_BASE + XBOX_HIGH_SIZE) {
        fprintf(stderr, "xbox_HeapAllocHigh: out of memory (requested %u, used %u/%u)\n",
                size, g_high_next - XBOX_HIGH_BASE, XBOX_HIGH_SIZE);
        return 0;
    }

    g_high_next = result + size;
    memset((void *)((uintptr_t)result + xbox_GetMemoryOffset()), 0, size);
    return result;
}

/* Everything above retains the existing allocation policy. Checkpointing is
 * opt-in and has no work on the normal allocation/free path. */
typedef struct {
    uint64_t magic,version,generation,mapping;
    uint32_t g_heap_next,g_heap_limit,g_high_next;
    int g_heap_alloc_count;
    HeapBlock g_heap_live[HEAP_LIVE_CAP],g_heap_free[HEAP_FREE_CAP];
    uint32_t g_heap_live_n,g_heap_live_used;
    int g_heap_free_n;
    uint32_t g_heap_free_bytes,g_heap_live_bytes,g_heap_untracked,g_heap_oom;
} HeapSnapshot;
struct XboxHeapCheckpoint {
    uint64_t generation,mapping;
    DWORD owner;
    ptrdiff_t offset;
    HeapSnapshot image;
    HeapBlock spans[HEAP_LIVE_CAP+HEAP_FREE_CAP];
    uint32_t empty_prefix[HEAP_LIVE_CAP*2+1];
};
static uint64_t checkpoint_generation,checkpoint_mapping=1;
void xbox_heap_checkpoint_invalidate(void)
{
    if(checkpoint_mapping) ++checkpoint_mapping; /* overflow permanently denies */
}
XboxHeapCheckpoint *xbox_heap_checkpoint_create(void)
{
    if(!checkpoint_mapping || checkpoint_generation==UINT64_MAX || g_heap_untracked) return NULL;
    XboxHeapCheckpoint *s=calloc(1,sizeof(*s));
    if(s) {
        s->generation=++checkpoint_generation;s->mapping=checkpoint_mapping;
        s->offset=xbox_GetMemoryOffset();s->owner=GetCurrentThreadId();
    }
    return s;
}
void xbox_heap_checkpoint_destroy(XboxHeapCheckpoint *s)
{
    if(s && s->owner==GetCurrentThreadId()) free(s);
}
size_t xbox_heap_checkpoint_size(void) { return sizeof(HeapSnapshot); }
static int checkpoint_current(const XboxHeapCheckpoint *s)
{
    return s && s->owner==GetCurrentThreadId() && checkpoint_mapping &&
        s->mapping==checkpoint_mapping && s->offset==xbox_GetMemoryOffset() && !g_heap_untracked;
}
static int span_compare(const void *a,const void *b)
{
    const HeapBlock *x=a,*y=b;
    return (x->va>y->va)-(x->va<y->va);
}
static int span_valid(HeapBlock b,uint32_t end)
{
    return b.va>=XBOX_HEAP_BASE && b.va<end && b.size && b.size<=end-b.va;
}
static int checkpoint_image_valid(XboxHeapCheckpoint *s)
{
    const HeapSnapshot *p=&s->image;
    if(p->magic!=UINT64_C(0x444F413348454150) || p->version!=1 ||
       p->generation!=s->generation || p->mapping!=s->mapping || p->g_heap_untracked ||
       p->g_heap_next<XBOX_HEAP_BASE || p->g_heap_next>p->g_heap_limit ||
       p->g_heap_limit>XBOX_HEAP_BASE+XBOX_HEAP_SIZE ||
       p->g_high_next<XBOX_HIGH_BASE || p->g_high_next>XBOX_HIGH_BASE+XBOX_HIGH_SIZE ||
       p->g_heap_free_n<0 || p->g_heap_free_n>HEAP_FREE_CAP ||
       p->g_heap_live_used>HEAP_LIVE_CAP-HEAP_LIVE_CAP/8 ||
       p->g_heap_live_n>p->g_heap_live_used) return 0;
    /* Reachability in the open-addressed hash: no empty slot may interrupt a
     * key's probe chain. Prefix counts keep corrupt long clusters linear-time. */
    s->empty_prefix[0]=0;
    for(unsigned i=0;i<HEAP_LIVE_CAP*2;++i)
        s->empty_prefix[i+1]=s->empty_prefix[i]+(p->g_heap_live[i&(HEAP_LIVE_CAP-1)].va==0);
    unsigned live=0,used=0,count=0;
    uint64_t live_bytes=0,free_bytes=0;
    for(unsigned i=0;i<HEAP_LIVE_CAP;++i) {
        HeapBlock b=p->g_heap_live[i];
        used+=b.va!=0;
        if(b.va<2) { if(b.size) return 0;continue; }
        if(!span_valid(b,p->g_heap_next) || (b.va&15u)) return 0;
        unsigned first=heap_hash(b.va)&(HEAP_LIVE_CAP-1),last=i<first ? i+HEAP_LIVE_CAP : i;
        if(s->empty_prefix[last]!=s->empty_prefix[first]) return 0;
        ++live;live_bytes+=b.size;s->spans[count++]=b;
    }
    for(int i=0;i<p->g_heap_free_n;++i) {
        HeapBlock b=p->g_heap_free[i];
        if(!span_valid(b,p->g_heap_next) ||
           (i && p->g_heap_free[i-1].va+p->g_heap_free[i-1].size>b.va)) return 0;
        free_bytes+=b.size;s->spans[count++]=b;
    }
    if(live!=p->g_heap_live_n || used!=p->g_heap_live_used ||
       live_bytes!=p->g_heap_live_bytes || free_bytes!=p->g_heap_free_bytes) return 0;
    qsort(s->spans,count,sizeof(s->spans[0]),span_compare);
    for(unsigned i=1;i<count;++i)
        if(s->spans[i-1].va+s->spans[i-1].size>s->spans[i].va) return 0;
    return 1;
}
#define HEAP_CHECKPOINT_FIELDS(F) \
    F(g_heap_next) F(g_heap_limit) F(g_high_next) F(g_heap_alloc_count) \
    F(g_heap_live) F(g_heap_free) F(g_heap_live_n) F(g_heap_live_used) \
    F(g_heap_free_n) F(g_heap_free_bytes) F(g_heap_live_bytes) F(g_heap_untracked) F(g_heap_oom)
int xbox_heap_checkpoint_capture(XboxHeapCheckpoint *s,void *out,size_t bytes)
{
    if(!out || bytes!=sizeof(HeapSnapshot) || !checkpoint_current(s)) return 0;
    HeapSnapshot *p=&s->image;memset(p,0,sizeof(*p));
    p->magic=UINT64_C(0x444F413348454150);p->version=1;
    p->generation=s->generation;p->mapping=s->mapping;
#define SAVE(field) memcpy(&p->field,&field,sizeof(field));
    HEAP_CHECKPOINT_FIELDS(SAVE)
#undef SAVE
    if(!checkpoint_image_valid(s)) return 0;
    memcpy(out,p,sizeof(*p));return 1;
}
int xbox_heap_checkpoint_validate(XboxHeapCheckpoint *s,const void *saved,size_t bytes)
{
    if(!saved || bytes!=sizeof(HeapSnapshot) || !checkpoint_current(s)) return 0;
    memcpy(&s->image,saved,bytes);
    return checkpoint_image_valid(s);
}
int xbox_heap_checkpoint_restore(XboxHeapCheckpoint *s,const void *saved,size_t bytes)
{
    if(!xbox_heap_checkpoint_validate(s,saved,bytes)) return 0;
    const HeapSnapshot *p=&s->image;
#define LOAD(field) memcpy(&field,&p->field,sizeof(field));
    HEAP_CHECKPOINT_FIELDS(LOAD)
#undef LOAD
    return 1;
}
#undef HEAP_CHECKPOINT_FIELDS

