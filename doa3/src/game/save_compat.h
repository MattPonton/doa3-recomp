#ifndef DOA3_SAVE_COMPAT_H
#define DOA3_SAVE_COMPAT_H
#include <stddef.h>
int doa3_save_keys_init(const void *xbe_header, size_t size);
int doa3_save_accept_compatible(const void *save, size_t size);
#endif
