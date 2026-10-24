/* Production completion queue; guest callbacks below are deterministic fixtures.
 * No host file is read and no whole-worker continuation is represented here. */
#include "kernel_apc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x); exit(1); } } while(0)
static XboxApcQueue queue;
static uint32_t guest[32], order[32], used;
static XboxApcSnapshot baseline;

static void deliver(int recurse)
{
    XboxApcEntry batch[XBOX_APC_CAPACITY];
    unsigned count=xbox_apc_begin(&queue,batch);
    if(!count) return;
    XboxApcSnapshot scratch;
    CHECK(!xbox_apc_capture(&queue,&scratch,sizeof(scratch)));
    CHECK(!xbox_apc_restore(&queue,&baseline,sizeof(baseline)));
    for(unsigned i=0;i<count;++i) {
        XboxApcEntry e=batch[i]; CHECK(e.iosb<32 && used<32);
        guest[e.iosb]=guest[e.iosb]*33+e.bytes;
        order[used++]=e.ctx;
        if(e.ctx==1) {
            /* Two callbacks enqueue while the outer batch still has entries.
             * The old shared-array iteration overwrote its next callback. */
            CHECK(xbox_apc_push(&queue,(XboxApcEntry){3,3,103}));
            CHECK(xbox_apc_push(&queue,(XboxApcEntry){4,4,104}));
            if(recurse) deliver(0);
            CHECK(!xbox_apc_capture(&queue,&scratch,sizeof(scratch)));
        }
    }
    xbox_apc_end(&queue);
}
static void check_rejected(XboxApcSnapshot *bad)
{
    XboxApcQueue before=queue;
    CHECK(!xbox_apc_restore(&queue,bad,sizeof(*bad)));
    CHECK(!memcmp(&before,&queue,sizeof(queue)));
}
int main(void)
{
    XboxApcSnapshot empty,final,again,bad;
    CHECK(xbox_apc_capture(&queue,&empty,sizeof(empty)) && !empty.count);
    CHECK(!xbox_apc_capture(&queue,&empty,sizeof(empty)-1));
    CHECK(xbox_apc_push(&queue,(XboxApcEntry){1,1,101}));
    CHECK(xbox_apc_push(&queue,(XboxApcEntry){2,2,102}));
    CHECK(xbox_apc_capture(&queue,&baseline,sizeof(baseline)) && baseline.count==2);
    for(int recurse=0;recurse<2;++recurse) {
        uint32_t expected_guest[32],expected_order[32];
        for(int replay=0;replay<3;++replay) {
            CHECK(xbox_apc_restore(&queue,&baseline,sizeof(baseline)));
            memset(guest,0,sizeof(guest)); memset(order,0,sizeof(order)); used=0;
            deliver(recurse);
            if(!recurse) {
                CHECK(used==2 && order[0]==1 && order[1]==2 && queue.count==2);
                XboxApcSnapshot pending;
                CHECK(xbox_apc_capture(&queue,&pending,sizeof(pending)));
                deliver(0); CHECK(used==4);
                /* Pending callbacks survive a checkpoint, in exact FIFO order. */
                CHECK(xbox_apc_restore(&queue,&pending,sizeof(pending)));
                used=2; guest[3]=guest[4]=0; order[2]=order[3]=0;
                deliver(0);
            }
            CHECK(used==4 && guest[1]==101 && guest[2]==102 && guest[3]==103 && guest[4]==104);
            CHECK(order[0]==1 && order[1]==(recurse?3u:2u) &&
                  order[2]==(recurse?4u:3u) && order[3]==(recurse?2u:4u));
            CHECK(xbox_apc_capture(&queue,&again,sizeof(again)));
            if(!replay) {
                memcpy(expected_guest,guest,sizeof(guest)); memcpy(expected_order,order,sizeof(order)); final=again;
            } else {
                CHECK(!memcmp(expected_guest,guest,sizeof(guest)));
                CHECK(!memcmp(expected_order,order,sizeof(order)));
                CHECK(!memcmp(&final,&again,sizeof(final)));
            }
        }
    }
    bad=baseline; bad.version++; check_rejected(&bad);
    bad=baseline; bad.count=XBOX_APC_CAPACITY+1; check_rejected(&bad);
    bad=baseline; bad.owner++; check_rejected(&bad);
    bad=baseline; bad.epoch++; check_rejected(&bad);
    bad=baseline; bad.entries[1].ctx=0; check_rejected(&bad);
    bad=baseline; bad.entries[1].iosb=0; check_rejected(&bad);
    bad=baseline; bad.entries[2].bytes=1; check_rejected(&bad);
    XboxApcQueue other={0};
    CHECK(!xbox_apc_restore(&other,&baseline,sizeof(baseline)));
    CHECK(xbox_apc_restore(&queue,&empty,sizeof(empty)));
    for(unsigned i=0;i<XBOX_APC_CAPACITY;++i) CHECK(xbox_apc_push(&queue,(XboxApcEntry){i+10,i+1,i}));
    CHECK(xbox_apc_capture(&queue,&again,sizeof(again)));
    CHECK(!xbox_apc_push(&queue,(XboxApcEntry){100,1,0}));
    CHECK(!xbox_apc_capture(&queue,&again,sizeof(again)));
    check_rejected(&empty); // overflow cannot be hidden by restoring a previous queue
    xbox_apc_reset(&queue);
    CHECK(!queue.tainted && !queue.count);
    check_rejected(&empty); // reset invalidates older snapshots
    CHECK(xbox_apc_capture(&queue,&again,sizeof(again)));
    CHECK(!xbox_apc_push(&queue,(XboxApcEntry){0,1,0}));
    CHECK(!xbox_apc_capture(&queue,&again,sizeof(again)));
    puts("Completion FIFO, nested delivery, repeated replay, in-flight guards and atomic malformed-state rejection pass.");
    return 0;
}
