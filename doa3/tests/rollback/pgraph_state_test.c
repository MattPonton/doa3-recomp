/* Real command handlers, private state and mock submission/Clear sinks.
 * No native resources or pixels are restored by this test. */
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d3d/d3d8_xbox.h"
#include "nv2a/nv2a_pgraph_d3d11.h"
#include "nv2a/nv2a_regs.h"
#include "nv2a/nv2a_pgraph_state.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
float g_mv[16];
volatile int g_doa3_post_movie;
volatile LONG g_doa3_heartbeat;
uint32_t g_mhist[0x800], g_xmode, g_xmode_n[4];
static void submit_draw(void);
static void nv_sync_render_target(void);
IDirect3DDevice8 *xbox_GetD3DDevice(void);
/* This handler-only fixture owns no derived geometry or native buffers. */
static int g_geometry;
static void nv_geometry_clear(int *unused) { (void)unused; }

#include "pgraph.generated.inc"

static Nv2aPgraphLogicalImage before, after, replayed, submitted, expected_submit, bad;
static unsigned draws, clears, target_syncs;
static uint32_t clear_flags, clear_color;
static uint32_t blend_states[256];
void d3d8_SetBlendFactor(float r, float g, float b, float a)
{ (void)r; (void)g; (void)b; (void)a; }
static HRESULT __stdcall set_render_state(IDirect3DDevice8 *self, D3DRENDERSTATETYPE state, DWORD value)
{ (void)self; CHECK((unsigned)state < 256); blend_states[state] = value; return S_OK; }
static void submit_draw(void)
{
    CHECK(nv2a_pgraph_logical_capture(&submitted, &g_pg, &g_pg_aux, g_mv));
    draws++;
}
static void nv_sync_render_target(void) { target_syncs++; }
static HRESULT __stdcall clear_sink(IDirect3DDevice8 *self, DWORD count,
    const D3DRECT *rects, DWORD flags, D3DCOLOR color, float depth, DWORD stencil)
{
    (void)self;
    CHECK(count == 0 && rects == NULL && depth == 1.0f && stencil == 0);
    CHECK(target_syncs == clears + 1);
    clear_flags = flags; clear_color = color; clears++;
    return S_OK;
}
static IDirect3DDevice8Vtbl vtable;
static IDirect3DDevice8 device;
IDirect3DDevice8 *xbox_GetD3DDevice(void) { return &device; }
static void method(uint32_t m, uint32_t p) { (void)pgraph_d3d11_method(0, m, p); }
static void capture(Nv2aPgraphLogicalImage *p)
{ CHECK(nv2a_pgraph_logical_capture(p, &g_pg, &g_pg_aux, g_mv)); }
static void restore(const Nv2aPgraphLogicalImage *p)
{ CHECK(nv2a_pgraph_logical_restore_private(p, &g_pg, &g_pg_aux, g_mv)); }

static void tail(void)
{
    unsigned i;
    for (i = 0; i < 4; i++) {
        method(0x1B00 + i * 0x40, 0x100000 + i * 0x1000);
        method(0x1B04 + i * 0x40, 0x12345678 + i);
        method(0x1B0C + i * 0x40, 0x40000000);
        method(0x1B20 + i * 0x40, 0x200000 + i * 0x400);
    }
    method(0x034C, 0x80604020); /* blend color */
    method(0x1E9C, 2);
    for (i = 0; i < 8; i++) method(0x0B00 + i * 4, 0x12340000 + i);
    method(0x1EA4, 12);
    for (i = 0; i < 32; i++) method(0x0B80 + i * 4, 0x3F800000 + i);
    for (i = 0; i < 16; i++) method(0x0480 + i * 4, 0x40000000 + i);
    for (i = 0; i < 128; i++) method(0x1000 + i * 4, 0x3F000000 + i);
    method(0x0100, 0xA3ABCDEF); method(0x0100, 0xA6123456);
    method(0x0100, 0xAA000080); method(0x0100, 0xA9FFFFFF);
    method(0x02C0, 0x01230004); method(0x02E0, 0x02340005);
    method(0x0208, 0x120); method(0x020C, 2560); method(0x0210, 0x400000);
    method(0x1818, 0xAABBCCDD);
    method(0x1808, 99); method(0x1810, 0x02000020);
    method(0x1D90, 0xFF123456); method(0x1D94, 0xF3);
    method(0x17FC, 0);
}

