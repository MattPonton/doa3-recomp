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
    extern unsigned char xbox_current_irql(void);
    extern unsigned char xbox_set_irql(unsigned char irql);
    unsigned char irql0;
    if (s_delivering) return;   /* an interrupt does not preempt its own service routine or DPC */
    /* Raised IRQL masks the APU interrupt and DPCs, as on the Xbox's one CPU:
     * leave the interrupt pending until the code that raised it lowers it. */
    {   extern unsigned g_doa3_frames_presented;
        static unsigned s_masked, s_warned;
        if (xbox_current_irql() >= 2) {
            if (++s_masked == 120 && s_warned++ < 4) {
                fprintf(stderr, "[IRQL] APU interrupt masked for 120 presents: IRQL %u left raised (frame %u)\n",
                        xbox_current_irql(), g_doa3_frames_presented);
                fflush(stderr);
            }
            return;
        }
        s_masked = 0;
    }
    if (!mcpx_apu_take_irq()) return;
    s_delivering = 1;
    irql0 = xbox_set_irql(0x0B);    /* the APU's DIRQL (HalGetInterruptVector) */
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
    xbox_set_irql(2);               /* DPCs run at DISPATCH_LEVEL */
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
    xbox_set_irql(irql0);
    s_delivering = 0;
}

volatile int g_doa3_post_movie = 0;

uint64_t doa3_guest_rdtsc(void)
{
    static LARGE_INTEGER s_f, s_0;
    LARGE_INTEGER n;
    if (!s_f.QuadPart) { QueryPerformanceFrequency(&s_f); QueryPerformanceCounter(&s_0); }
    QueryPerformanceCounter(&n);
    /* 733,333,333 Hz, as XAPI's QueryPerformanceFrequency (0x18B755) reports */
    {
        uint64_t d = (uint64_t)(n.QuadPart - s_0.QuadPart);
        return (d / (uint64_t)s_f.QuadPart) * 733333333ull +
               (d % (uint64_t)s_f.QuadPart) * 733333333ull / (uint64_t)s_f.QuadPart;
    }
}

/* ── Call profile of the probed functions (gen/recomp_probes.c) ──────
 * Each probe wrapper counts its calls and registers its counter on the first
 * one. doa3_fn_profile_tick() (once per presented frame) prints, every 5 s,
 * the functions called most in that window: "[PROF] t=..s va:calls ...".
 * Comparing a window where something works with one where it has stalled
 * shows what stopped running. DOA3_PROF=0 turns the lines off. */
static struct { uint32_t va; unsigned *cnt; unsigned last, prevd; } s_prof[6000];
static int s_nprof;
void doa3_fn_register(uint32_t va, unsigned *count)
{
    if (s_nprof < (int)(sizeof s_prof / sizeof s_prof[0])) {
        s_prof[s_nprof].va = va; s_prof[s_nprof].cnt = count; s_prof[s_nprof].last = 0; s_prof[s_nprof].prevd = 0;
        s_nprof++;
    }
}
void doa3_fn_profile_tick(void)
{
    static int s_on = -1, s_dumps;
    static DWORD s_t0, s_last;
    DWORD now = GetTickCount();
    enum { TOP = 40 };
    int top[TOP], ntop = 0, i, k;
    if (s_on < 0) { const char *e = getenv("DOA3_PROF"); s_on = !(e && *e == '0'); s_t0 = s_last = now; }
    if (!s_on || now - s_last < 5000 || s_dumps >= 60) return;
    s_last = now; s_dumps++;
    for (i = 0; i < s_nprof; i++) {
        unsigned d = *s_prof[i].cnt - s_prof[i].last;
        if (!d) continue;
        for (k = ntop; k > 0; k--) {
            int j = top[k - 1];
            if (*s_prof[j].cnt - s_prof[j].last >= d) break;
            if (k < TOP) top[k] = j;
        }
        if (k < TOP) { top[k] = i; if (ntop < TOP) ntop++; }
    }
    { extern void xbox_fiber_dump_runs(void); xbox_fiber_dump_runs(); }
    fprintf(stderr, "[PROF] t=%lus active=%d:", (unsigned long)((now - s_t0) / 1000), ntop);
    for (k = 0; k < ntop; k++)
        fprintf(stderr, " %X:%u", s_prof[top[k]].va, *s_prof[top[k]].cnt - s_prof[top[k]].last);
    fprintf(stderr, "\n");
    {   /* functions active in the previous window that did not run in this one */
        int n = 0;
        fprintf(stderr, "[PROF]   stopped:");
        for (i = 0; i < s_nprof && n < 60; i++)
            if (s_prof[i].prevd && *s_prof[i].cnt == s_prof[i].last) {
                fprintf(stderr, " %X:%u", s_prof[i].va, s_prof[i].prevd);
                n++;
            }
        fprintf(stderr, "\n[PROF]   new:");
        for (i = 0, n = 0; i < s_nprof && n < 60; i++)
            if (!s_prof[i].prevd && *s_prof[i].cnt != s_prof[i].last && s_dumps > 1) {
                fprintf(stderr, " %X:%u", s_prof[i].va, *s_prof[i].cnt - s_prof[i].last);
                n++;
            }
        fprintf(stderr, "\n");
    }
    fflush(stderr);
    {   /* every active function of the window, for offline comparison */
        static FILE *s_pf;
        if (!s_pf) s_pf = fopen("doa3_prof.txt", "w");
        if (s_pf) {
            fprintf(s_pf, "# t=%lu\n", (unsigned long)((now - s_t0) / 1000));
            for (i = 0; i < s_nprof; i++)
                if (*s_prof[i].cnt != s_prof[i].last)
                    fprintf(s_pf, "%lu %X %u\n", (unsigned long)((now - s_t0) / 1000),
                            s_prof[i].va, *s_prof[i].cnt - s_prof[i].last);
            fflush(s_pf);
        }
    }
    for (i = 0; i < s_nprof; i++) {
        s_prof[i].prevd = *s_prof[i].cnt - s_prof[i].last;
        s_prof[i].last = *s_prof[i].cnt;
    }
}

volatile int g_doa3_in_pump = 0;

