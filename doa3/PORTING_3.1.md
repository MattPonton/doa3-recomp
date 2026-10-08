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

## `recomp_manual.c`

Upstream's file (7,072 lines: 262 overrides, ~480 helper definitions, most of
them 3.0 diagnostics) is kept unbuilt as `src/game/recomp/reference/
recomp_manual_30.c`. The new file (about 300 lines) keeps only what the rest
of the runtime links against: the 17 symbols used outside the file plus their
dependencies, unchanged apart from the APU ISR range, which now uses the
DSOUND section bounds from `xbe_layout.h`.

Placeholders for helpers that read 3.0 game memory (each marked `TODO(3.1)`):

| Function | 3.0 behaviour | Now |
|---|---|---|
| `doa3_pump_cri_servers` | runs the CRI ADX/Sofdec servers each vblank | logs once, does nothing: streamed audio and movies are expected to stall |
| `doa3_guest_display_size` | reads the device's back-buffer size | returns 0, NV2A falls back to SET_SURFACE_CLIP |
| `doa3_workers_may_run` | also requires two CRI lock words clear | lock check dropped (gate stays closed until movie-flow overrides exist) |
| `doa3_netplay_force_pads` | writes the XPP pad table | logs once, does nothing |
| `doa3_ptinfo_check` | page-table diagnostic (`DOA3_PTCHK=1`) | no-op |
| `doa3_crt_lookup` | 3.0's separate constructor table | returns 0; constructors are in the dispatch table |

The override table is empty. Add 3.1 overrides there as defects are found.

## Gated to 3.0 builds (`#ifdef DOA3_XBE_ID_3_0`)

- `kernel_bridge.c` `bridge_NtWaitForSingleObject`: the installer-thread
  wait (ctx `0x0009D440`) and the CRI vblank pulse (`0x001C2CF0`). Other
  XBEs get the old "satisfied at once" result. TODO(3.1).
- `xbox_fiber.c`: the CRI watchdog exclusion (`0x0016A530`). TODO(3.1).
- `online/netplay_session.c`: every address in it is 3.0's. On other XBEs,
  arming a session or loading a replay is refused with a message, so none
  of it touches guest memory.

## First-run setup (`xiso_extract.c`)

The disc-image installer accepted only the USA `default.xbe` (title ID, size
and FNV-1a hash) and checked every disc file against the USA sizes. Those
now come from `xbe_layout.h` (`DOA3_TITLE_ID`, `DOA3_XBE_FILE_SIZE`,
`DOA3_XBE_FNV64`; for 3.0 they equal upstream's constants). For other
releases `default.xbe` must still match exactly, and the other disc files
are checked for presence only. TODO(3.1): add the 3.1 disc's file sizes.

## Compile check (without Windows)

Every runtime C file and all 12 generated units compile with clang against
the mingw-w64 headers (`--target=x86_64-w64-windows-gnu -fms-extensions`),
except `kernel_path.c`, whose `KernelMode` macro clashes with mingw's RPC
headers (also true of unmodified upstream; MSVC's headers do not clash).
Comparing undefined against defined symbols across the objects leaves only
Windows/CRT imports and symbols from files that check could not build
(`kernel_path.c`, the C++ UI). The real build is MSVC on Windows.

## Still to do

1. **First MSVC build** on Windows, then boot and fix what breaks.
2. **CRI pump and movie flow**: re-derive `doa3_pump_cri_servers` and the
   movie-flow overrides against 3.1's CRI library.
3. **Remaining runtime game symbols**: the three gated addresses above;
   `main.c` `DOA3_WATCHARR` debug watch (`0x00C05844`) and the
   `nv2a/*_snapshot.h` debug captures are 3.0 addresses, debug only.
4. **Netplay**: port the state regions, then lift the gate.
5. **Entry points reached only through pointers** beyond the CRT tables:
   vtables, callback and state tables (upstream's `recomp_vtbl.c`,
   `recomp_seedattract.c`). Found at runtime through `[ICALL-CENSUS]` /
   unresolved-dispatch logs.
6. **Fix scripts not yet run on the 3.1 output**: `fix_deferred_cmp`,
   `fix_fallthroughs` / `scan_fallthroughs2` (hardcode 3.0's .text end and an
   XDK keep-list), `fix_fpu_global`, `fix_selfspins` (hardcodes file names),
   `fix_cond_tailcall_ebp`, `fix_ftol_inline` (now layout-driven).
7. **3++ code cave**: 0x2355E0 in `.rdata` is skipped by the lifter; implement
   the 3++ throw-damage fix as an override of `sub_000A92C0` instead.

The NOTES.md "Lifter Defect Reference" describes each defect class upstream
hit on 3.0; expect the same classes on 3.1 at different addresses.
