"""Route 3.1 menu unlock queries through the reversible host Debug setting.

Run after generated-code fixes. Only selection/display consumers are patched;
reward checks, save initialization, serialization and writes use real data.
"""
import re
from pathlib import Path
from tools import xbe_layout as layout

BASES = {0x59FB3C, 0x59FB3D, 0x5A0B47, 0x5A0B49, 0x5A0C72}
BASES.update(range(0x59FB16, 0x59FB1D))  # includes unrolled costume list reads
MEM = re.compile(r"MEM8\(([^()]*)\)")
FUNCTION = re.compile(r"void sub_([0-9A-F]{8})(?:_gen)?\(void\)")


def patch(source):
    function = 0
    changed = 0
    out = []
    for line in source.splitlines(keepends=True):
        match = FUNCTION.match(line)
        if match:
            function = int(match[1], 16)
        # 0x83xxx/0x8Cxxx are reward/progression updates, not menu queries.
        consumer = 0x60000 <= function < 0x83000 or 0xC0000 <= function < 0xE0000
        if consumer and not line.lstrip().startswith('/*') and 'doa3_debug_unlock_read8' not in line:
            def replace(m):
                nonlocal changed
                # Never rewrite a store, even if it uses one of these fields.
                if re.match(r"\s*=(?!=)", line[m.end():]):
                    return m[0]
                constants = re.findall(r"0x[0-9A-Fa-f]+", m[1])
                if not any(int(v, 16) in BASES for v in constants):
                    return m[0]
                changed += 1
                return f"doa3_debug_unlock_read8({m[1]}, {m[0]})"
            line = MEM.sub(replace, line)
        out.append(line)
    result = ''.join(out)
    if changed and '#include "debug_settings.h"' not in result:
        result = result.replace('#include "recomp_funcs.h"',
            '#include "recomp_funcs.h"\n#include "debug_settings.h"', 1)
    return result, changed


def main():
    if layout.VERSION != '3.1':
        return
    count = 0
    for path in sorted(Path('src/game/recomp/gen').glob('recomp_*.c')):
        source = path.read_text()
        result, changed = patch(source)
        if changed:
            path.write_text(result)
            count += changed
    print(f'Debug unlock menu reads: {count} newly patched')


if __name__ == '__main__':
    main()
