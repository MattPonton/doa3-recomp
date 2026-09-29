# Dead or Alive 3 Static Recompilation Notes

This document is the maintained technical guide for the DOA3 static recompilation.
It describes the behavior implemented by the current tree, the fixes that must
survive regeneration, and the remaining limitations. Historical experiments,
reverted patches, and unverified hypotheses do not belong here.

## Current Status

The project boots the retail Xbox executable through a native Windows static
recompilation. The opening movie is user-verified to play through the Team Ninja
logo at the expected frame rate with no visible artifacts.

This is not yet a claim that the full game is playable. In particular:

- Audio behavior has not been accepted as complete.

## Source and Runtime Inputs

The project expects legally obtained game data outside the repository:

- XBE: `../doa3gamefiles/default.xbe`
- Opening movie: `../doa3gamefiles/ninja.sfd`

Relevant XBE facts:

- Title ID: `0x54430001`
- Image base: `0x00010000`
- Entry point: `0x001651A5`
- Kernel thunk table: `0x001ED0E0`, 112 imports
- Main `.text`: `0x00011000` through `0x001B0DE0`
- Middleware: CRI Sofdec/PSGSFD, AFS, ADX/AIX
- Graphics API: retail Xbox Direct3D 8

## Build and Run

Requirements:

- Windows with a recent MSVC toolchain
- CMake 3.20 or newer
- The game files listed above

Build from `doa3/`:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Output:

```text
build/release/doa3.exe
```

No external movie decoder or FFmpeg installation is required.

## Runtime Architecture

### Static recompilation

Generated functions use global Xbox register state and fixed guest-memory
mappings. Indirect guest calls are resolved through the recompilation dispatch
table. Manual overrides live in `src/game/recomp/recomp_manual.c`; generated
translation units live in `src/game/recomp/gen/`.

### Memory and kernel

The DOA3 image, XDK sections, stack, TLS, and runtime data are mapped at their
expected Xbox virtual addresses. Xbox kernel imports are bridged to Win32 by
ordinal. Guest execution uses cooperative fibers because the global register
model is not safe for unconstrained native multithreading.

Important layout values:

- Guest stack: `0x00D00000`
- Fake TLS: `0x00C40000`
- Fake runtime data: `0x00C50000`

### Graphics

The host translates the game's D3D8/NV2A work to D3D11. Sofdec normally writes
movie frames directly into the Xbox front-buffer pair rather than calling
`D3DDevice_Present`, so `src/game/movie_present.c` owns movie upload and
swap-chain presentation.

### Opening movie

The opening movie is handled by `src/game/movie_present.c`:

- The bundled PL_MPEG decoder decodes `ninja.sfd` to BGRA.
- `QueryPerformanceCounter` paces presentation at 30 fps.
- D3D11 presents all 571 frames through the Team Ninja logo.

This is the default path and is user-verified to play correctly without visible
artifacts.

## Applied Recompilation Fixes

These are behavior-changing fixes present in the current tree. Do not remove
them because an internal metric looks cleaner; validate changes against actual
game behavior.

### PSGSFD fall-through restoration

Function detection split several adjacent PSGSFD blocks and dropped required
fall-through control flow. Six generated transitions in
`src/game/recomp/gen/recomp_0011.c` restore the next block:

- `0x001E25B6` to `sub_001E25BA`
- `0x001E2890` to `sub_001E2C92`
- `0x001E3D2C` to `sub_001E3D30`
- `0x001E410F` to `sub_001E4112`
- `0x001E43E0` to `sub_001E478D`
- `0x001E4EC9` to `sub_001E4ED2`

The `sub_001E2C92` transition is especially important because it restores the
macroblock worker epilogue and prevents a 36-byte guest-stack drift. Before this
fix, decode collapsed after only a handful of real pictures.

### Deferred-compare operand preservation

At guest address `0x0017C025`, the original code compares `ebx` with the old
`ebp` value and then overwrites `ebp` before the conditional branch. Deferred
C emission previously evaluated the comparison after the overwrite.

`src/game/recomp/gen/recomp_extra2.c` snapshots both operands before the
clobber. This preserves delivery of the first video PES packet, including the
MPEG sequence header, so playback begins from the real first frame.

### MMX `pavgb`

