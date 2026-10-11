#ifndef DOA3_D3D8_PRESENTATION_H
#define DOA3_D3D8_PRESENTATION_H
#ifndef COBJMACROS
#define COBJMACROS
#endif
#include <d3d11.h>
#include <dxgi1_5.h>
#ifdef __cplusplus
extern "C" {
#endif
/* On success desc contains the actual (possibly fallback) configuration. */
HRESULT d3d8_create_presentation(D3D_DRIVER_TYPE driver, UINT device_flags,
    BOOL prefer_flip, DXGI_SWAP_CHAIN_DESC *desc, IDXGISwapChain **swap,
    ID3D11Device **device, ID3D11DeviceContext **context);
HRESULT d3d8_present_swap_chain(IDXGISwapChain *swap, UINT swap_flags);
#ifdef __cplusplus
}
#endif
#endif
