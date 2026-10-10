/* Execute the renderer's real vertex shader and input layout on WARP.
 * Stream output captures UVs directly, without pixel sampling tolerances. */
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "d3d8_nv2aff.h"
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static ID3D11Device *d3d8_GetD3D11Device(void) { return device; }
static ID3D11DeviceContext *d3d8_GetD3D11Context(void) { return context; }
#include "nv2aff_impl.h"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Line %d: %s\n", __LINE__, #c); exit(1); } } while (0)
static ID3D11Buffer *vertices, *output, *readback;
static unsigned checks;

static void expect_uv(const char *name, Nv2aFFConstants *cb, Nv2aFFVertex *v,
                      float u0, float v0, float u1, float v1)
{
    UINT offset = 0, stride = sizeof *v;
    D3D11_MAPPED_SUBRESOURCE map;
    float expected[4] = {u0, v0, u1, v1};
    CHECK(d3d8_Nv2aFF_Bind(cb));
    ID3D11DeviceContext_UpdateSubresource(context, (ID3D11Resource *)vertices, 0, NULL, v, 0, 0);
    ID3D11DeviceContext_IASetVertexBuffers(context, 0, 1, &vertices, &stride, &offset);
    ID3D11DeviceContext_SOSetTargets(context, 1, &output, &offset);
    ID3D11DeviceContext_Draw(context, 1, 0);
    ID3D11DeviceContext_SOSetTargets(context, 0, NULL, NULL);
    ID3D11DeviceContext_CopyResource(context, (ID3D11Resource *)readback, (ID3D11Resource *)output);
    CHECK(SUCCEEDED(ID3D11DeviceContext_Map(context, (ID3D11Resource *)readback, 0, D3D11_MAP_READ, 0, &map)));
    for (int i = 0; i < 4; i++) {
        float actual = ((float *)map.pData)[i];
        if (!isfinite(actual) || fabsf(actual - expected[i]) > 0.0001f) {
            fprintf(stderr, "%s: component %d expected %.6f, got %.6f\n", name, i, expected[i], actual);
            exit(1);
        }
    }
    ID3D11DeviceContext_Unmap(context, (ID3D11Resource *)readback, 0);
    printf("PASS %s\n", name);
    checks++;
}

