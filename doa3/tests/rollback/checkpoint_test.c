#include "rollback_checkpoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
typedef struct World World;
typedef struct { World *world;unsigned id,value; } Part;
struct World {
    Part parts[DOA3_CP_PARTS];
    uint64_t epoch;
    int held,deny,fail_capture,fail_validate,fail_restore,fail_recovery,epoch_restore,deny_restore;
    unsigned restores,releases;
    unsigned resource_value,live_resources;
    int fail_resource_capture,fail_resource_validate,fail_resource_restore,fail_resource_recovery;
    Doa3CheckpointSession *session;
};
static int acquire(void *v) { World *w=v;CHECK(!w->held);if(w->deny) return 0;w->held=1;return 1; }
static void release(void *v) { World *w=v;CHECK(w->held);w->held=0;++w->releases; }
static uint64_t epoch(void *v) { return ((World *)v)->epoch; }
static int capture(void *v,void *out,size_t n) {
    Part *p=v;World *w=p->world;CHECK(w->held && n==4);
    /* Reentrant attempts must fail without acquiring or overwriting scratch. */
    unsigned char nested[512];size_t nested_n=doa3_checkpoint_size(w->session);CHECK(nested_n<=sizeof(nested));
    CHECK(!doa3_checkpoint_capture(w->session,99,nested,nested_n));
    CHECK(!doa3_checkpoint_restore(w->session,nested,nested_n,NULL));
    CHECK(!doa3_checkpoint_image_capture(w->session,99));
    if(w->fail_capture==(int)p->id) return 0;
    memcpy(out,&p->value,n);return 1;
}
static int validate(void *v,const void *saved,size_t n) {
    Part *p=v;CHECK(p->world->held && saved && n==4);
    return p->world->fail_validate!=(int)p->id;
}
static int can_restore(void *v) {
    Part *p=v;CHECK(p->world->held);return p->world->deny_restore!=(int)p->id;
}
static int restore(void *v,const void *saved,size_t n) {
    Part *p=v;World *w=p->world;CHECK(w->held && n==4);++w->restores;
    memcpy(&p->value,saved,n); // deliberately mutate before reporting failure
    if(w->epoch_restore==(int)p->id) { ++w->epoch;w->epoch_restore=-1; }
    if(w->fail_restore==(int)p->id) { w->fail_restore=-1;return 0; }
    if(w->fail_recovery==(int)p->id) return -1;
    return 1;
}
typedef struct { unsigned value;uint64_t epoch; } Resource;
static void *resource_capture(void *v) {
    World *w=v;CHECK(w->held);
    if(w->fail_resource_capture) return NULL;
    Resource *r=malloc(sizeof(*r));CHECK(r);r->value=w->resource_value;r->epoch=w->epoch;
    ++w->live_resources;return r;
}
static int resource_validate(void *v,const void *saved) {
    World *w=v;const Resource *r=saved;CHECK(w->held);
    return !w->fail_resource_validate && r->epoch==w->epoch;
}
static int resource_restore(void *v,const void *saved) {
    World *w=v;const Resource *r=saved;CHECK(w->held);w->resource_value=r->value;
    if(w->fail_resource_restore) { w->fail_resource_restore=0;return 0; }
    return w->fail_resource_recovery ? -1 : 1;
}
static void resource_destroy(void *v,void *saved) {
    World *w=v;CHECK(w->held && w->live_resources);--w->live_resources;free(saved);
}
static int resource_equal(void *v,const void *a,const void *b){
    CHECK(((World *)v)->held);return ((const Resource *)a)->value==((const Resource *)b)->value;
}
static Doa3CheckpointSession *setup_with_renderer(World *w,int renderer) {
    memset(w,0,sizeof(*w));w->epoch=1;
    w->fail_capture=w->fail_validate=w->fail_restore=w->fail_recovery=w->epoch_restore=w->deny_restore=-1;
    Doa3CheckpointConfig c={0};c.context=w;c.acquire=acquire;c.release=release;c.epoch=epoch;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        w->parts[i].world=w;w->parts[i].id=i;w->parts[i].value=10+i;
        c.parts[i]=(Doa3CheckpointPart){&w->parts[i],4,capture,validate,restore,can_restore};
    }
    if(renderer) c.renderer=(Doa3CheckpointResource){w,resource_capture,resource_validate,resource_restore,resource_destroy,resource_equal};
    w->session=doa3_checkpoint_create(&c);CHECK(w->session);return w->session;
}
static Doa3CheckpointSession *setup(World *w) { return setup_with_renderer(w,0); }
static void values(World *w,unsigned base) { for(unsigned i=0;i<DOA3_CP_PARTS;++i) w->parts[i].value=base+i; }
static void check_values(World *w,unsigned base) { for(unsigned i=0;i<DOA3_CP_PARTS;++i) CHECK(w->parts[i].value==base+i);CHECK(!w->held); }
int main(void) {
    World w;Doa3CheckpointSession *s=setup(&w);size_t n=doa3_checkpoint_size(s);
    unsigned char *saved=malloc(n),*bad=malloc(n),*out=malloc(n);CHECK(saved && bad && out);
    uint64_t frame=999;
    CHECK(doa3_checkpoint_capture(s,7,saved,n));values(&w,100);
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        w.deny_restore=(int)i;unsigned old_restores=w.restores;
        CHECK(doa3_checkpoint_capture(s,8,out,n));
        CHECK(!doa3_checkpoint_restore(s,saved,n,&frame) && frame==999);
        CHECK(w.restores==old_restores && !doa3_checkpoint_poisoned(s));check_values(&w,100);w.deny_restore=-1;
        memset(out,0xA5,n);w.fail_capture=(int)i;
        CHECK(!doa3_checkpoint_capture(s,8,out,n));for(size_t j=0;j<n;++j) CHECK(out[j]==0xA5);
        CHECK(!doa3_checkpoint_restore(s,saved,n,&frame));CHECK(frame==999);check_values(&w,100);
        w.fail_capture=-1;w.fail_validate=(int)i;
        CHECK(!doa3_checkpoint_restore(s,saved,n,&frame));check_values(&w,100);w.fail_validate=-1;
        w.fail_restore=(int)i;CHECK(!doa3_checkpoint_restore(s,saved,n,&frame));
        CHECK(!doa3_checkpoint_poisoned(s) && frame==999);check_values(&w,100);
    }
    unsigned calls=w.restores;
    /* Every bit, including the checksum, header and alignment padding, must
     * reject before any restore callback. */
    for(size_t i=0;i<n;++i) for(unsigned bit=0;bit<8;++bit) {
        memcpy(bad,saved,n);bad[i]^=(unsigned char)(1u<<bit);
        CHECK(!doa3_checkpoint_restore(s,bad,n,&frame));CHECK(w.restores==calls);check_values(&w,100);
    }
    CHECK(!doa3_checkpoint_restore(s,saved,n-1,&frame));
    ++w.epoch;CHECK(!doa3_checkpoint_restore(s,saved,n,&frame));--w.epoch;
    w.deny=1;CHECK(!doa3_checkpoint_restore(s,saved,n,&frame));w.deny=0;
    CHECK(doa3_checkpoint_restore(s,saved,n,&frame)==1 && frame==7);check_values(&w,10);
    values(&w,100);CHECK(doa3_checkpoint_restore(s,saved,n,&frame)==1);check_values(&w,10);
    World other;Doa3CheckpointSession *second=setup(&other);
    CHECK(!doa3_checkpoint_restore(second,saved,n,NULL));doa3_checkpoint_destroy(second);
    values(&w,100);w.fail_restore=4;w.fail_recovery=2;
    CHECK(doa3_checkpoint_restore(s,saved,n,&frame)==-1 && doa3_checkpoint_poisoned(s));
    CHECK(!doa3_checkpoint_capture(s,10,out,n));CHECK(doa3_checkpoint_restore(s,saved,n,&frame)==-1);
    doa3_checkpoint_destroy(s);
    s=setup(&w);CHECK(!doa3_checkpoint_restore(s,saved,n,NULL)); // stale instance
    CHECK(doa3_checkpoint_capture(s,0,saved,n));w.epoch_restore=3;
    CHECK(doa3_checkpoint_restore(s,saved,n,NULL)==-1 && doa3_checkpoint_poisoned(s));
    doa3_checkpoint_destroy(s);free(saved);free(bad);free(out);
    s=setup_with_renderer(&w,1);w.resource_value=20;
    Doa3CheckpointImage *image=doa3_checkpoint_image_capture(s,7);CHECK(image && w.live_resources==1);
    Doa3CheckpointComparison comparison;
    CHECK(doa3_checkpoint_image_matches(s,image,&comparison)==1 && !comparison.different_parts && !comparison.renderer_different);
    unsigned restore_calls=w.restores;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i){
        w.parts[i].value^=1;
        CHECK(doa3_checkpoint_image_matches(s,image,&comparison)==0);
        CHECK(comparison.different_parts==(1u<<i) && comparison.first_byte[i]==0 && !comparison.renderer_different);
        for(unsigned j=0;j<DOA3_CP_PARTS;++j)if(i!=j)CHECK(comparison.first_byte[j]==SIZE_MAX);
        w.parts[i].value^=1;
        Doa3CheckpointComparison before=comparison;w.fail_capture=(int)i;
        CHECK(doa3_checkpoint_image_matches(s,image,&comparison)==-1 && !memcmp(&before,&comparison,sizeof before));w.fail_capture=-1;
    }
    ++w.resource_value;CHECK(doa3_checkpoint_image_matches(s,image,&comparison)==0 && !comparison.different_parts && comparison.renderer_different);--w.resource_value;
    CHECK(w.restores==restore_calls && w.live_resources==1);
    unsigned char raw[512];CHECK(n<=sizeof(raw));memset(raw,0,sizeof(raw));
    CHECK(!doa3_checkpoint_capture(s,7,raw,n)); /* raw API denied at valid size */
    CHECK(!doa3_checkpoint_restore(s,raw,n,NULL));
    second=setup_with_renderer(&other,1);
    CHECK(!doa3_checkpoint_image_restore(second,image,NULL));
    CHECK(doa3_checkpoint_image_matches(second,image,&comparison)==-1);
    CHECK(!doa3_checkpoint_image_discard(second,image));
    CHECK(w.live_resources==1 && !other.live_resources);
    CHECK(doa3_checkpoint_image_capture(second,8));
    doa3_checkpoint_destroy(second);CHECK(!other.live_resources && !other.held);
    values(&w,100);w.resource_value=200;frame=999;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        w.fail_restore=(int)i;CHECK(!doa3_checkpoint_image_restore(s,image,&frame));
        check_values(&w,100);CHECK(w.resource_value==200 && w.live_resources==1 && frame==999);
    }
    w.fail_resource_restore=1;CHECK(!doa3_checkpoint_image_restore(s,image,&frame));
    check_values(&w,100);CHECK(w.resource_value==200 && w.live_resources==1);
    w.fail_resource_capture=1;CHECK(!doa3_checkpoint_image_capture(s,8));
    CHECK(!doa3_checkpoint_image_restore(s,image,NULL));w.fail_resource_capture=0;
    w.fail_resource_validate=1;CHECK(!doa3_checkpoint_image_capture(s,8));
    CHECK(w.live_resources==1);w.fail_resource_validate=0;
    for(unsigned i=0;i<20;++i) {
        CHECK(doa3_checkpoint_image_restore(s,image,&frame)==1 && frame==7);
        CHECK(w.resource_value==20 && w.live_resources==1);check_values(&w,10);
        Doa3CheckpointImage *temporary=doa3_checkpoint_image_capture(s,i);CHECK(temporary);
        CHECK(doa3_checkpoint_image_discard(s,temporary) && w.live_resources==1);
    }
    ++w.epoch;CHECK(!doa3_checkpoint_image_restore(s,image,NULL));
    w.deny=1;CHECK(!doa3_checkpoint_image_discard(s,image));doa3_checkpoint_destroy(s);
    CHECK(w.live_resources==1);w.deny=0;
    CHECK(doa3_checkpoint_image_discard(s,image) && !w.live_resources);
    image=doa3_checkpoint_image_capture(s,9);CHECK(image);
    w.fail_resource_restore=w.fail_resource_recovery=1;
    CHECK(doa3_checkpoint_image_restore(s,image,NULL)==-1 && doa3_checkpoint_poisoned(s));
    CHECK(doa3_checkpoint_image_discard(s,image) && !w.live_resources);
    doa3_checkpoint_destroy(s);
    puts("Checkpoint transaction: every component failure compensated; corruption, stale sessions, ownership and failed compensation rejected.");
    return 0;
}
