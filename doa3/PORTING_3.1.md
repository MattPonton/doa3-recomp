# Porting the recomp to DOA3 3.1 (NTSC-J)

Branch `doa3.1`. Target XBE: `default.xbe` from 3.1, MD5
`4d1a10a16b8ad2065ad9dee65e951f08`, XDK 4134. Upstream targets 3.0 (XDK 3911),
which is a different build: only ~15% of 3.1's game functions match 3.0 byte
for byte, so upstream's per-address fixes do not carry over and are re-derived
here as the same defect classes turn up.

## Regenerating

The tools take their layout from the XBE named by `DOA3_XBE` (see
`tools/xbe_layout.py`), so the same pipeline handles 3.0, 3.1, 3.2 and 3++.
Run from `doa3/`:

```powershell
$env:DOA3_XBE = "build/release/assets/default.xbe"   # the 3.1 XBE
py -3 tools/xbe_parser/xbe_parser.py $env:DOA3_XBE --json tools/xbe_parser/doa3_analysis.json --quiet
py -3 -m tools.disasm $env:DOA3_XBE --force
py -3 -m tools.xbe_layout --crt-initializers > crt_ctors.txt
py -3 -m tools.recomp.seed_missing_functions crt_ctors.txt
# repeat until find_unresolved prints nothing:
py -3 -m tools.func_id $env:DOA3_XBE
py -3 -m tools.recomp $env:DOA3_XBE --all --split 1000
py -3 -m tools.recomp.find_unresolved > unresolved.txt
py -3 -m tools.recomp.seed_missing_functions unresolved.txt
# finally, the runtime layout header:
py -3 -m tools.xbe_layout --header src/game/recomp/gen/xbe_layout.h
```

`find_unresolved` lists symbols referenced but not defined in the generated
code, standing in for MSVC's "unresolved external symbol" errors.

Current result: 5,632 detected functions + 4,505 seeds (incl. the 64
initializer-table entries) = 10,137 entries, 0 unresolved call targets.

## Version-driven now

| Piece | Source |
|---|---|
| Section layout, entry point, thunk table (tools) | `tools/xbe_layout.py` |
| func_id layout (was Burnout 3's) | `tools/xbe_layout.py` |
| SEH prolog/epilog, `__ftol2` (lifter, translator, fix_ftol_inline) | signature search in the XBE |
| Runtime section mapping, raw offsets | `gen/xbe_layout.h` |
| Fake TLS / RW-data / kernel-data pages, guest stack base | `gen/xbe_layout.h`, placed above the image |
| Kernel import ordinals (116 for 3.1) | `gen/xbe_layout.h` |
| CRT `__xi`/`__xc` initializer tables | `gen/xbe_layout.h` |

Generated for 3.0, the header reproduces every upstream value (entry, TLS
0x00C32000, RW data 0x00C36000, stack 0x00C40000, the 112 ordinals, both CRT
tables). The one deliberate change: the kernel data exports page moved from
0x00740000, which is inside `.data`'s BSS in every DOA3 build, to its own page
above the image.

3.1 placements: image ends 0x00CABB00; fake TLS 0x00CAC000, RW data 0x00CB0000,
kernel data 0x00CB4000, stack 0x00CC0000. The low heap is ~0.5 MB smaller than
3.0's because the image is larger.

All 8 kernel imports new in 3.1 already have bridge handlers.

## Still 3.0-specific (to do)

1. **`src/game/recomp/recomp_manual.c`**: 262 function overrides and 585
   `sub_` references at 3.0 addresses, 317 of which are not functions in 3.1,
   plus ~50 helpers / 40 globals other modules use (push-buffer translation,
   CRI pump, small-block heap). Plan: keep the helpers, move the 3.0 overrides
   to a non-compiled reference file, re-derive 3.1 overrides as needed.
2. **Runtime game symbols**:
   - `kernel_bridge.c` `bridge_NtWaitForSingleObject`: installer thread ctx
     `0x0009D440`, CRI vblank event `0x001C2CF0`.
   - `xbox_fiber.c`: CRI watchdog `0x0016A530`.
   - `main.c` `DOA3_WATCHARR` debug watch (`0x00C05844`), debug only.
   - `nv2a/*_snapshot.h`: debug captures, 3.0 addresses.
3. **Netplay** (`src/online/netplay_session.c`): 63 fight-state addresses and
   `sub_000BB270`. Leave online disabled for 3.1 until offline play works.
4. **Entry points reached only through pointers** beyond the CRT tables:
   vtables, callback and state tables (upstream's `recomp_vtbl.c`,
   `recomp_seedattract.c`). Found at runtime through `[ICALL-CENSUS]` /
   unresolved-dispatch logs.
5. **Fix scripts not yet run on the 3.1 output**: `fix_deferred_cmp`,
   `fix_fallthroughs` / `scan_fallthroughs2` (hardcode 3.0's .text end and an
   XDK keep-list), `fix_fpu_global`, `fix_selfspins` (hardcodes file names),
   `fix_cond_tailcall_ebp`, `fix_ftol_inline` (now layout-driven).
6. **3++ code cave**: 0x2355E0 in `.rdata` is skipped by the lifter; implement
   the 3++ throw-damage fix as an override of `sub_000A92C0` instead (see
   `VERSION_DIFF.md` in the analysis notes).

The NOTES.md "Lifter Defect Reference" describes each defect class upstream
hit on 3.0; expect the same classes on 3.1 at different addresses.
