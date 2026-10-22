"""Keep the guest registers of a generated function in C locals.

In the generated code every x86 register (eax..edi, esp) and the x87 stack
(g_fp_stack / g_fp_top) are globals, and every guest memory access is a
volatile access through a computed pointer. The compiler must assume such a
pointer can alias any global, so after nearly every memory access it reloads
the registers from memory and stores them back after every change -- the
register file lives in memory.

This pass rewrites a function so its body works on locals:

    uint32_t L_eax = g_eax, ...;          on entry
    { REG_SAVE; sub_X(); REG_LOAD; }      around every call
    { REG_SAVE; return; }                 at every return

Callees still receive and return everything through the globals, so the
calling convention is untouched; inside the function a local whose address
is never taken cannot alias a guest pointer, and stays in a host register.

Only functions whose body uses nothing but the known-pure helpers (memory
macros, flag/compare macros, xmm/mmx helpers, maths) plus direct calls,
RECOMP_ICALL_SAFE and RECOMP_ITAIL are rewritten; anything else is left
alone. Idempotent: rewritten functions carry a marker.

Usage: python3 tools/recomp/localize_regs.py [--min-ratio N] [--only ADDR,...] FILE...
       python3 tools/recomp/localize_regs.py --out OUT.c FILE      (the build: writes a copy)
"""
import re
import sys

REGS = ["eax", "ecx", "edx", "ebx", "esi", "edi", "esp"]
MARK = "/* regs: locals (tools/recomp/localize_regs.py) */"

PURE = set("""
MEM8 MEM16 MEM32 SMEM8 SMEM16 SMEM32 MEMF MEMD XBOX_PTR PUSH32 POP32
LO8 HI8 LO16 SET_LO8 SET_HI8 SET_LO16 ZX8 ZX16 SX8 SX16
CMP_EQ CMP_NE CMP_L CMP_LE CMP_G CMP_GE CMP_B CMP_BE CMP_A CMP_AE
TEST_Z TEST_NZ TEST_S X86_PF8 ROR32 ROL32 BSF32
fp_push fp_pop fp_popp fp_top fp_st1 x87_frndint
if for while do switch sizeof return
sin cos tan atan2 sqrt sqrtf fabs log pow floor ceil remainder
memcpy memset
MMQ_LOAD MMQ_STORE
""".split())
PURE_PREFIX = ("xmm_", "mmx_")
CALL_RE = re.compile(r"\bsub_[0-9A-F]{8}\(\);")
ICALL_RE = re.compile(r"\b(RECOMP_ICALL_SAFE|RECOMP_ITAIL)\s*\(")
FUNC_RE = re.compile(r"^void (sub_[0-9A-F]{8}(?:_gen)?)\(void\)\n\{\n(.*?)\n\}\n", re.S | re.M)


def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    return re.sub(r"//[^\n]*", " ", s)


