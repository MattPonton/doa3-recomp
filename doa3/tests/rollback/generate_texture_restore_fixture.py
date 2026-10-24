"""Production cache lookup, format decoding and full mip/palette upload path."""
from pathlib import Path
import sys
s=(Path(__file__).parents[2]/'src/nv2a/nv2a_pgraph_d3d11.c').read_text(encoding='utf-8')
a=s.index('static struct {', s.index('} OutputVertex;'))
b=s.index('static float u2f',a)
parts=[s[a:b]]
a=s.index('static int nv_texture_format(')
b=s.index('/* NV2A texture address mode',a)
parts.append(s[a:b])
parts.append('#include "nv2a/nv2a_texture_signature.h"')
a=s.index('/* ---- texture cache:')
b=s.index('/* Called by the recompiled D3D draw wrapper',a)
parts.append(s[a:b])
Path(sys.argv[1]).write_text('\n'.join(parts),encoding='utf-8')
