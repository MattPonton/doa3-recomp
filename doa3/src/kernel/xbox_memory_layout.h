/**
 * Xbox Memory Layout Compatibility
 *
 * The Xbox has 64MB of unified memory shared between CPU and GPU.
 * Memory is identity-mapped (physical == virtual for most of it).
 * The game's code and data were linked expecting specific address ranges:
 *
 *   0x00010000 - 0x002BD000  .text (code)       ~2.73 MB
 *   0x002CC200 - 0x00362AE0  XDK library code   ~600 KB
 *   0x0036B7C0 - 0x003B2354  .rdata (constants) ~280 KB
 *   0x003B2360 - 0x0076F000  .data + BSS        ~3.9 MB
 *
 * On Windows, we need to:
 * 1. Reserve the same virtual address range (0x00010000+)
 * 2. Map sections to their expected addresses
 * 3. Handle the fact that Xbox has no address space layout randomization
 * 4. Provide contiguous memory for GPU resources (textures, VBs)
 *
 * Strategy:
 * - Use VirtualAlloc with specific base addresses to place sections
 * - The recompiled code uses the same addresses for globals and data
 * - GPU memory (D3D textures, etc.) is managed separately by D3D11
 * - Stack and heap use normal Windows allocation
 */

#ifndef DOA3_XBOX_MEMORY_LAYOUT_H
#define DOA3_XBOX_MEMORY_LAYOUT_H

#include <windows.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================
 * Xbox memory map constants
 * ================================================================ */

/* Base address of the XBE in Xbox memory */
#define XBOX_BASE_ADDRESS       0x00010000

/* Start of mapped region - includes low memory (KPCR at 0x0) because
 * game code reads from addresses like 0x20 and 0x28 (Xbox kernel structures). */
#define XBOX_MAP_START          0x00000000

/* Section layout comes from the XBE the code was generated from
 * (xbe_layout.h, written by tools/xbe_layout.py). */
#include "xbe_layout.h"

/* .text section */
#define XBOX_TEXT_VA            DOA3_TEXT_VA
#define XBOX_TEXT_SIZE          DOA3_TEXT_RAW_SIZE

/* .rdata section */
#define XBOX_RDATA_VA           DOA3_RDATA_VA
#define XBOX_RDATA_SIZE         DOA3_RDATA_RAW_SIZE

/* .data section (includes BSS) */
#define XBOX_DATA_VA            DOA3_DATA_VA
#define XBOX_DATA_SIZE          DOA3_DATA_VSIZE     /* virtual size incl. BSS */
#define XBOX_DATA_INIT_SIZE     DOA3_DATA_RAW_SIZE  /* Initialized data in XBE file (raw) */
/* BSS starts at DATA_VA + DATA_INIT_SIZE, zero-initialized */

/* Xbox physical memory */
#define XBOX_TOTAL_RAM          (64 * 1024 * 1024)  /* 64 MB */
#define XBOX_GPU_RESERVED       (4 * 1024 * 1024)   /* ~4 MB for GPU */

/* NV2A GPU register aperture. Emulated by faulting (see the VEH in main.c and
 * nv2a_hook_handle_mmio), so it must stay reserved-but-uncommitted: a host
 * allocation landing here turns it into ordinary RAM and every GPU status
 * register reads 0. xbox_MemoryLayoutInit reserves it before anything else
 * allocates. */
#define XBOX_NV2A_MMIO_BASE     0xFD000000u
#define XBOX_NV2A_MMIO_SIZE     0x01000000u  /* 16 MB */

/* End of mapped sections */
#define XBOX_MAP_END            (XBOX_DATA_VA + XBOX_DATA_SIZE)

/* Total virtual space needed (from XBOX_MAP_START, not XBOX_BASE_ADDRESS) */
#define XBOX_MAP_TOTAL_SIZE     (XBOX_MAP_END - XBOX_MAP_START)

/* ================================================================
 * Memory initialization
 * ================================================================ */

/**
 * Initialize the Xbox memory layout.
 *
 * Reserves the virtual address range 0x00010000 through 0x0076F000
 * and maps the XBE sections to their expected addresses:
 * - .rdata: copied from XBE, read-only
 * - .data: initialized portion copied from XBE, BSS zeroed
 *
 * Note: .text is NOT mapped here - the recompiled code is native
 * Windows code and doesn't need to be at the original address.
 * The data sections DO need to be at their original addresses
 * because the recompiled code references globals by absolute address.
 *
 * @param xbe_data  Pointer to the loaded XBE file contents.
 * @param xbe_size  Size of the XBE file.
 * @return TRUE on success, FALSE on failure.
 */
BOOL xbox_MemoryLayoutInit(const void *xbe_data, size_t xbe_size);

/**
 * Release the reserved Xbox memory layout.
 */
void xbox_MemoryLayoutShutdown(void);

/**
 * Check if an address falls within the Xbox memory map.
 */
