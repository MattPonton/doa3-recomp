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

static void pb4134_translate(uint32_t from, uint32_t to, int translate)
{
    uint32_t pos = from;
    while (pos + 4 <= to) {
        uint32_t word = MEM32(pos); pos += 4;
        uint32_t kind = word & 0xE0030003u;
        if (word == 0) continue;
        if (kind == 0 || kind == 0x40000000u) {      /* increasing / non-increasing */
            uint32_t count = (word >> 18) & 0x7FF, method = word & 0x1FFC;
            uint32_t sub = (word >> 13) & 7, i;
            if (count == 0 || pos + count * 4 > to) break;
            for (i = 0; i < count; i++) {
                uint32_t param = MEM32(pos); pos += 4;
                uint32_t m = kind == 0 ? method + i * 4 : method;
                if (m == 0x130) s_pb4134_flip = 1;   /* NV097_FLIP_STALL, end of D3DDevice_Swap */
                if (translate) pgraph_d3d11_method((int)sub, m, param);
            }
        }
        /* jump / call / return: no parameters to translate */
    }
}

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
    if (cursor >= start && cursor <= end) {
        if (s_pb4134_parsed < start || s_pb4134_parsed > cursor)
            s_pb4134_parsed = start;                 /* first kick, or the ring wrapped */
        if (cursor > s_pb4134_parsed) pb4134_translate(s_pb4134_parsed, cursor, s_on);
        s_pb4134_parsed = cursor;
    }
    if (s_log < 8) {
        s_log++;
        fprintf(stderr, "[PB] kickoff #%d dev=%08X cursor=%08X ring=%08X-%08X fence=%08X\n",
                s_log, dev, cursor, start, end, MEM32(dev + 0x30));
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
        static const uint32_t s_wv_va[2] = { 0xC864C0u, 0xC8BA80u };
        static uint32_t s_wv_last[2];
        static int s_wv_n;
        int w;
        for (w = 0; w < 2; w++) {
            uint32_t v = MEM32(s_wv_va[w]);
            if (v != s_wv_last[w] && s_wv_n < 60) {
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
        if (!e) e = "19e3c0,19e230,a4680,a4ab0";
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