uint32_t g_doa3_offrt_offs[64]; int g_doa3_offrt_n;   /* every texture surface seen */
static unsigned g_doa3_aa_shot;   /* frame to screenshot with its method trace (multisampled screens) */
static unsigned g_doa3_mv_shot[8]; /* frames to screenshot after a movie releases the screen */

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
    if (new_target) {
        /* An indirect call into nothing does nothing at all: name each new
         * target once (3.1: Sofdec's B-picture routines 0x20E200/0x20E290
         * were missing and every B-frame macroblock they own stayed blank). */
        fprintf(stderr, "[ICALL] no function at %08X (indirect call skipped; first time)\n", va);
        fflush(stderr);
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

#if defined(DOA3_XBE_ID_3_1)
/* The 4134 D3D device is static at 0x1EB3A0 (g_pDevice 0x1EDE80). Surface
 * pointers in it: +0x2070 render target, +0x2074 depth, +0x2078 buffer
 * count, +0x207C back buffer (the 2x2 supersampled one while multisampling),
 * +0x2080/+0x2084 the display buffers. Surface: +4 Data, +0xC Format,
 * +0x10 Size (width-1 bits 0-11, height-1 bits 12-23). Device flag +8 bit
 * 0x4000 = multisampled back buffer, resolved at Swap (sub_001E11A0). */
#define D3D31_DEV() MEM32(0x001EDE80u)

/* Guest display size: the display buffer's own surface size. On character
 * select the game turns on 2x2 supersampling and the back buffer becomes
 * 1440x960, while the 2D overlay keeps arriving in display pixels. */
int doa3_guest_display_size(unsigned *w, unsigned *h)
{
    uint32_t dev = D3D31_DEV(), s, sz, dw, dh;
    if (!dev) return 0;
    s = MEM32(dev + 0x2080);
    if (!s) return 0;
    sz = MEM32(s + 0x10);
    if (!sz) return 0;
    dw = (sz & 0xFFFu) + 1; dh = ((sz >> 12) & 0xFFFu) + 1;
    if (dw < 64u || dw > 4096u || dh < 64u || dh > 4096u) return 0;
    *w = dw; *h = dh;
    return 1;
}

/* While multisampling, Swap points the render target at the display buffer,
 * binds the supersampled back buffer as texture 0 and draws a filter quad
 * over the screen. The translator renders the back buffer straight into the
 * host swap chain, so that quad would sample guest memory nobody wrote (all
 * black) and cover the frame: character select came out black. This names
 * the surface such a draw samples so the translator can skip it. */
uint32_t doa3_aa_resolve_source(void)
{
    uint32_t dev = D3D31_DEV(), s;
    if (!dev || !(MEM32(dev + 8) & 0x4000u)) return 0;
    s = MEM32(dev + 0x207C);
    return s ? (MEM32(s + 4) & 0x03FFFFFFu) : 0;
}

/* D3DDevice_SetRenderTarget(pRenderTarget, pZBuffer), ret 8.
 * Records every surface the game renders into that is not one of the
 * device's own back/display buffers (the stage reflection targets), so the
 * translator sends those to an offscreen host target and later draws that
 * sample them read it. 3.0's equivalent hook is sub_001B1350. */
void sub_001DC8E0_gen(void);
void sub_001DC8E0(void)
{
    uint32_t arg = MEM32(esp + 4), dev = D3D31_DEV();
    sub_001DC8E0_gen();
    if (!arg && dev) arg = MEM32(dev + 0x2070);
    if (dev) {
        /* The device's own buffers are never offscreen targets, even when
         * their memory once held one (mode changes free and reallocate):
         * drop their current offsets from the list. */
        int b, k;
        for (b = 0; b < 3; b++) {
            uint32_t fs = MEM32(dev + 0x207C + 4u * (uint32_t)b), fd;
            if (fs < 0x1000 || fs >= 0x08000000u) continue;
            fd = MEM32(fs + 4) & 0x03FFFFFFu;
            for (k = 0; k < g_doa3_offrt_n; k++)
                if (g_doa3_offrt_offs[k] == fd) {
                    g_doa3_offrt_offs[k] = g_doa3_offrt_offs[--g_doa3_offrt_n];
                    k--;
                }
        }
    }
    if (dev && arg >= 0x1000 && arg < 0x08000000u &&
        arg != MEM32(dev + 0x207C) && arg != MEM32(dev + 0x2080) && arg != MEM32(dev + 0x2084)) {
        uint32_t data = MEM32(arg + 4) & 0x03FFFFFFu;
        int k, seen = 0;
        for (k = 0; k < g_doa3_offrt_n; k++) if (g_doa3_offrt_offs[k] == data) seen = 1;
        if (!seen && data && g_doa3_offrt_n < 64) {
            uint32_t sz = MEM32(arg + 0x10);
            g_doa3_offrt_offs[g_doa3_offrt_n++] = data;
            fprintf(stderr, "[RTT] offscreen target #%d: surface %08X data %08X size %ux%u format %08X\n",
                    g_doa3_offrt_n, arg, data, (sz & 0xFFFu) + 1, ((sz >> 12) & 0xFFFu) + 1, MEM32(arg + 0xC));
        }
    }
}
#else
int doa3_guest_display_size(unsigned *w, unsigned *h) { (void)w; (void)h; return 0; }
uint32_t doa3_aa_resolve_source(void) { return 0; }
#endif

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

/* Whether the cooperative worker fibers may be given a time slice. 3.0
 * only slices after its intro movie (whose timing was tuned without it) and
 * also requires its two CRI lock words (0x00B24D38, 0x00C0E384) to be clear.
 * 3.1 slices from the start: the CRI setup (0x1C79F0) waits for the file
 * server thread to load the AFS partitions by polling ADXF_GetPtStat in a
 * loop that never yields, which on hardware the scheduler preempts.
 * TODO(3.1): the CRI lock check, with this build's addresses. */
int doa3_workers_may_run(void)
{
    return !g_doa3_in_pump;
}

#if defined(DOA3_XBE_ID_3_1)
/* ── XPP (controller) input: host-backed pads ───────────────────────────
 * There is no USB stack behind XAPI's XPP functions, so 3.0 replaces them
 * (reference/recomp_manual_30.c, which has the full reasoning). On 3.1 the
 * XDK code is byte-identical and moved; the game's pad table moved by
 * +0x1569F8 (3.0 -> 3.1 matched in the pad open/poll code 0x9EA60 / 0xA4D40):
 *   pad structs  0x73C6C8 (+0x80 per port; 3.0 0x5E5CD0)
 *   inserted     0x73C6BC   removed 0x73C6C0   (3.0 0x5E5CC8 / 0x5E5CCC)
 *   open mask    0x73C8C8   aggregates 0x73C8D0..0x73C99C (3.0 0x5E5ED0 / ED8)
 * Each pad struct holds two XINPUT_STATEs, at +0x19 and +0x2F; the game asks
 * for +0x2F and its per-frame aggregate builder reads +0x19, so both are
 * written. Before this, no input reached 3.1 at all (START could not skip
 * mv_op.sfd). */
extern unsigned g_doa3_frames_presented;
#define XPP_PAD_BASE   0x0073C6C8u
#define XPP_PAD_STRIDE 0x80u
#define XPP_INSERTED   0x0073C6BCu
#define XPP_REMOVED    0x0073C6C0u
#define XPP_OPEN_MASK  0x0073C8C8u
#define XPP_AGG_LO     0x0073C8D0u
#define XPP_AGG_HI     0x0073C99Cu

static uint32_t s_xpp_prev_mask;   /* XGetDeviceChanges baseline */

void sub_0021157D(void)   /* XGetDevices(type) -> connected mask, stdcall ret 4 */
{
    extern DWORD xbox_InputHostMask(void);
    uint32_t mask = netplay_filter_pad_mask(xbox_InputHostMask());
    static int s_n;
    s_xpp_prev_mask = mask;       /* resets the change baseline, as XAPI does */
    if (s_n++ < 4) { fprintf(stderr, "[XPP] XGetDevices -> %X\n", mask); fflush(stderr); }
    eax = mask;
    esp += 8;
}

void sub_0021159F(void)   /* XGetDeviceChanges(type, &ins, &rem), stdcall ret 12 */
{
    extern DWORD xbox_InputHostMask(void);
    uint32_t p_ins = MEM32(esp + 8), p_rem = MEM32(esp + 12);
    uint32_t cur = netplay_filter_pad_mask(xbox_InputHostMask());
    uint32_t ins = cur & ~s_xpp_prev_mask, rem = s_xpp_prev_mask & ~cur;
    {   uint32_t forced = netplay_forced_pads(), p;
        for (p = 0; p < 4; p++)
            if ((forced & (1u << p)) && MEM32(XPP_PAD_BASE + XPP_PAD_STRIDE * p + 0x78u) == 0)
                ins |= 1u << p;
    }
    s_xpp_prev_mask = cur;
    if (p_ins) MEM32(p_ins) = ins;     /* both words, every call */
    if (p_rem) MEM32(p_rem) = rem;
    if (ins | rem) { fprintf(stderr, "[XPP] device changes: +%X -%X\n", ins, rem); fflush(stderr); }
    eax = (ins | rem) ? 1u : 0u;
    esp += 16;
}

void doa3_netplay_force_pads(uint32_t mask)
{
    uint32_t p, i;
    for (p = 0; p < 4; p++) {
        uint32_t pad = XPP_PAD_BASE + XPP_PAD_STRIDE * p;
        for (i = 0x00; i < 0x19; i++) MEM8(pad + i) = 0;
        for (i = 0x19; i < 0x45; i++) MEM8(pad + i) = 0;
        for (i = 0x71; i < 0x78; i++) MEM8(pad + i) = 0;
        MEM32(pad + 0x7C) = 0;
        if (mask & (1u << p)) {
            MEM8(pad) = 1;
            MEM32(pad + 0x78) = 0x0AD00001u + p;
        } else {
            MEM32(pad + 0x78) = 0;
        }
    }
    for (i = XPP_AGG_LO; i < XPP_AGG_HI; i++) MEM8(i) = 0;
    MEM32(XPP_OPEN_MASK) = mask;
    MEM32(XPP_INSERTED) = 0;
    MEM32(XPP_REMOVED) = 0;
    s_xpp_prev_mask = mask;
}

void sub_00211898(void)   /* XInputClose(handle), stdcall ret 4 */
{
    fprintf(stderr, "[XPP] XInputClose(0x%08X)\n", MEM32(esp + 4));
    eax = 0;
    esp += 8;
}

void sub_00211823(void)   /* XInputOpen(type, port, slot, attrs) -> handle, ret 16 */
{
    uint32_t port = MEM32(esp + 8);
    fprintf(stderr, "[XPP] XInputOpen(port %u)\n", port);
    fflush(stderr);
    eax = 0x0AD00001u + port;
    esp += 20;
}

void sub_002118A4(void)   /* XInputGetCapabilities(handle, caps), ret 8 */
{
    uint32_t caps = MEM32(esp + 8);
    if (caps) {
        int i;
        for (i = 0; i < 0x18; i += 4) MEM32(caps + i) = 0;
        MEM8(caps + 0x18) = 0;
        MEM8(caps) = 1;   /* XINPUT_DEVSUBTYPE_GC_GAMEPAD */
    }
    eax = 0;
    esp += 12;
}

/* 3.1's XAPI has no XInputPoll. Reading the code: 0x211A96 is
 * XInputGetState(handle, pState) -- cmp [h+0xA3],1 / 0x48F when the device is
 * gone / copies the report from h+0x14 -- and 0x211B07 is
 * XInputSetState(handle, pFeedback) (rumble: stamps the feedback header at
 * +0x40/+0x41 and queues it). The 3.0 port's names for its copies (0x1E70AD
 * "XInputPoll", 0x1E711E "XInputGetState") are one function off, and its
 * input was written from the rumble call. On 3.1 that left the pad state
 * updated only when the game rumbled: in a fight START stayed "held" from
 * the last rumble call, so the pause menu reopened every frame. Serve the
 * pad from GetState and complete rumble requests at once. */
static uint32_t xpp_port_of(uint32_t h)
{
    uint32_t p = h - 0x0AD00001u;
    return p < 4 ? p : 0;
}

void sub_00211A96(void)   /* XInputGetState(handle, pState) -> 0, ret 8 */
{
    uint32_t h = MEM32(esp + 4), st = MEM32(esp + 8), port = xpp_port_of(h);
    static uint32_t s_packet[4] = { 0x1000, 0x1000, 0x1000, 0x1000 };
    uint16_t buttons = 0;
    uint8_t an[8] = {0};
    int16_t lx = 0, ly = 0, rx = 0, ry = 0;
    int i;
    {
        extern DWORD xbox_InputGetState(DWORD, void *);
        uint8_t raw[32] = {0};
        if (xbox_InputGetState(port, raw) == 0) {
            buttons = *(uint16_t *)(raw + 4);
            memcpy(an, raw + 6, 8);
            lx = *(int16_t *)(raw + 14); ly = *(int16_t *)(raw + 16);
            rx = *(int16_t *)(raw + 18); ry = *(int16_t *)(raw + 20);
        }
    }
    {   static int s_log = 0, s_was = 0;
        int now = (buttons != 0), ai;
        for (ai = 0; ai < 8; ai++) if (an[ai] >= 30) now = 1;
        if (now && !s_was && s_log < 24) {
            s_log++;
            fprintf(stderr, "[XPP] press port %u buttons %04X analog %02X %02X (frame %u) state at %08X\n",
                    port, buttons, an[0], an[1], g_doa3_frames_presented, st);
            fflush(stderr);
        }
        s_was = now;
    }
    if (st) {
        s_packet[port]++;
        MEM32(st) = s_packet[port];   /* dwPacketNumber */
        MEM16(st + 4) = buttons;      /* wButtons */
        for (i = 0; i < 8; i++) MEM8(st + 6 + i) = an[i];
        MEM16(st + 14) = (uint16_t)lx; MEM16(st + 16) = (uint16_t)ly;
        MEM16(st + 18) = (uint16_t)rx; MEM16(st + 20) = (uint16_t)ry;
    }
    eax = 0;
    esp += 12;
}

void sub_00211B07(void)   /* XInputSetState(handle, pFeedback) -> 0, ret 8 */
{
    uint32_t fb = MEM32(esp + 8);
    if (fb) MEM32(fb) = 0;        /* XINPUT_FEEDBACK_HEADER.dwStatus: done */
    eax = 0;
    esp += 12;
}
/* mp_UpdatePlayerJoinAndStartInput (0x88910, once per frame). START in the
 * title-attract loop (main strand 2, action 2 = mv_op) should set the
 * attract input word 0x5C9248, which mp_UpdateTitleAttract turns into the
 * exit. START never skipped mv_op on 3.1: log the gate state whenever a
 * pad's per-frame aggregate (0x73C8D8 + pad*0x2C) has START (0x10). */
void sub_00088910_gen(void);
void sub_00088910(void)
{
    static int s_n;
    uint32_t agg0 = MEM32(0x73C8D8u), pre = MEM32(0x5C9248u);
    int any = 0, p;
    for (p = 0; p < 4; p++) if (MEM8(0x73C8D8u + p * 0x2C) & 0x10) any = 1;
    sub_00088910_gen();
    if (any && s_n < 40) {
        s_n++;
        fprintf(stderr, "[JOIN] START seen (frame %u): agg0 %08X aa5 %u strand %u action %u mode %u a2c %u c9364 %u c9288 %08X "
                "joinP1 %u joinP2 %u c9248 %u->%u c9254 %08X\n",
                g_doa3_frames_presented, agg0, MEM8(0x596AA5u), MEM8(0x598E80u), MEM8(0x5A262Au),
                MEM32(0x5A2858u), MEM8(0x596A2Cu), MEM8(0x5C9364u), MEM32(0x5C9288u),
                MEM8(0x596A32u), MEM8(0x596A33u), pre, MEM32(0x5C9248u), MEM32(0x5C9254u));
        fflush(stderr);
    }
}

/* Battle pause trigger (0x8A2E0): returns the controller of the first human
 * player whose pad has START in its per-frame aggregate (0x73C8D8 +
 * pad*0x2C), or whose assigned pad (0x30E3F4[player]) is not in the open
 * mask 0x73C8C8 -- the "controller gone" case, which also pauses. In fights
 * on 3.1 it fires every frame for player 1 with no START held. Log the
 * inputs each time it reports a player (first 30). */
void sub_0008A2E0_gen(void);
void sub_0008A2E0(void)
{
    static int s_n;
    sub_0008A2E0_gen();
    if ((eax & 0xFF) != 0xFF && s_n < 30) {
        int p;
        s_n++;
        fprintf(stderr, "[PAUSE] trigger -> %02X (frame %u): uiActive %u openmask %08X mode %u assign",
                eax & 0xFF, g_doa3_frames_presented, MEM8(0x5A27A0u), MEM32(0x73C8C8u), MEM8(0x5A2858u));
        for (p = 0; p < 4; p++) fprintf(stderr, " %02X", MEM8(0x30E3F4u + p));
        fprintf(stderr, " claimed");
        for (p = 0; p < 4; p++) fprintf(stderr, " %u", MEM8(0xC5985Cu + p));
        fprintf(stderr, " agg+8");
        for (p = 0; p < 4; p++) fprintf(stderr, " %08X", MEM32(0x73C8D8u + p * 0x2C));
        fprintf(stderr, " human %u %u\n", MEM8(0x59CF58u), MEM8(0x59CF58u + 0x68));
        fflush(stderr);
    }
}

/* Title-attract exit (0x53EB0): with attract input pending (0x5C9248) and
 * action 2 (mv_op), mp_UpdateTitleAttract calls this every frame. First call
 * arms a 60-frame fade (0x5A697A = 1, counter 0x5A6970); when the counter
 * runs out 0x53F20 ends the action. All of it is gated on 0x5A8B97 == 0.
 * START reached 0x5C9248 on 3.1 and mv_op still played on: log the gate. */
void sub_00053EB0_gen(void);
void sub_00053EB0(void)
{
    static int s_n;
    uint8_t gate = MEM8(0x5A8B97u), armed = MEM8(0x5A697Au);
    int32_t cnt = (int32_t)MEM32(0x5A6970u);
    sub_00053EB0_gen();
    if (s_n < 40 && (s_n < 8 || (g_doa3_frames_presented % 60u) == 0)) {
        s_n++;
        fprintf(stderr, "[ATTRACT] exit fade 0x53EB0 (frame %u): gate 5A8B97=%u armed %u->%u counter %d->%d ret %u\n",
                g_doa3_frames_presented, gate, armed, MEM8(0x5A697Au), cnt, (int32_t)MEM32(0x5A6970u), eax);
        fflush(stderr);
    }
}

/* The attract movie player (0xD7490, the pad index in eax -> ebx): plays
 * mv_op.sfd and leaves early when the pad's aggregate word at
 * 0x73C8D8 + pad*0x2C has bits 0x300 or 0x30. START did not skip it on
 * 3.1; log which pad it watches and that word while it runs. */
void sub_000D7490_gen(void);
void sub_000D7490(void)
{
    fprintf(stderr, "[ATTRACT] movie player 0xD7490 for pad %d (frame %u); words:", (int)eax, g_doa3_frames_presented);
    {   uint32_t pp;
        for (pp = 0; pp < 4; pp++) fprintf(stderr, " %08X", MEM32(0x73C8D8u + pp * 0x2C));
    }
    fprintf(stderr, "\n");
    fflush(stderr);
    sub_000D7490_gen();
    fprintf(stderr, "[ATTRACT] movie player returned (frame %u)\n", g_doa3_frames_presented);
    fflush(stderr);
}
#else
void doa3_netplay_force_pads(uint32_t mask)
{
    static int s_warned = 0;
    (void)mask;
    if (!s_warned) { s_warned = 1; doa3_port_todo_once("doa3_netplay_force_pads"); }
}
#endif /* DOA3_XBE_ID_3_1 (XPP) */

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

int g_fn_trace_on = -1;   /* DOA3_TRACE_FN, see doa3_fn_trace */

/* ── D3D 4134 push buffer (3.1) ─────────────────────────────────────
 * There is no GPU: the push buffer is translated to D3D11 synchronously
 * when the driver kicks it off, so by the time the driver looks, the "GPU"
 * has consumed everything and passed every fence.
 *
 * CDevice fields in this build (from the generated code):
 *   [dev+0x00] write cursor          [dev+0x08] flags (4 = recording into
 *   [dev+0x24]/[dev+0x28] ring start/end      [dev+0x35C]; 0x2000 = no GPU yet)
 *   [dev+0x2C] last kicked cursor    [dev+0x2264] FIFO channel window
 *   [dev+0x30] next fence time (+2 per fence, InsertFence 0x1E28B0)
 *   [dev+0x34] -> fence semaphore the GPU writes the last finished time to;
 *              BlockOnTime (0x1E2960) spins until it reaches its target.
 * 3.0 does the same job in its KickOff override at 0x1B88C0
 * (reference/recomp_manual_30.c). */
#if defined(DOA3_XBE_ID_3_1)
extern int  pgraph_d3d11_method(int subchannel, uint32_t method, uint32_t param);
static uint32_t s_pb4134_parsed;

static int s_pb4134_flip;     /* a FLIP_STALL went by: the frame is complete */

/* NV2A push-buffer control words besides method headers:
 *   old jump  001x xxxx xxxx xxxx xxxx xxxx xxxx xx00  (word & 0xE0000003) == 0x20000000
 *   new jump  word & 3 == 1        call  word & 3 == 2     return  0x00020000
 * Targets are physical addresses; guest RAM is mapped 1:1 from VA 0.
 * The translator used to skip these words and carry on with the next word
 * in the ring. After a jump the words that follow are whatever an earlier
 * lap left there, and those were translated as commands: stale draws with
 * stale vertex pointers every frame. A call (D3D's RunPushBuffer, the
 * precompiled buffers) was not followed at all. Follow both; a jump inside
 * the ring itself (the wrap back to its start) ends this span, KickOff picks
 * the ring up from its start next time. */
static unsigned s_pb_ctl[4], s_pb_ctl_logged;
static void pb4134_translate_span(uint32_t from, uint32_t to, int translate, uint32_t ring_lo,
                                  uint32_t ring_hi, int depth)
{
    uint32_t pos = from, budget = 8u << 20, ret_to = 0;
    int in_call = 0;
    while (budget--) {
        uint32_t word;
        if (!in_call && pos + 4 > to) break;
        if (pos < 0x1000 || pos >= 0x08000000u) break;
        word = MEM32(pos); pos += 4;
        if (word == 0) continue;
        if ((word & 0xE0000003u) == 0x20000000u || (word & 3u) == 1u) {      /* jump */
            uint32_t tgt = ((word & 3u) == 1u) ? (word & ~3u) : (word & 0x1FFFFFFCu);
            tgt &= 0x07FFFFFFu;
            s_pb_ctl[0]++;
            if (s_pb_ctl_logged < 24) {
                s_pb_ctl_logged++;
                fprintf(stderr, "[PB] jump at %08X -> %08X (ring %08X-%08X, span end %08X, call %d)\n",
                        pos - 4, tgt, ring_lo, ring_hi, to, in_call);
            }
            if (!in_call && tgt >= ring_lo && tgt < ring_hi) {
                if (tgt <= pos - 4) break;          /* wrap to the ring start: span ends */
                pos = tgt;                          /* skip forward inside the ring */
                continue;
            }
            pos = tgt;                              /* into another buffer (or back) */
            continue;
        }
        if ((word & 3u) == 2u) {                                                /* call */
            s_pb_ctl[1]++;
            if (s_pb_ctl_logged < 24) {
                s_pb_ctl_logged++;
                fprintf(stderr, "[PB] call at %08X -> %08X\n", pos - 4, word & 0x07FFFFFCu);
            }
            if (in_call) break;                     /* the NV2A has one return slot */
            ret_to = pos; in_call = 1;
            pos = word & 0x07FFFFFCu;
            continue;
        }
        if (word == 0x00020000u) {                                              /* return */
            s_pb_ctl[2]++;
            if (in_call) { in_call = 0; pos = ret_to; continue; }
            continue;
        }
        {
            uint32_t kind = word & 0xE0030003u;
            if (kind == 0 || kind == 0x40000000u) {      /* increasing / non-increasing */
                uint32_t count = (word >> 18) & 0x7FF, method = word & 0x1FFC;
                uint32_t sub = (word >> 13) & 7, i;
                if (count == 0) continue;
                if (!in_call && pos + count * 4 > to) break;
                for (i = 0; i < count; i++) {
                    uint32_t param = MEM32(pos); pos += 4;
                    uint32_t m = kind == 0 ? method + i * 4 : method;
                    if (m == 0x130) s_pb4134_flip = 1;   /* NV097_FLIP_STALL, end of D3DDevice_Swap */
                    if (translate) pgraph_d3d11_method((int)sub, m, param);
                }
            } else {
                s_pb_ctl[3]++;                       /* unknown control word */
            }
        }
    }
    (void)depth;
}

static void pb4134_translate(uint32_t from, uint32_t to, int translate)
{
    extern uint32_t g_pb4134_ring_lo, g_pb4134_ring_hi;
    pb4134_translate_span(from, to, translate, g_pb4134_ring_lo, g_pb4134_ring_hi, 0);
}
uint32_t g_pb4134_ring_lo, g_pb4134_ring_hi;

/* End of a frame (D3DDevice_Swap's FLIP_STALL reached the "GPU"): show it,
 * pump the window, and run the vertical-blank interrupt the display would
 * raise -- CMiniport's VBlank handler (0x1E4AA0, this = dev+0x2268) bumps
 * the vblank count, completes pending flips, signals the vblank event and
 * calls the game's vertical-blank callback. 3.0 does the equivalent by hand
 * in its SetFence override. Called after KickOff has finished its own work,
 * with every guest register put back afterwards. */
void sub_001E4AA0(void);
/* 60 Hz frame pacing (3.0: the Present-wrapper pacer in
 * reference/recomp_manual_30.c). On hardware the flip waits for the vertical
 * blank, and every per-frame counter in the game follows the flips 1:1; here
 * the flip completes at once, so the game free-ran at ~500 frames/s (the
 * twenty-eighth run showed the legal screen for 8 captured frames and the
 * TECMO logo for one). Hold each flip to the next 1/60 s deadline. While
 * waiting, give the CPU to ready worker fibers -- on hardware the CRI
 * decoder/streaming threads run while the game thread waits for vblank. */
static void d3d4134_pace(void)
{
    extern int xbox_fiber_workers_ready(void);
    extern int xbox_fiber_is_primary(void);
    extern void xbox_fiber_yield(void);
    extern int doa3_workers_may_run(void);
    static LONGLONG s_qpf, s_next;
    static HANDLE s_timer;
    LARGE_INTEGER now;
    LONGLONG period;
    if (getenv("DOA3_NOPACE")) return;
    if (!s_qpf) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        s_qpf = f.QuadPart;
        s_timer = CreateWaitableTimerExW(NULL, NULL, 0x00000002 /* HIGH_RESOLUTION */,
                                         TIMER_MODIFY_STATE | SYNCHRONIZE);
        if (!s_timer)
            s_timer = CreateWaitableTimerExW(NULL, NULL, 0, TIMER_MODIFY_STATE | SYNCHRONIZE);
    }
    period = s_qpf / 60;
    QueryPerformanceCounter(&now);
    if (!s_next) s_next = now.QuadPart;
    s_next += period;
    /* overran by more than 8 frames: write the hitch off, re-anchor */
    if (now.QuadPart - s_next > period * 8) s_next = now.QuadPart;
    for (;;) {
        LONGLONG rem;
        QueryPerformanceCounter(&now);
        rem = s_next - now.QuadPart;
        if (rem <= 0) break;
        extern int xbox_fiber_is_coroutine(void);
        extern void xbox_fiber_vblank_tick(void);
        xbox_fiber_vblank_tick();
        if (rem * 1000 > s_qpf && (xbox_fiber_is_primary() || xbox_fiber_is_coroutine()) &&
            doa3_workers_may_run() && xbox_fiber_workers_ready()) {
            xbox_fiber_yield();             /* a worker lap while we wait */
            continue;
        }
        if (s_timer && rem * 2000 > s_qpf) {
            LARGE_INTEGER due;
            LONGLONG wait = rem - s_qpf / 2000;
            if (wait > s_qpf / 1000) wait = s_qpf / 1000;   /* re-check workers each ms */
            due.QuadPart = -(wait * 10000000 / s_qpf);
            if (SetWaitableTimer(s_timer, &due, 0, NULL, NULL, FALSE))
                WaitForSingleObject(s_timer, 20);
            else
                Sleep(0);
        } else {
            YieldProcessor();
        }
    }
}

