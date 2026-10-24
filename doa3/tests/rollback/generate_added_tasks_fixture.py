"""Compare actual task-root control flow, with deterministic helper fixtures.

Helper behavior is intentionally not a game oracle. Live shadow replay tests
the real call graph; this test isolates conversion of the root and its yields.
"""
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(root / "tools/recomp"))
from rollback_tasks import transform

parts = []
for va, file in ((0x54110, "recomp_0001.c"), (0xBE4A0, "recomp_0003.c"),
                 (0x1C60AB, "recomp_0009.c")):
    source = (root / "src/game/recomp/gen" / file).read_text()
    match = re.search(rf"^void sub_{va:08X}_gen\(void\)\s*\{{(.*?)^\}}", source, re.M | re.S)
    assert match, hex(va)
    body = match[1]
    parts.append(f"static void original_{va:08X}(void)\n{{{body}}}\n")
    parts.append(transform(va, body))
# These two real helpers exercise the player's previous-value comparison,
# single/tag layout branches and local EBP frames. Rendering and initialization
# helpers remain explicit fixtures, rather than being claimed replay-safe.
source = (root / "src/game/recomp/gen/recomp_0009.c").read_text()
for va in (0x1C1705, 0x1C5F54):
    match = re.search(rf"^void sub_{va:08X}_gen\(void\)\s*\{{(.*?)^\}}", source, re.M | re.S)
    assert match, hex(va)
    parts.append(f"void sub_{va:08X}(void)\n{{{match[1]}}}\n")
Path(sys.argv[1]).write_text("\n".join(parts))
