/**
 * Dead or Alive 3 - Manually implemented / overridden recompiled functions
 *
 * Functions the automatic recompiler can't handle correctly (mid-function
 * entry points, hardware register polls, SEH continuations, etc.) are
 * hand-written here using the same register model and calling conventions
 * as the generated code, then registered in g_manual_funcs[] so RECOMP_ICALL
 * and the dispatch chain pick them up ahead of the generated versions.
 *
 * This starts empty. Add overrides during boot bring-up; record each one in
 * doa3/NOTES.md under "Runtime Architecture" so it survives a regen.
 */

#define RECOMP_GENERATED_CODE
#include "gen/recomp_funcs.h"
#include <stddef.h>
#include <stdio.h>
#include <intrin.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "online/netplay_session.h"

   /* CaptureStackBackTrace (SBH alloc-loop diagnostics) */

extern uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment);
extern void xbox_fiber_yield(void);
extern int  xbox_fiber_active(void);
extern void xbox_fiber_wake(uint32_t event_va);

/* sub_0016D330 — CRI ADXF partition-create wrapper.
 * DOA3 calls sub_00169660(name, 0xFFFFFFFF, buffer=0, size=0) to create CRI
 * partitions. With flags=0xFFFFFFFF the gen takes the "caller supplies the work
 * buffer" path and reads buffer=arg3(esp+0xC) and size=arg4(esp+0x10) — both 0 — so
 * its size check (nameLen <= size) fails and the partition (and thus ALL asset
 * loading) fails. Supply a real work buffer when the game passes 0/0 so the gen
 * creates a valid partition; the CRI then reads the AFS via kernel file I/O.
 * The gen body (renamed sub_0016D330_gen) does the Xbox stack cleanup. */
void sub_0016D330(void)
{
    extern void sub_0016D330_gen(void);
    if (MEM32(esp + 0xC) == 0) {                  /* arg3 = work buffer ptr */
        uint32_t buf = xbox_HeapAlloc(0x40000, 16);   /* 256 KB partition metadata */
        MEM32(esp + 0xC)  = buf;                   /* arg3 = buffer */
        MEM32(esp + 0x10) = 0x40000;               /* arg4 = size */
        fprintf(stderr, "[CRI] partition buffer supplied: 0x%08X (256KB)\n", buf);
    }
    sub_0016D330_gen();
}

/* sub_00170330 — CRI vsprintf logger; the main thread's CRI poll (sub_00169150)
 * reaches it every failed iteration. Wrap it to yield to the CRI worker fibers so
 * the busy-wait poll cooperatively schedules them. Gen body renamed _gen. */
void sub_00170330(void)
{
    extern void sub_00170330_gen(void);
    /* VBLANK emulation (cxbx-verified): on real hardware the GPU vblank interrupt's
     * D3D DPC (0x1BB0F0) does KeSetEvent(0x001C2CF0) every frame, and the CRI ADX
     * server threads (sub_0016A570/5E0) wait on exactly that event to pump async
     * file/stream I/O. We have no GPU interrupt, so pulse the vblank event at this
     * yield point (the main thread's CRI poll) before switching to the workers. */
    MEM32(0x001C2CF0 + 4) = 1;              /* KEVENT.SignalState = 1 */
    xbox_fiber_wake(0x001C2CF0);
    {   /* DIAG: are the ADXM server callbacks registered? (one-shot + on change) */
        static uint32_t s_cb0 = 0xDEAD, s_cb1 = 0xDEAD;
        uint32_t cb0 = MEM32(0xB254F0), cb1 = MEM32(0xB254F8);
        if (cb0 != s_cb0 || cb1 != s_cb1) {
            s_cb0 = cb0; s_cb1 = cb1;
        }
    }
    if (xbox_fiber_active())
        xbox_fiber_yield();
    sub_00170330_gen();
    {   /* DIAG: surface the CRI's formatted log text (buffer 0xC03220), deduped. */
        extern ptrdiff_t g_xbox_mem_offset;
        const char *msg = (const char *)((uintptr_t)0xC03220 + g_xbox_mem_offset);
        static char s_last[256];
        if (msg[0] && strncmp(msg, s_last, sizeof(s_last) - 1) != 0) {
            strncpy(s_last, msg, sizeof(s_last) - 1); s_last[sizeof(s_last) - 1] = 0;
            static int dumped = 0;
            if (!dumped && (strstr(msg, "device") || strstr(msg, "cvFs"))) {
                dumped = 1;
                char dd[16]; int i; for (i = 0; i < 15; i++) { dd[i] = (char)MEM8(0xB24FE0 + i); if (!dd[i]) break; } dd[15] = 0;
                for (int s = 0; s < 4; s++) {
                    uint32_t ent = 0xB25260 + s * 0x10;
                    char nm[16]; for (i = 0; i < 15; i++) { nm[i] = (char)MEM8(ent + 4 + i); if (!nm[i]) break; } nm[15] = 0;
                }
            }
        }
    }
}

/* ── XAPI fiber API → host fibers ─────────────────────────────────────────
 * DOA3 runs its game tasks (boot screens, intro, menus — sub_0009E422-registered
 * handlers) as XAPI fibers switched from the per-frame dispatcher sub_0009E69A.
 * The recompiled SwitchToFiber (sub_00164FEF) swaps the Xbox stack pointer but
 * its `ret` maps to a C return — control can NEVER transfer to the fiber, so no
 * game task ever ran. Back the three XAPI entry points with the host cooperative
 * fiber layer (xbox_fiber.c direct-switch coroutines). Handles are encoded as
 * 0xF1BE0000|index; any foreign handle (e.g. the ConvertThreadToFiber main-fiber
 * pointer stored at 0x5E5C48) means "switch back to the dispatcher fiber". */
extern int  xbox_fiber_create_dormant(uint32_t routine_va, uint32_t param,
                                      uint32_t stack_size);
extern void xbox_fiber_destroy(int idx);
extern void xbox_fiber_switch_direct(int idx);
/* SwitchToFiber(handle) — stdcall ret 4. Completes the call frame FIRST, then
 * transfers; when something switches back here, we return normally. */
#define XFIBER_TAG  0xF1BE0000u

/* CreateFiber(stack_size, start_routine, param) â€” stdcall ret 12. */
void sub_00164F50(void)
{
    extern uint32_t g_eax, g_esp;
    uint32_t stack_sz = MEM32(g_esp + 4);   /* arg0: the size the game asked for */
    uint32_t routine = MEM32(g_esp + 8);
    uint32_t param   = MEM32(g_esp + 0xC);
    int idx = xbox_fiber_create_dormant(routine, param, stack_sz);
    g_eax = (idx > 0) ? (XFIBER_TAG | (uint32_t)idx) : 0;
    g_esp += 16;
}

/* DeleteFiber(handle) â€” stdcall ret 4. */
void sub_00164FDC(void)
{
    extern uint32_t g_esp;
    uint32_t h = MEM32(g_esp + 4);
    if ((h & 0xFFFF0000u) == XFIBER_TAG)
        xbox_fiber_destroy((int)(h & 0xFFFF));
    g_esp += 8;
}

void sub_00164FEF(void)
{
    extern uint32_t g_esp;
    uint32_t h = MEM32(g_esp + 4);
    g_esp += 8;                                  /* finish the call (stdcall ret 4) */
    if ((h & 0xFFFF0000u) == XFIBER_TAG) {
        xbox_fiber_switch_direct((int)(h & 0xFFFF));
    } else {
        /* Foreign handle = the "main fiber" (dispatcher). We are a task fiber
         * yielding back: our creator/switcher resumes via the trampoline path. */
        extern void xbox_fiber_yield_back(void);
        xbox_fiber_yield_back();
    }
}

/* Boot-progress markers: the four init calls after sub_0009F730 in sub_000A0680.
 * One of them loops forever (frame-pumping wait); the markers show which. */
#define BOOT_MARK(name) \
    void name(void) { \
        extern void name##_gen(void); \
        static int n = 0; \
        int log = (n < 4); n++; \
        if (log) {   } \
        name##_gen(); \
        if (log) {   } \
    }
BOOT_MARK(sub_0009EA60)
BOOT_MARK(sub_0009DC00)
BOOT_MARK(sub_0009E59C)
BOOT_MARK(sub_0009DD20)
#undef BOOT_MARK

/* sub_001C7457 — DSOUND stream play/start (IDirectSoundStream-style; pumps the ADX
 * PCM-supply callback synchronously into the sound buffer and waits on the APU
 * voice — which our APU emulation doesn't advance, hanging the boot inside
 * sub_0009F730's final ADX stream start). Stub: report success without playing
 * (silent audio). RENDERING FIRST — restore when the APU voice path works.
 * stdcall ret 12 (gen does esp += 16 including the dummy). */
void sub_001C7457(void)
{
    extern void sub_001C7457_gen(void);
    extern uint32_t g_eax, g_esp;
    static int n = 0;
    uint32_t self = MEM32(g_esp + 4), a2 = MEM32(g_esp + 8), a3 = MEM32(g_esp + 12);
    /* Un-stubbed 2026-09-12: the stub dated from before the DSOUND
     * per-frame service esp leaks (sub_001C6D91 / sub_001C883B) were
     * enforced; the APU voice processor does advance CBO, so let the real
     * stream Play run (-> sub_001C6D46). */
    /* STUB again (2026-09-12): the game hands this call a NULL stream because the
     * host DirectSound shim creates no stream objects yet; running the XBE
     * body with that crashed 6 s after the movie. */
    g_eax = 0; g_esp += 16;
    if (n < 8) {  n++; }
}

/* sub_00168DE0 — ADXF op GetStat (used by the game's own load-wait spins, e.g.
 * sub_0007FFDD). Same vblank pulse + yield as the sub_0016916F wrapper so the CRI
 * worker fibers run while the main thread polls (see that wrapper for the cxbx
 * evidence: the GPU vblank DPC KeSetEvents 0x001C2CF0 every frame). */
void sub_00168DE0(void)
{
    extern void sub_00168DE0_gen(void);
    MEM32(0x001C2CF0 + 4) = 1;
    xbox_fiber_wake(0x001C2CF0);
    {   /* load-wait spin: keep the window serviced (no presents here) */
        extern void doa3_pump_messages(void);
        static unsigned s_mp3 = 0;
        if ((++s_mp3 & 63) == 0) doa3_pump_messages();
    }
    if (xbox_fiber_active())
        xbox_fiber_yield();
    sub_00168DE0_gen();
}

/* sub_00163134 — XAPI ReadFileEx(hFile, buf, len, overlapped, completion).
 * DIAG: log args, the NtReadFile thunk value, and the result. */
void sub_00163134(void)
{
    extern void sub_00163134_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t hf = MEM32(g_esp + 4), buf = MEM32(g_esp + 8), len = MEM32(g_esp + 0xC);
    uint32_t ov = MEM32(g_esp + 0x10);
    uint32_t thunk = MEM32(0x1ED0F4);
    sub_00163134_gen();
    static int n = 0;
    if (n < 10) {   n++; }
}

/* sub_0016BB00 — cvFs stream stepper (issues device seek/read for an active stream).
 * DIAG: dump the stream + file-object fields around the call. */
void sub_0016BB00(void)
{
    extern void sub_0016BB00_gen(void);
    extern uint32_t g_esp;
    uint32_t stm = MEM32(g_esp + 4);
    static int n = 0;
    int log = (n < 10);
    if (log && stm) {
        n++;
        uint32_t fo = MEM32(stm + 4);
    }
    sub_0016BB00_gen();
}

/* sub_001719E0 — WX per-handle request processor (runs on the worker each vblank).
 * DIAG: log the handle's gates: state byte +1 (2 = read pending), +0x148 (request
 * flag), 0xB2561C (reentry lock), 0xB25618 (async mode). */
void sub_001719E0(void)
{
    extern void sub_001719E0_gen(void);
    extern uint32_t g_esp;
    uint32_t h = MEM32(g_esp + 4);
    static uint32_t s_sig = 0xFFFFFFFF; static int n = 0;
    if (h && MEM8(h)) {
        uint32_t sig = (MEM8(h + 1) << 24) | (MEM32(h + 0x148) << 16) | (MEM32(0xB25618) & 0xFFFF);
        extern volatile int g_doa3_post_movie;
        if (sig != s_sig && (n < 24 || g_doa3_post_movie)) {
            s_sig = sig; n++;
        }
    }
    sub_001719E0_gen();
}

/* sub_00169BC0 — ADX master server (runs on the CRI worker fibers each vblank).
 * DIAG: count runs; dump the ADXSTM stream pool (0xC0C540, stride 0xA4) and the
 * ADXF op-pool entry states periodically. */
void sub_00169BC0(void)
{
    extern void sub_00169BC0_gen(void);
    static long n = 0;
    n++;
    sub_00169BC0_gen();
    if (n == 1 || n == 50 || n == 2000) {
        
        for (int i = 0; i < 40; i++) {
            uint32_t ent = 0xC05AC0 + i * 0x40;   /* cvFs stream pool (sub_0016BD20) */
            if (!MEM8(ent) && !MEM32(ent + 0x14)) continue;   /* skip empty slots */
            fprintf(stderr, " [%d]a=%u,st=%u,sz=0x%X,h=0x%X,pos=0x%X", i,
                    MEM8(ent), MEM8(ent + 1), MEM32(ent + 0x14), MEM32(ent + 8), MEM32(ent + 0xC));
        }
        uint32_t op = MEM32(0xC07620);
        {   /* the stream's FILE object + its vtbl (stream+4 -> file, file+0 -> vtbl) */
            uint32_t stm = op ? MEM32(op + 4) : 0;
            uint32_t fo = stm ? MEM32(stm + 4) : 0;
            uint32_t vt = fo ? MEM32(fo) : 0;
        }
    }
}

/* sub_001715A0 — wxCi real size query (stats the file by NAME via sub_001633B7).
 * DIAG: log the name + result. */
void sub_001715A0(void)
{
    extern void sub_001715A0_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t name = MEM32(g_esp + 4);
    char nm[64]; int i; for (i = 0; i < 63; i++) { nm[i] = name ? (char)MEM8(name + i) : 0; if (!nm[i]) break; } nm[63] = 0;
    sub_001715A0_gen();
    static int n = 0;
    if (n < 12) {   n++; }
}

/* sub_001633B7 — XAPI GetFileAttributesExA-like stat. DIAG: log name + result. */
void sub_001633B7(void)
{
    extern void sub_001633B7_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t name = MEM32(g_esp + 4);
    char nm[64]; int i; for (i = 0; i < 63; i++) { nm[i] = name ? (char)MEM8(name + i) : 0; if (!nm[i]) break; } nm[63] = 0;
    sub_001633B7_gen();
    static int n = 0;
    if (n < 12) {   n++; }
}

/* sub_0016916F — ADXF op processor (state machine). DIAG: log op-state transitions +
 * the AFS header buffer when the state reaches the header check, to find which state
 * swallows the read. */
void sub_0016916F(void)
{
    extern void sub_0016916F_gen(void);
    extern uint32_t g_eax;
    uint32_t op = MEM32(0xC07620);
    int st_in = op ? (int)(int8_t)MEM8(op + 1) : -99;
    /* VBLANK emulation at the poll: the main thread spins here waiting for the async
     * load; on real hardware the GPU vblank DPC KeSetEvents 0x001C2CF0 ~60x/s and the
     * CRI worker threads pump the ADX servers on each tick. Pulse + yield so the
     * workers actually run while the main thread polls (see sub_00170330 note). */
    MEM32(0x001C2CF0 + 4) = 1;
    xbox_fiber_wake(0x001C2CF0);
    if (xbox_fiber_active())
        xbox_fiber_yield();
    sub_0016916F_gen();
    uint32_t op2 = MEM32(0xC07620);
    int st_out = op2 ? (int)(int8_t)MEM8(op2 + 1) : -99;
    static int last = -100; static int n = 0;
    if (st_out != last && n < 40) {
        last = st_out; n++;
        uint32_t buf = MEM32(0xB22C1C);
    }
}

/* sub_001631C0 — XAPI CreateFileA(name, access, share, sa, disposition, flags).
 * DIAG: log args + returned handle to see where the CRI's file opens die. */
void sub_001631C0(void)
{
    extern void sub_001631C0_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t fname = MEM32(g_esp + 4);
    uint32_t access = MEM32(g_esp + 8);
    uint32_t disp = MEM32(g_esp + 0x14);
    uint32_t flags = MEM32(g_esp + 0x18);
    char nm[64]; int i; for (i = 0; i < 63; i++) { nm[i] = fname ? (char)MEM8(fname + i) : 0; if (!nm[i]) break; } nm[63] = 0;
    sub_001631C0_gen();
    static int n = 0;
    if (n < 20) {   n++; }
}

/* sub_0016D0F0 — cvFs partition-table lookup (searches 0xC057C0.. stride 0x30 for an
 * entry whose "%s%s" (prefix+node) matches the requested filename). DIAG: log the
 * requested name, dump the table entries + their nodes, and the result. */
void sub_0016D0F0(void)
{
    extern void sub_0016D0F0_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t want = MEM32(g_esp + 4);
    char wn[64]; int i; for (i = 0; i < 63; i++) { wn[i] = want ? (char)MEM8(want + i) : 0; if (!wn[i]) break; } wn[63] = 0;
    sub_0016D0F0_gen();
    static int n = 0;
    if (n < 6) {
        n++;
        for (int e = 0; e < 10; e++) {
            uint32_t ent = 0xC057C0 + e * 0x30;
            uint32_t active = MEM32(ent), prefix = MEM32(ent + 0x18);
            uint32_t count = MEM32(ent + 0x24), head = MEM32(ent + 0x28);
            if (!active && !prefix && !head) continue;
            char pf[32]; for (i = 0; i < 31; i++) { pf[i] = prefix ? (char)MEM8(prefix + i) : 0; if (!pf[i]) break; } pf[31] = 0;
            uint32_t nd = head;
            for (int k = 0; k < 4 && nd; k++) {
                uint32_t nm = MEM32(nd + 0xC);
                char nn[40]; for (i = 0; i < 39; i++) { nn[i] = nm ? (char)MEM8(nm + i) : 0; if (!nn[i]) break; } nn[39] = 0;
                nd = MEM32(nd + 8);
            }
        }
    }
}

/* sub_0016C970 — cvFs device resolver. The CRI registers named devices (MFS, WX) with WX
 * as the default; WX handles plain drive-letter paths (d:\, z:\, t:\) via the kernel. The
 * resolver only uses the default device when the path has NO prefix, so a drive-letter
 * path like "d:\loadfile.afs" extracts device "d", fails to match MFS/WX, and returns 0 —
 * cvFsOpen then fails. Fall back to the default device for an unresolved single-char drive
 * prefix (the full path is preserved separately and handed to the WX open). */
void sub_0016C970(void)
{
    extern void sub_0016C970_gen(void);
    extern uint32_t g_eax, g_ebx;
    uint32_t devbuf = g_ebx;
    uint8_t c0 = devbuf ? (uint8_t)MEM8(devbuf) : 0;
    uint8_t c1 = devbuf ? (uint8_t)MEM8(devbuf + 1) : 0;
    char dn[16]; int di; for (di = 0; di < 15; di++) { dn[di] = devbuf ? (char)MEM8(devbuf + di) : 0; if (!dn[di]) break; } dn[15] = 0;
    sub_0016C970_gen();
    { static int dn_n = 0; if (dn_n < 12) {   dn_n++; } }
    if (g_eax == 0 && c0 && !c1) {        /* single-char drive-letter device not found */
        MEM8(devbuf) = 0;                  /* clear prefix → resolver uses the default device */
        g_ebx = devbuf;
        sub_0016C970_gen();
        static int n = 0;
        if (n < 8) {   n++; }
    }
}

/* sub_0016CA10 — cvFsOpen wrapper. DIAG: log the filename it's asked to open, to find
 * where ADXF_LoaddPartitionNw's empty AFS filename comes from. */
void sub_0016CA10(void)
{
    extern void sub_0016CA10_gen(void);
    extern uint32_t g_esp;
    uint32_t fname = MEM32(g_esp + 4);
    char nm[48]; int i; for (i = 0; i < 47; i++) { nm[i] = fname ? (char)MEM8(fname + i) : 0; if (!nm[i]) break; } nm[47] = 0;
    static int n = 0;
    extern volatile int g_doa3_post_movie;
    if (n < 16 || g_doa3_post_movie) {   n++; }
    sub_0016CA10_gen();
}

/* sub_0018FC20 — CRT strncmp(s1, s2, n). The recompiler does not implement the
 * `repne scasb` / `repe cmpsb` REP string instructions it is built from (they were
 * emitted as comments), so the generated body compares garbage and reports unequal
 * strings as equal — which made cvFsAddDev("WX") collide with "MFS". Replace it with
 * a correct strncmp. cdecl, 3 args, caller-cleaned: pop only the dummy return slot. */
void sub_0018FC20(void)
{
    extern uint32_t g_eax, g_esp;
    uint32_t s1 = MEM32(g_esp + 4);
    uint32_t s2 = MEM32(g_esp + 8);
    uint32_t n  = MEM32(g_esp + 0xC);
    int r = 0;
    for (uint32_t i = 0; i < n; i++) {
        unsigned char c1 = (unsigned char)MEM8(s1 + i);
        unsigned char c2 = (unsigned char)MEM8(s2 + i);
        if (c1 != c2) { r = (c1 < c2) ? -1 : 1; break; }
        if (c1 == 0) break;          /* both reached NUL, equal */
    }
    g_eax = (uint32_t)r;
    g_esp += 4;                      /* mimic the original `ret` popping the dummy slot */
}

/* sub_0016C160 — CRI cvFs device-find-by-name. DIAG wrapper: log the name arg, its
 * content, and the result, to see why cvFsAddDev("WX") thinks the device exists. */
void sub_0016C160(void)
{
    extern void sub_0016C160_gen(void);
    extern uint32_t g_eax, g_esp;
    uint32_t name = MEM32(g_esp + 4);
    char nm[20]; int i; for (i = 0; i < 19; i++) { nm[i] = (char)MEM8(name + i); if (!nm[i]) break; } nm[19] = 0;
    sub_0016C160_gen();
    static int n = 0;
    if (n < 10) {   n++; }
}

/* sub_0016A530 — CRI watchdog worker thread (cooperative-fiber override).
 * The original busy-spins incrementing the liveness counter 0xB24D40 until the
 * shutdown flag 0xB24D58 is set, then terminates the thread (sub_00164B6F + int3).
 * A pure busy-spin would starve the cooperative scheduler, so this version yields
 * each iteration. Shutdown never fires during a load, so the loop runs for the
 * lifetime of the CRI; if it ever does, we return and the fiber is marked done. */
void sub_0016A530(void)
{
    extern void xbox_fiber_dump(void);
    unsigned long long it = 0;
    for (;;) {
        if (MEM32(0xB24D58) != 0) break;
        MEM32(0xB24D40) = MEM32(0xB24D40) + 1;
        if (++it == 3000000ULL) {   /* DIAG: once the watchdog is clearly the only runnable fiber, dump the deadlock */
            fprintf(stderr, "[CRI-WD] watchdog-only spin; 0xB24D40=%u 0xB24D44=%u 0xB24D48=%u 0xB24D70=%u\n",
                    MEM32(0xB24D40), MEM32(0xB24D44), MEM32(0xB24D48), MEM32(0xB24D70));
            xbox_fiber_dump();
        }
        xbox_fiber_yield();
    }
    MEM32(0xB24D5C) = 1;
    esp += 4; /* clean ret 0 */
}

/* sub_0009F640 — DirectSound effects-image download (burnout-style override).
 * The boot asset+audio init sub_0009F730 calls, in order: DirectSoundCreate (OK) ->
 * sub_001C6B72 (OK) -> sub_0009F640 (DSP effects download of dsstdfx.bin) -> THEN
 * sub_0009C840 (load loadfile.afs). The real effects download returns -1 here
 * (its DirectSound buffer-slot alloc sub_001599C0 yields 0), which makes sub_0009F730
 * bail out BEFORE loading loadfile.afs, so the game never loads assets / advances.
 * The DSP effects image is non-essential (cxbx logs "DownloadEffectsImage
 * incomplete!" and the real game proceeds anyway). Mirroring burnout3's audio-init
 * stub (recomp_manual.c sub_00135040: "set the ready state directly... so the audio
 * state machine can proceed"), return success and skip the effects download so the
 * asset load proceeds. cdecl, caller cleans the arg -> ret 0. Gen #if 0'd. */
void sub_0009F640(void) { eax = 0; esp += 4; }  /* effects-download skip: the real one
 * loads dsstdfx.bin fine now, but its aftermath bails sub_0009F730 before the AFS
 * loads (tried 2026-07-01) — keep the stub until that gate is understood. */

/* ── CRT memcpy/memmove native overrides (recomp bug #6 casualties) ──────────
 * The XBE has two identical MSVC CRT memcpy implementations (0x18DF40 and
 * 0x18EE90; overlap-safe = memmove semantics). Their trailing-byte dispatch
 * uses intra-function jump tables interleaved with the case bodies, which the
 * recompiler cannot lift (bug #6) — every non-dword-multiple copy lost its
 * tail AND leaked -4 esp per call through the failed RECOMP_ITAIL. Replace
 * natively. cdecl(dst, src, n) -> eax = dst. Gen bodies renamed _oldjt. */
static void doa3_native_memmove(void)
{
    extern ptrdiff_t g_xbox_mem_offset;
    uint32_t dst = MEM32(esp + 4), src = MEM32(esp + 8), n = MEM32(esp + 0xC);
    if (n)
        memmove((void *)((uintptr_t)dst + g_xbox_mem_offset),
                (void *)((uintptr_t)src + g_xbox_mem_offset), n);
    eax = dst;
    esp += 4; /* pop dummy return; caller cleans args (cdecl) */
}
void sub_0018DF40(void) { doa3_native_memmove(); }
void sub_0018EE90(void) { doa3_native_memmove(); }

/* DIAG: esp-load chokepoints (longjmp/SEH unwind sites in the gen — the only
 * places that assign esp from memory). site: 1=recomp_0008.c:3548 (longjmp),
 * 2-4=recomp_0009.c (SEH unwind family). */
void doa3_esp_load_log(int site, uint32_t esp_now, uint32_t esp_new)
{
    static int s_n = 0;
    if (s_n < 20) {
        s_n++;
    }
}

/* DIAG: the game-task yield (sub_0009E562) — backtrace every 600th call to
 * pinpoint WHICH sequencer loop each task spins in. */
void sub_0009E562_gen(void);
void sub_0009E562(void)
{
    static int s_n = 0;
    extern int xbox_fiber_current(void);
    extern uint32_t g_task_yields[16];
    {   int fi = xbox_fiber_current();
        if (fi >= 0 && fi < 16) g_task_yields[fi]++; }
    s_n++;
    if (xbox_fiber_current() == 5 && (g_task_yields[5] % 600) == 0) {
        void *bt[6]; int n = CaptureStackBackTrace(1, 6, bt, NULL);
    }
    sub_0009E562_gen();
}

/* DIAG: per-fiber yield counters, dumped with the fiber dump. */
uint32_t g_task_yields[16];

/* DIAG: per-frame text-draw helper — backtrace every 600 calls to identify
 * WHICH sequencer loop in sub_00021F70 the intro is stuck in. */
void sub_00055760_gen(void);
void sub_00055760(void)
{
    static int s_n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    s_n++;
    if ((s_n % 600) == 0) {
        void *bt[6]; int n = CaptureStackBackTrace(1, 6, bt, NULL);
    }
    sub_00055760_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* ── XPP (Xbox Peripheral Port) input overrides — host-backed pad 0 ─────────
 * DOA3's boot screens hold forever on the controller-present check (cxbx ran
 * with emulated pads). Report pad 0 connected and serve XInputGetState from
 * the host (src/input xbox_InputGetState; falls back to a neutral pad +
 * keyboard: Enter = START, arrows = d-pad, Z/X = A/B) so the boot flow takes
 * the pad-present path to the intro movie. Gen bodies renamed _gen. */
/* Guest-memory peek for the overlay Debug tab. The overlay is C++ and has no
 * MEM32; these keep the address arithmetic on this side. */
uint32_t doa3_dbg_read32(uint32_t va) { return MEM32(va); }
uint32_t doa3_dbg_read8(uint32_t va)  { return MEM8(va); }

static uint32_t s_xpp_prev_mask;   /* XGetDeviceChanges baseline (real XAPI PreviousConnected) */
void sub_001E6958(void)   /* XGetDevices(type) -> connected mask, stdcall ret 4 */
{
    static int s_n = 0;
    if (s_n < 4) { s_n++;   }
    /* Report the controllers the host actually has.
     *
     * This used to hardcode a single connected pad, because the boot flow
     * needed one. That fake pad is what stopped the attract screen working:
     * with a pad present and screen mode 2, sub_000821B0 writes 1 to
     * [0x004B8228], sub_0004FFC0 reads that as input activity and starts the
     * attract watchdog [0x0048E638], which counts 40 frames down and tears
     * the whole screen script down -- 38 opcodes in, two commands before the
     * one at script address 0x0031CFA8 that arms the corner logo.
     *
     * cxbx settles it in both directions: the real game with no pad keeps the
     * watchdog parked at 40 and draws the logo, and forcing [0x005E5CC8] to 1
     * inside cxbx reproduces this port's teardown exactly. */
    {
        extern DWORD xbox_InputHostMask(void);
        uint32_t mask = netplay_filter_pad_mask(xbox_InputHostMask());
        /* Real XAPI semantics (cxbx Xapi.cpp, XGetDevices): reporting the
         * connected set also resets the change baseline, so the next
         * XGetDeviceChanges does NOT re-report these pads as insertions. */
        s_xpp_prev_mask = mask;
        if (s_n <= 4) {   }
        eax = mask;
    }
    esp += 8;
}
void sub_001E697A(void)   /* XGetDeviceChanges(type, &ins, &rem), stdcall ret 12 */
{
    /* Real XAPI semantics (cxbx Xapi.cpp, XGetDeviceChanges): BOTH output
     * words are written on EVERY call -- zero when nothing changed -- and
     * only the pads that appeared/vanished since the previous call (or since
     * XGetDevices) are reported.
     *
     * DOA3 seeds 0x005E5CC8 from XGetDevices at boot (sub_0009EA60 opens
     * those pads itself) and then hands that SAME word to this function as
     * the insertions pointer every frame (sub_0009EAF0). So on real hardware
     * 0x5E5CC8 is "pads inserted THIS frame": 0 from the first frame on.
     *
     * This used to return FALSE without touching the outputs, which left
     * 0x5E5CC8 == 1 for the whole run whenever a controller was attached.
     * The attract driver sub_000821B0 tests exactly that word at 0x000822E5
     * (`test [0x5E5CC8], 1<<port`) as "pad just inserted" and treats it like
     * a button press: [0x4B8228] = 1 every frame, the watchdog [0x48E638]
     * counts 40 -> 0 and the intro scene (scr=5) tears itself down with no
     * input -- the same teardown a START press is supposed to cause. With a
     * controller plugged in there was therefore nothing left for START to
     * skip, and the corner logo never armed. */
    extern DWORD xbox_InputHostMask(void);
    uint32_t p_ins = MEM32(esp + 8), p_rem = MEM32(esp + 12);
    /* A netplay session fixes the topology at ports 0 and 1 on both
     * machines, whatever pads they have (netplay_session.c). */
    uint32_t cur = netplay_filter_pad_mask(xbox_InputHostMask());
    uint32_t ins = cur & ~s_xpp_prev_mask;
    uint32_t rem = s_xpp_prev_mask & ~cur;
    s_xpp_prev_mask = cur;
    if (p_ins) MEM32(p_ins) = ins;
    if (p_rem) MEM32(p_rem) = rem;
    if (ins | rem) {
    }
    eax = (ins | rem) ? 1u : 0u;
    esp += 16;
}
/* Netplay session start: put the four pad structs into the same state on
 * both machines -- the ports in `mask` open with the fake handle XInputOpen
 * hands out, the others closed, no carried-over pad or rumble state, and the
 * per-frame aggregates (which hold last frame's buttons and the auto-repeat
 * counters) cleared. The change baseline is set so XGetDeviceChanges reports
 * nothing for these ports on the next frame. */
void doa3_netplay_force_pads(uint32_t mask)
{
    uint32_t p, i;
    for (p = 0; p < 4; p++) {
        uint32_t pad = 0x5E5CD0u + 0x80u * p;
        for (i = 0x00; i < 0x19; i++) MEM8(pad + i) = 0;       /* caps blob */
        for (i = 0x19; i < 0x45; i++) MEM8(pad + i) = 0;       /* both state slots */
        for (i = 0x71; i < 0x78; i++) MEM8(pad + i) = 0;       /* rumble motors */
        MEM32(pad + 0x7C) = 0;                                  /* rumble timer */
        if (mask & (1u << p)) {
            MEM8(pad) = 1;                                      /* XINPUT_DEVSUBTYPE_GC_GAMEPAD */
            MEM32(pad + 0x78) = 0x0AD00001u + p;                /* XInputOpen's handle */
        } else {
            MEM32(pad + 0x78) = 0;
        }
    }
    for (i = 0x5E5ED8u; i < 0x5E5FA4u; i++) MEM8(i) = 0;
    MEM32(0x5E5ED0) = mask;
    MEM32(0x5E5CC8) = 0;
    MEM32(0x5E5CCC) = 0;
    s_xpp_prev_mask = mask;
}
void sub_001E6EAF_xppgen(void);
void sub_001E6EAF(void)   /* XInputClose(handle), stdcall ret 4 */
{
    /* Reached on a removal (sub_0009EAF0 closes the handle it opened). The
     * handles are fake and there is no USB device behind them. */
    fprintf(stderr, "[XPP] XInputClose(0x%08X)\n", MEM32(esp + 4));
    eax = 0;
    esp += 8;
}
void sub_001E6E3A(void)   /* XInputOpen(type, port, slot, attrs) -> handle, ret 16 */
{
    uint32_t port = MEM32(esp + 8);
    eax = 0x0AD00001u + port;   /* fake but consistent nonzero handle */
    esp += 20;
}
void sub_001E6EBB(void)   /* XInputGetCapabilities(handle, caps->pad), ret 8 */
{
    /* arg2 is the PAD STRUCT base; the real XAPI writes a 0x19-byte caps blob
     * there whose FIRST byte is the device (sub)type — the game's pad polls
     * check MEM8(pad)==1||2 (gamepad). */
    uint32_t caps = MEM32(esp + 8);
    if (caps) {
        for (int i = 0; i < 0x18; i += 4) MEM32(caps + i) = 0;
        MEM8(caps + 0x18) = 0;
        MEM8(caps) = 1;   /* XINPUT_DEVSUBTYPE_GC_GAMEPAD */
    }
    eax = 0;
    esp += 12;
}
void sub_001E70AD(void)   /* XInputPoll(handle, out) — complete instantly, ret 8 */
{
    /* The real XAPI kicks an async USB read and stores ERROR_IO_PENDING
     * (0x3E5) in the pad's state dword; the per-frame GetState looper
     * (sub_0009EDC0) skips any pad whose marker is 0x3E5 until the USB
     * completion clears it. We have no USB machinery — mark the poll
     * complete so GetState (our host-backed override) actually runs. */
    for (int i = 0; i < 4; i++) {
        uint32_t st = 0x5E5CFF + 0x80u * i;
        if (MEM32(st) == 0x3E5) MEM32(st) = 0;
    }
    eax = 0;
    esp += 12;
}
void sub_001E711E(void)   /* XInputGetState(handle, state) -> 0, ret 8 */
{
    { extern unsigned g_in_getstate; g_in_getstate++; }
    uint32_t st = MEM32(esp + 8);
    static uint32_t s_packet = 0x1000;   /* never collide with the 0x3E5
                                          * IO_PENDING marker */
    s_packet++;
    if (st) {
        uint16_t buttons = 0;
        uint8_t a = 0, b = 0;
        uint8_t an[8] = {0};          /* X, Y, Black, White, LT, RT at [2..7] */
        int16_t lx = 0, ly = 0, rx = 0, ry = 0;
        /* Which guest port is this? The state pointer is pad+0x2F of the
         * pad struct (base 0x5E5CD0, stride 0x80). This used to read host
         * pad 0 for EVERY port, so a controller enumerated on any other
         * XInput index fed nothing. */
        uint32_t port = 0;
        {
            uint32_t pi;
            for (pi = 0; pi < 4; pi++)
                if (st == 0x5E5CFFu + 0x80u * pi || st == 0x5E5CE9u + 0x80u * pi) port = pi;
        }
        /* host pad for this port (mapping; keyboard only overlays port 0) */
        {
            extern DWORD xbox_InputGetState(DWORD, void *);
            uint8_t raw[32] = {0};
            if (xbox_InputGetState(port, raw) == 0) {
                buttons = *(uint16_t *)(raw + 4);
                memcpy(an, raw + 6, 8);
                a = an[0]; b = an[1];
                lx = *(int16_t *)(raw + 14); ly = *(int16_t *)(raw + 16);
                rx = *(int16_t *)(raw + 18); ry = *(int16_t *)(raw + 20);
            }
        }
        /* Keyboard input comes ONLY through the Esc-menu mapping now; the old
         * fixed Enter/arrows/Z/X extras were removed once the mapper was
         * verified to reach the game. */
        {   /* DIAGNOSTIC ONLY (DOA3_FAKESTART=1), never on by default.
             *
             * The post-movie screen advances only on a real button press:
             * sub_000821B0 reaches the screen-change path at 0x0008243B via
             * sub_00081E90, which tests bits 4-15 of the pad aggregate at
             * 0x5E5EE0. Headless runs cannot press a key, so this pulses
             * START after the movie purely to establish whether the rest of
             * the chain (screen change -> title screen -> 3D draws) works.
             * It exists to isolate input plumbing from rendering, not to
             * stand in for input. */
            extern volatile int g_doa3_post_movie;
            static int s_fake = -1;
            static unsigned s_n = 0;
            if (s_fake < 0) {
                const char *e = getenv("DOA3_FAKESTART");
                s_fake = (e && *e && *e != '0') ? 1 : 0;
            }
            if (s_fake && g_doa3_post_movie) {
                s_n++;
                /* ONE press, then released for good.
                 *
                 * Pulsing repeatedly restarts the pad rumble on every press,
                 * and the screen latch is only released while the rumble is
                 * idle: sub_00050250 bails whenever sub_00067220 (the word at
                 * 0x0049231C) is non-zero, and bit 3 of it means "rumble
                 * running" (set at 0x00067342, cleared at 0x000672B0). A
                 * repeating press therefore holds the latch shut forever,
                 * which is not what a real press does. */
                if (s_n >= 300 && s_n < 340) {
                    buttons |= 0x0010;   /* START */
                    /* The post-movie screen gate at 0x000CDC20 wants bit 0x20
                     * or 0x200 of the pad word at 0x5E5ED8 + pad*0x2C, which
                     * START does not produce -- assert A (analog) too so the
                     * diagnostic covers the confirm buttons. */
                    /* BACK omitted: holding it pins the screen in state 0 */
                    a = 255;             /* A */
                }
                if (s_n == 300 || s_n == 340)
                    fprintf(stderr, "[FAKESTART] %s at frame %u\n",
                            s_n == 300 ? "press" : "release", s_n), fflush(stderr);
            }
        }
        an[0] = a; an[1] = b;
        {   /* One line per press edge (first 24): what the guest received and
             * what screen it was on, so a run log shows where a press went. */
            static int s_log = 0, s_was = 0;
            int now = (buttons != 0), ai;
            for (ai = 0; ai < 8; ai++) if (an[ai] >= 30) now = 1;
            if (now && !s_was && s_log < 24) {
                s_log++;
            }
            s_was = now;
        }
        /* Each pad struct (base 0x5E5CD0, stride 0x80) holds TWO consecutive
         * XINPUT_STATEs, 0x16 bytes each: one at pad+0x19 and one at pad+0x2F.
         * The game hands XInputGetState the second (pad+0x2F), but the
         * per-frame aggregate builder sub_0009EB90 reads the FIRST -- it walks
         * a cursor at 0x5E5CFB and loads wButtons from [cursor-0xE] = pad+0x1D,
         * which is pad+0x19 plus the 4-byte packet number. Filling only the
         * slot we were handed left the aggregates at 0x5E5ED8+i*0x2C at zero,
         * so no button ever reached the game and the post-movie screen sat in
         * its "press START" wait loop (0x00083480) forever.
         *
         * Write the state the caller asked for, and mirror it into the slot the
         * aggregate builder actually reads. */
        uint32_t slots[2];
        int nslots = 1;
        slots[0] = st;
        for (int i = 0; i < 4; i++) {
            if (st == 0x5E5CFFu + 0x80u * (uint32_t)i) {
                slots[1] = 0x5E5CE9u + 0x80u * (uint32_t)i;
                nslots = 2;
                break;
            }
        }
        for (int si = 0; si < nslots; si++) {
            uint32_t d = slots[si];
            MEM32(d) = s_packet;          /* dwPacketNumber */
            MEM16(d + 4) = buttons;       /* wButtons */
            for (int i = 0; i < 8; i++) MEM8(d + 6 + i) = an[i];   /* A B X Y Black White LT RT */
            MEM16(d + 14) = (uint16_t)lx; /* sThumbLX */
            MEM16(d + 16) = (uint16_t)ly; /* sThumbLY */
            MEM16(d + 18) = (uint16_t)rx; /* sThumbRX */
            MEM16(d + 20) = (uint16_t)ry; /* sThumbRY */
        }
    }
    {   extern unsigned g_in_getstate, g_in_build;
        extern volatile int g_doa3_post_movie;
        static unsigned n = 0;
        if (g_doa3_post_movie && (g_in_getstate % 256u) == 0 && n < 20) {
            n++;
             }
    }
    eax = 0;    /* ERROR_SUCCESS */
    esp += 12;
}

/* DIAG: cvFs device-range writers — the device table "count" (0xB25460/64)
 * goes to an empty range during the boot-phase transition, killing every
 * subsequent file open ('resolve dev="" -> 0', 'ptid range outside'). Trace
 * who resets it and with what values. */
/* DIAG: cvFsSetDefDev (sub_0016C8C0) - the default-device name at 0xB24FE0
 * gets CLEARED during the boot-phase transition (its failure path writes a
 * 0 byte), killing all subsequent default-device opens. Log the requested
 * name + result + the default before/after. */
void sub_0016C8C0_gen(void);
void sub_0016C8C0(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4);
    char name[24] = {0};
    for (int i = 0; i < 23 && a1; i++) { name[i] = (char)MEM8(a1 + i); if (!name[i]) break; }
    char pre[24] = {0};
    for (int i = 0; i < 23; i++) { pre[i] = (char)MEM8(0xB24FE0 + i); if (!pre[i]) break; }
    sub_0016C8C0_gen();
    s_n++;
    if (s_n <= 12) {
        char post[24] = {0};
        for (int i = 0; i < 23; i++) { post[i] = (char)MEM8(0xB24FE0 + i); if (!post[i]) break; }
        void *bt[5]; int n = CaptureStackBackTrace(1, 5, bt, NULL);
    }
}

void sub_0016C750_gen(void);
void sub_0016C750(void)
{
    static int s_n = 0;
    s_n++;
    if (s_n <= 8) {
        void *bt[6]; int n = CaptureStackBackTrace(1, 6, bt, NULL);
    }
    sub_0016C750_gen();
}

void sub_0016C767_gen(void);
void sub_0016C767(void)
{
    static int s_n = 0;
    s_n++;
    if (s_n <= 8) {
        void *bt[6]; int n = CaptureStackBackTrace(1, 6, bt, NULL);
    }
    sub_0016C767_gen();
}

void sub_0016C0F0_gen(void);
void sub_0016C0F0(void)
{
    static int s_n = 0;
    s_n++;
    if (s_n <= 8) {
        void *bt[6]; int n = CaptureStackBackTrace(1, 6, bt, NULL);
    }
    sub_0016C0F0_gen();
}


/* DIAG: LDW (load-work) op creation — the boot content load (fonts/screen
 * textures) never happens: sub_0009F730 kicks sub_0007FFB0(0x3BE, cfg) ->
 * sub_0007E720 creates the op, but phase op stayed 0. Log args/results. */
void sub_0007E720_gen(void);
void sub_0007E720(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    uint32_t esp0 = esp;
    sub_0007E720_gen();
    s_n++;
    if (s_n <= 40) {
        {   void *bt[10]; int nb = CaptureStackBackTrace(1, 10, bt, NULL), k;
             }
        if (a1 & 0x8000u) {
            /* named load: the "T:\%s" path is sprintf'd into the frame at
             * entry_esp-0x3C (sub_0007E720: sub esp,40h; push ebx; push esi;
             * lea ecx,[esp+0Ch]) and is still intact after the return. */
            char nm[48]; int k;
            for (k = 0; k < 47; k++) { nm[k] = (char)MEM8(esp0 - 0x3C + k); if (!nm[k]) break; }
            nm[k] = 0;
        }
        {   /* ADXF partition info (sub_00168E95): block = [0xC07640+4*ptid],
             * base sector u16 at +0x114, u16 sector sizes at +0x116+2*id,
             * sector(id) = base + sum(sizes[0..id)). Sample it on every load
             * so a corrupted table shows up against the boot values. */
            int pt;
            for (pt = 0; pt < 4; pt++) {
                uint32_t blk = MEM32(0xC07640 + 4u * pt);
                uint32_t sum = 0, i; char fn[24];
                if (blk < 0x10000 || blk >= 0x08000000u) continue;
                for (i = 0; i < 0x21D; i++) sum += MEM16(blk + 0x116 + 2u * i);
                for (i = 0; i < 23; i++) { fn[i] = (char)MEM8(blk + 0x10 + i); if (!fn[i]) break; }
                fn[i] = 0;
            }
        }
        {   /* DOA3 DIAG: DOA3_WATCHVA_EARLY=1 arms the exact-address write
             * watch here, at the second boot load (the table is verified
             * intact by the [PTINFO] line above), instead of after the movie,
             * to catch the writer that corrupts the loadfile.afs partition
             * table during the movie. */
            extern uint32_t g_watch_exact_va;
            extern void doa3_watch_arm(uint32_t xb_page);
            static int s_early = 0;
            if (s_n == 2) { extern void (*g_kernel_ptinfo_hook)(const char *); extern void doa3_ptinfo_check(const char *);
                            g_kernel_ptinfo_hook = doa3_ptinfo_check; doa3_ptinfo_check("ldw-op2"); }
            if (s_n == 2 && g_watch_exact_va && !s_early && getenv("DOA3_WATCHVA_EARLY")) {
                s_early = 1;
                doa3_watch_arm(g_watch_exact_va & ~0xFFFu);
            }
        }
    }
}
void sub_0007FFB0_gen(void);
void sub_0007FFB0(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0007FFB0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;   /* callee-saved: caller sub_0009F730 keeps 0 in esi across this call */
    s_n++;
    if (s_n <= 10) {
    }
}

/* DIAG: screen-element/texture-bind chain counters (draws=0 root hunt). */
/* High-rate pump/service traces. These emit tens of thousands of lines per
 * run; the write cost alone slowed the guest enough that timed runs ended
 * before the movie finished. Off unless DOA3_PUMPTRACE=1. */
static int doa3_pumptrace(void)
{
    static int s_v = -1;
    if (s_v < 0) {
        const char *e = getenv("DOA3_PUMPTRACE");
        s_v = (e && *e && *e != '0') ? 1 : 0;
    }
    return s_v;
}

#define CALL_COUNT_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        static int s_n = 0; \
        s_n++; \
        if (doa3_pumptrace() && (s_n <= 3 || (s_n % 5000) == 0)) { \
             \
             } \
        fn##_gen(); \
    }
/* Does the per-frame pad-vibration service run after the movie?
 *
 * sub_000679A0 is what advances the rumble elapsed counter at
 * 0x004920E0+0x24 and, when it passes the duration at +0x28, clears bit 3 of
 * 0x0049231C at 0x00067A92. That bit is the last thing holding the screen
 * latch shut: sub_00050250 refuses to release while sub_00067220 reports it.
 * Measured post-movie the elapsed counter never moves, so either this never
 * runs or it bails. Count it and its caller. */
void sub_000679A0_gen(void);
void sub_000679A0(void) {
    static unsigned s_n = 0;
    if ((++s_n % 600u) == 1) {
    }
    sub_000679A0_gen();
}
void sub_00068D90_gen(void);
void sub_00068D90(void) {
    static unsigned s_n = 0;
    if ((++s_n % 600u) == 1) {
    }
    sub_00068D90_gen();
}

CALL_COUNT_PROBE(sub_0008038C)
CALL_COUNT_PROBE(sub_000804EB)
void sub_0006B7E0_gen(void);
void sub_0006B7E0(void) {
    static int n = 0; int log = (n < 4); n++;
    if (log) {   }
    sub_0006B7E0_gen();
    if (log) {   }
}
CALL_COUNT_PROBE(sub_0006AAA0)
void sub_0006AD20_gen(void);
void sub_0006AD20(void) {
    static int n = 0; int log = (n < 4); n++;
    if (log) {   }
    sub_0006AD20_gen();
    if (log) {   }
}
CALL_COUNT_PROBE(sub_0006ACD0)
CALL_COUNT_PROBE(sub_0006AADC)
CALL_COUNT_PROBE(sub_0006AD6A)
CALL_COUNT_PROBE(sub_001C6740)
CALL_COUNT_PROBE(sub_00173B20)   /* wxCi cache-copy stream starter */
CALL_COUNT_PROBE(sub_0009DF60)   /* intro movie starter */
CALL_COUNT_PROBE(sub_00176580)   /* Sofdec movie player create */
/* sub_0009DE90 — movie frame blit (Sofdec frame -> D3D surface upload via
 * sub_001762B0 -> sub_001B3760/sub_001B37F0 push-buffer inline copy). The
 * per-frame movie service (sub_0009E1F0) calls it whenever sub_00177810
 * reports a frame, but while the mwPly state machine is broken (player state
 * obj+8 stays 0) the "frame" descriptor is garbage and the blit rep-movsd
 * marches off past the 4GB mapping (host memcpy crash, read base+>4GB).
 * Guard: only blit when the player exists and is PLAYING (state 2). */
void sub_0009DE90(void)
{
    extern void sub_0009DE90_gen(void);
    uint32_t obj = MEM32(0x5E5900);
    static int s_skip = 0;
    if (!obj || MEM32(obj + 8) != 2) {
        if (s_skip < 6) {
            s_skip++;
        }
        esp += 4;   /* cdecl: consume fake return, caller cleans args */
        return;
    }
    {
        static int s_run = 0;
        if (s_run < 6 || (s_run % 512) == 0) {
        }
        s_run++;
    }
    sub_0009DE90_gen();
    {
        static int s_done = 0;
        if (s_done < 6 || (s_done % 512) == 0) {
        }
        s_done++;
    }
}

/* DIAG: mwPly "new frame available?" peek. */
void sub_00177810_gen(void);
void sub_00177810(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4);
    sub_00177810_gen();
    if (s_n < 8 || (s_n % 2048) == 0) {
    }
    s_n++;
}

/* sub_001779A0 — "is the movie data stream finished?" = (streamStat == 4).
 * The gen lifts the `neg/sbb/inc` equality idiom with recomp bug #10 (carry
 * flag never produced), returning TRUE unconditionally — so the mwPly server
 * declared the ninja.sfd stream complete right after the first 1MB read and
 * killed the player (state 4) before a single frame decoded. Native impl. */
void sub_001779A0(void)
{
    extern void sub_0016D5D0(void);
    uint32_t h = MEM32(esp + 4);
    PUSH32(esp, h);
    PUSH32(esp, 0); sub_0016D5D0();
    esp += 4;                 /* our arg push */
    eax = (eax == 4) ? 1 : 0;
    esp += 4;                 /* consume fake return */
}

/* DIAG: mwPlyCreateSofdec (sub_001773E0/F0): print the config block + result.
 * cfg 0x5E59B0: +0x14 work ptr, +0x18 work size; fails silently (returns 0)
 * on "illegal work size" (needed = sub_00176550(cfg)+sub_00176500(cfg)). */
void sub_001773E0_gen(void);
void sub_001773E0(void)
{
    uint32_t cfg = MEM32(esp + 4);
    uint32_t wptr = cfg ? MEM32(cfg + 0x14) : 0, wsz = cfg ? MEM32(cfg + 0x18) : 0;
    sub_001773E0_gen();
}
CALL_COUNT_PROBE(sub_00176840)   /* mwPly destroy (DF60 bail path) */
/* sub_0009E1F0 is wrapped below with return-value counters instead. */
CALL_COUNT_PROBE(sub_00082EB0)   /* intro sequencer configure(movie_id) */
CALL_COUNT_PROBE(sub_00081EB0)   /* intro sequencer per-frame body */
CALL_COUNT_PROBE(sub_0007FD90)   /* screen-flip task (clears 0x4A10D0) */
CALL_COUNT_PROBE(sub_0007DFC0)   /* last call before 4A10D0 clear */
CALL_COUNT_PROBE(sub_0007F7B0)   /* first call in screen-flip task */
CALL_COUNT_PROBE(sub_00170660)   /* ADXM group-2 slot-1 dispatch (mwPly server) */
CALL_COUNT_PROBE(sub_00176E90)   /* mwPly server callback body */
CALL_COUNT_PROBE(sub_00170690)   /* ADXM group-5 dispatch (sfdec decode cb) */
CALL_COUNT_PROBE(sub_00176DC0)   /* mwPly group-5 callback (decode side) */
CALL_COUNT_PROBE(sub_0017C8D0)   /* sfdec main server */

/* DIAG: result probes along the ADXSTM-create chain. */
#define RET_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        static int s_n = 0; \
        fn##_gen(); \
        if (s_n < 8) { s_n++; \
             \
             } \
    }
void sub_0016EEF0_gen(void);
void sub_0016EEF0(void)
{
    static int s_n = 0;
    int u4 = 0;
    for (int k = 0; k < 16; k++)
        if (MEM8(0xC04380 + 0xA4u * k)) u4++;
    sub_0016EEF0_gen();
    if (s_n < 8) { s_n++;
         }
}
/* DIAG: PES queue append (fastcall eax=substream idx): find which substream
 * overflows (FF00040B). */
static void doa3_relocate_picture_index(void);
void sub_0017F330_gen(void);
void sub_0017F330(void)
{
    doa3_relocate_picture_index();
    static int s_n = 0;
    uint32_t q = eax, cls = ecx, h = MEM32(esp + 4), n = MEM32(esp + 8);
    uint32_t rec = (h >= 0x1000 && h < 0x8000000 && q < 8) ? h + q * 0x388u + 0xD34 : 0;
    uint32_t sj = rec ? MEM32(rec + 0x14) : 0;
    uint32_t c0 = (sj >= 0x1000 && sj < 0x8000000) ? MEM32(sj + 0xC) : 0;
    sub_0017F330_gen();
    uint32_t c1 = (sj >= 0x1000 && sj < 0x8000000) ? MEM32(sj + 0xC) : 0;
    if (s_n < 20 || (s_n % 2048) == 0 || (q == 1 && (s_n % 256) == 0) || q == 3 || q >= 4) {
    }
    s_n++;
}

/* DIAG: start-code scanner input (fastcall-ish: eax=parse ptr, ebx=len). */
void sub_0017BCC0_gen(void);
void sub_0017BCC0(void)
{
    static int s_n = 0;
    uint32_t p = eax, len = ebx;
    if (s_n < 12 || (s_n % 1024) == 0) {
    }
    s_n++;
    /* GUARD: with fewer than 4 bytes buffered there cannot be a start code;
     * the gen's not-enough-data path still reports "found" with code=0 and
     * the dispatcher treats code 0 as a fatal stream error (FF000D05),
     * permanently killing the handle on the FIRST pump tick (which races
     * the first ring read). Report not-found instead until data arrives. */
    if ((int)len < 4) {
        eax = 0;
        esp += 4;
        return;
    }
    sub_0017BCC0_gen();
}

/* item 102: sfdec frame-UNLOCK API (game vtable -> sub_0017AA20 ->
 * sub_00179DA0 -> sub_0017DB10 slot 2->0). If this fails (FF000F0E/F0F) or
 * stops being called, slots jam at state 2 = the item-95 stall. Log the
 * first calls, every 256th, and EVERY failure. Gen renamed _gen
 * (recomp_extra2.c — RE-APPLY after regen). */
void sub_0017AA20_gen(void);
void sub_0017AA20(void)
{
    static unsigned s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    sub_0017AA20_gen();
    s_n++;
    if (s_n <= 10 || (s_n & 255) == 0 || eax != 0) {
    }
}

/* DIAG: sfdec error reporter — log every error with a small backtrace of the
 * guest stack so the failing parse site is identifiable.
 * item 102: budget is PER ERROR CODE (a 40B storm used to eat the whole
 * 10-print budget and hid unlock failures like FF000F0E/F0F). */
void sub_0017C6A0_gen(void);
void sub_0017C6A0(void)
{
    static uint32_t s_codes[16]; static int s_counts[16]; static int s_nc = 0;
    uint32_t h = MEM32(esp + 4), err = MEM32(esp + 8);
    int ci = -1;
    for (int i = 0; i < s_nc; i++) if (s_codes[i] == err) { ci = i; break; }
    if (ci < 0 && s_nc < 16) { ci = s_nc++; s_codes[ci] = err; s_counts[ci] = 0; }
    int s_n = (ci >= 0) ? s_counts[ci] : 999;
    if (ci >= 0) s_counts[ci]++;
    if (s_n < 6) {
        /* NATIVE backtrace: the recompiled guest functions are host functions,
         * so the host stack IS the guest call chain. Resolve via doa3.map. */
        {
            void *bt[24];
            USHORT nfr = CaptureStackBackTrace(1, 24, bt, NULL);
        }
    }
    /* BACKPRESSURE: FF00040B = "substream ring full, cannot append". On
     * hardware the ingest simply retries next tick as the decoder drains;
     * storing it kills the handle (status -2). Swallow it as a warning. */
    if (err == 0xFF00040B) {
        static int s_bp = 0;
        if (s_bp < 4) {
            s_bp++;
        }
        eax = 0; esp += 4; return;
    }
    sub_0017C6A0_gen();
}
/* SJ commit APIs: sub_0016ABF0(sj, mode, chunk{ptr,len}) appends/credits —
 * mode 1: [sj+0xC] += len (+ ring copy); mode 0: [sj+0x10] += len.
 * sub_0016ACE0 similar family. Focus: does the READ RING SJ 0xC09890 ever
 * get credited (its 0-free state is what starves the demux, item 70/74). */
void sub_0016ABF0_gen(void);
void sub_0016ABF0(void)
{
    static int s_n = 0;
    uint32_t sj = MEM32(esp + 4), mode = MEM32(esp + 8), ch = MEM32(esp + 0xC);
    uint32_t len = (ch >= 0x1000 && ch < 0x8000000) ? MEM32(ch + 4) : 0;
    sub_0016ABF0_gen();
    if (s_n < 24 || sj == 0xC09890) {
        int log = (s_n < 24) || (s_n % 64 == 0);
        s_n++;
        if (log) {
        }
    } else s_n++;
}
void sub_0016ACE0_gen(void);
void sub_0016ACE0(void)
{
    static int s_n = 0;
    uint32_t sj = MEM32(esp + 4), mode = MEM32(esp + 8), ch = MEM32(esp + 0xC);
    uint32_t len = (ch >= 0x1000 && ch < 0x8000000) ? MEM32(ch + 4) : 0;
    sub_0016ACE0_gen();
    if (s_n < 24 || sj == 0xC09890) {
        int log = (s_n < 24) || (s_n % 64 == 0);
        s_n++;
        if (log) {
        }
    } else s_n++;
}
/* DIAG: decoder-system create chain. The frame-rate global 0xC0E4D0 divides
 * PTS math in sub_0017E240/sub_0017E7B6; if it stays 0 the parser crashes with
 * an integer divide once picture timing runs. It is written ONLY by
 * sub_0017C7B0 (rate = param struct +0xC), reached via
 * sub_0009DD20 -> sub_00175B90 -> sub_001768F0 -> sub_0017C890. */
void sub_00175B90_gen(void);
void sub_00175B90(void)
{
    uint32_t p = MEM32(esp + 4);
    sub_00175B90_gen();
}
void sub_001768F0_gen(void);
void sub_001768F0(void)
{
    sub_001768F0_gen();
}
void sub_0017C7B0_gen(void);
void sub_0017C7B0(void)
{
    sub_0017C7B0_gen();
}
/* DIAG: SJ sub-region carve helper (register-arg: eax=end, ecx=cur,
 * edx=base, ebx=mode, esi=out struct). size = eax-ecx; the negative-size
 * (0xFFFFF800) SJ creations come through here or its callers. */
void sub_0017EC10_gen(void);
void sub_0017EC10(void)
{
    static int s_n = 0;
    if (s_n < 40) {
        s_n++;
    }
    sub_0017EC10_gen();
}
/* DIAG: decoder state-machine pump + per-object dispatcher. */
void sub_0017D4B0_gen(void);
void sub_0017D4B0(void)
{
    static int s_n = 0;
    if (s_n < 6 || (s_n % 512) == 0) {
    }
    s_n++;
    sub_0017D4B0_gen();
}
/* DIAG: sub_0017D417 is a mid-body fragment of the sub_0017D380 state
 * machine (jumptable case "state 4"). It has NO static callers — reaching it
 * means an ICALL dispatched into the middle of the state machine with junk
 * registers. Dump the icall trace to find the dispatcher. */
void sub_0017D417_gen(void);
void sub_0017D417(void)
{
    static int s_n = 0;
    if (s_n < 8) {
        s_n++;
    }
    sub_0017D417_gen();
}
/* The picture index ring is carved immediately after its elementary-stream
 * ring, and the demuxer linearises a wrapped read by copying from the ring
 * base to ring_base+capacity -- the same address -- so the copy writes video
 * bytes straight over the index entries (verified: dst-src == capacity every
 * time, dst == the index table base).
 *
 * On hardware the decoder keeps up, the ring never approaches full, and the
 * wrapped span stays small enough to live in that slack. Here decode runs far
 * behind, the ring sits at ~100%% (c=0x2527E6 of 0x252800) and the linearised
 * span reaches 0x41316 bytes, shredding the table. The entries then decode as
 * garbage timestamps, the sfd handle never reaches PLAYEND and the intro
 * sequencer waits on a movie that never ends.
 *
 * Give the index its own allocation so the two cannot overlap. Cursors are
 * indices rather than pointers, so only the base moves; entries written
 * before the move are carried over. */
static void doa3_relocate_picture_index(void)
{
    static int s_done = 0;
    if (s_done) return;
    for (int q = 0; q < 8; q++) {
        uint32_t aux = 0xC0F7C0u + (uint32_t)q * 0x388u + 0xD44u;
        uint32_t ix  = aux + 0x30u;
        uint32_t rs  = MEM32(aux + 8), rb = MEM32(aux + 0xC);
        uint32_t t   = MEM32(ix), cap = MEM32(ix + 4);
        if (!rs || !rb || !t || !cap) continue;
        if (t != rs + rb) continue;          /* only the overlapping layout */
        {
            extern uint32_t xbox_HeapAllocHigh(uint32_t size, uint32_t alignment);
            uint32_t bytes = cap * 12u;
            uint32_t nb = xbox_HeapAllocHigh(bytes + 4096u, 4096);
            if (!nb) continue;
            memcpy(XBOX_PTR(nb), XBOX_PTR(t), bytes);
            MEM32(ix) = nb;
            s_done = 1;
        }
    }
}

/* GUARD: is `obj` one of the legit decoder-pool objects (base=[0xC0E518],
 * count=[0xC0E514], stride 0x60D8)? Junk objects reaching the state machine
 * trigger the sfdec error store at junk+0x988 -> corrupts the mwPly slot
 * table -> self-amplifying cascade (FF000222 spam, junk handles, crash). */
static int sfdec_pool_obj_ok(uint32_t obj)
{
    /* Reject only objects OUTSIDE guest RAM. On real hardware junk/residue
     * pointers inside guest RAM are walked harmlessly (low memory reads,
     * bounded error stores); the poison is 32-bit garbage beyond the 128MB
     * guest range, whose error-store at obj+0x988 aliases onto live
     * structures (mwPly slot table) and cascades. Alignment checks are too
     * strict: the state machine also legitimately runs on substream
     * sub-objects at interior offsets of the sfdec handle. */
    return obj >= 0x1000 && obj < 0x8000000u;
}
/* DIAG: sub_0017F710(h, i, k, ...) dispatches through a handler table at
 * h + i*0x610 + 0x2F4C, entry [k]. sub_0017D1D0 calls it with (7,7) on its
 * end-of-play path; a nonzero result makes sub_0017D1D0 return nonzero, so
 * the state-4 handler stores 1 instead of 6 (PLAYEND) and the handle cycles
 * 1->2->4->1 forever with h44 already 6. */
void sub_0017F710_gen(void);
void sub_0017F710(void)
{
    uint32_t h = MEM32(esp + 4), i = MEM32(esp + 8), k = MEM32(esp + 0xC);
    uint32_t tbl = 0, ent = 0;
    if (h >= 0x1000 && h < 0x8000000u) {
        tbl = MEM32(h + i * 0x610u + 0x2F4Cu);
        if (tbl >= 0x1000 && tbl < 0x8000000u) ent = MEM32(tbl + k * 4u);
    }
    sub_0017F710_gen();
    if (i == 7 && k == 7) {
        static int s_n = 0;
        if (s_n < 8) {
            s_n++;
        }
    }
}
void sub_0017D1D0_gen(void);
void sub_0017D1D0(void)
{
    static int s_junk = 0;
    if (!sfdec_pool_obj_ok(eax)) {
        if (s_junk < 4) {
            s_junk++;
        }
        eax = 0; esp += 4; return;
    }
    sub_0017D1D0_gen();
}
/* DIAG: frame get + YUV->RGB CSC kernel. */
void sub_0017D710_gen(void);
void sub_0017D710(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), outp = MEM32(esp + 8);
    sub_0017D710_gen();
    if (s_n < 8 || (s_n % 2048) == 0) {
    }
    s_n++;
}
void sub_001779D0_gen(void);
void sub_001779D0(void)
{
    static int s_n = 0;
    sub_001779D0_gen();
    if (s_n < 6 || (s_n % 2048) == 0) {
    }
    s_n++;
}
/* DIAG: mux queue-start propagation (q0 started + enough bytes ->
 * start queues [h+0x3564/3568/356C]). */
void sub_0017BDB0_gen(void);
void sub_0017BDB0(void)
{
    static int s_n = 0;
    uint32_t h = esi;
    sub_0017BDB0_gen();
    if (s_n < 8 || (s_n % 4096) == 0) {
        uint32_t ok = (h >= 0x1000 && h < 0x8000000u);
    }
    s_n++;
}
/* DIAG: prebuffer-complete gates. */
void sub_0017AAF0_gen(void);
void sub_0017AAF0(void)
{
    static int s_n = 0;
    uint32_t h = eax;
    sub_0017AAF0_gen();
    if (s_n < 8 || (s_n % 4096) == 0) {
        {
            uint32_t fc = MEM32(h + 0x3668); if (fc > 16) fc = 16;
            int c24 = 0;
            for (uint32_t k = 0; k < fc; k++) {
                uint32_t st = MEM32(h + 0x366C + 0x50u * k);
                if (st == 2 || st == 4) c24++;
            }
        }
    }
    s_n++;
}
void sub_00179E60_gen(void);
void sub_00179E60(void)
{
    static int s_n = 0;
    uint32_t h = edi;
    sub_00179E60_gen();
    if (s_n < 8 || (s_n % 4096) == 0) {
    }
    s_n++;
}
/* Unresolved RECOMP_ITAIL logger (see recomp_types.h). */
void recomp_itail_fail_log(uint32_t va)
{
    static uint32_t seen[16];
    static int n = 0;
    for (int i = 0; i < n; i++) if (seen[i] == va) return;
    if (n < 16) {
        seen[n++] = va;
    }
}
/* DIAG: esp-delta bisect for the skip-path -4 drift. */
#define DPROBE(fn) void fn##_gen(void); void fn(void) {     static int s_n = 0;     uint32_t e0 = esp;     fn##_gen();     if ((int)(esp - e0) != 4 && s_n < 6) {         s_n++;                       } }
DPROBE(sub_00179C30)
DPROBE(sub_00180EE0)
DPROBE(sub_00181060)
DPROBE(sub_001872C0)
DPROBE(sub_00186350)
DPROBE(sub_001863A0)
DPROBE(sub_001815D0)
DPROBE(sub_00181210)
DPROBE(sub_00181300)
#undef DPROBE
/* DIAG: skip-path slot alloc+header parse. */
void sub_00179A50_gen(void);
/* Per-picture slot FIFO (item 90): pictures complete in bitstream order,
 * but a picture can suspend across many decode invocations while LATER
 * pictures allocate — a single "last slot" cache binds the wrong slot.
 * PALLOC pushes; the completion patch in sub_0017A6D0 (recomp_cffix.c)
 * pops. */
uint32_t g_doa3_slotq[16];
uint32_t g_doa3_slotq_w = 0, g_doa3_slotq_r = 0;

static int doa3_reftrace_enabled(void)
{
    static int enabled = -1;
    if (enabled < 0)
        enabled = GetEnvironmentVariableA("DOA3_REFTRACE", NULL, 0) != 0;
    return enabled;
}

static int doa3_iframe_dump_enabled(void)
{
    static int enabled = -1;
    if (enabled < 0)
        enabled = GetEnvironmentVariableA("DOA3_IFRAME", NULL, 0) != 0;
    return enabled;
}

static int doa3_slot_index_from_record(uint32_t record)
{
    const uint32_t first = 0xC12E2Cu;
    if (record < first || record > first + 6u * 0x50u ||
        (record - first) % 0x50u != 0)
        return -1;
    return (int)((record - first) / 0x50u);
}

static uint32_t g_doa3_plane_hash[7][30];
static uint32_t g_doa3_plane_addr[7];
static uint32_t g_doa3_plane_serial[7];
static uint32_t g_doa3_plane_next_serial;

static uint32_t doa3_hash_bytes(const uint8_t *data, uint32_t size, uint32_t hash)
{
    for (uint32_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 16777619u;
    }
    return hash;
}

static uint32_t doa3_hash_plane_row(uint32_t plane, int macroblock_row)
{
    const uint8_t *y = (const uint8_t *)XBOX_PTR(plane);
    const uint8_t *u = y + 736u * 480u;
    const uint8_t *v = u + 384u * 240u;
    uint32_t hash = 2166136261u;
    for (int row = 0; row < 16; row++)
        hash = doa3_hash_bytes(y + (macroblock_row * 16 + row) * 736u, 720, hash);
    for (int row = 0; row < 8; row++) {
        hash = doa3_hash_bytes(u + (macroblock_row * 8 + row) * 384u, 360, hash);
        hash = doa3_hash_bytes(v + (macroblock_row * 8 + row) * 384u, 360, hash);
    }
    return hash;
}

static void doa3_snapshot_completed_plane(uint32_t record)
{
    static uint32_t completion = 0;
    int slot = doa3_slot_index_from_record(record);
    int trace = doa3_reftrace_enabled();
    int dump = doa3_iframe_dump_enabled();
    if ((!trace && !dump) || slot < 0)
        return;
    uint32_t plane = MEM32(record + 4);
    if (plane < 0x1000 || plane + 736u * 480u + 2u * 384u * 240u >= 0x8000000u)
        return;
    if (trace) {
        g_doa3_plane_addr[slot] = plane;
        g_doa3_plane_serial[slot] = ++g_doa3_plane_next_serial;
        for (int row = 0; row < 30; row++)
            g_doa3_plane_hash[slot][row] = doa3_hash_plane_row(plane, row);
    }
    if (dump && (completion < 36u || (completion % 6u) == 0)) {
        char path[64];
        snprintf(path, sizeof path, "iframe_%04u.raw", completion);
        FILE *frame = fopen(path, "wb");
        if (frame) {
            const uint8_t *y = (const uint8_t *)XBOX_PTR(plane);
            const uint8_t *u = y + 736u * 480u;
            const uint8_t *v = u + 384u * 240u;
            for (int row = 0; row < 480; row++) fwrite(y + row * 736, 1, 720, frame);
            for (int row = 0; row < 240; row++) fwrite(u + row * 384, 1, 360, frame);
            for (int row = 0; row < 240; row++) fwrite(v + row * 384, 1, 360, frame);
            fclose(frame);
        }
        FILE *index = fopen("iframe_index.csv", completion ? "ab" : "wb");
        if (index) {
            if (!completion)
                fputs("completion,slot,record,plane,pts,picture_type\n", index);
            fprintf(index, "%u,%d,%08X,%08X,%d,%u\n", completion, slot,
                    record, plane, (int)MEM32(record + 8), MEM32(0xC0F7C0 + 0x35F8));
            fclose(index);
        }
    }
    completion++;
}

static void doa3_compare_completed_plane(uint32_t plane)
{
    if (!doa3_reftrace_enabled())
        return;
    for (int slot = 0; slot < 7; slot++) {
        if (g_doa3_plane_addr[slot] != plane || !g_doa3_plane_serial[slot])
            continue;
        uint32_t changed = 0;
        for (int row = 0; row < 30; row++)
            if (g_doa3_plane_hash[slot][row] != doa3_hash_plane_row(plane, row))
                changed |= 1u << row;
        g_doa3_plane_serial[slot] = 0;
        return;
    }
}

void sub_00179A50(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), outp = MEM32(esp + 0xC);
    uint32_t old_states[7] = {0};
    uint32_t old_ref0 = 0, old_ref1 = 0, old_display = 0;
    int reftrace = doa3_reftrace_enabled();
    if (reftrace && h >= 0x1000 && h < 0x8000000u) {
        old_ref0 = MEM32(h + 0x3660);
        old_ref1 = MEM32(h + 0x3664);
        old_display = MEM32(h + 0x35C4);
        for (int i = 0; i < 7; i++)
            old_states[i] = MEM32(0xC12E2Cu + (uint32_t)i * 0x50u);
    }
    /* item 104: DECODE-AHEAD CAP. On hardware the 2-ref + 2-B-flip buffer
     * budget physically stops decode from running more than a few pictures
     * past display; our port had no such bound — decode raced ~47 pictures
     * ahead before the first serve, recycling the B flip-buffers, so the
     * movie visibly started ~1.5-2s in (item-95 #2) and mid-play content
     * could jump under load. Refuse the slot acquire (the gen's own
     * no-free-slot result, -1) while decode is >= 4 pictures ahead of
     * serve; the decoder retries next tick exactly like a full ring. */
    /* item 110 (user revert): the item-104 decode-ahead CAP is REMOVED. It
     * starved the MPEG parser and only swapped one freeze mode for another. */
    sub_00179A50_gen();
    if (reftrace && eax != 0) {
        static uint32_t failures = 0;
        if (failures++ < 32) {
        }
    }
    if (eax == 0 && outp >= 0x1000 && outp < 0x8000000u &&
        g_doa3_slotq_w - g_doa3_slotq_r < 16)
        g_doa3_slotq[g_doa3_slotq_w++ & 15] = MEM32(outp);
    if (reftrace && h >= 0x1000 && h < 0x8000000u &&
        outp >= 0x1000 && outp < 0x8000000u) {
        static uint32_t event = 0;
        uint32_t record = eax == 0 ? MEM32(outp) : 0;
        int slot = doa3_slot_index_from_record(record);
        uint32_t plane = slot >= 0 ? MEM32(record + 4) : 0;
        uint32_t duplicate = 0;
        if (plane) {
            for (int i = 0; i < 7; i++) {
                uint32_t other = 0xC12E2Cu + (uint32_t)i * 0x50u;
                if (i != slot && MEM32(other) != 0 && MEM32(other + 4) == plane)
                    duplicate |= 1u << i;
            }
        }
        if (eax == 0 || duplicate || record == old_ref0 || record == old_ref1) {
            uint32_t new_ref0 = MEM32(h + 0x3660), new_ref1 = MEM32(h + 0x3664);
        }
    }
    if (s_n < 10) {
        s_n++;
    }
}
/* DIAG: picture SKIP path — on hardware, prep-time pictures are skipped
 * (clock -1) and sub_0017A6D0 marks the slot state 2 via sub_0017DAF0;
 * skipped slots count toward the prebuffer target. */
void sub_0017A6D0_gen(void);
uint32_t g_doa3_a6d0_frame = 0;   /* item 90(c): A6D0 frame base (absolute) */
void sub_0017A6D0(void)
{
    static int s_n = 0;
    uint32_t esp_in = esp;
    /* locals live at (entry_esp - 4 fake-ret) - 0x38 frame; the completion
     * patch reads them via this absolute base when esp has drifted */
    uint32_t prev_frame = g_doa3_a6d0_frame;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    g_doa3_a6d0_frame = esp;   /* entry esp; the slot local sits at -0x38 */
    sub_0017A6D0_gen();
    g_doa3_a6d0_frame = prev_frame;
    if (s_n < 10) {
        s_n++;
    }
    /* item 91b attempt REVERTED: reclaiming the dead suspension frames
     * (esp = esp_in + 4) broke playback in the harness env too (frames=4)
     * — the drift ledger is compensated somewhere downstream; enforcement
     * at ANY boundary tried so far breaks it. (void) the saves. */
    (void)s_edi; (void)s_esi; (void)s_ebx;
}
/* Slot-lifecycle legality guards (see NOTES.md "Diagnostics").
 * The cooperative decode workers deliver picture lifecycle events OUT OF
 * ORDER vs real hardware: the user's stalled runs show ref-releases firing
 * on slots still DECODING (state 1 -> freed -> buffer reused mid-write =
 * tearing/half frames + state chaos jamming all slots at 2) and completion
 * marks landing on already-freed slots (0->2, 0->4 resurrections).
 * Semantics (gen): DB10: st==4 -> 3 else -> 0; DB30: st==4 -> 2 else -> 0;
 * DAF0: -> 2 (decode complete); DB00: -> 4 (serve). States: 0 free,
 * 1 decoding, 2 decoded/held, 3 released-after-display, 4 display-locked.
 * Guards: a release aimed at a DECODING slot is DEFERRED until decode
 * completes (then applies the original semantic); marks on freed slots are
 * DROPPED. Gen sub_0017DB10/sub_0017DB30 renamed _gen (recomp_0009.c —
 * RE-APPLY after regen). */
static int doa3_slot_rec_ok(uint32_t rec)
{
    return rec >= 0xC12E2Cu && rec <= 0xC1300Cu && ((rec - 0xC12E2Cu) % 0x50u) == 0;
}
/* item 104 (defect #2 start offset): decode races ~47 pictures ahead before
 * the first blit; the ref rotation frees DECODED-BUT-NEVER-DISPLAYED slots
 * (state 2 is ambiguous: awaiting-display and displayed-now-ref share it)
 * and the early pictures' buffers get rewritten — the movie visibly starts
 * ~1.5s in ([PDECIDE] proved nothing is DROPPED; 120-frame alignment puts
 * our first serves at ref 47-56). During the STARTUP WINDOW (first 90
 * blits) a decoded-but-unserved slot may not be freed: releases against it
 * defer, so decode blocks on slot availability exactly like hardware.
 * Fail-safe: a deferred entry stuck for >2000 drains force-releases (a
 * PSKIP'd never-displayed picture must not jam the ring). */
static uint8_t  g_doa3_slot_pending[7];
static uint32_t g_doa3_defer_age[16];
/* item 104 decode-ahead accounting (cap enforced in sub_00179A50) */
uint32_t g_doa3_pics_decoded = 0, g_doa3_pics_served = 0;
static int doa3_slot_idx(uint32_t rec)
{
    return doa3_slot_rec_ok(rec) ? (int)((rec - 0xC12E2Cu) / 0x50u) : -1;
}
static uint32_t g_doa3_rel_defer[16];
static int g_doa3_rel_defer_n = 0;
static void doa3_defer_release(uint32_t rec, int to3)
{
    for (int i = 0; i < g_doa3_rel_defer_n; i++)
        if ((g_doa3_rel_defer[i] & ~1u) == rec) return;
    if (g_doa3_rel_defer_n < 16) {
        g_doa3_defer_age[g_doa3_rel_defer_n] = 0;
        g_doa3_rel_defer[g_doa3_rel_defer_n++] = rec | (to3 ? 1u : 0u);
    }
}
void doa3_reset_slot_pending(void)
{
    for (int i = 0; i < 7; i++) g_doa3_slot_pending[i] = 0;
}
void doa3_drain_deferred_releases(void)
{
    for (int i = 0; i < g_doa3_rel_defer_n; ) {
        uint32_t rec = g_doa3_rel_defer[i] & ~1u;
        int to3 = (int)(g_doa3_rel_defer[i] & 1u);
        int idx = doa3_slot_idx(rec);
        uint32_t st = MEM32(rec);
        int hold = (st == 1) ||   /* decoding: never release */
                   (st == 2 && idx >= 0 && g_doa3_slot_pending[idx] &&
                    g_doa3_defer_age[i] <= 2000u);   /* awaiting first display */
        if (!hold) {
            MEM32(rec) = (st == 4) ? (to3 ? 3u : 2u) : 0u;
            if (idx >= 0) g_doa3_slot_pending[idx] = 0;
            g_doa3_rel_defer[i] = g_doa3_rel_defer[--g_doa3_rel_defer_n];
            g_doa3_defer_age[i] = g_doa3_defer_age[g_doa3_rel_defer_n];
        } else {
            g_doa3_defer_age[i]++;
            i++;
        }
    }
}
static int doa3_release_must_defer(uint32_t rec)
{
    /* item 110 (user revert): the item-104 release DEFERRAL is REMOVED — it was
     * a band-aid for the #2 start-offset that only changed the freeze mode, not
     * the underlying decode stall. Releases now run immediately (raw behavior). */
    (void)rec;
    return 0;
}
void sub_0017DB10_gen(void);
void sub_0017DB10(void)
{
    uint32_t rec = MEM32(esp + 4);
    if (doa3_reftrace_enabled() && doa3_slot_rec_ok(rec) && MEM32(rec) == 1) {
    }
    if (doa3_release_must_defer(rec)) {
        static int s_g = 0;
        if (s_g < 16) { s_g++;
             }
        doa3_defer_release(rec, 1);
        esp += 4;
        return;
    }
    sub_0017DB10_gen();
}
void sub_0017DB30_gen(void);
void sub_0017DB30(void)
{
    uint32_t rec = MEM32(esp + 4);
    if (doa3_reftrace_enabled() && doa3_slot_rec_ok(rec) && MEM32(rec) == 1) {
    }
    if (doa3_release_must_defer(rec)) {
        static int s_g = 0;
        if (s_g < 16) { s_g++;
             }
        doa3_defer_release(rec, 0);
        esp += 4;
        return;
    }
    sub_0017DB30_gen();
}
void sub_0017DB00_gen(void);
void sub_0017DB00(void)
{
    uint32_t slot = MEM32(esp + 4);
    if (slot && doa3_slot_rec_ok(slot) && MEM32(slot) == 0) {
        static int s_g = 0;
        if (s_g < 12) { s_g++;
             }
        esp += 4;
        return;
    }
    {   /* item 104: slot is being served — no longer awaiting first display */
        int idx = doa3_slot_idx(slot);
        if (idx >= 0) g_doa3_slot_pending[idx] = 0;
        g_doa3_pics_served++;
    }
    sub_0017DB00_gen();
    doa3_snapshot_completed_plane(slot);
    {   static int s_n = 0;
        if (s_n < 10) { s_n++;
             } }
}
void sub_0017DAF0_gen(void);
void sub_0017DAF0(void)
{
    static int s_n = 0;
    uint32_t slot = MEM32(esp + 4);
    if (slot && doa3_slot_rec_ok(slot) && MEM32(slot) == 0) {
        static int s_g = 0;
        if (s_g < 12) { s_g++;
             }
        esp += 4;
        return;
    }
    {   /* item 104: decode completed (1->2) during startup — this picture
         * must survive in its buffer until it is actually served */
        extern uint32_t g_doa3_movie_frames;
        int idx = doa3_slot_idx(slot);
        if (idx >= 0 && MEM32(slot) == 1) {
            g_doa3_pics_decoded++;
            if (g_doa3_movie_frames < 90u)
                g_doa3_slot_pending[idx] = 1;
        }
    }
    sub_0017DAF0_gen();
    doa3_snapshot_completed_plane(slot);
    if (s_n < 10) {
        s_n++;
    }
}
/* GUARD+POLICY: AV-sync picture decide sub_0017A540(ecx=pictype, edi=h).
 * During prep (status 2) the master clock is -1 (its hardware source is the
 * audio decode, which is stubbed) and the clock gate skips every picture, so
 * the prebuffer can never fill (see NOTES.md "Opening movie"). While in
 * prep and below the prebuffer target ([h+0x35D8] < [h+0xA2C]) decide=1
 * (decode) — exactly the behavior a live audio clock produces. Once PLAYING
 * the original pacing logic runs. */
void sub_0017A540_gen(void);
void sub_0017A540(void)
{
    static int s_n = 0; static unsigned s_all = 0, s_drop = 0;
    uint32_t pt = ecx, h = edi;
    sub_0017A540_gen();
    /* (item 89) labels: decide=1 -> DROP (sub_0017A640), 0 -> DECODE. */
    s_all++;
    if (eax == 1) s_drop++;
    /* item 104 (defect #2 start offset): the prep phase consumes ~47
     * pictures before the first serve — log EVERY early decision with the
     * gate inputs so one run names the dropping gate (clock h+0xCCC/0xCD0,
     * prebuffer have h+0x35D8 vs target h+0xA2C, served-pts h+0xCC4). */
    if (s_all <= 64 || (eax == 1 && s_drop <= 64)) {
    }
    (void)s_n;
}
/* item 104: the prep-complete gate (tail of the decide sub_0017A540) only
 * counts a picture toward the prebuffer ([h+0x35D8]++) when
 * sub_0017E720(h,&a,&b) >= 0 AND sub_00180B70(x,a,y,z) == 0. The counter
 * never moves in our runs -> prep never completes -> decode races ~47
 * pictures ahead recycling the B flip-buffers = the visible 1.5-2s start
 * offset. Probe both callees to name the failing one. */
void sub_0017E720_gen(void);
void sub_0017E720(void)
{
    static unsigned s_n = 0;
    uint32_t h = MEM32(esp + 4);
    sub_0017E720_gen();
    if (++s_n <= 40) {
    }
}
void sub_00180B70_gen(void);
void sub_00180B70(void)
{
    static unsigned s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC), a4 = MEM32(esp + 0x10);
    sub_00180B70_gen();
    if (++s_n <= 40) {
    }
}

/* DIAG: dword-fill helper sub_0017FB10(dst, val, ndwords) -- the ONLY writer
 * of the picture-type skip params h+0x99C..0x9A4 (write-watch verified).
 * Catch the fill that covers them and print a native backtrace. */
void sub_0017FB10_gen(void);
void sub_0017FB10(void)
{
    uint32_t dst = MEM32(esp + 4), val = MEM32(esp + 8), n = MEM32(esp + 0xC);
    if (dst <= 0xC1015C && dst + n * 4 > 0xC1015C) {
        static int s_n = 0;
        if (s_n < 6) {
            s_n++;
            void *bt[12];
            USHORT nf = CaptureStackBackTrace(1, 12, bt, NULL);
        }
    }
    sub_0017FB10_gen();
}
/* DIAG: video ES delivery + guarded index append. */
void sub_0017B7F0_gen(void);
void sub_0017B7F0(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    sub_0017B7F0_gen();
    if (s_n < 8 || (s_n % 2048) == 0) {
    }
    s_n++;
}
void sub_00182700_gen(void);
void sub_00182700(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), q = MEM32(esp + 8), e = MEM32(esp + 0xC);
    sub_00182700_gen();
    if (s_n < 8 || (s_n % 2048) == 0) {
    }
    s_n++;
}
/* DIAG: picture registration in the bitfeeder (writes rec+0x40?). */
void sub_00182000_gen(void);
void sub_00182000(void)
{
    static int s_n = 0;
    uint32_t ctx = eax;
    sub_00182000_gen();
    if (s_n < 8 || (s_n % 4096) == 0) {
    }
    s_n++;
}
/* DIAG: start-code class scanner + picture bitstream setup. */
void sub_001808C0_gen(void);
void sub_001808C0(void)
{
    static int s_n = 0;
    uint32_t p = MEM32(esp + 4), len = MEM32(esp + 8), mask = MEM32(esp + 0xC);
    sub_001808C0_gen();
    if (mask == 4 && (s_n < 8 || (s_n % 4096) == 0)) {
    }
    if (mask == 4) s_n++;
}
void sub_001827B0_gen(void);
void sub_001827B0(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), q = MEM32(esp + 8), pic = MEM32(esp + 0xC), outp = MEM32(esp + 0x10);
    sub_001827B0_gen();
    if (s_n < 8 || (s_n % 4096) == 0) {
        uint32_t aux = h + q * 0x388u + 0xD44u;
        uint32_t ix = aux + 0x30u;
        uint32_t tab = MEM32(ix), cap = MEM32(ix + 4), cnt = MEM32(ix + 8);
        uint32_t wr = MEM32(ix + 0xC), rd = MEM32(ix + 0x10);
        if (tab >= 0x1000 && tab < 0x8000000u && cap > 0 && cap < 200000) {
            for (uint32_t k = 0; k < 4; k++) {
                uint32_t e = tab + ((rd + k) % cap) * 12u;
            }
        }
    }
    s_n++;
}
/* DIAG: picture decode initiators inside sub_0017B2D0's tail. If the
 * sub_0017AB40 out-flag stays 0 these never run and no picture decodes. */
#define CNTPROBE(fn) void fn##_gen(void); void fn(void) {     static int s_n = 0;     fn##_gen();     if (s_n < 6 || (s_n % 4096) == 0) {                       }     s_n++; }
CNTPROBE(sub_0017A370)
CNTPROBE(sub_0017A400)
CNTPROBE(sub_0017AB40)
/* the ACTUAL picture decode (slices + MC/IDCT). Log frame-slot states after. */
void sub_0017A640_gen(void);
void sub_0017A640(void)
{
    static int s_n = 0;
    sub_0017A640_gen();
    if (s_n < 10 || (s_n % 1024) == 0) {
        uint32_t sh = 0xC0F7C0;
        uint32_t fc = MEM32(sh + 0x3668); if (fc > 16) fc = 16;
        for (uint32_t k = 0; k < fc; k++) {
            uint32_t sl = sh + 0x366C + 0x50u * k;
        }
    }
    s_n++;
}
#undef CNTPROBE
/* DIAG: picture-header parse chain (bit readers on ctx h+0x35E0). One of
 * these rejects every picture -> all pictures skipped -> zero decoded
 * frames -> black movie surface. */
#define PICPROBE(fn) void fn##_gen(void); void fn(void) {     static int s_n = 0;     uint32_t inh = eax, inc = ecx, ind = edx;     fn##_gen();     if (s_n < 40) {         s_n++;                       } }
PICPROBE(sub_00179990)
PICPROBE(sub_001799D0)
PICPROBE(sub_00179A10)
PICPROBE(sub_001798B0)
#undef PICPROBE
/* DIAG: slice decoder sub_0017A890(a1=win, a2=0xCC; esi=&ctx). Consumes
 * slice start codes from the ES window; frames stay state 1 because this
 * never completes a picture. */
void sub_0017A890_gen(void);
void sub_0017A890(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), sctx = esi;
    sub_0017A890_gen();
    if (s_n < 12 || (s_n % 4096) == 0) {
    }
    s_n++;
}
/* DIAG: queue window refresh sub_0017F130(ecx=h, esi=&window, arg=q).
 * Fills window {ptr,len,ptr2,len2} from the queue's SJ ring via vtbl+0x24
 * (avail) + vtbl+0x18 (region) + vtbl+0x1C (wrap region). The video decode
 * fetch sees an empty window while SJ 0xC09770 holds 2.4MB — this shows
 * exactly what the refresh yields. */
void sub_0017F130_gen(void);
void sub_0017F130(void)
{
    static int s_n = 0;
    uint32_t h = ecx, win = esi, q = MEM32(esp + 4);
    uint32_t rec = (h >= 0x1000 && h < 0x8000000 && q < 8) ? h + q * 0x388u + 0xD34 : 0;
    uint32_t sj = rec ? MEM32(rec + 0x14) : 0;
    sub_0017F130_gen();
    if ((q == 1 && (s_n < 12 || (s_n % 1024) == 0)) || s_n < 6) {
        uint32_t ok = (win >= 0x1000 && win < 0x8000000);
        uint32_t wp = ok ? MEM32(win) : 0;
    }
    s_n++;
}
/* DIAG: sequence-header configure path — sub_0017B2D0 (first-decode init)
 * and sub_0017B100 (configure + frame-slot setup via sub_0017AC20). */
void sub_0017B2D0_gen(void);
void sub_0017B2D0(void)
{
    static int s_n = 0;
    uint32_t h = ecx;
    sub_0017B2D0_gen();
    if (s_n < 8) {
        s_n++;
    }
}
void sub_0017B100_gen(void);
void sub_0017B100(void)
{
    static int s_n = 0;
    uint32_t h = eax;
    sub_0017B100_gen();
    if (s_n < 8) {
        s_n++;
    }
}
/* DIAG: video picture decode (sub_0017B3F0, reg-arg ecx=h + 2 stack args).
 * Nonzero return = decode failed/deferred; frame slots never fill. */
void sub_0017B3F0_gen(void);
void sub_0017B3F0(void)
{
    static int s_n = 0;
    uint32_t h = ecx, a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    sub_0017B3F0_gen();
    if (s_n < 12 || (s_n % 4096) == 0) {
    }
    s_n++;
}
/* DIAG: video-unit fetch/decode step head (sub_00179F00(h=ecx?, &out)).
 * Returns nonzero = nothing to do; the whole video decode chain bails on it
 * every tick (service sub_0017B6D0 always returns 0, no frame slots fill). */
void sub_00179F00_gen(void);
void sub_00179F00(void)
{
    static int s_n = 0;
    uint32_t h = ecx, outp = MEM32(esp + 4);
    sub_00179F00_gen();
    if (s_n < 10 || (s_n % 4096) == 0) {
    }
    s_n++;
}
/* NATIVE: substream vtable broadcast sub_0017F6D0(h, idx) — calls vtable
 * slot `idx` of each of the 9 substream contexts (h+0x2F4C stride 0x610).
 * Reimplemented natively because a callee in the seeded substream vtables
 * corrupts callee-saved registers / esp; the original translation then
 * restored junk into esi, which the state machine (sub_0017D380) used as
 * the current OBJECT -> junk-handle cascade (FF000222 storm, slot-table
 * corruption at h+0x988 error stores). Here we protect all guest registers
 * across each callee and log any clobber. */
void sub_0017F6D0(void)
{
    static int s_logs = 0;
    uint32_t h = MEM32(esp + 4), idx = MEM32(esp + 8);
    uint32_t s_esi = esi, s_edi = edi, s_ebx = ebx;
    uint32_t result = 0;
    if (h >= 0x1000 && h < 0x8000000u && idx < 32) {
        for (int i = 0; i < 9 && !result; i++) {
            uint32_t vt = MEM32(h + 0x2F4C + (uint32_t)i * 0x610);
            if (!vt) continue;
            if (vt < 0x10000 || vt >= 0x8000000u) {
                if (s_logs < 8) { s_logs++;
                      }
                continue;
            }
            uint32_t fn = MEM32(vt + idx * 4);
            if (!fn) continue;
            if (fn < 0x00011000 || fn >= 0x002CC800) {
                if (s_logs < 8) { s_logs++;
                      }
                continue;
            }
            uint32_t saved_esp = esp;
            PUSH32(esp, 0);
            PUSH32(esp, 0);
            PUSH32(esp, 0);
            PUSH32(esp, h);
            PUSH32(esp, 0);
            RECOMP_ICALL_SAFE(fn, saved_esp);
            result = eax;
            if (esp != saved_esp || esi != s_esi || edi != s_edi || ebx != s_ebx) {
                if (s_logs < 8) { s_logs++;
                     }
            }
            esp = saved_esp;
            esi = s_esi; edi = s_edi; ebx = s_ebx;
        }
    }
    eax = result;
    esp += 4; return;
}
/* item 100: movie AV-clock QPC pacing state (see the synthetic-clock block
 * below). File-scope so the not-playing branch can reset it between movies. */
static LONGLONG s_mvclk_last = 0;
static LONGLONG s_mvclk_qpf = 0;
static double   s_mvclk_accum = 0.0;

/* DIAG+GUARD: per-object sfdec server (register-arg esi=obj). The original
 * reads handler = [obj + state*4 + 0xA94] with state = sub_0017D510(obj,0xF)
 * and `call eax`s it. A garbage state indexes arbitrary memory; a garbage
 * table entry icalls junk with junk in the global eax = the junk-handle
 * cascade seed. Log and skip invalid state/handler. */
void sub_0017E9B0_gen(void);
void sub_0017E9B0(void)
{
    static int s_logs = 0;
    uint32_t obj = esi;
    uint32_t st = (obj >= 0x1000 && obj < 0x8000000u) ? MEM32(obj + 0x40) : 0xFFFF;
    uint32_t st15 = 0xFFFF;
    if (obj >= 0x1000 && obj < 0x8000000u) {
        /* replicate sub_0017D510(obj, 0xF) = read param 0xF: peek only */
        uint32_t saved = esp;
        PUSH32(esp, 0xF);
        PUSH32(esp, obj);
        PUSH32(esp, 0);
        { extern void sub_0017D510(void); sub_0017D510(); }
        st15 = eax;
        esp = saved;
    }
    if (st15 > 15) {
        if (s_logs < 8) {
            s_logs++;
        }
        return;
    }
    uint32_t h = MEM32(obj + st15 * 4 + 0xA94);
    if (h && (h < 0x00011000 || h >= 0x002CC800)) {
        if (s_logs < 8) {
            s_logs++;
        }
        MEM32(obj + st15 * 4 + 0xA94) = 0;   /* falls back to 0x17E2A0 default */
    }
    {
        static int s_dn = 0;
        if (s_dn < 8 || (s_dn % 2048) == 0) {
        }
        s_dn++;
    }
    sub_0017E9B0_gen();
    /* CLOCK BOOTSTRAP (see the [SFCRT] wrapper comment): the server just
     * cached the handler's clock, which is -1 while status==2 (prep). The
     * AV-sync decide skips every picture on a negative clock and the
     * prebuffer deadlocks (audio, the hardware clock source, is stubbed).
     * Substitute the demux PTS so prebuffer decode proceeds. */
    if (obj >= 0x1000 && obj < 0x8000000u && MEM32(obj + 0x40) == 2 &&
        (int)MEM32(obj + 0xCCC) < 0) {
        /* clock = 0 (start of stream), NOT the latest demux PTS: seeding the
         * far-ahead ingest PTS made the display believe it was at EOS the
         * moment PLAYING began (movie tore down after ~2 frames). */
        MEM32(obj + 0xCCC) = 0;
        MEM32(obj + 0xCD0) = MEM32(0xC0E4D0);
    }
    /* SYNTHETIC MOVIE CLOCK (item 82): in video-only playback the time-source
     * callback slot [obj+0xAD0] is empty and the default (sub_0017E2A0)
     * returns "no clock" (-1) forever — frames trickled at ~0.1fps and the
     * AV-sync decide skipped nearly every picture as "late". On hardware the
     * ADX audio clock drives this. Substitute vblank ticks since PLAYING
     * began (rate matches the sync-freq global 0xC0E4D0 = 59.94Hz ticks). */
    if (obj >= 0x1000 && obj < 0x8000000u) {
        extern uint32_t g_doa3_movie_ticks;   /* +2 per presented frame (FCOPY) */
        if (MEM32(obj + 0x40) == 4) {
            /* DECODE-PACED, not realtime: the recompiled decoder cannot keep
             * 29.97fps, so a wall-clock source makes every picture "late"
             * -> the AV-sync decide drops all of them and races to EOS.
             * Advance the clock (this server runs ~60Hz) only while a
             * decoded frame is waiting in a slot (state 2): the movie plays
             * at min(realtime, decode speed) with zero skips, and the clock
             * halts (no EOS race) whenever decode falls behind. */
            /* Picture PTS step is 750 clock units per frame (measured from
             * the index appends: 3000, 3750, 4500 ...). Advance at realtime
             * pace but never past the NEXT undelivered frame's due time —
             * otherwise the clock races ahead while a decoded frame waits
             * and the whole stream goes "late" (drop storm -> EOS race).
             * Delivered-frame count comes from the movie blit wrapper
             * (g_doa3_movie_frames).
             * item 100: "+375 per call" assumed this server runs at 60Hz;
             * after the QPC-detector fix the pump runs 90+/s and the movie
             * played ~1.5x fast (doa3_log_r3.txt) — just as the pre-fix
             * 40 pumps/s made it play 0.67x slow. The AV clock is the ONE
             * place wall time is correct (hardware feeds it from the
             * 59.94Hz crystal): accumulate elapsed QPC time at 22477.5
             * units/s (= 375 x 59.94). Scheduling stays count-based; the
             * undelivered-frame cap still halts the clock when decode
             * falls behind, and the accumulator is capped so a stall does
             * not bank a catch-up burst. */
            extern uint32_t g_doa3_movie_frames;
            {
                LARGE_INTEGER n;
                if (!s_mvclk_qpf) { LARGE_INTEGER f; QueryPerformanceFrequency(&f); s_mvclk_qpf = f.QuadPart; }
                QueryPerformanceCounter(&n);
                /* The clock's scale is [0xC0E4D0] (59939 units/second), so a
                 * second of wall time must add exactly that many units. The old
                 * 22477.5 (= 375 x 59.94) ran the clock at 0.375x the stream's
                 * rate: the library's display records carry PTS 0, 1000, 2000
                 * ... on a 30000 scale (1/30 s each), so a frame is
                 * 59939/29.97 = 2000 clock units, not 750. Every library test
                 * comparing this clock against a stream timestamp was off by
                 * 2.67x, including the end-of-display test that marks the video
                 * stream finished and drives the handle to PLAYEND. */
                if (s_mvclk_last)
                    s_mvclk_accum += (double)(n.QuadPart - s_mvclk_last) *
                                     (double)MEM32(0xC0E4D0) / (double)s_mvclk_qpf;
                s_mvclk_last = n.QuadPart;
                if (s_mvclk_accum > 4000.0) s_mvclk_accum = 4000.0;   /* max 2 frames of catch-up */
            }
            /* allow the clock past the next frame's PTS (delivery compare
             * may be strict-greater) but stay below the frame after it.
             * Drain CONTINUOUSLY (no 375 quantization): the quantized drain
             * measured 0.67x (doa3_log_r4.txt) — the clock parks cap-bound
             * mid-frame, the accumulator ceilings and every round discards
             * wall time. With a continuous drain the clock tracks wall time
             * exactly whenever the serve side keeps the cap ahead. */
            {
                /* The cap only exists to stop the clock racing past a picture
                 * that is decoded and still waiting for its display time. When
                 * no slot holds such a picture the cap protects nothing, and
                 * leaving it in place deadlocks the movie: the cap rises only
                 * when a frame is blitted, a frame is blitted only once the AV
                 * clock reaches its display time, and the clock cannot pass the
                 * cap -- playback used to park on (blits+1)*750+749 with the
                 * display queue empty. Cap only while a picture is pending. */
                int pending = 0;
                for (int k = 0; k < 7; k++)
                    if (MEM32(0xC12E2Cu + 0x50u * k) == 2) { pending = 1; break; }
                uint32_t cap = pending ? (g_doa3_movie_frames + 1u) * 2000u + 1999u
                                       : 0xFFFFFFFFu;
                if (g_doa3_movie_ticks < cap && s_mvclk_accum >= 1.0) {
                    double room = (double)(cap - g_doa3_movie_ticks);
                    double adv = s_mvclk_accum < room ? s_mvclk_accum : room;
                    uint32_t step = (uint32_t)adv;
                    g_doa3_movie_ticks += step;
                    s_mvclk_accum -= (double)step;
                }
            }
            MEM32(obj + 0xCCC) = g_doa3_movie_ticks;
            MEM32(obj + 0xCD0) = MEM32(0xC0E4D0);
        } else {
            g_doa3_movie_ticks = 0;    /* reset for the next movie */
            { extern uint32_t g_doa3_movie_frames; g_doa3_movie_frames = 0; }
            s_mvclk_last = 0; s_mvclk_accum = 0.0;
            {   /* item 104: clear startup slot-holds + ahead accounting */
                extern void doa3_reset_slot_pending(void);
                extern uint32_t g_doa3_pics_decoded, g_doa3_pics_served;
                doa3_reset_slot_pending();
                g_doa3_pics_decoded = 0;
                g_doa3_pics_served = 0;
            }
        }
    }
}
void sub_0017D380_gen(void);
void sub_0017D380(void)
{
    static int s_junk = 0;
    if (!sfdec_pool_obj_ok(eax)) {
        if (s_junk < 4) {
            s_junk++;
        }
        esp += 4; return;
    }
    sub_0017D380_gen();
}
void sub_0017D470_gen2(void);
void sub_0017D470(void)
{
    static int s_junk = 0;
    uint32_t h = MEM32(esp + 4);
    if (!sfdec_pool_obj_ok(h)) {
        if (s_junk < 4) {
            s_junk++;
        }
        eax = 0; esp += 4; return;
    }
    sub_0017D470_gen2();
}
void sub_0017ECC0_gen(void);
void sub_0017ECC0(void)
{
    static int s_n = 0;
    uint32_t slot = esi, vt = (esi >= 0x10000 && esi < 0x8000000) ? MEM32(esi) : 0;
    sub_0017ECC0_gen();
    if (s_n < 8) { s_n++;
        (void)vt;
         }
}
RET_PROBE(sub_001840F0)
RET_PROBE(sub_00184000)
RET_PROBE(sub_001842F0)
RET_PROBE(sub_001790C0)
RET_PROBE(sub_00178FD0)
RET_PROBE(sub_0017B6D0)
RET_PROBE(sub_0016DB80)
RET_PROBE(sub_0016DF10)
RET_PROBE(sub_00173930)
RET_PROBE(sub_001740D0)

/* DIAG: SJ creator + wxCi session creator (both inside the ADXSTM create). */
void sub_0016AE80_gen(void);
void sub_0016AE80(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    if (a2 & 0x80000000u) {
        /* Negative region size (X-0x800 with X=0 upstream). A ring with cap
         * 0xFFFFF800 writes anywhere = guest-wide memory corruption (junk
         * sfdec handles / ASCII stream bytes showing up as pointers).
         * Dump the caller frame to identify the layout bug, then clamp. */
        fprintf(stderr, "[SJNEG] a1=0x%X a2=0x%X frame:", a1, a2);
        MEM32(esp + 8) = 0x800;  /* clamp: minimal sane ring */
    }
    sub_0016AE80_gen();
    {   extern volatile int g_doa3_post_movie;
        if (s_n < 40 || g_doa3_post_movie) { s_n++;
             } }
}
void sub_0016E060_gen(void);
void sub_0016E060(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4);
    sub_0016E060_gen();
    if (s_n < 12) { s_n++;
         }
}

/* DIAG: ADXSTM creator (sub_0016A130(buf, bufsize, nstm)): the audio
 * substream init (sub_00178D60) fails here with FF000C04. */
void sub_0016A130_gen(void);
void sub_0016A130(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    sub_0016A130_gen();
    if (s_n < 10) {
        s_n++;
        {   int used = 0; char bits[20];
            for (int k = 0; k < 16; k++) {
                bits[k] = MEM8(0xC04DE0 + 0x9Cu * k) ? '1' : '0';
                if (bits[k] == '1') used++;
            }
            bits[16] = 0;
            {   int u2 = 0, u3 = 0, u4 = 0;
                for (int k = 0; k < 16; k++) {
                    if (MEM16(0xBF7BC0 + 0xB8u * k)) u2++;
                    if (MEM32(0xBF7460 + 0x34u * k)) u3++;
                    if (MEM8(0xC04380 + 0xA4u * k)) u4++;
                }
            }
        }
    }
}

/* DIAG: sfdec create (sub_0017D240): print the 15-dword param block to see
 * which field is null when the audio substream init is active. */
void sub_0017D240_gen(void);
void sub_0017D240(void)
{
    static int s_n = 0;
    uint32_t p = MEM32(esp + 4);
    if (s_n < 3) {
        s_n++;
    }
    sub_0017D240_gen();
    if (s_n <= 3) {
        uint32_t slot0 = MEM32(0xC0E518);
    }
    /* MISSING-ATTACH FIX: SJ-backed queues (type 5) carve an extra region
     * after their ring for the PICTURE INDEX (12-byte entries; size stored
     * at rec+0x24), but nothing in the emitted code ever initializes the
     * index struct at rec+0x40 {table,cap,count,cursor,rd}. Without it the
     * demux-side append (sub_00182700 -> sub_001825D0) skips silently and
     * the picture setup (sub_001827B0) rejects every picture -> no video
     * decode ever happens. Attach it here right after sfdec create. */
    if (eax >= 0x1000 && eax < 0x8000000u) {
        uint32_t hh = eax;
        for (uint32_t qq = 0; qq < 8; qq++) {
            uint32_t rec = hh + qq * 0x388u + 0xD34;
            if (MEM32(rec) != 5) continue;               /* type-5 only */
            uint32_t base = MEM32(rec + 0x18), ringsz = MEM32(rec + 0x1C);
            uint32_t idxsz = MEM32(rec + 0x24);
            if (!base || !idxsz || MEM32(rec + 0x40)) continue;
            MEM32(rec + 0x40) = base + ringsz;           /* table */
            MEM32(rec + 0x44) = idxsz / 12u;             /* cap (entries) */
            MEM32(rec + 0x48) = 0;                       /* count */
            MEM32(rec + 0x4C) = 0;                       /* write cursor */
            MEM32(rec + 0x50) = 0;                       /* read cursor */
            fprintf(stderr, "[IDXFIX] q%u table=0x%X cap=%u\n",
                    qq, base + ringsz, idxsz / 12u);
        }
        /* PASS-THROUGH BYPASS: substreams 4/5 (the q3->q5 and q4->q6 movers)
         * have no vtable in this config (table 0x2199AC slots 4/5 = 0), so no
         * service ever moves entries or flags between those queues, and the
         * output substreams (6=video, 7=audio) starve reading q5/q6. The
         * pass-throughs transform nothing -> point the consumers directly at
         * the producer queues. */
        if (MEM32(hh + 0x53B0) == 5) {
            MEM32(hh + 0x53B0) = 3;                      /* video out: q5 -> q3 */
            fprintf(stderr, "[QBYP] slot6 reads q3\n");
        }
        if (MEM32(hh + 0x59C0) == 6) {
            MEM32(hh + 0x59C0) = 4;                      /* audio out: q6 -> q4 */
            fprintf(stderr, "[QBYP] slot7 reads q4\n");
        }
    }
}

/* DIAG: sfdec size calculator (create path) — print the size table it fills.
 * After the 30 vtable-fn emissions the sfdec create started failing with
 * params+4==0; these outputs feed the buffer carve in sub_00176970. */
void sub_00176330_gen(void);
void sub_00176330(void)
{
    static int s_n = 0;
    sub_00176330_gen();
    if (s_n < 3) {
        s_n++;
    }
}

/* DIAG: sfdec state-2 evaluator (fastcall, handle in eax): print cmd (+0x44),
 * gate sub_0017C980 result path, and the state it returns. */
void sub_0017D0E0_gen(void);
void sub_0017D0E0(void)
{
    static unsigned s_n = 0;
    uint32_t h = eax;
    uint32_t cmd = MEM32(h + 0x44), st = MEM32(h + 0x40);
    sub_0017D0E0_gen();
    if (++s_n <= 6 || (s_n % 2000) == 0) {
    }
}

/* DIAG: gate sub_0017C980 (fastcall-ish, handle in esi). */
void sub_0017C980_gen(void);
void sub_0017C980(void)
{
    static unsigned s_n = 0;
    uint32_t h = esi;
    sub_0017C980_gen();
    if (++s_n <= 6 || (s_n % 2000) == 0) {
        for (int si = 0; si < 9; si++) {
            uint32_t vt = MEM32(h + 0x2F4C + 0x610u * si);
        }
    }
}
/* Boot-task body enter/exit markers (BOOT_MARK was #undef'd above). */
#define BOOT_MARK2(name) \
    void name(void) { \
        extern void name##_gen(void); \
        static int n = 0; \
        int log = (n < 60); n++; \
        if (log) {   } \
        name##_gen(); \
        if (log) {   } \
    }
/* DirectSoundCreate chain (cxbx symbol cache names): sub_001C800C = create
 * path, sub_001C7ECD = CDirectSound::Initialize, sub_001C6AA0 = DSOUND pool
 * alloc, sub_001C765A / sub_001C8A76 / sub_001C90C0 = APU driver setup steps.
 * Enter/exit markers to find which step leaves the game with pDS == NULL. */
#define BOOT_MARK2_ABI(name) \
    void name(void) { \
        extern void name##_gen(void); \
        static int n = 0; \
        uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx; \
        int log = (n < 60); n++; \
        if (log) {   } \
        name##_gen(); \
        if (log) {   } \
        edi = s_edi; esi = s_esi; ebx = s_ebx; \
    }
/* silence-loop buffer built at the end of CDirectSound::Initialize:
 * CreateSoundBuffer / SetBufferData / Play, and the two DSERR_INVALIDCALL
 * producers reachable from them. */
void sub_001C7DE1_gen(void);
void sub_001C7DE1(void)   /* CDirectSound::CreateSoundBuffer(desc in eax?, ...) */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp;
    uint32_t in_eax = eax, in_ecx = ecx, in_edx = edx, a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    sub_001C7DE1_gen();
    if (n < 6) { n++;
        
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI(sub_001C7B6F)
BOOT_MARK2_ABI(sub_001C7B33)
void sub_001C8135_gen(void);
void sub_001C8135(void)   /* CMcpxAPU buffer page/voice allocation check */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    uint32_t self = ecx, buf = MEM32(esp + 4);
    uint32_t st = (buf > 0x1000 && buf < 0x8000000) ? MEM32(buf + 0xE0) : 0;
    uint32_t fmt = (st > 0x1000 && st < 0x8000000) ? MEM32(st + 0x10) : 0;
    if (n < 6) { n++;
         }
    sub_001C8135_gen();
    if (n <= 6) {  }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI(sub_001C927C)
BOOT_MARK2_ABI(sub_001C7CE5)
BOOT_MARK2_ABI(sub_001C7D21)
BOOT_MARK2_ABI(sub_001C7C29)
/* APU interrupt delivery (see apu_core.c g_apu_irq_pending). Runs the
 * DirectSound driver's KINTERRUPT service routine (the one registered from
 * the DSOUND library, routine VA 0x001C0000..0x001E0000) and then every DPC
 * it queued, all as ordinary guest calls on this thread. */
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
        if (routine < 0x001C0000u || routine >= 0x001E0000u) continue;
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

/* DirectSound's retire wait (sub_001C9C92): spin until the buffer's
 * retire-pending bit (0x10000000) is cleared by the ISR's idle-voice
 * retirement. Both callers spin at PASSIVE level with the driver's busy
 * count released, where the hardware interrupt arrives; here it is delivered
 * from inside the wait. All guest registers are preserved across it. */
void doa3_apu_wait_retire(uint32_t obj)
{
    uint32_t r_eax = eax, r_ecx = ecx, r_edx = edx, r_ebx = ebx, r_esi = esi,
             r_edi = edi, r_esp = esp, r_seh = g_seh_ebp;
    DWORD t0 = GetTickCount();
    int warned = 0;
    while (MEM32(obj + 8) & 0x10000000u) {
        doa3_apu_deliver_irq();
        eax = r_eax; ecx = r_ecx; edx = r_edx; ebx = r_ebx; esi = r_esi;
        edi = r_edi; esp = r_esp; g_seh_ebp = r_seh;
        if (!(MEM32(obj + 8) & 0x10000000u)) break;
        Sleep(1);
        if (!warned && GetTickCount() - t0 > 500) {
            warned = 1;
        }
    }
}
BOOT_MARK2_ABI(sub_001CC584)
void sub_001CCBA7_gen(void);
void sub_001CCBA7(void)   /* CMcpxVoiceClient: fill / start after SetBufferData */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp, vc = ecx;
    int ok = (vc > 0x1000 && vc < 0x8000000);
    if (n < 8) { n++;
         }
    sub_001CCBA7_gen();
    if (n <= 8) {  }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
#define BOOT_MARK2_ABI_ESI(name)     void name(void) {         extern void name##_gen(void);         static int n = 0;         uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, s_esp = esp;         int log = (n < 12); n++;         name##_gen();         if (log) {   }         edi = s_edi; esi = s_esi; ebx = s_ebx;     }
BOOT_MARK2_ABI_ESI(sub_001CD60C)
void sub_001789B0_gen(void);
void sub_001789B0(void)   /* Sofdec audio server step: this in eax; gates on flag[6] and the audio stream state */
{
    static int n = 0;
    uint32_t pl = eax, idx = MEM32(pl + 0x4184);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001789B0_gen();
    n++;
    if (n <= 12 || (n % 300) == 0) {
        
    }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI_ESI(sub_0017D510)
BOOT_MARK2_ABI_ESI(sub_0017EDC0)
void sub_0017EDE0_gen(void);
void sub_0017EDE0(void)   /* Sofdec: bytes queued in stream N (player, N) */
{
    static int n = 0;
    uint32_t pl = MEM32(esp + 4), idx = MEM32(esp + 8);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0017EDE0_gen();
    if (idx == 0 && n < 60) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI_ESI(sub_0017CA00)
BOOT_MARK2_ABI_ESI(sub_0017EF40)
BOOT_MARK2_ABI_ESI(sub_00178A20)
BOOT_MARK2_ABI_ESI(sub_00178C20)
BOOT_MARK2_ABI_ESI(sub_00178CB0)
BOOT_MARK2_ABI_ESI(sub_00178430)
BOOT_MARK2_ABI_ESI(sub_0017FBA0)
BOOT_MARK2_ABI_ESI(sub_001CA851)
BOOT_MARK2_ABI_ESI(sub_001C984F)
BOOT_MARK2_ABI_ESI(sub_00172260)
BOOT_MARK2_ABI_ESI(sub_00172330)
uint32_t g_doa3_last_adx_dev = 0;   /* device handed to the ADX Start method most recently (the movie voice) */
void sub_00172360_gen(void);
void sub_00172360(void)
{
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    g_doa3_last_adx_dev = MEM32(esp + 4);
    sub_00172360_gen();
    
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI_ESI(sub_00172410)
BOOT_MARK2_ABI_ESI(sub_00172470)
BOOT_MARK2_ABI_ESI(sub_001724F0)
BOOT_MARK2_ABI_ESI(sub_00172580)
BOOT_MARK2_ABI_ESI(sub_001725B0)
BOOT_MARK2_ABI_ESI(sub_001725E0)
BOOT_MARK2_ABI_ESI(sub_00172610)
BOOT_MARK2_ABI_ESI(sub_00172640)
BOOT_MARK2_ABI_ESI(sub_00172680)
BOOT_MARK2_ABI_ESI(sub_001726B0)
BOOT_MARK2_ABI_ESI(sub_001726D0)
BOOT_MARK2_ABI_ESI(sub_0007FFD0)
BOOT_MARK2_ABI_ESI(sub_0009CF10)
BOOT_MARK2_ABI_ESI(sub_0009D320)
BOOT_MARK2_ABI_ESI(sub_001C6B92)
BOOT_MARK2_ABI_ESI(sub_001CCCE8)
/* 0x001CDCD8 is a 5-byte `jmp 0x001CDB16` stub that the page-manager vtable
 * (slot +0x14, called from sub_001CC3AC to map a buffer into SGE entries)
 * points at. The lifter glued it to the dead tail of sub_001CDC98, so the
 * indirect call resolved to nothing and returned 0 = "no entries". */
void sub_001CDCD8(void) { sub_001CDB16(); }
BOOT_MARK2_ABI_ESI(sub_001C7C92)
BOOT_MARK2_ABI_ESI(sub_001C76DF)
BOOT_MARK2_ABI_ESI(sub_001CCD0E)
BOOT_MARK2_ABI_ESI(sub_001C6DE3)
BOOT_MARK2_ABI_ESI(sub_001CCC97)
void sub_001C7873_gen(void);
void sub_001C7873(void)   /* CDirectSoundBufferSettings::SetBufferData(this=settings, pv, cb) */
{
    static int n = 0;
    uint32_t self = ecx, pv = MEM32(esp + 4), cb = MEM32(esp + 8);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp;
    sub_001C7873_gen();
    if (n < 16) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001CC223_gen(void);
void sub_001CC223(void)   /* CMcpxVoiceClient::GetCurrentPosition(this, pPlay, pWrite) */
{
    static int n = 0;
    uint32_t self = ecx, st = MEM32(ecx + 0x148), pplay = MEM32(esp + 4), pwrite = MEM32(esp + 8);
    uint32_t size = (st > 0x1000 && st < 0x8000000) ? MEM32(st + 0x4C) : 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (n < 12) { n++;
        { uint32_t fmt = st ? MEM32(st + 0x10) : 0, buf = st ? MEM32(st + 0x48) : 0, sum = 0, nz = 0, i;
          if (buf > 0x1000 && buf < 0x8000000) for (i = 0; i < size && i < 0x10000; i += 4) { uint32_t v = MEM32(buf + i); sum += v; nz += (v != 0); }
           }
         }
    if (size == 0) {   /* the original divides by the buffer size; report position 0 instead of faulting */
        if (pplay) MEM32(pplay) = 0;
        if (pwrite) MEM32(pwrite) = 0;
        eax = 0; esp += 12; return;   /* ret 8 */
    }
    sub_001CC223_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI_ESI(sub_001CE8E6)
BOOT_MARK2_ABI_ESI(sub_001CE347)
BOOT_MARK2_ABI_ESI(sub_001CE564)
BOOT_MARK2_ABI_ESI(sub_001CE3AD)
BOOT_MARK2_ABI_ESI(sub_001CE2D7)
BOOT_MARK2_ABI_ESI(sub_001CD7A3)
BOOT_MARK2_ABI_ESI(sub_001CD6D8)
BOOT_MARK2_ABI_ESI(sub_001CCE10)
BOOT_MARK2_ABI_ESI(sub_001CD1AA)
BOOT_MARK2_ABI_ESI(sub_001CD00F)
BOOT_MARK2_ABI_ESI(sub_001CD352)
BOOT_MARK2_ABI_ESI(sub_001CE507)
BOOT_MARK2_ABI_ESI(sub_001C925A)
BOOT_MARK2_ABI_ESI(sub_001CD8E8)
static void apu_dump_block(const char *tag, uint32_t apu)
{
    int i;
}
void sub_001CCEE5_gen(void);
void sub_001CCEE5(void)   /* CMcpxAPU: GP/EP program + page-list setup */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, apu = ecx;
    if (n < 4) { n++; apu_dump_block("[APU-CEE5 in ]", apu); }
    sub_001CCEE5_gen();
    if (n <= 4) { apu_dump_block("[APU-CEE5 out]", apu); }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001CDDD5_gen(void);
void sub_001CDDD5(void)   /* page-list builder: this=ecx, arg=[esp+4] */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, self = ecx, arg = MEM32(esp + 4);
    uint32_t desc = MEM32(self + 0x20), flag = MEM32(self + 8);
    uint32_t off = (flag != 1) ? 0xA8 : 0x90;
    uint32_t base = MEM32(desc + off + 0x10), size = MEM32(desc + off + 0x18), count = (size >> 3) & 0xFFFF;
    if (n < 6) { n++;
         }
    if ((int)(count - arg) < 0 || (int)(count - arg) > 0x4000) {
        
        esp += 8; edi = s_edi; esi = s_esi; ebx = s_ebx; return;   /* ret 4 */
    }
    sub_001CDDD5_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001C8C9A_gen(void);
void sub_001C8C9A(void)   /* CMcpxAPU::CreateVoiceClient(settings, callbacks, buffer, out) */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp;
    if (n < 8) { n++;
         }
    sub_001C8C9A_gen();
    if (n <= 8) {  }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001C7477_gen(void);
void sub_001C7477(void)   /* IDirectSoundBuffer method (this, value); this-0x1C = CDirectSoundBuffer */
{
    static int n = 0;
    uint32_t self = MEM32(esp + 4), arg = MEM32(esp + 8);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (self < 0x10000 || self >= 0x08000000u) {
        if (n < 20) { n++;
             }
        eax = 0x80004005u; esp += 12; return;   /* ret 8: refuse instead of walking the zero page */
    }
    if (n < 8) { n++;  }
    sub_001C7477_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001C7EA9_gen(void);
void sub_001C7EA9(void)   /* IDirectSound_CreateSoundBuffer(this, desc, out) */
{
    static int n = 0;
    uint32_t ds = MEM32(esp + 4), desc = MEM32(esp + 8), out = MEM32(esp + 12);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp;
    sub_001C7EA9_gen();
    if (n < 12) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001C75DD_gen(void);
void sub_001C75DD(void)   /* IDirectSoundBuffer_Lock(this, off, bytes, ppv1, pcb1, ppv2, pcb2, flags) */
{
    static int n = 0;
    uint32_t a[8], i; for (i = 0; i < 8; i++) a[i] = MEM32(esp + 4 + 4 * i);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    {   static uint32_t s_ppv = 0, s_pcb = 0;   /* what did the caller write into the region it locked last time? */
        if (s_ppv > 0x1000 && s_ppv < 0x8000000 && n < 80) { uint32_t k, nz = 0; for (k = 0; k < s_pcb; k += 4) nz += (MEM32(s_ppv + k) != 0);
             }
        sub_001C75DD_gen();
        s_ppv = (a[3] > 0x1000 && a[3] < 0x8000000) ? MEM32(a[3]) : 0; s_pcb = (a[4] > 0x1000 && a[4] < 0x8000000) ? MEM32(a[4]) : 0; if (s_pcb > 0x10000) s_pcb = 0;
    }
    if (n < 80) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_00172DE0_gen(void);
void sub_00172DE0(void)   /* CRI ADX output writer: (obj) -> pushes decoded PCM to each attached device */
{
    static int n = 0;
    uint32_t obj = MEM32(esp + 4);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (n < 40) { n++;
         }
    sub_00172DE0_gen();
    if (n <= 40) {  }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_00173070_gen(void);
void sub_00173070(void)   /* CRI ADX output server step: gates the PCM writer on the manager state at 0xBF8740 */
{
    static int n = 0; static uint32_t last[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, k;
    for (k = 0; k < 4; k++) { uint32_t m = 0xBF8740 + k * 0x4D8, st = MEM32(m);
        if (st != last[k] && n < 60) { n++; last[k] = st;
             } }
    sub_00173070_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_00172D30_gen(void);
void sub_00172D30(void)   /* CRI ADX output server, mode 1: pull decoded data from the source and probe it */
{
    static int n = 0;
    uint32_t obj = MEM32(esp + 4), src = MEM32(obj + 0x1C);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_00172D30_gen();
    if (n < 24) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
void sub_001CCB38_gen(void);
void sub_001CCB38(void)   /* CMcpxVoiceClient::Initialize(settings, callbacks, buffer) */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx, e_esp = esp, vc = ecx;
    int ok = (vc > 0x1000 && vc < 0x8000000);
    if (n < 8) { n++;
         }
    sub_001CCB38_gen();
    if (n <= 8) {  }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI(sub_001C800C)
BOOT_MARK2_ABI(sub_001C7ECD)
BOOT_MARK2_ABI_ESI(sub_001C6AA0)
BOOT_MARK2_ABI(sub_001C765A)
void sub_001C8A76_gen(void);
void sub_001C8A76(void)   /* CMcpxAPU constructor: ABI-enforced (edi leaked) */
{
    static int n = 0;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001C8A76_gen();
    if (n < 4) { n++;
         }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}
BOOT_MARK2_ABI(sub_001C90C0)

/* The resource-load service (sub_000804EB) keeps the slot pointer in ESI
 * across its callees and, on completion at 0x00080646, writes the request key
 * back through it: [esi+3] = [esi+1], [esi+4] = [esi+2]. ESI is callee-saved,
 * but the recompiled callees drop it (the split-epilogue class), so the
 * completed resource is filed under key (0xFF,0xFF) instead of its real one.
 * sub_0007ED10 then never finds it and sub_00074470 retries forever, which is
 * what keeps the screen dispatcher out of its loop. Enforce the ABI. */
#define ABI_MARK2(name) \
    void name##_gen(void); \
    void name(void) { \
        uint32_t _di = edi, _si = esi, _bx = ebx; \
        static int n = 0; int log = (n < 60); n++; \
        if (log) {   } \
        name##_gen(); \
        edi = _di; esi = _si; ebx = _bx; \
        if (log) {   } \
    }
#define ABI_MARK(name) \
    void name##_gen(void); \
    void name(void) { \
        uint32_t _di = edi, _si = esi, _bx = ebx; \
        name##_gen(); \
        edi = _di; esi = _si; ebx = _bx; \
    }
BOOT_MARK2(sub_000833E0)         /* boot task stage driver (enter/exit) */
/* boot task body (enter/exit) + arms the g_ebx data breakpoint (DOA3_EBXWP=1)
 * across its one-time init chain; sub_00084340_gen disarms it at loop top. */
void sub_00084340(void) {
    extern void sub_00084340_gen(void);
    extern void doa3_ebxwp_arm(void);
    static int n = 0;
    int log = (n < 60); n++;
    if (log) {   }
    doa3_ebxwp_arm();
    sub_00084340_gen();
    if (log) {   }
}
/* sub_00084340's one-time init chain, between the intro sequencer and the
 * screen loop at 0x00084430. The loop calls sub_000821B0 unconditionally on
 * every iteration and that never happens, while sub_00084340 has entered and
 * not exited -- so one of these does not return and the screen dispatcher
 * never starts. */
/* sub_00082060's callees: it enters and never returns. */
BOOT_MARK2(sub_00050940)
BOOT_MARK2(sub_0009EF10)
BOOT_MARK2(sub_0009F540)
BOOT_MARK2(sub_0009F010)
BOOT_MARK2(sub_0007F790)
BOOT_MARK2(sub_0007FE50)
BOOT_MARK2(sub_0007E510)
BOOT_MARK2(sub_0009DB30)
BOOT_MARK2(sub_0009D8F0)
BOOT_MARK2(sub_0007E300)
/* sub_0006AD20 is a wait loop: it polls sub_0006E050 and only returns when
 * that reports 0. Post-movie it never does, which blocks sub_00084340 before
 * its screen loop ever starts, so sub_000821B0 never runs and the title
 * screen is never created. Report the poll result together with the load-op
 * the [LDT] diag shows parked at status 2. */
void sub_0006E050_gen(void);
void sub_0006E050(void) {
    sub_0006E050_gen();
    {   static DWORD s_next = 0; static int s_n = 0;
        DWORD now = GetTickCount();
        if (s_n < 30 && now >= s_next) {
            uint32_t ph = MEM32(0x4A1004);
            uint32_t op = (ph < 8) ? MEM32(ph * 4 + 0x4A10A8) : 0;
            s_next = now + 1000; s_n++;
        }
    }
}
BOOT_MARK2(sub_00080200)
BOOT_MARK2(sub_00050060)
BOOT_MARK2(sub_00082E90)
BOOT_MARK2(sub_0009E482)
BOOT_MARK2(sub_00050995)
BOOT_MARK2(sub_00077580)
BOOT_MARK2(sub_0007CC10)
BOOT_MARK2(sub_00076FC0)
BOOT_MARK2(sub_000D5700)
/* Load-wait exit: what did the resource loader actually leave in the
 * destination? id/dest are the request globals (sub_0007E4A0). */
void sub_0007E860_gen(void);
void sub_0007E860(void) {
    uint32_t _di = edi, _si = esi, _bx = ebx;
    static int n = 0; int log = (n < 60); n++;
    if (log) {   }
    sub_0007E860_gen();
    edi = _di; esi = _si; ebx = _bx;
    if (log) {
        uint32_t dst = MEM32(0x4A212C); int k;
    }
}
ABI_MARK2(sub_0007E9C0)
ABI_MARK2(sub_0007E8C0)
BOOT_MARK2(sub_00076FE8)
BOOT_MARK2(sub_00076FF6)
BOOT_MARK2(sub_0006F6F0)
BOOT_MARK2(sub_00074470)
BOOT_MARK2(sub_00070A00)
BOOT_MARK2(sub_0007F2A0)
BOOT_MARK2(sub_0007F350)
BOOT_MARK2(sub_0007F410)
BOOT_MARK2(sub_00080BD0)
BOOT_MARK2(sub_00070130)
void sub_0007ED10_gen(void);
void sub_0007ED10(void) {
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    sub_0007ED10_gen();
    {   static DWORD nx = 0; static int n = 0; DWORD now = GetTickCount();
        if (n < 40 && now >= nx) { nx = now + 1000; n++;
              } }
}
ABI_MARK2(sub_00080C30)
ABI_MARK2(sub_0007E9FA)
ABI_MARK2(sub_0007EA0C)
ABI_MARK2(sub_000810A0)
ABI_MARK2(sub_00046B40)
ABI_MARK2(sub_001B4A30)
ABI_MARK2(sub_00080CC0)
ABI_MARK2(sub_0007DD40)
/* Make the resource loader synchronous again.
 *
 * sub_0007EA0C calls the loader through [0x4A10C8] and then immediately
 * parses the blob (sub_000810A0 -> sub_00080F10). On hardware that is safe:
 * the loader does not return until the data is resident. Here the CRI read is
 * queued and serviced by the file server, so the loader returns with the
 * destination buffer still zero -- and the parser is a tight loop with no
 * yield, so it never gives the pump a chance to service the read. The result
 * is a deadlock on an all-zero blob, which is what keeps the screen
 * dispatcher out of its loop and the title screen off the screen.
 *
 * Wait here instead, pumping the CRI file server (sub_00170710, which guards
 * its own re-entry) and yielding so the decode/IO fibers run. The load op is
 * the one the [LDT] diag tracks: table 0x4A10A8 indexed by the phase at
 * 0x4A1004, status byte at op+1, 3 = complete. Post-movie only: boot loads
 * already complete on their own and the movie timing is verified. */
static void doa3_wait_resource_load(void)
{
    extern volatile int g_doa3_post_movie;
    extern void sub_00170710(void);
    extern void xbox_fiber_yield(void);
    int spins;
    if (!g_doa3_post_movie) return;
    for (spins = 0; spins < 20000; spins++) {
        uint32_t ph = MEM32(0x4A1004);
        uint32_t op = (ph < 8u) ? MEM32(ph * 4 + 0x4A10A8) : 0;
        if (!op) break;                              /* nothing outstanding */
        if ((int)(int8_t)MEM8(op + 1) == 3) break;   /* complete */
        { uint32_t sv = esp; PUSH32(esp, 0); sub_00170710(); esp = sv; }
        xbox_fiber_yield();
    }
    {   static int n = 0;
        if (n < 8) { n++;
            uint32_t ph = MEM32(0x4A1004);
            uint32_t op = (ph < 8u) ? MEM32(ph * 4 + 0x4A10A8) : 0;
             } }
}
void sub_00080020_gen(void);
void sub_00080020(void) {
    uint32_t _di = edi, _si = esi, _bx = ebx;
    sub_00080020_gen();
    edi = _di; esi = _si; ebx = _bx;
    doa3_wait_resource_load();
}
void sub_00080070_gen(void);
void sub_00080070(void) {
    uint32_t _di = edi, _si = esi, _bx = ebx;
    sub_00080070_gen();
    edi = _di; esi = _si; ebx = _bx;
    doa3_wait_resource_load();
}

ABI_MARK(sub_0007E870)
ABI_MARK(sub_0007E8A6)
void sub_0016CFB0_gen(void);
void sub_0016CFB0(void) {
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    char nm[80]; int k;
    for (k = 0; k < 79; k++) { nm[k] = (char)MEM8(a2 + k); if (!nm[k]) break; }
    nm[79] = 0;
    sub_0016CFB0_gen();
    {   static int n = 0;
        if (n < 20) { n++;
             } }
}
void sub_0016CFF0_gen(void);
void sub_0016CFF0(void) {
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    char nm[80]; int k;
    for (k = 0; k < 79; k++) { nm[k] = (char)MEM8(a2 + k); if (!nm[k]) break; }
    nm[79] = 0;
    sub_0016CFF0_gen();
    {   static int n = 0;
        if (n < 20) { n++;
             } }
}
BOOT_MARK2(sub_000833C0)
BOOT_MARK2(sub_0009EFC0)
BOOT_MARK2(sub_0009EEB0)
BOOT_MARK2(sub_0009EEE0)
BOOT_MARK2(sub_00082060)
BOOT_MARK2(sub_00081950)
BOOT_MARK2(sub_001770B0)   /* movie teardown chain (enter/exit) */
BOOT_MARK2(sub_0017D320)   /* movie teardown chain (enter/exit) */
/* sub_00175AB0 -> sub_00177900 -> sub_00170540(5): dispatches CRI callback
 * slot 5 through the table at 0xB25598 (fn at +0, arg at +4, stride 8).
 * The final movie teardown never returns from here. */
void sub_00175AB0_gen(void);
void sub_00175AB0(void) {
    static int n = 0;
    int log = (n < 6); n++;
    if (log) {
         }
    sub_00175AB0_gen();
    if (log) {   }
}
BOOT_MARK2(sub_00173E20)   /* movie teardown chain (enter/exit) */
BOOT_MARK2(sub_001778E0)   /* movie teardown chain (enter/exit) */
/* [STAND] the post-movie standoff.
 *
 * Screen-mode byte 0x480B70 == 2 dispatches (jump table 0x00084574,
 * case 2) to sub_00083920, which creates the post-movie screen task
 * 0x000CEF80 -- but only while MEM8(0x47ADB8) == 0. Task sub_00050C30
 * sets that latch via sub_00050160 whenever the mode is 2, and clears
 * it again only when sub_00050250() returns 0. While it stays 1 the
 * screen task is never created and the title screen is never armed. */
/* [JOIN] the gate that decides whether the attract loop hands over to the
 * title screen. sub_000821B0 runs every frame from the screen dispatcher
 * sub_00084340. At 0x000822C0 it requires the mode byte 0x480B70 == 2, then
 * rejects screen ids 1 and 7, then opens the change path at 0x0008243B when
 * either the connected-pad mask 0x5E5CC8 has the port's bit, or
 * sub_00081E90(port) reports a press. That path is what reaches
 * sub_000826D5, the only site that sets the mode byte to 0 and creates the
 * title-screen task 0x000CEF80. Sample the inputs to that decision. */
void sub_000821B0_gen(void);
void sub_000821B0(void) {
    {   static DWORD s_next = 0; static int s_n = 0;
        DWORD now = GetTickCount();
        if (s_n < 40 && now >= s_next) {
            s_next = now + 1000; s_n++;
        }
    }
    sub_000821B0_gen();
}
void sub_00050250_gen(void);
void sub_00050250(void) {
    static unsigned n = 0, shown = 0;
    sub_00050250_gen();
    /* [SCRSTATE] the post-movie screen state machine, sampled over time.
     * mode  = 0x480B70 (main jump table 0x00084574: 0 title, 2 post-movie)
     * latch = 0x47ADB8 (screen busy; blocks sub_00083920)
     * scr   = 0x48A2FA (screen id; 1 or 7 unlock the change path at
     *                   0x0008243B, 0 exits early)
     * req   = 0x48A528 (screen request for sub_000821B0)
     * chg   = 0x484C32 / 0x47E722 (change registered -> clears the latch)
     * pads  = 0x5E5CC8 connection mask, agg = pad0 button aggregate */
    /* Sampled on a wall clock, not a call count: the post-movie loop runs at
     * only a few hertz, so "every 400th call" yielded a single line for a
     * whole run and made a moving state look frozen. */
    n++;
    {   static DWORD s_next = 0;
        DWORD now = GetTickCount();
        if (now < s_next || shown >= 300) return;
        s_next = now + 2000;
        shown++;
        /* 0x0049231C bit 3 = "pad rumble running" (set 0x00067342, cleared
         * 0x000672B0/0x00067350). It is the last thing holding the screen
         * latch shut, so show the rumble record it is derived from:
         * 0x004920E0 + pad*0x2C, +0x24 elapsed vs +0x28 duration, where the
         * duration is (0x004A0D94 + 1) * 0xF0. */
        fprintf(stderr, "          rumble cfg4A0D94=%d dur=(%d,%d) elapsed=(%d,%d) "
                        "act=(%u,%u)\n",
                (int)MEM32(0x4A0D94),
                (int)MEM32(0x4920E0 + 0x28), (int)MEM32(0x4920E0 + 0x2C + 0x28),
                (int)MEM32(0x4920E0 + 0x24), (int)MEM32(0x4920E0 + 0x2C + 0x24),
                MEM8(0x4920E0 + 0x20), MEM8(0x4920E0 + 0x2C + 0x20));
    }
}
void sub_00050160_gen(void);
void sub_00050160(void) {
    static int n = 0;
    if (n < 6) { n++;
          }
    sub_00050160_gen();
}
void sub_00083A90_gen(void);
void sub_00083A90(void) {
    static int n = 0;
    if (n < 4) { n++;
         }
    sub_00083A90_gen();
}
void sub_00067220_gen(void);
void sub_00067220(void) {
    static int n = 0;
    sub_00067220_gen();
    if (n < 8) { n++;
         }
}
/* Set once the movie teardown has returned -- i.e. the game has genuinely
 * left the intro movie. The frame-capture diagnostic keys off this. */
uint32_t g_blk50380;   /* last basic block entered in sub_00050380 */
uint32_t g_blkC5D70;   /* last basic block entered in sub_000C5D70 */
unsigned g_c5d70cnt[16];  /* decision-block hit counts in sub_000C5D70 */
volatile int g_doa3_post_movie = 0;
volatile int g_doa3_in_pump = 0;   /* doa3_pump_cri_servers is on the stack */
/* May the primary fiber hand a time slice to the worker threads right now?
 * Only after the movie (its verified timing is left alone), never from
 * inside the CRI server pump, and not while the CRI lock is held or a CRI
 * server is on the stack -- the workers would re-enter them. */
int doa3_workers_may_run(void)
{
    return g_doa3_post_movie && !g_doa3_in_pump && MEM32(0xB24D38) == 0 && MEM32(0xC0E384) == 0;
}

/* [FLOWCNT] which links of the title-screen chain actually execute.
 * mode 0 (sub_00083A90) -> mode 1 (sub_00083BC0 sets 0x48A39C) ->
 * sub_000CF500/sub_000C6E30 populate the player slots ->
 * sub_000C5D70 -> sub_000C4D00 advances the screen state byte ->
 * state >= 10 -> sub_000CEB20 sets 0x47E74C = 1 -> title screen. */
unsigned g_fc[16];
void sub_000D02A0_gen(void);
void sub_000D02A0(void) { g_fc[8]++; sub_000D02A0_gen(); }
void sub_000CEAC0_gen(void);
void sub_000CEAC0(void) { g_fc[9]++; sub_000CEAC0_gen(); }
void sub_000CDCD0_gen(void);
void sub_000CDCD0(void) { g_fc[10]++; sub_000CDCD0_gen(); }
void sub_00083BC0_gen(void);
void sub_00083BC0(void) { g_fc[0]++; sub_00083BC0_gen(); }
void sub_00082950_gen(void);
void sub_00082950(void) { g_fc[1]++; sub_00082950_gen(); }
void sub_000C5D70_gen(void);
void sub_000C5D70(void) {
    /* Does the consumer ever run on a frame where the producer had an edge to
     * publish? 0x5E5EE0 is the edge word sub_000CDB90 derives the 0x220 flag
     * from; 0x86132A is the flag the first bail tests. Sampled at entry, i.e.
     * immediately after sub_000CEAC0 ran earlier in sub_000C6890. */
    extern unsigned g_c5d70cnt[16];
    if (MEM32(0x5E5EE0)) g_c5d70cnt[11]++;          /* edge live at entry */
    if (MEM16(0x86132A) & 0x220) g_c5d70cnt[12]++;  /* flag actually set */
    if (MEM32(0x5E5ED8)) g_c5d70cnt[13]++;          /* held word live */
    g_fc[2]++;
    sub_000C5D70_gen();
}
void sub_000C4D00_gen(void);
void sub_000C4D00(void) { g_fc[3]++; sub_000C4D00_gen(); }
void sub_00082F10_gen(void);
void sub_00082F10(void) { g_fc[4]++; sub_00082F10_gen(); }
void sub_000CE7D0_gen(void);
void sub_000CE7D0(void) { g_fc[5]++; sub_000CE7D0_gen(); }
void sub_000CF500_gen(void);
void sub_000CF500(void) { g_fc[6]++; sub_000CF500_gen(); }
void sub_000C6E30_gen(void);
void sub_000C6E30(void) { g_fc[7]++; sub_000C6E30_gen(); }
void doa3_flowcount_dump(void)
{
}


/* D3DDevice_SetTransform (0x001B0EC0) and D3DDevice_SetViewport (0x001B18A0)
 * and D3D_UpdateProjectionViewportTransform (0x001B5FD0), from the cxbx symbol
 * cache. DIAGNOSTIC WRAPPERS (logging only).
 *
 * The composite matrix the GPU receives is all zeros because the driver builds
 * it as projection-viewport (device+0x5A0) x view (device+0x880), and both of
 * those are garbage: +0x5A0 is never written at all (only
 * D3D_UpdateProjectionViewportTransform writes it) and +0x880 comes back as
 * mostly zeros from SetTransform. Log what the game actually hands the driver
 * so it is clear whether the game computes a bad matrix or the driver loses
 * a good one. */
void doa3_dump_mat4(const char *tag, uint32_t p)
{
    int i;
    for (i = 0; i < 16; i++) {
        float f; uint32_t u = MEM32(p + i * 4);
        memcpy(&f, &u, 4);
    }
}
/* sub_001BA7D8 -- called by D3DDevice_CreateDevice (sub_001B9537) right before
 * it hands the implicit back buffer to SetRenderTarget:
 *
 *     0x1B9949  mov eax, [esp+4]        ; the device, from a frame local
 *     0x1B9962  add eax, 0x2150         ; -> the implicit colour surface
 *     0x1B9968  call SetRenderTarget
 *
 * SetRenderTarget received 0xEFFF222E instead of device+0x2150, i.e. that
 * frame-local read came back garbage, so the device has held a wild render
 * target pointer ever since. SetViewport clamps every viewport against that
 * pointer, which is why 720x480 in gives width 0 out, why the
 * projection-viewport matrix stays all zeros, and why every post-movie vertex
 * collapses onto one point.
 *
 * This function takes seven stack arguments and returns with `ret 0x1C`.
 * Enforce that contract (and the callee-saved registers) so the caller frame
 * it returns to is the one it left.  Delta is logged for the first few calls. */
void sub_001BA7D8_gen(void);
void sub_001BA7D8(void)
{
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001BA7D8_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    {   static int s_n = 0;
        if (s_n < 4) { s_n++;
             } }
    esp = ei + 4 + 0x1C;          /* dummy return slot + ret 0x1C */
}



/* D3DDevice_SetRenderTarget (0x001B1350). DIAGNOSTIC WRAPPER.
 *
 * The device keeps the current colour surface at +0x40C, and SetViewport
 * clamps the incoming viewport against that surface size. It reads back as
 * 0xEFFF222E -- a guest stack address, not a surface -- so every viewport the
 * game sets is clamped to nothing (720x480 in, b08=0 b0c=0 out), which zeroes
 * the projection-viewport matrix and with it every transformed vertex. The
 * only writer is this function (0x001B143A stores EBP), and EBP is held
 * across two calls, so log the argument and the result to tell a bad argument
 * from a clobbered register. */
void sub_001B1350_gen(void);
/* Render-target identity for the pgraph translator: the implicit back buffer
 * surface lives inside the device object (dev+0x2150 / dev+0x2168); anything
 * else the game hands SetRenderTarget is a texture surface (the reflection
 * target 0x04002E50 in stage 0x3F). Both have the same 720x480 pitch, so
 * the translator cannot tell them apart from surface registers alone; it
 * keys on the colour offsets recorded here (D3DResource.Data & 0x03FFFFFF,
 * which is what SET_SURFACE_COLOR_OFFSET carries). */
uint32_t g_doa3_fb_offs[4]; int g_doa3_fb_n;
uint32_t g_doa3_offrt_offs[8]; int g_doa3_offrt_n;   /* every texture surface seen */

/* The guest's own D3DPRESENT_PARAMETERS (0x0090F4A8): the DISPLAY resolution,
 * 720x480, which is the space every 2D overlay coordinate the game submits is
 * expressed in.  It is NOT the size of the colour surface -- on character
 * select the game turns on 2x2 supersampling (D3DRS_MultiSampleType 0x2022)
 * and the frame buffer becomes 1440x960, while the overlay quads keep coming
 * in at 0..720 / 0..480.  Anything mapping overlay coordinates onto the host
 * back buffer has to divide by this, not by SET_SURFACE_CLIP.
 * Returns 0 and leaves the outputs alone if the fields are not plausible
 * (before CreateDevice they are zero). */
int doa3_guest_display_size(unsigned *w, unsigned *h)
{
    uint32_t dw = MEM32(0x0090F4A8u), dh = MEM32(0x0090F4ACu);
    if (dw < 64u || dw > 4096u || dh < 64u || dh > 4096u) return 0;
    *w = (unsigned)dw; *h = (unsigned)dh;
    return 1;
}
void sub_001B1350(void)
{
    uint32_t arg = MEM32(esp + 4);
    sub_001B1350_gen();
    if (arg >= 0x1000 && arg < 0x08000000u) {
        uint32_t d = MEM32(0x001C3390), data = MEM32(arg + 4) & 0x07FFFFFFu; /* DOA3: 27-bit, see XBOX_LOW_END */
        if (arg == d + 0x2150 || arg == d + 0x2168) {
            int k, seen = 0;
            for (k = 0; k < g_doa3_fb_n; k++) if (g_doa3_fb_offs[k] == data) seen = 1;
            if (!seen && g_doa3_fb_n < 4) g_doa3_fb_offs[g_doa3_fb_n++] = data;
        } else {
            int k, seen = 0;
            for (k = 0; k < g_doa3_offrt_n; k++) if (g_doa3_offrt_offs[k] == data) seen = 1;
            if (!seen && g_doa3_offrt_n < 8) g_doa3_offrt_offs[g_doa3_offrt_n++] = data;
        }
    }
    {   /* DOA3 DIAG: per-frame render-target switch trace (post-movie). */
        extern volatile int g_doa3_post_movie; extern volatile LONG g_doa3_heartbeat;
        static int s_t = 0;
        if (g_doa3_post_movie && s_t < 200) { s_t++;
              }
    }
    {   static int s_n = 0;
        if (s_n < 20) { s_n++;
            uint32_t d = MEM32(0x001C3390);
            void *bt[8]; int nb = (int)CaptureStackBackTrace(1, 8, bt, NULL), bi;
            (void)bt; (void)nb; (void)bi;
            /* The tail of this function (loc_001B1562) resets the viewport to
             * the render-target size -- the only route to a real viewport, and
             * so to a non-zero projection-viewport and composite matrix. Show
             * the target and the implicit surface descriptor it sizes from. */
            fprintf(stderr, "[SETRT] arg=%08X -> dev+40C=%08X | surf %08X: "
                            "[0]=%08X [C]=%08X [10]=%08X\n",
                    arg, MEM32(d + 0x40C), d + 0x2150,
                    MEM32(d + 0x2150), MEM32(d + 0x2150 + 0xC),
                    MEM32(d + 0x2150 + 0x10));
             } }
}


void sub_001B18A0_gen(void);
void sub_001B18A0(void)
{
    uint32_t vp = MEM32(esp + 4);
    uint32_t x = MEM32(vp), y = MEM32(vp + 4);
    uint32_t w = MEM32(vp + 8), h = MEM32(vp + 0xC);
    float zn, zf; uint32_t u;
    u = MEM32(vp + 0x10); memcpy(&zn, &u, 4);
    u = MEM32(vp + 0x14); memcpy(&zf, &u, 4);
    sub_001B18A0_gen();
    {   static int s_n = 0;
        extern volatile int g_doa3_post_movie;
        static int s_pm = 0;
        if (s_n < 40 || (g_doa3_post_movie && s_pm < 12)) {
            if (s_n < 40) s_n++; else s_pm++;
            uint32_t d = MEM32(0x001C3390);
             } }
}
void sub_001B5FD0_gen(void);
void sub_001B5FD0(void)
{
    sub_001B5FD0_gen();
    {   static int s_n = 0;
        extern volatile int g_doa3_post_movie;
        static int s_pm = 0;
        if (s_n < 12 || (g_doa3_post_movie && s_pm < 12)) {
            if (s_n < 12) s_n++; else s_pm++;
            uint32_t d = MEM32(0x001C3390);
            float f4ec, f4f0, f4f4, f4f8, f500, f504;
            uint32_t u;
            u = MEM32(d + 0x4EC); memcpy(&f4ec, &u, 4);
            u = MEM32(d + 0x4F0); memcpy(&f4f0, &u, 4);
            u = MEM32(d + 0x4F4); memcpy(&f4f4, &u, 4);
            u = MEM32(d + 0x4F8); memcpy(&f4f8, &u, 4);
            u = MEM32(d + 0x500); memcpy(&f500, &u, 4);
            u = MEM32(d + 0x504); memcpy(&f504, &u, 4);
            doa3_dump_mat4("proj+0x8C0", d + 0x8C0);
            doa3_dump_mat4("pv+0x5A0", d + 0x5A0);
              } }
}


/* sub_001B7E50 -- the D3D8 driver 4x4 matrix multiply (pure SSE) that builds
 * the model-view and composite matrices it streams to the GPU.
 *
 * DIAGNOSTIC WRAPPER (logging only; the generated body still does the work).
 * The pgraph translator sees SET_COMPOSITE_MATRIX arrive as 16 zero dwords
 * and SET_MODEL_VIEW_MATRIX as a matrix with two zero rows, which collapses
 * every post-movie draw onto the viewport centre. Print the operands and the
 * result so it is clear whether the multiply is wrong or its inputs are.
 * Args (cdecl): [esp+4]=dst, [esp+8]=A, [esp+0xC]=B; ret 12. */
void sub_001B7E50_gen(void);
void sub_001B7E50(void)
{
    uint32_t dst = MEM32(esp + 4), a = MEM32(esp + 8), b = MEM32(esp + 0xC);
    sub_001B7E50_gen();
    {   extern volatile int g_doa3_post_movie;
        static int s_n = 0;
        int nan_r = 0;
        {   /* The composite matrix reaches the GPU as 16 x 0xFFC00000 (x87
             * indefinite). Every matrix the driver streams comes out of this
             * multiply, so alarm on the first products that produce a NaN and
             * show both operands -- that names the matrix that is already
             * bad rather than the one that merely carries it. */
            int i;
            for (i = 0; i < 16; i++) if (MEM32(dst + i * 4) == 0xFFC00000u) { nan_r = 1; break; }
        }
        if (nan_r) {
            static int s_nan = 0;
            if (s_nan < 12) { s_nan++;
                uint32_t d = MEM32(0x001C3390);
                doa3_dump_mat4(" A", a); doa3_dump_mat4(" B", b);
                  }
        }
        if (g_doa3_post_movie && s_n < 24) { s_n++;
            int i;
            for (i = 0; i < 16; i++) { float f; uint32_t u = MEM32(a + i * 4);
                memcpy(&f, &u, 4);  }
            for (i = 0; i < 16; i++) { float f; uint32_t u = MEM32(b + i * 4);
                memcpy(&f, &u, 4);  }
            for (i = 0; i < 16; i++) { float f; uint32_t u = MEM32(dst + i * 4);
                memcpy(&f, &u, 4);  }
             }
    }
}


/* CRI callback dispatch: enforce the callee-saved ABI.
 *
 * sub_001705E0 walks a table of registered callbacks and invokes each
 * through an indirect call, having pushed ebx/ebp/esi/edi on entry and
 * popping them on exit -- so on hardware a caller's edi survives it.
 * Here it does not: a callback corrupts the guest stack slots holding
 * those saved registers, and the pops read live data back instead
 * (observed: edi 0x00000001 -> 0x00000000, then 0x00C0F7C0, 0x001C0800).
 *
 * That single lost register is what deadlocked the movie teardown. The
 * CRI server loop at 0x0016A650 holds its sentinel in edi (edi = 1) and
 * clears the shutdown-ack flag only when [0xB24D3C] == edi. With edi
 * corrupted the compare never matched, so the ack was never given and
 * the teardown wait (sub_0016A4C0) spun its full 200,000,000 iterations
 * -- the game hung between the intro movie and the post-movie screen.
 * The server was running the whole time (its heartbeat 0xB24D48 climbed
 * past 700k during the stall); it simply never took the clear branch.
 *
 * Snapshot and restore in C, which is what the guest stack was meant to
 * do and cannot be trusted to here. Same remedy as ESP_PROBE above. */
void sub_001705E0_gen(void);
void sub_001705E0(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001705E0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* sub_000E00D0 -- same callee-saved leak, at the post-movie load dispatch.
 *
 * sub_000E8BB0 drives 175 objects at 0x370C48 (stride 0x3C) through a
 * vtable call, holding the base in EDI and the counter in EBX across it:
 *
 *     mov edi, 0x370c48 / mov ebx, 0xaf
 *   loop: mov edx,[edi] / push 0x3736c0 / mov ecx,edi / call [edx+4]
 *         add edi, 0x3c / dec ebx / jne loop
 *
 * The vtable target is sub_000E00D0, whose own callees (sub_000DF360 and
 * friends) were split by the disassembler so their push/pop pairs straddle
 * fragment boundaries -- sub_000DF360 holds the pushes and its epilogue
 * fragment sub_000DF443 holds the pops. EBX therefore came back holding
 * the pushed "this" (0x3736C0) instead of the loop counter, so the loop
 * ran ~3.6M times and walked EDI off the end of the array. That produced
 * the wild reads and the ~200 null vtable calls seen at sub_000E8BB0,
 * and ultimately an out-of-range APU voice index.
 *
 * A blanket save/restore inside RECOMP_ICALL is NOT the fix: too many
 * mis-split fragments are registered as icall targets, and forcing the
 * guarantee on all 1592 sites hangs the CRI server (sub_0017CC30 spins
 * forever). Restore the ABI at this one real call boundary instead. */
void sub_000E00D0_gen(void);
void sub_000E00D0(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_000E00D0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* sub_0006D140 -- third instance of the same callee-saved leak.
 *
 * sub_00051800 walks a 2-entry table with the base in ESI and the count
 * in EDI held across the call:
 *
 *     mov esi, 0x3057f4 / mov edi, 2
 *   loop: push 0 / push esi / call sub_0006D140 / add esp, 8
 *         add esi, 8 / dec edi / jne loop
 *
 * sub_0006D140 itself never touches ESI/EDI, but sub_0006CDB0 below it
 * pushes them lazily at 0x6CDCA/0x6CDCD and the disassembler split that
 * body at 0x6CDC1, so the pushes and pops now live in different C
 * functions (sub_0006CDC1 shows push1/pop2 for both). The loop therefore
 * lost its base and its counter: ESI wandered into the output element
 * array at 0x49A998 and the two-iteration loop ran 106 million times,
 * feeding float data (0x3F800000, 0xC5000000) to sub_0006D140 as the
 * record whose first dword becomes the table index at 0x49A8E4. That is
 * what sub_00069710 then read as [edi] and used to index 0x2FD668 /
 * 0x48F288, producing its wild reads. */
void sub_0006D140_gen(void);
void sub_0006D140(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0006D140_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* The three sibling sprite helpers have the same exposure and the same
 * callers. sub_00051800 drives sub_0006D410 from a second loop with the
 * identical shape (esi = 0x305804, edi = 2, stride 0xC), and all three
 * bottom out in the split bodies of sub_0006CDB0 / sub_0006CE40 /
 * sub_0006CED0 whose lazy push esi/edi is separated from its pop. */

void sub_0006D1B0_gen(void);
void sub_0006D1B0(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0006D1B0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

void sub_0006D230_gen(void);
void sub_0006D230(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0006D230_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

void sub_0006D410_gen(void);
void sub_0006D410(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0006D410_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* DOA3 DIAG: sub_00157700 (vertex-block walker, this in ecx) ring of the
 * last entries -- dumped by the VEH when a wild read fires inside it, so the
 * object whose block chain is garbage is identified at the fault. */
struct doa3_vbw_ent { uint32_t self, f4, f8, l50, l54, l58, l5c, l60, blk, blk68, seq; };
struct doa3_vbw_ent g_vbw_ring[8]; unsigned g_vbw_seq = 0;
void doa3_vbw_dump(void)
{
    unsigned i;
    for (i = 0; i < 8; i++) {
        struct doa3_vbw_ent *e = &g_vbw_ring[i];
        if (!e->seq) continue;
    }
}
void sub_00157700_gen(void);
void sub_00157700(void) {
    uint32_t t = ecx;
    struct doa3_vbw_ent *e = &g_vbw_ring[g_vbw_seq & 7];
    int ok = (t >= 0x1000 && t < 0x08000000u);
    e->seq = ++g_vbw_seq; e->self = t;
    e->f4 = ok ? MEM32(t + 4) : 0; e->f8 = ok ? MEM32(t + 8) : 0;
    e->l50 = ok ? MEM32(t + 0x50) : 0; e->l54 = ok ? MEM32(t + 0x54) : 0;
    e->l58 = ok ? MEM32(t + 0x58) : 0; e->l5c = ok ? MEM32(t + 0x5C) : 0;
    e->l60 = ok ? MEM32(t + 0x60) : 0;
    e->blk = ok ? (t + 0x60) : 0;          /* ebx after `lea ebx,[esi+0x50]; add ebx,0x10` */
    e->blk68 = ok ? MEM32(t + 0x60 + 0x68) : 0;
    {   /* DOA3 DIAG: the walker's own callee-saved/esp contract (ret 0, this in ecx). */
        uint32_t sb = ebx, ss = esi, sd = edi, sp = esp;
        static int n = 0;
        static LARGE_INTEGER s_tf = {0}; static LONGLONG s_tot = 0, s_max = 0; static unsigned s_calls = 0; static DWORD s_next = 0;
        LARGE_INTEGER t0, t1;
        uint32_t in_esi = esi, in_idx = (esi > 0x1000 && esi < 0x8000000u) ? MEM16(esi) : 0xFFFF, in_tbl = MEM32(esp + 0x20);
        uint32_t in_slot = (in_tbl > 0x1000 && in_tbl < 0x8000000u && in_idx != 0xFFFF) ? MEM32(in_tbl + in_idx * 4) : 0;
        if (!s_tf.QuadPart) QueryPerformanceFrequency(&s_tf);
        QueryPerformanceCounter(&t0);
        sub_00157700_gen();
        QueryPerformanceCounter(&t1);
        {   LONGLONG dt = t1.QuadPart - t0.QuadPart; s_tot += dt; if (dt > s_max) s_max = dt; s_calls++;
            if (dt * 1000 > 50 * s_tf.QuadPart) { static int slow_n = 0; if (slow_n < 12) { slow_n++;
                uint32_t hb = ok ? MEM32(t + 0x60 + 0x68) : 0, cnt = ok ? MEM32(t + 0x60 + 0x6C + (hb << 4)) : 0;
                
                
                { void *bt[12]; USHORT nb = CaptureStackBackTrace(0, 12, bt, NULL); int k; 
                  for (k = 0; k < nb; k++) ;  }
                 } }
            if (GetTickCount() >= s_next) { s_next = GetTickCount() + 2000;
                 s_tot = s_max = 0; s_calls = 0; } }
        if ((ebx != sb || esi != ss || edi != sd || esp != sp) && n < 30) { n++;
             }
    }
}
/* sub_001B3940 -- the same callee-saved leak on the D3D8 inline-vertex path,
 * and by far the most expensive instance of it.
 *
 * sub_00157700 walks a list of vertex blocks, accumulating a vertex count in
 * EDI and holding the block pointer in EBX across the submit:
 *
 *     0x1578A6: xor edi, edi                    ; per-block count
 *     0x1578B0: ... accumulate edi from [ebx] ... add ebx, 0x10
 *     0x1578FD: add eax, 2 / push eax           ; count = edi
 *     0x157903: call sub_001B3940
 *     0x157908: mov esi, [ebx]                  ; next block -- EBX must survive
 *
 * sub_001B3940 bottoms out in mis-split XDK bodies whose push/pop pairs
 * straddle fragment boundaries (sub_001B7700 shows push1/pop2 for EBX, ESI
 * and EDI). EBX came back wrong, so [ebx] gave a garbage block count and the
 * next iteration built a garbage vertex count -- 0x0EB1927B, 0x96B8CB08. The
 * MMX copy loop then ran for hundreds of millions of vertices and walked its
 * source pointer into the NV2A MMIO aperture at 0xFDxxxxxx. Every one of
 * those reads trapped through the VEH: the fault counter passed 54,000,000 in
 * a single run, and the sampling profiler put ~72% of all CPU inside this
 * function with another ~15% in ntdll dispatching the exceptions -- which is
 * why the game ran far too slowly to reach the title screen.
 *
 * (sub_001B2520, the other callee whose result feeds this call site, is
 * already ABI-enforced through ESP_PROBE above.) */
void doa3_pb_tss_marker(void);
void sub_001B3940_gen(void);
void sub_001B3940(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    doa3_pb_tss_marker();
    static unsigned draw_probe_count = 0;
    if (draw_probe_count < 8 || (MEM32(esp + 8) > 0x100000 && draw_probe_count < 16)) {
        draw_probe_count++;
    }
    {   /* DOA3 DIAG: the list is valid when the walk starts (rt168) but the
         * walk still lands on a non-record word, so watch it change from
         * inside.  ebx is the walker's record cursor at this call site. */
        extern uint32_t g_walk_start, g_walk_end, g_walk_canary;
        extern int g_walk_active;
        extern volatile int g_doa3_post_movie;
        extern volatile LONG g_doa3_heartbeat;
        static int s_n = 0;
        if (g_doa3_post_movie && g_walk_active && s_n < 10 &&
            ebx >= g_walk_start && ebx < g_walk_end) {
            uint32_t wp  = MEM32(0x00B1F390u);
            uint32_t can = (g_walk_end > g_walk_start + 0x14u) ? MEM32(g_walk_end - 0x14u) : g_walk_canary;
            uint32_t nxt = MEM32(ebx + 0x14u);
            const char *why = (wp < g_walk_end)      ? "write-ptr moved back" :
                              (can != g_walk_canary) ? "canary changed" :
                              (nxt > 2 && ebx + 0x14u < g_walk_end) ? "next record type bad" : NULL;
            if (why) {
                void *bt[14]; USHORT nf = CaptureStackBackTrace(1, 14, bt, NULL); USHORT k;
                s_n++;
            }
        }
    }
    {   /* DOA3 DIAG: this routine reserves push-buffer space and rep-movsd's
         * into it.  The render list goes bad during this call (canary), so
         * report whenever the destination is not inside our RAM push buffer. */
        extern uint32_t g_doa3_pb_base, g_doa3_pb_end;
        extern volatile int g_doa3_post_movie;
        extern volatile LONG g_doa3_heartbeat;
        static int told = 0;
        uint32_t cur = MEM32(0x001C0800u);
        uint32_t lim = MEM32(0x001C0804u);
        sub_001B3940_gen();
        if (g_doa3_post_movie && told < 10 && g_doa3_pb_base &&
            (cur < g_doa3_pb_base || cur >= g_doa3_pb_end)) { told++;
             }
    }
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* sub_001B0EC0 -- the leak that produced the garbage vertex counts.
 *
 * sub_00157700 keeps its "this" in ESI and derives the vertex-block list
 * from it only AFTER calling this function:
 *
 *     0x157713: mov esi, ecx           ; this
 *     0x157788: call sub_001B0EC0      ; ESI must survive
 *     0x15778D: lea ebx, [esi + 0x50]  ; block list base
 *     0x15779F: mov eax, [ebx + 0x10]
 *
 * The chain under sub_001B0EC0 straddles fragment boundaries the same way
 * the others do (sub_001B44F0 push2/pop1 on ESI, sub_001B5FD0 writes ESI
 * with no pop), so ESI came back wrong and EBX pointed at nothing. Every
 * per-block count the loop then read out of [ebx+8] was garbage -- the same
 * values every run, because the leaked pointer is deterministic -- and the
 * accumulated total handed to sub_001B3940 reached 0x0EB1927B / 0x96B8CB08.
 * That is what drove the MMX copy loop off the end of its source buffer and
 * into the NV2A MMIO aperture, costing tens of millions of VEH faults. */
void sub_001B0EC0_gen(void);
void sub_001B0EC0(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    {   /* DIAG: what matrix does the game actually set?
         * The boot calls are all identity; the ones that matter are the
         * post-movie ones, which is where the device copy at device+0x880
         * comes back with two zero rows. */
        extern volatile int g_doa3_post_movie;
        static int s_n = 0, s_pm = 0;
        int want = (s_n < 8) || (g_doa3_post_movie && s_pm < 24);
        {   /* The projection at device+0x8C0 reaches the composite multiply
             * with inf in m00/m11, which is what turns the whole composite
             * matrix into 0xFFC00000. Report any non-finite matrix the game
             * hands SetTransform, with the guest call chain, so it is clear
             * whether the game computes it or the driver loses it. */
            static int s_bad = 0;
            uint32_t m = MEM32(esp + 8);
            int i, bad = 0;
            if (m >= 0x1000 && m < 0x08000000u)
                for (i = 0; i < 16; i++) {
                    uint32_t u = MEM32(m + i * 4);
                    if ((u & 0x7F800000u) == 0x7F800000u) { bad = 1; break; }
                }
            if (bad && s_bad < 12) { s_bad++;
                void *bt[10]; USHORT nb = CaptureStackBackTrace(1, 10, bt, NULL); int k;
                extern void doa3_dump_mat4(const char *tag, uint32_t p);
                doa3_dump_mat4("M", m);
            }
        }
        if (want) { if (s_n < 8) s_n++; else s_pm++;
            extern void doa3_dump_mat4(const char *tag, uint32_t p);
            doa3_dump_mat4("M", MEM32(esp + 8));
              } }
    sub_001B0EC0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* sub_001568B0 -- the leak behind the garbage vertex counts.
 *
 * sub_00157700 reads the entry count of a vertex-block group into ESI and
 * only then makes a call, using both ESI and EBX afterwards:
 *
 *     0x157894: mov  esi, [ebx]      ; number of 0x10-byte entries
 *     0x157896: call sub_001568B0    ; ESI and EBX must survive
 *     0x15789B: add  ebx, 8
 *     0x15789E: test esi, esi        ; loop bound
 *     0x1578A6: ...accumulate [ebx+8] over ESI entries...
 *
 * The chain below it writes all three registers without restoring them
 * (sub_001B1EA0, sub_001B4230 and sub_001B45F0 each show w,no-pop), so the
 * entry count came back too large and the accumulation loop ran off the end
 * of the entry array into the float vertex data that follows it -- the block
 * dump shows [ebx+8] holding values like 0xC3A9CE00 (-339.6f). Those floats
 * were summed as vertex counts, producing the 0x0EB1927B / 0x96B8CB08 totals
 * handed to sub_001B3940, which then copied for hundreds of millions of
 * vertices and walked its source into the NV2A MMIO aperture. Same values
 * every run, because the leaked register is deterministic. */
void sub_001568B0_gen(void);
void sub_001568B0(void) {
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001568B0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* sub_00156300 -- same leak, in sub_00157700's inner per-block loop.
 *
 *     0x157820: push esi / mov edx, ebx / mov ecx, edi
 *     0x157825: call sub_00156300      ; EBX, ESI and EDI must survive
 *     0x15782A: mov  eax, [esp+0x90]   ; loop bound
 *     0x157831: inc  esi / add ebx, 0x10 / cmp esi, eax / jb
 *
 * The callee is a 1139-byte body the disassembler carved into 13 fragments,
 * so its own push/pop pairs no longer balance (ebx push1/pop4, esi push6/pop4,
 * edi push1/pop4) and the loop lost both its block pointer and its counter.
 * sub_00156300 then re-entered with edx = ebx = a wild value and faulted on
 * its first "mov eax, [edi]" -- addresses like 0xEE2D7B8F, which sit between
 * guest RAM and the GPU aperture so the VEH could only skip them. That spin
 * was what drove the fault counter to the 2,000,000 skip cap and aborted the
 * process. */
void sub_00156300_gen(void);
void sub_00156300(void) {
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_00156300_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    /* Enforce the stack contract as well as the registers.
     *
     * sub_00157700 re-reads its loop bound from [esp+0x90] after every
     * one of these calls, so any esp drift makes it read a neighbouring
     * slot as the texture-stage count. That is how the loop came to run
     * to stage 0xC/0xE (the Xbox has four) and hand float data to
     * D3DDevice_SetTexture, which passed it on to the refcount walker
     * sub_001B4960 as a resource pointer -- the crash.
     *
     * The body is 1139 bytes split into 13 fragments, so its own
     * push/pop pairs no longer balance. It is "ret 4": one stack arg,
     * so esp_out = esp_in + 4 (our fake return slot) + 4. */
    esp = ei + 8;
}

/* [REFW] diagnostic: sub_001B4960 walks a D3D resource chain, bumping a
 * refcount in [obj] and recursing into the child at [obj+0x14]. It is the
 * last thing every run does before dying, with the guest esp walking down,
 * so log the argument and the recursion depth. */
void sub_001B4960_gen(void);
void sub_001B4960(void) {
    static int s_depth = 0, s_max = 0, s_logged = 0;
    uint32_t obj = MEM32(esp + 4);
    s_depth++;
    if (s_depth > s_max) {
        s_max = s_depth;
    }
    if ((obj < 0x1000u || obj >= 0x08000000u) && s_logged < 10) {
        s_logged++;
    }
    /* Containment: a D3D resource pointer is always a guest heap address.
     * We are reaching here with floats and small integers (0x3F800000,
     * 3, 4, 0x60) because SetTexture upstream is handed non-pointers out
     * of a malformed state block. Dereferencing one walks a bogus chain
     * and bumps a refcount through it, which is what killed every run.
     * Drop the call rather than the process; the producer is still wrong
     * and is the real fix. */
    /* Cycle guard. This walks a D3D resource chain via [obj+0x14] and the
     * guest marks a node visited (add [esi],0x80000) only AFTER recursing,
     * so a chain that points at itself is unbounded on hardware too --
     * it never happens there because the data is well formed. Here we hit
     * obj=0x002DD3D8 whose child IS itself, and the recursion ran the host
     * stack out right as the intro advanced past the movie. Real chains are
     * a couple of links deep, so cap the walk rather than die. */
    if (s_depth > 32) {
        static int s_warned = 0;
        if (!s_warned) { s_warned = 1;
             }
        s_depth--;
        esp += 8;   /* ret 4 */
        return;
    }
    if (obj < 0x1000u || obj >= 0x08000000u) {
        s_depth--;
        esp += 8;   /* ret 4: our fake return slot + one arg */
        return;
    }
    sub_001B4960_gen();
    s_depth--;
}

/* [INCHAIN] end-to-end input plumbing.
 *
 * XInputGetState fills the pad state; sub_0009EB90 folds the four pads
 * into the per-frame aggregates at 0x5E5ED8 (+0x2C per pad), whose
 * button word at +8 is what the post-movie screen tests for START
 * (sub_000821B0, 0x000822BB: test byte [0x5E5EE0], 0x10).
 *
 * Counters, so the chain can be checked headless: if the builder never
 * runs post-movie, no keypress could ever reach the game. */
unsigned g_in_getstate, g_in_build;
void sub_0009EB90_gen(void);
void sub_0009EB90(void) {
    g_in_build++;
    /* Netplay: the lockstep frame. Supplies both players' pads to the raw
     * slots this builder reads (no-op outside a session). */
    netplay_input_tick();
    sub_0009EB90_gen();
    {   /* Did a press actually reach the aggregate the post-movie screen
         * tests (sub_00081E90 reads bits 4-15 of 0x5E5EE0)? */
        static int n = 0;
        if ((MEM32(0x5E5EE0) || MEM32(0x5E5ED8)) && n < 12) { n++;
             }
    }
}
void sub_0009E340_gen(void);
void sub_0009E340(void) {
    static int n = 0;
    int log = (n < 4); n++;
    if (log) {   }
    {   /* The game leaves the movie here on both paths: end of stream, and
         * START pressed in the poll loop at 0x00083480. The host presenter
         * (movie_present.c) runs the picture and ADX audio on its own clock
         * and only stops at end of file, so on a skip it kept owning the
         * screen (d3d8 Present drops guest frames while it does) and the
         * press looked like it did nothing. Hand the screen back now. */
        extern int  doa3_movie_presenter_active(void);
        extern void doa3_movie_present_finish(void);
        extern void xa2_movie_stop(void);
        extern int  g_doa3_host_movie_ended;
        if (doa3_movie_presenter_active()) {
            xa2_movie_stop();
            g_doa3_host_movie_ended = 1;
            doa3_movie_present_finish();
        }
    }
    sub_0009E340_gen();
    g_doa3_post_movie = 1;
    { extern int g_kernel_trace_reads; g_kernel_trace_reads = 1; }
    if (log) {
                }
}
CALL_COUNT_PROBE(sub_0016B400)   /* ADX stream handle create (movie work buf) */
BOOT_MARK2_ABI_ESI(sub_0009C840)   /* sound/cache init: registers wxCi groups; restores ebx/esi/edi (leaked esi=1 into sub_0009F730's voice-table clear) */
CALL_COUNT_PROBE(sub_0009F730)   /* caller of sub_0009C840 */

/* sub_00169150 — ADXF partition mount/install status poll. The boot task's
 * sound/cache init (sub_0009C840) hard-spins `while (sub_00169150(id) != 3)`
 * with NO yield; on Xbox the ADX server thread advances the mount, but here
 * the CRI workers are fibers that only run at yield points — the spin
 * deadlocks the whole process (frame counter freezes at 1). Pulse the vblank
 * event and yield to the workers whenever the mount isn't complete, mirroring
 * the sub_00170330 yield wrapper. Gen body renamed _gen. */
void sub_00169150(void)
{
    extern void sub_00169150_gen(void);
    static unsigned s_n = 0;
    sub_00169150_gen();
    if (eax != 3) {
        MEM32(0x001C2CF0 + 4) = 1;              /* vblank KEVENT.SignalState */
        xbox_fiber_wake(0x001C2CF0);
        /* MOUNT-STALL FORENSICS (user's session hangs here; harness doesn't):
         * after ~30s of failed polls, dump the fiber table + CRI lock
         * globals once so the user's own log identifies the parked fiber. */
        {
            static unsigned s_mn = 0;
            if (++s_mn == 600000) {
                extern void xbox_fiber_dump_states(void);
                xbox_fiber_dump_states();
                /* op pool snapshot: what op sits unprocessed? */
                for (int op = 0; op < 4; op++) {
                    uint32_t o = 0xC07A40 + op * 0x40u;
                }
            }
        }
        {   /* no presents happen during this spin — service the window so it
             * doesn't go "Not Responding" under the user's clicks */
            extern void doa3_pump_messages(void);
            static unsigned s_mp2 = 0;
            if ((++s_mp2 & 63) == 0) doa3_pump_messages();
        }
        {   /* item 100: deterministic mount-stall net. The item-99 stall
             * signature is exactly: ADXF op processor unclaimed (C065C0==-1)
             * with the ADXM locks FREE while a self-suspended server fiber
             * sits parked past a lost resume. Spuriously resume the workers
             * (they re-check their queue and re-park if idle — same benign
             * wake xbox_fiber_block's no-runnable fallback already does).
             * Count-based (no wall clock); 50k failed polls is far past any
             * healthy mount and ~2.5s into a real stall. */
            static unsigned s_nudge = 0, s_nlog = 0;
            if (++s_nudge >= 50000 && (s_nudge & 8191) == 0 &&
                MEM32(0xC065C0) == 0xFFFFFFFFu &&
                MEM32(0xB24D38) == 0 && MEM32(0xC0E384) == 0) {
                extern int xbox_fiber_wake_count(uint32_t event_va);
                int woke = xbox_fiber_wake_count(0xBEEF0001u);
                if (s_nlog < 4) {
                    s_nlog++;
                }
            }
        }
        if (xbox_fiber_active())
            xbox_fiber_yield();
    }
    if (++s_n <= 4 || (s_n % 20000) == 0) {
    }
}

/* sub_0009FAB0 — movie-system audio gate. Reads the DirectSound object's
 * internal output status (sub_001C7392 -> sub_001C6C44: ds->+8->+8) and
 * returns bit 16 ("audio output ready"); sub_0009DD20 stores the result in
 * 0x5E5A04, and sub_0009DF60 refuses to create the Sofdec movie player when
 * it is 0. Our DSound layer has no APU so the status never reports ready
 * (additionally the gen mis-lifts the neg/sbb CF idiom in sub_001C7392 —
 * recomp bug #10 — turning the object pointer into NULL). Report ready:
 * the movie VIDEO path runs; audio submission lands in the DS stubs. */
void sub_0009FAB0(void)
{
#if 1   /* movie system audio-gate: force ready (no APU in the port) */
    eax = 1;
#else
    extern void sub_0009FAB0_gen(void);
    sub_0009FAB0_gen();
    return;
#endif
    esp += 4;
}

/* sub_0009DDB0 — movie surface-ring setup (tails into sub_0009DDE0, which
 * carves the 8 frame surfaces out of the VRAM banks @0x4889B8). With the
 * cache pre-validated the intro task reaches the movie start before the
 * graphics init has populated the banks (real hardware orders these via the
 * first-boot install delay), so all bank cursors read 0 and the setup fails
 * -> intro never plays. Yield to the other tasks until bank 0 exists (the
 * banks fill within the first ~60 frames), then run the real setup. */
void sub_0009DDB0(void)
{
    extern void sub_0009DDB0_gen(void);
    int spins = 0;
    while ((MEM32(0x4889B8) == 0 || MEM32(0x4889B8 + 0x1C) == 0) && spins < 7200) {
        MEM32(0x001C2CF0 + 4) = 1;          /* pulse vblank for CRI workers */
        xbox_fiber_wake(0x001C2CF0);
        if (!xbox_fiber_active()) break;    /* not on a task fiber: don't spin */
        xbox_fiber_yield();
        spins++;
    }
    sub_0009DDB0_gen();
}

/* DIAG: movie-surface creator sub_00157FB0(w,h,?,fmt,out) -> D3D texture via
 * sub_001C6716. Print args + result to see why the Sofdec player surface
 * (720x480 fmt 0x12) fails, which makes sub_0009DDB0 return 0 and the movie
 * starter bail before creating the player. */
void sub_00157FB0_gen(void);
void sub_00157FB0(void)
{
    static int s_n = 0;
    uint32_t w = MEM32(esp + 4), h = MEM32(esp + 8), a3 = MEM32(esp + 0xC),
             fmt = MEM32(esp + 0x10), out = MEM32(esp + 0x14);
    sub_00157FB0_gen();
    if (s_n < 8) {
        s_n++;
    }
}

/* DIAG: ADXF_LoadPartition (sub_00169530): print ptid + filename to map which
 * partition mount hits "not AFS". */
void sub_00169530_gen(void);
void sub_00169530(void)
{
    uint32_t id = MEM32(esp + 4), fn = MEM32(esp + 8);
    char nm[40] = {0};
    if (fn >= 0x10000 && fn < 0x8000000)
        for (int i = 0; i < 39; i++) { nm[i] = (char)MEM8(fn + i); if (!nm[i]) break; }
    sub_00169530_gen();
}

/* DIAG: CRI CompareFileTime (used by the cache validator sub_0009D9B8 to
 * decide 0x5E56A0 cache-dirty). Print both FILETIMEs + result for the first
 * calls to see which compare rejects our pre-seeded cache files. */
void sub_001644E8_gen(void);
void sub_001644E8(void)
{
    static int s_n = 0;
    uint32_t a = MEM32(esp + 4), b = MEM32(esp + 8);
    uint32_t alo = MEM32(a), ahi = MEM32(a + 4), blo = MEM32(b), bhi = MEM32(b + 4);
    sub_001644E8_gen();
    if (++s_n <= 12) {
    }
}

/* DIAG: wxCi copy pump — gated on the wxCi object state (+1)==2. */
void sub_00173D10_gen(void);
void sub_00173D10(void)
{
    static unsigned s_n = 0;
    uint32_t obj = MEM32(esp + 4);
    s_n++;
    if (s_n <= 4 || (s_n % 3000) == 0) {
    }
    sub_00173D10_gen();
}


/* ── DIAG: texture-slot resolver (sub_0006E0B0) — the boot screen's quad draw
 * fails because its texture slot (0x48F288 table) is empty; this is the fn
 * that fills slots. Log calls: this=ecx, arg1(name/id), arg2(slot), result. */
void sub_0006E0B0_gen(void);
void sub_0006E0B0(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    uint32_t this_ = ecx;
    sub_0006E0B0_gen();
    s_n++;
    if (s_n <= 12) {
        /* sub_0006E0B0 rejects the blob unless it starts with 'XPR0' (Xbox
         * Packed Resource); dump the first bytes so an empty buffer (never
         * loaded) is distinguishable from wrong/misaligned data. */
        fprintf(stderr, "[TEXHDR] #%d a1=0x%08X magic=%08X bytes:", s_n, a1, MEM32(a1));
    }
}

/* ── DIAG: D3DX draw dispatch (sub_00069BF3) — icall target hunt ────────────
 * icalls [MEM32(obj@0x49A954)]+0x14 with 8 args; the call never reaches
 * sub_001C3F50 and leaks the args. Log the object + vtable + slot. */
void sub_00069BF3_gen(void);
void sub_00069BF3(void)
{
    static int s_n = 0;
    if (s_n < 6) {
        s_n++;
        uint32_t obj = MEM32(0x49A954);
        uint32_t vt  = obj ? MEM32(obj) : 0;
        uint32_t tgt = vt ? MEM32(vt + 0x14) : 0;
        uint32_t tp  = MEM32(0x49A95C);          /* texture slot ptr */
        fprintf(stderr, "[DXDRAW] obj=0x%08X vtbl=0x%08X slot14=0x%08X texptr=0x%08X tex=0x%08X esp=0x%08X\n",
                obj, vt, tgt, tp, tp ? MEM32(tp) : 0, esp);
    }
    sub_00069BF3_gen();
}

/* ── DIAG: per-glyph draw (sub_00055AD0) — esp corruption hunt ──────────────
 * Called per boot-screen glyph with the 0x305B00 struct. Track esp balance
 * per call; dump struct fields when esp goes insane. */
void sub_00055AD0_gen(void);
void sub_00055AD0(void)
{
    static int s_n = 0;
    uint32_t ei = esp, a1 = MEM32(esp + 4);
    s_n++;
    sub_00055AD0_gen();
    if ((esp != ei + 4 && s_n < 2000) || s_n <= 3) {
    }
}

/* ── DIAG: screen render walker (sub_0006C480) — draws never reach the PB ───
 * Called per frame by the render dispatch; walks the draw-task list built by
 * the boot task and issues sprite/glyph draws. Log call rate + first arg. */
void sub_0006C480_gen(void);
void sub_0006C480(void)
{
    static int s_n = 0;
    s_n++;
    if (s_n <= 4 || (s_n % 600) == 0) {
        /* The walk at 0x0006C548 iterates the dword vector
         * [0x49A98C, 0x49A990) with `jne`, re-reading the end pointer every
         * iteration, and is spinning -- hundreds of millions of D3DX quad
         * draws, all returning D3DERR_INVALIDCALL because the sprite argument
         * is NULL. Report the bounds so it is clear whether the end pointer is
         * simply unreachable or the list grows under the walk. */
        fprintf(stderr, "[RWALK] #%d a1=0x%08X a2=0x%08X ecx=0x%08X "
                        "beg=%08X end=%08X cap=%08X n=%d dxflag=%u dxobj=%08X\n",
                s_n, MEM32(esp + 4), MEM32(esp + 8), ecx,
                MEM32(0x49A98C), MEM32(0x49A990), MEM32(0x49A994),
                (int)((MEM32(0x49A990) - MEM32(0x49A98C)) / 4),
                MEM32(0x49A950), MEM32(0x49A954));
    }
    /* This walk used to be skipped whenever [0x49A954] was zero, on the
     * theory that it was the D3DX sprite every command here draws through.
     * It is not. 0x49A954 belongs to sub_00069B88, the BOOT WARNING SCREEN's
     * sprite, and that screen is deliberately never run (see sub_000566D0),
     * so the global stays zero for the whole session. This walker creates its
     * own sprite locally with sub_001C35C0 (D3DXCreateSprite) into its
     * [esp+0x10] slot and draws through that one.
     *
     * cxbx settles it: at the attract screen the real game also has
     * 0x49A950 == 0 and 0x49A954 == 0, and it still draws the corner
     * "DEAD OR ALIVE 3" quad -- DrawVerticesUP(prim 7, 4 verts, stride 28)
     * over (340.5,426.5)-(750.1,452.1) -- reached through exactly this walk
     * (sub_00065F70 -> sub_00065500 -> here -> sub_0006C560 ->
     * sub_001C3FDF -> sub_001C408A). The skip was removing the game's whole
     * 2D overlay layer. */
    sub_0006C480_gen();
}

/* ── DIAG: boot-task create loop (531B objects, never freed) ────────────────
 * sub_00084340 (boot screen handler) -> sub_00021F70 -> sub_000572C0 -> ... ->
 * operator new, ~4000/s. Log call rates + args to find the retry condition. */
void sub_00021F70_gen(void);
void sub_00021F70(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    s_n++;
    if (s_n <= 5 || (s_n % 2000) == 0) {
    }
    { extern void doa3_ebxwp_pause(void); doa3_ebxwp_pause(); }   /* DOA3 diag: no ebx watch through the movie */
    sub_00021F70_gen();
    { extern void doa3_ebxwp_resume(void); doa3_ebxwp_resume(); }
}
void sub_000572C0_gen(void);
void sub_000572C0(void)
{
    static int s_n = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx; /* ABI: its draw chain
        clobbers esi -> the intro sequencer's fade-out loop counter reset
        every frame -> the sequencer never left the warning screen. */
    s_n++;
    if (s_n <= 5 || (s_n % 2000) == 0) {
    }
    sub_000572C0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* ── DIAG: D3DX context Initialize (sub_001C3C65, thiscall this=ecx, 1 arg) —
 * fails every frame -> the game re-creates its render state block forever
 * (flag 0x49A950 never sets, 512B leak per retry, and likely draws=0). Log hr. */
void sub_001C3C65_gen(void);
void sub_001C3C65(void)
{
    uint32_t this_ = ecx, dev = MEM32(esp + 4), esp_in = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx; /* ABI: its fragment chain
        clobbers ebx -> the factory sub_001C35CE stored the created object to a
        garbage address instead of the out-param (0x49A954 stayed NULL). */
    sub_001C3C65_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    static int s_log = 0;
    if (s_log < 8) {
        s_log++;
    }
    esp = esp_in + 8;  /* enforce: thiscall, 1 stack arg (ret 4) — a residual
                        * -28 leak remains somewhere in its fragment chain */
}

/* D3DDevice_EndStateBlock(&handle) — ABI-enforcement wrapper (+ diagnostics).
 * Same disease as RecordStateBlock below: an inner callee's broken cleanup
 * makes the mis-split epilogue fragments pop leftover args into edi/esi/ebx
 * (edi came back = 0x1C0800 g_pDevice), which destroyed the D3DX create
 * loop's bound. Enforce callee-saved registers here. */
void sub_001BEEFE_gen(void);
void sub_001BEEFE(void)
{
    uint32_t outp = MEM32(esp + 4), esp_in = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001BEEFE_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    static int s_log = 0;
    if (s_log < 8) {
        s_log++;
    }
}

/* DIAG: esp-balance probes for the state-block batch callees (drift hunt).
 * Each prints espin/espout for its first few calls. */
#define ESP_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        uint32_t ei = esp; \
        uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx; /* enforce callee-saved ABI */ \
        fn##_gen(); \
        edi = s_edi; esi = s_esi; ebx = s_ebx; \
        static int s_n = 0; \
        if (s_n < 4) { s_n++; \
             \
             } \
    }
/* CreateDevice tail: two callees leak the guest stack, and that is what left
 * the device without a render target.
 *
 * D3DDevice_CreateDevice ends in the fragment at 0x001B9537, which reads the
 * device out of its own frame at [esp+4] and hands the implicit back buffer
 * (device + 0x2150) to SetRenderTarget. Measured across that tail, esp came
 * back 0xC0 bytes low -- sub_001B8EE0 returned -8 where it should return +4,
 * and sub_001B9130 returned -172 where it should return +8, which is exactly
 * the 192 lost. Both are split functions whose pushes are popped in a
 * successor fragment, so the pops never run on the path taken here.
 *
 * With the frame shifted, [esp+4] read 0xEFFF00DE instead of the device, so
 * SetRenderTarget stored 0xEFFF222E as the current colour surface. SetViewport
 * clamps every viewport against that surface: the game asks for 720x480 and
 * the device records width 0, D3D_UpdateProjectionViewportTransform then
 * builds an all-zero projection-viewport matrix, the composite matrix streamed
 * to the GPU is 16 zero dwords, and every transformed vertex lands on the
 * viewport centre. That is the black post-movie screen.
 *
 * Enforce the documented contract at both boundaries: pop the dummy return
 * slot plus the callee-popped argument bytes, and restore the callee-saved
 * registers the split epilogues never restored. */
ESP_PROBE(sub_001B8690)   /* GPU wait spin (ret 0)            expect d=+4  */
/* (the ESP_FIX uses live next to the macro definition further down) */

ESP_PROBE(sub_00176230)   /* movie ring finalize */
ESP_PROBE(sub_001B2440)
ESP_PROBE(sub_001B23C0)
ESP_PROBE(sub_001B2800)
ESP_PROBE(sub_001B2520)
ESP_PROBE(sub_001B28F0)
/* SetVertexShader(FVF) — after the call, dump the fixed-function shader
 * object (0x1C0688: +4 flags, +0x30 16 attr formats stride 0x10) and the
 * device's current-shader fields, to see where the FVF parse chain loses
 * the attribute layout (draw-time lazy apply reads all formats disabled). */
void sub_001B45F0_gen(void);
void sub_001B45F0(void)
{
    static int s_n = 0;
    uint32_t fvf = MEM32(esp + 4);
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001B45F0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    esp = ei + 4 + 4;   /* ret 4 (enforce; fragments) */
    if (s_n < 6) {
        s_n++;
        uint32_t o = 0x1C0688, dev = MEM32(0x1C3390);
        for (int k = 0; k < 20; k++) {
            uint32_t fm = MEM32(o + 0x30 + 0x10u * k);
        }
    }
}
/* Push-buffer append (0x1B2390) -- __fastcall(ecx, edx), plain ret.
 *
 * Reserves 8 bytes at the device write cursor and stores ecx/edx there:
 *
 *     mov eax, [0x1c0800] / add eax, 8 / cmp eax, [0x1c0804]
 *     jae  full
 *     mov [0x1c0800], eax / mov [eax-8], ecx / mov [eax-4], edx / ret
 *   full:
 *     call sub_001B8B00        ; make space
 *     jmp  0x1b2390            ; retry
 *
 * 0x1C0800 is not a private global: [0x1C3390] (the device pointer) is set
 * to 0x1C0800, so these are dev+0 and dev+4 -- the very cursor/limit pair
 * the sub_001B8DA0 and sub_001B8DC0 overrides already clamp. This path does
 * not go through either of them; it tests the limit itself and relies on
 * sub_001B8B00 to wrap, which never happens here. So once the buffer filled,
 * the retry never succeeded -- and because the guest "jmp" back to the top is
 * emitted as a call, that hardware retry loop became real recursion and ran
 * the host stack out (0xC00000FD, frames all sub_001B2390+0x31).
 *
 * Apply the same wrap the other two use and do the append directly, so there
 * is no retry to recurse on. */
static void doa3_pb_wrap(uint32_t dev, const char *who);
void sub_001B2390(void)
{
    extern uint32_t g_doa3_pb_base, g_doa3_pb_end;
    uint32_t dev = 0x001C0800u;
    uint32_t cursor = MEM32(dev);
    if (cursor + 8 >= MEM32(dev + 0x04)) {
        /* Buffer full: run the guest flush exactly as the original does
         * (it preserves ecx/edx across it), then enforce the wrap it
         * fails to perform. Skipping the flush entirely starved whatever
         * it advances and sent the matrix path into a 262M-call spin. */
        extern void sub_001B8B00(void);
        uint32_t s_ecx = ecx, s_edx = edx;
        ecx = MEM32(0x001C3390u);
        PUSH32(esp, 0);
        sub_001B8B00();
        ecx = s_ecx; edx = s_edx;
        cursor = MEM32(dev);
    }
    if (g_doa3_pb_base &&
        (cursor < g_doa3_pb_base || cursor + 0x1000 >= g_doa3_pb_end)) {
        doa3_pb_wrap(dev, "Append");
        cursor = g_doa3_pb_base;
    }
    MEM32(cursor)     = ecx;
    MEM32(cursor + 4) = edx;
    MEM32(dev)        = cursor + 8;
    esp += 4;   /* plain ret: only our fake return slot */
}
ESP_PROBE(sub_001B2930)
/* [SETTEX] sub_001B1CC0(stage, resource) -- ret 8, so arg0 = [esp+4] and
 * arg1 = [esp+8]. arg1 becomes EBP and is handed to the refcount walker
 * sub_001B4960, which faults on it. Log both plus the caller. */
void sub_001B1CC0_gen(void);
void sub_001B1CC0(void) {
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    uint32_t a0 = MEM32(esp + 4), a1 = MEM32(esp + 8);
    { static unsigned n = 0, bad = 0;
      n++;
      if (a1 && (a1 < 0x1000u || a1 >= 0x08000000u)) {
        bad++;
      } else if (n <= 4) {
      } }
    /* Same containment one level up. A non-pointer resource here also
     * gets stored into the device stage table at [edi + stage*4 + 0xba0];
     * with a wild stage that store lands in the object-pointer array at
     * 0x480D68 and corrupts it, which then feeds worse values back into
     * this same path. Rejecting the call breaks that feedback loop. */
    /* Reject only a non-pointer resource. The index is NOT bounded here:
     * [edi + idx*4 + 0xba0] covers more than the four texture stages, and
     * clamping it to 3 dropped legitimate calls (draws went to zero). */
    if (a1 && (a1 < 0x1000u || a1 >= 0x08000000u)) {
        edi = s_edi; esi = s_esi; ebx = s_ebx;
        esp = ei + 12;   /* ret 8 */
        return;
    }
    sub_001B1CC0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    (void)ei;
}
ESP_PROBE(sub_001B4230)
ESP_PROBE(sub_001B10B0)
ESP_PROBE(sub_001B0DE0)
ESP_PROBE(sub_001C3F50)   /* D3DX draw (8 args, ret 0x20): expect d=+36 */
/* DIAG: sub_001B7690 — dirty-state flusher called at the head of the lazy
 * vertex apply (sub_001B79F0). If it leaks esp, every esp-relative local in
 * the apply (incl. the saved device slot at esp+0x10) reads garbage and the
 * attr walker writes nowhere. Measure drift (expected +4). */
void sub_001B7690_gen(void);
void sub_001B7690(void)
{
    static int s_n = 0;
    uint32_t ei = esp;
    uint32_t entry_esi = esi, entry_ebx = ebx;
    sub_001B7690_gen();
    if (s_n < 16 && (esp != ei + 4 || esi != entry_esi || ebx != entry_ebx)) {
        s_n++;
    }
}

/* DIAG: drift probes for the 8 dirty-state sub-appliers under sub_001B7690. */
#define DRIFT_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        static int s_n = 0; \
        uint32_t ei = esp; \
        fn##_gen(); \
        if (s_n < 8 && (int)(esp - ei) != 8) { \
            s_n++; \
             \
             } \
    }
/* sub_001B6610 (combiner-state applier, ret 4): leaks -44 through its
 * sub_001B6650 fragment chain (bug class #7). ABI-enforced: */
void sub_001B6610_gen(void);
void sub_001B6610(void)
{
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001B6610_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    esp = ei + 4 + 4;
}
DRIFT_PROBE(sub_001B6120)
DRIFT_PROBE(sub_001B69F0)
DRIFT_PROBE(sub_001B74A0)
DRIFT_PROBE(sub_001B6810)
DRIFT_PROBE(sub_001B6290)
/* sub_001BCC00 and sub_001BC260 -- the two callees that destroy the device
 * pointer inside the texture-stage applier sub_001B6410.
 *
 * The generated bodies are split and their pops live in successor fragments
 * that the taken path never reaches, so on some paths they return with
 * ebx/esi/edi clobbered and the guest stack low. The checker the pipeline
 * already emits around these call sites recorded it directly ([CSCHK3]):
 *
 *   sub_001BCC00: esi 001C0800 -> 00000000, edi 00000000 -> 001C0800,
 *                 ebx 000007FF -> 00000000, esp d=-84  (correct d=+12)
 *   sub_001BC260: ebx 00000003 -> 001C0800, edi 001C0800 -> 00000000,
 *                 esp d=-72                            (correct d=+16)
 *
 * sub_001B6410 holds the device in esi across both calls and pushes it to
 * XMETAL_StartPush at 0x001B6496; its second StartPush site at 0x001B6790
 * reads the device from [esp+0x30]. With esi zeroed and esp 156 bytes low,
 * StartPush was called with a null device -- 106 of every 127 push-buffer
 * wraps -- and each of those rewound the translator to the ring base, so the
 * whole 1 MB push buffer was re-translated on every kick (~81 MB every two
 * seconds), replaying every draw and clear of the frames still in
 * the buffer over the live one.
 *
 * Both are __stdcall: 3 args (ret 0xC) and 4 args (ret 0x10) respectively,
 * matching the pushes at 0x001B6441 and 0x001B644B. Enforce the ABI. */
void sub_001BCC00_gen(void);
void sub_001BCC00(void)
{
    uint32_t ei = esp;
    uint32_t s_di = edi, s_si = esi, s_bx = ebx;
    sub_001BCC00_gen();
    edi = s_di; esi = s_si; ebx = s_bx;
    esp = ei + 4 + 0xCu;
}

void sub_001BC260_gen(void);
void sub_001BC260(void)
{
    uint32_t ei = esp;
    uint32_t s_di = edi, s_si = esi, s_bx = ebx;
    sub_001BC260_gen();
    edi = s_di; esi = s_si; ebx = s_bx;
    esp = ei + 4 + 0x10u;
}

/* sub_001B6410 (texture-stage dirty applier, ret 4): leaks -176 bytes via its
 * fragmented callee chain (sub_001BC260 -> _2CF/_45A fragments, bug class #7),
 * which sank every esp-relative local in the lazy vertex apply above it —
 * the attr walker wrote into a garbage device pointer and dev+0x874 stayed 0,
 * killing EVERY draw. ABI-enforce instead of probing: */
DRIFT_PROBE(sub_001B7180)
void sub_001B6410_gen(void);
void sub_001B6410(void)   /* ABI-enforced (ESP_FIX pattern; macro defined later), ret 4 */
{
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001B6410_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    esp = ei + 4 + 4;
}

/* sub_001B3760 — D3D inline vertex push (BEGIN prim + scatter-gather copy of
 * vertex attributes into the push buffer). The copy loop walks descriptors at
 * device+0x7F4 (pairs {dwords, skip}) with count device+0x874, both populated
 * by SetVertexShader. Print them at call time: garbage descriptors = the
 * movie-quad crash (host memcpy read past base+4GB). */
static int doa3_pb_recording(uint32_t dev);
extern uint32_t g_doa3_pb_base, g_doa3_pb_end;
void sub_001B3760_gen(void);
void sub_001B3760(void)
{
    static int s_n = 0;
    uint32_t dev = MEM32(0x1C3390);
    if (s_n < 6) {
        s_n++;
        {   uint32_t vb = MEM32(esp + 0xC);
            for (int k = 0; k < 24; k++) {
                union { uint32_t u; float f; } cv; cv.u = MEM32(vb + 4u * k);
            }
        }
    }
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    uint32_t ei = esp;
    /* GUARD: with no vertex attributes applied (dev+0x874 == 0) the inline
     * copy loop underflows (0-1 -> 4G iterations) and marches host memcpy
     * past the 4GB mapping. Run the lazy vertex-shader apply FIRST (the gen
     * draw does the same as its first step, and re-running is a no-op once
     * the dirty bit is consumed); only skip if the layout is STILL empty. */
    if (dev && MEM32(dev + 0x874) == 0) {
        extern void sub_001B79F0(void);
        uint32_t s2_edi = edi, s2_esi = esi, s2_ebx = ebx;
        ecx = dev;                               /* fastcall this = device */
        PUSH32(esp, 0); sub_001B79F0();
        edi = s2_edi; esi = s2_esi; ebx = s2_ebx;
        esp = ei;
    }
    if (!dev || MEM32(dev + 0x874) == 0) {
        static int s_g = 0;
        if (s_g < 8) {
            s_g++;
        }
        esp = ei + 4 + 16;   /* ret 16 */
        return;
    }
    /* Feed the pgraph the CPU-side vertex layout for this draw: walk the
     * FVF shader object's NV attribute slots (fmt at obj+0x30+i*0x10; fmt 2 =
     * disabled; type=fmt&0xF, count=(fmt>>4)&0xF) accumulating dword offsets.
     * NV slots: 0=position, 3=diffuse (D3DCOLOR, 1 dword), 9=texcoord0. */
    {
        extern void pgraph_d3d11_set_vertex_layout(uint32_t, int, int, int);
        uint32_t obj = MEM32(dev + 0x470);
        if (obj) {
            int off = 0, pos_dw = 2, uv_off = -1, color_off = -1;
            for (int sl = 0; sl < 16; sl++) {
                uint32_t fm = MEM32(obj + 0x30 + 0x10u * sl);
                if (fm == 2) continue;
                uint32_t ty = fm & 0xF, cn = (fm >> 4) & 0xF;
                int dw = (ty == 2) ? (int)cn
                       : (ty == 0 || ty == 4) ? 1
                       : (ty == 1 || ty == 5) ? (int)((cn + 1) / 2)
                       : (int)cn;
                if (sl == 0) pos_dw = dw;
                else if (sl == 3) color_off = off;
                else if (sl == 9) uv_off = off;
                off += dw;
            }
            pgraph_d3d11_set_vertex_layout((uint32_t)(MEM32(esp + 0x10) / 4),
                                           pos_dw, uv_off, color_off);
            {   /* DOA3: hand the pgraph the guest's vertex count and the
                 * declared dword total as well. The API stride is the stride
                 * of the SOURCE array, not of what gets pushed -- the D3DX
                 * sprite quad declares 5 dwords, has a 7-dword API stride and
                 * pushes 11 (position + diffuse + three texcoord sets), so
                 * inline_count / vertex-count is the only exact answer. */
                extern void pgraph_d3d11_set_inline_hint(uint32_t, uint32_t);
                pgraph_d3d11_set_inline_hint(MEM32(esp + 8), (uint32_t)off);
            }
            /* The same layout IN the stream, ahead of this draw's BEGIN, as
             * two NV2A NOPs (0xAC = layout, 0xAD = counts). The calls above
             * set globals the translator reads when the push buffer is
             * parsed at the next kick -- by then they hold whatever the LAST
             * DrawVerticesUP of the kick declared. The courtyard torch's
             * ember sprites (45 points, 4 dwords each) were sliced with the
             * 7-dword layout of the health bars drawn after them: 27 garbage
             * vertices with an rhw read from the diffuse word. Same
             * mechanism as doa3_pb_tss_marker; skipped while recording. */
            doa3_pb_tss_marker();            /* stage ops: the sprite's stage 3 */
            if (!doa3_pb_recording(dev) && g_doa3_pb_base) {
                uint32_t cursor = MEM32(dev), sdw = MEM32(esp + 0x10) / 4;
                if (cursor >= g_doa3_pb_base && cursor + 0x1000 < g_doa3_pb_end && sdw >= 2 && sdw <= 16) {
                    MEM32(cursor)      = 0x00040100u;
                    MEM32(cursor + 4)  = 0xAC000000u | ((sdw & 0x1Fu) << 16) | (((uint32_t)pos_dw & 0x7u) << 12) |
                                         (((uint32_t)uv_off & 0x3Fu) << 6) | ((uint32_t)color_off & 0x3Fu);
                    MEM32(cursor + 8)  = 0x00040100u;
                    MEM32(cursor + 12) = 0xAD000000u | ((MEM32(esp + 8) & 0xFFFFu) << 8) | ((uint32_t)off & 0xFFu);
                    MEM32(dev) = cursor + 16;
                }
            }
        }
    }
    sub_001B3760_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    (void)ei;
}
/* [TASKNEW] temporary: which task entry points actually get registered.
 * sub_0009E422(priority, entry) is the task-create the post-movie screen
 * (0x000CEF80) is launched through from sub_00083920. */
void sub_0009E422_gen(void);
void sub_0009E422(void) {
    uint32_t pri = MEM32(esp + 4), ent = MEM32(esp + 8);
    static int n = 0;
    if (n < 40) { n++;
         }
    sub_0009E422_gen();
}
/* [FLOW] temporary: is the post-movie screen task ever created?
 * sub_00083920 is mode-jump-table case 2 (table at 0x00084574) and is what
 * calls sub_0009E422(2, 0x000CEF80). It returns early when sub_00083320
 * reports a held reset combo, or when MEM8(0x47ADB8) == 1. */
void sub_00083920_gen(void);
void sub_00083920(void) {
    static int n = 0;
    if (n < 6) { n++;
         }
    sub_00083920_gen();
}
void sub_00083320_gen(void);
void sub_00083320(void) {
    static int n = 0;
    sub_00083320_gen();
    if (n < 6) { n++;
          }
}
/* [MWEND] temporary: the mwPly PLAYEND transition. sub_00176CE0 steps the
 * player; when obj[8]==2 it calls sub_00176C80, which sets obj[8]=3 once
 * sub_0017D4E0(obj[0x30]) reports SFD state 6. The intro sequencer waits on
 * obj[8]==3 (status 3 = PLAYEND) before leaving its post-movie wait. */
void sub_00176CE0_gen(void);
void sub_00176CE0(void) {
    uint32_t obj = MEM32(esp + 4);
    static int n = 0; static unsigned calls = 0;
    calls++;
    if (obj && n < 12 && (calls % 512) == 0) { n++;
         }
    sub_00176CE0_gen();
}
unsigned g_mw_calls, g_mw_st6, g_mw_set3, g_mw_laststate;
unsigned g_e1f0_calls, g_e1f0_nz;
/* [MVST] temporary: what the movie status vtable slot actually reports. */
unsigned g_st_calls; int g_st_lastret, g_st_lastst8; uint32_t g_st_lastobj;
void sub_00177170_gen(void);
void sub_00177170(void) {
    uint32_t obj = MEM32(esp + 4);
    g_st_calls++;
    g_st_lastobj = obj;
    g_st_lastst8 = obj ? (int)MEM32(obj + 8) : -1;
    sub_00177170_gen();
    g_st_lastret = (int)eax;
    /* [MVST2] The intro sequencer leaves state 1 only when this returns 3,
     * and 3 can only come from obj+8 being 3 verbatim (the obj+8==2 branch
     * can only yield 1, 2 or 4). Log every change so we can see whether
     * obj+8 ever reaches 3 at the moment this is polled. */
    { static int last_st8 = -99, last_ret = -99; static unsigned n = 0;
      int st8 = obj ? (int)MEM32(obj + 8) : -1;
      if ((st8 != last_st8 || (int)eax != last_ret) && n < 30) { n++;
        last_st8 = st8; last_ret = (int)eax;
         } }
}
void sub_0009E1F0_gen(void);
void sub_0009E1F0(void) {
    g_e1f0_calls++;
    sub_0009E1F0_gen();
    if (eax) g_e1f0_nz++;
    {   /* [MVPOLL] the intro sequencer polls this every frame while it waits
         * for the movie to report PLAYEND; report what it sees. */
        static unsigned n = 0;
        static int last8 = -99, lastret = -99, lastIntro = -99;
        uint32_t o0 = MEM32(0x5E5900);
        int cur8 = o0 ? (int)MEM32(o0 + 8) : -1;
        if ((cur8 != last8 || (int)eax != lastret ||
             (int)MEM8(0x4B83B0) != lastIntro) && n < 30) { n++;
            last8 = cur8; lastret = (int)eax; lastIntro = (int)MEM8(0x4B83B0);
            uint32_t o = MEM32(0x5E5900);
             }
    }
}
void sub_00176C80_gen(void);
void sub_00176C80(void) {
    uint32_t obj = MEM32(esp + 4);
    uint32_t h = obj ? MEM32(obj + 0x30) : 0;
    g_mw_calls++;
    if (h) { g_mw_laststate = MEM32(h + 0x40);
             if (g_mw_laststate == 6) g_mw_st6++; }
    sub_00176C80_gen();
    if (obj && MEM32(obj + 8) == 3) g_mw_set3++;
}
/* sub_001B37DC / sub_001B37EC -- the >16-vertex path of DrawVerticesUP
 * (sub_001B3760). 0x1B37DC computes the largest vertex count one
 * INLINE_ARRAY header can carry (0x7FF dwords / vertex dwords), stores it
 * at [ebp+0x10], and FALLS THROUGH 0x1B37EC (reload it into eax) into the
 * chunked push at 0x1B37F0; 0x1B37EC is also the re-entry the multi-chunk
 * loop tail-jumps to. Both generated bodies end at the trailing nop and
 * return, so every inline draw of more than 16 vertices wrote its BEGIN
 * and nothing else: no vertex data, no END. The courtyard torch's ember
 * sprites (DrawVerticesUP POINTLIST, 39-50 points) were 271 headless
 * BEGINs in a 4.5 s window; the translator never ran a point draw. */
void sub_001B37F0(void);
void sub_001B37DC(void)
{
    uint32_t ebp = g_seh_ebp;               /* fpo_leaf: caller's frame */
    uint32_t vd = MEM32(edi + 0x7E8);
    eax = vd ? 0x7FFu / vd : 0x7FFu;
    edx = vd ? 0x7FFu % vd : 0;
    MEM32(ebp + 0x10) = eax;
    g_seh_ebp = ebp; sub_001B37F0();        /* fall through 0x1B37EC/0x1B37F0 */
}
void sub_001B37EC(void)
{
    uint32_t ebp = g_seh_ebp;
    eax = MEM32(ebp + 0x10);
    g_seh_ebp = ebp; sub_001B37F0();        /* fall through 0x1B37F0 */
}
ESP_PROBE(sub_001B37F0)
ESP_PROBE(sub_001C4069)   /* D3DX context Release (vtbl slot 2) */
ESP_PROBE(sub_00069C70)   /* boot-screen D3DX teardown (RestoreState+Release) */
/* Boot warning/legal screen (arg 0 = reset and arm, non-zero = advance one
 * frame). SKIPPED on the user's instruction: the game is to boot straight
 * into the intro movie.
 *
 * Skipping it is also what stops a livelock. The screen draws its text one
 * glyph at a time through the D3DX quad path (sub_00055AD0 -> sub_00069BD0
 * -> sub_001C3F50 -> sub_001C408A), and the string it walks has no
 * terminator here, so the glyph loop never ends: 28 million quad draws, each
 * returning D3DERR_INVALIDCALL, starving the movie decode. The path only
 * became reachable once D3DDevice_SetRenderTarget started working -- the
 * quad-draw count is zero in builds where the device never came up.
 *
 * Take the sequencer's own completion branch (0x00056809) instead of running
 * the body: park the frame counter past its limit (3 * MEM8(0x2FD55C)) and
 * clear the screen-active flags. The D3DX teardown that branch tail-calls
 * (sub_00069C70) is deliberately NOT called -- the matching creation lives in
 * the advance path being skipped, so there is nothing to release. */
void sub_000566D0_gen(void);
void sub_000566D0(void) {
    uint32_t arg = MEM32(esp + 4);
    static int s_n = 0;
    if (arg == 0) {
        /* reset, verbatim from 0x000566DB */
        MEM32(0x491AFC) = 0;
        MEM32(0x491AF8) = 0;
        MEM32(0x491AE4) = 0;
        MEM8(0x491AF4) = 0;
        MEM8(0x305B70) = 1;
    } else if (MEM8(0x305B70)) {
        MEM32(0x491AFC) = 3u * (uint32_t)MEM8(0x2FD55C) + 1u;
        MEM8(0x305B70) = 0;
        MEM8(0x305B1C) = 0;
        MEM8(0x305B1B) = 0;
        if (s_n < 2) { s_n++;
             }
    }
    eax = 0;
    esp += 4;   /* ret */
}
ESP_PROBE(sub_00069B60)   /* boot-screen D3DX ensure-created */
ESP_PROBE(sub_00069BD0)   /* boot-screen draw dispatcher */

/* ESP_FIX: like ESP_PROBE but also ENFORCES the known stdcall stack cleanup
 * (esp_out = esp_in + 4 + argbytes) — for XDK functions whose inner callees
 * leak stack through mis-split fragments. Registers + esp fully restored to
 * the ABI contract; the inner rot stays contained. */
#define ESP_FIX(fn, argbytes) \
    void fn##_gen(void); \
    void fn(void) { \
        uint32_t ei = esp; \
        uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx; \
        fn##_gen(); \
        edi = s_edi; esi = s_esi; ebx = s_ebx; \
        esp = ei + 4 + (argbytes); \
    }
/* D3DTexture_LockRect (0x1B4B30, per cxbx symbols) — full semantic override.
 * The inner XDK path (sub_001BA3F0/sub_001BA1C0) is mis-split by the lifter
 * and returned a garbage D3DLOCKED_RECT (pBits=0/0x2D0), so the movie CSC
 * had no destination and the screen stayed black. Decode the Xbox
 * D3DPixelContainer header ourselves:
 *   +0 Common  +4 Data (GPU addr of pixels)  +8 Lock  +C Format  +10 Size
 *   Size(linear): width-1 [0:11] | height-1 [12:23] | pitch/64-1 [24:31]
 * pBits = Data (raw guest RAM address; 0x8xxxxxxx alias also maps there).
 * args: (pTex, level, pLockedRect, pRect, flags), ret 20, eax=HRESULT. */
void sub_001B4B30(void)
{
    uint32_t tex   = MEM32(esp + 4);
    uint32_t plr   = MEM32(esp + 0xC);
    static int s_n = 0;
    uint32_t data = 0, fmt = 0, size = 0, pitch = 0;
    if (tex >= 0x1000 && tex < 0x8000000u) {
        data = MEM32(tex + 4);
        fmt  = MEM32(tex + 0xC);
        size = MEM32(tex + 0x10);
        if (size) {
            pitch = (((size >> 24) & 0xFF) + 1) * 64;
        } else {
            /* swizzled/power-of-2 header: pitch from format U/P exponent */
            uint32_t u = (fmt >> 20) & 0xF;              /* log2 width */
            uint32_t bpp = ((fmt >> 8) & 0xFF);
            pitch = (1u << u) * ((bpp == 0x11 || bpp == 0x1E) ? 1 : 2);
        }
    }
    if (plr >= 0x1000 && plr < 0x8000000u) {
        MEM32(plr)     = pitch;                          /* D3DLOCKED_RECT.Pitch */
        MEM32(plr + 4) = data & 0x07FFFFFFu;             /* .pBits (raw RAM addr) */
    }
    if (s_n < 8) {
    }
    s_n++;
    eax = 0;
    esp += 24;   /* fake-ret slot + ret 20 */
}
/* NOT ENABLED YET -- enabling these two alone hangs the boot.
 *
 * They do remove the 0xC0 drift they were measured against (esp at 0x001B98F6
 * goes from 0x00EFFC6C to the expected 0x00EFFD2C), but the frame local is
 * still not the device there: at 0x001B981D the hardware reads it from
 * [esp+8], which puts the correct esp at 0x00EFFDC4, and we sit at 0x00EFFD28
 * -- 0x9C low from somewhere EARLIER in the same function. With the tail
 * corrected and the head still drifted, the fence spin at 0x001B98B8 reads a
 * notifier out of a wrong pointer and never exits, so boot stops before the
 * movie. The earlier drift has to be found first; then enable both.
 *
 * ESP_FIX(sub_001B8EE0, 0)   no stack args; ret 0    (measured -8,   want +4)
 * ESP_FIX(sub_001B9130, 4)   one stack arg; ret 4    (measured -172, want +8) */
/* D3DDevice_CreateDevice tail: five split callees leak the guest stack, and
 * that is what left the device without a render target.
 *
 * CreateDevice ends in the fragment at 0x001B9537, which reads the device out
 * of its own frame and hands the implicit back buffer (device + 0x2150) to
 * SetRenderTarget. Measured across that tail, esp ran 0x15C bytes low, so the
 * frame local read 0xEFFF00DE instead of the device and SetRenderTarget stored
 * 0xEFFF222E as the current colour surface. SetViewport clamps every viewport
 * against that surface, so the game asking for 720x480 left the device
 * recording width 0; D3D_UpdateProjectionViewportTransform then built an
 * all-zero projection-viewport matrix, the composite matrix streamed to the
 * GPU arrived as 16 zero dwords, and every transformed vertex collapsed onto
 * the viewport centre. That is the black post-movie screen.
 *
 * Each of these is a function the detector split, so its pushes are popped in
 * a successor fragment that the taken path never reaches. Measured deltas
 * (want = 4 for the dummy return slot + the callee-popped argument bytes):
 *
 *   sub_001BB770   want +4    measured -56
 *   sub_001BB256   want +8    (ret 4,   in the -0x60 run before 0x001B981D)
 *   sub_001BB66A   want +16   (ret 0xC, same run; its ret is past its end)
 *   sub_001B8EE0   want +4    measured -8
 *   sub_001B9130   want +8    measured -172
 *
 * The ret sizes are fixed properties of each function, so enforcing them is
 * correct at every call site, not just this one. */
ESP_FIX(sub_001BB770, 0)
ESP_FIX(sub_001BB256, 4)
ESP_FIX(sub_001BB66A, 0xC)
ESP_FIX(sub_001B8EE0, 0)

/* D3DDevice_CreateDevice tail (0x001B4F60) and the state-block writer
 * (0x001B52E0) are both split by the detector, so their pushes are popped in a
 * successor fragment the taken path never reaches. Enforcing each one's fixed
 * ret size is correct at every call site. Without the 0x001B52E0 fix the frame
 * under sub_001B5618 was shifted and D3DDevice_SetViewport was handed a stack
 * local that read back as device memory, which produced 14 garbage viewports a
 * frame. */
ESP_FIX(sub_001B4F60, 0x10)
ESP_FIX(sub_001B52E0, 16)

/* sub_001B5618 and sub_001B58AD are link_seed fragments of split parents
 * (0x001B5580 and 0x001B58A8), not callees, so they must be plain
 * pass-throughs.
 *
 * ESP_PROBE/ESP_FIX save edi/esi/ebx across the callee and write them back to
 * "enforce the callee-saved ABI". That is right for a real function and wrong
 * for a fragment, because a fragment is the tail of its parent and the pops it
 * runs ARE the parent's register restore. D3DDevice_Present's supersample
 * path leaves ebx = D3DRS_MultiSampleType & 0xF (2 on character select) in
 * sub_001B5580 and jumps into sub_001B5618, which correctly pops ebx = 0 --
 * and a wrapper then put the 2 straight back. That leaked ebx = 2 out of
 * Present into sub_00153DA0, whose multisample-enable path ends with
 * MEM8(0x90F4E5) = LO8(ebx) to clear the deferred-reset flag. The flag stayed
 * raised, the next frame took the disable branch, and the 1440x960
 * supersampled frame buffer the game had just asked for was rebuilt at
 * 720x480. Every 1440-wide viewport was then clamped to half by
 * sub_001B18A0 and the composite matrix came out exactly half of cxbx's. */
void sub_001B5618_gen(void);
void sub_001B5618(void) { sub_001B5618_gen(); }
void sub_001B58AD_gen(void);
void sub_001B58AD(void) { sub_001B58AD_gen(); }

ESP_FIX(sub_001B9130, 4)
ESP_FIX(sub_001BEA38, 4)   /* D3DDevice_ApplyStateBlock(handle), ret 4 */
ESP_FIX(sub_001BEBC0, 4)   /* D3DDevice_CaptureStateBlock(handle), ret 4 */
ESP_FIX(sub_001C35C0, 8)   /* D3DX context factory (dev, out), ret 8 */

/* DSOUND per-frame service chain: the main loop calls sub_001C760D every
 * frame; its stream-service path (sub_001C6D91 -> sub_001C883B QPC poll
 * loops) LEAKS ~850 bytes of stack per frame — after ~6600 frames the main
 * stack underflows below 0xD00000 into the game BSS and destroys the CRI
 * device tables at 0xB2xxxx (the "cvFs dies mid-boot" corruption). Probe
 * esp balance; registers ABI-enforced. */
ESP_FIX(sub_001C883B, 4)  /* DSOUND stream-service tail fragment (parent is
                           * stdcall ret 4): its body falls off the end with a
                           * -4 esp drift x thousands of poll calls per frame.
                           * NOT a restorable fall-through (recurses) - enforce
                           * the parent's contract here. */
ESP_FIX(sub_001C6D91, 4)  /* DSOUND stream service, stdcall ret 4: leaked 752
                           * bytes PER FRAME -> main stack underflowed into the
                           * game BSS after ~6600 frames, destroying the CRI
                           * device tables (the mid-boot cvFs death) */
ESP_PROBE(sub_001C760D)

/* Main-loop callee balance probes: the frame loop (sub_000A06D0) leaks
 * ~320 B/frame; find which per-frame callee drifts (prints every 600th
 * call with esp so the drift shows). */
#define LOOP_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        static unsigned s_n = 0; \
        uint32_t ei = esp; \
        fn##_gen(); \
        s_n++; \
        if ((s_n % 600) == 1) { \
             \
             } \
    }
LOOP_PROBE(sub_000A0370)
void sub_0017EC00_gen(void);
void sub_0017EC00(void)
{
    static unsigned s_n = 0;
    uint32_t ei = esp;
    sub_0017EC00_gen();
    s_n++;
    if ((s_n % 5000) == 1 && ei > 0xF00000) {   /* task-stack callers only (heap base) */
        void *bt[7]; int n = CaptureStackBackTrace(1, 7, bt, NULL);
    }
}
LOOP_PROBE(sub_0009DC40)
LOOP_PROBE(sub_0009EE80)
LOOP_PROBE(sub_00153D90)
ESP_FIX(sub_00153EC0, 0)  /* game audio-frame service: leaked 24 B/frame */
ESP_FIX(sub_00158B60, 0)  /* DSOUND buffer maintenance: leaked 24 B/frame */
LOOP_PROBE(sub_00164400)
LOOP_PROBE(sub_0009E67B)
LOOP_PROBE(sub_00153D70)
LOOP_PROBE(sub_000A02A0)
LOOP_PROBE(sub_000A0440)
ESP_FIX(sub_00153EF0, 0)  /* game sound-command flusher: leaked 628 B/frame —
                           * THE main-stack underflow driver (stack sank below
                           * 0xD00000 into the game BSS after ~2 min, wiping
                           * the CRI device tables at 0xB2xxxx) */


/* ── sub_0006D7F0 ABI-enforcement wrapper ───────────────────────────────────
 * The 2D-overlay sprite submitter. Its callee chain does not preserve esi:
 * it enters with esi = the overlay entry (0x00491B78) and returns with esi
 * pointing at the command record it just filled (0x0049A9C4). x86 cdecl makes
 * edi/esi/ebx callee-saved, and sub_00065090 relies on that -- immediately
 * after this call it does
 *      edx = [esi+4]; edx++; [esi+4] = edx
 * to advance the corner logo's fade counter. With esi clobbered the counter
 * at 0x00491B7C never leaves 0, so the alpha ramp never starts: the logo quad
 * is submitted every frame with colour 0x00FFFFFF (alpha 0) and blends away to
 * nothing. The stray write also lands inside the sprite command records.
 * Enforce the contract here until the inner culprit is root-caused, the same
 * way sub_001BE60A is handled above. */
void sub_0006D7F0_gen(void);
void sub_0006D7F0(void)
{
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_0006D7F0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* DIAG+contract: the D3DX 2D quad DRAW (vtbl slot 6 @0x1C408A, emitted
 * 2026-07-02). Called from sub_001C3F50's icall [vtbl+0x18]; this is where
 * the boot screen's glyphs/quads should turn into actual draw commands.
 * stdcall: this + 8 more args? — measure first (log d), enforce later. */
void sub_001C408A_gen(void);
void sub_001C408A(void)
{
    static int s_n = 0;
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001C408A_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    s_n++;
    if (s_n <= 3 || (s_n % 500000) == 0) {
        /* This draw runs millions of times a second and always returns
         * D3DERR_INVALIDCALL. It never ran at all before the device started
         * working, so name the native caller chain that is spinning on it
         * (the guest reaches it through an icall, so there is no static
         * call site to grep for). */
        void *bt[12]; USHORT nb = CaptureStackBackTrace(1, 12, bt, NULL); int k;
    }
}

/* ── D3D_RecordStateBlock (sub_001BE60A) ABI-enforcement wrapper ─────────────
 * The recompiled body has an inner callee with broken stack cleanup; its
 * `esp = ebp` epilogue masks the drift from callers, but the pop edi/esi/ebx
 * before it reads LEFTOVER CALL ARGS instead of the saved registers — edi came
 * back as 0x1C0800 (g_pDevice, the last pushed arg). That clobbered the D3DX
 * state-block create loop's bound (edi=2) -> infinite loop -> 500K+ 512-byte
 * allocations -> OOM. x86 ABI says edi/esi/ebx are callee-saved: enforce it
 * here until the inner culprit is root-caused. */
void sub_001BE60A_gen(void);
void sub_001BE60A(void)
{
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001BE60A_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* DIAG: D3DDevice_BeginStateBlock() — log esp at entry/exit (loop drift hunt). */
void sub_001BE5FC_gen(void);
void sub_001BE5FC(void)
{
    uint32_t esp_in = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001BE5FC_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    static int s_log = 0;
    if (s_log < 8) {
        s_log++;
    }
}

/* ── DIAG: sprite/glyph blitter (sub_000403F0) — wild-write storm source ─────
 * (struct, count) cdecl. Writes walk from a garbage dest; log the struct and
 * its dest-surface field (+0x278) to find where the bad pointer comes from. */
int g_blit_gen_hits = 0;   /* incremented by a probe at sub_000403F0_gen entry */
void sub_000403F0_gen(void);
void sub_000403F0(void)
{
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    static int s_log = 0;
    if (s_log < 10) {
        s_log++;
    }
    sub_000403F0_gen();
}

/* ── DIAG: boot-screen record-stream parser (sub_00080F10) ───────────────────
 * arg1 = loaded blob (12-byte header, then records tagged &0x70000, term
 * 0x80000000), arg2 = idx, arg3 = mode (0/1/2). Dump the blob header + first
 * records to see whether the file content actually loaded. */
void sub_00080F10_gen(void);
void sub_00080F10(void)
{
    uint32_t blob = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    static int s_log = 0;
    if (s_log < 8) {
        s_log++;
    }
    sub_00080F10_gen();
}

/* ── DIAG: boot-screen definition loader (sub_00080D00) ──────────────────────
 * Walks a loaded file blob: MEM32(blob+4)=list head, blob+0x14=offset table,
 * entries at offset+(blob-0x14). Crashes walking wild pointers -> suspect the
 * blob is garbage (its file never read). Log args + blob header to confirm. */
void sub_00080D00_gen(void);
void sub_00080D00(void)
{
    uint32_t a1 = MEM32(esp + 4), blob = MEM32(esp + 8);
    uint32_t a3 = MEM32(esp + 0xC), a4 = MEM32(esp + 0x10);
    static int s_log = 0;
    if (s_log < 12) {
        s_log++;
    }
    sub_00080D00_gen();
}

/* ── D3D8 push-buffer overrides (following burnout3's PB-trio pattern) ────────
 * burnout3 overrides its push-buffer management functions (sub_003518E0 kickoff,
 * sub_00351770 make-space, sub_00351A20 flush) in recomp_manual.c to: keep the
 * device write cursor inside a real RAM buffer, and simulate the GPU having
 * consumed the buffer + acked the fence (so the D3D8LTCG spin loops exit).
 * docs/d3d8ltcg_device_context.md: "device+0x30 -> device+0x2C ... GPU_read ==
 * write -> no spin loop".
 *
 * DOA3 is retail D3D8 (not D3D8LTCG); the cxbx symbol cache names the exact
 * analogues:  CDevice_KickOff=0x1B88C0, CDevice_MakeSpace=0x1B8B00,
 * XMETAL_StartPush=0x1B8DA0.  Device PB fields (mapped by reading the gen code):
 *   device[0]        = PB write cursor      device+0x04   = PB segment limit
 *   device+0x2304    = fence notifier ptr   notifier+0x44 = GPU-acked fence value
 *   device+0x2518    = FIFO GET (GPU read)  device+0x2B60 = FIFO PUT (CPU write)
 * The fence waits compare (MEM32(device) ^ MEM32(notifier+0x44)) & 0xFFFFFFF, so
 * setting notifier+0x44 = device[0] makes them self-equal -> always satisfied
 * (the "GPU read == write" trick, adapted to DOA3's notifier layout).
 * RAM push buffer is allocated in main.c. */
/* Flags carried across a generated-fragment boundary; see recomp_types.h.
 * Exact x86 flag semantics for CMP (a - b) and TEST (a & b) at the operand
 * width the compare used. */
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
uint32_t g_doa3_movie_ticks = 0;    /* synthetic movie clock (PTS units) */
uint32_t g_doa3_movie_frames = 0;   /* frames actually blitted (clock pace anchor) */
static uint32_t g_pb_parsed = 0;   /* last push-buffer offset translated to D3D11 */

/* Push-buffer -> D3D11 translation (following burnout3's parse_live_pushbuffer).
 * Walk new push-buffer dwords [from, to), decode NV2A Kelvin method headers, and
 * dispatch each method to the shared pgraph_d3d11 translator (renders through the
 * host D3D8->D3D11 device). NV097_CLEAR_SURFACE (0x1D0) begins a frame, so present
 * the previous frame's accumulated draws when we hit one. */
#define NV097_CLEAR_SURFACE 0x01D0u
static void doa3_translate_pb(uint32_t from, uint32_t to)
{
    extern int  pgraph_d3d11_method(int subchannel, uint32_t method, uint32_t param);
    extern void pgraph_d3d11_flush(void);
    extern void doa3_present_frame(void);
    static int drew_since_present = 0;

    (void)drew_since_present;
    uint32_t pos = from;
    /* DOA3 DIAG: words the parser cannot read as a method header, and
     * headers whose parameters run past the write cursor. Either one means
     * the parse is out of step with the guest's stream. */
    extern volatile int g_doa3_post_movie;
    static uint32_t s_skip = 0, s_over = 0, s_hdrs = 0; static int s_log = 0;
    static DWORD s_next = 0;
    uint32_t last_hdr = 0, last_pos = 0;
    while (pos + 4 <= to) {
        uint32_t word = MEM32(pos); pos += 4;
        if (word == 0) continue;                       /* NOP / padding */
        uint32_t kind = word & 0xE0030003u;
        if (kind == 0 || kind == 0x40000000u) {        /* increasing / non-increasing */
            uint32_t count   = (word >> 18) & 0x7FF;
            uint32_t method  = word & 0x1FFC;
            uint32_t subchan = (word >> 13) & 7;
            s_hdrs++;
            if (count == 0 || pos + count * 4 > to) {
                s_over++;
                if (g_doa3_post_movie && s_log < 40) { s_log++;
                     }
                continue;
            }
            last_hdr = word; last_pos = pos - 4 - from;
            for (uint32_t i = 0; i < count; i++) {
                uint32_t param = MEM32(pos); pos += 4;
                uint32_t m = (kind == 0) ? method + i * 4 : method;
                pgraph_d3d11_method((int)subchan, m, param);   /* translate to D3D11 */
            }
        }
        else {
            s_skip++;
            {   static uint32_t s_ctx = 0; static uint32_t s_ctx_from = 0;
                if (g_doa3_post_movie && s_ctx < 8 && s_ctx_from != from) { s_ctx++; s_ctx_from = from;
                    uint32_t k, b = (pos - 4 - from >= 48) ? pos - 4 - 48 : from;
                      }
            }
            if (g_doa3_post_movie && s_log < 40) { s_log++;
                 }
        }
        /* jump/call/return (low bits 1/2) carry no params we translate — skip */
    }
    if (g_doa3_post_movie && GetTickCount() >= s_next) {
        s_next = GetTickCount() + 2000;
    }
}

/* Wrap the write cursor back to the base of our RAM push buffer.
 *
 * Burnout's sub_00351770 parses [base, write_ptr) BEFORE it resets the
 * cursor. The DOA3 wrap sites below used to reset the cursor and leave
 * g_pb_parsed where it was, relying on the next KickOff to notice
 * g_pb_parsed > cursor and restart from the base. That only works if the
 * next kick is shorter than the old parsed offset: a frame that needs more
 * than the 1 MB buffer wraps mid-frame ([PBFULL] shows single kicks at
 * 786 KB), and the following kick then translated from an arbitrary point
 * inside the new stream -- index and vertex data read as method headers,
 * which put packed shorts into SET_SURFACE_PITCH/OFFSET, SET_VIEWPORT_SCALE
 * and SET_CLIP_MIN/MAX and routed the whole scene offscreen (the freeze
 * after the title stage). Everything written since the last kick was also
 * dropped. Translate the pending range, then reset both together. */
/* Recording mode. The XDK sets bit 2 of [dev+0xC] while the device records
 * into a scratch buffer (state-block / push-buffer-object recording): [dev]
 * then points at that scratch area (seen at 0x00E00244, in the guest stack
 * region), the live ring cursor is parked at [dev+0x400], and the original
 * MakeSpace (0x001B8B00, flag-4 branch) rewinds the scratch cursor to the
 * chunk base [[dev+0x3F4]+4] and adds the bytes written to [dev+0x3F8].
 * The overrides below used to treat that scratch cursor as "outside our
 * push buffer" and drag it to the ring base, so recorded commands were
 * written over the live stream and the parser was restarted on them:
 * 33 of the 40 logged wraps in rt163 were this case. */
static int doa3_pb_recording(uint32_t dev)
{
    return (MEM8(dev + 0xC) & 4) != 0;
}

/* Pass stage-0 COLOROP/ARG1/ARG2 (TSS table 0x1C0180) and whether stage 1 is
 * disabled (bit 17) to the translator as an NV2A NOP before each draw: the
 * combiner words are zero because sub_001BCC00 never completes. */
void doa3_pb_tss_marker(void)
{
    uint32_t dev = MEM32(0x1C3390), cursor;
    if (!dev || doa3_pb_recording(dev) || !g_doa3_pb_base) return;
    cursor = MEM32(dev);
    if (cursor < g_doa3_pb_base || cursor + 0x1000 >= g_doa3_pb_end) return;
    MEM32(cursor) = 0x00040100u;
    MEM32(cursor + 4) = 0xA3000000u | (MEM32(0x1C0200) == 1 ? 1u << 17 : 0) |
                        ((MEM32(0x1C0180) & 0x1Fu) << 12) |
                        ((MEM32(0x1C0188) & 0x3Fu) << 6) | (MEM32(0x1C018C) & 0x3Fu);
    /* Stage 3 (0x80 bytes per stage; ALPHAOP is four entries past COLOROP):
     * the point-sprite stage, whose ops the snow is combined with. */
    MEM32(cursor + 8)  = 0x00040100u;
    MEM32(cursor + 12) = 0xA4000000u | ((MEM32(0x1C0300) & 0x1Fu) << 12) |
                         ((MEM32(0x1C0308) & 0x3Fu) << 6) | (MEM32(0x1C030C) & 0x3Fu);
    MEM32(cursor + 16) = 0x00040100u;
    MEM32(cursor + 20) = 0xA5000000u | ((MEM32(0x1C0310) & 0x1Fu) << 12) |
                         ((MEM32(0x1C0318) & 0x3Fu) << 6) | (MEM32(0x1C031C) & 0x3Fu);
    /* Stage 1 colour / alpha ops (0xA6 / 0xA7) and D3DRS_TEXTUREFACTOR
     * (0xA9 = low 24 bits, 0xAA = high byte) from the deferred render-state
     * table at 0x1C0580: the combiner factor the push buffer carries is not
     * the game's value (see nv2a_pgraph_d3d11.c). */
    MEM32(cursor + 24) = 0x00040100u;
    MEM32(cursor + 28) = 0xA6000000u | ((MEM32(0x1C0200) & 0x1Fu) << 12) |
                         ((MEM32(0x1C0208) & 0x3Fu) << 6) | (MEM32(0x1C020C) & 0x3Fu);
    MEM32(cursor + 32) = 0x00040100u;
    MEM32(cursor + 36) = 0xA7000000u | ((MEM32(0x1C0210) & 0x1Fu) << 12) |
                         ((MEM32(0x1C0218) & 0x3Fu) << 6) | (MEM32(0x1C021C) & 0x3Fu);
    MEM32(cursor + 40) = 0x00040100u;
    MEM32(cursor + 44) = 0xA9000000u | (MEM32(0x1C0580) & 0x00FFFFFFu);
    MEM32(cursor + 48) = 0x00040100u;
    MEM32(cursor + 52) = 0xAA000000u | (MEM32(0x1C0580) >> 24);
    /* Stage 0 alpha op / args (0xAB): the castle's stone walls run stage 0 as
     * MODULATEALPHA_ADDCOLOR, where the DXT5 alpha is a lighting mask, and the
     * translator needs the guest's own alpha op to know what feeds the blend. */
    MEM32(cursor + 56) = 0x00040100u;
    MEM32(cursor + 60) = 0xAB000000u | ((MEM32(0x1C0190) & 0x1Fu) << 12) |
                         ((MEM32(0x1C0198) & 0x3Fu) << 6) | (MEM32(0x1C019C) & 0x3Fu);
    MEM32(dev) = cursor + 64;
}
static uint32_t doa3_pb_makespace_recording(uint32_t dev)
{
    uint32_t cursor = MEM32(dev);
    uint32_t desc   = MEM32(dev + 0x3F4);
    uint32_t chunk  = MEM32(desc + 4);
    extern volatile int g_doa3_post_movie;
    static int s_n = 0;
    MEM32(dev + 0x3F8) = MEM32(dev + 0x3F8) + (cursor - chunk);
    MEM32(dev) = chunk;
    if (g_doa3_post_movie && s_n < 12) { s_n++;
         }
    return chunk;
}

static void doa3_pb_wrap(uint32_t dev, const char *who)
{
    uint32_t cursor = MEM32(dev);
    extern volatile int g_doa3_post_movie;
    static int s_n = 0;
    if (cursor > g_pb_parsed && g_pb_parsed >= g_doa3_pb_base && cursor <= g_doa3_pb_end)
        doa3_translate_pb(g_pb_parsed, cursor);
    if (g_doa3_post_movie && s_n < 40) { s_n++;
         }
    {   /* DOA3 DIAG: a cursor outside our buffer altogether was not put
         * there by any of our overrides; name the guest routine that did. */
        static int s_odd = 0;
        if (g_doa3_post_movie && s_odd < 8 &&
            (cursor < g_doa3_pb_base || cursor > g_doa3_pb_end)) {
            void *bt[14]; USHORT nf = CaptureStackBackTrace(1, 14, bt, NULL); USHORT k;
            s_odd++;
        }
    }
    MEM32(dev + 0x00) = g_doa3_pb_base;              /* write cursor = base */
    MEM32(dev + 0x04) = g_doa3_pb_end;               /* segment limit = end */
    g_pb_parsed = g_doa3_pb_base;
}

/* CDevice_KickOff (0x1B88C0) — burnout sub_003518E0 analogue. fastcall (ecx=this),
 * ret 0. Simulate GPU drain + fence ack so the FIFO/fence spin loops exit. */
static unsigned g_kick_count = 0;
void sub_001B88C0(void)
{
    { extern void doa3_ptinfo_check(const char *); doa3_ptinfo_check("kickoff-in"); }
    uint32_t ctx = ecx;                              /* this-pointer (device/context) */
    if (ctx && ctx < 0x08000000u) {
        /* device[0] = current write cursor; while recording (flag 4) the
         * original reads the parked ring cursor from device+0x400 instead. */
        uint32_t cursor = doa3_pb_recording(ctx) ? MEM32(ctx + 0x400) : MEM32(ctx);
        uint32_t notifier = MEM32(ctx + 0x2304);
        if ((++g_kick_count % 10000) == 0) {
        }
        {   /* DOA3 DIAG: a cursor outside our buffer means the guest wrote
             * commands somewhere we never translate (and, past the end,
             * onto guest VA 0 through its 26-bit address mask). */
            extern volatile int g_doa3_post_movie;
            static int s_n = 0;
            if (g_doa3_post_movie && s_n < 12 && g_doa3_pb_base &&
                (cursor < g_doa3_pb_base || cursor > g_doa3_pb_end)) { s_n++;
                 }
        }
        /* Translate the push-buffer commands written since the last kick to D3D11
         * (only for the main device's RAM push buffer). */
        if (g_doa3_pb_base && cursor >= g_doa3_pb_base && cursor <= g_doa3_pb_end) {
            if (g_pb_parsed < g_doa3_pb_base || g_pb_parsed > cursor)
                g_pb_parsed = g_doa3_pb_base;        /* (re)sync after a wrap */
            if (cursor > g_pb_parsed) {
                doa3_translate_pb(g_pb_parsed, cursor);
                g_pb_parsed = cursor;
            }
        }
        /* Publish the write cursor as both PUT and GET.
         *
         * The notifier is normally in RAM or its cached mirror, but on the
         * retail path device+0x2304 points at the FIFO channel window in the
         * GPU aperture (0xFD800000): +0x40 is DMA_PUT and +0x44 is DMA_GET, and
         * D3DDevice_CreateDevice spins at 0x001B98B8 until GET matches the
         * cursor. The old guard stopped at 0xF0000000, so on that path neither
         * register was ever written and the spin could not exit -- the whole
         * device setup, including the initial SetRenderTarget, sat behind it.
         * Writes into the aperture go through the VEH to the NV2A USER block. */
        if (notifier && (notifier < 0xF0000000u ||
                         (notifier >= 0xFD000000u && notifier < 0xFE000000u))) {
            MEM32(notifier + 0x40) = cursor;         /* DMA_PUT (as the gen does) */
            MEM32(notifier + 0x44) = cursor;         /* DMA_GET == PUT -> drained */
        }
        /* GPU-side fence counter: [dev+0x3F0] points at the value the fence
         * spin (sub_001B8A10) polls; snap it to the CPU counter [dev+0x1C]. */
        {
            uint32_t fptr = MEM32(ctx + 0x3F0);
            if (fptr && fptr < 0xF0000000u) MEM32(fptr) = MEM32(ctx + 0x1C);
        }
        {   /* The push buffer is deliberately small (main.c). Say so loudly if
             * a single kick ever gets close to filling it, rather than letting
             * commands run off the end silently. */
            static int s_warned = 0;
            if (!s_warned && g_doa3_pb_end > g_doa3_pb_base &&
                cursor >= g_doa3_pb_base &&
                cursor - g_doa3_pb_base >
                    (g_doa3_pb_end - g_doa3_pb_base) / 4 * 3) {
                s_warned = 1;
            }
        }
        MEM32(ctx + 0x18)   = cursor;
        MEM32(ctx + 0x2518) = MEM32(ctx + 0x2B60);   /* FIFO drained: GET = PUT */
    }
    esp += 4;   /* ret 0 */
}

/* Render command-list walker (0x00158DE0) — cdecl(end, start), ret 0.
 *
 * The list is built by sub_00158BE0 as a flat run of records: type 0 is a
 * 0x290-byte state block, type 1 is 0xC bytes, type 2 is 0x14 bytes, and
 * every builder call appends at least the type-2 record.  The walker reads
 * the type word, dispatches, and adds the matching size.  A type word that
 * is not 0/1/2 sends it to loc_00158FC5, which only re-tests the bound —
 * ebx never advances, so the guest spins at 100% CPU with the process
 * alive.  That is the post-title hang: every [WDOG] report has the same
 * rip inside sub_00158DE0 and an unchanging ebx.
 *
 * The list does not overflow (watermark: 60 KB peak of 1 MB), so the chain
 * itself is breaking.  Replay it here before the walk, read-only, and say
 * where and on what.  The walk itself is unchanged. */
uint32_t g_walk_start, g_walk_end, g_walk_canary;
void doa3_walk_canary(const char *callee)
{
    /* Canary for the wild write that repaints the render list mid-walk.
     * [0x00B1F388] and [0x00B1F38C] are the two per-frame indirect-call slots
     * the list reset sub_00158B60 sets to 0x0017EC00 once a frame; nothing
     * else in the frame writes them, yet the stall dump finds them holding
     * the same junk as the list.  Check them after every call the walker
     * makes and name the callee that returned with them broken. */
    extern volatile int g_doa3_post_movie;
    extern volatile LONG g_doa3_heartbeat;
    static int told = 0;
    if (!g_doa3_post_movie || told >= 6) return;
    if (MEM32(0x00B1F388u) == 0x0017EC00u) return;   /* B1F38C carries a real game callback, not the init value */
    told++;
}

/* Per-record trace of the walk itself (called at loc_00158E20, once per
 * record).  The list validates clean at every entry and ebx survives every
 * callee, yet the walk still lands on a word that is not a record type, so
 * record what the walk actually visits and dump the tail when it breaks. */
void doa3_walk_step(uint32_t rec)
{
    extern volatile int g_doa3_post_movie;
    extern volatile LONG g_doa3_heartbeat;
    static uint32_t ring[12]; static unsigned n = 0; static int reported = 0;
    uint32_t t = MEM32(rec);
    ring[n % 12] = rec; n++;
    {   /* Who else touches this buffer while the walk is reading it?
         * sub_00158B20 repoints the RECORDING write pointer [0x99A1FC] at
         * 0xA1F388 -- the same 1 MB buffer the walk is reading -- and sets
         * the recording flag [0xA1F384]; the append path sub_00158D60 then
         * writes state-block records over it from the base.  Nothing moves
         * [0xB1F390], so the earlier write-pointer and canary checks could
         * not see it.  Latch the frame state at the first record of a walk
         * and report the first thing that changes under it. */
        extern int xbox_fiber_current(void);
        static uint32_t w0, f0, rp0, wp0; static int fib0, armed = 0, told = 0;
        if (w0 != g_walk_start || wp0 != g_walk_end) {
            w0 = g_walk_start; wp0 = g_walk_end;
            f0 = MEM32(0x00A1F384u); rp0 = MEM32(0x0099A1FCu);
            fib0 = xbox_fiber_current(); armed = 1;
        } else if (armed && g_doa3_post_movie && told < 6) {
            uint32_t f = MEM32(0x00A1F384u), rp = MEM32(0x0099A1FCu);
            int fib = xbox_fiber_current();
            if (f != f0 || rp != rp0 || fib != fib0) { told++; armed = 0;
                 }
        }
    }
    if (t <= 2 || !g_doa3_post_movie || reported >= 4) return;
    reported++;
    {
        unsigned i, first = (n > 12) ? n - 12 : 0;
        uint32_t p, good = 0; unsigned gi = 0;
        for (i = first; i < n; i++) {
            uint32_t r = ring[i % 12];
        }
        for (p = g_walk_start; p < g_walk_end; ) {
            uint32_t ct = MEM32(p);
            if (ct > 2) break;
            if (p <= rec) { good = p; gi++; } else break;
            p += (ct == 0) ? 0x290u : (ct == 1) ? 0xCu : 0x14u;
        }
    }
}
int      g_walk_active;
void sub_00158DE0_gen(void);
void sub_00158DE0(void)
{
    extern volatile int g_doa3_post_movie;
    extern volatile LONG g_doa3_heartbeat;
    static int s_bad = 0, s_ok = 0;
    uint32_t end   = MEM32(esp + 4);
    uint32_t start = MEM32(esp + 8);

    if (g_doa3_post_movie && s_bad < 8 && end > start && end - start < 0x200000u) {
        uint32_t p = start, prev[4] = { 0, 0, 0, 0 }, n = 0;
        while (p < end) {
            uint32_t t = MEM32(p);
            if (t > 2) {
                int k;
                s_bad++;
                break;
            }
            prev[0] = prev[1]; prev[1] = prev[2]; prev[2] = prev[3]; prev[3] = p;
            p += (t == 0) ? 0x290u : (t == 1) ? 0xCu : 0x14u;
            n++;
        }
        if (p == end && s_ok < 3) { s_ok++;
             }
        else if (p > end && s_bad < 8) { s_bad++;
             }
    }

    g_walk_start  = start;
    g_walk_end    = end;
    g_walk_canary = (end > start + 0x14u) ? MEM32(end - 0x14u) : 0;
    g_walk_active++;
    sub_00158DE0_gen();
    g_walk_active--;
}

/* CDevice_MakeSpace (0x1B8B00) — burnout sub_00351770 analogue. fastcall (ecx=this),
 * ret 0. GPU consumed everything -> reset the write cursor to our RAM buffer. */
void sub_001B8B00(void)
{
    uint32_t ctx = ecx;
    if (ctx && ctx < 0x04000000u && doa3_pb_recording(ctx)) {
        eax = doa3_pb_makespace_recording(ctx);
        esp += 4;   /* ret 0 */
        return;
    }
    if (ctx && ctx < 0x04000000u && g_doa3_pb_base)
        doa3_pb_wrap(ctx, "MakeSpace");
    esp += 4;   /* ret 0 */
}

/* D3D_SetFence (0x1B8970) wrapper — burnout _gen pattern (the gen body ends with a
 * clean ret 4, so this is safe). D3DDevice_Present calls SetFence with arg==2 once
 * per frame; that's our frame boundary. Run the real fence logic, then flush the
 * translated draws and flip the host D3D8->D3D11 swapchain. */

/* D3D fence wait (0x1B8A10, cxbx: BlockOnFence internal) — spins until the
 * GPU-side fence counter MEM32([dev+0x3F0]) catches the CPU counter [dev+0x1C].
 * Our pgraph translation is synchronous (draws complete inside KickOff), so
 * every fence is already retired: no-op. This unblocked movie blit #3, which
 * parked in Lock2DSurface -> D3D_BlockOnResource -> this spin (item 80). */
void sub_001B8A10(void)
{
    static int s_n = 0;
    if (s_n < 6) {   }
    s_n++;
    esp += 12;  /* fake-ret slot + ret 8 */
}

/* DIAG: block store (dequant/IDCT output). Called from the MB workers.
 * item 103 GUARD: negative a2 resolves the real count from the table at
 * 0xBF27C8-4*a2 and tail-calls the MMX kernel sub_00186440, which
 * transforms (count/4) x 64-byte chunks IN PLACE from a1 upward. One call
 * with a garbage count/pointer becomes a LINEAR MEMORY SWEEP — caught
 * red-handed by the [SLOTST] write-watch in the user's stalled run
 * (writer rips = 4 unroll lanes of sub_00186440 smashing PCM-like values
 * over the sfdec slot table 0xC12E2C+, killing the movie ~5 frames in =
 * item-95 #1 stop + #5 half frames). Skip insane calls instead. */
void sub_00186D00_gen(void);
void sub_00186D00(void)
{
    static int s_n = 0; static int s_guard = 0;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8);
    int32_t n = (int32_t)a2;
    uint32_t cnt = (n < 0) ? MEM32(0xBF27C8u - 4u * a2) : a2;
    if (a1 < 0x100000u || a1 >= 0x8000000u || (int32_t)cnt < 0 || cnt > 2048u) {
        if (s_guard < 12) {
            s_guard++;
        }
        eax = 0;
        esp += 4;   /* cdecl ret: pop dummy return */
        return;
    }
    sub_00186D00_gen();
    if (s_n < 10 || (s_n % 8192) == 0) {
    }
    s_n++;
}

/* DIAG/OVERRIDE: CSC coefficient builder (matrix invert). Dump in/out. */
static double doa3_memd(uint32_t a) { 
    uint64_t q = (uint64_t)MEM32(a) | ((uint64_t)MEM32(a + 4) << 32);
    double d; memcpy(&d, &q, 8); return d;
}
static void doa3_stored(uint32_t a, double d)
{
    uint64_t q; memcpy(&q, &d, 8);
    MEM32(a) = (uint32_t)q; MEM32(a + 4) = (uint32_t)(q >> 32);
}
/* Full C override (item 81): the x87 emission of this 3x3 matrix inversion
 * came out subtly wrong (row 2 badly off -> Y-table lanes unequal -> the
 * whole movie rendered green). src = forward RGB->YUV matrix (row-major),
 * dst = inverse (YUV->RGB, rows = R,G,B lanes) x MEMD(0x217A38) (=64, the
 * table fixed-point scale). Offsets/rounding are separate ROM constants
 * used by the table builder, not computed here. */
void sub_00175EC0(void)
{
    static int s_n = 0;
    uint32_t src = MEM32(esp + 4), dst = MEM32(esp + 8);
    double a[9], inv[9];
    for (int k = 0; k < 9; k++) a[k] = doa3_memd(src + 8u * k);
    double det = a[0]*(a[4]*a[8]-a[5]*a[7])
               - a[1]*(a[3]*a[8]-a[5]*a[6])
               + a[2]*(a[3]*a[7]-a[4]*a[6]);
    double s = doa3_memd(0x217A38) / det;
    inv[0]= (a[4]*a[8]-a[5]*a[7])*s; inv[1]=-(a[1]*a[8]-a[2]*a[7])*s; inv[2]= (a[1]*a[5]-a[2]*a[4])*s;
    inv[3]=-(a[3]*a[8]-a[5]*a[6])*s; inv[4]= (a[0]*a[8]-a[2]*a[6])*s; inv[5]=-(a[0]*a[5]-a[2]*a[3])*s;
    inv[6]= (a[3]*a[7]-a[4]*a[6])*s; inv[7]=-(a[0]*a[7]-a[1]*a[6])*s; inv[8]= (a[0]*a[4]-a[1]*a[3])*s;
    for (int k = 0; k < 9; k++) doa3_stored(dst + 8u * k, inv[k]);
    if (s_n < 2) {
    }
    s_n++;
    esp += 4;   /* cdecl */
}

/* DIAG: CSC plane-info fill: (frame, w?, h?, &out). Dump the out struct. */
void sub_001777C0_gen(void);
void sub_001777C0(void)
{
    static int s_n = 0;
    uint32_t a4 = MEM32(esp + 0x10);
    sub_001777C0_gen();
    if (s_n < 6) {
    }
    s_n++;
}

/* PSGSFD parse-resume continuations (item 82): the incremental bitstream
 * parser suspends mid-picture when the ES queue runs dry and later resumes
 * through saved continuation pointers (0x1E5B0C/0x1E5C23 = P/I resume,
 * 0x1E63D2/0x1E6E02 = helpers). These same slots hold GARBAGE during boot
 * (the decoder pool is memset-then-defaulted and probed once with junk
 * registers) — resolving them unconditionally wedged boot. Guard: only run
 * the real code while a movie is PLAYING; otherwise keep the old harmless
 * no-op behavior. */
#define RESUME_GUARD(fn)     void fn##_gen(void);     void fn(void) {         if (MEM32(0xC0F7C0 + 0x40) == 4) {             static int s_rg = 0;             if (s_rg < 4) { s_rg++;                   }             fn##_gen(); return; }         eax = 0; esp += 4; }
RESUME_GUARD(sub_001E5B0C)
RESUME_GUARD(sub_001E5C23)
RESUME_GUARD(sub_001E63D2)
RESUME_GUARD(sub_001E6E02)
#undef RESUME_GUARD

/* DIAG: MB-worker esp anomaly probes (the -36 hunt). Print only imbalances. */
#define ESPANOM_PROBE(fn)     void fn##_gen(void);     void fn(void) {         static int s_bad = 0;         uint32_t e0 = esp;         fn##_gen();         int _d = (int)(esp - e0);         if (_d != 4 && _d != 28 && s_bad < 12) {             s_bad++;                           } }
ESPANOM_PROBE(sub_001E5040)
ESPANOM_PROBE(sub_001E5500)
ESPANOM_PROBE(sub_0018137A)
ESPANOM_PROBE(sub_0018137C)
ESPANOM_PROBE(sub_001E3670)
ESPANOM_PROBE(sub_001E3060)
ESPANOM_PROBE(sub_001E4E80)
ESPANOM_PROBE(sub_001E3520)
#undef ESPANOM_PROBE

/* DIAG: skip-variant picture "decoders" (type-table entries 4/5) — esp
 * balance probes for the -36 leak in the skip chain. */
#define SKIPSTUB_PROBE(fn)     void fn##_gen(void);     void fn(void) {         static int s_n = 0;         uint32_t e0 = esp;         fn##_gen();         if (s_n < 10) {                           }         s_n++; }
SKIPSTUB_PROBE(sub_001E32D0)
SKIPSTUB_PROBE(sub_001E3010)
#undef SKIPSTUB_PROBE

/* DIAG: I-picture decoder entry — dump the MB worker pointers. */
void sub_001E4C40_gen(void);
void sub_001E4C40(void)
{
    static int s_n = 0;
    uint32_t c = MEM32(esp + 4);
    if (s_n < 6) {
        uint32_t ok = (c >= 0x1000 && c < 0x8000000u);
    }
    sub_001E4C40_gen();
    if (s_n < 6) {
    }
    s_n++;
}

/* DIAG: post-decode frame handoff (h, cls, fridx) — display-queue push? */
void sub_0017CD60_gen(void);
void sub_0017CD60(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), cls = MEM32(esp + 8), fr = MEM32(esp + 0xC);
    sub_0017CD60_gen();
    if (s_n < 14 || (s_n % 2048) == 0) {
    }
    s_n++;
}

/* GUARD (task #1/EOS): post-EOS the free-running video service can call the
 * display gate with a junk handle (junk esi propagation) -> wild reads ->
 * crash. Standard in-range object guard (item 73 family). */
void sub_0017DA20_gen(void);
void sub_0017DA20(void)
{
    uint32_t h = MEM32(esp + 4);
    if (!(h >= 0x1000 && h < 0x8000000u) || MEM32(h + 0x3668) > 16) {
        /* junk handle OR in-range residue with a garbage slot COUNT — the
         * gen walks count*0x50 bytes and runs off the map (post-EOS crash) */
        eax = 0xFFFFFFFFu;   /* "no displayable frames" */
        esp += 4;
        return;
    }
    sub_0017DA20_gen();
}

/* DIAG: display-side frame pop (sub_0017EE50(h,q,&out)) + frame gate
 * (sub_0017EAD0(h,frame)) — the FPEEK "new frame?" chain. */
void sub_0017EE50_gen(void);
static uint32_t g_doa3_display_frame;
static int doa3_display_trace_enabled(void)
{
    static int enabled = -1;
    if (enabled < 0)
        enabled = GetEnvironmentVariableA("DOA3_DISPLAYTRACE", NULL, 0) != 0;
    return enabled;
}

void sub_0017EE50(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), q = MEM32(esp + 8), outp = MEM32(esp + 0xC);
    sub_0017EE50_gen();
    if (doa3_display_trace_enabled()) {
        uint32_t frame = (outp >= 0x1000 && outp < 0x8000000u) ? MEM32(outp) : 0;
        if (frame >= 0x1000 && frame < 0x8000000u)
            g_doa3_display_frame = frame;
    }
    if (s_n < 40 || (s_n % 2048) == 0) {
        uint32_t rec = h + q * 0x388u + 0xD34u;
        uint32_t up = MEM32(rec + 0x360);
        uint32_t vt2 = MEM32(h + 2u * 0x610u + 0x2F4Cu);
        uint32_t fr2 = (outp >= 0x1000 && outp < 0x8000000u) ? MEM32(outp) : 0;
        /* item 104: dump the FULL popped frame record — the serve reads pts 0
         * and always plane 0x1AE0A20; these dwords name the record fields */
        if (s_n < 12 && fr2 >= 0x1000 && fr2 < 0x8000000u) {
        }
    }
    s_n++;
}
void sub_0017EAD0_gen(void);
void sub_0017EAD0(void)
{
    static int s_n = 0;
    uint32_t h = MEM32(esp + 4), fr = MEM32(esp + 8);
    sub_0017EAD0_gen();
    if (s_n < 12 || (s_n % 2048) == 0) {
    }
    s_n++;
}

/* DIAG: picture-index entry matcher (fastcall ecx=idxstruct; stack args
 * {pic, ringsize, ringbase}). Returns steps-to-entry or -1. */
void sub_00182630_gen(void);
void sub_00182630(void)
{
    static int s_n = 0;
    uint32_t st = ecx;
    uint32_t a1 = MEM32(esp + 4), a2 = MEM32(esp + 8), a3 = MEM32(esp + 0xC);
    sub_00182630_gen();
    if (s_n < 10 || (s_n % 1024) == 0) {
    }
    s_n++;
}

/* DIAG: decode-vs-skip decision (every picture currently skips -> zero YUV).
 * eax(entry)=handle. params array at h+0x994: [0x2F]=decode-all, [0x27]=skip. */
void sub_0017AEB0_gen(void);
void sub_0017AEB0(void)
{
    static int s_n = 0;
    uint32_t h = eax;
    sub_0017AEB0_gen();
    if (s_n < 16) {
        uint32_t ok = (h >= 0x1000 && h < 0x8000000u);
    }
    s_n++;
}

/* DIAG: mwPly frame copy+CSC (blit #3 hangs inside sub_0009DE90 — bisect
 * between the D3D surface lock and this). */
void sub_001762B0_gen(void);
void sub_001762B0(void)
{
    static int s_n = 0;
    static uint32_t display_n = 0;
    uint32_t a[6];
    for (int k = 0; k < 6; k++) a[k] = MEM32(esp + 4 + 4u * k);
    doa3_compare_completed_plane(a[0] & 0x07FFFFFFu);
    /* item 90: suspended pictures' slot dims fields can be zero (their
     * frame-locals were written at a different drift level). Dims are
     * stream-constant — substitute the sequence-header values. */
    if (a[2] == 0 || a[3] == 0) {
        uint32_t w = MEM32(0xC0F7C0 + 0x35A8), h = MEM32(0xC0F7C0 + 0x35AC);
        if (w == 0 || w > 1024) w = 0x2D0;
        if (h == 0 || h > 1024) h = 0x1E0;
        MEM32(esp + 4 + 8) = w;  a[2] = w;
        MEM32(esp + 4 + 12) = h; a[3] = h;
        static int s_dz = 0;
        if (s_dz < 6) { s_dz++;
              }
    }
    if (s_n < 8) {
        uint32_t src = a[0] & 0x07FFFFFFu, ysum = 0, usum = 0, vsum = 0;
        if (src >= 0x1000 && src < 0x7F00000u)
            for (int k = 0; k < 64; k++) {
                ysum += MEM32(src + 0x40000 + 4u * k);
                usum += MEM32(src + 0x54600 + 0x8000 + 4u * k);  /* U plane */
                vsum += MEM32(src + 0x69780 + 0x8000 + 4u * k);  /* V plane */
            }
    }
    if (doa3_display_trace_enabled() && display_n < 40) {
        uint32_t src = a[0] & 0x07FFFFFFu;
        uint32_t frame = g_doa3_display_frame;
        uint32_t expected = (frame >= 0x1000 && frame < 0x8000000u) ? MEM32(frame + 0x20) : 0;
        char path[64];
        snprintf(path, sizeof path, "display_yuv_%04u.raw", display_n);
        FILE *dump = fopen(path, "wb");
        if (dump && src >= 0x1000 && src + 736u * 480u + 2u * 384u * 240u < 0x8000000u) {
            const uint8_t *y = (const uint8_t *)XBOX_PTR(src);
            const uint8_t *u = y + 736u * 480u;
            const uint8_t *v = u + 384u * 240u;
            for (int row = 0; row < 480; row++) fwrite(y + row * 736, 1, 720, dump);
            for (int row = 0; row < 240; row++) fwrite(u + row * 384, 1, 360, dump);
            for (int row = 0; row < 240; row++) fwrite(v + row * 384, 1, 360, dump);
        }
        if (dump) fclose(dump);
        FILE *index = fopen("display_index.csv", display_n ? "ab" : "wb");
        if (index) {
            if (!display_n)
                fputs("display,descriptor,plane,expected_plane,width,height,field2,field3,field4,pts,rate,type\n", index);
            fprintf(index, "%u,%08X,%08X,%08X,%u,%u,%u,%u,%u,%u,%u,%u\n",
                    display_n, frame, src, expected,
                    frame ? MEM32(frame) : 0, frame ? MEM32(frame + 4) : 0,
                    frame ? MEM32(frame + 8) : 0, frame ? MEM32(frame + 0xC) : 0,
                    frame ? MEM32(frame + 0x10) : 0, frame ? MEM32(frame + 0x14) : 0,
                    frame ? MEM32(frame + 0x18) : 0, frame ? MEM32(frame + 0x1C) : 0);
            fclose(index);
        }
    }
    sub_001762B0_gen();
    g_doa3_movie_frames++;
    /* present the finished movie frame to the host window (item 82) */
    {
        uint32_t mvdst = a[1] & 0x07FFFFFFu;
        if (mvdst >= 0x1000 && mvdst + 2880u * 480u < 0x8000000u) {
            extern void doa3_present_movie_surface(const void *src, int w, int h, int pitch);
            doa3_present_movie_surface(XBOX_PTR(mvdst), 720, 480, 2880);
        }
    }
    if (doa3_display_trace_enabled() && display_n < 40) {
        uint32_t dst = a[1] & 0x07FFFFFFu;
        char path[64];
        snprintf(path, sizeof path, "display_bgra_%04u.raw", display_n);
        FILE *dump = fopen(path, "wb");
        if (dump && dst >= 0x1000 && dst + 2880u * 480u < 0x8000000u)
            for (int row = 0; row < 480; row++)
                fwrite((const uint8_t *)XBOX_PTR(dst) + row * 2880u, 1, 2880, dump);
        if (dump) fclose(dump);
        display_n++;
    }
    if (s_n < 8) {
        uint32_t dst = a[1] & 0x07FFFFFFu;   /* pBits from LockRect */
        uint32_t d0 = 0, d1 = 0, d2 = 0;
        if (dst >= 0x1000 && dst < 0x8000000u) {
            d0 = MEM32(dst);
            d1 = MEM32(dst + 2880 * 240);        /* mid-frame row */
            d2 = MEM32(dst + 2880 * 240 + 1440); /* mid-frame center */
        }
        {
            uint32_t Y = a[0] & 0x07FFFFFFu;
            uint32_t U = Y + 0x56400, V = Y + 0x6CC00;
            {   /* recompute one pixel from the live tables, both families */
                uint32_t yv = MEM8(Y + 240 * 0x2E0 + 360), uv = MEM8(U + 120 * 0x180 + 180),
                         vv = MEM8(V + 120 * 0x180 + 180);
                for (int t = 0; t < 2; t++) {
                    uint32_t tb = t ? 0xB25778 : 0xB271D0;
                    int out[3];
                    for (int l = 0; l < 3; l++) {
                        int sum = (int16_t)MEM16(tb + yv * 8 + l * 2)
                                + (int16_t)MEM16(tb + 0x800 + uv * 8 + l * 2)
                                + (int16_t)MEM16(tb + 0x1000 + vv * 8 + l * 2);
                        sum >>= 6; if (sum < 0) sum = 0; if (sum > 255) sum = 255;
                        out[l] = sum;
                    }
                }
            }
        }
    }
    s_n++;
}

/* CRI server pump — shared by the per-frame SetFence hook AND the vblank-wait
 * bridge. On hardware these servers run from the vsync interrupt / their own
 * threads; in the fiber model both starve unless pumped. Pumping ONLY from
 * Present livelocks the game-side movie wait loops ("wait vblank; poll
 * status") which never present — so the vblank wait also pumps (item 79). */

/* DOA3 DIAG: integrity check of the ADXF partition table for loadfile.afs
 * (block 0x4BDA20: u16 sector sizes at +0x116, 1102 entries). Verified
 * intact through the boot loads and corrupted by the first post-movie load,
 * with no write caught by the page watch -- so it is checked from several
 * hook points to bracket the writer (an alias/host-side write). */
void (*g_kernel_ptinfo_hook)(const char *where);   /* kernel_bridge.c calls it from NtReadFile */
static uint16_t s_pt_ref[1102];
static int s_pt_have = 0, s_pt_reported = 0;
static const char *s_pt_last_ok = "none"; static unsigned s_pt_checks = 0;
void doa3_ptinfo_check(const char *where)
{
    const uint32_t blk = 0x4BDA20u;
    int i;
    /* OFF unless DOA3_PTCHK=1. This walks 1102 entries and is called from
     * the present/flush/kick/pump/read hooks, i.e. several times a frame.
     * Left on by default it starves the cooperative decode fibers and the
     * intro movie ends after a handful of frames -- measured, not assumed. */
    {   static int s_want = -1;
        if (s_want < 0) { const char *e = getenv("DOA3_PTCHK"); s_want = (e && *e == '1'); }
        if (!s_want) return; }
    if (s_pt_reported) return;
    if (!s_pt_have) {
        if (MEM32(0xC07640 + 4) != blk || MEM16(blk + 0xC) != 1102 ||
            MEM16(blk + 0x116 + 2 * 0x15) != 0x328) return;
        for (i = 0; i < 1102; i++) s_pt_ref[i] = MEM16(blk + 0x116 + 2u * i);
        s_pt_have = 1;
        return;
    }
    {   int first = -1, last = -1, n = 0;
        for (i = 0; i < 1102; i++)
            if (MEM16(blk + 0x116 + 2u * i) != s_pt_ref[i]) { if (first < 0) first = i; last = i; n++; }
        if (first >= 0) {
            void *bt[16]; USHORT nb = CaptureStackBackTrace(0, 16, bt, NULL); int k;
            s_pt_reported = 1;
        } else { s_pt_last_ok = where; s_pt_checks++; }
    }
}
void doa3_pump_cri_servers(void)
{
    extern void sub_001705E0(void);
    doa3_ptinfo_check("pump");
    extern void sub_00170640(void);
    extern void sub_00170660(void);
    if (g_doa3_in_pump) return;               /* no recursive pumping */

    if (MEM32(0xB24D38) != 0 || MEM32(0xC0E384) != 0) return;  /* lock held / in server */
    {   /* EOS detector (task #2 end-half): when the movie is PLAYING but no
         * new frame has blitted for ~6s (stream exhausted; the port's
         * stream-finished status never fires), set the player to PLAYEND
         * (obj+8 = 3) so the game's own sequencer completes the movie and
         * proceeds — and the free-running pipeline stops churning garbage. */
        uint32_t mvobj = MEM32(0x5E5900);
        if (mvobj >= 0x1000 && mvobj < 0x8000000u && MEM32(mvobj + 8) == 2) {
            uint32_t herr = MEM32(0xC0F7C0 + 0x988);
            extern uint32_t g_doa3_movie_frames;
            /* EOS (task #2 end-half): the port's stream-finished status never
             * fires; instead the parser eventually runs off the stream's end
             * into an FF00xxxx parse error after all real pictures played.
             * Treat that error (past a sane frame count) as END: clear it and
             * complete the movie through the game's own PLAYEND path. */
            extern int g_doa3_host_movie_ended;
            /* Second trigger: the host presenter reached the end of the movie
             * file. The condition above reads the parse error at h+0x988 and
             * counts guest blits, and neither survives a mid-stream decoder
             * stall -- runs end with the picture fully played, h+0x988 == 0,
             * 13 blits, and the handle parked in PLAYING forever, so the
             * sequencer never advances and the game sits on the last frame.
             * End of file is the same event the middleware itself reports as
             * PLAYEND; take it from the decoder that actually reached it and
             * complete through the game's own path. */
            if ((herr & 0xFF000000u) == 0xFF000000u && g_doa3_movie_frames > 60) {
                MEM32(0xC0F7C0 + 0x988) = 0;
                MEM32(mvobj + 8) = 3;
            } else if (g_doa3_host_movie_ended && MEM32(0xC0F7C0 + 0x40) == 4) {
                static int s_eos2 = 0;
                if (s_eos2 < 2) { s_eos2++;
                     }
                MEM32(0xC0F7C0 + 0x988) = 0;
                MEM32(0xC0F7C0 + 0x40) = 6;    /* handle: PLAYEND */
                MEM32(mvobj + 8) = 3;          /* movie object: finished */
            }
        }
    }
    {   /* keep the window responsive even when server work is skipped */
        extern void doa3_pump_messages(void);
        static unsigned s_mp = 0;
        if ((++s_mp & 7) == 0) doa3_pump_messages();
    }
    g_doa3_in_pump = 1;
    {   /* DOA3 DIAG: arm the DOA3_WATCHVA exact-address watch once the movie
         * has handed the screen over. The runaway scan in sub_0017CC30 walks
         * the mwPly pool with the count at 0xC0E514 and the base at 0xC0E518;
         * post-movie both read as garbage, so the loop sweeps memory until it
         * leaves the mapping. Arming here (not at startup) keeps the page
         * writable through the movie, where it is written constantly. */
        extern uint32_t g_watch_exact_va;
        extern volatile int g_doa3_post_movie;
        static int s_armed = 0;
        if (g_watch_exact_va && !s_armed && g_doa3_post_movie) {
            extern void doa3_watch_arm(uint32_t xb_page);
            s_armed = 1;
            doa3_watch_arm(g_watch_exact_va & ~0xFFFu);
        }
    }
    {   /* item 109 (defect #1): OPT-IN write-watch on the HANDLER page
         * (0xC0F000) to catch the writer that corrupts the state word
         * 0xC0F800 (4 -> -4). Off by default (the page-guard faulting perturbs
         * timing); set DOA3_WATCH=1 to re-enable for diagnostics. */
        static int s_armed = 0, s_want = -1;
        if (s_want < 0) { const char *e = getenv("DOA3_WATCH"); s_want = (e && *e == '1'); }
        if (s_want && !s_armed && MEM32(0xC0F7C0 + 0x40) == 4) {
            extern void doa3_watch_arm(uint32_t xb_page);
            doa3_watch_arm(0x00C12000);   /* item 110: catch pictype writer @0xC12DB8 */
            s_armed = 1;
        }
    }
    {   /* [LDT] the post-movie load: sub_0007FFDD polls op+1 for status 3.
     * The periodic dump only samples every 600 frames, so log every
     * change of the phase and of the op status instead. */
    static uint32_t s_ph = 0xFFFFFFFFu, s_op = 0xFFFFFFFFu; static int s_st = -99, s_n = 0;
    uint32_t ph = MEM32(0x4A1004);
    uint32_t op = (ph < 8) ? MEM32(ph * 4 + 0x4A10A8) : 0;
    int st = op ? (int)(int8_t)MEM8(op + 1) : -1;
    if ((ph != s_ph || op != s_op || st != s_st) && s_n < 60) {
        s_n++; s_ph = ph; s_op = op; s_st = st;
        if (op >= 0x1000u && op < 0x08000000u) {
            int _k; char nm[40] = {0};
            /* any dword that looks like a guest pointer may be the name */
            for (_k = 0; _k < 10; _k++) {
                uint32_t v = MEM32(op + _k * 4);
                if (v >= 0x10000u && v < 0x08000000u) {
                    int c; int ok = 1;
                    for (c = 0; c < 12; c++) {
                        uint8_t ch = MEM8(v + c);
                        if (ch == 0) break;
                        if (ch < 0x20 || ch > 0x7E) { ok = 0; break; }
                    }
                    if (ok && c >= 3) {
                        for (c = 0; c < 39; c++) { nm[c] = (char)MEM8(v + c); if (!nm[c]) break; }
                    }
                }
            }
        }
    }
}
    {   /* item 103b: apply releases that were deferred because their slot
         * was still decoding (see the slot-lifecycle guards). */
        extern void doa3_drain_deferred_releases(void);
        doa3_drain_deferred_releases();
    }
    {   /* log every sfd-handle / movie-object state transition: the handle
         * must reach 6 (PLAYEND) for the movie object to report 3 and
         * release the intro sequencer. */
        static uint32_t s_p40 = 0xFFFFFFFFu, s_p44 = 0xFFFFFFFFu, s_pst8 = 0xFFFFFFFFu;
        uint32_t h40 = MEM32(0xC0F7C0u + 0x40), h44 = MEM32(0xC0F7C0u + 0x44);
        uint32_t mo = MEM32(0x5E5900u);
        uint32_t st8 = (mo >= 0x1000 && mo < 0x8000000u) ? MEM32(mo + 8) : 0xFFFFFFFFu;
        if (h40 != s_p40 || h44 != s_p44 || st8 != s_pst8) {
            static int s_t = 0;
            if (s_t < 200) {
                s_t++;
            }
            s_p40 = h40; s_p44 = h44; s_pst8 = st8;
        }
    }
    {   /* once the intro sequencer has finished (state 3), let the next
         * screen settle and dump the presented frame for inspection */
        /* Capture is driven from the pgraph draw counter now (see
         * nv2a_pgraph_d3d11.c): this hook lives in the movie repaint pump,
         * which stops once playback ends, and it fired the instant
         * doa3_movie_present_finish() had blanked every swap-chain buffer --
         * so it could only ever record a black frame. */
    }
    {   static unsigned s_vs = 0;
        if ((++s_vs % 60) == 0 && MEM32(0xC0F7C0u + 0x40) == 4) {
        {   /* video ES joint + picture index + frame slots: says whether
             * the decoder is starved of ES or blocked holding slots */
            uint32_t vsj = 0xC09770u, ix = 0xC108BCu, h = 0xC0F7C0u;
            /* end-of-stream inputs: sub_00179070 marks the video stream
             * ended when inEnd==1 and (p0F==0 || sub_0017E880 != 0), and
             * sub_0017E880 compares servedPTS/scale against clock/scale. */
            fprintf(stderr, "[VEOS2] ended6=%X inEnd=%X p0F=%X spts=%d sscale=%d clk=%d cscale=%d f940=%X 9A8=%X 9AC=%X | tcH=%X tcM=%X tcS=%X picPTS=%X picScale=%X\n",
                MEM32(h + 6u * 0x610u + 0x2978u),
                MEM32(h + 1u * 0x388u + 0xD70u),
                MEM32(h + 0x994u + 0x0Fu * 4u),
                (int)MEM32(h + 0xCC4), (int)MEM32(h + 0xCC8),
                (int)MEM32(h + 0xCCC), (int)MEM32(h + 0xCD0),
                MEM32(h + 0x940), MEM32(h + 0x9A8), MEM32(h + 0x9AC),
                /* running timecode accumulator at h+0xAD8 and the last
                 * picture timestamp sub_0017A400 produced at h+0xB44 */
                MEM32(h + 0xAE0), MEM32(h + 0xAE4), MEM32(h + 0xAE8),
                MEM32(h + 0xB44), MEM32(h + 0xB48));
        }
        }
    }
    {   /* item 102: movie-ingest stall forensics. In stalled user runs the
         * ninja.sfd stream joint (0xC09890) sticks at w=0x12000 with the
         * cvFs stream stepper never issuing another read. Dump the joint
         * cursors + every live cvFs stream-pool entry every ~10s so the
         * NEXT stalled run (any machine) shows which state it parked in. */
        static unsigned s_ig = 0;
        if ((++s_ig % 600) == 0) {
            uint32_t sj = 0xC09890;
            for (int i = 0; i < 16; i++) {
                uint32_t ent = 0xC05AC0 + i * 0x40u;
                if (!MEM8(ent) && !MEM32(ent + 0x14)) continue;
            }
        }
    }
    {
        static unsigned s_pumps = 0; static unsigned s_t0 = 0;
        unsigned now = (unsigned)GetTickCount();
        if (!s_t0) s_t0 = now;
        s_pumps++;
        if (now - s_t0 >= 10000) {
            uint32_t h = 0xC0F7C0;
            extern uint32_t g_doa3_movie_frames;
            s_t0 = now; s_pumps = 0;
        }
    }
    {
        uint32_t saved_esp2 = esp;
        /* group-5 handler-table sanity sweep (see [G5TBL]) */
        for (int sl = 0; sl < 4; sl++) {
            uint32_t fa = 0xB25550 + 8u * sl;
            uint32_t fn = MEM32(fa);
            if (fn && (fn < 0x00011000 || fn >= 0x002CC800))
                MEM32(fa) = 0;
        }
        for (int pt = 0; pt < 2; pt++) {
            PUSH32(esp, 5);                   /* dispatch ADXM user group 5 */
            PUSH32(esp, 0);
            sub_001705E0();
            esp = saved_esp2;
        }
       
        PUSH32(esp, 0); sub_00170640(); esp = saved_esp2;   /* ADX main server */
       
        PUSH32(esp, 0); sub_00170660(); esp = saved_esp2;   /* mwPly tick */
        /* CRI file server. This is the tick that actually performs file I/O:
         * sub_00170710 -> sub_00169BC0 -> sub_0016C6B0 -> sub_00171A70 ->
         * sub_001719FE -> sub_00171620 -> NtReadFile, confirmed by backtracing
         * the reads that do work. It was never pumped, so after the movie the
         * boot poll that used to drive it is gone and not one read is issued:
         * every post-movie resource load completes with an untouched
         * destination buffer (the record parser then walks an all-zero blob
         * forever, which is what keeps the screen dispatcher out of its loop).
         * It guards its own re-entry through [0xB255F4], so pumping it here is
         * safe alongside the servers above. */
        {   /* Post-movie only: ticking it during playback perturbs the CRI
             * stream state and the movie stops handing over. The verified
             * intro-movie timing is left exactly as it was. */
            extern volatile int g_doa3_post_movie;
            if (g_doa3_post_movie) {
                PUSH32(esp, 0); sub_00170710(); esp = saved_esp2;
            }
        }
        {   /* Dispatch the other registered ADXM groups once the movie is over.
             *
             * sub_001705E0(g) walks the four (fn,arg) slots at
             * 0xB254B0 + g*32; group 5 is the sfdec decode group pumped above,
             * and the CRI file reader lives in group 2 (0xB254F0). Only group 5
             * was ever dispatched, so after the movie the post-movie asset read
             * sat queued forever -- the load op parked at status 2 with no
             * NtReadFile ever issued, which blocked sub_00084340 in its init
             * (sub_00082060 -> sub_00050940 -> sub_0006B7E0 -> sub_0006AD20)
             * before its screen loop started, so sub_000821B0 never ran and the
             * title screen was never created.
             *
             * On hardware the CRI server thread dispatches every group it has
             * handlers for; this does the same for the groups the game actually
             * registered. Restricted to post-movie so the verified intro-movie
             * timing is left exactly as it is. */
            extern volatile int g_doa3_post_movie;
            if (g_doa3_post_movie) {
                for (int g = 0; g < 5; g++) {
                    uint32_t tbl = 0xB254B0u + (uint32_t)g * 32u;
                    int has = 0, sl;
                    for (sl = 0; sl < 4; sl++) {
                        uint32_t fn = MEM32(tbl + (uint32_t)sl * 8);
                        if (fn >= 0x00011000u && fn < 0x002CC800u) { has = 1; break; }
                    }
                    if (!has) continue;
                    {   static int s_log[5] = {0,0,0,0,0};
                        if (!s_log[g]) { s_log[g] = 1;
                             } }
                    PUSH32(esp, (uint32_t)g);
                    PUSH32(esp, 0);
                    sub_001705E0();
                    esp = saved_esp2;
                }
            }
        }
        /* item 92: decode runs on worker fibers that only progress when the
         * main fiber yields. Harness sessions yield thousands of times/s
         * (unthrottled), interactive sessions ~60/s (vsync-locked) -> their
         * decode ran ~80x slower and the movie looked frozen after the
         * prebuffer. Give the workers a fixed slice budget per frame. */
        if (MEM32(0xC0F7C0 + 0x40) == 4) {
            /* task #1: fixed 32-yield budget = measured optimum (20fps
             * under the throttle rig). Time-boxed variants (8ms, with and
             * without any-ready exit) measured WORSE (14fps) — watchdog
             * fibers are perpetually READY and soak the box; larger fixed
             * counts collapse the pump (item 94). Real 30fps needs decode-
             * kernel optimization, not scheduling. */
            extern void xbox_fiber_yield(void);
            for (int yi = 0; yi < 32; yi++) xbox_fiber_yield();
        }
        else {
            /* After the movie the main loop never blocks in the kernel, so the
             * CRI worker threads (vblank-paced ADX/file servers, and the I/O
             * completion worker) only ran when a deadlock breaker fired --
             * about once a second (measured: prim_yields=1..3 per 2 s). The
             * music stream then got a file read every ~4 s for ~2 s of audio,
             * and the voice replayed its ring buffer in the gaps (the 341 ms
             * echo). On hardware those threads run on every vblank; give them
             * one round-robin lap per pump, the same model the movie uses. */
            extern volatile int g_doa3_post_movie;
            extern void xbox_fiber_timeslice(void);
            xbox_fiber_timeslice();
        }
        /* keep the last movie frame on screen (~30Hz repaint; see
         * movie_present.c — nothing else presents during/after a movie) */
        {
            /* Only while the movie is actually playing: once the handle leaves
             * state 4 the game owns the screen again, and repainting the last
             * movie frame would paint straight over it. */
            static unsigned s_rpn = 0;
            static int s_was_playing = 0;
            extern void doa3_movie_repaint(void);
            extern void doa3_movie_present_finish(void);
            extern unsigned g_mw_calls, g_mw_st6, g_mw_set3, g_mw_laststate;
    extern unsigned g_e1f0_calls, g_e1f0_nz;
    extern unsigned g_st_calls; extern int g_st_lastret, g_st_lastst8;
    extern uint32_t g_st_lastobj;
    {   /* Title-screen arming chain, sampled only AFTER the movie ends
         * (SFD object state 6). The post-FMV title screen is armed when
         * sub_000CE7D0 takes case 10 of its jump table, which needs the screen
         * state byte 0x8612AD non-zero; that byte is written only by
         * sub_000C4D00, reached by sub_000C5D70 only when a player slot flag
         * is set. Case 10 is what sets 0x47E74C = 2, which sub_00083A90
         * waits on before showing the title screen. */
        static unsigned s_tn = 0, s_tick = 0; static int s_post = 0;
        /* Latch once the movie handle reports end-of-stream: it only
         * passes through state 6 briefly before the handle is released,
         * so gating each sample on it would report a single line. */
        if (MEM32(0xC0F7C0u + 0x40) == 6) s_post = 1;
        if (s_post && ((s_tick++ & 0xFFFu) == 0) && s_tn < 24) {
            s_tn++;
        }
    }
    if (MEM32(0xC0F7C0u + 0x40) == 4) {
                s_was_playing = 1;
                if ((++s_rpn & 1) == 0) doa3_movie_repaint();
            } else if (s_was_playing) {
                /* movie just ended: blank the chain so its last frames stop
                 * flipping on screen, and hand the display to the game */
                s_was_playing = 0;
                doa3_movie_present_finish();
            }
        }
    }
   
    g_doa3_in_pump = 0;
}

void sub_001B8970(void)
{
    extern void sub_001B8970_gen(void);
    extern void pgraph_d3d11_flush(void);
    extern void doa3_present_frame(void);
    uint32_t arg = MEM32(esp + 4);     /* fence type (2 == per-frame Present fence) */
    /* esi/edi/ebx are callee-saved. D3DDevice_Present (sub_001B5850) loads its
     * device pointer into esi from MEM32(0x1C3390) and then calls us FIRST
     * thing; its continuation sub_001B58AD uses that same esi for every device
     * field. This wrapper runs the real SetFence plus a flush, a present and
     * the whole vblank emulation, none of which preserved esi -- so Present
     * came back with a junk device (0x25E340, an address with no static
     * reference anywhere) and then waited on a vertical-blank event inside it
     * at 0x260830. Nothing signals that, so after the intro movie the primary
     * fiber and both CRI workers parked there forever and the game froze.
     * Restore the callee-saved registers the original SetFence guarantees. */
    uint32_t _sv_esi = esi, _sv_edi = edi, _sv_ebx = ebx;
    doa3_ptinfo_check("setfence-in");
    sub_001B8970_gen();                /* real D3D_SetFence (does its own ret 4 cleanup) */
    if (arg == 2) {
        pgraph_d3d11_flush();
        doa3_ptinfo_check("after-flush");
        doa3_present_frame();
        doa3_ptinfo_check("after-present");
        /* Full VBLANK emulation (mirrors the D3D vblank handler sub_001BAA30, which the
         * GPU PCRTC interrupt DPC would run): bump the vblank counter (device+0x820),
         * signal the vblank event (device+0x1E8 = 0x001C2CF0), and CALL the vblank
         * callback the game registered via D3DDevice_SetVerticalBlankCallback
         * (device+0x1E4) with the counter — DOA3 runs its per-frame screen/boot logic
         * from that callback, so without this the game idles forever in boot stage 0. */
        {
            const uint32_t dev = 0x001C2B08;         /* D3D device: 0x001C2CF0 - 0x1E8 */
            uint32_t cnt = MEM32(dev + 0x820) + 1;
            MEM32(dev + 0x820) = cnt;
            MEM32(0x001C2CF0 + 4) = 1;               /* KEVENT.SignalState */
            xbox_fiber_wake(0x001C2CF0);
            uint32_t cb = MEM32(dev + 0x1E4);
            static uint32_t s_cb_last = 0xFFFFFFFF;
            if (cb != s_cb_last) {
                s_cb_last = cb;
            }
            if (cb) {
                extern recomp_func_t recomp_lookup(uint32_t xbox_va);
                extern recomp_func_t recomp_lookup_manual(uint32_t xbox_va);
                recomp_func_t fn = recomp_lookup_manual(cb);
                if (!fn) fn = recomp_lookup(cb);
                if (fn) {
                    uint32_t saved_esp = esp;
                    PUSH32(esp, cnt);                /* arg: vblank counter (cdecl) */
                    PUSH32(esp, 0);                  /* dummy return address */
                    fn();
                    esp = saved_esp;
                }
            }
        }
        /* WATCH: mwPly slot table 0xC0E544..0xC0F7B4. Stream ASCII bytes are
         * landing in slot fields (junk sfdec "handles" -> FF000222 spam).
         * Detect the frame it changes and dump the first differing dwords. */
        {
            static uint32_t s_snap[0x494];   /* (0xC0F7B4-0xC0E544)/4 = 0x49C.. use 0x494 */
            static int s_have = 0, s_prints = 0;
            static uint32_t s_fr = 0;
            uint32_t cnt = ++s_fr;
            uint32_t n = (0xC0F7B4 - 0xC0E544) / 4;
            if (n > 0x494) n = 0x494;
            if (!s_have) {
                for (uint32_t i = 0; i < n; i++) s_snap[i] = MEM32(0xC0E544 + i * 4);
                s_have = 1;
            } else if (s_prints < 40) {
                int shown = 0;
                for (uint32_t i = 0; i < n; i++) {
                    uint32_t a = 0xC0E544 + i * 4;
                    uint32_t v = MEM32(a);
                    if (v != s_snap[i]) {
                        /* only care about slot control fields: base+4 (active),
                         * base+8 (state), base+0x30 (sfdec handle) per 0x90 slot */
                        uint32_t rel = (a - 0xC0E544) % 0x90;
                        if ((rel == 4 || rel == 8 || rel == 0x30) && shown < 8) {
                            shown++;
                        }
                        s_snap[i] = v;
                    }
                }
                if (shown) { s_prints++;  }
            }
            /* arm the page write-watch across the corruption window */
            extern void doa3_watch_arm(uint32_t xb_page);
            extern void doa3_watch_disarm(void);
            /* (retargeted, item 83) slot-state recycler hunt: armed from the
             * pump below once the movie is PLAYING. */
        }
        /* Sofdec decode pump — see doa3_pump_cri_servers() above. */
        doa3_pump_cri_servers();
        {
            uint32_t saved_esp2 = esp; (void)saved_esp2;
            /* EXPERIMENT (audio bypass, see NOTES.md "Known Limitations and Next Work"): the video
             * readiness chain waits on the AUDIO substream reaching prebuffer
             * state 5, but the audio substream never starts (h+0x59CC stays
             * 0 — ADXT attach path broken separately). Clear the "has audio"
             * params so the pack path sets the queue flags and video plays
             * silently: param0x31 (h+0xA58), param0x38 (h+0xA74), param6
             * (h+0x9AC). Re-applied per frame while the handle is in prep. */
            {
                uint32_t sfd = MEM32(0xC0E544 + 0x30);   /* slot0 handle */
                if (sfd >= 0x1000 && sfd < 0x8000000u && MEM32(sfd + 0x40) == 2) {
                    static int s_bp = 0;
                    if (MEM32(sfd + 0xA58) || MEM32(sfd + 0xA74) || MEM32(sfd + 0x9AC)) {
                        if (s_bp < 3) {
                            s_bp++;
                        }
                        MEM32(sfd + 0xA58) = 0;
                        MEM32(sfd + 0xA74) = 0;
                        MEM32(sfd + 0x9AC) = 0;
                    }
                    /* Force free-run AV mode ([h+0x35D0]=1). The no-audio
                     * seq-header branch (sub_0017D9F0) would set this, but
                     * the class-0x80 path never completes in our stream
                     * state; without it readiness (sub_0017DA20) demands
                     * decoded frames that the AV-sync mode never produces. */
                    /* CLOCK BOOTSTRAP: while the handle is in prep (status
                     * 2), the state handler reports clock=-1 (it only reports
                     * once PLAYING), and the AV-sync decide (sub_0017A540 ->
                     * sub_0017E720) skips every picture on a negative clock —
                     * a deadlock our dead audio cannot break (on hardware the
                     * audio decode clock ticks during prebuffer). Feed the
                     * demux PTS ([h+0xCE8]) as the master clock so prebuffer
                     * pictures decode; once PLAYING the real path takes over. */
                    /* MOVIE-0 BAIL SUPPRESSION: sub_0009E1F0 posts a STOP
                     * when [0x5E5A04]!=0 (audio system "ready" — forced 1 by
                     * the sub_0009FAB0 override) and [0x5E5978]==0 (the
                     * movie's ADXT audio stream handle — its creation
                     * sub_0016B400 has always failed in the port). The game
                     * refuses to play the movie without its AIX audio track.
                     * Pretend the stream exists so the video plays silent. */
                    if (MEM32(0x5E5900) != 0 && MEM32(0x5E5978) == 0) {
                        static int s_ab = 0;
                        if (s_ab < 3) { s_ab++;
                             }
                        MEM32(0x5E5978) = 0xFFFFFFFFu;
                    }
                    /* (older clock write + QFWD flag mirroring removed: the
                     * clock bootstrap lives in the sub_0017E9B0 wrapper with
                     * clock=0, and [QBYP] re-points the consumers directly at
                     * the producer queues — mirroring rec+0xC here copied the
                     * EOS/notify marker around and ended the movie early.) */
                }
            }
        }
        /* Device emulation: keep the pads' async-poll markers cleared (no
         * USB completion machinery exists; see the sub_001E70AD override). */
        for (int pi = 0; pi < 4; pi++) {
            uint32_t pst = 0x5E5CFF + 0x80u * pi;
            if (MEM32(pst) == 0x3E5) MEM32(pst) = 0;
        }
        static unsigned fr = 0;
        ++fr;
        if ((fr <= 1200 ? (fr % 60) : (fr % 600)) == 1) {
            extern void pgraph_d3d11_get_stats(void *out);
            struct { uint32_t frames, draws, verts, handled, ignored, clears; } s = {0};
            pgraph_d3d11_get_stats(&s);
            {   /* movie surface content check (both double-buffer surfaces) */
                fprintf(stderr, "[SURF] A: %08X %08X mid %08X | B: %08X %08X mid %08X\n",
                        MEM32(0x0258C000), MEM32(0x0258C004), MEM32(0x0258C000 + 0xA8C00),
                        MEM32(0x02A24000), MEM32(0x02A24004), MEM32(0x02A24000 + 0xA8C00));
            }
            {   extern void pgraph_diag_dump_ignored(void);
                pgraph_diag_dump_ignored(); }
            /* DIAG: intro sequencer (sub_00081EB0) + game state. 0x4B83B0=state,
             * B1=movie idx, B2/B4=phase durations, B6=frame counter; 0x5E597C=the
             * game-state flag gating per-frame sub_00153D90 (see NOTES.md). */
            fprintf(stderr, "[INTRO] st=%u mv=%u dur=%u/%u ctr=%u  gstate=0x%X 5E5A04=0x%X 5E5978=0x%X\n",
                    MEM8(0x4B83B0), MEM8(0x4B83B1), MEM16(0x4B83B2), MEM16(0x4B83B4),
                    MEM16(0x4B83B6), MEM32(0x5E597C), MEM32(0x5E5A04), MEM32(0x5E5978));
            /* DIAG: Sofdec movie player state (sub_0009DF60 init path).
             * 0x5E5900=movie obj (vtbl calls +0x18 open / +0x2C serve),
             * 0x5E59C8=sub_00176580 result, 0x5E5974=sub_0016B400 handle,
             * 0x331298[idx]=filename ptr table. */
            {   uint32_t mobj = MEM32(0x5E5900);
                uint32_t fn0 = MEM32(0x331298);
                char nm[32] = {0};
                if (fn0 >= 0x10000 && fn0 < 0x2000000) {
                    for (int i = 0; i < 31; i++) { nm[i] = (char)MEM8(fn0 + i); if (!nm[i]) break; }
                }
            }
            /* DIAG: XAPI game-task table @0x5E5A08 stride 0x20 (18 slots,
             * cur idx @0x5E5C4C): +0 flags, +4 sleep arg (sub_0009E562).
             * The boot task parks here and stops being dispatched. */
            {   char tl[240]; int tn = 0;
                for (int ti = 0; ti < 18 && tn < 220; ti++) {
                    uint32_t tb = 0x5E5A08 + 0x20u * ti;
                    if (MEM32(tb) || MEM32(tb + 4))
                        tn += snprintf(tl + tn, sizeof tl - tn, " %d:%X/%X", ti, MEM32(tb), MEM32(tb + 4));
                }
            }
            /* DIAG: VRAM bank table @0x4889B8 stride 0x1C (movie surface prep
             * sub_0009DDE0 reads bank[i]+0 as the frame buffer; entries beyond
             * 0 are empty -> 7 of 8 movie surfaces fail). */
            fprintf(stderr, "[BANKS]");
            /* DIAG: sfdec PES queues (h+0xD34 stride 0x388): +0xC = complete-
             * unit flag/count per queue, for queues 0..7. */
            {   uint32_t sh = 0xC0F7C0;
                /* deeper: PES queue record heads (first 6 dwords) for q0/q5
                 * (q5 = video), substream present flags h+0x994[0..8], and
                 * substream readiness h+0x2974+i*0x610 */
                fprintf(stderr, "[QDET] q0:");
                for (int q = 1; q < 8; q++) {
                }
                {   uint32_t vt6 = MEM32(sh + 0x2F4C + 0x610u * 6);
                }
                {   uint32_t fc = MEM32(sh + 0x3668);
                    if (fc > 16) fc = 16;
                }
                /* SJ pool dump: 0xC09500 stride 0x30, 16 slots. Fields:
                 * +0 vtbl, +4 used, +0xC/+0x10 counters, +0x1C base, +0x20 cap */
                fprintf(stderr, "[SJSTAT]");
                for (int i = 0; i < 16; i++) {
                    uint32_t sj = 0xC09500 + 0x30u * i;
                    if (!MEM32(sj + 4)) continue;
                }
            }
            /* DIAG: wxCi cache registry @0xC057C0 stride 0x30 (12 slots):
             * +0=active, +0x18=prefix, +0x24=file count, +0x28=list head.
             * Populated by sub_0009C840 via sub_0016D330; wxCiOpen's lookup
             * (cb 0xB25610=sub_0016D190) fails "not in cache" when empty. */
            {   char line[256]; int n = 0;
                for (int i = 0; i < 12 && n < 200; i++) {
                    uint32_t b = 0xC057C0 + 0x30u * i;
                    if (MEM32(b)) {
                        char pfx[20] = {0}, fn[20] = {0};
                        uint32_t pp = MEM32(b + 0x18);
                        if (pp >= 0x10000 && pp < 0x8000000)
                            for (int k = 0; k < 19; k++) { pfx[k] = (char)MEM8(pp + k); if (!pfx[k]) break; }
                        uint32_t node = MEM32(b + 0x28);          /* list head */
                        uint32_t np = (node >= 0x10000 && node < 0x8000000) ? MEM32(node + 0xC) : 0;
                        if (np >= 0x10000 && np < 0x8000000)
                            for (int k = 0; k < 19; k++) { fn[k] = (char)MEM8(np + k); if (!fn[k]) break; }
                        n += snprintf(line + n, sizeof line - n, " [%d]a=%X n=%d p='%s' f0='%s'", i, MEM32(b), (int)MEM32(b + 0x24), pfx, fn);
                    }
                }
                /* Walk the z: slot list in full: the count at +0x24 says 1
                 * but three files are scanned, so either only one node is
                 * linked or the count is not being incremented. Dump the head
                 * node raw so the link field can be identified. */
                {   uint32_t b = 0xC057C0 + 0x30u * 2;   /* z:\ slot */
                    uint32_t node = MEM32(b + 0x28);
                    int hop;
                    if (node >= 0x10000 && node < 0x8000000u) {
                        int k;
                    }
                    for (hop = 0; hop < 8 && node >= 0x10000 && node < 0x8000000u; hop++) {
                        char fn[24] = {0};
                        uint32_t np = MEM32(node + 0xC);
                        if (np >= 0x10000 && np < 0x8000000u)
                            for (int k = 0; k < 23; k++) { fn[k] = (char)MEM8(np + k); if (!fn[k]) break; }
                        /* node: +0x04 file size, +0x08 next, +0x0C -> inline
                         * name at +0x10. (+0x04 held 0x0D835000 = 226709504,
                         * bgm.afs to the byte, which identified the layout.) */
                        node = MEM32(node + 8);
                    }
                }
            }
            /* DIAG: ADXM user-callback group 4 (the mwPly/Sofdec server tick;
             * dispatcher sub_001705E0 from sub_001778F0). Empty slots = the
             * Sofdec state machine is never pumped (player state stuck 0). */
            fprintf(stderr, "[ADXCB4] %X/%X %X/%X %X/%X %X/%X\n",
                    MEM32(0xB25530), MEM32(0xB25534), MEM32(0xB25538), MEM32(0xB2553C),
                    MEM32(0xB25540), MEM32(0xB25544), MEM32(0xB25548), MEM32(0xB2554C));
            /* cvFs device table health: slot flags @0xB25264 stride 16 (wiped
             * -> every open fails), default name @0xB24FE0. */
            fprintf(stderr, "[CVFS] def=%c%c slotflags=%02X %02X wxci0=%02X wxci1=%02X wxciSt=%02X\n",
                    MEM8(0xB24FE0) ? MEM8(0xB24FE0) : 48,
                    MEM8(0xB24FE1) ? MEM8(0xB24FE1) : 48,
                    MEM8(0xB25264), MEM8(0xB25274),
                    MEM8(0xBFEE80), MEM8(0xBFEE80 + 0x10E0*4/2), MEM8(0xBFEE81));
            /* warning-screen sequencer (sub_000566D0 tail): 0x491AFC = frame
             * counter, limit = 3*MEM8(0x2FD55C); 0x305B70 = screen-active. */
            {   /* The post-movie load state machine. sub_00084340 blocks in
             * sub_0006AD20 -> sub_0006AD6A waiting for MEM8(0x4A2128) to
             * clear, which only the load task sub_00080200 does. That task
             * enters and never returns. Its gates are these bytes; note
             * 0x00080267 is a register spin with no reload, taken when
             * 0x4A2120 != 0 and 0x4A2122 == 0. */
                uint32_t ph = MEM32(0x4A1004);
                uint32_t op = (ph < 8) ? MEM32(ph * 4 + 0x4A10A8) : 0;
            }
            if ((fr % 600) == 1) {
                extern void xbox_fiber_dump(void);
                extern uint32_t g_task_yields[16];
                xbox_fiber_dump();
            }
            {   /* scene-manager stream poll (sub_0009CDA0): handle @0x4BE420 must reach
                 * state 5; partition handles 0x4BE3F8/0x4BE40C for reference. */
                uint32_t h420 = MEM32(0x4BE420), h3F8 = MEM32(0x4BE3F8), h40C = MEM32(0x4BE40C);
                {   /* boot loader (sub_0009F730 loc_0009F7A8): 5 phases at 0x4A1004;
                     * sub_0007FFDD polls op = MEM32(phase*4 + 0x4A10A8) for status 3. */
                    uint32_t ph = MEM32(0x4A1004);
                    uint32_t op = (ph < 8) ? MEM32(ph * 4 + 0x4A10A8) : 0;
                }
            }
        }
        /* ---- frame pacing: 60 Hz -----------------------------------------
         * D3DDevice_Swap (0x001B5850) is the game's frame gate.  It blocks in
         * the spin at 0x001B8690 while swaps submitted (dev+0x2B60) minus
         * swaps completed (dev+0x2518) is >= 2, and on hardware the GPU
         * advances "completed" once per retired flip -- the game asks for
         * D3DPRESENT_INTERVAL_ONE (dev+0x2B64 reads 1), so that gate paces the
         * whole loop at 60 Hz.  The only CPU write to dev+0x2518 in the XBE is
         * 0x001B895F, the simulate-the-GPU path; the real one is a GPU-written
         * location registered at 0x001B99A1.
         *
         * The kickoff override publishes completed = submitted on every kick
         * (the FIFO and fence spins need that), so the gate never blocks and
         * the loop free-ran: 59-204 swaps/s measured after the intro movie.
         * Every per-frame game counter follows the swap count 1:1 (72.0/s
         * measured against 69-77 presents/s), so character and camera motion
         * surged and dragged with scene load.  Hold the frame here until the
         * next 60 Hz deadline instead.
         *
         * Movie playback keeps its own verified timing and is never paced. */
        {
            extern volatile int g_doa3_post_movie;
            static LONGLONG s_qpf, s_next;
            static HANDLE s_timer;
            static int s_init;
            if (!g_doa3_post_movie) {
                s_next = 0;
            } else {
                LARGE_INTEGER now;
                LONGLONG period;
                if (!s_init) {
                    LARGE_INTEGER f;
                    s_init = 1;
                    QueryPerformanceFrequency(&f);
                    s_qpf = f.QuadPart;
                    /* Sleep() rounds up to the system timer tick, which is
                     * 15.6 ms unless something is holding it down -- the APU
                     * does (timeBeginPeriod(1)), but nothing in this file may
                     * depend on that.  A high-resolution waitable timer is not
                     * tied to the tick at all.  Measured frame interval with
                     * it: p50 16.67 ms, exactly the period.  Systems without
                     * it fall back to the ordinary timer. */
                    s_timer = CreateWaitableTimerExW(NULL, NULL, 0x00000002,
                                                     TIMER_MODIFY_STATE | SYNCHRONIZE);
                    if (!s_timer)
                        s_timer = CreateWaitableTimerExW(NULL, NULL, 0,
                                                     TIMER_MODIFY_STATE | SYNCHRONIZE);
                }
                period = s_qpf / 60;
                QueryPerformanceCounter(&now);
                if (!s_next) s_next = now.QuadPart;
                s_next += period;
                /* A frame that overran its slot leaves the deadline in the
                 * past, and the next frames then run without waiting until the
                 * schedule is met again.  The catch-up is bounded to 8 frames:
                 * beyond that the hitch is written off and the clock
                 * re-anchors, so a long stall can never be repaid by a long
                 * sprint (that sprint is the speed surge this change removes).
                 * Refusing to catch up at all was measured far worse -- every
                 * overshoot became permanent and the rate fell to 48 fps. */
                if (now.QuadPart - s_next > period * 8) s_next = now.QuadPart;
                for (;;) {
                    LONGLONG rem;
                    QueryPerformanceCounter(&now);
                    rem = s_next - now.QuadPart;
                    if (rem <= 0) break;
                    if (s_timer && rem * 2000 > s_qpf) {
                        /* wait all but the last 0.5 ms, then spin it out so
                         * the timer's wake-up slop cannot overshoot */
                        LARGE_INTEGER due;
                        due.QuadPart = -((rem - s_qpf / 2000) * 10000000 / s_qpf);
                        if (SetWaitableTimer(s_timer, &due, 0, NULL, NULL, FALSE))
                            WaitForSingleObject(s_timer, 20);
                        else
                            Sleep(0);
                    } else {
                        YieldProcessor();
                    }
                }
            }
        }
    }
    esi = _sv_esi; edi = _sv_edi; ebx = _sv_ebx;
}

/* PB space check (0x1B8DC0) — cdecl(device, dwords), ret 8, eax = cursor.
 *
 * The guest body only *validates*: it returns the cursor unless
 * cursor + dwords*4 >= [dev+4] + 0x200, in which case it delegates the wrap
 * to sub_001B8B00. The caller advances [dev] itself. In this port that
 * delegation never wraps, so the cursor walked straight off the end of the
 * push buffer: it climbed 0x800 per chunk to 0x03FFFF20 and then carried on
 * into 0x04000720, 0x04001720, ... Once past the end the MMX vertex copier
 * sub_001B3940 was writing through pointers that resolved into .data, and it
 * overwrote the vtable pointers of the 175-object array at 0x370C48 that
 * sub_000E8BB0 dispatches through -- objects whose vtables were verified
 * intact at the first dispatch were later found holding vertex data
 * (0xFF080C22 and friends: ARGB colours), and calling [vtbl+4] then faulted.
 *
 * Apply the same clamp sub_001B8DA0 already uses, which is what keeps the
 * write cursor inside our RAM push buffer; the translator resyncs on wrap
 * via g_pb_parsed. */
void sub_001B8DC0(void)
{
    uint32_t dev    = MEM32(esp + 4);
    uint32_t dwords = MEM32(esp + 8);
    uint32_t cursor = MEM32(dev);
    if (doa3_pb_recording(dev)) {                    /* original: scratch buffer */
        if (cursor + dwords * 4 >= MEM32(dev + 4) + 0x200)
            cursor = doa3_pb_makespace_recording(dev);
        eax = cursor;
        esp += 12;  /* ret 8 */
        return;
    }
    if (g_doa3_pb_base &&
        (cursor < g_doa3_pb_base ||
         cursor + dwords * 4 + 0x1000 >= g_doa3_pb_end)) {
        doa3_pb_wrap(dev, "Reserve");                /* wrap before the end */
        cursor = g_doa3_pb_base;
    }
    eax = cursor;
    esp += 12;  /* ret 8 */
}

/* XMETAL_StartPush (0x1B8DA0) — burnout PB begin/alloc analogue. cdecl, 1 stack
 * arg (device at esp+4), ret 4, returns eax = write cursor. Keep the cursor in
 * our RAM buffer so push-buffer writes don't fault at the GPU aperture. */
void sub_001B8DA0(void)
{
    uint32_t dev = MEM32(esp + 4);
    uint32_t cursor = MEM32(dev);
    if (doa3_pb_recording(dev)) {                    /* original: scratch buffer */
        if (cursor >= MEM32(dev + 4))
            cursor = doa3_pb_makespace_recording(dev);
        eax = cursor;
        esp += 8;   /* ret 4 */
        return;
    }
    if (g_doa3_pb_base &&
        (cursor < g_doa3_pb_base || cursor + 0x1000 >= g_doa3_pb_end)) {
        doa3_pb_wrap(dev, "StartPush");              /* (re)start at base / wrap before end */
        cursor = g_doa3_pb_base;
    }
    eax = cursor;
    esp += 8;   /* ret 4 */
}

/* ── Override: CRT small-block-heap allocator __sbh_alloc_block @ 0x001668FA ──
 * DOA3's recompiled SBH init leaves the heap descriptor global (0xBF5000) NULL,
 * so the generated allocator walks free-lists from a NULL descriptor and faults.
 * Serve allocations from a private bump arena instead. (Gen body #if 0'd in
 * recomp/gen/recomp_0008.c — re-apply after regen.) The matching SBH free path
 * finds our blocks in no (empty) SBH region and falls back to a no-op system
 * free, so blocks are effectively leaked-freed.
 * __stdcall(heapDesc, flags, size) -> block VA in eax; callee pops 3 args. */
static uint32_t g_sbh_next = 0, g_sbh_end = 0;

/* Size-classed allocator with REAL free (the pure bump version leaked every
 * CRT free -> 45 MB Xbox heap exhausted during the boot screen loads, and the
 * game doesn't check malloc failure -> wild-write storms). Each block gets a
 * 16-byte header {magic, chunk_size, class, freelist_next}; freed blocks go
 * on per-class LIFO lists and are reused. <1MB: power-of-2 classes; >=1MB: a
 * first-fit large list (64 KB granularity). Backing memory still comes from
 * the bump arena (xbox_HeapAlloc 8 MB refills). */
#define SBH_MAGIC 0x48334142u
#define SBH_NCLASS 16               /* 32 << 0 .. 32 << 15 (= 1 MB) */
static uint32_t g_sbh_class_head[SBH_NCLASS]; /* header VAs */
static uint32_t g_sbh_large_head = 0;

static uint32_t g_sbh_nalloc = 0, g_sbh_nfree = 0, g_sbh_nforeign = 0;
static uint64_t g_sbh_live = 0;

static uint32_t sbh_bump_raw(uint32_t csz)
{
    extern uint32_t xbox_HeapAllocHigh(uint32_t size, uint32_t alignment);
    if (g_sbh_next == 0 || g_sbh_next + csz > g_sbh_end) {
        uint32_t chunk = 8u * 1024u * 1024u;
        if (csz > chunk) chunk = (csz + 0xFFFFFu) & ~0xFFFFFu;
        uint32_t base = xbox_HeapAllocHigh(chunk, 16);
        {   /* native backtrace: who is allocating? (symbolize vs bin/doa3.map) */
            void *bt[12]; int n = CaptureStackBackTrace(1, 12, bt, NULL);
        }
        if (!base) return 0;
        g_sbh_next = base; g_sbh_end = base + chunk;
    }
    uint32_t p = g_sbh_next; g_sbh_next += csz; return p;
}

static uint32_t sbh_alloc(uint32_t size)
{
    uint32_t csz = (size + 16u + 15u) & ~15u;   /* payload + header */
    uint32_t hdr = 0;
    int cls;
    if (csz <= (32u << (SBH_NCLASS - 1))) {
        for (cls = 0; (32u << cls) < csz; cls++) ;
        csz = 32u << cls;
        hdr = g_sbh_class_head[cls];
        if (hdr) g_sbh_class_head[cls] = MEM32(hdr + 8);
    } else {
        cls = 0xFF;
        csz = (csz + 0xFFFFu) & ~0xFFFFu;
        uint32_t prev = 0, cur = g_sbh_large_head;
        while (cur) {                       /* first fit, no >2x waste */
            uint32_t sz = MEM32(cur + 4);
            if (sz >= csz && sz <= csz * 2) {
                if (prev) MEM32(prev + 8) = MEM32(cur + 8);
                else      g_sbh_large_head = MEM32(cur + 8);
                hdr = cur; csz = sz; break;
            }
            prev = cur; cur = MEM32(cur + 8);
        }
    }
    if (!hdr) {
        hdr = sbh_bump_raw(csz);
        if (!hdr) return 0;
    }
    MEM32(hdr) = SBH_MAGIC;
    MEM32(hdr + 4) = csz;
    MEM32(hdr + 8) = (uint32_t)cls;
    MEM32(hdr + 12) = 0;
    g_sbh_nalloc++; g_sbh_live += csz;
    return hdr + 16;
}

static void sbh_free(uint32_t block)
{
    if (block < 32) return;
    uint32_t hdr = block - 16;
    if (MEM32(hdr) != SBH_MAGIC) { g_sbh_nforeign++; return; } /* foreign/corrupt: leak */
    MEM32(hdr) = SBH_MAGIC + 1;             /* poison against double free */
    uint32_t cls = MEM32(hdr + 8);
    g_sbh_nfree++; g_sbh_live -= MEM32(hdr + 4);
    if (cls < SBH_NCLASS) {
        MEM32(hdr + 8) = g_sbh_class_head[cls];
        g_sbh_class_head[cls] = hdr;
    } else {
        MEM32(hdr + 8) = g_sbh_large_head;
        g_sbh_large_head = hdr;
    }
}

void sub_001668FA(void)   /* __sbh_alloc_block(desc, flags, size), stdcall */
{
    uint32_t size = MEM32(esp + 12);
    eax = sbh_alloc(size);
    esp += 16;
}

/* __sbh_free_block(desc, flags, block) — called by the CRT free front
 * sub_00163E3D. Was a no-op (blocks leaked). stdcall ret 12 -> esp += 16. */
void sub_0016707C(void)
{
    sbh_free(MEM32(esp + 12));
    eax = 1;
    esp += 16;
}

/* ── Override: CRT heap creator (__sbh_heap_init core) @ 0x001664D6 ──
 * The recompiled creator builds the SBH groups but returns a NULL descriptor
 * (its descriptor local stays 0 — the SEH-frame path that captures it is never
 * taken). Return a valid zeroed descriptor so _heap_init (sub_00167E5B) stores
 * a non-NULL 0xBF5000 and downstream code reads sane (zero) descriptor fields.
 * Actual allocations are served by the sub_001668FA bump override above.
 * __stdcall, 6 args -> descriptor VA in eax. Gen body #if 0'd in recomp_0008.c. */
void sub_001664D6(void)
{
    eax = xbox_HeapAlloc(0x2000, 16);  /* 8 KB zeroed; ~0x600 used by descriptor */
    esp += 28;                          /* pop dummy return (4) + 6 args (24) */
}

/* sub_001C6B7C -- IDirectSoundBuffer::Release() adjustor thunk
 * (`push eax; call [vtbl+8]; ret 4`, so __stdcall with one argument and
 * ebx/esi/edi callee-saved).
 *
 * The release chain under it (sub_001C792E -> sub_001C6BEF -> the class
 * destructor through vtable[0]) returns with the guest stack 4 to 8 bytes
 * low, and sub_001C6BEF's `pop edi; pop esi` epilogue then loads the wrong
 * slots -- measured directly at the call site: esi 006A69E8 -> 00000002,
 * esp delta 0 where +4 is correct.
 *
 * That lands in the attract flow: the screen-14 setup sweeps the voice table
 * in sub_0009F400 (`for esi = 0x6A69E8; esi < 0x6A78E8; esi += 0x60`) and
 * calls this Release for every voice of the group. With esi destroyed the
 * sweep restarts from a wild pointer instead of advancing, calls Release on
 * garbage forever, and the guest stack marches down megabytes a second until
 * the host APU refuses another voice.
 *
 * Enforce the ABI the hardware guarantees, the same way sub_001BCC00 and
 * sub_001BC260 are enforced above. */
void sub_001C6B7C_gen(void);
void sub_001C6B7C(void)
{
    uint32_t ei = esp;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    sub_001C6B7C_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
    esp = ei + 4 + 4;   /* dummy return slot + one argument (ret 4) */
}

/* ICALL failure diagnostic (rate-limited).
 * Called by the RECOMP_ICALL macros when an indirect-call target resolves to
 * neither a generated function, a manual override, nor a kernel thunk. */
static uint64_t g_icall_fail_logged = 0;

/* Census of DISTINCT unresolved indirect-call targets.
 *
 * The line-by-line report below is capped at 200 lines, which the boot
 * phase exhausts long before the game reaches its screens -- so a target
 * that only ever fails later (the way sub_000318D0 did, right before a
 * null-pointer crash) never showed up. Track every distinct target with
 * a hit count instead, and dump the table on demand. */
#define ICALL_CENSUS_MAX 256
static struct { uint32_t va; uint32_t hits; } g_icall_census[ICALL_CENSUS_MAX];
static int g_icall_census_n;
static int g_icall_census_overflow;

void recomp_icall_census_dump(void)
{
    int i;
}

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

void sub_00172260(void);
void sub_00172330(void);
void sub_00172360(void);
void sub_00172410(void);
void sub_00172470(void);
void sub_001724F0(void);
void sub_00172520(void);
void sub_00172520_gen(void);
BOOT_MARK2_ABI_ESI(sub_00172520)
void sub_00172580(void);
void sub_001725B0(void);
void sub_001725E0(void);
void sub_00172610(void);
void sub_00172640(void);
void sub_00172680(void);
void sub_001726B0(void);
void sub_001726D0(void);

void sub_001C792E(void) { sub_001C6BEF(); }   /* 0x1C792E: jmp 0x1C6BEF (Release) */

/* 0x0011CE30 / 0x0011CE40 -- two 7-byte thunks the disassembler never saw
 * (no direct caller; reached only through the function pointer at
 * 0x8C8770, which the stage-0x3F renderer sub_0011B5F0 sets to 0x11CE30
 * before its sub-passes issue `call [0x8C8770]`).
 *
 *     0011CE30  call 0x157700   ; draw this object through the walker
 *     0011CE35  ret  4
 *     0011CE40  ret  4          ; the no-op variant
 *
 * Without them the indirect call fell into the unresolved-target path:
 * the walker was never called for those objects (cxbx makes 49 such calls
 * per frame in this stage -- the walls, ground and skybox) and the 4-byte
 * argument was never popped, so the sub-pass then restored its saved
 * matrix from [esp+0x10] of a shifted frame and handed the next ten
 * objects a garbage matrix-stack top. Same call convention as generated
 * code: dummy return slot pushed by the caller, `ret 4` pops it plus the
 * argument. */
void sub_0011CE30(void)
{
    PUSH32(esp, 0); sub_00157700(); /* call 0x00157700 */
    esp += 8; return; /* ret 4 */
}
void sub_0011CE40(void)
{
    esp += 8; return; /* ret 4 */
}

/* ── Manual override table ──────────────────────────────────────────
 * Map original Xbox VA -> hand-written replacement; the trailing {0,0}
 * sentinel keeps the array valid and is skipped at lookup. */
static const struct {
    uint32_t      xbox_va;
    recomp_func_t func;
} g_manual_funcs[] = {
    { 0x0011CE30u, sub_0011CE30 },   /* stage renderer draw thunk: call sub_00157700; ret 4 */
    { 0x0011CE40u, sub_0011CE40 },   /* its no-op variant: ret 4 */
    { 0x001CDCD8u, sub_001CDCD8 },   /* DSOUND page-manager vtable jmp stub */
    { 0x001C792Eu, sub_001C792E },   /* CDirectSoundBuffer vtable Release jmp stub (folded into sub_001C7929) */
    { 0x00172260u, sub_00172260 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172330u, sub_00172330 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172360u, sub_00172360 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172410u, sub_00172410 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172470u, sub_00172470 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x001724F0u, sub_001724F0 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172520u, sub_00172520 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172580u, sub_00172580 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x001725B0u, sub_001725B0 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x001725E0u, sub_001725E0 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172610u, sub_00172610 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172640u, sub_00172640 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x00172680u, sub_00172680 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x001726B0u, sub_001726B0 },   /* CRI ADX sound-device method (recomp_extra3.c) */
    { 0x001726D0u, sub_001726D0 },   /* CRI ADX sound-device method (recomp_extra3.c) */
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




/* DOA3 DIAG: the Sofdec timecode chain sub_0017A2A0 -> sub_001796F0 ->
 * sub_001797FF writes its 4-dword result through the pointer at
 * [esp+0x20] of the shared frame, which should be sub_0017A2A0's stack
 * scratch (esp+8 after its sub esp,1Ch / push ebp / push edi) and instead
 * lands at 0x4BDB4E (the loadfile.afs partition sector table). Log the
 * frame at both ends whenever the destination is not on a stack. */
void sub_0017A2A0_gen(void);
void sub_0017A2A0(void)
{
    static int n = 0;
    uint32_t esp0 = esp;
    if (n < 6) { n++;
        void *bt[8]; USHORT nb = CaptureStackBackTrace(1, 8, bt, NULL); int k;
          }
    sub_0017A2A0_gen();
}
void sub_001797FF_gen(void);
void sub_001797FF(void)
{
    static int n = 0, bad = 0;
    uint32_t dst = MEM32(esp + 0x20);
    int onstack = (dst >= 0x00C40000u && dst < 0x00E40000u) ||   /* main Xbox stack */
                  (dst >= 0x00E40000u && dst < 0x08000000u);      /* heap + high heap (fiber stacks) */
    if (n < 6 || (!onstack && bad < 12)) { n++; if (!onstack) bad++;
         }
    sub_001797FF_gen();
}

/* DOA3 DIAG: esp balance of the PTS->frame chain under the timecode
 * splitter. Expected net +4 (each returns by popping only the dummy). */
#define ESP_BAL_PROBE(fn) \
    void fn##_gen(void); \
    void fn(void) { \
        uint32_t ei = esp; static int s_bad = 0, s_n = 0; \
        fn##_gen(); \
        if ((int)(esp - ei) != 4 && s_bad < 12) { s_bad++; \
             \
             } \
        else if (s_n < 3) { s_n++; \
              } \
    }
ESP_BAL_PROBE(sub_001809E0)
ESP_BAL_PROBE(sub_001809FE)
ESP_BAL_PROBE(sub_0018DE71)
ESP_BAL_PROBE(sub_00191A4D)
ESP_BAL_PROBE(sub_001918E8)
ESP_BAL_PROBE(sub_00191925)
ESP_BAL_PROBE(sub_00191848)

/* DOA3 DIAG: sub_00154420 = translate the matrix-stack top (0x90FAA0) by
 * (x,y,z) = args. It is the first routine that stores NaN into the stack
 * post-movie, and its own arithmetic is right, so the NaN comes in through
 * the arguments. Log them with the native caller chain. */
void sub_00154420_gen(void);
void sub_00154420(void)
{
    static int s_bad = 0, s_n = 0;
    uint32_t a = MEM32(esp + 4), b = MEM32(esp + 8), c = MEM32(esp + 0xC);
    int nan = ((a & 0x7F800000u) == 0x7F800000u) || ((b & 0x7F800000u) == 0x7F800000u) || ((c & 0x7F800000u) == 0x7F800000u);
    if ((nan && s_bad < 8) || s_n < 3) { if (nan) s_bad++; else s_n++;
        void *bt[10]; USHORT nb = CaptureStackBackTrace(1, 10, bt, NULL); int k;
        float fa, fb, fc; memcpy(&fa, &a, 4); memcpy(&fb, &b, 4); memcpy(&fc, &c, 4);
          }
    sub_00154420_gen();
}

/* DOA3 DIAG: sub_000910B9 is the fragment that writes the character position
 * table (0x4BB950 + 16*i) from the x87 stack (st0 = a frame count to
 * truncate, st1/st2 = accumulated deltas) and the struct at ebp (base
 * position). Post-movie the stored positions are NaN; show what comes in. */
void sub_000910B9_gen(void);
void sub_000910B9(void)
{
    static int n = 0, bad = 0;
    float f0, f1, f2, b0, b1, b2; uint32_t u;
    f0 = (float)g_fp_stack[g_fp_top & 7]; f1 = (float)g_fp_stack[(g_fp_top + 1) & 7]; f2 = (float)g_fp_stack[(g_fp_top + 2) & 7];
    u = MEM32(g_seh_ebp); memcpy(&b0, &u, 4); u = MEM32(g_seh_ebp + 4); memcpy(&b1, &u, 4); u = MEM32(g_seh_ebp + 8); memcpy(&b2, &u, 4);
    {   int isnan_any = (f0 != f0) || (f1 != f1) || (f2 != f2) || (b0 != b0) || (b1 != b1) || (b2 != b2);
        if (n < 3 || (isnan_any && bad < 8)) { if (isnan_any) bad++; else n++;
            void *bt[8]; USHORT nb = CaptureStackBackTrace(1, 8, bt, NULL); int k;
              } }
    sub_000910B9_gen();
}

/* D3DDevice_GetBackBuffer (0x1B15B0) and D3DDevice_GetDepthStencilSurface
 * (0x1B1870) must preserve esi/edi/ebx like any cdecl callee.
 *
 * sub_00157EC0 computes its slot index into esi (esi = 5*index), calls these
 * two, and only then reads the saved colour/depth surfaces at
 * [esi*8 + 0x90FE1C] / [+0x90FE20] to hand to SetRenderTarget. With esi
 * clobbered across the calls it read a wrong slot and passed 1 -- a refcount,
 * not a surface -- so SetViewport clamped every viewport against a 1x1
 * "surface" (the logged w=1 h=1 / w=1 h=4006) and the whole title screen
 * collapsed to nothing while the game still issued tens of thousands of
 * draws. Same failure mode as the Present/esi bug. Report any correction so
 * this is evidence, not a guess. */
#define D3D_ABI_KEEP(name)                                                   \
    void name##_gen(void);                                                   \
    void name(void) {                                                        \
        uint32_t _si = esi, _di = edi, _bx = ebx;                            \
        name##_gen();                                                        \
        if (esi != _si || edi != _di || ebx != _bx) {                        \
            static int s_n = 0;                                              \
            if (s_n < 6) { s_n++;                                            \
                                       \
                 }                                            \
            esi = _si; edi = _di; ebx = _bx;                                 \
        }                                                                    \
    }
D3D_ABI_KEEP(sub_001B15B0)
D3D_ABI_KEEP(sub_001B1870)

/* Sofdec player flag setter: (player, index, value). Index 6 = audio enabled. */
void sub_0017D520_gen(void);
void sub_0017D520(void)
{
    static int n = 0;
    uint32_t pl = MEM32(esp + 4), idx = MEM32(esp + 8), val = MEM32(esp + 12);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (idx == 6 && n < 20) { n++;
         }
    sub_0017D520_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* Sofdec per-track word setter: (player, track, value). Track 6 = audio; value 1 here disables it. */
void sub_0017F750_gen(void);
void sub_0017F750(void)
{
    static int n = 0;
    uint32_t pl = MEM32(esp + 4), idx = MEM32(esp + 8), val = MEM32(esp + 12);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (n < 30) { n++;
         }
    sub_0017F750_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* Sofdec per-stream word setter (player, stream, value) for the +0xD3C "finished" word. */
void sub_0017EEE0_gen(void);
void sub_0017EEE0(void)
{
    static int n = 0;
    uint32_t pl = MEM32(esp + 4), idx = MEM32(esp + 8), val = MEM32(esp + 12);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (n < 30) { n++;
         }
    sub_0017EEE0_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* CRI ADX DirectSound device: write method (dev, src, ?, nsamples, chflag). Two writes per region were observed. */
void sub_00172770_gen(void);
void sub_00172770(void)
{
    static int n = 0;
    uint32_t dev = MEM32(esp + 4), ch = MEM32(esp + 8), soff = MEM32(esp + 12), src = MEM32(esp + 16), ns = MEM32(esp + 20);
    uint32_t base = MEM32(dev + 0x20), dst = base + soff * 4;
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    int ok = (src > 0x1000 && src < 0x8000000), okd = (dst > 0x1000 && dst < 0x8000000);
    if (ch == 0 && (n < 200 || (n % 100) == 0)) {
         }
    sub_00172770_gen();
    if (ch == 0) { n++; if (n > 200 && (n % 100) != 1) goto skip_after;
         }
skip_after:
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

/* D3D8 resource create (sub_001B4B80): its tail stores the new resource through the
 * caller-supplied out pointer; a NULL out pointer writes guest address 0, which the
 * scene walker later reads as an object pointer (0x1E10D0) and spins on. */
void sub_001B4B80_gen(void);
void sub_001B4B80(void)
{
    static int n = 0;
    uint32_t a[9], i; for (i = 0; i < 9; i++) a[i] = MEM32(esp + 4 + 4 * i);
    uint32_t s_edi = edi, s_esi = esi, s_ebx = ebx;
    if (n < 40) { n++;
        
        { void *bt[10]; USHORT nb = CaptureStackBackTrace(0, 10, bt, NULL); int k;  for (k = 0; k < nb; k++) ; }
          }
    sub_001B4B80_gen();
    edi = s_edi; esi = s_esi; ebx = s_ebx;
}

BOOT_MARK2_ABI_ESI(sub_001BA530)
BOOT_MARK2_ABI_ESI(sub_00163E20)
void sub_001B4C10_gen(void);
void sub_001B4C10(void)   /* tail of the D3D resource create: *out = resource */
{
    static int n = 0;
    if (n < 8) { n++;  }
    sub_001B4C10_gen();
}
