/**
 * Dead or Alive 3 - Recompiled Game Host Entry Point
 *
 * Boots the statically-recompiled DOA3 code on Windows:
 *   load XBE -> map Xbox memory -> kernel init -> install fault-skip VEH ->
 *   call recompiled entry point (xbe_entry_point @ 0x001651A5).
 *
 * The VEH decoders (veh_skip_faulting_read / _write) are ported verbatim from
 * the burnout3 reference. They decode a faulting x86-64 instruction, return 0
 * for reads (and skip stores), and advance RIP so boot can push through wild
 * pointer probes and reveal real blockers. D3D/audio/input come later.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../kernel/kernel.h"
#include "../kernel/xbox_memory_layout.h"
#include "../kernel/xbox_fiber.h"
#include "../d3d/d3d8_xbox.h"
#include "../audio/dsound_xbox.h"
#include "recomp/gen/recomp_funcs.h"
#include "recomp/recomp_dispatch.h"   /* recomp_lookup for the CRT initializers */
#include "ui/doa3_ui.h"
#include "video_settings.h"

#define DOA3_ENTRY_POINT   0x001651A5
#define DOA3_XBE_PATH      "../doa3gamefiles/default.xbe"
#define DOA3_GAME_DIR      "../doa3gamefiles"

/* Host window + D3D8->D3D11 device (mirrors burnout3's graphics init). The NV2A
 * pgraph->D3D11 translator renders through this device via xbox_GetD3DDevice(). */
static HWND          g_hwnd;
static IDirect3D8   *g_d3d8;
static IDirect3DDevice8 *g_d3d_device;
static IDirectSound8 *g_dsound;   /* host DirectSound (mirrors burnout3) */

static LRESULT CALLBACK doa3_wndproc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    if (m == WM_CLOSE)   { DestroyWindow(h); return 0; }
    if (m == WM_DESTROY) { PostQuitMessage(0); return 0; }
    /* The Esc overlay gets first refusal on input: it opens/closes itself and,
     * while visible, keeps keyboard and mouse out of the game. */
    if (doa3_ui_wndproc(h, m, w, l)) return 0;
    return DefWindowProcA(h, m, w, l);
}

static HWND doa3_create_window(void)
{
    WNDCLASSA wc; RECT r = { 0, 0, 640, 480 };
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = doa3_wndproc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = "DOA3Window";
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    /* Black until the first frame: a borderless window covers the whole
     * monitor, and an unpainted one shows white blocks during boot. */
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassA(&wc);
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    HWND h = CreateWindowExA(0, "DOA3Window", "Dead or Alive 3",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             r.right - r.left, r.bottom - r.top, NULL, NULL,
                             wc.hInstance, NULL);
    /* Windowed or borderless fullscreen, from doa3_settings.ini. This sizes the
     * client area before the swap chain is created from it. */
    if (h) { video_apply_window_mode(h, video_get_window_mode()); UpdateWindow(h); }
    return h;
}

/* Create the D3D8->D3D11 device (burnout3 graphics init). */
static int doa3_init_graphics(void)
{
    D3DPRESENT_PARAMETERS pp;
    HRESULT hr;
    unsigned gw, gh;

    video_settings_load();
    {   /* online play: settings and winsock (no sockets until the user hosts or joins) */
        extern void netplay_init(void);
        netplay_init();
    }
    video_guest_target_size(video_get_aspect(), &gw, &gh);
    g_hwnd = doa3_create_window();
    if (!g_hwnd) { fprintf(stderr, "WARNING: window creation failed\n"); }

    g_d3d8 = xbox_Direct3DCreate8(0);
    if (!g_d3d8) { fprintf(stderr, "WARNING: xbox_Direct3DCreate8 failed\n"); return 0; }

    memset(&pp, 0, sizeof(pp));
    /* The guest frame buffer; the swap chain itself follows the window. */
    pp.BackBufferWidth = gw;
    pp.BackBufferHeight = gh;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.BackBufferCount = 1;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = g_hwnd;
    pp.Windowed = TRUE;
    pp.EnableAutoDepthStencil = TRUE;
    pp.AutoDepthStencilFormat = D3DFMT_D24S8;

    hr = g_d3d8->lpVtbl->CreateDevice(g_d3d8, 0, 0, g_hwnd, 0, &pp, &g_d3d_device);
    if (FAILED(hr)) { fprintf(stderr, "WARNING: CreateDevice failed 0x%08lX\n", (unsigned long)hr); return 0; }

    { extern void pgraph_d3d11_init(void); pgraph_d3d11_init(); }
    fprintf(stderr, "  Graphics: D3D8->D3D11 device created (guest target %ux%u, %s, %s)\n",
            gw, gh, video_get_aspect() == VIDEO_ASPECT_16_9 ? "16:9" : "4:3",
            video_get_window_mode() == VIDEO_BORDERLESS ? "borderless" : "windowed");
    return 1;
}

/* Present the host D3D8->D3D11 swapchain (flip the window). Called once per game
 * frame from the push-buffer translator in recomp_manual.c. Also pumps the Win32
 * message queue so the window stays responsive. (mirrors burnout's g_d3d_device
 * Present in its render loop.) */
/* Service the window's message queue WITHOUT presenting. The game only
 * presents from its frame loop; during the multi-second asset-load spins no
 * presents happen, the queue goes unserviced, and one click marks the window
 * "Not Responding" (users then close it, killing boot). Called from the
 * load-wait wrappers (main thread). */
void doa3_pump_messages(void)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            extern void netplay_shutdown(void);
            fprintf(stderr, "[EXIT] window closed by user%c", 10);
            fflush(stderr);
            netplay_shutdown();
            ExitProcess(0);
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

/* ── Hang watchdog ────────────────────────────────────────────────────────
 * The post-movie failures in this project are non-faulting hangs: the process
 * stays alive, every capped diagnostic has gone quiet, and the log simply
 * stops. There is nothing to catch with an exception handler, so sample the
 * guest thread instead. g_doa3_heartbeat is bumped once per present; if it
 * stops moving, suspend the guest thread, read its RIP from the thread
 * context and print it with the recompiler's register globals. Resolve the
 * RIP against build/debug/doa3.map ("Rva+Base" column) to get the function.
 * Always on: it costs one sleeping thread and it is the only thing that
 * localises this class of failure. */
volatile LONG g_doa3_heartbeat = 0;
static HANDLE g_doa3_guest_thread;

static DWORD WINAPI doa3_watchdog(LPVOID unused)
{
    LONG last = -1; int stalled = 0, reports = 0;
    (void)unused;
    for (;;) {
        LONG now;
        Sleep(2000);
        now = g_doa3_heartbeat;

        if (now != last) { last = now; stalled = 0; continue; }
        /* Never arm before the first present: the heartbeat is legitimately 0
         * through boot, and a "stall" there is meaningless. */
        if (now == 0) continue;
        if (++stalled < 3 || reports >= 10) continue;   /* ~6s of no progress */
        reports++;
        {
            CONTEXT ctx;
            int got = 0;
            memset(&ctx, 0, sizeof ctx);
            ctx.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
            /* CAPTURE ONLY while the guest thread is suspended -- no stdio.
             * The guest is very often inside fprintf holding the CRT stream
             * lock; printing from here while it is suspended deadlocks the
             * process (it froze boot exactly once and cost a run to find). */
            if (SuspendThread(g_doa3_guest_thread) != (DWORD)-1) {
                got = GetThreadContext(g_doa3_guest_thread, &ctx);
                ResumeThread(g_doa3_guest_thread);
            }
            if (got)
                fprintf(stderr,
                        "[WDOG] STALLED %ds at heartbeat %ld: rip=0x%llX rsp=0x%llX | "
                        "guest eax=%08X ecx=%08X edx=%08X ebx=%08X esp=%08X esi=%08X edi=%08X sehebp=%08X\n",
                        stalled * 2, (long)now,
                        (unsigned long long)ctx.Rip, (unsigned long long)ctx.Rsp,
                        g_eax, g_ecx, g_edx, g_ebx, g_esp, g_esi, g_edi, g_seh_ebp);
            else
                fprintf(stderr, "[WDOG] STALLED but could not read guest context\n");
            /* Fiber states: read after resuming, so this never contends with
             * the guest for the stdio lock while it cannot run. */
            if (reports <= 2) {
                extern void xbox_fiber_dump(void);
                xbox_fiber_dump();
            }
            fflush(stderr);
        }
    }
}

static void doa3_watchdog_start(void)
{
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
                    GetCurrentProcess(), &g_doa3_guest_thread,
                    0, FALSE, DUPLICATE_SAME_ACCESS);
    if (g_doa3_guest_thread) {
        HANDLE h = CreateThread(NULL, 0, doa3_watchdog, NULL, 0, NULL);
        if (h) CloseHandle(h);
    }
}

