"""Offline audio self-replay from a previously validated v5 task capture.

This does not compare against a recorded live audio advance: command captures
lack partial output periods. Each selected sample seeds a fresh private output
period, then tests checkpoint/advance/restore in the real VP/DSP implementation.
"""
import argparse
from pathlib import Path
import struct
import subprocess
import tempfile


def samples(path, digest):
    def exact(f, n):
        b = f.read(n)
        if len(b) != n:
            raise ValueError('Truncated capture')
        return b

    with path.open('rb') as f:
        h = struct.unpack('<8I', exact(f, 32))
        if h[:3] != (0x31434252, 5, 128 * 1024 * 1024) or not 0 < h[7] <= 10000:
            raise ValueError('Unsupported capture')
        if exact(f, 32) != digest:
            raise ValueError('Source digest differs from the supplied validated build')
        apu, gpu = struct.unpack('<2I', exact(f, 8))
        task = h[6] - apu - gpu
        if not (0 < apu < 4*1024*1024 and 0 <= gpu < 1024*1024 and 128 <= task < 1024*1024):
            raise ValueError('Invalid payload sizes')
        ram = bytearray(h[2])
        count, pending = 0, None
        while True:
            kind, routine, pages, frame = struct.unpack('<4I', exact(f, 16))
            if (kind, routine, pages, frame) == (0, 0, 0, 0):
                if pending or count != 2*h[7] or f.read(1):
                    raise ValueError('Incomplete capture or trailing bytes')
                return
            if kind != 1 + count % 2 or not routine or pages > h[2]//4096 or count >= 2*h[7]:
                raise ValueError('Invalid record')
            if kind == 1:
                pending = (routine, frame)
            elif pending != (routine, frame):
                raise ValueError('Mismatched task pair')
            else:
                pending = None
            exact(f, task)
            command = exact(f, apu)
            exact(f, gpu)
            last = -1
            for _ in range(pages):
                page, = struct.unpack('<I', exact(f, 4))
                if not last < page < h[2]//4096:
                    raise ValueError('Invalid page order/index')
                ram[page*4096:(page+1)*4096] = exact(f, 4096)
                last = page
            count += 1
            if kind == 1 and routine == 0x87420:
                yield frame, ram, command


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture', type=Path)
    p.add_argument('executable', type=Path)
    p.add_argument('--source-digest', required=True, help='Digest of the previously validated capture build')
    args = p.parse_args()
    digest = bytes.fromhex(args.source_digest)
    if len(digest) != 32:
        p.error('Expected a 32-byte source digest')
    # Validate through the footer before executing tests from any sample.
    for _ in samples(args.capture, digest):
        pass
    count = 0
    with tempfile.TemporaryDirectory(prefix='doa3-audio-replay-') as folder:
        ram_path, apu_path = Path(folder)/'ram.bin', Path(folder)/'apu.bin'
        for frame, ram, command in samples(args.capture, digest):
            ram_path.write_bytes(ram)
            apu_path.write_bytes(command)
            print(f'Captured Practice frame {frame}:', flush=True)
            subprocess.run([str(args.executable.resolve()), str(ram_path), str(apu_path)], check=True, timeout=60)
            count += 1
    if not count:
        raise ValueError('No main-task samples found')
    print(f'{count} captured-state audio self-replay checks passed; not a live-frame replay comparison.')


if __name__ == '__main__':
    main()
