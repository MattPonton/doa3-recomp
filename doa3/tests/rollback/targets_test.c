#include "nv2a_queue.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static uint32_t mem[0x20000/4];
static uint32_t read32(void *o,uint32_t a) { (void)o; CHECK(a<sizeof(mem) && !(a&3)); return mem[a/4]; }
#define PUT(a,v) (mem[(a)/4]=(v))
int main(void)
{
    const uint32_t dev=0x10000, surface=0x4000, display=0x5000;
    Nv2aQueueState q={0}, saved;
    Nv2aQueueSnapshot snap,bad;
    PUT(dev+0x2070,surface); PUT(surface+4,0x84006000);
    CHECK(nv2a_targets_update(&q.targets,dev,0,read32,NULL)==surface);
    CHECK(q.targets.count==1 && q.targets.offsets[0]==0x6000);
    CHECK(!nv2a_targets_update(&q.targets,dev,surface,read32,NULL));
    PUT(display+4,0x6000); PUT(dev+0x207C,display);
    /* Reallocated display offsets remove old reflection entries, including
     * duplicates. A distinct requested reflection can then re-add its offset. */
    q.targets.count=3; q.targets.offsets[1]=0x6000; q.targets.offsets[2]=0x7000;
    CHECK(!nv2a_targets_update(&q.targets,dev,display,read32,NULL));
    CHECK(q.targets.count==1 && q.targets.offsets[0]==0x7000);
    CHECK(nv2a_targets_update(&q.targets,dev,surface,read32,NULL)==surface);
    CHECK(q.targets.count==2 && q.targets.offsets[1]==0x6000);
    saved=q; CHECK(nv2a_queue_export(&q,&snap,sizeof(snap)));
    CHECK(sizeof(snap)==33344 && snap.schema==4);
    PUT(dev+0x207C,0); PUT(surface+4,0);
    CHECK(!nv2a_targets_update(&q.targets,dev,surface,read32,NULL));
    CHECK(!nv2a_targets_update(&q.targets,0,surface,read32,NULL));
    CHECK(!nv2a_targets_update(&q.targets,dev,0x8000000,read32,NULL));
    CHECK(!memcmp(&q,&saved,sizeof(q)));
    for(unsigned i=0;i<64;++i) q.targets.offsets[i]=i+1;
    q.targets.count=64; PUT(surface+4,0x9000);
    CHECK(!nv2a_targets_update(&q.targets,dev,surface,read32,NULL) && q.targets.count==64);
    CHECK(nv2a_queue_import(&q,&snap,sizeof(snap)) && !memcmp(&q,&saved,sizeof(q)));
    for(unsigned i=0;i<3;++i) {
        bad=snap;
        if(i==0) bad.queue.targets.count=65;
        if(i==1) bad.queue.targets.reserved=1;
        if(i==2) bad.schema=3;
        CHECK(!nv2a_queue_import(&q,&bad,sizeof(bad)) && !memcmp(&q,&saved,sizeof(q)));
    }
    puts("Target routing: defaults, aliases, duplicate removal, capacity and snapshot restore verified.");
}
