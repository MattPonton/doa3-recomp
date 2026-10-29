/*
 * fxrun: runs the GP DSP program of a decrypted XDK dsstdfx.bin on xemu's
 * DSP56300 interpreter, one 32-sample frame at a time, feeding a signal into
 * one mixbin and recording others. Used to measure the effects' responses.
 *
 * xemu's DSP core (hw/xbox/mcpx/apu/dsp: dsp.c, dsp_c.c, dsp_dma.c,
 * interp/dsp_cpu.c; espes, Matt Borgerson, from Hatari / ARAnyM) is compiled
 * in unchanged: GPL-2.0-or-later, as is this file.
 *
 * usage: fxrun image.bin state.bin|- in_bin frames end_pc out.raw bin...
 *   state.bin: words written over the start of the state area (FX0's block
 *              comes first, so a room from `py -3 -m tools.dsstdfx rooms`)
 *   in_bin:    mixbin given a unit impulse (0.5) at sample 0 of frame 0
 *   end_pc:    P address the frame stops at (0x4bb = after FX0)
 *   out.raw:   float32, one column per recorded bin, frames*32 rows
 */
#include "qemu/osdep.h"
#include "dsp.h"
#include "interp/dsp_cpu.h"

const DSPOps jit_dsp_ops;
void dsp_jit_init(DSPState *dsp) { (void)dsp; abort(); }

static uint8_t *s_scratch;
static size_t   s_scratch_size;

static void scratch_rw(void *o, uint8_t *ptr, uint32_t addr, size_t len, bool dir)
{
    (void)o;
    if (addr + len > s_scratch_size) { fprintf(stderr, "scratch %x+%zx out of range\n", addr, len); exit(2); }
    if (dir) memcpy(s_scratch + addr, ptr, len);
    else     memcpy(ptr, s_scratch + addr, len);
}

static void fifo_rw(void *o, uint8_t *ptr, unsigned index, size_t len, bool dir)
{
    (void)o; (void)index; (void)dir;
    memset(ptr, 0, len);
}

static float frac24(uint32_t v)
{
    int32_t s = (int32_t)(v << 8) >> 8;
    return (float)s / 8388608.0f;
}

int main(int argc, char **argv)
{
    static uint8_t img[1 << 20];
    FILE *f;
    uint32_t desc, i, frame, frames, in_bin, end_pc, nout, outs[32];
    DSPState *dsp;
    dsp_core_t *core;
    FILE *out;

    if (argc < 8) { fprintf(stderr, "usage: fxrun image state|- in_bin frames end_pc out.raw bin...\n"); return 1; }
    f = fopen(argv[1], "rb");
    if (!f) return 1;
    fread(img, 1, sizeof img, f);
    fclose(f);
#define W(o) (*(uint32_t *)(img + (o)))
    if (strcmp(argv[2], "-")) {
        f = fopen(argv[2], "rb");
        if (!f) return 1;
        fread(img + 0x818 + W(0x804) * 4, 4, W(0x80C), f);
        fclose(f);
    }
    in_bin = strtoul(argv[3], NULL, 0);
    frames = strtoul(argv[4], NULL, 0);
    end_pc = strtoul(argv[5], NULL, 0);
    nout = (uint32_t)argc - 7;
    for (i = 0; i < nout && i < 32; i++) outs[i] = strtoul(argv[7 + i], NULL, 0);
    desc = 0x818 + (W(0x804) + W(0x80C)) * 4;
    s_scratch_size = W(desc + 4) + 0x100000;
    s_scratch = calloc(1, s_scratch_size);

    dsp = dsp_init(NULL, scratch_rw, fifo_rw, true);
    core = (dsp_core_t *)dsp->backend;
    memset(core->xram, 0, sizeof core->xram);
    memset(core->yram, 0, sizeof core->yram);
    memset(core->pram, 0, sizeof core->pram);
    for (i = 0; i < W(0x804); i++) dsp_write_memory(dsp, 'P', i, W(0x818 + 4 * i) & 0xFFFFFF);
    for (i = 0; i < W(0x80C); i++) dsp_write_memory(dsp, 'X', 0x80 + i, W(0x818 + W(0x804) * 4 + 4 * i) & 0xFFFFFF);
    dsp_invalidate_opcache(dsp);

    out = fopen(argv[6], "wb");
    for (frame = 0; frame < frames; frame++) {
        long steps = 0;
        for (i = 0; i < 0x400; i++) core->mixbuffer[i] = 0;
        if (frame == 0) core->mixbuffer[in_bin * 32] = 0x400000;
        core->pc = 0;
        dsp_start_frame(dsp);
        while (core->pc != end_pc) {
            dsp_step(dsp);
            if (++steps > 2000000) { fprintf(stderr, "frame %u: stuck at p:%04x\n", frame, core->pc); return 3; }
        }
        for (i = 0; i < 32; i++) {
            float row[32];
            uint32_t k;
            for (k = 0; k < nout; k++) row[k] = 2.0f * frac24(core->mixbuffer[outs[k] * 32 + i]);
            fwrite(row, sizeof(float), nout, out);
        }
        if (frame == 0) fprintf(stderr, "frame 0: %ld instructions\n", steps);
    }
    fclose(out);
    return 0;
}
