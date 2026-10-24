"""Compile production device state and draws; exclude window/presentation/UI.

The fixture supplies a WARP device/targets and a vtable of these unchanged
methods. No draw, shader, resource-upload or state implementation is mocked.
"""
from pathlib import Path
import sys
s=(Path(__file__).parents[2]/'src/d3d/d3d8_device.c').read_text(encoding='utf-8')
def span(a,b):
    start=s.index(a)
    return s[start:s.index(b,start)]
def function(name):
    start=s.index(name);end=s.index('{',start)+1;depth=1
    while depth:
        depth+=(s[end]=='{')-(s[end]=='}');end+=1
    return s[start:end]
parts=[
    '#include "d3d8_nv2aff.h"\n',
    span('#define MAX_RENDER_STATES', '/* Forward declarations */'),
    span('IDirect3DDevice8    *d3d8_GetDevice', '/* ================================================================\n * D3D11 initialization helpers'),
    function('static void d3d8_init_default_states'),
    span('static DWORD g_d3d_begin_count', 'static int g_off_active;')+'static int g_off_active;\n',
    function('static HRESULT __stdcall dev_BeginScene'),
    function('static HRESULT __stdcall dev_EndScene'),
    span('#define OFFRT_N 8', 'static HRESULT __stdcall dev_Clear'),
    span('static HRESULT __stdcall dev_Clear', 'static void __stdcall dev_SetGammaRamp'),
    'static ID3D11Buffer *g_nv2aff_idx_ring;\nstatic UINT g_nv2aff_idx_off;\n',
    s[s.index('#define NV2AFF_IDX_RING_SIZE'):],
]
Path(sys.argv[1]).write_text('\n'.join(parts),encoding='utf-8')
