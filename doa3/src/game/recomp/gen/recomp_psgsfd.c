/**
 * DOA3 - PSGSFD_I/_P/_B picture-type slice decoders (never emitted:
 * reached only via the data table at 0x21A2A4 tail-jump [obj+0x22C];
 * PSGSFD_I/_P had 0 detected functions; sub_001E4E80 was truncated).
 * RE-GENERATE after any pipeline regen. See NOTES.md "Regeneration Contract".
 */
#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <stdio.h>
#include <math.h>

/**
 * sub_001E4C40
 * Original: 0x001E4C40 - 0x001E4E30 (496 bytes, 177 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E4C40_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E4C40: ;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    ebp = MEM32(esp + 0x10);
    PUSH32(esp, esi);
    PUSH32(esp, edi);

loc_001E4C49: ;
    edi = MEM32(ebp);
    edx = MEM32(ebp + 8);
    ebx = MEM32(ebp + 4);
    eax = MEM32(ebp + 0xC);
    ecx = edi;
    ecx = ecx >> 9;
    /* cmp edx, 9 - flags set for next jcc */
    MEM32(esp + 0x18) = ebx;
    MEM32(esp + 0x10) = ecx;
    if (CMP_LE(edx, 9)) goto loc_001E4C78; /* jle: less or equal (signed <=) */

loc_001E4C67: ;
    ecx = 0x29;
    ecx = ecx - edx;
    esi = ebx;
    esi = esi >> LO8(ecx);
    ecx = MEM32(esp + 0x10);
    ecx = ecx | esi;

loc_001E4C78: ;
    if (TEST_Z(ecx, ecx)) goto loc_001E4DEB; /* je: equal / zero */

loc_001E4C80: ;
    ecx = MEM32(ebp + 0x29C);
    MEM32(esp + 0x10) = ecx;
    /* nop */

loc_001E4C90: ;
    esi = edi;
    esi = esi >> 0x14;
    if (CMP_LE(edx, 0x14)) goto loc_001E4CA9; /* jle: less or equal (signed <=) */

loc_001E4C9A: ;
    ecx = 0x34;
    ecx = ecx - edx;
    ebx = ebx >> LO8(ecx);
    esi = esi | ebx;
    ebx = MEM32(esp + 0x18);

loc_001E4CA9: ;
    if (TEST_NZ(esi, 0xFFFFFF00u)) goto loc_001E4CB9; /* jne: not equal / not zero */

loc_001E4CB1: ;
    ecx = MEM32(0xC0DF44);
    goto loc_001E4CC2;

loc_001E4CB9: ;
    ecx = MEM32(0xC0DC20);
    esi = esi >> 6;

loc_001E4CC2: ;
    esi = (uint32_t)(int32_t)SMEM16(ecx + esi * 2);
    ecx = esi;
    ecx = ecx & 0xF;
    edx = edx + ecx;
    if (CMP_L(edx, 0x20)) goto loc_001E4D05; /* jl: less (signed <) */

loc_001E4CD2: ;
    edx = edx - 0x20;
    edi = ebx;
    ebx = ZX8(MEM8(eax + 1));
    ecx = edx;
    edi = edi << LO8(ecx);
    ecx = (uint32_t)(int32_t)SMEM8(eax);
    eax++;
    ecx = ecx << 8;
    ecx = ecx | ebx;
    ebx = ZX8(MEM8(eax + 1));
    eax++;
    ecx = ecx << 8;
    ecx = ecx | ebx;
    ebx = ZX8(MEM8(eax + 1));
    eax++;
    ecx = ecx << 8;
    ecx = ecx | ebx;
    ebx = ecx;
    MEM32(esp + 0x18) = ebx;
    eax++;
    goto loc_001E4D07;

loc_001E4D05: ;
    edi = edi << LO8(ecx);

loc_001E4D07: ;
    ecx = esi;
    ecx = ecx >> 2;
    ecx = ZX8(LO8(ecx));
    ecx = ecx >> 2;
    if (CMP_EQ(ecx, 0x22)) goto loc_001E4C90; /* je: equal / zero */

loc_001E4D1B: ;
    if (CMP_NE(ecx, 0x23)) goto loc_001E4D2C; /* jne: not equal / not zero */

loc_001E4D20: ;
    MEM32(ebp + 0x29C) = MEM32(ebp + 0x29C) + 0x21;
    goto loc_001E4C90;

loc_001E4D2C: ;
    if (CMP_EQ(ecx, 0x24)) goto loc_001E4DEB; /* je: equal / zero */

loc_001E4D35: ;
    MEM32(ebp + 0x29C) = MEM32(ebp + 0x29C) + ecx;
    ecx = MEM32(ebp + 0x29C);
    esi = esi >> 0xA;
    MEM32(ebp + 0x2A4) = esi;
    if (CMP_G(ecx, MEM32(ebp + 0x2A0))) goto loc_001E4DEB; /* jg: greater (signed >) */

loc_001E4D56: ;
    ecx = ecx - MEM32(esp + 0x10);
    if (CMP_EQ(ecx, 0xFFFFFFFEu)) goto loc_001E4DEB; /* je: equal / zero */

loc_001E4D63: ;
    if (TEST_Z(MEM8(ebp + 0x2A4), 0x10)) goto loc_001E4DC9; /* je: equal / zero */

loc_001E4D6C: ;
    if (CMP_L(edx, 0x1B)) goto loc_001E4DB8; /* jl: less (signed <) */

loc_001E4D71: ;
    edx = edx - 0x1B;
    if ((edx == 0)) goto loc_001E4D8C; /* je: equal / zero */

loc_001E4D76: ;
    ecx = 5;
    ecx = ecx - edx;
    esi = ebx;
    esi = esi >> LO8(ecx);
    ecx = edx;
    esi = esi | edi;
    esi = esi >> 0x1B;
    ebx = ebx << LO8(ecx);
    goto loc_001E4D91;

loc_001E4D8C: ;
    esi = edi;
    esi = esi >> 0x1B;

loc_001E4D91: ;
    ecx = (uint32_t)(int32_t)SMEM8(eax);
    eax++;
    ecx = ecx << 8;
    edi = ebx;
    ebx = ZX8(MEM8(eax));
    ecx = ecx | ebx;
    ebx = ZX8(MEM8(eax + 1));
    eax++;
    ecx = ecx << 8;
    ecx = ecx | ebx;
    ebx = ZX8(MEM8(eax + 1));
    eax++;
    ecx = ecx << 8;
    ecx = ecx | ebx;
    ebx = ecx;
    eax++;
    goto loc_001E4DC3;

loc_001E4DB8: ;
    esi = edi;
    edx = edx + 5;
    esi = esi >> 0x1B;
    edi = edi << 5;

loc_001E4DC3: ;
    MEM32(ebp + 0x250) = esi;