/* Frame captures by presented-frame number: DOA3_CAPTURE_FRAMES="a-b,c-d"
 * writes half-size frame_NNNN.bmp of each frame in the ranges, just before it
 * is presented. The default ranges bracket the two one-frame glitches seen on
 * the boot screens (legal text brightening, a box around the TECMO logo),
 * located relative to the draw-count captures: frame_at30 + ~206 and
 * frame_at400 + ~111 (the [CAPTURE] "draw N = presented frame M" lines map
 * those). DOA3_CAPTURE_FRAMES=0 turns it off. */
extern unsigned g_doa3_frames_presented;   /* nv2a_pgraph_d3d11.c */
static void d3d4134_capture_frame(unsigned frame)
{
    static int s_init, s_n;
    static unsigned s_lo[8], s_hi[8];
    int k;
    if (!s_init) {
        const char *e = getenv("DOA3_CAPTURE_FRAMES");
        s_init = 1;
        if (e && !strcmp(e, "0")) e = "";
        if (e) {
            while (*e && s_n < 8) {
                char *end; unsigned a = strtoul(e, &end, 10), b = a;
                if (end == e) break;
                if (*end == '-') { e = end + 1; b = strtoul(e, &end, 10); }
                s_lo[s_n] = a; s_hi[s_n] = b; s_n++;
                e = (*end == ',') ? end + 1 : end;
            }
        }
    }
    {   /* default: relative to the frames where draw 30 / draw 400 were captured */
        extern unsigned g_doa3_frame_at_draw30, g_doa3_frame_at_draw400;
        static int s_rel0, s_rel1;
        if (0) {   /* replaced by the automatic glitch finder (doa3_frame_monitor) */
            if (!s_rel0 && g_doa3_frame_at_draw30) {
                s_rel0 = 1;
                s_lo[s_n] = g_doa3_frame_at_draw30 - 1 + 200; s_hi[s_n] = s_lo[s_n] + 12;
                fprintf(stderr, "[CAPTURE] will capture frames %u-%u\n", s_lo[s_n], s_hi[s_n]);
                s_n++;
            }
            if (!s_rel1 && g_doa3_frame_at_draw400) {
                s_rel1 = 1;
                s_lo[s_n] = g_doa3_frame_at_draw400 - 1 + 105; s_hi[s_n] = s_lo[s_n] + 12;
                fprintf(stderr, "[CAPTURE] will capture frames %u-%u\n", s_lo[s_n], s_hi[s_n]);
                s_n++;
            }
        }
    }
    {   int q;
        for (q = 0; q < 8; q++)
            if (g_doa3_mv_shot[q] && frame == g_doa3_mv_shot[q]) {
                extern void doa3_capture_backbuffer(const char *path);
                char path[64];
                g_doa3_mv_shot[q] = 0;
                sprintf(path, "mvend_%05u.bmp", frame);
                doa3_capture_backbuffer(path);
            }
    }
    if (g_doa3_aa_shot && frame == g_doa3_aa_shot) {
        extern void doa3_capture_backbuffer(const char *path);
        char path[64];
        sprintf(path, "aa_%05u.bmp", frame);
        doa3_capture_backbuffer(path);
    }
    for (k = 0; k < s_n; k++)
        if (frame >= s_lo[k] && frame <= s_hi[k]) {
            extern void doa3_capture_backbuffer(const char *path);
            extern int g_doa3_capture_shift;
            char path[64];
            sprintf(path, "frame_%04u.bmp", frame);
            g_doa3_capture_shift = 1;
            doa3_capture_backbuffer(path);
            g_doa3_capture_shift = 0;
            return;
        }
}

