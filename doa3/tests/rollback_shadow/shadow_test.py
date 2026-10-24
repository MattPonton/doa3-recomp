"""Exercise the offline reader and real translated task's paused yield path."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0,str(Path(__file__).resolve().parents[2]/"tools/recomp"))
from rollback_shadow import remove_unused_flag_reads
from rollback_tasks import TASKS

dead = "    uint32_t _fs0 = 0;\n    _fs0 = MEM8(ecx); /* flag operand kept for a later jcc */\n"
assert "MEM8" not in remove_unused_flag_reads(dead)
live = dead + "    if (_fs0) goto loc_00000001;\n"
assert remove_unused_flag_reads(live) == live

exe, digest_file = map(Path, sys.argv[1:3])
digest = digest_file.read_bytes()
header = struct.pack("<8I",0x31434252,1,128*1024*1024,0,0,0,264,30)+digest
pages = {}


def put(address, value, size=4):
    page = pages.setdefault(address//4096,bytearray(4096))
    page[address%4096:address%4096+size] = value.to_bytes(size,"little")


put(0x596A37,3,1)  # native collision task skips gameplay while this phase is paused
put(0x73C644,10)
put(0x73C640,0xF1BE0000)
put(0x73C400+10*32,1)
put(0x73C404+10*32,1)
put(0x20000,1)
regs = [0]*16
regs[3],regs[15] = 0x20000,0x27F
post = regs.copy()
post[0],post[1],post[2] = 0xF1BE0000,0x73C400+10*32,1


def state(registers):
    return struct.pack("<16I8d34I",*registers,*([0.0]*8),1,1,*([0]*32))


def record(kind, routine, registers, changes):
    return (struct.pack("<4I",kind,routine,len(changes),0)+state(registers)+
            b"".join(struct.pack("<I",page)+bytes(data) for page,data in sorted(changes.items())))


def fixture(routine=0x7F470, mismatch=False):
    result=bytearray(header)
    for index in range(30):
        result.extend(record(1,routine,regs,pages if not index else {}))
        bad = {0:bytes([1])+bytes(4095)} if mismatch and not index else {}
        result.extend(record(2,routine,post,bad))
    result.extend(bytes(16))
    return result


with tempfile.TemporaryDirectory() as temp:
    path=Path(temp)/"fixture.rbc"

    def run(data, code, text):
        path.write_bytes(data)
        result=subprocess.run([str(exe),str(path)],capture_output=True,text=True,timeout=90)
        assert result.returncode==code,(result.returncode,result.stdout,result.stderr)
        assert text in result.stdout+result.stderr,(result.stdout,result.stderr)

    valid=fixture()
    run(valid,0,"matched=30 blocked=0 diverged=0")
    profile=subprocess.run([str(exe),str(path),"--native-camera"],capture_output=True,text=True,timeout=90)
    assert profile.returncode==0 and "PROFILE native-camera" in profile.stdout
    bad_option=subprocess.run([str(exe),str(path),"--ignore-differences"],capture_output=True,text=True,timeout=10)
    assert bad_option.returncode==2
    run(fixture(mismatch=True),1,"diverged=1")
    run(fixture(0xBAD),1,"blocked=30")
    run(valid[:-16],2,"Truncated or invalid")
    # The writer's bound grows with the converted task set. Keep rejecting
    # oversized/empty captures without a stale 64-pair ceiling on valid runs.
    for count in (0, len(TASKS)*6+1, 0xFFFFFFFF):
        invalid_count=bytearray(header)
        struct.pack_into("<I",invalid_count,28,count)
        run(invalid_count,2,"Invalid capture header")
    bad=bytearray(header); bad[32]^=1
    run(bad,2,"sources differ")
    run(header+struct.pack("<4I",1,0x7F470,1,0)+state(regs)+struct.pack("<I",32768),2,"Truncated or invalid")
    run(header+record(2,0x7F470,post,{}),2,"Truncated or invalid")
    # A real serialized APU image travels through the production reader and
    # isolated import/export even when this paused guest task issues no audio.
    apu_path=Path(temp)/"apu.bin"
    subprocess.run([sys.argv[3],"--snapshot",str(apu_path)],check=True,capture_output=True,timeout=30)
    apu=apu_path.read_bytes()
    apu_header=struct.pack("<8I",0x31434252,3,128*1024*1024,0,0,0,264+len(apu),1)+digest
    def apu_record(kind,registers,changes,payload):
        rec=record(kind,0x7F470,registers,changes)
        return rec[:280]+payload+rec[280:]
    apu_valid=(apu_header+apu_record(1,regs,pages,apu)+apu_record(2,post,{},apu)+bytes(16))
    run(apu_valid,0,"matched=1 blocked=0 diverged=0")
    bad=bytearray(apu); bad[4]^=1
    run(apu_header+apu_record(1,regs,pages,bad)+apu_record(2,post,{},apu)+bytes(16),1,"invalid_apu_snapshot")
    run(apu_header+apu_record(1,regs,pages,apu)+apu_record(2,post,{},bad)+bytes(16),1,"diverged=1")
    run(apu_valid[:64+280+len(apu)-1],2,"Truncated or invalid")
    # Production v4 reader + actual nested match task. This resumed path calls
    # 72F50 and returns to 72070, then yields; the selected state avoids setup IO.
    nested=struct.pack("<4I",2,1,2,0)+struct.pack("<66I",0x72070,1,*([0]*64))+bytes(15*264)
    state_bytes=264+len(nested)
    v4_header=struct.pack("<8I",0x31434252,4,128*1024*1024,0,0,0,state_bytes+len(apu),1)+digest
    put(0x5B1424,7)
    def v4_record(kind,registers,changes,stack=nested):
        rec=record(kind,0x72070,registers,changes)
        return rec[:280]+stack+apu+rec[280:]
    v4=v4_header+v4_record(1,regs,pages)+v4_record(2,post,{})+bytes(16)
    run(v4,0,"matched=1 blocked=0 diverged=0")
    corrupt=bytearray(nested); struct.pack_into("<I",corrupt,20,99999)
    run(v4_header+v4_record(1,regs,pages,corrupt)+v4_record(2,post,{})+bytes(16),1,"blocked=1")
    run(v4_header+v4_record(1,regs,pages)+v4_record(2,post,{},corrupt)+bytes(16),1,"diverged=1")
    run(v4[:64+280+len(nested)-1],2,"Truncated or invalid")
    # v5 carries both captured devices, preserving all serialized bytes.
    gpu=struct.pack("<8260I",4,0,*range(8258))+bytes(40+264)
    v5_header=struct.pack("<8I",0x31434252,5,128*1024*1024,0,0,0,state_bytes+len(apu)+len(gpu),1)+digest+struct.pack("<2I",len(apu),len(gpu))
    def v5_record(kind,registers,changes,payload=gpu):
        rec=v4_record(kind,registers,changes)
        split=16+state_bytes+len(apu)
        return rec[:split]+payload+rec[split:]
    v5=v5_header+v5_record(1,regs,pages)+v5_record(2,post,{})+bytes(16)
    run(v5,0,"matched=1 blocked=0 diverged=0")
    bad=bytearray(gpu); bad[8]^=1
    run(v5_header+v5_record(1,regs,pages)+v5_record(2,post,{},bad)+bytes(16),1,"diverged=1")
    bad=bytearray(gpu); bad[272+0x124C*4]^=1
    run(v5_header+v5_record(1,regs,pages)+v5_record(2,post,{},bad)+bytes(16),1,"diverged=1")
    bad=bytearray(v5); struct.pack_into("<I",bad,64,0xFFFFFFFF)
    run(bad,2,"Invalid capture payload sizes")
    bad=bytearray(gpu); bad[4]=1
    run(v5_header+v5_record(1,regs,pages,bad)+v5_record(2,post,{})+bytes(16),1,"invalid_gpu_snapshot")
print("Shadow checker: native yield replay, divergence, unsupported calls, stale sources, and malformed streams verified.")
