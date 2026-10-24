"""Generate explicit continuations from the existing 3.1 fight task bodies.

Single-yield roots use saved locals; the two flow roots use the checked nested
call graph. This does not rewrite their combat instructions or original bodies.
Unknown local declarations/yield patterns fail generation instead of silently
losing live C state. Generated output belongs in the build directory.
"""
from pathlib import Path
import argparse
import re
import hashlib
try:
    from .rollback_workers import generate_workers
    from .rollback_guest_render import verify as verify_guest_render
    from .rollback_match import generate_match, suspension_graph, ROOTS as NESTED_ROOTS
except ImportError:
    from rollback_workers import generate_workers
    from rollback_guest_render import verify as verify_guest_render
    from rollback_match import generate_match, suspension_graph, ROOTS as NESTED_ROOTS

# Dispatcher order is part of the consecutive-capture contract.
TASKS = (0x87420, 0x72070, 0x54110, 0xBE4A0, 0x11110, 0xB4720, 0xA00D0, 0x98DB0, 0x7F470, 0x235B0, 0x1C60AB, 0xE8030, 0x49790, 0x691F0, 0xBB500)
YIELD = "sub_001C65D8(); /* call 0x001C65D8 */"


def source_digest(directory):
    digest = hashlib.sha256()
    files = sorted(directory.glob("recomp_*.c")) + [directory / "xbe_layout.h",
        directory.parent / "recomp_types.h", directory.parent / "recomp_manual.c", Path(__file__)]
    # Host adapters and the APU snapshot ABI must agree with the capture too.
    root=directory.parents[2]
    files += [root/"kernel"/name for name in ("kernel_rtl.c","kernel_hal.c","kernel_bridge.c","xbox_fiber.c","xbox_fiber.h","kernel_apc.c","kernel_apc.h","kernel_interrupt.c","kernel_interrupt.h","kernel_stream.c","kernel_stream.h","xbox_heap.c","xbox_heap.h","xbox_memory_layout.c","xbox_memory_layout.h")]
    files += [root/"apu"/name for name in ("apu_core.c","apu_vp.c","apu_state.h","apu_regs.h",
        "apu_shim.h","apu_mmio_hook.c","apu_rollback.c","apu_rollback.h","apu_chip.h",
        "apu_live_checkpoint.inc","apu_live_checkpoint.h","apu_dsp.c","apu_xaudio2.c","apu_xaudio2.h")]
    files += [root/"online/rollback_capture.h", root/"online/rollback_frame.h", root/"game/rollback_frame.c"]
    files += [root/"game/rollback_ownership.c"]
    files += [root/"game/rollback_state_audit.c", root/"game/rollback_state_audit.h"]
    files += [root/"game/rollback_input.c", root/"game/rollback_input.h"]
    files += [root/"game/rollback_input_tape.c", root/"game/rollback_input_tape.h", root/"game/rollback_input_audit.c"]
    files += [root/"nv2a"/name for name in ("nv2a_queue.h","nv2a_frontend.c","nv2a_frontend.h","nv2a_submission.h","nv2a_targets.h","nv2a_pfifo_registers.h","nv2a_regs.h",
        "nv2a_core.c","nv2a_mmio_hook.c","nv2a_mmio_hook.h","nv2a_pgraph_d3d11.c","nv2a_pgraph_d3d11.h","nv2a_geometry_cache.h",
        "nv2a_pgraph_state.c","nv2a_pgraph_state.h","nv2a_renderer_checkpoint.c","nv2a_renderer_checkpoint.h",
        "nv2a_vertex_program.c","nv2a_vertex_program.h","nv2a_texture_signature.h")]
    files += [root/"online"/name for name in ("rollback_nested.c","rollback_nested.h","rollback_tasks.h","rollback_workers.h",
        "rollback_checkpoint.c","rollback_checkpoint.h","xbox_det.c","xbox_det.h")]
    files += [root/"game/rollback_workers.c"]
    files += [root/"game/rollback_guest_render.inc",root/"game/rollback_native_frame.c",
              root/"game/rollback_native_frame.h",root/"game/rollback_fight_window.c"]
    files += [root/"d3d"/name for name in ("d3d8_device.c","d3d8_resources.c","d3d8_nv2aff.h",
        "d3d8_swizzle.h","d3d8_replay_images.c","d3d8_replay_images.h","d3d8_render_checkpoint.inc",
        "d3d8_pipeline_checkpoint.h","d3d8_pipeline_checkpoint.inc","d3d8_internal.h",
        "d3d8_shaders.c","d3d8_states.c","d3d8_combiners.c","d3d8_combiners.h","d3d8_vsh.c","d3d8_vsh.h")]
    files += [Path(__file__).with_name(name) for name in ("rollback_nested.py","rollback_match.py","rollback_boundaries.py","rollback_workers.py","rollback_guest_render.py")]
    for file in files:
        if file.exists():
            digest.update(file.name.encode())
            digest.update(file.read_bytes().replace(b"\r\n", b"\n"))
    return digest.digest()


