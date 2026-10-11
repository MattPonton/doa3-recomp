from pathlib import Path
import hashlib, hmac, sys
root = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(root))
from tools.recomp import save_compat
source = (root/'src/game/recomp/gen/recomp_0000.c').read_text()
patched = save_compat.patch(source)
assert save_compat.patch(patched) == patched
assert save_compat.patch(patched.replace(save_compat.NEW, save_compat.OLD)) == patched
try:
    save_compat.patch(patched.replace(save_compat.NEW, 'changed'))
except ValueError:
    pass
else:
    raise AssertionError('Changed comparison was accepted')
key = bytes(range(16))
retail = hmac.digest(bytes.fromhex('5c0733ae0401f7e8ba7993fdcd2f1fe0'), key, 'sha1')[:16]
derived = hmac.digest(bytes(16), key, 'sha1')[:16]
payload = bytearray((i*17+3)&255 for i in range(0x3c34))
payload[0x3bfa:0x3c0a] = b'DEAD OR ALIVE 3\0'
assert len(payload) == 0x3c34
def array(name, data):
    return f'static const unsigned char {name}[] = {{' + ','.join(str(x) for x in data) + '};\n'
Path(sys.argv[1]).write_text(array('retail_key', retail) + array('derived_key', derived) +
    ''.join(array(f'signature_{i}', hmac.digest(k,payload,'sha1')) for i,k in enumerate((retail,derived,bytes(16)))))
print('Save patch boundary/idempotence checks and independent Python HMAC vectors passed')
