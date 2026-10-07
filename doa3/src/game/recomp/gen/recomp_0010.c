/**
 * Dead or Alive 3 - Recompiled code chunk 10
 * Functions: 137 (0x00215043 - 0xE9D2ECA9)
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <math.h>

/**
 * sub_00215043
 * Original: 0x00215043 - 0x00215071 (46 bytes, 17 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215043(void)
{
    int _flags = 0; /* fallback flag var */

loc_00215043: ;
    eax = MEM32(edx + 0x10);
    edx = MEM32(edx + 0x14);
    if (TEST_Z(LO8(edx), 4)) goto loc_00215052; /* je: equal / zero */

loc_0021504E: ;
    MEM32(eax + 8) = MEM32(eax + 8) & 0xFFFFFFFDu;

loc_00215052: ;
    if (TEST_Z(LO8(edx), 8)) goto loc_0021505B; /* je: equal / zero */

loc_00215057: ;
    MEM32(eax + 8) = MEM32(eax + 8) | 2;

loc_0021505B: ;
    if (TEST_NZ(LO8(edx), 1)) goto loc_0021506E; /* jne: not equal / not zero */

loc_00215060: ;
    ecx = MEM32(eax + 8);
    if (TEST_Z(LO8(ecx), 1)) goto loc_0021506E; /* je: equal / zero */

loc_00215068: ;
    ecx = ecx & 0xFFFFFFFEu;
    MEM32(eax + 8) = ecx;

loc_0021506E: ;
    eax = 0; /* xor self */
    esp += 4; return; /* ret */

}

/**
 * sub_00215071
 * Original: 0x00215071 - 0x00215096 (37 bytes, 14 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215071(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215071: ;
    eax = MEM32(ecx + 0x41C);
    PUSH32(esp, esi);
    esi = 0; /* xor self */
    if (CMP_EQ(eax, edx)) goto loc_0021508B; /* je: equal / zero */

loc_0021507E: ;
    esi = eax;
    eax = MEM32(eax + 0x24);
    if (CMP_NE(eax, edx)) goto loc_0021507E; /* jne: not equal / not zero */

loc_00215087: ;
    if (TEST_NZ(esi, esi)) { g_seh_ebp = ebp; sub_00215096(); return; } /* jne: not equal / not zero */

loc_0021508B: ;
    eax = MEM32(eax + 0x24);
    MEM32(ecx + 0x41C) = eax;
    g_seh_ebp = ebp; sub_0021509C(); return; /* tail jmp 0x0021509C */

}

/**
 * sub_00215096
 * Original: 0x00215096 - 0x0021509C (6 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215096(void)
{

loc_00215096: ;
    eax = MEM32(eax + 0x24);
    MEM32(esi + 0x24) = eax;

}

/**
 * sub_0021509C
 * Original: 0x0021509C - 0x002150AA (14 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021509C(void)
{
    int _flags = 0; /* fallback flag var */

loc_0021509C: ;
    eax = ecx + 0x420;
    if (CMP_NE(edx, MEM32(eax))) goto loc_002150A8; /* jne: not equal / not zero */

loc_002150A6: ;
    MEM32(eax) = esi;

loc_002150A8: ;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_002150AA
 * Original: 0x002150AA - 0x002150CF (37 bytes, 14 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002150AA(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002150AA: ;
    eax = MEM32(ecx + 0x424);
    PUSH32(esp, esi);
    esi = 0; /* xor self */
    if (CMP_EQ(eax, edx)) goto loc_002150C4; /* je: equal / zero */

loc_002150B7: ;
    esi = eax;
    eax = MEM32(eax + 0x24);
    if (CMP_NE(eax, edx)) goto loc_002150B7; /* jne: not equal / not zero */

loc_002150C0: ;
    if (TEST_NZ(esi, esi)) { g_seh_ebp = ebp; sub_002150CF(); return; } /* jne: not equal / not zero */

loc_002150C4: ;
    eax = MEM32(eax + 0x24);
    MEM32(ecx + 0x424) = eax;
    g_seh_ebp = ebp; sub_002150D5(); return; /* tail jmp 0x002150D5 */

}

/**
 * sub_002150CF
 * Original: 0x002150CF - 0x002150D5 (6 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002150CF(void)
{

loc_002150CF: ;
    eax = MEM32(eax + 0x24);
    MEM32(esi + 0x24) = eax;

}

/**
 * sub_002150D5
 * Original: 0x002150D5 - 0x002150E3 (14 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002150D5(void)
{
    int _flags = 0; /* fallback flag var */

loc_002150D5: ;
    eax = ecx + 0x428;
    if (CMP_NE(edx, MEM32(eax))) goto loc_002150E1; /* jne: not equal / not zero */

loc_002150DF: ;
    MEM32(eax) = esi;

loc_002150E1: ;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_002150E3
 * Original: 0x002150E3 - 0x00215102 (31 bytes, 14 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002150E3(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002150E3: ;
    eax = MEM32(ecx + 0x28);
    PUSH32(esp, esi);
    esi = 0; /* xor self */
    if (CMP_EQ(eax, edx)) goto loc_002150FA; /* je: equal / zero */

loc_002150ED: ;
    esi = eax;
    eax = MEM32(eax + 0x24);
    if (CMP_NE(eax, edx)) goto loc_002150ED; /* jne: not equal / not zero */

loc_002150F6: ;
    if (TEST_NZ(esi, esi)) { g_seh_ebp = ebp; sub_00215102(); return; } /* jne: not equal / not zero */

loc_002150FA: ;
    eax = MEM32(eax + 0x24);
    MEM32(ecx + 0x28) = eax;
    g_seh_ebp = ebp; sub_00215108(); return; /* tail jmp 0x00215108 */

}

/**
 * sub_00215102
 * Original: 0x00215102 - 0x00215108 (6 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215102(void)
{

loc_00215102: ;
    eax = MEM32(eax + 0x24);
    MEM32(esi + 0x24) = eax;

}

/**
 * sub_00215108
 * Original: 0x00215108 - 0x00215112 (10 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215108(void)
{
    int _flags = 0; /* fallback flag var */

loc_00215108: ;
    if (CMP_NE(edx, MEM32(ecx + 0x2C))) goto loc_00215110; /* jne: not equal / not zero */

loc_0021510D: ;
    MEM32(ecx + 0x2C) = esi;

loc_00215110: ;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_00215112
 * Original: 0x00215112 - 0x00215188 (118 bytes, 46 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215112(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215112: ;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    edi = edx;
    if (CMP_EQ(MEM8(edi + 0x26), 0)) goto loc_00215185; /* je: equal / zero */

loc_0021511C: ;
    eax = ZX8(MEM8(edi + 0x11));
    eax = eax - 0;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    if ((eax == 0)) goto loc_00215144; /* je: equal / zero */

loc_00215127: ;
    eax--;
    eax--;
    if ((eax == 0)) goto loc_00215136; /* je: equal / zero */

loc_0021512B: ;
    eax--;
    if ((eax != 0)) goto loc_00215183; /* jne: not equal / not zero */

loc_0021512E: ;
    ebx = edi + 0x28;
    ebp = edi + 0x2C;
    goto loc_00215150;

loc_00215136: ;
    ebx = ecx + 0x424;
    ebp = ecx + 0x428;
    goto loc_00215150;

loc_00215144: ;
    ebx = ecx + 0x41C;
    ebp = ecx + 0x420;

loc_00215150: ;
    PUSH32(esp, esi);

loc_00215151: ;
    esi = MEM32(ebx);
    if (CMP_NE(MEM32(esi + 0x10), edi)) goto loc_0021516F; /* jne: not equal / not zero */

loc_00215158: ;
    eax = MEM32(esi + 0x24);
    MEM32(ebx) = eax;
    MEM32(esi + 4) = 0xC000000Fu;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_0021516D: ;
    goto loc_00215176;

loc_0021516F: ;
    MEM32(esp + 0x10) = esi;
    ebx = esi + 0x24;

loc_00215176: ;
    if (CMP_NE(MEM32(ebp), esi)) goto loc_00215151; /* jne: not equal / not zero */

loc_0021517B: ;
    eax = MEM32(esp + 0x10);
    MEM32(ebp) = eax;
    POP32(esp, esi);

loc_00215183: ;
    POP32(esp, ebp);
    POP32(esp, ebx);

loc_00215185: ;
    POP32(esp, edi);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_00215188
 * Original: 0x00215188 - 0x002151C5 (61 bytes, 24 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215188(void)
{
    int _flags = 0; /* fallback flag var */

loc_00215188: ;
    PUSH32(esp, esi);
    esi = edx;
    MEM8(esi + 0x20) = MEM8(esi + 0x20) + 1;
    /* test MEM8(esi + 0x10), 0x20 - flags set for next jcc */
    PUSH32(esp, edi);
    edi = ecx;
    if (TEST_NZ(MEM8(esi + 0x10), 0x20)) goto loc_002151C2; /* jne: not equal / not zero */

loc_00215197: ;
    MEM8(esi + 1) = MEM8(esi + 1) | 0x40;
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_002151A0: ;
    eax++;
    MEM32(esi + 0x1C) = eax;
    if (CMP_EQ(MEM32(edi + 0x438), 0)) goto loc_002151B1; /* je: equal / zero */

loc_002151AD: ;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) | 0x40;

loc_002151B1: ;
    ecx = MEM32(edi);
    PUSH32(esp, 4);
    POP32(esp, eax);
    MEM32(ecx + 0xC) = eax;
    ecx = MEM32(edi);
    MEM32(ecx + 0x10) = eax;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) | 0x20;

loc_002151C2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_00215200
 * Original: 0x00215200 - 0x00215215 (21 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215200(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215200: ;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    if (TEST_Z(MEM8(esi + 0x22), 1)) { g_seh_ebp = ebp; sub_00215215(); return; } /* je: equal / zero */

loc_0021520B: ;
    eax = 0x40020000;
    g_seh_ebp = ebp; sub_002152B3(); return; /* tail jmp 0x002152B3 */

}

/**
 * sub_00215215
 * Original: 0x00215215 - 0x002152B3 (158 bytes, 53 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215215(void)
{
    int _flags = 0; /* fallback flag var */

loc_00215215: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_0021521D: ;
    edi = MEM32(esi + 0x10);
    /* test MEM8(edi + 0x10), 0x10 - flags set for next jcc */
    SET_LO8(ebx, LO8(eax));
    if (TEST_NZ(MEM8(edi + 0x10), 0x10)) goto loc_002152A2; /* jne: not equal / not zero */

loc_00215228: ;
    SET_LO16(eax, MEM16(esi + 0x22));
    if (TEST_Z(LO8(eax), 2)) goto loc_00215282; /* je: equal / zero */

loc_00215230: ;
    eax = ZX8(MEM8(edi + 0x11));
    eax = eax - 0;
    if ((eax == 0)) goto loc_0021525F; /* je: equal / zero */

loc_00215239: ;
    eax--;
    eax--;
    if ((eax == 0)) goto loc_00215252; /* je: equal / zero */

loc_0021523D: ;
    eax--;
    if ((eax == 0)) goto loc_00215247; /* je: equal / zero */

loc_00215240: ;
    esi = 0x80000600u;
    goto loc_002152A7;

loc_00215247: ;
    edx = esi;
    ecx = edi;
    PUSH32(esp, 0); sub_002150E3(); /* call 0x002150E3 */

loc_00215250: ;
    goto loc_0021526A;

loc_00215252: ;
    ecx = MEM32(esp + 0x10);
    edx = esi;
    PUSH32(esp, 0); sub_002150AA(); /* call 0x002150AA */

loc_0021525D: ;
    goto loc_0021526A;

loc_0021525F: ;
    ecx = MEM32(esp + 0x10);
    edx = esi;
    PUSH32(esp, 0); sub_00215071(); /* call 0x00215071 */

loc_0021526A: ;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;
    MEM8(esi + 0x22) = MEM8(esi + 0x22) | 1;
    PUSH32(esp, esi);
    MEM32(esi + 4) = 0xC000000Fu;
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_0021527E: ;
    esi = 0; /* xor self */
    goto loc_002152A7;

loc_00215282: ;
    ecx = MEM32(esp + 0x10);
    SET_LO16(eax, LO16(eax) | 1);
    MEM16(esi + 0x22) = LO16(eax);
    eax = ecx + 0x42C;
    edx = MEM32(eax);
    MEM32(esi + 0x24) = edx;
    edx = edi;
    MEM32(eax) = esi;
    PUSH32(esp, 0); sub_00215188(); /* call 0x00215188 */

loc_002152A2: ;
    esi = 0x40020000;

loc_002152A7: ;
    SET_LO8(ecx, LO8(ebx));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002152AF: ;
    POP32(esp, edi);
    eax = esi;
    POP32(esp, ebx);

}

/**
 * sub_002152B3
 * Original: 0x002152B3 - 0x002152B7 (4 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002152B3(void)
{

loc_002152B3: ;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

}

/**
 * sub_002152B7
 * Original: 0x002152B7 - 0x002152ED (54 bytes, 22 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002152B7(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002152B7: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = edx;
    esi = MEM32(edi + 0x10);
    ebp = ecx;
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002152C8: ;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) | 0x10;
    edx = esi;
    ecx = ebp;
    SET_LO8(ebx, LO8(eax));
    PUSH32(esp, 0); sub_00215112(); /* call 0x00215112 */

loc_002152D7: ;
    SET_LO8(eax, MEM8(esi + 0x11));
    if (TEST_Z(LO8(eax), LO8(eax))) { g_seh_ebp = ebp; sub_002152ED(); return; } /* je: equal / zero */

loc_002152DE: ;
    if (CMP_EQ(LO8(eax), 2)) { g_seh_ebp = ebp; sub_002152ED(); return; } /* je: equal / zero */

loc_002152E2: ;
    edx = esi;
    ecx = ebp;
    PUSH32(esp, 0); sub_0021653E(); /* call 0x0021653E */

loc_002152EB: ;
    g_seh_ebp = ebp; sub_002152F6(); return; /* tail jmp 0x002152F6 */

}

/**
 * sub_002152ED
 * Original: 0x002152ED - 0x002152F6 (9 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002152ED(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002152ED: ;
    edx = esi;
    ecx = ebp;
    PUSH32(esp, 0); sub_00216285(); /* call 0x00216285 */

}

/**
 * sub_002152F6
 * Original: 0x002152F6 - 0x0021531E (40 bytes, 15 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002152F6(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002152F6: ;
    edx = esi;
    ecx = ebp;
    PUSH32(esp, 0); sub_00215188(); /* call 0x00215188 */

loc_002152FF: ;
    eax = ebp + 0x434;
    ecx = MEM32(eax);
    MEM32(edi + 0x14) = ecx;
    SET_LO8(ecx, LO8(ebx));
    MEM32(eax) = edi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215314: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    eax = 0x40000000;
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}

/**
 * sub_0021531E
 * Original: 0x0021531E - 0x00215371 (83 bytes, 33 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021531E(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021531E: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = edx;
    PUSH32(esp, edi);
    edi = MEM32(esi + 0x10);
    ebp = ecx;
    ebx = 0; /* xor self */
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215332: ;
    edx = edi;
    ecx = ebp;
    MEM8(esp + 0x13) = LO8(eax);
    PUSH32(esp, 0); sub_00215112(); /* call 0x00215112 */

loc_0021533F: ;
    if (CMP_EQ(MEM8(edi + 0x27), LO8(ebx))) goto loc_0021535F; /* je: equal / zero */

loc_00215344: ;
    eax = ebp + 0x430;
    ecx = MEM32(eax);
    MEM32(esi + 0x14) = ecx;
    edx = edi;
    ecx = ebp;
    MEM32(eax) = esi;
    PUSH32(esp, 0); sub_00215188(); /* call 0x00215188 */

loc_0021535A: ;
    ebx = 0x40000000;

loc_0021535F: ;
    SET_LO8(ecx, MEM8(esp + 0x13));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215369: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    eax = ebx;
    POP32(esp, ebx);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_00215371
 * Original: 0x00215371 - 0x0021545E (237 bytes, 82 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215371(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00215371: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(ebp + 0xC);
    eax = ZX8(MEM8(edi + 1));
    if (CMP_G(eax, 0xC)) goto loc_00215409; /* jg: greater (signed >) */

loc_00215386: ;
    if (CMP_EQ(eax, 0xC)) goto loc_002153FD; /* je: equal / zero */

loc_00215388: ;
    PUSH32(esp, 2);
    POP32(esp, ecx);
    eax = eax - ecx;
    if ((eax == 0)) goto loc_002153F1; /* je: equal / zero */

loc_0021538F: ;
    eax = eax - ecx;
    if ((eax == 0)) goto loc_002153E5; /* je: equal / zero */

loc_00215393: ;
    eax--;
    if ((eax == 0)) goto loc_002153D6; /* je: equal / zero */

loc_00215396: ;
    eax = eax - ecx;
    if ((eax == 0)) goto loc_002153C4; /* je: equal / zero */

loc_0021539A: ;
    eax = eax - ecx;
    if ((eax == 0)) goto loc_002153B5; /* je: equal / zero */

loc_0021539E: ;
    eax = eax - ecx;
    if ((eax != 0)) goto loc_00215457; /* jne: not equal / not zero */

loc_002153A6: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00217012(); /* call 0x00217012 */

loc_002153B0: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_002153B5: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00216D98(); /* call 0x00216D98 */

loc_002153BF: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_002153C4: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_002153CC: ;
    MEM32(edi + 0x14) = eax;
    esi = 0; /* xor self */
    g_seh_ebp = ebp; sub_0021546A(); return; /* tail jmp 0x0021546A */

loc_002153D6: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00215043(); /* call 0x00215043 */

loc_002153E0: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_002153E5: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00215022(); /* call 0x00215022 */

loc_002153EF: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_002153F1: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00214EC3(); /* call 0x00214EC3 */

loc_002153FB: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_002153FD: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00217179(); /* call 0x00217179 */

loc_00215407: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_00215409: ;
    if (CMP_EQ(eax, 0xD)) { g_seh_ebp = ebp; sub_0021545E(); return; } /* je: equal / zero */

loc_0021540E: ;
    if (CMP_LE(eax, 0x3F)) goto loc_00215457; /* jle: less or equal (signed <=) */

loc_00215413: ;
    if (CMP_LE(eax, 0x41)) goto loc_0021544B; /* jle: less or equal (signed <=) */

loc_00215418: ;
    if (CMP_EQ(eax, 0x43)) goto loc_0021543F; /* je: equal / zero */

loc_0021541D: ;
    if (CMP_EQ(eax, 0x46)) goto loc_00215433; /* je: equal / zero */

loc_00215422: ;
    if (CMP_NE(eax, 0x4A)) goto loc_00215457; /* jne: not equal / not zero */

loc_00215427: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00216F45(); /* call 0x00216F45 */

loc_00215431: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_00215433: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_0021531E(); /* call 0x0021531E */

loc_0021543D: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_0021543F: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_002152B7(); /* call 0x002152B7 */

loc_00215449: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_0021544B: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00217943(); /* call 0x00217943 */

loc_00215455: ;
    g_seh_ebp = ebp; sub_00215468(); return; /* tail jmp 0x00215468 */

loc_00215457: ;
    esi = 0x80000200u;
    g_seh_ebp = ebp; sub_0021546A(); return; /* tail jmp 0x0021546A */

}

/**
 * sub_0021545E
 * Original: 0x0021545E - 0x00215468 (10 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021545E(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021545E: ;
    ecx = MEM32(ebp + 8);
    edx = edi;
    PUSH32(esp, 0); sub_00217298(); /* call 0x00217298 */

}

/**
 * sub_00215468
 * Original: 0x00215468 - 0x0021546A (2 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215468(void)
{

loc_00215468: ;
    esi = eax;

}

/**
 * sub_0021546A
 * Original: 0x0021546A - 0x002154DD (115 bytes, 39 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021546A(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021546A: ;
    eax = esi;
    eax = eax & 0xC0000000u;
    if (CMP_EQ(eax, 0x40000000)) goto loc_00215481; /* je: equal / zero */

loc_00215478: ;
    PUSH32(esp, edi);
    MEM32(edi + 4) = esi;
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_00215481: ;
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    POP32(esp, ebp);
    esp += 12; return; /* ret 8 */

    ecx = MEM32(0x40A314);
    ecx = ecx - 0;
    if ((ecx == 0)) goto loc_002154CE; /* je: equal / zero */

loc_002154A0: ;
    ecx--;
    if ((ecx == 0)) goto loc_002154BA; /* je: equal / zero */

loc_002154A3: ;
    ecx--;
    if ((ecx != 0)) goto loc_002154D9; /* jne: not equal / not zero */

loc_002154A6: ;
    PUSH32(esp, MEM32(0x40A2C8));
    ecx = esi;
    PUSH32(esp, 0x80000600u);
    PUSH32(esp, 0); sub_00213A7D(); /* call 0x00213A7D */

loc_002154B8: ;
    goto loc_002154D9;

loc_002154BA: ;
    PUSH32(esp, MEM32(0x40A2C8));
    ecx = esi;
    PUSH32(esp, 0x80000600u);
    PUSH32(esp, 0); sub_002137BF(); /* call 0x002137BF */

loc_002154CC: ;
    goto loc_002154D9;

loc_002154CE: ;
    eax = eax + 8;
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0); sub_00214267(); /* call 0x00214267 */

