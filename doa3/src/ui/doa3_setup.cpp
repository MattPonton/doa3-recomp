#include "doa3_setup.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include <d3d11.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "../game/xiso_extract.h"
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace {

enum State { ST_PICK, ST_EXTRACTING, ST_DONE };

bool   g_active = false;
State  g_state = ST_PICK;
HWND   g_hwnd;
const WCHAR *g_assets;

WCHAR  g_image[MAX_PATH];
char   g_image_utf8[MAX_PATH * 3];
char   g_error[512];

XisoProgress g_prog;
HANDLE g_worker;
int    g_worker_ok;
char   g_worker_err[512];

ID3D11Device           *g_dev;
ID3D11DeviceContext    *g_ctx;
IDXGISwapChain         *g_swap;
ID3D11RenderTargetView *g_rtv;
UINT g_w, g_h;

void SetImage(const WCHAR *path)
{
    wcscpy_s(g_image, MAX_PATH, path);
    WideCharToMultiByte(CP_UTF8, 0, g_image, -1, g_image_utf8, sizeof g_image_utf8, NULL, NULL);
    g_error[0] = 0;
}

void Browse()
{
    WCHAR file[MAX_PATH] = { 0 };
    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof ofn);
    ofn.lStructSize = sizeof ofn;
    ofn.hwndOwner = g_hwnd;
    /* *.iso also matches name.xiso.iso. */
    ofn.lpstrFilter = L"Xbox disc image (*.xiso, *.iso)\0*.xiso;*.iso\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Select your Dead or Alive 3 disc image";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    if (GetOpenFileNameW(&ofn)) SetImage(file);
}

DWORD WINAPI Worker(LPVOID)
{
    g_worker_ok = xiso_install(g_image, g_assets, &g_prog, g_worker_err, sizeof g_worker_err);
    return 0;
}

void StartInstall()
{
    memset(&g_prog, 0, sizeof g_prog);
    g_worker_err[0] = 0;
    g_error[0] = 0;
    g_worker_ok = 0;
    g_worker = CreateThread(NULL, 0, Worker, NULL, 0, NULL);
    if (g_worker) g_state = ST_EXTRACTING;
    else _snprintf_s(g_error, sizeof g_error, _TRUNCATE, "Could not start the extraction.");
}

void PollWorker()
{
    if (g_state != ST_EXTRACTING || WaitForSingleObject(g_worker, 0) != WAIT_OBJECT_0) return;
    CloseHandle(g_worker);
    g_worker = NULL;
    if (g_worker_ok) {
        g_state = ST_DONE;
    } else {
        g_state = ST_PICK;
        _snprintf_s(g_error, sizeof g_error, _TRUNCATE, "%s", g_worker_err);
    }
}

void StopWorker()
{
    if (!g_worker) return;
    InterlockedExchange(&g_prog.cancel, 1);
    WaitForSingleObject(g_worker, INFINITE);
    CloseHandle(g_worker);
    g_worker = NULL;
}

/* ── D3D11 ─────────────────────────────────────────────────────────────── */

void CreateRTV()
{
    ID3D11Texture2D *bb = nullptr;
    if (SUCCEEDED(g_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&bb))) {
        g_dev->CreateRenderTargetView(bb, nullptr, &g_rtv);
        bb->Release();
    }
}

bool CreateDevice()
{
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    g_w = (UINT)(rc.right - rc.left);
    g_h = (UINT)(rc.bottom - rc.top);

    DXGI_SWAP_CHAIN_DESC sd;
    memset(&sd, 0, sizeof sd);
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    /* Same blit model as the game's own swap chain, which is created on this
     * window as soon as this one is released. */
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                             nullptr, 0, D3D11_SDK_VERSION, &sd, &g_swap,
                                             &g_dev, nullptr, &g_ctx)))
        return false;
    CreateRTV();
    return g_rtv != nullptr;
}

void DestroyDevice()
{
    if (g_ctx) { g_ctx->ClearState(); g_ctx->Flush(); }
    if (g_rtv)  { g_rtv->Release();  g_rtv = nullptr; }
    if (g_swap) { g_swap->Release(); g_swap = nullptr; }
    if (g_ctx)  { g_ctx->Release();  g_ctx = nullptr; }
    if (g_dev)  { g_dev->Release();  g_dev = nullptr; }
}

void ResizeIfNeeded()
{
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    UINT w = (UINT)(rc.right - rc.left), h = (UINT)(rc.bottom - rc.top);
    if (!w || !h || (w == g_w && h == g_h)) return;
    if (g_rtv) { g_rtv->Release(); g_rtv = nullptr; }
    g_swap->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
    g_w = w; g_h = h;
    CreateRTV();
}

/* ── Drawing ──────────────────────────────────────────────────────────── */