void doa3_present_frame(void)
{
    InterlockedIncrement(&g_doa3_heartbeat);
    {   /* DOA3 DIAG: render command list occupancy.
         * list 1 = 0x00A1F388 .. write ptr [0x00B1F390]  (1 MB)
         * list 2 = 0x0099F378 .. write ptr [0x00A1F378]  (1 MB)
         * Both write-pointer globals sit immediately above their buffer, so
         * an overflow destroys the pointer itself and everything above it.
         * sub_00158BE0 appends without any bound check; the only reset is
         * sub_00158B60, once per frame from sub_000B8855. */
        extern volatile int g_doa3_post_movie;
        static DWORD s_next = 0;
        static uint32_t s_hi1 = 0, s_hi2 = 0, s_over = 0, s_frames = 0;
        uint32_t w1 = MEM32(0x00B1F390u), w2 = MEM32(0x00A1F378u);
        uint32_t u1 = (w1 >= 0x00A1F388u && w1 <= 0x00B1F388u) ? w1 - 0x00A1F388u : 0xFFFFFFFFu;
        uint32_t u2 = (w2 >= 0x0099F378u && w2 <= 0x00A1F378u) ? w2 - 0x0099F378u : 0xFFFFFFFFu;
        s_frames++;
        if (u1 == 0xFFFFFFFFu || u2 == 0xFFFFFFFFu) s_over++;
        else { if (u1 > s_hi1) s_hi1 = u1; if (u2 > s_hi2) s_hi2 = u2; }
        if (g_doa3_post_movie && GetTickCount() >= s_next) {
            s_next = GetTickCount() + 2000;
            fprintf(stderr, "  [LIST] frames=%u list1=%u/1048576 (hi %u) list2=%u/1048576 (hi %u) bad_ptr_frames=%u w1=%08X w2=%08X\n",
                    s_frames, (u1 == 0xFFFFFFFFu) ? 0 : u1, s_hi1,
                    (u2 == 0xFFFFFFFFu) ? 0 : u2, s_hi2, s_over, w1, w2);
            fflush(stderr);
            s_frames = 0;
        }
    }
    {   /* DOA3 DIAG: which surface the guest device is rendering into at
         * present time (device+0x40C), sampled every ~2 s post-movie. */
        extern volatile int g_doa3_post_movie;
        static DWORD s_next = 0;
        if (g_doa3_post_movie && GetTickCount() >= s_next) {
            s_next = GetTickCount() + 2000;
            fprintf(stderr, "[RT@PRESENT] dev40C=%08X dev5A0[0]=%g\n", MEM32(0x1C0C0Cu), MEMF(0x1C0DA0u));
        }
    }
    {   /* DOA3 DIAG: APU state every ~2 s (audio bring-up) */
        extern void apu_debug_stats_line(void);
        static DWORD s_next = 0;
        { extern void doa3_apu_deliver_irq(void); doa3_apu_deliver_irq(); }
        if (GetTickCount() >= s_next) { s_next = GetTickCount() + 2000; apu_debug_stats_line();
             }
    }
    if (getenv("DOA3_NO_PRESENT")) return;   /* isolation: skip pump + present entirely */
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            /* The recompiled game never returns to us — hard-exit the host
             * when the user closes the window. */
            extern void netplay_shutdown(void);
            fprintf(stderr, "[EXIT] window closed by user\n");
            fflush(stderr);
            netplay_shutdown();
            ExitProcess(0);
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    if (g_d3d_device)
        g_d3d_device->lpVtbl->Present(g_d3d_device, NULL, NULL, NULL, NULL);
    if (getenv("DOA3_THROTTLE")) Sleep(15);   /* legacy rig knob */

    /* item 101: presents are vsync-OFF (a blocking Present held the ONE
     * thread hosting every decode fiber at the monitor rate: the loop ran 40
     * rounds/s on the user's desktop -> the movie served 20fps, and the boot
     * spins crawled through their 30s black phase). Do NOT re-add a QPC
     * "self-pace to 60Hz, spend slack in yields" loop here (item 98 fix B):
     * re-tried 2026-07-05 (doa3_log_r6.txt) and it collapsed the loop to 2
     * rounds/s — each slack yield can burn unbounded decode work (item 94's
     * budget-collapse mechanism). Pacing comes from the wall-time movie
     * clock (recomp_manual.c synthetic-clock block); free-running the loop
     * is safe since that clock stopped being pump-rate-derived. */
}

extern volatile uint32_t g_icall_trace[16];
extern volatile uint32_t g_icall_trace_idx;
extern volatile uint64_t g_icall_count;

