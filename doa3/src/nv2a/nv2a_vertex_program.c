/*
 * NV2A vertex program (VP1.1) capture + CPU interpreter.
 *
 * Instruction encoding: 4 dwords per instruction. Each instruction may issue
 * one MAC (multiply/accumulate) op and one ILU (inverse logic unit) op, each
 * with its own write mask, plus at most one write to an output register. The
 * three source operands A, B and C each carry a register index, a source
 * select (temp / input / constant), a 4x2-bit swizzle and a negate bit.
 *
 * Field positions below follow the hardware layout; dword 0 carries no fields
 * we need. The operand-to-op mapping is not uniform and is easy to get wrong:
 * MUL reads A and B, ADD reads A and C, MAD reads A, B and C, and every ILU op
 * reads C.
 */

#include "nv2a_vertex_program.h"

#include <string.h>
#include <math.h>
#include <stdio.h>

#ifdef _MSC_VER
#define VP_INLINE __forceinline
#else
#define VP_INLINE inline __attribute__((always_inline))
#endif

/* ── MAC opcodes ── */
enum {
    MAC_NOP = 0, MAC_MOV, MAC_MUL, MAC_ADD, MAC_MAD, MAC_DP3, MAC_DPH,
    MAC_DP4, MAC_DST, MAC_MIN, MAC_MAX, MAC_SLT, MAC_SGE, MAC_ARL
};

/* ── ILU opcodes ── */
enum {
    ILU_NOP = 0, ILU_MOV, ILU_RCP, ILU_RCC, ILU_RSQ, ILU_EXP, ILU_LOG, ILU_LIT
};

/* ── operand source select ── */
enum { PARAM_UNKNOWN = 0, PARAM_R, PARAM_V, PARAM_C };

/* ── output destination select ── */
enum { OUTPUT_C = 0, OUTPUT_O };
enum { OMUX_MAC = 0, OMUX_ILU };

static uint32_t fld(const uint32_t t[4], int w, int b, int n)
{
    return (t[w] >> b) & ((n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u));
}

static float u2fv(uint32_t u)
{
    float f;
    memcpy(&f, &u, 4);
    return f;
}

void nv2a_vp_reset(Nv2aVertexProgram *vp)
{
    memset(vp, 0, sizeof(*vp));
}

void nv2a_vp_write_program(Nv2aVertexProgram *vp, uint32_t slot, uint32_t param)
{
    uint32_t idx = vp->prog_ld_ptr;
    if (idx < NV2A_VP_MAX_INSTR) {
        vp->instr[idx][slot % 4] = param;
        vp->gen++;
        if (idx + 1 > vp->instr_count)
            vp->instr_count = idx + 1;
        vp->have_program = 1;
    }
    if ((slot % 4) == 3)
        vp->prog_ld_ptr = idx + 1;
}

void nv2a_vp_write_constant(Nv2aVertexProgram *vp, uint32_t slot, uint32_t param)
{
    uint32_t idx = vp->const_ld_ptr;
    if (idx < NV2A_VP_NUM_CONST)
        vp->consts[idx][slot % 4] = u2fv(param);
    if ((slot % 4) == 3)
        vp->const_ld_ptr = idx + 1;
}

static void write_masked(float dst[4], const float src[4], int mask)
{
    /* Mask bits are x,y,z,w from the high bit down. */
    if (mask & 8) dst[0] = src[0];
    if (mask & 4) dst[1] = src[1];
    if (mask & 2) dst[2] = src[2];
    if (mask & 1) dst[3] = src[3];
}

static void splat(float dst[4], float v)
{
    dst[0] = dst[1] = dst[2] = dst[3] = v;
}

static float safe_rcp(float v)
{
    if (v == 0.0f) return 1.0e30f;
    return 1.0f / v;
}

/* Pull every field of the program out once, from the entry point to the
 * FINAL instruction, exactly as the per-vertex loop used to walk it. */
