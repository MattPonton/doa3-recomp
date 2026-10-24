"""Compile the production IRQ delivery body with private guest callbacks."""
from pathlib import Path
import sys
source = (Path(__file__).parents[2] / "src/game/recomp/recomp_manual.c").read_text(encoding="utf-8")
start = source.index("void doa3_apu_deliver_irq(void)")
end = source.index("\nvolatile int g_doa3_post_movie", start)
body = source[start:end]
assert body.count("xbox_kernel_interrupt_begin()") == 1
assert body.count("xbox_kernel_interrupt_end();") == 2
Path(sys.argv[1]).write_text(body, encoding="utf-8")
