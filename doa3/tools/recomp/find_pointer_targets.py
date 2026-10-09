"""List code addresses reached only through pointers, as seeds for the lifter.

The function detector follows direct calls. Anything the game only reaches
through a pointer -- thread start routines, callbacks handed to the CRT or
XAPI, vtable slots, state/jump tables of handlers -- gets no generated body
unless it is seeded, and an indirect call to it then resolves to nothing.
(On 3.1 the first casualty was the main thread: CreateThread passes XAPI's
thread trampoline as an immediate, so the boot thread never ran.)

Sources scanned:
  1. Immediates in code: push imm32, mov reg, imm32, mov [mem], imm32.
  2. Pointer tables in .rdata/.data: runs of at least MIN_RUN consecutive
     dwords that all point into code (vtables, callback/state tables).

A candidate is kept only if it lands on an instruction boundary of the
linear disassembly of the section it is in, and it either starts a known
function already, or looks like a function entry: preceded by ret/jmp/int3/
nop padding, or a common prologue (push ebp; mov ebp, esp / sub esp / push
is not enough). Addresses that already have a body are skipped.

Usage (from doa3/, with DOA3_XBE set):
  py -3 -m tools.recomp.find_pointer_targets > ptr_seeds.txt
  py -3 -m tools.recomp.seed_missing_functions ptr_seeds.txt
then re-run the usual func_id / recomp / find_unresolved loop.
"""
import json
import os
import struct
import sys
from collections import Counter

from capstone import CS_ARCH_X86, CS_MODE_32, CS_OP_IMM, CS_OP_MEM, CS_OP_REG, Cs

sys.path.insert(0, os.getcwd())
from tools import xbe_layout as L  # noqa: E402

MIN_RUN = 2
FUNCS = os.environ.get("DOA3_FUNCS", os.path.join("tools", "disasm", "output", "functions.json"))


def main():
    data = open(L.XBE_PATH, "rb").read()
    code_secs = [s for s in L.LAYOUT["sections"] if s[0] not in L.DATA_SECTION_NAMES
                 and s[0] != "DOLBY"]
    data_secs = [s for s in L.LAYOUT["sections"] if s[0] in (".rdata", ".data")]

    def in_code(va):
        for s in code_secs:
            if s[1] <= va < s[1] + s[4]:
                return s
        return None

    funcs = json.load(open(FUNCS))
    starts = {int(f["start"], 16) for f in funcs}
    # Starts the detector found on its own. Seeded fragments (link_seed) are
    # not evidence of an entry: data tables of small consecutive numbers
    # (0x1E00C4, 0x1E00C5, 0x1E00C6, ...) otherwise "confirm" themselves
    # once an earlier pass has seeded them.
    trusted = {int(f["start"], 16) for f in funcs
               if f.get("detection_method") in ("call_target", "prologue", "entry_point")}

    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    md.skipdata = True

    # Linear sweep of every code section: instruction boundaries and immediates.
    boundary, prev_mn, imms = set(), {}, Counter()
    for name, va, _vs, raw, rs, _fl in code_secs:
        last = None
        for ins in md.disasm(data[raw:raw + rs], va):
            if ins.mnemonic == ".byte":
                last = None
                continue
            boundary.add(ins.address)
            prev_mn[ins.address] = last
            last = ins
            ops = ins.operands
            if ins.mnemonic == "push" and ops and ops[0].type == CS_OP_IMM:
                imms[ops[0].imm & 0xFFFFFFFF] += 1
            elif ins.mnemonic == "mov" and len(ops) == 2 and ops[1].type == CS_OP_IMM \
                    and ops[0].type in (CS_OP_REG, CS_OP_MEM) and ins.size >= 5:
                imms[ops[1].imm & 0xFFFFFFFF] += 1

    def looks_like_entry(t):
        if t in trusted:
            return True
        if t not in boundary:
            return False
        p = prev_mn.get(t)
        if p is None or p.mnemonic in ("ret", "retn", "jmp", "int3", "nop") or \
                (p.mnemonic == "lea" and p.op_str in ("esi, [esi]", "edi, [edi]", "esp, [esp]")):
            return True
        s = in_code(t)
        o = s[3] + t - s[1]
        b = data[o:o + 3]
        # (a bare push ebx/esi/edi is not enough: it is just as often the
        # middle of a function, e.g. 0x1E00C6 inside D3D's 0x1E00C0)
        return b[:3] == b"\x55\x8b\xec" or b[:2] == b"\x83\xec" or b[:2] == b"\x81\xec"

    cands = {}
    for t, n in imms.items():
        if in_code(t) and looks_like_entry(t):
            cands[t] = "imm"

    # Pointer tables in data sections.
    tables = 0
    for name, va, _vs, raw, rs, _fl in data_secs:
        run = []
        for o in range(0, rs - 3, 4):
            v = struct.unpack_from("<I", data, raw + o)[0]
            if in_code(v) and v in boundary:
                run.append(v)
                continue
            # Null slots do not end a table: Sofdec's per-picture-type
            # handler tables (0x21AD80, 0x21ADA8 on 3.1) are mostly zeros
            # with one B-picture routine each, five slots apart.
            if v == 0 and run:
                continue
            if len(run) >= MIN_RUN:
                tables += 1
                for t in run:
                    if looks_like_entry(t):
                        cands.setdefault(t, "table")
            run = []

    if "--all" in sys.argv:
        for t in sorted(cands):
            print(f"sub_{t:08X} {cands[t]}{'' if t not in starts else ' (has function)'}")
        return
    new = sorted(t for t in cands if t not in starts)
    by_src = Counter(cands[t] for t in new)
    for t in new:
        print(f"sub_{t:08X}")
    print(f"{len(cands)} pointer targets ({len(new)} without a function yet: "
          f"{by_src['imm']} from immediates, {by_src['table']} from {tables} tables)",
          file=sys.stderr)


if __name__ == "__main__":
    main()
