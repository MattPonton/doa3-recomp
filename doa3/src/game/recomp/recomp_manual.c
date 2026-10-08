/**
 * Dead or Alive 3 - Manually implemented / overridden recompiled functions
 *
 * Functions the automatic recompiler can't handle correctly (mid-function
 * entry points, hardware register polls, SEH continuations, etc.) are
 * hand-written here using the same register model and calling conventions
 * as the generated code, then registered in g_manual_funcs[] so RECOMP_ICALL
 * and the dispatch chain pick them up ahead of the generated versions.
 *
 * doa3.1 branch: upstream's overrides were written against the 3.0 XBE and
 * are kept, unbuilt, in reference/recomp_manual_30.c. This file keeps only
 * the helpers the rest of the runtime links against, plus overrides
 * re-derived for the XBE the code was generated from (xbe_layout.h).
 * Record each new override in PORTING_3.1.md.
 */

#define RECOMP_GENERATED_CODE
#include "gen/recomp_funcs.h"
#include "xbe_layout.h"
#include <stddef.h>
#include <stdio.h>
#include <intrin.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "online/netplay_session.h"
#include "online/netplay.h"

extern uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment);
extern void xbox_fiber_yield(void);
extern int  xbox_fiber_active(void);
extern void xbox_fiber_wake(uint32_t event_va);

/* ======================================================================
 * Shared helpers, carried over from upstream (version-neutral).
 * Used by main.c, the kernel bridge, fibers, NV2A, netplay and the UI.
 * ====================================================================== */

uint32_t doa3_dbg_read32(uint32_t va) { return MEM32(va); }

uint32_t doa3_dbg_read8(uint32_t va)  { return MEM8(va); }

static uint32_t s_wseed[64];

void doa3_worker_rand_seed(uint32_t seed)
{
    int i;
    for (i = 0; i < 64; i++) s_wseed[i] = (seed ^ (uint32_t)(i * 0x9E3779B1u)) | 1u;
}

void recomp_itail_fail_log(uint32_t va)
{
    static uint32_t seen[16];
    static int n = 0;
    for (int i = 0; i < n; i++) if (seen[i] == va) return;
    if (n < 16) {
        seen[n++] = va;
    }
}

void doa3_apu_deliver_irq(void)
{
    typedef struct MCPXAPUState MCPXAPUState;
    extern MCPXAPUState *g_apu_state;
    extern uint64_t mcpx_apu_mmio_read(MCPXAPUState *, uint64_t, unsigned int);
    extern int mcpx_apu_take_irq(void);
    extern int xbox_kernel_get_isr(int, uint32_t *, uint32_t *, uint32_t *);
    extern int xbox_kernel_pop_dpc(uint32_t *, uint32_t *, uint32_t *);
    extern recomp_func_t recomp_lookup(uint32_t xbox_va);
    extern recomp_func_t recomp_lookup_manual(uint32_t xbox_va);
    static unsigned s_isr_calls = 0, s_dpc_calls = 0;
    static int s_delivering = 0;
    uint32_t obj, routine, ctx, dpc, a1, a2;
    int i;
    if (s_delivering) return;   /* an interrupt does not preempt its own service routine or DPC */
    if (!mcpx_apu_take_irq()) return;
    s_delivering = 1;
    for (i = 0; xbox_kernel_get_isr(i, &obj, &routine, &ctx); i++) {
        recomp_func_t fn;
        if (routine < DOA3_SEC_DSOUND_VA || routine >= DOA3_SEC_DSOUND_END) continue;  /* was 3.0's 0x001C0000..0x001E0000 */
        fn = recomp_lookup_manual(routine);
        if (!fn) fn = recomp_lookup(routine);
        if (!fn) continue;
        {
            uint32_t saved_esp = esp;
            PUSH32(esp, ctx);
            PUSH32(esp, obj);
            PUSH32(esp, 0);           /* dummy return address */
            fn();
            esp = saved_esp;
        }
        s_isr_calls++;
        if (s_isr_calls <= 5 || (s_isr_calls % 500) == 0) {
            
        }
    }
    while (xbox_kernel_pop_dpc(&dpc, &a1, &a2)) {
        uint32_t droutine = MEM32(dpc + 12), dctx = MEM32(dpc + 16);
        recomp_func_t fn = recomp_lookup_manual(droutine);
        if (!fn) fn = recomp_lookup(droutine);
        if (!fn) continue;
        {
            uint32_t saved_esp = esp;
            PUSH32(esp, a2);
            PUSH32(esp, a1);
            PUSH32(esp, dctx);
            PUSH32(esp, dpc);
            PUSH32(esp, 0);
            fn();
            esp = saved_esp;
        }
        s_dpc_calls++;
        if (s_dpc_calls <= 5 || (s_dpc_calls % 500) == 0) {
            
        }
    }
    s_delivering = 0;
}