static void vp_decode(Nv2aVertexProgram *vp)
{
    uint32_t pc = vp->start, n = 0;
    uint32_t inputs = 0;

    while (pc < vp->instr_count && pc < NV2A_VP_MAX_INSTR && n < NV2A_VP_MAX_INSTR) {
        const uint32_t *t = vp->instr[pc];
        Nv2aVpOp *o = &vp->dec[n++];
        int k;

        o->ilu    = (uint8_t)fld(t, 1, 25, 3);
        o->mac    = (uint8_t)fld(t, 1, 21, 4);
        o->c_base = (int16_t)fld(t, 1, 13, 8);
        o->v_idx  = (uint8_t)fld(t, 1,  9, 4);

        o->neg[0] = (uint8_t)fld(t, 1, 8, 1);
        o->swz[0][0] = (uint8_t)fld(t, 1, 6, 2); o->swz[0][1] = (uint8_t)fld(t, 1, 4, 2);
        o->swz[0][2] = (uint8_t)fld(t, 1, 2, 2); o->swz[0][3] = (uint8_t)fld(t, 1, 0, 2);
        o->reg[0] = (uint8_t)fld(t, 2, 28, 4); o->mux[0] = (uint8_t)fld(t, 2, 26, 2);

        o->neg[1] = (uint8_t)fld(t, 2, 25, 1);
        o->swz[1][0] = (uint8_t)fld(t, 2, 23, 2); o->swz[1][1] = (uint8_t)fld(t, 2, 21, 2);
        o->swz[1][2] = (uint8_t)fld(t, 2, 19, 2); o->swz[1][3] = (uint8_t)fld(t, 2, 17, 2);
        o->reg[1] = (uint8_t)fld(t, 2, 13, 4); o->mux[1] = (uint8_t)fld(t, 2, 11, 2);

        o->neg[2] = (uint8_t)fld(t, 2, 10, 1);
        o->swz[2][0] = (uint8_t)fld(t, 2, 8, 2); o->swz[2][1] = (uint8_t)fld(t, 2, 6, 2);
        o->swz[2][2] = (uint8_t)fld(t, 2, 4, 2); o->swz[2][3] = (uint8_t)fld(t, 2, 2, 2);
        o->reg[2] = (uint8_t)((fld(t, 2, 0, 2) << 2) | fld(t, 3, 30, 2));
        o->mux[2] = (uint8_t)fld(t, 3, 28, 2);

        o->mac_mask = (uint8_t)fld(t, 3, 24, 4);
        o->out_r    = (uint8_t)fld(t, 3, 20, 4);
        o->ilu_mask = (uint8_t)fld(t, 3, 16, 4);
        o->o_mask   = (uint8_t)fld(t, 3, 12, 4);
        o->orb      = (uint8_t)fld(t, 3, 11, 1);
        o->oaddr    = (uint8_t)fld(t, 3,  3, 8);
        o->omux     = (uint8_t)fld(t, 3,  2, 1);
        o->a0x      = (uint8_t)fld(t, 3,  1, 1);
        o->final    = (uint8_t)fld(t, 3,  0, 1);

        /* Fetch only operands the paired operations consume. */
        o->need[0] = o->mac >= MAC_MOV && o->mac <= MAC_ARL;
        o->need[1] = o->mac == MAC_MUL || (o->mac >= MAC_MAD && o->mac <= MAC_SGE);
        o->need[2] = o->mac == MAC_ADD || o->mac == MAC_MAD || o->ilu != ILU_NOP;
        for (k = 0; k < 3; k++)
            if (o->need[k] && o->mux[k] == PARAM_V) inputs |= 1u << (o->v_idx & 15);

        o->wrote_o0  = (o->o_mask && o->orb == OUTPUT_O && o->oaddr == 0);
        o->wrote_r12 = (o->mac != MAC_NOP && o->mac != MAC_ARL && o->mac_mask && (o->out_r & 15) == 12) ||
                       (o->ilu != ILU_NOP && o->ilu_mask &&
                        ((o->mac != MAC_NOP) ? 1 : (o->out_r & 15)) == 12);
        if (o->final)
            break;
        pc++;
    }
    vp->dec_n = n;
    vp->dec_inputs = inputs;
    vp->dec_gen = vp->gen;
    vp->dec_start = vp->start;
    vp->dec_valid = 1;
}

