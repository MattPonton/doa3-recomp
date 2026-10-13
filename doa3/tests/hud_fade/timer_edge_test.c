/* Captured timer geometry; synthetic atlas rows (no game assets).
 * Check sampled edge alpha and unchanged mapping at 1x through 4x across
 * all display widths, plus shadow fade and unrelated-quad isolation. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef struct {
    float x, y, z, rhw;
    uint32_t color;
    float u, v, u1, v1;
} OutputVertex;
#include "timer_edge.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
static void fixture(OutputVertex q[4], float sx, float sy, uint32_t color)
{
    const float x[4] = {321.5f,321.5f,397.5f,397.5f};
    const float y[4] = {37.5f,65.5f,65.5f,37.5f};
    const float u[4] = {52,52,128,128};
    const float v[4] = {48,76,76,48};
    unsigned i;
    memset(q, 0, 4 * sizeof(*q));
    for (i = 0; i < 4; ++i) {
        q[i].x=x[i]*sx; q[i].y=y[i]*sy; q[i].rhw=1;
        q[i].color=color; q[i].u=u[i]/128; q[i].v=v[i]/128;
    }
}
/* Row 47 is transparent, row 48 and onward opaque: the seam's bilinear
 * sampling failure is independent of the artwork's RGB values. */
static float edge_alpha(float v)
{
    float a = v*128 - 47.5f;
    return a < 0 ? 0 : a > 1 ? 1 : a;
}
int main(void)
{
    const float widths[] = {640,640,854,1120,1707};
    const uint32_t colors[] = {0,0x11000000,0x80000000,0xFFFFFFFF};
    OutputVertex q[4], out[12], changed[4];
    unsigned w, scale, c, i;
    int reproduced = 0;
    for (w=0; w<5; ++w) for (scale=1; scale<=4; ++scale)
    for (c=0; c<4; ++c) {
        fixture(q,widths[w]/720*scale,(float)scale,colors[c]);
        CHECK(nv_timer_body_edge(q,4,out));
        CHECK(out[0].x==q[0].x && out[0].y==q[0].y);
        CHECK(out[8].x==q[2].x && out[8].y==q[2].y);
        CHECK(out[1].y==out[6].y && out[2].y==out[11].y);
        CHECK(out[1].v==out[6].v && out[2].v==out[11].v);
        for (i=0;i<12;++i) {
            CHECK(out[i].color==colors[c] && out[i].rhw==1 && out[i].z==0);
            CHECK(out[i].x>=q[0].x && out[i].x<=q[2].x);
            CHECK(out[i].y>=q[0].y && out[i].y<=q[2].y);
        }
        for (i=0; i<28*scale; ++i) {
            float y=ceilf(q[0].y-0.5f)+0.5f+i;
            float old_v=q[0].v+(y-q[0].y)/(q[1].y-q[0].y)*(q[1].v-q[0].v);
            float v=out[0].v;
            if (y>=out[6].y)
                v=out[6].v+(y-out[6].y)/(out[7].y-out[6].y)*(out[7].v-out[6].v);
            if (edge_alpha(old_v)<0.99f) reproduced=1;
            CHECK(edge_alpha(v)>0.999f);
            if (old_v>=48.5f/128) CHECK(fabsf(v-old_v)<0.000001f);
        }
    }
    CHECK(reproduced); /* The original mapping must fail this synthetic case. */
    fixture(q,1,1,0xFFFFFFFF);
    CHECK(!nv_timer_body_edge(q,3,out));
    memcpy(changed,q,sizeof(q)); changed[0].v=77.0f/128; /* Upper rim. */
    CHECK(!nv_timer_body_edge(changed,4,out));
    memcpy(changed,q,sizeof(q)); changed[2].x+=1; /* Non-rectangular. */
    CHECK(!nv_timer_body_edge(changed,4,out));
    memcpy(changed,q,sizeof(q)); changed[1].rhw=0.5f; /* Perspective. */
    CHECK(!nv_timer_body_edge(changed,4,out));
    memcpy(changed,q,sizeof(q)); changed[1].color=0x80000000; /* Gradient. */
    CHECK(!nv_timer_body_edge(changed,4,out));
    memcpy(changed,q,sizeof(q)); changed[1].z=0.5f;
    CHECK(!nv_timer_body_edge(changed,4,out));
    puts("PASS: timer atlas edge sampling, scales/aspects, fade and isolation");
    return 0;
}
