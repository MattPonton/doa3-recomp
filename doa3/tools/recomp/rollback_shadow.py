"""Build an offline, isolated interpreter of the current translated fight code.

No live game or audio-output runtime is linked. The production APU command
backend runs only against a private captured instance. Unknown calls, host overrides, guest clocks,
out-of-range memory and excessive execution abort the sample; none is stubbed
as success. The direct-call closure is conservative; indirect targets outside
it are reported for audit rather than silently added.
"""
from pathlib import Path
import argparse
import re
from rollback_tasks import TASKS, transform, source_digest, nested_step_source, NESTED_ROOTS
from rollback_match import generate_match, NATIVE_BOUNDARIES

# These hooks only observe guest results when the training recorder is idle.
OBSERVERS = {0x11980,0x12B60,0x7F1A0,0xB7180,0xB9980,0xE8E60,0xE8F60}
# These wrappers run the original bodies unchanged and only log Start/join
# and pause diagnostics. Their static counters do not affect guest execution.
OBSERVERS |= {0x88910,0x8A2E0}
# Opt-in baseline profile only: free camera/FOV overrides are host state, not
# in the capture. Reproduce the native path and still compare EVERY byte.
NATIVE_CAMERA = {0x29E30}
# Captured indirect animation interpolation callbacks, audited as guest-only.
# 9DCC0 initializes a linear scalar interpolation: start value, per-frame
# delta and zero curvature. It uses only guest memory and x87 registers.
# 9DC20 initializes cubic interpolation coefficients from signed keyframe
# tangents, duration and endpoint values. It is a leaf using only guest
# memory/registers and x87 arithmetic; no host services or clocks are called.
# 1F3660 is the indirect HRTF table selector at guest pointer 20BEA4.
# It quantizes angles, reads guest tables, and writes two guest filter pointers;
# its only direct helper is the guest float-to-int routine 1F3006. It neither
# advances audio nor invokes device/host services. Replay the actual body.
INDIRECT_ROOTS = {0x9DB70,0x9DC20,0x9DCC0,0x9DCF0,0x1F3660}
# The seven callbacks installed at 20BE88..20BEA0 by 1F1307 compute
# listener-relative position, HRTF angles, distance/cone attenuation, filtering,
# and Doppler pitch. Their generated bodies and math helpers access only guest
# vectors/tables and x87 state. 1F3204 calls the audited HRTF selector above.
# Preserve the actual calculations, including all guest stack writes.
INDIRECT_ROOTS |= {0x1F31B8,0x1F3204,0x1F3354,0x1F33C6,0x1F345D,0x1F34EB,0x1F35C2}
# E8030's captured scene callback: a leaf writing guest bytes [ECX+4]=0
# and [ECX+5]=1. It has no rendering, clock, or native service interaction.
INDIRECT_ROOTS |= {0xEA070}
# EA080 is the captured scene/model callback: guest visibility/fade state,
# transforms and model dispatch. Execute its translated body and direct-call
# closure; virtual targets and all graphics/native overrides still fail closed
# unless independently supported. This is not a renderer replacement.
INDIRECT_ROOTS |= {0xEA080}
# EA630, reached through EA080's vtable in the chained capture, is the actual
# argument-shuffling wrapper for EA380's guest visibility test. Include that
# body/closure; do not replace its return value or discard its stack writes.
INDIRECT_ROOTS |= {0xEA630}
# Captured after submission replay: E9710 transforms three guest vectors,
# invokes the guest intersection helper and updates the nearest distance.
# EAF70 is the indirect scene-effects callback installed at C58A8C; execute
# its actual guest render-state/geometry dispatch and direct-call closure.
# Neither inclusion bypasses native graphics/flip guards or unknown callbacks.
# 1F12D7 is a leaf audio-object AddRef: increments [argument+4], returns that
# count in EAX and consumes its stack argument. It has no host audio effects.
INDIRECT_ROOTS |= {0xE9710,0xEAF70,0x1F12D7}
# The runtime wraps this original guest body with the shared host target tracker.
# It requires a schema-4 snapshot and the explicit submission-stream profile.
INDIRECT_ROOTS |= {0x1DC8E0}
# Schema-4 capture callbacks: E97F0 transforms three guest vectors and updates
# a nearest intersection through 2BB70 (the sibling of E9710). E69E0 is a leaf
# iterator over guest effects entries, copying four words and an opacity value
# into guest render constants. Neither owns native rendering resources.
# 1F1816 is the guarded audio AddRef variant: execute its guest lock/status
# logic and real refcount increment, retaining the existing kernel-call guards.
# Its critical-section callbacks are NOT replaced with invented success here.
INDIRECT_ROOTS |= {0xE97F0,0xE69E0,0x1F1816}
# Next captured dispatches: F5B90 returns the guest float at 250ED0; 51830
# projects a guest position and clamps its height using DBFF0. 1F4A7D derives
# voice format/channel flags from the guest descriptor and may write APU
# methods 202F8/20304. Replay that actual body through the private captured APU
# backend; unknown lock/service calls and unsupported MMIO still fail closed.
INDIRECT_ROOTS |= {0xF5B90,0x51830,0x1F4A7D}
# Captured scene predicates/dispatch: 36C00's real guest body returns EAX=1
# and pops its return address; 118FB0 is an exact tail wrapper for E73D0.
# Include the bodies rather than special-casing their observed results.
INDIRECT_ROOTS |= {0x36C00,0x118FB0}
# E7360 is the unfiltered sibling of E69E0: advance one guest effect-list
# element and copy its four constants/opacity, or return the exhausted state.
# This leaf has no host service, clock or graphics-resource operations.
INDIRECT_ROOTS |= {0xE7360}
# EF190 is a scene draw callback passing guest tables 299970/2999B0 and 1.0f
# into DDEE0. Execute the original wrapper and its translated draw path; native
# services and unsupported indirect callbacks retain their existing guards.
INDIRECT_ROOTS |= {0xEF190}
# 6DF00's manual override delegates ordinary sprites to the original body.
# The runtime checks the sprite ID and rejects the two host-owned Online
# assets; only the unchanged native branch may enter this translated body.
INDIRECT_ROOTS |= {0x6DF00}
# 5AF60 is the captured UI lookup callback: read the low byte of its stack
# argument and return a u16 from guest table 309480. No calls or host state.
INDIRECT_ROOTS |= {0x5AF60}
# 1EE9DD is the captured sprite object's begin callback: reject an already
# active object, capture/apply its guest D3D state blocks via 1E982D/1E96A5,
# then mark it active. Execute the real state-block bodies and keep all native
# graphics/resource guards; this does not substitute host D3D state.
INDIRECT_ROOTS |= {0x1EE9DD}
# Matching sprite end callback: require active state, apply the saved guest
# block through 1E96A5, clear the active flag, preserve ret-4 ABI and errors.
# 1EEA0C is the associated transform wrapper used by the same sprite caller:
# build its guest matrix via 1EE4C4 and dispatch the object's draw method.
# That indirect draw target still requires its own audited implementation.
INDIRECT_ROOTS |= {0x1EEAE3,0x1EEA0C}
# Captured sprite Draw method: validate guest texture/rectangle arguments,
# construct/transform four vertices, set guest texture/blend state and emit
# the original DrawPrimitiveUP commands. Its optional Begin/End virtual calls
# remain guarded through the audited methods above; no pixels are fabricated.
INDIRECT_ROOTS |= {0x1EEB46}
# Sprite Release decrements the guest reference count and, only at zero,
# executes the original state-block/device release and allocator path. Keep
# their native resource/kernel boundaries guarded rather than skipping cleanup.
INDIRECT_ROOTS |= {0x1EEB25}
# 49790's captured 31C43C..31C53F table dispatches per-player updates,
# predicates and draw setup. CD00/D3A0 interpolate guest coefficient arrays;
# E040/E1F0 advance guest animation state; E890/EA80 use guest vector/matrix
# helpers. The draw entries build guest descriptors and invoke the original
# draw helpers. The two CRT leaves really are a plain return / EAX=0 return.
# Execute their bodies and closure, preserving all native/indirect guards.
INDIRECT_ROOTS |= {0x4CD00,0x4D3A0,0x4D920,0x4E040,0x4E1F0,0x4E890,0x4EA80,
                  0x4D0F0,0x4D110,0x4D780,0x4D7A0,0x4D940,0x4DAB0,
                  0x4E550,0x4E5F0,0x4E830,0x4E860,0x4EC70,0x1A77B0,0x1A39B0}
