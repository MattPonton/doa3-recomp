"""Turn lifted jump-table dispatch into a C switch.

The lifter emits each intra-function jump table as a run of
`if (_jt == 0x...u) goto loc_...;` lines, one per target, ahead of the
indirect-tail fallback. MSVC compiles that run as a linear chain of compares,
so a 170-target table costs up to 170 compares per dispatch. The Sofdec
movie decoder's VLC readers (sub_001B0C40, sub_001B3FB0) dispatch through
such tables once per coefficient. As a switch, the compiler emits a
binary search or an indexed jump instead. Behavior is unchanged: the
targets are unique, and a value matching none of them still reaches the
RECOMP_ITAIL fallback. Idempotent; a block with any unexpected line is left
as it is.
"""
import re
import sys
from pathlib import Path

HEAD = re.compile(r"^(\s*)\{ uint32_t _jt = (.*); (/\* switch: [^*]*\*/)\s*$")
CASE = re.compile(r"^\s*if \(_jt == (0x[0-9A-F]+u)\) goto (loc_[0-9A-F]{8});\s*$")
DEAD = re.compile(r"^\s*if \(_jt == 0x[0-9A-F]+u\) \(void\)0; /\* goto loc_[0-9A-F]{8} - dead code, label not in function \*/\s*$")
TAIL = re.compile(r"^\s*g_seh_ebp = ebp; RECOMP_ITAIL\(_jt\); return; \}\s*$")


def patch(source):
    lines = source.splitlines(keepends=True)
    out, i, changed, skipped = [], 0, 0, 0
    while i < len(lines):
        m = HEAD.match(lines[i])
        if not m:
            out.append(lines[i]); i += 1; continue
        indent, expr, comment = m.groups()
        cases, j, ok = [], i + 1, False
        while j < len(lines):
            c = CASE.match(lines[j])
            if c:
                cases.append(c.groups()); j += 1; continue
            if DEAD.match(lines[j]):
                j += 1; continue
            ok = bool(TAIL.match(lines[j]))
            break
        if not ok or not cases or len({v for v, _ in cases}) != len(cases):
            # Already converted, or every target lies outside the function
            # (nothing to dispatch): not a failure.
            if ok and not cases:
                pass
            elif i + 1 < len(lines) and lines[i + 1].lstrip().startswith("switch (_jt)"):
                pass
            else:
                skipped += 1
            out.append(lines[i]); i += 1; continue
        nl = "\n"
        out.append(f"{indent}{{ uint32_t _jt = {expr}; {comment}{nl}")
        out.append(f"{indent}switch (_jt) {{{nl}")
        for v, label in cases:
            out.append(f"{indent}case {v}: goto {label};{nl}")
        out.append(f"{indent}default: break; }}{nl}")
        out.append(lines[j])  # the RECOMP_ITAIL fallback, unchanged
        changed += 1
        i = j + 1
    return "".join(out), changed, skipped


def main():
    root = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("src/game/recomp/gen")
    total = skipped_total = 0
    for path in sorted(root.glob("recomp_*.c")):
        source = path.read_text()
        result, changed, skipped = patch(source)
        skipped_total += skipped
        if changed:
            path.write_text(result)
            total += changed
    print(f"jump-table switches: {total} newly converted, {skipped_total} unexpected blocks left alone")


if __name__ == "__main__":
    main()
