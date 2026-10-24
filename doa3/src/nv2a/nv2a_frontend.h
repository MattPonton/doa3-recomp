#ifndef DOA3_NV2A_FRONTEND_H
#define DOA3_NV2A_FRONTEND_H
#include <stddef.h>
#include <stdint.h>
/* Private synchronous command replay, NOT a GPU renderer. Uses the production
 * USER/PFIFO handlers, push-buffer parser and target routing. PGRAPH commands
 * contribute to the ordered method digest; they do not draw or change textures.
 * Flips, IRQ writes and other MMIO fail closed. No device or window is created.
 * All calls are confined to the creating thread, with caller-owned RAM pinned.
 * Stop on a failed operation: RAM may have changed; faulted instances cannot
 * capture or restore. Never attach this component to the live renderer. */
typedef struct Nv2aFrontend Nv2aFrontend;
typedef uint32_t (*Nv2aFrontendRead)(void *,uint32_t);
typedef void (*Nv2aFrontendWrite)(void *,uint32_t,uint32_t);
Nv2aFrontend *nv2a_frontend_create(const void *queue_seed,size_t bytes);
void nv2a_frontend_destroy(Nv2aFrontend *);
const char *nv2a_frontend_error(const Nv2aFrontend *);
int nv2a_frontend_read(Nv2aFrontend *,uint32_t address,unsigned width,uint32_t *out);
int nv2a_frontend_write(Nv2aFrontend *,uint32_t address,unsigned width,uint32_t value);
int nv2a_frontend_submit(Nv2aFrontend *,uint32_t dev,Nv2aFrontendRead,Nv2aFrontendWrite,void *);
int nv2a_frontend_targets(Nv2aFrontend *,uint32_t dev,uint32_t surface,Nv2aFrontendRead,void *);
/* Raw queue export is for comparison with existing capture files. The local
 * checkpoint below additionally binds identity/generation and session epoch. */
int nv2a_frontend_export(Nv2aFrontend *,void *,size_t);
size_t nv2a_frontend_checkpoint_size(void);
int nv2a_frontend_capture(Nv2aFrontend *,uint64_t epoch,void *,size_t);
int nv2a_frontend_validate(Nv2aFrontend *,uint64_t epoch,const void *,size_t);
int nv2a_frontend_restore(Nv2aFrontend *,uint64_t epoch,const void *,size_t);
#endif