/* Method trace of chosen frames: DOA3_MTRACE="a-b,c-d" (default the
 * legal-text brightening at frame 308 and the TECMO logo fade-out, 590-612)
 * writes mtrace_NNNN.txt with every NV2A method of frame NNNN. Called at
 * each flip with the number of the frame that comes next. =0 turns it off. */
static void doa3_mtrace_frame(unsigned next)
{
    extern FILE *g_doa3_mtrace;
    extern void pgraph_mtrace_flush_run(void);
    static int s_init, s_n;
    static unsigned s_lo[8], s_hi[8];
    int k;
    if (!s_init) {
        const char *e = getenv("DOA3_MTRACE");
        s_init = 1;
        if (!e) e = "305-310,590-612";
        while (*e && s_n < 8) {
            char *end; unsigned a = strtoul(e, &end, 10), b = a;
            if (end == e) break;
            if (*end == '-') { e = end + 1; b = strtoul(e, &end, 10); }
            if (a) { s_lo[s_n] = a; s_hi[s_n] = b; s_n++; }
            e = (*end == ',') ? end + 1 : end;
        }
    }
    if (g_doa3_mtrace) { pgraph_mtrace_flush_run(); fclose(g_doa3_mtrace); g_doa3_mtrace = NULL; }
    for (k = 0; k < s_n; k++)
        if (next >= s_lo[k] && next <= s_hi[k]) {
            char path[64];
            sprintf(path, "mtrace_%04u.txt", next);
            g_doa3_mtrace = fopen(path, "w");
            return;
        }
    /* Frame-buffer sampling effects (Omega's after-image / blur): trace and
     * screenshot the next frame the first time one is seen, then every 10 s
     * while they continue (at most 6). */
    {   extern volatile int g_doa3_fbsample_seen;
        static int s_cnt; static unsigned s_last;
        if (g_doa3_fbsample_seen) {
            g_doa3_fbsample_seen = 0;
            if (s_cnt < 6 && (!s_last || next - s_last >= 600)) {
                char path[64];
                s_cnt++; s_last = next;
                g_doa3_aa_shot = next;
                sprintf(path, "mtrace_fb_%05u.txt", next);
                g_doa3_mtrace = fopen(path, "w");
                fprintf(stderr, "[FBFX] frame-buffer sampling effect: tracing frame %u -> %s + aa_%05u.bmp\n", next, path, next);
                return;
            }
        }
    }
    /* Movie skipped or over: trace and screenshot a few frames after the host
     * presenter hands the screen back (the missing fade after mv_op). */
    {   extern int doa3_movie_host_owns_screen(void);
        static int s_prev, s_cnt; static unsigned s_base;
        static const unsigned s_off[5] = { 1, 6, 15, 30, 60 };
        int own = doa3_movie_host_owns_screen(), q;
        if (s_prev && !own && s_cnt < 4) {
            s_base = next; s_cnt++;
            fprintf(stderr, "[MVEND] movie released the screen at frame %u: tracing +1,+6,+15,+30,+60\n", next);
        }
        s_prev = own;
        if (s_base)
            for (q = 0; q < 5; q++)
                if (next == s_base + s_off[q]) {
                    char path[64];
                    g_doa3_mv_shot[q] = next;
                    sprintf(path, "mtrace_mvend_%05u.txt", next);
                    g_doa3_mtrace = fopen(path, "w");
                    if (q == 4) s_base = 0;
                    return;
                }
    }
    /* Multisampled screens (character select): trace and screenshot one frame
     * two seconds after each switch into 2x2 supersampling (at most 6). */
    {   extern uint32_t doa3_aa_resolve_source(void);
        static int s_prev, s_cnt; static unsigned s_at;
        int on = doa3_aa_resolve_source() != 0;
        if (on && !s_prev && s_cnt < 6) { s_at = next + 120; s_cnt++; }
        s_prev = on;
        if (s_at && next == s_at) {
            char path[64];
            s_at = 0;
            g_doa3_aa_shot = next;
            sprintf(path, "mtrace_aa_%05u.txt", next);
            g_doa3_mtrace = fopen(path, "w");
            fprintf(stderr, "[AA] tracing frame %u (multisampled screen) -> %s + aa_%05u.bmp\n", next, path, next);
            return;
        }
    }
    /* In-engine screens: two consecutive frames every 25 s past frame 1500
     * (at most 40 files), to compare what changes between frames of the
     * same scene -- the 3D geometry differs from frame to frame. */
    {   static int s_auto;
        if (next > 1500 && (next % 1500u) <= 3 && s_auto < 48) {   /* four consecutive frames */
            char path[64];
            s_auto++;
            sprintf(path, "mtrace_%05u.txt", next);
            g_doa3_mtrace = fopen(path, "w");
        }
    }
}

