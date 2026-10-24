"""Production frame-audit writer and strict reader integration tests."""
from pathlib import Path
import subprocess
import sys
import tempfile
import struct
import re

sys.path.insert(0,str(Path(__file__).resolve().parents[2]/"tools"))
from rollback_frame_report import inspect
from rollback_boundaries import PHASES, patch

root=Path(__file__).resolve().parents[2]
header=(root/'src/online/rollback_frame.h').read_text()
assert tuple(int(x,16) for x in re.search(r'^#define DOA3_RBF_PHASES (.+)',header,re.M)[1].split(','))==PHASES
assert int(re.search(r'#define DOA3_RBF_PHASE_COUNT (\d+)u',header)[1])==len(PHASES)
source=(root/'src/game/recomp/gen/recomp_0002.c').read_text()
instrumented=patch(source)
assert patch(instrumented)==instrumented
strip=lambda s: re.sub(r'^    (?:extern void doa3_rollback_frame_phase\(uint32_t address\)|doa3_rollback_frame_phase\(0x[0-9A-F]+u\));\n','',s,flags=re.M)
assert strip(instrumented)==strip(source)
try: patch(source.replace('loc_000A64BA: ;','loc_00000000: ;'))
except ValueError: pass
else: raise AssertionError('missing phase boundary accepted')

exe=Path(sys.argv[1])
with tempfile.TemporaryDirectory() as folder:
    folder=Path(folder)
    path=folder/"valid.rbf"
    subprocess.run([str(exe),str(path),"valid"],check=True,timeout=60)
    report=inspect(path,bytes(32))
    assert report["frames"]==2
    phases=[r for r in report['records'] if r['kind']=='outer_loop_phase']
    assert [int(r['routine'],16) for r in phases]==list(PHASES)
    assert sum(r['changed_bytes'] for r in phases)==20
    assert sum(r['kernel_calls'].get('277',0) for r in phases)==20
    assert phases[0]['since']=='dispatcher_end:00000000'
    assert report["tasks"]["00011110"]["explicit_continuation"]
    extra=report["tasks"]["00049790"]
    assert extra["explicit_continuation"] and extra["calls"]==2
    assert extra["kernel_calls"]==4 and extra["presentations"]==2
    assert extra["changed_bytes"]==2
    gap=[r for r in report["records"] if r["kind"]=="between_dispatcher_passes"]
    assert len(gap)==1 and gap[0]["changed_bytes"]==1
    assert gap[0]["pages"][0]["first"]=="00500002"
    valid=path.read_bytes()

    # 49790 is now converted. Keep coverage for genuinely unknown tasks by
    # changing only its record-header identity, preserving every memory delta.
    unknown=bytearray(valid)
    offset=64
    while any(unknown[offset:offset+32]):
        fields=struct.unpack_from("<8I",unknown,offset)
        if fields[3]==0x49790:
            struct.pack_into("<I",unknown,offset+12,0xDEAD0)
        offset+=32+1600+fields[4]*(4096+4)
    path.write_bytes(unknown)
    assert not inspect(path,bytes(32))["tasks"]["000DEAD0"]["explicit_continuation"]

    def rejects(data, digest=bytes(32)):
        path.write_bytes(data)
        try:
            inspect(path,digest)
        except ValueError:
            return
        raise AssertionError("Malformed audit was accepted")

    rejects(valid[:-32])
    rejects(valid+b"trailing")
    rejects(valid,bytes([1])+bytes(31))
    bad=bytearray(valid); struct.pack_into("<I",bad,64+16,32769)
    rejects(bad)
    bad=bytearray(valid); struct.pack_into("<I",bad,64,3)
    rejects(bad)
    bad=bytearray(valid); struct.pack_into("<I",bad,64+32+1600,32768)
    rejects(bad)
    broken=folder/"bad-slot.rbf"
    subprocess.run([str(exe),str(broken),"bad-slot"],check=True,timeout=60)
    rejects(broken.read_bytes())
    for mode in ('bad-phase','skip-phase'):
        broken=folder/(mode+'.rbf')
        subprocess.run([str(exe),str(broken),mode],check=True,timeout=60)
        rejects(broken.read_bytes())
    bad=bytearray(valid); struct.pack_into('<I',bad,4,1)
    rejects(bad) # v1 may not contain v2 phase records
print("Dispatcher audit: gating, complete passes, task effects, inter-frame writes, and malformed/incomplete captures verified.")
