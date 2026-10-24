"""Generate a non-dispatching 3.1 outer-loop continuation from the actual lift.

The reviewed body is pinned, including every branch, stack effect and helper.
No live hook is installed. The caller must separately implement safe helper
admission/execution; returning CALL is not permission to call arbitrary code.
"""
from pathlib import Path
import hashlib
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
CALLS = (0xA65B0,0xA4DD0,0xA4E70,0xA50A0,0x1F1F01,0x1DC660,
         0x1DF010,0xBE2D0,0x17BAD0,0x180490,0x18BDD1,0x1C650C,
         0x1DC740,0xA5F90,0x1C76A0,0x1C6EF0,0x1C7700,0xA6680,
         0xBE2E0,0x1E14D0,0x17BCE0)
# Updated only after reviewing changes to the lifted outer-loop control flow.
REVIEWED = '349b0166a61d5aa9aee049c58b6bb66907a9b1958337557dce0eadab973ee743'

def body_from(source):
    match = re.search(r'^void sub_000A6450\(void\)\s*\{(.*?)^\}',source,re.M|re.S)
    if not match: raise ValueError('missing 3.1 outer loop')
    return match[1]

def clean(body):
    body = re.sub(r'^    (?:extern void doa3_rollback_frame_phase\(uint32_t address\);|doa3_rollback_frame_phase\(0x[0-9A-F]+u\);)\s*$', '',body,flags=re.M)
    body = re.sub(r'/\*.*?\*/|//[^\n]*','',body,flags=re.S)
    return '\n'.join(line.strip() for line in body.splitlines() if line.strip())

def generate(body):
    if hashlib.sha256(clean(body).encode()).hexdigest()!=REVIEWED:
        raise ValueError('3.1 outer loop changed; review helpers/branches/ABI before regenerating')
    found=tuple(int(x,16) for x in re.findall(r'sub_([0-9A-F]{8})\(\);',body))
    if found!=CALLS: raise ValueError('outer-loop helper list changed')
    code=clean(body)
    # The five flag locals are retained across every returned call. x87 and
    # integer registers remain in the caller-owned recompiler register bank.
    code=code.replace('uint32_t _fs0 = 0, _fs1 = 0, _fs2 = 0, _fs3 = 0;',
        'uint32_t _fs0=s->locals[0],_fs1=s->locals[1],_fs2=s->locals[2],_fs3=s->locals[3];')
    code=code.replace('int _flags = 0;', 'int _flags=(int)s->locals[4];')
    save='s->locals[0]=_fs0;s->locals[1]=_fs1;s->locals[2]=_fs2;s->locals[3]=_fs3;s->locals[4]=(uint32_t)_flags;'
    dispatch='switch(s->resume){case 0:goto interval_start;'+''.join(
        f'case {i}:goto resume_{i};' for i in range(1,len(CALLS)+1))+'default:return 0;}\n'
    code=code.replace('loc_000A6450: ;',dispatch+'loc_000A6450: ;',1)
    code=code.replace('loc_000A64B0: ;', 'loc_000A64B0: ;\n'+save+
        '\ns->resume=0;s->calls=0;++s->intervals;*out=(Doa3LoopAction){DOA3_LOOP_BOUNDARY,0};return 1;\ninterval_start: ;')
    for i,va in enumerate(CALLS,1):
        code=code.replace(f'sub_{va:08X}();', save+
            f'\ns->resume={i};s->pending=0x{va:X};++s->calls;'
            f'*out=(Doa3LoopAction){{DOA3_LOOP_CALL,0x{va:X}}};return 1;\nresume_{i}: ;',1)
    # Preserve address annotations without producing unused-label warnings.
    targets=set(re.findall(r'\bgoto (loc_[0-9A-F]+);',code))
    code=re.sub(r'^(loc_[0-9A-F]+): ;',lambda m: m[0] if m[1] in targets
                else '/* '+m[1]+' */',code,flags=re.M)
    return '''/* Generated from reviewed 3.1 A6450. Do not edit. No live hook. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_loop.h"
#include <string.h>
static const uint32_t routines[]={0,'''+','.join(hex(x) for x in CALLS)+'''};
void doa3_loop_init(Doa3LoopState *s){if(s){memset(s,0,sizeof(*s));s->schema=DOA3_LOOP_SCHEMA;}}
int doa3_loop_valid(const Doa3LoopState *s){
    return s && s->schema==DOA3_LOOP_SCHEMA && !s->reserved &&
        s->resume<sizeof(routines)/sizeof(routines[0]) && s->calls<=DOA3_LOOP_MAX_CALLS &&
        (s->resume ? s->calls>0 : !s->calls && !s->pending) &&
        (!s->pending || s->pending==routines[s->resume]);
}
int doa3_loop_at_boundary(const Doa3LoopState *s){return doa3_loop_valid(s) && !s->resume;}
int doa3_loop_complete(Doa3LoopState *s,uint32_t routine){
    if(!doa3_loop_valid(s) || !routine || s->pending!=routine)return 0;
    s->pending=0;return 1;
}
int doa3_loop_next(Doa3LoopState *s,Doa3LoopAction *out){
    if(!out || !doa3_loop_valid(s) || s->pending || s->calls>=DOA3_LOOP_MAX_CALLS ||
       s->intervals==UINT64_MAX)return 0;
'''+code+'\n}\n'

if __name__=='__main__':
    body=body_from((ROOT/'src/game/recomp/gen/recomp_0002.c').read_text())
    Path(sys.argv[1]).write_text(generate(body))