loc_002154D9: ;
    POP32(esp, esi);
    esp += 20; return; /* ret 16 */

}

/**
 * sub_002154DD
 * Original: 0x002154DD - 0x0021550D (48 bytes, 13 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002154DD(void)
{
    int _flags = 0; /* fallback flag var */

loc_002154DD: ;
    edx = MEM32(esp + 4);
    ecx = ecx | 0xFFFFFFFFu;
    /* test edx, edx - flags set for next jcc */
    eax = 0xFD050F80u;
    if (TEST_Z(edx, edx)) goto loc_002154F2; /* je: equal / zero */

loc_002154ED: ;
    eax = 0xFFB3B4C0u;

loc_002154F2: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x40A2F8);
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x40A2D0);
    MEM32(0x40A314) = edx;
    { uint32_t _icall_t = MEM32(0x217B2C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_0021550A: ;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_0021559C
 * Original: 0x0021559C - 0x002155F9 (93 bytes, 29 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021559C(void)
{

loc_0021559C: ;
    PUSH32(esp, esi);
    esi = MEM32(esp + 8);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_002155A9: ;
    SET_LO16(edi, ZX8(MEM8(eax + 3)));
    edx = 0; /* xor self */
    ecx = eax + 8;
    MEM8(ecx) = 0x30;
    PUSH32(esp, ecx);
    ecx = esi;
    MEM8(eax + 9) = 0x40;
    MEM32(eax + 0x10) = 0x215CE4;
    MEM32(eax + 0x14) = esi;
    MEM32(eax + 0x18) = edx;
    MEM32(eax + 0x20) = edx;
    MEM32(eax + 0x1C) = edx;
    MEM8(eax + 0x24) = LO8(edx);
    MEM8(eax + 0x25) = LO8(edx);
    MEM8(eax + 0x26) = LO8(edx);
    MEM8(eax + 0x30) = 0x23;
    MEM8(eax + 0x31) = 1;
    MEM16(eax + 0x32) = 1;
    MEM16(eax + 0x34) = LO16(edi);
    MEM16(eax + 0x36) = LO16(edx);
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_002155F4: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002155F9
 * Original: 0x002155F9 - 0x00215670 (119 bytes, 40 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002155F9(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002155F9: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(ebp + 8);
    ecx = edi;
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_00215609: ;
    esi = eax;
    SET_LO16(eax, MEM16(esi + 0x3A));
    ebx = 0; /* xor self */
    if (TEST_Z(LO8(eax), 0x10)) { g_seh_ebp = ebp; sub_00215670(); return; } /* je: equal / zero */

loc_00215615: ;
    if (CMP_NE(MEM32(0x40A314), 1)) goto loc_00215668; /* jne: not equal / not zero */

loc_0021561E: ;
    if (CMP_EQ(MEM32(0x40A2C8), ebx)) goto loc_00215668; /* je: equal / zero */

loc_00215626: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x40A2D0);
    edi = 0; /* xor self */
    { uint32_t _icall_t = MEM32(0x217B30); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215633: ;
    SET_LO16(eax, MEM16(esi + 0x38));
    if (TEST_Z(LO8(eax), 2)) goto loc_0021564B; /* je: equal / zero */

loc_0021563B: ;
    if (TEST_NZ(LO8(eax), 0x10)) goto loc_0021564B; /* jne: not equal / not zero */

loc_0021563F: ;
    if (TEST_Z(HI8(eax), 2)) goto loc_00215650; /* je: equal / zero */

loc_00215644: ;
    edi = 0x1000000;
    goto loc_00215650;

loc_0021564B: ;
    edi = 0x80000600u;

loc_00215650: ;
    eax = MEM32(0x40A2C8);
    ecx = MEM32(ebp + 8);
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    MEM32(0x40A2C8) = ebx;
    PUSH32(esp, 0); sub_002137BF(); /* call 0x002137BF */

loc_00215665: ;
    edi = MEM32(ebp + 8);

loc_00215668: ;
    MEM8(esi + 0x3A) = MEM8(esi + 0x3A) & 0xEF;
    PUSH32(esp, 0x14);
    g_seh_ebp = ebp; sub_002156E7(); return; /* tail jmp 0x002156E7 */

}

/**
 * sub_00215670
 * Original: 0x00215670 - 0x002156E7 (119 bytes, 45 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215670(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215670: ;
    if (TEST_Z(LO8(eax), 1)) goto loc_002156C1; /* je: equal / zero */

loc_00215674: ;
    ecx = 0; /* xor self */
    SET_LO8(ecx, MEM8(esi + 3));
    MEM8(ebp + 8) = LO8(ecx);
    SET_LO8(eax, 1);
    ecx--;
    SET_LO8(eax, LO8(eax) << LO8(ecx));
    if (TEST_Z(MEM8(esi + 0x38), 1)) goto loc_002156A1; /* je: equal / zero */

loc_00215687: ;
    if (TEST_Z(MEM8(esi + 5), LO8(eax))) goto loc_00215696; /* je: equal / zero */

loc_0021568C: ;
    PUSH32(esp, MEM32(ebp + 8));
    ecx = edi;
    PUSH32(esp, 0); sub_0021392D(); /* call 0x0021392D */

loc_00215696: ;
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_0021559C(); /* call 0x0021559C */

loc_0021569C: ;
    g_seh_ebp = ebp; sub_00215740(); return; /* tail jmp 0x00215740 */

loc_002156A1: ;
    SET_LO8(ecx, MEM8(esi + 5));
    if (TEST_Z(LO8(eax), LO8(ecx))) goto loc_002156B9; /* je: equal / zero */

loc_002156A8: ;
    PUSH32(esp, MEM32(ebp + 8));
    SET_LO8(eax, ~LO8(eax));
    SET_LO8(eax, LO8(eax) & LO8(ecx));
    ecx = edi;
    MEM8(esi + 5) = LO8(eax);
    PUSH32(esp, 0); sub_0021392D(); /* call 0x0021392D */

loc_002156B9: ;
    MEM8(esi + 0x3A) = MEM8(esi + 0x3A) & 0xFE;
    PUSH32(esp, 0x10);
    g_seh_ebp = ebp; sub_002156E7(); return; /* tail jmp 0x002156E7 */

loc_002156C1: ;
    if (TEST_Z(LO8(eax), 2)) goto loc_002156CD; /* je: equal / zero */

loc_002156C5: ;
    SET_LO16(eax, LO16(eax) & 0xFFFD);
    PUSH32(esp, 0x11);
    goto loc_002156E3;

loc_002156CD: ;
    if (TEST_Z(LO8(eax), 4)) goto loc_002156D9; /* je: equal / zero */

loc_002156D1: ;
    SET_LO16(eax, LO16(eax) & 0xFFFB);
    PUSH32(esp, 0x12);
    goto loc_002156E3;

loc_002156D9: ;
    if (TEST_Z(LO8(eax), 8)) { g_seh_ebp = ebp; sub_00215731(); return; } /* je: equal / zero */

loc_002156DD: ;
    SET_LO16(eax, LO16(eax) & 0xFFF7);
    PUSH32(esp, 0x13);

loc_002156E3: ;
    MEM16(esi + 0x3A) = LO16(eax);

}

/**
 * sub_002156E7
 * Original: 0x002156E7 - 0x00215747 (96 bytes, 32 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002156E7(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002156E7: ;
    POP32(esp, ecx);
    MEM16(esi + 0x32) = LO16(ecx);
    SET_LO16(ecx, ZX8(MEM8(esi + 3)));
    eax = esi + 8;
    MEM16(esi + 0x34) = LO16(ecx);
    PUSH32(esp, eax);
    ecx = edi;
    MEM8(eax) = 0x30;
    MEM8(esi + 9) = 0x40;
    MEM32(esi + 0x10) = 0x215CB4;
    MEM32(esi + 0x14) = edi;
    MEM32(esi + 0x18) = ebx;
    MEM32(esi + 0x20) = ebx;
    MEM32(esi + 0x1C) = ebx;
    MEM8(esi + 0x24) = LO8(ebx);
    MEM8(esi + 0x25) = LO8(ebx);
    MEM8(esi + 0x26) = LO8(ebx);
    MEM8(esi + 0x30) = 0x23;
    MEM8(esi + 0x31) = 1;
    MEM16(esi + 0x36) = LO16(ebx);
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_0021572F: ;
    goto loc_00215740;

    MEM16(esi + 0x3A) = MEM16(esi + 0x3A) & 0;
    PUSH32(esp, edi);
    esi = esi + 8;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00215CB4(); /* call 0x00215CB4 */

loc_00215740: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    POP32(esp, ebp);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00215731
 * Original: 0x00215731 - 0x00215740 (15 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215731(void)
{

loc_00215731: ;
    MEM16(esi + 0x3A) = MEM16(esi + 0x3A) & 0;
    PUSH32(esp, edi);
    esi = esi + 8;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00215CB4(); /* call 0x00215CB4 */

}

/**
 * sub_00215740
 * Original: 0x00215740 - 0x00215747 (7 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215740(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215740: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    POP32(esp, ebp);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00215747
 * Original: 0x00215747 - 0x00215769 (34 bytes, 9 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215747(void)
{

loc_00215747: ;
    eax = MEM32(esp + 4);
    ecx = MEM32(esp + 8);
    PUSH32(esp, eax);
    MEM8(eax) = 0x1C;
    MEM8(eax + 1) = 0xC3;
    MEM32(eax + 8) = 0x21550D;
    MEM32(eax + 0xC) = ecx;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215766: ;
    esp += 12; return; /* ret 8 */

}

/**
 * sub_00215769
 * Original: 0x00215769 - 0x00215839 (208 bytes, 51 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215769(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00215769: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_00215778: ;
    ecx = 0; /* xor self */
    /* cmp eax, ecx - flags set for next jcc */
    PUSH32(esp, 4);
    SET_LO8(edx, 3);
    POP32(esp, esi);
    MEM32(ebp + -4) = 1;
    edi = 0x21554C;
    if (CMP_EQ(eax, ecx)) { g_seh_ebp = ebp; sub_00215839(); return; } /* je: equal / zero */

loc_00215793: ;
    SET_LO8(ebx, MEM8(ebp + 0xC));
    if (CMP_B(MEM8(eax + 2), LO8(ebx))) { g_seh_ebp = ebp; sub_00215839(); return; } /* jb: below (unsigned <) */

loc_0021579F: ;
    /* cmp MEM8(ebp + 0x14), LO8(ecx) - flags set for next jcc */
    eax = MEM32(ebp + 0x10);
    MEM32(0x40A2C8) = eax;
    if (CMP_EQ(MEM8(ebp + 0x14), LO8(ecx))) goto loc_002157BD; /* je: equal / zero */

loc_002157AC: ;
    edx = 0; /* xor self */
    edx++;
    esi = edx;
    edi = 0x215578;
    MEM32(ebp + -4) = 2;

loc_002157BD: ;
    PUSH32(esp, MEM32(ebp + -4));
    SET_LO16(eax, ZX8(LO8(ebx)));
    MEM32(0x40A2A0) = edi;
    edi = MEM32(ebp + 8);
    MEM8(0x40A298) = 0x30;
    MEM8(0x40A299) = 0x40;
    MEM32(0x40A2A4) = edi;
    MEM32(0x40A2A8) = ecx;
    MEM32(0x40A2B0) = ecx;
    MEM32(0x40A2AC) = ecx;
    MEM8(0x40A2B4) = LO8(ecx);
    MEM8(0x40A2B5) = LO8(ecx);
    MEM8(0x40A2B6) = LO8(ecx);
    MEM8(0x40A2C0) = 0x23;
    MEM8(0x40A2C1) = LO8(edx);
    MEM16(0x40A2C2) = LO16(esi);
    MEM16(0x40A2C4) = LO16(eax);
    MEM16(0x40A2C6) = LO16(ecx);
    PUSH32(esp, 0); sub_002154DD(); /* call 0x002154DD */

loc_0021582B: ;
    PUSH32(esp, 0x40A298);
    ecx = edi;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215837: ;
    g_seh_ebp = ebp; sub_00215849(); return; /* tail jmp 0x00215849 */

}

/**
 * sub_00215839
 * Original: 0x00215839 - 0x00215849 (16 bytes, 4 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215839(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215839: ;
    PUSH32(esp, MEM32(ebp + 0x10));
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0x80000300u);
    PUSH32(esp, 0); sub_002137BF(); /* call 0x002137BF */

}

/**
 * sub_00215849
 * Original: 0x00215849 - 0x00215850 (7 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215849(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215849: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 20; return; /* ret 16 */

}

/**
 * sub_00215850
 * Original: 0x00215850 - 0x00215882 (50 bytes, 16 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215850(void)
{

loc_00215850: ;
    PUSH32(esp, esi);
    esi = MEM32(esp + 8);
    ecx = esi;
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_0021585C: ;
    edx = MEM32(eax + 0x3C);
    ecx = eax + 8;
    MEM8(ecx) = 0x1C;
    PUSH32(esp, ecx);
    ecx = esi;
    MEM8(eax + 9) = 0x43;
    MEM32(eax + 0x10) = 0x215747;
    MEM32(eax + 0x14) = esi;
    MEM32(eax + 0x18) = edx;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_0021587E: ;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002159C9
 * Original: 0x002159C9 - 0x00215A19 (80 bytes, 33 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002159C9(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002159C9: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_002159D7: ;
    esi = eax;
    MEM8(esi) = MEM8(esi) | 2;
    ebx = 0; /* xor self */
    ebx++;
    MEM8(ebp + -4) = LO8(ebx);

loc_002159E2: ;
    if (TEST_Z(MEM8(esi + 5), LO8(ebx))) goto loc_002159F9; /* je: equal / zero */

loc_002159E7: ;
    PUSH32(esp, MEM32(ebp + -4));
    ecx = MEM32(ebp + 8);
    PUSH32(esp, 0); sub_0021392D(); /* call 0x0021392D */

loc_002159F2: ;
    SET_LO8(eax, LO8(ebx));
    SET_LO8(eax, ~LO8(eax));
    MEM8(esi + 5) = MEM8(esi + 5) & LO8(eax);

loc_002159F9: ;
    ebx = ebx << 1;
    MEM8(ebp + -4) = MEM8(ebp + -4) + 1;
    SET_LO8(eax, MEM8(ebp + -4));
    if (CMP_BE(LO8(eax), MEM8(esi + 2))) goto loc_002159E2; /* jbe: below or equal (unsigned <=) */

loc_00215A06: ;
    /* test MEM8(esi), 8 - flags set for next jcc */
    POP32(esp, esi);
    POP32(esp, ebx);
    if (TEST_Z(MEM8(esi), 8)) goto loc_00215A15; /* je: equal / zero */

loc_00215A0D: ;
    PUSH32(esp, MEM32(ebp + 8));
    PUSH32(esp, 0); sub_00215850(); /* call 0x00215850 */

loc_00215A15: ;
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00215BEB
 * Original: 0x00215BEB - 0x00215C1C (49 bytes, 21 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215BEB(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00215BEB: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 8);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_00215BFB: ;
    SET_LO8(ebx, MEM8(eax + 4));
    edx = 0; /* xor self */
    SET_LO8(ecx, 1);
    MEM8(eax + 3) = LO8(edx);
    MEM8(ebp + 0xB) = LO8(ebx);

loc_00215C08: ;
    if (TEST_NZ(MEM8(ebp + 0xB), LO8(ecx))) { g_seh_ebp = ebp; sub_00215C1C(); return; } /* jne: not equal / not zero */

loc_00215C0D: ;
    MEM8(eax + 3) = MEM8(eax + 3) + 1;
    SET_LO8(ebx, MEM8(eax + 3));
    SET_LO8(ecx, LO8(ecx) << 1);
    if (CMP_BE(LO8(ebx), MEM8(eax + 2))) goto loc_00215C08; /* jbe: below or equal (unsigned <=) */

loc_00215C1A: ;
    g_seh_ebp = ebp; sub_00215C24(); return; /* tail jmp 0x00215C24 */

}

/**
 * sub_00215C1C
 * Original: 0x00215C1C - 0x00215C24 (8 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215C1C(void)
{

loc_00215C1C: ;
    SET_LO8(ecx, ~LO8(ecx));
    SET_LO8(ecx, LO8(ecx) & MEM8(eax + 4));
    MEM8(eax + 4) = LO8(ecx);

}

/**
 * sub_00215C24
 * Original: 0x00215C24 - 0x00215CB4 (144 bytes, 46 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215C24(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215C24: ;
    SET_LO8(ebx, MEM8(eax + 3));
    /* cmp LO8(ebx), MEM8(eax + 2) - flags set for next jcc */
    MEM8(eax + 0x26) = LO8(edx);
    ecx = eax + 8;
    if (CMP_BE(LO8(ebx), MEM8(eax + 2))) goto loc_00215C56; /* jbe: below or equal (unsigned <=) */

loc_00215C32: ;
    edi = MEM32(eax + 0x3C);
    MEM32(eax + 0x18) = edi;
    edi = eax + 0x38;
    MEM32(eax + 0x20) = edi;
    edi = ZX8(MEM8(eax + 7));
    MEM8(ecx) = 0x28;
    MEM8(eax + 9) = 0x41;
    MEM32(eax + 0x10) = 0x215A19;
    MEM8(eax + 0x25) = 1;
    goto loc_00215C9B;

loc_00215C56: ;
    /* cmp LO8(ebx), LO8(edx) - flags set for next jcc */
    edi = eax + 0x38;
    PUSH32(esp, 4);
    MEM32(eax + 0x20) = edi;
    POP32(esp, edi);
    MEM16(eax + 0x32) = LO16(edx);
    MEM8(eax + 0x31) = LO8(edx);
    MEM8(eax + 0x25) = LO8(edx);
    MEM32(eax + 0x18) = edx;
    MEM8(eax + 9) = 0x40;
    MEM8(ecx) = 0x30;
    MEM16(eax + 0x36) = LO16(edi);
    if (CMP_NE(LO8(ebx), LO8(edx))) goto loc_00215C88; /* jne: not equal / not zero */

loc_00215C7B: ;
    MEM32(eax + 0x10) = 0x215AC0;
    MEM8(eax + 0x30) = 0xA0;
    goto loc_00215C97;

loc_00215C88: ;
    MEM32(eax + 0x10) = 0x215913;
    MEM8(eax + 0x30) = 0xA3;
    SET_LO16(edx, ZX8(LO8(ebx)));

loc_00215C97: ;
    MEM16(eax + 0x34) = LO16(edx);

loc_00215C9B: ;
    PUSH32(esp, ecx);
    ecx = esi;
    MEM32(eax + 0x14) = esi;
    MEM32(eax + 0x1C) = edi;
    MEM8(eax + 0x24) = 2;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215CAD: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    POP32(esp, ebp);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00215CB4
 * Original: 0x00215CB4 - 0x00215CCD (25 bytes, 9 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215CB4(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215CB4: ;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    ecx = esi;
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_00215CC0: ;
    /* test MEM8(eax), 2 - flags set for next jcc */
    PUSH32(esp, esi);
    if (TEST_Z(MEM8(eax), 2)) { g_seh_ebp = ebp; sub_00215CCD(); return; } /* je: equal / zero */

loc_00215CC6: ;
    PUSH32(esp, 0); sub_00215850(); /* call 0x00215850 */

loc_00215CCB: ;
    g_seh_ebp = ebp; sub_00215CE0(); return; /* tail jmp 0x00215CE0 */

}

/**
 * sub_00215CCD
 * Original: 0x00215CCD - 0x00215CE0 (19 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215CCD(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215CCD: ;
    if (CMP_EQ(MEM16(eax + 0x3A), 0)) goto loc_00215CDB; /* je: equal / zero */

loc_00215CD4: ;
    PUSH32(esp, 0); sub_002155F9(); /* call 0x002155F9 */

loc_00215CD9: ;
    g_seh_ebp = ebp; sub_00215CE0(); return; /* tail jmp 0x00215CE0 */

loc_00215CDB: ;
    PUSH32(esp, 0); sub_00215BEB(); /* call 0x00215BEB */

}

/**
 * sub_00215CE0
 * Original: 0x00215CE0 - 0x00215E76 (406 bytes, 132 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215CE0(void)
{
    int _flags = 0; /* fallback flag var */

loc_00215CE0: ;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

    esi = eax;
    MEM8(esi + 0x3A) = MEM8(esi + 0x3A) & 0xFE;
    eax = 0; /* xor self */
    SET_LO8(eax, MEM8(esi + 3));
    ecx = 0; /* xor self */
    SET_LO8(ecx, LO8(eax));
    SET_LO8(ebx, 1);
    PUSH32(esp, 5);
    PUSH32(esp, eax);
    ecx--;
    SET_LO8(ebx, LO8(ebx) << LO8(ecx));
    ecx = edi;
    PUSH32(esp, 0); sub_0021357E(); /* call 0x0021357E */

loc_00215D10: ;
    SET_LO16(edx, ZX8(MEM8(esi + 3)));
    MEM8(esi + 5) = MEM8(esi + 5) | LO8(ebx);
    ecx = 0; /* xor self */
    eax = esi + 8;
    MEM32(esi + 0x18) = ecx;
    MEM32(esi + 0x20) = ecx;
    MEM32(esi + 0x1C) = ecx;
    MEM8(esi + 0x24) = LO8(ecx);
    MEM8(esi + 0x25) = LO8(ecx);
    MEM8(esi + 0x26) = LO8(ecx);
    MEM16(esi + 0x36) = LO16(ecx);
    PUSH32(esp, eax);
    ecx = edi;
    MEM8(eax) = 0x30;
    MEM8(esi + 9) = 0x40;
    MEM32(esi + 0x10) = 0x215CB4;
    MEM32(esi + 0x14) = edi;
    MEM8(esi + 0x30) = 0x23;
    MEM8(esi + 0x31) = 1;
    MEM16(esi + 0x32) = 0x10;
    MEM16(esi + 0x34) = LO16(edx);
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215D5E: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 12; return; /* ret 8 */

    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x40A2D0);
    esi = eax;
    { uint32_t _icall_t = MEM32(0x217B30); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215D7F: ;
    eax = MEM32(esp + 0x10);
    ebx = 0; /* xor self */
    if (CMP_GE(MEM32(eax + 4), ebx)) goto loc_00215D92; /* jge: greater or equal (signed >=) */

loc_00215D8A: ;
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_00215850(); /* call 0x00215850 */

loc_00215D90: ;
    goto loc_00215DF4;

loc_00215D92: ;
    SET_LO8(eax, MEM8(0x40A26A));
    /* cmp LO8(eax), 7 - flags set for next jcc */
    MEM8(esi + 2) = LO8(eax);
    if (CMP_BE(LO8(eax), 7)) goto loc_00215DA2; /* jbe: below or equal (unsigned <=) */

loc_00215D9E: ;
    MEM8(esi + 2) = 7;

loc_00215DA2: ;
    PUSH32(esp, ebx);
    ecx = edi;
    PUSH32(esp, 0); sub_00213B82(); /* call 0x00213B82 */

loc_00215DAA: ;
    eax = esi + 8;
    PUSH32(esp, eax);
    ecx = edi;
    MEM8(esi + 3) = 1;
    MEM8(eax) = 0x30;
    MEM8(esi + 9) = 0x40;
    MEM32(esi + 0x10) = 0x215B80;
    MEM32(esi + 0x14) = edi;
    MEM32(esi + 0x18) = ebx;
    MEM32(esi + 0x20) = ebx;
    MEM32(esi + 0x1C) = ebx;
    MEM8(esi + 0x24) = LO8(ebx);
    MEM8(esi + 0x25) = LO8(ebx);
    MEM8(esi + 0x26) = LO8(ebx);
    MEM8(esi + 0x30) = 0x23;
    MEM8(esi + 0x31) = 3;
    MEM16(esi + 0x32) = 8;
    MEM16(esi + 0x34) = 1;
    MEM16(esi + 0x36) = LO16(ebx);
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215DF4: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 12; return; /* ret 8 */

    esi = MEM32(esp + 8);
    eax = 0; /* xor self */
    if (CMP_GE(MEM32(esi + 4), eax)) goto loc_00215E1C; /* jge: greater or equal (signed >=) */

loc_00215E11: ;
    PUSH32(esp, MEM32(esp + 0xC));
    PUSH32(esp, 0); sub_00215850(); /* call 0x00215850 */

loc_00215E1A: ;
    goto loc_00215E72;

loc_00215E1C: ;
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x10);
    PUSH32(esp, 8);
    POP32(esp, ecx);
    PUSH32(esp, eax);
    MEM8(esi) = 0x30;
    MEM8(esi + 1) = 0x40;
    MEM32(esi + 8) = 0x215D64;
    MEM32(esi + 0xC) = edi;
    MEM32(esi + 0x10) = eax;
    MEM32(esi + 0x18) = 0x40A268;
    MEM32(esi + 0x14) = ecx;
    MEM8(esi + 0x1C) = 2;
    MEM8(esi + 0x1D) = 1;
    MEM8(esi + 0x1E) = LO8(eax);
    MEM8(esi + 0x28) = 0xA0;
    MEM8(esi + 0x29) = 6;
    MEM16(esi + 0x2A) = 0x2900;
    MEM16(esi + 0x2C) = LO16(eax);
    MEM16(esi + 0x2E) = LO16(ecx);
    PUSH32(esp, 0); sub_002154DD(); /* call 0x002154DD */

