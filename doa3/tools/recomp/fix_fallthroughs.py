import re, glob, os, sys
sys.path.insert(0, os.getcwd())
from tools import xbe_layout as _L
# addr -> emitted function name (prefer base name; gen may have _gen suffix)
addr2name = {}
for f in glob.glob('src/game/recomp/gen/*.c'):
    for line in open(f, encoding='utf-8', errors='ignore'):
        m = re.match(r'void (sub_([0-9A-Fa-f]{8}))(_gen)?\(void\)', line)
        if m:
            a=int(m.group(2),16)
            # base name (override or gen) is callable
            addr2name[a]=m.group(1)

LO, HI = _L.TEXT_VA_START, _L.TEXT_VA_END   # all of .text (3.0: 0x11000-0x1B0DE0, as upstream had it). NOTE: XDK libs also have ~225
# dropped fall-throughs but blanket-fixing them BREAKS boot (some XDK 'ends'
# are not real fall-throughs); apply targeted ones via KEEP_XDK below.
# KEEP_XDK: XDK-range functions PROVEN to need their fall-through restored:
#  0x1B4611 - D3D SetStateBlockFlags je-path (leaked 24B stack/call)
#  0x1C3FDD - D3DX draw helper 'hr=0' path (fell off the end: leaked ~0xA4/call,
#             swept esp through the whole address space)
#  0x1B1388 - D3DDevice_SetRenderTarget's linear-surface width branch. The
#             detector split 0x1B1388-0x1B1396 (5 insns, no terminator) out of
#             sub_001B1350 and dropped its fall-through into 0x001B1397, so
#             SetRenderTarget returned after 5 instructions whenever the target
#             carried a packed size at surface+0x10 (which the implicit 720x480
#             back buffer does). That skipped `MEM32(dev+0x40C) = ebp` and the
#             tail viewport reset, so the device held no colour surface,
#             SetViewport clamped 720x480 to 1x1, the projection-viewport matrix
#             at dev+0x5A0 stayed all zero and the composite matrix reached the
#             GPU as 16 zero dwords.
#  0x1B18C9 - D3DDevice_SetViewport's "render target is not the implicit
#             surface" branch (2 insns, falls into 0x001B18CF). Dropped, the
#             whole viewport clamp/store never ran for a non-implicit target.
#  0x1BB96C - the `or eax,1` fragment inside D3DDevice_CreateDevice's surface
#             setup (falls into 0x001BB96F). Dropped, the implicit surface
#             descriptors at device+0x2168/+0x2180/+0x2198 came back as
#             fmt=3 size=0, so SetViewport clamped every viewport to 1x1.
#             Needs the five ESP_FIX contracts, the drained-pusher pfifo_read,
#             the NV2A USER block and the KickOff notifier guard alongside it.
#  0x1B60A3 - D3D_UpdateProjectionViewportTransform's non-orthographic branch
#             (2 fld's, falls into 0x001B60AD). Dropped, the whole matrix build
#             and the sub_001B7E50 multiply into device+0x5A0 never ran for
#             transform mode != 2.
#  0x1E6774 - XAPI XInputGetDeviceChanges' device-query body. Its fall-through
#             into the shared epilogue at 0x001E683E (pop edi/esi/ebx; leave;
#             ret 0xC) was dropped, so it returned 176 bytes low and never
#             popped its 16. The game main loop calls it once a frame through
#             sub_0009DC40 -> sub_0009DB40 -> sub_001E6736, which leaked
#             exactly 192 bytes of guest stack per frame: after ~11k frames
#             esp had walked 2.3 MB down, off the stack base and into the
#             game's own data, where it overwrote the mwPly pool count/base at
#             0xC0E514/0xC0E518 with a D3D device pointer. sub_0017CC30 then
#             swept memory with a 1.8M-iteration scan and the process died on
#             the VEH fault-skip cap moments after the intro movie.
KEEP_XDK = {0x1B4611, 0x1C3FDD, 0x1B1388, 0x1B18C9, 0x1BB96C, 0x1B60A3, 0x1E6774}
if _L.VERSION != "3.0":
    KEEP_XDK = set()   # 3.0 addresses; re-derive per build as they are proven
