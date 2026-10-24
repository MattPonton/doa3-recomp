"""Verify isolated kernel ABI adapters; unsupported calls must fail closed."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile


def fixture(target, stack=0x20000, voice_flags=0, address=0xFFFFFFFF):
    output=bytearray(struct.pack("<8I",0x31434252,2,128*1024*1024,0,0,0,264,1)+bytes(32))
    regs=list(range(16)); regs[1]=0xAABBCC02; regs[3]=stack; regs[14]=0; regs[15]=0x27F
    audio=target==0x1F5157
    if audio: regs[0]=0x1234; regs[1]=0x30000
    local=[0]*32; local[30]=target; local[31]=0x4B45524E
    page=bytearray([0xA5]*4096)
    struct.pack_into("<4I",page,0,0x12345678,address,0x8000,1)
    kpcr=bytearray([0x5A]*4096); kpcr[0x24]=1
    irql=target in (0xFE000010,0xFE000014,0xFE000018)
    voice=bytearray([0x36]*4096); struct.pack_into("<H",voice,0x12,voice_flags)
    for kind in (1,2):
        output.extend(struct.pack("<4I",kind,0x11110,2+int(audio) if kind==1 else int(irql),100))
        output.extend(struct.pack("<16I8d34I",*regs,*map(float,range(8)),1,1,*local))
        if kind==1:
            output.extend(struct.pack("<I",0)+kpcr+struct.pack("<I",0x20)+page)
            if audio: output.extend(struct.pack("<I",0x30)+voice)
            else: regs[0]=address if target==0xFE000020 else int(irql and target!=0xFE000014)
            regs[3]+=4 if irql or audio else 16 if target==0xFE00001C else 8
            kpcr[0x24]=2
        elif irql:
            output.extend(struct.pack("<I",0)+kpcr)
    output.extend(bytes(16))
    return output


with tempfile.TemporaryDirectory() as directory:
    path=Path(directory)/"kernel.rbc"
    def run(target,code,message,stack=0x20000,voice_flags=0,address=0xFFFFFFFF):
        path.write_bytes(fixture(target,stack,voice_flags,address))
        result=subprocess.run([sys.argv[1],str(path)],capture_output=True,text=True,timeout=30)
        assert result.returncode==code,(result.stdout,result.stderr)
        assert message in result.stdout+result.stderr,(result.stdout,result.stderr)
    for slot in range(3):
        run(0xFE000000+slot*4,0,"matched=1 blocked=0 diverged=0")
    run(0xFE00000C,1,"unsupported_kernel_ordinal")
    for slot in (4,5,6):
        run(0xFE000000+slot*4,0,"matched=1 blocked=0 diverged=0")
    for address in (0,0x1234000,0xFFFFFFFF):
        for slot in (7,8):
            run(0xFE000000+slot*4,0,"matched=1 blocked=0 diverged=0",address=address)
    run(0xFE000024,1,"invalid_kernel_thunk")
    run(0xFE000001,1,"invalid_kernel_thunk")
    run(0xFE000000,1,"outside_guest_memory",128*1024*1024-4)
    run(0xFE00001C,1,"outside_guest_memory",128*1024*1024-12)
    run(0xFE000020,1,"outside_guest_memory",128*1024*1024-4)
    for flags in (0,1,0x8000):
        run(0x1F5157,0,"matched=1 blocked=0 diverged=0",voice_flags=flags)
    run(0x1F5157,1,"audio_retirement_requires_apu",voice_flags=0x8001)
    run(0xFE820010,1,"apu_mmio_requires_snapshot")
    run(0xFD800044,1,"graphics_state_requires_snapshot")
print("Kernel ABI/IRQL, page-lock ABI/identity address, inactive audio wait, bounds and unsupported host-effect rejection verified.")
