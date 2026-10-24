#pragma once
#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#ifdef DOA3_SHADOW_APU
size_t rb_apu_size(void);
int rb_apu_load(void *ram,const void *snapshot,size_t bytes);
int rb_apu_save(void *snapshot,size_t bytes);
uintptr_t rb_apu_pointer(uint32_t address);
int rb_apu_exception(EXCEPTION_POINTERS *exception);
const char *rb_apu_error(void);
#else
static size_t rb_apu_size(void) { return 0; }
static int rb_apu_load(void *a,const void *b,size_t n) { (void)a;(void)b; return n==0; }
static int rb_apu_save(void *a,size_t n) { (void)a; return n==0; }
static uintptr_t rb_apu_pointer(uint32_t address) { (void)address; return 0; }
static int rb_apu_exception(EXCEPTION_POINTERS *e) { (void)e; return EXCEPTION_EXECUTE_HANDLER; }
static const char *rb_apu_error(void) { return NULL; }
#endif
