from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/recomp'))
from rollback_loop_time import bodies,generate
parts=bodies()
generate(parts)
for name in parts:
    bad=dict(parts);bad[name]+='\nsub_DEADBEEF();\n'
    try:generate(bad)
    except ValueError:pass
    else:raise AssertionError(f'changed timing body accepted: {name}')
print('All four timing/XAPI/clock-bridge source mutations rejected')
