"""Exercise carried continuations, external writes, and checkpoint rewind."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0,str(Path(__file__).resolve().parents[2]/"tools/recomp"))
from rollback_tasks import TASKS
apu=b""
gpu_mode=len(sys.argv)>2 and sys.argv[2]=="--gpu"
nested_mode=gpu_mode or (len(sys.argv)>2 and sys.argv[2]=="--nested")
GPU_BYTES=33344


def fixture(bad=None):
    pages={}; prior={}; states={}; total=0; gpu_total=0; pfifo_total=0
    nested_bytes=16+16*264 if nested_mode else 0
    output=bytearray(struct.pack("<8I",0x31434252,5 if gpu_mode else 4 if nested_mode else 3 if apu else 2,128*1024*1024,0,0,0,264+nested_bytes+len(apu)+(GPU_BYTES if gpu_mode else 0),len(TASKS)*6)+bytes(32))
    if gpu_mode: output.extend(struct.pack("<2I",len(apu),GPU_BYTES))

    def put(address,value):
        page=pages.setdefault(address//4096,bytearray(4096))
        struct.pack_into("<I",page,address%4096,value)

    def record(kind,va,frame,regs,fp,local):
        changes={p:bytes(data) for p,data in pages.items() if prior.get(p)!=bytes(data)}
        output.extend(struct.pack("<4I",kind,va,len(changes),frame))
        output.extend(struct.pack("<16I8d34I",*regs,*fp,1,1,local,*([0]*31)))
        if nested_mode:
            child=[0]*64; child[40]=local
            if bad=="nested_result" and kind==2 and n==10: child[40]^=1
            if bad=="nested_gap" and kind==1 and n==len(TASKS): child[40]^=1
            output.extend(struct.pack("<4I",2,2,2,0))
            output.extend(struct.pack("<66I",va,2,*([0]*64)))
            output.extend(struct.pack("<66I",0x85820,1,*child))
            output.extend(bytes(14*264))
        if apu:
            payload=bytearray(apu)
            # CommandImage v1 begins with four u32 fields, then clock.base.
            # This clock changes between tasks as a recorded external input.
            clock=frame*100+regs[8]
            if bad=="apu_result" and kind==2 and n==10: clock+=1
            struct.pack_into("<Q",payload,16,clock)
            output.extend(payload)
        if gpu_mode:
            values=[gpu_total,gpu_total]+[0]*60+[0,regs[8]]
            if bad=="gpu_result" and kind==2 and n==10: values[1]^=1
            output.extend(struct.pack("<66I",4,0,*values))
            pfifo=[0]*0x2000; pfifo[0x124C]=pfifo_total
            pfifo[0x1FFF]=regs[8]^0x1234
            if bad=="pfifo_result" and kind==2 and n==10: pfifo[0x124C]^=1
            output.extend(struct.pack("<8194I",regs[8],3,*pfifo))
            output.extend(bytes(40+264))
        for page,data in sorted(changes.items()):
            output.extend(struct.pack("<I",page)+data)
        prior.update(changes)

    for n in range(len(TASKS)*6):
        va=TASKS[n%len(TASKS)]
        regs,fp,local=states.get(va,([0]*16,[0.0]*8,0))
        regs=regs.copy(); fp=fp.copy()
        regs[8]=n*7  # shared flag/string state supplied by the external boundary
        put(0x60000,n+1)  # explicit external input, not a task output
        frame=100+n//len(TASKS)
        if bad=="frame" and n>=len(TASKS): frame+=1
        if bad=="continuation" and n==len(TASKS): regs[0]^=1
        record(1,va,frame,regs,fp,local)
        local=(local+n+1+va)&0xFFFFFFFF
        gpu_total=(gpu_total+local)&0xFFFFFFFF
        pfifo_total=(pfifo_total*17+local)&0xFFFFFFFF
        total=(total*33+local)&0xFFFFFFFF
        put(0x50000,total)
        regs[0]=local; fp[0]=float(local)
        if bad=="result" and n==10: put(0x70000,1)
        record(2,va,frame,regs,fp,local)
        states[va]=(regs,fp,local)
    output.extend(bytes(16))
    return output


with tempfile.TemporaryDirectory() as directory:
    path=Path(directory)/"chain.rbc"
    if len(sys.argv)>2 and not nested_mode:
        blob=Path(directory)/"apu.bin"
        subprocess.run([sys.argv[2],"--snapshot",str(blob)],check=True,capture_output=True,timeout=30)
        apu=blob.read_bytes()
    def run(data,code,message):
        path.write_bytes(data)
        result=subprocess.run([sys.argv[1],str(path),"--chain"],capture_output=True,text=True,timeout=90)
        assert result.returncode==code,(result.returncode,result.stdout,result.stderr)
        assert message in result.stdout+result.stderr,(result.stdout,result.stderr)
    midpoint=len(TASKS)*3
    run(fixture(),0,f"checkpoint_after={midpoint} restored_steps={midpoint}")
    if nested_mode:
        run(fixture("nested_result"),1,"CHAIN DIFFERENT")
        run(fixture("nested_gap"),1,"continuation gap")
    if gpu_mode:
        run(fixture("gpu_result"),1,"CHAIN DIFFERENT")
        run(fixture("pfifo_result"),1,"CHAIN DIFFERENT")
        bad=fixture(); struct.pack_into("<I",bad,68,GPU_BYTES+1)
        run(bad,2,"Invalid capture payload sizes")
        run(fixture()[:71],2,"Invalid capture payload sizes")
        bad=fixture(); struct.pack_into("<I",bad,72+16+4504,1)
        run(bad,1,"invalid_gpu_snapshot")
        # GPU payload truncation before the first page delta is rejected.
        run(fixture()[:72+16+4504+GPU_BYTES-1],2,"nonconsecutive")
    if apu:
        run(fixture("apu_result"),1,"CHAIN DIFFERENT")
    else:
        run(fixture("continuation"),1,"continuation gap")
        run(fixture("frame"),2,"nonconsecutive")
        run(fixture("result"),1,"CHAIN DIFFERENT")
        run(fixture()[:-16],2,"nonconsecutive")
print("Chain checkpoint rewind, carried task state, external inputs, and missing/corrupt updates verified.")