The lifter previously dropped `pavgb` instructions used by half-pixel motion
compensation. The current implementation includes:

- `mmx_pavgb` in `src/game/recomp/recomp_types.h`
- the `pavgb` mapping in `tools/recomp/lifter.py`
- generated calls in `src/game/recomp/gen/recomp_psgsfd.c`

The helper implements unsigned packed-byte averaging with Xbox/MMX rounding:
`(a + b + 1) >> 1` for each byte.

### x87 control word and `frndint` rounding mode

The lifter emitted `fnstcw`/`fldcw` as comments and translated `frndint` as
host `rint()` (round-to-nearest). The CRT `floor()` (`sub_0018DE71`) sets the
rounding mode to "down" with `fldcw` around `frndint`, then reads the old
control word back through `_ctrlfp` (`sub_00191A4D`) to decide which
exceptions are masked. With neither modelled, `floor(x)` rounded to nearest,
flagged the result inexact, read stack garbage as the mask, and took its
exception path (`RtlRaiseException 0xC000008F`). On that path `_except1`
(`sub_00191848`) tail-jumps internally and leaves `g_seh_ebp` at its own
frame, and the fragment `sub_0018DF33` falls into the epilogue
`sub_0018DF3B` (`mov esp, ebp`) without restoring it, so `floor()` returned
with esp 52 bytes low.

The Sofdec timecode splitter (`sub_001796F0` -> `sub_001797FF`) calls this
`floor()` once per picture through `sub_001809E0` and then reads its output
pointer from `[esp+0x20]`; with esp shifted it read a leftover double
(`0x3FE0....`) as the pointer and wrote four dwords through it every frame of
the intro movie. Those writes landed in the loadfile.afs partition
sector-size table at `0x4BDA20+0x116`, so every post-movie resource load
resolved to the wrong file offset and the title screen never got its data.

Current fixes:

- `g_x87_cw` (`xbox_memory_layout.c`, default 0x027F) and `x87_frndint()`
  (`recomp_types.h`) model the control word; `tools/recomp/lifter.py` emits
  `MEM16(m) = g_x87_cw` / `g_x87_cw = MEM16(m)` for `fnstcw`/`fldcw` and
  `x87_frndint()` for `frndint`. The same translation was applied to every
  live site in `src/game/recomp/gen/` (13 `fnstcw`, 18 `fldcw`, 39 `frndint`).
- `sub_0018DF33` (`recomp_0009.c`) saves and restores `g_seh_ebp` around its
  call to `sub_00191848`.

Measured result: `floor` returns +4, no float exception is raised, the
timecode output lands on the stack, the partition table is intact after the
movie, and the post-movie loads read `XPR0` data.

### Packed SSE (xmm as 128-bit lanes)

The lifter modelled every xmm register as one `float`: `movaps` moved 4 of
16 bytes and `mulps`/`addps`/`subps`/`shufps`/`cmpneqps`/`orps` were emitted
as comments. The XDK maths library (matrix copy, multiply, inverse,
translate on the matrix stack at `0x90FAA0`) is built from those, so the
post-movie camera/model matrices came out NaN and every vertex of the title
screen collapsed.

`tools/recomp/lifter.py` now translates the SSE set through `xmm128_t` and
the `xmm_*` helpers in `recomp_types.h` (`translator.py` declares xmm
registers as `xmm128_t`); `comiss`/`ucomiss` set `_fpu_cmp` for the
following `jcc`. The 11 functions in the image that use xmm were re-emitted
into `src/game/recomp/gen/recomp_sse.c`; their previous bodies remain as
`sub_*_oldsse`. `sub_001B7E50` keeps its `recomp_manual.c` wrapper (body is
`sub_001B7E50_gen`).

### Dropped fall-through in the D3D state applier

`sub_001B632C` (`recomp_cffix.c`) lost its fall-through into the shared
epilogue `sub_001B63E5` during the cffix re-emission and returned with its
four pushes still on the stack (-24 bytes); the flusher `sub_001B7690` then
lost esi/ebx and the lazy vertex apply walked wild pointers on the first
post-movie draw. Restored.

### Function pointer target `sub_000E5590`

