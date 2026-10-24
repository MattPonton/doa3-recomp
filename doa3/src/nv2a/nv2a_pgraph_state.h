/* Pointer-free translator state. This is NOT a complete GPU checkpoint:
 * texture caches, render/depth contents, D3D bindings and presentation are
 * deliberately separate. No live restore entry point is exposed here. */
#ifndef NV2A_PGRAPH_STATE_H
#define NV2A_PGRAPH_STATE_H
#include <stdint.h>
#include "nv2a_vertex_program.h"
#define MAX_INLINE_VERTS 16384
#define MAX_IDX 65536
#define INLINE_VERT_DWORDS 5
typedef struct {
    /* Draw state */
    int in_draw;           /* Between BEGIN and END */
    uint32_t draw_mode;    /* NV2A draw mode (0=end, 6=tristrip, etc.) */
    int d3d_prim_type;     /* Translated D3D prim type */

    /* Inline vertex accumulator */
    uint32_t inline_data[MAX_INLINE_VERTS * INLINE_VERT_DWORDS];
    uint32_t inline_count; /* Number of dwords accumulated */
    uint32_t vert_stride;  /* Dwords per vertex (auto-detected) */
    /* DOA3: inline vertex LAYOUT supplied by the recompiled D3D draw wrapper
     * (sub_001B3760 in recomp_manual.c). The XDK never emits
     * SET_VERTEX_DATA_ARRAY_FORMAT on this path, so the pgraph can't derive
     * it from methods; the CPU-side FVF object holds the truth. Offsets are
     * DWORD indices within a vertex; -1 = attribute absent. */
    /* DOA3: the vertex count the guest passed to DrawVerticesUP, plus the
     * dword count its shader object declares. The push buffer carries the
     * attributes the DEVICE is set up to copy, which for the D3DX sprite path
     * is neither the API stride nor what the shader object lists (position +
     * diffuse + up to four texcoord sets). inline_count / this count is the
     * only exact per-vertex stride available. 0 = no hint for this draw. */
    uint32_t hint_verts;
    uint32_t hint_declared_dw;
    int layout_pos_dw;     /* dwords of position (2=XY, 3=XYZ, 4=XYZRHW) */
    int layout_uv_off;     /* dword offset of texcoord0, or -1 */
    int layout_color_off;  /* dword offset of diffuse (D3DCOLOR), or -1 */

    /* Clear state */
    uint32_t clear_color;
    uint32_t clear_rect_h;  /* (width << 16) | x */
    uint32_t clear_rect_v;  /* (height << 16) | y */

    /* Render state cache */
    int depth_test;
    int depth_mask;        /* NV097_SET_DEPTH_MASK: depth writes enabled */
    uint32_t depth_func;   /* NV097_SET_DEPTH_FUNC, GL constant 0x200..0x207 */
    int blend_enable;
    uint32_t blend_sfactor;
    uint32_t blend_dfactor;
    int cull_enable;
    uint32_t cull_face;      /* 0x404 FRONT, 0x405 BACK, 0x408 BOTH */
    uint32_t front_face;     /* 0x900 CW, 0x901 CCW */
    int stencil_enable;
    uint32_t stencil_mask, stencil_func, stencil_ref, stencil_func_mask;
    uint32_t stencil_fail, stencil_zfail, stencil_zpass;
    int alpha_test;
    uint32_t alpha_func;   /* NV097_SET_ALPHA_FUNC, GL constant 0x200..0x207 */
    uint32_t alpha_ref;    /* NV097_SET_ALPHA_REF, 0..255 */
    uint32_t color_mask;
    uint32_t line_width;      /* NV097_SET_LINE_WIDTH, 6.3 fixed point surface pixels */
    uint32_t shader_prog;     /* NV097_SET_SHADER_STAGE_PROGRAM (0x1E70): 5 bits per stage, 1 = 2D projective */
    uint32_t fog_enable, fog_mode, fog_gen, fog_color;   /* NV097_SET_FOG_* (0x2A4/0x29C/0x2A0/0x2A8) */
    float    fog_param[3];    /* NV097_SET_FOG_PARAMS (0x9C0): bias, scale, - */
    float    fog_plane[4];    /* NV097_SET_FOG_PLANE (0x9D0) */
    uint32_t comb_factor0[8];
    /* from doa3_pb_tss_marker: 0xA3<<24 | stage1-off<<17 | op<<12 | arg1<<6 | arg2 */
    uint32_t gtss;
    int      gtss_valid;
    uint32_t comb_control;  /* NV097_SET_COMBINER_CONTROL: bits 7:0 = stage count */
    /* Point sprites (the snow stage's falling snow). SET_POINT_PARAMS_ENABLE
     * 0x0318, SET_POINT_SMOOTH_ENABLE 0x031C (the sprite-coordinate switch:
     * the NV2A feeds a point's 0..1 coordinates to texture unit 3 only),
     * SET_POINT_SIZE 0x043C in 1/8 pixel units, SET_POINT_PARAMS 0x0A30. */
    uint32_t point_params_en, point_smooth, point_size;
    float    point_params[8];
    /* Stage-3 texture ops from the guest's TSS table (doa3_pb_tss_marker
     * 0xA4 = colour, 0xA5 = alpha: op<<12 | arg1<<6 | arg2). */
    uint32_t tss3_color, tss3_alpha;
    int      tss3_valid;
    /* Stage-1 ops (markers 0xA6 colour / 0xA7 alpha, same packing) and the
     * guest's D3DRS_TEXTUREFACTOR (markers 0xA9 low 24 bits / 0xAA high 8).
     * The push buffer's SET_COMBINER_FACTOR0 is NOT this value: the XDK's
     * combiner compiler never completes in this port, so factor0 reads
     * FFFFFFFF on every draw while the game's factor is e.g. 0x23FFFFFF. */
    uint32_t tss1_color, tss1_alpha;
    uint32_t tss0_alpha;               /* marker 0xAB: guest stage-0 ALPHAOP/args */
    int      tss0_alpha_valid;
    int      tss1_valid;
    uint32_t tfactor_lo, tfactor_hi;

    /* Viewport */
    float vp_offset[4];
    float vp_scale[4];
    /* NV097_SET_WINDOW_CLIP_* (0x02B4/0x02C0/0x02E0): the hardware scissor.
     * DOA3 programs it ~18.8k times per 2 s and it is what confines the
     * character-select portrait to its window. */
    uint32_t window_clip_type;
    uint32_t window_clip_h[8];
    uint32_t window_clip_v[8];
    uint32_t surface_clip_h;
    uint32_t surface_clip_v;
    /* DOA3: depth range the fixed-function output is normalised with.
     * SET_CLIP_MIN/MAX (0x0394/0x0398) are what the hardware clips z
     * against; the zeta format (SET_SURFACE_FORMAT bits 7:4) gives the
     * z-buffer range: Z16 = 65535, Z24S8 = 16777215. */
    float    clip_min, clip_max;
    uint32_t surface_fmt;

    /* Texture state per stage (4 stages) */
    struct {
        uint32_t offset;     /* NV2A VRAM offset (method 0x1B00) */
        uint32_t format;     /* Format register (method 0x1B04) */
        uint32_t address;    /* Address modes: U bits 0-3, V bits 8-11 (0x1B08) */
        uint32_t control0;   /* Control0 register (method 0x1B0C) */
        uint32_t control1;   /* Control1: linear pitch in hi16 (method 0x1B10) */
        uint32_t filter;     /* MIN bits 16-23, MAG bits 24-27 (method 0x1B14) */
        uint32_t image_rect; /* Linear size: (width<<16)|height (method 0x1B1C) */
        /* Palette for the indexed formats (method 0x1B20): bit 0 selects the
         * context DMA, bits 3:2 the entry count (0=256, 1=128, 2=64, 3=32)
         * and bits 31:6 the 64-byte-aligned offset of an A8R8G8B8 table. */
        uint32_t palette;
        int enabled;         /* Decoded from control0 bit 30 */
    } tex[4];

    /* Chyron scroll */
    float chyron_scroll_offset;  /* Pixels to shift X for chyron text */

    /* Transform program state.
     *
     * DOA3 draws its 3D screens with MODE_PROGRAM, so attribute 0 is an
     * object-space position that only becomes screen space after the uploaded
     * program runs. `attr_fmt` is the authoritative inline vertex layout from
     * SET_VERTEX_DATA_ARRAY_FORMAT, which this title does emit (the previous
     * title did not, hence the wrapper-supplied fallback layout above). */
    Nv2aVertexProgram vp;
    uint32_t xform_mode;        /* NV097_SET_TRANSFORM_EXECUTION_MODE */
    uint32_t attr_fmt[16];      /* SET_VERTEX_DATA_ARRAY_FORMAT per slot */
    int      attr_fmt_seen;

    /* Vertex-array (indexed / DRAW_ARRAYS) submission.
     *
     * The title screen does not use INLINE_ARRAY at all: it binds vertex
     * buffers and issues ~378 D3DDevice_DrawIndexedVertices per frame with
     * fixed-function T&L (confirmed against a cxbx trace of the real game).
     * Those arrive as SET_VERTEX_DATA_ARRAY_OFFSET + ARRAY_ELEMENT/DRAW_ARRAYS,
     * every one of which this translator previously ignored -- so the whole
     * screen was silently dropped. */
    uint32_t attr_off[16];      /* SET_VERTEX_DATA_ARRAY_OFFSET per slot */
    uint32_t idx[MAX_IDX];
    uint32_t idx_count;
    uint32_t idx_dropped;       /* indices past MAX_IDX in the current batch */
    float    composite[16];     /* NV097_SET_COMPOSITE_MATRIX */
    int      composite_seen;
    uint32_t lighting;          /* NV097_SET_LIGHTING_ENABLE 0x0314 */

    /* Fixed-function vertex lighting state (NV2A registers written by the
     * Xbox D3D runtime; light colours already carry the material factors). */
    struct {
        float amb[3], dif[3], spec[3];   /* 0x1000 / 0x100C / 0x1018 (+0x80*i) */
        float range;                     /* 0x1024 */
        float half[3], dir[3];           /* 0x1028 / 0x1034 (eye space) */
        float spot_fall[3], spot_dir[4]; /* 0x1040 / 0x104C */
        float pos[3], att[3];            /* 0x105C / 0x1068 (eye space) */
    } light[4];
    uint32_t light_mask;        /* 0x03BC: 2 bits per light, 0 off 1 inf 2 local 3 spot */
    uint32_t light_control;     /* 0x0294 */
    uint32_t color_material;    /* 0x0298 */
    uint32_t spec_enable;       /* 0x03B8 */
    uint32_t normalize_en;      /* 0x03A4 */
    float    scene_ambient[3];  /* 0x0A10 */
    float    emission[3];       /* 0x03A8 */
    float    material_alpha;    /* 0x03B4 */
    float    spec_params[6];    /* 0x09E0 */

} Nv2aPgraphDrawState;