static void vp_ensure_decoded(Nv2aVertexProgram *vp)
{
    if (!vp->dec_valid || vp->dec_gen != vp->gen || vp->dec_start != vp->start)
        vp_decode(vp);
}

uint32_t nv2a_vp_input_mask(Nv2aVertexProgram *vp)
{
    if (!vp->have_program || vp->instr_count == 0)
        return 0;
    vp_ensure_decoded(vp);
    return vp->dec_inputs;
}

static VP_INLINE void fetch_op(float dst[4], const Nv2aVpOp *o, int k, int c_idx,
                            float R[16][4],
                            const float in[NV2A_VP_NUM_INPUT][4],
                            const float consts[NV2A_VP_NUM_CONST][4])
{
    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    const float *src;
    const uint8_t *sw = o->swz[k];
    float x, y, z, w;

    switch (o->mux[k]) {
    case PARAM_R: src = R[o->reg[k] & 15]; break;
    case PARAM_V: src = in[o->v_idx & 15]; break;
    case PARAM_C:
        src = (c_idx >= 0 && c_idx < NV2A_VP_NUM_CONST) ? consts[c_idx] : zero;
        break;
    default: src = zero; break;
    }
    x = src[sw[0]]; y = src[sw[1]]; z = src[sw[2]]; w = src[sw[3]];
    if (o->neg[k]) { x = -x; y = -y; z = -z; w = -w; }
    dst[0] = x; dst[1] = y; dst[2] = z; dst[3] = w;
}

/* One instruction for one vertex. Shared by the single-vertex and batched
 * entry points, so both produce exactly the same bits. */
