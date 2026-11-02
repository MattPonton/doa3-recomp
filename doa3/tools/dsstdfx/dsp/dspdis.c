/*
 * dspdis: disassembles the DSP56300 effect programs of a decrypted XDK
 * dsstdfx.bin (see fxdecrypt.py), one listing per effect.
 *
 * The decoder is xemu's DSP56300 core (hw/xbox/mcpx/apu/dsp/interp, by espes
 * and Matt Borgerson, adapted from Hatari / ARAnyM), compiled in unchanged:
 * GPL-2.0-or-later. This file is under the same licence.
 */
#include "dsp_cpu.c"

static dsp_core_t s_dsp;

int main(int argc, char **argv)
{
    FILE *f;
    static uint8_t img[1 << 20];
    size_t n;
    uint32_t desc, count, i, base_code;
    if (argc < 2) { fprintf(stderr, "usage: dspdis decrypted.bin [fx]\n"); return 1; }
    f = fopen(argv[1], "rb");
    if (!f) return 1;
    n = fread(img, 1, sizeof img, f);
    fclose(f);
    (void)n;
#define W(o) (*(uint32_t *)(img + (o)))
    desc = 0x818 + (W(0x804) + W(0x80C)) * 4;
    count = W(desc);
    base_code = 0x818;
    dsp56k_reset_cpu(&s_dsp);
    /* all effect code, P:0 = image offset 0x818 */
    for (i = 0; i < W(0x804) && i < DSP_PRAM_SIZE; i++)
        s_dsp.pram[i] = W(base_code + 4 * i) & 0xFFFFFF;
    for (i = 0; i < count; i++) {
        uint32_t code = W(desc + 8 + 32 * i), size = W(desc + 12 + 32 * i);
        uint32_t pc = (code - base_code) / 4, end = pc + size / 4;
        if (argc > 2 && (uint32_t)atoi(argv[2]) != i) continue;
        printf("; ---- FX%u: P:%04x-%04x (%u words)\n", i, pc, end - 1, size / 4);
        s_dsp.pc = pc;
        while (s_dsp.pc < end) {
            uint16_t len;
            s_dsp.disasm_prev_inst_pc = 0xFFFFFFFF;
            len = disasm_instruction(&s_dsp, DSP_DISASM_MODE);
            s_dsp.instr_cycle = 0;
            printf("%s\n", disasm_get_instruction_text(&s_dsp));
            s_dsp.pc += len ? len : 1;
        }
    }
    return 0;
}
