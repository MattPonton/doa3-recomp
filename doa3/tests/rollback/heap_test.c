/* Include the unchanged production allocator to inject malformed bookkeeping
 * without exposing snapshot internals through a public API. */
#include "../../src/kernel/xbox_heap.c"
#include "guest_memory_fixture.h"
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
#define RAM_BYTES (128u*1024u*1024u)
static unsigned char *ram;
static ptrdiff_t offset;
ptrdiff_t xbox_GetMemoryOffset(void) { return offset; }
static DWORD WINAPI wrong_thread(void *v)
{
    XboxHeapCheckpoint *s=v;HeapSnapshot *p=malloc(sizeof(*p));CHECK(p);
    CHECK(!xbox_heap_checkpoint_capture(s,p,sizeof(*p)));
    CHECK(!xbox_heap_checkpoint_validate(s,p,sizeof(*p)));
    CHECK(!xbox_heap_checkpoint_restore(s,p,sizeof(*p)));free(p);return 0;
}
static void rejected(XboxHeapCheckpoint *s,const HeapSnapshot *bad,const HeapSnapshot *before)
{
    HeapSnapshot *after=malloc(sizeof(*after));CHECK(after);
    CHECK(!xbox_heap_checkpoint_validate(s,bad,sizeof(*bad)));
    CHECK(!xbox_heap_checkpoint_restore(s,bad,sizeof(*bad)));
    CHECK(xbox_heap_checkpoint_capture(s,after,sizeof(*after)));
    CHECK(!memcmp(before,after,sizeof(*after)));free(after);
}
static void exercise(uint32_t *addresses)
{
    for(unsigned i=0;i<96;++i) {
        uint32_t va=xbox_HeapAlloc(33+i*13,i%7 ? 16 : 4096);CHECK(va);
        addresses[i]=va;uint32_t bytes=xbox_HeapBlockSize(va);CHECK(bytes>=33+i*13);
        for(unsigned j=0;j<bytes;++j) CHECK(ram[va+j]==0);
        memset(ram+va,(int)i+1,bytes);
        if(i%3==0) xbox_HeapFree(va);
    }
    addresses[96]=xbox_HeapAllocHigh(97,256);CHECK(addresses[96] && !(addresses[96]&255));
    addresses[97]=xbox_HeapReserveTop(132,256);CHECK(addresses[97] && !(addresses[97]&255));
    memset(ram+addresses[96],0xA7,97);memset(ram+addresses[97],0xB8,132);
    addresses[98]=xbox_HeapAlloc(0,16);CHECK(xbox_HeapBlockSize(addresses[98])==4096);
    /* Restore must also put failed-allocation diagnostics back. */
    CHECK(!xbox_HeapAlloc(XBOX_HEAP_SIZE,4096));
}
int main(void)
{
    ram=guest_memory_fixture_alloc(RAM_BYTES);CHECK(ram);offset=(ptrdiff_t)ram;
    XboxHeapCheckpoint *s=xbox_heap_checkpoint_create();CHECK(s);
    size_t n=xbox_heap_checkpoint_size();CHECK(n==sizeof(HeapSnapshot));
    HeapSnapshot *saved=malloc(n),*expected=malloc(n),*actual=malloc(n),*bad=malloc(n);
    unsigned char *before=malloc(RAM_BYTES),*result=malloc(RAM_BYTES);CHECK(saved && expected && actual && bad && before && result);
    /* Fragmentation, tombstones, adjacency merging and alignment padding. */
    uint32_t blocks[12];for(unsigned i=0;i<12;++i) { blocks[i]=xbox_HeapAlloc(500+i*71,64);CHECK(blocks[i]); }
    xbox_HeapFree(blocks[2]);xbox_HeapFree(blocks[3]);xbox_HeapFree(blocks[8]);xbox_HeapFree(blocks[11]);
    xbox_HeapFree(blocks[11]);xbox_HeapFree(0x1234);CHECK(!xbox_HeapBlockSize(blocks[2]));
    CHECK(xbox_heap_checkpoint_capture(s,saved,n));memcpy(before,ram,RAM_BYTES);
    uint32_t a[99],b[99];exercise(a);CHECK(xbox_heap_checkpoint_capture(s,expected,n));memcpy(result,ram,RAM_BYTES);
    for(unsigned replay=0;replay<3;++replay) {
        CHECK(xbox_heap_checkpoint_restore(s,saved,n));memcpy(ram,before,RAM_BYTES);
        exercise(b);CHECK(!memcmp(a,b,sizeof(a)) && !memcmp(result,ram,RAM_BYTES));
        CHECK(xbox_heap_checkpoint_capture(s,actual,n) && !memcmp(expected,actual,n));
    }
    CHECK(!xbox_heap_checkpoint_restore(s,saved,n-1));
    unsigned first=0;while(expected->g_heap_live[first].va<2) ++first;
    unsigned second=first+1;while(expected->g_heap_live[second].va<2) ++second;
    for(unsigned fault=0;fault<10;++fault) {
        memcpy(bad,expected,n);
        switch(fault) {
        case 0:bad->g_heap_next=XBOX_HEAP_BASE-1;break;
        case 1:bad->g_heap_limit=bad->g_heap_next-1;break;
        case 2:bad->g_high_next=XBOX_HIGH_BASE+XBOX_HIGH_SIZE+1;break;
        case 3:bad->g_heap_free_n=HEAP_FREE_CAP+1;break;
        case 4:++bad->g_heap_live_bytes;break;
        case 5:bad->g_heap_live[first].size=UINT32_MAX;break;
        case 6:bad->g_heap_live[second].va=bad->g_heap_live[first].va;break;
        case 7:CHECK(bad->g_heap_free_n);bad->g_heap_free[0].va=bad->g_heap_live[first].va;break;
        case 8:bad->g_heap_untracked=1;break;
        case 9:bad->g_heap_live_used=HEAP_LIVE_CAP;break;
        }
        rejected(s,bad,expected);
    }
    /* Same accounting, valid spans, but moved beyond an empty hash slot. */
    memcpy(bad,expected,n);unsigned empty=(first+1)&(HEAP_LIVE_CAP-1);
    while(bad->g_heap_live[empty].va) empty=(empty+1)&(HEAP_LIVE_CAP-1);
    unsigned dest=(empty+1)&(HEAP_LIVE_CAP-1);while(bad->g_heap_live[dest].va) dest=(dest+1)&(HEAP_LIVE_CAP-1);
    bad->g_heap_live[dest]=bad->g_heap_live[first];bad->g_heap_live[first]=(HeapBlock){0,0};rejected(s,bad,expected);
    XboxHeapCheckpoint *other=xbox_heap_checkpoint_create();CHECK(other);
    CHECK(!xbox_heap_checkpoint_restore(other,saved,n));xbox_heap_checkpoint_destroy(other);
    HANDLE thread=CreateThread(NULL,0,wrong_thread,s,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,INFINITE)==WAIT_OBJECT_0);CloseHandle(thread);
    ++offset;CHECK(!xbox_heap_checkpoint_restore(s,saved,n));--offset;
    xbox_heap_checkpoint_invalidate();CHECK(!xbox_heap_checkpoint_restore(s,saved,n));
    memset(actual,0xA5,n);CHECK(!xbox_heap_checkpoint_capture(s,actual,n));
    for(size_t i=0;i<n;++i) CHECK(((unsigned char *)actual)[i]==0xA5);
    xbox_heap_checkpoint_destroy(s);s=xbox_heap_checkpoint_create();CHECK(s);
    CHECK(!xbox_heap_checkpoint_restore(s,saved,n));xbox_heap_checkpoint_destroy(s);
    /* Exhaust the real tracker, even though there is still physical capacity.
     * Untracked allocations must disable both capture and older restores. */
    s=xbox_heap_checkpoint_create();CHECK(s);CHECK(xbox_heap_checkpoint_capture(s,saved,n));
    for(unsigned i=0;i<HEAP_LIVE_CAP && !g_heap_untracked;++i) CHECK(xbox_HeapAlloc(64,16));
    CHECK(g_heap_untracked && !xbox_heap_checkpoint_create());
    CHECK(!xbox_heap_checkpoint_capture(s,actual,n) && !xbox_heap_checkpoint_restore(s,saved,n));
    xbox_heap_checkpoint_destroy(s);free(saved);free(expected);free(actual);free(bad);free(before);free(result);guest_memory_fixture_free(ram);
    puts("Heap checkpoint: identical addresses, metadata and RAM across three replays; invalid spans/hash chains, stale mapping/session, wrong thread and tracker exhaustion rejected.");return 0;
}