BOOL xbox_IsXboxAddress(uintptr_t address);

/**
 * Get the base pointer for direct memory access.
 * Returns NULL if memory layout is not initialized.
 */
void *xbox_GetMemoryBase(void);
size_t xbox_GetMemorySize(void);   /* size of one view (mirrors repeat at this stride) */

/**
 * Get the offset from Xbox VA to actual mapped address.
 * actual_address = xbox_va + offset
 * Returns 0 if memory is mapped at original Xbox addresses (ideal case).
 */
ptrdiff_t xbox_GetMemoryOffset(void);

/* ================================================================
 * Xbox stack for recompiled code
 * ================================================================ */

/* ================================================================
 * Kernel data export area
 * ================================================================ */

/** Base VA for kernel data exports (XboxHardwareInfo, XboxKrnlVersion, etc.)
 *  These are kernel exports that are DATA, not functions. The game reads
 *  their thunk entries and dereferences them to access the data. */
/* Upstream used 0x00740000, which lies inside .data's BSS for every DOA3
 * build. It now sits in its own page above the image (xbe_layout.h). */
#define XBOX_KERNEL_DATA_BASE   DOA3_KERNEL_DATA_VA
#define XBOX_KERNEL_DATA_SIZE   4096   /* 4 KB - plenty for all data exports */

/* Offsets within the kernel data area */
#define KDATA_HARDWARE_INFO     0x000  /* XBOX_HARDWARE_INFO (8 bytes) */
#define KDATA_KRNL_VERSION      0x010  /* XBOX_KRNL_VERSION (8 bytes) */
#define KDATA_TICK_COUNT        0x020  /* KeTickCount (4 bytes) */
#define KDATA_LAUNCH_DATA_PAGE  0x030  /* LaunchDataPage (4 bytes, pointer) */
#define KDATA_THREAD_OBJ_TYPE   0x040  /* PsThreadObjectType (4 bytes) */
#define KDATA_EVENT_OBJ_TYPE    0x050  /* ExEventObjectType (4 bytes) */
#define KDATA_XE_IMAGE_FILENAME 0x060  /* XeImageFileName (ANSI_STRING) */
#define KDATA_IO_COMPLETION_TYPE 0x070 /* IoCompletionObjectType (4 bytes) */
#define KDATA_IO_DEVICE_TYPE    0x080  /* IoDeviceObjectType (4 bytes) */
#define KDATA_HAL_DISK_CACHE_PARTITIONS 0x090 /* HalDiskCachePartitionCount (4 bytes) */
#define KDATA_HAL_BOOT_SMC_VIDEO_MODE   0x0A0 /* HalBootSMCVideoMode (4 bytes) */
#define KDATA_HD_KEY            0x100  /* XboxHDKey (16 bytes) */
#define KDATA_SIGNATURE_KEY     0x110  /* XboxSignatureKey (16 bytes) */
#define KDATA_LAN_KEY           0x120  /* XboxLANKey (16 bytes) */
#define KDATA_ALT_SIGNATURE_KEYS 0x130 /* XboxAlternateSignatureKeys (256 bytes) */
#define KDATA_XE_PUBLIC_KEY     0x300  /* XePublicKeyData (284 bytes) */

/** Size of the simulated Xbox stack (2 MB).
 *  Was 8 MB as defense against RECOMP_ICALL stdcall-arg stack leaks, but the
 *  frame-loop esp drift has since been fixed at the root (esp is byte-identical
 *  across frames). Trimmed back to 2 MB because the movie player needs the
 *  low (GPU-addressable, <64 MB) heap space: the Sofdec intro allocates a ring
 *  of 720x480 frame surfaces (~1.3 MB each) that exhausted the 43 MB heap. */
#define XBOX_STACK_SIZE     (256 * 1024)             /* was 1 MB, before that 2 MB; deepest main-thread esp ever logged is ~4 KB below the top, and
                                                 * the audio path (DSOUND regions + ADX movie voices) needs the low heap
                                                 * headroom: the ADX voice-open hit "out of memory (requested 65536)" at
                                                 * 51,105,792/51,118,080 with the 2 MB stack.
                                                 *
                                                 * Trimmed again to 256 KB. Fibers take their stacks from the guest heap
                                                 * (xbox_fiber_create_dormant), so this region only ever carries the main
                                                 * thread, whose deepest esp measured across a full boot + mode select +
                                                 * character select run is 0x00D3EF34 -- 0x10CC below the top, i.e. ~4 KB
                                                 * of the 1 MB in use. The 768 KB reclaimed goes to the low heap, which
                                                 * was running 346 KB short at the character-select reset: the 1.38 MB
                                                 * render-target allocation failed, sub_001B9260 bailed before filling the
                                                 * surface descriptors, and SetViewport then clamped the viewport against
                                                 * a surface it computed as one pixel wide. */

