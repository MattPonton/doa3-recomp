/**
 * DOA3 - post-movie screen tasks missed by function detection.
 *
 * Both of these are task entry points: the scheduler is handed their address
 * as data (sub_0009E4xx task-create), never a direct call, so the direct-call
 * scan never promoted them and RECOMP_ICALL had no body to dispatch to.
 *
 * sub_000CEF80 is the screen that runs when the intro movie ends. It builds
 * the screen object at 0x008610E0 (via sub_000CF500, the only writer of the
 * per-player slot flags at +0x23) and then drives that object's state machine,
 * dispatching each state through the handler table at 0x003691E0. Without it
 * the object stayed all-zero, the state byte 0x008612AD never left 0, and the
 * title screen was never armed -- the frame held a full-screen blit of an
 * empty surface.
 *
 * sub_0004FAA0 is attract-mode task 3, which shares that teardown path.
 *
 * Prototypes are declared here and in recomp_dispatch.c rather than in
 * recomp_funcs.h: that header is included by every generated unit, so touching
 * it forces a full rebuild of the whole recompilation.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <stdio.h>
#include <math.h>

void sub_0004FAA0(void);
void sub_0006F5B0(void);
void sub_000318D0(void);
void sub_000BA810(void);
void sub_00048140(void);
void sub_00050380(void);
void sub_000C89A0(void);
void sub_000CEF80(void);

/**
 * sub_0006F5B0 -- story-mode cutscene task.
 * Original: 0x0006F5B0 - 0x0006F5CC (inside sub_0006F560's range, never a
 * call target). sub_00079530 / sub_00077700 set word 0x4A03A4 to the scene
 * and register this address as task 3 through sub_0009E422; with no lifted
 * body the task never ran, so neither story cutscene ever played (story
 * mode went fight -> fight with no scene). Runs the match task on scene
 * 0x306538[0x4A03A4], then tail-jumps to the task exit sub_0009E525.
 */
void sub_0006F5B0(void)
{
    eax = ZX16(MEM16(0x4A03A4));
    ecx = ZX8(MEM8(eax + 0x306538));
    PUSH32(esp, ecx);
    PUSH32(esp, 0); sub_0007DA70(); /* call 0x0007DA70 */
    esp = esp + 4;
    sub_0009E525(); return;         /* jmp 0x0009E525 */
}

/**
 * sub_0004FAA0
 * Original: 0x0004FAA0 - 0x0004FB04 (100 bytes, 22 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_0004FAA0(void)
{
    uint32_t ebp;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0004FAA0: ;
    eax = ZX8(MEM8(0x48E65F));
    ecx = MEM32(eax * 4 + 0x2FD5DC);
    PUSH32(esp, ebx);
    SET_LO8(ebx, 0); /* xor self */
    PUSH32(esp, ecx);
    MEM8(0x47E723) = LO8(ebx);
    PUSH32(esp, 0); sub_0007DA70(); /* call 0x0007DA70 */

loc_0004FABD: ;
    SET_LO8(eax, MEM8(0x48E65F));
    SET_LO8(eax, LO8(eax) ^ 1);
    esp = esp + 4;
    MEM8(0x48E65F) = LO8(eax);
    PUSH32(esp, 0); sub_0007F790(); /* call 0x0007F790 */

loc_0004FAD1: ;
    PUSH32(esp, 0); sub_0009EF10(); /* call 0x0009EF10 */

loc_0004FAD6: ;
    PUSH32(esp, 0); sub_0009F010(); /* call 0x0009F010 */

loc_0004FADB: ;
    PUSH32(esp, 0); sub_0009F540(); /* call 0x0009F540 */

loc_0004FAE0: ;
    PUSH32(esp, 0); sub_0007F770(); /* call 0x0007F770 */

loc_0004FAE5: ;
    MEM8(0x85CEC4) = LO8(ebx);
    MEM8(0x48A528) = LO8(ebx);
    MEM8(0x47E723) = LO8(ebx);
    MEM8(0x480B70) = 2;
    POP32(esp, ebx);
    g_seh_ebp = ebp; sub_0009E525(); return; /* tail jmp 0x0009E525 */

}

