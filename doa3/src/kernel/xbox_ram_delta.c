/* doa3.1-rollback.17: delta snapshots of guest RAM (see xbox_ram_delta.h). */
#include "xbox_ram_delta.h"
#include "xbox_memory_layout.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>

#define PAGE        4096u
#define MAX_VIEWS   (1 + XBOX_NUM_MIRRORS)
#define RECORDS     16u          /* boundaries kept */
#define COOL_AFTER  16u          /* unchanged boundaries before a hot page is protected again */
#define POOL_PAGES  16384u       /* 64 MB of saved pages */

typedef struct { uint32_t page, buf; } Entry;
typedef struct {
    unsigned ncopy, npre;
    Entry *copy, *pre;           /* each sized for every page */
    unsigned long long seq;
} Record;

static int s_active;
static unsigned char *s_view[MAX_VIEWS];
static size_t s_view_bytes[MAX_VIEWS];
static int s_nviews;
static unsigned s_pages;                  /* pages in the base view */
static uint32_t *s_mask;                  /* views where the page is writable */
static unsigned char *s_hot, *s_streak, *s_untracked;
static uint32_t *s_lastbuf; static unsigned long long *s_lastseq;
static unsigned *s_stamp; static unsigned s_stamp_now;
static unsigned char *s_pool; static uint32_t *s_free; static unsigned s_nfree;
static Record s_rec[RECORDS];
static unsigned s_count;                  /* records held */
static unsigned long long s_seq;          /* seq of the open record */
static SRWLOCK s_lock = SRWLOCK_INIT;
static PVOID s_veh;
static volatile LONG s_faults, s_mirror_faults;
static unsigned s_copied, s_cooled, s_overflow;
static double s_boundary_ms;
static struct { uint32_t lo, hi; } s_untrack[16]; static unsigned s_nuntrack;

static Record *rec_seq(unsigned long long seq) { return &s_rec[seq % RECORDS]; }
static unsigned char *pool_page(uint32_t b) { return s_pool + (size_t)b * PAGE; }
static int pool_take(uint32_t *b)
{
    if (!s_nfree) { s_overflow = 1; return 0; }
    *b = s_free[--s_nfree]; return 1;
}
static void pool_give(uint32_t b) { s_free[s_nfree++] = b; }
static void record_clear(Record *r)
{
    for (unsigned i = 0; i < r->ncopy; ++i) pool_give(r->copy[i].buf);
    for (unsigned i = 0; i < r->npre; ++i) pool_give(r->pre[i].buf);
    r->ncopy = r->npre = 0;
}
static void protect(int v, unsigned page, DWORD prot)
{
    DWORD old;
    if ((size_t)(page + 1) * PAGE <= s_view_bytes[v])
        VirtualProtect(s_view[v] + (size_t)page * PAGE, PAGE, prot, &old);
}

static LONG CALLBACK delta_veh(EXCEPTION_POINTERS *e)
{
    EXCEPTION_RECORD *r = e->ExceptionRecord;
    if (!s_active || r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2 ||
        r->ExceptionInformation[0] != 1) return EXCEPTION_CONTINUE_SEARCH;
    uintptr_t a = (uintptr_t)r->ExceptionInformation[1];
    int v;
    for (v = 0; v < s_nviews; ++v)
        if (a >= (uintptr_t)s_view[v] && a < (uintptr_t)s_view[v] + s_view_bytes[v]) break;
    if (v == s_nviews) return EXCEPTION_CONTINUE_SEARCH;
    unsigned page = (unsigned)((a - (uintptr_t)s_view[v]) / PAGE);
    if (page >= s_pages) return EXCEPTION_CONTINUE_SEARCH;
    AcquireSRWLockExclusive(&s_lock);
    if (!(s_mask[page] & (1u << v))) {
        if (!s_hot[page] && !s_untracked[page]) {
            /* Untouched since it was protected: these bytes are its contents
             * at the open record's boundary. */
            Record *rc = rec_seq(s_seq);
            uint32_t b;
            if (pool_take(&b)) {
                memcpy(pool_page(b), s_view[0] + (size_t)page * PAGE, PAGE);
                rc->pre[rc->npre].page = page; rc->pre[rc->npre].buf = b; ++rc->npre;
            }
            s_hot[page] = 1; s_streak[page] = 0; s_lastseq[page] = ~0ull;
        }
        protect(v, page, PAGE_READWRITE);
        s_mask[page] |= 1u << v;
        InterlockedIncrement(v ? &s_mirror_faults : &s_faults);
    }
    ReleaseSRWLockExclusive(&s_lock);
    return EXCEPTION_CONTINUE_EXECUTION;
}

