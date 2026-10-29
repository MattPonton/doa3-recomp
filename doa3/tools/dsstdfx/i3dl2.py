"""
Each I3DL2 room's reverb state, computed by DSOUND itself.

IDirectSound_SetI3DL2Listener stores the twelve values, and the commit
(sub_001F40FD) reads the reverb's state words 0-69 (GetEffectData 0, 0x118),
runs sub_001F828D and sub_001F874E over a copy laid out as
[this] -> listener values, this+4 -> state word 0, and writes words 70-135
back (SetEffectData 0x118, 0x108). Here that x86 code runs unchanged from the
XBE under Unicorn (pip install unicorn), so the words are what the console's
DSOUND would write.
"""

import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP

CALC = 0x1F874E          # sub_001F874E (3.1)
ROOM_TABLE = 0x2D3EB8    # DSI3DL2LISTENER[50]: the 23 presets, "no reverb", 26 rooms
ROOM_COUNT = 50
OBJ, LISTENER, STACK, RET = 0xE00000, 0xE10000, 0xEF0000, 0xEFFF00


def _load_xbe(xbe):
    u = lambda o: struct.unpack_from("<I", xbe, o)[0]
    base = u(0x104)
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    mu.mem_map(0, 0x1000000)
    for i in range(u(0x11C)):
        h = u(0x120) - base + 0x38 * i
        mu.mem_write(u(h + 4), xbe[u(h + 12):u(h + 12) + u(h + 16)])
    return mu


def room_states(img, xbe):
    """Yields (index, listener values, FX0 state words) for every room."""
    mu = _load_xbe(xbe)
    fx = img.effects[0]
    state0 = list(fx.words)
    for idx in range(ROOM_COUNT):
        params = list(struct.unpack("<12i", bytes(mu.mem_read(ROOM_TABLE + 0x30 * idx, 0x30))))
        mu.mem_write(LISTENER, struct.pack("<12i", *params))
        mu.mem_write(OBJ, struct.pack("<I", LISTENER))
        mu.mem_write(OBJ + 4, struct.pack("<136I", *state0[:136]))
        mu.mem_write(STACK - 4, struct.pack("<I", RET))
        mu.reg_write(UC_X86_REG_ESP, STACK - 4)
        mu.reg_write(UC_X86_REG_ECX, OBJ)
        mu.emu_start(CALC, RET)
        words = struct.unpack("<136I", bytes(mu.mem_read(OBJ + 4, 136 * 4)))
        state = state0[:]
        state[70:136] = [w & 0xFFFFFF for w in words[70:136]]
        # the commit also sets flag bit 2 (new parameters: the DSP cross-fades
        # to them over 336 frames); left clear, the first frame starts on them
        yield idx, params, state