loc_00215E69: ;
    PUSH32(esp, esi);
    ecx = edi;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215E71: ;
    POP32(esp, edi);

loc_00215E72: ;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

}

/**
 * sub_00215E76
 * Original: 0x00215E76 - 0x00215EA9 (51 bytes, 18 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215E76(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00215E76: ;
    PUSH32(esp, ebp);
    ebp = esp;
    ecx = MEM32(ebp + 0xC);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_0021427A(); /* call 0x0021427A */

loc_00215E84: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x40A2D0);
    edi = eax;
    { uint32_t _icall_t = MEM32(0x217B30); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00215E91: ;
    esi = MEM32(ebp + 8);
    ebx = 0; /* xor self */
    if (CMP_GE(MEM32(esi + 4), ebx)) { g_seh_ebp = ebp; sub_00215EA9(); return; } /* jge: greater or equal (signed >=) */

loc_00215E9B: ;
    PUSH32(esp, MEM32(ebp + 0xC));
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00215747(); /* call 0x00215747 */

loc_00215EA4: ;
    g_seh_ebp = ebp; sub_00215F5B(); return; /* tail jmp 0x00215F5B */

}

/**
 * sub_00215E9B
 * Original: 0x00215E9B - 0x00215EA9 (14 bytes, 4 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215E9B(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215E9B: ;
    PUSH32(esp, MEM32(ebp + 0xC));
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00215747(); /* call 0x00215747 */

loc_00215EA4: ;
    g_seh_ebp = ebp; sub_00215F5B(); return; /* tail jmp 0x00215F5B */

}

/**
 * sub_00215EA9
 * Original: 0x00215EA9 - 0x00215F5B (178 bytes, 54 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215EA9(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215EA9: ;
    eax = 0x40A268;
    ecx = 0; /* xor self */

loc_00215EB0: ;
    eax = ZX8(MEM8(eax));
    ecx = ecx + eax;
    eax = ecx + 0x40A268;
    if (CMP_NE(MEM8(eax + 1), 5)) goto loc_00215EB0; /* jne: not equal / not zero */

loc_00215EC1: ;
    if (CMP_A(MEM16(eax + 4), 4)) goto loc_00215ED0; /* ja: above (unsigned >) */

loc_00215EC8: ;
    SET_LO8(ecx, MEM8(eax + 4));
    MEM8(edi + 7) = LO8(ecx);
    goto loc_00215ED4;

loc_00215ED0: ;
    MEM8(edi + 7) = 4;

loc_00215ED4: ;
    SET_LO8(ecx, MEM8(eax + 2));
    MEM8(edi + 1) = LO8(ecx);
    MEM8(esi) = 0x20;
    MEM8(esi + 1) = 2;
    MEM32(esi + 8) = ebx;
    SET_LO8(ecx, MEM8(eax + 2));
    MEM8(esi + 0x15) = LO8(ecx);
    SET_LO8(eax, MEM8(eax + 3));
    ecx = MEM32(ebp + 0xC);
    SET_LO8(eax, LO8(eax) & 3);
    MEM8(esi + 0x16) = LO8(eax);
    MEM8(esi + 0x17) = 0x10;
    SET_LO16(eax, ZX8(MEM8(edi + 7)));
    PUSH32(esp, esi);
    MEM16(esi + 0x1C) = LO16(eax);
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215F08: ;
    if (TEST_S(eax, eax)) { g_seh_ebp = ebp; sub_00215E9B(); return; } /* jl: less (signed <) */

loc_00215F0C: ;
    eax = MEM32(esi + 0x10);
    MEM32(edi + 0x3C) = eax;
    edi = MEM32(ebp + 0xC);
    PUSH32(esp, ebx);
    MEM8(esi) = 0x30;
    MEM8(esi + 1) = 0x40;
    MEM32(esi + 8) = 0x215DFA;
    MEM32(esi + 0xC) = edi;
    MEM32(esi + 0x10) = ebx;
    MEM32(esi + 0x18) = ebx;
    MEM32(esi + 0x14) = ebx;
    MEM8(esi + 0x1C) = LO8(ebx);
    MEM8(esi + 0x1D) = LO8(ebx);
    MEM8(esi + 0x1E) = LO8(ebx);
    MEM8(esi + 0x28) = LO8(ebx);
    MEM8(esi + 0x29) = 9;
    MEM16(esi + 0x2A) = 1;
    MEM16(esi + 0x2C) = LO16(ebx);
    MEM16(esi + 0x2E) = LO16(ebx);
    PUSH32(esp, 0); sub_002154DD(); /* call 0x002154DD */

loc_00215F53: ;
    PUSH32(esp, esi);
    ecx = edi;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

}

/**
 * sub_00215F5B
 * Original: 0x00215F5B - 0x00216048 (237 bytes, 77 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00215F5B(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00215F5B: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    POP32(esp, ebp);
    esp += 12; return; /* ret 8 */

    /* test MEM8(0x40A31C), 1 - flags set for next jcc */
    eax = 0x40A31C;
    if (TEST_Z(MEM8(0x40A31C), 1)) goto loc_00215F94; /* je: equal / zero */

loc_00215F89: ;
    ecx = eax;

loc_00215F8B: ;
    ecx = ecx + 0x40;
    esi++;
    if (TEST_NZ(MEM8(ecx), 1)) goto loc_00215F8B; /* jne: not equal / not zero */

loc_00215F94: ;
    MEM16(0x40A31A) = MEM16(0x40A31A) + 1;
    PUSH32(esp, ebp);
    ebp = MEM32(esp + 0x10);
    esi = esi << 6;
    PUSH32(esp, edi);
    esi = esi + eax;
    PUSH32(esp, esi);
    ecx = ebp;
    PUSH32(esp, 0); sub_0021427E(); /* call 0x0021427E */