loc_001E4DC9: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    MEM32(ebp) = edi;
    MEM32(ebp + 4) = ebx;
    MEM32(ebp + 8) = edx;
    MEM32(ebp + 0xC) = eax;
    { uint32_t _icall_t = MEM32(ebp + 0x234); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E4DDC: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    { uint32_t _icall_t = MEM32(ebp + 0x23C);
      PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E4DE3: ;
    esp = esp + 8;
    goto loc_001E4C49;

loc_001E4DEB: ;
    esi = MEM32(esp + 0x24);
    edi = MEM32(esi);
    edx = edx + 7;
    edx = (uint32_t)((int32_t)edx >> 3);
    ecx = edx + eax + -8;
    edx = MEM32(esp + 0x1C);
    ebx = MEM32(edx);
    eax = ecx;
    eax = eax - ebx;
    edi = edi + eax;
    MEM32(esi) = edi;
    edi = MEM32(esp + 0x20);
    ebp = MEM32(edi);
    PUSH32(esp, esi);
    ebp = ebp - eax;
    PUSH32(esp, edi);
    MEM32(edi) = ebp;
    PUSH32(esp, edx);
    MEM32(edx) = ecx;
    PUSH32(esp, 0); sub_001811C0(); /* call 0x001811C0 */

loc_001E4E1D: ;
    esp = esp + 0xC;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_001E5500
 * Original: 0x001E5500 - 0x001E5930 (1072 bytes, 345 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E5500_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E5500: ;
    esp = esp - 0xC;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x20);
    edx = MEM32(edi + 4);
    MEM32(esp + 0x14) = 1;
    MEM32(esp + 0x20) = edx;

loc_001E551A: ;
    ebx = MEM32(edi);
    ebp = MEM32(edi + 8);
    esi = MEM32(edi + 0xC);
    eax = ebx;
    eax = eax >> 9;
    /* cmp ebp, 9 - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 9)) goto loc_001E5543; /* jle: less or equal (signed <=) */

loc_001E5530: ;
    ecx = 0x29;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E5543: ;
    if (TEST_Z(eax, eax)) goto loc_001E58F1; /* je: equal / zero */

loc_001E554B: ;
    eax = MEM32(edi + 0x29C);
    MEM32(esp + 0x18) = eax;

loc_001E5555: ;
    eax = ebx;
    eax = eax >> 0x15;
    if (CMP_LE(ebp, 0x15)) goto loc_001E556E; /* jle: less or equal (signed <=) */

loc_001E555F: ;
    ecx = 0x35;
    ecx = ecx - ebp;
    edx = edx >> LO8(ecx);
    eax = eax | edx;
    edx = MEM32(esp + 0x20);

loc_001E556E: ;
    if (TEST_NZ(eax, 0xFFFFFF80u)) goto loc_001E5585; /* jne: not equal / not zero */

loc_001E5575: ;
    ecx = MEM32(0xC0DC28);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    MEM32(esp + 0x10) = eax;
    goto loc_001E5596;

loc_001E5585: ;
    ecx = MEM32(0xC0DC24);
    eax = eax >> 6;
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    MEM32(esp + 0x10) = eax;

loc_001E5596: ;
    ecx = eax;
    ecx = ecx & 0xF;
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E55D9; /* jl: less (signed <) */

loc_001E55A2: ;
    eax = (uint32_t)(int32_t)SMEM8(esi);
    ebp = ebp - 0x20;
    ebx = edx;
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    ecx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    esi++;
    edx = ZX8(MEM8(esi + 1));
    eax = eax | ecx;
    esi++;
    ecx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    eax = eax | edx;
    esi++;
    eax = eax << 8;
    eax = eax | ecx;
    MEM32(esp + 0x20) = eax;
    edx = eax;
    eax = MEM32(esp + 0x10);
    esi++;
    goto loc_001E55DB;

loc_001E55D9: ;
    ebx = ebx << LO8(ecx);

loc_001E55DB: ;
    ecx = eax;
    ecx = ecx >> 2;
    ecx = ZX8(LO8(ecx));
    ecx = ecx >> 2;
    if (CMP_EQ(ecx, 0x22)) goto loc_001E5555; /* je: equal / zero */

loc_001E55EF: ;
    if (CMP_NE(ecx, 0x23)) goto loc_001E5600; /* jne: not equal / not zero */

loc_001E55F4: ;
    MEM32(edi + 0x29C) = MEM32(edi + 0x29C) + 0x21;
    goto loc_001E5555;

loc_001E5600: ;
    if (CMP_EQ(ecx, 0x24)) goto loc_001E58F1; /* je: equal / zero */

loc_001E5609: ;
    MEM32(edi + 0x29C) = MEM32(edi + 0x29C) + ecx;
    ecx = MEM32(edi + 0x29C);
    eax = eax >> 0xA;
    MEM32(edi + 0x2A4) = eax;
    if (CMP_G(ecx, MEM32(edi + 0x2A0))) goto loc_001E58F1; /* jg: greater (signed >) */

loc_001E562A: ;
    ecx = ecx - MEM32(esp + 0x18);
    if (CMP_EQ(ecx, 0xFFFFFFFEu)) goto loc_001E58F1; /* je: equal / zero */

loc_001E5637: ;
    eax = MEM32(esp + 0x14);
    if (TEST_NZ(eax, eax)) goto loc_001E5684; /* jne: not equal / not zero */

loc_001E563F: ;
    if (CMP_BE(ecx, 1)) goto loc_001E5684; /* jbe: below or equal (unsigned <=) */

loc_001E5644: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(edi + 0x230); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E564C: ;
    edx = MEM32(esp + 0x28);
    esp = esp + 8;
    eax = 0; /* xor self */
    MEM32(edi + 0x264) = eax;
    MEM32(edi + 0x268) = eax;
    MEM32(edi + 0x26C) = eax;
    MEM32(edi + 0x270) = eax;
    eax = 0x400;
    MEM32(edi + 0x2AC) = eax;
    MEM32(edi + 0x2B4) = eax;
    MEM32(edi + 0x2B0) = eax;

loc_001E5684: ;
    if (TEST_NZ(MEM8(edi + 0x2A4), 0x20)) goto loc_001E5702; /* jne: not equal / not zero */

loc_001E568D: ;
    eax = ebx;
    eax = eax >> 0x1B;
    /* cmp ebp, 0x1B - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 0x1B)) goto loc_001E56AE; /* jle: less or equal (signed <=) */

loc_001E569B: ;
    ecx = 0x3B;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E56AE: ;
    ecx = MEM32(0xC0D840);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    ecx = eax;
    ecx = ecx >> 8;
    MEM32(edi + 0x2A4) = ecx;
    ecx = ZX8(LO8(eax));
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E5700; /* jl: less (signed <) */

loc_001E56CD: ;
    eax = (uint32_t)(int32_t)SMEM8(esi);
    ebp = ebp - 0x20;
    ebx = edx;
    edx = ZX8(MEM8(esi + 1));
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    esi++;
    ecx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    eax = eax | edx;
    esi++;
    edx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    eax = eax | ecx;
    esi++;
    eax = eax << 8;
    eax = eax | edx;
    MEM32(esp + 0x20) = eax;
    esi++;
    edx = eax;
    goto loc_001E5702;

loc_001E5700: ;
    ebx = ebx << LO8(ecx);

loc_001E5702: ;
    if (TEST_Z(MEM8(edi + 0x2A4), 0x10)) goto loc_001E576C; /* je: equal / zero */

loc_001E570B: ;
    if (CMP_L(ebp, 0x1B)) goto loc_001E575B; /* jl: less (signed <) */

loc_001E5710: ;
    ebp = ebp - 0x1B;
    if ((ebp == 0)) goto loc_001E572B; /* je: equal / zero */

loc_001E5715: ;
    ecx = 5;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = ebp;
    eax = eax | ebx;
    eax = eax >> 0x1B;
    edx = edx << LO8(ecx);
    goto loc_001E5730;

loc_001E572B: ;
    eax = ebx;
    eax = eax >> 0x1B;

loc_001E5730: ;
    ecx = (uint32_t)(int32_t)SMEM8(esi);
    esi++;
    ecx = ecx << 8;
    ebx = edx;
    edx = ZX8(MEM8(esi));
    ecx = ecx | edx;
    edx = ZX8(MEM8(esi + 1));
    esi++;
    ecx = ecx << 8;
    ecx = ecx | edx;
    edx = ZX8(MEM8(esi + 1));
    esi++;
    ecx = ecx << 8;
    ecx = ecx | edx;
    MEM32(esp + 0x20) = ecx;
    esi++;
    edx = ecx;
    goto loc_001E5766;

loc_001E575B: ;
    eax = ebx;
    ebp = ebp + 5;
    eax = eax >> 0x1B;
    ebx = ebx << 5;

loc_001E5766: ;
    MEM32(edi + 0x250) = eax;

loc_001E576C: ;
    if (TEST_Z(MEM8(edi + 0x2A4), 8)) goto loc_001E57D8; /* je: equal / zero */

loc_001E5775: ;
    eax = edi + 0x264;
    PUSH32(esp, eax);
    ecx = edi + 0x26C;
    MEM32(edi + 0xC) = esi;
    PUSH32(esp, ecx);
    esi = edi + 0x254;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    MEM32(edi) = ebx;
    MEM32(edi + 4) = edx;
    MEM32(edi + 8) = ebp;
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E579B: ;
    edx = edi + 0x268;
    PUSH32(esp, edx);
    MEM32(esp + 0x2C) = eax;
    eax = edi + 0x270;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E57B4: ;
    ecx = MEM32(edi + 4);
    ebx = MEM32(edi);
    ebp = MEM32(edi + 8);
    esi = MEM32(edi + 0xC);
    MEM32(esp + 0x40) = ecx;
    ecx = MEM32(esp + 0x38);
    esp = esp + 0x20;
    eax = eax | ecx;
    if ((eax != 0)) goto loc_001E58F1; /* jne: not equal / not zero */

loc_001E57D2: ;
    edx = MEM32(esp + 0x20);
    goto loc_001E57F2;

loc_001E57D8: ;
    eax = 0; /* xor self */
    MEM32(edi + 0x264) = eax;
    MEM32(edi + 0x268) = eax;
    MEM32(edi + 0x26C) = eax;
    MEM32(edi + 0x270) = eax;

loc_001E57F2: ;
    if (TEST_Z(MEM8(edi + 0x2A4), 2)) goto loc_001E5875; /* je: equal / zero */

loc_001E57FB: ;
    eax = ebx;
    eax = eax >> 0x17;
    /* cmp ebp, 0x17 - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 0x17)) goto loc_001E581C; /* jle: less or equal (signed <=) */

loc_001E5809: ;
    ecx = 0x37;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E581C: ;
    ecx = MEM32(0xC0DE64);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    ecx = eax;
    ecx = ecx & 0xFFFFFFF0u;
    ecx = ecx << 0x10;
    MEM32(edi + 0x2A8) = ecx;
    ecx = ZX8(LO8(eax));
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E5871; /* jl: less (signed <) */

loc_001E583E: ;
    eax = (uint32_t)(int32_t)SMEM8(esi);
    ebp = ebp - 0x20;
    ebx = edx;
    edx = ZX8(MEM8(esi + 1));
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    esi++;
    ecx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    eax = eax | edx;
    esi++;
    edx = ZX8(MEM8(esi + 1));
    eax = eax << 8;
    eax = eax | ecx;
    esi++;
    eax = eax << 8;
    eax = eax | edx;
    MEM32(esp + 0x20) = eax;
    esi++;
    edx = eax;
    goto loc_001E587F;

loc_001E5871: ;
    ebx = ebx << LO8(ecx);
    goto loc_001E587F;

loc_001E5875: ;
    MEM32(edi + 0x2A8) = 0;

loc_001E587F: ;
    /* test MEM8(edi + 0x2A4), 1 - flags set for next jcc */
    MEM32(edi) = ebx;
    MEM32(edi + 4) = edx;
    MEM32(edi + 8) = ebp;
    MEM32(edi + 0xC) = esi;
    if (TEST_Z(MEM8(edi + 0x2A4), 1)) goto loc_001E58A6; /* je: equal / zero */

loc_001E5893: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(edi + 0x234); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E589A: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(edi + 0x23C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E58A1: ;
    esp = esp + 8;
    goto loc_001E58DB;

loc_001E58A6: ;
    eax = MEM32(edi + 0x2A8);
    if (TEST_Z(eax, eax)) goto loc_001E58BA; /* je: equal / zero */

loc_001E58B0: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(edi + 0x238); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E58B7: ;
    esp = esp + 4;

loc_001E58BA: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(edi + 0x248); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E58C1: ;
    eax = 0x400;
    esp = esp + 4;
    MEM32(edi + 0x2AC) = eax;
    MEM32(edi + 0x2B4) = eax;
    MEM32(edi + 0x2B0) = eax;

loc_001E58DB: ;
    eax = MEM32(edi + 4);
    MEM32(esp + 0x20) = eax;
    MEM32(esp + 0x14) = 0;
    edx = eax;
    goto loc_001E551A;

loc_001E58F1: ;
    edx = MEM32(esp + 0x24);
    ebx = MEM32(edx);
    ebp = ebp + 7;
    ebp = (uint32_t)((int32_t)ebp >> 3);
    ecx = esi + ebp + -8;
    esi = MEM32(esp + 0x2C);
    edi = MEM32(esi);
    eax = ecx;
    eax = eax - ebx;
    edi = edi + eax;
    MEM32(esi) = edi;
    edi = MEM32(esp + 0x28);
    ebp = MEM32(edi);
    PUSH32(esp, esi);
    ebp = ebp - eax;
    PUSH32(esp, edi);
    MEM32(edi) = ebp;
    PUSH32(esp, edx);
    MEM32(edx) = ecx;
    PUSH32(esp, 0); sub_001811C0(); /* call 0x001811C0 */

loc_001E5923: ;
    esp = esp + 0xC;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp = esp + 0xC;
    esp += 4; return; /* ret */

}

/**
 * sub_001E4E80
 * Original: 0x001E4E80 - 0x001E5033 (435 bytes, 159 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E4E80_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E4E80: ;
    esp = esp - 8;
    eax = MEM32(esp + 0xC);
    edx = MEM32(eax + 0xC);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    ebp = MEM32(eax + 4);
    PUSH32(esp, esi);
    esi = MEM32(eax + 8);
    PUSH32(esp, edi);
    edi = MEM32(eax);
    ebx = edi;
    ebx = ebx >> 0x15;
    /* cmp esi, 0x15 - flags set for next jcc */
    MEM32(esp + 0x14) = 0;
    if (CMP_LE(esi, 0x15)) goto loc_001E4EB9; /* jle: less or equal (signed <=) */

loc_001E4EA8: ;
    ecx = 0x35;
    ecx = ecx - esi;
    eax = ebp;
    eax = eax >> LO8(ecx);
    ebx = ebx | eax;
    eax = MEM32(esp + 0x1C);

loc_001E4EB9: ;
    if (TEST_NZ(ebx, 0xFFFFFF80u)) goto loc_001E4EC9; /* jne: not equal / not zero */

loc_001E4EC1: ;
    ecx = MEM32(0xC0DF48);
    goto loc_001E4ED2;

loc_001E4EC9: ;
    ecx = MEM32(0xC0D42C);
    ebx = ebx >> 6;

loc_001E4ED2: ;
    ecx = (uint32_t)(int32_t)SMEM16(ecx + ebx * 2);
    ebx = SX8(LO8(ecx));
    /* cmp ebx, 0x7F - flags set for next jcc */
    MEM32(esp + 0x1C) = ebx;
    if (CMP_NE(ebx, 0x7F)) goto loc_001E4EEF; /* jne: not equal / not zero */

loc_001E4EE2: ;
    MEM32(esp + 0x14) = 0xFFFFFFFFu;
    goto loc_001E501C;

loc_001E4EEF: ;
    ecx = ecx >> 8;
    ecx = ZX8(LO8(ecx));
    esi = esi + ecx;
    if (CMP_L(esi, 0x20)) goto loc_001E4F2B; /* jl: less (signed <) */

loc_001E4EFC: ;
    esi = esi - 0x20;
    edi = ebp;
    ebp = ZX8(MEM8(edx + 1));
    ecx = esi;
    edi = edi << LO8(ecx);
    ecx = (uint32_t)(int32_t)SMEM8(edx);
    edx++;
    ecx = ecx << 8;
    ecx = ecx | ebp;
    ebp = ZX8(MEM8(edx + 1));
    edx++;
    ecx = ecx << 8;
    ecx = ecx | ebp;
    ebp = ZX8(MEM8(edx + 1));
    edx++;
    ecx = ecx << 8;
    ecx = ecx | ebp;
    ebp = ecx;
    edx++;
    goto loc_001E4F2D;

loc_001E4F2B: ;
    edi = edi << LO8(ecx);

loc_001E4F2D: ;
    if (TEST_NZ(ebx, ebx)) goto loc_001E4F42; /* jne: not equal / not zero */

loc_001E4F31: ;
    ecx = MEM32(esp + 0x28);
    ebx = MEM32(ecx);
    ecx = MEM32(esp + 0x24);
    MEM32(ecx) = ebx;
    goto loc_001E5011;

loc_001E4F42: ;
    ecx = MEM32(esp + 0x20);
    ebx = MEM32(ecx + 4);
    if (TEST_Z(ebx, ebx)) goto loc_001E4FE5; /* je: equal / zero */

loc_001E4F51: ;
    ecx = 0x20;
    ecx = ecx - ebx;
    /* cmp esi, ecx - flags set for next jcc */
    MEM32(esp + 0x10) = ecx;
    if (CMP_L(esi, ecx)) goto loc_001E4FA9; /* jl: less (signed <) */

loc_001E4F60: ;
    esi = esi + ebx + -32;
    if (TEST_Z(esi, esi)) goto loc_001E4F7E; /* je: equal / zero */

loc_001E4F68: ;
    ecx = ebx;
    ecx = ecx - esi;
    ebx = ebp;
    ebx = ebx >> LO8(ecx);
    ecx = MEM32(esp + 0x10);
    ebx = ebx | edi;
    ebx = ebx >> LO8(ecx);
    ecx = esi;
    ebp = ebp << LO8(ecx);
    goto loc_001E4F82;

loc_001E4F7E: ;
    ebx = edi;
    ebx = ebx >> LO8(ecx);

loc_001E4F82: ;
    ecx = (uint32_t)(int32_t)SMEM8(edx);
    edx++;
    ecx = ecx << 8;
    edi = ebp;
    ebp = ZX8(MEM8(edx));
    ecx = ecx | ebp;
    ebp = ZX8(MEM8(edx + 1));
    edx++;
    ecx = ecx << 8;
    ecx = ecx | ebp;
    ebp = ZX8(MEM8(edx + 1));
    edx++;
    ecx = ecx << 8;
    ecx = ecx | ebp;
    ebp = ecx;
    edx++;
    goto loc_001E4FB8;

loc_001E4FA9: ;
    esi = esi + ebx;
    ebx = edi;
    ebx = ebx >> LO8(ecx);
    ecx = MEM32(esp + 0x20);
    ecx = MEM32(ecx + 4);
    edi = edi << LO8(ecx);

loc_001E4FB8: ;
    ecx = MEM32(esp + 0x20);
    ecx = MEM32(ecx + 0xC);
    ecx = ecx - ebx;
    ebx = MEM32(esp + 0x1C);
    ecx--;
    MEM32(esp + 0x10) = ecx;
    ecx = MEM32(esp + 0x20);
    ecx = MEM32(ecx + 4);
    ebx = ebx << LO8(ecx);
    ecx = MEM32(esp + 0x10);
    if (CMP_LE(ebx & ebx, 0)) goto loc_001E4FDF; /* jle: less or equal (signed <=) */

loc_001E4FDB: ;
    ebx = ebx - ecx;
    goto loc_001E4FE1;

loc_001E4FDF: ;
    ebx = ebx + ecx;

loc_001E4FE1: ;
    MEM32(esp + 0x1C) = ebx;

loc_001E4FE5: ;
    ecx = MEM32(esp + 0x28);
    ebx = MEM32(ecx);
    ebx = ebx + MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x20);
    ecx = MEM32(ecx + 8);
    ebx = ebx << LO8(ecx);
    ecx = MEM32(esp + 0x20);
    ecx = MEM32(ecx + 8);
    ebx = (uint32_t)((int32_t)ebx >> LO8(ecx));
    ecx = MEM32(esp + 0x24);
    MEM32(ecx) = ebx;
    ecx = MEM32(esp + 0x28);
    MEM32(ecx) = ebx;
    ecx = MEM32(esp + 0x24);

loc_001E5011: ;
    ebx = MEM32(esp + 0x20);
    if (CMP_EQ(MEM32(ebx), 0)) goto loc_001E501C; /* je: equal / zero */

loc_001E501A: ;
    MEM32(ecx) = MEM32(ecx) << 1;

loc_001E501C: ;
    MEM32(eax) = edi;
    POP32(esp, edi);
    MEM32(eax + 8) = esi;
    POP32(esp, esi);
    MEM32(eax + 4) = ebp;
    POP32(esp, ebp);
    MEM32(eax + 0xC) = edx;
    eax = MEM32(esp + 8);
    POP32(esp, ebx);
    esp = esp + 8;
    esp += 4; return; /* ret */

}


