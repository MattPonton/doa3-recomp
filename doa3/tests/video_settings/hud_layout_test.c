#include "video_settings.h"
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
ptrdiff_t g_xbox_mem_offset;
HWND d3d8_GetHWND(void) { return NULL; }
typedef struct { float x,y,z,rhw; uint32_t color; float u,v,u1,v1; } OutputVertex;
static struct {
    unsigned surface_clip_h,surface_clip_v,window_clip_type;
    unsigned window_clip_h[1],window_clip_v[1];
    int depth_test;
    struct { unsigned enabled,offset; } tex[1];
} g_pg;
static unsigned bw,bh;
static int offscreen,clip_x,clip_w;
static unsigned d3d8_GetBackbufferWidth(void) { return bw; }
static unsigned d3d8_GetBackbufferHeight(void) { return bh; }
static int d3d8_OffscreenTargetActive(void) { return offscreen; }
int doa3_guest_display_size(unsigned *w,unsigned *h) { *w=720; *h=480; return 1; }
static int nv_samples_framebuffer(unsigned offset) { return offset==123; }
void d3d8_SetScissorRect(int x,int y,int w,int h) { (void)y;(void)h;clip_x=x;clip_w=w; }
#include "clip.h"
#include "layout.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void quad(OutputVertex q[4],float x,float y,float w,float h)
{
    for (unsigned i=0;i<4;++i) {
        q[i]=(OutputVertex){0}; q[i].rhw=1;
        q[i].x=x+(i>=2?w:0); q[i].y=y+((i==1||i==2)?h:0);
    }
}
int main(void)
{
    OutputVertex q[4];
    for (int aspect=0;aspect<VIDEO_ASPECT_COUNT;++aspect) for (int scale=1;scale<=3;++scale) {
        video_set_aspect(aspect); video_set_scale(scale);
        video_guest_target_size(aspect,&bw,&bh);
        g_pg.surface_clip_h=1440u<<16; g_pg.surface_clip_v=960u<<16;
        g_pg.window_clip_h[0]=1439u<<16; g_pg.window_clip_v[0]=959u<<16;
        float x,w; video_hud_region(bw,&x,&w);
        quad(q,0,30,720,40);
        int hud=nv_fit_to_backbuffer(q,4,1);
        CHECK(hud==(aspect>VIDEO_ASPECT_16_9));
        CHECK(fabsf(q[0].x-x)<0.001f && fabsf(q[2].x-(x+w))<0.001f);
        CHECK(fabsf(q[0].y-30*scale)<0.001f);
        if (hud) {
            CHECK(nv_apply_window_clip(hud));
            CHECK(clip_x==(int)floorf(x) && clip_x+clip_w==(int)ceilf(x+w));
            g_pg.window_clip_h[0]=200u|(999u<<16);
            CHECK(nv_apply_window_clip(hud));
            CHECK(clip_x==(int)floorf(x+200*w/1440));
            CHECK(clip_x+clip_w==(int)ceilf(x+1000*w/1440));
        }
        quad(q,-.5f,-.5f,720,480); CHECK(!nv_fit_to_backbuffer(q,4,1)); /* Full fade. */
        CHECK(q[0].x<0 && q[2].x>(float)bw-4);
        quad(q,.53125f,.53125f,720,480); /* Captured menu dimmer. */
        CHECK(!nv_fit_to_backbuffer(q,4,1));
        CHECK(fabsf(q[0].x-.53125f*bw/720)<.001f && q[0].y==.53125f*scale);
        CHECK(q[2].x>bw && q[2].y>bh); /* Preserve the native pixel offset. */
        quad(q,.53125f,.53125f,720,43); /* Captured menu header. */
        nv_fit_to_backbuffer(q,4,1);
        CHECK(fabsf(q[0].x-(x+.53125f*w/720))<.001f && q[0].y==.53125f*scale);
        CHECK(fabsf(q[2].x-(x+720.53125f*w/720))<.001f);
        CHECK(fabsf(q[2].y-43.53125f*scale)<.001f); /* Interior edge unchanged. */
        g_pg.tex[0].enabled=1; g_pg.tex[0].offset=123;
        quad(q,0,30,720,40); CHECK(!nv_fit_to_backbuffer(q,4,1)); /* Framebuffer effect. */
        CHECK(fabsf(q[2].x-(float)bw)<.001f);
        g_pg.tex[0].enabled=0;
        g_pg.depth_test=1;
        quad(q,0,30,720,40); CHECK(!nv_fit_to_backbuffer(q,4,1));
        CHECK(fabsf(q[2].x-(float)bw)<.001f);
        g_pg.depth_test=0;
        quad(q,0,30,720,40); q[0].rhw=.5f; CHECK(!nv_fit_to_backbuffer(q,4,1));
        quad(q,0,0,1440,960); CHECK(!nv_fit_to_backbuffer(q,4,0)); /* World geometry. */
        CHECK(fabsf(q[2].x-(float)bw)<.001f);
        offscreen=1; quad(q,0,0,1440,960); CHECK(!nv_fit_to_backbuffer(q,4,1));
        CHECK(fabsf(q[2].x-(float)bw)<.001f); offscreen=0;
    }
    puts("PASS: centered ultrawide HUD and scissor, legacy scaling, full-screen/world isolation");
    return 0;
}
