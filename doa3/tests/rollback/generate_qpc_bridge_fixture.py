"""Compile the actual kernel QPC bridge in the clock checkpoint regression."""
from pathlib import Path
import sys
s = (Path(__file__).parents[2] / "src/kernel/kernel_bridge.c").read_text(encoding="utf-8")
a = s.index("static void bridge_KeQueryPerformanceCounter(void)")
b = s.index("{", a) + 1
depth = 1
while depth:
    depth += (s[b] == "{") - (s[b] == "}")
    b += 1
Path(sys.argv[1]).write_text(s[a:b], encoding="utf-8")
