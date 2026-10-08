"""Hand generated functions over to hand-written overrides.

tools/recomp/overrides_<version>.txt lists the guest addresses whose
replacement lives in src/game/recomp/recomp_manual.c (one `0xADDR  comment`
per line). For each one the generated definition `void sub_X(void)` is
renamed `sub_X_gen`, so the override takes the name every caller and the
dispatch table use, and can still call the original body. Idempotent; run
after tools.recomp.postprocess and before tools.recomp.esp_probes (which
leaves these addresses alone).

Usage (from doa3/, with DOA3_XBE set):  py -3 -m tools.recomp.apply_overrides
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.getcwd())
from tools import xbe_layout as L  # noqa: E402

GEN = os.path.join("src", "game", "recomp", "gen")


def override_list():
    path = os.path.join("tools", "recomp", f"overrides_{L.VERSION}.txt")
    out = set()
    if os.path.exists(path):
        for line in open(path):
            line = line.split("#")[0].strip()
            if line:
                out.add(int(line.split()[0], 16))
    return out


def main():
    want = override_list()
    done = set()
    for f in sorted(glob.glob(os.path.join(GEN, "recomp_*.c"))):
        if os.path.basename(f) in ("recomp_probes.c", "recomp_dispatch.c"):
            continue
        s = open(f).read()

        def ren(m):
            va = int(m.group(1)[4:], 16)
            if va in want:
                done.add(va)
                return f"void {m.group(1)}_gen(void)"
            return m.group(0)
        s2 = re.sub(r"^void (sub_[0-9A-F]{8})\(void\)$", ren, s, flags=re.M)
        for m in re.finditer(r"^void (sub_[0-9A-F]{8})_gen\(void\)$", s2, flags=re.M):
            if int(m.group(1)[4:], 16) in want:
                done.add(int(m.group(1)[4:], 16))
        if s2 != s:
            open(f, "w").write(s2)
    missing = want - done
    print(f"{len(done)} overrides applied for {L.VERSION}"
          + (f"; no generated body for {', '.join(hex(v) for v in sorted(missing))}" if missing else ""),
          file=sys.stderr)


if __name__ == "__main__":
    main()