static VP_INLINE void vp_step(const Nv2aVpOp *o, const float consts[NV2A_VP_NUM_CONST][4],
                           float R[16][4], const float in[NV2A_VP_NUM_INPUT][4],
                           float out[NV2A_VP_NUM_OUTPUT][4], int *a0p)
{
    int a0 = *a0p;
    int mac = o->mac, ilu = o->ilu;
    int c_idx = o->c_base + (o->a0x ? a0 : 0);
    float A[4], B[4], C[4], res[4];

    /* Keep every fetch before either operation writes a temporary:
     * paired ILU may read the same register that MAC is about to
     * overwrite. */
    if (o->need[0]) fetch_op(A, o, 0, c_idx, R, in, consts);
    if (o->need[1]) fetch_op(B, o, 1, c_idx, R, in, consts);
    if (o->need[2]) fetch_op(C, o, 2, c_idx, R, in, consts);

    if (mac != MAC_NOP) {
        switch (mac) {
        case MAC_MOV:
            res[0] = A[0]; res[1] = A[1]; res[2] = A[2]; res[3] = A[3];
            break;
        case MAC_MUL:
            res[0] = A[0]*B[0]; res[1] = A[1]*B[1];
            res[2] = A[2]*B[2]; res[3] = A[3]*B[3];
            break;
        case MAC_ADD:   /* A + C, not A + B */
            res[0] = A[0]+C[0]; res[1] = A[1]+C[1];
            res[2] = A[2]+C[2]; res[3] = A[3]+C[3];
            break;
        case MAC_MAD:
            res[0] = A[0]*B[0]+C[0]; res[1] = A[1]*B[1]+C[1];
            res[2] = A[2]*B[2]+C[2]; res[3] = A[3]*B[3]+C[3];
            break;
        case MAC_DP3:
            splat(res, A[0]*B[0] + A[1]*B[1] + A[2]*B[2]);
            break;
        case MAC_DPH:
            splat(res, A[0]*B[0] + A[1]*B[1] + A[2]*B[2] + B[3]);
            break;
        case MAC_DP4:
            splat(res, A[0]*B[0] + A[1]*B[1] + A[2]*B[2] + A[3]*B[3]);
            break;
        case MAC_DST:
            res[0] = 1.0f; res[1] = A[1]*B[1]; res[2] = A[2]; res[3] = B[3];
            break;
        case MAC_MIN:
            res[0] = A[0]<B[0]?A[0]:B[0]; res[1] = A[1]<B[1]?A[1]:B[1];
            res[2] = A[2]<B[2]?A[2]:B[2]; res[3] = A[3]<B[3]?A[3]:B[3];
            break;
        case MAC_MAX:
            res[0] = A[0]>B[0]?A[0]:B[0]; res[1] = A[1]>B[1]?A[1]:B[1];
            res[2] = A[2]>B[2]?A[2]:B[2]; res[3] = A[3]>B[3]?A[3]:B[3];
            break;
        case MAC_SLT:
            res[0] = A[0]<B[0]?1.0f:0.0f; res[1] = A[1]<B[1]?1.0f:0.0f;
            res[2] = A[2]<B[2]?1.0f:0.0f; res[3] = A[3]<B[3]?1.0f:0.0f;
            break;
        case MAC_SGE:
            res[0] = A[0]>=B[0]?1.0f:0.0f; res[1] = A[1]>=B[1]?1.0f:0.0f;
            res[2] = A[2]>=B[2]?1.0f:0.0f; res[3] = A[3]>=B[3]?1.0f:0.0f;
            break;
        case MAC_ARL:
            a0 = (int)floorf(A[0]);
            res[0] = res[1] = res[2] = res[3] = 0.0f;
            break;
        default:
            res[0] = res[1] = res[2] = res[3] = 0.0f;
            break;
        }

        if (mac != MAC_ARL) {
            if (o->omux == OMUX_MAC && o->o_mask && o->orb == OUTPUT_O &&
                o->oaddr < NV2A_VP_NUM_OUTPUT)
                write_masked(out[o->oaddr], res, o->o_mask);
            if (o->mac_mask)
                write_masked(R[o->out_r & 15], res, o->mac_mask);
        }
    }

    if (ilu != ILU_NOP) {
        float s = C[0];
        switch (ilu) {
        case ILU_MOV:
            res[0] = C[0]; res[1] = C[1]; res[2] = C[2]; res[3] = C[3];
            break;
        case ILU_RCP: splat(res, safe_rcp(s)); break;
        case ILU_RCC: {
            float r = safe_rcp(s);
            /* Clamped reciprocal: keep the magnitude inside the hardware's
             * representable window, preserving sign. */
            if (r >= 0.0f) { if (r < 5.42101086e-20f) r = 5.42101086e-20f;
                             if (r > 1.884467e+19f)   r = 1.884467e+19f; }
            else           { if (r > -5.42101086e-20f) r = -5.42101086e-20f;
                             if (r < -1.884467e+19f)   r = -1.884467e+19f; }
            splat(res, r);
            break;
        }
        case ILU_RSQ: {
            float a = fabsf(s);
            splat(res, a == 0.0f ? 1.0e30f : 1.0f / sqrtf(a));
            break;
        }
        case ILU_EXP: splat(res, powf(2.0f, s)); break;
        case ILU_LOG: splat(res, s == 0.0f ? -1.0e30f : log2f(fabsf(s))); break;
        case ILU_LIT: {
            /* C = (diffuse, specular, -, power) */
            float d = C[0], sp = C[1], p = C[3];
            if (p < -127.9961f) p = -127.9961f;
            if (p >  127.9961f) p =  127.9961f;
            res[0] = 1.0f;
            res[1] = d > 0.0f ? d : 0.0f;
            res[2] = (d > 0.0f && sp > 0.0f) ? powf(sp, p) : 0.0f;
            res[3] = 1.0f;
            break;
        }
        default:
            res[0] = res[1] = res[2] = res[3] = 0.0f;
            break;
        }

        if (o->omux == OMUX_ILU && o->o_mask && o->orb == OUTPUT_O &&
            o->oaddr < NV2A_VP_NUM_OUTPUT)
            write_masked(out[o->oaddr], res, o->o_mask);
        if (o->ilu_mask) {
            /* When the instruction also issues a MAC op, the MAC owns the
             * named temp and the ILU result is forced to R1. */
            int dst = (mac != MAC_NOP) ? 1 : (o->out_r & 15);
            write_masked(R[dst], res, o->ilu_mask);
        }
    }

    /* R12 is a mirror of oPos (xemu: "#define R12 oPos"). The XDK's
     * viewport epilogue reads the clip position back through R12 after the
     * game's own code wrote o[0]; without the mirror it saw zeros and the
     * pond's water surface collapsed onto one point. */
    if (o->wrote_o0) memcpy(R[12], out[NV2A_VP_OUT_POS], sizeof(float) * 4);
    else if (o->wrote_r12) memcpy(out[NV2A_VP_OUT_POS], R[12], sizeof(float) * 4);
    *a0p = a0;
}

