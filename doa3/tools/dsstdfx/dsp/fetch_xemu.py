"""
Downloads xemu's MCPX DSP sources (hw/xbox/mcpx/apu/dsp) at a fixed commit
into tools/dsstdfx/dsp/xemu/, for build.bat. They are not kept in this
repository: xemu is GPL-2.0-or-later. Run from doa3/:

  py -3 tools/dsstdfx/dsp/fetch_xemu.py
"""

import urllib.request
from pathlib import Path

COMMIT = "67cc79e663038d1f55448c0f566b37dde016adf6"   # xemu master, 2026-06-12
BASE = "https://raw.githubusercontent.com/xemu-project/xemu/%s/" % COMMIT
FILES = [
    "LICENSE",
    "hw/xbox/mcpx/apu/dsp/debug.h",
    "hw/xbox/mcpx/apu/dsp/dsp.c",
    "hw/xbox/mcpx/apu/dsp/dsp.h",
    "hw/xbox/mcpx/apu/dsp/dsp_c.c",
    "hw/xbox/mcpx/apu/dsp/dsp_dma.c",
    "hw/xbox/mcpx/apu/dsp/dsp_dma.h",
    "hw/xbox/mcpx/apu/dsp/dsp_dma_regs.h",
    "hw/xbox/mcpx/apu/dsp/dsp_internal.h",
    "hw/xbox/mcpx/apu/dsp/interp/dsp_cpu.c",
    "hw/xbox/mcpx/apu/dsp/interp/dsp_cpu.h",
    "hw/xbox/mcpx/apu/dsp/interp/dsp_cpu_regs.h",
    "hw/xbox/mcpx/apu/dsp/interp/dsp_dis.c.inc",
    "hw/xbox/mcpx/apu/dsp/interp/dsp_emu.c.inc",
]


def main():
    out = Path(__file__).resolve().parent / "xemu"
    for name in FILES:
        dst = out / name.replace("hw/xbox/mcpx/apu/dsp/", "")
        dst.parent.mkdir(parents=True, exist_ok=True)
        with urllib.request.urlopen(BASE + name) as r:
            dst.write_bytes(r.read())
        print(dst.relative_to(out))
    (out / "COMMIT").write_text(COMMIT + "\n")


if __name__ == "__main__":
    main()
