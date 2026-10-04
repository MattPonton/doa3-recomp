"""Byte-level diff of two netplay state captures (netplay_state_*.bin).

    py state_diff.py A.bin B.bin [first_frame] [last_frame]

For each frame in range, lists per region the differing byte runs as guest
addresses with both values (first 12 runs per region). The last column of a
capture is the RNG seed.
"""
import struct
import sys


def load(path):
    d = open(path, 'rb').read()
    assert d[:8] == b'DOA3STA1', path
    n = struct.unpack_from('<I', d, 8)[0]
    off = 12
    regions = []
    for _ in range(n):
        va, ln = struct.unpack_from('<II', d, off)
        name = d[off + 8:off + 24].split(b'\0')[0].decode()
        regions.append((va, ln, name))
        off += 24
    frames = struct.unpack_from('<I', d, off)[0]
    off += 4
    stride = sum(r[1] for r in regions) + 4
    return regions, [d[off + i * stride: off + (i + 1) * stride] for i in range(frames)]


def runs(a, b):
    out, i = [], 0
    while i < len(a):
        if a[i] != b[i]:
            j = i
            while j < len(a) and (a[j] != b[j] or (j + 1 < len(a) and a[j + 1] != b[j + 1])):
                j += 1
            out.append((i, j))
            i = j
        i += 1
    return out


def main():
    ra, fa = load(sys.argv[1])
    rb, fb = load(sys.argv[2])
    assert ra == rb, 'captures use different regions'
    lo = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    hi = int(sys.argv[4]) if len(sys.argv) > 4 else lo
    for fr in range(lo, min(hi, len(fa) - 1, len(fb) - 1) + 1):
        A, B = fa[fr], fb[fr]
        print('frame %d' % fr)
        off = 0
        for va, ln, name in ra:
            rr = runs(A[off:off + ln], B[off:off + ln])
            if rr:
                print('  %-8s %d runs, %d bytes differ' % (name, len(rr), sum(e - s for s, e in rr)))
                for s, e in rr[:12]:
                    print('    %08X +%04X  %s | %s' % (va + s, s, A[off + s:off + e].hex(' '), B[off + s:off + e].hex(' ')))
            off += ln
        sa, sb = struct.unpack_from('<I', A, off)[0], struct.unpack_from('<I', B, off)[0]
        if sa != sb:
            print('  seed     %08X | %08X' % (sa, sb))


if __name__ == '__main__':
    main()
