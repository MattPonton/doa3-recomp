/* Optional CPU-side draw preparation benchmark. This does not measure game FPS
 * or GPU rendering. Run only while the game is closed to avoid interference. */
#include "d3d8_internal.h"
#include "d3d8_debug_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ID3D11Device *device;
static ID3D11DeviceContext *ctx;
static DWORD states[256], tss[32];
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return ctx; }
const DWORD *d3d8_GetRenderStates(void) { return states; }
const DWORD *d3d8_GetTSS(DWORD stage) { (void)stage; return tss; }
int d3d8_OffscreenTargetActive(void) { return 0; }
unsigned g_d3d8_bind_epoch = 1;
void d3d8_BindInvalidate(void) { g_d3d8_bind_epoch++; }

static int compare(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}
static void measure(const char *label, int alternate, int mode)
{
    LARGE_INTEGER start, end, frequency;
    double samples[7];
    QueryPerformanceFrequency(&frequency);
    d3d8_debug_view_set(mode);
    for (int round = -1; round < 7; ++round) {
        QueryPerformanceCounter(&start);
        for (int i = 0; i < 10000; ++i) {
            int value = alternate ? (i & 1) : 0;
            states[D3DRS_ALPHABLENDENABLE] = value;
            states[D3DRS_ZWRITEENABLE] = !value;
            states[D3DRS_CULLMODE] = value ? D3DCULL_CW : D3DCULL_CCW;
            d3d8_states_apply();
        }
        ID3D11DeviceContext_Flush(ctx);
        QueryPerformanceCounter(&end);
        if (round >= 0) samples[round] = 1000.0*(end.QuadPart-start.QuadPart)/frequency.QuadPart;
    }
    qsort(samples, 7, sizeof(double), compare);
    printf("%s: median %.3f ms / 10000 preparations (%.3f us each), range %.3f..%.3f ms\n",
        label, samples[3], samples[3]/10, samples[0], samples[6]);
}
int main(void)
{
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, &device, NULL, &ctx);
    if (FAILED(hr)) { fprintf(stderr, "Hardware device unavailable: %08lX\n", hr); return 1; }
    states[D3DRS_ZENABLE] = 1; states[D3DRS_ZWRITEENABLE] = 1;
    states[D3DRS_ZFUNC] = D3DCMP_LESSEQUAL;
    states[D3DRS_FILLMODE] = D3DFILL_SOLID; states[D3DRS_CULLMODE] = D3DCULL_CCW;
    states[D3DRS_SRCBLEND] = D3DBLEND_SRCALPHA;
    states[D3DRS_DESTBLEND] = D3DBLEND_INVSRCALPHA;
    states[D3DRS_BLENDOP] = 1; states[D3DRS_COLORWRITEENABLE] = 15;
    states[D3DRS_STENCILFUNC] = D3DCMP_ALWAYS;
    states[D3DRS_STENCILMASK] = states[D3DRS_STENCILWRITEMASK] = 255;
    states[D3DRS_STENCILFAIL] = states[D3DRS_STENCILZFAIL] = states[D3DRS_STENCILPASS] = 1;
    tss[D3DTSS_MINFILTER] = tss[D3DTSS_MAGFILTER] = tss[D3DTSS_MIPFILTER] = D3DTEXF_LINEAR;
    tss[D3DTSS_ADDRESSU] = tss[D3DTSS_ADDRESSV] = D3DTADDRESS_WRAP;
    d3d8_states_init();
    measure("Normal, unchanged states", 0, D3D8_DEBUG_NORMAL);
    measure("Normal, alternating states", 1, D3D8_DEBUG_NORMAL);
    measure("Vertex colors, unchanged states", 0, D3D8_DEBUG_VERTEX_COLOR);
    measure("Wireframe, unchanged states", 0, D3D8_DEBUG_WIREFRAME);
    d3d8_states_shutdown(); d3d8_states_release_samplers();
    ID3D11DeviceContext_ClearState(ctx); ID3D11DeviceContext_Flush(ctx);
    ID3D11DeviceContext_Release(ctx); ID3D11Device_Release(device);
    return 0;
}
