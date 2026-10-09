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
$env:DOA3_XBE = "build/release/HDD/D/default.xbe"   # the 3.1 XBE
py -3 tools/xbe_parser/xbe_parser.py $env:DOA3_XBE --json tools/xbe_parser/doa3_analysis.json --quiet
py -3 -m tools.disasm $env:DOA3_XBE --force
py -3 -m tools.xbe_layout --crt-initializers > crt_ctors.txt
py -3 -m tools.recomp.seed_missing_functions crt_ctors.txt
py -3 -m tools.recomp.find_pointer_targets > ptr_seeds.txt
py -3 -m tools.recomp.seed_missing_functions ptr_seeds.txt
py -3 -m tools.recomp.extend_functions ptr_seeds.txt
# repeat until find_unresolved prints nothing:
py -3 -m tools.func_id $env:DOA3_XBE
py -3 -m tools.recomp $env:DOA3_XBE --all --split 1000
py -3 -m tools.recomp.find_unresolved > unresolved.txt
py -3 -m tools.recomp.seed_missing_functions unresolved.txt
py -3 -m tools.recomp.extend_functions ptr_seeds.txt
# then the generated-code fix-ups, and the runtime layout header:
py -3 -m tools.recomp.postprocess
py -3 -m tools.xbe_layout --header src/game/recomp/gen/xbe_layout.h
```

`find_unresolved` lists symbols referenced but not defined in the generated
code, standing in for MSVC's "unresolved external symbol" errors.

`find_pointer_targets` seeds code reached only through pointers: immediates
(`push`/`mov` of a code address: thread starts, callbacks) and pointer tables
in `.rdata`/`.data` (vtables, handler tables), kept when the target is an
instruction boundary that looks like a function entry. Run on 3.0 against
upstream's function list it rediscovers all nine pointer-only entries upstream
seeded by hand (the eight attract-flow handlers and `0x000E5590`). On 3.1 it
added 554 entries, including XAPI's thread trampoline (`0x0018C690`) and the
game's main-thread routine (`0x0018CB1A`): without them the first
`CreateThread` resolved to nothing and the entry point returned at once.

Current result: 10,757 entries (5,632 detected, the rest seeds: call targets,
64 initializer-table entries, 554 pointer targets), 0 unresolved. Seven are
empty "translation failed" stubs for wild targets decoded from inline data,
as in the first pass.

## Function extents (`tools.recomp.extend_functions`)

The detector often ends a function early, and seeding then turns the rest of
it into separate fragments; the translator emits a jump between fragments as
a C call, so a loop spanning fragments recursed natively once per iteration.
The fourth 3.1 run died silently (no handler output) right after CRT init;
the tree had 267 such call cycles (750 fragments), the CRT heap allocator
among them. `extend_functions` grows each function over the code reachable
from its entry by fall-through and jumps (not calls), stopping at other real
entries and at anything that is not reachable code or padding. On 3.1:
3,676 functions extended (+745 KB), cycles 267 -> 59, and the remaining ones
are mostly branch-target fragments whose code is now also covered by the real
function (e.g. the allocator at 0x1BA53A spans to 0x1BB0D4). Restored
fall-throughs dropped from 1,096 to 74.

`main.c` now reserves 64 KB of stack for the exception handlers
(`SetThreadStackGuarantee`), so a host stack overflow gets logged.

## Diagnostics for silent deaths

The fifth 3.1 run still died with nothing logged after CRT init. Added:

- `[KCALL]` lines in `doa3_log.txt`: the first 400 kernel calls (ordinal,
  fiber, guest esp, first two args), then a count every 2 s. `DOA3_KTRACE=N`
  changes the 400.
- A ring of the last 32 kernel calls (`[KTRACE]`), dumped on the first
  non-AV exception, on an unhandled exception, and at process detach.
- A process-detach hook (TLS callback): `[EXIT] process detach` means the
  process ended through ExitProcess / return from main; no such line means it
  was killed (fast fail, double fault).
- 64 KB stack guarantee in every fiber, not just the main thread (each fiber
  keeps its own, and new fibers start with none).
- Release builds now write `DOA3.map` and a PDB, to map crash offsets.

## Kernel ordinals (XDK 4134 table)

The bridge's ordinal tables came from another title and were tuned against
3.0 by hand; several entries carry a neighbouring export's name, argument
count or data/function kind (14 ExAllocatePool was DbgPrint, 17 ExFreePool
and 65 IoCreateDevice were data, 16/40/354 data exports were functions, 46
HalReadWritePCISpace popped 8 bytes of 24, 67 was IoCreateFile, 87 is a
fastcall, 335-337 are XcSHA*, 305 RtlTimeToTimeFields had no bridge). The
sixth 3.1 run's last kernel calls were D3D's miniport init: two
HalReadWritePCISpace calls 16 bytes apart in guest esp, then
RtlTimeToTimeFields, whose unfilled month drives a loop at 0x1E551F.

Builds other than 3.0 now resolve every import through `g_kx_ords` in
`kernel_bridge.c` (names, conventions and argument sizes from the retail
kernel, checked against Cxbx-Reloaded's export definitions and the push
counts at the XBE's own call sites). 3.0 keeps the old tables. New bridges:
pool alloc/free/size, ExQueryNonVolatileSetting (language, video, AV and
game region per release), PCI config space, HalReturnToFirmware/KeBugCheck/
HalInitiateShutdown (logged as `[HALT]`), IoCreateDevice,
KeQueryInterruptTime, MmCreateKernelStack, NtQueryVirtualMemory, the Rtl
time conversions, RtlCompareMemoryUlong, RtlEqualString, XcSHA*,
NtUserIoApcDispatcher.

The watchdog now also arms before the first present: 6 s without a kernel
call logs `[WDOG] STALLED` with the host RIP, return addresses into
DOA3.exe (look them up in DOA3.map) and the kernel-call ring.

## Guest-stack probes (`tools.recomp.esp_probes`)

The seventh run got past the kernel fixes into D3D's CreateDevice and died
in `InitializeFrameBuffers` (0x1E30C0) copying a "BufferSurfaces" array
with a garbage count: its presentation-parameters pointer, read from
`[esp+0x174]` in the frame-pointer-less `CDevice::Init` (0x1E35B0), came
from a guest esp 16 bytes below where it should have been. Something
between the two leaves the stack unbalanced.

`esp_probes` renames each generated function whose `ret`s agree on a size
to `sub_X_gen` and adds a wrapper `sub_X` (gen/recomp_probes.c) that checks
the function moved esp by exactly 4 + N; mismatches are logged as `[ESP]`
lines (first two per function, 400 in all, `DOA3_ESPPROBE=0` to silence).
Only real entries are probed (not seeded fragments, which start mid-function,
and not functions framed by the SEH prolog/epilog helpers): 5,033 on 3.1.
`--remove` takes them out again. Run it after `postprocess`.

First result (eighth run): `sub_001E2D10 moved -12, expected +4`. Its callee
0x1E00C0 (D3D's shader-constant-mode setter) had been cut into fragments at
0x1E00C6 and 0x1E0109, and the 0x1E00DA fragment ended on a `push esi` with
its fall-through dropped. Two causes, both fixed:

- `find_pointer_targets` accepted data tables of small consecutive numbers
  (0x1E00C4, 0x1E00C5, 0x1E00C6, ...) as pointer tables, because any
  existing function start counted as an entry, including seeded fragments,
  and a bare `push ebx` counted as a prologue. Now only detector-found
  starts and real prologues/terminator boundaries count.
- `extend_functions` treated every linear-sweep call target as a function
  entry; 4,233 of them land mid-function. They now need to look like an
  entry. 0x1E00C0 now spans 0x1E00C0-0x1E01B5.
- `fix_fallthroughs` covered only `.text` (+D3DX) on 3.1, leaving the D3D,
  DSOUND and XPP sections unfixed; with the disassembly guard it now covers
  every code section on non-3.0 builds (69 restored after the regeneration). The gen source glob now has CONFIGURE_DEPENDS so new
generated files are picked up without a manual cmake re-run.

## Drives (`kernel_path.c`)

The console's drives are folders under `HDD\` next to the exe, one per
partition, as Cxbx-Reloaded lays out its emulated disk: `HDD\D` is the
ripped disc (D:, `\Device\CdRom0`; the first-run setup extracts the XISO
there), `HDD\E\TDATA\<title id>` and `HDD\E\UDATA\<title id>` are T: and
U: (3.1's title id is 54430001, so saves can move to and from a real console
or Cxbx-R as they are), `HDD\X|Y|Z` are the cache partitions (the game's own
first-boot install copies the AFS files to Z:), and C, F, G map the same way,
along with their `\Device\Harddisk0\PartitionN` names (1 = E, 2 = C,
3-5 = X-Z, 6-7 = F-G). An `assets` folder from before this layout is renamed
to `HDD\D` once at startup. Nothing goes to AppData any more. The first use
of each mapping is logged (`[PATH]` in xbox_kernel.log).

## Overrides (`tools.recomp.apply_overrides`)

`tools/recomp/overrides_<version>.txt` lists guest functions replaced by
hand-written code in `recomp_manual.c`; `apply_overrides` renames their
generated bodies to `sub_X_gen` (run after `postprocess`, before
`esp_probes`, which skips them).

3.1 so far:

- `0x1E27C0` CDevice::KickOff. The ninth run created the device and then
  hung in BlockOnTime (0x1E2960), spinning on the fence semaphore
  `[[dev+0x34]]` that only a GPU writes. The override translates the push
  buffer written since the last kick to D3D11 (as 3.0's KickOff override
  does; `DOA3_PB=0` turns that off), publishes DMA_PUT/GET, and sets the
  semaphore to the last fence inserted (`[dev+0x30] - 2`), so every wait
  on the GPU returns at once. XDK 4134 CDevice fields are listed above the
  override. When the translated range contains NV097_FLIP_STALL (0x130, the
  end of D3DDevice_Swap) the frame is presented (`doa3_present_frame`, which
  also pumps the window) and CMiniport's VBlank handler (0x1E4AA0, this =
  dev+0x2268) is run once: vblank count, pending flips, vblank event and the
  game's vertical-blank callback, which 3.0 emulates by hand in its SetFence
  override. Without a present the window stopped pumping and Windows marked
  the game "Not Responding" while it was still running (tenth run: 20,000
  kernel calls and steady KickOffs, no frames shown).
- `0x18C934` / `0x18C9C0` / `0x18C9D3` XAPI CreateFiber / DeleteFiber /
  SwitchToFiber, backed by the host coroutines in xbox_fiber.c exactly as on
  3.0. The lifted SwitchToFiber moved esp onto the other fiber's stack and
  then returned to its caller (`[ESP] sub_0018C9D3 ... moved +22077656`).

More 3.1 overrides (twelfth run: one frame presented, then the first game
task spun forever re-opening an AFS file, `[CRI] 'ptid' is range outside`):

- `0xA5C60` DirectSound effects-image download (dsstdfx.bin) returns success,
  as 3.0's `0x9F640` stub does. The audio init `0xA5DC0` skips the whole CRI
  setup (`0x1C79F0`: ADX server threads, AFS partitions) when it fails.
- `0x1B73D0` / `0x1B7CB0` CRT memmove, native (3.0: 0x18DF40 / 0x18EE90).
- `0x19A330` CRI message sink, logs `[CRI]` lines.

Thirteenth run: the CRI setup ran (files opened, four CRI threads spawned
as fibers), then the first CRI thread spun forever in XAPI's thread
notification walk (0x18C3E1). Two fixes:

- main.c no longer runs the `__xi`/`__xc` initializer tables on 3.1: XAPI's
  main-thread routine (0x18CB1A) runs them itself (0x18FDF5, 0x18FD9D), so
  everything was initialised twice, and the CRT's thread-notification entry
  went into XAPI's circular list at 0x2521CC twice, which made the list
  loop on itself. (3.0 enters past that code and still needs them.)
- `0x1926B0` CRI idle/watchdog thread yields every lap, as 3.0's override
  of 0x16A530 does; xbox_fiber.c's "not a real worker" exclusion now knows
  the 3.1 address too.

Fourteenth run: the CRI setup reached its partition load and the main
fiber spun in `do { st = ADXF_GetPtStat(2); } while (st != 3)` (0x1C7AE0)
while the four CRI threads sat READY: nothing ever handed them the CPU.
On hardware the scheduler preempts that loop. Two changes:

- Every esp-probe wrapper is now also a scheduling point: when the 4 ms
  tick is due it calls `xbox_fiber_timeslice()`, as RECOMP_ICALL_SAFE and
  the kernel dispatcher already do. The slice timer is started at
  `xbox_fiber_init` (it was only created on the first due slice, i.e. never,
  unless something else called the timeslice first).
- `doa3_workers_may_run` lets slices run from the start; 3.0 waits for its
  intro movie and two CRI lock words. TODO(3.1): the lock check.

Fifteenth run: the game loop ran (2,400+ frames presented) and the first
-boot HDD cache install started (installer thread, start context 0x1C67C0,
copies each AFS from d:\ to z:\), but the boot's join on it was answered
"finished" at once, so it opened `z:\loadfile.afs` before the copy existed
and ADXF parked partition 2 in error. 3.0 hit exactly this; the thread-join
logic in `bridge_NtWaitForSingleObject` (poll -> STATUS_TIMEOUT while the
installer fiber is alive, block -> yield until it exits, pulsing the vblank
event for the CRI workers) now covers 3.1 too. (Drive layout: see Drives above.)

Diagnostics added on the way: `[KCALL]` lines now end with the bridge's
return value; `DOA3_TRACE_FN=addr,addr,...` logs returns of probed functions
(`[FN]` lines, with stack and register arguments). The NtQueryInformationFile
FileNetworkOpenInformation reply had AllocationSize and EndOfFile swapped,
so GetFileSize returned sizes rounded up to 4 KB.

Twenty-fifth run: hung after ~2,900 frames with the game task spinning in
0x192640 (ADXM "run the group-5 thread now": raise its priority, resume
it, wait for it to clear [0x40C6FC]). The group-5 thread (0x1927D0, handle
in [0xC80078]) had started before ADXM_SetupThrd stored its handle --
our worker fibers start READY, XAPI creates them suspended -- so its first
SuspendThread named handle 0 and it parked on a key no resume ever wakes.
`bridge_NtSuspendThread` now treats a zero / current-thread handle as the
caller's own (shared) thread handle. The ADXM threads (0x192870):
[0xC80070] idle 0x1926B0 (resumed by the ADXM lock so it out-prioritises
the servers while the lock is held), [0xC8008C] vsync 0x1926F0, [0xC80094]
mwPly 0x192760, [0xC80078] group 5 0x1927D0. Lock 0x1925D0 / unlock
0x192610 (via 0x19A2F0 / 0x19A310); XAPI ResumeThread 0x18C548,
SuspendThread 0x18C522.

## Generated-code fix-ups (`tools.recomp.postprocess`)

Runs upstream's fixers for the lifter defect classes in NOTES.md, in order.
Counts on 3.1:

| Fixer | 3.1 | Notes |
|---|---|---|
| `fix_fallthroughs` | 1,096 restored | range = the XBE's `.text` + D3DX (3.0: 0x11000-0x1B0DE0, as upstream). For builds other than 3.0 each candidate is checked against the original code first; 32 skipped (19 data decoded as code, 11 calls to no-return functions, 2 into padding). 3.0's hand-proven XDK exceptions are 3.0-only. |
| `fix_ftol_inline` | 1,541 | `__ftol2` address from the layout; the inline form now uses `g_fp_stack`/`g_fp_top` directly, since the `fp_*` macros only exist in functions that touch the x87 stack |
| `fix_cond_tailcall_ebp` | 9 | |
| `fix_fpu_global` | 0 | the lifter already emits the global stack |
| `fix_deferred_cmp` | 0 (was 791) | now covered by the lifter's flag snapshots, below |
| `fix_selfspins` | 11 | selects XDK functions by section address instead of upstream's `recomp_0010/0011.c` |

The third 3.1 run hung after CRT init with 264 bytes of guest-stack drift in
two constructors: `_getptd` (0x1BB573) had been split at a seeded branch
target (0x1BB5AC) whose fall-through was dropped, so every call leaked 12
bytes and returned garbage instead of the per-thread data pointer. That is
one of the 1,096.

**Flag snapshots (lifter).** The lifter rebuilds a jcc/setcc/cmovcc/adc/sbb
condition from the flag-setter's operands at the consumer, so an operand
rewritten in between made the branch test the new value. `fix_deferred_cmp`
patched that afterwards for `cmp`/`test` only. The ADX stereo decoder
(0x19DC30) ends its inner loop with `dec ecx / mov [esp+24], ecx /
mov ecx, [esp+28] / ... / jne`; the lifted `if (ecx != 0)` tested the
reloaded source pointer, so the loop never ended and the decoder wrote PCM
across all of guest RAM and on through the mirror views (twenty-sixth run:
the `.data` corruption, the CRI thread-mode flag at 0x25478C, then a
stack overflow in the thread-notify walk). `lift_basic_block` now copies the
operands into `_fsN` locals when an instruction may write a register or
memory they read and a consumer can still follow, for every integer
flag-setter. 34,401 snapshots on 3.1; the regenerated code differs from the
previous output only in those lines.

`cmpxchg` was never lifted (a `TODO` comment). Its one real use on 3.1 is
DirectSound's DPC (0x1F3FFA, run under KeSynchronizeExecution from the DPC
loop 0x1F4496): it swaps the ISR's pending bits out with `cmpxchg`, so with
no store the bits never cleared and the DPC looped forever once the intro
movie's voice finished (twenty-seventh run). Now lifted, with ZF in `_flags`.

Intro movie (twenty-seventh run: ninja.sfd's audio played on a black
screen). Two causes: `doa3_movie_host_owns_screen` reported 3.0's host
presenter as owning the screen from boot until its movie ended, so on 3.1
(no host presenter) every guest draw, clear and flip was dropped for good;
and nothing showed the movie, which Sofdec writes straight to the display.
On builds other than 3.0 the screen is now "owned" only while guest movie
frames arrive, and the Sofdec frame copy + colour conversion (0x19EA30, the
same code as 3.0's 0x1762B0) is wrapped to show each converted frame
(`doa3_present_movie_guest`, guest-decoded, no host decoder).

Twenty-eighth run: the legal screen and TECMO logo drew, but each lasted a
few host frames: the flip completes at once, so the game loop free-ran at
~500 frames/s, and every per-frame counter in the game follows the flips.
`d3d4134_frame_done` now holds each flip to the next 1/60 s deadline (3.0's
Present-wrapper pacer), running ready worker fibers while it waits, as the
CRI threads run on hardware while the game thread waits for vblank.
`DOA3_NOPACE=1` turns it off. The movie copied only 6 frames and the
ninja.sfd stream stopped reading at ~20 MB of 35 MB; with the game loop
paced, see whether the decoder keeps up.

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