def eligible(body):
    code = strip_comments(body)
    if MARK in body or "setjmp" in code or "__try" in code or "__asm" in code:
        return None
    if re.search(r"\bg_(e[a-d]x|e[sd]i|ebp)\b", code):     # direct global register use
        return None
    if re.search(r"&\s*(?:g_)?(?:e[a-d]x|e[sd]i|esp)\b", code):   # address taken
        return None
    calls = len(CALL_RE.findall(code)) + len(ICALL_RE.findall(code))
    for m in re.finditer(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\(", code):
        n = m.group(1)
        if n in PURE or n.startswith(PURE_PREFIX) or re.fullmatch(r"sub_[0-9A-F]{8}", n):
            continue
        if n in ("RECOMP_ICALL_SAFE", "RECOMP_ITAIL"):
            continue
        if n in ("uint32_t", "int32_t", "int64_t", "uint64_t", "int", "double", "float"):
            continue                                       # casts
        return None
    return calls


WRITE_OPS = r"(?:=(?!=)|\+=|-=|\*=|/=|%=|&=|\|=|\^=|<<=|>>=|\+\+|--)"


def regs_written(code, r):
    tok = r"(?:\b%s\b|\bg_%s\b)" % (r, r)
    return bool(re.search(tok + r"\s*" + WRITE_OPS, code) or
                re.search(r"(?:\+\+|--)\s*" + tok, code) or
                re.search(r"\b(?:SET_LO8|SET_HI8|SET_LO16|PUSH32|POP32)\(\s*" + tok, code) or
                re.search(r"\bPOP32\([^;]*?,\s*" + tok + r"\s*\)", code))


def rewrite(name, body):
    code = strip_comments(body)
    uses_fp = re.search(r"\bg_fp_(stack|top)\b", code) is not None
    # Locals for the registers the body names; a callee may read any of them
    # (implicit parameters) and change any of them, so every used register
    # is reloaded after a call, and every register the body writes is stored
    # before one. A register that is used but never written equals its
    # global at every call and return, so it is never stored.
    regs = [r for r in REGS if re.search(r"\b(?:g_)?%s\b" % r, code)]
    wr = [r for r in regs if regs_written(code, r)]
    save = " ".join("g_%s = L_%s;" % (r, r) for r in wr)
    load = " ".join("L_%s = g_%s;" % (r, r) for r in regs)
    # The x87 stack is kept rotated so the body's top starts at 0: every
    # stack index in straight-line code is then a constant and the compiler
    # can keep the slots in registers. Element i of the local copy is
    # physical slot (L_fp_base + i) & 7; the physical top is
    # L_fp_base + L_fp_top.
    if uses_fp:
        save += (" { int _i; for (_i = 0; _i < 8; _i++) g_fp_stack[(L_fp_base + _i) & 7] = L_fp_stack[_i]; }"
                 " g_fp_top = L_fp_base + L_fp_top;")
        load += (" { int _i; L_fp_base = g_fp_top; L_fp_top = 0;"
                 " for (_i = 0; _i < 8; _i++) L_fp_stack[_i] = g_fp_stack[(L_fp_base + _i) & 7]; }")
    b = body
    # Registers and the x87 globals -> locals (whole-token, outside comments
    # is not required: comments are harmless).
    for r in regs:
        b = re.sub(r"\b%s\b" % r, "L_" + r, b)
        b = re.sub(r"\bg_%s\b" % r, "L_" + r, b)
    if uses_fp:
        b = re.sub(r"\bg_fp_stack\b", "L_fp_stack", b)
        b = re.sub(r"\bg_fp_top\b", "L_fp_top", b)
    b = CALL_RE.sub(lambda m: "{ REGS_SAVE; %s REGS_LOAD; }" % m.group(0), b)

    # Indirect calls: the macro bodies use the globals themselves.
    def icall(m):
        return "{ REGS_SAVE; " + m.group(0)
    out, i = [], 0
    for m in ICALL_RE.finditer(b):
        # find the end of the macro invocation: matching paren then ';'
        j = m.end()
        depth = 1
        while depth:
            c = b[j]
            if c == "(": depth += 1
            elif c == ")": depth -= 1
            j += 1
        while b[j] in " \t": j += 1
        assert b[j] == ";", name
        out.append(b[i:m.start()])
        out.append("{ REGS_SAVE; " + b[m.start():j + 1] + " REGS_LOAD; }")
        i = j + 1
    out.append(b[i:])
    b = "".join(out)
    b = re.sub(r"\breturn;", "{ REGS_SAVE; return; }", b)

    decl = ("    uint32_t " + ", ".join("L_%s = g_%s" % (r, r) for r in regs) + ";\n") if regs else ""
    if uses_fp:
        decl += ("    double L_fp_stack[8]; int L_fp_base = g_fp_top, L_fp_top = 0;\n"
                 "    { int _i; for (_i = 0; _i < 8; _i++) L_fp_stack[_i] = g_fp_stack[(L_fp_base + _i) & 7]; }\n")
    pre = ("    " + MARK + "\n" + decl +
           "    #define REGS_SAVE do { %s } while (0)\n" % save +
           "    #define REGS_LOAD do { %s } while (0)\n" % load)
    # Declarations of the body (ebp, flag temps) must stay first in C89-ish
    # style? MSVC C11 accepts mixed declarations, so prepend.
    post = "\n    REGS_SAVE;\n    #undef REGS_SAVE\n    #undef REGS_LOAD"
    return pre + b + post


# Signed arithmetic the lifter emits with C's undefined-overflow semantics,
# where x86 wraps. Kept in globals the compiler could not reason about the
# operands; in locals it can, and clang at -O2 was seen folding the sign of
# an overflowed imul (sub_0018C288). Rewritten to the same bits with
# defined behaviour: imul's low half is the unsigned product, and edx:eax is
# assembled unsigned before the conversion back to int64_t.
_OPND = r"(?:[A-Za-z_][A-Za-z0-9_]*\((?:[^()]|\([^()]*\))*\)|[A-Za-z_][A-Za-z0-9_]*|0x[0-9A-Fa-f]+u?|\d+)"
_IMUL_RE = re.compile(r"\(int32_t\)(" + _OPND + r") \* \(int32_t\)(" + _OPND + r")")
_DIVIDEND_RE = re.compile(r"\(\(int64_t\)\(int32_t\)(" + _OPND + r") << 32\) \| (" + _OPND + r")")


def fix_ub(s):
    s = _IMUL_RE.sub(r"(int32_t)((uint32_t)(\1) * (uint32_t)(\2))", s)
    s = _DIVIDEND_RE.sub(r"(int64_t)(((uint64_t)(uint32_t)(\1) << 32) | (uint32_t)(\2))", s)
    return s


def process(path, min_ratio, only, out=None):
    s = fix_ub(open(path).read())
    n_done = n_seen = 0

    def f(m):
        nonlocal n_done, n_seen
        name, body = m.group(1), m.group(2)
        n_seen += 1
        va = int(name[4:12], 16)
        if only is not None and va not in only:
            return m.group(0)
        calls = eligible(body)
        if calls is None:
            return m.group(0)
        stmts = body.count(";")
        if only is None and stmts < min_ratio * (calls + 1):
            return m.group(0)
        n_done += 1
        return "void %s(void)\n{\n%s\n}\n" % (name, rewrite(name, body))
    s2 = FUNC_RE.sub(f, s)
    if out is not None:
        with open(out, "w") as fh:
            fh.write(s2)
    else:
        open(path, "w").write(s2)
    return n_done, n_seen


def main(argv):
    min_ratio, only, files, out = 12, None, [], None
    i = 0
    while i < len(argv):
        if argv[i] == "--out":
            out = argv[i + 1]; i += 2; continue
        if argv[i] == "--min-ratio":
            min_ratio = float(argv[i + 1]); i += 2; continue
        if argv[i] == "--only":
            only = {int(x, 16) for x in argv[i + 1].split(",")}; i += 2; continue
        files.append(argv[i]); i += 1
    tot = seen = 0
    if out is not None and len(files) != 1:
        sys.exit("--out takes exactly one input file")
    for p in files:
        d, s = process(p, min_ratio, only, out)
        tot += d; seen += s
    print("%d of %d functions rewritten" % (tot, seen), file=sys.stderr)


if __name__ == "__main__":
    main(sys.argv[1:])