/**
 * sub_000CEF80
 * Original: 0x000CEF80 - 0x000CF05C (220 bytes, 53 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_000CEF80(void)
{
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000CEF80: ;
    { static int n = 0; if (n < 4) { n++;
      fprintf(stderr, "[SCRTASK] sub_000CEF80 entered\n");
      fflush(stderr); } }

    ecx = 0x4902F8;
    PUSH32(esp, 0); sub_000691A0(); /* call 0x000691A0 */

loc_000CEF8A: ;
    ecx = 0x490B38;
    PUSH32(esp, 0); sub_000691A0(); /* call 0x000691A0 */

loc_000CEF94: ;
    eax = MEM32(0x48FD70);
    PUSH32(esp, 0);
    eax = eax + 0x4E800;
    PUSH32(esp, eax);
    PUSH32(esp, 0x4907B8);
    PUSH32(esp, 0); sub_0006AD20(); /* call 0x0006AD20 */

loc_000CEFAB: ;
    ecx = MEM32(0x48FD70);
    PUSH32(esp, 0);
    ecx = ecx + 0xBE800;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x490838);
    PUSH32(esp, 0); sub_0006AD20(); /* call 0x0006AD20 */

loc_000CEFC4: ;
    PUSH32(esp, 0x8610E0);
    PUSH32(esp, 0); sub_000CF500(); /* call 0x000CF500 */

loc_000CEFCE: ;
    esp = esp + 0x1C;

loc_000CEFD1: ;
    {   /* DOA3 DIAG: the post-movie screen object as it runs.
         * 0x8612AD is the state byte this loop dispatches on (handler
         * table 0x003691E0); case 10 is what arms the title screen by
         * setting 0x47E74C = 2. The per-player slot flags at
         * 0x8610E0 + 0x23 + i*0x38 gate sub_000C5D70 -> sub_000C4D00,
         * the only writer of that state byte. */
        extern uint32_t g_blkC5D70;
        extern unsigned g_c5d70cnt[16];
        static unsigned t = 0, shown = 0;
        if ((t++ % 300u) == 1 && shown < 20) {
            shown++;
            fprintf(stderr, "[SCROBJ] state8612AD=%d slots=%u/%u/%u/%u arm47E74C=%u join=%u/%u/%u/%u gates=%u/%u/%u blkC5D70=0x000%05X cd=%d hits[605B=%u exp6066=%u st10_6081=%u call60E0=%u 5DC6=%u 5E5B=%u nz6073=%u] bail[flag=%u ebx=%u c4ee0=%u write=%u] flagw=%04X/%04X/%04X/%04X ebx0=%d atEntry[edge=%u flagset=%u held=%u]\n",
                    (int)(int8_t)MEM8(0x8612AD),
                    MEM8(0x8610E0 + 0x23 + 0 * 0x38),
                    MEM8(0x8610E0 + 0x23 + 1 * 0x38),
                    MEM8(0x8610E0 + 0x23 + 2 * 0x38),
                    MEM8(0x8610E0 + 0x23 + 3 * 0x38),
                    MEM8(0x47E74C),
                    MEM8(0x48E608), MEM8(0x48E609), MEM8(0x48E60A), MEM8(0x48E60B),
                    MEM8(0x48A39C), MEM8(0x47E780), MEM8(0x48A490),
                    g_blkC5D70,
                    /* the gate at 0x000C605B / 0x000C60D9 reads a signed
                     * 16-bit countdown at obj + (state + 0xC) * 20 + 0xE */
                    (int)(int16_t)MEM16(0x8610E0 +
                        ((uint32_t)(int8_t)MEM8(0x8612AD) + 0xC) * 20 + 0xE),
                    g_c5d70cnt[0], g_c5d70cnt[1], g_c5d70cnt[2],
                    g_c5d70cnt[3], g_c5d70cnt[4], g_c5d70cnt[5], g_c5d70cnt[6],
                    g_c5d70cnt[7], g_c5d70cnt[8], g_c5d70cnt[9], g_c5d70cnt[10],
                    /* the per-player flag word the first bail tests */
                    MEM16(0x86132A + 0 * 8), MEM16(0x86132A + 1 * 8),
                    MEM16(0x86132A + 2 * 8), MEM16(0x86132A + 3 * 8),
                    /* [ebx] for player 0: obj + 0x30 */
                    (int)MEM32(0x8610E0 + 0x30),
                    g_c5d70cnt[11], g_c5d70cnt[12], g_c5d70cnt[13]);
            { extern void doa3_flowcount_dump(void);
              doa3_flowcount_dump(); }
            { extern void recomp_icall_census_dump(void);
              recomp_icall_census_dump(); }
            fflush(stderr);
        }
    }
    SET_LO8(eax, MEM8(0x8612AD));
    /* test LO8(eax), LO8(eax) - flags set for next jcc */
    MEM8(0x8612B0) = LO8(eax);
    if (TEST_S(LO8(eax), LO8(eax))) goto loc_000CF00D; /* jl: less (signed <) */