int main(void)
{
    ID3DBlob *blob = NULL;
    ID3D11GeometryShader *so = NULL;
    D3D11_BUFFER_DESC bd = {0};
    D3D11_SO_DECLARATION_ENTRY decl[] = {
        {0, "TEXCOORD", 0, 0, 2, 0}, {0, "TEXCOORD", 1, 0, 2, 0}
    };
    UINT so_stride = 4 * sizeof(float);
    Nv2aFFConstants cb = {0};
    Nv2aFFVertex v = {0};
    CHECK(SUCCEEDED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0,
          D3D11_SDK_VERSION, &device, NULL, &context)));
    CHECK(SUCCEEDED(d3d8_Nv2aFF_Init()));
    CHECK(SUCCEEDED(D3DCompile(g_nv2aff_vs_source, strlen(g_nv2aff_vs_source), "texgen_test",
          NULL, NULL, "main", "vs_5_0", 0, 0, &blob, NULL)));
    CHECK(SUCCEEDED(ID3D11Device_CreateGeometryShaderWithStreamOutput(device,
          ID3D10Blob_GetBufferPointer(blob), ID3D10Blob_GetBufferSize(blob), decl, 2,
          &so_stride, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &so)));
    ID3D10Blob_Release(blob);
    ID3D11DeviceContext_GSSetShader(context, so, NULL, 0);
    ID3D11DeviceContext_IASetPrimitiveTopology(context, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    bd.ByteWidth = sizeof v;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(device, &bd, NULL, &vertices)));
    bd.ByteWidth = so_stride;
    bd.BindFlags = D3D11_BIND_STREAM_OUTPUT;
    CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(device, &bd, NULL, &output)));
    bd.BindFlags = 0;
    bd.Usage = D3D11_USAGE_STAGING;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    CHECK(SUCCEEDED(ID3D11Device_CreateBuffer(device, &bd, NULL, &readback)));
    for (int i = 0; i < 4; i++) cb.clip[i][i] = cb.texmat0[i][i] = cb.texmat1[i][i] = 1;
    for (int i = 0; i < 3; i++) cb.mv[i][i] = 1;
    v.pos[2] = -2; v.pos[3] = 1;
    v.nrm[0] = 0.6f; v.nrm[2] = 0.8f;
    v.uv[0] = 0.125f; v.uv[1] = 0.25f;
    v.uv1[0] = 0.75f; v.uv1[1] = 0.875f;
    expect_uv("ordinary coordinates", &cb, &v, .125f, .25f, .75f, .875f);

    /* Zack C3 capture: REFLECTION_MAP S/T/R, matrix (0.5*x+0.5,
     * -0.5*y+0.5), stage program 2D_PROJECTIVE. For U=(0,0,-1)
     * and N=(.6,0,.8), the reflected ray is (.96,0,.28). */
    cb.tex0_mode[0] = cb.tex0_mode[1] = cb.tex0_mode[2] = 1;
    cb.texmat0[0][0] = .5f; cb.texmat0[0][3] = .5f;
    cb.texmat0[1][1] = -.5f; cb.texmat0[1][3] = .5f;
    expect_uv("stage 0 silver reflection", &cb, &v, .98f, .5f, .75f, .875f);
    v.nrm[0] = -.6f;
    expect_uv("reflection follows surface normal", &cb, &v, .02f, .5f, .75f, .875f);
    cb.tex1_mode[0] = cb.tex1_mode[1] = cb.tex1_mode[2] = 1;
    memcpy(cb.texmat1, cb.texmat0, sizeof cb.texmat1);
    expect_uv("both reflection layers", &cb, &v, .02f, .5f, .02f, .5f);
    memset(cb.tex0_mode, 0, sizeof cb.tex0_mode);
    expect_uv("stage 1 reflection unchanged", &cb, &v, .125f, .25f, .02f, .5f);
    cb.tex0_mode[1] = cb.tex0_mode[2] = 1;
    cb.texmat0[3][3] = 2;
    expect_uv("stage 0 matrix and projection", &cb, &v, .28125f, .1875f, .02f, .5f);
    /* Beach shoreline capture: unlit, vertex-colored DXT5 foam with
     * negative V and a scrolling stage-0 matrix. Its unused Z/Q rows are
     * zero; a zero Q must not discard the translated coordinates. */
    memset(cb.tex1_mode, 0, sizeof cb.tex1_mode);
    memset(cb.texmat0, 0, sizeof cb.texmat0);
    cb.flags = NV2AFF_FLAG_HAS_COLOR;
    cb.texmat0[0][0] = cb.texmat0[1][1] = 1;
    cb.texmat0[1][3] = .163402155f;
    v.uv[0] = 1; v.uv[1] = -.500976622f;
    expect_uv("shoreline scroll with zero Q", &cb, &v, 1, -.337574467f, .75f, .875f);
    cb.texmat0[1][3] = .245854929f;
    expect_uv("shoreline scroll advances", &cb, &v, 1, -.255121693f, .75f, .875f);
    v.uv[0] = .125f; v.uv[1] = .25f;
    memset(cb.tex0_mode, 0, sizeof cb.tex0_mode);
    memset(cb.tex1_mode, 0, sizeof cb.tex1_mode);
    expect_uv("return to ordinary material", &cb, &v, .125f, .25f, .75f, .875f);
    printf("%u shader checks passed.\n", checks);
    ID3D11DeviceContext_ClearState(context);
    d3d8_Nv2aFF_Shutdown();
    ID3D11Buffer_Release(vertices); ID3D11Buffer_Release(output); ID3D11Buffer_Release(readback);
    ID3D11GeometryShader_Release(so); ID3D11DeviceContext_Release(context); ID3D11Device_Release(device);
    return 0;
}