static void d3d4134_frame_done(uint32_t dev)
{
    extern void pgraph_d3d11_flush(void);
    extern void doa3_present_frame(void);
    static int s_frames;
    uint32_t sv_eax = eax, sv_ecx = ecx, sv_edx = edx, sv_ebx = ebx;
    uint32_t sv_esi = esi, sv_edi = edi, sv_esp = esp, sv_seh = g_seh_ebp;
    pgraph_d3d11_flush();
    d3d4134_capture_frame(++g_doa3_frames_presented);
    { extern void doa3_frame_monitor(unsigned frame); doa3_frame_monitor(g_doa3_frames_presented); }
    doa3_mtrace_frame(g_doa3_frames_presented + 1);
    { void doa3_adx_frame(unsigned frame); doa3_adx_frame(g_doa3_frames_presented); }
    doa3_present_frame();
    d3d4134_pace();
    { extern void doa3_fn_profile_tick(void); doa3_fn_profile_tick(); }
    eax = sv_eax; ecx = sv_ecx; edx = sv_edx; ebx = sv_ebx;    /* the worker laps */
    esi = sv_esi; edi = sv_edi; esp = sv_esp; g_seh_ebp = sv_seh;
    if (++s_frames <= 3 || (s_frames % 600) == 0)
        fprintf(stderr, "[PB] frame %d presented\n", s_frames);
    ecx = dev + 0x2268;
    PUSH32(esp, 0);
    sub_001E4AA0();
    eax = sv_eax; ecx = sv_ecx; edx = sv_edx; ebx = sv_ebx;
    esi = sv_esi; edi = sv_edi; esp = sv_esp; g_seh_ebp = sv_seh;
}

void sub_001E27C0_gen(void);
void sub_001E27C0(void)
{
    static int s_on = -1, s_log;
    uint32_t dev = ecx, flags = MEM32(dev + 8), cursor, start, end, sem;
    if (s_on < 0) { const char *e = getenv("DOA3_PB"); s_on = !(e && *e == '0'); }
    if (flags & 0x2000) {            /* no live GPU channel: the original only does bookkeeping */
        sub_001E27C0_gen();
        return;
    }
    cursor = (flags & 4) ? MEM32(dev + 0x35C) : MEM32(dev);
    start = MEM32(dev + 0x24); end = MEM32(dev + 0x28);
    g_pb4134_ring_lo = start; g_pb4134_ring_hi = end + 4;
    if (cursor >= start && cursor <= end) {
        if (s_pb4134_parsed > cursor && s_pb4134_parsed <= end) {
            /* the ring wrapped: finish the old lap up to its jump back to
             * the start (the span stops there), then go on from the start */
            pb4134_translate(s_pb4134_parsed, end + 4, s_on);
            s_pb4134_parsed = start;
        }
        if (s_pb4134_parsed < start || s_pb4134_parsed > cursor)
            s_pb4134_parsed = start;                 /* first kick */
        if (cursor > s_pb4134_parsed) pb4134_translate(s_pb4134_parsed, cursor, s_on);
        s_pb4134_parsed = cursor;
    }
    if (s_log < 8) {
        s_log++;
        fprintf(stderr, "[PB] kickoff #%d dev=%08X cursor=%08X ring=%08X-%08X fence=%08X\n",
                s_log, dev, cursor, start, end, MEM32(dev + 0x30));
    }
    {   static DWORD s_dnext;
        extern unsigned g_doa3_drop[4];
        if (GetTickCount() >= s_dnext && g_doa3_frames_presented > 1500) {
            s_dnext = GetTickCount() + 5000;
            fprintf(stderr, "[DRAW] frame %u, last 5 s of array draws: drawn %u, dropped (non-finite vertex) %u (%u verts), clipped away %u\n",
                    g_doa3_frames_presented, g_doa3_drop[2], g_doa3_drop[0], g_doa3_drop[3], g_doa3_drop[1]);
            g_doa3_drop[0] = g_doa3_drop[1] = g_doa3_drop[2] = g_doa3_drop[3] = 0;
        }
    }
    {   static DWORD s_next;
        if (GetTickCount() >= s_next) {
            s_next = GetTickCount() + 10000;
            fprintf(stderr, "[PB] control words so far: jump %u call %u return %u unknown %u\n",
                    s_pb_ctl[0], s_pb_ctl[1], s_pb_ctl[2], s_pb_ctl[3]);
            {   extern void xbox_HeapStats(uint32_t *, uint32_t *, uint32_t *, uint32_t *,
                                           uint32_t *, uint32_t *, uint32_t *, uint32_t *);
                uint32_t bu, lim, ln, lb, fn, fb, big, oom;
                xbox_HeapStats(&bu, &lim, &ln, &lb, &fn, &fb, &big, &oom);
                fprintf(stderr, "[HEAP] bump %u KB of %u KB, live %u blocks %u KB, free %u blocks %u KB, largest %u KB, failures %u\n",
                        bu >> 10, lim >> 10, ln, lb >> 10, fn, fb >> 10, big >> 10, oom);
            }
        }
    }
    {   uint32_t chan = MEM32(dev + 0x2264);
        if (chan) {
            MEM32(chan + 0x40) = cursor & 0x3FFFFFFu;   /* DMA_PUT */
            MEM32(chan + 0x44) = cursor & 0x3FFFFFFu;   /* DMA_GET: consumed */
        }
    }
    MEM32(dev + 0x2C) = cursor;
    sem = MEM32(dev + 0x34);
    if (sem) MEM32(sem) = MEM32(dev + 0x30) - 2;     /* every fence inserted so far is done */
    esp += 4;                                        /* fastcall, ret 0 */
    if (s_pb4134_flip) {
        s_pb4134_flip = 0;
        d3d4134_frame_done(dev);
    }
}
/* ── XAPI fibers (3.1) ──────────────────────────────────────────────
 * Same as 3.0 (reference/recomp_manual_30.c, 0x164F50/0x164FDC/0x164FEF):
 * the lifted SwitchToFiber swaps the guest esp, but its `ret` is a C return,
 * so control never reaches the other fiber -- the eleventh run logged
 * SwitchToFiber "returning" with esp on another fiber's stack. Back
 * CreateFiber / DeleteFiber / SwitchToFiber with the host coroutines in
 * xbox_fiber.c; handles are 0xF1BE0000|index, and any other handle (the
 * ConvertThreadToFiber main fiber) means "switch back to the dispatcher". */
#define XFIBER_TAG_4134 0xF1BE0000u
extern int  xbox_fiber_create_dormant(uint32_t routine_va, uint32_t param, uint32_t stack_size);
extern void xbox_fiber_destroy(int idx);
extern void xbox_fiber_switch_direct(int idx);
extern void xbox_fiber_yield_back(void);

