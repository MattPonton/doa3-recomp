/* Full production PGRAPH translator and D3D draw/shader/state/upload code.
 * Only window/UI/game services are replaced. Commands are synthetic, not a
 * captured fight. Guest RAM is held fixed; coordinated RAM tests are separate. */
#define DOA3_FULL_PIPELINE_FIXTURE
/* Only intercept the device fixture's new pixel-only call. The production
 * shader translation unit still calls the real function. This lets identical
 * draws compare the old full setup with the optimized setup on a real GPU. */
#include "d3d8_internal.h"
static void test_prepare_pixel(void);
#define d3d8_shaders_prepare_pixel test_prepare_pixel
#include "render_fixture.h"
#undef d3d8_shaders_prepare_pixel
static int fail_mesh_create;
static Nv2aFFMesh *test_mesh_create(const Nv2aFFVertex *v,unsigned nv,const uint16_t *i,unsigned ni)
{ return fail_mesh_create ? NULL:d3d8_Nv2aFF_MeshCreate(v,nv,i,ni); }
#define d3d8_Nv2aFF_MeshCreate test_mesh_create
#include "../../src/nv2a/nv2a_pgraph_d3d11.c"
#undef d3d8_Nv2aFF_MeshCreate
#include "nv2a/nv2a_renderer_checkpoint.h"
#include "nv2a/nv2a_submission.h"

