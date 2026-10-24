"""Extend 3.1 Mode Select without overwriting the adjacent guest tables.
Idempotent, applied by apply_overrides after regeneration.
"""
import re

def patch(source):
    source = re.sub(r"0x29CB[2-8][0-9A-F]\b", lambda m:
        f"(g_online_menu_descriptors + {int(m[0],16)-0x29CB20})", source)
    # Only the carousel computes bounds for our sentinel sprite. Other sprites
    # retain their original addresses, and 6DF00 intercepts its actual draw.
    start=source.index("void sub_000CE070_gen(void)")
    end=source.index("void sub_000CE530_gen(void)",start)
    carousel = re.sub(r"MEM16\(([^()\n]*0x3049B[ACE8])\)",
                    r"MEM16(online_menu_sprite_address(\1))", source[start:end])
    source=source[:start]+carousel+source[end:]
    needle = "    MEM32(edi + 0x1D8) = eax;\n    if (TEST_Z(MEM16(ebx * 8 + 0x9B7C0E), 0x110))"
    replacement = """    MEM32(edi + 0x1D8) = eax;
    if (eax == ONLINE_MENU_MODE && (MEM16(ebx * 8 + 0x9B7C0E) & 0x110)) {
        online_menu_queue(ONLINE_MENU_OPEN);
        goto loc_000D657F;
    }
    if (TEST_Z(MEM16(ebx * 8 + 0x9B7C0E), 0x110))"""
    if needle not in source and replacement not in source:
        raise ValueError("3.1 Mode Select confirm path changed; Online must not launch a native mode")
    source = source.replace(needle,replacement)
    if 'g_online_menu_descriptors' in source and '#include "online_menu.h"' not in source:
        source = source.replace('#include "recomp_funcs.h"','#include "recomp_funcs.h"\n#include "online_menu.h"')
    return source
