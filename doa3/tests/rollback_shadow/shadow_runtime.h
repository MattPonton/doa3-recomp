#ifndef DOA3_SHADOW_RUNTIME_H
#define DOA3_SHADOW_RUNTIME_H
#include <stdint.h>
#include <stddef.h>
#include "rollback_capture.h"
uintptr_t rb_ptr(uint32_t address, size_t bytes);
void rb_call(uint32_t va);
void rb_dispatch(uint32_t va);
void rb_step(uint32_t va, Doa3RollbackTask *task);
void rb_budget(void);
void rb_site(uint32_t va);
extern int rb_native_camera;
__declspec(noreturn) void rb_abort(const char *reason, uint32_t value);
/* Save acceptance depends on host key state, which captures do not contain. */
int doa3_save_accept_compatible(const void *save, size_t size);
static inline uint8_t doa3_debug_unlock_read8(uint32_t address,uint8_t saved_value) {
    (void)saved_value; rb_abort("debug_unlock_requires_host_state",address);
}
void *rb_memcpy(void *dst, const void *src, size_t bytes);
void *rb_memmove(void *dst, const void *src, size_t bytes);
void *rb_memset(void *dst, int value, size_t bytes);
#endif
