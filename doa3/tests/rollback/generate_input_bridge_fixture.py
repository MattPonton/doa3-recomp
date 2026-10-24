"""Use the actual 3.1 XPP overrides with fixture guest RAM/physical input."""
from pathlib import Path
import sys
s = (Path(__file__).parents[2] / 'src/game/recomp/recomp_manual.c').read_text(encoding='utf-8')
def function(name):
    start=s.index(name);end=s.index('{',start)+1;depth=1
    while depth:
        depth+=(s[end]=='{')-(s[end]=='}');end+=1
    return s[start:end]
parts=[line for line in s.splitlines() if line.startswith('#define XPP_')]
parts += [function(name) for name in ('void sub_0021157D(void)', 'void sub_0021159F(void)',
    'void doa3_netplay_force_pads(uint32_t mask)', 'static uint32_t xpp_port_of(uint32_t h)',
    'void sub_00211898(void)', 'void sub_00211823(void)',
    'void sub_00211A96(void)', 'void sub_002118A4(void)', 'void sub_00211B07(void)')]
Path(sys.argv[1]).write_text('\n'.join(parts),encoding='utf-8')
