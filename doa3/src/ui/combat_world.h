#pragma once
#include <d3d11.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Returns a borrowed composite SRV, or NULL when no hazards are drawn.
 * Scene color/depth and all modified pipeline bindings are preserved. */
ID3D11ShaderResourceView *combat_world_render(ID3D11Device *device,
    ID3D11DeviceContext *context, ID3D11Texture2D *scene,
    ID3D11DepthStencilView *depth);
void combat_world_shutdown(void);
const char *combat_world_error(void);
#ifdef __cplusplus
}
#endif
