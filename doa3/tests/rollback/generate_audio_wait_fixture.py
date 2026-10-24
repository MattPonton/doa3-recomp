from pathlib import Path
import re
import sys
root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/'tools/recomp'))
from rollback_boundaries import patch_audio_wait
source=(root/'src/game/recomp/gen/recomp_0007.c').read_text()
patched=patch_audio_wait(source)
assert patch_audio_wait(patched)==patched
try: patch_audio_wait(source.replace('MEM32(0xC75620)','MEM32(0xC75628)'))
except ValueError: pass
else: raise AssertionError('changed deadline accepted')
wait=re.search(r'^void sub_00199DB0(?:_gen)?\(void\)\n\{.*?^\}',patched,re.M|re.S)[0]
xapi=(root/'src/game/recomp/gen/recomp_0006.c').read_text()
clock=re.search(r'^void sub_0018B744_gen\(void\)\n\{.*?^\}',xapi,re.M|re.S)[0]
Path(sys.argv[1]).write_text(clock.replace('sub_0018B744_gen','sub_0018B744')+'\n'+wait)