loc_000CEFDF: ;
    if (CMP_GE(LO8(eax), 0xA)) goto loc_000CF039; /* jge: greater or equal (signed >=) */

loc_000CEFE3: ;
    eax = 0x8610E0;
    PUSH32(esp, 0); sub_000CDFE0(); /* call 0x000CDFE0 */

loc_000CEFED: ;
    edx = (uint32_t)(int32_t)SMEM8(0x8612AD);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x8610E0);
    { uint32_t _icall_t = MEM32(edx * 4 + 0x3691E0); PUSH32(esp, 0); RECOMP_ICALL_SAFE(_icall_t, _icall_esp); } /* indirect call */
    }

loc_000CF000: ;
    esp = esp + 4;
    PUSH32(esp, 0); sub_000CE720(); /* call 0x000CE720 */

loc_000CF008: ;
    SET_LO8(eax, MEM8(0x8612AD));

loc_000CF00D: ;
    if (CMP_GE(LO8(eax), 0xA)) goto loc_000CF039; /* jge: greater or equal (signed >=) */

loc_000CF011: ;
    eax = SX8(LO8(eax));
    PUSH32(esp, 0x8610E0);
    MEM32(0x861350) = eax;
    PUSH32(esp, 0); sub_000CE4D0(); /* call 0x000CE4D0 */

loc_000CF023: ;
    PUSH32(esp, 0x8610E0);
    PUSH32(esp, 0); sub_000CD8C0(); /* call 0x000CD8C0 */

loc_000CF02D: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_0009E562(); /* call 0x0009E562 */

loc_000CF034: ;
    esp = esp + 0xC;
    goto loc_000CEFD1;

loc_000CF039: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_0009E562(); /* call 0x0009E562 */

loc_000CF040: ;
    PUSH32(esp, 0); sub_000CE6D0(); /* call 0x000CE6D0 */

loc_000CF045: ;
    PUSH32(esp, 0); sub_0007E300(); /* call 0x0007E300 */

loc_000CF04A: ;
    PUSH32(esp, 0x8610E0);
    PUSH32(esp, 0); sub_000CEB20(); /* call 0x000CEB20 */

loc_000CF054: ;
    esp = esp + 8;
    g_seh_ebp = ebp; sub_0009E525(); return; /* tail jmp 0x0009E525 */

}

/**
 * sub_000318D0
 * Original: 0x000318D0 - 0x00031950 (128 bytes, 32 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_000318D0(void)
{

loc_000318D0: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    ebx = MEM32(esi + 0x73C);
    PUSH32(esp, edi);
    ecx = 0; /* xor self */
    PUSH32(esp, 0); sub_00154610(); /* call 0x00154610 */

loc_000318E4: ;
    PUSH32(esp, 0); sub_00154670(); /* call 0x00154670 */

loc_000318E9: ;
    edi = esi + 0x630;
    ecx = edi;
    PUSH32(esp, 0); sub_00154690(); /* call 0x00154690 */

