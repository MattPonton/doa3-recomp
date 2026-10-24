from pathlib import Path
import sys
import tempfile
import re
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.recomp.rollback_tasks import transform, generate, TASKS, validate_task_hooks

# The native scheduler/capture and Python generator/probe must agree on order.
header=(Path(__file__).resolve().parents[2]/"src/online/rollback_tasks.h").read_text()
assert int(re.search(r"#define DOA3_RB_TASK_COUNT (\d+)u",header)[1])==len(TASKS)
values=re.search(r"#define DOA3_RB_TASK_ROUTINES (.*)",header)[1]
assert tuple(int(value,16) for value in values.split(','))==TASKS

source = Path(__file__).with_name("task_fixture.inc").read_text()
body = source[source.index("{") + 1:source.rindex("}")]
out = Path(sys.argv[1])
out.write_text(transform(0x1000, body))

# Fail-closed behavior when the source no longer fits the audited translation.
for bad in [body.replace("uint32_t ebp;", "uint64_t ebp;"),
            body.replace("uint32_t ebp;", "uint32_t ebp[2];"),
            body.replace("sub_001C65D8(); /* call 0x001C65D8 */", "sub_001C64B7();"),
            body.replace("loc_00001020: ;", "    int extra = 0;\nloc_00001020: ;")]:
    try:
        transform(0x1000, bad)
    except ValueError:
        pass
    else:
        raise AssertionError("unsupported continuation was accepted")
generate(Path(__file__).resolve().parents[2] / "src/game/recomp/gen")
manual=(Path(__file__).resolve().parents[2]/"src/game/recomp/recomp_manual.c").read_text()
validate_task_hooks(manual,set(TASKS),set(TASKS))
for text,generated,overrides in (
        (manual.replace("xbox_fiber_run_restorable_task(0x49790)","missing_hook(0x49790)"),set(TASKS),set(TASKS)),
        (manual,set(TASKS)-{0x49790},set(TASKS)),
        (manual,set(TASKS),set(TASKS)-{0x49790})):
    try: validate_task_hooks(text,generated,overrides)
    except ValueError: pass
    else: raise AssertionError("missing live task hook accepted")
with tempfile.TemporaryDirectory() as temp:
    directory = Path(temp)
    (directory / "xbe_layout.h").write_text("#define DOA3_XBE_ID_3_0 1\n")
    assert "step_00011110" not in generate(directory)
