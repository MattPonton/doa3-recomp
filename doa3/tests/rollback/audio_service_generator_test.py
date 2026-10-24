from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/recomp'))
from rollback_audio_service import bodies,generate
import rollback_audio_sync as sync
parts=bodies();generate(parts)
for va in parts:
    bad=dict(parts);bad[va]+='\nsub_DEADBEEF();\n'
    try:generate(bad)
    except ValueError:pass
    else:raise AssertionError(f'changed audio body accepted: {va:X}')
original=sync.source
sync.source=lambda: original()+'void new_effect(void){write_to_host();}'
try:generate(parts)
except ValueError:pass
else:raise AssertionError('changed host synchronization accepted')
print('All nine audio service source mutations and synchronization changes rejected')
