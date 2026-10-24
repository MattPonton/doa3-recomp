"""Read-only Windows probe for the experimental fight-task continuations.

Use the executable and linker map from the running build. This does not take
screenshots, write game memory, enable rollback, or prove replay determinism.
"""
import argparse
import ctypes as c
from ctypes import wintypes as w
import json
from pathlib import Path
import re
import struct
import time
import sys
sys.path.insert(0,str(Path(__file__).resolve().parent / "recomp"))
from rollback_tasks import TASKS


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pid", type=int)
    parser.add_argument("--exe", type=Path,
                        default=Path(__file__).resolve().parents[1] / "build/release/DOA3.exe")
    parser.add_argument("--seconds", type=float, default=2)
    args = parser.parse_args()
    if not 0 <= args.seconds <= 30:
        parser.error("--seconds must be between 0 and 30")
    kernel = c.WinDLL("kernel32", use_last_error=True)
    psapi = c.WinDLL("psapi", use_last_error=True)
    kernel.OpenProcess.argtypes = [w.DWORD, w.BOOL, w.DWORD]
    kernel.OpenProcess.restype = w.HANDLE
    kernel.CloseHandle.argtypes = [w.HANDLE]
    kernel.ReadProcessMemory.argtypes = [w.HANDLE, c.c_void_p, c.c_void_p,
                                         c.c_size_t, c.POINTER(c.c_size_t)]
    psapi.EnumProcessModulesEx.argtypes = [w.HANDLE, c.c_void_p, w.DWORD,
                                         c.POINTER(w.DWORD), w.DWORD]
    handle = kernel.OpenProcess(0x410, False, args.pid)  # query + read only
    if not handle:
        raise c.WinError(c.get_last_error())
    try:
        def read(address, size):
            buffer = c.create_string_buffer(size)
            count = c.c_size_t()
            if not kernel.ReadProcessMemory(handle, address, buffer, size, c.byref(count)):
                raise c.WinError(c.get_last_error())
            if count.value != size:
                raise RuntimeError("Short process read")
            return buffer.raw

        modules = (w.HMODULE * 1024)()
        count = w.DWORD()
        if not psapi.EnumProcessModulesEx(handle, modules, c.sizeof(modules), c.byref(count), 3):
            raise c.WinError(c.get_last_error())
        base = modules[0]
        executable = args.exe.read_bytes()
        pe = struct.unpack_from("<I", executable, 0x3C)[0]
        remote_pe = struct.unpack("<I", read(base + 0x3C, 4))[0]
        if read(base + remote_pe, 24) != executable[pe:pe + 24]:
            raise RuntimeError("The executable differs from the running build")
        preferred = struct.unpack_from("<Q", executable, pe + 48)[0]
        symbols = {}
        for line in args.exe.with_suffix(".map").read_text().splitlines():
            match = re.match(r"\s+[0-9A-Fa-f]{4}:[0-9A-Fa-f]+\s+(\S+)\s+([0-9A-Fa-f]{16})", line)
            if match:
                symbols[match[1]] = base + int(match[2], 16) - preferred
        address = symbols["g_rollback_task_steps"]
        counter_format = "<" + str(len(TASKS)) + "I"
        before = struct.unpack(counter_format, read(address, len(TASKS)*4))
        worker_address = symbols.get("g_rollback_worker_steps")
        worker_before = struct.unpack("<I", read(worker_address, 4))[0] if worker_address else None
        apc_symbols = {name: symbols.get("g_rollback_apc_" + name) for name in ("queued", "delivered")}
        apc_before = {name: struct.unpack("<I", read(va, 4))[0] for name, va in apc_symbols.items() if va}
        stream_symbols = {name: symbols.get("g_rollback_stream_worker_" + name)
                          for name in ("steps", "nested_yields")}
        stream_before = {name: struct.unpack("<3I", read(va, 12))
                         for name, va in stream_symbols.items() if va}
        time.sleep(args.seconds)
        after = struct.unpack(counter_format, read(address, len(TASKS)*4))
        worker_after = struct.unpack("<I", read(worker_address, 4))[0] if worker_address else None
        apc_after = {name: struct.unpack("<I", read(va, 4))[0] for name, va in apc_symbols.items() if va}
        stream_after = {name: struct.unpack("<3I", read(va, 12))
                        for name, va in stream_symbols.items() if va}
        offset = struct.unpack("<q", read(symbols["g_xbox_mem_offset"], 8))[0]
        tasks = []
        for slot in range(18):
            flags, delay, fiber, routine = struct.unpack("<4I", read(offset + 0x73C400 + slot * 32, 16))
            if flags & 1:
                tasks.append(dict(slot=slot, routine=f"{routine:08X}", flags=f"{flags:08X}",
                                  delay=delay, fiber=f"{fiber:08X}"))
        routines = TASKS
        workers = {"001926B0": {"total": worker_after,
                   "delta": (worker_after - worker_before) & 0xFFFFFFFF}} if worker_address else {}
        completions = {name: {"total": total, "delta": (total-apc_before[name]) & 0xFFFFFFFF}
                       for name, total in apc_after.items()}
        streams = {f"{routine:08X}": {name: {
            "total": values[index],
            "delta": (values[index]-stream_before[name][index]) & 0xFFFFFFFF}
            for name, values in stream_after.items()}
            for index, routine in enumerate((0x1926F0, 0x192760, 0x1927D0))}
        print(json.dumps(dict(seconds=args.seconds, completions=completions, worker_steps=workers,
            streaming_workers=streams, steps={f"{va:08X}": {
            "total": total, "delta": (total - old) & 0xFFFFFFFF}
            for va, old, total in zip(routines, before, after)}, active_tasks=tasks), indent=2))
    finally:
        kernel.CloseHandle(handle)


if __name__ == "__main__":
    main()
