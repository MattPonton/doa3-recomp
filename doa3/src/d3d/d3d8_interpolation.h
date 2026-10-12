#ifndef DOA3_D3D8_INTERPOLATION_H
#define DOA3_D3D8_INTERPOLATION_H
#include "d3d8_nv2aff.h"

/* Presentation-only snapshots. No guest pointers, callbacks, or memory writes. */
void d3d8_interpolation_draw(const void *vertices, unsigned count, unsigned stride,
    const uint16_t *indices, unsigned index_count, const Nv2aFFConstants *ff);
void d3d8_interpolation_clear(unsigned flags, uint32_t color, float z, unsigned stencil);
void d3d8_interpolation_invalidate(void);
void d3d8_interpolation_finish(int active, unsigned scene);
/* Returns a borrowed SRV; NULL means use the ordinary completed guest frame. */
struct ID3D11ShaderResourceView *d3d8_interpolation_render(float alpha);
void d3d8_interpolation_shutdown(void);
struct ID3D11Resource;
void d3d8_interpolation_texture_changed(struct ID3D11Resource *resource);
int d3d8_interpolation_ready(void);
struct ID3D11Buffer;
void d3d8_interpolation_constants(struct ID3D11Buffer *buffer, const void *data, unsigned size);
void d3d8_interpolation_key(uint64_t key);
struct ID3D11Texture2D;
void d3d8_interpolation_validate(struct ID3D11Texture2D *native);
void d3d8_SchedulePresentations(int steps);
void d3d8_ServicePresentation(void);
#endif
