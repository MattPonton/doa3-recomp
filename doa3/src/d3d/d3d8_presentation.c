#include "d3d8_presentation.h"
#include "../perf_capture.h"
#include <stdio.h>

static BOOL tearing_supported(void)
{
    IDXGIFactory1 *factory = NULL;
    IDXGIFactory5 *modern = NULL;
    BOOL supported = FALSE;
    if (SUCCEEDED(CreateDXGIFactory1(&IID_IDXGIFactory1, (void **)&factory))) {
        if (SUCCEEDED(IDXGIFactory1_QueryInterface(factory, &IID_IDXGIFactory5, (void **)&modern))) {
            if (FAILED(IDXGIFactory5_CheckFeatureSupport(modern, DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                &supported, sizeof(supported)))) supported = FALSE;
            IDXGIFactory5_Release(modern);
        }
        IDXGIFactory1_Release(factory);
    }
    return supported;
}

HRESULT d3d8_create_presentation(D3D_DRIVER_TYPE driver, UINT device_flags,
    BOOL prefer_flip, DXGI_SWAP_CHAIN_DESC *desc, IDXGISwapChain **swap,
    ID3D11Device **device, ID3D11DeviceContext **context)
{
    DXGI_SWAP_CHAIN_DESC legacy = *desc;
    BOOL flip = prefer_flip && desc->Windowed && tearing_supported();
    HRESULT hr;
    if (flip) {
        desc->SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc->BufferCount = 2;
        desc->Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }
    *swap = NULL; *device = NULL; *context = NULL;
    hr = D3D11CreateDeviceAndSwapChain(NULL, driver, NULL, device_flags,
        NULL, 0, D3D11_SDK_VERSION, desc, swap, device, NULL, context);
    if (FAILED(hr) && flip) {
        fprintf(stderr, "[D3D8] Flip presentation unavailable (0x%08lX), using compatibility path\n", (unsigned long)hr);
        if (*context) { ID3D11DeviceContext_Release(*context); *context = NULL; }
        if (*device) { ID3D11Device_Release(*device); *device = NULL; }
        if (*swap) { IDXGISwapChain_Release(*swap); *swap = NULL; }
        *desc = legacy;
        hr = D3D11CreateDeviceAndSwapChain(NULL, driver, NULL, device_flags,
            NULL, 0, D3D11_SDK_VERSION, desc, swap, device, NULL, context);
    }
    if (FAILED(hr)) return hr;

    /* The game implements borderless/windowed modes itself. Prevent DXGI's
     * default Alt+Enter from unexpectedly entering exclusive fullscreen. */
    {
        IDXGIFactory *factory = NULL;
        if (SUCCEEDED(IDXGISwapChain_GetParent(*swap, &IID_IDXGIFactory, (void **)&factory))) {
            IDXGIFactory_MakeWindowAssociation(factory, desc->OutputWindow, DXGI_MWA_NO_ALT_ENTER);
            IDXGIFactory_Release(factory);
        }
    }
    /* No waitable-object flag: both presentation models use the device-level
     * limit. Keep the existing post-present 60Hz guest pacing. */
    {
        IDXGIDevice1 *dxgi = NULL;
        UINT latency = 0;
        HRESULT limit = ID3D11Device_QueryInterface(*device, &IID_IDXGIDevice1, (void **)&dxgi);
        if (SUCCEEDED(limit)) {
            limit = IDXGIDevice1_SetMaximumFrameLatency(dxgi, 1);
            if (SUCCEEDED(limit)) limit = IDXGIDevice1_GetMaximumFrameLatency(dxgi, &latency);
            IDXGIDevice1_Release(dxgi);
        }
        if (SUCCEEDED(limit)) fprintf(stderr, "[D3D8] Maximum queued frames: %u\n", latency);
        else fprintf(stderr, "[D3D8] Frame queue limit unavailable: 0x%08lX\n", (unsigned long)limit);
    }
    fprintf(stderr, "[D3D8] Presentation: %s\n",
        desc->SwapEffect == DXGI_SWAP_EFFECT_FLIP_DISCARD ? "flip discard, tearing allowed" : "compatibility blit");
    return hr;
}

HRESULT d3d8_present_swap_chain(IDXGISwapChain *swap, UINT swap_flags)
{
    UINT flags = 0;
    if (swap_flags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) {
        BOOL fullscreen = FALSE;
        /* ALLOW_TEARING is invalid in exclusive fullscreen, even if something
         * outside our window-mode UI put the chain there. */
        if (SUCCEEDED(IDXGISwapChain_GetFullscreenState(swap, &fullscreen, NULL)) && !fullscreen)
            flags = DXGI_PRESENT_ALLOW_TEARING;
    }
    LONGLONG present_begin = doa3_perf_begin();
    HRESULT hr = IDXGISwapChain_Present(swap, 0, flags);
    doa3_perf_end("DXGI Present", present_begin);
    return hr;
}