volatile int g_doa3_post_movie = 0;

volatile int g_doa3_in_pump = 0;

uint32_t g_doa3_offrt_offs[8]; int g_doa3_offrt_n;   /* every texture surface seen */

uint32_t g_flg_a, g_flg_b;
int g_flg_w = 4, g_flg_test;

int rc_flg_cc(int cc)
{
    uint32_t mask = (g_flg_w == 1) ? 0xFFu : (g_flg_w == 2) ? 0xFFFFu : 0xFFFFFFFFu;
    uint32_t sign = (g_flg_w == 1) ? 0x80u : (g_flg_w == 2) ? 0x8000u : 0x80000000u;
    uint32_t a = g_flg_a & mask, b = g_flg_b & mask, r;
    int zf, sf, cf = 0, of = 0, pf, i, ones = 0;
    unsigned lo;

    if (g_flg_test) {
        r = (a & b) & mask;
    } else {
        r = (a - b) & mask;
        cf = (a < b);
        of = ((((a ^ b) & (a ^ r)) & sign) != 0);
    }
    zf = (r == 0);
    sf = ((r & sign) != 0);
    lo = (unsigned)(r & 0xFFu);
    for (i = 0; i < 8; i++) if (lo & (1u << i)) ones++;
    pf = ((ones & 1) == 0);

    switch (cc) {
    case 0:  return zf;                 /* je  */
    case 1:  return !zf;                /* jne */
    case 2:  return cf;                 /* jb  */
    case 3:  return !cf;                /* jae */
    case 4:  return cf || zf;           /* jbe */
    case 5:  return !cf && !zf;         /* ja  */
    case 6:  return sf != of;           /* jl  */
    case 7:  return sf == of;           /* jge */
    case 8:  return zf || (sf != of);   /* jle */
    case 9:  return !zf && (sf == of);  /* jg  */
    case 10: return sf;                 /* js  */
    case 11: return !sf;                /* jns */
    case 12: return of;                 /* jo  */
    case 13: return !of;                /* jno */
    case 14: return pf;                 /* jp  */
    case 15: return !pf;                /* jnp */
    default: return 0;
    }
}

uint32_t g_doa3_pb_base = 0, g_doa3_pb_end = 0;

static uint64_t g_icall_fail_logged = 0;

#define ICALL_CENSUS_MAX 256
static struct { uint32_t va; uint32_t hits; } g_icall_census[ICALL_CENSUS_MAX];
static int g_icall_census_n;

static int g_icall_census_overflow;

void recomp_icall_fail_log(uint32_t va)
{
    int new_target = 0;
    {   int i;
        for (i = 0; i < g_icall_census_n; i++)
            if (g_icall_census[i].va == va) { g_icall_census[i].hits++; break; }
        if (i == g_icall_census_n) {
            if (g_icall_census_n < ICALL_CENSUS_MAX) {
                new_target = 1;
                g_icall_census[g_icall_census_n].va = va;
                g_icall_census[g_icall_census_n].hits = 1;
                g_icall_census_n++;
            } else {
                g_icall_census_overflow = 1;
            }
        }
    }
    if (g_icall_fail_logged < 200 || new_target) {
        if (va == 0) {
            void *bt[10]; int n = CaptureStackBackTrace(1, 10, bt, NULL);
        }
        g_icall_fail_logged++;
    }
}

