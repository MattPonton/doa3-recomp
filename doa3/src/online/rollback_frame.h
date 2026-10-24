#ifndef DOA3_ROLLBACK_FRAME_H
#define DOA3_ROLLBACK_FRAME_H
#include <stdint.h>

/* Bounded dispatcher audit, NOT a restorable snapshot. Little-endian stream:
 * header[8] = magic, version, RAM bytes, guest offset, page bytes, frame limit,
 *             kernel counter count, reserved; followed by source SHA-256.
 * record[8] = kind, frame, slot, routine, changed pages, presents, mode, phase;
 * followed by 400 kernel counters, then (page index, 4096 bytes) per page.
 * An all-zero record marks successful completion. No footer on failure.
 * Async APU/clock writers remain active: these are observed boundary deltas,
 * not a claim that every change was made by the named task. */
#define DOA3_RBF_MAGIC 0x31464252u
#define DOA3_RBF_VERSION 2u
#define DOA3_RBF_PAGE 4096u
#define DOA3_RBF_FRAMES 2u
#define DOA3_RBF_KERNEL_COUNTS 400u
#define DOA3_RBF_PHASE_COUNT 20u
/* v2 adds ordered outer-loop markers between dispatcher passes only. The
 * routine field is the marker's guest address; slot is UINT32_MAX. */
#define DOA3_RBF_PHASES 0xA64BA,0xA64BF,0xA64C9,0xA6514,0xA6519,0xA651E,0xA6525,0xA652A,0xA6450,0xA6455,0xA645A,0xA645F,0xA6464,0xA6469,0xA6477,0xA6492,0xA64A1,0xA64A6,0xA64AB,0xA64B0
enum { DOA3_RBF_BEGIN=1, DOA3_RBF_TASK_BEGIN, DOA3_RBF_TASK_END, DOA3_RBF_END, DOA3_RBF_PHASE };
void doa3_rollback_frame_boundary(uint32_t routine, int before);
void doa3_rollback_frame_phase(uint32_t address);
void doa3_rollback_ownership_boundary(void);
#endif
