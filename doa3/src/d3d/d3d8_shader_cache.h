#ifndef DOA3_D3D8_SHADER_CACHE_H
#define DOA3_D3D8_SHADER_CACHE_H

#include <d3dcompiler.h>

/* Generated shaders have no includes or macros. Cache their DXBC, not GPU
 * objects: source, entry point, profile and compile flags identify the result.
 * Cache failures always fall back to the compiler. Optional hit output is
 * useful for diagnostics without depending on timing to detect cache reuse. */
HRESULT d3d8_compile_shader_cached(const char *source, SIZE_T length,
    const char *name, const char *entry, const char *profile, UINT flags,
    ID3DBlob **code, ID3DBlob **errors, BOOL *cache_hit);

#endif
