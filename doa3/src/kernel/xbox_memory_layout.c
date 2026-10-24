/**
 * Xbox Memory Layout Implementation
 *
 * Maps the XBE data sections to their expected virtual addresses on Windows.
 * This is critical for the recompiled code which references globals by
 * absolute address (e.g., mov eax, [0x004D532C]).
 *
 * Implementation:
 * 1. VirtualAlloc a contiguous region at XBOX_BASE_ADDRESS
 * 2. Copy .rdata and initialized .data from the XBE
 * 3. Zero-fill the BSS region
 * 4. Set memory protection (read-only for .rdata)
 */

#include "xbox_memory_layout.h"
#include "xbox_heap.h"
#include <stdio.h>
#include <string.h>

/* Section info from XBE analysis */

/* Raw file offsets of the main sections (xbe_layout.h) */
#define TEXT_RAW_OFFSET         DOA3_TEXT_RAW_OFFSET
#define RDATA_RAW_OFFSET        DOA3_RDATA_RAW_OFFSET
#define DATA_RAW_OFFSET         DOA3_DATA_RAW_OFFSET

/* Additional XBE sections to map (DOA3).
 * All sections are placed at their original Xbox VAs because the recompiled
 * code references library data/code by absolute address, and the game's data
 * structures may point into these regions. */
static const struct {
    const char *name;
    DWORD va;
    DWORD size;
    DWORD raw_offset;
} g_extra_sections[] = {
    /* Every section other than .text/.rdata/.data, from xbe_layout.h */
    DOA3_EXTRA_SECTIONS
};
#define NUM_EXTRA_SECTIONS (sizeof(g_extra_sections) / sizeof(g_extra_sections[0]))

static void *g_memory_base = NULL;
static size_t g_memory_size = 0;
static ptrdiff_t g_memory_offset = 0;  /* actual_base - XBOX_BASE_ADDRESS */

/* File mapping handle for the Xbox memory region.
 * Using CreateFileMapping + MapViewOfFileEx allows mirror views to alias
 * the same physical pages as the base region, so writes to mirror addresses
 * (which wrap modulo 64 MB on real Xbox hardware) correctly modify the
 * underlying data. */
static HANDLE g_mapping_handle = NULL;

/* Mirror view pointers for cleanup */
static void *g_mirror_views[XBOX_NUM_MIRRORS] = {0};

/* Separate allocation for Xbox kernel address space (0x80010000+).
 * Some RenderWare code reads the kernel PE header to detect features. */
static void *g_kernel_memory = NULL;

/* Global offset accessible by recompiled code (via recomp_types.h) */
ptrdiff_t g_xbox_mem_offset = 0;

/* Global registers for recompiled code (via recomp_types.h) */
uint32_t g_eax = 0, g_ecx = 0, g_edx = 0, g_esp = 0;
uint32_t g_ebx = 0, g_esi = 0, g_edi = 0;

/* Last operand pair compared by a REP cmps/scas string op — the following jcc/setcc
 * reads these (the recompiler implements rep cmpsb/cmpsw/cmpsd/scasb as a loop that
 * leaves the final compared values here). No yield occurs between the op and its jcc,
 * so plain globals are safe under the cooperative fiber scheduler. */
uint32_t g_str_a = 0, g_str_b = 0;
/* GLOBAL x87 FPU stack (recomp bug #8) — real x87 state is process-global;
 * per-function locals broke every cross-function ST0 value transfer. */
double g_fp_stack[8];
int    g_fp_top;
uint16_t g_x87_cw = 0x027F;   /* x87 default: all exceptions masked, 53-bit, round-nearest */

/* SEH frame pointer bridge (see recomp_types.h for explanation) */
uint32_t g_seh_ebp = 0;

/* ICALL trace ring buffer */
volatile uint32_t g_icall_trace[16] = {0};
volatile uint32_t g_icall_trace_idx = 0;
volatile uint64_t g_icall_count = 0;