void sub_0018C934(void)          /* CreateFiber(stack, routine, param), stdcall ret 12 */
{
    int idx = xbox_fiber_create_dormant(MEM32(esp + 8), MEM32(esp + 12), MEM32(esp + 4));
    eax = (idx > 0) ? (XFIBER_TAG_4134 | (uint32_t)idx) : 0;
    esp += 16;
}

void sub_0018C9C0(void)          /* DeleteFiber(handle), stdcall ret 4 */
{
    uint32_t h = MEM32(esp + 4);
    if ((h & 0xFFFF0000u) == XFIBER_TAG_4134) xbox_fiber_destroy((int)(h & 0xFFFF));
    esp += 8;
}

void sub_0018C9D3(void)          /* SwitchToFiber(handle), stdcall ret 4 */
{
    uint32_t h = MEM32(esp + 4);
    esp += 8;                    /* finish the call before transferring */
    if ((h & 0xFFFF0000u) == XFIBER_TAG_4134) xbox_fiber_switch_direct((int)(h & 0xFFFF));
    else xbox_fiber_yield_back();
}
/* DirectSound effects-image download (0xA5C60; 0x9F640 on 3.0). The audio
 * init sub_000A5DC0 runs DirectSoundCreate, then this (dsstdfx.bin into the
 * DSP), then the CRI middleware setup (sub_001C79F0: ADX threads, the AFS
 * partitions). The real download fails here, and on failure the init skips
 * the CRI setup entirely -- no partitions, so the first task's AFS open loop
 * (sub_00085C40) retried forever ("'ptid' is range outside"). The effects
 * image is not needed; report success, as 3.0 does. Register argument (eax =
 * file name), ret 0. */
void sub_000A5C60(void) { eax = 0; esp += 4; }

/* CRT memcpy/memmove (0x1B73D0 and 0x1B7CB0, the same two copies 3.0 has at
 * 0x18DF40/0x18EE90): the trailing-byte dispatch is an intra-function jump
 * table the lifter cannot follow, so copies lost their tail bytes. Native
 * memmove. cdecl(dst, src, n) -> eax = dst. */
static void crt4134_memmove(void)
{
    uint32_t dst = MEM32(esp + 4), src = MEM32(esp + 8), n = MEM32(esp + 12);
    if (n) memmove((void *)((uintptr_t)dst + g_xbox_mem_offset),
                   (void *)((uintptr_t)src + g_xbox_mem_offset), n);
    eax = dst;
    esp += 4;
}
void sub_001B73D0(void) { crt4134_memmove(); }
void sub_001B7CB0(void) { crt4134_memmove(); }

/* CRI idle/watchdog thread (0x1926B0; 3.0: 0x16A530). The original spins
 * incrementing the liveness counter [0x40C700] until the shutdown flag
 * [0x40C718] is set; on hardware it runs at idle priority and is preempted.
 * With cooperative fibers the spin starved everything else (the thirteenth
 * run: CRI started its four threads and this one never let go). Yield every
 * lap, as 3.0 does. */
extern void xbox_fiber_yield(void);
void sub_001926B0(void)
{
    while (MEM32(0x0040C718u) == 0) {
        MEM32(0x0040C700u) = MEM32(0x0040C700u) + 1;
        xbox_fiber_yield();
    }
    MEM32(0x0040C71Cu) = 1;
    esp += 4;
}

/* CRI file server tick (0x191D60; 3.0: 0x170710): registered as ADXM user
 * server group 2 when [0x25478C] == 1, then run by the vsync thread after
 * each vertical blank. Counted for the watchdog. */
extern int xbox_fiber_current(void);
static unsigned s_crifs_ticks;
void sub_00191D60_gen(void);
void sub_00191D60(void)
{
    if (s_crifs_ticks++ == 0) { fprintf(stderr, "[CRI] file server first tick (fiber %d)\n", xbox_fiber_current()); fflush(stderr); }
    sub_00191D60_gen();
}

/* ADX stereo decoder (0x19DC30, reached through a pointer). It wrote PCM
 * through guest 0x0825xxxx, past the end of guest RAM, which wraps onto the
 * game's .data. Log its arguments: the first 8 calls, then every call with an
 * argument past guest RAM. */
void doa3_fn_trace(uint32_t va, uint32_t esp_in);
void sub_0019DC30_gen(void);
void sub_0019DC30(void)
{
    /* traced on entry: the decoder reuses its argument slots as locals */
    if (g_fn_trace_on) doa3_fn_trace(0x0019DC30u, esp);
    sub_0019DC30_gen();
}

/* Sofdec frame copy + colour conversion (0x19EA30; 3.0's 0x1762B0, same
 * code). Args: decoded picture, destination surface, width, height, ...
 * On Xbox the converted frame goes straight to the display and the game does
 * not Present while a movie plays, so show it from here
 * (doa3_present_movie_guest, movie_present.c). */
void sub_0019EA30_gen(void);
void sub_0019EA30(void)
{
    extern void doa3_present_movie_guest(const void *src, int w, int h, int pitch);
    static int s_n;
    uint32_t a[6]; int k;
    for (k = 0; k < 6; k++) a[k] = MEM32(esp + 4 + 4u * k);
    sub_0019EA30_gen();
    if (++s_n <= 6 || (s_n % 60) == 0) {
        fprintf(stderr, "[MOVIE] frame copy #%d (", s_n);
        fprintf(stderr, "%08X, %08X, %u, %u, %08X, %08X)\n",
                a[0], a[1], a[2], a[3], a[4], a[5]);
        fflush(stderr);
    }
    {
        uint32_t dst = a[1] & 0x07FFFFFFu;
        if (dst >= 0x1000u && dst + 2880u * 480u < 0x08000000u) {
            if (s_n == 30 || s_n == 200) {
                /* What the guest decoded and converted: the 720x480 32bpp
                 * destination as movie_NNN.bmp, plus how much of it is lit. */
                char path[64]; FILE *f; unsigned nz = 0, k, y, x;
                for (k = 0; k < 2880u * 480u; k += 64) nz += MEM8(dst + k) != 0;
                fprintf(stderr, "[MOVIE] frame copy #%d: %u of %u sampled bytes non-zero\n",
                        s_n, nz, 2880u * 480u / 64u);
                sprintf(path, "movie_%03d.bmp", s_n);
                f = fopen(path, "wb");
                if (f) {
                    uint8_t hdr[54] = { 'B', 'M' };
                    uint32_t size = 54u + 720u * 480u * 3u, v;
                    memcpy(hdr + 2, &size, 4); v = 54; memcpy(hdr + 10, &v, 4);
                    v = 40; memcpy(hdr + 14, &v, 4); v = 720; memcpy(hdr + 18, &v, 4);
                    v = 480; memcpy(hdr + 22, &v, 4); hdr[26] = 1; hdr[28] = 24;
                    fwrite(hdr, 1, 54, f);
                    for (y = 480; y-- > 0;)
                        for (x = 0; x < 720; x++) {
                            uint32_t p = dst + y * 2880u + x * 4u;
                            uint8_t bgr[3] = { MEM8(p), MEM8(p + 1), MEM8(p + 2) };
                            fwrite(bgr, 1, 3, f);
                        }
                    fclose(f);
                }
            }
            doa3_present_movie_guest((const void *)XBOX_PTR(dst), 720, 480, 2880);
        }
    }
}

/* Watchdog peek: the state the 3.1 boot is waiting on. */
void doa3_wdog_peek(void)
{
    uint32_t pt = MEM32(0x00C7E1A0u);
    fprintf(stderr, "[PEEK] adxm_thread_mode[0x25478C]=%u grp2fn[0x40CDF0]=%08X fs_busy[0x40C5C4]=%u "
            "fs_ticks=%u pt_state=%08X(stat %d) grp5fn[0x40CE30]=%08X\n",
            MEM32(0x0025478Cu), MEM32(0x0040CDF0u), MEM32(0x0040C5C4u), s_crifs_ticks,
            pt, pt ? (int)(int8_t)MEM8(pt + 1) : -99, MEM32(0x0040CE30u));
}

/* CRI middleware message sink (0x19A330, cdecl: formats into 0xC75500 and
 * hands it to the registered callback). The ADX/Sofdec error reporters
 * (0x1934D0 / 0x193510) end here, so log what the middleware says. */
void sub_0019A330_gen(void);
void sub_0019A330(void)
{
    static int s_n;
    uint32_t fmt = MEM32(esp + 4);
    s_n++;
    if (s_n <= 40 || (s_n & (s_n - 1)) == 0) {
        char buf[200]; int i;
        for (i = 0; i < 199; i++) {
            uint8_t c = fmt ? MEM8(fmt + i) : 0;
            if (!c) break;
            buf[i] = (c >= 0x20 && c < 0x7F) ? (char)c : '?';
        }
        buf[i] = 0;
        fprintf(stderr, "[CRI] message #%d: %s\n", s_n, buf);
        fflush(stderr);
    }
    sub_0019A330_gen();
}

/* DirectSound's voice retire wait (0x1F5157, ecx = voice): if the voice
 * is active (+0x12 bit 0), spin until the APU interrupt's DPC has retired it
 * (+0x12 bit 0x8000 clear). On the Xbox the interrupt arrives during the
 * spin. Here nothing interrupts recompiled code, and fix_selfspins had
 * removed the loop, so a voice was destroyed while still linked into the
 * APU's active list (+0x6C4): its node stayed there with the destructed
 * object's base vtable 0x218090, the DPC then called slot 0x14 (AddRef) on
 * it every interrupt, and when its memory was reused the list looped onto
 * itself -- the crash at the end of mv_op.sfd. Deliver the interrupt from
 * inside the wait, as 3.0 does (doa3_apu_wait_retire), keeping the guest
 * registers. */
