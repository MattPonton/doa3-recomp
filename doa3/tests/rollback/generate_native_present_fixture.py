from pathlib import Path
import sys
root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/'tools/recomp'))
from rollback_loop_input import extract
s=(root/'src/game/recomp/recomp_manual.c').read_text()
body=extract(s.replace('d3d4134_frame_done(uint32_t dev)','d3d4134_frame_done(void)'), 'd3d4134_frame_done')
Path(sys.argv[1]).write_text('static void d3d4134_frame_done(uint32_t dev){'+body+'}\n')
