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
    float vp_offset[4];
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
        /* Portrait viewport/scissor move together without shrinking the
         * already aspect-correct projected model. Exercise both sides. */
        for (int side=0;side<2;++side) {
            unsigned left=94+side*648, right=left+607;
            g_pg.window_clip_h[0]=left|(right<<16);
            g_pg.window_clip_v[0]=154|(625u<<16);
            g_pg.vp_offset[0]=left+.53125f; g_pg.vp_offset[1]=154.53125f;
            CHECK(nv_menu_preview()==(aspect>VIDEO_ASPECT_16_9));
            float shift=aspect>VIDEO_ASPECT_16_9 ? x+(left+.53125f+304)*(w-bw)/1440 : 0;
            CHECK(fabsf(nv_menu_preview_shift(bw,1440)-shift)<.001f);
            quad(q,(float)left,154,200,100);
            nv_fit_to_backbuffer(q,4,0);
            CHECK(fabsf(q[0].x-(left*(float)bw/1440+shift))<.001f);
            CHECK(fabsf((q[2].x-q[0].x)-200*(float)bw/1440)<.001f);
            CHECK(nv_apply_window_clip(0));
            if (aspect>VIDEO_ASPECT_16_9) {
                CHECK(clip_x==(int)floorf(x+left*w/1440));
                CHECK(clip_x+clip_w==(int)ceilf(x+(right+1)*w/1440));
            }
        }
        g_pg.depth_test=1;
        quad(q,46,77,304,236);
        OutputVertex temp=q[1];q[1]=q[3];q[3]=q[2];q[2]=temp; /* captured strip order */
        for (int i=0;i<4;++i) q[i].z=.9999f;
        CHECK(nv_fit_to_backbuffer(q,4,1)==(aspect>VIDEO_ASPECT_16_9));
        CHECK(fabsf(q[0].x-(x+46*w/720))<.001f);
        /* Tag/four-slot capture: 1440x480 surface, 92,77-389,312
         * scissor and 92.53125,77.53125 origin. Backings remain display
         * pixels (46,77-196,313), including their bottom below y=240. */
        g_pg.surface_clip_v=480u<<16;
        static const unsigned panels[4]={46,200,370,522};
        for (int slot=0;slot<4;++slot) {
            unsigned left=panels[slot]*2, right=left+297;
            g_pg.window_clip_h[0]=left|(right<<16);
            g_pg.window_clip_v[0]=77|(312u<<16);
            g_pg.vp_offset[0]=left+.53125f; g_pg.vp_offset[1]=77.53125f;
            CHECK(nv_menu_preview()==(aspect>VIDEO_ASPECT_16_9));
            float shift=aspect>VIDEO_ASPECT_16_9 ? x+(left+.53125f+149)*(w-bw)/1440 : 0;
            quad(q,(float)left,77,100,100);
            nv_fit_to_backbuffer(q,4,0);
            CHECK(fabsf(q[0].x-(left*(float)bw/1440+shift))<.001f);
            CHECK(fabsf((q[2].x-q[0].x)-100*(float)bw/1440)<.001f);
            CHECK(fabsf(q[0].y-77*(float)bh/480)<.001f);
            CHECK(nv_apply_window_clip(0));
            if (aspect>VIDEO_ASPECT_16_9) {
                CHECK(clip_x==(int)floorf(x+left*w/1440));
                CHECK(clip_x+clip_w==(int)ceilf(x+(right+1)*w/1440));
            }
            quad(q,(float)panels[slot],77,150,236);
            temp=q[1];q[1]=q[3];q[3]=q[2];q[2]=temp;
            for (int i=0;i<4;++i) q[i].z=.9999f;
            CHECK(nv_fit_to_backbuffer(q,4,1)==(aspect>VIDEO_ASPECT_16_9));
            CHECK(fabsf(q[0].x-(x+panels[slot]*w/720))<.001f);
            CHECK(fabsf(q[3].x-(x+(panels[slot]+150)*w/720))<.001f);
            CHECK(fabsf(q[2].y-313*scale)<.001f);
        }
        offscreen=1;CHECK(!nv_menu_preview());offscreen=0;
        g_pg.window_clip_h[0]=1439u<<16;g_pg.window_clip_v[0]=479u<<16;
        CHECK(!nv_menu_preview()); /* Full-screen world viewport. */
        g_pg.surface_clip_v=960u<<16;
        g_pg.depth_test=0;
        offscreen=1;CHECK(!nv_menu_preview());offscreen=0;
        g_pg.surface_clip_h=720u<<16;CHECK(!nv_menu_preview());
        g_pg.surface_clip_h=1440u<<16;
        g_pg.window_clip_h[0]=1439u<<16;g_pg.window_clip_v[0]=959u<<16;
        CHECK(!nv_menu_preview());
        offscreen=1; quad(q,0,0,1440,960); CHECK(!nv_fit_to_backbuffer(q,4,1));
        CHECK(fabsf(q[2].x-(float)bw)<.001f); offscreen=0;
    }
    puts("PASS: centered ultrawide HUD and scissor, legacy scaling, full-screen/world isolation");
    return 0;
}
