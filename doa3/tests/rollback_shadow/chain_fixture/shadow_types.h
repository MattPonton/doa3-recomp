#ifndef CHAIN_FIXTURE_TYPES_H
#define CHAIN_FIXTURE_TYPES_H
#include "shadow_runtime.h"
#include <string.h>
extern ptrdiff_t g_xbox_mem_offset;
extern uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
extern uint32_t g_str_a,g_str_b,g_flg_a,g_flg_b;
extern int g_flg_w,g_flg_test,g_fp_top;
extern uint16_t g_x87_cw;
extern double g_fp_stack[8];
#define MEM32(address) (*(uint32_t *)rb_ptr((address),4))
#endif
