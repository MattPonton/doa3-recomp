"""Bounded DirectSound service continuation; external effects are requests."""
from pathlib import Path
import hashlib
import re
import sys
from rollback_loop_input import extract
from rollback_audio_sync import verify as verify_sync
ROOT=Path(__file__).resolve().parents[2]
ROUTINES=(0x1F1F01,0x1F1358,0x1F1526,0x1F42B6,0x1F3C45,0x1F3C78,0x1F3C1B,0x1F1260,0x1F688C)
# Callback dispatch only. Stack cleanup reviewed at each actual callee return;
# the operations themselves are not admitted for replay by this generator.
VOICE_ARGS={0x1F64A8:4,0x1F66FE:4,0x1F6403:0,0x1F659E:0}
REVIEWED='5bacf9eddc49ad6e08add47f1f81953dd9938ec61ec90db55e19a93ab872cb10'
def bodies():
    source=(ROOT/'src/game/recomp/gen/recomp_0010.c').read_text()
    return {va:extract(source,f'sub_{va:08X}'+('' if va==0x1F688C else '_gen')) for va in ROUTINES}
def digest(parts):
    text='\n'.join(f'{va:X}\n{body}' for va,body in sorted(parts.items()))
    text=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
    return hashlib.sha256('\n'.join(x.strip() for x in text.splitlines() if x.strip()).encode()).hexdigest()
def transform(va,body):
    first=re.search(r'^loc_[0-9A-F]+: ;',body,re.M)
    declarations,code=body[:first.start()],body[first.start():]
    names=[]
    for line in declarations.splitlines():
        line=re.sub(r'/\*.*?\*/','',line).strip()
        if not line:continue
        m=re.fullmatch(r'(uint32_t|int) (.*);',line)
        if not m:raise ValueError('unknown audio local: '+line)
        for part in m[2].split(','):
            name,init=map(str.strip,part.split('='))
            if init not in ('0','g_seh_ebp'):raise ValueError('unknown initializer')
            names.append((m[1],name,init))
    if len(names)>10:raise ValueError('too many locals')
    decl=''.join(f'{typ} {name}=frame->resume ? ({typ})frame->locals[{i}] : {init};\n' for i,(typ,name,init) in enumerate(names))
    save=''.join(f'frame->locals[{i}]=(uint32_t){name};' for i,(_,name,_) in enumerate(names))
    points=[]
    # The two scoped ICALL temporaries are the only additional C locals.
    code=code.replace('uint32_t _icall_esp = g_esp;','frame->call_sp = g_esp;')
    code=re.sub(r'uint32_t _icall_t = ([^;]+);',r'frame->call_target = \1;',code)
    pattern=r'sub_([0-9A-F]{8})\(\);|RECOMP_ICALL_SAFE\(_icall_t, _icall_esp\);'
    def call(m):
        internal=int(m[1],16) if m[1] else 0
        pre=''
        if internal in VOICE_ARGS:
            pre=f'frame->call_target=0x{internal:X};frame->call_sp=g_esp+{4+VOICE_ARGS[internal]};'
            internal=0
        elif internal and internal not in ROUTINES:raise ValueError('unknown internal helper')
        points.append(internal);i=len(points)
        return pre+save+f'frame->resume={i};return (AudioAction){{{1 if internal else 2},{hex(internal) if internal else "frame->call_target"}}};\naudio_resume_{i}: ;'
    code=re.sub(pattern,call,code)
    code=re.sub(r'\breturn;', 'return (AudioAction){3,0};',code)
    if re.search(r'RECOMP_|sub_[0-9A-F]+\(|\b(?:uint32_t|int)\s',code):raise ValueError('unreviewed transfer/local')
    code=re.sub(r'^(loc_[0-9A-F]+): ;',r'\1: ; if(!audio_budget(r))return (AudioAction){0,0};',code,flags=re.M)
    for macro in ('MEM8','MEM32','PUSH32','POP32'):code=code.replace(macro+'(', 'AUDIO_'+macro+'(')
    dispatch='switch(frame->resume){case 0:break;'+''.join(f'case {i}:goto audio_resume_{i};' for i in range(1,len(points)+1))+'default:return (AudioAction){0,0};}\n'
    targets=set(re.findall(r'\bgoto (loc_[0-9A-F]+);',code))
    code=re.sub(r'^(loc_[0-9A-F]+): ;',lambda m:m[0] if m[1] in targets else '/* '+m[1]+' */',code,flags=re.M)
    return f'static AudioAction audio_{va:X}(AudioRuntime *r,Doa3AudioServiceFrame *frame){{\n{decl}{dispatch}{code}\n}}\n',points
