"""Original lifted outer loop with controlled external helpers for comparison."""
from pathlib import Path
import sys
root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/'tools/recomp'))
from rollback_loop import body_from, generate, CALLS
body=body_from((root/'src/game/recomp/gen/recomp_0002.c').read_text())
generate(body)  # Apply the same production source guard before fixture creation.
for old,new in [('0x9B2390','0x9B2394'),('sub_001C650C();','sub_001C650D();'),
                ('goto loc_000A6450;','goto loc_000A64C9;'),('esp = esp + 4;','esp = esp + 8;')]:
    assert old in body
    try: generate(body.replace(old,new,1))
    except ValueError: pass
    else: raise AssertionError('unreviewed outer-loop mutation accepted')
parts=[f'static void sub_{va:08X}(void){{fixture_helper(0x{va:X});}}' for va in CALLS]
# Begin at the same pre-dispatch boundary rather than the native function's
# first-ever entry. All subsequent guest instructions/control flow are original.
body=body.replace('loc_000A6450: ;','goto loc_000A64B0;\nloc_000A6450: ;',1)
parts.append('static void native_loop(void){'+body+'}')
Path(sys.argv[1]).write_text('\n'.join(parts))
