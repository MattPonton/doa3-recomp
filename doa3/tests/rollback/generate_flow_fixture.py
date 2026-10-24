"""Actual main-flow -> setup -> resource-wait bodies; external work is synthetic."""
from pathlib import Path
import re
import sys

root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/"tools/recomp"))
from rollback_nested import generate, transform
from rollback_match import FLOW_ROUTINES, NATIVE_BOUNDARIES, suspension_graph

all_bodies={int(m[1],16):m[2] for f in (root/"src/game/recomp/gen").glob("recomp_*.c")
    for m in re.finditer(r"^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\s*\{(.*?)^\}",f.read_text(),re.M|re.S)}
manual={int(x,16) for x in re.findall(r"^void sub_([0-9A-F]{8})\(void\)",
    (root/"src/game/recomp/recomp_manual.c").read_text(),re.M)}
assert suspension_graph(all_bodies,manual,0x87420)[0]==FLOW_ROUTINES
assert 0xA47E0 in NATIVE_BOUNDARIES and 0xA47E0 not in FLOW_ROUTINES
bodies={va:all_bodies[va] for va in (0x87420,0x87620,0x873A0)}
synchronous={int(x,16) for b in bodies.values() for x in re.findall(r"sub_([0-9A-F]{8})\(",b)}-set(bodies)-{0x1C65D8}
parts=[f"static void sub_{va:08X}(void);" for va in sorted(set(bodies)|synchronous)]
parts += [f"static void sub_{va:08X}(void) {{ fixture_helper(0x{va:X}); }}" for va in sorted(synchronous)]
parts += [f"static void sub_{va:08X}(void) {{{body}}}" for va,body in sorted(bodies.items())]
parts.append(generate(bodies,synchronous))
Path(sys.argv[1]).write_text("\n".join(parts))

# Each admitted local table is pinned to its known expression and target set.
for va in (0x87420,0x20DE0,0x18A430):
    body=all_bodies[va]
    calls={int(x,16) for x in re.findall(r"sub_([0-9A-F]{8})\(",body)}-{0x1C65D8,va}
    transform(va,body,{va},calls)
    bad=body.replace("RECOMP_ITAIL(_jt)","RECOMP_ITAIL(eax)")
    try: transform(va,bad,{va},calls)
    except ValueError: pass
    else: raise AssertionError("changed local table accepted")

