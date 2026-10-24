"""Exercise the real runtime wrapper and private captured target table."""
import struct, subprocess, sys, tempfile
from pathlib import Path
GPU_SIZE=33344
STATE_SIZE=4504

def make(bad=None):
    pages={}
    def put(a,v): struct.pack_into('<I',pages.setdefault(a//4096,bytearray(4096)),a%4096,v)
    dev=0x10000
    put(0x1EDE80,dev);put(dev+0x2070,0x6000);put(dev+0x207C,0x5000)
    put(0x5004,0x8000);put(0x4004,0x84009000);put(0x6004,0xA000)
    put(0x20004,0);put(0x20008,0x4000)
    regs=[0]*16;regs[1]=0xBAD;regs[3]=0x20000
    locals_=[0]*32;locals_[30]=0x1DC8E0;locals_[31]=0x4B45524E
    gpu=bytearray(GPU_SIZE);struct.pack_into('<II',gpu,0,4,0)
    struct.pack_into('<5I',gpu,33080,3,0,0x8000,0x8000,0x7000)
    out=bytearray(struct.pack('<8I',0x31434252,5,128*1024*1024,0,0,0,STATE_SIZE+GPU_SIZE,1)+bytes(32)+struct.pack('<2I',0,GPU_SIZE))
    def record(kind):
        out.extend(struct.pack('<4I',kind,0x87420,len(pages),100))
        out.extend(struct.pack('<16I8d34I',*regs,*([0.]*8),1,1,*locals_)+bytes(4240))
        out.extend(gpu)
        for page,data in sorted(pages.items()):out.extend(struct.pack('<I',page)+data)
    record(1)
    regs[0]=0x1234;regs[1]=0;regs[3]+=12;put(dev+0x2070,0x4000)
    struct.pack_into('<5I',gpu,33080,2,0,0x7000,0x9000,0x7000)
    if bad=='state':struct.pack_into('<I',gpu,33080+12,0xA000)
    record(2);out.extend(bytes(16));return out

with tempfile.TemporaryDirectory() as temp:
    p=Path(temp)/'targets.rbc'
    def run(data,args,code,needle):
        p.write_bytes(data)
        r=subprocess.run([sys.argv[1],str(p),*args],capture_output=True,text=True,timeout=30)
        assert r.returncode==code and needle in r.stdout+r.stderr,(r.returncode,r.stdout,r.stderr)
    run(make(),[],1,'render_target_requires_replay_profile')
    run(make(),['--submission-stream'],0,'matched=1 blocked=0 diverged=0')
    run(make('state'),['--submission-stream'],1,'diverged=1')
    bad=make();struct.pack_into('<I',bad,72+16+STATE_SIZE+33080,65)
    run(bad,['--submission-stream'],1,'invalid_gpu_snapshot')
print('Target wrapper ABI, post-body fallback, captured routing, repeatability and strict guards verified.')