loc_000318F6: ;
    ecx = 1;
    MEM32(esi + 0x430) = edi;
    MEM32(esi + 0x4FC) = edi;
    PUSH32(esp, 0); sub_00154640(); /* call 0x00154640 */

loc_0003190C: ;
    eax = 0; /* xor self */
    MEM8(esi + 0xA31) = LO8(eax);
    MEM8(esi + 0xA34) = LO8(eax);
    MEM32(esi + 0x730) = eax;
    MEM8(esi + 0x740) = LO8(eax);
    SET_LO8(eax, MEM8(esi + 0xB0C));
    MEM8(esi + 0x741) = LO8(eax);
    ecx = (uint32_t)(int32_t)SMEM16(ebx + 2);
    eax = MEM32(esi + 0x73C);
    ecx = ecx << 2;
    eax = eax + ecx;
    POP32(esp, edi);
    MEM32(esi + 0x73C) = eax;
    POP32(esp, esi);
    eax = 1;
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}

/**
 * sub_000BA810
 * Original: 0x000BA810 - 0x000BA850 (64 bytes, 24 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_000BA810(void)
{
    int _flags = 0; /* fallback flag var */

loc_000BA810: ;
    PUSH32(esp, 0); sub_000B9060(); /* call 0x000B9060 */

loc_000BA815: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_0009E562(); /* call 0x0009E562 */

loc_000BA81C: ;
    esp = esp + 4;
    PUSH32(esp, 0); sub_000BA030(); /* call 0x000BA030 */

loc_000BA824: ;
    if (CMP_NE(MEM8(0x48A528), 5)) goto loc_000BA815; /* jne: not equal / not zero */

loc_000BA82D: ;
    if (CMP_NE(MEM8(0x47E727), 4)) goto loc_000BA83F; /* jne: not equal / not zero */

loc_000BA836: ;
    SET_LO8(eax, MEM8(0x4A0450));
    if (TEST_NZ(LO8(eax), LO8(eax))) goto loc_000BA815; /* jne: not equal / not zero */

loc_000BA83F: ;
    PUSH32(esp, 0); sub_000B97D0(); /* call 0x000B97D0 */

loc_000BA844: ;
    goto loc_000BA815;

    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */

}

/**
 * sub_00048140
 * Original: 0x00048140 - 0x00048190 (80 bytes, 33 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00048140(void)
{
    int _flags = 0; /* fallback flag var */

loc_00048140: ;
    PUSH32(esp, 0); sub_00047AC0(); /* call 0x00047AC0 */

loc_00048145: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_0009E562(); /* call 0x0009E562 */

loc_0004814C: ;
    esp = esp + 4;
    PUSH32(esp, 0); sub_00046C60(); /* call 0x00046C60 */

loc_00048154: ;
    PUSH32(esp, 0); sub_00048080(); /* call 0x00048080 */

loc_00048159: ;
    goto loc_00048145;

    /* nop */
    /* nop */
    /* nop */
    /* nop */
    /* nop */
    eax = MEM32(esp + 4);
    ecx = MEM32(eax + 0xC);
    edx = MEM32(eax);
    PUSH32(esp, esi);
    if (CMP_L(edx, MEM32(ecx + 0xC))) goto loc_00048173; /* jl: less (signed <) */

loc_0004816F: ;
    eax = 0; /* xor self */
    POP32(esp, esi);
    esp += 4; return; /* ret */

loc_00048173: ;
    esi = MEM32(ecx);
    PUSH32(esp, edi);
    edi = MEM32(esi + edx * 4 + 0xC);
    edi = edi + esi;
    MEM32(eax + 4) = edi;
    ecx = MEM32(ecx + 8);
    edx = MEM32(ecx + edx * 4);
    POP32(esp, edi);
    MEM32(eax + 8) = edx;
    eax = 1;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

/**
 * sub_00050380
 * Original: 0x00050380 - 0x0005068C (780 bytes, 206 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_00050380(void)
{
    extern uint32_t g_blk50380;
    uint32_t ebp;
    int _flags = 0; /* fallback flag var */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00050380: ;
    g_blk50380 = 0x50380;
    MEM8(0x48E620) = 1;
    MEM8(0x48E621) = 0;
    PUSH32(esp, 0); sub_00082E90(); /* call 0x00082E90 */