/* ── Fault-skip decoders (ported verbatim from burnout3/src/game/main.c) ── */
static BOOL veh_skip_faulting_read(PCONTEXT ctx)
{
    uint8_t *rip = (uint8_t *)ctx->Rip;
    int prefix_len = 0;
    int rex_w = 0, rex_r = 0, rex_x = 0, rex_b = 0;

    /* Map register index to CONTEXT field */
    DWORD64 *gpr[] = {
        &ctx->Rax, &ctx->Rcx, &ctx->Rdx, &ctx->Rbx,
        &ctx->Rsp, &ctx->Rbp, &ctx->Rsi, &ctx->Rdi,
        &ctx->R8,  &ctx->R9,  &ctx->R10, &ctx->R11,
        &ctx->R12, &ctx->R13, &ctx->R14, &ctx->R15
    };

    /* Parse legacy prefixes (segment, operand size, etc.) */
    while (prefix_len < 4) {
        uint8_t b = rip[prefix_len];
        if (b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 ||
            b == 0x2E || b == 0x3E || b == 0x26 || b == 0x36 ||
            b == 0x64 || b == 0x65) {
            prefix_len++;
        } else {
            break;
        }
    }

    /* Parse REX prefix (0x40-0x4F) */
    if ((rip[prefix_len] & 0xF0) == 0x40) {
        uint8_t rex = rip[prefix_len];
        rex_w = (rex >> 3) & 1;
        rex_r = (rex >> 2) & 1;
        rex_x = (rex >> 1) & 1;
        rex_b = rex & 1;
        prefix_len++;
    }

    uint8_t *op = rip + prefix_len;

    /* Calculate ModRM displacement length */
    /* Returns total bytes for modrm + optional SIB + displacement */
    #define MODRM_LEN(modrm_byte) do { \
        int _mod = ((modrm_byte) >> 6) & 3; \
        int _rm  = ((modrm_byte) & 7) | (rex_b << 3); \
        modrm_total = 1; /* modrm byte itself */ \
        if (_mod == 0 && (_rm & 7) == 4) modrm_total++; /* SIB */ \
        if (_mod == 0 && (_rm & 7) == 5) modrm_total += 4; /* RIP-rel disp32 */ \
        if (_mod == 1) { modrm_total++; if ((_rm & 7) == 4) modrm_total++; } \
        if (_mod == 2) { modrm_total += 4; if ((_rm & 7) == 4) modrm_total++; } \
        if (_mod == 3) modrm_total = 1; /* reg-reg, shouldn't fault */ \
    } while(0)

    int modrm_total = 0;
    int reg_idx;

    /* 8B /r : mov r32/r64, r/m32/r/m64 */
    if (op[0] == 0x8B) {
        reg_idx = ((op[1] >> 3) & 7) | (rex_r << 3);
        MODRM_LEN(op[1]);
        *gpr[reg_idx] = 0;
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* 8A /r : mov r8, r/m8 */
    if (op[0] == 0x8A) {
        reg_idx = ((op[1] >> 3) & 7) | (rex_r << 3);
        MODRM_LEN(op[1]);
        /* Zero just the low byte of the register */
        *gpr[reg_idx] &= ~(DWORD64)0xFF;
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* 0F B6 /r : movzx r32, r/m8 */
    /* 0F B7 /r : movzx r32, r/m16 */
    /* 0F BE /r : movsx r32, r/m8 */
    /* 0F BF /r : movsx r32, r/m16 */
    if (op[0] == 0x0F && (op[1] == 0xB6 || op[1] == 0xB7 ||
                           op[1] == 0xBE || op[1] == 0xBF)) {
        reg_idx = ((op[2] >> 3) & 7) | (rex_r << 3);
        MODRM_LEN(op[2]);
        *gpr[reg_idx] = 0;
        ctx->Rip += prefix_len + 2 + modrm_total;
        return TRUE;
    }

    /* 3B /r : cmp r32, r/m32 - set flags as if comparing with 0 */
    if (op[0] == 0x3B) {
        reg_idx = ((op[1] >> 3) & 7) | (rex_r << 3);
        MODRM_LEN(op[1]);
        /* Set ZF=0, CF based on comparison with 0 */
        DWORD64 val = *gpr[reg_idx];
        ctx->EFlags &= ~(0x8D5);  /* clear OF, SF, ZF, AF, PF, CF */
        if (val == 0) ctx->EFlags |= 0x40;  /* ZF */
        if (val & (rex_w ? 0x8000000000000000ULL : 0x80000000ULL))
            ctx->EFlags |= 0x80;  /* SF */
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* 39 /r : cmp r/m32, r32 - set flags as if mem=0 */
    if (op[0] == 0x39) {
        reg_idx = ((op[1] >> 3) & 7) | (rex_r << 3);
        MODRM_LEN(op[1]);
        DWORD64 val = *gpr[reg_idx];
        ctx->EFlags &= ~(0x8D5);
        if (val == 0) ctx->EFlags |= 0x40;  /* ZF: 0 == val */
        /* 0 - val: CF set if val != 0 */
        if (val != 0) ctx->EFlags |= 0x01;  /* CF */
        /* SF: sign of (0 - val) */
        DWORD64 result = (rex_w ? (DWORD64)(-(int64_t)val) : (DWORD64)(uint32_t)(-(int32_t)(uint32_t)val));
        if (result & (rex_w ? 0x8000000000000000ULL : 0x80000000ULL))
            ctx->EFlags |= 0x80;  /* SF */
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* SSE instructions with memory operands.
     * These use legacy prefixes (F3/F2/66/none) + 0F opcode + modrm.
     * For faulting memory reads, zero the destination XMM register
     * and advance RIP to let execution continue.
     */
    {
        int has_f3 = 0, has_f2 = 0, has_66 = 0;
        for (int i = 0; i < prefix_len; i++) {
            if (rip[i] == 0xF3) has_f3 = 1;
            if (rip[i] == 0xF2) has_f2 = 1;
            if (rip[i] == 0x66) has_66 = 1;
        }

        if (op[0] == 0x0F) {
            int is_sse_mem_read = 0;

            /* F3 0F xx: scalar single-precision */
            if (has_f3) {
                switch (op[1]) {
                case 0x10: /* movss xmm, m32 */
                case 0x58: /* addss xmm, m32 */
                case 0x59: /* mulss xmm, m32 */
                case 0x5C: /* subss xmm, m32 */
                case 0x5E: /* divss xmm, m32 */
                case 0x51: /* sqrtss xmm, m32 */
                case 0x5D: /* minss xmm, m32 */
                case 0x5F: /* maxss xmm, m32 */
                case 0x2A: /* cvtsi2ss xmm, r/m32 */
                case 0x2C: /* cvttss2si r32, xmm/m32 */
                case 0x2D: /* cvtss2si r32, xmm/m32 */
                    is_sse_mem_read = 1;
                    break;
                }
            }
            /* F2 0F xx: scalar double-precision */
            else if (has_f2) {
                switch (op[1]) {
                case 0x10: /* movsd xmm, m64 */
                case 0x58: /* addsd */
                case 0x59: /* mulsd */
                case 0x5C: /* subsd */
                case 0x5E: /* divsd */
                    is_sse_mem_read = 1;
                    break;
                }
            }
            /* 66 0F xx: packed double / integer */
            else if (has_66) {
                switch (op[1]) {
                case 0x28: /* movapd xmm, m128 */
                case 0x10: /* movupd xmm, m128 */
                case 0x6F: /* movdqa xmm, m128 */
                    is_sse_mem_read = 1;
                    break;
                }
            }
            /* No prefix: packed single-precision */
            else {
                switch (op[1]) {
                case 0x28: /* movaps xmm, m128 */
                case 0x10: /* movups xmm, m128 */
                case 0x58: /* addps xmm, m128 */
                case 0x59: /* mulps xmm, m128 */
                case 0x5C: /* subps xmm, m128 */
                case 0x5E: /* divps xmm, m128 */
                    is_sse_mem_read = 1;
                    break;
                }
            }

            if (is_sse_mem_read) {
                int xmm_idx = ((op[2] >> 3) & 7) | (rex_r << 3);
                MODRM_LEN(op[2]);

                /* For cvttss2si/cvtss2si, dest is GPR, not XMM */
                if ((has_f3 && (op[1] == 0x2C || op[1] == 0x2D))) {
                    *gpr[xmm_idx] = 0;
                } else if (xmm_idx < 16) {
                    M128A *xmm = &ctx->Xmm0 + xmm_idx;
                    xmm->Low = 0;
                    xmm->High = 0;
                }
                ctx->Rip += prefix_len + 2 + modrm_total;
                return TRUE;
            }
        }
    }

    #undef MODRM_LEN
    return FALSE;
}

/**
 * Skip a faulting write instruction by advancing RIP past it.
 * Unlike read skips, write skips don't need to set a register.
 * Handles common store instructions: mov r/m, r and mov r/m, imm.
 */
static BOOL veh_skip_faulting_write(PCONTEXT ctx)
{
    uint8_t *rip = (uint8_t *)ctx->Rip;
    int prefix_len = 0;
    int rex_w = 0, rex_r = 0, rex_b = 0;

    /* Parse legacy prefixes */
    while (prefix_len < 4) {
        uint8_t b = rip[prefix_len];
        if (b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 ||
            b == 0x2E || b == 0x3E || b == 0x26 || b == 0x36 ||
            b == 0x64 || b == 0x65) {
            prefix_len++;
        } else {
            break;
        }
    }

    /* Parse REX prefix */
    if ((rip[prefix_len] & 0xF0) == 0x40) {
        uint8_t rex = rip[prefix_len];
        rex_w = (rex >> 3) & 1;
        rex_r = (rex >> 2) & 1;
        rex_b = rex & 1;
        prefix_len++;
    }

    uint8_t *op = rip + prefix_len;
    int modrm_total = 0;

    #define MODRM_LEN(modrm_byte) do { \
        int _mod = ((modrm_byte) >> 6) & 3; \
        int _rm  = ((modrm_byte) & 7) | (rex_b << 3); \
        modrm_total = 1; \
        if (_mod == 0 && (_rm & 7) == 4) modrm_total++; \
        if (_mod == 0 && (_rm & 7) == 5) modrm_total += 4; \
        if (_mod == 1) { modrm_total++; if ((_rm & 7) == 4) modrm_total++; } \
        if (_mod == 2) { modrm_total += 4; if ((_rm & 7) == 4) modrm_total++; } \
        if (_mod == 3) modrm_total = 1; \
    } while(0)

    (void)rex_r; (void)rex_w;

    /* 89 /r : mov r/m32, r32 */
    if (op[0] == 0x89) {
        MODRM_LEN(op[1]);
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* 88 /r : mov r/m8, r8 */
    if (op[0] == 0x88) {
        MODRM_LEN(op[1]);
        ctx->Rip += prefix_len + 1 + modrm_total;
        return TRUE;
    }

    /* C7 /0 id : mov r/m32, imm32 */
    if (op[0] == 0xC7) {
        MODRM_LEN(op[1]);
        ctx->Rip += prefix_len + 1 + modrm_total + 4;
        return TRUE;
    }

    /* C6 /0 ib : mov r/m8, imm8 */
    if (op[0] == 0xC6) {
        MODRM_LEN(op[1]);
        ctx->Rip += prefix_len + 1 + modrm_total + 1;
        return TRUE;
    }

    /* 66 89 /r : mov r/m16, r16 (handled via 0x66 prefix + 89) */
    /* Already handled above since 0x66 is parsed as prefix */

    /* 0F 11 /r : movups xmm, m128 (SSE store) */
    if (op[0] == 0x0F && op[1] == 0x11) {
        MODRM_LEN(op[2]);
        ctx->Rip += prefix_len + 2 + modrm_total;
        return TRUE;
    }

    /* F3 0F 11 /r : movss m32, xmm (SSE scalar store) */
    {
        int has_f3 = 0;
        for (int i = 0; i < prefix_len; i++) {
            if (rip[i] == 0xF3) has_f3 = 1;
        }
        if (has_f3 && op[0] == 0x0F && op[1] == 0x11) {
            MODRM_LEN(op[2]);
            ctx->Rip += prefix_len + 2 + modrm_total;
            return TRUE;
        }

        /* F3 A4 : rep movsb (inline memcpy)
         * F3 A5 : rep movsd (inline memcpy, 4-byte)
         * F3 AA : rep stosb (inline memset)
         * F3 AB : rep stosd (inline memset, 4-byte)
         *
         * Cancel remaining iterations: set RCX=0, advance RSI/RDI past
         * the unmapped region. The rep prefix with RCX=0 is a no-op,
         * so the CPU will naturally advance RIP past the instruction.
         * Assumes DF=0 (CLD), which is standard for MSVC code.
         */
        if (has_f3 && (op[0] == 0xA4 || op[0] == 0xA5)) {
            /* rep movsb / rep movsd */
            uint64_t stride = (op[0] == 0xA5) ? 4 : 1;
            uint64_t remaining = ctx->Rcx * stride;
            ctx->Rcx = 0;
            ctx->Rsi += remaining;
            ctx->Rdi += remaining;
            return TRUE;
        }
        if (has_f3 && (op[0] == 0xAA || op[0] == 0xAB)) {
            /* rep stosb / rep stosd */
            uint64_t stride = (op[0] == 0xAB) ? 4 : 1;
            uint64_t remaining = ctx->Rcx * stride;
            ctx->Rcx = 0;
            ctx->Rdi += remaining;
            return TRUE;
        }
    }

    #undef MODRM_LEN
    return FALSE;
}

/* ── Crash / fault-skip VEH ─────────────────────────────────────── */

static uint64_t g_fault_skips = 0;
static uint64_t g_fault_logged = 0;
#define XBOX_MEM_SIZE (64u * 1024u * 1024u)

/* ── Write-watch (diagnostic): page-protect a guest range and log the RIP of
 * every writer via AV + single-step re-arm. Used to catch the corruption of
 * the mwPly slot table (guest 0xC0E544..0xC0EFFF). ── */
static volatile uintptr_t g_watch_page = 0;     /* host page base (0 = off) */
static volatile int g_watch_pending = 0;        /* reprotect after single-step */
static uint32_t g_watch_rips_seen[24];
static int g_watch_rips_n = 0;

/* DOA3 DIAG: opt-in exact-address write watch, DOA3_WATCHVA=<hex guest VA>.
 * Arms the containing page at startup and reports every writer RIP plus the
 * value stored. Used to find who writes the mwPly handle state word. */
uint32_t g_watch_exact_va = 0;
uint32_t g_watch_exact_len = 8;   /* DOA3_WATCHLEN=<hex bytes> widens the window */

/* DOA3 DIAG: the watched page is protected in EVERY view of the RAM
 * mapping (base + the 28 mirror views at xbox_GetMemorySize() stride,
 * which includes the on-demand 0x80000000 cached-RAM mirror), so a write
 * through an alias address is caught too. The partition table at 0x4BDA20
 * was corrupted during the movie with the primary-view watch armed and
 * silent, which is only possible through an alias or the host. */
static uint32_t g_watch_pageoff = 0;    /* page offset inside one view */
static void watch_protect_all(DWORD prot)
{
    uintptr_t base = (uintptr_t)xbox_GetMemoryBase();
    size_t sz = xbox_GetMemorySize();
    DWORD old; int m;
    for (m = 0; m <= XBOX_NUM_MIRRORS; m++)
        VirtualProtect((LPVOID)(base + (uintptr_t)m * sz + g_watch_pageoff), 0x1000, prot, &old);
}
/* host address -> guest offset inside the view it belongs to (any view),
 * or 0xFFFFFFFF when it is not inside the RAM mapping at all. */
static uint32_t watch_view_offset(uintptr_t f)
{
    uintptr_t base = (uintptr_t)xbox_GetMemoryBase();
    size_t sz = xbox_GetMemorySize();
    if (!sz || f < base || f >= base + (uintptr_t)(XBOX_NUM_MIRRORS + 1) * sz) return 0xFFFFFFFFu;
    return (uint32_t)((f - base) % sz);
}
void doa3_watch_arm(uint32_t xb_page)
{
    uintptr_t host = (uintptr_t)g_xbox_mem_offset + xb_page;
    DWORD old;
    if (VirtualProtect((LPVOID)host, 0x1000, PAGE_READONLY, &old)) {
        g_watch_page = host;
        g_watch_pageoff = (uint32_t)(host - (uintptr_t)xbox_GetMemoryBase());
        watch_protect_all(PAGE_READONLY);
    }
}

void doa3_watch_disarm(void)
{
    if (g_watch_page) {
        watch_protect_all(PAGE_READWRITE);
        g_watch_page = 0;
    }
}

/* DOA3: generic "watch one field of a strided array" filter, used to find
 * who overwrites the vtable pointers of the 175-object array at 0x370C48
 * that sub_000E8BB0 dispatches through. Armed from the dispatch loop once
 * the array has been verified intact, so any hit is real corruption. */
uint32_t g_watch_arr_base = 0, g_watch_arr_stride = 0, g_watch_arr_n = 0;

static uint32_t g_watch_last_va = 0;
static uintptr_t g_watch_last_rip = 0;
static uint32_t g_watch_last_old = 0;

/* item 102: sfdec picture-slot STATE words (h=0xC0F7C0, +0x366C + 0x50*i).
 * Slot 6 (0xC1300C) is off the watched page; slots 0-5 suffice. The slot
 * lifecycle (0 free -> decoded 2 -> served -> back to 0) is the item-95
 * killer: stalled runs park ALL slots at state 2 (release never runs). */
static int watch_slot_state(uint32_t xbva)
{
    if (xbva < 0xC12E2Cu || xbva > 0xC12FBCu) return -1;
    uint32_t rel = xbva - 0xC12E2Cu;
    if (rel % 0x50u) return -1;
    return (int)(rel / 0x50u);
}
/* DOA3 diag (opt-in, DOA3_EBXWP=1): hardware data breakpoint on the
 * recompiler's g_ebx global. The boot task sub_00084340 zeroes ebx once and
 * relies on it staying 0 (callee-saved) through its one-time init chain, but
 * measured at its loop top ebx already holds a stack-looking value, so the
 * gate that clears 0x4B838A after the intro movie never fires. Armed by the
 * sub_00084340 wrapper and disarmed at its loop top, both from the guest
 * thread by raising a private exception: the VEH edits Dr0/Dr7 in the
 * continuation context and NtContinue applies them to the thread. Every
 * fiber runs on this one thread, so the breakpoint follows all of them. */
#define DOA3_EXC_WPARM 0xE0D0A301u
#define DOA3_EXC_WPOFF 0xE0D0A302u
static int g_ebxwp_armed = 0, g_ebxwp_reports = 0, g_ebxwp_exact = 0;
void doa3_ebxwp_arm(void)
{
    const char *e = getenv("DOA3_EBXWP");
    if (e && *e == '1') RaiseException(DOA3_EXC_WPARM, 0, 0, NULL);
}
void doa3_ebxwp_disarm(void)
{
    if (g_ebxwp_armed) RaiseException(DOA3_EXC_WPOFF, 0, 0, NULL);
}
/* Pause across the intro sequencer (sub_00021F70 runs the whole opening
 * movie; a data breakpoint through the decoder would starve it). */
static int g_ebxwp_paused = 0;
void doa3_ebxwp_pause(void)
{
    if (g_ebxwp_armed) { g_ebxwp_paused = 1; doa3_ebxwp_disarm(); }
}
void doa3_ebxwp_resume(void)
{
    if (g_ebxwp_paused) { g_ebxwp_paused = 0; doa3_ebxwp_arm(); }
}
/* Same, plus esp drift: in this recomp model a callee pops only the dummy
 * return slot, so esp must come back exactly where it was before the
 * PUSH32(esp, 0) -- except for callee-clean (ret N) functions, which show a
 * constant positive delta at every site. Deduped per (caller, callee). */
void doa3_cs_report2(const char *caller, const char *callee, uint32_t site, uint32_t before, uint32_t esp_before)
{
    static struct { const char *a, *b; uint32_t site; int n; } seen[512];
    static int nseen = 0;
    int i;
    for (i = 0; i < nseen; i++)
        if (seen[i].a == caller && seen[i].b == callee && seen[i].site == site) break;
    if (i == nseen) { if (nseen >= 512) return; seen[nseen].a = caller; seen[nseen].b = callee; seen[nseen].site = site; seen[nseen].n = 0; nseen++; }
    if (seen[i].n >= 3) return;
    seen[i].n++;
    fprintf(stderr, "[CSCHK2] %s: %s (site 0x%08X) ebx %08X -> %08X, esp %08X -> %08X (d=%+d) fiber=%d\n",
            caller, callee, site, before, g_ebx, esp_before, g_esp, (int)(g_esp - esp_before), xbox_fiber_current());
    fflush(stderr);
}
/* Callee-saved ebx/esi/edi + esp drift, deduped per (caller, callee, site).
 * Emitted into the vertex-block walker sub_00157700 and its callees. */
void doa3_cs_report3(const char *caller, const char *callee, uint32_t site,
                     uint32_t b, uint32_t sp, uint32_t si, uint32_t di)
{
    /* Retired: the register-drift report this produced is no longer read,
     * and its 512-entry linear search ran on every drifting call (1-1.7%
     * of the game thread in fights). The generated call sites stay. */
    (void)caller; (void)callee; (void)site; (void)b; (void)sp; (void)si; (void)di;
    return;
    static struct { const char *a, *b; uint32_t site; int n; } seen[512];
    static int nseen = 0;
    int i;
    for (i = 0; i < nseen; i++)
        if (seen[i].a == caller && seen[i].b == callee && seen[i].site == site) break;
    if (i == nseen) { if (nseen >= 512) return; seen[nseen].a = caller; seen[nseen].b = callee; seen[nseen].site = site; seen[nseen].n = 0; nseen++; }
    if (seen[i].n >= 3) return;
    seen[i].n++;
    fprintf(stderr, "[CSCHK3] %s: %s (site 0x%08X)%s%s%s%s ebx %08X->%08X esi %08X->%08X edi %08X->%08X esp %08X->%08X (d=%+d) fiber=%d\n",
            caller, callee, site,
            (g_ebx != b) ? " EBX" : "", (g_esi != si) ? " ESI" : "", (g_edi != di) ? " EDI" : "", (g_esp != sp) ? " ESP" : "",
            b, g_ebx, si, g_esi, di, g_edi, sp, g_esp, (int)(g_esp - sp), xbox_fiber_current());
    fflush(stderr);
}
/* Call-site callee-saved check emitted into sub_00084340_gen and
 * sub_00021F70_gen: reports a callee that returned with ebx changed. */
void doa3_cs_report(const char *caller, const char *callee, uint32_t site, uint32_t before)
{
    static int n = 0;
    if (n >= 60) return;
    n++;
    fprintf(stderr, "[CSCHK] %s: %s (site 0x%08X) returned with ebx %08X -> %08X esp=%08X fiber=%d\n",
            caller, callee, site, before, g_ebx, g_esp, xbox_fiber_current());
    fflush(stderr);
}
/* Walk the guest thread's native frames from an exception context. */
static void doa3_ctx_backtrace(const CONTEXT *src, int max)
{
    CONTEXT c = *src;
    int i;
    for (i = 0; i < max && c.Rip; i++) {
        DWORD64 base = 0; PVOID hd = NULL; DWORD64 est = 0;
        PRUNTIME_FUNCTION rf = RtlLookupFunctionEntry(c.Rip, &base, NULL);
        fprintf(stderr, " %llX", (unsigned long long)c.Rip);
        if (!rf) { c.Rip = *(DWORD64 *)c.Rsp; c.Rsp += 8; continue; }
        RtlVirtualUnwind(UNW_FLAG_NHANDLER, base, c.Rip, rf, &c, &hd, &est, NULL);
    }
}
/* returns 1 when the exception was ours (always continue execution) */
static int doa3_ebxwp_veh(DWORD code, PEXCEPTION_POINTERS info)
{
    CONTEXT *c = info->ContextRecord;
    if (code == DOA3_EXC_WPARM || code == DOA3_EXC_WPOFF) {
        c->ContextFlags |= CONTEXT_DEBUG_REGISTERS;
        if (code == DOA3_EXC_WPARM) {
            c->Dr0 = (DWORD64)(uintptr_t)&g_ebx;
            /* L0 | RW0=01 (write) | LEN0=11 (4 bytes) */
            c->Dr7 = (c->Dr7 & ~0xF0003ull) | 0x1ull | (0x1ull << 16) | (0x3ull << 18);
            g_ebxwp_armed = 1;
        } else {
            c->Dr7 &= ~0xF0003ull; c->Dr0 = 0; g_ebxwp_armed = 0;
        }
        fprintf(stderr, "[EBXWP] %s (g_ebx at %p, fiber=%d)\n",
                g_ebxwp_armed ? "armed" : "disarmed", (void *)&g_ebx, xbox_fiber_current());
        fflush(stderr);
        return 1;
    }
    if (code == EXCEPTION_SINGLE_STEP && (c->Dr6 & 1)) {
        uint32_t v = g_ebx;
        int exact = (v == 0x024AAEA4u);
        c->Dr6 &= ~0xFull;
        if ((exact && g_ebxwp_exact < 20) ||
            (!exact && v >= 0x02000000u && v < 0x03000000u && g_ebxwp_reports < 80)) {
            if (exact) g_ebxwp_exact++; else g_ebxwp_reports++;
            fprintf(stderr, "[EBXWP] g_ebx=%08X%s esp=%08X fiber=%d rip=0x%llX bt:",
                    v, exact ? " EXACT" : "", g_esp, xbox_fiber_current(),
                    (unsigned long long)c->Rip);
            doa3_ctx_backtrace(c, 10);
            fprintf(stderr, "\n");
            fflush(stderr);
        }
        return g_watch_pending ? 0 : 1;
    }
    return 0;
}
static LONG WINAPI crash_veh(PEXCEPTION_POINTERS info)
{
    DWORD code = info->ExceptionRecord->ExceptionCode;
    if (doa3_ebxwp_veh(code, info)) return EXCEPTION_CONTINUE_EXECUTION;
    /* write-watch single-step: re-protect and resume */
    if (code == EXCEPTION_SINGLE_STEP && g_watch_pending) {
        g_watch_pending = 0;
        if (g_watch_last_va) {   /* log the VALUE the watched write stored */
            uint32_t newv = *(volatile uint32_t *)((uintptr_t)g_xbox_mem_offset + g_watch_last_va);
            int slot = watch_slot_state(g_watch_last_va);
            if (slot >= 0) {
                /* slot-state transition: log only real changes, dedupe by
                 * (slot, old, new, rip) so steady-state cycles print once */
                static uint64_t s_seen[64]; static int s_sn = 0; static int s_prints = 0;
                if (newv != g_watch_last_old && s_prints < 200) {
                    uint64_t key = ((uint64_t)(uint32_t)g_watch_last_rip << 16) |
                                   ((uint64_t)slot << 8) |
                                   ((g_watch_last_old & 0xF) << 4) | (newv & 0xF);
                    int dup = 0;
                    for (int i = 0; i < s_sn; i++) if (s_seen[i] == key) { dup = 1; break; }
                    if (!dup) {
                        if (s_sn < 64) s_seen[s_sn++] = key;
                        s_prints++;
                        fprintf(stderr, "[SLOTST] slot=%d %u->%u rip=0x%llX%c",
                                slot, g_watch_last_old, newv,
                                (unsigned long long)g_watch_last_rip, 10);
                        fflush(stderr);
                    }
                }
            } else {
                static int s_vals = 0;
                /* DOA3_WATCHNAN=1: only report stores of NaN/Inf floats, so a
                 * hot matrix slot does not burn the line budget on sane
                 * values before the corrupting store arrives. */
                static int s_nan_only = -1;
                if (s_nan_only < 0) { const char *e = getenv("DOA3_WATCHNAN"); s_nan_only = (e && *e == '1'); }
                if (s_vals < 400 && (!s_nan_only || (newv & 0x7F800000u) == 0x7F800000u)) {
                    s_vals++;
                    fprintf(stderr, "[WATCHV] xbva=0x%08X = 0x%X (rip 0x%llX)%c",
                            g_watch_last_va, newv,
                            (unsigned long long)g_watch_last_rip, 10);
                    fflush(stderr);
                }
            }
            g_watch_last_va = 0;
        }
        if (g_watch_page)
            watch_protect_all(PAGE_READONLY);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    /* write-watch fault: log new writer RIPs, unprotect, single-step */
    if (code == EXCEPTION_ACCESS_VIOLATION && g_watch_page) {
        uintptr_t f = info->ExceptionRecord->ExceptionInformation[1];
        uint32_t voff = watch_view_offset(f);
        if (voff >= g_watch_pageoff && voff < g_watch_pageoff + 0x1000 &&
            info->ExceptionRecord->ExceptionInformation[0]) {
            uint32_t rip32 = (uint32_t)info->ContextRecord->Rip;
            /* guest VA of the write as seen through the PRIMARY view; the
             * alias it actually came through is logged separately. */
            uint32_t xbva = voff;
            {   static int s_alias = 0;
                if (f < g_watch_page || f >= g_watch_page + 0x1000) {
                    if (s_alias < 20) { s_alias++;
                        fprintf(stderr, "[WATCH-ALIAS] write via alias host=%llX (guest-ish 0x%llX) -> xbva=0x%08X rip=0x%llX\n",
                                (unsigned long long)f,
                                (unsigned long long)(f - (uintptr_t)g_xbox_mem_offset),
                                xbva, (unsigned long long)info->ContextRecord->Rip);
                        fflush(stderr); } } }
            /* focus filter (item 87): the decode-slot index [h+0x35F8]
             * (0xC12DB8) — its writer is the decode-path slot manager we
             * cannot find statically (computed addressing). */
            int slot_idx = watch_slot_state(xbva);
            int interesting = (xbva == 0xC12DB8u) || slot_idx >= 0;
            if (g_watch_exact_va &&
                xbva >= g_watch_exact_va && xbva < g_watch_exact_va + g_watch_exact_len)
                interesting = 1;
            if (!interesting && g_watch_arr_stride) {
                uint32_t _rel = xbva - g_watch_arr_base;
                if (xbva >= g_watch_arr_base &&
                    _rel < g_watch_arr_n * g_watch_arr_stride &&
                    (_rel % g_watch_arr_stride) < 4u) {
                    interesting = 1;
                    fprintf(stderr, "[VTHIT] elem=%u xbva=0x%08X old=0x%08X rip=0x%llX eax=%08X ebx=%08X ecx=%08X edx=%08X esi=%08X edi=%08X ebp=%08X\n",
                            _rel / g_watch_arr_stride, xbva,
                            *(volatile uint32_t *)f,
                            (unsigned long long)info->ContextRecord->Rip,
                            g_eax, g_ebx, g_ecx, g_edx, g_esi, g_edi);
                    fflush(stderr);
                }
            }
            if (interesting) {
                g_watch_last_va = xbva;
                g_watch_last_rip = (uintptr_t)info->ContextRecord->Rip;
                g_watch_last_old = *(volatile uint32_t *)f;   /* pre-write value (page readable) */
            }
            int seen = !interesting;   /* value log below covers repeats */
            for (int i = 0; i < g_watch_rips_n; i++)
                if (g_watch_rips_seen[i] == rip32) { seen = 1; break; }
            if (interesting && !seen && g_watch_rips_n < 24) {
                g_watch_rips_seen[g_watch_rips_n++] = rip32;
                fprintf(stderr, "[WATCHW] rip=0x%llX xbva=0x%08X\n",
                        (unsigned long long)info->ContextRecord->Rip,
                        (uint32_t)(f - (uintptr_t)g_xbox_mem_offset));
                fflush(stderr);
            }
            watch_protect_all(PAGE_READWRITE);
            info->ContextRecord->EFlags |= 0x100;   /* TF: single-step */
            g_watch_pending = 1;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }
    if (code == STATUS_GUARD_PAGE_VIOLATION) {
        /* Recompiled code occasionally touches a page inside a fiber stack's
         * guard region (wild pointer landing in a stack reserve). Windows
         * clears PAGE_GUARD before raising this, so resuming simply retries
         * the access against the now-normal committed page. */
        static int s_guard_logged = 0;
        if (s_guard_logged < 8) {
            s_guard_logged++;
            fprintf(stderr, "[GUARD] rip=0x%llX %s fault=0x%llX g_esp=0x%08X (unguarded, resuming)\n",
                    (unsigned long long)info->ContextRecord->Rip,
                    info->ExceptionRecord->ExceptionInformation[0] ? "write" : "read",
                    (unsigned long long)info->ExceptionRecord->ExceptionInformation[1], g_esp);
            fflush(stderr);
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    /* OutputDebugString with no debugger attached surfaces here. The retail
     * build still emits its diagnostic stream this way, so decode it rather
     * than reporting it as an unexplained non-AV crash: ExceptionInformation
     * is { length, string pointer }. */
    if (code == 0x40010006UL || code == 0x4001000AUL) {
        const void *sp = (const void *)info->ExceptionRecord->ExceptionInformation[1];
        if (sp) {
            if (code == 0x40010006UL)
                fprintf(stderr, "[DBGPRINT] %.*s\n",
                        (int)info->ExceptionRecord->ExceptionInformation[0],
                        (const char *)sp);
            else
                fprintf(stderr, "[DBGPRINT] %.*ls\n",
                        (int)(info->ExceptionRecord->ExceptionInformation[0] / 2),
                        (const wchar_t *)sp);
            fflush(stderr);
        }
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (code != EXCEPTION_ACCESS_VIOLATION) {
        /* Surface non-AV crashes (stack overflow, illegal instr, etc.) that would
         * otherwise terminate the process silently. */
        static int s_nonav_logged = 0;
        if (s_nonav_logged < 8) {
            s_nonav_logged++;
            fprintf(stderr, "[CRASH-NONAV] code=0x%08lX rip=0x%llX g_esp=0x%08X fault=0x%llX\n",
                    (unsigned long)code,
                    (unsigned long long)info->ContextRecord->Rip, g_esp,
                    (unsigned long long)info->ExceptionRecord->ExceptionInformation[1]);
            fflush(stderr);
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }

    uintptr_t fault = info->ExceptionRecord->ExceptionInformation[1];
    int is_write = (int)info->ExceptionRecord->ExceptionInformation[0];
    uintptr_t base = (uintptr_t)g_xbox_mem_offset;

    /* Only handle faults that land in / near the Xbox mapped region's wrap
     * space (a recompiled MEM access with a bad VA). Genuine host crashes
     * fall through to the OS. We treat the whole 4 GB above base as ours
     * because wild 32-bit VAs (e.g. 0xFFFFFFFC) land there after +offset. */
    /* +64KB margin past the 4GB alias: wild pointers like 0xFFFFFFFC plus a small
     * positive offset land just beyond base+4GB and still need the skip treatment. */
    int in_region = (fault >= base && fault < base + 0x100010000ull);

    /* Log the first 60 faults, then every 1,000,000th (to reveal a spin's
     * repeated faulting access without flooding). */
    g_fault_logged++;
    {   /* First wild accesses: outside guest RAM and outside the GPU MMIO
         * aperture, i.e. a genuinely corrupted pointer rather than the normal
         * emulated-register traffic. Report the guest block of sub_00050380
         * that was executing, so the bad value can be traced to a statement. */
        uint32_t _xv = (uint32_t)(fault - base);
        extern uint32_t g_blk50380;
        static int s_wild = 0;
        if (_xv >= 0x08000000u && (_xv & 0xFF000000u) != 0xFD000000u &&
            g_blk50380 != 0 && s_wild < 12) {
            s_wild++;
            fprintf(stderr, "[WILD] #%d xbva=0x%08X rip=0x%llX blk50380=0x000%05X "
                            "eax=%08X ecx=%08X edx=%08X ebx=%08X esi=%08X edi=%08X\n",
                    s_wild, _xv, (unsigned long long)info->ContextRecord->Rip,
                    g_blk50380, g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi);
            { extern void doa3_vbw_dump(void); if (s_wild <= 6) doa3_vbw_dump(); }
            fflush(stderr);
        }
    }
    if (g_fault_logged <= 60 || (g_fault_logged % 1000000ull) == 0) {
        fprintf(stderr,
            "[FAULT] #%llu %s 0x%llX (xbva 0x%08X) rip=0x%llX  eax=%08X ecx=%08X edx=%08X "
            "ebx=%08X esi=%08X edi=%08X esp=%08X\n",
            (unsigned long long)g_fault_logged,
            is_write ? "write" : "read",
            (unsigned long long)fault,
            (uint32_t)(fault - base),
            (unsigned long long)info->ContextRecord->Rip,
            g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp);
        fflush(stderr);
    }

    /* NV2A GPU / hardware I/O address space (Xbox VA >= 0xF0000000):
     *   0xFD000000-0xFDFFFFFF  GPU MMIO registers -> NV2A register state machine
     *   0xF0000000-0xFCFFFFFF  framebuffer / push-buffer DMA / AGP -> zero pages
     * Routing MMIO register reads to the NV2A emulation (instead of skip-as-0)
     * lets CMiniport_InitHardware's GPU-ready polling complete. (burnout pattern) */
    if (in_region) {
        uint32_t fault_xbox_va = (uint32_t)(fault - base);
        /* Cached RAM mirror (Xbox VA 0x80000000-0x83FFFFFF = cached alias of the
         * 64 MB physical RAM). The D3D fence notifier lives here, so map a 64 MB
         * view of the same file mapping on demand to alias the real pages. NOTE:
         * keep this range TIGHT — mapping arbitrary out-of-range faults as RAM would
         * turn a wild/corrupted-pointer write (which must be skipped) into a
         * silently-continued infinite fault loop. */
        /* DOA3: guest RAM runs to 128 MB (low heap to XBOX_LOW_END, high heap
         * above), so the cached alias covers 0x80000000-0x87FFFFFF and each
         * 64 MB window maps the matching offset of the section. */
        if (fault_xbox_va >= 0x80000000u && fault_xbox_va < 0x88000000u) {
            extern HANDLE xbox_GetMappingHandle(void);
            HANDLE h = xbox_GetMappingHandle();
            if (h) {
                uintptr_t mbase = fault & ~(uintptr_t)(0x4000000u - 1);  /* 64 MB align (host) */
                DWORD sect_off = (DWORD)((fault_xbox_va - 0x80000000u) & ~0x3FFFFFFu);
                LPVOID p = MapViewOfFileEx(h, FILE_MAP_ALL_ACCESS, 0, sect_off,
                                           0x4000000u, (LPVOID)mbase);
                if (p) {
                    if (g_fault_logged < 60) {
                        fprintf(stderr, "  [MIRROR] RAM mirror view for xbva 0x%08X\n",
                                fault_xbox_va & ~0x3FFFFFFu);
                        fflush(stderr);
                    }
                    return EXCEPTION_CONTINUE_EXECUTION;
                }
            }
        }
        if (fault_xbox_va >= 0xF0000000u) {
            if (fault_xbox_va >= 0xFD000000u && fault_xbox_va < 0xFE000000u) {
                extern bool nv2a_hook_handle_mmio(PCONTEXT ctx, uintptr_t fault_addr,
                                                  uint32_t fault_xbox_va, int is_write);
                if (nv2a_hook_handle_mmio(info->ContextRecord, fault, fault_xbox_va, is_write))
                    return EXCEPTION_CONTINUE_EXECUTION;
                /* decode failed -> fall through to zero page */
            }
            /* MCPX APU MMIO registers: 0xFE800000-0xFE87FFFF (mirrors burnout3). The
             * recompiled DirectSound driver pokes these to init/run the audio DSP. */
            if (fault_xbox_va >= 0xFE800000u && fault_xbox_va < 0xFE880000u) {
                extern bool apu_hook_handle_mmio(PCONTEXT ctx, uintptr_t fault_addr,
                                                 uint32_t fault_xbox_va, int is_write);
                if (apu_hook_handle_mmio(info->ContextRecord, fault, fault_xbox_va, is_write))
                    return EXCEPTION_CONTINUE_EXECUTION;
            }
            /* framebuffer / DMA / other GPU memory: back with a zero page */
            uintptr_t alloc_base = fault & ~(uintptr_t)0xFFFF;
            LPVOID p = VirtualAlloc((LPVOID)alloc_base, 0x10000,
                                    MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!p) p = VirtualAlloc((LPVOID)alloc_base, 0x10000, MEM_COMMIT, PAGE_READWRITE);
            if (p) {
                memset(p, 0, 0x10000);
                /* MCPX AC97 codec controller (0xFEC00000): the recompiled
                 * DirectSound driver brings the codec up in CMcpxAPU::Initialize
                 * (sub_001CE17A: release cold reset in the global control
                 * register 0x12C, then poll global status 0x130 bit 8, primary
                 * codec ready, sub_001CE1B8 / sub_001CE1A9) and reports
                 * DSERR_NODRIVER when it never sets. There is no codec to
                 * emulate here -- the APU mix is delivered by the host mixer --
                 * so the page that backs the controller reports both codecs
                 * ready. Only 0x12C, 0x130 and 0x17C are ever touched. */
                if ((fault_xbox_va & 0xFFFF0000u) == 0xFEC00000u)
                    *(volatile uint32_t *)((uintptr_t)p + 0x130) = 0x00000300u;
                if (g_fault_logged < 60) {
                    fprintf(stderr, "  [NV2A] GPU mem page 0x%08X (%s)\n",
                            fault_xbox_va & 0xFFFF0000u, is_write ? "W" : "R");
                    fflush(stderr);
                }
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }

    if (in_region) {
        /* Fault RIP inside a host DLL (CRT memcpy/memset called by recompiled code
         * with a wild Xbox pointer, e.g. the lifted `rep movs` helpers): we cannot
         * decode-and-skip a system routine, so back the page with junk memory and
         * let the copy complete harmlessly (same strategy as the GPU zero-page). */
        uintptr_t rip = (uintptr_t)info->ContextRecord->Rip;
        if (rip < 0x140000000ull || rip >= 0x142000000ull) {
            uintptr_t alloc_base = fault & ~(uintptr_t)0xFFFF;
            LPVOID p = VirtualAlloc((LPVOID)alloc_base, 0x10000,
                                    MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!p) p = VirtualAlloc((LPVOID)alloc_base, 0x10000, MEM_COMMIT, PAGE_READWRITE);
            if (p) {
                if (g_fault_logged < 60) {
                    fprintf(stderr, "  [JUNK] backed wild page 0x%08X for host-DLL access (%s, rip=0x%llX)\n",
                            (uint32_t)(fault - base) & 0xFFFF0000u, is_write ? "W" : "R",
                            (unsigned long long)rip);
                    fflush(stderr);
                }
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
        BOOL handled = is_write
            ? veh_skip_faulting_write(info->ContextRecord)
            : veh_skip_faulting_read(info->ContextRecord);
        if (handled) {
            g_fault_skips++;
            /* Safety valve: if we're skipping endlessly we're stuck in a loop. */
            if (g_fault_skips > 2000000ull) {
                fprintf(stderr, "[FAULT] skip cap reached (%llu) - aborting\n",
                        (unsigned long long)g_fault_skips);
                return EXCEPTION_CONTINUE_SEARCH;
            }
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        if (g_fault_logged < 60) {
            fprintf(stderr, "[FAULT] could not decode faulting instr at rip=0x%llX\n",
                    (unsigned long long)info->ContextRecord->Rip);
            fflush(stderr);
        }
    } else {
        /* Native (non-Xbox-region) access violation — a real host-side crash, e.g.
         * in the pgraph/D3D11 path. Log it so we can locate the culprit. */
        {   /* Name the faulting module: a bare rip in a system DLL tells us
             * nothing, and guessing which of d3d11/dxgi/ntdll it is has
             * already cost a run. */
            HMODULE mod = NULL; char modname[MAX_PATH] = "?";
            if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                   (LPCSTR)info->ContextRecord->Rip, &mod) && mod)
                GetModuleFileNameA(mod, modname, sizeof modname);
            fprintf(stderr, "[NATIVE-CRASH] %s 0x%llX rip=0x%llX in %s (+0x%llX)\n",
                    is_write ? "write" : "read",
                    (unsigned long long)fault,
                    (unsigned long long)info->ContextRecord->Rip,
                    modname,
                    (unsigned long long)(info->ContextRecord->Rip - (uintptr_t)mod));
            fflush(stderr);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

/* ── XBE loading ────────────────────────────────────────────────── */

static BOOL load_xbe(const char *path, void **out_data, size_t *out_size)
{
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "Cannot open XBE: %s\n", path); return FALSE; }
    fseek(f, 0, SEEK_END); long size = ftell(f); fseek(f, 0, SEEK_SET);
    if (size <= 0) { fclose(f); return FALSE; }
    void *data = malloc((size_t)size);
    if (!data) { fclose(f); return FALSE; }
    if (fread(data, 1, (size_t)size, f) != (size_t)size) { free(data); fclose(f); return FALSE; }
    fclose(f);
    *out_data = data; *out_size = (size_t)size;
    return TRUE;
}

static HANDLE g_diag_main_thread;
/* Sampling profiler (task #1): after 45s (movie running), sample the main
 * thread RIP every 2ms for 90s, aggregate into 256-byte buckets, and dump
 * the top buckets — symbolize against bin/doa3.map to find the decode
 * hotspots. Enabled with DOA3_PROFILE=1. */
static DWORD WINAPI diag_sampler(LPVOID p)
{
    (void)p;
    if (!getenv("DOA3_PROFILE")) return 0;
    Sleep(45000);
    enum { NBUK = 8192 };
    static uintptr_t keys[NBUK]; static unsigned cnt[NBUK];
    unsigned total = 0;
    ULONGLONG end = GetTickCount64() + 90000;
    while (GetTickCount64() < end) {
        CONTEXT c; c.ContextFlags = CONTEXT_CONTROL;
        SuspendThread(g_diag_main_thread);
        GetThreadContext(g_diag_main_thread, &c);
        ResumeThread(g_diag_main_thread);
        uintptr_t k = c.Rip >> 8;   /* 256-byte buckets */
        unsigned h = (unsigned)(k * 2654435761u) & (NBUK - 1);
        for (int probe = 0; probe < 64; probe++) {
            unsigned i = (h + probe) & (NBUK - 1);
            if (cnt[i] == 0) { keys[i] = k; cnt[i] = 1; break; }
            if (keys[i] == k) { cnt[i]++; break; }
        }
        total++;
        Sleep(2);
    }
    fprintf(stderr, "[PROF] %u samples; top buckets:%c", total, 10);
    for (int rank = 0; rank < 40; rank++) {
        unsigned best = 0, bi = 0;
        for (unsigned i = 0; i < NBUK; i++)
            if (cnt[i] > best) { best = cnt[i]; bi = i; }
        if (!best) break;
        fprintf(stderr, "[PROF] %6.2f%% 0x%llX00%c",
                100.0 * best / total, (unsigned long long)keys[bi], 10);
        cnt[bi] = 0;
    }
    return 0;
}

static void doa3_atexit(void)
{
    extern void netplay_shutdown(void);
    fprintf(stderr, "[EXIT] process exiting via exit()/CRT (g_esp=0x%08X)\n", g_esp);
    fflush(stderr);
    netplay_shutdown();   /* tell the other player, close the router port */
}

static LONG WINAPI doa3_unhandled(PEXCEPTION_POINTERS info)
{
    fprintf(stderr, "[EXIT] UNHANDLED exception 0x%08lX rip=0x%llX g_esp=0x%08X",
            (unsigned long)info->ExceptionRecord->ExceptionCode,
            (unsigned long long)info->ContextRecord->Rip, g_esp);
    if (info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
        fprintf(stderr, " %s 0x%llX",
                info->ExceptionRecord->ExceptionInformation[0] ? "write" : "read",
                (unsigned long long)info->ExceptionRecord->ExceptionInformation[1]);
    fprintf(stderr, "\n[EXIT] our frames:");
    {
        uintptr_t *sp = (uintptr_t *)info->ContextRecord->Rsp;
        int shown = 0;
        for (int k = 0; k < 2000 && shown < 16; k++) {
            if (IsBadReadPtr(&sp[k], 8)) break;
            uintptr_t v = sp[k];
            if (v >= 0x140000000ull && v < 0x142000000ull) {
                fprintf(stderr, " 0x%llX", (unsigned long long)v);
                shown++;
            }
        }
    }
    fprintf(stderr, "\n");
    fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}

unsigned g_e1660_n, g_e1660_lo = 0xFFFFFFFFu, g_e1660_hi = 0xFFFFFFFFu, g_e1660_mask[8];

int main(int argc, char **argv)
{
    void *xbe_data = NULL; size_t xbe_size = 0;
    (void)argc; (void)argv;
    {   /* The CRT math library picks FMA3 or non-FMA code per CPU; online
         * lockstep needs identical results on both machines. */
        extern int __cdecl _set_FMA3_enable(int flag);
        _set_FMA3_enable(0);
    }
    doa3_watchdog_start();   /* localise non-faulting hangs (see doa3_watchdog) */
    /* Find the project root so relative asset paths work from any build
     * configuration directory. */
    {
        char exedir[MAX_PATH];
        DWORD n = GetModuleFileNameA(NULL, exedir, MAX_PATH);
        if (n > 0 && n < MAX_PATH) {
            char *slash = strrchr(exedir, 0x5c);
            if (slash) {
                char asset[MAX_PATH];
                *slash = 0;
                for (int depth = 0; depth < 4; depth++) {
                    snprintf(asset, sizeof(asset),
                             "%s\\..\\doa3gamefiles\\default.xbe", exedir);
                    if (GetFileAttributesA(asset) != INVALID_FILE_ATTRIBUTES) {
                        SetCurrentDirectoryA(exedir);
                        break;
                    }
                    slash = strrchr(exedir, 0x5c);
                    if (!slash)
                        break;
                    *slash = 0;
                }
            }
        }
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    /* Diagnostics go to a file whenever stderr would otherwise reach a
     * console.
     *
     * This build logs heavily and stderr is unbuffered, so every line is a
     * synchronous console write. Launched from a shell that redirects
     * stderr that costs nothing, but double-clicked -- where the process
     * owns a real console -- it starves the guest badly enough that boot
     * never completes and the window just stays blank. Release builds have
     * no console at all and already needed this.
     *
     * GetConsoleMode only succeeds on a console handle, so a redirected or
     * piped stderr is left exactly as the caller set it. */
    {
        HANDLE herr = GetStdHandle(STD_ERROR_HANDLE);
        DWORD cmode;
        if (herr == NULL || herr == INVALID_HANDLE_VALUE ||
            GetConsoleMode(herr, &cmode))
            freopen("doa3_log.txt", "w", stderr);
    }
    setvbuf(stderr, NULL, _IONBF, 0);

    atexit(doa3_atexit);
    SetUnhandledExceptionFilter(doa3_unhandled);

    {   /* Pad mapping saved from the Esc menu (doa3_input.ini next to the
         * exe). Without this the file was written but never read back, so
         * every launch ran on the built-in defaults. */
        extern int pad_mapping_load(const char *);
        extern const char *pad_mapping_default_path(void);
        extern int pad_mapping_has_key_binding(void);
        int ok = pad_mapping_load(NULL);
        fprintf(stderr, "[PADMAP] %s %s (keyboard bound: %s)\n",
                ok ? "loaded" : "no file, defaults:",
                pad_mapping_default_path(),
                pad_mapping_has_key_binding() ? "yes" : "no");
    }

    printf("=== Dead or Alive 3 - Static Recompilation (boot bring-up) ===\n");
    AddVectoredExceptionHandler(1, crash_veh);
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
                    &g_diag_main_thread, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CreateThread(NULL, 0, diag_sampler, NULL, 0, NULL);

    if (!load_xbe(DOA3_XBE_PATH, &xbe_data, &xbe_size)) {
        {
            char cwd[MAX_PATH] = {0};
            GetCurrentDirectoryA(MAX_PATH, cwd);
            char msg[512];
            snprintf(msg, sizeof(msg),
                     "Could not load %s\n(working directory: %s)\n\n"
                     "Expected the game files in a doa3gamefiles folder next to the doa3 project folder.",
                     DOA3_XBE_PATH, cwd);
            fprintf(stderr, "FATAL: %s\n", msg);
            MessageBoxA(NULL, msg, "DOA3 recomp - startup error", 0x10);
            return 1;
        }
    }
    printf("XBE loaded: %zu bytes\n", xbe_size);

    if (!xbox_MemoryLayoutInit(xbe_data, xbe_size)) {
        fprintf(stderr, "FATAL: xbox_MemoryLayoutInit failed\n"); return 1;
    }
    printf("Xbox memory mapped. Offset: 0x%llX\n",
           (unsigned long long)(uintptr_t)xbox_GetMemoryOffset());

    xbox_kernel_init();
    xbox_path_init(DOA3_GAME_DIR, NULL);
    xbox_kernel_bridge_init();

    /* Pre-initialize CRT bootstrap locks (replicates _mtinitlocks @ 0x00191ACA).
     *
     * DOA3's CRT lock table is at VA 0x3C0FB8 (36 entries, 8 bytes each:
     * [CRITICAL_SECTION* ptr, ULONG flag]). Bootstrap locks have flag==1 and
     * must have a non-NULL ptr before any __lock() (sub_00191B96) runs. We
     * bypass the CRT entry point that would call _mtinitlocks, so without this
     * __lock(n) -> _mtinitlocknum(n) -> __lock(m) recurses forever (the Enter/
     * Leave-CriticalSection storm) and the heap init (which takes a lock) fails,
     * leaving the heap descriptor (0xBF5000) NULL. CS ops are no-ops here, so
     * the ptr just needs to be non-NULL; we use the CRT's own CS buffer at
     * 0xBF4CF0 (stride 0x1C), exactly as the original _mtinitlocks does. */
    {
        uint32_t cs_addr = 0xBF4CF0;   /* DOA3 CS buffer array (BSS) */
        int locks = 0;
        for (int i = 0; i < 36; i++) {
            uint32_t ptr_va  = 0x3C0FB8 + i * 8;
            uint32_t flag_va = ptr_va + 4;
            if (MEM32(flag_va) == 1) {
                MEM32(ptr_va) = cs_addr;
                cs_addr += 0x1C;
                locks++;
            }
        }
        fprintf(stderr, "  CRT locks: %d bootstrap locks pre-initialized\n", locks);
    }

    /* Initialize the NV2A GPU register emulation (allocates VRAM/RAMIN, sets up
     * the register state machine). The VEH routes 0xFD000000 MMIO faults to it
     * so GPU hardware init (CMiniport_InitHardware) can complete. */
    {
        extern void nv2a_hook_init(ptrdiff_t xbox_mem_offset);
        nv2a_hook_init(g_xbox_mem_offset);
        {   /* DOA3 DIAG: record the exact-address write watch target
             * (DOA3_WATCHVA=<hex guest VA>). The page is armed later, from
             * doa3_pump_cri_servers once the movie has handed the screen
             * over -- protecting a hot page for the whole movie perturbs the
             * decode timing enough to change what happens. */
            extern uint32_t g_watch_exact_va;
            const char *wv = getenv("DOA3_WATCHVA");
            if (wv) {
                extern uint32_t g_watch_exact_len;
                const char *wl = getenv("DOA3_WATCHLEN");
                g_watch_exact_va = (uint32_t)strtoul(wv, NULL, 16);
                if (wl) g_watch_exact_len = (uint32_t)strtoul(wl, NULL, 16);
                fprintf(stderr, "  [WATCHVA] will arm on guest 0x%08X\n",
                        g_watch_exact_va);
                if (getenv("DOA3_WATCHVA_NOW")) {   /* DOA3 DIAG: arm before any guest code runs */
                    extern void doa3_watch_arm(uint32_t xb_page);
                    doa3_watch_arm(g_watch_exact_va & ~0xFFFu);
                    fprintf(stderr, "  [WATCHVA] armed NOW (now = 0x%08X)\n", MEM32(g_watch_exact_va));
                }
            }
        }
    }

    /* D3D8 push-buffer RAM backing (mirrors burnout3 main.c step 2d/2e).
     * DOA3's retail-D3D8 driver points its push-buffer write cursor (device[0])
     * at the GPU command aperture (0xFDFExxxx), which isn't CPU-mapped here, so
     * push-buffer writes would fault. Allocate a real RAM buffer; the PB-trio
     * overrides in recomp_manual.c (CDevice_KickOff/MakeSpace/XMETAL_StartPush)
     * keep the device write cursor inside it — exactly as burnout's
     * sub_003518E0/sub_00351770 overrides keep device[0] in its cmd_buf. */
    {
        /* Must stay in the LOW heap: the guest converts push-buffer addresses
         * to physical with a bare 26-bit mask, so anything above 64 MB aliases
         * onto .data. It is pure port overhead though — the console has no
         * equivalent reservation — so keep it small. 1 MB is ample: the
         * MakeSpace override resets the write cursor to the base on every
         * kick, so this only ever holds one kick's worth of commands.
         *
         * At 4 MB the game ran out of low heap once D3DDevice_CreateDevice
         * started allocating its 2.8 MB of back buffers again. The game's own
         * allocations come to roughly 45 MB (14 MB pool + 4x4.8 MB screen
         * workspaces + the back buffers + change) against the 45 MB left after
         * this reservation, and the asset loader was the one that lost: it
         * failed a 1.25 MB allocation and then span forever on
         * "E9040828:'flid' is range outside". */
        extern uint32_t xbox_HeapReserveTop(uint32_t size, uint32_t alignment);
        extern uint32_t g_doa3_pb_base, g_doa3_pb_end;
        uint32_t sz = 1u * 1024 * 1024;
        g_doa3_pb_base = xbox_HeapReserveTop(sz, 4096);
        g_doa3_pb_end  = g_doa3_pb_base + sz;
        fprintf(stderr, "  D3D8 push buffer: %u KB at Xbox VA 0x%08X-0x%08X\n",
                sz / 1024, g_doa3_pb_base, g_doa3_pb_end);
        fflush(stderr);
    }

    /* Graphics: window + D3D8->D3D11 device (the NV2A translator renders here). */
    doa3_init_graphics();

    /* Audio: MCPX APU emulation + host DirectSound (mirrors burnout3 main.c steps
     * 5 + 10). DOA3's recompiled DirectSound init (DirectSoundCreate ->
     * DownloadEffectsImage of dsstdfx.bin) talks to the APU via MMIO
     * (0xFE800000-0xFE87FFFF, routed in the VEH) and to the host DirectSound
     * backend; without these the game's audio init fails and its boot state
     * machine never advances to load assets. */
    {
        typedef struct MCPXAPUState MCPXAPUState;
        extern MCPXAPUState *mcpx_apu_init_standalone(uint8_t *ram_ptr);
        extern MCPXAPUState *g_apu_state;
        /* APU reads voice data from physical RAM, mapped at g_xbox_mem_offset+0. */
        uint8_t *phys_ram = (uint8_t *)(uintptr_t)g_xbox_mem_offset;
        g_apu_state = mcpx_apu_init_standalone(phys_ram);
        fprintf(stderr, "  APU: %s\n", g_apu_state ? "MCPX APU emulation initialized"
                                                    : "APU init FAILED");
        fflush(stderr);
    }
    {
        extern HRESULT xbox_DirectSoundCreate(void *pGuid, IDirectSound8 **ppDS, void *pUnkOuter);
        HRESULT hr = xbox_DirectSoundCreate(NULL, &g_dsound, NULL);
        fprintf(stderr, "  DirectSound: %s (hr=0x%08lX)\n",
                SUCCEEDED(hr) ? "host DirectSound created" : "FAILED", (unsigned long)hr);
        fflush(stderr);
    }

    /* Convert this (main) OS thread into the primary cooperative fiber before the
     * game runs, so DOA3's CRI worker threads can be spawned as fibers and run
     * cooperatively (they pump the file-load queue). See src/kernel/xbox_fiber.*. */
    xbox_fiber_init();

    /* CRT static initializers (_initterm).
     *
     * We enter at the XBE entry point and bypass the CRT startup that would
     * normally walk these tables, so without this every C initializer and C++
     * static constructor is skipped and the objects they build stay zeroed --
     * which is what left the title-screen path with a null screen-definition
     * blob, invalid texture parameters, and vtable slots dispatching to junk.
     *
     * Standard MSVC layout, null markers at both ends (which _initterm skips).
     * The bodies live in recomp_ctors.c: they are only ever reached through
     * these tables, so direct-call function detection never found them. */
    {
        static const struct { uint32_t lo, hi; const char *what; } s_ini[] = {
            { 0x00219640u, 0x0021964Cu, "C initializers"   },  /* __xi_a .. __xi_z */
            { 0x00219650u, 0x002196F4u, "C++ constructors" },  /* __xc_a .. __xc_z */
        };
        /* DOA3_CRTMAX=N limits how many initializers run (bisecting a bad
         * one); DOA3_CRTMAX=0 disables them entirely. */
        int limit = -1;
        { const char *e = getenv("DOA3_CRTMAX"); if (e) limit = atoi(e); }
        int done = 0;
        for (int t = 0; t < 2; t++) {
            unsigned ran = 0, miss = 0;
            for (uint32_t va = s_ini[t].lo; va < s_ini[t].hi; va += 4) {
                uint32_t fn = MEM32(va);
                recomp_func_t f;
                if (!fn)
                    continue;              /* _initterm skips null slots */
                if (limit >= 0 && done >= limit)
                    continue;
                done++;
                {   /* local table first (recomp_ctors.c); the dispatch table
                     * still covers any initializer that was already translated */
                    extern recomp_func_t doa3_crt_lookup(uint32_t va);
                    f = doa3_crt_lookup(fn);
                    if (!f) f = recomp_lookup(fn);
                }
                if (!f) {
                    miss++;
                    fprintf(stderr, "  CRT init: no body for 0x%08X\n", fn);
                    continue;
                }
                /* Translated bodies pop their own return slot on ret, but a
                 * few do not balance exactly; restore the stack around each
                 * call so a drifting initializer cannot hand the entry point a
                 * shifted guest stack. */
                {
                    uint32_t saved_esp = g_esp;
                    PUSH32(g_esp, 0);
                    f();
                    if (g_esp != saved_esp)
                        fprintf(stderr, "  CRT init: 0x%08X left esp %+d\n",
                                fn, (int)(g_esp - saved_esp));
                    g_esp = saved_esp;
                }
                ran++;
            }
            fprintf(stderr, "  CRT init: %s: %u run, %u missing\n",
                    s_ini[t].what, ran, miss);
        }
        {   extern unsigned g_e1660_n, g_e1660_lo, g_e1660_hi, g_e1660_mask[8];
            int _m;
            fprintf(stderr, "  [CTOR370C48] constructed=%u of 175 (idx %u..%u) mask:",
                    g_e1660_n, g_e1660_lo, g_e1660_hi);
            for (_m = 0; _m < 6; _m++)
                fprintf(stderr, " %08X", g_e1660_mask[_m]);
            fprintf(stderr, "\n");
        }
        fflush(stderr);
    }

    printf("Init complete. Entry=0x%08X  ESP=0x%08X\n", DOA3_ENTRY_POINT, g_esp);
    printf("\n--- Calling xbe_entry_point() ---\n");
    fflush(stdout);

    if (getenv("DOA3_WATCHARR")) {
        extern uint32_t g_watch_arr_base, g_watch_arr_stride, g_watch_arr_n;
        /* wxCi cache registry, z:\ slot (0xC057C0 + 0x30*2 = 0xC05820):
         * +0x24 file count, +0x28 list head. Only one file ever registers,
         * so watch both and let the VEH name the registrar. */
        g_watch_arr_base = 0x00C05844u; g_watch_arr_stride = 4; g_watch_arr_n = 2;
        doa3_watch_arm(0x00C05000u);
        fprintf(stderr, "[ARR] watch armed on 0xC05844 (count) / 0xC05848 (head)\n");
        fflush(stderr);
    }

    xbe_entry_point();

    fprintf(stderr, "\nxbe_entry_point returned (g_eax=0x%08X), fault-skips=%llu\n",
            g_eax, (unsigned long long)g_fault_skips);

    xbox_kernel_shutdown();
    xbox_MemoryLayoutShutdown();
    free(xbe_data);
    return 0;
}
