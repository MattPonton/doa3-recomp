import re
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.recomp import direct_afs as patcher

gen = Path(sys.argv[1])
sources = tuple((gen / f'recomp_{i:04d}.c').read_text() for i in (2, 9))
patched = patcher.patch_sources(*sources)
assert patcher.patch_sources(*patched) == patched
raw = (patched[0].replace(patcher.DIRECT_VALIDATE, patcher.VALIDATE),
       patched[1].replace(patcher.DIRECT_ROUTE, patcher.ROUTE))
assert patcher.patch_sources(*raw) == patched
try:
    patcher.patch_sources(raw[0], raw[1].replace(patcher.ROUTE, 'changed'))
except ValueError:
    pass
else:
    raise AssertionError('Changed archive route accepted')

def function(source, address):
    return re.search(rf'void sub_{address}(?:_gen)?\(void\)\n\{{\n.*?\n\}}', source, re.S)[0]

# Execute the actual early-return block and full native archive initializer.
validator = function(patched[0], '000A4300')
assert patcher.DIRECT_VALIDATE in validator
init = function(patched[1], '001C79F0').replace('sub_001C79F0_gen', 'sub_001C79F0')
boot = function(patched[0], '00087420')
start, end = boot.index('loc_000874BE: ;'), boot.index('loc_000874D9: ;')
Path(sys.argv[2]).write_text(
    'void sub_000A4300(void) {\n' + patcher.DIRECT_VALIDATE + '}\n' + init +
    '\nstatic void start_copy_if_requested(void) { uint32_t _fs0=0, _fs1=0;\n' +
    boot[start:end] + 'loc_000874D9: ;\n}\n')
print('Direct AFS fresh patch, idempotence and boundary checks passed')
