"""Actual match setup + wait helper, with explicitly synthetic external work."""
from pathlib import Path
import re
import sys

root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/"tools/recomp"))
from rollback_nested import generate, transform

all_bodies={int(m[1],16):m[2] for f in (root/"src/game/recomp/gen").glob("recomp_*.c")
    for m in re.finditer(r"^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\s*\{(.*?)^\}",f.read_text(),re.M|re.S)}
bodies={va:all_bodies[va] for va in (0x72FE0,0x85820)}
bodies[0x1000]="""
    uint32_t counter = 0;
    uint32_t ebp = g_seh_ebp;
loc_00001000: ;
    counter++;
    MEM32(0x61000) = counter;
    PUSH32(esp, 0); sub_00001001(); /* call 0x00001001 */
loc_00001005: ;
    MEM32(0x61004) = counter + ebp;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_001C65D8(); /* call 0x001C65D8 */
loc_00001010: ;
    esp += 4;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_001C65D8(); /* call 0x001C65D8 */
loc_00001020: ;
    esp += 4;
    goto loc_00001000;
"""
bodies[0x1001]="""
loc_00001001: ;
    sub_00072FE0(); return; /* tail jmp 0x00072FE0 */
"""
synchronous={0x49180,0x72930,0x72E90,0x75A40,0x762B0,0x76490,0x76AC0,0x76CA0,
    0x76D70,0x76DD0,0x7B3D0,0x7C0A0,0x7C340,0x8ABF0,0xA5B40,0xADE30,0xDBA40,
    0x1C1BE4,0x1C6465}
parts=[f"static void sub_{va:08X}(void);" for va in sorted(set(bodies)|synchronous)]
parts += [f"static void sub_{va:08X}(void) {{ fixture_helper(0x{va:X}); }}" for va in sorted(synchronous)]
parts += [f"static void sub_{va:08X}(void) {{{body}}}" for va,body in sorted(bodies.items())]
parts.append(generate(bodies,synchronous))
Path(sys.argv[1]).write_text("\n".join(parts))

# Scope, declaration and allowlist failures must not produce a partial program.
base=bodies[0x1000]
bad=[base.replace("uint32_t counter = 0;","double counter = 0;"),
     base.replace("sub_00001001();", "sub_0000DEAD();"),
     base.replace("    PUSH32(esp, 0); sub_00001001();", "    if (eax) sub_00001001();"),
     base.replace("    PUSH32(esp, 0); sub_00001001();", "    {\n    PUSH32(esp, 0); sub_00001001();").replace("loc_00001005:","    }\nloc_00001005:"),
     base.replace("sub_00001001();", "RECOMP_ICALL(eax);"),
     base.replace("uint32_t counter = 0;","uint32_t counter[2];")]
for source in bad:
    try: transform(0x1000,source,set(bodies),synchronous)
    except ValueError: pass
    else: raise AssertionError("unsafe nested conversion accepted")
try: generate(bodies,synchronous|{0x1C65D8})
except ValueError: pass
else: raise AssertionError("yield accepted as synchronous helper")