BOOL xbox_MemoryLayoutInit(const void *xbe_data, size_t xbe_size)
{
    DWORD old_protect;
    const uint8_t *xbe = (const uint8_t *)xbe_data;

    if (g_memory_base) {
        fprintf(stderr, "xbox_MemoryLayoutInit: already initialized\n");
        return FALSE;
    }

    xbox_heap_checkpoint_invalidate();

    /*
     * Calculate the full range we need to map.
     * From XBOX_MAP_START (0x0) to the end of the furthest section.
     * This includes low memory (KPCR at 0x0-0xFF) which game code reads
     * from, the XBE sections, and the simulated stack.
     */
    DWORD map_end = XBOX_HIGH_BASE + XBOX_HIGH_SIZE;  /* low 64 MB + high CPU-only heap */
    g_memory_size = map_end - XBOX_MAP_START;

    /*
     * Create a file mapping backed by the page file.
     *
     * Using file mapping instead of VirtualAlloc allows us to map the same
     * physical pages at multiple virtual addresses via MapViewOfFileEx.
     * This is critical for the Xbox RAM mirror: the Xbox memory controller
     * uses a 26-bit address bus, so ALL addresses wrap modulo 64 MB.
     * Code that writes to address 0x20000448 is really writing to 0x00000448.
     * With file mapping views, we create aliased mappings at 64 MB intervals
     * that all point to the same physical memory.
     */
    g_mapping_handle = CreateFileMappingA(
        INVALID_HANDLE_VALUE,   /* page file backed */
        NULL,                   /* default security */
        PAGE_READWRITE,         /* read-write access */
        0,                      /* high DWORD of size */
        (DWORD)g_memory_size,   /* low DWORD of size (64 MB) */
        NULL                    /* unnamed mapping */
    );
    if (!g_mapping_handle) {
        fprintf(stderr, "xbox_MemoryLayoutInit: CreateFileMapping failed (error %lu)\n",
                GetLastError());
        return FALSE;
    }

    /*
     * Map the base view at the desired virtual address.
     * Try the original Xbox base address first. If that fails (common on
     * Windows 11 where low addresses are often reserved), try page-aligned
     * addresses upward until we find a free region.
     */
    {
        static const uintptr_t try_bases[] = {
            XBOX_BASE_ADDRESS,      /* 0x00010000 - original Xbox address */
            0x00800000,             /* 8 MB - above typical PEB/TEB region */
            0x01000000,             /* 16 MB */
            0x02000000,             /* 32 MB */
            0x10000000,             /* 256 MB */
            0x20000000,             /* additional fallbacks: the 128MB view is
                                     * fragile to DLL/heap placement (adding a
                                     * user32 import shifted layout enough to
                                     * kill 0x10000000) */
            0x30000000,
            0x40000000,
            0x60000000,
            0x70000000,
            0,                      /* sentinel - let OS choose */
        };

        for (int i = 0; try_bases[i] != 0 || i == 0; i++) {
            LPVOID hint = try_bases[i] ? (LPVOID)try_bases[i] : NULL;
            g_memory_base = MapViewOfFileEx(
                g_mapping_handle,
                FILE_MAP_ALL_ACCESS,
                0, 0,           /* offset into mapping */
                g_memory_size,  /* size */
                hint            /* desired base address */
            );
            if (g_memory_base) {
                if (try_bases[i] != 0 && (uintptr_t)g_memory_base != try_bases[i]) {
                    /* OS gave us a different address, retry */
                    UnmapViewOfFile(g_memory_base);
                    g_memory_base = NULL;
                    continue;
                }
                break;
            }
        }
    }

    if (!g_memory_base) {
        fprintf(stderr, "xbox_MemoryLayoutInit: failed to map base view (%zu KB)\n",
                g_memory_size / 1024);
        CloseHandle(g_mapping_handle);
        g_mapping_handle = NULL;
        return FALSE;
    }

    g_memory_offset = (uintptr_t)g_memory_base - XBOX_MAP_START;

    if (g_memory_offset == 0) {
        fprintf(stderr, "xbox_MemoryLayoutInit: mapped %zu KB at 0x%08X (original Xbox address)\n",
                g_memory_size / 1024, XBOX_MAP_START);
    } else {
        fprintf(stderr, "xbox_MemoryLayoutInit: mapped %zu KB at 0x%p (offset %+td from Xbox base)\n",
                g_memory_size / 1024, g_memory_base, g_memory_offset);
    }

    /*
     * Reserve the NV2A MMIO aperture (Xbox VA 0xFD000000-0xFDFFFFFF).
     *
     * The GPU registers are emulated by faulting: the VEH in main.c catches an
     * access in this window and routes it to nv2a_hook_handle_mmio, which reads
     * and writes the NV2A register state machine. That only works while the
     * window is INACCESSIBLE, so the access raises the exception.
     *
     * Nothing used to hold the window, and every host allocation in the port
     * (xbox_HeapAlloc, the kernel Mm* bridges, the CRT) calls VirtualAlloc with
     * a NULL base, letting the OS pick. A multi-megabyte allocation landed on
     * 0x11C000000 -- Xbox VA 0xFC000000 -- and ran straight through the whole
     * aperture, leaving it committed PAGE_READWRITE. From then on GPU register
     * accesses no longer faulted: they read plain zeroed RAM, so every status
     * register answered 0.
     *
     * That is fatal to the D3D FIFO-idle wait (sub_001BB705). It spins until
     * NV_PFIFO_CACHE1_STATUS and NV_PFIFO_RUNOUT_STATUS both report LOW_MARK
     * (bit 4). pfifo_read answers both correctly, but with the aperture backed
     * by RAM it was never consulted, the bits read 0, and the loop never
     * exited -- the game hung with the last frame still on screen.
     *
     * Reserving without committing does both jobs: it keeps every later
     * VirtualAlloc(NULL, ...) out of the window, and a reserved page is
     * inaccessible, so accesses still fault into the emulator. Do it here,
     * before the heap and the kernel bridges allocate anything.
     */
    {
        void *mmio = (void *)((uintptr_t)XBOX_NV2A_MMIO_BASE + g_memory_offset);
        LPVOID r = VirtualAlloc(mmio, XBOX_NV2A_MMIO_SIZE,
                                MEM_RESERVE, PAGE_NOACCESS);
        if (r == mmio) {
            fprintf(stderr, "xbox_MemoryLayoutInit: NV2A MMIO aperture reserved "
                            "at %p (Xbox VA 0x%08X, %u MB)\n",
                    mmio, (unsigned)XBOX_NV2A_MMIO_BASE,
                    (unsigned)(XBOX_NV2A_MMIO_SIZE / (1024 * 1024)));
        } else {
            /* Loud: without it the GPU status registers silently read 0 and
             * the first FIFO-idle wait hangs the game. */
            fprintf(stderr, "xbox_MemoryLayoutInit: WARNING could not reserve the "
                            "NV2A MMIO aperture at %p (got %p, error %lu) -- GPU "
                            "register emulation will not trap\n",
                    mmio, r, GetLastError());
            if (r) VirtualFree(r, 0, MEM_RELEASE);
        }
    }

    /*
     * Reserve the MCPX APU MMIO aperture (Xbox VA 0xFE800000-0xFE87FFFF) for
     * the same reason. The recompiled DirectSound driver drives the audio
     * front end by storing methods into this window, and the VEH routes those
     * faults to apu_hook_handle_mmio. Nothing held the window, so a host
     * allocation could land on it: measured at the attract start, the page
     * at 0xFE820000 turned up committed PAGE_READWRITE (a 3 MB region), the
     * driver's SET_CURRENT_VOICE / CFG_FMT / VOICE_OFF stores for a buffer it
     * was stopping stopped faulting, the voice stayed active, and
     * CMcpxBuffer_Stop spun forever in its retire wait -- the intermittent
     * freeze ten seconds into the attract loop. Reserved, the stores trap
     * again and later VirtualAlloc(NULL, ...) calls stay out of the window.
     */
    {
        void *apu = (void *)((uintptr_t)0xFE800000u + g_memory_offset);
        LPVOID r = VirtualAlloc(apu, 0x00080000u, MEM_RESERVE, PAGE_NOACCESS);
        if (r == apu) {
            fprintf(stderr, "xbox_MemoryLayoutInit: APU MMIO aperture reserved at %p "
                            "(Xbox VA 0xFE800000, 512 KB)\n", apu);
        } else {
            fprintf(stderr, "xbox_MemoryLayoutInit: WARNING could not reserve the APU "
                            "MMIO aperture at %p (got %p, error %lu) -- audio register "
                            "writes may be lost\n", apu, r, GetLastError());
            if (r) VirtualFree(r, 0, MEM_RELEASE);
        }
    }

    /*
     * Helper macro: convert Xbox VA to actual mapped address.
     * When g_memory_offset == 0 (ideal case), this is identity.
     */
    #define XBOX_VA(va) ((void *)((uintptr_t)(va) + g_memory_offset))

    /*
     * Copy XBE header to base address.
     * The Xbox kernel maps the XBE image header at 0x00010000.
     * Game code reads kernel thunk table, certificate data, and
     * section info from this region.
     */
    {
        /* XBE header size is at file offset 0x0108 (SizeOfImageHeader) */
        DWORD header_size = 0;
        if (xbe_size >= 0x10C) {
            header_size = *(const DWORD *)(xbe + 0x0108);
        }
        if (header_size == 0 || header_size > 0x10000)
            header_size = 0x1000;  /* fallback: 4KB */
        if (header_size > xbe_size)
            header_size = (DWORD)xbe_size;
        memcpy(XBOX_VA(XBOX_BASE_ADDRESS), xbe, header_size);
        fprintf(stderr, "  XBE header: %u bytes at %p (Xbox VA 0x%08X)\n",
                header_size, XBOX_VA(XBOX_BASE_ADDRESS), XBOX_BASE_ADDRESS);
    }

    /*
     * Copy .text section from XBE to its original Xbox VA.
     *
     * Even though the recompiled code runs natively (not from the .text
     * section), the RW engine's memory walker processes ALL physical RAM
     * as data structures, including the code pages. On Xbox, addresses
     * past 64MB wrap back to lower memory via the RAM mirror. When the
     * walker crosses 64MB and reads from mirrored .text addresses, it
     * expects actual code bytes (not zeros). Without this, the walker's
     * internal data structures are corrupted by zero-filled gaps.
     */
    if (TEXT_RAW_OFFSET + XBOX_TEXT_SIZE <= xbe_size) {
        memcpy(XBOX_VA(XBOX_TEXT_VA), xbe + TEXT_RAW_OFFSET, XBOX_TEXT_SIZE);
        fprintf(stderr, "  .text: %u bytes at %p (Xbox VA 0x%08X) [for memory walker]\n",
                XBOX_TEXT_SIZE, XBOX_VA(XBOX_TEXT_VA), XBOX_TEXT_VA);
    } else {
        fprintf(stderr, "  WARNING: .text raw data out of bounds\n");
    }

    /*
     * Copy .rdata section from XBE.
     */
    if (RDATA_RAW_OFFSET + XBOX_RDATA_SIZE <= xbe_size) {
        memcpy(XBOX_VA(XBOX_RDATA_VA), xbe + RDATA_RAW_OFFSET, XBOX_RDATA_SIZE);
        fprintf(stderr, "  .rdata: %u bytes at %p (Xbox VA 0x%08X)\n",
                XBOX_RDATA_SIZE, XBOX_VA(XBOX_RDATA_VA), XBOX_RDATA_VA);
    } else {
        fprintf(stderr, "  WARNING: .rdata raw data out of bounds\n");
    }

    /*
     * Copy initialized .data section from XBE.
     * BSS (the rest of .data) is already zeroed by VirtualAlloc.
     */
    if (DATA_RAW_OFFSET + XBOX_DATA_INIT_SIZE <= xbe_size) {
        memcpy(XBOX_VA(XBOX_DATA_VA), xbe + DATA_RAW_OFFSET, XBOX_DATA_INIT_SIZE);
        fprintf(stderr, "  .data: %u bytes initialized, %u bytes BSS at %p (Xbox VA 0x%08X)\n",
                XBOX_DATA_INIT_SIZE, XBOX_DATA_SIZE - XBOX_DATA_INIT_SIZE,
                XBOX_VA(XBOX_DATA_VA), XBOX_DATA_VA);
    } else {
        fprintf(stderr, "  WARNING: .data raw data out of bounds\n");
    }

    /*
     * Copy extra sections (DOLBY, XON_RD, .data1).
     */
    for (size_t i = 0; i < NUM_EXTRA_SECTIONS; i++) {
        if (g_extra_sections[i].raw_offset + g_extra_sections[i].size <= xbe_size) {
            memcpy(XBOX_VA(g_extra_sections[i].va),
                   xbe + g_extra_sections[i].raw_offset, g_extra_sections[i].size);
            fprintf(stderr, "  %s: %u bytes at %p (Xbox VA 0x%08X)\n",
                    g_extra_sections[i].name, g_extra_sections[i].size,
                    XBOX_VA(g_extra_sections[i].va), g_extra_sections[i].va);
        }
    }

    /*
     * NOTE: .rdata is NOT set read-only.
     * VirtualProtect rounds to page boundaries, and the .rdata end (0x003B2454)
     * and .data start (0x003B2360) share the same 4KB page (0x003B2000-0x003B2FFF).
     * Making .rdata read-only also makes the first ~0xCA0 bytes of .data read-only,
     * which causes game initialization code to fault when writing to .data globals
     * in that overlap range.
     */
    (void)old_protect;

    #undef XBOX_VA

    /* Set the global offset for recompiled code MEM macros */
    g_xbox_mem_offset = g_memory_offset;

    /*
     * Initialize the Xbox stack for recompiled code.
     * The stack area lives at XBOX_STACK_BASE in Xbox address space.
     * g_esp is the global stack pointer shared by all translated functions.
     */
    g_esp = XBOX_STACK_TOP;
    fprintf(stderr, "  Stack: %u KB at Xbox VA 0x%08X (ESP = 0x%08X)\n",
            XBOX_STACK_SIZE / 1024, XBOX_STACK_BASE, g_esp);

    /*
     * Populate the fake Thread Information Block (TIB) at Xbox VA 0x0.
     *
     * The original Xbox code uses fs:[offset] to read per-thread data,
     * but the recompiler drops the fs: segment prefix and generates
     * MEM32(offset) instead. Since we mapped low memory (0x0-0xFFFF),
     * we populate the TIB fields that game code accesses:
     *
     *   fs:[0x00] = SEH exception list (-1 = end of chain)
     *   fs:[0x04] = stack base (top of stack)
     *   fs:[0x08] = stack limit (bottom of stack)
     *   fs:[0x18] = self pointer (TIB address)
     *   fs:[0x20] = KPCR Prcb pointer (→ fake structure)
     *   fs:[0x28] = TLS / RW engine context pointer
     *
     * We use free space in the BSS area for the fake structures.
     */
    {
        #define XBOX_VA(va) ((void *)((uintptr_t)(va) + g_memory_offset))
        #define MEM32_INIT(va, val) (*(uint32_t *)XBOX_VA(va) = (uint32_t)(val))

        /* Fake TIB at address 0x0 */
        MEM32_INIT(0x00, 0xFFFFFFFF);       /* SEH: end of chain */
        MEM32_INIT(0x04, XBOX_STACK_TOP);   /* Stack base (high address) */
        MEM32_INIT(0x08, XBOX_STACK_BASE);  /* Stack limit (low address) */
        MEM32_INIT(0x18, 0x00000000);       /* Self pointer (TIB at VA 0) */

        /*
         * fs:[0x20] - On Xbox KPCR, this is the Prcb pointer.
         * Game code reads [fs:[0x20] + 0x250] which on the real Xbox
         * accesses a D3D cache structure. We set it to 0 so the read
         * at offset 0x250 returns 0, causing the cache init to be skipped.
         */
        MEM32_INIT(0x20, 0x00000000);

        /*
         * fs:[0x28] - Thread local storage / RW engine context.
         * The RW engine reads [fs:[0x28] + 0x28] to get a pointer
         * to its data area. We allocate a fake structure at 0x00760000
         * (in the BSS area) and a data buffer at 0x00700000.
         */
        /* Placed in the gap between DOA3's image end (~0x00C31500) and the
         * stack base (XBOX_STACK_BASE) so they don't collide with game BSS.
         * The stack base was lowered to 0x00C40000 to give the low heap the
         * 768 KB it needs for the driver's real depth buffer, so these two
         * pages moved down with it and now sit immediately after the image. */
        #define FAKE_TLS_VA     DOA3_FAKE_TLS_VA     /* Fake TLS structure (0x00C32000 for 3.0) */
        #define FAKE_RWDATA_VA  DOA3_FAKE_RWDATA_VA  /* Scratch context data area (0x00C36000 for 3.0) */

        MEM32_INIT(0x28, FAKE_TLS_VA);
        /* TLS[0x28] = pointer to RW data area */
        MEM32_INIT(FAKE_TLS_VA + 0x28, FAKE_RWDATA_VA);

        fprintf(stderr, "  TIB: fake TIB at VA 0x0, TLS at 0x%08X, RW data at 0x%08X\n",
                FAKE_TLS_VA, FAKE_RWDATA_VA);

        #undef FAKE_TLS_VA
        #undef FAKE_RWDATA_VA
        #undef MEM32_INIT
        #undef XBOX_VA
    }

    /*
     * Allocate a page at Xbox kernel address space (0x80010000).
     *
     * RenderWare's Xbox driver code (xbcache.c) reads MEM32(0x8001003C)
     * to parse the Xbox kernel's PE header and find the INIT section for
     * CPU cache line sizing. On PC, we provide a minimal fake PE header
     * with 0 sections so the function gracefully skips the cache init.
     *
     * The actual native address is 0x80010000 + g_memory_offset.
     */
    {
        #define XBOX_KERNEL_BASE 0x80010000u
        #define KERNEL_PAGE_SIZE 4096
        uintptr_t kernel_native = XBOX_KERNEL_BASE + g_memory_offset;
        g_kernel_memory = VirtualAlloc(
            (LPVOID)kernel_native,
            KERNEL_PAGE_SIZE,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );
        if (g_kernel_memory) {
            /* Zero-fill then set e_lfanew = 0x80 (offset to PE header).
             * With the rest zeroed, NumberOfSections = 0 and the INIT
             * section search finds nothing, which is the safe path. */
            memset(g_kernel_memory, 0, KERNEL_PAGE_SIZE);
            *(uint32_t *)((uint8_t *)g_kernel_memory + 0x3C) = 0x80;  /* e_lfanew */
            fprintf(stderr, "  Kernel: fake PE header at Xbox VA 0x%08X (native %p)\n",
                    XBOX_KERNEL_BASE, g_kernel_memory);
        } else {
            fprintf(stderr, "  WARNING: could not map Xbox kernel VA 0x%08X\n",
                    XBOX_KERNEL_BASE);
        }
        #undef XBOX_KERNEL_BASE
        #undef KERNEL_PAGE_SIZE
    }

    /* Initialize the dynamic heap. */
    fprintf(stderr, "  Heap: %u MB at Xbox VA 0x%08X-0x%08X\n",
            XBOX_HEAP_SIZE / (1024 * 1024), XBOX_HEAP_BASE,
            XBOX_HEAP_BASE + XBOX_HEAP_SIZE);

    /*
     * Map mirror views of the 64 MB region.
     *
     * On retail Xbox, physical RAM wraps at 64 MB due to the 26-bit
     * address bus. Address 0x04070000 reads the same data as 0x00070000.
     * The RenderWare engine's memory walker crosses 64 MB and accesses
     * mirrored data for an extended walk covering 256+ MB of virtual
     * addresses. Game init code also writes large data structures past
     * 64 MB that on real hardware wrap into physical RAM.
     *
     * We map additional views of the SAME file mapping section at 64 MB
     * intervals. All views alias the same physical pages, so reads and
     * writes at any mirror address correctly access the base data.
     */
    {
        int mirrors_ok = 0, mirrors_partial = 0;
        for (int m = 0; m < XBOX_NUM_MIRRORS; m++) {
            uintptr_t mirror_base = (uintptr_t)g_memory_base +
                                    (uintptr_t)(m + 1) * g_memory_size;
            g_mirror_views[m] = MapViewOfFileEx(
                g_mapping_handle,
                FILE_MAP_ALL_ACCESS,
                0, 0,
                g_memory_size,
                (LPVOID)mirror_base
            );
            if (g_mirror_views[m]) {
                mirrors_ok++;
            } else {
                /* A full-size view can fail simply because the address
                 * range is a little short, and losing the whole mirror
                 * leaves a 128 MB hole that guest code walks straight
                 * into -- every access there faults through the VEH and
                 * reads garbage. Mirror 11 at 0x78000000 is the case that
                 * bit us: the range is MEM_FREE but only 0x7FE0000 bytes
                 * of it, because KUSER_SHARED_DATA sits at 0x7FFE0000.
                 * Map as much as will fit rather than nothing at all --
                 * that covered every faulting address we had seen in the
                 * hole (guest 0x5FC00004, 0x5FD1BE90, 0x5FE4F910, all
                 * below the 0x5FFE0000 cut-off). */
                DWORD err = GetLastError();
                MEMORY_BASIC_INFORMATION mbi;
                SIZE_T avail = 0;
                if (VirtualQuery((LPCVOID)mirror_base, &mbi, sizeof mbi) &&
                    mbi.State == MEM_FREE) {
                    avail = mbi.RegionSize & ~(SIZE_T)0xFFFF;   /* 64K granularity */
                    if (avail > g_memory_size) avail = g_memory_size;
                }
                if (avail >= 0x10000) {
                    g_mirror_views[m] = MapViewOfFileEx(
                        g_mapping_handle, FILE_MAP_ALL_ACCESS, 0, 0,
                        avail, (LPVOID)mirror_base);
                }
                if (g_mirror_views[m]) {
                    mirrors_partial++;
                    fprintf(stderr, "  Mirror %d: partial at %p (%llu of %llu MB; "
                                    "full view gave error %lu)\n",
                            m + 1, (void *)mirror_base,
                            (unsigned long long)(avail / (1024 * 1024)),
                            (unsigned long long)(g_memory_size / (1024 * 1024)), err);
                } else {
                    fprintf(stderr, "  Mirror %d: FAILED at %p (error %lu, "
                                    "free run %llu bytes)\n",
                            m + 1, (void *)mirror_base, err,
                            (unsigned long long)mbi.RegionSize);
                }
            }
        }
        fprintf(stderr, "  RAM mirror: %d full + %d partial of %d views\n",
                mirrors_ok, mirrors_partial, XBOX_NUM_MIRRORS);
    }

    fprintf(stderr, "xbox_MemoryLayoutInit: complete\n");
    return TRUE;
}

