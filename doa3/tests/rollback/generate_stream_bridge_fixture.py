"""Extract the production file table, checkpoint bridge and directory query.

The fixture supplies guest memory/clock plumbing; file and enumeration calls
remain real Win32 operations. No live game data is used.
"""
from pathlib import Path
import sys

source = (Path(__file__).parents[2] / 'src/kernel/kernel_bridge.c').read_text(encoding='utf-8')

def section(start, end):
    a = source.index(start)
    return source[a:source.index(end, a)]

parts = [
    section('#define XFH_TAG', 'static void bridge_write_handle'),
    section('static XboxApcQueue s_apc_queue;', '/* Diagnostic counters, not part of restored state. */'),
    section('#define QDIR_MAX_ENUM', '/* ── NtOpenSymbolicLinkObject'),
]
Path(sys.argv[1]).write_text('\n'.join(parts), encoding='utf-8')