static void packet_equivalence(void)
{
    uint32_t words[2047],methods[]={0x1818,0x1800,0x1808};
    unsigned counts[]={1,2,3,4,31,2047},rooms[]={0,1,2,31,81920};
    for(unsigned i=0;i<2047;i++)words[i]=0x7FC00000u^(i*0x1234567u);
    for(unsigned m=0;m<3;m++)for(unsigned c=0;c<6;c++)
    for(unsigned r=0;r<5;r++)for(unsigned drawing=0;drawing<2;drawing++) {
        pgraph_d3d11_init();
        g_pg.in_draw=drawing;
        unsigned cap=m==0 ? MAX_INLINE_VERTS*INLINE_VERT_DWORDS:MAX_IDX;
        unsigned used=rooms[r]>cap ? 0:cap-rooms[r];
        if(m==0)g_pg.inline_count=used;else g_pg.idx_count=used;
        g_pg.idx_dropped=UINT32_MAX-2; /* wrap exactly like scalar increments */
        capture(&before);
        for(unsigned i=0;i<counts[c];i++)CHECK(pgraph_d3d11_method(3,methods[m],words[i]));
        capture(&after);PgraphD3D11Stats stats=g_pg_host.stats;
        restore(&before);memset(&g_pg_host.stats,0,sizeof(g_pg_host.stats));
        CHECK(pgraph_d3d11_data_packet(3,methods[m],words,counts[c]));
        capture(&replayed);
        CHECK(!memcmp(&after,&replayed,sizeof(after)) && !memcmp(&stats,&g_pg_host.stats,sizeof(stats)));
    }
    capture(&before);
    CHECK(!pgraph_d3d11_data_packet(0,0x17FC,words,4));
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,NULL,4));
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,words,0));
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,words,2048));
    CHECK(!pgraph_d3d11_force_scalar_data(1));
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,words,4));
    CHECK(pgraph_d3d11_force_scalar_data(0));
    g_doa3_mtrace=tmpfile();CHECK(g_doa3_mtrace);
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,words,4));
    fclose(g_doa3_mtrace);g_doa3_mtrace=NULL;
    capture(&replayed);CHECK(!memcmp(&before,&replayed,sizeof(before)));
    pgraph_d3d11_shutdown();
    CHECK(!pgraph_d3d11_data_packet(0,0x1818,words,4));
}
static void packet_bench(void)
{
    uint32_t words[1024];LARGE_INTEGER freq,begin,end;
    for(unsigned i=0;i<1024;i++)words[i]=i*0x12345u;
    QueryPerformanceFrequency(&freq);
    for(unsigned sample=0;sample<4;sample++)for(unsigned order=0;order<2;order++) {
        int bulk=(sample+order)&1;
        pgraph_d3d11_init();g_pg.in_draw=1;
        QueryPerformanceCounter(&begin);
        for(unsigned packet=0;packet<10000;packet++) {
            g_pg.inline_count=0;
            if(bulk)CHECK(pgraph_d3d11_data_packet(0,0x1818,words,1024));
            else for(unsigned i=0;i<1024;i++)pgraph_d3d11_method(0,0x1818,words[i]);
        }
        QueryPerformanceCounter(&end);
        CHECK(g_pg.inline_count==1024 && !memcmp(g_pg.inline_data,words,sizeof(words)));
        CHECK(g_pg_host.stats.methods_handled==10240000 && g_nvreg[0x1818>>2]==words[1023]);
        printf("PACKET-BENCH sample=%u path=%s words=10240000 ms=%.3f\n",sample+1,bulk ? "bulk":"scalar",
            1000.0*(end.QuadPart-begin.QuadPart)/freq.QuadPart);
    }
}
int main(int argc,char **argv)
{
    unsigned i;
    int classifications[0x800];
    unsigned char host_before[sizeof g_pg_host];
    vtable.Clear = clear_sink; vtable.SetRenderState = set_render_state; device.lpVtbl = &vtable;
    packet_equivalence();
    pgraph_d3d11_init();
    CHECK(g_pg.vert_stride == 5 && g_pg.depth_func == 0x201 && g_pg_host.initialized);
    CHECK(g_pg_aux.blend_src_valid == D3DBLEND_SRCALPHA && g_pg_aux.draw_has_uv == 1);
    method(0x0100, 0xAC052084); /* wrapper-provided layout */
    method(0x0100, 0xAD000505);
    method(0x17FC, 6);
    method(0x1818, 0x3F800000); method(0x1800, 0x00020001);
    CHECK(g_pg.inline_count == 1 && g_pg.idx_count == 2 && g_pg.in_draw);
    CHECK(g_pg_cpu_layout_seen && g_pg.hint_verts == 5);
    /* These auxiliary values are consumed by draw paths, not method handlers. */
    g_pg_aux.framebuffer_offsets[0] = 0x600000; g_pg_aux.framebuffer_count = 1;
    g_pg_aux.blend_src_valid = D3DBLEND_ONE;
    g_pg_aux.blend_dst_valid = D3DBLEND_ZERO;
    g_pg_host.dyn_tex = (IDirect3DTexture8 *)(uintptr_t)0x12345678;
    g_pg_host.texcache[3].tex = (IDirect3DTexture8 *)(uintptr_t)0xABCDEF00;
    g_pg_host.texcache[3].sig = 123; g_pg_host.frame = 50;
    capture(&before);
    tail(); capture(&after); expected_submit = submitted;
    CHECK(draws == 1 && clears == 1 && clear_flags == 7 && clear_color == 0xFF123456);
    CHECK(g_pg.vp.instr_count == 4 && g_pg.vp.consts[19][3] == u2f(0x3F80001F));
    CHECK(g_pg.tex[3].offset == 0x103000 && g_pg.light[3].amb[0] == u2f(0x3F000060));
    CHECK(submitted.draw.inline_count == 2 && submitted.draw.idx_count == 6);
    for (i = 0; i < 3; i++) {
        memcpy(host_before, &g_pg_host, sizeof g_pg_host);
        restore(&before);
        CHECK(!memcmp(host_before, &g_pg_host, sizeof g_pg_host));
        tail(); capture(&replayed);
        CHECK(!memcmp(&after, &replayed, sizeof after));
        CHECK(!memcmp(&expected_submit, &submitted, sizeof submitted));
        CHECK(clear_flags == 7 && clear_color == 0xFF123456);
    }
    /* Invalid images fail before ANY destination changes. */
    for (i = 0; i < 7; i++) {
        bad = before;
        switch (i) {
        case 0: bad.version++; break;
        case 1: bad.size--; break;
        case 2: bad.draw.inline_count = MAX_INLINE_VERTS * INLINE_VERT_DWORDS + 1; break;
        case 3: bad.draw.idx_count = MAX_IDX + 1; break;
        case 4: bad.draw.vp.instr_count = NV2A_VP_MAX_INSTR + 1; break;
        case 5: bad.aux.framebuffer_count = -1; break;
        case 6: bad.draw.in_draw = 2; break;
        }
        CHECK(!nv2a_pgraph_logical_restore_private(&bad, &g_pg, &g_pg_aux, g_mv));
        capture(&replayed); CHECK(!memcmp(&after, &replayed, sizeof after));
    }
    /* Raw register bit patterns are preserved, not sanitized. */
    method(0x1E9C, 0xFFFFFFFF); method(0x1EA4, 0xFFFFFFFE);
    method(0x0480, 0x7FC12345); capture(&before);
    CHECK(nv2a_pgraph_logical_validate(&before));
    method(0x0480, 0); restore(&before); capture(&replayed);
    CHECK(!memcmp(&before, &replayed, sizeof before));
    /* Real draw-state application retains the last valid blend pair across
     * malformed factors. That hidden history must rewind too. */
    method(0x0344, 1); method(0x0348, 0);
    apply_blend_fixture(&device); capture(&before);
    method(0x0344, 0x0302); method(0x0348, 0x0303);
    apply_blend_fixture(&device);
    CHECK(blend_states[D3DRS_SRCBLEND] == D3DBLEND_SRCALPHA);
    restore(&before);
    method(0x0344, 0xDEADBEEF); method(0x0348, 0xDEADBEEF);
    apply_blend_fixture(&device);
    CHECK(blend_states[D3DRS_SRCBLEND] == D3DBLEND_ONE);
    CHECK(blend_states[D3DRS_DESTBLEND] == D3DBLEND_ZERO);
    /* Full accumulators remain bounded; replay reproduces dropped-index counts. */
    method(0x17FC, 5);
    g_pg.inline_count = MAX_INLINE_VERTS * INLINE_VERT_DWORDS;
    g_pg.idx_count = MAX_IDX;
    capture(&before);
    method(0x1818, 123); method(0x1800, 0x10002); pgraph_d3d11_flush(); capture(&after);
    restore(&before);
    method(0x1818, 123); method(0x1800, 0x10002); pgraph_d3d11_flush(); capture(&replayed);
    CHECK(!memcmp(&after, &replayed, sizeof after) && g_pg.idx_dropped == 2);
    /* Exercise every dispatch-table entry and compare a second pass from the
     * same starting state, including handlers' return classifications. */
    pgraph_d3d11_init(); capture(&before);
    for (i = 0; i < 0x2000; i += 4)
        classifications[i >> 2] = pgraph_d3d11_method(0, i, 0x3F800000 | i);
    capture(&after); restore(&before);
    for (i = 0; i < 0x2000; i += 4)
        CHECK(classifications[i >> 2] == pgraph_d3d11_method(0, i, 0x3F800000 | i));
    capture(&replayed); CHECK(!memcmp(&after, &replayed, sizeof after));
    CHECK(pgraph_d3d11_method(0, 0x2000, 0) == 0);
    CHECK(pgraph_d3d11_method(0, 0x1801, 0) == 0);
    pgraph_d3d11_shutdown(); CHECK(!g_pg_host.initialized);
    if(argc==2 && !strcmp(argv[1],"--bench"))packet_bench();
    puts("PGRAPH production handlers: exact private replay, bounded accumulators, unchanged host resources; no pixel restore");
    return 0;
}