loc_00215FAE: ;
    SET_LO8(eax, MEM8(esi));
    SET_LO8(eax, LO8(eax) & 0xF5);
    edi = esi + 8;
    SET_LO8(eax, LO8(eax) | 1);
    PUSH32(esp, edi);
    ecx = ebp;
    MEM8(esi) = LO8(eax);
    MEM8(esi + 5) = LO8(ebx);
    MEM8(esi + 6) = LO8(ebx);
    MEM8(edi) = 0x20;
    MEM8(esi + 9) = 0x82;
    MEM32(esi + 0x10) = ebx;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00215FD1: ;
    PUSH32(esp, 0x30);
    POP32(esp, eax);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    PUSH32(esp, 0x215489);
    PUSH32(esp, 0x40A2F8);
    MEM8(edi) = LO8(eax);
    MEM8(esi + 9) = 0x40;
    MEM32(esi + 0x10) = 0x215E76;
    MEM32(esi + 0x14) = ebp;
    MEM32(esi + 0x18) = ebx;
    MEM32(esi + 0x20) = 0x40A268;
    MEM32(esi + 0x1C) = eax;
    MEM8(esi + 0x24) = 2;
    MEM8(esi + 0x25) = 1;
    MEM8(esi + 0x26) = LO8(ebx);
    MEM8(esi + 0x30) = 0x80;
    MEM8(esi + 0x31) = 6;
    MEM16(esi + 0x32) = 0x200;
    MEM16(esi + 0x34) = LO16(ebx);
    MEM16(esi + 0x36) = LO16(eax);
    { uint32_t _icall_t = MEM32(0x217B18); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216023: ;
    PUSH32(esp, ebx);
    PUSH32(esp, 0); sub_002154DD(); /* call 0x002154DD */

loc_00216029: ;
    PUSH32(esp, edi);
    ecx = ebp;
    PUSH32(esp, 0); sub_00214621(); /* call 0x00214621 */

loc_00216031: ;
    POP32(esp, edi);
    POP32(esp, ebp);
    goto loc_00216043;

loc_00216035: ;
    ecx = MEM32(esp + 0xC);
    PUSH32(esp, 0x80000100u);
    PUSH32(esp, 0); sub_00213B82(); /* call 0x00213B82 */

loc_00216043: ;
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00216048
 * Original: 0x00216048 - 0x002160D7 (143 bytes, 56 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216048(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216048: ;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    /* cmp MEM8(esi + 0x460), 0 - flags set for next jcc */
    ebp = edx;
    SET_LO8(ebx, 1);
    if (CMP_BE(MEM8(esi + 0x460), 0)) goto loc_002160D2; /* jbe: below or equal (unsigned <=) */

loc_0021605B: ;
    MEM8(esp + 0xC) = LO8(ebx);
    PUSH32(esp, edi);

loc_00216060: ;
    SET_LO16(eax, ZX8(LO8(ebx)));
    if (TEST_Z(MEM16(ebp), LO16(eax))) goto loc_002160BD; /* je: equal / zero */

loc_0021606A: ;
    ecx = 0; /* xor self */
    SET_LO16(ecx, MEM16(ebp + 2));
    ecx = ecx & eax;
    if (TEST_Z(LO16(ecx), LO16(ecx))) goto loc_0021609F; /* je: equal / zero */

loc_00216077: ;
    ecx = esi + 0x461;
    SET_LO8(eax, MEM8(ecx));
    if (TEST_Z(LO8(ebx), LO8(eax))) goto loc_0021608F; /* je: equal / zero */

loc_00216083: ;
    PUSH32(esp, MEM32(esp + 0x10));
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00213A6A(); /* call 0x00213A6A */

loc_0021608D: ;
    goto loc_00216093;

loc_0021608F: ;
    SET_LO8(eax, LO8(eax) | LO8(ebx));
    MEM8(ecx) = LO8(eax);

loc_00216093: ;
    PUSH32(esp, MEM32(esp + 0x10));
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_00213748(); /* call 0x00213748 */

loc_0021609D: ;
    goto loc_002160BD;

loc_0021609F: ;
    edi = esi + 0x461;
    SET_LO8(eax, MEM8(edi));
    if (TEST_Z(LO8(ebx), LO8(eax))) goto loc_002160BD; /* je: equal / zero */

loc_002160AB: ;
    PUSH32(esp, MEM32(esp + 0x10));
    SET_LO8(ecx, LO8(ebx));
    SET_LO8(ecx, ~LO8(ecx));
    SET_LO8(ecx, LO8(ecx) & LO8(eax));
    PUSH32(esp, esi);
    MEM8(edi) = LO8(ecx);
    PUSH32(esp, 0); sub_00213A6A(); /* call 0x00213A6A */

loc_002160BD: ;
    MEM8(esp + 0x10) = MEM8(esp + 0x10) + 1;
    SET_LO8(eax, MEM8(esp + 0x10));
    SET_LO8(ebx, LO8(ebx) << 1);
    SET_LO8(eax, LO8(eax) - 1);
    if (CMP_B(LO8(eax), MEM8(esi + 0x460))) goto loc_00216060; /* jb: below (unsigned <) */

loc_002160D1: ;
    POP32(esp, edi);

loc_002160D2: ;
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_002160D7
 * Original: 0x002160D7 - 0x00216124 (77 bytes, 25 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002160D7(void)
{
    uint32_t ebp;

loc_002160D7: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    ebp = esp;
    eax = MEM32(ebp + 8);
    ecx = MEM32(ebp + 0x10);
    edx = MEM32(ebp + 0xC);
    MEM32(eax + 0x470) = ecx;
    ecx = MEM32(ebp + 0x14);
    PUSH32(esp, esi);
    MEM32(eax + 0x474) = ecx;
    ecx = MEM32(eax);
    MEM16(ebp + 8) = 0x10;
    esi = MEM32(ebp + 8);
    MEM32(ecx + edx * 4 + 0x50) = esi;
    esi = eax + 0x4A0;
    PUSH32(esp, esi);
    edx = edx | 0xFFFFFFFFu;
    PUSH32(esp, edx);
    ecx = 0xFFF0BDC0u;
    PUSH32(esp, ecx);
    eax = eax + 0x478;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x217B2C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_0021611F: ;
    POP32(esp, esi);
    POP32(esp, ebp);
    esp += 20; return; /* ret 16 */

}

/**
 * sub_00216124
 * Original: 0x00216124 - 0x00216141 (29 bytes, 11 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216124(void)
{
    uint32_t ebp;

loc_00216124: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    eax = MEM32(ebp + 8);
    eax = MEM32(eax);
    ecx = MEM32(ebp + 0xC);
    MEM16(ebp + -4) = 1;
    edx = MEM32(ebp + -4);
    MEM32(eax + ecx * 4 + 0x50) = edx;
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 12; return; /* ret 8 */

}

/**
 * sub_0021616C
 * Original: 0x0021616C - 0x0021622B (191 bytes, 64 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021616C(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_0021616C: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x1C;
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi);
    edx = ZX8(MEM8(esi + 0x460));
    ecx = eax + 0x54;
    eax = MEM32(ecx);
    PUSH32(esp, edi);
    eax = 0; /* xor self */
    /* test edx, edx - flags set for next jcc */
    edi = ebp + -8;
    MEM32(edi) = eax; edi += 4; /* stosd */
    MEM32(ebp + -12) = 1;
    if (CMP_BE(edx & edx, 0)) goto loc_0021621D; /* jbe: below or equal (unsigned <=) */

loc_00216199: ;
    MEM32(ebp + -16) = ecx;
    MEM32(ebp + -20) = edx;
    PUSH32(esp, ebx);

loc_002161A0: ;
    edi = MEM32(ecx);
    MEM32(ebp + -4) = edi;
    if (TEST_Z(MEM8(ebp + -2), 0x10)) goto loc_002161EA; /* je: equal / zero */

loc_002161AB: ;
    ecx = MEM32(esi + 0x474);
    ebx = esi + 0x470;
    eax = MEM32(ebx);
    /* test eax, eax - flags set for next jcc */
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -24) = ecx;
    if (TEST_Z(eax, eax)) goto loc_002161EA; /* je: equal / zero */

loc_002161C3: ;
    eax = esi + 0x478;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x217B30); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002161D0: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, MEM32(ebp + -24));
    MEM32(ebx) = MEM32(ebx) & 0;
    MEM32(esi + 0x474) = MEM32(esi + 0x474) & 0;
    edi = edi & 0x200;
    edi = edi << 0xF;
    PUSH32(esp, edi);
    { uint32_t _icall_t = MEM32(ebp + -28); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002161EA: ;
    if (TEST_Z(MEM8(ebp + -2), 1)) goto loc_00216201; /* je: equal / zero */

loc_002161F0: ;
    eax = MEM32(ebp + -12);
    MEM16(ebp + -8) = MEM16(ebp + -8) | LO16(eax);
    if (TEST_Z(MEM8(ebp + -4), 1)) goto loc_00216201; /* je: equal / zero */

loc_002161FD: ;
    MEM16(ebp + -6) = MEM16(ebp + -6) | LO16(eax);

loc_00216201: ;
    MEM16(ebp + -4) = MEM16(ebp + -4) & 0;
    ecx = MEM32(ebp + -16);
    eax = MEM32(ebp + -4);
    MEM32(ebp + -12) = MEM32(ebp + -12) << 1;
    MEM32(ecx) = eax;
    ecx = ecx + 4;
    MEM32(ebp + -20) = MEM32(ebp + -20) - 1;
    MEM32(ebp + -16) = ecx;
    if ((MEM32(ebp + -20) != 0)) goto loc_002161A0; /* jne: not equal / not zero */

loc_0021621C: ;
    POP32(esp, ebx);

loc_0021621D: ;
    edx = ebp + -8;
    ecx = esi;
    PUSH32(esp, 0); sub_00216048(); /* call 0x00216048 */

loc_00216227: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_0021622B
 * Original: 0x0021622B - 0x00216248 (29 bytes, 7 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021622B(void)
{

loc_0021622B: ;
    eax = MEM32(esp + 4);
    MEM32(eax + 8) = MEM32(eax + 8) | 0xFFFFFFFFu;
    MEM8(eax + 0x1F) = MEM8(eax + 0x1F) | 0xFF;
    ecx = MEM32(0xC6B9AC);
    MEM32(eax + 0x14) = ecx;
    MEM32(0xC6B9AC) = eax;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00216248
 * Original: 0x00216248 - 0x0021625C (20 bytes, 7 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216248(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216248: ;
    /* cmp MEM8(edx + 0x11), 0 - flags set for next jcc */
    eax = MEM32(ecx);
    PUSH32(esp, esi);
    if (CMP_NE(MEM8(edx + 0x11), 0)) { g_seh_ebp = ebp; sub_0021625C(); return; } /* jne: not equal / not zero */

loc_00216251: ;
    esi = ecx + 0x40C;
    eax = eax + 0x20;
    g_seh_ebp = ebp; sub_00216265(); return; /* tail jmp 0x00216265 */

}

/**
 * sub_0021625C
 * Original: 0x0021625C - 0x00216265 (9 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021625C(void)
{

loc_0021625C: ;
    esi = ecx + 0x410;
    eax = eax + 0x28;

}

/**
 * sub_00216265
 * Original: 0x00216265 - 0x00216285 (32 bytes, 14 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216265(void)
{
    int _flags = 0; /* fallback flag var */

loc_00216265: ;
    ecx = MEM32(esi);
    MEM32(edx + 0x18) = ecx;
    MEM32(esi) = edx;
    ecx = MEM32(edx + 0x18);
    /* test ecx, ecx - flags set for next jcc */
    POP32(esp, esi);
    if (TEST_NZ(ecx, ecx)) goto loc_00216279; /* jne: not equal / not zero */

loc_00216274: ;
    MEM32(edx + 0xC) = MEM32(edx + 0xC) & ecx;
    goto loc_0021627F;

loc_00216279: ;
    ecx = MEM32(ecx + 0x14);
    MEM32(edx + 0xC) = ecx;

loc_0021627F: ;
    ecx = MEM32(edx + 0x14);
    MEM32(eax) = ecx;
    esp += 4; return; /* ret */

}

/**
 * sub_00216285
 * Original: 0x00216285 - 0x0021629A (21 bytes, 8 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216285(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216285: ;
    /* cmp MEM8(edx + 0x11), 0 - flags set for next jcc */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    if (CMP_NE(MEM8(edx + 0x11), 0)) { g_seh_ebp = ebp; sub_0021629A(); return; } /* jne: not equal / not zero */

loc_0021628D: ;
    esi = ecx + 0x40C;
    ecx = MEM32(ecx);
    ecx = ecx + 0x20;
    g_seh_ebp = ebp; sub_002162A5(); return; /* tail jmp 0x002162A5 */

}

/**
 * sub_0021629A
 * Original: 0x0021629A - 0x002162A5 (11 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021629A(void)
{

loc_0021629A: ;
    esi = ecx + 0x410;
    ecx = MEM32(ecx);
    ecx = ecx + 0x28;

}

/**
 * sub_002162A5
 * Original: 0x002162A5 - 0x002162D5 (48 bytes, 22 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002162A5(void)
{
    int _flags = 0; /* fallback flag var */

loc_002162A5: ;
    eax = MEM32(esi);
    edi = 0; /* xor self */

loc_002162A9: ;
    if (CMP_EQ(edx, eax)) goto loc_002162B6; /* je: equal / zero */

loc_002162AD: ;
    edi = eax;
    eax = MEM32(eax + 0x18);
    if (TEST_NZ(eax, eax)) goto loc_002162A9; /* jne: not equal / not zero */

loc_002162B6: ;
    if (TEST_Z(edi, edi)) goto loc_002162C8; /* je: equal / zero */

loc_002162BA: ;
    ecx = MEM32(eax + 0x18);
    MEM32(edi + 0x18) = ecx;
    eax = MEM32(eax + 0xC);
    MEM32(edi + 0xC) = eax;
    goto loc_002162D2;

loc_002162C8: ;
    edx = MEM32(eax + 0x18);
    MEM32(esi) = edx;
    eax = MEM32(eax + 0xC);
    MEM32(ecx) = eax;

loc_002162D2: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_002162D5
 * Original: 0x002162D5 - 0x002162EB (22 bytes, 12 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002162D5(void)
{
    int _flags = 0; /* fallback flag var */

loc_002162D5: ;
    eax = 0; /* xor self */
    if (CMP_BE(ecx & ecx, 0)) goto loc_002162EA; /* jbe: below or equal (unsigned <=) */

loc_002162DB: ;
    PUSH32(esp, esi);

loc_002162DC: ;
    esi = edx;
    esi = esi & 1;
    edx = edx >> 1;
    ecx--;
    eax = esi + eax * 2;
    if ((ecx != 0)) goto loc_002162DC; /* jne: not equal / not zero */

loc_002162E9: ;
    POP32(esp, esi);

loc_002162EA: ;
    esp += 4; return; /* ret */

}

/**
 * sub_002162EB
 * Original: 0x002162EB - 0x00216313 (40 bytes, 18 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002162EB(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002162EB: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    esi = ecx;
    SET_LO8(ecx, MEM8(ebp + 8));
    /* cmp LO8(ecx), 0x20 - flags set for next jcc */
    PUSH32(esp, edi);
    edi = edx;
    if (CMP_B(LO8(ecx), 0x20)) { g_seh_ebp = ebp; sub_00216313(); return; } /* jb: below (unsigned <) */

loc_002162FD: ;
    edx = ZX8(LO8(ecx));
    PUSH32(esp, 5);
    edx = edx - 0x20;
    POP32(esp, ecx);
    PUSH32(esp, 0); sub_002162D5(); /* call 0x002162D5 */

loc_0021630B: ;
    ecx = MEM32(esi + 8);
    MEM32(ecx + eax * 4) = edi;
    g_seh_ebp = ebp; sub_0021635C(); return; /* tail jmp 0x0021635C */

}

/**
 * sub_00216313
 * Original: 0x00216313 - 0x0021635C (73 bytes, 28 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216313(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216313: ;
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) & 0;
    SET_LO8(eax, LO8(ecx));
    SET_LO8(eax, LO8(eax) << 1);
    PUSH32(esp, ebx);
    SET_LO8(eax, LO8(eax) + 1);
    SET_LO8(ebx, LO8(ecx));
    MEM8(ebp + -4) = LO8(eax);
    SET_LO8(ebx, LO8(ebx) << 1);

loc_00216325: ;
    eax = ZX8(LO8(eax));
    eax = eax << 4;
    eax = eax + esi + 0xC;
    if (CMP_NE(MEM32(eax + 8), 0)) goto loc_00216343; /* jne: not equal / not zero */

loc_00216335: ;
    PUSH32(esp, MEM32(ebp + -4));
    edx = edi;
    ecx = esi;
    PUSH32(esp, 0); sub_002162EB(); /* call 0x002162EB */

loc_00216341: ;
    goto loc_00216349;

loc_00216343: ;
    eax = MEM32(eax + 0xC);
    MEM32(eax + 0xC) = edi;

loc_00216349: ;
    SET_LO8(eax, LO8(ebx));
    /* test LO8(eax), LO8(eax) - flags set for next jcc */
    MEM8(ebp + -4) = LO8(eax);
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_0021635B; /* je: equal / zero */

loc_00216352: ;
    MEM8(ebp + 0xB) = MEM8(ebp + 0xB) + 1;
    if (CMP_B(MEM8(ebp + 0xB), 2)) goto loc_00216325; /* jb: below (unsigned <) */

loc_0021635B: ;
    POP32(esp, ebx);

}

/**
 * sub_0021635C
 * Original: 0x0021635C - 0x00216362 (6 bytes, 4 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021635C(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021635C: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00216362
 * Original: 0x00216362 - 0x002163F3 (145 bytes, 52 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216362(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00216362: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x10;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = edx;
    /* cmp MEM8(esi + 0x11), 3 - flags set for next jcc */
    PUSH32(esp, edi);
    if (CMP_NE(MEM8(esi + 0x11), 3)) goto loc_002163C6; /* jne: not equal / not zero */

loc_00216373: ;
    SET_LO8(eax, 0x20);
    if (CMP_AE(MEM8(esi + 0x13), LO8(eax))) goto loc_00216383; /* jae: above or equal (unsigned >=) */

loc_0021637A: ;
    SET_LO8(edx, MEM8(esi + 0x13));

loc_0021637D: ;
    SET_LO8(eax, LO8(eax) >> 1);
    if (CMP_A(LO8(eax), LO8(edx))) goto loc_0021637D; /* ja: above (unsigned >) */

loc_00216383: ;
    SET_LO8(ebx, LO8(eax));
    SET_LO8(ebx, LO8(ebx) << 1);
    SET_LO8(ebx, LO8(ebx) - 1);
    /* cmp LO8(eax), LO8(ebx) - flags set for next jcc */
    edi = 0x2EE0;
    MEM8(ebp + -1) = LO8(eax);
    if (CMP_A(LO8(eax), LO8(ebx))) goto loc_002163D2; /* ja: above (unsigned >) */

loc_00216395: ;
    eax = ZX8(LO8(eax));
    eax = eax << 4;
    edx = eax + ecx + 0x10;

loc_0021639F: ;
    eax = 0; /* xor self */
    SET_LO16(eax, MEM16(edx + -2));
    SET_LO16(eax, LO16(eax) + MEM16(edx + 2));
    SET_LO16(eax, LO16(eax) + MEM16(edx));
    if (CMP_AE(LO16(eax), LO16(edi))) goto loc_002163B9; /* jae: above or equal (unsigned >=) */

loc_002163B1: ;
    edi = eax;
    SET_LO8(eax, MEM8(ebp + -1));
    MEM8(ebp + -5) = LO8(eax);

loc_002163B9: ;
    MEM8(ebp + -1) = MEM8(ebp + -1) + 1;
    edx = edx + 0x10;
    if (CMP_BE(MEM8(ebp + -1), LO8(ebx))) goto loc_0021639F; /* jbe: below or equal (unsigned <=) */

loc_002163C4: ;
    goto loc_002163D2;

loc_002163C6: ;
    SET_LO16(edi, MEM16(ecx + 0x10));
    SET_LO16(edi, LO16(edi) + MEM16(ecx + 0xE));
    MEM8(ebp + -5) = MEM8(ebp + -5) & 0;

loc_002163D2: ;
    SET_LO16(eax, MEM16(esi + 0x22));
    edi = ZX16(LO16(edi));
    edx = ZX16(LO16(eax));
    edx = edx + edi;
    edi = ZX16(MEM16(ecx + 0x414));
    if (CMP_LE(edx, edi)) { g_seh_ebp = ebp; sub_002163F3(); return; } /* jle: less or equal (signed <=) */

loc_002163E9: ;
    eax = 0x80000800u;
    g_seh_ebp = ebp; sub_00216539(); return; /* tail jmp 0x00216539 */

}

/**
 * sub_002163F3
 * Original: 0x002163F3 - 0x00216539 (326 bytes, 115 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002163F3(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002163F3: ;
    SET_LO8(edx, MEM8(ebp + -5));
    edi = ZX8(LO8(edx));
    edi = edi << 4;
    edi = edi + ecx + 0xC;
    MEM8(esi + 0x12) = LO8(edx);
    MEM16(edi + 2) = MEM16(edi + 2) + LO16(eax);
    if (TEST_NZ(LO8(edx), LO8(edx))) goto loc_00216412; /* jne: not equal / not zero */

loc_0021640B: ;
    SET_LO8(eax, 1);
    MEM8(ebp + -1) = LO8(eax);
    goto loc_00216423;

loc_00216412: ;
    SET_LO8(eax, LO8(edx));
    SET_LO8(eax, LO8(eax) << 1);
    MEM8(ebp + -1) = LO8(eax);
    SET_LO8(eax, LO8(edx));
    SET_LO8(eax, LO8(eax) << 1);
    SET_LO8(eax, LO8(eax) + 1);
    if (CMP_A(LO8(eax), 0x40)) goto loc_00216463; /* ja: above (unsigned >) */

loc_00216423: ;
    if (CMP_A(MEM8(ebp + -1), LO8(eax))) goto loc_00216455; /* ja: above (unsigned >) */

loc_00216428: ;
    edx = ZX8(MEM8(ebp + -1));
    edx = edx << 4;
    edx = edx + ecx + 0x12;
    MEM32(ebp + -16) = edx;
    SET_LO8(edx, LO8(eax));
    SET_LO8(edx, LO8(edx) - MEM8(ebp + -1));
    SET_LO8(edx, LO8(edx) + 1);
    edx = ZX8(LO8(edx));
    MEM32(ebp + -12) = edx;
    edx = MEM32(ebp + -16);

loc_00216446: ;
    SET_LO16(ebx, MEM16(esi + 0x22));
    MEM16(edx) = MEM16(edx) + LO16(ebx);
    edx = edx + 0x10;
    MEM32(ebp + -12) = MEM32(ebp + -12) - 1;
    if ((MEM32(ebp + -12) != 0)) goto loc_00216446; /* jne: not equal / not zero */

loc_00216455: ;
    MEM8(ebp + -1) = MEM8(ebp + -1) << 1;
    SET_LO8(eax, LO8(eax) << 1);
    SET_LO8(eax, LO8(eax) + 1);
    if (CMP_BE(LO8(eax), 0x40)) goto loc_00216423; /* jbe: below or equal (unsigned <=) */

loc_00216460: ;
    SET_LO8(edx, MEM8(ebp + -5));

loc_00216463: ;
    /* cmp LO8(edx), 1 - flags set for next jcc */
    MEM8(ebp + -1) = LO8(edx);
    if (CMP_BE(LO8(edx), 1)) goto loc_002164CC; /* jbe: below or equal (unsigned <=) */

loc_0021646B: ;
    SET_LO8(eax, MEM8(ebp + -1));
    edx = ZX8(MEM8(ebp + -1));
    SET_LO8(eax, LO8(eax) ^ 1);
    eax = ZX8(LO8(eax));
    edx = edx << 4;
    edx = edx + ecx + 0xC;
    eax = eax << 4;
    ebx = 0; /* xor self */
    SET_LO16(ebx, MEM16(edx + 4));
    SET_LO16(edx, MEM16(edx + 2));
    eax = eax + ecx + 0xC;
    MEM16(ebp + -12) = LO16(edx);
    edx = ZX16(MEM16(eax + 4));
    eax = ZX16(MEM16(eax + 2));
    edx = edx + eax;
    eax = ZX16(MEM16(ebp + -12));
    MEM32(ebp + -16) = ebx;
    ebx = ZX16(LO16(ebx));
    eax = eax + ebx;
    if (CMP_LE(eax, edx)) goto loc_002164CC; /* jle: less or equal (signed <=) */

loc_002164AD: ;
    SET_LO8(eax, MEM8(ebp + -1));
    ebx = MEM32(ebp + -16);
    edx = MEM32(ebp + -12);
    SET_LO8(eax, LO8(eax) >> 1);
    edx = edx + ebx;
    ebx = ZX8(LO8(eax));
    ebx = ebx << 4;
    /* cmp LO8(eax), 1 - flags set for next jcc */
    MEM16(ebx + ecx + 0x10) = LO16(edx);
    MEM8(ebp + -1) = LO8(eax);
    if (CMP_A(LO8(eax), 1)) goto loc_0021646B; /* ja: above (unsigned >) */

loc_002164CC: ;
    if (CMP_NE(MEM8(ebp + -1), 1)) goto loc_002164DE; /* jne: not equal / not zero */

loc_002164D2: ;
    SET_LO16(eax, MEM16(ecx + 0x20));
    SET_LO16(eax, LO16(eax) + MEM16(ecx + 0x1E));
    MEM16(ecx + 0x10) = LO16(eax);

loc_002164DE: ;
    eax = MEM32(edi + 8);
    edx = 0; /* xor self */
    if (CMP_NE(eax, edx)) goto loc_0021651D; /* jne: not equal / not zero */

loc_002164E7: ;
    SET_LO8(eax, MEM8(ebp + -5));
    goto loc_002164F0;

loc_002164EC: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_002164FE; /* je: equal / zero */

loc_002164F0: ;
    SET_LO8(eax, LO8(eax) >> 1);
    ebx = ZX8(LO8(eax));
    ebx = ebx << 4;
    if (CMP_EQ(MEM32(ebx + ecx + 0x14), edx)) goto loc_002164EC; /* je: equal / zero */

loc_002164FE: ;
    eax = ZX8(LO8(eax));
    eax = eax << 4;
    eax = MEM32(eax + ecx + 0x14);
    if (CMP_EQ(eax, edx)) goto loc_00216512; /* je: equal / zero */

loc_0021650C: ;
    eax = MEM32(eax + 0x14);
    MEM32(esi + 0xC) = eax;

loc_00216512: ;
    MEM32(edi + 8) = esi;
    MEM32(edi + 0xC) = esi;
    MEM32(esi + 0x18) = edx;
    goto loc_0021652C;

loc_0021651D: ;
    MEM32(esi + 0x18) = eax;
    MEM32(edi + 8) = esi;
    eax = MEM32(esi + 0x18);
    eax = MEM32(eax + 0x14);
    MEM32(esi + 0xC) = eax;

loc_0021652C: ;
    PUSH32(esp, MEM32(ebp + -5));
    edx = MEM32(esi + 0x14);
    PUSH32(esp, 0); sub_002162EB(); /* call 0x002162EB */

loc_00216537: ;
    eax = 0; /* xor self */

}

/**
 * sub_00216539
 * Original: 0x00216539 - 0x0021653E (5 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216539(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216539: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_0021653E
 * Original: 0x0021653E - 0x002165D5 (151 bytes, 58 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021653E(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_0021653E: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x14;
    SET_LO16(eax, MEM16(edx + 0x22));
    MEM32(ebp + -4) = MEM32(ebp + -4) & 0;
    PUSH32(esp, ebx);
    SET_LO8(ebx, MEM8(edx + 0x12));
    MEM16(ebp + -16) = LO16(eax);
    eax = ZX8(LO8(ebx));
    PUSH32(esp, esi);
    eax = eax << 4;
    esi = ecx;
    PUSH32(esp, edi);
    edi = eax + esi + 0xC;
    eax = MEM32(edi + 8);
    MEM32(ebp + -20) = edx;
    MEM8(ebp + -12) = LO8(ebx);

loc_0021656B: ;
    if (CMP_EQ(eax, edx)) goto loc_00216579; /* je: equal / zero */

loc_0021656F: ;
    MEM32(ebp + -4) = eax;
    eax = MEM32(eax + 0x18);
    if (TEST_NZ(eax, eax)) goto loc_0021656B; /* jne: not equal / not zero */

loc_00216579: ;
    ecx = MEM32(eax + 0x18);
    if (TEST_NZ(ecx, ecx)) goto loc_002165B4; /* jne: not equal / not zero */

loc_00216580: ;
    ecx = MEM32(ebp + -4);
    edx = 0; /* xor self */
    /* test LO8(ebx), LO8(ebx) - flags set for next jcc */
    MEM32(edi + 0xC) = ecx;
    MEM32(ebp + -8) = edx;
    if (TEST_Z(LO8(ebx), LO8(ebx))) goto loc_002165BC; /* je: equal / zero */

loc_0021658F: ;
    SET_LO8(edx, LO8(ebx));
    goto loc_00216597;

loc_00216593: ;
    if (TEST_Z(LO8(edx), LO8(edx))) goto loc_002165A6; /* je: equal / zero */

loc_00216597: ;
    SET_LO8(edx, LO8(edx) >> 1);
    ecx = ZX8(LO8(edx));
    ecx = ecx << 4;
    if (CMP_EQ(MEM32(ecx + esi + 0x14), 0)) goto loc_00216593; /* je: equal / zero */

loc_002165A6: ;
    ecx = ZX8(LO8(edx));
    ecx = ecx << 4;
    ecx = MEM32(ecx + esi + 0x14);
    if (TEST_Z(ecx, ecx)) goto loc_002165B9; /* je: equal / zero */

