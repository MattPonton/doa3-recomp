/*
 * NV2A vertex program (transform program) capture and execution.
 *
 * The pgraph translator was originally written against a title that only ever
 * used the fixed-function pipeline with pre-transformed, screen-space inline
 * vertices, so it read attribute 0 as literal pixel coordinates. DOA3 draws its
 * 3D screens in MODE_PROGRAM: attribute 0 is an object-space position that only
 * becomes screen space after the uploaded vertex program runs. Without this the
 * geometry is submitted with meaningless coordinates and nothing is visible.
 *
 * This implements the NV2A "VP1.1" transform program: capture through the
 * pushbuffer methods, and a CPU interpreter that produces the output registers
 * for one vertex. Running it on the CPU (rather than translating to HLSL) keeps
 * the existing D3D8 fixed-function draw path intact -- we simply hand it the
 * post-transform vertices it already knows how to draw.
 */
#ifndef NV2A_VERTEX_PROGRAM_H
#define NV2A_VERTEX_PROGRAM_H

#include <stdint.h>

#define NV2A_VP_MAX_INSTR   136   /* transform program slots */
#define NV2A_VP_NUM_CONST   192   /* c[0..191]; D3D c0 is uploaded at index 96 */
#define NV2A_VP_NUM_INPUT   16    /* v0..v15 */
#define NV2A_VP_NUM_OUTPUT  13    /* see the o[] map below */

/* Output register indices, matching the Xbox vertex shader o[] numbering.
 * (These are the same indices the FVF attribute slots use: 0 position,
 * 3 diffuse, 9 texcoord0 -- which is why the D3D wrapper keys on 3 and 9.) */
enum {
    NV2A_VP_OUT_POS  = 0,
    NV2A_VP_OUT_D0   = 3,   /* diffuse  */
    NV2A_VP_OUT_D1   = 4,   /* specular */
    NV2A_VP_OUT_FOG  = 5,
    NV2A_VP_OUT_PTS  = 6,
    NV2A_VP_OUT_B0   = 7,
    NV2A_VP_OUT_B1   = 8,
    NV2A_VP_OUT_T0   = 9,
    NV2A_VP_OUT_T1   = 10,
    NV2A_VP_OUT_T2   = 11,
    NV2A_VP_OUT_T3   = 12
};

/* One instruction with every field pulled out once. The interpreter used to
 * re-extract ~35 bit fields per instruction per vertex; the reflective floors
 * run a ~30-instruction program over ~9,000 vertices a frame on the CPU. */
typedef struct {
    uint8_t mac, ilu, v_idx, a0x;
    uint8_t mux[3], reg[3], neg[3];
    uint8_t swz[3][4];
    uint8_t mac_mask, out_r, ilu_mask, o_mask, orb, oaddr, omux, final;
    uint8_t need[3];             /* operand A/B/C is fetched */
    uint8_t wrote_o0, wrote_r12; /* R12 <-> oPos mirror */
    int16_t c_base;
} Nv2aVpOp;

typedef struct {
    uint32_t instr[NV2A_VP_MAX_INSTR][4];
    float    consts[NV2A_VP_NUM_CONST][4];

    uint32_t prog_ld_ptr;    /* NV097_SET_TRANSFORM_PROGRAM_LOAD  */
    uint32_t const_ld_ptr;   /* NV097_SET_TRANSFORM_CONSTANT_LOAD */
    uint32_t start;          /* NV097_SET_TRANSFORM_PROGRAM_START */
    uint32_t instr_count;    /* highest instruction slot written + 1 */
    int      have_program;   /* at least one instruction uploaded */

    /* Decoded copy of the program from `start`, rebuilt when an instruction
     * is written or the entry point moves. */
    uint32_t gen;            /* bumped by every program write */
    uint32_t dec_gen, dec_start, dec_n;
    int      dec_valid;
    uint32_t dec_inputs;     /* bit v: some fetched operand reads v[v] */
    Nv2aVpOp dec[NV2A_VP_MAX_INSTR];
} Nv2aVertexProgram;

void nv2a_vp_reset(Nv2aVertexProgram *vp);

/* `slot` is (method - NV097_SET_TRANSFORM_PROGRAM) / 4 for program writes and
 * (method - NV097_SET_TRANSFORM_CONSTANT) / 4 for constant writes; the load
 * pointer advances every fourth dword, exactly as the hardware does. */
void nv2a_vp_write_program(Nv2aVertexProgram *vp, uint32_t slot, uint32_t param);
void nv2a_vp_write_constant(Nv2aVertexProgram *vp, uint32_t slot, uint32_t param);

/* Execute the program for one vertex.
 * `in` holds v0..v15, `out` receives the output registers.
 * Returns 0 when there is no usable program. */
int nv2a_vp_execute(Nv2aVertexProgram *vp,
                    const float in[NV2A_VP_NUM_INPUT][4],
                    float out[NV2A_VP_NUM_OUTPUT][4]);

/* Execute the program for n vertices (in[k] -> out[k]); the same results as n
 * calls of nv2a_vp_execute (up to the sign of NaNs), much cheaper per vertex. */
int nv2a_vp_execute_batch(Nv2aVertexProgram *vp, uint32_t n,
                          const float (*in)[NV2A_VP_NUM_INPUT][4],
                          float (*out)[NV2A_VP_NUM_OUTPUT][4]);

/* Input registers the current program reads (bit v = v[v]); inputs it never
 * reads need not be fetched. */
uint32_t nv2a_vp_input_mask(Nv2aVertexProgram *vp);

/* One-shot disassembly of the loaded program, for diagnostics. */
void nv2a_vp_dump(const Nv2aVertexProgram *vp);

#endif /* NV2A_VERTEX_PROGRAM_H */