void xbox_ram_delta_untrack(uint32_t lo, uint32_t hi)
{
    if (!s_active && s_nuntrack < 16 && hi > lo) { s_untrack[s_nuntrack].lo = lo; s_untrack[s_nuntrack].hi = hi; ++s_nuntrack; }
}
int xbox_ram_delta_active(void) { return s_active; }
int xbox_ram_delta_tracked(uint32_t va)
{
    unsigned p = va / PAGE;
    return s_untracked && p < s_pages && !s_untracked[p];
}

static void *big(size_t bytes) { return VirtualAlloc(NULL, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); }
static int allocate(void)
{
    if (s_pool) return 1;
    size_t ram = xbox_GetMemorySize();
    s_pages = (unsigned)(ram / PAGE);
    s_mask = big(s_pages * sizeof(*s_mask));
    s_hot = big(s_pages); s_streak = big(s_pages); s_untracked = big(s_pages);
    s_lastbuf = big(s_pages * sizeof(*s_lastbuf)); s_lastseq = big(s_pages * sizeof(*s_lastseq));
    s_stamp = big(s_pages * sizeof(*s_stamp));
    s_pool = big((size_t)POOL_PAGES * PAGE); s_free = big(POOL_PAGES * sizeof(*s_free));
    for (unsigned i = 0; i < RECORDS; ++i) {
        s_rec[i].copy = big(s_pages * sizeof(Entry)); s_rec[i].pre = big(s_pages * sizeof(Entry));
        if (!s_rec[i].copy || !s_rec[i].pre) return 0;
    }
    return s_mask && s_hot && s_streak && s_untracked && s_lastbuf && s_lastseq && s_stamp && s_pool && s_free;
}

int xbox_ram_delta_begin(void)
{
    if (s_active || !allocate()) return 0;
    s_view[0] = xbox_GetMemoryBase(); s_view_bytes[0] = xbox_GetMemorySize(); s_nviews = 1;
    for (int m = 0; m < XBOX_NUM_MIRRORS; ++m) {
        void *b; size_t n;
        if (xbox_GetMirrorView(m, &b, &n)) { s_view[s_nviews] = b; s_view_bytes[s_nviews] = n; ++s_nviews; }
    }
    s_nfree = 0;
    for (uint32_t b = POOL_PAGES; b-- > 0; ) s_free[s_nfree++] = b;
    memset(s_mask, 0, s_pages * sizeof(*s_mask)); memset(s_hot, 0, s_pages); memset(s_streak, 0, s_pages);
    memset(s_untracked, 0, s_pages); memset(s_stamp, 0, s_pages * sizeof(*s_stamp)); s_stamp_now = 0;
    for (unsigned p = 0; p < s_pages; ++p) s_lastseq[p] = ~0ull;
    for (unsigned u = 0; u < s_nuntrack; ++u)
        for (unsigned p = s_untrack[u].lo / PAGE; p < s_pages && p < (s_untrack[u].hi + PAGE - 1) / PAGE; ++p) {
            /* only pages wholly inside the range */
            if ((uint32_t)p * PAGE >= s_untrack[u].lo && (uint32_t)(p + 1) * PAGE <= s_untrack[u].hi) s_untracked[p] = 1;
        }
    for (unsigned i = 0; i < RECORDS; ++i) s_rec[i].ncopy = s_rec[i].npre = 0;
    s_count = 1; s_seq = 0; s_rec[0].seq = 0;
    s_faults = s_mirror_faults = 0; s_copied = s_cooled = s_overflow = 0;
    s_veh = AddVectoredExceptionHandler(1, delta_veh);
    if (!s_veh) return 0;
    s_active = 1;
    /* Read-only everywhere except the untracked pages: contiguous runs. */
    for (int v = 0; v < s_nviews; ++v) {
        unsigned limit = (unsigned)(s_view_bytes[v] / PAGE);
        if (limit > s_pages) limit = s_pages;
        for (unsigned p = 0; p < limit; ) {
            if (s_untracked[p]) { s_mask[p] |= 1u << v; ++p; continue; }
            unsigned q = p; while (q < limit && !s_untracked[q]) ++q;
            DWORD old; VirtualProtect(s_view[v] + (size_t)p * PAGE, (size_t)(q - p) * PAGE, PAGE_READONLY, &old);
            p = q;
        }
        for (unsigned p = limit; p < s_pages; ++p) s_mask[p] |= 1u << v;   /* not mapped in this view */
    }
    return 1;
}

