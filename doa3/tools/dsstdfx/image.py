"""
XDK DirectSound effects image (dsstdfx.bin, XDK 4134) reader.

Layout, as DSOUND's IDirectSound_DownloadEffectsImage reads it
(sub_001F3A9F / sub_001F3832 on 3.1):

  0x000  0x800 bytes the file leaves zero (DSOUND's own use)
  0x800  header: +4 code words, +8 code end (bytes), +0xC state words,
         +0x10 command word (DSOUND writes 3 and waits for the DSP to clear it)
  0x818  GP DSP code, P:0 onwards, one 24-bit word per dword, encrypted
  ...    effect state blocks, X:0x80 onwards (code end + 0)
  desc   DSEFFECTIMAGEDESC at 0x818 + (code words + state words) * 4:
         dwEffectCount, dwTotalScratchSize, then per effect a DSEFFECTMAP
         (code, code size, state, state size, Y memory, Y size, scratch,
         scratch size; offsets into the image / DSP memory)
  keys   8 bytes per effect after the descriptor, themselves encrypted

Each effect's state block starts with dwScratchOffset, dwScratchLength,
dwYMemoryOffset, dwYMemoryLength, dwFlags, then its input and output mixbin
pointers. Mixbin n is X:0x1400 + 0x20 * n (the VP's 32 bins of 32 samples),
also reachable as X:0x0C00 + 0x20 * n.

The cipher is DSOUND's (sub_001F72ED, sub_001F7366, sub_001F7393): a 64-bit
LFSR seeded with 0x7FA49BCA:49DE12BA and stepped 81 times gives a master key,
which decrypts the per-effect keys, which decrypt each effect's code.
"""

import struct

M32 = 0xFFFFFFFF
MIXBIN_BASE = 0x1400
MIXBIN_ALIAS = 0x0C00
STATE_X_BASE = 0x80

MIXBIN_NAMES = {
    0: "FRONT_LEFT", 1: "FRONT_RIGHT", 2: "FRONT_CENTER", 3: "LOW_FREQUENCY",
    4: "BACK_LEFT", 5: "BACK_RIGHT", 6: "XTLK_FRONT_LEFT", 7: "XTLK_FRONT_RIGHT",
    8: "XTLK_BACK_LEFT", 9: "XTLK_BACK_RIGHT", 10: "I3DL2",
}


def mixbin_name(n):
    return MIXBIN_NAMES.get(n, "FXSEND_%d" % (n - 11))


class _Lfsr:
    def __init__(self):
        self.lo = self.hi = 0

    def step(self, lo=0, hi=0):
        """One step (seeded when lo:hi is non-zero); returns the new state."""
        if lo == 0 and hi == 0:
            lo, hi = self.lo, self.hi
        t = ((lo << 2) & M32) ^ lo
        t = ((t << 1) & M32) ^ lo
        t = ((t << 28) & M32) ^ hi
        self.lo = ((lo >> 1) | (hi << 31)) & M32
        self.hi = (hi >> 1) | (t & 0x80000000)
        return struct.pack("<II", self.lo, self.hi)


def _master_key(lfsr):
    lfsr.step(0x49DE12BA, 0x7FA49BCA)
    for _ in range(0x50):
        key = lfsr.step()
    return key


def _s8(b):
    return b - 256 if b & 0x80 else b


def _decrypt(data, key):
    lfsr = _Lfsr()
    k1 = _master_key(lfsr)
    lfsr.step(*struct.unpack("<II", k1))
    out = bytearray(len(data))
    for i, c in enumerate(data):
        p = (_s8(k1[i & 7]) * _s8(key[i & 7])) & 0xFF
        out[i] = k1[i & 7] ^ lfsr.step()[0] ^ ((c - p) & 0xFF)
    return bytes(out)


class Effect:
    def __init__(self, index, fields, state):
        (self.code, self.code_size, self.state, self.state_size,
         self.ymem, self.ymem_size, self.scratch, self.scratch_size) = fields
        self.index = index
        self.words = state

    @property
    def p_address(self):
        return (self.code - 0x818) // 4

    def mixbins(self):
        """The mixbin pointers after dwFlags, as bin numbers. The block does
        not say how many are inputs: that is in the effect's code."""
        bins = []
        for w in self.words[5:]:
            base = MIXBIN_BASE if w >= MIXBIN_BASE else MIXBIN_ALIAS
            if not (base <= w < base + 0x400 and (w - base) % 0x20 == 0):
                break
            bins.append((w - base) // 0x20)
        return bins


class Image:
    def __init__(self, data):
        self.data = bytearray(data)
        w = self.word
        self.code_words = w(0x804)
        self.state_words = w(0x80C)
        self.state_base = 0x818 + self.code_words * 4
        desc = self.state_base + self.state_words * 4
        self.count = w(desc)
        self.total_scratch = w(desc + 4)
        self.effects = []
        for i in range(self.count):
            fields = [w(desc + 8 + 32 * i + 4 * j) for j in range(8)]
            st = [w(fields[2] + 4 * k) for k in range(fields[3] // 4)]
            self.effects.append(Effect(i, fields, st))
        self.keys_at = desc + 8 + 32 * self.count
        self.decrypted = False

    def word(self, off):
        return struct.unpack_from("<I", self.data, off)[0]

    def state_x(self, fx):
        return STATE_X_BASE + (fx.state - self.state_base) // 4

    def decrypt(self):
        """Decrypt every effect's code in place."""
        if self.decrypted:
            return
        n = self.count
        keys = _decrypt(bytes(self.data[self.keys_at:self.keys_at + 8 * n]),
                        _master_key(_Lfsr()))
        for fx in self.effects:
            code = bytes(self.data[fx.code:fx.code + fx.code_size])
            self.data[fx.code:fx.code + fx.code_size] = _decrypt(code, keys[8 * fx.index:8 * fx.index + 8])
        self.decrypted = True

    def code(self):
        return [self.word(0x818 + 4 * i) & 0xFFFFFF for i in range(self.code_words)]

    def state(self):
        return [self.word(self.state_base + 4 * i) & 0xFFFFFF for i in range(self.state_words)]
