#include "renderer_draw_fixture.h"
#include "checkpoint_renderer.h"
static Nv2aRendererCheckpointSource source;
static Doa3CheckpointResource resource;
static int *fence,fail_restore;
static unsigned live_images;
static void invalidate(void *v) {
    (void)v;CHECK(*fence);pgraph_d3d11_invalidate_guest_uploads();
}
static void *capture_renderer(void *v) {
    (void)v;CHECK(*fence);void *p=resource.capture(resource.context);
    if(p)++live_images;return p;
}
static int validate_renderer(void *v,const void *p) {
    (void)v;CHECK(*fence);return resource.validate(resource.context,p);
}
static int restore_renderer(void *v,const void *p) {
    (void)v;CHECK(*fence);int result=resource.restore(resource.context,p);
    if(result==1 && fail_restore){fail_restore=0;return 0;}return result;
}
static void destroy_renderer(void *v,void *p) {
    (void)v;CHECK(*fence && live_images);--live_images;
    resource.destroy(resource.context,p);
}
static int equal_renderer(void *v,const void *a,const void *b){
    (void)v;CHECK(*fence);return resource.equal(resource.context,a,b);
}
Doa3CheckpointResource fixture_renderer_init(unsigned char *ram,uint32_t offset,
    int *held,void *context,uint64_t (*epoch)(void *)) {
    fence=held;init_draw_fixture(ram,offset);
    source=(Nv2aRendererCheckpointSource){&g_pg,&g_pg_aux,model_view,context,epoch,invalidate};
    resource=nv2a_renderer_checkpoint_resource(&source);
    return (Doa3CheckpointResource){NULL,capture_renderer,validate_renderer,restore_renderer,destroy_renderer,equal_renderer};
}
void fixture_renderer_advance(unsigned frame,uint32_t task_value) {
    CHECK(!*fence);
    /* Depend on the restored CPU task state, and on the preceding GPU frame.
     * Each upload changes lower mip bytes while retaining the base level. */
    change_input((frame+task_value)%8);draw_now();
}
void fixture_renderer_observe(uint64_t hashes[20]) { CHECK(*fence);hash_all(hashes); }
void fixture_renderer_corrupt(void) { CHECK(!*fence);fill(cur_rtv(),cur_dsv(),.1f,99); }
void fixture_renderer_fail_restore(void) { CHECK(!*fence);fail_restore=1; }
unsigned fixture_renderer_images(void) { return live_images; }
void fixture_renderer_shutdown(void) { CHECK(!live_images && !*fence);close_draw_fixture(); }