/**
 * sub_001E5040
 * Original: 0x001E5040 - 0x001E5500 (1216 bytes, 392 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E5040_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E5040: ;
    esp = esp - 0xC;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x1C);
    edx = MEM32(esi + 4);
    PUSH32(esp, edi);
    MEM32(esp + 0x14) = 1;
    MEM32(esp + 0x20) = edx;

loc_001E505A: ;
    ebx = MEM32(esi);
    ebp = MEM32(esi + 8);
    edi = MEM32(esi + 0xC);
    eax = ebx;
    eax = eax >> 9;
    /* cmp ebp, 9 - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 9)) goto loc_001E5083; /* jle: less or equal (signed <=) */

loc_001E5070: ;
    ecx = 0x29;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E5083: ;
    if (TEST_Z(eax, eax)) goto loc_001E54BA; /* je: equal / zero */

loc_001E508B: ;
    eax = MEM32(esi + 0x29C);
    MEM32(esp + 0x18) = eax;

loc_001E5095: ;
    eax = ebx;
    eax = eax >> 0x15;
    if (CMP_LE(ebp, 0x15)) goto loc_001E50AE; /* jle: less or equal (signed <=) */

loc_001E509F: ;
    ecx = 0x35;
    ecx = ecx - ebp;
    edx = edx >> LO8(ecx);
    eax = eax | edx;
    edx = MEM32(esp + 0x20);

