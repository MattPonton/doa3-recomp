#pragma once
#include <stddef.h>
#include <stdint.h>
typedef struct MCPXAPUState MCPXAPUState;
/* Command-backend snapshot only. No threads, locks, OS audio handles or host
 * pointers are serialized. Caller pins the APU while exporting. Restoring is
 * for a private offline instance; this does not rewind a running audio device. */
typedef struct ApuRollbackClock {
    uint64_t base, chip;
    int32_t debt;
    uint32_t irq_pending;
} ApuRollbackClock;
size_t apu_rollback_size(void);
int apu_rollback_export(const MCPXAPUState *, const ApuRollbackClock *, void *, size_t);
int apu_rollback_import(MCPXAPUState *, ApuRollbackClock *, const void *, size_t);
int apu_rollback_validate(const void *, size_t, ApuRollbackClock *); /* optional clock output */
/* Production capture: succeeds only while deterministic APU ownership is held. */
int mcpx_apu_rollback_capture(void *, size_t);
/* Nonblocking, nonrecursive pin of an already deterministic command backend.
 * The deterministic-session owner must call at an unwound game boundary.
 * On success the APU lock stays held until release on that same thread.
 * No stepping, yielding, session changes or output submission while pinned.
 * This does not snapshot/fence the host mixer or rewind queued device audio. */
int mcpx_apu_rollback_acquire(void);
int mcpx_apu_rollback_release(void);
