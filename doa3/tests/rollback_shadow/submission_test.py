"""Real runtime ABI/private queue adapter; independent expected command digest."""
import struct, subprocess, sys, tempfile
from pathlib import Path
GPU_SIZE=33344
STATE_SIZE=4504
def make(bad=None):
    pages={}
    def put(a,v):
        struct.pack_into('<I',pages.setdefault(a//4096,bytearray(4096)),a%4096,v)
    dev=0x10000
    for offset,value in {0:0x4008,8:0,0x24:0x4000,0x28:0x4100,0x30:10,0x34:0x30000,0x2264:0xFD800000}.items(): put(dev+offset,value)
    method=0x130 if bad=='flip' else 0x200
    put(0x4000,(1<<18)|method);put(0x4004,123)
    regs=[0]*16;regs[1]=dev;regs[3]=0x20000
    locals_=[0]*32;locals_[30]=0x1E27C0;locals_[31]=0x4B45524E
    gpu=bytearray(GPU_SIZE);struct.pack_into('<II',gpu,0,4,0)
    seed=14695981039346656037
    struct.pack_into('<6I2Q',gpu,33040,0,0,0,0,1,1,0,seed)
    out=bytearray(struct.pack('<8I',0x31434252,5,128*1024*1024,0,0,0,STATE_SIZE+GPU_SIZE,1)+bytes(32)+struct.pack('<2I',0,GPU_SIZE))
    def record(kind):
        out.extend(struct.pack('<4I',kind,0x87420,len(pages),100))
        out.extend(struct.pack('<16I8d34I',*regs,*([0.]*8),1,1,*locals_)+bytes(4240))
        out.extend(gpu)
        for page,data in sorted(pages.items()):out.extend(struct.pack('<I',page)+data)
    record(1)
    regs[3]+=4;put(dev+0x2C,0x4008);put(0x30000,8)
    struct.pack_into('<II',gpu,8,0x4008,0x4008)
    digest=seed
    for b in struct.pack('<III',0,method,123):digest=((digest^b)*1099511628211)&0xFFFFFFFFFFFFFFFF
    if bad=='digest':digest^=1
    struct.pack_into('<6I2Q',gpu,33040,0x4008,0,0x4000,0x4104,1,1,1,digest)
    record(2);out.extend(bytes(16));return out
with tempfile.TemporaryDirectory() as temp:
    p=Path(temp)/'submission.rbc'
    def run(data,args,code,needle):
        p.write_bytes(data)
        r=subprocess.run([sys.argv[1],str(p),*args],capture_output=True,text=True,timeout=30)
        assert r.returncode==code and needle in r.stdout+r.stderr,(r.returncode,r.stdout,r.stderr)
    run(make(),[],1,'graphics_submission_requires_replay_profile')
    run(make(),['--submission-stream'],0,'matched=1 blocked=0 diverged=0')
    run(make('digest'),['--submission-stream'],1,'diverged=1')
    run(make('flip'),['--submission-stream'],1,'graphics_flip_requires_vblank_state')
    bad=make();struct.pack_into('<I',bad,72+16+STATE_SIZE+33040+20,0)
    run(bad,['--submission-stream'],1,'submission_trace_not_captured')
    run(make(),['--submission-stream','--submission-stream'],2,'Unknown or duplicate')
print('Submission ABI, cursor/fence effects, independent digest, strict default and flip rejection verified.')