static int reference_vertex_setup;
static unsigned reference_setups, pixel_setups;
static void test_prepare_pixel(void)
{
    if(reference_vertex_setup) {
        ++reference_setups;
        d3d8_shaders_prepare_draw(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    } else {
        ++pixel_setups;
        d3d8_shaders_prepare_pixel();
    }
}

ptrdiff_t g_xbox_mem_offset;
TXD_Dict g_global_txd;
int g_textures_loaded;
Nv2aTargetsState g_doa3_targets;
volatile int g_doa3_post_movie;
volatile LONG g_doa3_heartbeat;
FILE *g_doa3_mtrace;
unsigned g_doa3_frames_presented,g_doa3_frame_at_draw30,g_doa3_frame_at_draw400;
IDirect3DTexture8 *txd_find(const TXD_Dict *d,const char *n) { (void)d;(void)n;return NULL; }
IDirect3DDevice8 *xbox_GetD3DDevice(void) { return &g_device; }
void d3d8_DebugDumpTargetState(void) {}
void d3d8_debug_views_apply(void) {}
void d3d8_debug_views_shutdown(void) {}
int debug_hide_ui_get(void) { return 0; }
int debug_hide_player_vertices(uint32_t a) { (void)a;return 0; }
int video_get_aspect(void) { return VIDEO_ASPECT_16_9; }
void video_hud_region(unsigned w,float *x,float *width) { *x=0;*width=(float)w; }
int doa3_guest_display_size(unsigned *w,unsigned *h) { *w=*h=64;return 1; }
uint32_t doa3_aa_resolve_source(void) { return 0; }
int doa3_movie_drop_guest_draws(void) { return 0; }
int doa3_movie_host_owns_screen(void) { return 0; }
void doa3_capture_backbuffer(const char *p) { (void)p;CHECK(0); }
static uint64_t epoch(void *v) { (void)v;return 12; }
static void invalidate(void *v) { (void)v;pgraph_d3d11_invalidate_guest_uploads(); }
static uint32_t bits(float x) { uint32_t n;memcpy(&n,&x,4);return n; }
static void method(uint32_t m,uint32_t p) {
    int handled=pgraph_d3d11_method(0,m,p);
    if(!handled)fprintf(stderr,"unhandled method %04X\n",m);
    CHECK(handled);
}
static unsigned bulk_packets,scalar_words;
static uint32_t packet_read(void *v,uint32_t addr) {
    (void)v;CHECK(addr>=0x4000 && addr<0x4100);
    uint32_t word;memcpy(&word,(void *)(g_xbox_mem_offset+addr),4);return word;
}
static int packet_method(void *v,uint32_t sub,uint32_t m,uint32_t value) {
    (void)v;++scalar_words;CHECK(pgraph_d3d11_method(sub,m,value));return 1;
}
static int packet_bulk(void *v,uint32_t sub,uint32_t m,uint32_t address,uint32_t count) {
    (void)v;int result=pgraph_d3d11_data_packet(sub,m,(const uint32_t *)(g_xbox_mem_offset+address),count);
    bulk_packets+=result!=0;return result;
}
static void payload(uint32_t m,const uint32_t *words,unsigned count) {
    CHECK(count<=32);
    uint32_t header=0x40000000u|(count<<18)|m;
    memcpy((void *)(g_xbox_mem_offset+0x4000),&header,4);
    memcpy((void *)(g_xbox_mem_offset+0x4004),words,count*4);
    Nv2aSubmissionState s=NV2A_SUBMISSION_INITIAL;s.tracing=0;
    Nv2aSubmissionOps ops={packet_read,NULL,packet_method,NULL,NULL,1,packet_bulk};
    CHECK(nv2a_submission_span(&s,&ops,0x4000,0x4004+count*4)==1);
}

static void triangle(float x,uint32_t color)
{
    const float xy[3][2]={{8,8},{52,8},{8,52}};
    unsigned i;DWORD before=g_d3d_draw_count;
    method(NV097_SET_BEGIN_END,5);
    for(i=0;i<3;i++) {
        uint32_t v[7]={bits(xy[i][0]+x),bits(xy[i][1]),bits(.4f),bits(1),color,bits(i==1 ? 1.0f:0),bits(i==2 ? 1.0f:0)};
        payload(NV097_INLINE_ARRAY,v,7);
    }
    method(NV097_SET_BEGIN_END,0);
    CHECK(g_d3d_draw_count==before+!skip_replay_preparation);
}
static unsigned geometry_primitive=5;
static void scene(unsigned n)
{
    /* Actual inline submit -> texture binding -> D3D8 UP -> shaders -> Draw. */
    d3d8_RestoreDefaultTarget();
    fill(cur_rtv(),cur_dsv(),.95f,0);
    method(NV097_SET_TEXTURE_OFFSET,0x1000);
    triangle((float)n,0xC0FFFFFFu-(n<<16));
    CHECK(d3d8_SetOffscreenTarget(0x8000,64,64));
    fill(cur_rtv(),cur_dsv(),.95f,0);
    triangle((float)n,0xA0C0FFFFu);
    d3d8_RestoreDefaultTarget();
    method(NV097_SET_TEXTURE_OFFSET,0x8000);
    triangle(3,0x80FFFFFFu);
    /* Indexed object-space batch through the real GPU fixed-function shader
     * and register-combiner shader, including both dynamic upload rings. */
    method(NV097_SET_TEXTURE_OFFSET,0x1000);
    /* These methods update the raw register shadow but return unhandled. */
    pgraph_d3d11_method(0,0x0288,4);pgraph_d3d11_method(0,0x028C,0x1400);
    CHECK(NVREG(0x0288)==4 && NVREG(0x028C)==0x1400); /* final RGB/alpha V0 */
    DWORD draws=g_d3d_draw_count;uint32_t gpu_draws=g_gpuff_stat[0];
    uint64_t before_indexed=pixels(g_device_state.default_tex);
    method(NV097_SET_BEGIN_END,geometry_primitive);
    const uint32_t indices[]={0,1,2,0,1,2};
    const uint32_t packed[]={0x00010000,0x00000002,0x00020001,0x00010000,0x00000002,0x00020001};
    payload(n&1 ? 0x1808:0x1800,n&1 ? indices:packed,6);
    method(NV097_SET_BEGIN_END,0);
    CHECK(g_d3d_draw_count==draws+!skip_replay_preparation && g_gpuff_stat[0]==gpu_draws+!skip_replay_preparation);
    CHECK((pixels(g_device_state.default_tex)!=before_indexed)==(!g_replay_suppress_draws && !skip_replay_preparation));
    CHECK(d3d8_BindFramebufferCopy(2));d3d8_keep_prev_frame();
}
static void hostile_future(void)
{
    DWORD code[4]={0,0,0,1},handle;
    float c[4]={.8f,.3f,.2f,1};
    CHECK(SUCCEEDED(d3d8_vsh_create_shader(code,1,&handle)));
    d3d8_vsh_set_constant(5,c,1);
    memset(g_device_state.transforms,0x3F,sizeof(g_device_state.transforms));
    memset(&g_device_state.material,0x40,sizeof(g_device_state.material));
    memset(g_device_state.lights,0x41,sizeof(g_device_state.lights));
    memset(g_device_state.light_enable,0,sizeof(g_device_state.light_enable));
    dev_SetRenderState(&g_device,D3DRS_FILLMODE,D3DFILL_WIREFRAME);
    dev_SetRenderState(&g_device,D3DRS_BLENDOP,2 /* SUBTRACT */);
    dev_SetPixelShader(&g_device,0x1234);
    dev_SetVertexShader(&g_device,handle);
    d3d8_SetScissorRect(2,3,7,9);
    d3d8_SetBlendFactor(.2f,.3f,.4f,.5f);
    d3d8_SetPolygonOffset(1,1,2,0,1);
    NV2ACombinerState direct={0};d3d8_combiners_set_direct(&direct);
    ID3D11DeviceContext_ClearState(g_device_state.d3d11_context);
    /* Restore target only, retaining deliberately poisoned logical state. */
    d3d8_RestoreDefaultTarget();
}
static void geometry_bounds_test(void)
{
    Nv2aPgraphDrawState *old=malloc(sizeof(g_pg));CHECK(old);*old=g_pg;
    NvGeometryKey key;NvGeometrySpan spans[5];unsigned ns;
    const unsigned slots[]={0,2,3,9,10};
    for(unsigned i=0;i<5;i++){g_pg.attr_off[slots[i]]=0x3000+16*i;g_pg.attr_fmt[slots[i]]=(80<<8)|0x42;}
    g_pg.idx_count=3;g_pg.idx[0]=0;g_pg.idx[1]=1;g_pg.idx[2]=2;
    CHECK(nv_geometry_inputs(&key,spans,&ns,31,4,0) && ns==1 && spans[0].address==0x3000 && spans[0].size==240);
    g_pg.idx[2]=UINT32_MAX;CHECK(!nv_geometry_inputs(&key,spans,&ns,31,4,0));
    g_pg.idx[2]=2;g_pg.attr_off[0]=0x07FFFFF0;CHECK(!nv_geometry_inputs(&key,spans,&ns,31,4,0));
    g_pg.attr_off[0]=0x3000;g_pg.idx[2]=60000;CHECK(!nv_geometry_inputs(&key,spans,&ns,31,4,0));
    g_pg.idx[2]=2;g_pg.attr_fmt[0]=0x42; /* constant attribute, zero stride */
    CHECK(nv_geometry_inputs(&key,spans,&ns,1,4,0) && ns==1 && spans[0].size==16);
    *(&g_pg)=*old;free(old);
}
static void geometry_mutation_test(Doa3CheckpointResource *resource,void *saved,unsigned char *ram)
{
    unsigned char original[0x200];memcpy(original,ram+0x2000,sizeof(original));
    uint64_t expected[20],actual[20];
    for(unsigned variant=0;variant<12;variant++){
        /* Changed positions, colors, topology, and per-draw camera constants.
         * Restore can revisit old source bytes: resident buffers must remain
         * a derived cache, never part of logical state. */
        memcpy(ram+0x2000,original,sizeof(original));
        ((float *)(ram+0x2000))[0]+=(float)(variant%3);
        ((uint32_t *)(ram+0x2100))[0]^=(variant%4)<<12;
        geometry_primitive=5+variant%4;
        pgraph_d3d11_force_geometry_rebuild(1);
        CHECK(resource->restore(resource->context,saved)==1);
        g_pg.composite[3]+=(float)(variant%2);
        scene(1);hash_all(expected);
        pgraph_d3d11_force_geometry_rebuild(0);
        if(variant==9){nv_geometry_clear(&g_geometry);fail_mesh_create=1;}
        for(unsigned repeat=0;repeat<2;repeat++){
            CHECK(resource->restore(resource->context,saved)==1);
            g_pg.composite[3]+=(float)(variant%2);
            scene(1);hash_all(actual);CHECK(!memcmp(expected,actual,sizeof(actual)));
        }
        fail_mesh_create=0;
    }
    memcpy(ram+0x2000,original,sizeof(original));geometry_primitive=5;
    CHECK(resource->restore(resource->context,saved)==1);
}
static DWORD WINAPI probe_wrong_thread(void *arg)
{
    D3D8BoundaryProbe p;
    d3d8_pipeline_probe(*(DWORD *)arg,&p);
    return p.flags==D3D8_PROBE_OWNER && !p.identity && !p.image_count ? 0:1;
}
static void boundary_probe_test(void)
{
    D3D8BoundaryProbe a,b;
    DWORD owner=GetCurrentThreadId(),exit_code=1;
    uint64_t before[20],after[20];
    HANDLE thread;
    hash_all(before);
    d3d8_pipeline_probe(owner,&a);
    CHECK(!a.flags && a.image_count==6 && a.image_bytes==6u*64*64*4);
    thread=CreateThread(NULL,0,probe_wrong_thread,&owner,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,10000)==WAIT_OBJECT_0);
    CHECK(GetExitCodeThread(thread,&exit_code) && !exit_code);CloseHandle(thread);
    g_offrt[0].key^=1;d3d8_pipeline_probe(owner,&b);CHECK(b.identity!=a.identity);g_offrt[0].key^=1;
    ID3D11DeviceContext_OMSetRenderTargets(g_device_state.d3d11_context,0,NULL,NULL);
    d3d8_pipeline_probe(owner,&b);CHECK(b.flags&D3D8_PROBE_BINDINGS);
    d3d8_RestoreDefaultTarget();
    ID3D11DeviceContext_RSSetViewports(g_device_state.d3d11_context,0,NULL);
    d3d8_pipeline_probe(owner,&b);CHECK(b.flags&D3D8_PROBE_VIEWPORT);
    d3d8_RestoreDefaultTarget();
    ID3D11Texture2D *depth=g_offrt[0].depth;g_offrt[0].depth=NULL;
    d3d8_pipeline_probe(owner,&b);CHECK(b.flags&D3D8_PROBE_INVENTORY);g_offrt[0].depth=depth;
    CHECK(!_putenv_s("DOA3_ROLLBACK_RENDER_AUDIT","boundary-audit-test.csv"));
    for (unsigned i=0;i<125;i++) pgraph_d3d11_rollback_audit_boundary(1,0);
    CHECK(!g_boundary_audit.count && !g_boundary_audit.warmup);
    for (unsigned i=0;i<240;i++) {
        if(i==120)g_pg.in_draw=1;
        pgraph_d3d11_rollback_audit_boundary(1,1);
        g_pg.in_draw=0;
        pgraph_d3d11_rollback_audit_boundary(0,1);
    }
    CHECK(!pgraph_d3d11_rollback_audit_enabled() && g_boundary_audit.count==240);
    CHECK(g_boundary_audit.samples[0].flags==D3D8_PROBE_BATCH);
    for (unsigned i=1;i<240;i++) CHECK(!g_boundary_audit.samples[i].flags);
    FILE *f=fopen("boundary-audit-test.csv","r");CHECK(f);
    char line[256];unsigned lines=0;while(fgets(line,sizeof(line),f))++lines;
    CHECK(lines==241 && !ferror(f));fclose(f);CHECK(!remove("boundary-audit-test.csv"));
    hash_all(after);CHECK(!memcmp(before,after,sizeof(before)));
    /* Leaving the eligible mode terminates a partial audit. */
    memset(&g_boundary_audit,0,sizeof(g_boundary_audit));g_boundary_audit.warmup=120;
    pgraph_d3d11_rollback_audit_boundary(1,1);
    pgraph_d3d11_rollback_audit_boundary(0,0);
    CHECK(!pgraph_d3d11_rollback_audit_enabled() && g_boundary_audit.count==1);
    CHECK(!remove("boundary-audit-test.csv"));
    CHECK(!_putenv_s("DOA3_ROLLBACK_RENDER_AUDIT",""));
}
static DWORD WINAPI wrong_snapshot_thread(void *unused)
{
    Nv2aRendererCheckpointSource source;
    (void)unused;
    CHECK(!pgraph_d3d11_rollback_snapshot_source(&source));
    return 0;
}
static void live_snapshot_test(void)
{
    Nv2aRendererCheckpointSource source;
    uint64_t before[20],after[20];
    CHECK(pgraph_d3d11_rollback_snapshot_source(&source));
    Doa3CheckpointResource resource=nv2a_renderer_checkpoint_resource(&source);
    hash_all(before);
    void *a=resource.capture(resource.context),*b=resource.capture(resource.context);
    CHECK(a && b && nv2a_renderer_checkpoint_equal(&source,a,b));
    hash_all(after);CHECK(!memcmp(before,after,sizeof(before)));
    float previous_view=g_mv[0];g_mv[0]+=1.0f;
    void *changed=resource.capture(resource.context);CHECK(changed);
    CHECK(!nv2a_renderer_checkpoint_equal(&source,a,changed));
    g_mv[0]=previous_view;
    resource.destroy(resource.context,changed);
    DWORD previous=g_device_state.render_states[D3DRS_FILLMODE];
    g_device_state.render_states[D3DRS_FILLMODE]=D3DFILL_WIREFRAME;
    changed=resource.capture(resource.context);CHECK(changed);
    CHECK(!nv2a_renderer_checkpoint_equal(&source,a,changed));
    g_device_state.render_states[D3DRS_FILLMODE]=previous;
    resource.destroy(resource.context,changed);
    HANDLE thread=CreateThread(NULL,0,wrong_snapshot_thread,NULL,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,10000)==WAIT_OBJECT_0);CloseHandle(thread);
    g_pg.in_draw=1;
    Nv2aRendererCheckpointSource refused;
    CHECK(!pgraph_d3d11_rollback_snapshot_source(&refused));
    CHECK(!nv2a_renderer_checkpoint_equal(&source,a,b));
    g_pg.in_draw=0;
    ++g_pg_checkpoint_generation;
    CHECK(!nv2a_renderer_checkpoint_equal(&source,a,b));
    resource.destroy(resource.context,b);resource.destroy(resource.context,a);
}
int main(void)
{
    IDirect3DDevice8Vtbl vt={0};unsigned i,pass;
    uint64_t clear_hash,reference[3][20],actual[20];
    D3D8PipelineImage *before=calloc(1,sizeof(*before)),*after=calloc(1,sizeof(*after));
    unsigned char *ram=calloc(1,128u*1024*1024);CHECK(before && after && ram);
    SetEnvironmentVariableA("DOA3_AUTO_CAPTURE",NULL);
    g_xbox_mem_offset=(ptrdiff_t)ram;
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,
        &g_device_state.d3d11_device,NULL,&g_device_state.d3d11_context)));
    CHECK(d3d8_SetOffscreenTarget(0x100,64,64));
    g_device_state.default_tex=g_offrt[0].tex;g_device_state.default_depth=g_offrt[0].depth;
    g_device_state.default_rtv=g_offrt[0].rtv;g_device_state.default_dsv=g_offrt[0].dsv;
    memset(&g_offrt[0],0,sizeof(g_offrt[0]));g_device_state.width=g_device_state.height=64;
    d3d8_init_default_states(&g_device_state);d3d8_RestoreDefaultTarget();d3d8_ResetScissorRect();
    fill(cur_rtv(),cur_dsv(),.95f,0);clear_hash=pixels(g_device_state.default_tex);
    CHECK(d3d8_BindFramebufferCopy(2));d3d8_keep_prev_frame();
    CHECK(d3d8_SetOffscreenTarget(0x8000,64,64));fill(cur_rtv(),cur_dsv(),.95f,0);d3d8_RestoreDefaultTarget();
    CHECK(SUCCEEDED(d3d8_shaders_init()) && SUCCEEDED(d3d8_states_init()));
    CHECK(SUCCEEDED(d3d8_combiners_init()) && SUCCEEDED(d3d8_vsh_init()));
    CHECK(SUCCEEDED(d3d8_Nv2aFF_Init()));
