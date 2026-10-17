#include "d3d8_internal.h"
#include "d3d8_screenshot.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include "../../third_party/stb_image_write.h"

/* One capture/encoder at a time bounds memory use and repeated key presses.
 * Only the requested frame is read back; PNG compression runs off-thread. */
static volatile LONG s_pending, s_state, s_sequence;
/* Screenshot at another internal resolution (0.0.165): the request sets a
 * one-frame scale override, the target is rebuilt after the next present,
 * the first frame rendered at that size is captured, and the override is
 * cleared so the following frame boundary restores the game's scale. */
static volatile LONG s_resize;
static int s_resize_wait;
extern int  debug_screenshot_scale_get(void);
extern int  video_get_scale(void);
extern int  video_get_aspect(void);
extern void video_set_scale_override(int scale);
extern void video_guest_target_size(int aspect, unsigned *w, unsigned *h);
static HANDLE s_worker;
enum { SHOT_IDLE, SHOT_SAVING, SHOT_SAVED, SHOT_FAILED };
typedef struct { unsigned w, h; unsigned char *pixels; } Screenshot;
typedef struct { HANDLE file; int failed; } Output;

void d3d8_screenshot_request(void)
{
    int scale = debug_screenshot_scale_get();
    if (InterlockedCompareExchange(&s_state, 0, 0) == SHOT_SAVING ||
        InterlockedCompareExchange(&s_resize, 0, 0)) return;
    if (scale && scale != video_get_scale()) {
        s_resize_wait = 0;
        video_set_scale_override(scale);
        InterlockedExchange(&s_resize, 1);
        InterlockedExchange(&s_state, SHOT_SAVING);
    } else {
        InterlockedExchange(&s_pending, 1);
    }
}
const char *d3d8_screenshot_status(void)
{
    switch (InterlockedCompareExchange(&s_state, 0, 0)) {
    case SHOT_SAVING: return "Saving screenshot...";
    case SHOT_SAVED: return "PNG saved in Screenshots next to DOA3.exe.";
    case SHOT_FAILED: return "Screenshot failed. Check available disk space and folder permissions.";
    default: return "Screenshots save as PNGs in Screenshots next to DOA3.exe.";
    }
}
void d3d8_screenshot_shutdown(void)
{
    if (s_worker) {
        WaitForSingleObject(s_worker, 10000);
        CloseHandle(s_worker); s_worker = NULL;
    }
}
static void write_png(void *context, void *data, int size)
{
    Output *out = context;
    DWORD written = 0;
    if (!out->failed && (!WriteFile(out->file, data, (DWORD)size, &written, NULL) || written != (DWORD)size))
        out->failed = 1;
}
static DWORD WINAPI encode_screenshot(void *data)
{
    Screenshot *shot = data;
    wchar_t folder[MAX_PATH], path[MAX_PATH];
    SYSTEMTIME time;
    Output out = {INVALID_HANDLE_VALUE, 0};
    int ok = 0;
    DWORD length = GetModuleFileNameW(NULL, folder, MAX_PATH);
    if (length && length < MAX_PATH) {
        wchar_t *slash = wcsrchr(folder, L'\\');
        if (slash && (size_t)(slash - folder) + 13 < MAX_PATH) {
            wcscpy(slash + 1, L"Screenshots");
            if (CreateDirectoryW(folder, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
                GetLocalTime(&time);
                for (int attempt = 0; attempt < 16; ++attempt) {
                    LONG sequence = InterlockedIncrement(&s_sequence);
                    int n = _snwprintf(path, MAX_PATH,
                        L"%ls\\DOA3_%04u-%02u-%02u_%02u-%02u-%02u-%03u_%ld.png",
                        folder, time.wYear, time.wMonth, time.wDay, time.wHour,
                        time.wMinute, time.wSecond, time.wMilliseconds, sequence);
                    if (n < 0 || n >= MAX_PATH) break;
                    out.file = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (out.file != INVALID_HANDLE_VALUE || GetLastError() != ERROR_FILE_EXISTS) break;
                }
                if (out.file != INVALID_HANDLE_VALUE) {
                    ok = stbi_write_png_to_func(write_png, &out, shot->w, shot->h, 3, shot->pixels, shot->w*3);
                    if (!CloseHandle(out.file)) out.failed = 1;
                    ok = ok && !out.failed;
                    if (!ok) DeleteFileW(path); /* Only the new, incomplete file. */
                }
            }
        }
    }
    free(shot->pixels); free(shot);
    InterlockedExchange(&s_state, ok ? SHOT_SAVED : SHOT_FAILED);
    return 0;
}

void d3d8_screenshot_capture(ID3D11Texture2D *source)
{
    D3D11_TEXTURE2D_DESC td;
    D3D11_MAPPED_SUBRESOURCE map;
    ID3D11Texture2D *staging = NULL;
    ID3D11DeviceContext *ctx = d3d8_GetD3D11Context();
    Screenshot *shot = NULL;
    if (InterlockedCompareExchange(&s_resize, 0, 0) && source) {
        unsigned ew, eh;
        video_guest_target_size(video_get_aspect(), &ew, &eh);
        ID3D11Texture2D_GetDesc(source, &td);
        if (td.Width == ew && td.Height == eh) {
            InterlockedExchange(&s_pending, 1);     /* this frame is at the new size */
        } else if (++s_resize_wait > 30) {          /* never got there: give up */
            video_set_scale_override(0);
            InterlockedExchange(&s_resize, 0);
            InterlockedExchange(&s_state, SHOT_FAILED);
            return;
        } else {
            return;
        }
        video_set_scale_override(0);                /* back to the game's scale next frame */
        InterlockedExchange(&s_resize, 0);
    }
    if (!InterlockedExchange(&s_pending, 0) || !source || !ctx) return;
    if (s_worker) { CloseHandle(s_worker); s_worker = NULL; }
    InterlockedExchange(&s_state, SHOT_SAVING);
    ID3D11Texture2D_GetDesc(source, &td);
    if (td.Format != DXGI_FORMAT_R8G8B8A8_UNORM || td.SampleDesc.Count != 1 ||
        !td.Width || !td.Height || (uint64_t)td.Width * td.Height > 67108864) goto fail;   /* 32:9 at 5x is 20.5 MP */
    shot = calloc(1, sizeof(*shot));
    if (!shot) goto fail;
    shot->w = td.Width; shot->h = td.Height;
    shot->pixels = malloc((size_t)td.Width * td.Height * 3);
    if (!shot->pixels) goto fail;
    td.Usage = D3D11_USAGE_STAGING; td.BindFlags = 0; td.CPUAccessFlags = D3D11_CPU_ACCESS_READ; td.MiscFlags = 0;
    if (FAILED(ID3D11Device_CreateTexture2D(d3d8_GetD3D11Device(), &td, NULL, &staging))) goto fail;
    ID3D11DeviceContext_CopyResource(ctx, (ID3D11Resource *)staging, (ID3D11Resource *)source);
    if (FAILED(ID3D11DeviceContext_Map(ctx, (ID3D11Resource *)staging, 0, D3D11_MAP_READ, 0, &map))) goto fail;
    for (unsigned y = 0; y < td.Height; ++y) {
        const unsigned char *row = (const unsigned char *)map.pData + y * map.RowPitch;
        unsigned char *dest = shot->pixels + (size_t)y * td.Width * 3;
        for (unsigned x = 0; x < td.Width; ++x) memcpy(dest + x*3, row + x*4, 3);
    }
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource *)staging, 0);
    ID3D11Texture2D_Release(staging); staging = NULL;
    s_worker = CreateThread(NULL, 0, encode_screenshot, shot, 0, NULL);
    if (s_worker) return;
fail:
    if (staging) ID3D11Texture2D_Release(staging);
    if (shot) { free(shot->pixels); free(shot); }
    InterlockedExchange(&s_state, SHOT_FAILED);
}