loc_00050393: ;
    g_blk50380 = 0x50393;
    SET_LO8(eax, MEM8(0x48E622));
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00050668; /* je: equal / zero */

loc_000503A0: ;
    g_blk50380 = 0x503A0;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = esi | 0xFFFFFFFFu;

loc_000503A5: ;
    g_blk50380 = 0x503A5;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_0009E562(); /* call 0x0009E562 */

loc_000503AC: ;
    g_blk50380 = 0x503AC;
    eax = ZX8(MEM8(0x48E621));
    esp = esp + 4;
    if (CMP_A(eax, 4)) goto loc_00050659; /* ja: above (unsigned >) */

loc_000503BF: ;
    g_blk50380 = 0x503BF;
    { uint32_t _jt = MEM32(eax * 4 + 0x5068C); /* switch: 5 entries, 5 targets */
    if (_jt == 0x000503C6u) goto loc_000503C6;
    if (_jt == 0x00050430u) goto loc_00050430;
    if (_jt == 0x0005046Fu) goto loc_0005046F;
    if (_jt == 0x00050553u) goto loc_00050553;
    if (_jt == 0x000505F5u) goto loc_000505F5;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_000503C6: ;
    g_blk50380 = 0x503C6;
    eax = (uint32_t)(int32_t)SMEM8(0x4B83B8);
    edx = 0; /* xor self */
    ecx = 0xF;
    { uint64_t _dividend = ((uint64_t)edx << 32) | eax;
      eax = (uint32_t)(_dividend / (uint32_t)ecx);
      edx = (uint32_t)(_dividend % (uint32_t)ecx); }
    PUSH32(esp, 0xB);
    eax = SX8(LO8(edx));
    SET_LO8(eax, MEM8(eax + 0x2FD5B4));
    SET_LO8(edx, LO8(edx) + 1);
    MEM8(0x4B83B8) = LO8(edx);
    MEM8(0x48E623) = LO8(eax);
    MEM8(0x48A2D8) = LO8(eax);
    PUSH32(esp, 0); sub_0009E4D3(); /* call 0x0009E4D3 */

loc_000503F8: ;
    g_blk50380 = 0x503F8;
    PUSH32(esp, 0xD);
    PUSH32(esp, 0); sub_0009E4D3(); /* call 0x0009E4D3 */

loc_000503FF: ;
    g_blk50380 = 0x503FF;
    ecx = 0; /* xor self */
    SET_LO8(ecx, MEM8(0x48E623));
    PUSH32(esp, ecx);
    PUSH32(esp, 0);
    PUSH32(esp, 0); sub_0007F150(); /* call 0x0007F150 */

loc_0005040F: ;
    g_blk50380 = 0x5040F;
    esp = esp + 0x10;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_0005042A; /* je: equal / zero */

loc_00050416: ;
    g_blk50380 = 0x50416;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_00080BD0(); /* call 0x00080BD0 */

loc_00050427: ;
    g_blk50380 = 0x50427;
    esp = esp + 0x18;

loc_0005042A: ;
    g_blk50380 = 0x5042A;
    MEM8(0x48E621) = MEM8(0x48E621) + 1;

loc_00050430: ;
    g_blk50380 = 0x50430;
    edx = 0; /* xor self */
    SET_LO8(edx, MEM8(0x48E623));
    PUSH32(esp, edx);
    PUSH32(esp, 0);
    PUSH32(esp, 0); sub_0007EC00(); /* call 0x0007EC00 */

loc_00050440: ;
    g_blk50380 = 0x50440;
    esp = esp + 8;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00050659; /* je: equal / zero */

loc_0005044B: ;
    g_blk50380 = 0x5044B;
    PUSH32(esp, 0xB);
    PUSH32(esp, 0); sub_0009E502(); /* call 0x0009E502 */

loc_00050452: ;
    g_blk50380 = 0x50452;
    PUSH32(esp, 0xD);
    PUSH32(esp, 0); sub_0009E502(); /* call 0x0009E502 */

loc_00050459: ;
    g_blk50380 = 0x50459;
    SET_LO8(eax, MEM8(0x48E621));
    esp = esp + 8;
    SET_LO8(eax, LO8(eax) + 1);
    MEM8(0x48E611) = 0;
    MEM8(0x48E621) = LO8(eax);

loc_0005046F: ;
    g_blk50380 = 0x5046F;
    PUSH32(esp, 0); sub_00067260(); /* call 0x00067260 */

loc_00050474: ;
    g_blk50380 = 0x50474;
    if (TEST_NZ(LO8(eax), LO8(eax))) goto loc_0005049C; /* jne: not equal / not zero */

loc_00050478: ;
    g_blk50380 = 0x50478;
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFF);
    PUSH32(esp, 0);
    PUSH32(esp, 0); sub_00067880(); /* call 0x00067880 */