static void vp_init_regs(float R[16][4], float out[NV2A_VP_NUM_OUTPUT][4])
{
    memset(R, 0, sizeof(float) * 16 * 4);
    memset(out, 0, sizeof(float) * NV2A_VP_NUM_OUTPUT * 4);
    /* Colours default to opaque white so a program that only writes position
     * still produces visible geometry rather than fully transparent black. */
    out[NV2A_VP_OUT_D0][0] = out[NV2A_VP_OUT_D0][1] =
    out[NV2A_VP_OUT_D0][2] = out[NV2A_VP_OUT_D0][3] = 1.0f;
    out[NV2A_VP_OUT_POS][3] = 1.0f;
}

int nv2a_vp_execute(Nv2aVertexProgram *vp,
                    const float in[NV2A_VP_NUM_INPUT][4],
                    float out[NV2A_VP_NUM_OUTPUT][4])
{
    float R[16][4];
    int a0 = 0;
    uint32_t i;

    if (!vp->have_program || vp->instr_count == 0)
        return 0;
    vp_ensure_decoded(vp);
    vp_init_regs(R, out);
    for (i = 0; i < vp->dec_n; i++)
        vp_step(&vp->dec[i], vp->consts, R, in, out, &a0);
    return 1;
}

/* Many vertices at once, instruction by instruction: the decode and every
 * branch on the instruction's fields (operation, operand source, swizzle,
 * write masks) are taken once per instruction per chunk instead of once per
 * instruction per vertex, and the per-vertex inner loops are straight-line.
 * Each vertex sees the same operations in the same order on the same values
 * as nv2a_vp_execute, so every finite result is bit-identical; the only
 * difference measured is the sign bit of a NaN that two NaN operands produce
 * (the compiler may swap the operands of a commutative op), and a NaN
 * position drops the batch and a NaN texcoord is undefined either way. */
#define VP_CHUNK 64

