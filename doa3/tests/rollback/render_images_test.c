#include "render_fixture.h"
static D3D8RenderCheckpoint *thread_snapshot;
static void depth16_compare_test(void)
{
    ID3D11Texture2D *texture=NULL;
    ID3D11DepthStencilView *view=NULL;
    D3D11_TEXTURE2D_DESC d={0};
    d.Width=7;d.Height=3;d.MipLevels=d.ArraySize=1;
    d.Format=DXGI_FORMAT_D16_UNORM;d.SampleDesc.Count=1;
    d.Usage=D3D11_USAGE_DEFAULT;d.BindFlags=D3D11_BIND_DEPTH_STENCIL;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device,&d,NULL,&texture)));
    CHECK(SUCCEEDED(ID3D11Device_CreateDepthStencilView(g_device_state.d3d11_device,(ID3D11Resource *)texture,NULL,&view)));
    ID3D11DeviceContext_ClearDepthStencilView(g_device_state.d3d11_context,view,D3D11_CLEAR_DEPTH,.1f,0);
    D3D8ReplayImages *a=d3d8_replay_images_capture(g_device_state.d3d11_device,g_device_state.d3d11_context,1,&texture,1);
    D3D8ReplayImages *b=d3d8_replay_images_capture(g_device_state.d3d11_device,g_device_state.d3d11_context,1,&texture,1);
    CHECK(a && b && d3d8_replay_images_equal(a,b));
    CHECK(d3d8_replay_images_destroy(b));
    ID3D11DeviceContext_ClearDepthStencilView(g_device_state.d3d11_context,view,D3D11_CLEAR_DEPTH,.8f,0);
    b=d3d8_replay_images_capture(g_device_state.d3d11_device,g_device_state.d3d11_context,1,&texture,1);
    CHECK(b && !d3d8_replay_images_equal(a,b));
    CHECK(d3d8_replay_images_destroy(a) && d3d8_replay_images_destroy(b));
    ID3D11DepthStencilView_Release(view);ID3D11Texture2D_Release(texture);
}
static DWORD WINAPI wrong_thread(void *unused)
{
    (void)unused;
    CHECK(!d3d8_render_checkpoint_validate(thread_snapshot, 7));
    CHECK(!d3d8_render_checkpoint_restore(thread_snapshot, 7));
    CHECK(!d3d8_render_checkpoint_destroy(thread_snapshot));
    CHECK(!d3d8_render_checkpoint_equal(thread_snapshot,thread_snapshot));
    return 0;
}
int main(void)
{
    D3D_FEATURE_LEVEL level;
    D3D8RenderCheckpoint *s;
    ID3D11Texture2D *list[20], *replacement;
    ID3D11RenderTargetView *bound = NULL;
    ID3D11ShaderResourceView *sample = NULL;
    D3D11_TEXTURE2D_DESC d;
    D3D11_VIEWPORT vp;
    uint64_t expected[20], actual[20], mutated[20];
    unsigned i, pass; UINT count;
    HANDLE thread;
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,
        D3D11_SDK_VERSION,&g_device_state.d3d11_device,&level,&g_device_state.d3d11_context)));
    depth16_compare_test();
    /* Use real production offscreen allocation to seed guest color/depth. */
    CHECK(d3d8_SetOffscreenTarget(0x1000,32,16));
    g_device_state.default_tex=g_offrt[0].tex; g_device_state.default_depth=g_offrt[0].depth;
    g_device_state.default_rtv=g_offrt[0].rtv; g_device_state.default_dsv=g_offrt[0].dsv;
    memset(&g_offrt[0],0,sizeof g_offrt[0]);
    g_device_state.width=32; g_device_state.height=16;
    d3d8_RestoreDefaultTarget();
    fill(cur_rtv(),cur_dsv(),.25f,31);
    CHECK(d3d8_BindFramebufferCopy(0)); d3d8_keep_prev_frame();
    for(i=0;i<8;i++) {
        CHECK(d3d8_SetOffscreenTarget(0x2000+i*0x100,8+i,8));
        fill(cur_rtv(),cur_dsv(),.1f+i*.08f,40+i);
    }
    /* Checkpoint while a nondefault target and its viewport are active. */
    s=d3d8_render_checkpoint_capture(7); CHECK(s);
    D3D8RenderCheckpoint *pair=d3d8_render_checkpoint_capture(7);CHECK(pair);
    CHECK(d3d8_render_checkpoint_equal(s,pair));
    CHECK(!d3d8_render_checkpoint_equal(s,NULL));
    CHECK(d3d8_render_checkpoint_destroy(pair));
    /* Depth/stencil-only changes must be detected, with colors untouched. */
    ID3D11DeviceContext_ClearDepthStencilView(g_device_state.d3d11_context,cur_dsv(),
        D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,.99f,219);
    pair=d3d8_render_checkpoint_capture(7);CHECK(pair);
    CHECK(!d3d8_render_checkpoint_equal(s,pair));
    CHECK(d3d8_render_checkpoint_destroy(pair));
    CHECK(d3d8_render_checkpoint_restore(s,7)==1);
    hash_all(expected);
    thread_snapshot=s;
    thread=CreateThread(NULL,0,wrong_thread,NULL,0,NULL); CHECK(thread);
    CHECK(WaitForSingleObject(thread,10000)==WAIT_OBJECT_0); CloseHandle(thread);
    for(pass=0;pass<3;pass++) {
        d3d8_RestoreDefaultTarget(); fill(cur_rtv(),cur_dsv(),.9f,200);
        CHECK(d3d8_BindFramebufferCopy(0)); d3d8_keep_prev_frame();
        for(i=0;i<8;i++) fill(g_offrt[i].rtv,g_offrt[i].dsv,.85f,150+i);
        CHECK(d3d8_BindPrevFrame(1));
        hash_all(mutated);
        pair=d3d8_render_checkpoint_capture(7);CHECK(pair);
        CHECK(!d3d8_render_checkpoint_equal(s,pair));
        CHECK(d3d8_render_checkpoint_destroy(pair));
        for(i=0;i<20;i++) CHECK(expected[i]!=mutated[i]);
        CHECK(d3d8_render_checkpoint_restore(s,7)==1);
        hash_all(actual); CHECK(!memcmp(expected,actual,sizeof expected));
        CHECK(g_off_cur==7 && g_off_active && g_prevframe_valid);
        count=1; ID3D11DeviceContext_RSGetViewports(g_device_state.d3d11_context,&count,&vp);
        CHECK(count==1 && vp.Width==15 && vp.Height==8);
        ID3D11DeviceContext_OMGetRenderTargets(g_device_state.d3d11_context,1,&bound,NULL);
        CHECK(bound==g_offrt[7].rtv); ID3D11RenderTargetView_Release(bound);
        ID3D11DeviceContext_PSGetShaderResources(g_device_state.d3d11_context,1,1,&sample);
        CHECK(sample==NULL);
    }
    /* Epoch/key/topology mismatch rejects before touching pixels or bindings. */
    fill(cur_rtv(),cur_dsv(),.95f,99); hash_all(mutated);
    CHECK(!d3d8_render_checkpoint_restore(s,8));
    g_offrt[0].key++; CHECK(!d3d8_render_checkpoint_restore(s,7)); g_offrt[0].key--;
    CHECK(render_checkpoint_inventory(list));
    ID3D11Texture2D_GetDesc(list[0],&d);
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device,&d,NULL,&replacement)));
    list[0]=replacement;
    CHECK(!d3d8_replay_images_restore(s->images,g_device_state.d3d11_context,7,list,20));
    ID3D11Texture2D_Release(replacement);
    hash_all(actual); CHECK(!memcmp(mutated,actual,sizeof mutated));
    CHECK(d3d8_SetOffscreenTarget(0x2700,19,11)); /* replaces a captured target */
    CHECK(!d3d8_render_checkpoint_restore(s,7)); CHECK(d3d8_render_checkpoint_destroy(s));
    /* Capture supports absent history and rejects later resource creation. */
    d3d8_RestoreDefaultTarget();
    ID3D11ShaderResourceView_Release(g_prevframe.srv); ID3D11Texture2D_Release(g_prevframe.tex);
    memset(&g_prevframe,0,sizeof g_prevframe); g_prevframe_valid=0;
    s=d3d8_render_checkpoint_capture(9); CHECK(s);
    CHECK(d3d8_render_checkpoint_restore(s,9)==1 && !g_prevframe_valid);
    d3d8_keep_prev_frame(); CHECK(!d3d8_render_checkpoint_restore(s,9));
    CHECK(d3d8_render_checkpoint_destroy(s));
    CHECK(!d3d8_render_checkpoint_capture(0));
    /* Unsupported resources/contexts fail after cleaning partial allocations. */
    CHECK(render_checkpoint_inventory(list));
    CHECK(!d3d8_replay_images_capture(g_device_state.d3d11_device,
        g_device_state.d3d11_context,9,list,21));
    ID3D11Texture2D_GetDesc(list[0],&d); d.Format=DXGI_FORMAT_R16G16B16A16_FLOAT;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(g_device_state.d3d11_device,&d,NULL,&replacement)));
    list[19]=replacement; /* nineteen valid copies allocated before rejection */
    CHECK(!d3d8_replay_images_capture(g_device_state.d3d11_device,
        g_device_state.d3d11_context,9,list,20));
    ID3D11Texture2D_Release(replacement);
    {
        ID3D11DeviceContext *deferred=NULL;
        CHECK(SUCCEEDED(ID3D11Device_CreateDeferredContext(g_device_state.d3d11_device,0,&deferred)));
        CHECK(!d3d8_replay_images_capture(g_device_state.d3d11_device,deferred,9,list,20));
        ID3D11DeviceContext_Release(deferred);
    }
    ID3D11DeviceContext_OMSetRenderTargets(g_device_state.d3d11_context,0,NULL,NULL);
    CHECK(!d3d8_render_checkpoint_capture(9));
    d3d8_RestoreDefaultTarget();
    ID3D11DeviceContext_ClearState(g_device_state.d3d11_context);
    for(i=0;i<8;i++) offrt_free(&g_offrt[i]);
    ID3D11ShaderResourceView_Release(g_fbcopy.srv); ID3D11Texture2D_Release(g_fbcopy.tex);
    ID3D11ShaderResourceView_Release(g_prevframe.srv); ID3D11Texture2D_Release(g_prevframe.tex);
    ID3D11RenderTargetView_Release(g_device_state.default_rtv);
    ID3D11DepthStencilView_Release(g_device_state.default_dsv);
    ID3D11Texture2D_Release(g_device_state.default_tex); ID3D11Texture2D_Release(g_device_state.default_depth);
    ID3D11DeviceContext_Release(g_device_state.d3d11_context);
    CHECK(ID3D11Device_Release(g_device_state.d3d11_device)==0);
    puts("WARP: all 20 guest color/depth/feedback images restored exactly, routing restored, stale snapshots rejected");
    return 0;
}
