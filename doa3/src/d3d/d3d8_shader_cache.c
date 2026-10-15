#define COBJMACROS
#include "d3d8_shader_cache.h"
#include "../perf_capture.h"
#include <bcrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#pragma comment(lib, "bcrypt.lib")

#define CACHE_MAGIC 0x31435344u /* DSC1 */
#define MAX_BYTECODE (4u * 1024u * 1024u)
typedef struct CacheHeader {
    uint32_t magic, size;
    unsigned char key[32];
    unsigned char digest[32];
} CacheHeader;

/* The algorithm provider is immutable and shared; each digest still owns
 * its hash state. Keep the provider for the lifetime of the process. */
static INIT_ONCE s_sha_once = INIT_ONCE_STATIC_INIT;
static BCRYPT_ALG_HANDLE s_sha_algorithm;
static BOOL CALLBACK init_sha(PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    (void)once; (void)parameter; (void)context;
    return BCryptOpenAlgorithmProvider(&s_sha_algorithm,
        BCRYPT_SHA256_ALGORITHM, NULL, 0) >= 0;
}

static BOOL digest(const void *a, ULONG na, const void *b, ULONG nb,
                   const void *c, ULONG nc, const void *d, ULONG nd,
                   unsigned char out[32])
{
    BCRYPT_HASH_HANDLE hash = NULL;
    BOOL ok = FALSE;
    if (!InitOnceExecuteOnce(&s_sha_once, init_sha, NULL, NULL))
        return FALSE;
    if (BCryptCreateHash(s_sha_algorithm, &hash, NULL, 0, NULL, 0, 0) >= 0 &&
        BCryptHashData(hash, (PUCHAR)a, na, 0) >= 0 &&
        (!nb || BCryptHashData(hash, (PUCHAR)b, nb, 0) >= 0) &&
        (!nc || BCryptHashData(hash, (PUCHAR)c, nc, 0) >= 0) &&
        (!nd || BCryptHashData(hash, (PUCHAR)d, nd, 0) >= 0) &&
        BCryptFinishHash(hash, out, 32, 0) >= 0) ok = TRUE;
    if (hash) BCryptDestroyHash(hash);
    return ok;
}