# NOTE: 0x1C883B (DSOUND stream-service fragment) is NOT a real fall-through -
# restoring it caused infinite recursion (native stack overflow). Its -4
# esp/call is contained by an ESP_FIX wrapper in recomp_manual.c instead.
# The whole D3DX section (0x1C35C0-0x1C64A0) is fall-through-safe: its
# fragments are real split epilogues (Initialize leaked its 0xD8 frame per
# call via sub_001C3ED7's dropped fall-through). The boot-breaking false
# fall-throughs were in the D3D GPU-init code, which stays excluded.
_d3dx = _L.section("D3DX")
D3DX_LO, D3DX_HI = _d3dx[1], _d3dx[1] + _d3dx[2]   # 3.0: 0x1C35C0-0x1C64A0
# Was 0x1B0DE0 (.text only) - the XDK D3D/DSOUND libs had ~225 dropped
# fall-throughs too; one (sub_001B4611, in D3D SetStateBlock-flags) leaked
# 24 bytes of stack per call and made the D3DX state-block create loop run
# forever (131K 512-byte allocations -> OOM). 2026-07-02.
# Disassembly guard (every build but 3.0, which keeps upstream's behaviour).
# Seeding splits functions at branch targets, so a fresh build has far more
# fragments than upstream's hand-curated 3.0 tree; check each candidate
# against the original code before restoring it.
_skipped = {}
def _real_fallthrough(start, end):
    """True if the code in [start, end) really continues into `end`."""
    if _L.VERSION == "3.0":
        return True
    if not hasattr(_real_fallthrough, "md"):
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        import json
        _real_fallthrough.md = Cs(CS_ARCH_X86, CS_MODE_32)
        _real_fallthrough.data = open(_L.XBE_PATH, "rb").read()
        fj = json.load(open(os.path.join("tools", "disasm", "output", "functions.json")))
        _real_fallthrough.ends = {int(e["start"], 16): int(e["end"], 16) for e in fj}
    md, data, ends = _real_fallthrough.md, _real_fallthrough.data, _real_fallthrough.ends
    def raw(va):
        for nm, sva, _vs, sraw, srs, _fl in _L.LAYOUT["sections"]:
            if sva <= va < sva + srs:
                return sraw + va - sva
        return None
    def why(reason):
        _skipped[reason] = _skipped.get(reason, 0) + 1
        return False
    o = raw(start)
    if o is None:
        return why("outside image")
    ins = list(md.disasm(data[o:o + (end - start) + 16], start))
    body = [i for i in ins if i.address < end]
    if not body:
        return why("no decodable code")
    last = body[-1]
    if last.address + last.size != end:
        return why("not contiguous (data decoded as code)")
    if last.mnemonic in ("ret", "retn", "jmp", "int3", "hlt", "ud2"):
        return why("ends in " + last.mnemonic)
    nxt = [i for i in ins if i.address == end]
    if nxt and (nxt[0].mnemonic in ("nop", "int3") or nxt[0].bytes == b"\x00\x00"):
        return why("falls into padding")
    if last.mnemonic == "call" and last.op_str.startswith("0x"):
        tgt = int(last.op_str, 16)
        te = ends.get(tgt)
        to = raw(tgt)
        if te and to is not None:
            callee = list(md.disasm(data[to:to + (te - tgt)], tgt))
            if not any(c.mnemonic in ("ret", "retn") for c in callee):
                return why("ends in call to a function with no ret (noreturn)")
    return True

fixed = 0
for f in glob.glob('src/game/recomp/gen/*.c'):
    lines = open(f, encoding='utf-8', errors='ignore').read().split('\n')
    out=[]; cur=None; end=None; has_ebp=False; body=[]; start_idx=None
    pend=None
    i=0
    while i < len(lines):
        line=lines[i]
        mo=re.search(r'Original: 0x([0-9A-Fa-f]+) - 0x([0-9A-Fa-f]+)', line)
        if mo: pend=int(mo.group(2),16)
        m=re.match(r'void sub_([0-9A-Fa-f]{8})(_gen)?\(void\)', line)
        if m:
            cur=int(m.group(1),16); end=pend; has_ebp=False; body=[]
            out.append(line); i+=1
            continue
        if cur is not None:
            if 'uint32_t ebp;' in line: has_ebp=True
            if line=='}':
                # decide fall-through
                last=''
                for b in reversed(body):
                    s=b.strip()
                    if not s or s.startswith('/*') or s.endswith(': ;') or s in ('{','};') or s.startswith('#') or s.startswith('loc_') or s.startswith('uint') or s.startswith('int '):
                        continue
                    last=s; break
                # A conditional tail-call (`if (...) { ...; return; }`) is a jcc:
                # the not-taken path still falls into the next function.
                term = (('return' in last and not last.startswith('if ')) or
                        last.startswith('goto ') or 'int3' in last or last.endswith('break;'))
                if (not last or not term) and end in addr2name and (LO<=cur<HI or cur in KEEP_XDK or D3DX_LO<=cur<D3DX_HI) and _real_fallthrough(cur, end):
                    tgt=addr2name[end]
                    if has_ebp:
                        ins='    g_seh_ebp = ebp; %s(); return; /* DOA3: restored dropped fall-through to %s */' % (tgt,tgt)
                    else:
                        ins='    %s(); return; /* DOA3: restored dropped fall-through to %s */' % (tgt,tgt)
                    if os.environ.get('FIX_DRYRUN'): print('%s: sub_%08X -> %s | %s' % (os.path.basename(f), cur, tgt, last[:100]))
                    else: out.append(ins)
                    fixed+=1
                out.append(line); cur=None; i+=1; continue
            body.append(line)
        out.append(line); i+=1
    if not os.environ.get('FIX_DRYRUN'): open(f,'w',encoding='utf-8').write('\n'.join(out))
print("CRI-range fall-throughs fixed:", fixed)
for k, v in sorted(_skipped.items(), key=lambda kv: -kv[1]):
    print(f"  skipped {v}: {k}")