void xbox_MemoryLayoutShutdown(void)
{
    xbox_heap_checkpoint_invalidate();
    if (g_kernel_memory) {
        VirtualFree(g_kernel_memory, 0, MEM_RELEASE);
        g_kernel_memory = NULL;
    }
    /* Unmap mirror views first */
    for (int m = 0; m < XBOX_NUM_MIRRORS; m++) {
        if (g_mirror_views[m]) {
            UnmapViewOfFile(g_mirror_views[m]);
            g_mirror_views[m] = NULL;
        }
    }
    /* Unmap base view */
    if (g_memory_base) {
        UnmapViewOfFile(g_memory_base);
        g_memory_base = NULL;
        g_memory_size = 0;
    }
    /* Close file mapping handle */
    if (g_mapping_handle) {
        CloseHandle(g_mapping_handle);
        g_mapping_handle = NULL;
    }
    fprintf(stderr, "xbox_MemoryLayoutShutdown: released\n");
}

BOOL xbox_IsXboxAddress(uintptr_t address)
{
    return (address >= XBOX_BASE_ADDRESS &&
            address < XBOX_BASE_ADDRESS + g_memory_size);
}

void *xbox_GetMemoryBase(void)
{
    return g_memory_base;
}
size_t xbox_GetMemorySize(void)
{
    return g_memory_size;
}

ptrdiff_t xbox_GetMemoryOffset(void)
{
    return g_memory_offset;
}

HANDLE xbox_GetMappingHandle(void)
{
    return g_mapping_handle;
}