/* Former file/function statics that influence subsequent draws. Their process
 * lifetime defaults are intentionally preserved separately from pgraph init. */
typedef struct {
    uint32_t blend_color, blend_src_valid, blend_dst_valid;
    int draw_has_uv, draw_inline, cpu_layout_seen;
    uint32_t surface_offset, surface_pitch;
    uint32_t framebuffer_offsets[4];
    int framebuffer_count;
    uint32_t registers[0x800];
} Nv2aPgraphAuxState;

/* In-process private replay image; not a disk/wire format. External users of
 * g_mv still share that array, so copy it explicitly with the translator state.
 * Caller must own/quiesce the source and use non-overlapping buffers. This
 * low-level copy has no owner, epoch or resource-generation validation.
 * Import is for PRIVATE state only: native
 * resources must be reconciled before a live renderer adapter can use this. */
typedef struct {
    uint32_t version, size;
    Nv2aPgraphDrawState draw;
    Nv2aPgraphAuxState aux;
    float model_view[16];
} Nv2aPgraphLogicalImage;
int nv2a_pgraph_logical_capture(Nv2aPgraphLogicalImage *image,
    const Nv2aPgraphDrawState *draw, const Nv2aPgraphAuxState *aux,
    const float model_view[16]);
int nv2a_pgraph_logical_validate(const Nv2aPgraphLogicalImage *image);
int nv2a_pgraph_logical_restore_private(const Nv2aPgraphLogicalImage *image,
    Nv2aPgraphDrawState *draw, Nv2aPgraphAuxState *aux, float model_view[16]);
#endif
