"""Keep native 3.1 save validation, accepting authenticated older signing keys."""
import re
from pathlib import Path

OLD = '    if ((g_str_a != g_str_b)) goto loc_0002151D; /* jne: not equal / not zero */'
NEW = ('    if ((g_str_a != g_str_b)) {\n'
       '        extern int doa3_save_accept_compatible(const void *, size_t);\n'
       '        if (!doa3_save_accept_compatible(XBOX_PTR(ebp), 0x3C48)) goto loc_0002151D;\n'
       '    } /* Verified alternate signature; native format checks remain. */')

def patch(source):
    pattern = re.compile(r'void sub_00020DE0(?:_gen)?\(void\)\n\{\n.*?\n\}', re.S)
    matches = list(pattern.finditer(source))
    if len(matches) != 1:
        raise ValueError('Expected one 3.1 save loader')
    match = matches[0]
    body = match[0]
    if body.count(NEW) == 1: return source
    if body.count(OLD) != 1: raise ValueError('Save signature comparison changed')
    return source[:match.start()] + body.replace(OLD, NEW, 1) + source[match.end():]

def main():
    from tools import xbe_layout as layout
    if layout.VERSION != '3.1': return
    path = Path('src/game/recomp/gen/recomp_0000.c')
    source = path.read_text()
    result = patch(source)
    if result != source: path.write_text(result)
    print('Saves: authenticated retail, zero-root title-derived and legacy PC signatures')

if __name__ == '__main__': main()
