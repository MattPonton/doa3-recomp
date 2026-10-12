/* Captured round-start timer state: zero is a transparent vertex colour,
 * not an absent colour. Exercise the production legacy-path decision. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
static uint32_t regs[0x800];
static int g_nv_draw_inline;
#define NVREG(m) regs[(m) >> 2]
#include "diffuse.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
int main(void)
{
    const uint32_t shadow_fade[] = {0x00000000, 0x11000000, 0x50000000, 0x80000000};
    unsigned i;
    g_nv_draw_inline = 1;
    NVREG(0x1E60) = 1;
    NVREG(0x0AC0) = 0x08040000;
    NVREG(0x0260) = 0x18140000;
    NVREG(0x1E40) = NVREG(0x0AA0) = 0xC00;
    NVREG(0x0288) = 0xC;
    NVREG(0x028C) = 0x1C80;
    for (i = 0; i < sizeof(shadow_fade)/sizeof(shadow_fade[0]); ++i)
        CHECK(nv_use_vertex_diffuse(shadow_fade[i] == 0));
    CHECK(nv_use_vertex_diffuse(0)); /* White RGB with zero alpha. */
    /* Other legacy draws still retain the established missing-diffuse fallback. */
    g_nv_draw_inline = 0; CHECK(!nv_use_vertex_diffuse(1));
    g_nv_draw_inline = 1;
    NVREG(0x0AC0) = 0x08200000; CHECK(!nv_use_vertex_diffuse(1)); /* Texture only. */
    CHECK(nv_use_vertex_diffuse(0));
    NVREG(0x0AC0) = 0x08040000;
    NVREG(0x0260) = 0x18200000; CHECK(!nv_use_vertex_diffuse(1)); /* Texture alpha only. */
    NVREG(0x0260) = 0x18140000;
    NVREG(0x1E60) = 2; CHECK(!nv_use_vertex_diffuse(1));
    NVREG(0x1E60) = 1;
    NVREG(0x0288) = 0x130C0300; CHECK(!nv_use_vertex_diffuse(1)); /* Different final combiner. */
    puts("PASS: transparent timer shadow, fade progression, and legacy fallback isolation");
    return 0;
}