# Descriptor callbacks installed by D110/D7A0/DAB0: enumerate guest model
# entries and return their blend weights (the first two clamp via 23570).
INDIRECT_ROOTS |= {0x4D1F0,0x4D880,0x4DB40}


def remove_unused_flag_reads(body):
    # Lifter bookkeeping is not an x86 load. A copied cmp operand can refer to
    # a register already overwritten by the time the bookkeeping is emitted.
    # Only erase assignments whose local has NO read anywhere in this helper.
    # Task-root locals are excluded: they are part of captured continuations.
    clean = re.sub(r"/\*.*?\*/|//[^\n]*", "",body,flags=re.S)
    for name in set(re.findall(r"\b_fs\d+\b",clean)):
        assignments = list(re.finditer(r"^    "+name+r" = .*?; /\* flag operand kept for a later jcc \*/\n",body,re.M))
        if not assignments:
            continue
        # One token in the declaration, plus exactly the assignments.
        if len(re.findall(r"\b"+name+r"\b",clean)) == 1+len(assignments):
            for match in reversed(assignments):
                body=body[:match.start()]+body[match.end():]
    return body


def generate(gen, out):
    bodies = {}
    for file in sorted(gen.glob("recomp_*.c")):
        for m in re.finditer(r"^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\s*\{(.*?)^\}",
                             file.read_text(), re.M | re.S):
            bodies[int(m[1],16)] = m[2]
    manual_source = (gen.parent / "recomp_manual.c").read_text()
    manual = {int(x,16) for x in re.findall(r"^void sub_([0-9A-F]{8})\(void\)", manual_source, re.M)}
    # The carousel patch relocates descriptors using a host-owned base address.
    # Do not invent that address or quietly execute the unpatched menu path.
    host_menu = {va for va,body in bodies.items() if re.search(r"\bg_online_menu_descriptors\b",body)}
    if host_menu & set(TASKS):
        raise ValueError("task root now requires uncaptured Online menu state")
    # A frame yield is translated explicitly. All other yields must abort.
    blocked = (manual - OBSERVERS - NATIVE_CAMERA - set(TASKS)) | host_menu | NATIVE_BOUNDARIES | {0x1C65D8,0x1C64B7,0x1C64C7}
    selected = set(TASKS) | INDIRECT_ROOTS
    pending = list(selected)
    while pending:
        va = pending.pop()
        for target in re.findall(r"\bsub_([0-9A-F]{8})\(", bodies[va]):
            child = int(target,16)
            if child in bodies and child not in selected and child not in blocked:
                selected.add(child); pending.append(child)
    out.mkdir(parents=True, exist_ok=True)
    header = (gen.parent / "recomp_types.h").read_text()
    header = header.replace("#include <stdint.h>",
                            '#include <stdint.h>\n#include "shadow_runtime.h"')
    start = header.index("static __forceinline uintptr_t xbox_ptr_resolve")
    end = header.index("#define XBOX_PTR",start)
    header = header[:start] + ("static __forceinline uintptr_t xbox_ptr_resolve(uint32_t addr)\n"
                              "{ return rb_ptr(addr,16); }\n") + header[end:]
    (out / "shadow_types.h").write_text(header)
    parts = ['#define RECOMP_GENERATED_CODE\n#include "shadow_types.h"\n'
             '#include "rollback_tasks.h"\n'
             '#undef RECOMP_ICALL\n#undef RECOMP_ICALL_SAFE\n#undef RECOMP_ITAIL\n'
             '#define RECOMP_ICALL(va) rb_call((uint32_t)(va))\n'
             '#define RECOMP_ICALL_SAFE(va,sp) rb_call((uint32_t)(va))\n'
             '#define RECOMP_ITAIL(va) rb_call((uint32_t)(va))\n']
    parts.append('const unsigned char rb_source_digest[32] = {' +
                 ','.join(str(x) for x in source_digest(gen)) + '};\n')
    layout = (gen / "xbe_layout.h").read_text()
    count = int(re.search(r"#define DOA3_KERNEL_THUNK_COUNT\s+(\d+)", layout)[1])
    table = re.search(r"#define DOA3_KERNEL_THUNK_ORDINALS(.*?)(?:\n\s*\n)", layout, re.S)[1]
    ordinals = re.findall(r"\b\d+\b", table)
    if len(ordinals) != count:
        raise ValueError("kernel thunk count differs from ordinal table")
    parts.append(f'const uint32_t rb_kernel_count = {count};\n'
                 'const uint32_t rb_kernel_ordinals[] = {' + ','.join(ordinals) + '};\n')
    (out / "source_digest.bin").write_bytes(source_digest(gen))
    flags = re.search(r"^int rc_flg_cc\(int cc\)\n\{.*?^\}", manual_source, re.M | re.S)
    if not flags:
        raise ValueError("missing native flag evaluator")
    parts.append(flags[0])
    for va in sorted(selected - set(TASKS)):
        parts.append(f"static void shadow_{va:08X}(void);\n")
    for va in sorted(selected):
        body = bodies[va]
        if va in NESTED_ROOTS:
            continue
        if va in TASKS:
            source = transform(va,body)
        else:
            body = remove_unused_flag_reads(body)
            source = f"static void shadow_{va:08X}(void)\n{{{body}}}\n"
        source = re.sub(r"sub_([0-9A-F]{8})\(\)",r"rb_call(0x\1u)",source)
        source = source.replace("__debugbreak()", 'rb_abort("debugbreak",0)')
        source = re.sub(r"\b(memcpy|memmove|memset)\(",r"rb_\1(",source)
        source = re.sub(r"^(loc_([0-9A-F]+): ;)",r"\1 rb_site(0x\2u);",source,flags=re.M)
        parts.append(source)
    nested_source=generate_match(bodies,manual)+nested_step_source()
    nested_source=re.sub(r"sub_([0-9A-F]{8})\(\)",r"rb_call(0x\1u)",nested_source)
    nested_source=re.sub(r"^(loc_([0-9A-F]+): ;)",r"\1 rb_site(0x\2u);",nested_source,flags=re.M)
    nested_source=re.sub(r"\b(memcpy|memmove|memset)\(",r"rb_\1(",nested_source)
    parts.append(nested_source)
    parts.append("void rb_dispatch(uint32_t va) { switch(va) {\n")
    for va in sorted(selected - set(TASKS)):
        guard = ('if (!rb_native_camera) rb_abort("native_camera_profile_required",va); '
                 if va in NATIVE_CAMERA else '')
        parts.append(f"case 0x{va:08X}u: {guard}shadow_{va:08X}(); return;\n")
    parts.append('default: rb_abort("unsupported_call",va); } }\n')
    parts.append("void rb_step(uint32_t va, Doa3RollbackTask *task) { task->yielded=0; switch(va) {\n")
    parts.extend(f"case 0x{va:08X}u: step_{va:08X}(task); return;\n" for va in TASKS)
    parts.append('default: rb_abort("unsupported_task",va); } }\n')
    source = "\n".join(parts)
    # Calls must resolve to this translation or a known local runtime/helper.
    helper_names = set(re.findall(r"#define\s+(\w+)\(",header))
    helper_names |= set(re.findall(r"(?:inline|__inline|__forceinline)\s+\w+\s+(\w+)\(",header))
    allowed = helper_names | {"if","while","for","switch","sizeof","return",
        "rb_call","rb_budget","rb_site","rb_abort","rb_dispatch","rb_step","rb_memcpy","rb_memmove","rb_memset",
        "doa3_rollback_task_sleep","doa3_guest_rdtsc","fp_push","fp_pop","fp_popp","fp_top","fp_st1",
        "sqrt","sqrtf","sin","sinf","cos","cosf","tan","atan","atan2","atan2f",
        "fabs","fabsf","floor","ceil","trunc","rint","remainder","pow","log","log10","exp",
        "memcpy","memmove","memset","rc_flg_cc","doa3_nested_init","doa3_nested_advance","doa3_debug_unlock_read8",
        "doa3_save_accept_compatible"}
    clean = re.sub(r"/\*.*?\*/|//[^\n]*", "",source,flags=re.S)
    unknown = {n for n in re.findall(r"\b(\w+)\s*\(",clean)
               if n not in allowed and not n.startswith(("shadow_","step_","nested_","xmm_","mmx_"))}
    if unknown:
        raise ValueError(f"unaudited native calls: {sorted(unknown)}")
    (out / "shadow.generated.c").write_text(source)
    print(f"Generated {len(selected)} isolated guest functions")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--gen",type=Path,required=True)
    parser.add_argument("--out",type=Path,required=True)
    args = parser.parse_args()
    generate(args.gen,args.out)
