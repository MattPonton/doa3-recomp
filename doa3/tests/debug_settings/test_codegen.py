"""Run from doa3/: python -m unittest discover -s tests/debug_settings."""
import unittest
from pathlib import Path
from tools.recomp.debug_unlock_reads import patch


class UnlockReadsTest(unittest.TestCase):
    def test_read_store_and_progression_separation(self):
        source = '''#include "recomp_funcs.h"
void sub_000CD7D0(void)
{
    SET_LO8(edx, MEM8(eax + 0x59FB16));
    MEM8(eax + 0x59FB16) = LO8(edx);
    MEM8(0x59FB3C) = 1;
    if (CMP_EQ(MEM8(0x5A0B47), 1)) goto done;
}
void sub_0008C7E0_gen(void)
{
    SET_LO8(edx, MEM8(eax + 0x59FB16));
    MEM8(eax + 0x59FB16) = LO8(edx);
}
'''
        result, count = patch(source)
        self.assertEqual(count, 2)
        self.assertIn('MEM8(eax + 0x59FB16) = LO8(edx);', result)
        self.assertIn('MEM8(0x59FB3C) = 1;', result)
        self.assertEqual(source.split('void sub_0008C7E0')[1], result.split('void sub_0008C7E0')[1])
        self.assertEqual(patch(result), (result, 0))

    def test_generated_tree_is_fully_patched_and_idempotent(self):
        hits = 0
        for path in Path('src/game/recomp/gen').glob('recomp_*.c'):
            source = path.read_text()
            result, count = patch(source)
            self.assertEqual(count, 0, path.name)
            self.assertEqual(result, source, path.name)
            hits += source.count('doa3_debug_unlock_read8(')
        self.assertEqual(hits, 74)


if __name__ == '__main__':
    unittest.main()
