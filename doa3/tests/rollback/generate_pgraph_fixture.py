"""Compile unchanged production method handlers with private GPU sinks.

Keep the actual host structure in this fixture so the test can prove that a
logical restore leaves native texture ownership and statistics untouched.
"""
from pathlib import Path
import sys

source = (Path(__file__).parents[2] / 'src/nv2a/nv2a_pgraph_d3d11.c').read_text(encoding='utf-8')

def function(name):
    start = source.index(name)
    brace = source.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end] + '\n'

start = source.index('static struct {', source.index('} OutputVertex;'))
end = source.index('static float u2f', start)
parts = ['\n'.join(line for line in source.splitlines() if line.startswith('#define NV097_')), source[start:end]]
for name in ('static float u2f', 'static int nv2a_draw_mode_to_d3d',
             'static uint32_t nv2a_blend_to_d3d',
             'void pgraph_d3d11_init', 'void pgraph_d3d11_shutdown',
             'void pgraph_d3d11_set_inline_hint', 'void pgraph_d3d11_set_vertex_layout'):
    parts.append(function(name))
parts.append(source[source.index('#define DIAG_VP_PTR'):source.index('\nuint32_t g_mhist[0x800];', source.index('#define DIAG_VP_PTR'))])
parts.append(function('void pgraph_d3d11_flush'))
start = source.index('    {\n', source.index('    /* Blending.'))
end = source.index('    /* Alpha test.', start)
parts.append('static void apply_blend_fixture(IDirect3DDevice8 *dev) {\n' + source[start:end] + '}\n')
Path(sys.argv[1]).write_text('\n'.join(parts), encoding='utf-8')
