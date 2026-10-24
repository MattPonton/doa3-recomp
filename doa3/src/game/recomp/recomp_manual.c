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
#include "input/xinput_xbox.h"
#include "../free_camera.h"
#include "../combat_debug.h"
#include "../training_dummy.h"
#include "../debug_capture.h"
#include "../debug_settings.h"
#include "../dip_switches.h"
#include "../game_audio.h"

#include "apu/apu_irq.h"
#include "../../perf_capture.h"

extern uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment);
extern void xbox_fiber_yield(void);
extern int  xbox_fiber_active(void);
extern void xbox_fiber_wake(uint32_t event_va);

#if defined(DOA3_XBE_ID_JAPAN)
/* The input fiber decodes both players, runs commands, then yields. Keep the
 * virtual dummy input confined to that interval, including command commit. */
void sub_00011980_gen(void);
void sub_00012B60_gen(void);
void sub_001894B0_gen(void);
void doa3_costume_watch(void);
/* Dip switch "Correct offset of menu layout" (0.1.33): every 2D drawer of
 * the sprite system (0x6E6C0, 0x6EAD0, 0x6F9B0, 0x71730, 0xCF660, ...)
 * adds the float at 0x251244, 0.53125, to its positions, and the
 * pre-transformed passthrough program adds the screen-space offset 0.53125
 * again, so the 2D layer sits about one pixel right and down (the menu
 * header band is traced at 1.06..721.06). With the switch on the constant is
 * -0.53125, which cancels the second offset; off restores the original. */
static void doa3_menu_offset_apply(void)
{
    float want = dip_get(DIP_MENU_OFFSET) ? -0.53125f : 0.53125f;
    if (MEMF(0x251244) != want) MEMF(0x251244) = want;
}
void sub_00011980(void) { sub_00011980_gen(); doa3_costume_watch(); training_input_begin(); doa3_menu_offset_apply(); }
/* CPU stun handler: Stagger Escape hold for a CPU fighter (0.1.14). */
void sub_00016840_gen(void);
void sub_00016840(void)
{
    training_stagger_cpu_before();
    sub_00016840_gen();
    training_stagger_cpu_after();
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
void sub_00012B60(void) { sub_00012B60_gen(); training_input_end(); }
void sub_001894B0(void) { training_reset(); sub_001894B0_gen(); }

/* Observe the actual collision pass after its existing game logic completes.
 * The helper never touches guest registers, x87 state, or guest memory. */
void sub_0007F1A0_gen(void);
void sub_0007F1A0(void)
{
    sub_0007F1A0_gen();
    combat_debug_capture();
}

void sub_000B7180_gen(void);
void sub_000B9980_gen(void);
void sub_000B7180(void)
{
    CombatContact contact = combat_debug_contact_begin();
    sub_000B7180_gen();
    combat_debug_contact_end(contact);
}
void sub_000B9980(void)
{
    sub_000B9980_gen();
    { extern void doa3_perf_logic_sample(void); doa3_perf_logic_sample(); }
    combat_debug_logic_tick();
}

/* Observe stage-object geometry only when the game queries it. Do not call
 * the guest helpers from the overlay: some stage implementations update state. */
void sub_000E8E60_gen(void);
void sub_000E8F60_gen(void);
void sub_000E8E60(void)
{
    unsigned index = ecx, output = eax;
    sub_000E8E60_gen();
    combat_debug_hazard_object(index, output, eax != 0);
}
void sub_000E8F60(void)
{
    unsigned index = eax;
    sub_000E8F60_gen();
    combat_debug_hazard_disabled(index, eax != 0);
}

/* FREE_CAMERA_HOOK_BEGIN
 * 3.1 camera slots: base 4DAE90, stride C80; eye A8, target B8,
 * view 150, projection 190, combined 1D0. Reuse the game's builder so
 * its handedness, projection, angle-based culling and reflection globals agree.
 * Restore the source camera after activation: scripts keep their normal camera
 * trajectory and disabling free camera resumes it without feeding back our pose.
 */
void sub_00027720_gen(void);
void sub_00029E30_gen(void);
void sub_00029E30(void)
{
    uint32_t camera;
    float eye[3], target[3], override_eye[3], override_target[3];
    unsigned char backup[0x210 - 0xA8];
    if (ebx >= 5) { sub_00029E30_gen(); return; }
    camera = 0x4DAE90u + ebx * 0xC80u;
    if (!MEM8(camera + 0xABC)) { sub_00029E30_gen(); return; }
    for (int i = 0; i < 3; ++i) {
        eye[i] = MEMF(camera + 0xA8 + 4*i);
        target[i] = MEMF(camera + 0xB8 + 4*i);
    }
    int override_pose = free_camera_view(ebx, eye, target, override_eye, override_target);
    float original_fov = MEMF(camera + 0xD8);
    float fov = free_camera_projection_fov(ebx, original_fov);
    if (!override_pose && memcmp(&fov, &original_fov, sizeof(fov)) == 0) {
        sub_00029E30_gen();
        return;
    }
    memcpy(backup, (void *)XBOX_PTR(camera + 0xA8), sizeof(backup));
    if (override_pose) {
        for (int i = 0; i < 3; ++i) {
            MEMF(camera + 0xA8 + 4*i) = override_eye[i];
            MEMF(camera + 0xB8 + 4*i) = override_target[i];
        }
        MEMF(camera + 0xDC) = 0; /* No scripted camera roll while flying. */
    }
    /* Native builder uses tan(D8 * 0.5); D8 is vertical FOV in radians.
     * Rebuild projection and culling together, then restore scripted values. */
    MEMF(camera + 0xD8) = fov;
    {
        uint32_t regs[] = {eax, ecx, edx, ebx, esi, edi, esp, g_seh_ebp};
        double fp[8];
        int top = g_fp_top;
        uint16_t cw = g_x87_cw;
        memcpy(fp, g_fp_stack, sizeof(fp));
        eax = camera;
        PUSH32(esp, 0);
        sub_00027720_gen();
        eax = regs[0]; ecx = regs[1]; edx = regs[2]; ebx = regs[3];
        esi = regs[4]; edi = regs[5]; esp = regs[6]; g_seh_ebp = regs[7];
        memcpy(g_fp_stack, fp, sizeof(fp));
        g_fp_top = top; g_x87_cw = cw;
    }
    sub_00029E30_gen();
    memcpy((void *)XBOX_PTR(camera + 0xA8), backup, sizeof(backup));
}
/* FREE_CAMERA_HOOK_END */
#endif

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
 * shows what stopped running. Opt in with DOA3_PROF=1; normal gameplay must
 * not sort counters or flush doa3_prof.txt every five seconds. */
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
    DWORD now;
    enum { TOP = 40 };
    int top[TOP], ntop = 0, i, k;
    if (s_on < 0) { const char *e = getenv("DOA3_PROF"); s_on = e && *e == '1'; s_t0 = s_last = GetTickCount(); }
    if (!s_on || s_dumps >= 60) return;
    now = GetTickCount();
    if (now - s_last < 5000) return;
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
        if (!s_pf) s_pf = doa3_diag_fopen("doa3_prof.txt", "w");
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
    fprintf(stderr, "[%s] %s: not ported to this XBE yet\n", DOA3_XBE_REGION, what);
}

#if defined(DOA3_XBE_ID_JAPAN)
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