loc_001E50AE: ;
    if (TEST_NZ(eax, 0xFFFFFF80u)) goto loc_001E50C5; /* jne: not equal / not zero */

loc_001E50B5: ;
    ecx = MEM32(0xC0DA60);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    MEM32(esp + 0x10) = eax;
    goto loc_001E50D6;

loc_001E50C5: ;
    ecx = MEM32(0xC0D844);
    eax = eax >> 6;
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    MEM32(esp + 0x10) = eax;

loc_001E50D6: ;
    ecx = eax;
    ecx = ecx & 0xF;
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E5119; /* jl: less (signed <) */

loc_001E50E2: ;
    eax = (uint32_t)(int32_t)SMEM8(edi);
    ebp = ebp - 0x20;
    ebx = edx;
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    ecx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    edi++;
    edx = ZX8(MEM8(edi + 1));
    eax = eax | ecx;
    edi++;
    ecx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    eax = eax | edx;
    edi++;
    eax = eax << 8;
    eax = eax | ecx;
    MEM32(esp + 0x20) = eax;
    edx = eax;
    eax = MEM32(esp + 0x10);
    edi++;
    goto loc_001E511B;

loc_001E5119: ;
    ebx = ebx << LO8(ecx);

loc_001E511B: ;
    ecx = eax;
    ecx = ecx >> 2;
    ecx = ZX8(LO8(ecx));
    ecx = ecx >> 2;
    if (CMP_EQ(ecx, 0x22)) goto loc_001E5095; /* je: equal / zero */

loc_001E512F: ;
    if (CMP_NE(ecx, 0x23)) goto loc_001E5140; /* jne: not equal / not zero */

loc_001E5134: ;
    MEM32(esi + 0x29C) = MEM32(esi + 0x29C) + 0x21;
    goto loc_001E5095;

loc_001E5140: ;
    if (CMP_EQ(ecx, 0x24)) goto loc_001E54BA; /* je: equal / zero */

loc_001E5149: ;
    MEM32(esi + 0x29C) = MEM32(esi + 0x29C) + ecx;
    ecx = MEM32(esi + 0x29C);
    eax = eax >> 0xA;
    MEM32(esi + 0x2A4) = eax;
    if (CMP_G(ecx, MEM32(esi + 0x2A0))) goto loc_001E54BA; /* jg: greater (signed >) */

loc_001E516A: ;
    ecx = ecx - MEM32(esp + 0x18);
    if (CMP_EQ(ecx, 0xFFFFFFFEu)) goto loc_001E54BA; /* je: equal / zero */

loc_001E5177: ;
    eax = MEM32(esp + 0x14);
    if (TEST_NZ(eax, eax)) goto loc_001E51AA; /* jne: not equal / not zero */

loc_001E517F: ;
    if (CMP_BE(ecx, 1)) goto loc_001E51AA; /* jbe: below or equal (unsigned <=) */

