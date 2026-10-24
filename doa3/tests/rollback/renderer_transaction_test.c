/* Actual GPU draws/uploads and production checkpoint adapters. Nine byte
 * domains use fixture data here; the full CPU/audio harness is separate. */
#include "renderer_draw_fixture.h"
ptrdiff_t g_xbox_mem_offset;
#define RAM_BYTES 8192
static unsigned char ram[RAM_BYTES];
typedef struct World World;
typedef struct { World *w;unsigned id,value; } Part;
struct World {
    DWORD owner;int held,fail_part,fail_renderer,fail_compensation,skip_invalidation;
    uint64_t epoch;
    unsigned live_images,invalidations;
    Part part[DOA3_CP_PARTS];
    Doa3CheckpointResource renderer;
};
static int acquire(void *v) { World *w=v;if(w->owner!=GetCurrentThreadId() || w->held)return 0;w->held=1;return 1; }
static void release(void *v) { World *w=v;CHECK(w->held);w->held=0; }
static uint64_t epoch(void *v) { return ((World *)v)->epoch; }
static int capture_part(void *v,void *out,size_t n) {
    Part *p=v;CHECK(p->w->held);
    memcpy(out,p->id==DOA3_CP_RAM ? (void *)ram : (void *)&p->value,n);return 1;
}
static int validate_part(void *v,const void *saved,size_t n) {
    Part *p=v;CHECK(p->w->held);return saved && n==(p->id==DOA3_CP_RAM ? RAM_BYTES : sizeof(p->value));
}
static int restore_part(void *v,const void *saved,size_t n) {
    Part *p=v;CHECK(validate_part(v,saved,n));
    memcpy(p->id==DOA3_CP_RAM ? (void *)ram : (void *)&p->value,saved,n);
    if(p->w->fail_part==(int)p->id) { p->w->fail_part=-1;return 0; }return 1;
}
static void invalidate(void *v) {
    World *w=v;CHECK(w->held);++w->invalidations;
    if(!w->skip_invalidation) pgraph_d3d11_invalidate_guest_uploads();
}
static void *capture_renderer(void *v) {
    World *w=v;CHECK(w->held);void *p=w->renderer.capture(w->renderer.context);
    if(p)++w->live_images;return p;
}
static int validate_renderer(void *v,const void *p) {
    World *w=v;CHECK(w->held);return w->renderer.validate(w->renderer.context,p);
}
static int restore_renderer(void *v,const void *p) {
    World *w=v;int result;CHECK(w->held);
    result=w->renderer.restore(w->renderer.context,p);if(result!=1)return result;
    if(w->fail_renderer) { w->fail_renderer=0;return 0; }
    return w->fail_compensation ? -1 : 1;
}
static void destroy_renderer(void *v,void *p) {
    World *w=v;CHECK(w->held && w->live_images);--w->live_images;
    w->renderer.destroy(w->renderer.context,p);
}
int main(void) {
    World w={0};Doa3CheckpointConfig config={0};Nv2aRendererCheckpointSource source={0};
    Doa3CheckpointSession *session;Doa3CheckpointImage *a,*b;
    unsigned i,pass;uint64_t frame=99;
    uint64_t initial[20],reference[3][20],actual[20],future[20],after_draw[20];
    unsigned char saved_ram[RAM_BYTES];
    g_xbox_mem_offset=(ptrdiff_t)ram;init_draw_fixture(ram,1024);
    w.owner=GetCurrentThreadId();w.epoch=7;w.fail_part=-1;
    source=(Nv2aRendererCheckpointSource){&g_pg,&g_pg_aux,model_view,&w,epoch,invalidate};
    w.renderer=nv2a_renderer_checkpoint_resource(&source);
    config.context=&w;config.acquire=acquire;config.release=release;config.epoch=epoch;
    for(i=0;i<DOA3_CP_PARTS;i++) {
        w.part[i]=(Part){&w,i,10+i};
        config.parts[i]=(Doa3CheckpointPart){&w.part[i],i==DOA3_CP_RAM ? RAM_BYTES : sizeof(unsigned),capture_part,validate_part,restore_part};
    }
    config.renderer=(Doa3CheckpointResource){&w,capture_renderer,validate_renderer,restore_renderer,destroy_renderer};
    session=doa3_checkpoint_create(&config);CHECK(session);
    change_input(0);draw_now();a=doa3_checkpoint_image_capture(session,0);CHECK(a);hash_all(initial);
    /* A checkpoint cannot follow a caller that changes its source descriptor. */
    float replaced_view[16]={0};source.model_view=replaced_view;
    CHECK(!doa3_checkpoint_image_restore(session,a,&frame) && frame==99);
    hash_all(actual);CHECK(!memcmp(initial,actual,sizeof initial));
    source.model_view=model_view;
    for(i=0;i<3;i++){change_input(i+1);draw_now();hash_all(reference[i]);}
    for(pass=0;pass<3;pass++) {
        CHECK(doa3_checkpoint_image_restore(session,a,&frame)==1 && frame==0);
        hash_all(actual);CHECK(!memcmp(initial,actual,sizeof initial));
        for(i=0;i<3;i++){change_input(i+1);draw_now();hash_all(actual);CHECK(!memcmp(reference[i],actual,sizeof actual));}
        CHECK(w.live_images==1);
    }
    b=doa3_checkpoint_image_capture(session,3);CHECK(b);hash_all(future);memcpy(saved_ram,ram,sizeof ram);
    draw_now();hash_all(after_draw);
    for(i=0;i<=DOA3_CP_PARTS;i++) {
        CHECK(doa3_checkpoint_image_restore(session,b,NULL)==1);
        if(i==DOA3_CP_PARTS)w.fail_renderer=1;else w.fail_part=(int)i;
        CHECK(doa3_checkpoint_image_restore(session,a,NULL)==0 && !doa3_checkpoint_poisoned(session));
        hash_all(actual);CHECK(!memcmp(future,actual,sizeof actual) && !memcmp(saved_ram,ram,sizeof ram));
        draw_now();hash_all(actual);CHECK(!memcmp(after_draw,actual,sizeof actual));CHECK(w.live_images==2);
    }
    /* Negative control: lower mip changes evade the unchanged base-level hash. */
    CHECK(doa3_checkpoint_image_restore(session,a,NULL)==1);draw_now();hash_all(after_draw);
    CHECK(doa3_checkpoint_image_restore(session,b,NULL)==1);draw_now();
    w.skip_invalidation=1;CHECK(doa3_checkpoint_image_restore(session,a,NULL)==1);draw_now();hash_all(actual);
    CHECK(memcmp(after_draw,actual,sizeof actual)!=0);
    w.skip_invalidation=0;CHECK(doa3_checkpoint_image_restore(session,a,NULL)==1);draw_now();hash_all(actual);
    CHECK(!memcmp(after_draw,actual,sizeof actual));
    g_pg.in_draw=1;CHECK(!doa3_checkpoint_image_capture(session,8));g_pg.in_draw=0;
    w.fail_renderer=w.fail_compensation=1;
    CHECK(doa3_checkpoint_image_restore(session,b,NULL)==-1 && doa3_checkpoint_poisoned(session));
    doa3_checkpoint_destroy(session);CHECK(!w.live_images && !w.held);
    close_draw_fixture();
    puts("Coordinated WARP draw replay: exact pixels/RAM, every restore failure compensated, invalidation negative control detected, resources released");
    return 0;
}