int xbox_ram_delta_boundary(void)
{
    LARGE_INTEGER f, t0, t1; QueryPerformanceFrequency(&f); QueryPerformanceCounter(&t0);
    if (!s_active) return 0;
    AcquireSRWLockExclusive(&s_lock);
    unsigned long long prev = s_seq;
    if (s_count == RECORDS) { record_clear(rec_seq(s_seq + 1)); --s_count; }  /* drop the oldest */
    Record *rc = rec_seq(++s_seq); rc->ncopy = rc->npre = 0; rc->seq = s_seq; ++s_count;
    s_copied = s_cooled = 0;
    for (unsigned p = 0; p < s_pages; ++p) {
        if (!s_hot[p]) continue;
        const unsigned char *now = s_view[0] + (size_t)p * PAGE;
        if (s_lastseq[p] == prev && !memcmp(now, pool_page(s_lastbuf[p]), PAGE)) {
            if (++s_streak[p] >= COOL_AFTER) {
                for (int v = 0; v < s_nviews; ++v) if (s_mask[p] & (1u << v)) protect(v, p, PAGE_READONLY);
                s_mask[p] = 0; s_hot[p] = 0; s_streak[p] = 0; s_lastseq[p] = ~0ull; ++s_cooled;
                continue;
            }
        } else s_streak[p] = 0;
        uint32_t b;
        if (!pool_take(&b)) break;
        memcpy(pool_page(b), now, PAGE);
        rc->copy[rc->ncopy].page = p; rc->copy[rc->ncopy].buf = b; ++rc->ncopy;
        s_lastbuf[p] = b; s_lastseq[p] = s_seq; ++s_copied;
    }
    ReleaseSRWLockExclusive(&s_lock);
    QueryPerformanceCounter(&t1);
    s_boundary_ms = 1000.0 * (double)(t1.QuadPart - t0.QuadPart) / (double)f.QuadPart;
    return !s_overflow;
}

