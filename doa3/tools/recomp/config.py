"""
Recompiler configuration - section mappings and constants.

Target: Dead or Alive 3 (Title ID 0x54430001), Retail, XDK 3911.
Values from: py -3 tools/xbe_parser/xbe_parser.py ../doa3gamefiles/default.xbe

NOTE: the size field below is the *raw* (on-disk) size so that
va_to_file_offset never points past the end of the XBE file. BSS tails
(virtual size > raw size, e.g. in .data) resolve to None and are treated
as zero-initialized, which matches Xbox load behavior.
"""

# Layout is read from the XBE selected by DOA3_XBE (see tools/xbe_layout.py).
from tools import xbe_layout as _L

# Section virtual address -> file offset mappings
# (name, va_start, raw_size, raw_addr)
SECTIONS = _L.RAW_SECTIONS

TEXT_VA_START = _L.TEXT_VA_START
TEXT_VA_END = _L.TEXT_VA_END
RDATA_VA_START = _L.RDATA_VA_START
RDATA_VA_END = _L.RDATA_VA_END
DATA_VA_START = _L.DATA_VA_START
DATA_VA_END = _L.DATA_VA_END
KERNEL_THUNK_ADDR = _L.KERNEL_THUNK_ADDR
ENTRY_POINT = _L.ENTRY_POINT
SEH_PROLOG = _L.SEH_PROLOG
SEH_EPILOG = _L.SEH_EPILOG
FTOL2 = _L.FTOL2


def va_to_file_offset(va):
    """Convert virtual address to XBE file offset."""
    for _, sec_va, sec_size, sec_raw in SECTIONS:
        if sec_va <= va < sec_va + sec_size:
            return va - sec_va + sec_raw
    return None


def is_code_address(va):
    """Check if VA is in an executable section (.text or XDK library sections)."""
    if TEXT_VA_START <= va < TEXT_VA_END:
        return True
    # XDK library sections also contain executable code
    for name, sec_va, sec_size, _ in SECTIONS:
        if name in (".text", ".rdata", ".data", "$$XTIMAGE"):
            continue  # skip data sections
        if sec_va <= va < sec_va + sec_size:
            return True
    return False


def is_data_address(va):
    """Check if VA is in .rdata or .data (including BSS)."""
    return RDATA_VA_START <= va <= DATA_VA_END