loc_002165B4: ;
    edx = MEM32(ecx + 0x14);
    goto loc_002165BC;

loc_002165B9: ;
    edx = MEM32(ebp + -8);

loc_002165BC: ;
    ecx = MEM32(ebp + -4);
    /* test ecx, ecx - flags set for next jcc */
    eax = MEM32(eax + 0x18);
    if (TEST_NZ(ecx, ecx)) { g_seh_ebp = ebp; sub_002165D5(); return; } /* jne: not equal / not zero */

loc_002165C6: ;
    PUSH32(esp, MEM32(ebp + -12));
    ecx = esi;
    MEM32(edi + 8) = eax;
    PUSH32(esp, 0); sub_002162EB(); /* call 0x002162EB */

loc_002165D3: ;
    g_seh_ebp = ebp; sub_002165DB(); return; /* tail jmp 0x002165DB */

}

/**
 * sub_002165D5
 * Original: 0x002165D5 - 0x002165DB (6 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002165D5(void)
{

loc_002165D5: ;
    MEM32(ecx + 0x18) = eax;
    MEM32(ecx + 0xC) = edx;

}

/**
 * sub_002165DB
 * Original: 0x002165DB - 0x0021674D (370 bytes, 129 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002165DB(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002165DB: ;
    SET_LO16(eax, MEM16(ebp + -16));
    MEM16(edi + 2) = MEM16(edi + 2) - LO16(eax);
    if (TEST_NZ(LO8(ebx), LO8(ebx))) goto loc_002166AE; /* jne: not equal / not zero */

loc_002165EB: ;
    SET_LO8(eax, 1);
    SET_LO8(ecx, LO8(eax));

loc_002165EF: ;
    if (CMP_A(LO8(ecx), LO8(eax))) goto loc_0021661B; /* ja: above (unsigned >) */

loc_002165F3: ;
    edx = ZX8(LO8(ecx));
    edx = edx << 4;
    edi = edx + esi + 0x12;
    SET_LO8(edx, LO8(eax));
    SET_LO8(edx, LO8(edx) - LO8(ecx));
    SET_LO8(edx, LO8(edx) + 1);
    edx = ZX8(LO8(edx));
    MEM32(ebp + -4) = edx;

loc_00216609: ;
    edx = MEM32(ebp + -20);
    SET_LO16(edx, MEM16(edx + 0x22));
    MEM16(edi) = MEM16(edi) - LO16(edx);
    edi = edi + 0x10;
    MEM32(ebp + -4) = MEM32(ebp + -4) - 1;
    if ((MEM32(ebp + -4) != 0)) goto loc_00216609; /* jne: not equal / not zero */

loc_0021661B: ;
    SET_LO8(eax, LO8(eax) << 1);
    SET_LO8(ecx, LO8(ecx) << 1);
    SET_LO8(eax, LO8(eax) + 1);
    if (CMP_BE(LO8(eax), 0x40)) goto loc_002165EF; /* jbe: below or equal (unsigned <=) */

loc_00216625: ;
    if (CMP_BE(LO8(ebx), 1)) goto loc_0021669B; /* jbe: below or equal (unsigned <=) */

loc_0021662A: ;
    SET_LO8(edx, LO8(ebx));
    SET_LO8(edx, LO8(edx) ^ 1);
    ecx = ZX8(LO8(edx));
    ecx = ecx << 4;
    ecx = ecx + esi + 0xC;
    edi = ZX16(MEM16(ecx + 4));
    ecx = ZX16(MEM16(ecx + 2));
    edi = edi + ecx;
    ecx = ZX8(LO8(ebx));
    ecx = ecx << 4;
    MEM32(ebp + -4) = edi;
    ecx = ecx + esi + 0xC;
    edi = ZX16(MEM16(ecx + 4));
    ecx = ZX16(MEM16(ecx + 2));
    SET_LO8(eax, LO8(ebx));
    edi = edi + ecx;
    SET_LO8(eax, LO8(eax) >> 1);
    if (CMP_G(edi, MEM32(ebp + -4))) goto loc_00216675; /* jg: greater (signed >) */

loc_00216663: ;
    ecx = ZX8(LO8(eax));
    ecx = ecx << 4;
    ecx = ZX16(MEM16(ecx + esi + 0x10));
    if (CMP_EQ(MEM32(ebp + -4), ecx)) goto loc_00216698; /* je: equal / zero */

loc_00216673: ;
    SET_LO8(ebx, LO8(edx));

loc_00216675: ;
    ecx = ZX8(LO8(ebx));
    ecx = ecx << 4;
    ecx = ecx + esi + 0xC;
    SET_LO16(edx, MEM16(ecx + 4));
    SET_LO16(edx, LO16(edx) + MEM16(ecx + 2));
    ecx = ZX8(LO8(eax));
    ecx = ecx << 4;
    /* cmp LO8(eax), 1 - flags set for next jcc */
    MEM16(ecx + esi + 0x10) = LO16(edx);
    SET_LO8(ebx, LO8(eax));
    if (CMP_A(LO8(eax), 1)) goto loc_0021662A; /* ja: above (unsigned >) */

loc_00216698: ;
    /* cmp LO8(ebx), 1 - flags set for next jcc */

loc_0021669B: ;
    if (CMP_NE(LO8(ebx), 1)) goto loc_002166A9; /* jne: not equal / not zero */

loc_0021669D: ;
    SET_LO16(eax, MEM16(esi + 0x20));
    SET_LO16(eax, LO16(eax) + MEM16(esi + 0x1E));
    MEM16(esi + 0x10) = LO16(eax);

loc_002166A9: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

loc_002166AE: ;
    SET_LO8(ecx, LO8(ebx));
    SET_LO8(eax, LO8(ebx));
    goto loc_0021661B;

    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    ecx = MEM32(esi);
    MEM32(0x40A4A0) = MEM32(0x40A4A0) + 1;
    eax = MEM32(ecx + 0x10);
    edx = MEM32(ecx + 0xC);
    edx = edx & eax;
    PUSH32(esp, edi);
    if ((edx == 0)) goto loc_00216746; /* je: equal / zero */

loc_002166CF: ;
    edi = 0x80000000u;
    if (TEST_Z(edi, eax)) goto loc_00216746; /* je: equal / zero */

loc_002166D8: ;
    eax = 0; /* xor self */
    eax++;
    /* test LO8(eax), LO8(edx) - flags set for next jcc */
    MEM32(ecx + 0x14) = edi;
    if (TEST_Z(LO8(eax), LO8(edx))) goto loc_002166E8; /* je: equal / zero */

loc_002166E2: ;
    MEM32(ecx + 0xC) = eax;
    edx = edx & 0xFFFFFFFEu;

loc_002166E8: ;
    if (TEST_Z(LO8(edx), 0x20)) goto loc_0021671C; /* je: equal / zero */

loc_002166ED: ;
    PUSH32(esp, ebx);
    ebx = MEM32(esi + 8);
    ebx = ZX16(MEM16(ebx + 0x80));
    edi = esi + 0x418;
    eax = MEM32(edi);
    ebx = ebx ^ eax;
    ebx = ebx & 0x8000;
    eax = eax - ebx;
    eax = eax + 0x10000;
    MEM32(edi) = eax;
    MEM32(ecx + 0xC) = 0x20;
    edx = edx & 0xFFFFFFDFu;
    POP32(esp, ebx);

loc_0021671C: ;
    if (TEST_Z(edx, edx)) goto loc_00216739; /* je: equal / zero */

loc_00216720: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    MEM32(esi + 0x438) = edx;
    PUSH32(esp, 0);
    esi = esi + 0x440;
    PUSH32(esp, esi);
    { uint32_t _icall_t = MEM32(0x217B8C); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216737: ;
    goto loc_00216742;

loc_00216739: ;
    eax = MEM32(esi);
    MEM32(eax + 0x10) = 0x80000000u;

loc_00216742: ;
    SET_LO8(eax, 1);
    goto loc_00216748;

loc_00216746: ;
    SET_LO8(eax, 0); /* xor self */

loc_00216748: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

}

/**
 * sub_0021674D
 * Original: 0x0021674D - 0x00216771 (36 bytes, 10 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021674D(void)
{

loc_0021674D: ;
    eax = MEM32(ecx + 8);
    edx = MEM32(ecx + 0x418);
    ecx = ZX16(MEM16(eax + 0x80));
    eax = ecx;
    eax = eax ^ edx;
    ecx = ecx & 0x7FFF;
    eax = eax & 0x8000;
    ecx = ecx | edx;
    eax = eax + ecx;
    esp += 4; return; /* ret */

}

/**
 * sub_00216771
 * Original: 0x00216771 - 0x002167ED (124 bytes, 48 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216771(void)
{
    int _flags = 0; /* fallback flag var */

loc_00216771: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x10);
    esi = edx;
    MEM8(esi + 0x27) = MEM8(esi + 0x27) - 1;
    eax = MEM32(edi + 0x18);
    /* test eax, eax - flags set for next jcc */
    ebx = ecx;
    if (TEST_Z(eax, eax)) goto loc_00216794; /* je: equal / zero */

loc_00216786: ;
    ecx = ZX16(MEM16(edi + 0x20));
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x217B88); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216794: ;
    if (TEST_Z(MEM8(edi + 0x22), 1)) goto loc_002167DD; /* je: equal / zero */

loc_0021679A: ;
    eax = MEM32(ebx + 0x42C);
    ecx = 0; /* xor self */
    if (TEST_Z(eax, eax)) goto loc_002167DD; /* je: equal / zero */

loc_002167A6: ;
    if (CMP_EQ(edi, eax)) goto loc_002167B3; /* je: equal / zero */

loc_002167AA: ;
    ecx = eax;
    eax = MEM32(eax + 0x24);
    if (TEST_NZ(eax, eax)) goto loc_002167A6; /* jne: not equal / not zero */

loc_002167B3: ;
    if (TEST_Z(eax, eax)) goto loc_002167DD; /* je: equal / zero */

loc_002167B7: ;
    if (TEST_NZ(ecx, ecx)) goto loc_002167C6; /* jne: not equal / not zero */

loc_002167BB: ;
    ecx = MEM32(eax + 0x24);
    MEM32(ebx + 0x42C) = ecx;
    goto loc_002167CC;

loc_002167C6: ;
    edx = MEM32(eax + 0x24);
    MEM32(ecx + 0x24) = edx;

loc_002167CC: ;
    MEM32(eax + 0x24) = MEM32(eax + 0x24) & 0;
    MEM8(esi + 0x20) = MEM8(esi + 0x20) - 1;
    if ((MEM8(esi + 0x20) != 0)) goto loc_002167DD; /* jne: not equal / not zero */

loc_002167D5: ;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) & 0xDF;
    MEM8(esi + 1) = MEM8(esi + 1) & 0xBF;

loc_002167DD: ;
    MEM8(edi + 0x22) = MEM8(edi + 0x22) | 8;
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_002167E7: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_002167ED
 * Original: 0x002167ED - 0x0021681E (49 bytes, 14 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002167ED(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002167ED: ;
    eax = ZX8(MEM8(edx + 0x11));
    eax = eax - 0;
    if ((eax == 0)) goto loc_00216812; /* je: equal / zero */

loc_002167F6: ;
    eax--;
    eax--;
    if ((eax == 0)) goto loc_00216806; /* je: equal / zero */

loc_002167FA: ;
    eax--;
    if ((eax != 0)) { g_seh_ebp = ebp; sub_0021681E(); return; } /* jne: not equal / not zero */

loc_002167FD: ;
    MEM16(edx + 0x24) = MEM16(edx + 0x24) - 1;
    g_seh_ebp = ebp; sub_002177AB(); return; /* tail jmp 0x002177AB */

loc_00216806: ;
    MEM16(0xC6B9C6) = MEM16(0xC6B9C6) + 1;
    g_seh_ebp = ebp; sub_002177FE(); return; /* tail jmp 0x002177FE */

loc_00216812: ;
    MEM16(0xC6B9C2) = MEM16(0xC6B9C2) + 1;
    g_seh_ebp = ebp; sub_0021784B(); return; /* tail jmp 0x0021784B */

}

/**
 * sub_0021681E
 * Original: 0x0021681E - 0x0021681F (1 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021681E(void)
{

loc_0021681E: ;
    esp += 4; return; /* ret */

}

/**
 * sub_0021681F
 * Original: 0x0021681F - 0x0021688E (111 bytes, 41 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021681F(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_0021681F: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x10;
    eax = MEM32(edx);
    PUSH32(esp, ebx);
    ebx = MEM32(edx + 0x18);
    PUSH32(esp, esi);
    eax = eax >> 0x1C;
    /* cmp eax, 9 - flags set for next jcc */
    PUSH32(esp, edi);
    edi = MEM32(edx + 0x14);
    MEM32(ebp + -8) = ecx;
    MEM32(ebp + -16) = ebx;
    MEM8(ebp + -1) = 1;
    if (CMP_NE(eax, 9)) { g_seh_ebp = ebp; sub_0021688E(); return; } /* jne: not equal / not zero */

loc_00216842: ;
    if (CMP_EQ(MEM8(ebx + 0x1D), 0)) { g_seh_ebp = ebp; sub_0021688E(); return; } /* je: equal / zero */

loc_00216848: ;
    eax = MEM32(edx + 4);
    if (TEST_Z(eax, eax)) goto loc_0021687D; /* je: equal / zero */

loc_0021684F: ;
    ecx = MEM32(edx + 0xC);
    esi = 0xFFF;
    eax = eax & esi;
    ecx = ecx & esi;
    /* cmp ecx, eax - flags set for next jcc */
    MEM32(ebp + -12) = eax;
    if (CMP_L(ecx, eax)) goto loc_0021686D; /* jl: less (signed <) */

loc_00216862: ;
    eax = ZX8(MEM8(edx + 0x1D));
    eax = eax - ecx;
    eax = eax + MEM32(ebp + -12);
    goto loc_0021687A;

loc_0021686D: ;
    esi = ZX8(MEM8(edx + 0x1D));
    esi = esi - ecx;
    eax = esi + eax + -4096;

loc_0021687A: ;
    eax--;
    goto loc_00216881;

loc_0021687D: ;
    eax = ZX8(MEM8(edx + 0x1D));

loc_00216881: ;
    MEM32(ebx + 0x14) = MEM32(ebx + 0x14) + eax;
    MEM32(ebx + 4) = MEM32(ebx + 4) & 0;
    MEM8(ebp + -1) = MEM8(ebp + -1) & 0;
    g_seh_ebp = ebp; sub_002168A7(); return; /* tail jmp 0x002168A7 */

}

/**
 * sub_0021688E
 * Original: 0x0021688E - 0x002168A7 (25 bytes, 7 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021688E(void)
{
    int _flags = 0; /* fallback flag var */

loc_0021688E: ;
    if (CMP_NE(eax, 0xF)) goto loc_0021689A; /* jne: not equal / not zero */

loc_00216893: ;
    MEM32(ebx + 4) = 0xC000000Fu;

loc_0021689A: ;
    eax = MEM32(edx);
    eax = eax >> 0x1C;
    eax = eax | 0xC0000000u;
    MEM32(ebx + 4) = eax;

}

/**
 * sub_002168A7
 * Original: 0x002168A7 - 0x00216913 (108 bytes, 39 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002168A7(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002168A7: ;
    esi = MEM32(edi + 8);
    esi = esi & 0xFFFFFFF0u;

loc_002168AD: ;
    SET_LO8(ebx, MEM8(edx + 0x1C));
    PUSH32(esp, edx);
    SET_LO8(ebx, LO8(ebx) & 2);
    PUSH32(esp, 0); sub_0021622B(); /* call 0x0021622B */

loc_002168B9: ;
    ecx = MEM32(ebp + -8);
    edx = edi;
    PUSH32(esp, 0); sub_002167ED(); /* call 0x002167ED */

loc_002168C3: ;
    eax = MEM32(0xC6B9A0);
    edx = eax + esi;
    /* cmp MEM8(edx + 0x1E), 2 - flags set for next jcc */
    esi = MEM32(edx + 8);
    if (CMP_NE(MEM8(edx + 0x1E), 2)) goto loc_002168DA; /* jne: not equal / not zero */

loc_002168D4: ;
    if (CMP_EQ(MEM8(ebp + -1), 0)) goto loc_002168DE; /* je: equal / zero */

loc_002168DA: ;
    if (TEST_Z(LO8(ebx), LO8(ebx))) goto loc_002168AD; /* je: equal / zero */

loc_002168DE: ;
    eax = MEM32(edx + 0x10);
    eax = eax ^ MEM32(edi + 8);
    eax = eax & 0xF;
    eax = eax ^ MEM32(edx + 0x10);
    /* test LO8(ebx), LO8(ebx) - flags set for next jcc */
    MEM32(edi + 8) = eax;
    if (TEST_Z(LO8(ebx), LO8(ebx))) goto loc_002168FE; /* je: equal / zero */

loc_002168F1: ;
    PUSH32(esp, MEM32(ebp + -16));
    ecx = MEM32(ebp + -8);
    edx = edi;
    PUSH32(esp, 0); sub_00216771(); /* call 0x00216771 */

loc_002168FE: ;
    if (CMP_EQ(MEM8(edi + 0x11), 0)) goto loc_0021690A; /* je: equal / zero */

loc_00216904: ;
    if (CMP_NE(MEM8(ebp + -1), 0)) goto loc_0021690E; /* jne: not equal / not zero */

loc_0021690A: ;
    MEM32(edi + 8) = MEM32(edi + 8) & 0xFFFFFFFEu;

loc_0021690E: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00216913
 * Original: 0x00216913 - 0x0021695A (71 bytes, 25 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216913(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00216913: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    esi = edx;
    /* cmp MEM8(esi + 0x1E), 1 - flags set for next jcc */
    eax = MEM32(esi + 0x14);
    PUSH32(esp, edi);
    edi = MEM32(esi + 0x18);
    MEM32(ebp + -4) = ecx;
    MEM32(ebp + -8) = eax;
    if (CMP_NE(MEM8(esi + 0x1E), 1)) goto loc_00216948; /* jne: not equal / not zero */

loc_0021692E: ;
    eax = MEM32(esi + 0xC);
    ecx = MEM32(0xC6B9A0);
    eax = eax + ecx + -7;
    PUSH32(esp, eax);
    PUSH32(esp, 0); sub_0021622B(); /* call 0x0021622B */

loc_00216941: ;
    MEM16(0xC6B9C2) = MEM16(0xC6B9C2) + 1;

loc_00216948: ;
    if (TEST_Z(MEM8(esi + 3), 0xF0)) { g_seh_ebp = ebp; sub_0021695A(); return; } /* je: equal / zero */

loc_0021694E: ;
    ecx = MEM32(ebp + -4);
    edx = esi;
    PUSH32(esp, 0); sub_0021681F(); /* call 0x0021681F */

loc_00216958: ;
    g_seh_ebp = ebp; sub_002169BB(); return; /* tail jmp 0x002169BB */

}

/**
 * sub_0021695A
 * Original: 0x0021695A - 0x002169BB (97 bytes, 35 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021695A(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021695A: ;
    eax = MEM32(esi + 4);
    /* test eax, eax - flags set for next jcc */
    PUSH32(esp, ebx);
    if (TEST_Z(eax, eax)) goto loc_00216988; /* je: equal / zero */

loc_00216962: ;
    ecx = MEM32(esi + 0xC);
    edx = 0xFFF;
    eax = eax & edx;
    ebx = eax;
    eax = ZX8(MEM8(esi + 0x1D));
    ecx = ecx & edx;
    eax = eax - ecx;
    if (CMP_L(ecx, ebx)) goto loc_0021697E; /* jl: less (signed <) */

loc_0021697A: ;
    eax = eax + ebx;
    goto loc_00216985;

loc_0021697E: ;
    eax = eax + ebx + -4096;

loc_00216985: ;
    eax--;
    goto loc_0021698C;

loc_00216988: ;
    eax = ZX8(MEM8(esi + 0x1D));

loc_0021698C: ;
    MEM32(edi + 0x14) = MEM32(edi + 0x14) + eax;
    SET_LO8(ebx, MEM8(esi + 0x1C));
    PUSH32(esp, esi);
    SET_LO8(ebx, LO8(ebx) & 2);
    PUSH32(esp, 0); sub_0021622B(); /* call 0x0021622B */

loc_0021699B: ;
    edx = MEM32(ebp + -8);
    ecx = MEM32(ebp + -4);
    PUSH32(esp, 0); sub_002167ED(); /* call 0x002167ED */

loc_002169A6: ;
    /* test LO8(ebx), LO8(ebx) - flags set for next jcc */
    POP32(esp, ebx);
    if (TEST_Z(LO8(ebx), LO8(ebx))) { g_seh_ebp = ebp; sub_002169BB(); return; } /* je: equal / zero */

loc_002169AB: ;
    edx = MEM32(ebp + -8);
    ecx = MEM32(ebp + -4);
    MEM32(edi + 4) = MEM32(edi + 4) & 0;
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_00216771(); /* call 0x00216771 */

}

