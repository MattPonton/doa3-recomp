"""
XBE-driven layout for the recompilation pipeline.

Upstream hardcodes the DOA3 3.0 (NTSC-U) layout in each tool's config.py.
This module reads the layout from whichever XBE the pipeline is pointed at,
so the same tools can process 3.0, 3.1 (NTSC-J), 3.2 (PAL) and 3++.

Select the XBE with the DOA3_XBE environment variable. If it is unset, the
first of the default locations that exists is used. If none exists, the
values fall back to the 3.0 layout upstream was written for.
"""

import hashlib
import os
import struct
from pathlib import Path

_DEFAULT_PATHS = [
    "build/release/assets/default.xbe",
    "assets/default.xbe",
]

KNOWN_VERSIONS = {
    "f502d8a89e5cda7df2e74b5d8e159b71": "3.0",
    "4d1a10a16b8ad2065ad9dee65e951f08": "3.1",
    "045481052505d9cc73f89d7d4ca41f3b": "3.2",
    "1ad1504f2866ac07449478649cf920b7": "3pp-1.48",
}

# Upstream 3.0 values, used when no XBE is available.
_FALLBACK_30 = dict(
    version="3.0", path=None, md5=None, base=0x00010000, image_size=0x00C21500,
    entry=0x001651A5, thunk=0x001ED0E0,
    sections=[
        # name, va, vsize, raw_addr, raw_size, flags
        (".text", 0x00011000, 0x0019FDE0, 0x00001000, 0x0019FDE0, 0x16),
        ("D3D", 0x001B0DE0, 0x000127CC, 0x001A1000, 0x0000F36C, 0x7),
        ("D3DX", 0x001C35C0, 0x00002EC8, 0x001B1000, 0x00002EC0, 0x7),
        ("XGRPH", 0x001C64A0, 0x000005FC, 0x001B4000, 0x000005FC, 0x7),
        ("DSOUND", 0x001C6AA0, 0x0001AFE8, 0x001B5000, 0x0001AEB4, 0x7),
        ("PSGSFD00", 0x001E1AA0, 0x00003190, 0x001D0000, 0x00003190, 0x26),
        ("PSGSFD_I", 0x001E4C40, 0x000001F0, 0x001D4000, 0x000001F0, 0x36),
        ("PSGSFD_B", 0x001E4E40, 0x000006C0, 0x001D5000, 0x000006C0, 0x16),
        ("PSGSFD_P", 0x001E5500, 0x00000430, 0x001D6000, 0x00000430, 0x6),
        ("XPP", 0x001E5940, 0x0000779C, 0x001D7000, 0x0000779C, 0x7),
        (".rdata", 0x001ED0E0, 0x0002C54C, 0x001DF000, 0x0002C53C, 0x6),
        (".data", 0x00219640, 0x00A0E91C, 0x0020C000, 0x001A8450, 0x7),
        ("DOLBY", 0x00C27F60, 0x00006D98, 0x003B5000, 0x00006D84, 0x26),
        ("$$XTIMAGE", 0x00C2ED00, 0x00002800, 0x003BC000, 0x00002800, 0x38),
    ],
    seh_prolog=0x0018F494, seh_epilog=0x0018F4CD, ftol2=0x0018DB30,
)

# CRT helpers the lifter special-cases, located by byte signature.
# None = wildcard byte (absolute addresses that move between builds).
_SIG_SEH_PROLOG = [0x68, None, None, None, None, 0x64, 0xA1, 0, 0, 0, 0, 0x50,
                   0x64, 0x89, 0x25, 0, 0, 0, 0, 0x8B, 0x44, 0x24, 0x10]
_SIG_SEH_EPILOG = bytes.fromhex("8b4df064890d00000000595f5e5bc951c3")
_SIG_FTOL2 = bytes.fromhex("558bec83ec2083e4f0d9c0d9542418df7c2410df6c2410")


def _find_masked(data, sig, start, end):
    first = sig[0]
    i = data.find(bytes([first]), start, end)
    n = len(sig)
    while 0 <= i < end - n:
        if all(s is None or data[i + k] == s for k, s in enumerate(sig)):
            return i
        i = data.find(bytes([first]), i + 1, end)
    return -1


