"""Extract the bounded input helpers; pin guest bodies AND XPP endpoints."""
from pathlib import Path
import hashlib
import re
import sys
ROOT=Path(__file__).resolve().parents[2]
HELPERS=(0xA4DD0,0xA50A0)
DECODER=(0xA4E70,0xA4E99,0xA4EA0)
ENDPOINTS=(0x21159F,0x211898,0x211823,0x2118A4,0x211A96,0x211B07)
REVIEWED='2b083bbf32fcad4acedc063fb3eb612fb6439ce49b5521d129adb15d83a9cfca'
def extract(source,name):
    start=source.index('void '+name+'(void)');begin=source.index('{',start);end=begin+1;depth=1
    while depth:
        depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[begin+1:end-1]
def bodies():
    source=(ROOT/'src/game/recomp/gen/recomp_0002.c').read_text()
    manual=(ROOT/'src/game/recomp/recomp_manual.c').read_text()
    guest={va:extract(source,f'sub_{va:08X}_gen') for va in HELPERS}
    guest.update({va:extract(source,f'sub_{va:08X}') for va in DECODER})
    return guest, {va:extract(manual,f'sub_{va:08X}') for va in ENDPOINTS}
def digest(guest,bridges):
    text='\n'.join(f'{va:X}\n{body}' for va,body in sorted({**guest,**bridges}.items()))
    text=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
    return hashlib.sha256('\n'.join(x.strip() for x in text.splitlines() if x.strip()).encode()).hexdigest()
def generate(guest,bridges,fixture=False):
    if digest(guest,bridges)!=REVIEWED: raise ValueError('input helper/XPP body changed; re-review replay admission')
    prefix='reference_input_' if fixture else 'replay_input_'
    output='\n'.join(f'void sub_{va:08X}(void);' for va in ENDPOINTS)+'\n'
    output+='\n'.join(f'static void {prefix}{va:X}(void);' for va in DECODER)+'\n'
    for va in HELPERS+DECODER:
        body=guest[va]
        for target in DECODER:
            body=body.replace(f'sub_{target:08X}()',f'{prefix}{target:X}()')
        targets=set(re.findall(r'\bgoto (loc_[0-9A-F]+);',body))
        body=re.sub(r'^(loc_[0-9A-F]+): ;',lambda m:m[0] if m[1] in targets else '/* '+m[1]+' */',body,flags=re.M)
        output+=f'static void {prefix}{va:X}(void){{{body}}}\n'
    if fixture:return output
    return '''/* Generated reviewed input-helper bodies. No live hook. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop_input.h"
#include "kernel/xbox_memory_layout.h"
'''+output+'''
/* 3.1 mapping tables: 8 digital, 6 low-threshold and 2 high-threshold
 * entries, each followed by a zero-mask sentinel. Bound the guest loops
 * before any write; analog indices may only address the eight pad bytes.
 * Shorter/custom tables are safe if they terminate within these extents. */
static int replay_input_table(uint32_t base,unsigned slots,int analog){
    for(unsigned i=0;i<slots;++i){
        uint32_t entry=base+i*8;
        if(!MEM32(entry+4))return 1;
        if(analog && MEM32(entry)>=8)return 0;
    }
    return 0;
}
static int replay_input_decoder_ready(uint32_t sp){
    /* Includes scratch pushes and return slot. Prevent writes from changing
     * a validated table, port-loop counter, saved registers or pad outputs.
     * The enclosing trial must exclude all concurrent guest-memory writers. */
    uint32_t low=sp-24,high=sp+4;
    if((low<0x2D4CE8 && high>0x2D4C50) ||
       (low<0x73C9AC && high>0x73C6C8))return 0;
    return replay_input_table(0x2D4C50,9,0) &&
           replay_input_table(0x2D4C98,7,1) &&
           replay_input_table(0x2D4CD0,3,1);
}
int doa3_loop_input_run(Doa3LoopState *s,Doa3InputTape *t){
    if(!doa3_loop_valid(s) || !doa3_input_tape_replay_ready(t) ||
       (s->pending!=0xA4DD0 && s->pending!=0xA4E70 && s->pending!=0xA50A0))return 0;
    /* Guest stack must contain the return slot and room for all fixed pushes.
     * Pad/capability/feedback destinations are constants. */
    size_t bytes=xbox_GetMemorySize();uint32_t sp=g_esp,routine=s->pending;
    if(!xbox_GetMemoryBase() || (ptrdiff_t)xbox_GetMemoryBase()!=g_xbox_mem_offset ||
       bytes<0x73CA00 || bytes>128u*1024u*1024u || sp<64 || sp>bytes-4 || (sp&3))return 0;
    if(routine==0xA4E70){
        if(!replay_input_decoder_ready(sp))return 0;
        replay_input_A4E70();
    }else if(routine==0xA4DD0)replay_input_A4DD0();else replay_input_A50A0();
    if(g_esp!=sp+4 || !doa3_input_tape_replay_ready(t))return -1;
    return doa3_loop_complete(s,routine) ? 1:-1;
}
'''
if __name__=='__main__':
    guest,bridges=bodies()
    Path(sys.argv[1]).write_text(generate(guest,bridges,'--fixture' in sys.argv))
