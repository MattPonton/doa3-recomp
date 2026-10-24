"""Pin/extract 3.1 timestamp bookkeeping; no presentation or waiting."""
from pathlib import Path
import hashlib
import re
import sys
from rollback_loop_input import extract
ROOT=Path(__file__).resolve().parents[2]
REVIEWED='b3ed05720f64b2c921277bf19f769de5e3d3d6910de3336958f68089248695e2'
def bodies():
    timing=(ROOT/'src/game/recomp/gen/recomp_0002.c').read_text()
    xapi=(ROOT/'src/game/recomp/gen/recomp_0006.c').read_text()
    manual=(ROOT/'src/game/recomp/recomp_manual.c').read_text()
    return {**{f'sub_{va:08X}_gen':extract(timing,f'sub_{va:08X}_gen') for va in (0xA65B0,0xA6680)},
            'sub_0018B744_gen':extract(xapi,'sub_0018B744_gen'),
            'doa3_guest_rdtsc':extract(manual.replace('uint64_t doa3_guest_rdtsc','void doa3_guest_rdtsc'),'doa3_guest_rdtsc')}
def digest(parts):
    text='\n'.join(name+'\n'+body for name,body in sorted(parts.items()))
    text=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
    return hashlib.sha256('\n'.join(x.strip() for x in text.splitlines() if x.strip()).encode()).hexdigest()
def generate(parts,fixture=False):
    if digest(parts)!=REVIEWED:raise ValueError('timing/XAPI/CPU clock bridge changed; review replay admission')
    prefix='reference_time_' if fixture else 'replay_time_'
    output=''
    for name in ('doa3_guest_rdtsc','sub_0018B744_gen','sub_000A65B0_gen','sub_000A6680_gen'):
        body=parts[name].replace('doa3_guest_rdtsc()',prefix+'rdtsc()').replace('sub_0018B744()',prefix+'0018B744()')
        targets=set(re.findall(r'\bgoto (loc_[0-9A-F]+);',body))
        body=re.sub(r'^(loc_[0-9A-F]+): ;',lambda m:m[0] if m[1] in targets else '/* '+m[1]+' */',body,flags=re.M)
        label='rdtsc' if name=='doa3_guest_rdtsc' else name[4:12]
        output+=f'static {"uint64_t" if label=="rdtsc" else "void"} {prefix}{label}(void){{{body}}}\n'
    if fixture:return output
    return '''/* Generated reviewed timestamp helpers. No live hook. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop_time.h"
#include "kernel/xbox_memory_layout.h"
#include "online/xbox_det.h"
#include <limits.h>
'''+output+'''
int doa3_loop_time_run(Doa3LoopState *s,Doa3InputTape *t){
    if(!doa3_loop_valid(s) || !doa3_input_tape_replay_ready(t) ||
       (s->pending!=0xA65B0 && s->pending!=0xA6680))return 0;
    size_t bytes=xbox_GetMemorySize();uint32_t sp=g_esp,routine=s->pending;
    if(!xbox_GetMemoryBase() || (ptrdiff_t)xbox_GetMemoryBase()!=g_xbox_mem_offset ||
       bytes<0x9B23B0 || bytes>128u*1024u*1024u || sp<64 || sp>bytes-4 || (sp&3) ||
       g_fp_top<INT_MIN+16 || g_fp_top>INT_MAX-16)return 0;
    uint32_t low=sp-24,high=sp+4;
    if((low<0x9B23B0 && high>0x9B2340) || (low<0x250EF8 && high>0x250EF4) ||
       (low<0x2511E0 && high>0x2511DC))return 0;
    if(routine==0xA6680 && MEM32(0x9B238C)>7)return 0;
    /* Both reviewed helpers read time only; no kernel dispatch, poll boost,
     * worker execution, clock advance, presentation or host pacing. */
    if(!xbox_det_checkpoint_acquire())return 0;
    if(routine==0xA65B0)replay_time_000A65B0();else replay_time_000A6680();
    int ok=g_esp==sp+4 && doa3_input_tape_replay_ready(t);
    if(!xbox_det_checkpoint_release())return -1;
    if(!ok)return -1;
    return doa3_loop_complete(s,routine) ? 1:-1;
}
'''
if __name__=='__main__':
    Path(sys.argv[1]).write_text(generate(bodies(),'--fixture' in sys.argv))
