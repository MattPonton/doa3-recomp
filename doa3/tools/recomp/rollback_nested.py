"""Generate experimental nested continuations for an explicit audited subset.

Only root-scope direct calls may suspend. Unlisted direct calls fail generation;
indirect dispatch and native/scheduler effects must be audited separately.
The live generator uses it for the match/game-flow roots; other tasks retain
their single-yield continuation representation.
"""
import re

YIELD = "sub_001C65D8(); /* call 0x001C65D8 */"


def transform(va, body, selected, synchronous):
    first = re.search(r"^loc_[0-9A-F]+: ;", body, re.M)
    if not first:
        raise ValueError("missing entry label")
    declarations, code = body[:first.start()], body[first.start():]
    names = []
    for line in declarations.splitlines():
        clean = re.sub(r"/\*.*?\*/", "", line).strip()
        if not clean or clean.startswith("#define") or clean == "ebp = g_seh_ebp;":
            continue
        match = re.fullmatch(r"(uint32_t|int) (.*);", clean)
        if not match:
            raise ValueError(f"unsupported declaration {clean!r}")
        for part in match[2].split(","):
            m = re.fullmatch(r"\s*([A-Za-z_]\w*)(?:\s*=\s*(0|g_seh_ebp))?\s*", part)
            if not m or (m[2] == "g_seh_ebp" and (m[1], match[1]) != ("ebp", "uint32_t")):
                raise ValueError(f"unsupported local {part!r}")
            names.append((m[1], match[1]))
    if len(names) > 64 or re.search(r"^    (?:uint\w*_t|int|float|double|void|char) \w", code, re.M):
        raise ValueError("unsupported live locals")
    # 72F50's seven-way local jump table already handles every known target
    # with gotos. Route its fallback through the bounded selected-frame table;
    # an unknown target fails instead of entering an uncaptured native stack.
    if va == 0x72F50:
        old = "g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }"
        if code.count(old) != 1:
            raise ValueError("72F50 jump-table fallback changed")
        code = code.replace(old, "g_seh_ebp = ebp; return (Doa3NestedAction){DOA3_NESTED_TAIL,_jt}; }")
    # These local jump tables enumerate every target in the original body.
    # Preserve those gotos; refuse unknown targets instead of entering a
    # native tail call or treating another selected routine as a valid case.
    local_tables = {
        0x20DE0: ("eax * 4 + 0x21690", (0x20E15,0x20E38,0x20E4F,0x20E66,0x20E7B)),
        0x87420: ("eax * 4 + 0x8760C", (0x875AB,0x875B2,0x875B9,0x875DC,0x875E3)),
        0x18A430: ("eax * 4 + 0x18A4D4", (0x18A481,0x18A48C,0x18A497,0x18A4A2)),
    }
    if va in local_tables:
        expression, targets = local_tables[va]
        old = (f"    {{ uint32_t _jt = MEM32({expression}); /* switch: {len(targets)} entries, {len(targets)} targets */\n" +
               "".join(f"    if (_jt == 0x{target:08X}u) goto loc_{target:08X};\n" for target in targets) +
               "    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }")
        if code.count(old) != 1:
            raise ValueError(f"{va:08X} local jump table changed")
        code = code.replace(old, old.replace("g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return;",
                            "return (Doa3NestedAction){DOA3_NESTED_BLOCK,0};"))
    if re.search(r"\b(?:RECOMP_ICALL\w*|RECOMP_ITAIL)\b", code):
        raise ValueError("indirect control transfer needs explicit audit")
    points = []
    save = "".join(f"    frame->locals[{i}] = (uint32_t){name};\n" for i, (name, _) in enumerate(names))
    lines = []
    for line in code.splitlines(keepends=True):
        calls = list(re.finditer(r"sub_([0-9A-F]{8})\(\);", line))
        if not calls:
            lines.append(line.replace("return;", "return (Doa3NestedAction){DOA3_NESTED_RETURN,0};"))
            continue
        if len(calls) != 1:
            raise ValueError("multiple calls on one line")
        m = calls[0]; target = int(m[1], 16)
        if target in synchronous:
            lines.append(line.replace("return;", "return (Doa3NestedAction){DOA3_NESTED_RETURN,0};"))
            continue
        if target not in selected and target != 0x1C65D8:
            raise ValueError(f"unaudited call {target:08X}")
        prefix, suffix = line[:m.start()], line[m.end():]
        # The lifter represents conditional cross-function tail branches this
        # way. No caller survives a tail transfer, so no scoped local is saved.
        if (target in selected and re.fullmatch(
                r"    if \(.+\) \{ g_seh_ebp = ebp; ", prefix) and
                re.fullmatch(r" return; \} /\* j\w+: [^\n]* \*/\n?", suffix)):
            lines.append(prefix + f"return (Doa3NestedAction){{DOA3_NESTED_TAIL,0x{target:X}}}; }}\n")
            continue
        # Suspending inside a scoped temporary/conditional call would lose
        # live C state. Only the lifter's simple root-level call syntax is used.
        if (not line.startswith("    ") or line.startswith("        ") or
                re.search(r"\b(?:if|for|while|switch)\b|[{}]", prefix)):
            raise ValueError("suspending call must be root scope")
        # Also track surrounding braces, not merely the indentation.
        preceding = "".join(lines)
        clean = re.sub(r"/\*.*?\*/|//[^\n]*", "", preceding, flags=re.S)
        if clean.count("{") != clean.count("}"):
            raise ValueError("suspending call inside instruction block")
        tail = suffix.strip() == f"return; /* tail jmp 0x{target:08X} */"
        if tail:
            if target == 0x1C65D8:
                raise ValueError("tail yield unsupported")
            lines.append(prefix + f"return (Doa3NestedAction){{DOA3_NESTED_TAIL,0x{target:X}}};\n")
            continue
        if suffix.strip() != f"/* call 0x{target:08X} */":
            raise ValueError("unsupported call suffix")
        index = len(points) + 1
        kind = "YIELD" if target == 0x1C65D8 else "CALL"
        points.append(kind)
        lines.append(prefix + ("doa3_rollback_task_sleep();\n" if kind == "YIELD" else "\n"))
        lines.append(save + f"    frame->resume = {index};\n")
        lines.append(f"    return (Doa3NestedAction){{DOA3_NESTED_{kind},{0 if kind == 'YIELD' else target}}};\n")
        lines.append(f"nested_resume_{index}: ;\n")
    resume = "    if (frame->resume) {\n"
    resume += "".join(f"        {name} = ({kind})frame->locals[{i}];\n" for i, (name, kind) in enumerate(names))
    resume += "        switch (frame->resume) {\n"
    resume += "".join(f"        case {i}: goto nested_resume_{i};\n" for i in range(1, len(points)+1))
    resume += "        default: return (Doa3NestedAction){DOA3_NESTED_BLOCK,0};\n        }\n    }\n"
    source = f"static Doa3NestedAction nested_{va:08X}(Doa3NestedFrame *frame)\n{{{declarations}{resume}{''.join(lines)}\n    return (Doa3NestedAction){{DOA3_NESTED_BLOCK,0}};\n}}\n"
    return source, points


def generate(bodies, synchronous):
    if set(bodies) & set(synchronous) or 0x1C65D8 in synchronous:
        raise ValueError("synchronous and suspending routines overlap")
    output, validations = [], []
    for va, body in sorted(bodies.items()):
        source, points = transform(va, body, set(bodies), set(synchronous))
        output.append(source)
        cases = "case 0: return DOA3_NESTED_ENTRY;"
        cases += "".join(f"case {i}: return DOA3_NESTED_{kind}_POINT;" for i, kind in enumerate(points, 1))
        validations.append(f"case 0x{va:X}: switch(resume) {{{cases}}} return DOA3_NESTED_INVALID;")
    output.append("static int nested_validate(uint32_t routine,uint32_t resume) { switch(routine) {" +
                  "".join(validations) + "} return DOA3_NESTED_INVALID; }\n")
    output.append("static Doa3NestedAction nested_dispatch(Doa3NestedFrame *frame) { switch(frame->routine) {" +
                  "".join(f"case 0x{va:X}: return nested_{va:08X}(frame);" for va in sorted(bodies)) +
                  "} return (Doa3NestedAction){DOA3_NESTED_BLOCK,0}; }\n")
    return "\n".join(output)