const ImVec4 kRed(1.0f, 0.45f, 0.35f, 1.0f);

void Draw(bool *quit)
{
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + vp->WorkSize.x * 0.5f,
                                   vp->WorkPos.y + vp->WorkSize.y * 0.5f),
                            ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(560, 0), ImGuiCond_Always);

    ImGui::Begin("Dead or Alive 3 - Game files", nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    if (g_state == ST_PICK) {
        ImGui::TextWrapped("The Dead or Alive 3 game files are not installed yet.");
        ImGui::Spacing();
        ImGui::TextWrapped("Select your Dead or Alive 3 (USA) Xbox disc image in .xiso "
                           "or .iso format and then click Add. "
                           "This only happens once and needs about 4 GB of free space.");
        ImGui::Spacing();
        ImGui::TextDisabled("You can also drag the image onto this window.");
        ImGui::Spacing();

        ImGui::SetNextItemWidth(-90.0f);
        ImGui::InputTextWithHint("##image", "No disc image selected", g_image_utf8,
                                 sizeof g_image_utf8, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Browse...", ImVec2(-1, 0))) Browse();

        if (g_error[0]) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, kRed);
            ImGui::TextWrapped("%s", g_error);
            ImGui::PopStyleColor();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        bool have = g_image[0] != 0;
        if (!have) ImGui::BeginDisabled();
        if (ImGui::Button("Add", ImVec2(120, 0))) StartInstall();
        if (!have) ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Quit", ImVec2(120, 0))) *quit = true;
    } else {
        LONG64 done = g_prog.bytes_done, total = g_prog.bytes_total;
        float frac = total > 0 ? (float)((double)done / (double)total) : 0.0f;
        char overlay[64];
        _snprintf_s(overlay, sizeof overlay, _TRUNCATE, "%.2f / %.2f GB",
                    done / 1073741824.0, total / 1073741824.0);

        ImGui::TextWrapped(total ? "Copying the game files into the assets folder..."
                                 : "Checking the disc image...");
        ImGui::Spacing();
        ImGui::ProgressBar(frac, ImVec2(-1, 0), total ? overlay : "");
        if (total) {
            LONG n = g_prog.files_done + 1, of = g_prog.files_total;
            ImGui::TextDisabled("File %ld of %ld: %s", n < of ? n : of, of, g_prog.current_name);
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) InterlockedExchange(&g_prog.cancel, 1);
    }

    ImGui::End();
}

} /* namespace */

extern "C" int doa3_setup_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (!g_active) return 0;
    if (msg == WM_DROPFILES) {
        WCHAR f[MAX_PATH];
        HDROP drop = (HDROP)wp;
        if (g_state == ST_PICK && DragQueryFileW(drop, 0, f, MAX_PATH)) SetImage(f);
        DragFinish(drop);
        return 1;
    }
    if (ImGui::GetCurrentContext())
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wp, lp);
    /* Keys stop here so the Esc menu cannot open underneath. Everything else,
     * including Alt+F4 (WM_SYSKEYDOWN) and the frame, still reaches
     * DefWindowProc. */
    return msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_CHAR;
}

extern "C" int doa3_setup_run(HWND hwnd, const WCHAR *assets_dir)
{
    int result = 0;
    bool quit = false;

    g_hwnd = hwnd;
    g_assets = assets_dir;
    g_state = ST_PICK;
    if (!hwnd || !CreateDevice()) {
        DestroyDevice();
        MessageBoxW(hwnd, L"Could not start Direct3D 11 for the setup screen.",
                    L"Dead or Alive 3", MB_ICONERROR);
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_dev, g_ctx);
    DragAcceptFiles(hwnd, TRUE);
    g_active = true;

    while (!quit && g_state != ST_DONE) {
        MSG m;
        while (PeekMessageW(&m, NULL, 0, 0, PM_REMOVE)) {
            if (m.message == WM_QUIT) { quit = true; break; }
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
        if (quit) break;
        PollWorker();
        ResizeIfNeeded();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        Draw(&quit);
        ImGui::Render();

        const float black[4] = { 0, 0, 0, 1 };
        g_ctx->OMSetRenderTargets(1, &g_rtv, nullptr);
        g_ctx->ClearRenderTargetView(g_rtv, black);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_swap->Present(1, 0);
    }

    StopWorker();
    result = g_state == ST_DONE;

    /* Leave the window black for the moment before the game's first present. */
    if (g_rtv) {
        const float black[4] = { 0, 0, 0, 1 };
        g_ctx->ClearRenderTargetView(g_rtv, black);
        g_swap->Present(0, 0);
    }

    g_active = false;
    DragAcceptFiles(hwnd, FALSE);
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    DestroyDevice();
    return result;
}