def _parse(path):
    d = Path(path).read_bytes()
    u = lambda o: struct.unpack_from("<I", d, o)[0]
    if d[:4] != b"XBEH":
        raise ValueError(f"{path} is not an XBE")
    base = u(0x104)
    off = lambda va: va - base
    secs = []
    for i in range(u(0x11C)):
        o = off(u(0x120)) + i * 56
        fl, va, vs, ra, rs, na = struct.unpack_from("<6I", d, o)
        name = d[off(na):d.index(b"\0", off(na))].decode()
        secs.append((name, va, vs, ra, rs, fl))
    md5 = hashlib.md5(d).hexdigest()
    text = next(s for s in secs if s[0] == ".text")

    def va_of(raw):
        for s in secs:
            if s[3] <= raw < s[3] + s[4]:
                return s[1] + raw - s[3]
        return None

    lo, hi = text[3], text[3] + text[4]
    p = _find_masked(d, _SIG_SEH_PROLOG, lo, hi)
    e = d.find(_SIG_SEH_EPILOG, lo, hi)
    f = d.find(_SIG_FTOL2, lo, hi)
    return dict(
        version=KNOWN_VERSIONS.get(md5, "unknown"), path=str(path), md5=md5,
        base=base, image_size=u(0x10C),
        entry=u(0x128) ^ 0xA8FC57AB, thunk=u(0x158) ^ 0x5B6D40B6,
        sections=secs,
        seh_prolog=va_of(p) if p >= 0 else None,
        seh_epilog=va_of(e) if e >= 0 else None,
        ftol2=va_of(f) if f >= 0 else None,
    )


def _locate():
    env = os.environ.get("DOA3_XBE")
    if env:
        return _parse(env)
    for cand in _DEFAULT_PATHS:
        if Path(cand).exists():
            return _parse(cand)
    return dict(_FALLBACK_30)


LAYOUT = _locate()

VERSION = LAYOUT["version"]
XBE_PATH = LAYOUT["path"]
XBE_BASE_ADDRESS = LAYOUT["base"]
XBE_IMAGE_SIZE = LAYOUT["image_size"]
ENTRY_POINT = LAYOUT["entry"]
KERNEL_THUNK_ADDR = LAYOUT["thunk"]
SEH_PROLOG = LAYOUT["seh_prolog"]
SEH_EPILOG = LAYOUT["seh_epilog"]
FTOL2 = LAYOUT["ftol2"]

_S = {s[0]: s for s in LAYOUT["sections"]}
DATA_SECTION_NAMES = (".rdata", ".data", "$$XTIMAGE")


def section(name):
    """(name, va, vsize, raw_addr, raw_size, flags)"""
    return _S[name]


TEXT_VA_START = _S[".text"][1]
TEXT_VA_SIZE = _S[".text"][2]
TEXT_VA_END = TEXT_VA_START + TEXT_VA_SIZE
TEXT_RAW_ADDR = _S[".text"][3]
RDATA_VA_START = _S[".rdata"][1]
RDATA_VA_SIZE = _S[".rdata"][2]
RDATA_VA_END = _S[".data"][1]
RDATA_RAW_ADDR = _S[".rdata"][3]
DATA_VA_START = _S[".data"][1]
DATA_VA_END = XBE_BASE_ADDRESS + XBE_IMAGE_SIZE

# Executable sections in address order, excluding data sections.
EXECUTABLE_SECTIONS = [(s[0], s[1], s[2]) for s in LAYOUT["sections"]
                       if s[0] not in DATA_SECTION_NAMES]
DATA_SECTIONS = [(s[0], s[1], s[2]) for s in LAYOUT["sections"]
                 if s[0] in DATA_SECTION_NAMES]
# (name, va_start, raw_size, raw_addr) as tools/recomp/config.py expects.
RAW_SECTIONS = [(s[0], s[1], s[4], s[3]) for s in LAYOUT["sections"]]


def describe():
    lines = [f"DOA3 layout: version {VERSION}  ({XBE_PATH or 'built-in 3.0 fallback'})",
             f"  entry 0x{ENTRY_POINT:08X}  thunks 0x{KERNEL_THUNK_ADDR:08X}  "
             f"image 0x{XBE_BASE_ADDRESS:08X}+0x{XBE_IMAGE_SIZE:X}"]
    for nm, val in (("SEH_prolog", SEH_PROLOG), ("SEH_epilog", SEH_EPILOG), ("__ftol2", FTOL2)):
        lines.append(f"  {nm:11} {'0x%08X' % val if val else 'NOT FOUND'}")
    return "\n".join(lines)


if __name__ == "__main__":
    print(describe())
