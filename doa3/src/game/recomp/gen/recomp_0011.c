/**
 * Dead or Alive 3 - Recompiled code chunk 11
 * Functions: 29 (0x0021743D - 0xE9D2ECA9)
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <math.h>

/**
 * sub_0021743D
 * Original: 0x0021743D - 0x00217440 (3 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021743D(void)
{

loc_0021743D: ;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00217440
 * Original: 0x00217440 - 0x00217473 (51 bytes, 23 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217440_gen(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_00217440: ;
    ecx = MEM32(ecx + 0x14);
    PUSH32(esp, esi);
    esi = edx;
    edx = 0; /* xor self */
    SET_LO16(edx, MEM16(esi + 2));
    eax = 0; /* xor self */
    edx = edx & 0x7FF;
    if (TEST_Z(ecx, ecx)) goto loc_00217468; /* je: equal / zero */

loc_00217458: ;
    PUSH32(esp, edi);
    edi = ZX16(LO16(edx));
    edx = 0; /* xor self */
    eax = ecx;
    { uint64_t _dividend = ((uint64_t)edx << 32) | eax;
      eax = (uint32_t)(_dividend / (uint32_t)edi);
      edx = (uint32_t)(_dividend % (uint32_t)edi); }
    POP32(esp, edi);
    if (TEST_Z(edx, edx)) goto loc_00217468; /* je: equal / zero */

loc_00217467: ;
    eax++;

loc_00217468: ;
    /* cmp MEM8(esi + 0x11), 0 - flags set for next jcc */
    _fs0 = (uint32_t)(MEM8(esi + 0x11)); /* flag operand kept for a later jcc */
    POP32(esp, esi);
    if (CMP_NE(LO8(_fs0), 0)) goto loc_00217472; /* jne: not equal / not zero */

loc_0021746F: ;
    eax = eax + 3;

loc_00217472: ;
    esp += 4; return; /* ret */

}

/**
 * sub_00217473
 * Original: 0x00217473 - 0x002174AD (58 bytes, 25 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217473_gen(void)
{
    int _flags = 0; /* fallback flag var */

loc_00217473: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    PUSH32(esp, MEM32(esi));
    edi = edx;
    { uint32_t _icall_t = MEM32(0x217B84); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217482: ;
    ecx = MEM32(esi);
    ecx = ecx & 0xFFF;
    edx = 0x1000;
    edx = edx - ecx;
    ecx = MEM32(esp + 0x10);
    MEM32(ecx) = edx;
    ebx = MEM32(edi);
    if (CMP_BE(edx, ebx)) goto loc_0021749F; /* jbe: below or equal (unsigned <=) */

loc_0021749D: ;
    MEM32(ecx) = ebx;

loc_0021749F: ;
    edx = MEM32(ecx);
    MEM32(edi) = MEM32(edi) - edx;
    ecx = MEM32(ecx);
    MEM32(esi) = MEM32(esi) + ecx;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002174AD
 * Original: 0x002174AD - 0x002177AB (766 bytes, 247 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002174AD_gen(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0, _fs4 = 0, _fs5 = 0, _fs6 = 0, _fs7 = 0, _fs8 = 0, _fs9 = 0, _fs10 = 0, _fs11 = 0, _fs12 = 0, _fs13 = 0, _fs14 = 0, _fs15 = 0, _fs16 = 0, _fs17 = 0, _fs18 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002174AD: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x28;
    eax = 0; /* xor self */
    PUSH32(esp, ebx);
    ebx = edx;
    SET_LO16(eax, MEM16(ebx + 2));
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(ebp + 8);
    MEM32(ebp + -32) = ecx;
    SET_LO8(ecx, MEM8(ecx + 0x45C));
    MEM8(ebp + -24) = LO8(ecx);
    eax = eax & 0x7FF;
    MEM32(ebp + -40) = eax;
    eax = MEM32(edi + 0x14);
    MEM32(ebp + -16) = eax;
    eax = 0; /* xor self */
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) & LO8(eax);
    MEM8(ebx + 0x26) = MEM8(ebx + 0x26) - 1;
    MEM8(ebx + 0x27) = MEM8(ebx + 0x27) + 1;
    SET_LO16(ecx, MEM16(edi + 0x22));
    SET_LO16(ecx, LO16(ecx) & 0xFFFD);
    SET_LO16(ecx, LO16(ecx) | 4);
    MEM16(edi + 0x22) = LO16(ecx);
    esi = MEM32(ebx + 4);
    /* cmp esi, eax - flags set for next jcc */
    MEM32(ebp + -20) = eax;
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -12) = eax;
    MEM32(ebp + -8) = eax;
    if (CMP_EQ(esi, eax)) goto loc_00217513; /* je: equal / zero */

loc_0021750A: ;
    eax = MEM32(0xC6B9A0);
    esi = esi + eax;
    goto loc_0021752C;

loc_00217513: ;
    PUSH32(esp, MEM32(ebp + -24));
    _fs0 = (uint32_t)(esi); /* flag operand kept for a later jcc */
    _fs1 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_0021751B: ;
    esi = eax;
    eax = MEM32(esi + 0x10);
    eax = eax ^ MEM32(ebx + 8);
    eax = eax & 0xF;
    eax = eax ^ MEM32(esi + 0x10);
    _fs2 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    _fs3 = (uint32_t)(MEM32(esi + 0x10)); /* flag operand kept for a later jcc */
    MEM32(ebx + 8) = eax;

loc_0021752C: ;
    /* cmp MEM8(ebx + 0x11), 0 - flags set for next jcc */
    _fs4 = (uint32_t)(MEM8(ebx + 0x11)); /* flag operand kept for a later jcc */
    MEM32(ebp + -4) = esi;
    if (CMP_NE(LO8(_fs4), 0)) goto loc_0021759B; /* jne: not equal / not zero */

loc_00217535: ;
    eax = 0; /* xor self */
    SET_LO8(eax, 0xFE);
    SET_LO8(eax, LO8(eax) - MEM8(ebp + -24));
    PUSH32(esp, eax);
    _fs5 = (uint32_t)(LO8(eax)); /* flag operand kept for a later jcc */
    _fs6 = (uint32_t)(MEM8(ebp + -24)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217542: ;
    ecx = MEM32(edi + 0x28);
    PUSH32(esp, MEM32(ebp + -24));
    MEM32(eax) = ecx;
    ecx = MEM32(edi + 0x2C);
    MEM32(ebp + -36) = eax;
    MEM32(eax + 4) = ecx;
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217558: ;
    esi = eax;
    eax = MEM32(ebp + -4);
    ecx = MEM32(eax);
    ecx = ecx & 0x3FFFF;
    ecx = ecx | 0xE2E00000u;
    MEM32(eax) = ecx;
    ecx = MEM32(ebp + -36);
    edx = MEM32(ecx + 0x10);
    MEM32(eax + 4) = edx;
    edx = MEM32(esi + 0x10);
    MEM32(eax + 8) = edx;
    ecx = MEM32(ecx + 0x10);
    MEM8(eax + 0x1C) = MEM8(eax + 0x1C) & 0;
    ecx = ecx + 7;
    MEM8(eax + 0x1D) = MEM8(eax + 0x1D) & 0;
    _fs7 = (uint32_t)(MEM8(eax + 0x1D)); /* flag operand kept for a later jcc */
    MEM8(ebp + 0xB) = 2;
    MEM32(eax + 0xC) = ecx;
    MEM32(eax + 0x14) = ebx;
    MEM8(eax + 0x1E) = 1;
    MEM32(eax + 0x18) = edi;

loc_0021759B: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002175B7; /* je: equal / zero */

loc_002175A1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, MEM32(ebp + -16));
    PUSH32(esp, MEM32(edi + 0x18));
    _fs8 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    { uint32_t _icall_t = MEM32(0x217B88); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002175AF: ;
    eax = MEM32(edi + 0x18);
    MEM32(ebp + -36) = eax;
    goto loc_002175BB;

loc_002175B7: ;
    MEM8(edi + 0x1C) = 1;

loc_002175BB: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002175C5: ;
    eax = ebp + -20;
    PUSH32(esp, eax);
    edx = ebp + -16;
    ecx = ebp + -36;
    _fs9 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217473(); /* call 0x00217473 */

loc_002175D4: ;
    MEM32(ebp + -4) = eax;

loc_002175D7: ;
    edx = MEM32(ebp + -20);
    eax = MEM32(ebp + -12);
    ecx = MEM32(ebp + -40);
    eax = eax + edx;
    if (CMP_AE(eax, ecx)) goto loc_002175F8; /* jae: above or equal (unsigned >=) */

loc_002175E6: ;
    if (TEST_Z(edx, edx)) goto loc_002176B0; /* je: equal / zero */

loc_002175EE: ;
    if (CMP_NE(MEM32(ebp + -16), 0)) goto loc_002176B0; /* jne: not equal / not zero */

loc_002175F8: ;
    if (CMP_EQ(MEM32(ebp + -12), 0)) goto loc_00217620; /* je: equal / zero */

loc_002175FE: ;
    ecx = ecx - MEM32(ebp + -12);
    eax = MEM32(ebp + -28);
    /* cmp edx, ecx - flags set for next jcc */
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_0021760D; /* jae: above or equal (unsigned >=) */

loc_0021760B: ;
    _fs10 = (uint32_t)(edx); /* flag operand kept for a later jcc */
    _fs11 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    ecx = edx;

loc_0021760D: ;
    SET_LO8(eax, MEM8(ebp + -12));
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    SET_LO8(eax, LO8(eax) + LO8(ecx));
    edx = edx - ecx;
    MEM32(ebp + -12) = MEM32(ebp + -12) & 0;
    MEM8(esi + 0x1D) = LO8(eax);
    goto loc_0021763E;

loc_00217620: ;
    /* cmp edx, ecx - flags set for next jcc */
    eax = MEM32(ebp + -4);
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_00217636; /* jae: above or equal (unsigned >=) */

loc_0021762A: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + edx;
    MEM32(ebp + -20) = MEM32(ebp + -20) & 0;
    MEM8(esi + 0x1D) = LO8(edx);
    goto loc_00217641;

loc_00217636: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    MEM8(esi + 0x1D) = LO8(ecx);
    edx = edx - ecx;

loc_0021763E: ;
    MEM32(ebp + -20) = edx;

loc_00217641: ;
    eax = MEM32(ebp + -4);
    ecx = MEM32(esi);
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) ^ 1;
    MEM8(esi + 0x1C) = MEM8(esi + 0x1C) & 0;
    MEM8(esi + 0x1E) = MEM8(esi + 0x1E) & 0;
    eax--;
    MEM32(esi + 0xC) = eax;
    eax = ZX8(MEM8(ebp + 0xB));
    PUSH32(esp, MEM32(ebp + -24));
    ecx = ecx & 0xFFBFFFF;
    ecx = ecx | 0xE0000000u;
    eax = eax << 0x18;
    eax = eax ^ ecx;
    eax = eax & 0x3000000;
    eax = eax ^ ecx;
    MEM32(esi) = ecx;
    eax = eax | 0xE00000;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    MEM32(esi + 0x14) = ebx;
    SET_LO8(ecx, MEM8(edi + 0x1C));
    eax = eax & 0xF3E7FFFFu;
    MEM32(esi + 0x18) = edi;
    MEM32(ebp + -8) = esi;
    ecx = ecx & 3;
    ecx = ecx << 0x13;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    _fs12 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs13 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_002176A0: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_002175D7;

loc_002176B0: ;
    /* cmp MEM32(ebp + -16), 0 - flags set for next jcc */
    eax = MEM32(ebp + -4);
    _fs14 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -12) = edx;
    if (CMP_NE(_fs14, 0)) goto loc_002175C5; /* jne: not equal / not zero */

