#include "d3d8_internal.h"
#include "d3d8_screenshot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static ID3D11Device *device;
static ID3D11DeviceContext *ctx;
ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return ctx; }
void d3d8_screenshot_capture(ID3D11Texture2D *source);
/* Screenshot resolution (0.0.165): "same as the game", so no override. */
int debug_screenshot_scale_get(void) { return 0; }
int video_get_scale(void) { return 3; }
int video_get_aspect(void) { return 2; }
void video_set_scale_override(int scale) { (void)scale; }
void video_guest_target_size(int aspect, unsigned *w, unsigned *h) { (void)aspect; *w = 2562; *h = 1440; }
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static int count_files(void)
{
    char path[MAX_PATH]; WIN32_FIND_DATAA entry;
    GetModuleFileNameA(NULL, path, MAX_PATH);
    strcpy(strrchr(path, '\\') + 1, "Screenshots\\*.png");
    HANDLE find = FindFirstFileA(path, &entry);
    int count = 0;
    if (find != INVALID_HANDLE_VALUE) {
        do { ++count; } while (FindNextFileA(find, &entry));
        FindClose(find);
    }
    return count;
}
int main(void)
{
    ID3D11Texture2D *texture;
    D3D11_TEXTURE2D_DESC td = {0};
    /* Alpha deliberately zero: screenshots must still save opaque RGB. */
    const unsigned char pixels[] = {255,0,0,0, 0,255,0,0, 0,0,255,0,
                                   255,255,255,0, 0,0,0,0, 255,128,0,0};
    D3D11_SUBRESOURCE_DATA initial = {pixels, 12, 0};
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, &device, NULL, &ctx)));
    td.Width = 3; td.Height = 2; td.ArraySize = td.MipLevels = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; td.SampleDesc.Count = 1;
    CHECK(SUCCEEDED(ID3D11Device_CreateTexture2D(device, &td, &initial, &texture)));
    int before = count_files();
    for (int shot = 0; shot < 2; ++shot) {
        d3d8_screenshot_request(); d3d8_screenshot_capture(texture);
        d3d8_screenshot_shutdown();
        CHECK(strstr(d3d8_screenshot_status(), "PNG saved") != NULL);
    }
    CHECK(count_files() == before + 2);
    d3d8_screenshot_capture(texture); CHECK(count_files() == before + 2); /* no pending request */
    ID3D11Texture2D_Release(texture); ID3D11DeviceContext_Release(ctx); ID3D11Device_Release(device);
    puts("WARP screenshot readback, async PNG completion and unique filenames passed.");
    return 0;
}
