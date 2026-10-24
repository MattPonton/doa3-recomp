#pragma once
#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#ifdef DOA3_SHADOW_GPU
size_t rb_gpu_size(void);
int rb_gpu_load(const void *snapshot,size_t bytes);
int rb_gpu_save(void *snapshot,size_t bytes);
uintptr_t rb_gpu_pointer(uint32_t address);
/* Unrelated exceptions return CONTINUE_SEARCH so APU can handle its range. */
int rb_gpu_exception(EXCEPTION_POINTERS *exception);
const char *rb_gpu_error(void);
uint32_t rb_gpu_error_address(void);
int rb_gpu_targets(uint32_t dev,uint32_t arg,uint32_t (*read)(void *,uint32_t),void *opaque);
int rb_gpu_submit(uint32_t dev,uint32_t (*read)(void *,uint32_t),void (*write)(void *,uint32_t,uint32_t),void *opaque);
#else
static size_t rb_gpu_size(void) { return 0; }
static int rb_gpu_load(const void *s,size_t n) { (void)s; return n==0; }
static int rb_gpu_save(void *s,size_t n) { (void)s; return n==0; }
static uintptr_t rb_gpu_pointer(uint32_t a) { (void)a; return 0; }
static int rb_gpu_exception(EXCEPTION_POINTERS *e) { (void)e; return EXCEPTION_CONTINUE_SEARCH; }
static const char *rb_gpu_error(void) { return NULL; }
static uint32_t rb_gpu_error_address(void) { return 0; }
static int rb_gpu_targets(uint32_t d,uint32_t a,uint32_t (*r)(void *,uint32_t),void *o)
{ (void)d;(void)a;(void)r;(void)o; return 0; }
static int rb_gpu_submit(uint32_t d,uint32_t (*r)(void *,uint32_t),void (*w)(void *,uint32_t,uint32_t),void *o)
{ (void)d;(void)r;(void)w;(void)o; return 0; }
#endif