static BOOL cache_path(const unsigned char key[32], wchar_t path[MAX_PATH])
{
    wchar_t folder[MAX_PATH], hex[65];
    DWORD n = GetEnvironmentVariableW(L"DOA3_SHADER_CACHE_DIR", folder, MAX_PATH);
    int i;
    /* Override is for test/profiling isolation; an existing directory is used
     * as-is, and an invalid/unwritable location simply disables persistence. */
    if (!n) {
        n = GetEnvironmentVariableW(L"LOCALAPPDATA", folder, MAX_PATH);
        if (!n || n >= MAX_PATH - 100) return FALSE;
        wcscat_s(folder, MAX_PATH, L"\\DOA3");
        if (!CreateDirectoryW(folder, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
            return FALSE;
        wcscat_s(folder, MAX_PATH, L"\\shader-cache-v1");
    }
    if (n >= MAX_PATH - 100 || wcslen(folder) >= MAX_PATH - 100) return FALSE;
    if (!CreateDirectoryW(folder, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
        return FALSE;
    for (i = 0; i < 32; ++i) swprintf_s(hex + 2*i, 65 - 2*i, L"%02x", key[i]);
    return swprintf_s(path, MAX_PATH, L"%s\\%s.dxbc", folder, hex) > 0;
}

static ID3DBlob *load_bytecode(const wchar_t *path, const unsigned char key[32])
{
    HANDLE f = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE,
                          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CacheHeader header;
    LARGE_INTEGER size;
    DWORD got;
    ID3DBlob *blob = NULL;
    unsigned char actual[32];
    if (f == INVALID_HANDLE_VALUE) return NULL;
    if (!GetFileSizeEx(f, &size) ||
        !ReadFile(f, &header, sizeof(header), &got, NULL) || got != sizeof(header) ||
        header.magic != CACHE_MAGIC || memcmp(header.key, key, 32) ||
        header.size < 32 || header.size > MAX_BYTECODE ||
        size.QuadPart != (LONGLONG)sizeof(header) + header.size ||
        FAILED(D3DCreateBlob(header.size, &blob))) goto done;
    if (!ReadFile(f, ID3D10Blob_GetBufferPointer(blob), header.size, &got, NULL) ||
        got != header.size ||
        memcmp(ID3D10Blob_GetBufferPointer(blob), "DXBC", 4) ||
        !digest(ID3D10Blob_GetBufferPointer(blob), header.size, NULL, 0, NULL, 0, NULL, 0, actual) ||
        memcmp(actual, header.digest, sizeof(actual))) {
        ID3D10Blob_Release(blob);
        blob = NULL;
    }
done:
    CloseHandle(f);
    return blob;
}

static void save_bytecode(const wchar_t *path, const unsigned char key[32], ID3DBlob *blob)
{
    CacheHeader header;
    wchar_t temporary[MAX_PATH];
    HANDLE f;
    DWORD written;
    BOOL ok;
    SIZE_T size = ID3D10Blob_GetBufferSize(blob);
    if (size > MAX_BYTECODE || size < 32 ||
        !digest(ID3D10Blob_GetBufferPointer(blob), (ULONG)size,
                NULL, 0, NULL, 0, NULL, 0, header.digest)) return;
    header.magic = CACHE_MAGIC;
    header.size = (uint32_t)size;
    memcpy(header.key, key, 32);
    /* Separate temporary file per process/thread; readers only see complete
     * records, including when two running copies encounter the same shader. */
    if (swprintf_s(temporary, MAX_PATH, L"%s.%lu.%lu.tmp", path,
                   GetCurrentProcessId(), GetCurrentThreadId()) < 0) return;
    f = CreateFileW(temporary, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE) return;
    ok = WriteFile(f, &header, sizeof(header), &written, NULL) && written == sizeof(header);
    if (ok) ok = WriteFile(f, ID3D10Blob_GetBufferPointer(blob), (DWORD)size, &written, NULL)
                 && written == size;
    CloseHandle(f);
    if (ok) ok = MoveFileExW(temporary, path, MOVEFILE_REPLACE_EXISTING);
    if (!ok) DeleteFileW(temporary);
}

HRESULT d3d8_compile_shader_cached(const char *source, SIZE_T length,
    const char *name, const char *entry, const char *profile, UINT flags,
    ID3DBlob **code, ID3DBlob **errors, BOOL *cache_hit)
{
    unsigned char key[32];
    wchar_t path[MAX_PATH];
    BOOL persistent = FALSE;
    LARGE_INTEGER begin = {0}, end, frequency;
    HRESULT hr;
    if (cache_hit) *cache_hit = FALSE;
    if (errors) *errors = NULL;
    if (!code) return E_INVALIDARG;
    *code = NULL;
    if (!source || !entry || !profile || !length) return E_INVALIDARG;
    begin.QuadPart = doa3_perf_begin();
    if (length <= UINT32_MAX &&
        digest(source, (ULONG)length, entry, (ULONG)strlen(entry) + 1,
               profile, (ULONG)strlen(profile) + 1, &flags, sizeof(flags), key))
        persistent = cache_path(key, path);
    if (persistent && (*code = load_bytecode(path, key)) != NULL) {
        if (cache_hit) *cache_hit = TRUE;
        if (begin.QuadPart) {
            QueryPerformanceCounter(&end); QueryPerformanceFrequency(&frequency);
            fprintf(stderr, "[SHADER] %s disk hit %.3f ms\n", profile,
                1000.0 * (end.QuadPart - begin.QuadPart) / frequency.QuadPart);
        }
        return S_OK;
    }
    hr = D3DCompile(source, length, name, NULL, NULL, entry, profile,
                    flags, 0, code, errors);
    if (SUCCEEDED(hr) && persistent) save_bytecode(path, key, *code);
    if (begin.QuadPart) {
        QueryPerformanceCounter(&end); QueryPerformanceFrequency(&frequency);
        fprintf(stderr, "[SHADER] %s compile%s %.3f ms\n", profile,
            FAILED(hr) ? " failed" : "", 1000.0 * (end.QuadPart - begin.QuadPart) / frequency.QuadPart);
    }
    return hr;
}
