"""Use 3.1's original D: archive loader without the Xbox Z: cache copy.

D: is already backed by the user's installed HDD/D files. Skip the cache
validator (including cache deletion), leave the copy-request flag clear,
and select CRI's existing direct-source branch. Do not alter kernel I/O,
streaming buffers, save paths, or the user's existing cache files.
"""
import re
from pathlib import Path

VALIDATE = 'loc_000A4300: ;\n'
DIRECT_VALIDATE = VALIDATE + (
    '    /* Host D: is already on disk; no Xbox cache validation/copy. */\n'
    '    MEM32(0x73C0A4) = 0;\n'
    '    eax = 0;\n'
    '    esp += 4; return; /* direct AFS source */\n')
ROUTE = ('loc_001C7A86: ;\n'
         '    if (CMP_NE(MEM32(0x73C0A4), esi)) goto loc_001C7B3B;')
DIRECT_ROUTE = ('loc_001C7A86: ;\n'
                '    goto loc_001C7B3B; /* Host: always mount the D: originals. */')


def patch_function(source, address, old, new):
    pattern = re.compile(rf'void sub_{address}(?:_gen)?\(void\)\n\{{\n.*?\n\}}', re.S)
    matches = list(pattern.finditer(source))
    if len(matches) != 1:
        raise ValueError(f'Expected one sub_{address} for direct AFS hook')
    match = matches[0]
    body = match[0]
    if body.count(new) == 1:
        return source
    if body.count(old) != 1:
        raise ValueError(f'Direct AFS boundary changed: sub_{address}')
    return source[:match.start()] + body.replace(old, new, 1) + source[match.end():]


def patch_sources(validator, init):
    return (patch_function(validator, '000A4300', VALIDATE, DIRECT_VALIDATE),
            patch_function(init, '001C79F0', ROUTE, DIRECT_ROUTE))


def main():
    from tools import xbe_layout as layout
    if layout.VERSION != '3.1':
        return
    paths = [Path(f'src/game/recomp/gen/recomp_{i:04d}.c') for i in (2, 9)]
    sources = [p.read_text() for p in paths]
    results = patch_sources(*sources)
    for path, source, result in zip(paths, sources, results):
        if source != result:
            path.write_text(result)
    print('AFS: skip Xbox cache validation/copy and mount D: originals directly')


if __name__ == '__main__':
    main()