loc_001E5184: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    { uint32_t _icall_t = MEM32(esi + 0x230); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E518C: ;
    edx = MEM32(esp + 0x28);
    eax = 0x400;
    esp = esp + 8;
    MEM32(esi + 0x2AC) = eax;
    MEM32(esi + 0x2B4) = eax;
    MEM32(esi + 0x2B0) = eax;

loc_001E51AA: ;
    if (TEST_NZ(MEM8(esi + 0x2A4), 0x20)) goto loc_001E5228; /* jne: not equal / not zero */

loc_001E51B3: ;
    eax = ebx;
    eax = eax >> 0x1A;
    /* cmp ebp, 0x1A - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 0x1A)) goto loc_001E51D4; /* jle: less or equal (signed <=) */

loc_001E51C1: ;
    ecx = 0x3A;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E51D4: ;
    ecx = MEM32(0xC0DE00);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    ecx = eax;
    ecx = ecx >> 8;
    MEM32(esi + 0x2A4) = ecx;
    ecx = ZX8(LO8(eax));
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E5226; /* jl: less (signed <) */

loc_001E51F3: ;
    eax = (uint32_t)(int32_t)SMEM8(edi);
    ebp = ebp - 0x20;
    ebx = edx;
    edx = ZX8(MEM8(edi + 1));
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    edi++;
    ecx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    eax = eax | edx;
    edi++;
    edx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    eax = eax | ecx;
    edi++;
    eax = eax << 8;
    eax = eax | edx;
    MEM32(esp + 0x20) = eax;
    edi++;
    edx = eax;
    goto loc_001E5228;

loc_001E5226: ;
    ebx = ebx << LO8(ecx);

loc_001E5228: ;
    if (TEST_Z(MEM8(esi + 0x2A4), 0x10)) goto loc_001E5292; /* je: equal / zero */

loc_001E5231: ;
    if (CMP_L(ebp, 0x1B)) goto loc_001E5281; /* jl: less (signed <) */

loc_001E5236: ;
    ebp = ebp - 0x1B;
    if ((ebp == 0)) goto loc_001E5251; /* je: equal / zero */

loc_001E523B: ;
    ecx = 5;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = ebp;
    eax = eax | ebx;
    eax = eax >> 0x1B;
    edx = edx << LO8(ecx);
    goto loc_001E5256;

loc_001E5251: ;
    eax = ebx;
    eax = eax >> 0x1B;

loc_001E5256: ;
    ecx = (uint32_t)(int32_t)SMEM8(edi);
    edi++;
    ecx = ecx << 8;
    ebx = edx;
    edx = ZX8(MEM8(edi));
    ecx = ecx | edx;
    edx = ZX8(MEM8(edi + 1));
    edi++;
    ecx = ecx << 8;
    ecx = ecx | edx;
    edx = ZX8(MEM8(edi + 1));
    edi++;
    ecx = ecx << 8;
    ecx = ecx | edx;
    MEM32(esp + 0x20) = ecx;
    edi++;
    edx = ecx;
    goto loc_001E528C;

loc_001E5281: ;
    eax = ebx;
    ebp = ebp + 5;
    eax = eax >> 0x1B;
    ebx = ebx << 5;

loc_001E528C: ;
    MEM32(esi + 0x250) = eax;

loc_001E5292: ;
    if (TEST_Z(MEM8(esi + 0x2A4), 8)) goto loc_001E52FC; /* je: equal / zero */

loc_001E529B: ;
    eax = esi + 0x264;
    PUSH32(esp, eax);
    ecx = esi + 0x26C;
    MEM32(esi + 0xC) = edi;
    PUSH32(esp, ecx);
    edi = esi + 0x254;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    MEM32(esi) = ebx;
    MEM32(esi + 4) = edx;
    MEM32(esi + 8) = ebp;
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E52C1: ;
    edx = esi + 0x268;
    PUSH32(esp, edx);
    MEM32(esp + 0x2C) = eax;
    eax = esi + 0x270;
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E52DA: ;
    ecx = MEM32(esi + 4);
    ebx = MEM32(esi);
    ebp = MEM32(esi + 8);
    edi = MEM32(esi + 0xC);
    MEM32(esp + 0x40) = ecx;
    ecx = MEM32(esp + 0x38);
    esp = esp + 0x20;
    eax = eax | ecx;
    if ((eax != 0)) goto loc_001E54BA; /* jne: not equal / not zero */

loc_001E52F8: ;
    edx = MEM32(esp + 0x20);

loc_001E52FC: ;
    if (TEST_Z(MEM8(esi + 0x2A4), 4)) goto loc_001E5366; /* je: equal / zero */

loc_001E5305: ;
    MEM32(esi + 4) = edx;
    edx = esi + 0x288;
    PUSH32(esp, edx);
    eax = esi + 0x290;
    MEM32(esi + 0xC) = edi;
    PUSH32(esp, eax);
    edi = esi + 0x278;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    MEM32(esi) = ebx;
    MEM32(esi + 8) = ebp;
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E532B: ;
    ecx = esi + 0x28C;
    PUSH32(esp, ecx);
    edx = esi + 0x294;
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    MEM32(esp + 0x38) = eax;
    PUSH32(esp, 0); sub_001E4E80(); /* call 0x001E4E80 */

loc_001E5344: ;
    ecx = MEM32(esi + 4);
    ebx = MEM32(esi);
    ebp = MEM32(esi + 8);
    edi = MEM32(esi + 0xC);
    MEM32(esp + 0x40) = ecx;
    ecx = MEM32(esp + 0x38);
    esp = esp + 0x20;
    eax = eax | ecx;
    if ((eax != 0)) goto loc_001E54BA; /* jne: not equal / not zero */

loc_001E5362: ;
    edx = MEM32(esp + 0x20);

loc_001E5366: ;
    if (TEST_Z(MEM8(esi + 0x2A4), 2)) goto loc_001E53E9; /* je: equal / zero */

loc_001E536F: ;
    eax = ebx;
    eax = eax >> 0x17;
    /* cmp ebp, 0x17 - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_LE(ebp, 0x17)) goto loc_001E5390; /* jle: less or equal (signed <=) */

loc_001E537D: ;
    ecx = 0x37;
    ecx = ecx - ebp;
    eax = edx;
    eax = eax >> LO8(ecx);
    ecx = eax;
    eax = MEM32(esp + 0x10);
    eax = eax | ecx;

loc_001E5390: ;
    ecx = MEM32(0xC0DE64);
    eax = (uint32_t)(int32_t)SMEM16(ecx + eax * 2);
    ecx = eax;
    ecx = ecx & 0xFFFFFFF0u;
    ecx = ecx << 0x10;
    MEM32(esi + 0x2A8) = ecx;
    ecx = ZX8(LO8(eax));
    ebp = ebp + ecx;
    if (CMP_L(ebp, 0x20)) goto loc_001E53E5; /* jl: less (signed <) */

loc_001E53B2: ;
    eax = (uint32_t)(int32_t)SMEM8(edi);
    ebp = ebp - 0x20;
    ebx = edx;
    edx = ZX8(MEM8(edi + 1));
    ecx = ebp;
    ebx = ebx << LO8(ecx);
    edi++;
    ecx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    eax = eax | edx;
    edi++;
    edx = ZX8(MEM8(edi + 1));
    eax = eax << 8;
    eax = eax | ecx;
    edi++;
    eax = eax << 8;
    eax = eax | edx;
    MEM32(esp + 0x20) = eax;
    edi++;
    edx = eax;
    goto loc_001E53F3;

loc_001E53E5: ;
    ebx = ebx << LO8(ecx);
    goto loc_001E53F3;

loc_001E53E9: ;
    MEM32(esi + 0x2A8) = 0;

loc_001E53F3: ;
    /* test MEM8(esi + 0x2A4), 1 - flags set for next jcc */
    MEM32(esi) = ebx;
    MEM32(esi + 4) = edx;
    MEM32(esi + 8) = ebp;
    MEM32(esi + 0xC) = edi;
    if (TEST_Z(MEM8(esi + 0x2A4), 1)) goto loc_001E545C; /* je: equal / zero */

loc_001E5407: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_t = MEM32(esi + 0x234); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E540E: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_t = MEM32(esi + 0x23C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5415: ;
    esp = esp + 8;
    eax = 0; /* xor self */
    MEM32(esi + 0x264) = eax;
    MEM32(esi + 0x268) = eax;
    MEM32(esi + 0x26C) = eax;
    MEM32(esi + 0x270) = eax;
    MEM32(esi + 0x288) = eax;
    MEM32(esi + 0x28C) = eax;
    MEM32(esi + 0x290) = eax;
    MEM32(esi + 0x294) = eax;
    ecx = MEM32(esi + 4);
    MEM32(esp + 0x20) = ecx;
    MEM32(esp + 0x14) = eax;
    edx = ecx;
    goto loc_001E505A;

loc_001E545C: ;
    eax = MEM32(esi + 0x2A8);
    if (TEST_Z(eax, eax)) goto loc_001E5470; /* je: equal / zero */

loc_001E5466: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_t = MEM32(esi + 0x238); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E546D: ;
    esp = esp + 4;

loc_001E5470: ;
    eax = MEM32(esi + 0x2A4);
    eax = (uint32_t)((int32_t)eax >> 2);
    eax = eax & 3;
    eax = MEM32(esi + eax * 4 + 0x240);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    MEM32(esi + 0x240) = eax;
    { uint32_t _icall_t = eax; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E548C: ;
    ecx = MEM32(esi + 4);
    eax = 0x400;
    esp = esp + 4;
    MEM32(esi + 0x2AC) = eax;
    MEM32(esi + 0x2B4) = eax;
    MEM32(esi + 0x2B0) = eax;
    eax = 0; /* xor self */
    MEM32(esp + 0x20) = ecx;
    MEM32(esp + 0x14) = eax;
    edx = ecx;
    goto loc_001E505A;

loc_001E54BA: ;
    edx = MEM32(esp + 0x24);
    ebx = MEM32(edx);
    esi = MEM32(esp + 0x2C);
    ebp = ebp + 7;
    ebp = (uint32_t)((int32_t)ebp >> 3);
    ecx = edi + ebp + -8;
    edi = MEM32(esi);
    eax = ecx;
    eax = eax - ebx;
    edi = edi + eax;
    MEM32(esi) = edi;
    edi = MEM32(esp + 0x28);
    ebp = MEM32(edi);
    PUSH32(esp, esi);
    ebp = ebp - eax;
    PUSH32(esp, edi);
    MEM32(edi) = ebp;
    PUSH32(esp, edx);
    MEM32(edx) = ecx;
    PUSH32(esp, 0); sub_001811C0(); /* call 0x001811C0 */

loc_001E54EC: ;
    esp = esp + 0xC;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp = esp + 0xC;
    esp += 4; return; /* ret */

}

/**
 * sub_001E3010
 * Original: 0x001E3010 - 0x001E3060 (80 bytes, 33 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3010_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E3010: ;
    edx = MEM32(esp + 8);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x14);
    ebx = MEM32(edi + 0x29C);
    ebp = MEM32(edi + 0x240);
    esi = ebx;
    esi = esi - edx;
    esi++;
    /* cmp esi, ebx - flags set for next jcc */
    MEM32(edi + 0x2A8) = 0;
    if (CMP_GE(esi, ebx)) goto loc_001E3051; /* jge: greater or equal (signed >=) */

loc_001E303B: ;
    goto loc_001E3040;

    /* nop */

loc_001E3040: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    MEM32(edi + 0x29C) = esi;
    { uint32_t _icall_t = ebp; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E3049: ;
    esp = esp + 4;
    esi++;
    if (CMP_L(esi, ebx)) goto loc_001E3040; /* jl: less (signed <) */

loc_001E3051: ;
    MEM32(edi + 0x29C) = ebx;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}


/**
 * sub_001E3060
 * Original: 0x001E3060 - 0x001E30F0 (144 bytes, 47 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3060_gen(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E3060: ;
    eax = MEM32(esp + 4);
    ecx = MEM32(eax + 0x29C);
    ecx = eax + ecx * 2 + 0xC68;
    PUSH32(esp, ebx);
    ebx = (uint32_t)(int32_t)SMEM16(eax + 0x20E);
    PUSH32(esp, esi);
    esi = (uint32_t)(int32_t)SMEM8(ecx);
    PUSH32(esp, edi);
    edi = (uint32_t)(int32_t)SMEM8(ecx + 1);
    ecx = (uint32_t)(int32_t)SMEM16(eax + 0x20C);
    esi = esi << 3;
    ecx = (uint32_t)((int32_t)ecx * (int32_t)esi);
    esi = esi + esi;
    esi = (uint32_t)((int32_t)esi * (int32_t)ebx);
    edi = edi << 3;
    ecx = ecx + edi;
    esi = esi + edi * 2;
    edi = MEM32(eax + 0x200);
    edi = edi + ecx;
    edx = eax + 0xF8;
    MEM32(edx + 4) = edi;
    edi = MEM32(eax + 0x204);
    edi = edi + ecx;
    MEM32(edx + 0xC) = edi;
    ecx = MEM32(eax + 0x208);
    ecx = ecx + esi;
    MEM32(edx + 0x14) = ecx;
    ecx = ecx + 8;
    MEM32(edx + 0x1C) = ecx;
    ecx = MEM32(edx + 0x14);
    ecx = ecx + ebx * 8;
    MEM32(edx + 0x24) = ecx;
    ecx = ecx + 8;
    POP32(esp, edi);
    MEM32(edx + 0x2C) = ecx;
    POP32(esp, esi);
    ecx = eax + 0x2E0;
    eax = MEM32(eax + 0x38);
    POP32(esp, ebx);
    g_seh_ebp = ebp; sub_001E20C0(); return; /* tail jmp 0x001E20C0 */

    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */

}


/**
 * sub_001E32D0
 * Original: 0x001E32D0 - 0x001E3350 (128 bytes, 55 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E32D0_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E32D0: ;
    esp = esp - 8;
    PUSH32(esp, ebx);
    ebx = MEM32(esp + 0x10);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x1C);
    esi--;
    PUSH32(esp, edi);
    edi = ebx + 0x1D0;
    ebp = edi + 0x10;
    if ((esi == 0)) goto loc_001E333B; /* je: equal / zero */

loc_001E32EB: ;
    goto loc_001E32F0;

    /* nop */

loc_001E32F0: ;
    eax = MEM32(ebx + 0x29C);
    edx = (uint32_t)(int32_t)SMEM16(edi + 0xC);
    eax = eax - esi;
    ecx = ebx + eax * 2 + 0xC68;
    eax = (uint32_t)(int32_t)SMEM8(ecx);
    ecx = (uint32_t)(int32_t)SMEM8(ecx + 1);
    eax = eax << 3;
    edx = (uint32_t)((int32_t)edx * (int32_t)eax);
    ecx = ecx << 3;
    edx = edx + ecx;
    MEM32(esp + 0x10) = edx;
    edx = (uint32_t)(int32_t)SMEM16(edi + 0xE);
    eax = eax + eax;
    eax = (uint32_t)((int32_t)eax * (int32_t)edx);
    eax = eax + ecx * 2;
    ecx = esp + 0x10;
    MEM32(esp + 0x14) = eax;
    PUSH32(esp, ecx);
    eax = ebp;
    PUSH32(esp, 0); sub_001E2CC0(); /* call 0x001E2CC0 */

loc_001E3335: ;
    esp = esp + 4;
    esi--;
    if ((esi != 0)) goto loc_001E32F0; /* jne: not equal / not zero */

loc_001E333B: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp = esp + 8;
    esp += 4; return; /* ret */

}


/**
 * sub_001E20C0
 * Original: 0x001E20C0 - 0x001E2280 (448 bytes, 152 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E20C0(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E20C0: ;
    esp = esp - 8;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = eax;
    eax = MEM32(edx);
    MEM32(esp + 0x10) = eax;
    eax = 6;
    PUSH32(esp, edi);
    edx = edx + 4;
    MEM32(esp + 0x10) = eax;
    goto loc_001E20E0;

    /* nop */

