"""Compile actual production queue/parser checkpoint adapters in isolation."""
from pathlib import Path
import re,sys
root=Path(__file__).resolve().parents[2]
out=[]
for path,names in (
 ('src/nv2a/nv2a_core.c',('nv2a_rollback_queue_capture','nv2a_rollback_queue_validate_live','nv2a_rollback_queue_restore')),
 ('src/game/recomp/recomp_manual.c',('doa3_gpu_frontend_capture','doa3_gpu_frontend_validate','doa3_gpu_frontend_restore'))):
    text=(root/path).read_text(encoding='utf-8')
    for name in names:
        m=re.search(r'\bint '+name+r'\([^)]*\)\s*\{',text)
        if not m:raise ValueError(name)
        end=m.end();depth=1
        while depth:
            depth+=(text[end]=='{')-(text[end]=='}');end+=1
        out.append(text[m.start():end])
Path(sys.argv[1]).write_text('\n'.join(out),encoding='utf-8')