loc_002176C3: ;
    if (CMP_EQ(MEM32(ebp + -8), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176C9: ;
    if (CMP_EQ(MEM8(edi + 0x1D), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176CF: ;
    eax = MEM32(ebp + -8);
    MEM8(eax + 2) = MEM8(eax + 2) | 4;

loc_002176D6: ;
    if (CMP_NE(MEM8(ebx + 0x11), 0)) goto loc_00217746; /* jne: not equal / not zero */

loc_002176DC: ;
    MEM8(esi + 2) = MEM8(esi + 2) & 0xFB;
    ecx = MEM32(esi);
    eax = 0; /* xor self */
    /* cmp MEM8(edi + 0x1C), 2 - flags set for next jcc */
    _fs15 = (uint32_t)(MEM8(edi + 0x1C)); /* flag operand kept for a later jcc */
    PUSH32(esp, MEM32(ebp + -24));
    SET_LO8(eax, (CMP_NE(LO8(_fs15), 2)) ? 1 : 0); /* setne */
    MEM32(ebp + -8) = esi;
    eax++;
    eax = eax << 0x13;
    eax = eax ^ ecx;
    eax = eax & 0x180000;
    eax = eax ^ ecx;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    SET_LO8(ecx, MEM8(edi + 0x1E));
    MEM32(esi + 4) = MEM32(esi + 4) & 0;
    MEM32(esi + 0xC) = MEM32(esi + 0xC) & 0;
    MEM8(esi + 0x1D) = MEM8(esi + 0x1D) & 0;
    eax = eax & 0x1FFFFF;
    MEM32(esi + 0x14) = ebx;
    MEM8(esi + 0x1C) = 2;
    ecx = ecx & 7;
    ecx = ecx | 0xFFFFFF18u;
    ecx = ecx << 0x15;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    MEM8(esi + 0x1E) = 2;
    MEM32(esi + 0x18) = edi;
    _fs16 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs17 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217739: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_0021775E;

loc_00217746: ;
    ecx = ZX8(MEM8(edi + 0x1E));
    eax = MEM32(ebp + -8);
    ecx = ecx << 0x15;
    ecx = ecx ^ MEM32(eax);
    MEM8(eax + 0x1C) = 2;
    ecx = ecx & 0xE00000;
    MEM32(eax) = MEM32(eax) ^ ecx;

loc_0021775E: ;
    MEM8(esi + 0x1E) = 3;
    SET_LO16(eax, MEM16(edi + 0x14));
    MEM32(edi + 0x14) = MEM32(edi + 0x14) & 0;
    MEM16(edi + 0x20) = LO16(eax);
    /* cmp MEM8(ebx + 0x20), 0 - flags set for next jcc */
    eax = MEM32(esi + 0x10);
    _fs18 = (uint32_t)(MEM8(ebx + 0x20)); /* flag operand kept for a later jcc */
    MEM32(ebx + 4) = eax;
    if (CMP_NE(LO8(_fs18), 0)) goto loc_0021777E; /* jne: not equal / not zero */

loc_0021777A: ;
    MEM8(ebx + 1) = MEM8(ebx + 1) & 0xBF;

loc_0021777E: ;
    SET_LO8(ebx, MEM8(ebx + 0x11));
    if (TEST_NZ(LO8(ebx), LO8(ebx))) goto loc_00217793; /* jne: not equal / not zero */

loc_00217785: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 2;
    goto loc_002177A4;

loc_00217793: ;
    if (CMP_NE(LO8(ebx), 2)) goto loc_002177A4; /* jne: not equal / not zero */

loc_00217798: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 4;

loc_002177A4: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00217513
 * Original: 0x00217513 - 0x002177AB (664 bytes, 212 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217513(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0, _fs4 = 0, _fs5 = 0, _fs6 = 0, _fs7 = 0, _fs8 = 0, _fs9 = 0, _fs10 = 0, _fs11 = 0, _fs12 = 0, _fs13 = 0, _fs14 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217513: ;
    PUSH32(esp, MEM32(ebp + -24));
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_0021751B: ;
    esi = eax;
    eax = MEM32(esi + 0x10);
    eax = eax ^ MEM32(ebx + 8);
    eax = eax & 0xF;
    eax = eax ^ MEM32(esi + 0x10);
    MEM32(ebx + 8) = eax;
    /* cmp MEM8(ebx + 0x11), 0 - flags set for next jcc */
    _fs0 = (uint32_t)(MEM8(ebx + 0x11)); /* flag operand kept for a later jcc */
    MEM32(ebp + -4) = esi;
    if (CMP_NE(LO8(_fs0), 0)) goto loc_0021759B; /* jne: not equal / not zero */

loc_00217535: ;
    eax = 0; /* xor self */
    SET_LO8(eax, 0xFE);
    SET_LO8(eax, LO8(eax) - MEM8(ebp + -24));
    PUSH32(esp, eax);
    _fs1 = (uint32_t)(LO8(eax)); /* flag operand kept for a later jcc */
    _fs2 = (uint32_t)(MEM8(ebp + -24)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217542: ;
    ecx = MEM32(edi + 0x28);
    PUSH32(esp, MEM32(ebp + -24));
    MEM32(eax) = ecx;
    ecx = MEM32(edi + 0x2C);
    MEM32(ebp + -36) = eax;
    MEM32(eax + 4) = ecx;
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217558: ;
    esi = eax;
    eax = MEM32(ebp + -4);
    ecx = MEM32(eax);
    ecx = ecx & 0x3FFFF;
    ecx = ecx | 0xE2E00000u;
    MEM32(eax) = ecx;
    ecx = MEM32(ebp + -36);
    edx = MEM32(ecx + 0x10);
    MEM32(eax + 4) = edx;
    edx = MEM32(esi + 0x10);
    MEM32(eax + 8) = edx;
    ecx = MEM32(ecx + 0x10);
    MEM8(eax + 0x1C) = MEM8(eax + 0x1C) & 0;
    ecx = ecx + 7;
    MEM8(eax + 0x1D) = MEM8(eax + 0x1D) & 0;
    _fs3 = (uint32_t)(MEM8(eax + 0x1D)); /* flag operand kept for a later jcc */
    MEM8(ebp + 0xB) = 2;
    MEM32(eax + 0xC) = ecx;
    MEM32(eax + 0x14) = ebx;
    MEM8(eax + 0x1E) = 1;
    MEM32(eax + 0x18) = edi;

loc_0021759B: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002175B7; /* je: equal / zero */

loc_002175A1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, MEM32(ebp + -16));
    PUSH32(esp, MEM32(edi + 0x18));
    _fs4 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    { uint32_t _icall_t = MEM32(0x217B88); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002175AF: ;
    eax = MEM32(edi + 0x18);
    MEM32(ebp + -36) = eax;
    goto loc_002175BB;

loc_002175B7: ;
    MEM8(edi + 0x1C) = 1;

loc_002175BB: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002175C5: ;
    eax = ebp + -20;
    PUSH32(esp, eax);
    edx = ebp + -16;
    ecx = ebp + -36;
    _fs5 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217473(); /* call 0x00217473 */

loc_002175D4: ;
    MEM32(ebp + -4) = eax;

loc_002175D7: ;
    edx = MEM32(ebp + -20);
    eax = MEM32(ebp + -12);
    ecx = MEM32(ebp + -40);
    eax = eax + edx;
    if (CMP_AE(eax, ecx)) goto loc_002175F8; /* jae: above or equal (unsigned >=) */

loc_002175E6: ;
    if (TEST_Z(edx, edx)) goto loc_002176B0; /* je: equal / zero */

loc_002175EE: ;
    if (CMP_NE(MEM32(ebp + -16), 0)) goto loc_002176B0; /* jne: not equal / not zero */

loc_002175F8: ;
    if (CMP_EQ(MEM32(ebp + -12), 0)) goto loc_00217620; /* je: equal / zero */

loc_002175FE: ;
    ecx = ecx - MEM32(ebp + -12);
    eax = MEM32(ebp + -28);
    /* cmp edx, ecx - flags set for next jcc */
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_0021760D; /* jae: above or equal (unsigned >=) */

loc_0021760B: ;
    _fs6 = (uint32_t)(edx); /* flag operand kept for a later jcc */
    _fs7 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    ecx = edx;

loc_0021760D: ;
    SET_LO8(eax, MEM8(ebp + -12));
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    SET_LO8(eax, LO8(eax) + LO8(ecx));
    edx = edx - ecx;
    MEM32(ebp + -12) = MEM32(ebp + -12) & 0;
    MEM8(esi + 0x1D) = LO8(eax);
    goto loc_0021763E;

loc_00217620: ;
    /* cmp edx, ecx - flags set for next jcc */
    eax = MEM32(ebp + -4);
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_00217636; /* jae: above or equal (unsigned >=) */

loc_0021762A: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + edx;
    MEM32(ebp + -20) = MEM32(ebp + -20) & 0;
    MEM8(esi + 0x1D) = LO8(edx);
    goto loc_00217641;

loc_00217636: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    MEM8(esi + 0x1D) = LO8(ecx);
    edx = edx - ecx;

loc_0021763E: ;
    MEM32(ebp + -20) = edx;

loc_00217641: ;
    eax = MEM32(ebp + -4);
    ecx = MEM32(esi);
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) ^ 1;
    MEM8(esi + 0x1C) = MEM8(esi + 0x1C) & 0;
    MEM8(esi + 0x1E) = MEM8(esi + 0x1E) & 0;
    eax--;
    MEM32(esi + 0xC) = eax;
    eax = ZX8(MEM8(ebp + 0xB));
    PUSH32(esp, MEM32(ebp + -24));
    ecx = ecx & 0xFFBFFFF;
    ecx = ecx | 0xE0000000u;
    eax = eax << 0x18;
    eax = eax ^ ecx;
    eax = eax & 0x3000000;
    eax = eax ^ ecx;
    MEM32(esi) = ecx;
    eax = eax | 0xE00000;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    MEM32(esi + 0x14) = ebx;
    SET_LO8(ecx, MEM8(edi + 0x1C));
    eax = eax & 0xF3E7FFFFu;
    MEM32(esi + 0x18) = edi;
    MEM32(ebp + -8) = esi;
    ecx = ecx & 3;
    ecx = ecx << 0x13;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    _fs8 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs9 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_002176A0: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_002175D7;

loc_002176B0: ;
    /* cmp MEM32(ebp + -16), 0 - flags set for next jcc */
    eax = MEM32(ebp + -4);
    _fs10 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -12) = edx;
    if (CMP_NE(_fs10, 0)) goto loc_002175C5; /* jne: not equal / not zero */

loc_002176C3: ;
    if (CMP_EQ(MEM32(ebp + -8), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176C9: ;
    if (CMP_EQ(MEM8(edi + 0x1D), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176CF: ;
    eax = MEM32(ebp + -8);
    MEM8(eax + 2) = MEM8(eax + 2) | 4;

loc_002176D6: ;
    if (CMP_NE(MEM8(ebx + 0x11), 0)) goto loc_00217746; /* jne: not equal / not zero */

loc_002176DC: ;
    MEM8(esi + 2) = MEM8(esi + 2) & 0xFB;
    ecx = MEM32(esi);
    eax = 0; /* xor self */
    /* cmp MEM8(edi + 0x1C), 2 - flags set for next jcc */
    _fs11 = (uint32_t)(MEM8(edi + 0x1C)); /* flag operand kept for a later jcc */
    PUSH32(esp, MEM32(ebp + -24));
    SET_LO8(eax, (CMP_NE(LO8(_fs11), 2)) ? 1 : 0); /* setne */
    MEM32(ebp + -8) = esi;
    eax++;
    eax = eax << 0x13;
    eax = eax ^ ecx;
    eax = eax & 0x180000;
    eax = eax ^ ecx;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    SET_LO8(ecx, MEM8(edi + 0x1E));
    MEM32(esi + 4) = MEM32(esi + 4) & 0;
    MEM32(esi + 0xC) = MEM32(esi + 0xC) & 0;
    MEM8(esi + 0x1D) = MEM8(esi + 0x1D) & 0;
    eax = eax & 0x1FFFFF;
    MEM32(esi + 0x14) = ebx;
    MEM8(esi + 0x1C) = 2;
    ecx = ecx & 7;
    ecx = ecx | 0xFFFFFF18u;
    ecx = ecx << 0x15;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    MEM8(esi + 0x1E) = 2;
    MEM32(esi + 0x18) = edi;
    _fs12 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs13 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217739: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_0021775E;

loc_00217746: ;
    ecx = ZX8(MEM8(edi + 0x1E));
    eax = MEM32(ebp + -8);
    ecx = ecx << 0x15;
    ecx = ecx ^ MEM32(eax);
    MEM8(eax + 0x1C) = 2;
    ecx = ecx & 0xE00000;
    MEM32(eax) = MEM32(eax) ^ ecx;

loc_0021775E: ;
    MEM8(esi + 0x1E) = 3;
    SET_LO16(eax, MEM16(edi + 0x14));
    MEM32(edi + 0x14) = MEM32(edi + 0x14) & 0;
    MEM16(edi + 0x20) = LO16(eax);
    /* cmp MEM8(ebx + 0x20), 0 - flags set for next jcc */
    eax = MEM32(esi + 0x10);
    _fs14 = (uint32_t)(MEM8(ebx + 0x20)); /* flag operand kept for a later jcc */
    MEM32(ebx + 4) = eax;
    if (CMP_NE(LO8(_fs14), 0)) goto loc_0021777E; /* jne: not equal / not zero */

loc_0021777A: ;
    MEM8(ebx + 1) = MEM8(ebx + 1) & 0xBF;

loc_0021777E: ;
    SET_LO8(ebx, MEM8(ebx + 0x11));
    if (TEST_NZ(LO8(ebx), LO8(ebx))) goto loc_00217793; /* jne: not equal / not zero */

loc_00217785: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 2;
    goto loc_002177A4;

loc_00217793: ;
    if (CMP_NE(LO8(ebx), 2)) goto loc_002177A4; /* jne: not equal / not zero */

loc_00217798: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 4;

loc_002177A4: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

}

/**
 * sub_0021752C
 * Original: 0x0021752C - 0x002177AB (639 bytes, 204 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021752C(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0, _fs4 = 0, _fs5 = 0, _fs6 = 0, _fs7 = 0, _fs8 = 0, _fs9 = 0, _fs10 = 0, _fs11 = 0, _fs12 = 0, _fs13 = 0, _fs14 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021752C: ;
    /* cmp MEM8(ebx + 0x11), 0 - flags set for next jcc */
    _fs0 = (uint32_t)(MEM8(ebx + 0x11)); /* flag operand kept for a later jcc */
    MEM32(ebp + -4) = esi;
    if (CMP_NE(LO8(_fs0), 0)) goto loc_0021759B; /* jne: not equal / not zero */

loc_00217535: ;
    eax = 0; /* xor self */
    SET_LO8(eax, 0xFE);
    SET_LO8(eax, LO8(eax) - MEM8(ebp + -24));
    PUSH32(esp, eax);
    _fs1 = (uint32_t)(LO8(eax)); /* flag operand kept for a later jcc */
    _fs2 = (uint32_t)(MEM8(ebp + -24)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217542: ;
    ecx = MEM32(edi + 0x28);
    PUSH32(esp, MEM32(ebp + -24));
    MEM32(eax) = ecx;
    ecx = MEM32(edi + 0x2C);
    MEM32(ebp + -36) = eax;
    MEM32(eax + 4) = ecx;
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217558: ;
    esi = eax;
    eax = MEM32(ebp + -4);
    ecx = MEM32(eax);
    ecx = ecx & 0x3FFFF;
    ecx = ecx | 0xE2E00000u;
    MEM32(eax) = ecx;
    ecx = MEM32(ebp + -36);
    edx = MEM32(ecx + 0x10);
    MEM32(eax + 4) = edx;
    edx = MEM32(esi + 0x10);
    MEM32(eax + 8) = edx;
    ecx = MEM32(ecx + 0x10);
    MEM8(eax + 0x1C) = MEM8(eax + 0x1C) & 0;
    ecx = ecx + 7;
    MEM8(eax + 0x1D) = MEM8(eax + 0x1D) & 0;
    _fs3 = (uint32_t)(MEM8(eax + 0x1D)); /* flag operand kept for a later jcc */
    MEM8(ebp + 0xB) = 2;
    MEM32(eax + 0xC) = ecx;
    MEM32(eax + 0x14) = ebx;
    MEM8(eax + 0x1E) = 1;
    MEM32(eax + 0x18) = edi;

loc_0021759B: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002175B7; /* je: equal / zero */

loc_002175A1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, MEM32(ebp + -16));
    PUSH32(esp, MEM32(edi + 0x18));
    _fs4 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    { uint32_t _icall_t = MEM32(0x217B88); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002175AF: ;
    eax = MEM32(edi + 0x18);
    MEM32(ebp + -36) = eax;
    goto loc_002175BB;

loc_002175B7: ;
    MEM8(edi + 0x1C) = 1;

loc_002175BB: ;
    if (CMP_EQ(MEM32(ebp + -16), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002175C5: ;
    eax = ebp + -20;
    PUSH32(esp, eax);
    edx = ebp + -16;
    ecx = ebp + -36;
    _fs5 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217473(); /* call 0x00217473 */

loc_002175D4: ;
    MEM32(ebp + -4) = eax;

loc_002175D7: ;
    edx = MEM32(ebp + -20);
    eax = MEM32(ebp + -12);
    ecx = MEM32(ebp + -40);
    eax = eax + edx;
    if (CMP_AE(eax, ecx)) goto loc_002175F8; /* jae: above or equal (unsigned >=) */

loc_002175E6: ;
    if (TEST_Z(edx, edx)) goto loc_002176B0; /* je: equal / zero */

loc_002175EE: ;
    if (CMP_NE(MEM32(ebp + -16), 0)) goto loc_002176B0; /* jne: not equal / not zero */

loc_002175F8: ;
    if (CMP_EQ(MEM32(ebp + -12), 0)) goto loc_00217620; /* je: equal / zero */

loc_002175FE: ;
    ecx = ecx - MEM32(ebp + -12);
    eax = MEM32(ebp + -28);
    /* cmp edx, ecx - flags set for next jcc */
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_0021760D; /* jae: above or equal (unsigned >=) */

loc_0021760B: ;
    _fs6 = (uint32_t)(edx); /* flag operand kept for a later jcc */
    _fs7 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    ecx = edx;

loc_0021760D: ;
    SET_LO8(eax, MEM8(ebp + -12));
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    SET_LO8(eax, LO8(eax) + LO8(ecx));
    edx = edx - ecx;
    MEM32(ebp + -12) = MEM32(ebp + -12) & 0;
    MEM8(esi + 0x1D) = LO8(eax);
    goto loc_0021763E;

loc_00217620: ;
    /* cmp edx, ecx - flags set for next jcc */
    eax = MEM32(ebp + -4);
    MEM32(esi + 4) = eax;
    if (CMP_AE(edx, ecx)) goto loc_00217636; /* jae: above or equal (unsigned >=) */

loc_0021762A: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + edx;
    MEM32(ebp + -20) = MEM32(ebp + -20) & 0;
    MEM8(esi + 0x1D) = LO8(edx);
    goto loc_00217641;

loc_00217636: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) + ecx;
    MEM8(esi + 0x1D) = LO8(ecx);
    edx = edx - ecx;

loc_0021763E: ;
    MEM32(ebp + -20) = edx;

loc_00217641: ;
    eax = MEM32(ebp + -4);
    ecx = MEM32(esi);
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) ^ 1;
    MEM8(esi + 0x1C) = MEM8(esi + 0x1C) & 0;
    MEM8(esi + 0x1E) = MEM8(esi + 0x1E) & 0;
    eax--;
    MEM32(esi + 0xC) = eax;
    eax = ZX8(MEM8(ebp + 0xB));
    PUSH32(esp, MEM32(ebp + -24));
    ecx = ecx & 0xFFBFFFF;
    ecx = ecx | 0xE0000000u;
    eax = eax << 0x18;
    eax = eax ^ ecx;
    eax = eax & 0x3000000;
    eax = eax ^ ecx;
    MEM32(esi) = ecx;
    eax = eax | 0xE00000;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    MEM32(esi + 0x14) = ebx;
    SET_LO8(ecx, MEM8(edi + 0x1C));
    eax = eax & 0xF3E7FFFFu;
    MEM32(esi + 0x18) = edi;
    MEM32(ebp + -8) = esi;
    ecx = ecx & 3;
    ecx = ecx << 0x13;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    _fs8 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs9 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_002176A0: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_002175D7;

loc_002176B0: ;
    /* cmp MEM32(ebp + -16), 0 - flags set for next jcc */
    eax = MEM32(ebp + -4);
    _fs10 = (uint32_t)(MEM32(ebp + -16)); /* flag operand kept for a later jcc */
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -12) = edx;
    if (CMP_NE(_fs10, 0)) goto loc_002175C5; /* jne: not equal / not zero */

loc_002176C3: ;
    if (CMP_EQ(MEM32(ebp + -8), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176C9: ;
    if (CMP_EQ(MEM8(edi + 0x1D), 0)) goto loc_002176D6; /* je: equal / zero */

loc_002176CF: ;
    eax = MEM32(ebp + -8);
    MEM8(eax + 2) = MEM8(eax + 2) | 4;

loc_002176D6: ;
    if (CMP_NE(MEM8(ebx + 0x11), 0)) goto loc_00217746; /* jne: not equal / not zero */

loc_002176DC: ;
    MEM8(esi + 2) = MEM8(esi + 2) & 0xFB;
    ecx = MEM32(esi);
    eax = 0; /* xor self */
    /* cmp MEM8(edi + 0x1C), 2 - flags set for next jcc */
    _fs11 = (uint32_t)(MEM8(edi + 0x1C)); /* flag operand kept for a later jcc */
    PUSH32(esp, MEM32(ebp + -24));
    SET_LO8(eax, (CMP_NE(LO8(_fs11), 2)) ? 1 : 0); /* setne */
    MEM32(ebp + -8) = esi;
    eax++;
    eax = eax << 0x13;
    eax = eax ^ ecx;
    eax = eax & 0x180000;
    eax = eax ^ ecx;
    ecx = 0; /* xor self */
    MEM32(esi) = eax;
    SET_LO8(ecx, MEM8(edi + 0x1E));
    MEM32(esi + 4) = MEM32(esi + 4) & 0;
    MEM32(esi + 0xC) = MEM32(esi + 0xC) & 0;
    MEM8(esi + 0x1D) = MEM8(esi + 0x1D) & 0;
    eax = eax & 0x1FFFFF;
    MEM32(esi + 0x14) = ebx;
    MEM8(esi + 0x1C) = 2;
    ecx = ecx & 7;
    ecx = ecx | 0xFFFFFF18u;
    ecx = ecx << 0x15;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    MEM8(esi + 0x1E) = 2;
    MEM32(esi + 0x18) = edi;
    _fs12 = (uint32_t)(ecx); /* flag operand kept for a later jcc */
    _fs13 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002173E2(); /* call 0x002173E2 */

loc_00217739: ;
    ecx = MEM32(ebp + -8);
    esi = eax;
    eax = MEM32(esi + 0x10);
    MEM32(ecx + 8) = eax;
    goto loc_0021775E;

loc_00217746: ;
    ecx = ZX8(MEM8(edi + 0x1E));
    eax = MEM32(ebp + -8);
    ecx = ecx << 0x15;
    ecx = ecx ^ MEM32(eax);
    MEM8(eax + 0x1C) = 2;
    ecx = ecx & 0xE00000;
    MEM32(eax) = MEM32(eax) ^ ecx;

loc_0021775E: ;
    MEM8(esi + 0x1E) = 3;
    SET_LO16(eax, MEM16(edi + 0x14));
    MEM32(edi + 0x14) = MEM32(edi + 0x14) & 0;
    MEM16(edi + 0x20) = LO16(eax);
    /* cmp MEM8(ebx + 0x20), 0 - flags set for next jcc */
    eax = MEM32(esi + 0x10);
    _fs14 = (uint32_t)(MEM8(ebx + 0x20)); /* flag operand kept for a later jcc */
    MEM32(ebx + 4) = eax;
    if (CMP_NE(LO8(_fs14), 0)) goto loc_0021777E; /* jne: not equal / not zero */

loc_0021777A: ;
    MEM8(ebx + 1) = MEM8(ebx + 1) & 0xBF;

loc_0021777E: ;
    SET_LO8(ebx, MEM8(ebx + 0x11));
    if (TEST_NZ(LO8(ebx), LO8(ebx))) goto loc_00217793; /* jne: not equal / not zero */

loc_00217785: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 2;
    goto loc_002177A4;

loc_00217793: ;
    if (CMP_NE(LO8(ebx), 2)) goto loc_002177A4; /* jne: not equal / not zero */

loc_00217798: ;
    eax = MEM32(ebp + -32);
    eax = MEM32(eax);
    MEM32(eax + 8) = 4;

loc_002177A4: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002177AB
 * Original: 0x002177AB - 0x002177FE (83 bytes, 34 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002177AB_gen(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002177AB: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    esi = edx;
    /* cmp MEM32(esi + 0x28), 0 - flags set for next jcc */
    _fs0 = (uint32_t)(MEM32(esi + 0x28)); /* flag operand kept for a later jcc */
    MEM32(ebp + -4) = ecx;
    if (CMP_EQ(_fs0, 0)) goto loc_002177FB; /* je: equal / zero */

loc_002177BB: ;
    PUSH32(esp, ebx);

loc_002177BC: ;
    eax = MEM32(esi + 0x28);
    SET_LO16(edx, MEM16(esi + 0x24));
    ebx = ZX16(MEM16(eax + 0x20));
    ecx = ZX16(LO16(edx));
    ecx = ecx + ebx;
    if (CMP_G(ecx, 3)) goto loc_002177FA; /* jg: greater (signed >) */

loc_002177D1: ;
    ecx = MEM32(eax + 0x24);
    /* test ecx, ecx - flags set for next jcc */
    MEM32(esi + 0x28) = ecx;
    if (TEST_NZ(ecx, ecx)) goto loc_002177DE; /* jne: not equal / not zero */

loc_002177DB: ;
    MEM32(esi + 0x2C) = MEM32(esi + 0x2C) & ecx;

loc_002177DE: ;
    SET_LO16(ecx, MEM16(eax + 0x20));
    SET_LO16(ecx, LO16(ecx) + LO16(edx));
    MEM16(esi + 0x24) = LO16(ecx);
    ecx = MEM32(ebp + -4);
    PUSH32(esp, eax);
    edx = esi;
    _fs1 = (uint32_t)(LO16(ecx)); /* flag operand kept for a later jcc */
    _fs2 = (uint32_t)(LO16(edx)); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002174AD(); /* call 0x002174AD */

loc_002177F4: ;
    if (CMP_NE(MEM32(esi + 0x28), 0)) goto loc_002177BC; /* jne: not equal / not zero */

loc_002177FA: ;
    POP32(esp, ebx);

loc_002177FB: ;
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002177FE
 * Original: 0x002177FE - 0x0021784B (77 bytes, 26 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002177FE_gen(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_002177FE: ;
    PUSH32(esp, esi);
    esi = ecx;
    if (CMP_EQ(MEM32(esi + 0x424), 0)) goto loc_00217849; /* je: equal / zero */

loc_0021780A: ;
    _fs0 = (uint32_t)(MEM32(esi + 0x424)); /* flag operand kept for a later jcc */
    PUSH32(esp, edi);

loc_0021780B: ;
    edi = MEM32(esi + 0x424);
    eax = 0; /* xor self */
    SET_LO16(eax, MEM16(edi + 0x20));
    PUSH32(esp, eax);
    _fs1 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217421(); /* call 0x00217421 */

loc_0021781D: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00217848; /* je: equal / zero */

loc_00217821: ;
    eax = MEM32(edi + 0x24);
    /* test eax, eax - flags set for next jcc */
    MEM32(esi + 0x424) = eax;
    if (TEST_NZ(eax, eax)) goto loc_00217834; /* jne: not equal / not zero */

loc_0021782E: ;
    MEM32(esi + 0x428) = MEM32(esi + 0x428) & eax;

loc_00217834: ;
    edx = MEM32(edi + 0x10);
    PUSH32(esp, edi);
    ecx = esi;
    _fs2 = (uint32_t)(MEM32(esi + 0x428)); /* flag operand kept for a later jcc */
    _fs3 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002174AD(); /* call 0x002174AD */

loc_0021783F: ;
    if (CMP_NE(MEM32(esi + 0x424), 0)) goto loc_0021780B; /* jne: not equal / not zero */

loc_00217848: ;
    POP32(esp, edi);

loc_00217849: ;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_0021784B
 * Original: 0x0021784B - 0x00217898 (77 bytes, 26 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021784B_gen(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_0021784B: ;
    PUSH32(esp, esi);
    esi = ecx;
    if (CMP_EQ(MEM32(esi + 0x41C), 0)) goto loc_00217896; /* je: equal / zero */

loc_00217857: ;
    _fs0 = (uint32_t)(MEM32(esi + 0x41C)); /* flag operand kept for a later jcc */
    PUSH32(esp, edi);

loc_00217858: ;
    edi = MEM32(esi + 0x41C);
    eax = 0; /* xor self */
    SET_LO16(eax, MEM16(edi + 0x20));
    PUSH32(esp, eax);
    _fs1 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217402(); /* call 0x00217402 */

loc_0021786A: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00217895; /* je: equal / zero */

loc_0021786E: ;
    eax = MEM32(edi + 0x24);
    /* test eax, eax - flags set for next jcc */
    MEM32(esi + 0x41C) = eax;
    if (TEST_NZ(eax, eax)) goto loc_00217881; /* jne: not equal / not zero */

loc_0021787B: ;
    MEM32(esi + 0x420) = MEM32(esi + 0x420) & eax;

loc_00217881: ;
    edx = MEM32(edi + 0x10);
    PUSH32(esp, edi);
    ecx = esi;
    _fs2 = (uint32_t)(MEM32(esi + 0x420)); /* flag operand kept for a later jcc */
    _fs3 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_002174AD(); /* call 0x002174AD */

loc_0021788C: ;
    if (CMP_NE(MEM32(esi + 0x41C), 0)) goto loc_00217858; /* jne: not equal / not zero */

loc_00217895: ;
    POP32(esp, edi);

loc_00217896: ;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_00217898
 * Original: 0x00217898 - 0x002178CB (51 bytes, 17 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217898_gen(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_00217898: ;
    eax = MEM32(esp + 4);
    if (CMP_BE(MEM16(eax + 0x20), 3)) goto loc_002178AA; /* jbe: below or equal (unsigned <=) */

loc_002178A3: ;
    eax = 0x80000500u;
    goto loc_002178C8;

loc_002178AA: ;
    PUSH32(esp, esi);
    esi = MEM32(edx + 0x2C);
    if (TEST_Z(esi, esi)) goto loc_002178B7; /* je: equal / zero */

loc_002178B2: ;
    MEM32(esi + 0x24) = eax;
    goto loc_002178BA;

loc_002178B7: ;
    MEM32(edx + 0x28) = eax;

loc_002178BA: ;
    MEM32(edx + 0x2C) = eax;
    PUSH32(esp, 0); sub_002177AB(); /* call 0x002177AB */

loc_002178C2: ;
    eax = 0x40000000;
    _fs0 = (uint32_t)(esi); /* flag operand kept for a later jcc */
    POP32(esp, esi);

loc_002178C8: ;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002178AA
 * Original: 0x002178AA - 0x002178CB (33 bytes, 12 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002178AA(void)
{
    int _flags = 0; /* fallback flag var */

loc_002178AA: ;
    PUSH32(esp, esi);
    esi = MEM32(edx + 0x2C);
    if (TEST_Z(esi, esi)) goto loc_002178B7; /* je: equal / zero */

loc_002178B2: ;
    MEM32(esi + 0x24) = eax;
    goto loc_002178BA;

loc_002178B7: ;
    MEM32(edx + 0x28) = eax;

loc_002178BA: ;
    MEM32(edx + 0x2C) = eax;
    PUSH32(esp, 0); sub_002177AB(); /* call 0x002177AB */

loc_002178C2: ;
    eax = 0x40000000;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002178C8
 * Original: 0x002178C8 - 0x002178CB (3 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002178C8(void)
{

loc_002178C8: ;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002178CB
 * Original: 0x002178CB - 0x00217907 (60 bytes, 16 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002178CB_gen(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_002178CB: ;
    SET_LO16(eax, MEM16(edx + 0x20));
    if (CMP_BE(LO16(eax), MEM16(0xC6B9C4))) goto loc_002178DE; /* jbe: below or equal (unsigned <=) */

loc_002178D8: ;
    eax = 0x80000500u;
    esp += 4; return; /* ret */

loc_002178DE: ;
    eax = ecx + 0x424;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_002178F4; /* je: equal / zero */

loc_002178E9: ;
    eax = MEM32(ecx + 0x428);
    MEM32(eax + 0x24) = edx;
    goto loc_002178F6;

loc_002178F4: ;
    _fs0 = (uint32_t)(MEM32(eax)); /* flag operand kept for a later jcc */
    MEM32(eax) = edx;

loc_002178F6: ;
    MEM32(ecx + 0x428) = edx;
    PUSH32(esp, 0); sub_002177FE(); /* call 0x002177FE */

loc_00217901: ;
    eax = 0x40000000;
    esp += 4; return; /* ret */

}

/**
 * sub_002178DE
 * Original: 0x002178DE - 0x00217907 (41 bytes, 11 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002178DE(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_002178DE: ;
    eax = ecx + 0x424;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_002178F4; /* je: equal / zero */

loc_002178E9: ;
    eax = MEM32(ecx + 0x428);
    MEM32(eax + 0x24) = edx;
    goto loc_002178F6;

loc_002178F4: ;
    _fs0 = (uint32_t)(MEM32(eax)); /* flag operand kept for a later jcc */
    MEM32(eax) = edx;

loc_002178F6: ;
    MEM32(ecx + 0x428) = edx;
    PUSH32(esp, 0); sub_002177FE(); /* call 0x002177FE */

loc_00217901: ;
    eax = 0x40000000;
    esp += 4; return; /* ret */

}

/**
 * sub_00217907
 * Original: 0x00217907 - 0x00217943 (60 bytes, 16 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217907_gen(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_00217907: ;
    SET_LO16(eax, MEM16(edx + 0x20));
    if (CMP_BE(LO16(eax), MEM16(0xC6B9C0))) goto loc_0021791A; /* jbe: below or equal (unsigned <=) */

loc_00217914: ;
    eax = 0x80000500u;
    esp += 4; return; /* ret */

loc_0021791A: ;
    eax = ecx + 0x41C;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_00217930; /* je: equal / zero */

loc_00217925: ;
    eax = MEM32(ecx + 0x420);
    MEM32(eax + 0x24) = edx;
    goto loc_00217932;

loc_00217930: ;
    _fs0 = (uint32_t)(MEM32(eax)); /* flag operand kept for a later jcc */
    MEM32(eax) = edx;

loc_00217932: ;
    MEM32(ecx + 0x420) = edx;
    PUSH32(esp, 0); sub_0021784B(); /* call 0x0021784B */

loc_0021793D: ;
    eax = 0x40000000;
    esp += 4; return; /* ret */

}

/**
 * sub_0021791A
 * Original: 0x0021791A - 0x00217943 (41 bytes, 11 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021791A(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    int _flags = 0; /* fallback flag var */

loc_0021791A: ;
    eax = ecx + 0x41C;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_00217930; /* je: equal / zero */

loc_00217925: ;
    eax = MEM32(ecx + 0x420);
    MEM32(eax + 0x24) = edx;
    goto loc_00217932;

loc_00217930: ;
    _fs0 = (uint32_t)(MEM32(eax)); /* flag operand kept for a later jcc */
    MEM32(eax) = edx;

loc_00217932: ;
    MEM32(ecx + 0x420) = edx;
    PUSH32(esp, 0); sub_0021784B(); /* call 0x0021784B */

loc_0021793D: ;
    eax = 0x40000000;
    esp += 4; return; /* ret */

}

/**
 * sub_00217943
 * Original: 0x00217943 - 0x002179C9 (134 bytes, 53 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217943_gen(void)
{
    uint32_t _fs0 = 0, _fs1 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00217943: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = edx;
    PUSH32(esp, edi);
    edi = MEM32(esi + 0x10);
    ebx = ecx;
    edx = edi;
    ecx = esi;
    PUSH32(esp, 0); sub_00217440(); /* call 0x00217440 */

loc_0021795A: ;
    MEM16(esi + 0x20) = LO16(eax);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217964: ;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) + 1;
    MEM32(esi + 0x24) = MEM32(esi + 0x24) & 0;
    MEM8(ebp + -1) = LO8(eax);
    MEM16(esi + 0x22) = 2;
    eax = ZX8(MEM8(edi + 0x11));
    eax = eax - 0;
    if ((eax == 0)) goto loc_002179A2; /* je: equal / zero */

loc_0021797D: ;
    eax--;
    eax--;
    if ((eax == 0)) goto loc_00217997; /* je: equal / zero */

loc_00217981: ;
    eax--;
    if ((eax == 0)) goto loc_0021798B; /* je: equal / zero */

loc_00217984: ;
    ebx = 0x80000600u;
    goto loc_002179B1;

loc_0021798B: ;
    PUSH32(esp, esi);
    edx = edi;
    ecx = ebx;
    _fs0 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    PUSH32(esp, 0); sub_00217898(); /* call 0x00217898 */

loc_00217995: ;
    goto loc_002179AB;

loc_00217997: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_002178CB(); /* call 0x002178CB */

loc_002179A0: ;
    goto loc_002179AB;

loc_002179A2: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_00217907(); /* call 0x00217907 */

loc_002179AB: ;
    ebx = eax;
    if (CMP_GE(ebx & ebx, 0)) goto loc_002179B9; /* jge: greater or equal (signed >=) */

loc_002179B1: ;
    MEM16(esi + 0x22) = MEM16(esi + 0x22) & 0;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;

loc_002179B9: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    _fs1 = (uint32_t)(MEM8(edi + 0x26)); /* flag operand kept for a later jcc */
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002179C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002179A2
 * Original: 0x002179A2 - 0x002179C9 (39 bytes, 16 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179A2(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179A2: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_00217907(); /* call 0x00217907 */

loc_002179AB: ;
    ebx = eax;
    if (CMP_GE(ebx & ebx, 0)) goto loc_002179B9; /* jge: greater or equal (signed >=) */

loc_002179B1: ;
    MEM16(esi + 0x22) = MEM16(esi + 0x22) & 0;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;

loc_002179B9: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    _fs0 = (uint32_t)(MEM8(edi + 0x26)); /* flag operand kept for a later jcc */
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002179C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002179AB
 * Original: 0x002179AB - 0x002179C9 (30 bytes, 13 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179AB(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179AB: ;
    ebx = eax;
    if (CMP_GE(ebx & ebx, 0)) goto loc_002179B9; /* jge: greater or equal (signed >=) */

loc_002179B1: ;
    MEM16(esi + 0x22) = MEM16(esi + 0x22) & 0;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;

loc_002179B9: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    _fs0 = (uint32_t)(MEM8(edi + 0x26)); /* flag operand kept for a later jcc */
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002179C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002179B1
 * Original: 0x002179B1 - 0x002179CC (27 bytes, 13 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179B1(void)
{
    uint32_t _fs0 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179B1: ;
    MEM16(esi + 0x22) = MEM16(esi + 0x22) & 0;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;
    SET_LO8(ecx, MEM8(ebp + -1));
    _fs0 = (uint32_t)(MEM8(edi + 0x26)); /* flag operand kept for a later jcc */
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002179C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002179B9
 * Original: 0x002179B9 - 0x002179CC (19 bytes, 11 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179B9(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179B9: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002179C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002179CC
 * Original: 0x002179CC - 0x002179FD (49 bytes, 31 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179CC(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179CC: ;
    POP32(esp, esp);
    esp++;
    if ((esp == 0) /* CF taken as 0 */) { g_seh_ebp = ebp; sub_00217A3A(); return; } /* jbe: below or equal (unsigned <=) */

loc_002179D1: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    ebp--;
    PUSH32(esp, ebp);
    POP32(esp, edi);
    MEM8(eax) = MEM8(eax) ^ LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(ecx) = MEM8(ecx) + HI8(ebx);
    eax = eax ^ 0x45304631;
    esi++;
    esi = esi ^ MEM32(eax);
    esp++;
    ebx++;
    SET_LO8(eax, LO8(eax) ^ 0x36);
    /* cmp MEM32(edi + 0x43), ebx - flags set for next jcc */
    edi--;
    PUSH32(esp, edx);
    PUSH32(esp, edx);
    PUSH32(esp, ebp);
    PUSH32(esp, eax);
    PUSH32(esp, esp);
    POP32(esp, edi);
    PUSH32(esp, ebx);
    ebp++;
    ebx++;
    PUSH32(esp, esp);
    edi--;
    PUSH32(esp, edx);
    MEM8(eax) = MEM8(eax) + LO8(eax);

}

/**
 * sub_00217A3A
 * Original: 0x00217A3A - 0x00252050 (239126 bytes, 324 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217A3A(void)
{
    uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0; /* flag operands kept for a later jcc */
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    int _cf = 0; /* carry flag */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217A3A: ;
    MEM8(eax + -2147483448) = MEM8(eax + -2147483448) + LO8(eax);
    /* TODO: les eax, ptr [eax] */
    MEM8(eax + -2147483470) = MEM8(eax + -2147483470) + LO8(eax);
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax + eax + 0xAB8000) = MEM8(eax + eax + 0xAB8000) & 0;
    MEM8(0x31800001) = MEM8(0x31800001) - 0;
    MEM8(eax + -2147483433) = MEM8(eax + -2147483433) + LO8(eax);
    esp += 4; return; /* ret */

loc_00217ABA: ;
    MEM8(eax + -2147483414) = MEM8(eax + -2147483414) + LO8(eax);
    /* TODO: arpl word ptr [eax], ax */
    MEM8(eax + -2147483354) = MEM8(eax + -2147483354) + LO8(eax);
    _cf = ((uint32_t)(MEM8(eax + -2147483354)) < (uint32_t)(LO8(eax))); /* CF from add */
    eax = eax + 0xFA800001u + _cf; /* adc */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(edi + -159383552) = MEM8(edi + -159383552) | 0;
    MEM8(eax + -2147483389) = MEM8(eax + -2147483389) + LO8(eax);
    if (((int8_t)MEM8(eax + -2147483389) < 0)) goto loc_00217ADE; /* jl: less (signed <) */

loc_00217ADE: ;
    MEM8(eax + -2147483504) = MEM8(eax + -2147483504) + LO8(eax);
    /* TODO: out 0, eax */
    MEM8(eax + -2147483424) = MEM8(eax + -2147483424) + LO8(eax);
    MEM32(eax) = MEM32(eax) + eax;
    SET_HI8(edx, HI8(edx) - 0);
    MEM8(eax + -2147483390) = MEM8(eax + -2147483390) + LO8(eax);
    MEM32(eax) = MEM32(eax) + 1;
    MEM8(eax + -2147483487) = MEM8(eax + -2147483487) + LO8(eax);
    MEM32(eax) = MEM32(eax) + 0x418000;
    MEM8(eax + -2147483634) = MEM8(eax + -2147483634) + LO8(eax);
    { uint32_t _tmp = ecx;
    ecx = eax;
    eax = _tmp; }
    MEM8(eax) = MEM8(eax) + LO8(eax);
    _cf = ((uint32_t)(MEM8(eax)) < (uint32_t)(LO8(eax))); /* CF from add */
    MEM8(edi + 0x71800000) = MEM8(edi + 0x71800000) - 0 - _cf; /* sbb */
    MEM8(eax + -2147483541) = MEM8(eax + -2147483541) + LO8(eax);
    edx--;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    _cf = ((uint32_t)(MEM8(eax)) < (uint32_t)(LO8(eax))); /* CF from add */
    MEM8(edi) = MEM8(edi) - 0 - _cf; /* sbb */
    MEM8(ecx + -1434451968) = MEM8(ecx + -1434451968) - 0;
    MEM8(eax + -2147483499) = MEM8(eax + -2147483499) + LO8(eax);
    /* TODO: popal  */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(0x23800001) = MEM8(0x23800001) | 1;
    MEM8(eax + -2147483631) = MEM8(eax + -2147483631) + LO8(eax);
    PUSH32(esp, ebx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    _cf = ((uint32_t)(MEM8(eax)) < (uint32_t)(LO8(eax))); /* CF from add */
    MEM8(edi) = MEM8(edi) + 0 + _cf; /* adc */
    MEM8(ecx) = MEM8(ecx) + 0 + _cf; /* adc */
    MEM8(edi + 1) = MEM8(edi + 1) & 0;
    MEM8(esi + 1) = MEM8(esi + 1) & 0;
    /* cmp MEM8(ebp), 0 - flags set for next jcc */
    MEM8(edx + 1) = MEM8(edx + 1) + 0;
    MEM8(eax + 1) = MEM8(eax + 1) - 0;
    _cf = ((uint32_t)((MEM8(eax + 1)) + (0)) < (uint32_t)(0)); /* CF from sub */
    MEM8(edi) = MEM8(edi) + 1 + _cf; /* adc */
    MEM8(eax + -2147483321) = MEM8(eax + -2147483321) + LO8(eax);
    eax--;
    MEM32(eax) = MEM32(eax) + eax;
    MEM8(eax + eax) = MEM8(eax + eax) - 0;
    MEM8(eax + 0x2F800000) = MEM8(eax + 0x2F800000) & 0;
    MEM8(eax + -2147483550) = MEM8(eax + -2147483550) + LO8(eax);
    /* TODO: insd dword ptr es:[edi], dx */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    _cf = ((uint32_t)(MEM8(eax)) < (uint32_t)(LO8(eax))); /* CF from add */
    MEM8(edi + -1384120320) = MEM8(edi + -1384120320) + 0 + _cf; /* adc */
    MEM8(eax + -2147483473) = MEM8(eax + -2147483473) + LO8(eax);
    if (((uint32_t)(MEM8(eax + -2147483473)) >= (uint32_t)(LO8(eax)) && (MEM8(eax + -2147483473)) != 0)) goto loc_00217B8E; /* ja: above (unsigned >) */

loc_00217B8E: ;
    MEM8(eax + -2147483472) = MEM8(eax + -2147483472) + LO8(eax);
    MEM32(eax) = MEM32(eax) + eax;
    MEM8(eax + -2147483646) = MEM8(eax + -2147483646) + LO8(eax);
    eax = eax + MEM32(eax);
    MEM8(eax + -2147483644) = MEM8(eax + -2147483644) + LO8(eax);
    MEM8(eax) = MEM8(eax) | LO8(eax);
    MEM8(eax + -2147483602) = MEM8(eax + -2147483602) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax + -1904214016) = MEM8(eax + -1904214016) - 0;
    MEM8(eax + -2147483509) = MEM8(eax + -2147483509) + LO8(eax);
    { uint32_t _tmp; POP32(esp, _tmp); } /* pop ss - segment register */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(edi) = MEM8(edi) | 0;
    MEM8(eax + -2147483511) = MEM8(eax + -2147483511) + LO8(eax);
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    /* cmp MEM8(eax), 1 - flags set for next jcc */
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    PUSH32(esp, 0 /* seg:es */);
    MEM8(edi) = MEM8(edi) + LO8(eax);
    SET_HI8(eax, HI8(eax) + HI8(edx));
    /* unhandled flags: add -> jnp */
    if (_flags /* jnp: not parity */) (void)0; /* goto loc_00217C00 - dead code, label not in function */

loc_00217BDF: ;
    MEM8(esp + eax * 2 + 0x65) = MEM8(esp + eax * 2 + 0x65) + LO8(ebx);
    if (((uint32_t)(MEM8(esp + eax * 2 + 0x65)) < (uint32_t)(LO8(ebx)) || (MEM8(esp + eax * 2 + 0x65)) == 0)) (void)0; /* goto loc_00217C4E - dead code, label not in function */ /* jbe: below or equal (unsigned <=) */

loc_00217BE5: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    ebx++;
    /* unhandled flags: inc -> jb */
    if (_flags /* jb: below (unsigned <) */) goto loc_00217C5B;

loc_00217BEC: ;
    /* TODO: insd dword ptr es:[edi], dx */
    MEM8(eax) = MEM8(eax) ^ LO8(eax);
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(edi + edi + 0x3F) = MEM8(edi + edi + 0x3F) + LO8(ebx);
    POP32(esp, esp);
    esp++;
    /* cmp LO8(eax), MEM8(eax) - flags set for next jcc */
    MEM8(eax + eax) = MEM8(eax + eax) + LO8(ebx);
    _cf = ((uint32_t)(MEM8(eax + eax)) < (uint32_t)(LO8(ebx))); /* CF from add */
    eax = eax - 0x217C0400 - _cf; /* sbb */
    MEM8(esp + eax * 2 + 0x65) = MEM8(esp + eax * 2 + 0x65) + LO8(ebx);
    if (((uint32_t)(MEM8(esp + eax * 2 + 0x65)) < (uint32_t)(LO8(ebx)) || (MEM8(esp + eax * 2 + 0x65)) == 0)) (void)0; /* goto loc_00217C72 - dead code, label not in function */ /* jbe: below or equal (unsigned <=) */

loc_00217C09: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    _fs0 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217C74 - dead code, label not in function */

loc_00217C10: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61705C30);
    /* unhandled flags: imul -> jb */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217C8E - dead code, label not in function */

loc_00217C1A: ;
    esi = (uint32_t)((int32_t)MEM32(ecx + ebp * 2 + 0x6F) * (int32_t)0x306E);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    POP32(esp, esp);
    /* TODO: aas  */
    /* TODO: aas  */
    POP32(esp, esp);
    POP32(esp, eax);
    /* cmp LO8(eax), MEM8(eax) - flags set for next jcc */
    MEM8(edi + edi + 0x3F) = MEM8(edi + edi + 0x3F) + LO8(ebx);
    POP32(esp, esp);
    PUSH32(esp, edi);
    /* cmp LO8(eax), MEM8(eax) - flags set for next jcc */
    MEM8(ebx + edx * 2 + 0x61) = MEM8(ebx + edx * 2 + 0x61) + LO8(ebx);
    if (((uint32_t)(MEM8(ebx + edx * 2 + 0x61)) < (uint32_t)(LO8(ebx)) || (MEM8(ebx + edx * 2 + 0x61)) == 0)) goto loc_00217C9E; /* jbe: below or equal (unsigned <=) */

loc_00217C39: ;
    ebp--;
    if ((ebp == 0)) goto loc_00217C9E; /* je: equal / zero */

loc_00217C3D: ;
    if (((int32_t)ebp < 0)) goto loc_00217CA2; /* js: sign (negative) */

loc_00217C40: ;
    if (((int32_t)ebp < 0)) goto loc_00217C42; /* js: sign (negative) */

loc_00217C42: ;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    eax = eax | 0x5C000000;
    PUSH32(esp, esp);
    esi = (uint32_t)((int32_t)MEM32(esp + ebp * 2 + 0x65) * (int32_t)0x6174654D);
    /* unhandled flags: imul -> js */
    if (_flags /* js: sign (negative) */) goto loc_00217CB7;

loc_00217C55: ;
    /* unhandled flags: imul -> js */
    if (_flags /* js: sign (negative) */) goto loc_00217C57;

loc_00217C57: ;
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);

loc_00217C5B: ;
    MEM8(esp + edx * 2 + 0x69) = MEM8(esp + edx * 2 + 0x69) + LO8(ebx);
    if ((MEM8(esp + edx * 2 + 0x69) == 0)) (void)0; /* goto loc_00217CCD - dead code, label not in function */ /* je: equal / zero */

loc_00217C61: ;
    ecx--;
    /* TODO: insd dword ptr es:[edi], dx */
    /* TODO: popal  */
    if (_flags /* js: sign (negative) */) (void)0; /* goto loc_00217CCC - dead code, label not in function */

loc_00217C6A: ;
    if (_flags /* js: sign (negative) */) goto loc_00217C6C;

loc_00217C6C: ;
    /* TODO: sldt word ptr [eax] */
    MEM8(ebx + edx * 2 + 0x61) = MEM8(ebx + edx * 2 + 0x61) + LO8(ebx);
    if (((uint32_t)(MEM8(ebx + edx * 2 + 0x61)) < (uint32_t)(LO8(ebx)) || (MEM8(ebx + edx * 2 + 0x61)) == 0)) (void)0; /* goto loc_00217CDA - dead code, label not in function */ /* jbe: below or equal (unsigned <=) */

loc_00217C75: ;
    ecx--;
    /* TODO: insd dword ptr es:[edi], dx */
    /* TODO: popal  */
    if (_flags /* js: sign (negative) */) goto loc_00217CDF;

loc_00217C7D: ;
    if (_flags /* js: sign (negative) */) goto loc_00217C7F;

loc_00217C7F: ;
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    /* TODO: popal  */
    MEM8(ebp) = MEM8(ebp) + HI8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax + eax + 0x69) = MEM8(eax + eax + 0x69) + LO8(edx);
    MEM8(eax + eax + 0x6C) = MEM8(eax + eax + 0x6C) + HI8(edx);
    MEM8(ebp) = MEM8(ebp) + HI8(eax);
    esi--;
    MEM8(ecx) = MEM8(ecx) + HI8(eax);

loc_00217C9E: ;
    /* TODO: insd dword ptr es:[edi], dx */
    MEM8(ebp) = MEM8(ebp) + HI8(eax);

loc_00217CA2: ;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    SET_LO8(eax, LO8(eax) - MEM8(eax));
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM32(eax) = MEM32(eax) + eax;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    eax = eax | 0xA00;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    /* TODO: outsd dx, dword ptr [esi] */

loc_00217CB7: ;
    MEM8(ebx) = MEM8(ebx) + LO8(eax);
    /* TODO: outsd dx, dword ptr [esi] */
    MEM8(eax) = MEM8(eax) + HI8(edx);
    if (((int8_t)MEM8(eax) >= 0)) goto loc_00217CC0; /* jns: not sign (positive) */

loc_00217CC0: ;
    /* cmp eax, 0xD003100 - flags set for next jcc */
    MEM8(edx) = MEM8(edx) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(-16777216) = MEM8(-16777216) + HI8(ebx);
    MEM8(eax) = MEM8(eax) + 1;
    MEM8(esi) = MEM8(esi) + LO8(eax);
    MEM8(edi) = MEM8(edi) + LO8(eax);
    SET_HI8(eax, HI8(eax) + HI8(ebx));
    if (((int8_t)HI8(eax) < 0)) goto loc_00217CFC; /* jl: less (signed <) */

loc_00217CDB: ;
    MEM8(esp + eax * 2 + 0x65) = MEM8(esp + eax * 2 + 0x65) + LO8(ebx);

loc_00217CDF: ;
    if (((uint32_t)(MEM8(esp + eax * 2 + 0x65)) < (uint32_t)(LO8(ebx)) || (MEM8(esp + eax * 2 + 0x65)) == 0)) goto loc_00217D4A; /* jbe: below or equal (unsigned <=) */

loc_00217CE1: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    _fs1 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D4C - dead code, label not in function */

loc_00217CE8: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61505C30);
    /* unhandled flags: imul -> jb */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D66 - dead code, label not in function */

loc_00217CF2: ;
    esi = (uint32_t)((int32_t)MEM32(ecx + ebp * 2 + 0x6F) * (int32_t)0x5C64256E);
    MEM8(eax) = MEM8(eax) + LO8(eax);

loc_00217CFC: ;
    POP32(esp, esp);
    /* TODO: aas  */
    /* TODO: aas  */
    POP32(esp, esp);
    POP32(esp, edx);
    /* cmp LO8(eax), MEM8(eax) - flags set for next jcc */
    MEM8(0x786C3830) = MEM8(0x786C3830) + HI8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(edi) = MEM8(edi) + HI8(ecx);
    MEM8(eax) = MEM8(eax) + HI8(edx);
    MEM8(edi * 2 + 0x445C0021) = MEM8(edi * 2 + 0x445C0021) + LO8(edx);
    if (((uint32_t)(MEM8(edi * 2 + 0x445C0021)) < (uint32_t)(LO8(edx)) || (MEM8(edi * 2 + 0x445C0021)) == 0)) (void)0; /* goto loc_00217D82 - dead code, label not in function */ /* jbe: below or equal (unsigned <=) */

loc_00217D19: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    _fs2 = (uint32_t)(eax); /* flag operand kept for a later jcc */
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D84 - dead code, label not in function */

loc_00217D20: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61705C30);
    /* unhandled flags: imul -> jb */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D9E - dead code, label not in function */

loc_00217D2A: ;
    esi = (uint32_t)((int32_t)MEM32(ecx + ebp * 2 + 0x6F) * (int32_t)0x555C316E);
    esp++;
    ecx++;
    PUSH32(esp, esp);
    ecx++;
    POP32(esp, esp);
    esi--;
    ecx--;
    ebx++;
    ebx--;
    esi--;
    ecx++;
    ebp--;
    ebp++;
    POP32(esp, eax);
    edx++;
    esi--;
    MEM8(ebx + edx * 2 + 0x61) = MEM8(ebx + edx * 2 + 0x61) + LO8(ebx);
    if (((uint32_t)(MEM8(ebx + edx * 2 + 0x61)) < (uint32_t)(LO8(ebx)) || (MEM8(ebx + edx * 2 + 0x61)) == 0)) goto loc_00217DAE; /* jbe: below or equal (unsigned <=) */

loc_00217D49: ;
    ebp--;

loc_00217D4A: ;
    if ((ebp == 0)) goto loc_00217DAE; /* je: equal / zero */

loc_00217D4D: ;
    if (((int32_t)ebp < 0)) goto loc_00217DB2; /* js: sign (negative) */

loc_00217D50: ;
    if (((int32_t)ebp < 0)) goto loc_00217D52; /* js: sign (negative) */

loc_00217D52: ;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    eax = eax | 0x5C000000;
    PUSH32(esp, esp);
    esi = (uint32_t)((int32_t)MEM32(esp + ebp * 2 + 0x65) * (int32_t)0x6174654D);
    /* unhandled flags: imul -> js */
    if (_flags /* js: sign (negative) */) goto loc_00217DC7;

loc_00217D65: ;
    /* unhandled flags: imul -> js */
    if (_flags /* js: sign (negative) */) goto loc_00217D67;

loc_00217D67: ;
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(esp + edx * 2 + 0x69) = MEM8(esp + edx * 2 + 0x69) + LO8(ebx);
    if ((MEM8(esp + edx * 2 + 0x69) == 0)) (void)0; /* goto loc_00217DDD - dead code, label not in function */ /* je: equal / zero */

loc_00217D71: ;
    ecx--;
    /* TODO: insd dword ptr es:[edi], dx */
    /* TODO: popal  */
    if (_flags /* js: sign (negative) */) (void)0; /* goto loc_00217DDC - dead code, label not in function */

loc_00217D7A: ;
    if (_flags /* js: sign (negative) */) goto loc_00217D7C;

loc_00217D7C: ;
    /* TODO: sldt word ptr [eax] */
    MEM8(ebx + edx * 2 + 0x61) = MEM8(ebx + edx * 2 + 0x61) + LO8(ebx);
    if (((uint32_t)(MEM8(ebx + edx * 2 + 0x61)) < (uint32_t)(LO8(ebx)) || (MEM8(ebx + edx * 2 + 0x61)) == 0)) (void)0; /* goto loc_00217DEA - dead code, label not in function */ /* jbe: below or equal (unsigned <=) */

loc_00217D85: ;
    ecx--;
    /* TODO: insd dword ptr es:[edi], dx */
    /* TODO: popal  */
    if (_flags /* js: sign (negative) */) goto loc_00217DEF;

loc_00217D8D: ;
    if (_flags /* js: sign (negative) */) goto loc_00217D8F;

loc_00217D8F: ;
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    /* TODO: popal  */
    MEM8(ebp) = MEM8(ebp) + HI8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax + eax + 0x69) = MEM8(eax + eax + 0x69) + LO8(edx);
    MEM8(eax + eax + 0x6C) = MEM8(eax + eax + 0x6C) + HI8(edx);
    MEM8(ebp) = MEM8(ebp) + HI8(eax);
    esi--;
    MEM8(ecx) = MEM8(ecx) + HI8(eax);

loc_00217DAE: ;
    /* TODO: insd dword ptr es:[edi], dx */
    MEM8(ebp) = MEM8(ebp) + HI8(eax);

loc_00217DB2: ;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    SET_LO8(eax, LO8(eax) - MEM8(eax));
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM32(eax) = MEM32(eax) + eax;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    eax = eax | 0xA00;
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(esi) = MEM8(esi) + LO8(ecx);
    /* TODO: outsd dx, dword ptr [esi] */

loc_00217DC7: ;
    MEM8(ebx) = MEM8(ebx) + LO8(eax);
    /* TODO: outsd dx, dword ptr [esi] */
    MEM8(eax) = MEM8(eax) + HI8(edx);
    if (((int8_t)MEM8(eax) >= 0)) goto loc_00217DD0; /* jns: not sign (positive) */

loc_00217DD0: ;
    /* cmp eax, 0xD003100 - flags set for next jcc */
    MEM8(edx) = MEM8(edx) + LO8(ecx);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(-16777216) = MEM8(-16777216) + HI8(ebx);
    MEM8(eax) = MEM8(eax) + 1;
    MEM8(0x63006C00) = MEM8(0x63006C00) + HI8(eax);
    MEM8(0x73006C00) = MEM8(0x73006C00) + HI8(eax);

loc_00217DEF: ;
    MEM8(0x63006C00) = MEM8(0x63006C00) + HI8(eax);
    MEM8(0x73006C00) = MEM8(0x73006C00) + HI8(eax);
    MEM8(0x73006C00) = MEM8(0x73006C00) + HI8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    MEM8(eax) = MEM8(eax) + LO8(eax);
    SET_HI8(ebx, HI8(ebx) + HI8(ebx));

}

/* FAILED: sub_0262FC57 at 0x0262FC57 */
void sub_0262FC57(void) { /* translation failed */ }

/* FAILED: sub_101B913A at 0x101B913A */
void sub_101B913A(void) { /* translation failed */ }

/* FAILED: sub_3501672F at 0x3501672F */
void sub_3501672F(void) { /* translation failed */ }

/* FAILED: sub_E965EA97 at 0xE965EA97 */
void sub_E965EA97(void) { /* translation failed */ }

/* FAILED: sub_E96CEAA3 at 0xE96CEAA3 */
void sub_E96CEAA3(void) { /* translation failed */ }

/* FAILED: sub_E9D2ECA9 at 0xE9D2ECA9 */
void sub_E9D2ECA9(void) { /* translation failed */ }