/** Base VA of the stack area (above last XBE section).
 *  DOA3's image (incl. BSS, DOLBY, $$XTIMAGE) extends to ~0x00C31500, so the
 *  stack must sit above that, unlike Burnout 3 (which ended near 0x00770000).
 *
 *  Sits directly above the fake TLS/RW-data pages (xbox_memory_layout.c) with
 *  no padding, because everything below XBOX_HEAP_BASE is low-heap space the
 *  game cannot use. Once D3DDevice_CreateDevice started allocating its real
 *  720x480 depth buffer (1,474,560 bytes -- it had been getting a 4096-byte
 *  stub from a garbage descriptor) the game's own allocations came to
 *  51.7 MB against a 51.4 MB heap and the asset loader died with
 *  "E9040828:'flid' is range outside". Dropping the base from 0x00D00000
 *  reclaims the 768 KB of dead gap between the image and the stack. */
/* Version-specific: first 64 KB boundary above the image, fake TLS/RW-data
 * pages and kernel data page (xbe_layout.h). 0x00C40000 for 3.0. */
#define XBOX_STACK_BASE     DOA3_STACK_BASE

/** Initial ESP value (top of stack, 16-byte aligned). */
#define XBOX_STACK_TOP      (XBOX_STACK_BASE + XBOX_STACK_SIZE - 16)

/* ================================================================
 * Xbox dynamic heap (for MmAllocateContiguousMemory, etc.)
 * ================================================================ */

/** Base VA of the dynamic heap area (above stack). */
#define XBOX_HEAP_BASE      (XBOX_STACK_BASE + XBOX_STACK_SIZE)  /* 0x00880000 */

/** Size of the dynamic heap.
 *  Xbox has 64 MB total RAM. The total mapped region (data + stack + heap)
 *  must equal 64 MB so the RenderWare engine's memory probing stops at the
 *  correct boundary. On a real Xbox, probing past 64 MB causes a page fault
 *  that the engine catches via SEH to determine available memory. */
/* DOA3: the low (GPU/APU-addressable) heap runs to 80 MB, not 64. The
 * console had ~50 MB for the title; cxbx grants the same game ~62 MB and the
 * story fight + Continue needs ~53 MB (measured 2026-09-19: the port sat
 * 40 KB from its cap and any extra voice buffer hung the Continue). Every
 * 26-bit physical mask in the port (D3D8 lib, APU, NV2A, mirror alias) is
 * widened to 27 bits to match. */
#define XBOX_LOW_END        0x05000000u
#define XBOX_HEAP_SIZE      (XBOX_LOW_END - XBOX_HEAP_BASE)  /* ~67.5 MB */

/** No static mirror/guard region. RAM mirror is handled via file mapping
 *  views that alias the same physical pages as the base 64 MB region. */
#define XBOX_MIRROR_SIZE    0
#define XBOX_GUARD_SIZE     0

/* ================================================================
 * High heap (above the console's 64 MB) — DOA3 port extension.
 *
 * The port needs more RAM than the console: the CRT small-block heap is
 * served by 8 MB arena chunks (no SBH bookkeeping) and several runtime
 * buffers (push buffer, fiber stacks) live in the same 64 MB the game's
 * own contiguous allocations use, so boot exhausts the low heap. CPU-only
 * allocations (CRT malloc arenas) go here instead. Nothing GPU-addressable
 * may live here: NV2A physical masks are 26-bit (& 0x03FFFFFF) and would
 * alias low memory. VAs stay 32-bit clean and MEM32-addressable; the
 * 0x80000000 cached mirror only aliases the low 64 MB, which is correct —
 * the console had nothing to mirror up here.
 * ================================================================ */
#define XBOX_HIGH_BASE      0x05000000u   /* above XBOX_LOW_END */
#define XBOX_HIGH_SIZE      (48u * 1024u * 1024u) /* to 128 MB */

/** Allocate CPU-only memory from the high heap (above 64 MB). */
uint32_t xbox_HeapAllocHigh(uint32_t size, uint32_t alignment);
uint32_t xbox_HeapReserveTop(uint32_t size, uint32_t alignment);

/** Number of 64 MB mirror views to pre-map (covers 1.75 GB of address space). */
#define XBOX_NUM_MIRRORS    28

/**
 * Allocate from the Xbox heap. Returns an Xbox VA, or 0 on failure.
 * Alignment must be a power of 2 (minimum 4).
 * Thread-safe: no (single-threaded recompiled code).
 */
uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment);

/**
 * Free a block from the Xbox heap. Currently a no-op (bump allocator).
 */
void xbox_HeapFree(uint32_t xbox_va);

/**
 * Get the file mapping handle for the Xbox memory region.
 * Used by the VEH handler to map additional mirror views on demand.
 * Returns NULL if file mapping is not available.
 */
HANDLE xbox_GetMappingHandle(void);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_XBOX_MEMORY_LAYOUT_H */
