from pathlib import Path
import re
import sys
root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/'tools/recomp'))
from rollback_tasks import transform
from rollback_workers import ROOTS,generate_workers
bodies={}
for name in ('recomp_0007.c','recomp_0009.c'):
    for m in re.finditer(r'^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\n\{(.*?)^\}',
                         (root/'src/game/recomp/gen'/name).read_text(),re.M|re.S):
        bodies[int(m[1],16)]=m[2]
text='\n'.join(f'static void original_{va:08X}(void)\n{{{bodies[va]}}}\n' for va in ROOTS)
text+=f'void sub_001DC660(void)\n{{{bodies[0x1DC660]}}}\n'
text+=f'void sub_0018C522(void)\n{{{bodies[0x18C522]}}}\n'
# Fail closed if either audited wrapper or a worker's wait shape changes.
for va in (0x1DC660,0x18C522):
    changed=dict(bodies); changed[va]+='\n/* changed */'
    try: generate_workers(changed,transform)
    except ValueError: pass
    else: raise AssertionError('changed wait body accepted')
text+=generate_workers(bodies,transform)
Path(sys.argv[1]).write_text(text)