loc_001E20E0: ;
    if (CMP_NE(eax, MEM32(esp + 0x14))) goto loc_001E20E9; /* jne: not equal / not zero */

loc_001E20E6: ;
    esi = esi - 0x10;

loc_001E20E9: ;
    eax = MEM32(edx);
    edi = MEM32(edx + 4);
    edx = edx + 4;
    edx = edx + 4;
    ebp = 2;
    /* nop */

loc_001E2100: ;
    ebx = (uint32_t)(int32_t)SMEM16(ecx);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 2);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 1) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 4);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 2) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 6);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 3) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 8);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 4) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xA);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 5) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xC);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 6) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xE);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 7) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0x10);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + edi) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0x12);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + edi + 1) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0x14);
    SET_LO8(ebx, MEM8(ebx + esi));
    ecx = ecx + 0x10;
    MEM8(eax + edi + 2) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 6);
    SET_LO8(ebx, MEM8(ebx + esi));
    eax = eax + edi;
    MEM8(eax + 3) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 8);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 4) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xA);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 5) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xC);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 6) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xE);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 7) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0x10);
    SET_LO8(ebx, MEM8(ebx + esi));
    ecx = ecx + 0x10;
    MEM8(eax + edi) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 2);
    SET_LO8(ebx, MEM8(ebx + esi));
    eax = eax + edi;
    MEM8(eax + 1) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 4);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 2) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 6);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 3) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 8);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 4) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xA);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 5) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xC);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 6) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xE);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 7) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0x10);
    SET_LO8(ebx, MEM8(ebx + esi));
    ecx = ecx + 0x10;
    eax = eax + edi;
    MEM8(eax) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 2);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 1) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 4);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 2) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 6);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 3) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 8);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 4) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xA);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 5) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xC);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 6) = LO8(ebx);
    ebx = (uint32_t)(int32_t)SMEM16(ecx + 0xE);
    SET_LO8(ebx, MEM8(ebx + esi));
    MEM8(eax + 7) = LO8(ebx);
    ecx = ecx + 0x10;
    eax = eax + edi;
    ebp--;
    if ((ebp != 0)) goto loc_001E2100; /* jne: not equal / not zero */

loc_001E225A: ;
    eax = MEM32(esp + 0x10);
    eax--;
    /* test eax, eax - flags set for next jcc */
    MEM32(esp + 0x10) = eax;
    if (CMP_G(eax & eax, 0)) goto loc_001E20E0; /* jg: greater (signed >) */

loc_001E226B: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp = esp + 8;
    esp += 4; return; /* ret */

}

/**
 * sub_001E3350
 * Original: 0x001E3350 - 0x001E33E0 (144 bytes, 43 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3350(void)
{

loc_001E3350: ;
    esp = esp - 8;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x10);
    PUSH32(esp, edi);
    ecx = esi + 0x1D0;
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0xF0);
    edx = esp + 0xC;
    edi = esi + 0xE8;
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    eax = esi + 0x254;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_001E30F0(); /* call 0x001E30F0 */

loc_001E337E: ;
    edx = MEM32(esi + 0x200);
    eax = MEM32(esp + 0x18);
    edx = edx + eax;
    ecx = esi + 0xF8;
    MEM32(ecx + 4) = edx;
    edx = MEM32(esi + 0x204);
    edx = edx + eax;
    MEM32(ecx + 0xC) = edx;
    eax = MEM32(esi + 0x208);
    edx = MEM32(esp + 0x1C);
    eax = eax + edx;
    MEM32(ecx + 0x14) = eax;
    edx = MEM32(ecx + 0x14);
    eax = eax + 8;
    MEM32(ecx + 0x1C) = eax;
    eax = (uint32_t)(int32_t)SMEM16(esi + 0x20E);
    eax = edx + eax * 8;
    MEM32(ecx + 0x24) = eax;
    eax = eax + 8;
    MEM32(ecx + 0x2C) = eax;
    eax = MEM32(esi + 0x2A8);
    PUSH32(esp, eax);
    eax = edi;
    PUSH32(esp, 0); sub_001E2280(); /* call 0x001E2280 */

loc_001E33D7: ;
    esp = esp + 0x14;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = esp + 8;
    esp += 4; return; /* ret */

}


/**
 * sub_001E33E0
 * Original: 0x001E33E0 - 0x001E3470 (144 bytes, 43 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E33E0(void)
{

loc_001E33E0: ;
    esp = esp - 8;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x10);
    PUSH32(esp, edi);
    ecx = esi + 0x1E0;
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0xF0);
    edx = esp + 0xC;
    edi = esi + 0xE8;
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    eax = esi + 0x278;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_001E30F0(); /* call 0x001E30F0 */

loc_001E340E: ;
    edx = MEM32(esi + 0x200);
    eax = MEM32(esp + 0x18);
    edx = edx + eax;
    ecx = esi + 0xF8;
    MEM32(ecx + 4) = edx;
    edx = MEM32(esi + 0x204);
    edx = edx + eax;
    MEM32(ecx + 0xC) = edx;
    eax = MEM32(esi + 0x208);
    edx = MEM32(esp + 0x1C);
    eax = eax + edx;
    MEM32(ecx + 0x14) = eax;
    edx = MEM32(ecx + 0x14);
    eax = eax + 8;
    MEM32(ecx + 0x1C) = eax;
    eax = (uint32_t)(int32_t)SMEM16(esi + 0x20E);
    eax = edx + eax * 8;
    MEM32(ecx + 0x24) = eax;
    eax = eax + 8;
    MEM32(ecx + 0x2C) = eax;
    eax = MEM32(esi + 0x2A8);
    PUSH32(esp, eax);
    eax = edi;
    PUSH32(esp, 0); sub_001E2280(); /* call 0x001E2280 */

loc_001E3467: ;
    esp = esp + 0x14;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = esp + 8;
    esp += 4; return; /* ret */

}


/**
 * sub_001E3470
 * Original: 0x001E3470 - 0x001E3520 (176 bytes, 59 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3470(void)
{

loc_001E3470: ;
    esp = esp - 8;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x14);
    edx = MEM32(esi + 0xF0);
    PUSH32(esp, edi);
    ebx = esi + 0x1D0;
    PUSH32(esp, ebx);
    ecx = esp + 0x10;
    edi = esi + 0xE8;
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    eax = esi + 0x254;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_001E30F0(); /* call 0x001E30F0 */

loc_001E349F: ;
    edx = MEM32(edi + 0xC);
    ebx = ebx + 0x10;
    PUSH32(esp, ebx);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    eax = esi + 0x278;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_001E30F0(); /* call 0x001E30F0 */

loc_001E34B8: ;
    ecx = MEM32(esi + 0x200);
    eax = MEM32(esp + 0x2C);
    ecx = ecx + eax;
    edx = esi + 0xF8;
    MEM32(edx + 4) = ecx;
    ecx = MEM32(esi + 0x204);
    ecx = ecx + eax;
    MEM32(edx + 0xC) = ecx;
    eax = MEM32(esi + 0x208);
    ecx = MEM32(esp + 0x30);
    eax = eax + ecx;
    MEM32(edx + 0x14) = eax;
    ecx = MEM32(edx + 0x14);
    eax = eax + 8;
    MEM32(edx + 0x1C) = eax;
    eax = (uint32_t)(int32_t)SMEM16(esi + 0x20E);
    eax = ecx + eax * 8;
    MEM32(edx + 0x24) = eax;
    eax = eax + 8;
    MEM32(edx + 0x2C) = eax;
    eax = MEM32(esi + 0x2A8);
    PUSH32(esp, eax);
    eax = edi;
    PUSH32(esp, 0); sub_001E25E0(); /* call 0x001E25E0 */

loc_001E3511: ;
    esp = esp + 0x24;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = esp + 8;
    esp += 4; return; /* ret */

}

/**
 * sub_001E3C70
 * Original: 0x001E3C70 - 0x001E3D00 (144 bytes, 59 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3C70(void)
{
    uint64_t mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

loc_001E3C70: ;
    esp = esp - 8;
    eax = MEM32(esp + 0xC);
    ecx = MEM32(eax + 0x14);
    edx = MEM32(eax + 8);
    eax = MEM32(eax + 0x10);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    MEM32(esp + 0xC) = ecx;
    MEM32(esp + 0x14) = edx;
    MEM32(esp + 8) = eax;
    esi = MEM32(esp + 0xC);
    eax = MEM32(esp + 8);
    edi = MEM32(esp + 0x14);
    mm0 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm0); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm1 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm1); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm2 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm2); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm3 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm3); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm4 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm4); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm5 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm5); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm6 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm6); /* movq */
    esi = esi + eax;
    edi = edi + 8;
    mm7 = MMQ_LOAD(esi); /* movq */
    MMQ_STORE(edi, mm7); /* movq */
    /* emms */
    POP32(esp, edi);
    POP32(esp, esi);
    esp = esp + 8;
    esp += 4; return; /* ret */

}

