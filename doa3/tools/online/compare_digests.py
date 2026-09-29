"""Compare netplay digest logs (netplay_digest_*.txt) frame by frame.

    py compare_digests.py A.txt B.txt [C.txt ...]

For every log after the first, prints the first frame whose total digest
differs from A, then for each state region the first frame it differs and on
how many frames it differs in total. Regions that differ from the first frame
on but never affect anything else are usually host-side noise; a region whose
first difference comes later is where to look.
"""
import sys


def load(path):
    cols, frames = None, {}
    for line in open(path):
        if line.startswith('# frame'):
            cols = line[2:].split()
            continue
        if line.startswith('#') or not line.strip():
            continue
        parts = line.split()
        frames[int(parts[0])] = parts
    return cols, frames


def main():
    paths = sys.argv[1:]
    if len(paths) < 2:
        print(__doc__)
        return
    cols, a = load(paths[0])
    names = cols[2:2 + len(next(iter(a.values()))) - 2]
    for p in paths[1:]:
        _, b = load(p)
        common = sorted(set(a) & set(b))
        print('%s vs %s: %d / %d frames in common' % (paths[0], p, len(common), max(len(a), len(b))))
        first_total = next((f for f in common if a[f][1] != b[f][1]), None)
        print('  total digest: %s' % ('identical' if first_total is None else 'first differs at frame %d' % first_total))
        for k, name in enumerate(cols[2:], start=2):
            if name in ('p1', 'p2', 'mode/scrst/gmode', 'kcalls', 'boost', 'rand_game', 'rand_workers', 'active'):
                diff = [f for f in common if k < len(a[f]) and a[f][k] != b[f][k]]
                if diff:
                    print('  %-16s differs on %d frames, first %d  (%s vs %s)' % (name, len(diff), diff[0], a[diff[0]][k], b[diff[0]][k]))
                continue
            diff = [f for f in common if a[f][k] != b[f][k]]
            if diff:
                print('  %-16s differs on %6d frames, first at %d' % (name, len(diff), diff[0]))


if __name__ == '__main__':
    main()