static void writable_base(unsigned p)
{
    if (!(s_mask[p] & 1u)) { protect(0, p, PAGE_READWRITE); s_mask[p] |= 1u; }
    s_hot[p] = 1; s_streak[p] = 0;
}
int xbox_ram_delta_restore(unsigned back)
{
    if (!s_active || s_overflow || back >= s_count) return 0;
    AcquireSRWLockExclusive(&s_lock);
    unsigned long long t = s_seq - back;
    Record *rt = rec_seq(t);
    unsigned stamp = ++s_stamp_now;
    for (unsigned i = 0; i < rt->ncopy; ++i) {
        unsigned p = rt->copy[i].page;
        s_stamp[p] = stamp; writable_base(p);
        memcpy(s_view[0] + (size_t)p * PAGE, pool_page(rt->copy[i].buf), PAGE);
        s_lastbuf[p] = rt->copy[i].buf; s_lastseq[p] = t;
    }
    for (unsigned i = 0; i < rt->npre; ++i) {
        unsigned p = rt->pre[i].page;
        if (s_stamp[p] == stamp) continue;
        s_stamp[p] = stamp; writable_base(p); s_lastseq[p] = ~0ull;
        memcpy(s_view[0] + (size_t)p * PAGE, pool_page(rt->pre[i].buf), PAGE);
    }
    /* Later records, oldest first: the first pre-image of a page is its
     * contents at t. Keep those in record t (a second rewind needs them). */
    for (unsigned long long j = t + 1; j <= s_seq; ++j) {
        Record *rj = rec_seq(j);
        for (unsigned i = 0; i < rj->npre; ++i) {
            unsigned p = rj->pre[i].page;
            if (s_stamp[p] == stamp) { pool_give(rj->pre[i].buf); continue; }
            s_stamp[p] = stamp; writable_base(p); s_lastseq[p] = ~0ull;
            memcpy(s_view[0] + (size_t)p * PAGE, pool_page(rj->pre[i].buf), PAGE);
            rt->pre[rt->npre++] = rj->pre[i];
        }
        for (unsigned i = 0; i < rj->ncopy; ++i) pool_give(rj->copy[i].buf);
        rj->ncopy = rj->npre = 0;
    }
    s_count -= back; s_seq = t;
    ReleaseSRWLockExclusive(&s_lock);
    return 1;
}

void xbox_ram_delta_prepare_write(uint32_t va, uint32_t bytes)
{
    if (!s_active || !bytes) return;
    unsigned lo = va / PAGE, hi = (unsigned)(((uint64_t)va + bytes + PAGE - 1) / PAGE);
    AcquireSRWLockExclusive(&s_lock);
    for (unsigned p = lo; p < hi && p < s_pages; ++p) {
        if (s_mask[p] & 1u) continue;
        if (!s_hot[p] && !s_untracked[p]) {
            Record *rc = rec_seq(s_seq);
            uint32_t b;
            if (pool_take(&b)) {
                memcpy(pool_page(b), s_view[0] + (size_t)p * PAGE, PAGE);
                rc->pre[rc->npre].page = p; rc->pre[rc->npre].buf = b; ++rc->npre;
            }
            s_hot[p] = 1; s_streak[p] = 0; s_lastseq[p] = ~0ull;
        }
        protect(0, p, PAGE_READWRITE); s_mask[p] |= 1u;
    }
    ReleaseSRWLockExclusive(&s_lock);
}

void xbox_ram_delta_end(void)
{
    if (!s_active) return;
    AcquireSRWLockExclusive(&s_lock);
    for (int v = 0; v < s_nviews; ++v) {
        DWORD old; size_t n = s_view_bytes[v];
        if (n > (size_t)s_pages * PAGE) n = (size_t)s_pages * PAGE;
        VirtualProtect(s_view[v], n, PAGE_READWRITE, &old);
    }
    for (unsigned i = 0; i < RECORDS; ++i) record_clear(&s_rec[i]);
    s_count = 0; s_active = 0;
    ReleaseSRWLockExclusive(&s_lock);
    if (s_veh) { RemoveVectoredExceptionHandler(s_veh); s_veh = NULL; }
    s_nuntrack = 0;
}

void xbox_ram_delta_stats(XboxRamDeltaStats *o)
{
    unsigned hot = 0;
    if (s_hot) for (unsigned p = 0; p < s_pages; ++p) hot += s_hot[p];
    o->records = s_count; o->hot_pages = hot; o->copied = s_copied; o->cooled = s_cooled;
    o->faults = (unsigned)InterlockedExchange(&s_faults, 0);
    o->mirror_faults = (unsigned)InterlockedExchange(&s_mirror_faults, 0);
    o->pool_pages = POOL_PAGES; o->pool_used = POOL_PAGES - s_nfree; o->overflow = s_overflow;
    o->boundary_ms = s_boundary_ms;
}