def transform(va, body):
    if body.count(YIELD) != 1:
        raise ValueError(f"{va:08X}: expected exactly one direct frame yield")
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
            raise ValueError(f"{va:08X}: unsupported local declaration {clean!r}")
        for part in match[2].split(","):
            name = re.fullmatch(r"\s*([A-Za-z_]\w*)(?:\s*=\s*(0|g_seh_ebp))?\s*", part)
            if not name:
                raise ValueError(f"unsupported local {part!r}")
            if name[2] == "g_seh_ebp" and (name[1] != "ebp" or match[1] != "uint32_t"):
                raise ValueError(f"unsupported frame initializer {part!r}")
            names.append((name[1], match[1]))
    if len(names) > 32:
        raise ValueError("too many continuation locals")
    # Locals inside a nested instruction block do not survive the direct yield.
    # Root-scope declarations after the entry label would require a new audit.
    if re.search(r"^    (?:uint\w*_t|int|float|double|void|char) \w", code, re.M):
        raise ValueError("unexpected root-scope local after entry")
    resume = "    if (task->resume) {\n"
    for index, (name, kind) in enumerate(names):
        resume += f"        {name} = ({kind})task->locals[{index}];\n"
    resume += "        goto rollback_resume;\n    }\n"
    suspend = "doa3_rollback_task_sleep();\n"
    for index, (name, _) in enumerate(names):
        suspend += f"    task->locals[{index}] = (uint32_t){name};\n"
    suspend += "    task->resume = 1;\n    task->yielded = 1;\n    return;\nrollback_resume: ;"
    code = code.replace(YIELD, suspend)
    return f"static void step_{va:08X}(Doa3RollbackTask *task)\n{{{declarations}{resume}\n{code}}}\n"


def validate_task_hooks(manual_source, generated_overrides, overrides):
    for va in TASKS:
        wrapper = (f"void sub_{va:08X}(void) {{ if (!xbox_fiber_run_restorable_task(0x{va:X})) "
                   f"sub_{va:08X}_gen(); }}")
        if (wrapper not in manual_source or va not in generated_overrides or va not in overrides):
            raise ValueError(f"{va:08X} missing audited live task hook/override")