loc_0005048B: ;
    g_blk50380 = 0x5048B;
    PUSH32(esp, 0xFF);
    PUSH32(esp, 1);
    PUSH32(esp, 0);
    PUSH32(esp, 0); sub_000672F0(); /* call 0x000672F0 */

loc_00050499: ;
    g_blk50380 = 0x50499;
    esp = esp + 0x24;

loc_0005049C: ;
    g_blk50380 = 0x5049C;
    SET_LO8(eax, MEM8(0x480B70));
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_000504BF; /* je: equal / zero */

loc_000504A5: ;
    g_blk50380 = 0x504A5;
    PUSH32(esp, 0);
    PUSH32(esp, 3);
    MEM8(0x48E625) = 3;
    MEM8(0x48E624) = 0;
    PUSH32(esp, 0); sub_0004FF30(); /* call 0x0004FF30 */

loc_000504BC: ;
    g_blk50380 = 0x504BC;
    esp = esp + 8;

loc_000504BF: ;
    g_blk50380 = 0x504BF;
    eax = ZX8(MEM8(0x48E611));
    SET_LO8(ebx, MEM8(eax + 0x484D74));
    MEM8(eax + 0x484D74) = 0x32;
    PUSH32(esp, 0); sub_000D5700(); /* call 0x000D5700 */

loc_000504D8: ;
    g_blk50380 = 0x504D8;
    eax = ZX8(MEM8(0x48E611));
    MEM8(eax + 0x484D74) = LO8(ebx);
    PUSH32(esp, 0); sub_0002D030(); /* call 0x0002D030 */

loc_000504EA: ;
    g_blk50380 = 0x504EA;
    ecx = ZX8(MEM8(0x48E611));
    edx = 0; /* xor self */
    SET_LO8(edx, MEM8(ecx + 0x484D74));
    PUSH32(esp, edx);
    PUSH32(esp, 0); sub_0004FCD0(); /* call 0x0004FCD0 */

loc_000504FF: ;
    g_blk50380 = 0x504FF;
    PUSH32(esp, 0x2D060);
    PUSH32(esp, 0xB);
    MEM8(0x484C24) = 1;
    MEM8(0x481568) = 1;
    MEM32(0x48E628) = esi;
    PUSH32(esp, 0); sub_0009E422(); /* call 0x0009E422 */

loc_0005051F: ;
    g_blk50380 = 0x5051F;
    PUSH32(esp, 0xE0690);
    PUSH32(esp, 0xD);
    PUSH32(esp, 0); sub_0009E422(); /* call 0x0009E422 */

loc_0005052B: ;
    g_blk50380 = 0x5052B;
    PUSH32(esp, 0xB83B0);
    PUSH32(esp, 0xC);
    PUSH32(esp, 0); sub_0009E422(); /* call 0x0009E422 */

loc_00050537: ;
    g_blk50380 = 0x50537;
    SET_LO8(eax, MEM8(0x48E622));
    esp = esp + 0x1C;
    if (CMP_NE(LO8(eax), 1)) goto loc_00050548; /* jne: not equal / not zero */

loc_00050543: ;
    g_blk50380 = 0x50543;
    PUSH32(esp, 0); sub_000500B0(); /* call 0x000500B0 */

