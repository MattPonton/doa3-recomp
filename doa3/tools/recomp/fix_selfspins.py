"""DOA3 gen fix: force hardware busy-wait self-spins through.

The XDK D3D/DSOUND/XPP libraries poll GPU/APU registers with single-instruction
self-loops (`loc_X: if (MEM...) goto loc_X;`). Our port has no live hardware
behind most of those registers (VEH zero-pages them), so the value never
changes and the loop hangs the game (e.g. DirectSoundDoWork's DSP-FIFO drain
at loc_001C92D1 on 3.0). On the host the condition these gates model
("hardware ready / FIFO drained") is always satisfiable, so drop the loops.

Only functions in the XDK library sections are touched; game .text keeps its
loops. Upstream selected recomp_0010/0011.c, which held those sections in its
3.0 split; this selects by address so it works for any build and split.
Re-apply after regen.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.getcwd())
from tools import xbe_layout as L  # noqa: E402

XDK = [(va, va + vs) for name, va, vs in L.EXECUTABLE_SECTIONS
       if name not in (".text", "DOLBY")]
FUNC = re.compile(r'^void sub_([0-9A-Fa-f]{8})(?:_gen)?\(void\)')


def in_xdk(a):
    return any(lo <= a < hi for lo, hi in XDK)


total = 0
for f in sorted(glob.glob('src/game/recomp/gen/recomp_*.c')):
    lines = open(f, encoding='utf-8', errors='ignore').read().split('\n')
    changed = 0
    cur = None
    for i in range(len(lines) - 2):
        fm = FUNC.match(lines[i])
        if fm:
            cur = int(fm.group(1), 16)
            continue
        if cur is None or not in_xdk(cur):
            continue
        m = re.match(r'loc_([0-9A-Fa-f]{8}): ;', lines[i].strip())
        if not m:
            continue
        lab = m.group(1)
        for j in (i + 1, i + 2):
            if j >= len(lines): break
            s = lines[j].strip()
            if not s: continue
            m2 = re.match(r'if \((.+)\) goto loc_' + lab + r'; (/\*.*\*/)?', s)
            if m2 and 'MEM' in m2.group(1):
                indent = lines[j][:len(lines[j]) - len(lines[j].lstrip())]
                lines[j] = (indent + '/* DOA3: hw busy-wait forced through (was: if ('
                            + m2.group(1)[:70] + ') goto loc_' + lab + ') */')
                changed += 1
            break
    if changed:
        open(f, 'w', encoding='utf-8').write('\n'.join(lines))
        total += changed
print("hw self-spins forced through:", total)