Reached only through the callback table built at `sub_000B8xxx`
(`recomp_0005.c`: `[ebp-56] = 0xE5590`); binds the post-movie screen's four
texture stages. Seeded in `functions.json` and emitted into
`recomp_extra2.c`.

### Dropped x87 arithmetic in functions emitted before bug #13

234 live functions still carried `/* FPU: fsubr|fdivr|fidiv|fimul|fiadd|
fisub|fdivrp|fsubrp ... */` comments -- instructions the pre-bug-#13 lifter
dropped -- because they were never in the fpuarith re-emission batch. One of
them is the character position update `sub_000910B9` (writes the position
table at `0x4BB950 + 16*i`), whose dropped `fsubr` produced NaN positions
after the movie and so NaN camera/model matrices. They were re-emitted with
the current lifter into `src/game/recomp/gen/recomp_fpu2.c` (previous
bodies kept as `sub_*_oldfpu2`; 31 restored fall-throughs transplanted).
The remaining `/* FPU: fnsave|frstor|fnclex|ffree|fxam */` sites are CRT
state save/restore and were dropped before as well.

### Dropped `fld st(i)` / `fstp st(0)` in hand-emitted units

13 live functions (the `recomp_extra2.c` animation helpers `sub_00095430`,
`sub_000954E0`, `sub_000955B0`, `sub_00046030`, and nine in `recomp_fpufix.c`
/ `recomp_jumptables.c`) still had `fld st(i)` and `fstp st(0)` as bare
comments (emitted before recomp bug #12 was fixed). A dropped load with a
kept pop unbalances the global x87 stack index by one per call, which is how
the character base positions at `0x4BAFD0 + 0xE8*k` became NaN after the
movie. Re-emitted into `src/game/recomp/gen/recomp_fpu3.c` (previous bodies
`sub_*_oldfpu3`). Rule for any hand emission: run it through the CURRENT
lifter and grep the result for bare `/* f... */` lines before committing it.

### Fall-throughs after a conditional tail-call, and statement-less fragments

`tools/recomp/fix_fallthroughs.py` decided that a body was terminated when its
last statement contained `return`; the lifter's conditional tail-call form
`if (...) { g_seh_ebp = ebp; sub_X(); return; }` (a `jcc` whose not-taken
path continues into the next function) matched that test, and a fragment
with no statements at all (a lone `test esi,esi` before a split epilogue)
never reached it. Both classes lost their fall-through.

- `sub_0019037C` (the `write_multi_char` loop body of the CRT `_output`) ended
  in `je -> sub_00190395` with no continuation into `sub_0019038F` (`pop esi;
  pop ebp; ret`), so every `sprintf` with a padded field returned 12 bytes low
  and `_output`'s final `pop ebx` read a pushed pointer. The boot task
  `sub_00084340` keeps ebx = 0 across its init chain and compares
  `[0x4B83AD]` against `bl`; with ebx = 0x024AAEA4 the post-movie attract gate
  at 0x00084479 never cleared and the screen id stayed at 5.
- `sub_001579A0` (the `test esi,esi` at the exit of the vertex-block walker
  `sub_00157700`) had no statements and no fall-through into `sub_001579A2`
  (`jne loop; pop edi; pop esi; pop ebx; add esp,0x88; ret`): every walker
  call returned 0x94 bytes low with ebx/esi/edi unpopped, the caller's index
  loop in `sub_00152B9A` walked off its 4-entry table and the run died on the
  fault-skip cap about a minute into the title screen.

The tool now treats a trailing `if (...)` line as a jcc and restores the
fall-through for empty bodies (`not last or not term`); `FIX_DRYRUN=1` lists
candidates without writing. Rerun restored 85 + 9 fragments in the game
`.text` range. The four non-`if` candidates in `recomp_ctors.c` /
`recomp_vtbl.c` target padding (`sub_0019B000`, `sub_001A4000`, ...) and were
not applied.

Measured: ebx stays 0 through `sub_000833C0`, screen id 5 -> 1, ~250 array
draws per frame, the walker's boundary check shows only the +4 of the dummy
return slot, no wild reads in the title phase. FMV unchanged (568 frames).

### Composite matrix convention in the array-draw path

`SET_COMPOSITE_MATRIX` rows are the coefficients of each output component
(the D3D runtime uploads the row-vector matrix transposed), and the Xbox D3D
folds the viewport (`proj x [w/2, -h/2, 2^24-1; x0+w/2, y0+h/2]`, visible in
the game's `[MATMUL]`) into it, so `c.xy / w` is already the pixel position.
`nv_transform_clip` in `src/nv2a/nv2a_pgraph_d3d11.c` multiplied the
transpose and then applied `SET_VIEWPORT_SCALE/OFFSET` again, which put the
2^24 z-scale into w and collapsed every title-screen draw to one point. Now
`c[i] = dot(in, row i)`, screen = `c.xy / w`, z = `(c.z / w) / vp_scale.z`.
Pre-movie screens issue no array draws, so this path cannot affect the FMV.
User-observed: real stage geometry after the movie.

### Title-screen array path: render target, depth test, near-plane clipping

Three more pieces were needed before the title stage was visible for more
than one frame (all in `src/nv2a/nv2a_pgraph_d3d11.c` / `src/d3d/d3d8_device.c`):

- **Render target.** Every frame the guest binds a 256x256 texture surface
  (reflection/shadow pass: clear + a few draws) before the back buffer. The
  compat layer's `SetRenderTarget` is a no-op and the translator ignored
  `NV097_SET_SURFACE_COLOR_OFFSET`, so that pass and its clear landed on the
  swap chain. The surface clip rectangle (0x0200/0x0204, which arrives after
  the colour offset) now selects the target lazily at each clear/draw:
  `nv_sync_render_target` binds an offscreen D3D11 target of the clip size
  (`d3d8_SetOffscreenTarget`) and restores the swap chain when the clip is the
  guest frame size (720x480 -- not the 640x480 host back buffer). `dev_Clear`
  clears whichever target is bound. The offscreen result is not yet fed back
  to draws that sample it.
- **Depth.** `nv_apply_draw_state` forced `D3DRS_ZENABLE = FALSE` (2D menu
  state); it now follows `SET_DEPTH_TEST_ENABLE`. Without it the stage wall
  drawn last covered the scene.
- **Clipping.** XYZRHW cannot represent w <= 0. Batches with any vertex behind
  the eye or the near plane are rebuilt in clip space from the transformed
  output, clipped against Zc >= 0 (Sutherland-Hodgman, attributes interpolated
  pre-divide) and re-projected; strips/fans become triangle lists only then.
  `DOA3_NOCLIP=1` bypasses it for A/B. `DOA3_FLIPW=1` (negate the homogeneous
  position) was tested and is wrong: the game's own w sign is correct.


### Deferred compares whose operands are overwritten before the branch

The screen-ready wait above was one instance of a class. The lifter emits
`cmp`/`test` as a comment and re-evaluates the operand EXPRESSIONS at the
conditional branch, so any write to an operand register in between makes the
branch test the wrong values. `tools/recomp/fix_deferred_cmp.py` finds and
repairs them; it rewrites a site to the `_rccf` form:

```c
/* test LO8(edx), LO8(edx) - flags set for next jcc */
_rccf = (TEST_NZ(LO8(edx), LO8(edx)));   /* evaluated where x86 evaluates it */
SET_LO8(edx, MEM8(esp + 0x10));
if (_rccf) goto loc_X;
```

Nothing is rewritten on the C text alone. Each candidate is checked against
the guest disassembly and the C-to-guest mapping has to be unambiguous:

- the comment and the branch must be in the same straight-line run (the scan
  stops at a `loc_` label, a bare `goto`, a `return` and at the next compare
  comment) -- past any of those the branch can be reached with other flags;
- the block must hold exactly one candidate in the C and exactly one
  qualifying compare in the guest;
- the guest compare must be the same kind, carry the same literal operand when
  there is one, and reach its branch with no other flag-writing instruction,
  no `call` and no branch target in between.

Run applied 271 rewrites across 20 generated units and skipped 5 ambiguous
blocks. `FIX_DRYRUN=1` lists candidates without writing. Re-run after any
re-emission; the rewrite is idempotent.

Do not "simplify" the tool to a text substitution. An earlier version paired a
comment with a consumer past a label and produced a wrong rewrite in
`sub_000F2506` (two `fnstsw`/`test` pairs in a row); the straight-line-run rule
is what stops that.

### Attract-flow targets reached only through a function-pointer table

Eight entry points in the screen/demo cluster were never emitted, because the
only references to them are in a dispatch table the detector cannot follow:
`0x0004A630`, `0x0004A6E0`, `0x0004A930`, `0x0004A960`, `0x0004BBB0`,
`0x0004C040`, `0x0004C930`, `0x0004CA10`. Every call through them resolved to
nothing and returned 0 -- `[ICALL-CENSUS]` counted 816 failed calls each in a
single attract cycle. They sit inside larger detected functions, so the code
was already present; what was missing was an entry point at the table's
address.

They are seeded into `tools/disasm/output/functions.json` as `link_seed`
entries and emitted into `src/game/recomp/gen/recomp_seedattract.c`
(`tools.recomp -f`), declared in `recomp_funcs.h` and registered in
`recomp_dispatch.c`. Distinct unresolved indirect-call targets fell from 65 to
46 in an attract cycle.

`g_recomp_table` must stay sorted -- `recomp_lookup` binary-searches it -- and
`g_recomp_table_size` must be bumped for every entry inserted before the
prefix it covers. Note that the constant (11927) is smaller than the number of
entries in the array (12144): the last 217, from VA `0x001E938E` up, are not
searched. That predates this work and is left alone deliberately; making them
resolvable changes behaviour for 217 functions and wants its own measurement.

### The attract loop's screen-ready wait, and two general compare defects

After the opening movie the attract flow runs a demo fight (screen id
`0x484C49` = 12), then asks the screen manager for the next screen. The port
used to stop there: the picture went black except the corner logo, and the
fight tasks stayed suspended forever. Three defects were in the way; the first
two are lifting bugs of a general class, the third an ABI leak.

**1. A deferred `test` whose operand was overwritten before the branch.**
`sub_0007BBA0` sets the requested screen id and then spins until the screen's
resources are resident:

```
loc_7BC60: if (sub_0007ED10(slot, id, sub) == 0xFF) goto wait;
           if (sub_0007ED80(slot, id, 0xFF) != 0xFF) return;   /* ready */
wait:      sub_00081240();      /* suspend the fight tasks   */
           sub_0009E562(1);     /* sleep one frame           */
           sub_00081270();      /* resume them               */
           goto loc_7BC60;
```

`sub_0007ED80` tests the slot state byte and then reloads `dl` with its third
argument before the `jne` consumes the flags (`test dl,dl` at 0x0007ED95,
`mov dl,[esp+0x10]` at 0x0007ED97, `jne` at 0x0007ED9B). The lifter emits the
compare as a comment and re-evaluates the operands at the branch, so the
branch tested the *third argument* (0xFF) instead of the state byte, skipped
the matching slot, and returned 0xFF forever. The wait loop therefore never
exited: screen 20 stayed current, tasks 1/5/6/7/8 stayed suspended (the
scheduler `sub_0009E67B` skips flag bit 0x20), and only the 2D overlay task
kept drawing -- the black screen with the logo. `sub_0007ED10` has the same
pattern and was only accidentally right because its caller passes 0 there.

Both are fixed in `src/game/recomp/gen/recomp_0002.c` with the `_rcc` form the
pipeline already uses elsewhere: evaluate the condition at the compare.
**There are 284 more sites in `src/game/recomp/gen/` where a register the
compare reads is written before the branch that consumes its flags**; a scan
for them is worth re-running when a branch looks impossible.

**2. Unsigned compares were evaluated at 32 bits.**
x86 sign-extends a byte immediate to the operand size, so
`cmp word ptr [m], -1` compares the 16-bit value against 0xFFFF. The lifter
writes that immediate already sign-extended (`0xFFFFFFFFu`) and `CMP_EQ`/
`CMP_NE`/`CMP_B`/`CMP_A`/`CMP_AE`/`CMP_BE` compared a zero-extended
`uint16_t` against it, so the equality was permanently false. 61 sites in the
tree compare an 8- or 16-bit operand against an immediate that does not fit
that width, and every one of them was a dead branch.

The character animation event loop is one: `sub_0008C420` runs
`cmp word ptr [esi*2+0x4BADEC], -1; jne loop`, which never terminated once
the attract flow advanced to its second fight. `RC_ZXB` in
`recomp_types.h` now narrows the right operand to the left operand's width,
the same way `RC_SXB` already did for the signed conditions. Operands that
already fit compare exactly as before, so the only behaviour that changes is
the 61 provably-wrong sites.

**3. `sub_001C6B7C` (IDirectSoundBuffer::Release adjustor) leaks the guest
stack and loses esi.**
The release chain below it (`sub_001C792E` -> `sub_001C6BEF` -> the class
destructor through vtable[0]) returns 4 to 8 bytes low, and `sub_001C6BEF`'s
`pop edi; pop esi` then reads the wrong slots. Measured at the call site:
`esi 006A69E8 -> 00000002`, esp delta 0 where +4 is correct. The screen-14
setup sweeps the voice table in `sub_0009F400`
(`for esi = 0x6A69E8; esi < 0x6A78E8; esi += 0x60`) and releases every voice
of a group; with esi destroyed the sweep restarted from a wild pointer
instead of advancing, and the guest stack marched down megabytes a second
until the host APU refused another voice (`apu_vp.c` voice assert).
`recomp_manual.c` now ABI-enforces it (`ret 4`, ebx/esi/edi callee-saved),
the same way `sub_001BCC00` and `sub_001BC260` are enforced.

Measured after the three fixes (Release build, no input): screen ids run
0 -> 12 -> 20 -> 14 -> 9 -> 8 -> 11 -> 5 -> 20 -> 24 -> ..., each demo fight
renders its own stage and characters, no watchdog stall, no crash over an
eight-minute soak. Cxbx-Reloaded on the same XBE runs 0 -> 12 -> 20 -> 14 ->
9 -> 1 -> 9 -> 8 -> 11, so the order matches the hardware oracle. The opening
movie is unchanged at 568 presented frames.

### Callee-saved ABI leak in the texture-stage applier's callees

`sub_001BCC00` (`ret 0xC`) and `sub_001BC260` (`ret 0x10`), both called from
the texture-stage applier `sub_001B6410`, returned with `ebx`/`esi`/`edi`
clobbered and the guest stack 84 and 72 bytes low. `sub_001B6410` holds the
D3D device -- guest `0x001C0800`, published in `0x001C3390` by
`D3DDevice_CreateDevice` at `0x001B4F00` -- in `esi` across both calls and
pushes it to `XMETAL_StartPush` at `0x001B6496`; its second StartPush site at
`0x001B6790` reads the device from `[esp+0x30]`. The device therefore arrived
as null. The callee-saved checker the pipeline already emits around those call
sites named both functions and the exact damage (`[CSCHK3]`).

Measured consequences on the title stage, before the fix:

- 106 of every 127 push-buffer wraps carried a null device, and each rewound
  `g_pb_parsed` to the ring base. 100% of parse calls then started at the base
  and ~81 MB was re-translated every two seconds, replaying every
  draw *and every clear* of the frames still in the buffer over the live one:
  ~890 array draws a frame instead of ~263.
- `sub_001B6410`'s own pushes landed at the ring base, on top of the live
  command stream. That produced ~1000 undecodable DMA words every two seconds
  and the parse walked past each of them a dword at a time,
  spraying floats and vertex data into `SET_BLEND_FUNC`, `SET_FRONT_FACE`,
  `SET_DEPTH_FUNC`, `SET_SURFACE_PITCH` and the matrices.
- ~600 batches every two seconds transformed to NaN.

`src/game/recomp/recomp_manual.c` now ABI-enforces both (bodies renamed
`_gen` in `gen/recomp_0010.c`).
After the fix: re-parse gone (6.8 MB every two seconds), undecodable words 0,
NaN batches 0, the blend-factor registers only ever hold the three values the
game actually programs, ~263 draws a frame, and the title phase runs at ~53
fps in the debug build instead of ~23.

Two "obvious" fixes for the same symptoms were tried first and both regressed;
do not repeat them:

- Stopping the parse on an undecodable word instead of walking past it dropped
  83 MB every two seconds and froze the picture earlier (run 205).
- Skipping the parse-cursor rewind for wraps whose device is not the main one
  produced **zero draws** (run 207) -- while the device pointer was being lost,
  that rewind was the only thing making those pushes reachable.

### Guest blend state is applied

`SET_BLEND_ENABLE` / `SET_BLEND_FUNC_SFACTOR` / `SET_BLEND_FUNC_DFACTOR` were
tracked and never read: `nv_apply_draw_state` forced `ALPHABLENDENABLE` on with
`SRCALPHA`/`INVSRCALPHA` for every draw. Measured on the title stage, the guest
asks for blending **off on ~65%** of its batches, and uses two functions when
it is on (`SRCALPHA`/`INVSRCALPHA` and the additive `SRCALPHA`/`ONE`); a third,
`DESTCOLOR`/`INVSRCCOLOR`, appears in places. Forcing blending on made every
opaque surface depend on an alpha channel it was not written for and rendered
the additive pass as an ordinary blend.

`nv2a_blend_to_d3d` now covers the whole NV2A factor set and returns 0 for a
value that is not a blend factor at all, in which case the caller keeps the last
pair the guest really programmed rather than mapping garbage to `ONE` (which
turns an ordinary surface into an additive blowout).

### Near-plane clipping also clips on W

`nv_clip_triangle` clipped only against `Zc >= 0`. `Zc` here is the pre-divide
depth (`o->z * W`), so for a vertex behind the eye -- `W < 0` -- a positive
view-space depth term comes back out as a **positive** `Zc` and the vertex was
kept, with its screen position mirrored through the origin. Those are the
triangles that sweep across the frame. The clipper now runs Sutherland-Hodgman
against `Wc > 0` first and `Zc >= 0` second (two planes can turn a triangle into
a 5-gon, so the output buffer is 9 vertices per source triangle, not 6).

Batches whose transform produces a non-finite vertex are dropped: D3D's
behaviour is undefined there and what comes out is a random smear.

### Mip chains are uploaded

Only level 0 was ever uploaded and `D3DTSS_MIPFILTER` was pinned to `NONE`.
The title stage declares 3 to 9 levels on most of its draws
(`SET_TEXTURE_FORMAT` bits 19:16) and asks for minification enum **4 =
LINEAR_MIPMAP_NEAREST** on every one of them, so every minified surface was
aliasing -- the "blocky" textures.

`get_dynamic_texture` now creates the texture with the declared level count
(clamped to the real chain length) and uploads each level from the Xbox's
packed chain: level *n* starts immediately after level *n-1*, dimensions
halving with a floor of 1, DXT levels sized by block count. The whole chain is
bounds-checked against guest RAM, not just level 0. Linear surfaces (movie
frames, render targets) stay single-level.

The D3D8 shim needed per-level support for this: `D3D8Texture` now carries
`level_mem` / `level_pitch` / `level_w` / `level_h`, `LockRect`/`UnlockRect`
accept any level below the level count, and `UnlockRect` unswizzles and uploads
into that level's subresource.

### Sampler-state cache

`d3d8_states_apply_sampler` released and recreated an `ID3D11SamplerState` for
all four stages on every state apply. The NV2A translator applies state per
draw, so the title stage was making ~3500 `CreateSamplerState` calls a frame for
a handful of distinct samplers. They are now cached by the six stage states that
feed the descriptor, and a stage that already has the right sampler bound does
nothing.


## Regeneration Contract

A full pipeline regeneration can overwrite generated fixes. Run from `doa3/`:

```powershell
py -3 tools/xbe_parser/xbe_parser.py ../doa3gamefiles/default.xbe --json tools/xbe_parser/doa3_analysis.json --quiet
py -3 -m tools.disasm ../doa3gamefiles/default.xbe --force -v
py -3 -m tools.func_id ../doa3gamefiles/default.xbe
py -3 -m tools.recomp ../doa3gamefiles/default.xbe --all --split 1000
```

The analysis filename is retained for loader compatibility even though it
contains DOA3 data. Use full disassembly; data sections marked executable by the
XBE must still be excluded by section name.

After regeneration:

1. Re-run missing-function seeding until every valid in-section call target has
   a generated definition and dispatch entry.
2. Confirm immediate function-pointer targets, thread starts, callbacks, and
   vtable entries are seeded; direct-call scans cannot discover all of them.
3. Restore the six PSGSFD fall-throughs listed above unless the generator has
   gained a general fix for adjacent split blocks.
4. Confirm the `0x0017C025` deferred comparison still snapshots operands before
   `ebp` is overwritten.
5. Confirm generated PSGSFD code emits `mmx_pavgb`; the lifter mapping should
   make this survive regeneration.
6. Confirm `fnstcw`/`fldcw`/`frndint` emit the `g_x87_cw` / `x87_frndint`
   forms (the lifter now does this) and that `sub_0018DF33` still preserves
   `g_seh_ebp` across `sub_00191848` -- a fragment-boundary fix the lifter
   does not yet make on its own.
6. Re-run `py -3 -m tools.recomp.fix_deferred_cmp` (deferred compares whose
   operands are overwritten before the branch) -- 271 sites.
7. Re-seed the eight attract-flow function-pointer targets and re-emit
   `recomp_seedattract.c`; confirm `recomp_dispatch.c` stays sorted and its
   size constant covers the new entries.
8. Build Release and run the opening movie before accepting regenerated output.

Do not edit generated files casually. When a generated correction is general,
implement it in the lifter/translator as well; keep a local generated patch only
when the correction is specific to an imperfect function boundary or this XBE.

### Generated support units

Several generated translation units are deliberate pipeline supplements and
must be recreated or preserved during regeneration:

- `recomp_extra.c` and `recomp_extra2.c` cover seeded direct and indirect
   targets that ordinary function discovery misses.
- `recomp_fpufix.c` contains corrected x87 translations whose originals are
   retained under alternate names.
- `recomp_jumptables.c` carries switch bodies that cross imperfect detected
   function boundaries.
- `recomp_mwply.c` and `recomp_psgsfd.c` contain CRI movie functions emitted
   from the non-main XBE code sections.
- `recomp_seedattract.c` carries the eight attract-flow entry points that only
   a function-pointer table references.

Do not treat these files as disposable build products. A regeneration is not
complete until their symbols and behavior are represented in the new output.

## Diagnostics

Normal runs write `doa3_log.txt`; generated logs and frame captures are ignored
by Git.

Useful opt-in switches:

- `DOA3_DISPLAYTRACE=1`: capture exact display-time YUV/BGRA and descriptor data.
- `DOA3_REFTRACE=1`: trace reference-pair and plane-integrity behavior.
- `DOA3_IFRAME=1`: dump selected completion-time planes and an index CSV.

Frame dumps are evidence, not perceptual acceptance. For visible or audible
issues, the running game is the final validation.

## Next Work

### The two render-list walkers spin on a bad record

`sub_00158DE0` (flat list at `0x00A1F388`) and `sub_00159180` (block-chained
list from `[0x0099A1F8]`) both dispatch on a record type word of 0, 1 or 2 and
send anything else to a bound check that does not advance the cursor -- the
guest spins at 100% CPU with the process alive. That is the hang about a minute
into the title phase; real hardware would spin too, so the list is genuinely
bad.

`[WALKCHK]` validates the whole chain at every walk entry and it has **never**
reported a bad chain, so the corruption happens *during* the walk. Captured
live at the stall, the record the walk stopped on held `0x001C0800` -- the
address of the D3D device object, i.e. a push-buffer method/parameter pair
written through the wrong cursor -- with runs of small integers around it.
`src/game/main.c` carries an opt-in write watch (`DOA3_WATCHVA=<hex guest VA>`,
`DOA3_WATCHLEN=<hex>`) that reports the writing RIP; it is armed post-movie and
has not yet been pointed at the list pages.

### The pixel pipeline is fixed-function only

DOA3 drives colour through the NV2A register combiners
(`SET_COMBINER_*`, ~330k writes every two seconds) and the translator
substitutes a single fixed-function stage. Only texture stage 0 is ever bound,
which matches the guest -- `[TEXCTL0]` shows stages 1 to 3 enabled zero times --
so multi-texturing is not the gap; the combiner program is.

`SET_SURFACE_ZETA_OFFSET` (0x0214) and `SET_WINDOW_CLIP_*` (0x02B4/0x02C0/
0x02E0) are still ignored, so the render-to-texture pass shares the main depth
buffer and no scissor is applied.

Hardware vertex blending is **not** a gap: `SET_SKIN_MODE` is written with
non-zero modes, but model-view matrices 1 to 3 (`0x04C0`, `0x0500`, `0x0540`)
are never uploaded, so there is nothing to blend.

