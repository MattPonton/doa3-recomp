"""Extend function ranges over the code reachable from their entry.

The detector often ends a function early (at the next address something
calls, or at a block it could not see a way into). Seeding then turns the rest
of the real function into separate "link_seed" fragments, and the translator
emits a jump between fragments as a C call (`sub_X(); return;`). A loop that
spans fragments then recurses natively once per iteration: on 3.1 that left
267 call cycles (750 fragments, the CRT heap allocator among them) and a
silent host stack overflow at boot.

This pass recomputes each function's end from control flow: starting at the
entry it follows fall-through, conditional and direct unconditional jumps, but
not calls, and stops at another function's real entry (a true tail call). The
new end is the end of the contiguous run of reachable code starting at the
entry (alignment padding between blocks is allowed; anything else stops the
run), so data is never pulled into a function. Ends only ever grow. Fragment
entries are kept, so code that calls or jumps into them still resolves; they
just stop being the only copy of their code.

"Real" entries: anything that is called directly, the XBE entry point, the
detector's call_target/prologue/entry_point finds, and pointer-target seeds
(tools.recomp.find_pointer_targets). Plain branch-target seeds are not.

Usage (from doa3/, with DOA3_XBE set), after the disassembler and seeding:
  py -3 -m tools.recomp.extend_functions [ptr_seeds.txt ...]
"""
import json
import os
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs

sys.path.insert(0, os.getcwd())
from tools import xbe_layout as L  # noqa: E402

FUNCS = os.path.join("tools", "disasm", "output", "functions.json")
MAX_SIZE = 0x10000
TERM = {"ret", "retn", "jmp", "int3", "hlt", "ud2"}
PAD_BYTES = {0x90, 0xCC}


def main():
    data = open(L.XBE_PATH, "rb").read()
    secs = [s for s in L.LAYOUT["sections"] if s[0] not in L.DATA_SECTION_NAMES and s[0] != "DOLBY"]

    def sec_of(va):
        for s in secs:
            if s[1] <= va < s[1] + s[4]:
                return s
        return None

    md = Cs(CS_ARCH_X86, CS_MODE_32)
    cache = {}

    def decode(va):
        if va in cache:
            return cache[va]
        s = sec_of(va)
        ins = None
        if s:
            o = s[3] + va - s[1]
            ins = next(md.disasm(data[o:o + 16], va), None)
        cache[va] = ins
        return ins

    funcs = json.load(open(FUNCS))
    by_start = {int(f["start"], 16): f for f in funcs}

    # Real entries: detector finds, pointer seeds named on the command line,
    # and direct call targets that look like a function entry. A linear sweep
    # also decodes calls out of inline data and misaligned bytes; taken at face
    # value those "targets" land mid-function (0x1E00C6, a `push ebx` inside
    # D3D's 0x1E00C0) and split real functions into fragments whose
    # fall-throughs then get dropped.
    boundary, prev = set(), {}
    for name, va, _vs, raw, rs, _fl in secs:
        last = None
        for ins in md.disasm(data[raw:raw + rs], va):
            boundary.add(ins.address)
            prev[ins.address] = last
            last = ins

    def looks_like_entry(t):
        p = prev.get(t)
        if t not in boundary:
            return False
        if p is None or p.mnemonic in ("ret", "retn", "jmp", "int3", "nop", "hlt") or \
                (p.mnemonic == "lea" and p.op_str in ("esi, [esi]", "edi, [edi]", "esp, [esp]")):
            return True
        s = sec_of(t)
        o = s[3] + t - s[1]
        b = data[o:o + 3]
        return b == b"\x55\x8b\xec" or b[:2] in (b"\x83\xec", b"\x81\xec")

    real = {L.ENTRY_POINT}
    calls = set()
    for f in funcs:
        if f.get("detection_method") in ("prologue", "entry_point"):
            real.add(int(f["start"], 16))
        elif f.get("detection_method") == "call_target":
            calls.add(int(f["start"], 16))   # same entry test as other call targets
        for c in f.get("calls_to", []) or []:
            calls.add(int(c, 16))
    for path in sys.argv[1:]:
        for line in open(path):
            line = line.strip()
            if line.startswith("sub_"):
                real.add(int(line[4:12], 16))
    for name, va, _vs, raw, rs, _fl in secs:
        for ins in md.disasm(data[raw:raw + rs], va):
            if ins.mnemonic == "call" and ins.op_str.startswith("0x"):
                calls.add(int(ins.op_str, 16))
    dropped = 0
    for t in calls:
        if t in real:
            continue
        if looks_like_entry(t):
            real.add(t)
        else:
            dropped += 1
    print(f"{dropped} call targets ignored as entries (mid-function)", file=sys.stderr)

    def is_pad(va):
        s = sec_of(va)
        return s is not None and data[s[3] + va - s[1]] in PAD_BYTES

    grown = 0
    total_added = 0
    for start, f in sorted(by_start.items()):
        old_end = int(f["end"], 16)
        if not sec_of(start):
            continue
        # Reachable instructions from the entry.
        seen = {}
        work = [start]
        while work:
            va = work.pop()
            if va in seen or va - start > MAX_SIZE or va < start:
                continue
            if va != start and va in real:
                continue                  # another function's entry: tail call
            ins = decode(va)
            if ins is None:
                continue
            seen[va] = ins
            mn = ins.mnemonic
            nxt = va + ins.size
            if mn == "jmp":
                if ins.op_str.startswith("0x"):
                    work.append(int(ins.op_str, 16))
                continue                  # indirect jmp: the translator handles switch tables
            if mn in TERM:
                continue
            if mn.startswith("j") or mn.startswith("loop"):
                if ins.op_str.startswith("0x"):
                    work.append(int(ins.op_str, 16))
            work.append(nxt)
        if not seen:
            continue
        # Contiguous run from the entry (padding allowed between blocks).
        va = start
        end = start
        while va - start <= MAX_SIZE:
            if va in seen:
                end = va + seen[va].size
                va = end
                continue
            if is_pad(va) and any(start < k < start + MAX_SIZE and k > va for k in (va + 1,)):
                # step over padding only if reachable code follows it
                p = va
                while is_pad(p) and p - va < 64:
                    p += 1
                if p in seen:
                    va = p
                    continue
            break
        if end > old_end:
            f["end"] = f"0x{end:08X}"
            f["size"] = end - start
            grown += 1
            total_added += end - old_end

    json.dump(funcs, open(FUNCS, "w"), indent=1)
    print(f"{grown} of {len(funcs)} functions extended, +{total_added} bytes", file=sys.stderr)


if __name__ == "__main__":
    main()