def generate(directory):
    fingerprint = ('const unsigned char doa3_rollback_source_digest[32] = {' +
                   ','.join(str(x) for x in source_digest(directory)) + '};\n')
    layout = (directory / "xbe_layout.h").read_text()
    if not re.search(r"^#define DOA3_XBE_ID_3_1\s+1\b", layout, re.M):
        return fingerprint + ('/* This experiment supports only DOA3 3.1. */\n'
                '#include "rollback_tasks.h"\n#include "rollback_workers.h"\n'
                'int doa3_rollback_worker_supported(uint32_t routine) { (void)routine; return 0; }\n'
                'int doa3_rollback_worker_step(uint32_t routine,Doa3RollbackWorker *worker) { (void)routine; (void)worker; return 0; }\n'
                'int doa3_rollback_worker_checkpoint_valid(uint32_t routine,const Doa3RollbackWorker *worker) { (void)routine; (void)worker; return 0; }\n'
                'int doa3_rollback_task_supported(uint32_t routine) { (void)routine; return 0; }\n'
                'int doa3_rollback_task_checkpoint_valid(uint32_t routine,const Doa3RollbackTask *task) { (void)routine; (void)task; return 0; }\n'
                'int doa3_rollback_task_step(uint32_t routine, Doa3RollbackTask *task) '
                '{ (void)routine; (void)task; return 0; }\n')
    bodies = {}
    generated_overrides = set()
    for file in sorted(directory.glob("recomp_*.c")):
        for match in re.finditer(r"^void sub_([0-9A-F]{8})(?:_gen)?\(void\)\n\{(.*?)^\}",
                                 file.read_text(), re.M | re.S):
            va = int(match[1], 16)
            if va in bodies:
                raise ValueError(f"duplicate routine {va:08X}")
            bodies[va] = match[2]
            if match[0].startswith(f"void sub_{va:08X}_gen(void)"):
                generated_overrides.add(va)
    missing = set(TASKS) - bodies.keys()
    if missing:
        raise ValueError(f"missing task bodies: {sorted(missing)}")
    result = fingerprint + ('/* Generated by tools/recomp/rollback_tasks.py; do not edit. */\n'
              '#define RECOMP_GENERATED_CODE\n#include "recomp_funcs.h"\n#include "rollback_tasks.h"\n'
              '#include "xbe_layout.h"\n#ifdef DOA3_XBE_ID_3_1\n')
    result += "\n".join(transform(va, bodies[va]) for va in TASKS if va not in NESTED_ROOTS)
    manual_source=(directory.parent/"recomp_manual.c").read_text()
    overrides={int(x,16) for x in re.findall(r"^0x([0-9A-Fa-f]+)\b",
        Path(__file__).with_name("overrides_3.1.txt").read_text(), re.M)}
    validate_task_hooks(manual_source,generated_overrides,overrides)
    verify_guest_render(bodies,generated_overrides,overrides,manual_source,
                        (directory/"recomp_probes.c").read_text())
    for va in (0x1926F0,0x192760,0x1927D0):
        wrapper=f"void sub_{va:08X}(void) {{ if (!xbox_fiber_run_restorable_stream_worker(0x{va:X})) sub_{va:08X}_gen(); }}"
        if wrapper not in manual_source or va not in generated_overrides or va not in overrides:
            raise ValueError(f"{va:08X}: missing streaming-worker entry hook")
    manual={int(x,16) for x in re.findall(r"^void sub_([0-9A-F]{8})\(void\)",manual_source,re.M)}
    # These display roots have only their direct frame yield in the audited
    # direct-call graph. Reject new nested waits instead of losing C locals.
    # Indirect/native suspension remains guarded by the production scheduler.
    for root in (0xE8030,0x49790,0x691F0):
        selected,_=suspension_graph(bodies,manual,root)
        if selected!={root}:
            raise ValueError(f"{root:08X} display suspension graph changed: {selected}")
    result += generate_match(bodies,manual)
    result += generate_workers(bodies,transform)
    result += nested_step_source()
    result += "\n#endif\nint doa3_rollback_task_supported(uint32_t routine)\n{\n"
    result += "#ifdef DOA3_XBE_ID_3_1\n    switch (routine) {\n"
    result += "".join(f"    case 0x{va:08X}u:\n" for va in TASKS)
    result += "        return 1;\n    }\n#endif\n    return 0;\n}\n"
    result += "int doa3_rollback_task_step(uint32_t routine, Doa3RollbackTask *task)\n{\n"
    result += "    if (!task || task->resume > 1) return 0;\n    task->yielded = 0;\n"
    result += "#ifdef DOA3_XBE_ID_3_1\n    switch (routine) {\n"
    result += "".join(f"    case 0x{va:08X}u: step_{va:08X}(task); return task->yielded == 1;\n" for va in TASKS)
    result += "    }\n#endif\n    return 0;\n}\n"
    result += """
int doa3_rollback_task_checkpoint_valid(uint32_t routine,const Doa3RollbackTask *task)
{
    if (!task || task->resume!=1 || task->yielded!=1 || !doa3_rollback_task_supported(routine)) return 0;
#ifdef DOA3_XBE_ID_3_1
    if (routine==0x72070 || routine==0x87420) {
        Doa3NestedStack checked;
        return task->nested.frames[0].routine==routine &&
            doa3_nested_capture(&task->nested,&checked,sizeof(checked),nested_validate);
    }
#endif
    const Doa3NestedStack empty={0};
    return !memcmp(&task->nested,&empty,sizeof(empty));
}
"""
    return result


def nested_step_source():
    return "\n".join(f"""
static void step_{root:08X}(Doa3RollbackTask *task)
{{
    if (!task->resume && !task->nested.schema &&
        !doa3_nested_init(&task->nested,0x{root:X},nested_validate)) return;
    int status=doa3_nested_advance(&task->nested,nested_dispatch,nested_validate,4096);
    task->resume=1;
    task->yielded=status==DOA3_NESTED_YIELDED;
}}
""" for root in NESTED_ROOTS)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--gen", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    text = generate(args.gen)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if not args.output.exists() or args.output.read_text() != text:
        args.output.write_text(text)


if __name__ == "__main__":
    main()