/**
 * sub_002169BB
 * Original: 0x002169BB - 0x002169BF (4 bytes, 4 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002169BB(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002169BB: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002169BF
 * Original: 0x002169BF - 0x00216AC1 (258 bytes, 87 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002169BF(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002169BF: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x18;
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    edi = edx;
    ebx = ecx;
    edx = 0; /* xor self */
    /* cmp MEM32(ebx + 0x42C), edx - flags set for next jcc */
    MEM32(ebp + -20) = edi;
    MEM32(ebp + -8) = ebx;
    MEM32(ebp + -4) = edx;
    if (CMP_EQ(MEM32(ebx + 0x42C), edx)) goto loc_00216AB0; /* je: equal / zero */

loc_002169E2: ;
    PUSH32(esp, esi);
    goto loc_002169E8;

loc_002169E5: ;
    edi = MEM32(ebp + -20);

loc_002169E8: ;
    ecx = MEM32(ebx + 0x42C);
    eax = MEM32(ecx + 0x24);
    MEM32(ebx + 0x42C) = eax;
    esi = MEM32(ecx + 0x10);
    if (CMP_AE(edi, MEM32(esi + 0x1C))) goto loc_00216A0A; /* jae: above or equal (unsigned >=) */

loc_002169FF: ;
    MEM32(ecx + 0x24) = edx;
    MEM32(ebp + -4) = ecx;
    goto loc_00216A9F;

loc_00216A0A: ;
    SET_LO8(eax, MEM8(esi + 0x10));
    if (TEST_Z(LO8(eax), 0x40)) goto loc_00216A1C; /* je: equal / zero */

loc_00216A11: ;
    edi++;
    SET_LO8(eax, LO8(eax) & 0xBF);
    MEM32(esi + 0x1C) = edi;
    MEM8(esi + 0x10) = LO8(eax);
    goto loc_002169FF;

loc_00216A1C: ;
    eax = MEM32(esi + 8);
    edi = MEM32(0xC6B9A0);
    MEM32(ebp + -12) = MEM32(ebp + -12) & 0;
    MEM32(ebp + -24) = eax;
    eax = eax & 0xFFFFFFF0u;
    edx = edi + eax;
    /* cmp ecx, MEM32(edx + 0x18) - flags set for next jcc */
    MEM32(ebp + -16) = eax;
    if (CMP_EQ(ecx, MEM32(edx + 0x18))) goto loc_00216A52; /* je: equal / zero */

loc_00216A3A: ;
    ebx = MEM32(esi + 4);

loc_00216A3D: ;
    if (CMP_EQ(eax, ebx)) goto loc_00216A4F; /* je: equal / zero */

loc_00216A41: ;
    MEM32(ebp + -12) = eax;
    eax = MEM32(edx + 8);
    edx = edi + eax;
    if (CMP_NE(ecx, MEM32(edx + 0x18))) goto loc_00216A3D; /* jne: not equal / not zero */

loc_00216A4F: ;
    ebx = MEM32(ebp + -8);

loc_00216A52: ;
    eax = MEM32(edx + 8);
    eax = eax ^ MEM32(ebp + -24);
    ecx = ebx;
    eax = eax & 0xF;
    eax = eax ^ MEM32(edx + 8);
    MEM32(esi + 8) = eax;
    MEM8(edx + 3) = MEM8(edx + 3) | 0xF0;
    PUSH32(esp, 0); sub_00216913(); /* call 0x00216913 */

loc_00216A6C: ;
    eax = MEM32(ebp + -12);
    if (TEST_Z(eax, eax)) goto loc_00216A92; /* je: equal / zero */

loc_00216A73: ;
    ecx = MEM32(esi + 8);
    edx = MEM32(0xC6B9A0);
    ecx = ecx & 0xFFFFFFF0u;
    MEM32(edx + eax + 8) = ecx;
    eax = MEM32(esi + 8);
    eax = eax ^ MEM32(ebp + -16);
    eax = eax & 0xF;
    eax = eax ^ MEM32(ebp + -16);
    MEM32(esi + 8) = eax;

loc_00216A92: ;
    MEM8(esi + 0x20) = MEM8(esi + 0x20) - 1;
    if ((MEM8(esi + 0x20) != 0)) goto loc_00216A9F; /* jne: not equal / not zero */

loc_00216A97: ;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) & 0xDF;
    MEM8(esi + 1) = MEM8(esi + 1) & 0xBF;

loc_00216A9F: ;
    /* cmp MEM32(ebx + 0x42C), 0 - flags set for next jcc */
    edx = MEM32(ebp + -4);
    if (CMP_NE(MEM32(ebx + 0x42C), 0)) goto loc_002169E5; /* jne: not equal / not zero */

loc_00216AAF: ;
    POP32(esp, esi);

loc_00216AB0: ;
    eax = 0; /* xor self */
    /* test edx, edx - flags set for next jcc */
    POP32(esp, edi);
    MEM32(ebx + 0x42C) = edx;
    SET_LO8(eax, (TEST_NZ(edx, edx)) ? 1 : 0); /* setne */
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00216AC1
 * Original: 0x00216AC1 - 0x00216B17 (86 bytes, 35 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216AC1(void)
{
    int _flags = 0; /* fallback flag var */

loc_00216AC1: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = edx;
    edi = ecx;
    SET_LO8(ebx, 0); /* xor self */

loc_00216ACA: ;
    eax = MEM32(esi + 8);
    ecx = eax;
    ecx = ecx & 0xFFFFFFF0u;
    if ((ecx == 0)) goto loc_00216B13; /* je: equal / zero */

loc_00216AD4: ;
    edx = MEM32(0xC6B9A0);
    edx = edx + ecx;
    if (CMP_EQ(ecx, MEM32(esi + 4))) goto loc_00216AFD; /* je: equal / zero */

loc_00216AE1: ;
    eax = MEM32(edx + 8);
    MEM8(edx + 3) = MEM8(edx + 3) | 0xF0;
    eax = eax ^ MEM32(esi + 8);
    ecx = edi;
    eax = eax & 0xF;
    eax = eax ^ MEM32(edx + 8);
    MEM32(esi + 8) = eax;
    PUSH32(esp, 0); sub_00216913(); /* call 0x00216913 */

loc_00216AFB: ;
    goto loc_00216B0F;

loc_00216AFD: ;
    MEM32(esi + 4) = MEM32(esi + 4) & 0;
    eax = eax & 0xF;
    PUSH32(esp, edx);
    MEM32(esi + 8) = eax;
    PUSH32(esp, 0); sub_0021622B(); /* call 0x0021622B */

loc_00216B0D: ;
    SET_LO8(ebx, 1);

loc_00216B0F: ;
    if (TEST_Z(LO8(ebx), LO8(ebx))) goto loc_00216ACA; /* je: equal / zero */

loc_00216B13: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}

/**
 * sub_00216B17
 * Original: 0x00216B17 - 0x00216B9E (135 bytes, 50 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216B17(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216B17: ;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    ebx = ecx;
    ebp = 0; /* xor self */
    /* cmp MEM32(ebx + 0x430), ebp - flags set for next jcc */
    MEM32(esp + 8) = edx;
    if (CMP_EQ(MEM32(ebx + 0x430), ebp)) goto loc_00216B8D; /* je: equal / zero */

loc_00216B2A: ;
    PUSH32(esp, esi);
    PUSH32(esp, edi);

loc_00216B2C: ;
    edi = MEM32(ebx + 0x430);
    eax = MEM32(edi + 0x24);
    MEM32(ebx + 0x430) = eax;
    esi = MEM32(edi + 0x10);
    if (CMP_AE(edx, MEM32(esi + 0x1C))) goto loc_00216B4A; /* jae: above or equal (unsigned >=) */

loc_00216B43: ;
    MEM32(edi + 0x14) = ebp;
    ebp = edi;
    goto loc_00216B82;

loc_00216B4A: ;
    SET_LO8(eax, MEM8(esi + 0x10));
    if (TEST_Z(LO8(eax), 0x40)) goto loc_00216B5E; /* je: equal / zero */

loc_00216B51: ;
    ecx = edx + 1;
    SET_LO8(eax, LO8(eax) & 0xBF);
    MEM32(esi + 0x1C) = ecx;
    MEM8(esi + 0x10) = LO8(eax);
    goto loc_00216B43;

loc_00216B5E: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_00216AC1(); /* call 0x00216AC1 */

loc_00216B67: ;
    MEM8(esi + 0x20) = MEM8(esi + 0x20) - 1;
    if ((MEM8(esi + 0x20) != 0)) goto loc_00216B74; /* jne: not equal / not zero */

loc_00216B6C: ;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) & 0xDF;
    MEM8(esi + 1) = MEM8(esi + 1) & 0xBF;

loc_00216B74: ;
    MEM32(edi + 4) = MEM32(edi + 4) & 0;
    PUSH32(esp, edi);
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_00216B7E: ;
    edx = MEM32(esp + 0x10);

loc_00216B82: ;
    if (CMP_NE(MEM32(ebx + 0x430), 0)) goto loc_00216B2C; /* jne: not equal / not zero */

loc_00216B8B: ;
    POP32(esp, edi);
    POP32(esp, esi);

loc_00216B8D: ;
    eax = 0; /* xor self */
    MEM32(ebx + 0x430) = ebp;
    /* test ebp, ebp - flags set for next jcc */
    POP32(esp, ebp);
    SET_LO8(eax, (TEST_NZ(ebp, ebp)) ? 1 : 0); /* setne */
    POP32(esp, ebx);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_00216B9E
 * Original: 0x00216B9E - 0x00216C78 (218 bytes, 75 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216B9E(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00216B9E: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0xC;
    PUSH32(esp, ebx);
    ebx = ecx;
    ecx = 0; /* xor self */
    /* cmp MEM32(ebx + 0x434), ecx - flags set for next jcc */
    MEM32(ebp + -8) = edx;
    MEM32(ebp + -4) = ecx;
    if (CMP_EQ(MEM32(ebx + 0x434), ecx)) goto loc_00216C68; /* je: equal / zero */

loc_00216BBB: ;
    PUSH32(esp, esi);
    PUSH32(esp, edi);

loc_00216BBD: ;
    esi = MEM32(ebx + 0x434);
    eax = MEM32(esi + 0x14);
    MEM32(ebx + 0x434) = eax;
    edi = MEM32(esi + 0x10);
    eax = MEM32(ebp + -8);
    if (CMP_AE(eax, MEM32(edi + 0x1C))) goto loc_00216BDF; /* jae: above or equal (unsigned >=) */

loc_00216BD7: ;
    MEM32(esi + 0x14) = ecx;
    MEM32(ebp + -4) = esi;
    goto loc_00216C56;

loc_00216BDF: ;
    SET_LO8(eax, MEM8(edi + 0x10));
    if (TEST_Z(LO8(eax), 0x40)) goto loc_00216BED; /* je: equal / zero */

loc_00216BE6: ;
    SET_LO8(eax, LO8(eax) & 0xBF);
    MEM8(edi + 0x10) = LO8(eax);
    goto loc_00216BD7;

loc_00216BED: ;
    /* cmp MEM8(esi + 1), 0x4A - flags set for next jcc */
    ecx = ebx;
    if (CMP_NE(MEM8(esi + 1), 0x4A)) goto loc_00216BFE; /* jne: not equal / not zero */

loc_00216BF5: ;
    edx = esi;
    PUSH32(esp, 0); sub_00216F89(); /* call 0x00216F89 */

loc_00216BFC: ;
    goto loc_00216C56;

loc_00216BFE: ;
    edx = edi;
    PUSH32(esp, 0); sub_00216AC1(); /* call 0x00216AC1 */

loc_00216C05: ;
    edx = MEM32(esi + 0x18);
    if (TEST_Z(edx, edx)) goto loc_00216C3E; /* je: equal / zero */

loc_00216C0C: ;
    ecx = MEM32(edi);
    MEM32(ebp + -12) = ecx;
    ecx = ecx >> 7;
    eax = 0; /* xor self */
    ecx = ecx & 0xF;
    eax++;
    eax = eax << LO8(ecx);
    ecx = MEM32(ebp + -12);
    ecx = ecx & 0x1800;
    if (CMP_NE(ecx, 0x1000)) goto loc_00216C30; /* jne: not equal / not zero */

loc_00216C2D: ;
    eax = eax << 0x10;

loc_00216C30: ;
    if (TEST_Z(MEM8(edi + 8), 2)) goto loc_00216C3A; /* je: equal / zero */

loc_00216C36: ;
    MEM32(edx) = MEM32(edx) | eax;
    goto loc_00216C3E;

loc_00216C3A: ;
    eax = ~eax;
    MEM32(edx) = MEM32(edx) & eax;

loc_00216C3E: ;
    eax = MEM32(0xC6B9A8);
    MEM32(edi + 0x18) = eax;
    MEM32(0xC6B9A8) = edi;
    MEM32(esi + 4) = MEM32(esi + 4) & 0;
    PUSH32(esp, esi);
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_00216C56: ;
    /* cmp MEM32(ebx + 0x434), 0 - flags set for next jcc */
    ecx = MEM32(ebp + -4);
    if (CMP_NE(MEM32(ebx + 0x434), 0)) goto loc_00216BBD; /* jne: not equal / not zero */

loc_00216C66: ;
    POP32(esp, edi);
    POP32(esp, esi);

loc_00216C68: ;
    eax = 0; /* xor self */
    /* test ecx, ecx - flags set for next jcc */
    MEM32(ebx + 0x434) = ecx;
    SET_LO8(eax, (TEST_NZ(ecx, ecx)) ? 1 : 0); /* setne */
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00216C78
 * Original: 0x00216C78 - 0x00216D86 (270 bytes, 94 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216C78(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00216C78: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 0xC);
    MEM8(ebp + 0xF) = MEM8(ebp + 0xF) & 0;
    ebx = MEM32(esi);
    eax = esi + 0x438;
    ecx = MEM32(eax);
    PUSH32(esp, edi);
    edi = 0; /* xor self */
    MEM32(eax) = MEM32(eax) & edi;
    /* test LO8(ecx), 2 - flags set for next jcc */
    MEM32(ebp + -4) = ecx;
    if (TEST_Z(LO8(ecx), 2)) goto loc_00216CF4; /* je: equal / zero */

loc_00216C9D: ;
    ecx = MEM32(esi + 8);
    ecx = ecx + 0x84;
    eax = MEM32(ecx);
    eax = eax & 0xFFFFFFF0u;
    MEM32(ecx) = edi;
    if ((eax == 0)) goto loc_00216CE0; /* je: equal / zero */

loc_00216CAF: ;
    ecx = MEM32(0xC6B9A0);
    ecx = ecx + eax;
    eax = MEM32(ecx + 8);
    /* test eax, eax - flags set for next jcc */
    MEM32(ecx + 8) = edi;
    edi = ecx;
    if (TEST_NZ(eax, eax)) goto loc_00216CAF; /* jne: not equal / not zero */

loc_00216CC3: ;
    edx = edi;
    /* test MEM8(edx + 2), 1 - flags set for next jcc */
    edi = MEM32(edi + 8);
    ecx = esi;
    if (TEST_Z(MEM8(edx + 2), 1)) goto loc_00216CD7; /* je: equal / zero */

loc_00216CD0: ;
    PUSH32(esp, 0); sub_002172F2(); /* call 0x002172F2 */

loc_00216CD5: ;
    goto loc_00216CDC;

loc_00216CD7: ;
    PUSH32(esp, 0); sub_00216913(); /* call 0x00216913 */

loc_00216CDC: ;
    if (TEST_NZ(edi, edi)) goto loc_00216CC3; /* jne: not equal / not zero */

loc_00216CE0: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) & 0xFFFFFFFDu;
    MEM32(ebx + 0xC) = 2;
    eax = MEM32(esi);
    MEM32(eax + 8) = 6;

loc_00216CF4: ;
    /* test MEM8(ebp + -4), 4 - flags set for next jcc */
    PUSH32(esp, 4);
    POP32(esp, edi);
    if (TEST_Z(MEM8(ebp + -4), 4)) goto loc_00216D07; /* je: equal / zero */

loc_00216CFD: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) & 0xFFFFFFFBu;
    MEM32(ebx + 0xC) = edi;
    MEM32(ebx + 0x14) = edi;

loc_00216D07: ;
    ecx = esi;
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_00216D0E: ;
    edx = eax;
    ecx = esi;
    MEM32(ebp + -8) = edx;
    PUSH32(esp, 0); sub_002169BF(); /* call 0x002169BF */

loc_00216D1A: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00216D22; /* je: equal / zero */

loc_00216D1E: ;
    MEM8(ebp + 0xF) = 1;

loc_00216D22: ;
    edx = MEM32(ebp + -8);
    ecx = esi;
    PUSH32(esp, 0); sub_00216B17(); /* call 0x00216B17 */

loc_00216D2C: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00216D34; /* je: equal / zero */

loc_00216D30: ;
    MEM8(ebp + 0xF) = 1;

loc_00216D34: ;
    edx = MEM32(ebp + -8);
    ecx = esi;
    PUSH32(esp, 0); sub_00216B9E(); /* call 0x00216B9E */

loc_00216D3E: ;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00216D46; /* je: equal / zero */

loc_00216D42: ;
    MEM8(ebp + 0xF) = 1;

loc_00216D46: ;
    if (CMP_EQ(MEM8(ebp + 0xF), 0)) goto loc_00216D56; /* je: equal / zero */

loc_00216D4C: ;
    eax = MEM32(esi);
    MEM32(eax + 0xC) = edi;
    eax = MEM32(esi);
    MEM32(eax + 0x10) = edi;

loc_00216D56: ;
    if (TEST_Z(MEM8(ebp + -4), 0x40)) goto loc_00216D6E; /* je: equal / zero */

loc_00216D5C: ;
    ecx = esi;
    PUSH32(esp, 0); sub_0021616C(); /* call 0x0021616C */

loc_00216D63: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) & 0xFFFFFFBFu;
    MEM32(ebx + 0xC) = 0x40;

loc_00216D6E: ;
    eax = MEM32(ebp + -4);
    if (TEST_Z(eax, eax)) goto loc_00216D78; /* je: equal / zero */

loc_00216D75: ;
    MEM32(ebx + 0xC) = eax;

loc_00216D78: ;
    POP32(esp, edi);
    POP32(esp, esi);
    MEM32(ebx + 0x10) = 0x80000000u;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 20; return; /* ret 16 */

}

/**
 * sub_00216D86
 * Original: 0x00216D86 - 0x00216D98 (18 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216D86(void)
{
    int _flags = 0; /* fallback flag var */

loc_00216D86: ;
    eax = MEM32(0xC6B9C8);
    if (TEST_Z(eax, eax)) goto loc_00216D97; /* je: equal / zero */

loc_00216D8F: ;
    ecx = MEM32(eax);
    MEM32(0xC6B9C8) = ecx;

loc_00216D97: ;
    esp += 4; return; /* ret */

}

/**
 * sub_00216D98
 * Original: 0x00216D98 - 0x00216DC5 (45 bytes, 16 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216D98(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00216D98: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x14;
    eax = MEM32(0xC6B9CC);
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    ebx = edx;
    MEM32(ebp + -20) = ecx;
    MEM32(ebp + -8) = eax;
    PUSH32(esp, 0); sub_00216D86(); /* call 0x00216D86 */

loc_00216DB2: ;
    edi = eax;
    /* test edi, edi - flags set for next jcc */
    MEM32(ebp + -12) = edi;
    if (TEST_NZ(edi, edi)) { g_seh_ebp = ebp; sub_00216DC5(); return; } /* jne: not equal / not zero */

loc_00216DBB: ;
    edi = 0x80000100u;
    g_seh_ebp = ebp; sub_00216F3C(); return; /* tail jmp 0x00216F3C */

}