def generate(parts,fixture=False):
    verify_sync()
    if digest(parts)!=REVIEWED:raise ValueError('audio service changed; review control flow/effects before regenerating')
    if fixture:
        out='\n'.join(f'static void reference_audio_{va:X}(void);' for va in ROUTINES)+'\n'
        for va,body in parts.items():
            for target in ROUTINES:body=body.replace(f'sub_{target:08X}()',f'reference_audio_{target:X}()')
            for target,args in VOICE_ARGS.items():
                body=body.replace(f'sub_{target:08X}();',f'reference_external(0x{target:X},g_esp+{4+args});')
            body=body.replace('RECOMP_ICALL_SAFE(','reference_external(')
            out+=f'static void reference_audio_{va:X}(void){{{body}}}\n'
        return out
    out='''/* Reviewed generated audio continuation. No live hook. */
#define RECOMP_GENERATED_CODE
#include "game/recomp/recomp_types.h"
#include "game/rollback_audio_service.h"
#include "kernel/xbox_memory_layout.h"
#include "online/xbox_det.h"
#include <string.h>
typedef struct {uint32_t kind,target;} AudioAction;
typedef struct {Doa3AudioService *s;unsigned char *memory;size_t bytes;uint32_t sink;} AudioRuntime;
static void *audio_memory(AudioRuntime *r,uint32_t address,unsigned bytes){
    if(r->s->fault || address>r->bytes-bytes){r->s->fault=DOA3_AUDIO_SERVICE_MEMORY;return &r->sink;}
    return r->memory+address;
}
static int audio_budget(AudioRuntime *r){
    if(r->s->fault)return 0;
    if(!r->s->remaining){r->s->fault=DOA3_AUDIO_SERVICE_EXHAUSTED;return 0;}
    --r->s->remaining;return 1;
}
#define AUDIO_MEM8(a) (*(volatile uint8_t *)audio_memory(r,(uint32_t)(a),1))
#define AUDIO_MEM32(a) (*(volatile uint32_t *)audio_memory(r,(uint32_t)(a),4))
#define AUDIO_PUSH32(sp,v) do{uint32_t value=(uint32_t)(v);(sp)-=4;AUDIO_MEM32(sp)=value;}while(0)
#define AUDIO_POP32(sp,v) do{(v)=AUDIO_MEM32(sp);(sp)+=4;}while(0)
'''
    points={}
    for va,body in parts.items():
        text,points[va]=transform(va,body);out+=text
    out+='static uint32_t audio_point(uint32_t routine,uint32_t resume){switch(routine){'
    for va,ps in points.items():
        out+=f'case 0x{va:X}:switch(resume){{case 0:return 1;'+''.join(f'case {i}:return {hex(v) if v else "2"};' for i,v in enumerate(ps,1))+'}break;'
    out+='}return 0;}\n'
    out+='static AudioAction audio_step(AudioRuntime *r,Doa3AudioServiceFrame *f){switch(f->routine){'+''.join(f'case 0x{va:X}:return audio_{va:X}(r,f);' for va in parts)+'}return (AudioAction){0,0};}\n'
    return out+(ROOT/'src/game/rollback_audio_service.inc').read_text()
if __name__=='__main__':
    Path(sys.argv[1]).write_text(generate(bodies(),'--fixture' in sys.argv))
