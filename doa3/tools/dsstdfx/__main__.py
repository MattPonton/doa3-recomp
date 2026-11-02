"""
dsstdfx.bin tools. Run from doa3/:

  py -3 -m tools.dsstdfx info    [image]            effect graph and state blocks
  py -3 -m tools.dsstdfx decrypt [image] out.bin    the image with its code decrypted
  py -3 -m tools.dsstdfx rooms   [image] [xbe] dir  each I3DL2 room's reverb state
                                                    (DSOUND's own calculator; needs unicorn)

The decrypted image is what tools/dsstdfx/dsp (xemu's DSP56300 core)
disassembles and runs. See README.md.
"""

import struct
import sys
from pathlib import Path

from .image import Image, mixbin_name

DEFAULT_IMAGE = "build/release/HDD/D/dsstdfx.bin"
DEFAULT_XBE = "build/release/HDD/D/default.xbe"

# what the game uses of the image (sub_000A5C60's pImageLoc: I3DL2 0, XTalk 1;
# the sound module's mixbins, 0xA54E1: XTalk 6-9, I3DL2 10, LFE)
KNOWN = {0: "I3DL2 reverb (24 kHz)", 1: "XTalk", 8: "I3DL2 reverb (24 kHz), fixed preset",
         9: "level meter (speaker bins 0-5)"}


def _frac(v):
    v &= 0xFFFFFF
    return (v - (1 << 24) if v & 0x800000 else v) / float(1 << 23)


def cmd_info(args):
    img = Image(Path(args[0] if args else DEFAULT_IMAGE).read_bytes())
    print("%d effects, code %d words, state %d words, scratch 0x%X bytes"
          % (img.count, img.code_words, img.state_words, img.total_scratch))
    for fx in img.effects:
        bins = fx.mixbins()
        print("\nFX%d %s" % (fx.index, KNOWN.get(fx.index, "")))
        print("  P:%04X +%d words  X:%04X +%d words  Y:%04X +%d  scratch %X +%X"
              % (fx.p_address, fx.code_size // 4, img.state_x(fx), fx.state_size // 4,
                 fx.ymem, fx.ymem_size, fx.scratch, fx.scratch_size))
        print("  flags %X, mixbins %s" % (fx.words[4], ", ".join("%d %s" % (b, mixbin_name(b)) for b in bins)))
        rest = fx.words[5 + len(bins):]
        for k, v in enumerate(rest, 5 + len(bins)):
            if v:
                print("  [%3d] %06X  %+.6f  %d" % (k, v & 0xFFFFFF, _frac(v), v & 0xFFFFFF))


def cmd_decrypt(args):
    src, dst = (args if len(args) == 2 else [DEFAULT_IMAGE] + args)
    img = Image(Path(src).read_bytes())
    img.decrypt()
    Path(dst).write_bytes(bytes(img.data))
    print("wrote %s" % dst)


def cmd_rooms(args):
    from .i3dl2 import room_states
    image, xbe, out = (args if len(args) == 3 else [DEFAULT_IMAGE, DEFAULT_XBE] + args)
    img = Image(Path(image).read_bytes())
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    for idx, params, state in room_states(img, Path(xbe).read_bytes()):
        (out / ("room%02d.state" % idx)).write_bytes(struct.pack("<%dI" % len(state), *state))
        f = struct.unpack("<12f", struct.pack("<12i", *params))
        print("room %2d: room %6d hf %6d decay %5.2f hfratio %.2f refl %6d %.3f reverb %6d %.3f diff %3.0f dens %3.0f"
              % (idx, params[0], params[1], f[3], f[4], params[5], f[6], params[7], f[8], f[9], f[10]))


def main(argv):
    cmds = {"info": cmd_info, "decrypt": cmd_decrypt, "rooms": cmd_rooms}
    if not argv or argv[0] not in cmds:
        print(__doc__)
        return 1
    cmds[argv[0]](argv[1:])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
