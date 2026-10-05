/**
 * DOA3 - re-emitted MMX decoder kernels (recomp bug #11: MMX was
 * dropped as comments). Originals renamed _oldmmx.
 * RE-GENERATE after any pipeline regen.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <stdio.h>
#include <intrin.h>
#include <math.h>

/**
 * sub_001866BD
 * Original: 0x001866BD - 0x00186890 (467 bytes, 141 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001866BD(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    uint64_t mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001866BD: ;
    ecx = 2;
    edi = MEM32(ebp + 8);

loc_001866C5: ;
    mm0 = MMQ_LOAD(edi); /* movq */
    mm2 = MMQ_LOAD(edi + 0x20); /* movq */
    mm4 = MMQ_LOAD(edi + 0x40); /* movq */
    mm6 = MMQ_LOAD(edi + 0x60); /* movq */
    mm1 = mm2; /* movq */
    mm2 = mmx_psubw(mm2, mm6); /* psubw */
    mm6 = mmx_paddw(mm6, mm1); /* paddw */
    mm3 = mm2; /* movq */
    mm3 = mmx_pmulhw(mm3, MMQ_LOAD(esi)); /* pmulhw */
    mm2 = mmx_pmullw(mm2, MMQ_LOAD(esi)); /* pmullw */
    mm5 = mm2; /* movq */
    mm2 = mmx_punpcklwd(mm2, mm3); /* punpcklwd */
    mm2 = mmx_psrad(mm2, 0xE); /* psrad */
    mm5 = mmx_punpckhwd(mm5, mm3); /* punpckhwd */
    mm5 = mmx_psrad(mm5, 0xE); /* psrad */
    mm2 = mmx_packssdw(mm2, mm5); /* packssdw */
    mm1 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm4); /* psubw */
    mm4 = mmx_paddw(mm4, mm1); /* paddw */
    mm2 = mmx_psubw(mm2, mm6); /* psubw */
    mm1 = mm4; /* movq */
    mm4 = mmx_psubw(mm4, mm6); /* psubw */
    mm6 = mmx_paddw(mm6, mm1); /* paddw */
    mm1 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm2); /* psubw */
    mm2 = mmx_paddw(mm2, mm1); /* paddw */
    mm1 = MMQ_LOAD(edi + 0x10); /* movq */
    mm3 = MMQ_LOAD(edi + 0x30); /* movq */
    mm5 = MMQ_LOAD(edi + 0x50); /* movq */
    mm7 = MMQ_LOAD(edi + 0x70); /* movq */
    MMQ_STORE(ebp + -28, mm0); /* movq */
    MMQ_STORE(ebp + -20, mm2); /* movq */
    MMQ_STORE(ebp + -12, mm4); /* movq */
    mm0 = mm5; /* movq */
    mm5 = mmx_psubw(mm5, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm0 = mm1; /* movq */
    mm1 = mmx_psubw(mm1, mm7); /* psubw */
    mm7 = mmx_paddw(mm7, mm0); /* paddw */
    mm0 = mm7; /* movq */
    mm7 = mmx_psubw(mm7, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm0 = mm5; /* movq */
    mm0 = mmx_psubw(mm0, mm1); /* psubw */
    mm2 = mm0; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 0x20)); /* pmulhw */
    mm0 = mmx_pmullw(mm0, MMQ_LOAD(esi + 0x20)); /* pmullw */
    mm4 = mm0; /* movq */
    mm0 = mmx_punpcklwd(mm0, mm2); /* punpcklwd */
    mm0 = mmx_psrad(mm0, 0xF); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xF); /* psrad */
    mm0 = mmx_packssdw(mm0, mm4); /* packssdw */
    mm2 = mm7; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi)); /* pmulhw */
    mm7 = mmx_pmullw(mm7, MMQ_LOAD(esi)); /* pmullw */
    mm4 = mm7; /* movq */
    mm7 = mmx_punpcklwd(mm7, mm2); /* punpcklwd */
    mm7 = mmx_psrad(mm7, 0xE); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xE); /* psrad */
    mm7 = mmx_packssdw(mm7, mm4); /* packssdw */
    mm2 = mm1; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 0x18)); /* pmulhw */
    mm1 = mmx_pmullw(mm1, MMQ_LOAD(esi + 0x18)); /* pmullw */
    mm4 = mm1; /* movq */
    mm1 = mmx_punpcklwd(mm1, mm2); /* punpcklwd */
    mm1 = mmx_psrad(mm1, 0xE); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xE); /* psrad */
    mm1 = mmx_packssdw(mm1, mm4); /* packssdw */
    mm1 = mmx_psubw(mm1, mm0); /* psubw */
    mm1 = mmx_psubw(mm1, mm3); /* psubw */
    mm7 = mmx_psubw(mm7, mm1); /* psubw */
    mm2 = mm5; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 8)); /* pmulhw */
    mm5 = mmx_pmullw(mm5, MMQ_LOAD(esi + 8)); /* pmullw */
    mm4 = mm5; /* movq */
    mm5 = mmx_punpcklwd(mm5, mm2); /* punpcklwd */
    mm5 = mmx_psrad(mm5, 0xE); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xE); /* psrad */
    mm5 = mmx_packssdw(mm5, mm4); /* packssdw */
    mm2 = mm7; /* movq */
    mm2 = mmx_psraw(mm2, 1); /* psraw */
    mm0 = mmx_psraw(mm0, 1); /* psraw */
    mm5 = mmx_psubw(mm5, mm2); /* psubw */
    mm5 = mmx_psubw(mm5, mm0); /* psubw */
    mm5 = mmx_psllw(mm5, 1); /* psllw */
    mm0 = mm6; /* movq */
    mm6 = mmx_psubw(mm6, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm2 = MMQ_LOAD(ebp + -20); /* movq */
    mm0 = mm2; /* movq */
    mm2 = mmx_psubw(mm2, mm1); /* psubw */
    mm1 = mmx_paddw(mm1, mm0); /* paddw */
    mm4 = MMQ_LOAD(ebp + -12); /* movq */
    mm0 = mm4; /* movq */
    mm4 = mmx_psubw(mm4, mm5); /* psubw */
    mm5 = mmx_paddw(mm5, mm0); /* paddw */
    mm3 = mmx_paddw(mm3, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm3 = mmx_psraw(mm3, 6); /* psraw */
    MMQ_STORE(edi, mm3); /* movq */
    mm0 = MMQ_LOAD(ebp + -28); /* movq */
    mm3 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm7); /* psubw */
    mm7 = mmx_paddw(mm7, mm3); /* paddw */
    mm1 = mmx_paddw(mm1, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm1 = mmx_psraw(mm1, 6); /* psraw */
    MMQ_STORE(edi + 0x10, mm1); /* movq */
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    MMQ_STORE(edi + 0x20, mm7); /* movq */
    mm5 = mmx_paddw(mm5, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm5 = mmx_psraw(mm5, 6); /* psraw */
    MMQ_STORE(edi + 0x30, mm5); /* movq */
    mm4 = mmx_paddw(mm4, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm4 = mmx_psraw(mm4, 6); /* psraw */
    MMQ_STORE(edi + 0x40, mm4); /* movq */
    mm0 = mmx_paddw(mm0, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm0 = mmx_psraw(mm0, 6); /* psraw */
    MMQ_STORE(edi + 0x50, mm0); /* movq */
    mm2 = mmx_paddw(mm2, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm2 = mmx_psraw(mm2, 6); /* psraw */
    MMQ_STORE(edi + 0x60, mm2); /* movq */
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + 0x28)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    MMQ_STORE(edi + 0x70, mm6); /* movq */
    ecx--;
    if (((int32_t)ecx <= 0)) goto loc_00186887; /* jle: less or equal (signed <=) */

loc_0018687F: ;
    edi = edi + 8;
    goto loc_001866C5;

loc_00186887: ;
    /* emms */
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    esp += 4; return; /* ret */

}

/**
 * sub_001779D0
 * Original: 0x001779D0 - 0x00177ED4 (1284 bytes, 356 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001779D0_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    uint64_t mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

loc_001779D0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x28;
    ecx = MEM32(ebp + 8);
    eax = MEM32(ebp + 0x10);
    MEM32(ebp + -40) = eax;
    eax = MEM32(ecx);
    MEM32(ebp + -16) = eax;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 0xC);
    ebx = MEM32(esi + 0xC);
    PUSH32(esp, edi);
    edi = MEM32(ecx + 0xC);
    edx = edi + eax;
    eax = MEM32(ecx + 4);
    MEM32(ebp + -4) = eax;
    eax = MEM32(esi);
    MEM32(ebp + 0xC) = eax;
    eax = eax + ebx;
    MEM32(ebp + 8) = eax;
    eax = MEM32(esi + 4);
    MEM32(ebp + -12) = edx;
    edx = MEM32(ecx + 8);
    MEM32(ebp + -8) = edx;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    eax = eax - edx;
    eax = (uint32_t)((int32_t)eax >> 1);
    MEM32(ebp + -36) = eax;
    eax = MEM32(esi + 8);
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    eax = eax - edx;
    eax = (uint32_t)((int32_t)eax >> 1);
    edx = edi + edi;
    MEM32(ebp + 0x10) = eax;
    eax = MEM32(ecx + 0x14);
    ecx = MEM32(ecx + 0x10);
    MEM32(ebp + -32) = edx;
    edx = ebx + ebx;
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -24) = ecx;
    MEM32(ebp + -20) = edx;
    edx = 0xFF;
    esi = MEM32(ebp + -40);
    ebx = MEM32(ebp + -36);

loc_00177A45: ;
    ecx = 0;

loc_00177A4A: ;
    edi = MEM32(ebp + -16);
    mm0 = MMQ_LOAD(edi + ecx * 2); /* movq */
    mm1 = MMQ_LOAD(edi + ecx * 2 + 8); /* movq */
    edi = MEM32(ebp + -12);
    mm2 = MMQ_LOAD(edi + ecx * 2); /* movq */
    mm3 = MMQ_LOAD(edi + ecx * 2 + 8); /* movq */
    edi = MEM32(ebp + -4);
    mm4 = MMQ_LOAD(edi + ecx); /* movq */
    edi = MEM32(ebp + -8);
    mm5 = MMQ_LOAD(edi + ecx); /* movq */
    edi = MEM32(ebp + 0xC);
    ebx = MEM32(ebp + 8);
    eax = (uint32_t)((mm4 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 4) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm2 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm2 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 4) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 8) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0xC) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm2 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 8) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm2 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0xC) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x10) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x14) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm2 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x10) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm2 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x14) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x18) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm0 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x1C) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm2 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x18) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm2 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x1C) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x20) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x24) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm3 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x20) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm3 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x24) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x28) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x2C) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm3 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x28) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm3 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x2C) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x30) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x34) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm3 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x30) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm3 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x34) = (uint32_t)mm7; /* movd */
    eax = (uint32_t)((mm4 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = MMQ_LOAD(esi + eax * 8 + 0x800); /* movq */
    eax = (uint32_t)((mm5 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8 + 0x1000)); /* paddw */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x38) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm1 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(edi + ecx * 8 + 0x3C) = (uint32_t)mm6; /* movd */
    mm6 = mm7; /* movq */
    eax = (uint32_t)((mm3 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax & edx;
    mm6 = mmx_paddw(mm6, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm6 = mmx_psraw(mm6, 6); /* psraw */
    mm6 = mmx_packuswb(mm6, mm6); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x38) = (uint32_t)mm6; /* movd */
    eax = (uint32_t)((mm3 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    eax = eax >> 8;
    mm7 = mmx_paddw(mm7, MMQ_LOAD(esi + eax * 8)); /* paddw */
    mm7 = mmx_psraw(mm7, 6); /* psraw */
    mm7 = mmx_packuswb(mm7, mm7); /* packuswb */
    MEM32(ebx + ecx * 8 + 0x3C) = (uint32_t)mm7; /* movd */
    edi = MEM32(ebp + -16);
    /* TODO: prefetcht0 byte ptr [edi + ecx*2 + 0x10] */
    ecx = ecx + 8;
    if (CMP_L(ecx, MEM32(ebp + -36))) goto loc_00177A4A; /* jl: less (signed <) */

loc_00177E88: ;
    eax = MEM32(ebp + -16);
    eax = eax + MEM32(ebp + -32);
    MEM32(ebp + -16) = eax;
    eax = MEM32(ebp + -12);
    eax = eax + MEM32(ebp + -32);
    MEM32(ebp + -12) = eax;
    eax = MEM32(ebp + -8);
    eax = eax + MEM32(ebp + -28);
    MEM32(ebp + -8) = eax;
    eax = MEM32(ebp + -4);
    eax = eax + MEM32(ebp + -24);
    MEM32(ebp + -4) = eax;
    eax = MEM32(ebp + 0xC);
    eax = eax + MEM32(ebp + -20);
    MEM32(ebp + 0xC) = eax;
    eax = MEM32(ebp + 8);
    eax = eax + MEM32(ebp + -20);
    MEM32(ebp + 8) = eax;
    eax = MEM32(ebp + 0x10);
    eax--;
    MEM32(ebp + 0x10) = eax;
    /* DOA3 fix (bug #16): lifter emitted the uninitialized _flags fallback for
     * `dec eax; ja` -- the CSC row loop ran once (2 rows/frame). dec leaves CF
     * clear here, so ja == (eax != 0): loop over all row pairs. */
    if (eax != 0) goto loc_00177A45;

loc_00177ECB: ;
    /* emms */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    esp += 4; return; /* ret */

}

/**
 * sub_00186440
 * Original: 0x00186440 - 0x001866BD (637 bytes, 184 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00186440(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    uint64_t mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

loc_00186440: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x1C;
    eax = MEM32(ebp + 0xC);
    eax = eax + 3;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    edx = edx & 3;
    eax = eax + edx;
    PUSH32(esp, esi);
    eax = (uint32_t)((int32_t)eax >> 2);
    PUSH32(esp, edi);
    MEM32(ebp + -4) = 0x3C0D18;
    MEM32(ebp + 0xC) = eax;
    esi = MEM32(ebp + -4);
    edi = MEM32(ebp + 8);
    ecx = MEM32(ebp + 0xC);

loc_0018646A: ;
    mm0 = MMQ_LOAD(edi); /* movq */
    mm2 = MMQ_LOAD(edi + 0x10); /* movq */
    mm4 = MMQ_LOAD(edi + 0x20); /* movq */
    mm6 = MMQ_LOAD(edi + 0x30); /* movq */
    mm1 = mm2; /* movq */
    mm2 = mmx_psubw(mm2, mm6); /* psubw */
    mm6 = mmx_paddw(mm6, mm1); /* paddw */
    mm3 = mm2; /* movq */
    mm3 = mmx_pmulhw(mm3, MMQ_LOAD(esi)); /* pmulhw */
    mm2 = mmx_pmullw(mm2, MMQ_LOAD(esi)); /* pmullw */
    mm5 = mm2; /* movq */
    mm2 = mmx_punpcklwd(mm2, mm3); /* punpcklwd */
    mm2 = mmx_psrad(mm2, 0xE); /* psrad */
    mm5 = mmx_punpckhwd(mm5, mm3); /* punpckhwd */
    mm5 = mmx_psrad(mm5, 0xE); /* psrad */
    mm2 = mmx_packssdw(mm2, mm5); /* packssdw */
    mm1 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm4); /* psubw */
    mm4 = mmx_paddw(mm4, mm1); /* paddw */
    mm2 = mmx_psubw(mm2, mm6); /* psubw */
    mm1 = mm4; /* movq */
    mm4 = mmx_psubw(mm4, mm6); /* psubw */
    mm6 = mmx_paddw(mm6, mm1); /* paddw */
    mm1 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm2); /* psubw */
    mm2 = mmx_paddw(mm2, mm1); /* paddw */
    mm1 = MMQ_LOAD(edi + 8); /* movq */
    mm3 = MMQ_LOAD(edi + 0x18); /* movq */
    mm5 = MMQ_LOAD(edi + 0x28); /* movq */
    mm7 = MMQ_LOAD(edi + 0x38); /* movq */
    MMQ_STORE(ebp + -28, mm0); /* movq */
    MMQ_STORE(ebp + -20, mm2); /* movq */
    MMQ_STORE(ebp + -12, mm4); /* movq */
    mm0 = mm5; /* movq */
    mm5 = mmx_psubw(mm5, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm0 = mm1; /* movq */
    mm1 = mmx_psubw(mm1, mm7); /* psubw */
    mm7 = mmx_paddw(mm7, mm0); /* paddw */
    mm0 = mm7; /* movq */
    mm7 = mmx_psubw(mm7, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm0 = mm5; /* movq */
    mm0 = mmx_psubw(mm0, mm1); /* psubw */
    mm2 = mm0; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 0x20)); /* pmulhw */
    mm0 = mmx_pmullw(mm0, MMQ_LOAD(esi + 0x20)); /* pmullw */
    mm4 = mm0; /* movq */
    mm0 = mmx_punpcklwd(mm0, mm2); /* punpcklwd */
    mm0 = mmx_psrad(mm0, 0xF); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xF); /* psrad */
    mm0 = mmx_packssdw(mm0, mm4); /* packssdw */
    mm2 = mm7; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi)); /* pmulhw */
    mm7 = mmx_pmullw(mm7, MMQ_LOAD(esi)); /* pmullw */
    mm4 = mm7; /* movq */
    mm7 = mmx_punpcklwd(mm7, mm2); /* punpcklwd */
    mm7 = mmx_psrad(mm7, 0xE); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xE); /* psrad */
    mm7 = mmx_packssdw(mm7, mm4); /* packssdw */
    mm2 = mm1; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 0x18)); /* pmulhw */
    mm1 = mmx_pmullw(mm1, MMQ_LOAD(esi + 0x18)); /* pmullw */
    mm4 = mm1; /* movq */
    mm1 = mmx_punpcklwd(mm1, mm2); /* punpcklwd */
    mm1 = mmx_psrad(mm1, 0xE); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xE); /* psrad */
    mm1 = mmx_packssdw(mm1, mm4); /* packssdw */
    mm1 = mmx_psubw(mm1, mm0); /* psubw */
    mm1 = mmx_psubw(mm1, mm3); /* psubw */
    mm7 = mmx_psubw(mm7, mm1); /* psubw */
    mm2 = mm5; /* movq */
    mm2 = mmx_pmulhw(mm2, MMQ_LOAD(esi + 8)); /* pmulhw */
    mm5 = mmx_pmullw(mm5, MMQ_LOAD(esi + 8)); /* pmullw */
    mm4 = mm5; /* movq */
    mm5 = mmx_punpcklwd(mm5, mm2); /* punpcklwd */
    mm5 = mmx_psrad(mm5, 0xD); /* psrad */
    mm4 = mmx_punpckhwd(mm4, mm2); /* punpckhwd */
    mm4 = mmx_psrad(mm4, 0xD); /* psrad */
    mm5 = mmx_packssdw(mm5, mm4); /* packssdw */
    mm5 = mmx_psubw(mm5, mm0); /* psubw */
    mm5 = mmx_psubw(mm5, mm7); /* psubw */
    mm0 = mm6; /* movq */
    mm6 = mmx_psubw(mm6, mm3); /* psubw */
    mm3 = mmx_paddw(mm3, mm0); /* paddw */
    mm2 = MMQ_LOAD(ebp + -20); /* movq */
    mm0 = mm2; /* movq */
    mm2 = mmx_psubw(mm2, mm1); /* psubw */
    mm1 = mmx_paddw(mm1, mm0); /* paddw */
    mm4 = MMQ_LOAD(ebp + -12); /* movq */
    mm0 = mm4; /* movq */
    mm4 = mmx_psubw(mm4, mm5); /* psubw */
    mm5 = mmx_paddw(mm5, mm0); /* paddw */
    eax = (uint32_t)((mm3 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi) = LO16(eax);
    eax = (uint32_t)((mm3 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x10) = LO16(eax);
    eax = (uint32_t)((mm3 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x20) = LO16(eax);
    eax = (uint32_t)((mm3 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x30) = LO16(eax);
    mm0 = MMQ_LOAD(ebp + -28); /* movq */
    mm3 = mm0; /* movq */
    mm0 = mmx_psubw(mm0, mm7); /* psubw */
    mm7 = mmx_paddw(mm7, mm3); /* paddw */
    eax = (uint32_t)((mm1 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 2) = LO16(eax);
    eax = (uint32_t)((mm1 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x12) = LO16(eax);
    eax = (uint32_t)((mm1 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x22) = LO16(eax);
    eax = (uint32_t)((mm1 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x32) = LO16(eax);
    eax = (uint32_t)((mm7 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 4) = LO16(eax);
    eax = (uint32_t)((mm7 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x14) = LO16(eax);
    eax = (uint32_t)((mm7 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x24) = LO16(eax);
    eax = (uint32_t)((mm7 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x34) = LO16(eax);
    eax = (uint32_t)((mm5 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 6) = LO16(eax);
    eax = (uint32_t)((mm5 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x16) = LO16(eax);
    eax = (uint32_t)((mm5 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x26) = LO16(eax);
    eax = (uint32_t)((mm5 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x36) = LO16(eax);
    eax = (uint32_t)((mm4 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 8) = LO16(eax);
    eax = (uint32_t)((mm4 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x18) = LO16(eax);
    eax = (uint32_t)((mm4 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x28) = LO16(eax);
    eax = (uint32_t)((mm4 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x38) = LO16(eax);
    eax = (uint32_t)((mm0 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0xA) = LO16(eax);
    eax = (uint32_t)((mm0 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x1A) = LO16(eax);
    eax = (uint32_t)((mm0 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x2A) = LO16(eax);
    eax = (uint32_t)((mm0 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x3A) = LO16(eax);
    eax = (uint32_t)((mm2 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0xC) = LO16(eax);
    eax = (uint32_t)((mm2 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x1C) = LO16(eax);
    eax = (uint32_t)((mm2 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x2C) = LO16(eax);
    eax = (uint32_t)((mm2 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x3C) = LO16(eax);
    eax = (uint32_t)((mm6 >> ((0 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0xE) = LO16(eax);
    eax = (uint32_t)((mm6 >> ((1 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x1E) = LO16(eax);
    eax = (uint32_t)((mm6 >> ((2 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x2E) = LO16(eax);
    eax = (uint32_t)((mm6 >> ((3 & 3) * 16)) & 0xFFFF); /* pextrw */
    MEM16(edi + 0x3E) = LO16(eax);
    ecx--;
    if (((int32_t)ecx <= 0)) { g_seh_ebp = ebp; sub_001866BD(); return; } /* jle: less or equal (signed <=) */

loc_001866B5: ;
    edi = edi + 0x40;
    goto loc_0018646A;

}

/**
 * sub_001B3940
 * Original: 0x001B3940 - 0x001B3BA8 (616 bytes, 196 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001B3940_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    uint64_t mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

loc_001B3940: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 8;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(0x1C3390);
    eax = MEM32(esi + 0x478);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(ebp + -8) = esi;
    PUSH32(esp, 0); sub_001B7700(); /* call 0x001B7700 */

loc_001B3960: ;
    eax = MEM32(esi + 0xC);
    PUSH32(esp, 0x209);
    eax = eax | 0x800;
    PUSH32(esp, esi);
    MEM32(esi + 0xC) = eax;
    PUSH32(esp, 0); sub_001B8DC0(); /* call 0x001B8DC0 */

loc_001B3976: ;
    /* DOA3 probe: arg2 ([ebp+0x10]) is the SOURCE vertex pointer this
     * function copies from. The sampling profiler puts ~72% of all CPU in
     * here and the fault counter passed 54M, every one a read from the
     * NV2A MMIO aperture (0xFDxxxxxx) -- i.e. the source is garbage and
     * every movntq loop iteration traps through the VEH. */
    ecx = MEM32(ebp + 8);
    edx = MEM32(ebp + 0xC);
    ebx = eax;
    /* DOA3 probe: eax is the pushbuffer write pointer from
     * sub_001B8DC0. Writes through it were landing inside the game
     * object array at 0x370C48, smashing its vtable pointers. */
    MEM32(ebx + 4) = ecx;
    ebx = ebx + 8;
    MEM32(ebx + -8) = 0x417FC;
    eax = ebx;
    eax = (uint32_t)(-(int32_t)eax);
    eax = eax >> 2;
    eax = eax & 7;
    ecx = eax + eax + 0x3E;
    /* cmp edx, ecx - flags set for next jcc */
    MEM32(ebp + 8) = ebx;
    if (CMP_B(edx, ecx)) goto loc_001B3B2E; /* jb: below (unsigned <) */

loc_001B39A4: ;
    if (TEST_Z(eax, eax)) goto loc_001B39E5; /* je: equal / zero */

loc_001B39A8: ;
    eax--;
    ecx = eax;
    ecx = ecx << 0x12;
    ecx = ecx + 0x40001800;
    MEM32(ebp + 0xC) = eax;
    MEM32(ebx) = ecx;
    esi = MEM32(ebp + 0x10);
    edi = MEM32(ebp + 8);
    edi = edi + 4;
    ecx = MEM32(ebp + 0xC);
    memcpy((void*)XBOX_PTR(edi), (void*)XBOX_PTR(esi), ecx * 4);
    esi += ecx * 4; edi += ecx * 4; ecx = 0; /* rep movsd */
    esi = MEM32(ebp + 0x10);
    ecx = eax * 4;
    esi = esi + ecx;
    eax = (uint32_t)(-(int32_t)eax);
    ebx = ebx + ecx + 4;
    edx = edx + eax * 2;
    MEM32(ebp + 8) = ebx;
    MEM32(ebp + 0x10) = esi;
    MEM32(ebp + 0xC) = edx;

loc_001B39E5: ;
    eax = 0x804021;
    { uint64_t _r = (uint64_t)eax * (uint64_t)edx;
      eax = (uint32_t)_r; edx = (uint32_t)(_r >> 32); }
    eax = MEM32(ebp + 0xC);
    ecx = eax;
    ecx = ecx - edx;
    ecx = ecx >> 1;
    ecx = ecx + edx;
    ecx = ecx >> 9;
    ecx = (uint32_t)((int32_t)ecx * (int32_t)0x3FE);
    edx = eax;
    edx = edx - ecx;
    if (CMP_B(edx, 0x3E)) goto loc_001B3A12; /* jb: below (unsigned <) */

loc_001B3A09: ;
    edx = edx >> 1;
    edx++;
    edx = edx & 0xFFFFFFF0u;
    edx--;
    goto loc_001B3A17;

loc_001B3A12: ;
    edx = 0x1FF;

loc_001B3A17: ;
    MEM32(ebp + -4) = edx;
    /* nop */

loc_001B3A20: ;
    eax = edx;
    eax = eax << 0x12;
    eax = eax + 0x40001800;
    MEM32(ebx) = eax;
    esi = MEM32(ebp + 0x10);
    edi = MEM32(ebp + 8);
    ecx = MEM32(ebp + -4);
    ecx = ecx - 0xF;
    ecx = ecx >> 4;
    /* TODO: prefetchnta byte ptr [esi + 0x3c] */
    /* TODO: prefetchnta byte ptr [esi + 0x5c] */
    eax = MEM32(esi);
    mm1 = MMQ_LOAD(esi + 4); /* movq */
    mm2 = MMQ_LOAD(esi + 0xC); /* movq */
    mm3 = MMQ_LOAD(esi + 0x14); /* movq */
    mm4 = MMQ_LOAD(esi + 0x1C); /* movq */
    mm5 = MMQ_LOAD(esi + 0x24); /* movq */
    mm6 = MMQ_LOAD(esi + 0x2C); /* movq */
    mm7 = MMQ_LOAD(esi + 0x34); /* movq */
    /* nop */
    /* nop */
    MEM32(edi + 4) = eax;
    MMQ_STORE(edi + 8, mm1); /* movntq */
    MMQ_STORE(edi + 0x10, mm2); /* movntq */
    MMQ_STORE(edi + 0x18, mm3); /* movntq */
    MMQ_STORE(edi + 0x20, mm4); /* movntq */
    MMQ_STORE(edi + 0x28, mm5); /* movntq */
    MMQ_STORE(edi + 0x30, mm6); /* movntq */
    MMQ_STORE(edi + 0x38, mm7); /* movntq */
    esi = esi + 0x3C;
    edi = edi + 0x40;
    goto loc_001B3A90;

    /* nop */

loc_001B3A90: ;
    /* TODO: prefetchnta byte ptr [esi + 0x40] */
    /* TODO: prefetchnta byte ptr [esi + 0x60] */
    mm0 = MMQ_LOAD(esi); /* movq */
    mm1 = MMQ_LOAD(esi + 8); /* movq */
    mm2 = MMQ_LOAD(esi + 0x10); /* movq */
    mm3 = MMQ_LOAD(esi + 0x18); /* movq */
    mm4 = MMQ_LOAD(esi + 0x20); /* movq */
    mm5 = MMQ_LOAD(esi + 0x28); /* movq */
    mm6 = MMQ_LOAD(esi + 0x30); /* movq */
    mm7 = MMQ_LOAD(esi + 0x38); /* movq */
    /* nop */
    /* nop */
    MMQ_STORE(edi, mm0); /* movntq */
    MMQ_STORE(edi + 8, mm1); /* movntq */
    MMQ_STORE(edi + 0x10, mm2); /* movntq */
    MMQ_STORE(edi + 0x18, mm3); /* movntq */
    MMQ_STORE(edi + 0x20, mm4); /* movntq */
    MMQ_STORE(edi + 0x28, mm5); /* movntq */
    MMQ_STORE(edi + 0x30, mm6); /* movntq */
    MMQ_STORE(edi + 0x38, mm7); /* movntq */
    esi = esi + 0x40;
    edi = edi + 0x40;
    ecx--;
    if ((ecx != 0)) goto loc_001B3A90; /* jne: not equal / not zero */

loc_001B3AE1: ;
    ecx = MEM32(ebp + 0xC);
    edi = MEM32(ebp + 0x10);
    eax = edx * 4;
    edx = (uint32_t)(-(int32_t)edx);
    edx = ecx + edx * 2;
    edi = edi + eax;
    ecx = eax + ebx + 4;
    eax = MEM32(ebp + -8);
    PUSH32(esp, 0x204);
    PUSH32(esp, eax);
    MEM32(ebp + 0x10) = edi;
    MEM32(ebp + 0xC) = edx;
    MEM32(eax) = ecx;
    PUSH32(esp, 0); sub_001B8DC0(); /* call 0x001B8DC0 */

loc_001B3B0F: ;
    /* DOA3 probe: second pushbuffer acquisition, inside the outer
     * copy loop. The first one (loc_001B3976) is always sane, so if
     * the destination is drifting into the game heap it starts here. */
    ebx = eax;
    eax = MEM32(ebp + 0xC);
    /* cmp eax, 0x3FE - flags set for next jcc */
    edx = 0x1FF;
    MEM32(ebp + 8) = ebx;
    MEM32(ebp + -4) = edx;
    if (CMP_AE(eax, 0x3FE)) goto loc_001B3A20; /* jae: above or equal (unsigned >=) */

loc_001B3B2A: ;
    /* emms */
    edx = eax;

loc_001B3B2E: ;
    eax = edx;
    eax = eax >> 1;
    ecx = eax;
    ecx = ecx << 0x12;
    ecx = ecx + 0x40001800;
    MEM32(ebp + 0xC) = eax;
    MEM32(ebx) = ecx;
    esi = MEM32(ebp + 0x10);
    edi = MEM32(ebp + 8);
    edi = edi + 4;
    ecx = MEM32(ebp + 0xC);
    memcpy((void*)XBOX_PTR(edi), (void*)XBOX_PTR(esi), ecx * 4);
    esi += ecx * 4; edi += ecx * 4; ecx = 0; /* rep movsd */
    ecx = MEM32(ebp + 0x10);
    eax = eax << 2;
    ecx = ecx + eax;
    /* test LO8(edx), 1 - flags set for next jcc */
    ebx = ebx + eax + 4;
    if (TEST_Z(LO8(edx), 1)) goto loc_001B3B70; /* je: equal / zero */

loc_001B3B61: ;
    eax = ZX16(MEM16(ecx));
    MEM32(ebx) = 0x41808;
    MEM32(ebx + 4) = eax;
    ebx = ebx + 8;

loc_001B3B70: ;
    esi = MEM32(ebp + -8);
    MEM32(ebx) = 0x417FC;
    MEM32(ebx + 4) = 0;
    eax = MEM32(esi + 0xC);
    ebx = ebx + 8;
    /* test HI8(eax), 0x10 - flags set for next jcc */
    MEM32(esi) = ebx;
    if (TEST_Z(HI8(eax), 0x10)) goto loc_001B3B94; /* je: equal / zero */

loc_001B3B8D: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_001B8970(); /* call 0x001B8970 */

loc_001B3B94: ;
    eax = MEM32(esi + 0xC);
    eax = eax & 0xFFFFE7FFu;
    POP32(esp, edi);
    MEM32(esi + 0xC) = eax;
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    esp += 16; return; /* ret 12 */

}
