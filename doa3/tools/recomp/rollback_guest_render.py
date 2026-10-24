"""Pin the model-state paths and the remaining deformation skip candidate.

This is an opt-in experiment, not a proof that all their writes are disposable.
Live replay still compares ALL guest RAM/CPU and logical renderer state.
"""
import hashlib
import re
from pathlib import Path

REVIEWED = {
    0x17D900: '0b0a784948e9d22d1f165c9a7bbc09f68e94f72a94203a83b347f7f0f383be86',
    0x17DF00: '0a6e8846b89b8d74e7bf9d1a2b740e58ba05f08284aca5d419a009304efa9200',
    0x52046: '56558672c8fd5ba56d4da68803281953695abcea0f3eef708a8426b0266d5009',
}
RETURN_BYTES = {0x17D900: 4, 0x17DF00: 8, 0x52046: 4}

def verify(bodies, generated_overrides, overrides, manual_source, probes):
    if '#include "../rollback_guest_render.inc"' not in manual_source:
        raise ValueError('missing guest render wrappers')
    wrappers = (Path(__file__).resolve().parents[2] / 'src/game/rollback_guest_render.inc').read_text()
    for va, expected in REVIEWED.items():
        if re.search(rf'^P\({va:08X},',probes,re.M):
            raise ValueError(f'{va:08X}: duplicate stack-probe wrapper; regenerate probes')
        body = bodies.get(va, '')
        if hashlib.sha256(body.encode()).hexdigest() != expected:
            raise ValueError(f'{va:08X}: guest render body changed; review effects before skipping')
        if va not in generated_overrides or va not in overrides:
            raise ValueError(f'{va:08X}: missing generated guest render override')
        gate = (rf'if\(doa3_native_frame_skip_guest_render\(0x{va:X}\)\)'
                rf'\{{g_esp\+={RETURN_BYTES[va]};return;\}}\s*') if va==0x52046 else ''
        if not re.search(rf'void sub_{va:08X}\(void\)\s*\{{\s*'+gate+
                         rf'sub_{va:08X}_gen\(\);\s*\}}', wrappers):
            raise ValueError(f'{va:08X}: guest ABI/gate wrapper changed')