/* ======================================================================
 * Version-specific hooks still to be re-derived for this XBE.
 * Upstream's 3.0 bodies are in reference/recomp_manual_30.c.
 * ====================================================================== */

static void doa3_port_todo_once(const char *what)
{
    fprintf(stderr, "[%s] %s: not ported to this XBE yet\n", DOA3_XBE_VERSION, what);
}

/* Guest display size (the D3D device's back-buffer width/height fields).
 * 3.0 read them from 0x0090F4A8/0x0090F4AC. Returning 0 makes the NV2A
 * translator fall back to SET_SURFACE_CLIP, which is right until 2x2
 * supersampling is enabled at character select.
 * TODO(3.1): locate the fields in this build's device object. */
int doa3_guest_display_size(unsigned *w, unsigned *h)
{
    (void)w; (void)h;
    return 0;
}

/* CRI server pump, called from the vblank-wait bridge (kernel_bridge.c).
 * On hardware the ADX/Sofdec servers run from the vsync interrupt; 3.0
 * drives them with its server entry points and globals.
 * TODO(3.1): re-derive against this build's CRI library. Until then streamed
 * audio and movies are expected to stall. */
void doa3_pump_cri_servers(void)
{
    static int s_warned = 0;
    if (!s_warned) { s_warned = 1; doa3_port_todo_once("doa3_pump_cri_servers"); }
}

/* Whether the cooperative worker fibers may run this frame. 3.0 also
 * required its two CRI lock words (0x00B24D38, 0x00C0E384) to be clear.
 * g_doa3_post_movie is only set by movie-flow overrides, so this stays 0
 * until those exist for this XBE.
 * TODO(3.1): restore the CRI lock check with this build's addresses. */
int doa3_workers_may_run(void)
{
    return g_doa3_post_movie && !g_doa3_in_pump;
}

/* Netplay: force the guest's XPP pad table to the session's pad mask.
 * 3.0 writes its pad state at 0x005E5CD0 (+0x80 per port) and friends.
 * Online play is not ported to this XBE yet, so this only reports.
 * TODO(3.1): port with the netplay state regions (netplay_session.c). */
void doa3_netplay_force_pads(uint32_t mask)
{
    static int s_warned = 0;
    (void)mask;
    if (!s_warned) { s_warned = 1; doa3_port_todo_once("doa3_netplay_force_pads"); }
}

/* Page-table integrity diagnostic (DOA3_PTCHK=1 on 3.0). Its table addresses
 * are 3.0's; a no-op here. */
void doa3_ptinfo_check(const char *where)
{
    (void)where;
}

/* NtReadFile diagnostic hook (kernel_bridge.c); unset. */
void (*g_kernel_ptinfo_hook)(const char *where);

/* CRT initializer lookup used by main.c before recomp_lookup(). 3.0 kept a
 * separate table (gen/recomp_ctors.c); here the __xi/__xc entries are seeded
 * like any other function (tools.xbe_layout --crt-initializers), so they are
 * in the dispatch table and this defers to it. */
recomp_func_t doa3_crt_lookup(uint32_t va)
{
    (void)va;
    return 0;
}

/* ======================================================================
 * Overrides for this XBE.
 * ====================================================================== */

/* (none yet) */

/* ── Manual override table ──────────────────────────────────────────
 * Map original Xbox VA -> hand-written replacement; the trailing {0,0}
 * sentinel keeps the array valid and is skipped at lookup. */
static const struct {
    uint32_t      xbox_va;
    recomp_func_t func;
} g_manual_funcs[] = {
    { 0u, 0 },  /* sentinel */
};
#define NUM_MANUAL_FUNCS (sizeof(g_manual_funcs) / sizeof(g_manual_funcs[0]))

recomp_func_t recomp_lookup_manual(uint32_t xbox_va)
{
    for (size_t i = 0; i < NUM_MANUAL_FUNCS; i++) {
        if (g_manual_funcs[i].func && g_manual_funcs[i].xbox_va == xbox_va)
            return g_manual_funcs[i].func;
    }
    return NULL;
}
