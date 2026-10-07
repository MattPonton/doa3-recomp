"""List sub_XXXXXXXX symbols referenced by generated code but not defined in it.

Stand-in for collecting MSVC 'unresolved external symbol' errors, so the
seed_missing_functions loop can run without a Windows build.
Usage: python -m tools.recomp.find_unresolved > unresolved.txt
"""
import glob, re, sys

ref, defined = set(), set()
files = glob.glob("src/game/recomp/gen/*.c") + ["src/game/recomp/recomp_manual.c"]
for f in files:
    s = open(f, encoding="utf-8", errors="ignore").read()
    defined |= set(re.findall(r"^(?:static\s+)?void\s+(sub_[0-9A-F]{8})\s*\(void\)\s*\{?$", s, re.M))
    ref |= set(re.findall(r"\b(sub_[0-9A-F]{8})\b(?!_)", s))
for name in sorted(ref - defined):
    print(name)
print(f"{len(ref - defined)} unresolved, {len(defined)} defined", file=sys.stderr)
