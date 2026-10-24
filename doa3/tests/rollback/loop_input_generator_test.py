"""Changed guest or XPP effects must invalidate replay helper admission."""
from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/recomp'))
from rollback_loop_input import bodies, generate
guest,bridges=bodies()
generate(guest,bridges)
for source in (guest,bridges):
    for va in source:
        bad=dict(source)
        bad[va]+='\nsub_DEADBEEF();\n'
        try: generate(bad if source is guest else guest,bad if source is bridges else bridges)
        except ValueError: pass
        else: raise AssertionError(f'changed helper/endpoint {va:X} accepted')
print(f'All {len(guest)+len(bridges)} helper/endpoint source mutations rejected before compilation')
