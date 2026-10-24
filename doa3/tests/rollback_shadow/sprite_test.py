"""The real sprite adapter must delegate native IDs and reject host assets."""
import struct, subprocess, sys, tempfile
from pathlib import Path

def fixture(sprite, pointer=0x30000):
    out=bytearray(struct.pack('<8I',0x31434252,2,128*1024*1024,0,0,0,264,1)+bytes(32))
    regs=[0]*16; regs[0]=pointer; regs[3]=0x20000
    local=[0]*32; local[30]=0x6DF00; local[31]=0x4B45524E
    page=bytearray(4096);struct.pack_into('<I',page,0,sprite)
    for kind in (1,2):
        out.extend(struct.pack('<4I',kind,0x11110,int(kind==1),100))
        out.extend(struct.pack('<16I8d34I',*regs,*([0.]*8),1,1,*local))
        if kind==1:
            out.extend(struct.pack('<I',0x30)+page)
            regs[0]=0xABC;regs[3]+=4
    out.extend(bytes(16));return out

with tempfile.TemporaryDirectory() as temp:
    p=Path(temp)/'sprite.rbc'
    def run(sprite,code,needle,pointer=0x30000):
        p.write_bytes(fixture(sprite,pointer))
        r=subprocess.run([sys.argv[1],str(p)],capture_output=True,text=True,timeout=30)
        assert r.returncode==code and needle in r.stdout+r.stderr,(r.returncode,r.stdout,r.stderr)
    for sprite in (0,693,829,29998,30001):
        run(sprite,0,'matched=1 blocked=0 diverged=0')
    for sprite in (29999,30000):
        run(sprite,1,'online_sprite_requires_host_state')
    run(0,1,'outside_guest_memory',128*1024*1024-2)
print('Native sprite delegation, custom Online asset rejection and pointer bounds verified.')
