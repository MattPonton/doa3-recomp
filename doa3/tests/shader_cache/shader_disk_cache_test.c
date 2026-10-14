#define COBJMACROS
#include "d3d8_shader_cache.h"
#include <d3d11.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); return 1; } } while (0)
static const char source[] =
    "float4 main() : SV_Target { return float4(.2,.4,.6,1); }\n"
    "float4 alternate() : SV_Target { return float4(1,0,0,1); }\n"
    "float4 vertex(float4 p : POSITION) : SV_Position { return p; }\n";

static HRESULT compile(const char *text, const char *entry, const char *profile,
                        UINT flags, ID3DBlob **blob, BOOL *hit)
{
    ID3DBlob *errors = NULL;
    HRESULT hr = d3d8_compile_shader_cached(text, strlen(text), "cache_test", entry,
        profile, flags, blob, &errors, hit);
    if (errors) ID3D10Blob_Release(errors);
    return hr;
}

int main(void)
{
    wchar_t root[MAX_PATH], pattern[MAX_PATH], file[MAX_PATH], missing[MAX_PATH];
    WIN32_FIND_DATAW found;
    HANDLE search, f;
    DWORD written;
    ID3DBlob *cold = NULL, *warm = NULL, *other = NULL;
    ID3D11Device *device = NULL;
    ID3D11PixelShader *ps = NULL;
    ID3D11VertexShader *vs = NULL;
    BOOL hit;
    UINT flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
    LARGE_INTEGER begin, end, frequency;
    double cold_ms, warm_ms;
    swprintf_s(root, MAX_PATH, L"shader-cache-test-%lu-%llu", GetCurrentProcessId(), GetTickCount64());
    CHECK(CreateDirectoryW(root, NULL));
    CHECK(SetEnvironmentVariableW(L"DOA3_SHADER_CACHE_DIR", root));
    QueryPerformanceFrequency(&frequency); QueryPerformanceCounter(&begin);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&cold,&hit)) && !hit);
    QueryPerformanceCounter(&end); cold_ms=1000.0*(end.QuadPart-begin.QuadPart)/frequency.QuadPart;
    QueryPerformanceCounter(&begin);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&warm,&hit)) && hit);
    QueryPerformanceCounter(&end); warm_ms=1000.0*(end.QuadPart-begin.QuadPart)/frequency.QuadPart;
    CHECK(ID3D10Blob_GetBufferSize(cold)==ID3D10Blob_GetBufferSize(warm));
    CHECK(!memcmp(ID3D10Blob_GetBufferPointer(cold),ID3D10Blob_GetBufferPointer(warm),ID3D10Blob_GetBufferSize(cold)));
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_WARP,NULL,0,NULL,0,D3D11_SDK_VERSION,&device,NULL,NULL)));
    CHECK(SUCCEEDED(ID3D11Device_CreatePixelShader(device,ID3D10Blob_GetBufferPointer(warm),ID3D10Blob_GetBufferSize(warm),NULL,&ps)));
    ID3D11PixelShader_Release(ps); ID3D10Blob_Release(warm); warm=NULL;

    swprintf_s(pattern,MAX_PATH,L"%s\\*.dxbc",root);
    search=FindFirstFileW(pattern,&found); CHECK(search!=INVALID_HANDLE_VALUE); FindClose(search);
    swprintf_s(file,MAX_PATH,L"%s\\%s",root,found.cFileName);
    /* Damaged/truncated records recover through compilation and replace the
     * bad record. No damaged bytecode is handed to the graphics driver. */
    f=CreateFileW(file,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(f!=INVALID_HANDLE_VALUE); CHECK(WriteFile(f,"broken",6,&written,NULL)); CloseHandle(f);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&warm,&hit)) && !hit);
    ID3D10Blob_Release(warm);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&warm,&hit)) && hit);
    ID3D10Blob_Release(warm);
    /* Keep the valid header and file length but damage the DXBC payload. */
    f=CreateFileW(file,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(f!=INVALID_HANDLE_VALUE);
    {
        unsigned char byte;
        CHECK(SetFilePointer(f,-1,NULL,FILE_END)!=INVALID_SET_FILE_POINTER);
        CHECK(ReadFile(f,&byte,1,&written,NULL) && written==1);
        byte^=0x80;
        CHECK(SetFilePointer(f,-1,NULL,FILE_END)!=INVALID_SET_FILE_POINTER);
        CHECK(WriteFile(f,&byte,1,&written,NULL) && written==1);
    }
    CloseHandle(f);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&warm,&hit)) && !hit);
    ID3D10Blob_Release(warm);

    /* Entry point, flags and source changes must each produce a miss. */
    CHECK(SUCCEEDED(compile(source,"alternate","ps_5_0",flags,&other,&hit)) && !hit); ID3D10Blob_Release(other);
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",D3DCOMPILE_SKIP_OPTIMIZATION,&other,&hit)) && !hit); ID3D10Blob_Release(other);
    CHECK(SUCCEEDED(compile("float4 main():SV_Target{return 0;}","main","ps_5_0",flags,&other,&hit)) && !hit); ID3D10Blob_Release(other);
    CHECK(SUCCEEDED(compile(source,"vertex","vs_5_0",flags,&other,&hit)) && !hit); ID3D10Blob_Release(other);
    CHECK(SUCCEEDED(compile(source,"vertex","vs_5_0",flags,&other,&hit)) && hit);
    CHECK(SUCCEEDED(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(other),ID3D10Blob_GetBufferSize(other),NULL,&vs)));
    ID3D11VertexShader_Release(vs); ID3D10Blob_Release(other);
    CHECK(FAILED(compile("invalid shader", "main","ps_5_0",flags,&other,&hit)) && !hit && !other);

    swprintf_s(missing,MAX_PATH,L"%s\\absent\\child",root);
    CHECK(SetEnvironmentVariableW(L"DOA3_SHADER_CACHE_DIR", missing));
    CHECK(SUCCEEDED(compile(source,"main","ps_5_0",flags,&other,&hit)) && !hit); ID3D10Blob_Release(other);
    ID3D10Blob_Release(cold); ID3D11Device_Release(device);
    search=FindFirstFileW(pattern,&found); CHECK(search!=INVALID_HANDLE_VALUE);
    do {swprintf_s(file,MAX_PATH,L"%s\\%s",root,found.cFileName);CHECK(DeleteFileW(file));} while(FindNextFileW(search,&found));
    FindClose(search); CHECK(RemoveDirectoryW(root));
    printf("Shader cache tests passed; cold %.3f ms, disk reuse %.3f ms\n",cold_ms,warm_ms);
    return 0;
}