loc_00050548: ;
    g_blk50380 = 0x50548;
    MEM8(0x48E621) = MEM8(0x48E621) + 1;
    goto loc_00050659;

loc_00050553: ;
    g_blk50380 = 0x50553;
    if (CMP_NE(MEM32(0x48E628), esi)) goto loc_0005056E; /* jne: not equal / not zero */

loc_0005055B: ;
    g_blk50380 = 0x5055B;
    PUSH32(esp, 0); sub_00025600(); /* call 0x00025600 */

loc_00050560: ;
    g_blk50380 = 0x50560;
    PUSH32(esp, eax);
    PUSH32(esp, 0); sub_00026550(); /* call 0x00026550 */

loc_00050566: ;
    g_blk50380 = 0x50566;
    esp = esp + 4;
    MEM32(0x48E628) = eax;

loc_0005056E: ;
    g_blk50380 = 0x5056E;
    SET_LO8(eax, MEM8(0x480B70));
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_000505A3; /* je: equal / zero */

loc_00050577: ;
    g_blk50380 = 0x50577;
    PUSH32(esp, 0); sub_00067220(); /* call 0x00067220 */

loc_0005057C: ;
    g_blk50380 = 0x5057C;
    if (TEST_NZ(eax, eax)) goto loc_000505A3; /* jne: not equal / not zero */

loc_00050580: ;
    g_blk50380 = 0x50580;
    PUSH32(esp, 0); sub_00067260(); /* call 0x00067260 */

loc_00050585: ;
    g_blk50380 = 0x50585;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_000505A3; /* je: equal / zero */

loc_00050589: ;
    g_blk50380 = 0x50589;
    PUSH32(esp, 0);
    PUSH32(esp, 3);
    MEM8(0x48E625) = 3;
    MEM8(0x48E624) = 0;
    PUSH32(esp, 0); sub_0004FF30(); /* call 0x0004FF30 */

loc_000505A0: ;
    g_blk50380 = 0x505A0;
    esp = esp + 8;

loc_000505A3: ;
    g_blk50380 = 0x505A3;
    eax = MEM32(0x48E628);
    if (CMP_LE(eax & eax, 0)) goto loc_000505E6; /* jle: less or equal (signed <=) */

loc_000505AC: ;
    g_blk50380 = 0x505AC;
    SET_LO8(ecx, MEM8(0x48E622));
    eax--;
    /* test LO8(ecx), LO8(ecx) - flags set for next jcc */
    MEM32(0x48E628) = eax;
    if ((TEST_S(LO8(ecx), LO8(ecx)) || TEST_Z(LO8(ecx), LO8(ecx)))) goto loc_000505E6; /* jle: less or equal (signed <=) */

loc_000505BC: ;
    g_blk50380 = 0x505BC;
    if (CMP_NE(eax, 0x3C)) goto loc_000505E6; /* jne: not equal / not zero */

loc_000505C1: ;
    g_blk50380 = 0x505C1;
    if (CMP_NE(LO8(ecx), 1)) goto loc_000505DC; /* jne: not equal / not zero */

loc_000505C6: ;
    g_blk50380 = 0x505C6;
    eax = ZX8(MEM8(0x48E650));
    eax = eax + eax * 2;
    ecx = 0; /* xor self */
    SET_LO8(ecx, MEM8(eax * 4 + 0x2FD56C));
    PUSH32(esp, ecx);
    goto loc_000505DE;

loc_000505DC: ;
    g_blk50380 = 0x505DC;
    PUSH32(esp, 2);

loc_000505DE: ;
    g_blk50380 = 0x505DE;
    PUSH32(esp, 0); sub_00050220(); /* call 0x00050220 */

loc_000505E3: ;
    g_blk50380 = 0x505E3;
    esp = esp + 4;

loc_000505E6: ;
    g_blk50380 = 0x505E6;
    eax = MEM32(0x48E628);
    if (TEST_NZ(eax, eax)) goto loc_00050659; /* jne: not equal / not zero */

