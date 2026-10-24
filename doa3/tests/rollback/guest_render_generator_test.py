"""A source/ABI change must disable the guest render skip until reviewed."""
import re
import sys
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.recomp.rollback_guest_render import REVIEWED,verify

class GuestRenderGuard(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.bodies={};cls.generated=set()
        for path in (ROOT/'src/game/recomp/gen').glob('recomp_*.c'):
            for m in re.finditer(r'^void sub_([0-9A-F]{8})(_gen)?\(void\)\s*\{(.*?)^\}',path.read_text(),re.M|re.S):
                cls.bodies[int(m[1],16)]=m[3]
                if m[2]:cls.generated.add(int(m[1],16))
        cls.overrides={int(x,16) for x in re.findall(r'^0x([0-9A-Fa-f]+)\b',(ROOT/'tools/recomp/overrides_3.1.txt').read_text(),re.M)}
        cls.manual=(ROOT/'src/game/recomp/recomp_manual.c').read_text()
        cls.probes=(ROOT/'src/game/recomp/gen/recomp_probes.c').read_text()

    def test_current_sources(self):
        verify(self.bodies,self.generated,self.overrides,self.manual,self.probes)

    def test_changed_body(self):
        for va in REVIEWED:
            with self.subTest(va=va),self.assertRaisesRegex(ValueError,'body changed'):
                verify(self.bodies | {va:self.bodies[va]+'\n'},self.generated,self.overrides,self.manual,self.probes)

    def test_missing_hooks(self):
        for va in REVIEWED:
            with self.subTest(va=va),self.assertRaisesRegex(ValueError,'missing generated'):
                verify(self.bodies,self.generated-{va},self.overrides,self.manual,self.probes)
            with self.subTest(va=va),self.assertRaisesRegex(ValueError,'missing generated'):
                verify(self.bodies,self.generated,self.overrides-{va},self.manual,self.probes)
        with self.assertRaisesRegex(ValueError,'missing guest render wrappers'):
            verify(self.bodies,self.generated,self.overrides,'',self.probes)

    def test_duplicate_probe(self):
        for va in REVIEWED:
            with self.subTest(va=va),self.assertRaisesRegex(ValueError,'duplicate stack-probe'):
                verify(self.bodies,self.generated,self.overrides,self.manual,self.probes+f'\nP({va:08X}, 0)\n')

if __name__=='__main__':unittest.main()
