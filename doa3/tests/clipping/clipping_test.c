#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "vertices.h"
#include "clip.h"
#include "ice_cave_fixtures.h"

static int failures;
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); ++failures; } } while (0)
static int close_to(float a, float b)
{
    return isfinite(a) && fabsf(a - b) <= 2e-6f * fmaxf(1.0f, fabsf(b));
}

int main(void)
{
    unsigned c;
    for (c = 0; c < sizeof(ice_cases) / sizeof(ice_cases[0]); ++c) {
        const IceCase *f = &ice_cases[c];
        ClipVert input[3] = {0}, clipped[8];
        OutputVertex rendered[9];
        int i, k, n, rotation, reverse;
        /* Check every winding/start vertex: clipping a shared edge must not
         * depend on which triangle traverses it first. */
        for (reverse = 0; reverse < 2; ++reverse) for (rotation = 0; rotation < 3; ++rotation) {
            for (i = 0; i < 3; ++i) {
                int index = (rotation + (reverse ? 3 - i : i)) % 3;
                memcpy(&input[i], f->in[index], 4 * sizeof(float));
            }
            n = nv_clip_plane(input, 3, 1, 0.0f, clipped);
            CHECK(n == f->count);
            for (i = 0; i < n; ++i) {
                int found = 0;
                for (k = 0; k < f->count; ++k) {
                    const float *v = (const float *)&clipped[i];
                    if (close_to(v[0], f->out[k][0]) && close_to(v[1], f->out[k][1]) &&
                        close_to(v[2], f->out[k][2]) && close_to(v[3], f->out[k][3])) found = 1;
                }
                CHECK(found);
                CHECK(clipped[i].Z >= 0.0f);
            }
            n = nv_clip_triangle(input, rendered);
            CHECK(n == f->count - 2);
            for (i = 0; i < n * 3; ++i) {
                int found = 0;
                for (k = 0; k < f->count; ++k)
                    if (close_to(rendered[i].x, f->out[k][0] / f->out[k][3]) &&
                        close_to(rendered[i].y, f->out[k][1] / f->out[k][3])) found = 1;
                CHECK(found);
                CHECK(isfinite(rendered[i].rhw) && rendered[i].rhw > 0.0f);
            }
        }
    }
    {
        /* An ordinary cut must preserve color and BOTH texture coordinates. */
        ClipVert input[3] = {
            {10, 20, -1, 2, 0, 20, 40, 60, 0, 2, 4, 6},
            {30, 40,  1, 2, 20, 40, 60, 80, 2, 4, 6, 8},
            {50, 60,  1, 2, 40, 60, 80, 100, 4, 6, 8, 10}
        }, out[8];
        OutputVertex rendered[9];
        int n = nv_clip_plane(input, 3, 1, 0, out);
        CHECK(n == 4);
        CHECK(out[0].X == 20 && out[0].Y == 30 && out[0].Z == 0);
        CHECK(out[0].r == 10 && out[0].g == 30 && out[0].b == 50 && out[0].a == 70);
        CHECK(out[0].u == 1 && out[0].v == 3 && out[0].u1 == 5 && out[0].v1 == 7);
        input[0].Z = 1;
        CHECK(nv_clip_plane(input, 3, 1, 0, out) == 3);
        CHECK(memcmp(input, out, sizeof(input)) == 0);
        input[0].Z = input[1].Z = input[2].Z = -1;
        CHECK(nv_clip_triangle(input, rendered) == 0);
        /* Eye-plane clipping must land exactly on its positive W boundary. */
        input[0].W = -1e20f; input[1].W = input[2].W = 2;
        n = nv_clip_plane(input, 3, 0, 0.01f, out);
        CHECK(n == 4);
        CHECK(out[0].W == 0.01f && out[3].W == 0.01f);
        input[1].W = input[2].W = -1;
        CHECK(nv_clip_triangle(input, rendered) == 0);
    }
    if (failures) fprintf(stderr, "%d clipping checks failed\n", failures);
    else puts("Captured Ice Cave triangles, winding, attributes, and clip planes passed.");
    return failures ? 1 : 0;
}
