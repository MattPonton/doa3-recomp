"""Actual display-task roots with explicit deterministic external helpers.

This checks continuation conversion, not graphics replay or host restoration.
The E8100/E8830 state helpers run their real translated bodies as well.
"""
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(root / "tools/recomp"))
from rollback_tasks import transform
from rollback_match import suspension_graph, suspension_body

bodies = {}
for file in (root / "src/game/recomp/gen").glob("recomp_*.c"):
    for match in re.finditer(r"^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\s*\{(.*?)^\}",
                             file.read_text(), re.M | re.S):
        bodies[int(match[1], 16)] = match[2]
manual_source = (root / "src/game/recomp/recomp_manual.c").read_text()
manual = {int(x, 16) for x in re.findall(r"^void sub_([0-9A-F]{8})\(void\)", manual_source, re.M)}
parts = []
for va in (0xE8030, 0x49790, 0x691F0):
    selected, _ = suspension_graph(bodies, manual, va)
    assert selected == {va}, (hex(va), selected)
    body = bodies[va]
    parts.append(f"static void original_{va:08X}(void)\n{{{body}}}\n")
    parts.append(transform(va, body))
for va in (0xE8100, 0xE8830):
    parts.append(f"void sub_{va:08X}(void)\n{{{bodies[va]}}}\n")
Path(sys.argv[1]).write_text("\n".join(parts))

# A reachable jump/call or lost final return must invalidate the leaf proof.
leaf = bodies[0x496D0]
prefix, tail = leaf.split("loc_00049795: ;")
for bad in (prefix.replace("goto loc_00049729;", "goto loc_00049795;") + "loc_00049795: ;" + tail,
            prefix.rstrip().removesuffix("/* ret */").rstrip().removesuffix("return;") + "\nloc_00049795: ;" + tail,
            leaf.replace("loc_000496D0: ;", "loc_000496D0: ;\n    sub_001C65D8();"),
            leaf.replace("loc_000496D0: ;", "loc_000496D0: ;\n    RECOMP_ITAIL(eax);")):
    try: suspension_body(0x496D0,bad)
    except ValueError: pass
    else: raise AssertionError("unproven leaf accepted")
assert "sub_001C65D8" not in suspension_body(0x496D0,leaf)
