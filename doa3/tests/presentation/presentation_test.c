#include "d3d8_presentation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
int main(void)
{
    /* No visible window or focus changes. Exercise the actual DXGI path,
     * including fallback on machines where tearing is unavailable. */
    HWND hwnd = CreateWindowExA(0,"STATIC","DOA3 presentation test",WS_OVERLAPPEDWINDOW,
        0,0,320,240,NULL,NULL,GetModuleHandleA(NULL),NULL);
    CHECK(hwnd);
    for (int modern=0; modern<2; ++modern) {
        DXGI_SWAP_CHAIN_DESC desc={0}, actual;
        ID3D11Device *device=NULL;
        ID3D11DeviceContext *context=NULL;
        IDXGISwapChain *swap=NULL;
        IDXGIDevice1 *dxgi=NULL;
        UINT latency=0;
        desc.BufferCount=1; desc.BufferDesc.Width=320; desc.BufferDesc.Height=240;
        desc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.OutputWindow=hwnd; desc.SampleDesc.Count=1;
        desc.Windowed=TRUE; desc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
        CHECK(SUCCEEDED(d3d8_create_presentation(D3D_DRIVER_TYPE_WARP,0,modern,
            &desc,&swap,&device,&context)));
        CHECK(SUCCEEDED(IDXGISwapChain_GetDesc(swap,&actual)));
        CHECK(actual.SwapEffect==desc.SwapEffect && actual.Flags==desc.Flags);
        if (!modern) CHECK(actual.SwapEffect==DXGI_SWAP_EFFECT_DISCARD && !actual.Flags);
        if (actual.SwapEffect==DXGI_SWAP_EFFECT_FLIP_DISCARD)
            CHECK(actual.BufferCount==2 && (actual.Flags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));
        CHECK(SUCCEEDED(ID3D11Device_QueryInterface(device,&IID_IDXGIDevice1,(void **)&dxgi)));
        CHECK(SUCCEEDED(IDXGIDevice1_GetMaximumFrameLatency(dxgi,&latency)) && latency==1);
        IDXGIDevice1_Release(dxgi);
        for (int frame=0; frame<4; ++frame) {
            ID3D11Texture2D *buffer=NULL;
            ID3D11RenderTargetView *rtv=NULL;
            const float color[4]={.1f,.2f,.3f,1};
            CHECK(SUCCEEDED(IDXGISwapChain_GetBuffer(swap,0,&IID_ID3D11Texture2D,(void **)&buffer)));
            CHECK(SUCCEEDED(ID3D11Device_CreateRenderTargetView(device,(ID3D11Resource *)buffer,NULL,&rtv)));
            ID3D11DeviceContext_OMSetRenderTargets(context,1,&rtv,NULL);
            ID3D11DeviceContext_ClearRenderTargetView(context,rtv,color);
            CHECK(SUCCEEDED(d3d8_present_swap_chain(swap,desc.Flags)));
            ID3D11DeviceContext_OMSetRenderTargets(context,0,NULL,NULL);
            ID3D11RenderTargetView_Release(rtv); ID3D11Texture2D_Release(buffer);
            CHECK(SUCCEEDED(IDXGISwapChain_ResizeBuffers(swap,0,320+frame*16,240+frame*16,
                DXGI_FORMAT_UNKNOWN,desc.Flags)));
            CHECK(SUCCEEDED(IDXGISwapChain_GetDesc(swap,&actual)) && actual.Flags==desc.Flags);
        }
        CHECK(SUCCEEDED(ID3D11Device_GetDeviceRemovedReason(device)));
        ID3D11DeviceContext_ClearState(context); ID3D11DeviceContext_Flush(context);
        IDXGISwapChain_Release(swap); ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    }
    DestroyWindow(hwnd);
    puts("Compatibility/flip selection, queue limit, repeated present and resize passed.");
    return 0;
}