static void vp_fetch_batch(float (*dst)[4], const Nv2aVpOp *o, int k, uint32_t m,
                           float (*R)[16][4], const float (*in)[NV2A_VP_NUM_INPUT][4],
                           const float consts[NV2A_VP_NUM_CONST][4], const int *a0)
{
    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    const int s0 = o->swz[k][0], s1 = o->swz[k][1], s2 = o->swz[k][2], s3 = o->swz[k][3];
    uint32_t v;
#define VP_GATHER(SRC) do { for (v = 0; v < m; v++) { const float *src_ = (SRC); \
        float x = src_[s0], y = src_[s1], z = src_[s2], w = src_[s3]; \
        if (o->neg[k]) { x = -x; y = -y; z = -z; w = -w; } \
        dst[v][0] = x; dst[v][1] = y; dst[v][2] = z; dst[v][3] = w; } } while (0)
    switch (o->mux[k]) {
    case PARAM_R: { int r = o->reg[k] & 15; VP_GATHER(R[v][r]); break; }
    case PARAM_V: { int r = o->v_idx & 15;  VP_GATHER(in[v][r]); break; }
    case PARAM_C:
        if (o->a0x) {
            for (v = 0; v < m; v++) {
                int c = o->c_base + a0[v];
                const float *src_ = (c >= 0 && c < NV2A_VP_NUM_CONST) ? consts[c] : zero;
                float x = src_[s0], y = src_[s1], z = src_[s2], w = src_[s3];
                if (o->neg[k]) { x = -x; y = -y; z = -z; w = -w; }
                dst[v][0] = x; dst[v][1] = y; dst[v][2] = z; dst[v][3] = w;
            }
        } else {
            int c = o->c_base;
            const float *src_ = (c >= 0 && c < NV2A_VP_NUM_CONST) ? consts[c] : zero;
            float x = src_[s0], y = src_[s1], z = src_[s2], w = src_[s3];
            if (o->neg[k]) { x = -x; y = -y; z = -z; w = -w; }
            for (v = 0; v < m; v++) { dst[v][0] = x; dst[v][1] = y; dst[v][2] = z; dst[v][3] = w; }
        }
        break;
    default: VP_GATHER(zero); break;
    }
#undef VP_GATHER
}

/* dst[v] (a row of an array of `stride` floats per vertex) <- res[v] under mask. */
static void vp_write_batch(float *dst, size_t stride, const float (*res)[4], int mask, uint32_t m)
{
    uint32_t v;
    if (mask == 0xF) {
        for (v = 0; v < m; v++, dst += stride) {
            dst[0] = res[v][0]; dst[1] = res[v][1]; dst[2] = res[v][2]; dst[3] = res[v][3];
        }
        return;
    }
    for (v = 0; v < m; v++, dst += stride) {
        if (mask & 8) dst[0] = res[v][0];
        if (mask & 4) dst[1] = res[v][1];
        if (mask & 2) dst[2] = res[v][2];
        if (mask & 1) dst[3] = res[v][3];
    }
}

