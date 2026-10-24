"""Use production target allocation, routing, feedback and checkpoint adapter."""
from pathlib import Path
import sys
source = (Path(__file__).parents[2]/'src/d3d/d3d8_device.c').read_text(encoding='utf-8')
a = source.index('#define OFFRT_N 8')
b = source.index('#include "d3d8_pipeline_checkpoint.inc"', a)
Path(sys.argv[1]).write_text(source[a:b], encoding='utf-8')
