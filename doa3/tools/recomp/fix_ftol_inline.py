# RECOMP BUG #8 fix: inline CRT _ftol2 at every call site (RE-APPLY after regen).
#
# The recompiler gives each function a LOCAL FPU stack (double _fp_stack[8]),
# so the caller's `fld x; call _ftol2` pattern breaks: the callee
# (sub_0018DB30) reads its own uninitialized stack and returns garbage in
# edx:eax for EVERY float->int conversion (~1,390 sites). Worse, _ftol2's
# body was fragmented (sub_0018DB53/sub_0018DB8F) and its `mov esp, ebp`
# epilogue ran with a mismatched frame, wrecking g_esp (0xEFFFFFxx storms).
#
# Correct semantics = do the conversion inline in the CALLER, popping the
# value from the CALLER's local FPU stack. _ftol2 truncates toward zero and
# returns the int64 in edx:eax.
import re, glob, sys, os
sys.path.insert(0, os.getcwd())
from tools.xbe_layout import FTOL2

CALL = f"PUSH32(esp, 0); sub_{FTOL2:08X}(); /* call 0x{FTOL2:08X} */"
INLINE = ("{ int64_t _ft = (int64_t)fp_top(); fp_popp(); "
          "eax = (uint32_t)_ft; edx = (uint32_t)((uint64_t)_ft >> 32); } "
          "/* inline _ftol2 (bug #8) */")

total = 0
for f in glob.glob('src/game/recomp/gen/*.c'):
    s = open(f, encoding='utf-8', errors='ignore').read()
    n = s.count(CALL)
    if n:
        s = s.replace(CALL, INLINE)
        open(f, 'w', encoding='utf-8').write(s)
        total += n
print("inlined _ftol2 call sites:", total)