#define METHOD(name) vt.name=dev_##name
    METHOD(SetRenderState);METHOD(SetTextureStageState);METHOD(SetTexture);METHOD(SetVertexShader);
    METHOD(GetVertexShader);METHOD(CreateTexture);METHOD(DrawPrimitiveUP);METHOD(Clear);
    METHOD(DrawIndexedPrimitiveUP);METHOD(SetTransform);METHOD(SetViewport);
    METHOD(BeginScene);METHOD(EndScene);
#undef METHOD
    g_device.lpVtbl=&vt;pgraph_d3d11_init();pgraph_d3d11_set_vertex_layout(7,4,5,4);
    method(NV097_SET_SURFACE_CLIP_HORIZONTAL,64u<<16);method(NV097_SET_SURFACE_CLIP_VERTICAL,64u<<16);
    method(NV097_SET_DEPTH_TEST_ENABLE,1);method(NV097_SET_BLEND_ENABLE,1);
    method(NV097_SET_BLEND_FUNC_SFACTOR,0x0302);method(NV097_SET_BLEND_FUNC_DFACTOR,0x0303);
    method(NV097_SET_TEXTURE_FORMAT,(3u<<24)|(3u<<20)|(1u<<16)|(6u<<8));
    method(NV097_SET_TEXTURE_CONTROL0,1u<<30);
    for(i=0;i<64;i++) ((uint32_t *)(ram+0x1000))[i]=i&1 ? 0xFF40C0FFu:0xFFFF8040u;
    const float positions[3][4]={{24,24,3000000,1},{56,24,3000000,1},{24,56,3000000,1}};
    memcpy(ram+0x2000,positions,sizeof(positions));
    ((uint32_t *)(ram+0x2100))[0]=0xFF20FF20;((uint32_t *)(ram+0x2100))[1]=0xFF2020FF;((uint32_t *)(ram+0x2100))[2]=0xFFFF2020;
    method(0x1720,0x2000);method(0x1760,(16u<<8)|0x42u);
    method(0x172C,0x2100);method(0x176C,(4u<<8)|0x40u);
    for(i=0;i<16;i++)method(0x0680+i*4,bits(i%5==0 ? 1.0f:0));
    scene(0);CHECK(pixels(g_device_state.default_tex)!=clear_hash);
    live_snapshot_test();
    Nv2aRendererCheckpointSource source={&g_pg,&g_pg_aux,g_mv,NULL,epoch,invalidate};
    Doa3CheckpointResource resource=nv2a_renderer_checkpoint_resource(&source);
    void *saved=resource.capture(resource.context);CHECK(saved);
    CHECK(d3d8_pipeline_capture(before));
    reference_vertex_setup=1;
    pgraph_d3d11_force_geometry_rebuild(1);
    pgraph_d3d11_force_scalar_data(1);scalar_words=0;
    for(i=0;i<3;i++) { scene(i+1);hash_all(reference[i]); }
    CHECK(reference_setups==3 && scalar_words);
    CHECK(pgraph_d3d11_force_scalar_data(0));bulk_packets=0;
    reference_vertex_setup=0;pixel_setups=0;
    CHECK(pgraph_d3d11_force_geometry_rebuild(0));
    for(pass=0;pass<3;pass++) {
        pgraph_d3d11_force_guest_uploads(pass==0);
        hostile_future();CHECK(resource.restore(resource.context,saved)==1);
        CHECK(d3d8_pipeline_capture(after) && !memcmp(before,after,sizeof(*before)));
        for(i=0;i<3;i++) { scene(i+1);hash_all(actual);CHECK(!memcmp(reference[i],actual,sizeof(actual))); }
    }
    CHECK(!pgraph_d3d11_force_guest_uploads(0));
    CHECK(pixel_setups==9 && bulk_packets);
    CHECK(g_geometry.hits && g_geometry_resident_draws);
    /* Draw suppression must omit GPU work without changing logical PGRAPH,
     * pipeline or target metadata. The original endpoint remains recoverable. */
    CHECK(resource.restore(resource.context,saved)==1);scene(1);
    void *rendered=resource.capture(resource.context);CHECK(rendered);
    CHECK(resource.restore(resource.context,saved)==1);
    uint64_t skipped=d3d8_replay_skipped_draws();
    UINT vertex_ring=g_up_ring_offset,index_ring=g_nv2aff_idx_off;
    CHECK(d3d8_replay_suppress_draws(1)==0);scene(1);
    CHECK(d3d8_replay_suppress_draws(0)==1);
    CHECK(d3d8_replay_skipped_draws()>skipped);
    CHECK(vertex_ring==g_up_ring_offset && index_ring==g_nv2aff_idx_off);
    void *undrawn=resource.capture(resource.context);CHECK(undrawn);
    CHECK(!resource.equal(resource.context,rendered,undrawn));
    CHECK(resource.equal_state(resource.context,rendered,undrawn));
    CHECK(resource.restore(resource.context,rendered)==1);hash_all(actual);
    CHECK(!memcmp(reference[0],actual,sizeof(actual)));
    /* A logical-state difference must not be mistaken for omitted pixels. */
    method(NV097_SET_BLEND_ENABLE,0);
    void *different=resource.capture(resource.context);CHECK(different);
    CHECK(!resource.equal_state(resource.context,rendered,different));
    resource.destroy(resource.context,different);
    resource.destroy(resource.context,undrawn);resource.destroy(resource.context,rendered);
    /* Skip preparation above vertex fetch/transform, texture/material setup
     * and packing for two frames; the last frame must rebuild independently.
     * Texture source changes during the hidden interval must be respected. */
    CHECK(resource.restore(resource.context,saved)==1);
    uint32_t original_texel=*(uint32_t *)(ram+0x1000);
    scene(1);*(uint32_t *)(ram+0x1000)^=0x00FFFFFFu;
    pgraph_d3d11_invalidate_guest_uploads();scene(2);scene(3);
    void *prepared=resource.capture(resource.context);CHECK(prepared);
    hash_all(reference[2]);
    *(uint32_t *)(ram+0x1000)=original_texel;
    CHECK(resource.restore(resource.context,saved)==1);
    uint64_t prep_before[3],prep_after[3];
    pgraph_d3d11_preparation_stats(&prep_before[0],&prep_before[1],&prep_before[2]);
    vertex_ring=g_up_ring_offset;index_ring=g_nv2aff_idx_off;
    CHECK(!pgraph_d3d11_skip_preparation(1));scene(1);
    *(uint32_t *)(ram+0x1000)^=0x00FFFFFFu;scene(2);
    CHECK(pgraph_d3d11_skip_preparation(0));
    pgraph_d3d11_preparation_stats(&prep_after[0],&prep_after[1],&prep_after[2]);
    CHECK(prep_after[0]>prep_before[0] && prep_after[1]>prep_before[1] && prep_after[2]>prep_before[2]);
    CHECK(vertex_ring==g_up_ring_offset && index_ring==g_nv2aff_idx_off);
    pgraph_d3d11_invalidate_guest_uploads();scene(3);
    void *last_frame=resource.capture(resource.context);CHECK(last_frame);
    CHECK(resource.equal_state(resource.context,prepared,last_frame));
    CHECK(resource.equal(resource.context,prepared,last_frame));
    hash_all(actual);CHECK(!memcmp(reference[2],actual,sizeof(actual)));
    resource.destroy(resource.context,last_frame);resource.destroy(resource.context,prepared);
    *(uint32_t *)(ram+0x1000)=original_texel;
    /* Negative control: leave one backend field at a future value after
     * restoring everything else, then run identical production commands. */
    CHECK(resource.restore(resource.context,saved)==1);
    dev_SetRenderState(&g_device,D3DRS_FILLMODE,D3DFILL_WIREFRAME);
    scene(1);hash_all(actual);CHECK(memcmp(reference[0],actual,sizeof(actual)));
    CHECK(resource.restore(resource.context,saved)==1);scene(1);hash_all(actual);
    CHECK(!memcmp(reference[0],actual,sizeof(actual)));
    geometry_bounds_test();
    geometry_mutation_test(&resource,saved,ram);
    boundary_probe_test();
    /* Unsupported bound resources reject capture/restore without mutation. */
    IDirect3DVertexBuffer8 *vb=NULL;CHECK(SUCCEEDED(d3d8_CreateVertexBufferImpl(128,0,0,&vb)));
    g_cur_vb=vb;CHECK(!resource.capture(resource.context));CHECK(!resource.validate(resource.context,saved));
    Nv2aRendererCheckpointSource refused_source;
    CHECK(!pgraph_d3d11_rollback_snapshot_source(&refused_source));
    D3D8BoundaryProbe probe;d3d8_pipeline_probe(GetCurrentThreadId(),&probe);CHECK(probe.flags&D3D8_PROBE_BUFFERS);
    g_cur_vb=NULL;
    vb->lpVtbl->Release(vb);
    CHECK(d3d8_pipeline_capture(after));after->vsh.count=999;
    CHECK(!d3d8_pipeline_restore(after));
    resource.destroy(resource.context,saved);
    ID3D11DeviceContext_ClearState(g_device_state.d3d11_context);
    nv_geometry_clear(&g_geometry);CHECK(!g_geometry.bytes);
    for(i=0;i<TEXCACHE_N;i++) if(g_pg_host.texcache[i].tex)texcache_evict((int)i);
    d3d8_shaders_shutdown();d3d8_combiners_shutdown();d3d8_vsh_shutdown();d3d8_states_shutdown();
    d3d8_states_release_samplers();d3d8_Nv2aFF_Shutdown();up_ring_shutdown();
    if(g_nv2aff_idx_ring)ID3D11Buffer_Release(g_nv2aff_idx_ring);
    for(i=0;i<8;i++)offrt_free(&g_offrt[i]);
    ID3D11ShaderResourceView_Release(g_fbcopy.srv);ID3D11Texture2D_Release(g_fbcopy.tex);
    ID3D11ShaderResourceView_Release(g_prevframe.srv);ID3D11Texture2D_Release(g_prevframe.tex);
    ID3D11RenderTargetView_Release(g_device_state.default_rtv);ID3D11DepthStencilView_Release(g_device_state.default_dsv);
    ID3D11Texture2D_Release(g_device_state.default_tex);ID3D11Texture2D_Release(g_device_state.default_depth);
    ID3D11DeviceContext_Release(g_device_state.d3d11_context);CHECK(ID3D11Device_Release(g_device_state.d3d11_device)==0);
    free(s_gv);free(s_gi);free(s_gs);free(before);free(after);free(ram);
    puts("Production PGRAPH/D3D replay: matching pixels, backend-state negative control, bound-buffer rejection, COM cleanup");
    return 0;
}