void sub_001F5157(void)
{
    uint32_t v = ecx;
    uint32_t r_eax = eax, r_ecx = ecx, r_edx = edx, r_ebx = ebx, r_esi = esi,
             r_edi = edi, r_esp = esp, r_seh = g_seh_ebp;
    DWORD t0 = GetTickCount(), tlog = 0;
    static int s_n;
    if ((MEM8(v + 0x12) & 1) && (MEM16(v + 0x12) & 0x8000)) {
        while (MEM16(v + 0x12) & 0x8000) {
            doa3_apu_deliver_irq();
            eax = r_eax; ecx = r_ecx; edx = r_edx; ebx = r_ebx; esi = r_esi;
            edi = r_edi; esp = r_esp; g_seh_ebp = r_seh;
            if (!(MEM16(v + 0x12) & 0x8000)) break;
            {   extern volatile int g_apu_det;
                extern void mcpx_apu_det_step(void);
                if (g_apu_det) { mcpx_apu_det_step(); continue; }
            }
            Sleep(1);
            if (GetTickCount() - t0 > 1000 + tlog) {
                tlog += 2000;
                fprintf(stderr, "[DSRETIRE] voice %08X still not retired after %lu ms (flags %04X)\n",
                        v, (unsigned long)(GetTickCount() - t0), MEM16(v + 0x12));
                fflush(stderr);
            }
        }
        if (s_n++ < 8) {
            fprintf(stderr, "[DSRETIRE] voice %08X retired after %lu ms\n", v, (unsigned long)(GetTickCount() - t0));
            fflush(stderr);
        }
    }
    esp += 4;   /* ret */
}

/* DirectSound's DPC voice service (0x1F3FA0, ecx = the APU object): walks
 * three voice lists at +0x6C4/+0x6CC/+0x6D4 calling [obj->vtbl+0x14] on
 * obj = node-0x4C, then drains +0x6DC (obj = node-0x54). On 3.1, right after
 * mv_op.sfd, one node's object had a vtable whose slot 0x14 is AddRef
 * 0x1F1816 (a COM vtable, 0x218090/0x2180B4), so the "service" call read
 * a garbage `this` and wrote into host memory. Log every change to the lists
 * (frame, node, object, vtable, slot 0x14) so the bad node's arrival shows. */
void sub_001F3FA0_gen(void);
void sub_001F3FA0(void)
{
    enum { MAXN = 64 };
    static uint32_t s_prev[4][MAXN]; static int s_prevn[4];
    static int s_logs;
    uint32_t apu = ecx, k;
    if (apu >= 0x1000 && apu < 0x08000000u && s_logs < 200) {
        for (k = 0; k < 4; k++) {
            uint32_t head = apu + 0x6C4 + k * 8, node = MEM32(head), cur[MAXN];
            int n = 0, i, j, changed = 0;
            uint32_t off = (k < 3) ? 0x4C : 0x54;
            while (node != head && node >= 0x1000 && node < 0x08000000u && n < MAXN) {
                cur[n++] = node;
                node = MEM32(node);
            }
            if (n != s_prevn[k]) changed = 1;
            for (i = 0; i < n && !changed; i++) if (cur[i] != s_prev[k][i]) changed = 1;
            if (changed || (node != head)) {
                s_logs++;
                fprintf(stderr, "[DSVOICE] frame %u list %u (head %08X)%s: %d nodes:", g_doa3_frames_presented,
                        k, head, node != head ? " BROKEN" : "", n);
                for (i = 0; i < n; i++) {
                    uint32_t obj = cur[i] - off, vt = MEM32(obj);
                    int seen = 0;
                    for (j = 0; j < s_prevn[k]; j++) if (s_prev[k][j] == cur[i]) seen = 1;
                    fprintf(stderr, " %s%08X(vt %08X s14 %08X)", seen ? "" : "+", cur[i], vt,
                            (vt >= 0x1000 && vt < 0x08000000u) ? MEM32(vt + 0x14) : 0);
                }
                if (node != head) fprintf(stderr, " ->%08X", node);
                fprintf(stderr, "\n");
                for (i = 0; i < n; i++) {
                    uint32_t obj = cur[i] - off, vt = MEM32(obj), w;
                    if (vt >= 0x1000 && vt < 0x08000000u && MEM32(vt + 0x14) == 0x1F1816u) {
                        fprintf(stderr, "[DSVOICE]   node %08X obj %08X looks like a COM object:", cur[i], obj);
                        for (w = 0; w < 0x80; w += 4)
                            fprintf(stderr, "%s%08X", (w % 32) ? " " : "\n[DSVOICE]     ", MEM32(obj + w));
                        fprintf(stderr, "\n");
                    }
                }
                fflush(stderr);
            }
            memcpy(s_prev[k], cur, n * sizeof(uint32_t));
            s_prevn[k] = n;
        }
    }
    sub_001F3FA0_gen();
}

/* The movie's ADX decode chain at the end of the movie. ADXT [0xC86480+0x3E00]
 * -> ADXSJD [adxt+4] (0x9C-byte slots from 0xC7BC80): +1 state (3 = decode
 * end, which ADXT's error check 0x196300 needs once the input runs dry),
 * +3 supply-ended, +8 input stream joint, +0x14/+0x18 current chunk,
 * +0x2C decoded samples, +0x34; decoder [sjd+4]: +0x10, +0x18, +0xA8 status.
 * 0x1952E0 sets decode end on: supply ended and input empty; decoder idle
 * and the next two bytes are the 0x8001 ADX end block; or [sjd+0x34] >=
 * [dec+0x18]. It only runs while the decoder [sjd+4] is idle (0x19B8F0). */
static void doa3_adx_dump(const char *why)
{
    uint32_t adxt = MEM32(0xC86480u + 0x3E00), sjd, dec, sj, ck, k;
    if (adxt < 0x1000 || adxt >= 0x08000000u) return;
    sjd = MEM32(adxt + 4);
    if (sjd < 0x1000 || sjd >= 0x08000000u) return;
    dec = MEM32(sjd + 4); sj = MEM32(sjd + 8); ck = MEM32(sjd + 0x14);
    fprintf(stderr, "[ADXSJD] %s frame %u: adxt state %u sjd %08X state %u supply-end %u decoded %u lim34 %d chunk %08X+%d",
            why, g_doa3_frames_presented, MEM8(adxt + 1), sjd, MEM8(sjd + 1), MEM8(sjd + 3),
            MEM32(sjd + 0x2C), (int)MEM32(sjd + 0x34), ck, (int)MEM32(sjd + 0x18));
    if (ck >= 0x1000 && ck < 0x08000000u)
        fprintf(stderr, " [%02X %02X %02X %02X]", MEM8(ck), MEM8(ck + 1), MEM8(ck + 2), MEM8(ck + 3));
    if (dec >= 0x1000 && dec < 0x08000000u)
        fprintf(stderr, " dec %08X +10=%d +18=%d +A8=%d", dec, (int)MEM32(dec + 0x10),
                (int)MEM32(dec + 0x18), (int)(int16_t)MEM16(dec + 0xA8));
    fprintf(stderr, "\n");
    if (!strcmp(why, "error")) {
        fprintf(stderr, "[ADXSJD] sjd:");
        for (k = 0; k < 0xA0; k += 4) fprintf(stderr, "%s%08X", (k % 32) ? " " : "\n[ADXSJD]   ", MEM32(sjd + k));
        if (dec >= 0x1000 && dec < 0x08000000u) {
            fprintf(stderr, "\n[ADXSJD] dec %08X:", dec);
            for (k = 0; k < 0xC0; k += 4) fprintf(stderr, "%s%08X", (k % 32) ? " " : "\n[ADXSJD]   ", MEM32(dec + k));
        }
        if (sj >= 0x1000 && sj < 0x08000000u) {
            fprintf(stderr, "\n[ADXSJD] input sj %08X:", sj);
            for (k = 0; k < 0x60; k += 4) fprintf(stderr, "%s%08X", (k % 32) ? " " : "\n[ADXSJD]   ", MEM32(sj + k));
        }
        fprintf(stderr, "\n");
    }
    fflush(stderr);
}

/* Once the movie's input has ended ([h+0x43D4] substream +0xF94), one
 * [ADXSJD] line every 15 frames until the ADX side ends or errors. */
void doa3_adx_frame(unsigned frame)
{
    static unsigned s_lines;
    const uint32_t h = 0xC86480u;
    uint32_t ia = MEM32(h + 0x43D4u), ib = MEM32(h + 0x43D8u);
    if (ia >= 16 || ib >= 16 || s_lines >= 80) return;
    if (MEM32(h + ia * 0x388u + 0xF94u) != 1 || MEM32(h + ib * 0x388u + 0xF94u) == 1) return;
    if (frame % 15) return;
    s_lines++;
    doa3_adx_dump("tick");
}

/* CRI error sink for formatted messages (0x19A380; the Sofdec/mwPly error
 * formatter 0x1A0040 lands here). The intro movie's handle went to state -4
 * (error) right after one of these, mid-movie. */
void sub_0019A380_gen(void);
void sub_0019A380(void)
{
    static int s_n;
    uint32_t msg = MEM32(esp + 4);
    s_n++;
    if (s_n <= 60 || (s_n & (s_n - 1)) == 0) {
        char buf[256]; int i;
        for (i = 0; i < 255; i++) {
            uint8_t c = msg ? MEM8(msg + i) : 0;
            if (!c) break;
            buf[i] = (c >= 0x20 && c < 0x7F) ? (char)c : '?';
        }
        buf[i] = 0;
        fprintf(stderr, "[CRI] error #%d (fiber %d, frame %u): %s\n", s_n, xbox_fiber_current(),
                g_doa3_frames_presented, buf);
        fflush(stderr);
        if (strstr(buf, "FF000C0")) {
            /* The movie's ADX link (sfd handle 0xC86480 + 0x3E00): ADXT
             * object, its decoder [+4] and input stream-joint [+0x14]. */
            uint32_t sfd = 0xC86480u, adxt = MEM32(sfd + 0x3E00), k;
            fprintf(stderr, "[ADXT] sfd state %d, adxt %08X:", (int)MEM32(sfd + 0x40), adxt);
            if (adxt >= 0x1000 && adxt < 0x08000000u)
                for (k = 0; k < 0x80; k += 4) fprintf(stderr, "%s%08X", (k % 32) ? " " : "\n[ADXT]   ", MEM32(adxt + k));
            fprintf(stderr, "\n");
            doa3_adx_dump("error");
            fflush(stderr);
        }
    }
    sub_0019A380_gen();
}
#else
void doa3_wdog_peek(void) { }
#endif /* DOA3_XBE_ID_3_1 */

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