#if defined(DOA3_XBE_ID_JAPAN)
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
    /* Reopen configured local ports so the game refreshes controller type.
     * Network-owned ports retain their forced presence. */
    cur &= ~(xbox_InputTakeReconfiguredMask() & ~netplay_forced_pads());
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
    XBOX_VIBRATION off = { 0, 0 };
    uint32_t port = MEM32(esp + 4) - 0x0AD00001u;
    if (port < XBOX_MAX_CONTROLLERS) xbox_InputSetState(port, &off);
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
    uint32_t port = MEM32(esp + 4) - 0x0AD00001u;
    if (caps) {
        int i;
        for (i = 0; i < 0x18; i += 4) MEM32(caps + i) = 0;
        MEM8(caps + 0x18) = 0;
        MEM8(caps) = xbox_InputGuestSubtype((int)port);
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
/* Dip switch "Left stick navigates menus" (0.1.5). The menus test only the
 * directional-pad bits of the per-frame aggregate (0x73C8D0 + pad*0x2C: held
 * +0, pressed +8, auto-repeat +0x10), which the builder 0xA4E70 takes from
 * wButtons; the fight reads the sticks separately (+0x1C, with its own
 * thresholds and hysteresis, 0x1EF20). So outside a fight the left stick is
 * folded into wButtons as directional-pad bits and goes through the game's
 * own held/pressed/repeat logic. A fight is left untouched unless the pause
 * menu is up (0x5A27A0 = 1, which also covers the Sparring settings screen).
 * 8-way sectors, an axis switches on at 0x5000 (the fight's stick threshold)
 * and off below 0x3000.
 *
 * 0.1.6: "in a fight" is the fight's own stick reader (0x1EF20, once per
 * player per logic frame) having run in the last few frames. 0.1.5 used the
 * phase byte 0x596A37, but it reads 1 on the menus too (Matt's log: phase 1
 * on mode select), so the menus never got the stick. */
#include "../dip_switches.h"
static volatile unsigned g_fight_stick_frame, g_fight_stick_seen;
void sub_0001EF20_gen(void);
void sub_0001EF20(void)   /* fight stick reader (player in [esp+4]) */
{
    g_fight_stick_frame = g_doa3_frames_presented;
    g_fight_stick_seen = 1;
    sub_0001EF20_gen();
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
/* Dip switch "Random includes alternate costume colours" (0.1.15).
 * 0xCC980 resolves a random character -- the player's Random (char 0xFF) and
 * the CPU's random opponents (Story, Survival, Time Attack...) -- then turns
 * the random costume slot into a costume with 0xCD870(al = char, dl = slot,
 * [esp+4] = alternate). It always passes alternate 0. Character select gets
 * the alternate from the held buttons (0xCD110), limited only by the slot's
 * variant count, byte [ [0x29C790 + char*4] + slot ]. While the picker runs,
 * replace that 0 with a random alternate below the count, taken from the
 * game's own random pool (0x404998, the one the picker uses for the slot) so
 * the choice stays deterministic. Scripted opponents (0xCC760) never pass
 * through here. */
static int s_random_pick_depth;
void sub_000CC980_gen(void);
void sub_000CC980(void)
{
    s_random_pick_depth++;
    sub_000CC980_gen();
    s_random_pick_depth--;
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
int doa3_opt_costume(uint32_t ch, uint32_t slot, uint32_t alt);
int doa3_opt_lookup(uint32_t ch, uint32_t costume, uint32_t *slot, uint32_t *alt);
/* Disc options of costume slot `slot` (0 when it is not an unlocked disc
 * costume slot). */
static uint32_t disc_variants(uint32_t ch, uint32_t slot)
{
    uint32_t grp, have, max, count, tab;
    if (ch > 0x18) return 0;
    grp = MEM8(0x2FF868 + ch);
    have = doa3_debug_unlock_read8(0x59FB16 + grp, MEM8(0x59FB16 + grp));
    max = MEM8(0x29C70C + ch);
    count = have < max ? have : max;
    tab = MEM32(0x29C790 + ch * 4);
    return (slot < count && tab) ? MEM8(tab + slot) : 0;
}
/* 0.1.26: the options a random pick can choose for a disc costume slot --
 * its disc options 0..v-1, plus (with both random switches on) the storage
 * options (x/y/l files, options 1-3) on buttons the disc costume leaves free.
 * Fills alts[] and returns how many. */
static uint32_t random_option_set(uint32_t ch, uint32_t slot, uint32_t v, uint8_t alts[8])
{
    uint32_t n = 0, a;
    for (a = 0; a < v && n < 8; a++) alts[n++] = (uint8_t)a;
    if (ch <= 0x18 && dip_get(DIP_RANDOM_HDD_COSTUMES))
        for (a = v; a <= 3 && n < 8; a++)
            if (doa3_opt_costume(ch, slot, a) >= 0) alts[n++] = (uint8_t)a;
    return n;
}
void sub_000CD870_gen(void);
void sub_000CD870(void)
{
    {   /* 0.1.23: an option past the disc ones that a storage file provides */
        uint32_t ch = eax & 0xFF, slot = edx & 0xFF, alt = MEM8(esp + 4);
        uint32_t v = disc_variants(ch, slot);
        if (v && alt >= v) {
            int id = doa3_opt_costume(ch, slot, alt);
            if (id >= 0) {
                eax = (eax & ~0xFFu) | (uint32_t)id;
                esp += 4;
                return;
            }
        }
    }
    /* 0.1.18, storage costumes: slots past the unlocked count map to 0x40 + n
     * for the n-th costume loaded from TDATA (count per character group in
     * 0x5B3174[0x2FF868[c]], filled by Character Select). The picker draws
     * its slot from the unlocked count only; redraw it over both. */
    if (s_random_pick_depth > 0 && dip_get(DIP_RANDOM_HDD_COSTUMES) && MEM8(esp + 4) == 0) {
        uint32_t ch = eax & 0xFF;
        if (ch <= 0x18) {
            uint32_t grp = MEM8(0x2FF868 + ch);
            uint32_t have = MEM8(0x59FB16 + grp), max = MEM8(0x29C70C + ch);
            uint32_t count = have < max ? have : max, extra = MEM8(0x5B3174 + grp);
            if (count + extra > 99) extra = count < 99 ? 99 - count : 0;   /* the list's 99 */
            if (extra && count + extra < 0xFF) {
                static uint32_t s_stir2;
                uint32_t pool = MEM32(0x404998);
                uint32_t r = (pool ? MEM8(pool + ((MEM8(0x40499D) + 0x55u + s_stir2) & 0xFF)) : 0) + s_stir2 * 53u + ch;
                uint32_t pick = r % (count + extra);
                s_stir2++;
                edx = (edx & ~0xFFu) | pick;
                {   static int s_log;
                    if (s_log < 16) {
                        s_log++;
                        fprintf(stderr, "[RANDHDD] character %02X: slot %u of %u (%u unlocked + %u from storage)%s\n",
                                ch, pick, count + extra, count, extra, pick >= count ? " -> storage costume" : "");
                        fflush(stderr);
                    }
                }
            }
        }
    }
    if (s_random_pick_depth > 0 && dip_get(DIP_RANDOM_ALT_COSTUMES) && MEM8(esp + 4) == 0) {
        uint32_t ch = eax & 0xFF, slot = edx & 0xFF;
        if (ch <= 0x18) {
            uint32_t grp = MEM8(0x2FF868 + ch);
            uint32_t have = MEM8(0x59FB16 + grp), max = MEM8(0x29C70C + ch);
            uint32_t count = have < max ? have : max;
            uint32_t tab = MEM32(0x29C790 + ch * 4);
            if (slot < count && tab) {
                uint32_t variants = MEM8(tab + slot);
                uint8_t alts[8];
                uint32_t nalts = random_option_set(ch, slot, variants, alts);
                if (nalts > 1) {
                    uint32_t pool = MEM32(0x404998), r = pool ? MEM8(pool + MEM8(0x40499D)) : 0;
                    static int s_log;
                    /* Mix in the slot so a reused pool byte does not tie the
                     * alternate to the slot. */
                    uint32_t alt = alts[(r + slot * 7u + ch * 3u) % nalts];
                    if (s_log < 16) {
                        s_log++;
                        fprintf(stderr, "[RANDALT] character %02X costume slot %u: option %u (%u disc, %u in all)%s\n",
                                ch, slot, alt, variants, nalts, alt >= variants ? " -> storage option" : "");
                        fflush(stderr);
                    }
                    if (alt >= variants) {
                        /* a storage option: its own costume id */
                        eax = (eax & ~0xFFu) | (uint32_t)doa3_opt_costume(ch, slot, alt);
                        esp += 4;
                        return;
                    }
                    MEM8(esp + 4) = (uint8_t)alt;
                }
            }
        }
    }
    {   /* 0.1.16 diagnostic: every slot+alternate -> costume mapping, with
         * whether the random picker made it, and what the fighters end up
         * wearing (logged when it changes, from the input hook). */
        static int s_n;
        uint32_t ch = eax & 0xFF, slot = edx & 0xFF, alt = MEM8(esp + 4);
        sub_000CD870_gen();
        if (s_n < 200) {
            s_n++;
            fprintf(stderr, "[COSTUME] frame %u: map character %02X slot %u alternate %u -> costume %02X (%s, mode %u/%u)\n",
                    g_doa3_frames_presented, ch, slot, alt, eax & 0xFF,
                    s_random_pick_depth > 0 ? "random pick" : "other", MEM8(0x5A2858u), MEM8(0x596A33u));
            fflush(stderr);
        }
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
/* 0.1.17: CPU opponents in Survival and Time Attack. Their costume comes
 * from 0x76B30(character in [esp+4]) -> eax, not from the random picker:
 *   game mode 0x5A2858 == 2 (Story): the Story table 0x5B1454[fight] (the
 *     Fight 3/6 easter eggs that follow the player's outfit) -- untouched;
 *   == 3 or 5 (Time Attack, Survival): costume 1, or costume 2 when the
 *     player has the same character in costume 1 (mirror rule, via 0xCD900);
 *   otherwise costume 1.
 * With the dip switch on, for modes 3 and 5 pick a random unlocked costume
 * (slot < min(0x59FB16[0x2FF868[c]], 0x29C70C[c])) and a random colour of it
 * (alternate < 0x29C790[c][slot]), mapped as 0xCD870 does:
 * 0x29C728[c][sum of earlier slots' counts + alternate]. A pick that equals a
 * human player's character and costume is dropped for the game's own. The
 * game asks more than once while setting up one fight, so the answer for a
 * character is kept for 120 frames. Random bytes come from the game's pool
 * 0x404998 stirred by a call counter, never the host. */
void sub_00076B30_gen(void);
void sub_00076B30(void)
{
    uint32_t ch = MEM8(esp + 4), mode = MEM8(0x5A2858u);
    /* One entry per character: a tag fight asks for two opponents. */
    static uint32_t s_cache_frame[0x19], s_cache_costume[0x19], s_cache_valid[0x19], s_stir;
    sub_00076B30_gen();
    if (dip_get(DIP_RANDOM_ALT_COSTUMES) && (mode == 3 || mode == 5) && ch <= 0x18) {
        uint32_t grp = MEM8(0x2FF868 + ch), have = MEM8(0x59FB16 + grp), max = MEM8(0x29C70C + ch);
        uint32_t count = have < max ? have : max, total = MEM8(0x29C660 + ch);
        uint32_t vt = MEM32(0x29C790 + ch * 4), ct = MEM32(0x29C728 + ch * 4);
        if (s_cache_valid[ch] && g_doa3_frames_presented - s_cache_frame[ch] < 120u) {
            eax = (eax & ~0xFFu) | s_cache_costume[ch];
        } else if (count && vt && ct) {
            int tries;
            /* Storage costumes (0.1.18): slots past `count` are 0x40 + n. */
            uint32_t extra = dip_get(DIP_RANDOM_HDD_COSTUMES) ? MEM8(0x5B3174 + grp) : 0;
            if (count + extra > 99) extra = count < 99 ? 99 - count : 0;   /* the list's 99 */
            for (tries = 0; tries < 4; tries++) {
                uint32_t pool = MEM32(0x404998), base = 0, i, slot, alt, idx, costume, p;
                uint32_t r = (pool ? MEM8(pool + ((MEM8(0x40499D) + s_stir) & 0xFF)) : 0) +
                             s_stir * 97u + ch * 13u;
                s_stir++;
                slot = r % (count + extra);
                if (slot >= count) {
                    alt = 0;
                    costume = 0x40 + (slot - count);
                } else {
                    uint8_t alts[8];
                    uint32_t v = MEM8(vt + slot), nalts = random_option_set(ch, slot, v, alts);
                    alt = nalts ? alts[(r / count) % nalts] : 0;
                    if (alt >= v && v) {
                        int sid = doa3_opt_costume(ch, slot, alt);   /* storage option */
                        if (sid < 0) continue;
                        costume = (uint32_t)sid;
                    } else {
                        for (i = 0; i < slot; i++) base += MEM8(vt + i);
                        idx = base + alt;
                        if (idx >= total) continue;
                        costume = MEM8(ct + idx);
                    }
                }
                for (p = 0; p < 2; p++)
                    if (MEM8(0x59CF58 + p * 0x68) == 1 && MEM8(0x59CF59 + p * 0x68) == ch &&
                        MEM8(0x59CF5A + p * 0x68) == costume) break;
                if (p < 2) continue;   /* mirror match in the same outfit */
                {   static int s_log;
                    if (s_log < 32) {
                        s_log++;
                        fprintf(stderr, "[RANDALT] CPU opponent character %02X (mode %u): costume slot %u alternate %u -> %02X (game chose %02X)\n",
                                ch, mode, slot, alt, costume, eax & 0xFF);
                        fflush(stderr);
                    }
                }
                eax = (eax & ~0xFFu) | costume;
                break;
            }
            s_cache_valid[ch] = 1; s_cache_frame[ch] = g_doa3_frames_presented; s_cache_costume[ch] = eax & 0xFF;
        }
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}

/* Costume list scrolls past nine (0.1.19; a dip switch until 0.1.30, now
 * always on -- a player of the original sees no difference).
 * 0xCD7C0(al = character) -> the number of costume buttons: the unlocked
 * disc costumes (min(0x59FB16[0x2FF868[c]], 0x29C70C[c])) plus the storage
 * costumes (0x5B3174[group]) that fit in nine, min(storage, 9 - 0x29C70C[c]).
 * Character Select's cursor (0xCCB80) wraps within that count and the draw
 * (0xD18A0) puts one button per costume, 20 units apart, labelled "COS."
 * (sprite descriptor 0x29BC04) plus one digit (0x29BC0C: sprite 0x2E6 + the
 * digit, 10 px wide, at x 82), all through 0x6DF00.
 * In the port:
 *   - the count includes every storage costume, so the cursor reaches them;
 *   - during the draw only, the count is capped at nine, and each player's
 *     selected slot is shown relative to a scroll window that follows the
 *     cursor (the real slot is put back right after);
 *   - the label of row r becomes window + r + 1, with a tens digit (and
 *     "COS." 3 px further left) from 10 up.
 * The buttons themselves never move. */
static int s_cc_in_draw, s_cc_cur = -1, s_cc_ncalls;
static int s_cc_order[4], s_cc_norder, s_cc_off[4];
static uint32_t cc_full_count(uint32_t ch)
{
    uint32_t max, grp, have;
    if (ch > 0x18) return 0;
    max = MEM8(0x29C70C + ch);
    grp = MEM8(0x2FF868 + ch);
    have = doa3_debug_unlock_read8(0x59FB16 + grp, MEM8(0x59FB16 + grp));
    {   /* 0.1.22: labels stay two digits -- the list ends at COS.99 and
         * wraps to COS.1 even if more storage costumes exist. */
        uint32_t n = (have > max ? max : have) + MEM8(0x5B3174 + grp);
        return n > 99 ? 99 : n;
    }
}
void sub_000CD7C0_gen(void);
void sub_000CD7C0(void)
{
    uint32_t ch = eax & 0xFF;
    sub_000CD7C0_gen();
    if (ch <= 0x18) {   /* always on since 0.1.30 */
        uint32_t full = cc_full_count(ch);
        if (s_cc_in_draw) {
            /* The draw asks once per drawn player, in its loop order. */
            s_cc_cur = s_cc_ncalls < s_cc_norder ? s_cc_order[s_cc_ncalls] : -1;
            s_cc_ncalls++;
            if (full > 9) full = 9;
        }
        if (full > (eax & 0xFF) || s_cc_in_draw) eax = (eax & ~0xFFu) | (full & 0xFF);
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
void sub_000D18A0_gen(void);
void sub_000D18A0(void)
{
    uint32_t base = edx, p;
    uint32_t saved_addr[4];
    uint8_t saved_val[4];
    int nsaved = 0;
        s_cc_norder = 0;
    for (p = 0; p < 4; p++) {
        uint32_t S = base + 0x10 + p * 0x38, idx, ch, full, sel, addr;
        int off;
        if (!MEM8(S + 0x13) || MEM32(S + 0x34) != 1) continue;
        s_cc_order[s_cc_norder++] = (int)p;
        idx = MEM8(S + 0x16);
        ch = MEM8(0x29CA60 + MEM8(0x9B76B0 + idx) * 10);
        full = cc_full_count(ch);
        addr = S + (uint32_t)(int32_t)(int8_t)idx;
        sel = MEM8(addr);
        off = s_cc_off[p];
        if (full <= 9) off = 0;
        else {
            if ((int)sel < off) off = (int)sel;
            if ((int)sel > off + 8) off = (int)sel - 8;
            if (off > (int)full - 9) off = (int)full - 9;
            if (off < 0) off = 0;
        }
        s_cc_off[p] = off;
        if (off) {
            saved_addr[nsaved] = addr; saved_val[nsaved] = (uint8_t)sel; nsaved++;
            MEM8(addr) = (uint8_t)(sel - off);
        }
    }
    s_cc_in_draw = 1; s_cc_ncalls = 0; s_cc_cur = -1;
    sub_000D18A0_gen();
    s_cc_in_draw = 0; s_cc_cur = -1;
    while (nsaved-- > 0) MEM8(saved_addr[nsaved]) = saved_val[nsaved];
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
void sub_0006DF00_gen(void);
void sub_0006DF00(void)
{
    if (s_cc_in_draw && s_cc_cur >= 0 && (eax == 0x29BC04u || eax == 0x29BC0Cu)) {
        int off = s_cc_off[s_cc_cur];
        /* "COS." is drawn with the row (0-based) in edi; the digit after
         * edi++, so edi is then the 1-based row. */
        int label = eax == 0x29BC04u ? off + (int)edi + 1 : off + (int)edi;
        uint32_t desc = eax, pos = MEM32(desc + 4);
        if (desc == 0x29BC04u) {
            if (label >= 10) MEM32(desc + 4) = (pos & ~0xFFFu) | (((pos & 0xFFFu) - 3) & 0xFFFu);
            sub_0006DF00_gen();
            MEM32(desc + 4) = pos;
        } else {
            uint32_t id = MEM32(desc), digit0 = id - ((uint32_t)edi % 10u);
            uint32_t sv_eax = eax, sv_edx = edx, sv_ecx = ecx;
            if (label < 10) {
                MEM32(desc) = digit0 + (uint32_t)label;
                sub_0006DF00_gen();
            } else {
                /* tens digit 3 px left of the usual place, units 10 px on */
                uint32_t x = (pos & 0xFFFu) - 3;
                uint32_t a0 = MEM32(esp + 4);   /* the caller's stack argument */
                MEM32(desc) = digit0 + (uint32_t)(label / 10 % 10);
                MEM32(desc + 4) = (pos & ~0xFFFu) | (x & 0xFFFu);
                sub_0006DF00_gen();
                /* second sprite: same call shape (argument + return slot) */
                eax = sv_eax; edx = sv_edx; ecx = sv_ecx;
                PUSH32(esp, a0);
                PUSH32(esp, 0);
                MEM32(desc) = digit0 + (uint32_t)(label % 10);
                MEM32(desc + 4) = (pos & ~0xFFFu) | ((x + 10) & 0xFFFu);
                sub_0006DF00_gen();
                esp += 4;
                MEM32(desc + 4) = pos;
            }
            MEM32(desc) = id;
        }
    } else {
        sub_0006DF00_gen();
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}

/* Storage costume capacity (0.1.20). At boot (0x87420 -> 0x86AD0) the game
 * scans T:\<prefix>??.* for each of the 19 character groups (prefix table
 * 0x302C58: "zac", "tin", ... "gok"), reads the two characters after the
 * prefix as a hex number ("%2X", so kas14 is costume 0x14) and pairs .xpr
 * with .cat. Original limits:
 *   - name table 0x5B2140, 16 bytes per file, counter 0x5B212C capped at
 *     0x100 files in total (128 pairs) -- the "100";
 *   - per group, a working list of 7 numbers (0x5B3148, 6 bytes each:
 *     number, xpr file, cat file) and 7 slots in the pair table 0x5B1E58
 *     (19 groups x 7 x {cat|0x8000, xpr|0x8000}), count in 0x5B3174[group].
 * A storage costume is costume 0x40 + n, read back as pair table
 * [group*7 + n] by 0x840D0 and 0x86060, file names via 0x85C40.
 * The scan and those readers now use the variables below. They start at the
 * original addresses and limits, and doa3_tdata_alloc() (start of the scan)
 * moves them to larger tables in guest memory: 2048 files and 99 costumes per
 * character (0.1.21; costumes 0x40-0xA2 -- no code reads the costume byte as
 * signed, the readers test it with an unsigned < 0x40). If the allocation
 * fails the originals stay. */
uint32_t g_tdata_names = 0x5B2140u, g_tdata_pairs = 0x5B1E58u, g_tdata_tmp = 0x5B3148u;
uint32_t g_tdata_cap = 7, g_tdata_max_files = 0x100, g_tdata_pair_dwords = 0x85;
#define TDATA_GROUPS    19u
#define TDATA_CAP       99u   /* Matt: at most 99 per character (costumes 0x40-0xA2) */
#define TDATA_MAX_FILES 2048u
void doa3_tdata_alloc(void)
{
    extern uint32_t xbox_HeapAllocHigh(uint32_t size, uint32_t alignment);
    static int s_done;
    uint32_t names, pairs, tmp;
    if (s_done) return;
    s_done = 1;
    names = xbox_HeapAllocHigh(TDATA_MAX_FILES * 16u, 16);
    pairs = xbox_HeapAllocHigh(TDATA_GROUPS * TDATA_CAP * 4u, 16);
    tmp   = xbox_HeapAllocHigh(TDATA_CAP * 6u + 16u, 16);
    if (!names || !pairs || !tmp) {
        fprintf(stderr, "[TDATA] could not allocate the larger storage-costume tables; keeping 7 per character, 256 files\n");
        fflush(stderr);
        return;
    }
    memset((void *)XBOX_PTR(names), 0, TDATA_MAX_FILES * 16u);
    memset((void *)XBOX_PTR(pairs), 0, TDATA_GROUPS * TDATA_CAP * 4u);
    memset((void *)XBOX_PTR(tmp), 0xFF, TDATA_CAP * 6u + 16u);
    g_tdata_names = names; g_tdata_pairs = pairs; g_tdata_tmp = tmp;
    g_tdata_cap = TDATA_CAP; g_tdata_max_files = TDATA_MAX_FILES;
    g_tdata_pair_dwords = TDATA_GROUPS * TDATA_CAP;
    fprintf(stderr, "[TDATA] storage costumes: up to %u per character, %u files (tables at %08X / %08X)\n",
            TDATA_CAP, TDATA_MAX_FILES, pairs, names);
    fflush(stderr);
}
/* Storage costume options (0.1.23). A file named
 *   <prefix><two hex digits>.<x|y|l>.<xpr|cat>      e.g. aya01.y.xpr
 * is an extra option, on that button, for the character's disc costume that
 * file number <NN> belongs to: NN is a disc costume id, the number of its own
 * aya<NN>.xpr (0.1.27; 0.1.23-0.1.26 wrongly read it as a costume position).
 * Ayane (0x0D): COS.1 = 02, COS.2 = 00 (+X 01), COS.3 = 03 (+X 04), so
 * aya00.y is COS.2's Y and aya02.x COS.1's X. Buttons: X, Y or L (held with
 * A, X or Y), options 1-3 in 0xCD110 (A / Start is 0). The original game takes the text
 * after the FIRST dot as the extension, so "y.xpr" is neither xpr nor cat and
 * such files are ignored on hardware. Here they are kept out of the numbered
 * storage list, appended to the pair table after it (so each gets a storage
 * costume id 0x40 + n that the loaders already handle, but no COS.n entry),
 * and used only for a button the disc costume does not have. */
#define OPT_MAX 64
static struct { uint8_t grp, slot, alt, ok; uint16_t xpr, cat; uint8_t pair; } s_opt[OPT_MAX];
static int s_nopt;
static int opt_hex(int c) { return c >= '0' && c <= '9' ? c - '0' : (c | 0x20) >= 'a' && (c | 0x20) <= 'f' ? (c | 0x20) - 'a' + 10 : -1; }
static int opt_ieq(const char *a, const char *b, int n) { int i; for (i = 0; i < n; i++) if ((a[i] | 0x20) != (b[i] | 0x20)) return 0; return 1; }
static int opt_find(uint32_t grp, uint32_t slot, uint32_t alt)
{
    int i;
    for (i = 0; i < s_nopt; i++)
        if (s_opt[i].ok && s_opt[i].grp == grp && s_opt[i].slot == slot && s_opt[i].alt == alt) return i;
    return -1;
}
/* The costume slot of character `ch` whose disc ids (0x29C728, grouped by
 * the slot's option count 0x29C790) include `num`, or -1. */
static int opt_num_to_slot(uint32_t ch, uint32_t num)
{
    uint32_t max = MEM8(0x29C70C + ch), total = MEM8(0x29C660 + ch);
    uint32_t vt = MEM32(0x29C790 + ch * 4), ct = MEM32(0x29C728 + ch * 4), s, a, base = 0;
    if (!vt || !ct) return -1;
    for (s = 0; s < max; s++) {
        uint32_t v = MEM8(vt + s);
        for (a = 0; a < v && base + a < total; a++)
            if (MEM8(ct + base + a) == num) return (int)s;
        base += v;
    }
    return -1;
}
/* Costume id of character ch's storage option (costume slot, option), or -1. */
int doa3_opt_costume(uint32_t ch, uint32_t slot, uint32_t alt)
{
    int i;
    uint32_t grp;
    if (ch > 0x18) return -1;
    grp = MEM8(0x2FF868 + ch);
    for (i = 0; i < s_nopt; i++)
        if (s_opt[i].ok && s_opt[i].grp == grp && s_opt[i].alt == alt &&
            opt_num_to_slot(ch, s_opt[i].slot) == (int)slot)
            return 0x40 + s_opt[i].pair;
    return -1;
}
/* The (costume slot, option) of character ch a storage-option id stands for. */
int doa3_opt_lookup(uint32_t ch, uint32_t costume, uint32_t *slot, uint32_t *alt)
{
    int i, sl;
    uint32_t grp;
    if (ch > 0x18) return 0;
    grp = MEM8(0x2FF868 + ch);
    for (i = 0; i < s_nopt; i++)
        if (s_opt[i].ok && s_opt[i].grp == grp && 0x40u + s_opt[i].pair == costume &&
            (sl = opt_num_to_slot(ch, s_opt[i].slot)) >= 0) {
            *slot = (uint32_t)sl; *alt = s_opt[i].alt; return 1;
        }
    return 0;
}
void sub_00086C20_gen(void);
void sub_00086C20(void)   /* scan: one file name at edi, its name-table index in bx */
{
    const char *nm = (const char *)XBOX_PTR(edi);
    uint32_t idx = ebx & 0xFFFF;
    int h1 = opt_hex(nm[3]), h2 = opt_hex(nm[4]);
    if (idx < g_tdata_max_files && h1 >= 0 && h2 >= 0 && nm[5] == '.') {
        const char *suf = nm + 6, *dot = strchr(suf, '.');
        int len = dot ? (int)(dot - suf) : 0, alt = 0, isx, isc;
        if (len == 1 && (suf[0] | 0x20) == 'x') alt = 1;
        else if (len == 1 && (suf[0] | 0x20) == 'y') alt = 2;
        else if (len == 1 && (suf[0] | 0x20) == 'l') alt = 3;
        isx = dot && opt_ieq(dot + 1, "xpr", 4);
        isc = dot && opt_ieq(dot + 1, "cat", 4);
        if (alt && (isx || isc)) {
            uint32_t g, grp = 0xFF, slot = (uint32_t)(h1 * 16 + h2);
            int i;
            for (g = 0; g < 19; g++) {
                uint32_t pp = MEM32(0x302C58 + g * 4);
                if (pp && opt_ieq((const char *)XBOX_PTR(pp), nm, 3)) { grp = g; break; }
            }
            if (grp != 0xFF) {
                for (i = 0; i < s_nopt; i++)
                    if (s_opt[i].grp == grp && s_opt[i].slot == slot && s_opt[i].alt == alt) break;
                if (i == s_nopt && s_nopt < OPT_MAX) {
                    s_nopt++;
                    s_opt[i].grp = (uint8_t)grp; s_opt[i].slot = (uint8_t)slot; s_opt[i].alt = (uint8_t)alt;
                    s_opt[i].xpr = s_opt[i].cat = 0xFFFF; s_opt[i].ok = 0;
                }
                if (i < OPT_MAX) {
                    if (isx) s_opt[i].xpr = (uint16_t)idx; else s_opt[i].cat = (uint16_t)idx;
                }
            }
            esp += 4;   /* handled: not a numbered storage costume */
            return;
        }
    }
    sub_00086C20_gen();
    if (g_fib_slice_due) xbox_fiber_timeslice();
}

/* 0x86CC0 (group in bl): copy the group's complete xpr+cat pairs from the
 * working list into the pair table and store the count. The original is
 * unrolled seven times; this is the same per-entry step as a loop. */
void sub_00086CC0_gen(void);
void sub_00086CC0(void)
{
    uint32_t grp = ebx & 0xFF, n = 0, i;
    for (i = 0; i < g_tdata_cap; i++) {
        uint32_t e = g_tdata_tmp + i * 6u, xpr, cat, slot;
        if (MEM8(e) == 0xFF) break;
        xpr = MEM16(e + 2); cat = MEM16(e + 4);
        if (xpr == 0xFFFFu || cat == 0xFFFFu) continue;
        slot = (grp * g_tdata_cap + n) * 4u;
        MEM16(g_tdata_pairs + slot + 2) = (uint16_t)(xpr | 0x8000u);
        MEM16(g_tdata_pairs + slot) = (uint16_t)(cat | 0x8000u);
        n++;
    }
    MEM8(0x5B3174 + grp) = (uint8_t)n;
    {   /* options after the numbered ones; the count stays the numbered n */
        int k; uint32_t at = n;
        for (k = 0; k < s_nopt; k++) {
            uint32_t slot2;
            if (s_opt[k].grp != grp || s_opt[k].xpr == 0xFFFF || s_opt[k].cat == 0xFFFF) continue;
            if (at >= g_tdata_cap || at > 0x7F) break;
            slot2 = (grp * g_tdata_cap + at) * 4u;
            MEM16(g_tdata_pairs + slot2 + 2) = (uint16_t)(s_opt[k].xpr | 0x8000u);
            MEM16(g_tdata_pairs + slot2) = (uint16_t)(s_opt[k].cat | 0x8000u);
            s_opt[k].pair = (uint8_t)at; s_opt[k].ok = 1;
            fprintf(stderr, "[TDATA] group %u: option %u for the disc costume with file %02X, from storage (costume id %02X)\n",
                    grp, s_opt[k].alt, s_opt[k].slot, 0x40 + at);
            at++;
        }
    }
    if (n) {
        fprintf(stderr, "[TDATA] group %u (%.3s): %u storage costume(s)\n", grp,
                (const char *)XBOX_PTR(MEM32(0x302C58 + grp * 4)), n);
        fflush(stderr);
    }
    eax = grp; ecx = n;
    esp += 4;   /* ret */
    if (g_fib_slice_due) xbox_fiber_timeslice();
}

/* 0xCD110(al = costume slot, dl = character, ecx = port) -> option from the
 * confirm button (word 0x9B7C0E + port*8, high byte: 1 A, 4 X, 8 Y; LT held =
 * 0x4000 in the pad aggregate), limited to the costume's disc options.
 * 0.1.23: when the button means an option the disc costume does not have and
 * a storage file provides it, return that option. */
void sub_000CD110_gen(void);
void sub_000CD110(void)
{
    uint32_t slot = eax & 0xFF, ch = edx & 0xFF, port = ecx;
    sub_000CD110_gen();
    if (ch <= 0x18 && port < 4) {
        uint32_t v = disc_variants(ch, slot), btn = MEM8(0x9B7C0F + port * 8), want;
        int lt = (MEM32(0x73C8D0 + port * 0x2C) & 0x4000u) != 0;
        /* Holding L with A, X or Y is the L option (0.1.25; for Leifang's
         * first costume the game itself gives 3 for L + any of them). */
        want = lt ? 3 : (btn & 4) ? 1 : (btn & 8) ? 2 : 0;
        if (v && want >= v && doa3_opt_costume(ch, slot, want) >= 0)
            eax = (eax & ~0xFFu) | want;
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
/* 0xCD900(cl = character, [esp+4] = costume id, [esp+8] = option out) ->
 * costume slot. A storage-option id maps back to its disc costume + button. */
void sub_000CD900_gen(void);
void sub_000CD900(void)
{
    uint32_t ch = ecx & 0xFF, id = MEM8(esp + 4), out = MEM32(esp + 8), slot, alt;
    if (ch <= 0x18 && id >= 0x40 && id != 0xFF &&
        doa3_opt_lookup(ch, id, &slot, &alt)) {
        if (out) MEM8(out) = (uint8_t)alt;
        eax = slot;
        esp += 4;
        return;
    }
    sub_000CD900_gen();
    if (g_fib_slice_due) xbox_fiber_timeslice();
}

/* DOA3 0.1.32: Random on Character Select without Ein.
 * 0xCD9E0 builds the grid: with Random allowed (state +0x1D8, 0 on Story's
 * select) and Ein unlocked (0x5A0B47) it uses all 18 cells of the table
 * 0x29CA60 (10 bytes per cell, byte 0 = character, 3 = Ein, 0xFF = Random);
 * otherwise 16 cells, leaving out Ein and Random. The select code already
 * handles a cell whose character is 0xFE as a locked slot: drawn with the
 * lock sprite 0x313 (texture 7 of the chr_sel sprite file, 0xCF9C2),
 * refused with sound 0x29 (0xCB614), skipped by Random (0xCCA3F). So with
 * Random allowed and Ein locked, Ein's cell (8, byte 0x29CAB0) is set to
 * 0xFE and the 18-cell grid is used. Every other call puts the 3 back.
 * Hand edits in gen/recomp_0003.c call these. */
#define EIN_CELL_CHAR 0x29CAB0u
uint8_t doa3_ein_grid_gate(uint8_t ein_unlocked)
{
    if (!ein_unlocked) {
        if (MEM8(EIN_CELL_CHAR) != 0xFE) {
            fprintf(stderr, "[SELECT] Ein locked: Random shown, Ein's slot locked\n");
            fflush(stderr);
        }
        MEM8(EIN_CELL_CHAR) = 0xFE;
        return 1;
    }
    MEM8(EIN_CELL_CHAR) = 3;
    return ein_unlocked;
}
void doa3_ein_grid_restore(void)
{
    MEM8(EIN_CELL_CHAR) = 3;
}

/* DOA3 0.1.31: dip switch "Random is default stage option".
 * Stage Select init (0xD8470) takes the starting cursor as its argument;
 * every caller passes the constant 2 (Beach (Day)) except the one at
 * 0xCADAC, which passes the remembered stage (state +0x1EA) and sets
 * g_stage_cursor_keep first. The cursor byte is 0x9B7BAA, indexing the
 * order list 0x9B76C4[0x29CA5A]; table entry 0x0E is Random, which the
 * init appends at the end of the list. Called after the list is built. */
int g_stage_cursor_keep;
uint8_t doa3_stage_initial_cursor(uint8_t cur)
{
    int keep = g_stage_cursor_keep;
    unsigned n, i;
    g_stage_cursor_keep = 0;
    if (keep || cur != 2 || !dip_get(DIP_RANDOM_STAGE_DEFAULT)) return cur;
    n = MEM8(0x29CA5A);
    if (n > 32) n = 32;
    for (i = 0; i < n; i++)
        if (MEM8(0x9B76C4 + i) == 0x0E) return (uint8_t)i;
    return cur;
}

/* Dip switch "Same costume, different option" (0.1.28).
 * Character Select keeps a costume from being highlighted when an opposing
 * player on the same character has it (0xCCC70: an opponent's cursor record
 * on that slot; 0xCCD40: the opposing side's confirmed picks -- slot
 * 0x9B7C00[side*5+i] (0xFF = none), character 0x588838[..], costume id
 * 0x5A26F8[..]). Cursor record S: +0x14 character, +0x16 grid index, slot at
 * S + (int8)index, +0x1A side (bit 0).
 * With the switch on, such a costume can be highlighted when it has two or
 * more options (disc options plus storage options) and at least one is not
 * confirmed by the other side; confirming (0xCB760, buttons 0xD10) is
 * refused only for the exact costume + option the other side confirmed. */
static uint32_t costume_slot_ids(uint32_t ch, uint32_t slot, uint8_t ids[8])
{
    uint32_t grp, have, max, count, vt, ct, total, n = 0, s, base = 0, a, v;
    if (ch > 0x18) return 0;
    grp = MEM8(0x2FF868 + ch);
    have = doa3_debug_unlock_read8(0x59FB16 + grp, MEM8(0x59FB16 + grp));
    max = MEM8(0x29C70C + ch);
    count = have < max ? have : max;
    vt = MEM32(0x29C790 + ch * 4); ct = MEM32(0x29C728 + ch * 4); total = MEM8(0x29C660 + ch);
    if (slot >= count) {   /* a numbered storage costume: one option */
        ids[n++] = (uint8_t)(0x40 + (slot - count));
        return n;
    }
    if (!vt || !ct) return 0;
    for (s = 0; s < slot; s++) base += MEM8(vt + s);
    v = MEM8(vt + slot);
    for (a = 0; a < v && base + a < total && n < 8; a++) ids[n++] = MEM8(ct + base + a);
    for (a = v; a <= 3 && n < 8; a++) {
        int id = doa3_opt_costume(ch, slot, a);
        if (id >= 0) ids[n++] = (uint8_t)id;
    }
    return n;
}
/* The side index of a cursor record as the game files its picks (0.1.29):
 * 0xCC760 stores them at side*5 + member with side = 0xCCEF0(record) & 1,
 * which follows the mode, not just the record's +0x1A. Call the game's own
 * 0xCCEF0, keeping the guest registers as they were. */
static uint32_t record_side(uint32_t rec)
{
    uint32_t sv_eax = eax, sv_ebx = ebx, sv_ecx = ecx, sv_edx = edx, sv_esi = esi, sv_edi = edi, sv_esp = esp, r;
    eax = rec;
    PUSH32(esp, 0);
    sub_000CCEF0();
    r = eax & 1;
    eax = sv_eax; ebx = sv_ebx; ecx = sv_ecx; edx = sv_edx; esi = sv_esi; edi = sv_edi; esp = sv_esp;
    return r;
}
static int costume_taken_by_other_side(uint32_t side, uint32_t ch, uint32_t id)
{
    uint32_t other = (side & 1) ^ 1, m;
    for (m = 0; m < 5; m++) {
        uint32_t k = other * 5 + m;
        if (MEM8(0x9B7C00 + k) != 0xFF && MEM8(0x588838 + k) == ch && MEM8(0x5A26F8 + k) == id)
            return 1;
    }
    return 0;
}
void sub_000CCC70_gen(void);
void sub_000CCC70(void)   /* al = slot, edi = cursor record -> eax nonzero = blocked */
{
    uint32_t slot = eax & 0xFF, rec = edi;
    sub_000CCC70_gen();
    if (eax && dip_get(DIP_SHARED_COSTUMES)) {
        uint32_t ch = MEM8(rec + 0x14), side = record_side(rec), n, i, free = 0;
        uint8_t ids[8];
        n = costume_slot_ids(ch, slot, ids);
        for (i = 0; i < n; i++) if (!costume_taken_by_other_side(side, ch, ids[i])) free++;
        if (n >= 2 && free >= 1) eax = 0;
    }
    if (g_fib_slice_due) xbox_fiber_timeslice();
}
/* The confirm test: 0xD10 (Start A X Y), or 0 when the press would give the
 * exact costume + option the other side confirmed. The option follows
 * 0xCD110 (and its storage-option extension). */
uint32_t doa3_costume_confirm_mask(uint32_t rec, uint32_t port)
{
    uint32_t ch, slot, v, btn, a, want, grp, have, max, count;
    int lt, id = -1;
    if (!dip_get(DIP_SHARED_COSTUMES) || port >= 4) return 0xD10u;
    ch = MEM8(rec + 0x14);
    slot = MEM8(rec + (uint32_t)(int32_t)(int8_t)MEM8(rec + 0x16));
    if (ch > 0x18) return 0xD10u;
    v = disc_variants(ch, slot);
    btn = MEM8(0x9B7C0F + port * 8);
    lt = (MEM32(0x73C8D0 + port * 0x2C) & 0x4000u) != 0;
    if (lt && v >= 4) a = (btn & 1) ? 3 : ((btn & 4) && v >= 5) ? 4 : ((btn & 8) && v >= 6) ? 5 : 3;
    else a = ((btn & 4) && v >= 2) ? 1 : ((btn & 8) && v >= 3) ? 2 : 0;
    want = lt ? 3 : (btn & 4) ? 1 : (btn & 8) ? 2 : 0;
    if (v && want >= v && doa3_opt_costume(ch, slot, want) >= 0) a = want;
    if (v && a >= v) id = doa3_opt_costume(ch, slot, a);
    else if (v) {
        uint32_t vt = MEM32(0x29C790 + ch * 4), ct = MEM32(0x29C728 + ch * 4), s, base = 0;
        for (s = 0; s < slot; s++) base += MEM8(vt + s);
        id = MEM8(ct + base + a);
    } else {
        grp = MEM8(0x2FF868 + ch);
        have = doa3_debug_unlock_read8(0x59FB16 + grp, MEM8(0x59FB16 + grp));
        max = MEM8(0x29C70C + ch);
        count = have < max ? have : max;
        if (slot >= count) id = (int)(0x40 + (slot - count));
    }
    if (id >= 0 && costume_taken_by_other_side(record_side(rec), ch, (uint32_t)id)) {
        static int s_log;
        if (s_log < 16) {
            s_log++;
            fprintf(stderr, "[SHARED] confirm refused: character %02X costume %02X is the other side's pick\n", ch, id);
            fflush(stderr);
        }
        /* 0.1.32: a refused press plays sound 0x29, the sound the game
         * itself plays for a press on a locked (0xFE) slot (0xCB614). */
        if (MEM16(0x9B7C0E + port * 8) & 0xD10u) {
            uint32_t sv_eax = eax, sv_ebx = ebx, sv_ecx = ecx, sv_edx = edx, sv_esi = esi, sv_edi = edi, sv_esp = esp;
            eax = 0x29;
            PUSH32(esp, 0);
            sub_000A5610();
            eax = sv_eax; ebx = sv_ebx; ecx = sv_ecx; edx = sv_edx; esi = sv_esi; edi = sv_edi; esp = sv_esp;
        }
        return 0;
    }
    return 0xD10u;
}

void doa3_costume_watch(void)
{
    static uint32_t s_last = 0xFFFFFFFFu;
    static int s_n;
    uint32_t now = MEM8(0x59CF59u) | (MEM8(0x59CF5Au) << 8) | (MEM8(0x59CFC1u) << 16) | ((uint32_t)MEM8(0x59CFC2u) << 24);
    if (now != s_last && s_n < 100) {
        s_n++;
        fprintf(stderr, "[COSTUME] frame %u: fighting P1 character %02X costume %02X, P2 character %02X costume %02X (mode %u/%u)\n",
                g_doa3_frames_presented, now & 0xFF, (now >> 8) & 0xFF, (now >> 16) & 0xFF, now >> 24,
                MEM8(0x5A2858u), MEM8(0x596A33u));
        fflush(stderr);
    }
    s_last = now;
}

static uint16_t xpp_lstick_menu_bits(uint32_t port, int16_t lx, int16_t ly)
{
    static uint8_t s_on[4];         /* bit0 x axis, bit1 y axis engaged */
    static int s_log;
    extern int netplay_active(void);
    uint8_t phase, paused, on;
    int ax = lx < 0 ? -(int)lx : lx, ay = ly < 0 ? -(int)ly : ly;
    uint16_t bits = 0;
    int gated;
    if (port >= 4 || !dip_get(DIP_LSTICK_MENUS)) return 0;
    on = s_on[port];
    on = (uint8_t)((ax >= 0x5000 || ((on & 1) && ax >= 0x3000)) ? (on | 1) : (on & ~1));
    on = (uint8_t)((ay >= 0x5000 || ((on & 2) && ay >= 0x3000)) ? (on | 2) : (on & ~2));
    s_on[port] = on;
    if (!on) return 0;
    /* 8-way: the minor axis counts only within 22.5 degrees of a diagonal. */
    if ((on & 1) && (ax * 1000 >= ay * 414)) bits |= lx < 0 ? 0x0004 : 0x0008;   /* LEFT / RIGHT */
    if ((on & 2) && (ay * 1000 >= ax * 414)) bits |= ly > 0 ? 0x0001 : 0x0002;   /* UP / DOWN */
    phase = MEM8(0x596A37u);
    paused = MEM8(0x5A27A0u);
    gated = netplay_active() ||
            (g_fight_stick_seen && g_doa3_frames_presented - g_fight_stick_frame <= 3u && paused != 1);
    if (s_log < 24) {
        static uint16_t s_last = 0xFFFF;
        if (bits != s_last) {
            s_log++;
            fprintf(stderr, "[DIP] left stick -> pad bits %X port %u phase %u paused %u mode %u fight-reader %d frames ago: %s (frame %u)\n",
                    bits, port, phase, paused, MEM8(0x5A2858u),
                    g_fight_stick_seen ? (int)(g_doa3_frames_presented - g_fight_stick_frame) : -1,
                    gated ? "fight, not folded" : "folded", g_doa3_frames_presented);
            fflush(stderr);
        }
        s_last = bits;
    }
    return gated ? 0 : bits;
}

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
        uint8_t raw[32] = {0};
        if (xbox_InputGetState(port, (XBOX_INPUT_STATE *)raw) == 0) {
            extern void doa3_perf_input_sample(unsigned port);
            doa3_perf_input_sample(port);
            buttons = *(uint16_t *)(raw + 4);
            memcpy(an, raw + 6, 8);
            lx = *(int16_t *)(raw + 14); ly = *(int16_t *)(raw + 16);
            rx = *(int16_t *)(raw + 18); ry = *(int16_t *)(raw + 20);
            buttons |= xpp_lstick_menu_bits(port, lx, ly);
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

void sub_00211B07(void)   /* XInputSetState(handle, pFeedback), ret 8 */
{
    uint32_t port = MEM32(esp + 4) - 0x0AD00001u;
    uint32_t fb = MEM32(esp + 8);
    {
        static int trace = -1;
        static unsigned next_frame;
        if (trace < 0) trace = getenv("DOA3_RUMBLE_TRACE") != NULL;
        if (trace && port == 0 && g_doa3_frames_presented >= next_frame) {
            next_frame = g_doa3_frames_presented + 300;
            fprintf(stderr, "[RUMBLE-GAME] frame=%u option=%u pending=%u/%u feedback=%u/%u timer=%u\n",
                    g_doa3_frames_presented, MEM8(0x59D0D6), MEM16(0x9B28B0), MEM16(0x9B28B2),
                    fb ? MEM16(fb + 0x42) : 0, fb ? MEM16(fb + 0x44) : 0, MEM32(0x73C744));
            fflush(stderr);
        }
    }
    eax = xbox_InputSetFeedback(port, fb ? (void *)&MEM8(fb) : NULL);
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
#endif /* DOA3_XBE_ID_JAPAN (XPP) */

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
#if defined(DOA3_XBE_ID_JAPAN)
extern int  pgraph_d3d11_method(int subchannel, uint32_t method, uint32_t param);
extern void pgraph_d3d11_method_run(int subchannel, uint32_t method, const uint32_t *params,
                                    uint32_t count, int incr);
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
                                  uint32_t ring_hi, int *flip)
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
                /* NV097_FLIP_STALL, end of D3DDevice_Swap */
                if (kind == 0 ? (method <= 0x130 && 0x130 <= method + (count - 1) * 4) : method == 0x130)
                    *flip = 1;
                if (translate)
                    pgraph_d3d11_method_run((int)sub, method, (const uint32_t *)XBOX_PTR(pos), count, kind == 0);
                pos += count * 4;
                (void)i;
            } else {
                s_pb_ctl[3]++;                       /* unknown control word */
            }
        }
    }
}

/* Host-thread-only counters. PB time is a subset of the frame interval;
 * worker time is a subset of pacing, not additional time to sum with it. */
static struct {
    LONGLONG presented, pace, workers, longest_worker, pb;
    unsigned kicks;
} s_frame_perf;
static LONGLONG s_phase_prev_present;      /* last frame's present, QPC ticks */
double g_doa3_phase_ms[7] = { -1, -1, -1, -1, -1, -1, -1 };
unsigned g_doa3_phase_kicks;

static void pb4134_translate(uint32_t from, uint32_t to, int translate)
{
    extern uint32_t g_pb4134_ring_lo, g_pb4134_ring_hi;
    /* Always timed (0.0.181): a QPC read per kick-off is negligible and the
     * 5-second [PERF-5S] summary needs it without the diagnostics switch. */
    LONGLONG begin = doa3_perf_now();
    pb4134_translate_span(from, to, translate, g_pb4134_ring_lo, g_pb4134_ring_hi, &s_pb4134_flip);
    if (begin) {
        s_frame_perf.pb += doa3_perf_now() - begin;
        ++s_frame_perf.kicks;
    }
}
uint32_t g_pb4134_ring_lo, g_pb4134_ring_hi;

/* ── Asynchronous GPU (0.0.195; default since 0.0.197, DOA3_ASYNC_GPU=0 off)
 * KickOff queues the span instead of translating it; a render
 * thread translates the queue in order, and only then marks the span
 * consumed -- DMA_GET and the fence semaphore, the two things the game
 * reads to know the GPU is done with memory -- the way the console's GPU
 * runs behind the CPU. At the end of a frame the render thread copies the
 * finished guest frame into one of two buffers; the game thread presents
 * the newest one at its next frame end, then paces and runs the frame
 * callback as before. The game is at most one frame ahead of the render
 * thread. The game thread finds the frame end (FLIP_STALL) itself with a
 * header-only walk of the span, which skips every data run.
 *
 * Off while the host movie presenter owns the screen, before the first
 * movie hand-over, and in netplay; switching drains the queue first. */
static void d3d4134_capture_frame(unsigned frame);
static void doa3_mtrace_frame(unsigned next);
#define PBQ_N 512
typedef struct {
    uint32_t from, to, ring_lo, ring_hi;
    uint32_t chan, sem, semval, cursor;   /* consumed-marks, written after the span */
    int      translate, marks, flip;
    unsigned frame;                      /* with flip: this frame's number */
} PbItem;
static PbItem          s_pbq[PBQ_N];
static volatile LONG   s_pbq_head, s_pbq_tail;          /* head: next to run; tail: next free */
static HANDLE          s_pbq_work, s_pbq_done;          /* auto-reset events */
static HANDLE          s_rthread;
static volatile LONG   s_frames_done;                   /* frames the render thread finished */
static volatile LONG   s_frames_presented;              /* frames the game thread showed */
static unsigned        s_frames_submitted;              /* game thread only */
static int             s_async_on = -1;                 /* the switch */
static int             s_async_active;                  /* game thread: currently asynchronous */
static LONG            s_buf_frame[2] = { -1, -1 };     /* frame held by each copy */
LONGLONG               g_doa3_rthread_busy;             /* QPC ticks spent translating, for [PERF-5S] */

static int pbq_empty(void) { return s_pbq_head == s_pbq_tail; }

static DWORD WINAPI pb_render_thread(LPVOID unused)
{
    extern void d3d8_Lock(void), d3d8_Unlock(void);
    extern int d3d8_AsyncFrameCopy(int buf);
    extern void pgraph_d3d11_flush(void);
    (void)unused;
    for (;;) {
        PbItem it;
        int flip = 0;
        if (pbq_empty()) { WaitForSingleObject(s_pbq_work, INFINITE); continue; }
        it = s_pbq[s_pbq_head % PBQ_N];
        if (it.translate) {
            LONGLONG begin = doa3_perf_now();
            d3d8_Lock();
            pb4134_translate_span(it.from, it.to, 1, it.ring_lo, it.ring_hi, &flip);
            if (it.flip) {
                /* The frame end: the pgraph flush and the copy the game
                 * thread will present. Wait until it has shown the frame
                 * that used this copy last (two frames back). */
                d3d8_Unlock();
                while (s_frames_presented < (LONG)it.frame - 2)
                    WaitForSingleObject(s_pbq_work, 2);
                d3d8_Lock();
                pgraph_d3d11_flush();
                d3d4134_capture_frame(++g_doa3_frames_presented);
                doa3_mtrace_frame(g_doa3_frames_presented + 1);
                if (d3d8_AsyncFrameCopy((int)(it.frame & 1))) s_buf_frame[it.frame & 1] = (LONG)it.frame;
            }
            d3d8_Unlock();
            g_doa3_rthread_busy += doa3_perf_now() - begin;
        }
        if (it.marks) {
            MemoryBarrier();
            if (it.chan) {
                MEM32(it.chan + 0x40) = it.cursor & 0x3FFFFFFu;   /* DMA_PUT */
                MEM32(it.chan + 0x44) = it.cursor & 0x3FFFFFFu;   /* DMA_GET: consumed */
            }
            if (it.sem) MEM32(it.sem) = it.semval;
        }
        if (it.flip) InterlockedExchange(&s_frames_done, (LONG)it.frame);
        MemoryBarrier();
        InterlockedIncrement(&s_pbq_head);
        SetEvent(s_pbq_done);
    }
    return 0;
}

static int pb_async_wanted(void)
{
    extern volatile int g_doa3_post_movie;
    extern int doa3_movie_host_owns_screen(void);
    extern int netplay_active(void);
    if (s_async_on < 0) {
        /* On by default since 0.0.197; DOA3_ASYNC_GPU=0 translates at
         * kick-off on the game thread as before. */
        const char *e = getenv("DOA3_ASYNC_GPU");
        s_async_on = !(e && *e == '0');
        if (s_async_on) {
            extern void d3d8_LockInit(void);
            d3d8_LockInit();
            s_pbq_work = CreateEventA(NULL, FALSE, FALSE, NULL);
            s_pbq_done = CreateEventA(NULL, FALSE, FALSE, NULL);
            s_rthread = CreateThread(NULL, 1 << 20, pb_render_thread, NULL, 0, NULL);
            if (!s_pbq_work || !s_pbq_done || !s_rthread) s_async_on = 0;
            fprintf(stderr, "[ASYNC] GPU commands on a render thread: %s\n", s_async_on ? "on" : "failed, off");
        }
    }
    /* Not g_doa3_post_movie: only the USA build ever sets it (0.0.195 never
     * switched on). Ten seconds of frames keeps boot synchronous; a movie
     * owning the screen or netplay switches back. */
    (void)g_doa3_post_movie;
    return s_async_on && g_doa3_frames_presented > 600 && !doa3_movie_host_owns_screen() && !netplay_active();
}

/* Game thread: wait for the render thread, keeping the window alive. */
static void pb_wait(int (*done)(LONG), LONG arg)
{
    extern void doa3_pump_messages(void);
    while (!done(arg)) {
        MsgWaitForMultipleObjects(1, &s_pbq_done, FALSE, 2, QS_ALLINPUT);
        doa3_pump_messages();
    }
}
static int pb_is_drained(LONG unused) { (void)unused; return pbq_empty(); }
static int pb_frame_is_done(LONG f) { return s_frames_done >= f; }

static void pbq_push(const PbItem *it)
{
    while (s_pbq_tail - s_pbq_head >= PBQ_N - 1) {         /* full: let it catch up */
        extern void doa3_pump_messages(void);
        MsgWaitForMultipleObjects(1, &s_pbq_done, FALSE, 2, QS_ALLINPUT);
        doa3_pump_messages();
    }
    s_pbq[s_pbq_tail % PBQ_N] = *it;
    MemoryBarrier();
    InterlockedIncrement(&s_pbq_tail);
    SetEvent(s_pbq_work);
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
            LONGLONG worker_begin = doa3_perf_now();
            xbox_fiber_yield();             /* a worker lap while we wait */
            if (worker_begin) {
                LONGLONG elapsed = doa3_perf_now() - worker_begin;
                s_frame_perf.workers += elapsed;
                if (elapsed > s_frame_perf.longest_worker)
                    s_frame_perf.longest_worker = elapsed;
            }
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
 * is presented. Disabled unless ranges are explicitly supplied.
 * DOA3_CAPTURE_FRAMES=0 turns it off. */
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
        if (0) {   /* old boot capture (the glitch finder that replaced it is gone too) */
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

/* Method trace of chosen frames: DOA3_MTRACE="a-b,c-d" (off by default)
 * writes mtrace_NNNN.txt with every NV2A method of frame NNNN. Called at
 * each flip with the number of the frame that comes next. =0 turns it off. */
/* Method-trace files: one ~3 MB text file per traced frame on busy stages,
 * written by ~130k small fprintf calls. A 4 MB stdio buffer turns that into
 * a handful of large writes instead of hundreds of 4 KB ones. */
static FILE *doa3_mtrace_open(const char *name)
{
    FILE *f = doa3_diag_fopen(name, "w");
    if (f) setvbuf(f, NULL, _IOFBF, 4u << 20);
    return f;
}

volatile LONG g_doa3_trace_request;   /* set by the Debug tab */
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
        if (!e) e = doa3_auto_capture_enabled() ? "305-310,590-612" : "";
        while (*e && s_n < 8) {
            char *end; unsigned a = strtoul(e, &end, 10), b = a;
            if (end == e) break;
            if (*end == '-') { e = end + 1; b = strtoul(e, &end, 10); }
            if (a) { s_lo[s_n] = a; s_hi[s_n] = b; s_n++; }
            e = (*end == ',') ? end + 1 : end;
        }
    }
    if (g_doa3_mtrace) { pgraph_mtrace_flush_run(); fclose(g_doa3_mtrace); g_doa3_mtrace = NULL; }
    {   /* Debug > Diagnostics > "Trace the next frame": one method trace plus
         * its screenshot, on demand (works with automatic captures off). */
        extern volatile LONG g_doa3_trace_request;
        if (InterlockedExchange(&g_doa3_trace_request, 0)) {
            char path[64];
            g_doa3_aa_shot = next;
            sprintf(path, "mtrace_manual_%05u.txt", next);
            g_doa3_mtrace = doa3_mtrace_open(path);
            fprintf(stderr, "[TRACE] requested from the menu: tracing frame %u -> %s + aa_%05u.bmp\n", next, path, next);
            return;
        }
    }
    for (k = 0; k < s_n; k++)
        if (next >= s_lo[k] && next <= s_hi[k]) {
            char path[64];
            sprintf(path, "mtrace_%04u.txt", next);
            g_doa3_mtrace = doa3_mtrace_open(path);
            return;
        }
    if (!doa3_auto_capture_enabled()) return;
    /* The frame-buffer-effect captures (mtrace_fb_* plus aa_*.bmp on the
     * first draw that samples a frame buffer, then every 10 s) are gone
     * (0.1.3): Omega's after-image they were added for was fixed long ago,
     * and the REPLAY -> WINNER cross-fade samples a frame buffer at every
     * match end, so they fired there -- the ~80 ms capture held the
     * cross-fade's first frame (the REPLAY outline without its fill) on
     * screen for 4-5 refreshes instead of one. */
    /* The movie-end captures (mtrace_mvend_* plus mvend_*.bmp at +1, +6,
     * +15, +30 and +60 frames after each of the first four movies, ~22 MB
     * per screenshot at 32:9) are gone (0.0.136). */
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
            g_doa3_mtrace = doa3_mtrace_open(path);
            fprintf(stderr, "[AA] tracing frame %u (multisampled screen) -> %s + aa_%05u.bmp\n", next, path, next);
            return;
        }
    }
    /* The periodic traces (four consecutive frames every 1500, at most 48
     * files) are gone: on a tester's low-end PC the four ~3 MB X Octagon
     * traces at frame 3000 froze the game for ~10 s (the log's 5-second
     * [DRAW] lines: frames 2971 -> 3000 -> 3002). Event-triggered traces
     * (multisampled screens, frame-buffer effects, movie ends) and explicit
     * DOA3_MTRACE ranges remain. */
}

/* Every 5 s, always (0.0.181): where the host thread's frame time went,
 * averaged over all frames and over the frames that missed 60 fps (interval
 * > 17.5 ms). game = guest code outside push-buffer translation, pb =
 * translation (state and draws), flush/aux = end of frame, present =
 * compose + overlay + DXGI, pace = the 60 Hz wait (CRI workers run inside
 * it). Draws per frame from the translator's counter. One line per 5 s;
 * at most 720 lines (an hour). */
#include "../../nv2a/nv2a_pgraph_d3d11.h"
static void perf_5s(double interval, const double ph[7], double present, double ms)
{
    static double sum[2][8]; static unsigned n[2], frames; static double worst;
    static DWORD s_next; static unsigned s_lines; static uint32_t s_draws_prev;
    PgraphD3D11Stats st;
    uint32_t draws;
    double v[8];
    int k, slow;
    (void)ms;
    if (interval < 0.0 || ph[0] < 0.0) return;
    pgraph_d3d11_get_stats(&st);
    draws = st.draw_calls - s_draws_prev; s_draws_prev = st.draw_calls;
    v[0] = interval; v[1] = ph[0]; v[2] = ph[1]; v[3] = ph[2] + ph[3];
    v[4] = present; v[5] = ph[5] < 0 ? 0 : ph[5]; v[6] = ph[6] < 0 ? 0 : ph[6]; v[7] = (double)draws;
    slow = interval > 17.5;
    for (k = 0; k < 8; k++) { sum[0][k] += v[k]; if (slow) sum[1][k] += v[k]; }
    n[0]++; if (slow) n[1]++;
    if (interval > worst) worst = interval;
    frames++;
    if (!s_next) s_next = GetTickCount() + 5000;
    if ((int)(GetTickCount() - s_next) < 0) return;
    if (s_lines < 720 && n[0]) {
        int r;
        s_lines++;
        fprintf(stderr, "[PERF-5S] frames %u (%u over 17.5 ms, worst %.1f ms)", n[0], n[1], worst);
        for (r = 0; r < 2; r++) {
            double c = n[r] ? (double)n[r] : 1.0;
            if (r && !n[1]) break;
            fprintf(stderr, " | %s: interval %.2f game %.2f pb %.2f flush+aux %.2f present %.2f pace %.2f (workers %.2f) draws %.0f",
                    r ? "slow" : "all", sum[r][0] / c, sum[r][1] / c, sum[r][2] / c, sum[r][3] / c,
                    sum[r][4] / c, sum[r][5] / c, sum[r][6] / c, sum[r][7] / c);
        }
        fprintf(stderr, "\n");
    }
    memset(sum, 0, sizeof sum); n[0] = n[1] = 0; worst = 0.0; frames = 0;
    s_next = GetTickCount() + 5000;
}

static void d3d4134_frame_done(uint32_t dev, unsigned async_frame)
{
    {   extern void doa3_prof_set_thread(void);
        static int s_prof_reg;
        if (!s_prof_reg) { s_prof_reg = 1; doa3_prof_set_thread(); } }
    extern void pgraph_d3d11_flush(void);
    extern void doa3_present_frame(void);
    static int s_frames;
    uint32_t sv_eax = eax, sv_ecx = ecx, sv_edx = edx, sv_ebx = ebx;
    uint32_t sv_esi = esi, sv_edi = edi, sv_esp = esp, sv_seh = g_seh_ebp;
    int perf_on = doa3_perf_begin() != 0;      /* [PERF-PHASE] lines only with diagnostics */
    LONGLONG entry = doa3_perf_now(), flushed, prepared, presented;
    if (!async_frame) {
        pgraph_d3d11_flush();
        flushed = entry ? doa3_perf_now() : 0;
        d3d4134_capture_frame(++g_doa3_frames_presented);
        doa3_mtrace_frame(g_doa3_frames_presented + 1);
    } else {
        /* The render thread flushes and copies each frame; wait for the one
         * before this, so the game is at most one frame ahead. */
        pb_wait(pb_frame_is_done, (LONG)async_frame - 1);
        flushed = entry ? doa3_perf_now() : 0;
    }
    { void doa3_adx_frame(unsigned frame); doa3_adx_frame(g_doa3_frames_presented); }
    game_audio_frame();               /* game volumes, streamer mode */

    prepared = entry ? doa3_perf_now() : 0;
    if (entry) {   /* hand the phase split to the frame capture (main.c) */
        extern double g_doa3_phase_ms[7]; extern unsigned g_doa3_phase_kicks;
        LARGE_INTEGER fq; double ms;
        QueryPerformanceFrequency(&fq); ms = 1000.0 / fq.QuadPart;
        g_doa3_phase_ms[0] = s_frame_perf.presented ?
            (entry - s_frame_perf.presented - s_frame_perf.pace - s_frame_perf.pb) * ms : -1.0;
        g_doa3_phase_ms[1] = s_frame_perf.pb * ms;
        g_doa3_phase_ms[2] = (flushed - entry) * ms;
        g_doa3_phase_ms[3] = (prepared - flushed) * ms;
        g_doa3_phase_ms[4] = s_phase_prev_present ? s_phase_prev_present * ms : -1.0;
        g_doa3_phase_ms[5] = s_frame_perf.presented ? s_frame_perf.pace * ms : -1.0;
        g_doa3_phase_ms[6] = s_frame_perf.presented ? s_frame_perf.workers * ms : -1.0;
        g_doa3_phase_kicks = s_frame_perf.kicks;
    }
    if (!async_frame) doa3_present_frame();
    else {
        extern void doa3_present_async(int buf);
        LONG f = (LONG)async_frame - 1;
        doa3_present_async(s_buf_frame[f & 1] == f ? (int)(f & 1) : -1);
        InterlockedExchange(&s_frames_presented, f);
    }
    presented = entry ? doa3_perf_now() : 0;
    if (entry) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        double ms = 1000.0 / frequency.QuadPart;
        perf_5s(s_frame_perf.presented ? (presented - s_frame_perf.presented) * ms : -1.0,
                g_doa3_phase_ms, (presented - prepared) * ms, ms);
        if (perf_on && s_frame_perf.presented &&
            (presented - s_frame_perf.presented) * ms >= 25.0) {
            fprintf(stderr, "[PERF-PHASE] frame %u interval %.3f pace-prev %.3f workers-prev %.3f longest-worker %.3f before-flush %.3f pb-subset %.3f kicks %u flush %.3f auxiliary %.3f present %.3f end-qpc %lld\n",
                g_doa3_frames_presented,
                (presented - s_frame_perf.presented) * ms,
                s_frame_perf.pace * ms, s_frame_perf.workers * ms,
                s_frame_perf.longest_worker * ms,
                (entry - s_frame_perf.presented - s_frame_perf.pace) * ms,
                s_frame_perf.pb * ms, s_frame_perf.kicks,
                (flushed - entry) * ms, (prepared - flushed) * ms,
                (presented - prepared) * ms, (long long)presented);
        }
        s_frame_perf.presented = presented;
        s_phase_prev_present = presented - prepared;
        s_frame_perf.pb = s_frame_perf.workers = s_frame_perf.longest_worker = 0;
        s_frame_perf.kicks = 0;
    }
    LONGLONG pace_begin = entry ? doa3_perf_now() : 0;
    d3d4134_pace();
    if (entry) s_frame_perf.pace = doa3_perf_now() - pace_begin;
    else memset(&s_frame_perf, 0, sizeof s_frame_perf);
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
    unsigned async_flip_frame = 0;
    if (s_on < 0) { const char *e = getenv("DOA3_PB"); s_on = !(e && *e == '0'); }
    if (flags & 0x2000) {            /* no live GPU channel: the original only does bookkeeping */
        sub_001E27C0_gen();
        return;
    }
    cursor = (flags & 4) ? MEM32(dev + 0x35C) : MEM32(dev);
    start = MEM32(dev + 0x24); end = MEM32(dev + 0x28);
    g_pb4134_ring_lo = start; g_pb4134_ring_hi = end + 4;
    {   /* Asynchronous GPU: switching either way drains the queue first. */
        int want = pb_async_wanted();
        if (want != s_async_active) {
            if (s_async_on > 0) pb_wait(pb_is_drained, 0);
            s_async_active = want;
            if (want) {
                s_frames_done = s_frames_presented = (LONG)s_frames_submitted;
                s_buf_frame[0] = s_buf_frame[1] = -1;
                fprintf(stderr, "[ASYNC] render thread takes over at frame %u\n", g_doa3_frames_presented);
            } else
                fprintf(stderr, "[ASYNC] back to translating at kick-off (frame %u)\n", g_doa3_frames_presented);
        }
    }
    if (s_async_active) {
        /* Queue the spans, find the frame end with a header-only walk, and
         * leave DMA_GET and the fence semaphore to the render thread. */
        PbItem it;
        int flip = 0;
        memset(&it, 0, sizeof it);
        it.ring_lo = start; it.ring_hi = end + 4; it.translate = s_on;
        if (cursor >= start && cursor <= end) {
            if (s_pb4134_parsed > cursor && s_pb4134_parsed <= end) {
                pb4134_translate_span(s_pb4134_parsed, end + 4, 0, start, end + 4, &flip);
                it.from = s_pb4134_parsed; it.to = end + 4;
                pbq_push(&it);
                s_pb4134_parsed = start;
            }
            if (s_pb4134_parsed < start || s_pb4134_parsed > cursor)
                s_pb4134_parsed = start;
            it.from = s_pb4134_parsed; it.to = cursor;
            if (cursor > s_pb4134_parsed) pb4134_translate_span(s_pb4134_parsed, cursor, 0, start, end + 4, &flip);
            else it.translate = 0;                   /* nothing new: the marks only */
            s_pb4134_parsed = cursor;
        } else it.translate = 0;
        it.marks = 1;
        it.chan = MEM32(dev + 0x2264);
        it.sem = MEM32(dev + 0x34);
        it.semval = MEM32(dev + 0x30) - 2;           /* every fence inserted so far */
        it.cursor = cursor;
        if (flip) { it.flip = 1; it.frame = ++s_frames_submitted; async_flip_frame = it.frame; }
        pbq_push(&it);
        goto kick_diag;
    }
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
kick_diag:
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
            {   /* per-draw cost of the GPU fixed-function path (timing on only) */
                extern LONGLONG g_doa3_dt[6];
                extern unsigned g_doa3_dt_n, g_doa3_dt_all;
                static unsigned s_frames_at;
                if (g_doa3_dt_n) {
                    LARGE_INTEGER f; double us, tot = 0; int k;
                    unsigned frames = g_doa3_frames_presented - s_frames_at;
                    QueryPerformanceFrequency(&f);
                    us = 1e6 / (double)f.QuadPart / (double)g_doa3_dt_n;
                    for (k = 0; k < 6; k++) tot += (double)g_doa3_dt[k];
                    fprintf(stderr, "[DRAWTIME] last 5 s: %u GPU draws (%.0f per frame), us per draw: fetch %.2f state %.2f "
                            "constants %.2f combiners %.2f d3d-draw %.2f cleanup %.2f total %.2f (%.2f ms per frame)\n",
                            g_doa3_dt_all, frames ? (double)g_doa3_dt_all / frames : 0.0,
                            g_doa3_dt[0] * us, g_doa3_dt[1] * us, g_doa3_dt[2] * us, g_doa3_dt[3] * us,
                            g_doa3_dt[4] * us, g_doa3_dt[5] * us, tot * us,
                            frames ? tot * us * g_doa3_dt_all / frames / 1000.0 : 0.0);
                    for (k = 0; k < 6; k++) g_doa3_dt[k] = 0;
                    g_doa3_dt_n = 0; g_doa3_dt_all = 0;
                }
                {   /* where the rest of the draw time goes */
                    extern LONGLONG g_doa3_kt[6], g_doa3_vpt[3];
                    extern unsigned g_doa3_vpt_verts;
                    unsigned frames = g_doa3_frames_presented - s_frames_at;
                    LARGE_INTEGER f; double ms; int k;
                    QueryPerformanceFrequency(&f);
                    if (frames) {
                        ms = 1e3 / (double)f.QuadPart / (double)frames;
                        fprintf(stderr, "[DRAWCPU] last 5 s, ms per frame: GPU-path array draws %.2f (%.0f), CPU-built array draws %.2f (%.0f; "
                                "program prepass %.2f for %.0f vertices, vertex build %.2f), inline draws %.2f (%.0f)\n",
                                g_doa3_kt[0] * ms, (double)g_doa3_kt[3] / frames,
                                g_doa3_kt[1] * ms, (double)g_doa3_kt[4] / frames,
                                g_doa3_vpt[0] * ms, (double)g_doa3_vpt_verts / frames, g_doa3_vpt[1] * ms,
                                g_doa3_kt[2] * ms, (double)g_doa3_kt[5] / frames);
                    }
                    for (k = 0; k < 6; k++) g_doa3_kt[k] = 0;
                    for (k = 0; k < 3; k++) g_doa3_vpt[k] = 0;
                    g_doa3_vpt_verts = 0;
                }
                s_frames_at = g_doa3_frames_presented;
            }
            {   extern void pgraph_texdiag_report(void);
                pgraph_texdiag_report();   /* [TEXCACHE] */
            }
            if (s_async_on > 0) {
                static unsigned s_rt_frames_at;
                unsigned frames = g_doa3_frames_presented - s_rt_frames_at;
                LARGE_INTEGER f;
                QueryPerformanceFrequency(&f);
                if (frames)
                    fprintf(stderr, "[ASYNC] last 5 s: render thread busy %.2f ms per frame, %s\n",
                            (double)g_doa3_rthread_busy * 1e3 / (double)f.QuadPart / frames,
                            s_async_active ? "active" : "inactive (boot, a movie or netplay)");
                g_doa3_rthread_busy = 0;
                s_rt_frames_at = g_doa3_frames_presented;
            }
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
    if (s_async_active) {                            /* the render thread marks it consumed */
        MEM32(dev + 0x2C) = cursor;
        esp += 4;                                    /* fastcall, ret 0 */
        if (async_flip_frame) d3d4134_frame_done(dev, async_flip_frame);
        return;
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
        s_frames_submitted++;                        /* keeps the async frame count in step */
        d3d4134_frame_done(dev, 0);
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
#include "game/announcer_policy.h"
/* Game-volume guest calls for game_audio.c, made from the per-frame hook or
 * from inside the hooks below, always on the game thread. The caller-saved
 * registers are preserved so either context resumes untouched. */
void doa3_guest_set_music_volume(int percent)
{
    uint32_t sv_eax = eax, sv_ecx = ecx, sv_edx = edx;
    eax = (uint32_t)percent;            /* 0xA61C0: register argument, clamps,  */
    PUSH32(esp, 0);                     /* sets 0x2D3E9C, applies to every song */
    sub_000A61C0();
    eax = sv_eax; ecx = sv_ecx; edx = sv_edx;
}

void doa3_guest_apply_sfx_voice(void)
{
    uint32_t sv_eax = eax, sv_ecx = ecx, sv_edx = edx;
    PUSH32(esp, 0);                     /* 0x8A3F0: reads 0x59D095/96 (boot path) */
    sub_0008A3F0();
    eax = sv_eax; ecx = sv_ecx; edx = sv_edx;
}

/* ADXT_StartAfs(adxt, partition, fid), cdecl, no return value. */
void sub_00191DD0_gen(void);
void sub_00191DD0(void)
{
    uint32_t adxt = MEM32(esp + 4), part = MEM32(esp + 8), fid = MEM32(esp + 12);
    sub_00191DD0_gen();
    game_audio_on_adxt_start(adxt, part, fid);
}

/* Movie start (eax = index into the movie table at 0x2D4DD8). */
void sub_000A47E0_gen(void);
void sub_000A47E0(void)
{
    game_audio_on_movie_start(eax);     /* preserves eax */
    sub_000A47E0_gen();
}

/* SaveGame (no arguments). */
void sub_00020C80_gen(void);
void sub_00020C80(void)
{
    game_audio_before_save();
    sub_00020C80_gen();
    game_audio_after_save();
}


/* Filter Survival transitions at the common streamed-voice request entry.
 * Both direct calls and the game's sound wrappers reach this function. */
void sub_001C7920_gen(void);
void sub_001C7920(void)
{
    static Doa3AnnouncerState state;
    static int trace = -1;
    static unsigned reports;
    bool suppressed = doa3_announcer_suppress(&state, (uint16_t)eax,
        MEM8(0x5A2858), MEM8(0x596A74), MEM8(0x59CF58), MEM8(0x59CFC0),
        MEM16(0x59CF78), MEM16(0x59CFE0));
    if (trace < 0)
        trace = GetEnvironmentVariableA("DOA3_ANNOUNCER_TRACE", NULL, 0) != 0;
    if (trace && reports < 160 && (eax & 0xFFFFu) >= 0x8000u &&
        (eax & 0xFFFFu) <= 0x800Eu) {
        extern unsigned g_doa3_frames_presented;
        ++reports;
        fprintf(stderr, "[ANNOUNCER] frame=%u clip=%04X mode=%u phase=%u human=%u/%u hp=%u/%u start=%u active=%u end=%u wins=%u/%u bl=%u suppressed=%u\n",
                g_doa3_frames_presented, eax & 0xFFFFu, MEM8(0x5A2858),
                MEM8(0x596A74), MEM8(0x59CF58), MEM8(0x59CFC0),
                MEM16(0x59CF78), MEM16(0x59CFE0), MEM8(0x5A0EC8),
                MEM8(0x9B3722), MEM8(0x9B379C), MEM8(0x59CFB8),
                MEM8(0x59D020), LO8(ebx), suppressed ? 1u : 0u);
        fflush(stderr);
    }
    if (suppressed)
        esp += 4; /* Guest cdecl return: consume only the return address. */
    else
        sub_001C7920_gen();
    if (g_fib_slice_due) xbox_fiber_timeslice();
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

/* IDirectSound_SetI3DL2Listener(pDS, pParams, dwApply), stdcall 12 (0.0.170).
 * The room reverb runs on the console's GP DSP, which is not emulated; hand
 * the parameters to the host reverb (apu_dsp.c). The game takes them from a
 * table at 0x2D3EB8 (the standard I3DL2 presets, then its own rooms from
 * 0x2D4338), index in 0x7FE2F0; 0x2D4308 (index 23) is "no reverb". */
void sub_001F2A27_gen(void);
void sub_001F2A27(void)
{
    extern void apu_reverb_set_i3dl2(const uint32_t raw[12]);
    uint32_t p = MEM32(esp + 8);
    if (p >= 0x10000u && p < 0x08000000u - 48u) {
        static unsigned s_logged;
        uint32_t raw[12];
        int i;
        for (i = 0; i < 12; i++) raw[i] = MEM32(p + 4u * (uint32_t)i);
        apu_reverb_set_i3dl2(raw);
        if (s_logged < 200) {
            float f[12];
            s_logged++;
            memcpy(f, raw, sizeof f);
            fprintf(stderr, "[I3DL2] params %08X (table index %d): room %d roomHF %d decay %.3f hfratio %.3f "
                    "reflections %d delay %.3f reverb %d delay %.3f diffusion %.0f density %.0f hf %.0f\n",
                    p, (p >= 0x2D3EB8u && (p - 0x2D3EB8u) % 0x30u == 0) ? (int)((p - 0x2D3EB8u) / 0x30u) : -1,
                    (int)raw[0], (int)raw[1], f[3], f[4], (int)raw[5], f[6], (int)raw[7], f[8], f[9], f[10], f[11]);
        }
    }
    sub_001F2A27_gen();
}

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
            if (doa3_auto_capture_enabled() && (s_n == 30 || s_n == 200)) {
                /* What the guest decoded and converted: the 720x480 32bpp
                 * destination as movie_NNN.bmp, plus how much of it is lit. */
                char path[64]; FILE *f; unsigned nz = 0, k, y, x;
                for (k = 0; k < 2880u * 480u; k += 64) nz += MEM8(dst + k) != 0;
                fprintf(stderr, "[MOVIE] frame copy #%d: %u of %u sampled bytes non-zero\n",
                        s_n, nz, 2880u * 480u / 64u);
                sprintf(path, "movie_%03d.bmp", s_n);
                f = doa3_diag_fopen(path, "wb");
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
    extern int mcpx_apu_poll_idle(void);
    uint32_t v = ecx;
    uint32_t r_eax = eax, r_ecx = ecx, r_edx = edx, r_ebx = ebx, r_esi = esi,
             r_edi = edi, r_esp = esp, r_seh = g_seh_ebp;
    DWORD t0 = GetTickCount(), tlog = 0;
    static int s_n;
    static int s_perf = -1;
    LARGE_INTEGER perf_begin = {0}, perf_freq = {0};
    LONGLONG sleep_ticks = 0, max_sleep_ticks = 0;
    unsigned sleep_count = 0, idle_polls = 0;
    if (s_perf < 0) s_perf = getenv("DOA3_PERF_CAPTURE") != NULL;
    if ((MEM8(v + 0x12) & 1) && (MEM16(v + 0x12) & 0x8000)) {
        if (s_perf) { QueryPerformanceFrequency(&perf_freq); QueryPerformanceCounter(&perf_begin); }
        while (MEM16(v + 0x12) & 0x8000) {
            idle_polls += mcpx_apu_poll_idle();
            doa3_apu_deliver_irq();
            eax = r_eax; ecx = r_ecx; edx = r_edx; ebx = r_ebx; esi = r_esi;
            edi = r_edi; esp = r_esp; g_seh_ebp = r_seh;
            if (!(MEM16(v + 0x12) & 0x8000)) break;
            {   extern volatile int g_apu_det;
                extern void mcpx_apu_det_step(void);
                if (g_apu_det) { mcpx_apu_det_step(); continue; }
            }
            if (s_perf) {
                LARGE_INTEGER before, after;
                LONGLONG elapsed;
                QueryPerformanceCounter(&before);
                mcpx_apu_wait_irq(1);
                QueryPerformanceCounter(&after);
                elapsed = after.QuadPart - before.QuadPart;
                sleep_ticks += elapsed;
                if (elapsed > max_sleep_ticks) max_sleep_ticks = elapsed;
                ++sleep_count;
            } else mcpx_apu_wait_irq(1);
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
        if (s_perf) {
            LARGE_INTEGER end;
            QueryPerformanceCounter(&end);
            if (idle_polls || end.QuadPart - perf_begin.QuadPart > perf_freq.QuadPart / 500)
                fprintf(stderr, "[PERF-RETIRE] frame %u voice %08X total %.3f ms waits %u wait %.3f ms max %.3f ms idle-polls %u\n",
                    g_doa3_frames_presented, v,
                    1000.0 * (end.QuadPart - perf_begin.QuadPart) / perf_freq.QuadPart, sleep_count,
                    1000.0 * sleep_ticks / perf_freq.QuadPart, 1000.0 * max_sleep_ticks / perf_freq.QuadPart, idle_polls);
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
#endif /* DOA3_XBE_ID_JAPAN */

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
    /* Startup bring-up tracing is opt-in. The generated wrappers stop
     * entering this function once the first call resolves the setting. */
    if (g_fn_trace_on < 0) {
        const char *e = getenv("DOA3_TRACE_FN");
        g_fn_trace_on = 0;
        while (e && *e && s_fn_trace_cnt < 32) {
            char *end;
            unsigned long v = strtoul(e, &end, 16);
            if (end == e) break;
            s_fn_trace_va[s_fn_trace_cnt++] = (uint32_t)v;
            e = (*end == ',') ? end + 1 : end;
        }
        g_fn_trace_on = s_fn_trace_cnt > 0;
    }
    if (!g_fn_trace_on) return;
#if defined(DOA3_XBE_ID_JAPAN)
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