/**
 * sub_00216DC5
 * Original: 0x00216DC5 - 0x00216F3C (375 bytes, 126 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216DC5(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216DC5: ;
    PUSH32(esp, esi);
    esi = MEM32(ebp + -8);
    esi = esi << 6;
    eax = 0; /* xor self */
    ecx = esi + 0x30;
    edx = ecx;
    ecx = ecx >> 2;
    { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = eax; }
    edi += ecx * 4; ecx = 0; /* rep stosd */
    ecx = edx;
    ecx = ecx & 3;
    memset((void*)XBOX_PTR(edi), (uint8_t)eax, ecx);
    edi += ecx; ecx = 0; /* rep stosb */
    eax = MEM32(ebp + -12);
    esi = esi + eax;
    MEM32(esi + 0x2C) = eax;
    eax = esi;
    eax = eax - MEM32(0xC6B9A0);
    MEM8(esi + 0x11) = 1;
    MEM32(esi + 0x14) = eax;
    eax = 0; /* xor self */
    MEM8(esi + 0x13) = 1;
    SET_LO16(eax, MEM16(ebx + 0x16));
    PUSH32(esp, 0);
    PUSH32(esp, 1);
    PUSH32(esp, eax);
    PUSH32(esp, 0); sub_002128E9(); /* call 0x002128E9 */

loc_00216E0A: ;
    edx = MEM32(ebp + -8);
    MEM16(esi + 0x22) = LO16(eax);
    MEM8(esi + 0x24) = LO8(edx);
    SET_LO8(eax, MEM8(ebx + 0x18));
    SET_LO8(eax, LO8(eax) & 1);
    MEM8(esi + 0x10) = LO8(eax);
    eax = 0; /* xor self */
    SET_LO8(eax, MEM8(ebx + 0x14));
    PUSH32(esp, 0);
    eax = eax ^ MEM32(esi);
    eax = eax & 0x7F;
    MEM32(esi) = MEM32(esi) ^ eax;
    eax = ZX8(MEM8(ebx + 0x15));
    ecx = MEM32(esi);
    eax = eax << 7;
    eax = eax ^ ecx;
    eax = eax & 0x780;
    eax = eax ^ ecx;
    MEM32(esi) = eax;
    /* test MEM8(ebx + 0x15), 0x80 - flags set for next jcc */
    POP32(esp, ecx);
    SET_LO8(ecx, (TEST_NZ(MEM8(ebx + 0x15), 0x80)) ? 1 : 0); /* setne */
    eax = eax & 0xFFFFC7FFu;
    ecx++;
    ecx = ecx & 3;
    ecx = ecx | 0x18;
    ecx = ecx << 0xB;
    ecx = ecx | eax;
    MEM32(esi) = ecx;
    eax = ZX16(MEM16(ebx + 0x16));
    eax = eax << 0x10;
    eax = eax ^ ecx;
    eax = eax & 0x7FF0000;
    eax = eax ^ ecx;
    ecx = MEM32(esi + 8);
    MEM32(esi) = eax;
    eax = MEM32(esi + 0x2C);
    eax = eax - MEM32(0xC6B9A0);
    ecx = ecx ^ eax;
    ecx = ecx & 0xF;
    ecx = ecx ^ eax;
    MEM32(esi + 8) = ecx;
    SET_LO8(ecx, 0); /* xor self */
    /* test edx, edx - flags set for next jcc */
    MEM32(esi + 4) = eax;
    MEM8(ebp + -1) = LO8(ecx);
    if (CMP_BE(edx & edx, 0)) goto loc_00216EEA; /* jbe: below or equal (unsigned <=) */

loc_00216E8D: ;
    eax = 0; /* xor self */

loc_00216E8F: ;
    edx = MEM32(esi + 0x2C);
    eax = eax << 6;
    edi = eax + edx;
    MEM32(ebp + -16) = edi;
    edi = edi - MEM32(0xC6B9A0);
    edx = MEM32(ebp + -16);
    edi = edi + 0x40;
    SET_LO8(ecx, LO8(ecx) - 1);
    MEM8(edx + 0x2D) = LO8(ecx);
    edx = MEM32(esi + 0x2C);
    SET_LO8(ecx, MEM8(ebp + -1));
    MEM8(eax + edx + 0x2C) = LO8(ecx);
    edx = MEM32(esi + 0x2C);
    MEM32(eax + edx + 0x20) = esi;
    edx = MEM32(esi + 0x2C);
    MEM32(eax + edx + 0x28) = MEM32(eax + edx + 0x28) & 0;
    edx = MEM32(esi + 0x2C);
    MEM32(eax + edx + 0x24) = MEM32(eax + edx + 0x24) & 0;
    edx = MEM32(esi + 0x2C);
    edx = edx + eax;
    MEM8(edx + 2) = MEM8(edx + 2) | 1;
    edx = MEM32(esi + 0x2C);
    SET_LO8(ecx, LO8(ecx) + 1);
    MEM32(eax + edx + 8) = edi;
    eax = ZX8(LO8(ecx));
    /* cmp eax, MEM32(ebp + -8) - flags set for next jcc */
    MEM8(ebp + -1) = LO8(ecx);
    if (CMP_B(eax, MEM32(ebp + -8))) goto loc_00216E8F; /* jb: below (unsigned <) */

loc_00216EEA: ;
    edx = MEM32(esi + 0x2C);
    eax = ZX8(LO8(ecx));
    eax = eax << 6;
    MEM32(eax + edx + -56) = MEM32(eax + edx + -56) & 0;
    eax = MEM32(esi + 0x2C);
    SET_LO8(ecx, LO8(ecx) - 1);
    MEM8(eax + 0x2D) = LO8(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216F06: ;
    ecx = MEM32(ebp + -20);
    edx = esi;
    MEM8(ebp + -1) = LO8(eax);
    PUSH32(esp, 0); sub_00216362(); /* call 0x00216362 */

loc_00216F13: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    edi = eax;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216F1E: ;
    if (TEST_S(edi, edi)) goto loc_00216F27; /* jl: less (signed <) */

loc_00216F22: ;
    MEM32(ebx + 0x10) = esi;
    goto loc_00216F3B;

loc_00216F27: ;
    MEM32(ebx + 0x10) = MEM32(ebx + 0x10) & 0;
    ecx = MEM32(0xC6B9C8);
    eax = MEM32(ebp + -12);
    MEM32(eax) = ecx;
    MEM32(0xC6B9C8) = eax;

loc_00216F3B: ;
    POP32(esp, esi);

}

/**
 * sub_00216F3C
 * Original: 0x00216F3C - 0x00216F45 (9 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216F3C(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216F3C: ;
    MEM32(ebx + 4) = edi;
    eax = edi;
    POP32(esp, edi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00216F45
 * Original: 0x00216F45 - 0x00216F89 (68 bytes, 27 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216F45(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216F45: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = edx;
    ebp = MEM32(esi + 0x10);
    PUSH32(esp, edi);
    edi = ecx;
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216F56: ;
    edx = ebp;
    ecx = edi;
    SET_LO8(ebx, LO8(eax));
    PUSH32(esp, 0); sub_0021653E(); /* call 0x0021653E */

loc_00216F61: ;
    edx = ebp;
    ecx = edi;
    PUSH32(esp, 0); sub_00215188(); /* call 0x00215188 */

loc_00216F6A: ;
    eax = edi + 0x434;
    ecx = MEM32(eax);
    MEM32(esi + 0x14) = ecx;
    SET_LO8(ecx, LO8(ebx));
    MEM32(eax) = esi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216F7F: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    eax = 0x40000000;
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}

/**
 * sub_00216F89
 * Original: 0x00216F89 - 0x00217012 (137 bytes, 52 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00216F89(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00216F89: ;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    ebp = edx;
    PUSH32(esp, esi);
    esi = MEM32(ebp + 0x10);
    SET_LO8(eax, MEM8(esi + 0x25));
    ebx = ZX8(MEM8(esi + 0x26));
    ecx = ZX8(LO8(eax));
    ebx = ebx - ecx;
    ecx = ZX8(MEM8(esi + 0x24));
    ebx = ebx + ecx;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00216FEB; /* je: equal / zero */

loc_00216FA7: ;
    PUSH32(esp, edi);

loc_00216FA8: ;
    ecx = ZX8(MEM8(esi + 0x24));
    eax = ebx;
    edx = 0; /* xor self */
    { uint64_t _dividend = ((uint64_t)edx << 32) | eax;
      eax = (uint32_t)(_dividend / (uint32_t)ecx);
      edx = (uint32_t)(_dividend % (uint32_t)ecx); }
    MEM8(esi + 0x25) = MEM8(esi + 0x25) - 1;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    ebx = edx;
    edi = ebx;
    edi = edi << 6;
    edi = edi + MEM32(esi + 0x2C);
    ebx++;
    PUSH32(esp, MEM32(edi + 4));
    { uint32_t _icall_t = MEM32(0x217B90); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216FCB: ;
    eax = MEM32(edi + 0xC);
    ecx = MEM32(edi + 4);
    ecx = ecx ^ eax;
    if (TEST_Z(ecx, 0xFFFFF000u)) goto loc_00216FE4; /* je: equal / zero */

loc_00216FDB: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x217B90); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00216FE4: ;
    if (CMP_NE(MEM8(esi + 0x25), 0)) goto loc_00216FA8; /* jne: not equal / not zero */

loc_00216FEA: ;
    POP32(esp, edi);

loc_00216FEB: ;
    eax = ZX8(MEM8(esi + 0x24));
    MEM8(esi + 0x25) = MEM8(esi + 0x25) - 1;
    eax = eax << 6;
    esi = esi - eax;
    eax = MEM32(0xC6B9C8);
    MEM32(esi) = eax;
    MEM32(0xC6B9C8) = esi;
    MEM32(ebp + 4) = MEM32(ebp + 4) & 0;
    PUSH32(esp, ebp);
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_0021700E: ;
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}

/**
 * sub_00217012
 * Original: 0x00217012 - 0x00217049 (55 bytes, 18 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217012(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00217012: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x24;
    MEM32(ebp + -20) = MEM32(ebp + -20) & 0;
    PUSH32(esp, esi);
    esi = edx;
    PUSH32(esp, edi);
    edi = MEM32(esi + 0x10);
    MEM32(ebp + -16) = esi;
    MEM32(ebp + -36) = ecx;
    MEM32(ebp + -32) = edi;
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217032: ;
    SET_LO8(ecx, MEM8(edi + 0x24));
    /* cmp LO8(ecx), MEM8(edi + 0x25) - flags set for next jcc */
    MEM8(ebp + -1) = LO8(eax);
    if (CMP_NE(LO8(ecx), MEM8(edi + 0x25))) { g_seh_ebp = ebp; sub_00217049(); return; } /* jne: not equal / not zero */

loc_0021703D: ;
    MEM32(ebp + -20) = 0xC0000D00u;
    g_seh_ebp = ebp; sub_0021715E(); return; /* tail jmp 0x0021715E */

}

/**
 * sub_00217049
 * Original: 0x00217049 - 0x0021715E (277 bytes, 101 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217049(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217049: ;
    eax = ZX8(MEM8(edi + 0x26));
    PUSH32(esp, ebx);
    ebx = eax;
    ebx = ebx << 6;
    ebx = ebx + MEM32(edi + 0x2C);
    eax++;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    ecx = ZX8(LO8(ecx));
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)ecx));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)ecx)); }
    esi = MEM32(esi + 0x18);
    MEM32(ebp + -12) = MEM32(ebp + -12) & 0;
    MEM32(ebp + -28) = esi;
    MEM8(edi + 0x26) = LO8(edx);
    eax = MEM32(esi + 0x18);
    MEM32(ebx + 0x24) = eax;
    eax = MEM32(esi + 0x1C);
    MEM32(ebx + 0x28) = eax;
    eax = MEM32(ebp + -16);
    MEM32(ebx + 0x20) = edi;
    eax = ZX8(MEM8(eax + 0x14));
    eax = eax << 0x15;
    eax = eax ^ MEM32(ebx);
    eax = eax & 0xE00000;
    MEM32(ebx) = MEM32(ebx) ^ eax;
    ecx = MEM32(esi);
    eax = MEM32(ebx);
    ecx--;
    ecx = ecx << 0x18;
    ecx = ecx ^ eax;
    ecx = ecx & 0x7000000;
    ecx = ecx ^ eax;
    MEM32(ebx) = ecx;
    eax = MEM32(esi + 4);
    eax = eax & 0xFFF;
    /* cmp MEM32(esi), 0 - flags set for next jcc */
    MEM32(ebp + -24) = eax;
    if (CMP_BE(MEM32(esi), 0)) goto loc_002170DB; /* jbe: below or equal (unsigned <=) */

loc_002170B0: ;
    ecx = esi + 8;
    MEM32(ebp + -8) = ecx;
    ecx = ebx + 0x10;

loc_002170B9: ;
    edx = eax;
    SET_LO16(edx, LO16(edx) | 0xE000);
    MEM16(ecx) = LO16(edx);
    edx = MEM32(ebp + -8);
    edx = ZX16(MEM16(edx));
    MEM32(ebp + -8) = MEM32(ebp + -8) + 2;
    eax = eax + edx;
    MEM32(ebp + -12) = MEM32(ebp + -12) + 1;
    edx = MEM32(ebp + -12);
    ecx++;
    ecx++;
    if (CMP_B(edx, MEM32(esi))) goto loc_002170B9; /* jb: below (unsigned <) */

loc_002170DB: ;
    eax = eax - MEM32(ebp + -24);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    PUSH32(esp, MEM32(esi + 4));
    MEM32(ebp + -24) = eax;
    { uint32_t _icall_t = MEM32(0x217B88); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002170ED: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, MEM32(esi + 4));
    { uint32_t _icall_t = MEM32(0x217B84); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002170F6: ;
    ecx = MEM32(ebp + -24);
    MEM32(ebx + 4) = eax;
    eax = MEM32(esi + 4);
    eax = eax + ecx + -1;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(0x217B84); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_0021710A: ;
    MEM32(ebx + 0xC) = eax;
    if (TEST_Z(MEM8(edi + 0x10), 1)) goto loc_00217123; /* je: equal / zero */

loc_00217113: ;
    esi = ebx + 0x10;
    edi = ebx + 0x30;
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    esi = MEM32(ebp + -28);
    edi = MEM32(ebp + -32);

loc_00217123: ;
    if (TEST_Z(MEM8(edi + 0x10), 2)) goto loc_00217149; /* je: equal / zero */

loc_00217129: ;
    ecx = MEM32(ebp + -36);
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_00217131: ;
    ecx = MEM32(edi + 0x28);
    eax++;
    edx = ecx;
    edx = edx - eax;
    if (CMP_LE(edx & edx, 0)) goto loc_0021713F; /* jle: less or equal (signed <=) */

loc_0021713D: ;
    eax = ecx;

loc_0021713F: ;
    MEM16(ebx) = LO16(eax);
    ecx = MEM32(esi);
    ecx = ecx + eax;
    MEM32(edi + 0x28) = ecx;

loc_00217149: ;
    MEM8(edi + 0x25) = MEM8(edi + 0x25) + 1;
    SET_LO8(eax, MEM8(edi + 0x25));
    /* cmp LO8(eax), MEM8(edi + 0x24) - flags set for next jcc */
    esi = MEM32(ebp + -16);
    if (CMP_EQ(LO8(eax), MEM8(edi + 0x24))) goto loc_0021715D; /* je: equal / zero */

loc_00217157: ;
    eax = MEM32(ebx + 8);
    MEM32(edi + 4) = eax;

loc_0021715D: ;
    POP32(esp, ebx);

}

/**
 * sub_0021715E
 * Original: 0x0021715E - 0x00217179 (27 bytes, 11 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021715E(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021715E: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217167: ;
    edi = MEM32(ebp + -20);
    PUSH32(esp, esi);
    MEM32(esi + 4) = edi;
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_00217173: ;
    eax = edi;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00217179
 * Original: 0x00217179 - 0x002171AC (51 bytes, 18 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217179(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_00217179: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x18;
    MEM32(ebp + -16) = MEM32(ebp + -16) & 0;
    PUSH32(esp, ebx);
    ebx = MEM32(0x217B00);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = edx;
    esi = MEM32(edi + 0x10);
    MEM32(ebp + -12) = edi;
    MEM32(ebp + -8) = ecx;
    { uint32_t _icall_t = ebx; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217199: ;
    /* test MEM8(esi + 0x10), 2 - flags set for next jcc */
    MEM8(ebp + -1) = LO8(eax);
    if (TEST_Z(MEM8(esi + 0x10), 2)) { g_seh_ebp = ebp; sub_002171AC(); return; } /* je: equal / zero */

loc_002171A2: ;
    esi = 0xC0000E00u;
    g_seh_ebp = ebp; sub_00217278(); return; /* tail jmp 0x00217278 */

}

/**
 * sub_002171AC
 * Original: 0x002171AC - 0x00217278 (204 bytes, 67 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002171AC(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002171AC: ;
    ecx = MEM32(ebp + -8);
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_002171B4: ;
    SET_LO8(edx, MEM8(esi + 0x10));
    if (TEST_Z(LO8(edx), 4)) goto loc_002171F6; /* je: equal / zero */

loc_002171BC: ;
    SET_LO8(edx, LO8(edx) & 0xFB);
    /* cmp MEM32(esi + 0x1C), eax - flags set for next jcc */
    MEM8(esi + 0x10) = LO8(edx);
    if (CMP_NE(MEM32(esi + 0x1C), eax)) goto loc_002171EE; /* jne: not equal / not zero */

loc_002171C7: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002171D0: ;
    MEM32(ebp + -20) = MEM32(ebp + -20) | 0xFFFFFFFFu;
    eax = ebp + -24;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    MEM32(ebp + -24) = 0xFFFFD8F0u;
    { uint32_t _icall_t = MEM32(0x217AC0); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002171E9: ;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = ebx; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002171EB: ;
    MEM8(ebp + -1) = LO8(eax);

loc_002171EE: ;
    ecx = MEM32(ebp + -8);
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_002171F6: ;
    if (TEST_Z(MEM8(esi + 0x10), 1)) goto loc_0021720B; /* je: equal / zero */

loc_002171FC: ;
    SET_LO8(ecx, MEM8(esi + 0x24));
    if (CMP_EQ(LO8(ecx), MEM8(esi + 0x25))) goto loc_0021720B; /* je: equal / zero */

loc_00217204: ;
    esi = 0xC0001000u;
    g_seh_ebp = ebp; sub_00217278(); return; /* tail jmp 0x00217278 */

loc_0021720B: ;
    /* test MEM8(edi + 0x18), 1 - flags set for next jcc */
    ecx = eax + 1;
    if (TEST_NZ(MEM8(edi + 0x18), 1)) goto loc_00217226; /* jne: not equal / not zero */

loc_00217214: ;
    edx = MEM32(edi + 0x14);
    eax = edx;
    eax = eax - ecx;
    if (((int32_t)eax < 0)) { g_seh_ebp = ebp; sub_00217291(); return; } /* js: sign (negative) */

loc_0021721D: ;
    if (CMP_G(eax, 0x400)) { g_seh_ebp = ebp; sub_00217291(); return; } /* jg: greater (signed >) */

loc_00217224: ;
    ecx = edx;

loc_00217226: ;
    SET_LO8(edx, MEM8(esi + 0x24));
    SET_LO8(eax, LO8(edx));
    SET_LO8(eax, LO8(eax) - MEM8(esi + 0x25));
    edi = ZX8(LO8(edx));
    SET_LO8(eax, LO8(eax) + MEM8(esi + 0x26));
    eax = ZX8(LO8(eax));
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)edi));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)edi)); }
    edi = MEM32(esi + 0x2C);

loc_0021723D: ;
    edx = ZX8(LO8(edx));
    eax = edx;
    eax = eax << 6;
    MEM16(edi + eax) = LO16(ecx);
    edi = MEM32(esi + 0x2C);
    eax = ZX8(MEM8(edi + eax + 3));
    ebx = ZX8(MEM8(esi + 0x24));
    eax = eax & 7;
    ecx = ecx + eax + 1;
    eax = edx + 1;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)ebx));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)ebx)); }
    if (CMP_NE(LO8(edx), MEM8(esi + 0x26))) goto loc_0021723D; /* jne: not equal / not zero */

loc_00217267: ;
    MEM8(esi + 1) = MEM8(esi + 1) & 0xBF;
    MEM8(esi + 0x10) = MEM8(esi + 0x10) | 2;
    edi = MEM32(ebp + -12);
    MEM32(esi + 0x28) = ecx;
    esi = MEM32(ebp + -16);

}

/**
 * sub_00217278
 * Original: 0x00217278 - 0x00217298 (32 bytes, 13 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217278(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217278: ;
    SET_LO8(ecx, MEM8(ebp + -1));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_00217281: ;
    PUSH32(esp, edi);
    MEM32(edi + 4) = esi;
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_0021728A: ;
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_00217291
 * Original: 0x00217291 - 0x00217298 (7 bytes, 2 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217291(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217291: ;
    esi = 0xC0000B00u;
    g_seh_ebp = ebp; sub_00217278(); return; /* tail jmp 0x00217278 */

}

