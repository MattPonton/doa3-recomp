"""Pin and expose the actual HAL/RTL synchronization endpoints for testing."""
from pathlib import Path
import hashlib
import re
import sys
ROOT=Path(__file__).resolve().parents[2]
REVIEWED='5cfa39975ad695b6acfd6e87c17bbbdf111d20f1721d28aa026ffbccafb3b7f0'
FUNCTIONS={
 'kernel_hal.c':[
  ('static void irql_mirror(void)','irql_mirror'),
  ('KIRQL xbox_current_irql(void)','xbox_current_irql'),
  ('KIRQL xbox_set_irql(KIRQL irql)','xbox_set_irql'),
  ('KIRQL __fastcall xbox_KfRaiseIrql(KIRQL NewIrql)','xbox_KfRaiseIrql'),
  ('VOID __fastcall xbox_KfLowerIrql(KIRQL NewIrql)','xbox_KfLowerIrql')],
 'kernel_rtl.c':[
  ('VOID __stdcall xbox_RtlEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)','xbox_RtlEnterCriticalSection'),
  ('VOID __stdcall xbox_RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)','xbox_RtlLeaveCriticalSection')],
 'kernel_bridge.c':[(f'static void bridge_{name}(void)',f'bridge_{name}') for name in
                    ('KfRaiseIrql','KfLowerIrql','RtlEnterCriticalSection','RtlLeaveCriticalSection')]}
def source():
    parts=[]
    for file,functions in FUNCTIONS.items():
        text=(ROOT/'src/kernel'/file).read_text(encoding='utf-8')
        for declaration,name in functions:
            match=re.search(r'\b'+name+r'\([^)]*\)\s*\{',text)
            if not match:raise ValueError('missing synchronization function '+name)
            start=match.end();end=start;depth=1
            while depth:
                depth+=(text[end]=='{')-(text[end]=='}');end+=1
            parts.append(declaration+'{'+text[start:end-1]+'}\n')
    return ''.join(parts)
def digest(text):
    text=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
    return hashlib.sha256('\n'.join(x.strip() for x in text.splitlines() if x.strip()).encode()).hexdigest()
def verify():
    text=source()
    if digest(text)!=REVIEWED:raise ValueError('audio synchronization endpoints changed; review host effects before replay')
    return text
if __name__=='__main__':
    Path(sys.argv[1]).write_text('static __declspec(thread) KIRQL g_current_irql=0;\n'+verify())