int nv2a_vp_execute_batch(Nv2aVertexProgram *vp, uint32_t n,
                          const float (*in)[NV2A_VP_NUM_INPUT][4],
                          float (*out)[NV2A_VP_NUM_OUTPUT][4])
{
    static float R[VP_CHUNK][16][4];
    static float A[VP_CHUNK][4], B[VP_CHUNK][4], C[VP_CHUNK][4], res[VP_CHUNK][4];
    int a0[VP_CHUNK];
    uint32_t base, i, v, m;

    if (!vp->have_program || vp->instr_count == 0)
        return 0;
    vp_ensure_decoded(vp);
    for (base = 0; base < n; base += VP_CHUNK) {
        float (*O)[NV2A_VP_NUM_OUTPUT][4] = out + base;
        const float (*I)[NV2A_VP_NUM_INPUT][4] = in + base;
        m = n - base < VP_CHUNK ? n - base : VP_CHUNK;
        for (v = 0; v < m; v++) { vp_init_regs(R[v], O[v]); a0[v] = 0; }

        for (i = 0; i < vp->dec_n; i++) {
            const Nv2aVpOp *o = &vp->dec[i];
            int mac = o->mac, ilu = o->ilu;
            int o_ok = o->o_mask && o->orb == OUTPUT_O && o->oaddr < NV2A_VP_NUM_OUTPUT;

            if (o->need[0]) vp_fetch_batch(A, o, 0, m, R, I, vp->consts, a0);
            if (o->need[1]) vp_fetch_batch(B, o, 1, m, R, I, vp->consts, a0);
            if (o->need[2]) vp_fetch_batch(C, o, 2, m, R, I, vp->consts, a0);

            if (mac != MAC_NOP) {
#define VP_LOOP(...) for (v = 0; v < m; v++) { __VA_ARGS__; } break
                switch (mac) {
                case MAC_MOV: VP_LOOP(res[v][0] = A[v][0]; res[v][1] = A[v][1]; res[v][2] = A[v][2]; res[v][3] = A[v][3]);
                case MAC_MUL: VP_LOOP(res[v][0] = A[v][0]*B[v][0]; res[v][1] = A[v][1]*B[v][1];
                                      res[v][2] = A[v][2]*B[v][2]; res[v][3] = A[v][3]*B[v][3]);
                case MAC_ADD: VP_LOOP(res[v][0] = A[v][0]+C[v][0]; res[v][1] = A[v][1]+C[v][1];
                                      res[v][2] = A[v][2]+C[v][2]; res[v][3] = A[v][3]+C[v][3]);
                case MAC_MAD: VP_LOOP(res[v][0] = A[v][0]*B[v][0]+C[v][0]; res[v][1] = A[v][1]*B[v][1]+C[v][1];
                                      res[v][2] = A[v][2]*B[v][2]+C[v][2]; res[v][3] = A[v][3]*B[v][3]+C[v][3]);
                case MAC_DP3: VP_LOOP(splat(res[v], A[v][0]*B[v][0] + A[v][1]*B[v][1] + A[v][2]*B[v][2]));
                case MAC_DPH: VP_LOOP(splat(res[v], A[v][0]*B[v][0] + A[v][1]*B[v][1] + A[v][2]*B[v][2] + B[v][3]));
                case MAC_DP4: VP_LOOP(splat(res[v], A[v][0]*B[v][0] + A[v][1]*B[v][1] + A[v][2]*B[v][2] + A[v][3]*B[v][3]));
                case MAC_DST: VP_LOOP(res[v][0] = 1.0f; res[v][1] = A[v][1]*B[v][1]; res[v][2] = A[v][2]; res[v][3] = B[v][3]);
                case MAC_MIN: VP_LOOP(int k_; for (k_ = 0; k_ < 4; k_++) res[v][k_] = A[v][k_] < B[v][k_] ? A[v][k_] : B[v][k_]);
                case MAC_MAX: VP_LOOP(int k_; for (k_ = 0; k_ < 4; k_++) res[v][k_] = A[v][k_] > B[v][k_] ? A[v][k_] : B[v][k_]);
                case MAC_SLT: VP_LOOP(int k_; for (k_ = 0; k_ < 4; k_++) res[v][k_] = A[v][k_] < B[v][k_] ? 1.0f : 0.0f);
                case MAC_SGE: VP_LOOP(int k_; for (k_ = 0; k_ < 4; k_++) res[v][k_] = A[v][k_] >= B[v][k_] ? 1.0f : 0.0f);
                case MAC_ARL: VP_LOOP(a0[v] = (int)floorf(A[v][0]));
                default:      VP_LOOP(res[v][0] = res[v][1] = res[v][2] = res[v][3] = 0.0f);
                }
                if (mac != MAC_ARL) {
                    if (o->omux == OMUX_MAC && o_ok)
                        vp_write_batch(O[0][o->oaddr], NV2A_VP_NUM_OUTPUT * 4, res, o->o_mask, m);
                    if (o->mac_mask)
                        vp_write_batch(R[0][o->out_r & 15], 16 * 4, res, o->mac_mask, m);
                }
            }

            if (ilu != ILU_NOP) {
                switch (ilu) {
                case ILU_MOV: VP_LOOP(res[v][0] = C[v][0]; res[v][1] = C[v][1]; res[v][2] = C[v][2]; res[v][3] = C[v][3]);
                case ILU_RCP: VP_LOOP(splat(res[v], safe_rcp(C[v][0])));
                case ILU_RCC: VP_LOOP(
                    float r = safe_rcp(C[v][0]);
                    if (r >= 0.0f) { if (r < 5.42101086e-20f) r = 5.42101086e-20f;
                                     if (r > 1.884467e+19f)   r = 1.884467e+19f; }
                    else           { if (r > -5.42101086e-20f) r = -5.42101086e-20f;
                                     if (r < -1.884467e+19f)   r = -1.884467e+19f; }
                    splat(res[v], r));
                case ILU_RSQ: VP_LOOP(float a_ = fabsf(C[v][0]); splat(res[v], a_ == 0.0f ? 1.0e30f : 1.0f / sqrtf(a_)));
                case ILU_EXP: VP_LOOP(splat(res[v], powf(2.0f, C[v][0])));
                case ILU_LOG: VP_LOOP(float s_ = C[v][0]; splat(res[v], s_ == 0.0f ? -1.0e30f : log2f(fabsf(s_))));
                case ILU_LIT: VP_LOOP(
                    float d = C[v][0], sp = C[v][1], p = C[v][3];
                    if (p < -127.9961f) p = -127.9961f;
                    if (p >  127.9961f) p =  127.9961f;
                    res[v][0] = 1.0f;
                    res[v][1] = d > 0.0f ? d : 0.0f;
                    res[v][2] = (d > 0.0f && sp > 0.0f) ? powf(sp, p) : 0.0f;
                    res[v][3] = 1.0f);
                default:      VP_LOOP(res[v][0] = res[v][1] = res[v][2] = res[v][3] = 0.0f);
                }
#undef VP_LOOP
                if (o->omux == OMUX_ILU && o_ok)
                    vp_write_batch(O[0][o->oaddr], NV2A_VP_NUM_OUTPUT * 4, res, o->o_mask, m);
                if (o->ilu_mask) {
                    int dst = (mac != MAC_NOP) ? 1 : (o->out_r & 15);
                    vp_write_batch(R[0][dst], 16 * 4, res, o->ilu_mask, m);
                }
            }

            /* R12 mirrors oPos (see nv2a_vp_execute). */
            if (o->wrote_o0)
                for (v = 0; v < m; v++) memcpy(R[v][12], O[v][NV2A_VP_OUT_POS], sizeof(float) * 4);
            else if (o->wrote_r12)
                for (v = 0; v < m; v++) memcpy(O[v][NV2A_VP_OUT_POS], R[v][12], sizeof(float) * 4);
        }
    }
    return 1;
}

