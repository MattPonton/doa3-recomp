"""Exercise the unchanged production ownership/handoff code with a controlled
audio-output wait. No physical audio device is opened by the test.
"""
from pathlib import Path
import sys
s = (Path(__file__).parents[2] / 'src/apu/apu_core.c').read_text(encoding='utf-8')
a = s.index('volatile int g_apu_det = 0;')
b = s.index('static void *mcpx_apu_frame_thread', a)
Path(sys.argv[1]).write_text(s[a:b], encoding='utf-8')