/**
 * sub_001E5B0C
 * Original: 0x001E5B0C - 0x001E5D4C (576 bytes, 171 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E5B0C_gen(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_001E5B0C: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x10;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    eax = 0; /* xor self */
    ecx = 0xC04;
    edi = 0xB1F818;
    { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = eax; }
    edi += ecx * 4; ecx = 0; /* rep stosd */
    eax = 0xB1F81C;
    PUSH32(esp, 0x60);
    MEM32(0xB1F820) = eax;
    MEM32(0xB1F81C) = eax;
    { uint32_t _icall_t = MEM32(0x1ED1DC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5B39: ;
    edi = eax;
    PUSH32(esp, 0x18);
    POP32(esp, ecx);
    eax = 0; /* xor self */
    MEM32(0xB22824) = edi;
    { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = eax; }
    edi += ecx * 4; ecx = 0; /* rep stosd */
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0); sub_001E5E8A(); /* call 0x001E5E8A */

loc_001E5B50: ;
    if (TEST_Z(eax, eax)) goto loc_001E5B5D; /* je: equal / zero */

loc_001E5B54: ;
    MEM32(ebp + -8) = 8;
    goto loc_001E5B77;

loc_001E5B5D: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0x1E5960);
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5B6A: ;
    PUSH32(esp, 8);
    POP32(esp, ecx);
    /* cmp eax, ecx - flags set for next jcc */
    MEM32(ebp + -8) = eax;
    if (CMP_BE(eax, ecx)) goto loc_001E5B77; /* jbe: below or equal (unsigned <=) */

loc_001E5B74: ;
    MEM32(ebp + -8) = ecx;

loc_001E5B77: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1ED09C);
    eax = ebp + -16;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x1ED11C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5B86: ;
    esi = 0; /* xor self */
    if (CMP_BE(MEM32(ebp + -8), esi)) goto loc_001E5BFD; /* jbe: below or equal (unsigned <=) */

loc_001E5B8D: ;
    /* cmp esi, 9 - flags set for next jcc */
    eax = esi + 0x41;
    if (CMP_A(esi, 9)) goto loc_001E5B98; /* ja: above (unsigned >) */

loc_001E5B95: ;
    eax = esi + 0x30;

loc_001E5B98: ;
    ecx = MEM32(ebp + -12);
    MEM8(ecx + 0xB) = LO8(eax);
    eax = ebp + -4;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3A);
    eax = ebp + -16;
    PUSH32(esp, eax);
    PUSH32(esp, 0x170);
    PUSH32(esp, 0x1E5998);
    { uint32_t _icall_t = MEM32(0x1ED1D8); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5BBA: ;
    if (TEST_S(eax, eax)) goto loc_001E5BFD; /* jl: less (signed <) */

loc_001E5BBE: ;
    eax = MEM32(ebp + -4);
    edx = MEM32(eax + 0x18);
    eax = 0; /* xor self */
    PUSH32(esp, 0x5C);
    POP32(esp, ecx);
    edi = edx;
    { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = eax; }
    edi += ecx * 4; ecx = 0; /* rep stosd */
    eax = MEM32(ebp + -4);
    MEM32(edx) = eax;
    MEM32(edx + 4) = esi;
    MEM32(edx + 0xC) = 4;
    eax = MEM32(ebp + -4);
    MEM8(eax + 0x1E) = 1;
    eax = MEM32(ebp + -4);
    MEM32(eax + 0x14) = MEM32(eax + 0x14) | 4;
    eax = MEM32(ebp + -4);
    MEM32(eax + 0x14) = MEM32(eax + 0x14) & 0xFFFFFFEFu;
    PUSH32(esp, edx);
    PUSH32(esp, 0); sub_001E6A02(); /* call 0x001E6A02 */

loc_001E5BF7: ;
    esi++;
    if (CMP_B(esi, MEM32(ebp + -8))) goto loc_001E5B8D; /* jb: below (unsigned <) */

loc_001E5BFD: ;
    eax = MEM32(ebp + -8);
    /* cmp eax, 1 - flags set for next jcc */
    POP32(esp, edi);
    MEM8(0x1E5989) = LO8(eax);
    POP32(esp, esi);
    if (CMP_BE(eax, 1)) goto loc_001E5C12; /* jbe: below or equal (unsigned <=) */

loc_001E5C0C: ;
    MEM8(0x1E598F) = MEM8(0x1E598F) << 1;

loc_001E5C12: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0x1E5988);
    PUSH32(esp, 0); sub_001E5E96(); /* call 0x001E5E96 */

loc_001E5C1F: ;
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    ecx = esi;
    MEM32(0x1E5A4C) = 1;
    MEM16(0xB22828) = 4;
    PUSH32(esp, 0); sub_001E5E8A(); /* call 0x001E5E8A */

loc_001E5C50: ;
    if (TEST_NZ(eax, eax)) goto loc_001E5C94; /* jne: not equal / not zero */

loc_001E5C54: ;
    PUSH32(esp, 0x1E59E0);
    ecx = esi;
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C60: ;
    PUSH32(esp, 0x1E59EC);
    ecx = esi;
    MEM8(0x1E5AA8) = LO8(eax);
    ebx = ZX8(LO8(eax));
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C74: ;
    MEM8(0x1E5AC0) = LO8(eax);
    eax = ZX8(LO8(eax));
    PUSH32(esp, 0x1E59F8);
    ecx = esi;
    ebx = ebx + eax;
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C8A: ;
    MEM8(0x1E5AD8) = LO8(eax);
    eax = ZX8(LO8(eax));
    goto loc_001E5CAB;

loc_001E5C94: ;
    eax = ZX8(MEM8(0x1E5AC0));
    ebx = ZX8(MEM8(0x1E5AD8));
    ebx = ebx + eax;
    eax = ZX8(MEM8(0x1E5AA8));

loc_001E5CAB: ;
    ebx = ebx + eax;
    eax = ZX16(MEM16(0xB22828));
    if (CMP_BE(ebx, eax)) goto loc_001E5CBA; /* jbe: below or equal (unsigned <=) */

loc_001E5CB8: ;
    ebx = eax;

loc_001E5CBA: ;
    ecx = ebx;
    ecx = (uint32_t)((int32_t)ecx * (int32_t)0xA8);
    eax = eax + eax * 8;
    eax = ecx + eax * 2;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x1ED1DC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5CCF: ;
    ecx = 0; /* xor self */
    /* test ebx, ebx - flags set for next jcc */
    MEM32(0xB22830) = ecx;
    if (CMP_BE(ebx & ebx, 0)) goto loc_001E5CF3; /* jbe: below or equal (unsigned <=) */

loc_001E5CDB: ;
    edx = ebx;

loc_001E5CDD: ;
    MEM32(eax + 0xA4) = ecx;
    ecx = eax;
    eax = eax + 0xA8;
    edx--;
    if ((edx != 0)) goto loc_001E5CDD; /* jne: not equal / not zero */

loc_001E5CED: ;
    MEM32(0xB22830) = ecx;

loc_001E5CF3: ;
    MEM16(0xB2282A) = MEM16(0xB2282A) & 0;
    edx = 0; /* xor self */
    /* cmp MEM16(0xB22828), LO16(edx) - flags set for next jcc */
    MEM32(0xB2282C) = eax;
    if (CMP_BE(MEM16(0xB22828), LO16(edx))) goto loc_001E5D28; /* jbe: below or equal (unsigned <=) */

loc_001E5D0B: ;
    ecx = 0; /* xor self */

loc_001E5D0D: ;
    eax = MEM32(0xB2282C);
    eax = ecx + eax + 4;
    MEM8(eax) = MEM8(eax) & 0xFE;
    eax = ZX16(MEM16(0xB22828));
    edx++;
    ecx = ecx + 0x12;
    if (CMP_B(edx, eax)) goto loc_001E5D0D; /* jb: below (unsigned <) */

loc_001E5D28: ;
    PUSH32(esp, 0x1E5A40);
    ecx = esi;
    MEM8(0x1E5A41) = LO8(ebx);
    PUSH32(esp, 0); sub_001E5E96(); /* call 0x001E5E96 */

loc_001E5D3A: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0xB22880);
    { uint32_t _icall_t = MEM32(0x1ED1E8); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5D47: ;
    POP32(esp, esi);
    POP32(esp, ebx);

loc_001E5D49: ;
    esp += 8; return; /* ret 4 */

}


/**
 * sub_001E5C23
 * Original: 0x001E5C23 - 0x001E5D4C (297 bytes, 78 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E5C23_gen(void)
{
    int _flags = 0; /* fallback flag var */

loc_001E5C23: ;
    if (CMP_NE(MEM32(0x1E5A4C), 0)) goto loc_001E5D49; /* jne: not equal / not zero */

loc_001E5C30: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    ecx = esi;
    MEM32(0x1E5A4C) = 1;
    MEM16(0xB22828) = 4;
    PUSH32(esp, 0); sub_001E5E8A(); /* call 0x001E5E8A */

loc_001E5C50: ;
    if (TEST_NZ(eax, eax)) goto loc_001E5C94; /* jne: not equal / not zero */

loc_001E5C54: ;
    PUSH32(esp, 0x1E59E0);
    ecx = esi;
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C60: ;
    PUSH32(esp, 0x1E59EC);
    ecx = esi;
    MEM8(0x1E5AA8) = LO8(eax);
    ebx = ZX8(LO8(eax));
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C74: ;
    MEM8(0x1E5AC0) = LO8(eax);
    eax = ZX8(LO8(eax));
    PUSH32(esp, 0x1E59F8);
    ecx = esi;
    ebx = ebx + eax;
    PUSH32(esp, 0); sub_001E5E53(); /* call 0x001E5E53 */

loc_001E5C8A: ;
    MEM8(0x1E5AD8) = LO8(eax);
    eax = ZX8(LO8(eax));
    goto loc_001E5CAB;

loc_001E5C94: ;
    eax = ZX8(MEM8(0x1E5AC0));
    ebx = ZX8(MEM8(0x1E5AD8));
    ebx = ebx + eax;
    eax = ZX8(MEM8(0x1E5AA8));

loc_001E5CAB: ;
    ebx = ebx + eax;
    eax = ZX16(MEM16(0xB22828));
    if (CMP_BE(ebx, eax)) goto loc_001E5CBA; /* jbe: below or equal (unsigned <=) */