/* ── Guest-stack balance reports (gen/recomp_probes.c) ──────────────
 * A wrapped function returned with esp somewhere other than 4 + N bytes
 * above its entry esp. Logged in the order they happen, at most 2 per
 * function and 400 in all, so the first one in the log is the earliest
 * drift. DOA3_ESPPROBE=0 silences them. */
void esp_probe_report(uint32_t va, uint32_t esp_in, uint32_t esp_out, uint32_t expect)
{
    static int s_on = -1, s_total;
    static struct { uint32_t va; int n; } s_seen[512];
    static int s_nseen;
    extern unsigned long long xbox_kernel_call_count(void);
    int i;
    if (s_on < 0) { const char *e = getenv("DOA3_ESPPROBE"); s_on = !(e && *e == '0'); }
    if (!s_on || s_total >= 400) return;
    for (i = 0; i < s_nseen; i++) if (s_seen[i].va == va) break;
    if (i == s_nseen) {
        if (s_nseen == 512) return;
        s_seen[s_nseen].va = va; s_seen[s_nseen].n = 0; s_nseen++;
    }
    if (s_seen[i].n >= 2) return;
    s_seen[i].n++; s_total++;
    fprintf(stderr, "[ESP] sub_%08X: esp %08X -> %08X, moved %+d, expected %+u (after kernel call #%llu)\n",
            va, esp_in, esp_out, (int)(esp_out - esp_in), expect, xbox_kernel_call_count());
    fflush(stderr);
}

/* ── Function return tracing (gen/recomp_probes.c wrappers) ─────────
 * DOA3_TRACE_FN=a5c60,181640,... logs each return of those guest functions
 * (probed ones only: real entries whose rets agree) with their first four
 * stack arguments, eax, and the register arguments ecx/edx. Up to 64 lines
 * per function. */
extern int xbox_fiber_current(void);
static uint32_t s_fn_trace_va[32];
static int      s_fn_trace_n[32], s_fn_trace_cnt;
static unsigned s_fn_calls[32];
/* Return ring of every probed function (while tracing is on), so a traced
 * call can print the calls nested inside it: [NEST] lines. */
static struct { uint32_t va, e0, eax; int fib; } s_ring[8192];
static unsigned s_ring_n;
/* Crash context: the last probed-function returns, newest first, with the
 * fiber each ran on (main.c's fault handlers call this). */
void doa3_ring_dump(const char *why)
{
    unsigned j, k = 0;
    fprintf(stderr, "[RING] %s: g_esp=%08X eax=%08X ecx=%08X edx=%08X ebx=%08X esi=%08X edi=%08X fiber %d, last returns:",
            why, g_esp, g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, xbox_fiber_current());
    for (j = s_ring_n; j > 0 && k < 96; j--, k++)
        fprintf(stderr, "%s%X/%d=%X", (k % 8) ? " " : "\n[RING]   ",
                s_ring[(j - 1) & 8191].va, s_ring[(j - 1) & 8191].fib, s_ring[(j - 1) & 8191].eax);
    fprintf(stderr, "\n");
    fflush(stderr);
}
static void doa3_fn_nest_dump(uint32_t va, uint32_t e0, unsigned callno)
{
    int fib = xbox_fiber_current(), k, n = 0;
    unsigned j, stop = s_ring_n > 8192 ? s_ring_n - 8192 : 0;
    static unsigned idx[400];
    for (j = s_ring_n - 1; j > stop && n < 400; j--) {   /* skip this call's own entry */
        unsigned r = (j - 1) & 8191;
        if (s_ring[r].fib != fib) continue;
        if (s_ring[r].e0 >= e0) break;           /* returned before this call began */
        idx[n++] = r;
    }
    fprintf(stderr, "[NEST] sub_%08X call #%u -> eax=%08X, %d nested returns:", va, callno, eax, n);
    for (k = n - 1; k >= 0; k--)
        fprintf(stderr, " %X(%X)%s", s_ring[idx[k]].va, s_ring[idx[k]].eax,
                ((n - 1 - k) % 12 == 11) ? "\n[NEST]  " : "");
    fprintf(stderr, "\n");
    fflush(stderr);
}

void doa3_fn_trace(uint32_t va, uint32_t esp_in)
{
    int i;
#if defined(DOA3_XBE_ID_3_1)
    {   /* Value watch by polling at every probed return. The movie's
         * get-frame handler (0x1A4C80) delivers only while the sfd handle's
         * state [0xC86480+0x40] is 3 or 4; mid-movie it leaves those and no
         * frame is delivered again. Name the function that returned when the
         * watched words changed. */
        /* [2..5]: the end flags (+0xF90 / +0xF94, 0x388 per substream) of
         * the two substreams the ADX end check 0x1A4350 looks at,
         * [h+0x43D4] (input) and [h+0x43D8] (ADX side): when the input one
         * ends while ADXT is playing, it marks the ADX one ended. */
        /* [6..8]: the attract hand-off after the logos: 0x593038
         * (title/attract loop active), 0x598E80 (main strand state),
         * 0x5A2858 (current game mode). */
        /* [9..14]: title-attract input path -- 0x596AA5 (input blocked
         * while the stage loader runs), 0x596A32 (P1 join), 0x5A262A
         * (attract action),
         * 0x5A697C (join result). */
        static uint32_t s_wv_va[15] = { 0xC864C0u, 0xC8BA80u, 0, 0, 0, 0,
                                       0x593038u, 0x598E80u, 0x5A2858u,
                                       0x596AA4u, 0x596A30u, 0x5A2628u, 0, 0, 0x5A697Cu };
        static uint32_t s_wv_last[15];
        static int s_wv_n;
        int w;
        {
            const uint32_t h = 0xC86480u;
            uint32_t ia = MEM32(h + 0x43D4u), ib = MEM32(h + 0x43D8u);
            if (ia < 16 && ib < 16) {
                s_wv_va[2] = h + ia * 0x388u + 0xF90u; s_wv_va[3] = h + ia * 0x388u + 0xF94u;
                s_wv_va[4] = h + ib * 0x388u + 0xF90u; s_wv_va[5] = h + ib * 0x388u + 0xF94u;
            }
        }
        for (w = 0; w < 15; w++) {
            if (!s_wv_va[w]) continue;
            uint32_t v = MEM32(s_wv_va[w]);
            if (v != s_wv_last[w] && s_wv_n < 400) {
                unsigned j, k = 0;
                s_wv_n++;
                fprintf(stderr, "[VWATCH] [%08X] %08X -> %08X at return of sub_%08X (fiber %d, frame %u); recent:",
                        s_wv_va[w], s_wv_last[w], v, va, xbox_fiber_current(), g_doa3_frames_presented);
                for (j = s_ring_n; j > 0 && k < 24; j--, k++)
                    fprintf(stderr, " %X/%d", s_ring[(j - 1) & 8191].va, s_ring[(j - 1) & 8191].fib);
                fprintf(stderr, "\n");
                fflush(stderr);
            }
            s_wv_last[w] = v;
        }
    }
#endif
    if (g_fn_trace_on > 0) {
        unsigned r = s_ring_n++ & 8191;
        s_ring[r].va = va; s_ring[r].e0 = esp_in; s_ring[r].eax = eax;
        s_ring[r].fib = xbox_fiber_current();
    }
    if (g_fn_trace_on < 0) {
        const char *e = getenv("DOA3_TRACE_FN");
#if defined(DOA3_XBE_ID_3_1)
        /* Default while bringing 3.1 up: the audio/CRI init path that decides
         * whether the CRI middleware is set up at all (sub_000A5DC0). */
        /* ADX stereo decoder: it wrote PCM through guest 0x0825xxxx, which
         * wraps onto .data (the 128 MB view repeats). Whose pointer is it? */
        /* Intro movie service 0xA4AB0: get current frame (0x19E3C0), blit
         * (0xA4680 -> frame copy 0x19EA30), release frame (0x19E230). */
        if (!e) e = "19e3c0,19e230,a4680,a4ab0,1a81e0,1a8220";
#endif
        g_fn_trace_on = 0;
        while (e && *e && s_fn_trace_cnt < 32) {
            char *end;
            unsigned long v = strtoul(e, &end, 16);
            if (end == e) break;
            s_fn_trace_va[s_fn_trace_cnt++] = (uint32_t)v;
            e = (*end == ',') ? end + 1 : end;
        }
        g_fn_trace_on = s_fn_trace_cnt > 0;
        if (!g_fn_trace_on) return;
    }
    for (i = 0; i < s_fn_trace_cnt; i++) {
        uint32_t a[8]; int k, odd = 0;
        if (s_fn_trace_va[i] != va) continue;
        for (k = 0; k < 8; k++) {
            a[k] = MEM32(esp_in + 4 + 4 * k);
            if (a[k] >= 0x08000000u && a[k] < 0x10000000u) odd = 1;   /* past guest RAM */
        }
        /* the first 8 calls, then only calls with an argument past the end of
         * guest RAM (up to 64 lines in all) */
        s_fn_calls[i]++;
        if (i == 0 && (s_fn_calls[i] <= 10 || s_fn_calls[i] == 40 || s_fn_calls[i] == 300 ||
                       s_fn_calls[i] == 1000))
            doa3_fn_nest_dump(va, esp_in, s_fn_calls[i]);
        if (s_fn_trace_n[i] >= 64 || (s_fn_trace_n[i] >= 8 && !odd && (s_fn_calls[i] % 256u) != 0)) return;
        s_fn_trace_n[i]++;
        fprintf(stderr, "[FN]%s #%u sub_%08X(%08X, %08X, %08X, %08X, %08X, %08X, %08X, %08X) ecx=%08X edx=%08X -> eax=%08X fiber=%d\n",
                odd ? "[PAST-RAM]" : "", s_fn_calls[i], va, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
                ecx, edx, eax, xbox_fiber_current());
        fflush(stderr);
        return;
    }
}