void nv2a_vp_dump(const Nv2aVertexProgram *vp)
{
    static const char *mac_name[] = {
        "nop","mov","mul","add","mad","dp3","dph","dp4",
        "dst","min","max","slt","sge","arl","?","?"
    };
    static const char *ilu_name[] = {
        "nop","mov","rcp","rcc","rsq","exp","log","lit"
    };
    uint32_t i;

    fprintf(stderr, "[VP] start=%u count=%u\n", vp->start, vp->instr_count);
    for (i = vp->start; i < vp->instr_count && i < NV2A_VP_MAX_INSTR; i++) {
        const uint32_t *t = vp->instr[i];
        int mac = (int)fld(t, 1, 21, 4);
        int ilu = (int)fld(t, 1, 25, 3);
        fprintf(stderr, "  [%3u] %-3s %-3s  cst=%-3u v=%-2u out=%s%u omask=%X "
                        "macmask=%X R%u %s\n",
                i, mac_name[mac & 15], ilu_name[ilu & 7],
                (unsigned)fld(t, 1, 13, 8), (unsigned)fld(t, 1, 9, 4),
                fld(t, 3, 11, 1) ? "o" : "c", (unsigned)fld(t, 3, 3, 8),
                (unsigned)fld(t, 3, 12, 4), (unsigned)fld(t, 3, 24, 4),
                (unsigned)fld(t, 3, 20, 4),
                fld(t, 3, 0, 1) ? "FINAL" : "");
    }
    fflush(stderr);
}