loc_001E5CB8: ;
    ebx = eax;

loc_001E5CBA: ;
    ecx = ebx;
    ecx = (uint32_t)((int32_t)ecx * (int32_t)0xA8);
    eax = eax + eax * 8;
    eax = ecx + eax * 2;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x1ED1DC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5CCF: ;
    ecx = 0; /* xor self */
    /* test ebx, ebx - flags set for next jcc */
    MEM32(0xB22830) = ecx;
    if (CMP_BE(ebx & ebx, 0)) goto loc_001E5CF3; /* jbe: below or equal (unsigned <=) */

loc_001E5CDB: ;
    edx = ebx;

loc_001E5CDD: ;
    MEM32(eax + 0xA4) = ecx;
    ecx = eax;
    eax = eax + 0xA8;
    edx--;
    if ((edx != 0)) goto loc_001E5CDD; /* jne: not equal / not zero */

loc_001E5CED: ;
    MEM32(0xB22830) = ecx;

loc_001E5CF3: ;
    MEM16(0xB2282A) = MEM16(0xB2282A) & 0;
    edx = 0; /* xor self */
    /* cmp MEM16(0xB22828), LO16(edx) - flags set for next jcc */
    MEM32(0xB2282C) = eax;
    if (CMP_BE(MEM16(0xB22828), LO16(edx))) goto loc_001E5D28; /* jbe: below or equal (unsigned <=) */

loc_001E5D0B: ;
    ecx = 0; /* xor self */

loc_001E5D0D: ;
    eax = MEM32(0xB2282C);
    eax = ecx + eax + 4;
    MEM8(eax) = MEM8(eax) & 0xFE;
    eax = ZX16(MEM16(0xB22828));
    edx++;
    ecx = ecx + 0x12;
    if (CMP_B(edx, eax)) goto loc_001E5D0D; /* jb: below (unsigned <) */

loc_001E5D28: ;
    PUSH32(esp, 0x1E5A40);
    ecx = esi;
    MEM8(0x1E5A41) = LO8(ebx);
    PUSH32(esp, 0); sub_001E5E96(); /* call 0x001E5E96 */

loc_001E5D3A: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0xB22880);
    { uint32_t _icall_t = MEM32(0x1ED1E8); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E5D47: ;
    POP32(esp, esi);
    POP32(esp, ebx);

loc_001E5D49: ;
    esp += 8; return; /* ret 4 */

}


/**
 * sub_001E63D2
 * Original: 0x001E63D2 - 0x001E63F3 (33 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E63D2_gen(void)
{

loc_001E63D2: ;
    MEM16(0xB22A6A) = MEM16(0xB22A6A) & 0;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0xB22A20);
    MEM16(0xB22A68) = 6;
    { uint32_t _icall_t = MEM32(0x1ED1E8); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_001E63F0: ;
    esp += 8; return; /* ret 4 */

}


/**
 * sub_001E6E02
 * Original: 0x001E6E02 - 0x001E6E3A (56 bytes, 12 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E6E02_gen(void)
{

loc_001E6E02: ;
    eax = MEM32(0x1ED1F0);
    MEM32(0x1E59A4) = eax;
    MEM32(0x1E59A8) = eax;
    MEM32(0x1E59B4) = eax;
    MEM32(0x1E59B8) = eax;
    MEM32(0x1E59BC) = eax;
    MEM32(0x1E59C0) = eax;
    MEM32(0x1E59C4) = eax;
    MEM32(0x1E59C8) = eax;
    MEM32(0x1E59D4) = eax;
    MEM32(0x1E59D8) = eax;
    esp += 4; return; /* ret */

}

/**
 * sub_001E3720
 * Original: 0x001E3720 - 0x001E3B20 (1024 bytes, 318 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3720(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001E3720: ;
    edx = MEM32(esp + 4);
    eax = MEM32(edx + 0x14);
    ecx = MEM32(edx + 0x18);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = MEM32(edx + 8);
    PUSH32(esp, edi);
    edi = MEM32(edx + 0x10);
    ebp = 2;
    /* nop */

loc_001E3740: ;
    ebx = ZX8(MEM8(eax));
    edx = ZX8(MEM8(ecx));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 1) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 2) = LO8(edx);
    ebx = ZX8(MEM8(eax + 3));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 3) = LO8(edx);
    ebx = ZX8(MEM8(eax + 5));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 4) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 5));
    edx = ZX8(MEM8(eax + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 6));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 5) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 6));
    edx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 6) = LO8(edx);
    ebx = ZX8(MEM8(eax + 8));
    edx = ZX8(MEM8(ecx + 8));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 7) = LO8(edx);
    ebx = ZX8(MEM8(eax + edi));
    edx = ZX8(MEM8(ecx + edi));
    eax = eax + edi;
    ecx = ecx + edi;
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 8) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 9) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xA) = LO8(edx);
    ebx = ZX8(MEM8(eax + 3));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xB) = LO8(edx);
    ebx = ZX8(MEM8(eax + 5));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xC) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 5));
    edx = ZX8(MEM8(eax + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 6));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xD) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 6));
    edx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xE) = LO8(edx);
    ebx = ZX8(MEM8(eax + 8));
    edx = ZX8(MEM8(ecx + 8));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0xF) = LO8(edx);
    ebx = ZX8(MEM8(eax + edi));
    edx = ZX8(MEM8(ecx + edi));
    ecx = ecx + edi;
    eax = eax + edi;
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x10) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x11) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x12) = LO8(edx);
    ebx = ZX8(MEM8(eax + 3));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x13) = LO8(edx);
    ebx = ZX8(MEM8(eax + 5));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x14) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 5));
    edx = ZX8(MEM8(eax + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 6));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x15) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 6));
    edx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x16) = LO8(edx);
    ebx = ZX8(MEM8(eax + 8));
    edx = ZX8(MEM8(ecx + 8));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x17) = LO8(edx);
    ebx = ZX8(MEM8(eax + edi));
    edx = ZX8(MEM8(ecx + edi));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + edi + 1));
    eax = eax + edi;
    ecx = ecx + edi;
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x18) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 2));
    edx = ZX8(MEM8(eax + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 1));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 1));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x19) = LO8(edx);
    edx = ZX8(MEM8(eax + 2));
    ebx = ZX8(MEM8(ecx + 2));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1A) = LO8(edx);
    ebx = ZX8(MEM8(eax + 3));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 3));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1B) = LO8(edx);
    ebx = ZX8(MEM8(eax + 5));
    edx = ZX8(MEM8(ecx + 4));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 4));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1C) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 5));
    edx = ZX8(MEM8(eax + 5));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 6));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1D) = LO8(edx);
    ebx = ZX8(MEM8(ecx + 6));
    edx = ZX8(MEM8(eax + 6));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1E) = LO8(edx);
    ebx = ZX8(MEM8(eax + 8));
    edx = ZX8(MEM8(ecx + 8));
    edx = edx + ebx;
    ebx = ZX8(MEM8(eax + 7));
    edx = edx + ebx;
    ebx = ZX8(MEM8(ecx + 7));
    edx = edx + ebx + 2;
    edx = (uint32_t)((int32_t)edx >> 2);
    MEM8(esi + 0x1F) = LO8(edx);
    eax = eax + edi;
    ecx = ecx + edi;
    esi = esi + 0x20;
    ebp--;
    if ((ebp != 0)) goto loc_001E3740; /* jne: not equal / not zero */

loc_001E3B18: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}


/**
 * sub_001E3B20
 * Original: 0x001E3B20 - 0x001E3BD0 (176 bytes, 67 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3B20(void)
{
    uint64_t mm0;

loc_001E3B20: ;
    esp = esp - 0xC;
    eax = MEM32(esp + 0x10);
    ecx = MEM32(eax + 0x10);
    edx = MEM32(eax + 0x14);
    MEM32(esp + 0x10) = ecx;
    ecx = MEM32(eax + 0x18);
    MEM32(esp + 8) = edx;
    edx = MEM32(eax + 8);
    PUSH32(esp, ebx);
    MEM32(esp + 8) = ecx;
    MEM32(esp + 4) = edx;
    eax = MEM32(esp + 0xC);
    ebx = MEM32(esp + 8);
    ecx = MEM32(esp + 4);
    edx = MEM32(esp + 0x14);
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 8, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x10, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x18, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x20, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x28, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x30, mm0); /* movq */
    eax = eax + edx;
    ebx = ebx + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(ebx)); /* pavgb */
    MMQ_STORE(ecx + 0x38, mm0); /* movq */
    /* emms */
    POP32(esp, ebx);
    esp = esp + 0xC;
    esp += 4; return; /* ret */

}


/**
 * sub_001E3BD0
 * Original: 0x001E3BD0 - 0x001E3C70 (160 bytes, 58 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_001E3BD0(void)
{
    uint64_t mm0;

loc_001E3BD0: ;
    esp = esp - 8;
    eax = MEM32(esp + 0xC);
    ecx = MEM32(eax + 0x10);
    edx = MEM32(eax + 0x14);
    eax = MEM32(eax + 8);
    MEM32(esp + 0xC) = ecx;
    MEM32(esp + 4) = edx;
    MEM32(esp) = eax;
    eax = MEM32(esp + 4);
    ecx = MEM32(esp);
    edx = MEM32(esp + 0xC);
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 8, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x10, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x18, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x20, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x28, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x30, mm0); /* movq */
    eax = eax + edx;
    mm0 = MMQ_LOAD(eax); /* movq */
    mm0 = mmx_pavgb(mm0, MMQ_LOAD(eax + 1)); /* pavgb */
    MMQ_STORE(ecx + 0x38, mm0); /* movq */
    /* emms */
    esp = esp + 8;
    esp += 4; return; /* ret */

}

