"""Run every generated-code fix-up, in order, after `tools.recomp`.

Each step is idempotent and re-applies upstream's fixes for a known lifter
defect class (see NOTES.md, "Lifter Defect Reference"). Run from doa3/ with
DOA3_XBE set to the XBE the code was generated from:

  py -3 -m tools.recomp.postprocess
"""
import os
import subprocess
import sys

STEPS = [
    ("restore dropped fall-throughs", [sys.executable, "tools/recomp/fix_fallthroughs.py"]),
    ("inline __ftol2 call sites", [sys.executable, "tools/recomp/fix_ftol_inline.py"]),
    ("sync g_seh_ebp on conditional tail-calls", [sys.executable, "tools/recomp/fix_cond_tailcall_ebp.py"]),
    ("global x87 stack", [sys.executable, "tools/recomp/fix_fpu_global.py"]),
    ("deferred compares", [sys.executable, "-m", "tools.recomp.fix_deferred_cmp"]),
    ("hardware busy-wait self-spins", [sys.executable, "tools/recomp/fix_selfspins.py"]),
]


def main():
    if not os.environ.get("DOA3_XBE"):
        sys.exit("set DOA3_XBE to the XBE the code was generated from")
    for title, cmd in STEPS:
        print(f"== {title}", flush=True)
        r = subprocess.run(cmd)
        if r.returncode:
            sys.exit(f"step failed: {title}")


if __name__ == "__main__":
    main()
