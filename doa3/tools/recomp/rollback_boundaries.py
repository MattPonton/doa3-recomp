"""Read-only markers in the audited 3.1 outer loop; no instruction replacement."""
import re
import hashlib

def patch_audio_wait(source):
    """Keep the native CRI stop/status/deadline flow; progress its frozen clock."""
    match=re.search(r"^void sub_00199DB0(?:_gen)?\(void\)\n\{.*?^\}",source,re.M|re.S)
    if not match:raise ValueError("missing CRI voice-stop wait 199DB0")
    declaration="    extern int xbox_det_audio_wait_poll(void);\n"
    call="    (void)xbox_det_audio_wait_poll();\n"
    body=match[0].replace(declaration,"").replace(call,"")
    checked=re.sub(r'/\*.*?\*/|//[^\n]*','',body,flags=re.S).replace('sub_00199DB0_gen','sub_00199DB0')
    digest=hashlib.sha256('\n'.join(x.strip() for x in checked.splitlines() if x.strip()).encode()).hexdigest()
    if digest!='d32a663f49b02fdade17eb0073ac63f83ef5acf98d907269b1d123ca1f8f46e5':
        raise ValueError("CRI voice-stop wait changed; review status/deadline semantics")
    label="loc_00199E20: ;"
    if body.count(label)!=1 or body.count("goto loc_00199E20;")!=2 or "sub_001F1E79();" not in body:
        raise ValueError("CRI voice-stop wait control flow changed")
    body=body.replace("{\n","{\n"+declaration,1).replace(label,label+"\n"+call.rstrip())
    return source[:match.start()]+body+source[match.end():]

# Execution order from dispatcher return to the next dispatcher call.
PHASES = (0xA64BA,0xA64BF,0xA64C9,0xA6514,0xA6519,0xA651E,0xA6525,0xA652A,
          0xA6450,0xA6455,0xA645A,0xA645F,0xA6464,0xA6469,0xA6477,0xA6492,
          0xA64A1,0xA64A6,0xA64AB,0xA64B0)

def patch(source):
    match = re.search(r"^void sub_000A6450(?:_gen)?\(void\)\n\{.*?^\}",source,re.M|re.S)
    if not match:
        raise ValueError("missing 3.1 outer loop A6450")
    body=match[0]
    declaration="    extern void doa3_rollback_frame_phase(uint32_t address);\n"
    body=body.replace(declaration,"")
    body=re.sub(r"^    doa3_rollback_frame_phase\(0x[0-9A-F]+u\);\n","",body,flags=re.M)
    for va in PHASES:
        label=f"loc_{va:08X}: ;"
        if body.count(label)!=1:
            raise ValueError(f"outer-loop boundary {va:08X} changed")
        body=body.replace(label,label+f"\n    doa3_rollback_frame_phase(0x{va:X}u);")
    body=body.replace("{\n","{\n"+declaration,1)
    return source[:match.start()]+body+source[match.end():]