/**
 * sub_00217298
 * Original: 0x00217298 - 0x002172D2 (58 bytes, 24 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217298(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217298: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = edx;
    esi = MEM32(edi + 0x10);
    ebx = ecx;
    ebp = 0; /* xor self */
    { uint32_t _icall_t = MEM32(0x217B00); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002172AC: ;
    /* test MEM8(esi + 0x10), 2 - flags set for next jcc */
    MEM8(esp + 0x13) = LO8(eax);
    if (TEST_Z(MEM8(esi + 0x10), 2)) { g_seh_ebp = ebp; sub_002172D2(); return; } /* je: equal / zero */

loc_002172B6: ;
    MEM8(esi + 1) = MEM8(esi + 1) | 0x40;
    ecx = ebx;
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_002172C1: ;
    eax++;
    eax++;
    MEM32(esi + 0x1C) = eax;
    SET_LO8(eax, MEM8(esi + 0x10));
    SET_LO8(eax, LO8(eax) & 0xFD);
    SET_LO8(eax, LO8(eax) | 4);
    MEM8(esi + 0x10) = LO8(eax);
    g_seh_ebp = ebp; sub_002172D7(); return; /* tail jmp 0x002172D7 */

}

/**
 * sub_002172D2
 * Original: 0x002172D2 - 0x002172D7 (5 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002172D2(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002172D2: ;
    ebp = 0xC0000F00u;

}

/**
 * sub_002172D7
 * Original: 0x002172D7 - 0x002172F2 (27 bytes, 12 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002172D7(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002172D7: ;
    SET_LO8(ecx, MEM8(esp + 0x13));
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_t = MEM32(0x217AFC); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002172E1: ;
    PUSH32(esp, edi);
    MEM32(edi + 4) = ebp;
    PUSH32(esp, 0); sub_002128D5(); /* call 0x002128D5 */

loc_002172EA: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebp;
    POP32(esp, ebp);
    POP32(esp, ebx);
    POP32(esp, ecx);
    esp += 4; return; /* ret */

}

/**
 * sub_002172F2
 * Original: 0x002172F2 - 0x00217399 (167 bytes, 67 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002172F2(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002172F2: ;
    PUSH32(esp, ebp);
    ebp = esp;
    esp = esp - 0x2C;
    PUSH32(esp, ebx);
    ebx = edx;
    eax = MEM32(ebx);
    PUSH32(esp, esi);
    edx = MEM32(ebx + 0x20);
    MEM32(ebx + 8) = MEM32(ebx + 8) & 0;
    esi = eax;
    esi = esi >> 0x1C;
    MEM32(ebp + -44) = esi;
    eax = eax >> 0x18;
    eax = eax & 7;
    PUSH32(esp, edi);
    eax++;
    MEM32(ebp + -40) = eax;
    eax = MEM32(ebx + 0x28);
    esi = ebx + 0x10;
    MEM32(ebp + -8) = esi;
    edi = ebp + -36;
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(ebp + -16) = eax;
    eax = MEM32(ebx + 0x24);
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    edi = MEM32(edx + 0x2C);
    MEM32(ebp + -20) = eax;
    eax = ZX8(MEM8(ebx + 0x2D));
    esi = ebx;
    esi = esi - MEM32(0xC6B9A0);
    eax = eax << 6;
    MEM32(eax + edi + 8) = esi;
    /* test MEM8(edx + 0x10), 1 - flags set for next jcc */
    MEM32(ebp + -4) = edx;
    MEM32(ebp + -12) = esi;
    if (TEST_Z(MEM8(edx + 0x10), 1)) { g_seh_ebp = ebp; sub_00217399(); return; } /* je: equal / zero */

loc_00217353: ;
    PUSH32(esp, 0); sub_0021674D(); /* call 0x0021674D */

loc_00217358: ;
    ecx = MEM32(ebp + -4);
    esi = eax;
    eax = MEM32(ecx + 0x28);
    esi++;
    edx = eax;
    edx = edx - esi;
    if (((int32_t)edx < 0)) goto loc_00217369; /* js: sign (negative) */

loc_00217367: ;
    esi = eax;

loc_00217369: ;
    edi = MEM32(ebp + -8);
    MEM16(ebx) = LO16(esi);
    eax = 0; /* xor self */
    SET_LO8(eax, MEM8(ebx + 3));
    eax = eax & 7;
    eax = eax + esi + 1;
    MEM32(ecx + 0x28) = eax;
    esi = ebx + 0x30;
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    MEM32(edi) = MEM32(esi); esi += 4; edi += 4; /* movsd */
    eax = ZX8(MEM8(ecx + 0x26));
    esi = ZX8(MEM8(ecx + 0x24));
    eax++;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)esi));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)esi)); }
    esi = MEM32(ebp + -12);
    MEM8(ecx + 0x26) = LO8(edx);
    g_seh_ebp = ebp; sub_002173D0(); return; /* tail jmp 0x002173D0 */

}

/**
 * sub_00217399
 * Original: 0x00217399 - 0x002173D0 (55 bytes, 20 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217399(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217399: ;
    edi = MEM32(0x217B90);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, MEM32(ebx + 4));
    { uint32_t _icall_t = edi; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002173A6: ;
    eax = MEM32(ebx + 0xC);
    ecx = MEM32(ebx + 4);
    ecx = ecx ^ eax;
    if (TEST_Z(ecx, 0xFFFFF000u)) goto loc_002173BB; /* je: equal / zero */

loc_002173B6: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, eax);
    { uint32_t _icall_t = edi; PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002173BB: ;
    ecx = MEM32(ebp + -4);
    SET_LO8(eax, MEM8(ecx + 0x25));
    /* cmp LO8(eax), MEM8(ecx + 0x24) - flags set for next jcc */
    SET_LO8(edx, (CMP_EQ(LO8(eax), MEM8(ecx + 0x24))) ? 1 : 0); /* sete */
    SET_LO8(eax, LO8(eax) - 1);
    /* test LO8(edx), LO8(edx) - flags set for next jcc */
    MEM8(ecx + 0x25) = LO8(eax);
    if (TEST_Z(LO8(edx), LO8(edx))) { g_seh_ebp = ebp; sub_002173D3(); return; } /* je: equal / zero */

}

/**
 * sub_002173D0
 * Original: 0x002173D0 - 0x002173E2 (18 bytes, 10 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002173D0(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002173D0: ;
    MEM32(ecx + 4) = esi;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, MEM32(ebp + -16));
    eax = ebp + -44;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(ebp + -20); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002173DD: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002173D3
 * Original: 0x002173D3 - 0x002173E2 (15 bytes, 9 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002173D3(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002173D3: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, MEM32(ebp + -16));
    eax = ebp + -44;
    PUSH32(esp, eax);
    { uint32_t _icall_t = MEM32(ebp + -20); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_002173DD: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp); /* leave */
    esp += 4; return; /* ret */

}

/**
 * sub_002173E2
 * Original: 0x002173E2 - 0x00217402 (32 bytes, 9 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002173E2(void)
{
    int _flags = 0; /* fallback flag var */

loc_002173E2: ;
    eax = MEM32(0xC6B9AC);
    if (TEST_Z(eax, eax)) goto loc_002173F4; /* je: equal / zero */

loc_002173EB: ;
    ecx = MEM32(eax + 0x14);
    MEM32(0xC6B9AC) = ecx;

loc_002173F4: ;
    SET_LO8(ecx, MEM8(esp + 4));
    MEM8(eax + 2) = MEM8(eax + 2) & 0xFE;
    MEM8(eax + 0x1F) = LO8(ecx);
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00217402
 * Original: 0x00217402 - 0x00217414 (18 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217402(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217402: ;
    SET_LO16(eax, MEM16(esp + 4));
    if (CMP_AE(MEM16(0xC6B9C2), LO16(eax))) { g_seh_ebp = ebp; sub_00217414(); return; } /* jae: above or equal (unsigned >=) */

loc_00217410: ;
    eax = 0; /* xor self */
    g_seh_ebp = ebp; sub_0021741E(); return; /* tail jmp 0x0021741E */

}

/**
 * sub_00217414
 * Original: 0x00217414 - 0x0021741E (10 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217414(void)
{

loc_00217414: ;
    MEM16(0xC6B9C2) = MEM16(0xC6B9C2) - LO16(eax);
    eax = 0; /* xor self */
    eax++;

}

/**
 * sub_0021741E
 * Original: 0x0021741E - 0x00217421 (3 bytes, 1 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021741E(void)
{

loc_0021741E: ;
    esp += 8; return; /* ret 4 */

}

/**
 * sub_00217421
 * Original: 0x00217421 - 0x00217433 (18 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217421(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217421: ;
    SET_LO16(eax, MEM16(esp + 4));
    if (CMP_AE(MEM16(0xC6B9C6), LO16(eax))) { g_seh_ebp = ebp; sub_00217433(); return; } /* jae: above or equal (unsigned >=) */

loc_0021742F: ;
    eax = 0; /* xor self */
    g_seh_ebp = ebp; sub_0021743D(); return; /* tail jmp 0x0021743D */

}

/**
 * sub_00217433
 * Original: 0x00217433 - 0x0021743D (10 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217433(void)
{

loc_00217433: ;
    MEM16(0xC6B9C6) = MEM16(0xC6B9C6) - LO16(eax);
    eax = 0; /* xor self */
    eax++;

}

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
void sub_00217440(void)
{
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
    POP32(esp, esi);
    if (CMP_NE(MEM8(esi + 0x11), 0)) goto loc_00217472; /* jne: not equal / not zero */

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
void sub_00217473(void)
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
 * Original: 0x002174AD - 0x00217513 (102 bytes, 35 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002174AD(void)
{
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
    if (CMP_EQ(esi, eax)) { g_seh_ebp = ebp; sub_00217513(); return; } /* je: equal / zero */

loc_0021750A: ;
    eax = MEM32(0xC6B9A0);
    esi = esi + eax;
    g_seh_ebp = ebp; sub_0021752C(); return; /* tail jmp 0x0021752C */

}

/**
 * sub_00217513
 * Original: 0x00217513 - 0x0021752C (25 bytes, 8 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217513(void)
{
    uint32_t ebp;
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

}

/**
 * sub_0021752C
 * Original: 0x0021752C - 0x002177AB (639 bytes, 204 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0021752C(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0021752C: ;
    /* cmp MEM8(ebx + 0x11), 0 - flags set for next jcc */
    MEM32(ebp + -4) = esi;
    if (CMP_NE(MEM8(ebx + 0x11), 0)) goto loc_0021759B; /* jne: not equal / not zero */

loc_00217535: ;
    eax = 0; /* xor self */
    SET_LO8(eax, 0xFE);
    SET_LO8(eax, LO8(eax) - MEM8(ebp + -24));
    PUSH32(esp, eax);
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
    MEM32(ebp + -28) = eax;
    MEM32(ebp + -12) = edx;
    if (CMP_NE(MEM32(ebp + -16), 0)) goto loc_002175C5; /* jne: not equal / not zero */

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
    PUSH32(esp, MEM32(ebp + -24));
    SET_LO8(eax, (CMP_NE(MEM8(edi + 0x1C), 2)) ? 1 : 0); /* setne */
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
    MEM32(ebx + 4) = eax;
    if (CMP_NE(MEM8(ebx + 0x20), 0)) goto loc_0021777E; /* jne: not equal / not zero */

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
void sub_002177AB(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */

loc_002177AB: ;
    PUSH32(esp, ebp);
    ebp = esp;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    esi = edx;
    /* cmp MEM32(esi + 0x28), 0 - flags set for next jcc */
    MEM32(ebp + -4) = ecx;
    if (CMP_EQ(MEM32(esi + 0x28), 0)) goto loc_002177FB; /* je: equal / zero */

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
void sub_002177FE(void)
{
    int _flags = 0; /* fallback flag var */

loc_002177FE: ;
    PUSH32(esp, esi);
    esi = ecx;
    if (CMP_EQ(MEM32(esi + 0x424), 0)) goto loc_00217849; /* je: equal / zero */

loc_0021780A: ;
    PUSH32(esp, edi);

loc_0021780B: ;
    edi = MEM32(esi + 0x424);
    eax = 0; /* xor self */
    SET_LO16(eax, MEM16(edi + 0x20));
    PUSH32(esp, eax);
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
void sub_0021784B(void)
{
    int _flags = 0; /* fallback flag var */

loc_0021784B: ;
    PUSH32(esp, esi);
    esi = ecx;
    if (CMP_EQ(MEM32(esi + 0x41C), 0)) goto loc_00217896; /* je: equal / zero */

loc_00217857: ;
    PUSH32(esp, edi);

loc_00217858: ;
    edi = MEM32(esi + 0x41C);
    eax = 0; /* xor self */
    SET_LO16(eax, MEM16(edi + 0x20));
    PUSH32(esp, eax);
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
 * Original: 0x00217898 - 0x002178AA (18 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217898(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00217898: ;
    eax = MEM32(esp + 4);
    if (CMP_BE(MEM16(eax + 0x20), 3)) { g_seh_ebp = ebp; sub_002178AA(); return; } /* jbe: below or equal (unsigned <=) */

loc_002178A3: ;
    eax = 0x80000500u;
    g_seh_ebp = ebp; sub_002178C8(); return; /* tail jmp 0x002178C8 */

}

/**
 * sub_002178AA
 * Original: 0x002178AA - 0x002178C8 (30 bytes, 11 insns)
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
 * Original: 0x002178CB - 0x002178DE (19 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002178CB(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */
    int _flags = 0; /* fallback flag var */

loc_002178CB: ;
    SET_LO16(eax, MEM16(edx + 0x20));
    if (CMP_BE(LO16(eax), MEM16(0xC6B9C4))) { g_seh_ebp = ebp; sub_002178DE(); return; } /* jbe: below or equal (unsigned <=) */

loc_002178D8: ;
    eax = 0x80000500u;
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
    int _flags = 0; /* fallback flag var */

loc_002178DE: ;
    eax = ecx + 0x424;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_002178F4; /* je: equal / zero */

loc_002178E9: ;
    eax = MEM32(ecx + 0x428);
    MEM32(eax + 0x24) = edx;
    goto loc_002178F6;

loc_002178F4: ;
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
 * Original: 0x00217907 - 0x0021791A (19 bytes, 5 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217907(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */
    int _flags = 0; /* fallback flag var */

loc_00217907: ;
    SET_LO16(eax, MEM16(edx + 0x20));
    if (CMP_BE(LO16(eax), MEM16(0xC6B9C0))) { g_seh_ebp = ebp; sub_0021791A(); return; } /* jbe: below or equal (unsigned <=) */

loc_00217914: ;
    eax = 0x80000500u;
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
    int _flags = 0; /* fallback flag var */

loc_0021791A: ;
    eax = ecx + 0x41C;
    if (CMP_EQ(MEM32(eax), 0)) goto loc_00217930; /* je: equal / zero */

loc_00217925: ;
    eax = MEM32(ecx + 0x420);
    MEM32(eax + 0x24) = edx;
    goto loc_00217932;

loc_00217930: ;
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
 * Original: 0x00217943 - 0x002179A2 (95 bytes, 37 insns)
 * Category: game_input
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217943(void)
{
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
    if ((eax == 0)) { g_seh_ebp = ebp; sub_002179A2(); return; } /* je: equal / zero */

loc_0021797D: ;
    eax--;
    eax--;
    if ((eax == 0)) goto loc_00217997; /* je: equal / zero */

loc_00217981: ;
    eax--;
    if ((eax == 0)) goto loc_0021798B; /* je: equal / zero */

loc_00217984: ;
    ebx = 0x80000600u;
    g_seh_ebp = ebp; sub_002179B1(); return; /* tail jmp 0x002179B1 */

loc_0021798B: ;
    PUSH32(esp, esi);
    edx = edi;
    ecx = ebx;
    PUSH32(esp, 0); sub_00217898(); /* call 0x00217898 */

loc_00217995: ;
    g_seh_ebp = ebp; sub_002179AB(); return; /* tail jmp 0x002179AB */

loc_00217997: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_002178CB(); /* call 0x002178CB */

loc_002179A0: ;
    g_seh_ebp = ebp; sub_002179AB(); return; /* tail jmp 0x002179AB */

}

/**
 * sub_002179A2
 * Original: 0x002179A2 - 0x002179AB (9 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179A2(void)
{

loc_002179A2: ;
    edx = esi;
    ecx = ebx;
    PUSH32(esp, 0); sub_00217907(); /* call 0x00217907 */

}

/**
 * sub_002179AB
 * Original: 0x002179AB - 0x002179B1 (6 bytes, 3 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179AB(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */
    int _flags = 0; /* fallback flag var */

loc_002179AB: ;
    ebx = eax;
    if (CMP_GE(ebx & ebx, 0)) { g_seh_ebp = ebp; sub_002179B9(); return; } /* jge: greater or equal (signed >=) */

}

/**
 * sub_002179B1
 * Original: 0x002179B1 - 0x002179CC (27 bytes, 13 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_002179B1(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_002179B1: ;
    MEM16(esi + 0x22) = MEM16(esi + 0x22) & 0;
    MEM8(edi + 0x26) = MEM8(edi + 0x26) - 1;
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
 * Original: 0x002179CC - 0x002179FB (47 bytes, 30 insns)
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
    if (_flags /* jbe: below or equal (unsigned <=) */) { sub_00217A3A(); return; }

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

}

/**
 * sub_00217A3A
 * Original: 0x00217A3A - 0x00252050 (239126 bytes, 324 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00217A3A(void)
{
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
    if (((int32_t)MEM8(eax + -2147483389) < 0)) goto loc_00217ADE; /* jl: less (signed <) */

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
    if (_flags /* ja: above (unsigned >) */) goto loc_00217B8E;

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
    if (_flags /* jnp: not parity */) (void)0; /* goto loc_00217C00 - dead code, label not in function */

loc_00217BDF: ;
    MEM8(esp + eax * 2 + 0x65) = MEM8(esp + eax * 2 + 0x65) + LO8(ebx);
    if (_flags /* jbe: below or equal (unsigned <=) */) (void)0; /* goto loc_00217C4E - dead code, label not in function */

loc_00217BE5: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    ebx++;
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
    if (_flags /* jbe: below or equal (unsigned <=) */) (void)0; /* goto loc_00217C72 - dead code, label not in function */

loc_00217C09: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217C74 - dead code, label not in function */

loc_00217C10: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61705C30);
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
    if (_flags /* jbe: below or equal (unsigned <=) */) goto loc_00217C9E;

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
    if (_flags /* js: sign (negative) */) goto loc_00217CB7;

loc_00217C55: ;
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
    if (_flags /* jbe: below or equal (unsigned <=) */) (void)0; /* goto loc_00217CDA - dead code, label not in function */

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
    if (((int32_t)MEM8(eax) >= 0)) goto loc_00217CC0; /* jns: not sign (positive) */

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
    if (((int32_t)HI8(eax) < 0)) goto loc_00217CFC; /* jl: less (signed <) */

loc_00217CDB: ;
    MEM8(esp + eax * 2 + 0x65) = MEM8(esp + eax * 2 + 0x65) + LO8(ebx);

loc_00217CDF: ;
    if (_flags /* jbe: below or equal (unsigned <=) */) goto loc_00217D4A;

loc_00217CE1: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D4C - dead code, label not in function */

loc_00217CE8: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61505C30);
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
    if (_flags /* jbe: below or equal (unsigned <=) */) (void)0; /* goto loc_00217D82 - dead code, label not in function */

loc_00217D19: ;
    /* TODO: arpl word ptr [ebp + 0x5c], sp */
    eax--;
    /* TODO: popal  */
    if (_flags /* jb: below (unsigned <) */) (void)0; /* goto loc_00217D84 - dead code, label not in function */

loc_00217D20: ;
    esi = (uint32_t)((int32_t)MEM32(ebx + 0x6B) * (int32_t)0x61705C30);
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
    if (_flags /* jbe: below or equal (unsigned <=) */) goto loc_00217DAE;

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
    if (_flags /* js: sign (negative) */) goto loc_00217DC7;

loc_00217D65: ;
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
    if (_flags /* jbe: below or equal (unsigned <=) */) (void)0; /* goto loc_00217DEA - dead code, label not in function */

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
    if (((int32_t)MEM8(eax) >= 0)) goto loc_00217DD0; /* jns: not sign (positive) */

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