loc_000505EF: ;
    g_blk50380 = 0x505EF;
    MEM8(0x48E621) = MEM8(0x48E621) + 1;

loc_000505F5: ;
    g_blk50380 = 0x505F5;
    SET_LO8(eax, MEM8(0x48E622));
    if ((TEST_S(LO8(eax), LO8(eax)) || TEST_Z(LO8(eax), LO8(eax)))) goto loc_00050634; /* jle: less or equal (signed <=) */

loc_000505FE: ;
    g_blk50380 = 0x505FE;
    PUSH32(esp, 0); sub_00067260(); /* call 0x00067260 */

loc_00050603: ;
    g_blk50380 = 0x50603;
    if (TEST_Z(LO8(eax), LO8(eax))) goto loc_00050634; /* je: equal / zero */

loc_00050607: ;
    g_blk50380 = 0x50607;
    PUSH32(esp, 0xB);
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_0005060E: ;
    g_blk50380 = 0x5060E;
    PUSH32(esp, 0xD);
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_00050615: ;
    g_blk50380 = 0x50615;
    PUSH32(esp, 0xC);
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_0005061C: ;
    g_blk50380 = 0x5061C;
    SET_LO8(eax, MEM8(0x48E622));
    esp = esp + 0xC;
    SET_LO8(eax, LO8(eax) - 1);
    MEM8(0x48E622) = LO8(eax);
    MEM8(0x48E621) = 0;
    goto loc_00050659;

loc_00050634: ;
    g_blk50380 = 0x50634;
    edx = ZX8(MEM8(0x48E611));
    eax = 0; /* xor self */
    SET_LO8(eax, MEM8(edx + 0x484D74));
    PUSH32(esp, eax);
    PUSH32(esp, 0); sub_0004FCD0(); /* call 0x0004FCD0 */

loc_00050649: ;
    g_blk50380 = 0x50649;
    esp = esp + 4;
    MEM8(0x48E621) = 3;
    MEM32(0x48E628) = esi;

loc_00050659: ;
    g_blk50380 = 0x50659;
    SET_LO8(eax, MEM8(0x48E622));
    if (TEST_NZ(LO8(eax), LO8(eax))) goto loc_000503A5; /* jne: not equal / not zero */

loc_00050666: ;
    g_blk50380 = 0x50666;
    POP32(esp, esi);
    POP32(esp, ebx);

loc_00050668: ;
    g_blk50380 = 0x50668;
    PUSH32(esp, 0xB);
    MEM8(0x48E620) = 0;
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_00050676: ;
    g_blk50380 = 0x50676;
    PUSH32(esp, 0xD);
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_0005067D: ;
    g_blk50380 = 0x5067D;
    PUSH32(esp, 0xC);
    PUSH32(esp, 0); sub_0009E482(); /* call 0x0009E482 */

loc_00050684: ;
    g_blk50380 = 0x50684;
    esp = esp + 0xC;
    g_seh_ebp = ebp; sub_0009E525(); return; /* tail jmp 0x0009E525 */

}

/**
 * sub_000C89A0
 * Original: 0x000C89A0 - 0x000C89BC (28 bytes, 9 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void sub_000C89A0(void)
{
    int _flags = 0; /* fallback flag var */

loc_000C89A0: ;
    PUSH32(esp, 0); sub_00025600(); /* call 0x00025600 */

loc_000C89A5: ;
    ecx = MEM32(esp + 4);
    if (CMP_EQ(eax, ecx)) goto loc_000C89B6; /* je: equal / zero */

loc_000C89AD: ;
    PUSH32(esp, ecx);
    PUSH32(esp, 0); sub_00025300(); /* call 0x00025300 */

loc_000C89B3: ;
    esp = esp + 4;

loc_000C89B6: ;
    eax = 1;
    esp += 4; return; /* ret */

}

void sub_000DF670(void)
{
    MEM8(ecx + 4) = 0;
    MEM8(ecx + 5) = 1;
    esp += 4;
}

void sub_000E56A0(void)
{
    g_fp_stack[--g_fp_top & 7] = MEMF(0x1ED480);
    esp += 4;
}
