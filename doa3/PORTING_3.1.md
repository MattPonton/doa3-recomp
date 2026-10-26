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

Thirtieth run: the movie handle went PLAYING -> -4 at frame 616 with
`SFD ERROR(FF000C09)`: Sofdec's check of its ADX stream (0x1A43A0) found
ADXT error -2, set by ADXT_ExecErrChk (0x19634F) when the decoded-sample
count has not moved for 5 x svrfreq server ticks. Those ticks are vblanks on
hardware; here every vblank wait was answered at once, so the vsync/mwPly
threads ticked millions of times a second and the check fired within
microseconds. Worker fibers now park on `XBOX_FIB_VBLANK_KEY` when they wait
for the D3D vblank event and are released at 59.94 Hz
(`xbox_fiber_vblank_tick`, run at every scheduling decision). The flip pacer
also gives worker laps while a game-task coroutine waits.

Thirty-first run (0.0.37): the movie then took ~30 s to prebuffer and its
ADX stream starved (`SFD ERROR(FF000C08)`, ADXT error -1). [FIBRUNS] showed
the group-5 decode thread resumed 180 times a second but its server
(0x19A690) not running: ADXM's lock release (0x192610) SUSPENDS the idle
thread [0xC80070], and with one shared thread handle the bridge parked the
caller instead, so the decode server advanced one lock step per wake-up.
3.1 now gives every thread its own handle (0xBEEF0100 + n); NtSuspendThread
blocks only a thread suspending itself and ignores suspending another one.

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

Thirty-third run (0.0.39): the intro movie played (540 frames copied) but
showed as a 2-pixel strip at the top of the window. Sofdec's MMX colour
converter (0x1A3480) ends its row loop with `dec eax / mov [ebp+10h], eax /
ja`; the lifter had no condition for ja/jbe after inc/dec and fell back to
`if (_flags)`, always false, so each frame got two rows. inc/dec now take CF
as 0 for ja/jbe (CF survives from the previous instruction, a pointer add
there), and add gets ja/jbe from its result (CF = result < addend). A
"/* unhandled flags: setter -> jcc */" comment now marks every remaining
fallback (35 sites left on 3.1: imul/sbb/adc combinations, mostly in code
that is never reached).

Thirty-fourth run (0.0.40): the movie shows, with green and stale 16x16
blocks in its early frames. Two instructions the lifter left as TODO
comments are in the paths it runs: `rdtsc` (Sofdec's decode timer 0x1A9300
at 733 MHz, XAPI's QueryPerformanceCounter 0x18B744) and `rcr` (the CRT's
64-bit divide/remainder helpers 0x1B87A0/0x1B8890/0x1B8940/0x1BD580 shift a
64-bit pair with `shr hi,1 / rcr lo,1`, so large-divisor 64-bit divisions
were wrong). rdtsc now reads host QPC scaled to 733.33 MHz
(`doa3_guest_rdtsc`); rcr/rcl by one rotate through `_cf`, which the
preceding shift latches. The movie file is read to its last byte (frame
1472) and the ADX error follows ~4 s later: the audio side never sees the
end of its data.


Thirty-fifth/sixth runs (0.0.41/0.0.42): blocks unchanged. The green ones
are in every other frame (frame means alternate ~41/~58), i.e. the
B-pictures. The picture-header parser 0x1AAEE0 picks its macroblock
routines from per-picture-type tables (0x21ACC8..0x21ADA8, five slots per
table, mostly null). The B-only slots 0x21AD80/0x21ADA8 hold 0x20E200,
0x20E290 (and 0x20CC10/0x20CC70 for the other mode). Each sits alone
between nulls, so find_pointer_targets (runs of >= 2 code pointers) never
saw them, and the detector folded them into the preceding functions.
`recomp_icall_fail_log` counted the misses without printing anything, so
every B-macroblock those routines own was skipped silently: zero YUV
(green) in buffers never written yet, stale blocks from older frames
after that. Null slots no longer end a pointer run; the four are seeded
(tools/recomp/ptr_seeds_3.1.txt, seed_missing_functions) and the
unresolved-icall log now names each new target once ([ICALL]). The widened
scan also proposes 11 more targets, mostly SEH filter/handler blocks
(`mov reg,[ebp-0x18]` after a ret) - not seeded yet.

The ADX end: ADXSJD (0xC7BC80) decoded all 911,881 samples of the movie's
ADX header ([adxt+0x64]), but ADXT stayed in state 3 and the input ran dry
-> error -1 (FF000C08). 0x1952E0 sets decode-end only while the decoder is
idle; 0.0.43 logs [ADXSJD] lines from the input end to the error.

Thirty-seventh run (0.0.43): the ninja movie plays clean; it still ends in
FF000C08. At the error ADXSJD had decoded all 911,881 samples, its decoder
was idle and [sjd+0x34] == [dec+0x18], so the next pass would have set
decode-end. The error came early: ADXT's check 0x196300 lets the input run
dry for svrfreq*5 ticks (5 s) before raising -1, but its `jle` at 0x1963F3
is shared by two paths, one of which arrives by `jmp` after its own `cmp`.
The lifter carried flags only from the block lifted just before, so that
path compared the other path's registers (300 <= 60: false) and raised the
error on the first dry tick. The translator now joins pending flags at
blocks that read them: every predecessor latches its compare operands into
_fjN locals and the jcc reads those (_lift_blocks_with_joins; 550 latches
across the game, mostly compiler-merged compare chains). Two more indirect
targets from the [ICALL] log are seeded (XPP 0x210A43, 0x2111F2).

Thirty-eighth run (0.0.44): the ninja movie ends normally, the attract flag
0x593038 goes to 1 (frame 1780) and mv_op.sfd plays through. Right after it
(frame 8276, sfd state 4 -> 1 -> 6 -> 1 -> 0) DirectSound's AddRef 0x1F1816
ran `inc [this+4]` with this = 0xF57F4AA9. Host = guest + 0x20000000 put
that inside a fiber stack's guard page; the guard handler resumed the access
against the host stack and the process then died at rip=0. 0.0.45: a guard
fault from recompiled code at guest 0x08000000..0xF0000000 now restores the
guard and skips the instruction, and the guard / native-crash handlers print
the host call stack ([BT], exe offsets for DOA3.map) and the last probed
returns ([RING]) to find where the bad pointer came from.

Thirty-ninth run (0.0.45): same crash, with a stack this time: frame present
-> doa3_apu_deliver_irq -> DSOUND DPC 0x1F4560/0x1F4496 -> voice service
0x1F3FA0, which walks three voice lists (+0x6C4/+0x6CC/+0x6D4, obj =
node-0x4C) calling [vtbl+0x14]. One node's object carried a COM vtable
(0x218090/0x2180B4, slot 0x14 = AddRef 0x1F1816), so `this` came from
the stack as garbage (0xF5../0xF9.. guest, host fiber stacks; the guard
skip did not cover guest >= 0xF0000000). A sound effect was heard at the
crash. Two fidelity gaps on that path: DirectSound reads its IRQL from the
KPCR (`fs:[0x24]`, guest byte 0x24 here), which stayed 0 inside our DPCs,
so the DPC took the DSound critical section like a thread would; and
neither the APU interrupt delivery nor the worker timeslice looked at the
emulated IRQL. 0.0.46 mirrors IRQL into guest byte 0x24, runs the ISR at
DIRQL and DPCs at DISPATCH, holds the interrupt while IRQL >= DISPATCH, and
skips timeslices there. The voice-service wrapper ([DSVOICE]) logs every
change to the lists, so if the bad node is still there its arrival shows.

Fortieth run (0.0.46): same end-of-movie crash, now explained by [DSVOICE].
At frame 1765 (mv_op start) a voice node appeared in the APU's active list
whose object already carried the root vtable 0x218090 -- the one the voice
destructor chain (0x1F6642 -> 0x1F5566) leaves behind. A voice leaves that
list only from the interrupt path (0x1F44E8 -> 0x1F40D6 -> 0x1F4EF7); the
thread side waits for it in 0x1F5157 (`while (flags & 0x8000)`), which
fix_selfspins had removed because nothing interrupts recompiled code. 3.0
handles its copy of this wait by delivering the APU interrupt from inside it
(doa3_apu_wait_retire); 0x1F5157 is now overridden the same way. At frame
8277 the dangling node's memory was reused and the list pointed at itself.
DirectSound's auto-lock (0x1F3C45) also confirms the IRQL work: it raises to
DISPATCH only when fs:[0x24] < 2.

Input: no XPP override existed for 3.1, so no pad reached the game. The
3.0 overrides (XGetDevices, XGetDeviceChanges, XInputOpen/Close,
XInputGetCapabilities, XInputPoll, XInputGetState) are ported: the XDK code
matches byte for byte at 0x21157D/0x21159F/0x211823/0x211898/0x2118A4/
0x211A96/0x211B07, and the pad table moved by +0x1569F8 (0x73C6C8).

Red box at the TECMO fade-out (glitch_0596): that frame's push buffer uploads
a vertex program (0x0B00.., execution mode 0x1E94 = 6) for the fade quad and
the logo's transparent texels come out dark red. Not looked at further yet.

Forty-first run (0.0.47): no crash at the end of mv_op.sfd any more (the
retire wait returned in 0 ms both times); START skips the ninja movie. The
title logo then froze mid-fade and the process died on the fault-skip cap:
the stage loader thread (0xE8030 -> 0xE8290, stage 0x38 -> 0xEFF70 ->
0xF3300) walks -1-terminated short index lists with `test ax,ax / jge`, and
the lifter wrote that as CMP_GE(LO16(eax) & LO16(eax), 0). The AND promotes
to int, so CMP_GE compared at 32 bits and 0xFFFF counted as >= 0: the walk
ran off the end. Signed/unsigned conditions after an 8/16-bit `test` now
cast the AND back to the operand width (474 sites).

Forty-second run (0.0.48): past the title logo. The Press Start screen
rendered, then stopped updating; Nine Lives started (song first, then the
in-engine dance with exploded vertices). START still does not skip mv_op.
The log named five indirect-call targets with no function: 0x9DB70,
0x9DC20, 0x9DCC0, 0x9DCF0, 0x69040 -- code in gaps no function covers. A
scan of every uncovered gap for 16-aligned entries after ret/padding gave
14 plausible ones, now seeded (ptr_seeds_3.1.txt). 0.0.49 also writes a
half-res scene_NNNNN.bmp every 3 s after frame 1500, logs the pad aggregates
at each press, and logs the attract movie player 0xD7490 (it skips on bits
0x300/0x30 of 0x73C8D8 + pad*0x2C). Exe icon: the game's save icon.

Forty-third run (0.0.49): the Press Start screen and the Nine Lives
sequence run (no hang at the hand-off), with exploded geometry: in the
scene snapshots rigid character parts and some stage pieces are right while
the skinned/blended meshes (hair, joints, torso links) are missing or flung
out. The sequence stalls at stage-tier transitions and never leaves Lost
World. START never skipped mv_op: the attract movie is played by
0xD7490's sibling path (0x53E60, action 2), not 0xD7490 (no [ATTRACT] line);
its exit is decided by mp_UpdatePlayerJoinAndStartInput (0x88xxx), which
ignores pads while 0x596AA5 == 1 (set by the stage loader thread while it
loads). Watched now: 0x596AA5, P1/P2 join 0x596A32/33, attract action
0x5A262A, join result 0x5A697C.

New [ICALL] misses: 0x132750..0x1327F0, eight 3-5 instruction x87 helpers
(`fld [esp+8]; fadd [k]; ret`) folded into one link_seed fragment. A scan of
every function range for 16-aligned entries after `ret` + padding found 30
such swallowed functions (incl. a second helper set at 0x141C40..); all
seeded.

Forty-fourth run (0.0.50): Nine Lives stalls at Azuchi's tier drop; START
skips Nine Lives; the main menu and character select are reachable. The
geometry is scrambled differently every frame on every 3D screen (the
animation itself is right). mv_op still does not skip: during it the
loader flag 0x596AA5 stayed 0, attract action was 2, and the P1 join flag
never moved, so the START press is lost before the join logic.

The push-buffer translator in the KickOff override (pb4134_translate)
skipped NV2A jump/call/return words and carried on with the next word in
the ring. After a jump those words are left over from an earlier lap and
were translated as live commands, and calls (precompiled push buffers) were
never followed. 0.0.51 follows jumps and calls (one return slot, as the
NV2A has), finishes the old lap before a ring wrap instead of dropping it,
and logs the control-word counts ([PB] control words). Also: [JOIN] lines
from mp_UpdatePlayerJoinAndStartInput (0x88910) whenever an aggregate has
START, and two consecutive method traces every 1500 frames past 1500.

Forty-fifth run (0.0.51): the scrambled geometry is gone -- the only
control words in the ring were its wrap jumps (one per lap), so the fix was
finishing the old lap before the wrap instead of dropping it. Materials and
reflections are still wrong, character select renders black, tier
transitions halt and some hit reactions are the wrong animation. In a fight
the pause menu reopens one frame after it is dismissed.

Input: 3.1's XAPI has no XInputPoll. 0x211A96 is XInputGetState and
0x211B07 is XInputSetState (rumble; the game calls it every frame its
feedback header is not ERROR_IO_PENDING). The 3.0 port's names for its
copies are one function off and its pad state was written from the rumble
call; ported as-is, 3.1 got its input only through SetState into the
feedback header (pad+0x2F) and a mirror at pad+0x19. Now GetState fills the
state it is given from the host pad and SetState completes at once.

Local rumble fix: SetState now forwards the packed feedback's motor words
at +0x42/+0x44 through the same guest-port-to-XInput-slot map as input, then
writes the Windows result to dwStatus. The layout is verified against
sub_000A50A0 (feedback at pad+0x2F, motors at pad+0x71/+0x73) and the
rumble worker at 0xBB500. Requests still complete synchronously. Close,
focus loss, exit, and port remapping stop outstanding effects. The game's
own per-player vibration option remains in control of which effects it sends.

mv_op: START reaches mp_UpdatePlayerJoinAndStartInput as the attract input
branch (0x5C9254 = 2; 0x5C9248 was already 1), so mp_UpdateTitleAttract
should be running the exit fade 0x53EB0 (gated on 0x5A8B97 == 0, 60-frame
counter 0x5A6970, then 0x53F20). 0.0.52 logs it ([ATTRACT] exit fade).

Forty-sixth run (0.0.52): START during mv_op does arm the exit fade
(0x53EB0: 0x5A697A 0 -> 1, counter 0x5A6970 = 60), but the counter went
60 .. 0 .. 65535 and never ended the movie: `dec word [0x5A6970] / jns`
was lifted as `(int32_t)MEM16(...) >= 0`, which a 16-bit value always is.
Every result-based sign condition (js/jns/jl/jge/jle/jg after
sub/add/adc/sbb/and/or/xor/inc/dec/neg/shifts) took 32-bit width; they now
use the operand's width (_sign_cast), and sub's reconstructed operand for
ordered compares wraps at 8/16 bits. 54 sites. The pause loop in fights is
unchanged by the XPP fix; no device change was reported.

Forty-seventh run (0.0.53): START/A skips mv_op (after the 60-frame fade).
XEMU comparison shots (scene_*.webp beside our scene_*.bmp): most of the
frame matches; missing are the title stage's floor light rows, part of the
torch flames, Christie's specular sheen, floor reflections, and on Azuchi's
top floor the whole room (we show sky). Demo fights also lose wall hits
(a tree hit becomes a knockdown) and stall at tier drops -- possibly the same
missing stage geometry seen by collision. In fights "PAUSED PLAYER 1" comes
back every frame. The pause trigger 0x8A2E0 also fires for a human player
whose assigned pad (0x30E3F4[player]) is not in the open mask 0x73C8C8;
0.0.54 logs its inputs ([PAUSE]) and counts array draws dropped for a
non-finite vertex or clipped away entirely ([DRAW], every 5 s).

Forty-eighth run (0.0.54): the pause trigger 0x8A2E0 reported player 1 with
the pad open (mask 1), assigned (pad 0), claimed, and no START in its
aggregate -- so the function itself was wrong. At 0x8A347 one `jne` is
reached from `test [pad agg],0x10; jmp` and from a fall-through `cmp
[0x5A27A0],1`: different flag setters, which the operand join cannot merge,
so both paths compared the pause-UI byte (0 != 1: "pause"). Joins with
different setters now evaluate the jcc condition in each predecessor into
an _fjN boolean the jcc reads (223 sites). No draws were dropped or clipped
away ([DRAW]), so the missing Azuchi room is never submitted.

Forty-ninth run (0.0.55): first full playthrough. Boot, both movie skips,
Nine Lives with wall hits and tier drops through to its end, Press Start,
Story mode (Hayate) through every fight, the boss, the ending movie (skipped),
"Now Saving" (HDD/E/TDATA/54430001/DOA3SAVE.DAT) and back to Press Start.
Still open: character select draws black, missing glow/specular/reflection
effects (title floor lights, torch flames, Christie's sheen, floor
reflections), the Azuchi top-floor room (never submitted for drawing), no
fade when a movie is skipped. Two more [ICALL] misses (0x10F6E0, 0xCE050)
plus one similar entry (0x91AA0: 16-aligned prologue after padding) seeded.

Fiftieth run (0.0.56): main menu visible; Options, Sparring, Watch and stage
select all work. After Sparring then Story, BGM and voices were gone while
sound effects played: "xbox_HeapAlloc: out of memory" (requests of 0x10014
and 0x8014 = DirectSound pool blocks plus our 16-byte header), then
"ADXT_StartAfs: can't open" and two [ICALL]s through null pointers left by
failed allocations. ExFreePool was a no-op and the heap tracker was a flat
512-entry table (later allocations untracked, so their frees were ignored),
so every DirectSound stream leaked. 0.0.57: ExFreePool frees, the heap
tracks live blocks in a 64K-entry hash and free blocks in a sorted,
coalescing list that also hands blocks at the top back to the bump pointer;
MmQueryAllocationSize answers from it. [HEAP] usage is logged every 10 s.
0.0.57 hung at boot in sub_001F6D91 (DirectSound slop-heap walk). DSound
allocates 4- and 16-byte blocks with page alignment and gives the rest of
each page to its slop heap. The new allocator kept the alignment gaps and
64-byte granules reusable, so a later pool block landed inside that slop and
overwrote its list links. 0.0.58: page-aligned requests always take whole
pages, as the console's Mm* allocators do.

Fifty-first run (0.0.58): no more audio loss. [HEAP] still climbed by
1.35 MB per display-mode change: D3D's PersistDisplay copy of the frame
(720x480x4) is freed through AvGetSavedDataAddress, which returned 0.
0.0.59: AvGet/SetSavedDataAddress keep the address.
Character select (black): it turns on 2x2 supersampling (device flag
+8 & 0x4000). Swap (sub_001E11A0) then targets the display buffer, binds
the 1440x960 back buffer (dev+0x207C) as texture 0 and draws a filter quad
over the screen; the translator renders the back buffer straight into the
host swap chain, so that quad sampled unwritten guest memory and covered
the frame in black. The translator now skips that quad
(doa3_aa_resolve_source). doa3_guest_display_size reads the display
buffer's surface size (dev+0x2080). The SetRenderTarget hook (3.0
sub_001B1350) is ported as sub_001DC8E0: it records render-to-texture
surfaces so the translator sends them offscreen; until now 3.1 fell back
to the pitch rule, which put the 720-pitch reflection targets on the
screen itself. The device's own buffers are purged from that list so a
reused address cannot misroute the frame.
0.0.59 result: character select visible; the three character panels are
solid black with the fighters behind them (in Tag a 1-px sliver shows at
each panel's left edge). The 0.0.58 trace of that screen: 88 colour-masked
LINES and one colour-masked strip with depth on, the fighters (depth on,
then stencil REPLACE 0xFE), then untextured blended strips with depth off.
0.0.60 adds per-draw lines (@DRAW: host box, z range, colours, state) to
the method trace and traces + screenshots one frame two seconds after each
switch into supersampling (mtrace_aa_N.txt, aa_N.bmp).
0.0.60 trace (mtrace_aa_04522): the window backing quads (FF0D0D0D) are
drawn before the fighters with depth test and write on, as XYZRHW at
z = 1.0 -- but the translator read them with a 2-dword position (stale
default layout: 3.1 has no DrawVerticesUP hook feeding 0xAC layout
markers) and wrote depth 0 over each window. 0.0.61: without a CPU layout
feed, INLINE_ARRAY is unpacked from the SET_VERTEX_DATA_ARRAY_FORMAT slots
(slot 0 = 4 floats here, slot 3 colour, slot 9 texcoord0), as the hardware
does. User report: 0.0.59 (SetRenderTarget hook) also improved Azuchi's
walls.
0.0.61: character select fighters visible. Panel borders were 1 host pixel:
the guest sets NV097_SET_LINE_WIDTH (0x0380, 6.3 fixed point, surface
pixels; 0x10 / 0x20 here = 2 / 4 px of the 1440x960 surface) and D3D11
lines are always 1 px. 0.0.62 draws inline lines as quads of the guest's
width scaled to the host target.
0.0.62: borders thicker, but top/bottom edges overshot the sides (the
square caps; the guest's segments already overlap at the corners) -- caps
removed in 0.0.63. Since 0.0.61 the main menu's black header band blinked:
with the attribute layout its texcoord slot is read, and an untextured
draw leaves stack garbage there (0x00D02590, 0x05137F5C). Texcoords are now
zeroed when stage 0 is disabled and sanitised (NaN / huge -> 0) otherwise.
0.0.63: corners right-angled. The main menu header still blinked, together
with a full-screen dim (30000000) overlay: both are untextured draws whose
disabled stage 0 still holds 0x01ADD400, which since 0.0.59 is a known
render target -- the "sampled offscreen surface" path bound it without
checking that the stage is enabled. 0.0.64 requires it. Movie skip fade:
ownership timeout 250 -> 100 ms, and [MVEND] traces/screenshots frames
+1..+60 after the presenter releases the screen (mtrace_mvend_N.txt,
mvend_N.bmp).
0.0.64: menu header fixed; the Press Start fade-in now shows, the fade-out
after skipping mv_op still does not. Stage atmosphere (X Octagon's green
haze, Aquarium, Forest) missing: the translator ignored NV2A fog entirely.
Azuchi's trace: SET_FOG_ENABLE 1 on 518 of 583 3D draws, MODE 0x2601
(linear), GEN_MODE 2 (planar), colour FF000000, PARAMS 3.5 / -0.00025.
0.0.65 tracks SET_FOG_* (0x29C/0x2A0/0x2A4/0x2A8, params 0x9C0, plane
0x9D0) and computes the factor in the GPU fixed-function vertex shader the
way xemu does (linear: bias + d*scale - 1; exp/exp2 with the 16/32 scale
and -1.5); the pixel stage blends with D3DRS_FOGCOLOR. Auto method traces
now take four consecutive frames, and gpu @DRAW lines carry stencil, fog,
colour/lighting and stage-1 state (Azuchi's floor alternates dark/light).
0.0.65: fog brought Forest, Aquarium and X Octagon close to hardware.
Skipping mv_op: START arms the exit fade 0x53EB0 (counter 0x5A6970 60 -> 0
over 60 frames) while the movie KEEPS playing; FUN_00053490 runs the fade
(FUN_0006BF50) as ordinary 2D draws over the CSC'd movie frame. Our
presenter dropped every guest draw and present while movie frames were
arriving, so the fade never showed. 0.0.66 (non-3.0): guest draws are no
longer dropped during a movie; the movie frame is drawn into the guest
target, the game's own Present shows it with the game's 2D on top, and the
clean movie frame is put back after each guest present
(doa3_movie_composite_present / doa3_movie_after_guest_present). With no
guest presents in the last 100 ms the presenter presents itself as before.
Azuchi floor: dark vs light frames submit the same draws (paused traces
07500 vs 04500). Many stage and character draws enable texture stage 1
(e.g. tex1 0x02CCCB80, 0x017A6480) and the translator ignores stage 1
entirely -- no second texture, no texgen, no register combiners. That is
the likely root of the missing specular/env maps and probably the floor.
0.0.66: the mv_op skip fade works. Ice Cave (trace 13500): the mirrored scene
is rendered into the 256x256 target 0x019FF200, then the floor is drawn by a
vertex program (TRANSFORM_EXECUTION_MODE 6, CPU path) with stage 0 in
SHADER_STAGE_PROGRAM 2D_PROJECTIVE (0x1E70 = 1 / 0x21) and o[T0] = the
projected position. The CPU path took T0.xy raw, without the divide by T0.w,
so the reflection landed shifted and doubled. 0.0.67 tracks 0x1E70 and
divides by q per vertex in that mode (q = 0 when unwritten -> left alone).
Stage 1 texgen REFLECTION_MAP (0x3D0-0x3D8 = 0x8512) with its texture matrix
enabled (0x424 = 1) is the character/water env map -- still unimplemented.
0.0.67: Ice Cave reflection better; occasional vertex explosion there
(present before 0.0.67, not yet captured). 0.0.68: first NV2A register
combiner support. The translator keeps a raw shadow of every NV097 register
(g_nvreg) and decodes the combiners from it (colour/alpha ICW 0x0AC0/0x0260,
OCW 0x1E40/0x0AA0, final 0x0288/0x028C, control 0x1E60, factors
0x0A60/0x0A80, final factors 0x1E20/0x1E24, stage modes from 0x1E70) into
d3d8_combiners' NV2ACombinerState, handed over with
d3d8_combiners_set_direct for GPU fixed-function draws that enable texture
stage 1. Stage 1 is uploaded through the stage-0 cache (nv_stage_texture)
and bound; the NV2A FF vertex shader now generates stage-1 coordinates
(REFLECTION/SPHERE/NORMAL map, eye position, or texcoord set 1) through the
stage-1 texture matrix (0x0700, enable 0x0424) and the 2D-projective
divide. The combiner PS takes the fog factor (FOG.a) from the VS; mux now
selects CD when R0.a >= 0.5 (was inverted); the final combiner reads its own
constants. Azuchi character/stage example: stage 0 R0 = T0, stage 1
R0 = lerp(R0, T1, T1.a), final = fog lerp of (V1 + R0). V1 (FF specular
lighting) is still 0. [COMB] logs each distinct configuration.
DOA3_NO_COMBINERS=1 restores the old path.
Ice Cave streaks (capture_02270/02307): long translucent triangles across
the frame. CPU-path batches were only sent through the near-plane clipper
when a vertex had rhw <= 0 or z < 0; a vertex just in front of the eye
(0 < W < 0.01, rhw > 100) skipped clipping and its divide by W threw it far
off screen. 0.0.69 also clips when rhw > 100 or is NaN. [COMB] from 0.0.68
shows the character configuration really has a third stage, R0 = V0 * R0
(040C0000), so lighting is applied in the combiner.
0.0.69: streaks not fixed. 0.0.68: Forest water better, still no shine on
leather/metal (Bass's thigh ring, cmp_scene_0594). V1 was always 0: the GPU
fixed-function shader computed no specular. 0.0.70 adds NV2A specular
lighting: per light LSpec * att * pow(N.H, power), H from the light's
INFINITE_HALF_VECTOR (directional) or L + V (local), gated by
SET_SPECULAR_ENABLE. The power comes from SET_SPECULAR_PARAMS[0]: the XDK's
SetMaterial (sub_001E6B50 -> sub_001E1D70) fits pow(x, P) with coefficients
interpolated from a table indexed by 3*log2(P) (param 0 from 0x1EA158), so
walking that table inverts it exactly -- DOA3's materials are P = 10, 13,
15, 20, 49. The D3D8 pixel path adds V1 (SPECULARENABLE) only when the
final combiner reads V1 or V1+R0. capture_06132: a large dark translucent
wedge from the top-left (Ice Cave); [PROJ] now counts projective texcoords
with q < 0 (texture projected from behind the projector).
0.0.70: specular/metal on characters matches hardware. No [PROJ] lines.
DOATEC HK: bridge and walkway lights missing (scene_03600 vs 3.0), the
street's wet sheen at full strength (scene_05580). The [COMB] census shows
what the texture-stage approximation got wrong: many draws combine T0
alone (cICW 0x08200000, no vertex colour), take alpha from V0 instead of T0
(aICW 0x00002014), or scale by a combiner factor in a second stage
(0x010C0000, C0 * R0 -- the sheen, which 3.0 handled through TSS markers
3.1 never emits). 0.0.71 sends every GPU fixed-function draw through the
register combiners (DOA3_COMB_STAGE1_ONLY=1 for the 0.0.68 scope,
DOA3_NO_COMBINERS=1 for none).
0.0.71: big visual gain (Azuchi floor now right), but frame drops under 30
(Aquarium even in stage select, DOATEC top tier, Tao, Iron Hell). Cause:
d3d8_combiners' shader cache keyed on the whole NV2ACombinerState including
the C0/C1/final constant colours, which are per-draw values (shadow
opacity, fades) -- a new D3DCompile whenever one changed, with LRU thrash in
a 128-entry cache. 0.0.72 keys the cache on the structure only (constants
zeroed in the key; they go through the constant buffer as before) and logs
"[COMB] pixel shader compile #N".
0.0.72: performance fixed (23 shader compiles over a session). Omega: a
camera blur / after-image effect looks wrong, and on his death (slow motion)
the voice crackles and the audio turns choppy. 0.0.73 is diagnostic:
draws that sample the frame buffer (the blur pass) set
g_doa3_fbsample_seen, and the next frame is traced to mtrace_fb_N.txt with
aa_N.bmp ("[FBFX]" in the log; at most 6, 600 frames apart). The XAudio2
output meter now prints "[XA2] last 2 s: ..." for any window with starved
buffers (host underrun -> gaps) or clipped samples (mix overflow ->
crackle), with the min/max voice pitch rate seen in that window (slow
motion should show rates above 1).
0.0.73 failed to link: g_doa3_fbsample_seen's definition had landed inside
nv_mtrace_draw's body (a local), leaving the extern unresolved. 0.0.74
moves it to file scope (checked with llvm-nm on the objects this time).
Beach: the shoreline wave overlay fades in and out but its image never
animates. The texture cache keys palettised textures on the palette
REGISTER and re-uploads only when a fingerprint of the texel bytes changes,
so a palette rewritten in place (palette-cycled animation) was never seen.
The fingerprint now folds in the palette table's bytes, and textures up to
16 KB are hashed whole (larger ones by 256 samples, was 64) so partial
in-place rewrites are caught too.

Local crossfade fix: the host previous-frame image stopped updating 120
presents after its last use but remained valid. A later Sparring reset
therefore captured an earlier moment in the fight. Keep the image current
at every present; the 120-frame demand window now controls only the
pre-HUD scene snapshot for after-image effects. The D3D11 WARP regression
test in tests/frame_history reproduces the stale image before the fix and
checks both a fresh capture after inactivity and the pre-HUD behavior.

Local announcer balance adjustment: a fight capture aligned GET READY at
frame 1800 with activity in stereo bins 0/1; positional bins 6/7 were quiet
there but peaked at 1.84 during combat and clipped at the output. The stub
DSP now applies -6.02 dB to its positional branch (6/8 left, 7/9 right),
preserving stereo announcer/music gain and VP headroom compensation. This
is an empirical balance adjustment, not verified Xbox DSP behavior;
listening confirmation remains necessary. DOA3_MIX_TRACE=1 logs raw bin
RMS/peaks and post-balance clipping every half second for up to 180 seconds.
tests/audio_mix compiles the actual DSP stub and checks stereo levels,
positional transients/routing, headroom, clipping and frame buffer writes.

Local Survival announcer filter: the request trace at 0x1C7920 captured
mode 5, phase 0, human flags 1/0: opening 0x8001/0x8002, then 0x8003
with health 240/0 and a new 0x8002 with health 240/240. Filter that
intermediate KO and its following Fight at the common streamed-voice
request entry. Keep the opening call, player-loss/double KO, character
voices, and other modes. GET READY and requests outside solo Survival
reset the transition state. This is a targeted host behavior correction;
the upstream cause of the extra requests is not yet established.
DOA3_ANNOUNCER_TRACE=1 logs up to 160 requests and suppression decisions.
tests/announcer replays the capture and checks successive opponents,
restarts, side swapping, player defeat and mode changes.

Local Escape > Debug > Unlock all unlockables: a reversible host override
for 3.1 menu/selection queries, saved as [Debug] UnlockAll in the host INI.
Costume counts at 0x59FB16..28 use the XBE's 0x29C70C limits mapped through
0x2FF868; availability gates are 0x59FB3C/3D and 0x5A0B47, with ending
access at 0x5A0B49..5B and the Theater menu gate at 0x5A0C72. No save
bytes are changed. tools.recomp.debug_unlock_reads routes consumer reads
through debug_settings.c and is part of postprocess, leaving the award
routines at 0x83xxx/0x8Cxxx and every store untouched. Existing selection
lists refresh on re-entry. Tests cover reversal, bounds, INI persistence,
untouched stores/reward code, and idempotent generated-code patching.

Local Zack costume 3 reflection fix: the Beach Practice capture at frame
3000 programs stage-0 S/T/R as REFLECTION_MAP (0x03C0/4/8 = 0x8512),
enables its matrix (0x0420 = 1), and uploads the environment-map transform
u = 0.5*Rx + 0.5, v = -0.5*Ry + 0.5 at 0x06C0. The GPU FF shader used
the mesh's ordinary UVs for stage 0 and only honored texgen/matrices on
stage 1, making the environment image stick to the silver suit. Both
stages now use the same texgen/matrix/projection shader routine, with
independent constants and normal fetching for either reflective stage.
tests/texgen executes the actual shader and input layout on D3D11 WARP,
using stream output to check ordinary UVs, stage-0 reflections following
the normal, independent/simultaneous stage-1 reflections, matrix projection,
and switching back to ordinary materials. The baseline fails the silver
reflection case (u = 0.125 instead of 0.98); the fixed shader passes all
seven checks. In-game appearance still needs visual confirmation.

Local Beach shoreline investigation: the Practice capture at frames
3000..3003 binds two 128x256 DXT5 foam textures (0x020AFC80 and 0x020B7C80
in that session), not palettised textures. Their stage-0 matrix is enabled
and its V translation advances from 0.163402 to 0.168298; the vertex color
and combiner factor handle the separate fade. The earlier palette-cache
explanation above was not demonstrated for these shoreline draws. The
stage-0 matrix support added for Zack also handles their scrolling. The
foam uses negative V coordinates and zero Z/Q matrix rows; tests/texgen
now includes those captured settings at two animation times. All nine
shader checks pass. On 2026-10-10, the user confirmed that the shoreline
waves move in Practice in the current executable, resolving the reported
fade-only behavior. No additional renderer change was needed beyond the
stage-0 texture-matrix support.

Local Beach water reflection fix (2026-10-10): the sharp cut shapes were
introduced by the later vertex-program reflection composite, not by the
scrolling foam draws. The captured pass samples the mirrored scene at
0x02188F80 and a separate mask at 0x020DE680 with texture program 0x21.
Its two combiner stages retain T0.rgb and multiply T0.a by T1.a. The CPU
vertex-program fallback previously discarded o[T1] and used a single-stage
approximation, exposing the reflection mesh boundary. It now preserves
both UV outputs through vertex reuse and clipping, binds the second
texture, and uses the guest register combiners for two-stage 2D programs.
The approximate vertex factor fold is skipped on that direct path so the
combiner applies it only once. Other CPU draw paths keep their existing
single-stage handling. Temporary water redraw diagnostics were removed.
Release build succeeded. tests/reflection_mask exercises the production
vertex shader, combiner generator/register decoder, and clip attributes
on D3D11 WARP: independent mask UVs produce alpha 0/51/102/204 while
reflection RGB remains unchanged. The user confirmed the cuts are fixed
in Beach Practice in the rebuilt executable. Other stages have not yet
been visually rechecked with this change.

Local free camera (2026-10-10): Escape > Debug now exposes a temporary free
camera, speed slider and reset-to-game-view button. WASD/mouse and E/Q move/look;
the first connected XInput pad uses twin sticks and RT/LT, with Shift/LB boost
and Ctrl/RB precision movement. Windows raw relative mouse input is consumed
only while flying in the foreground. Escape, focus loss and closing the window
release cursor confinement. Guest input and rumble are suppressed while enabled;
the game continues simulating. Online sessions disable free camera.

The 3.1 override at 00029E30 activates camera slots at 4DAE90 + index*C80.
It temporarily substitutes eye/target (A8/B8) and zero roll (DC), calls the native
00027720 view/projection builder, then invokes the original activation routine.
This supplies the native render globals and D3D transforms, including the view
used by reflection passes, instead of editing only the final GPU transform.
Registers, guest stack and x87 state around the extra builder call are preserved.
The camera object region A8..20F is restored afterwards so game camera scripts
retain their trajectory; invalid slots and the upload-current-view sentinel
7FFFFFFF pass through. Auxiliary slots remain independent of the selected view.
The override is listed in overrides_3.1.txt and replaces its generated ESP probe.

Release build succeeded. tests/free_camera exercises movement direction,
frame-rate independence, normalized diagonal movement, elapsed-time cap, stick
dead zones, pitch limit, reset and online gating. Its extracted production hook
test checks pose injection and camera/ABI restoration against stub native calls.
tests/input_rumble also checks captured guest input, raw pad access, release,
keyboard fallback and the existing rumble behavior. Visual stage/culling and
controller/mouse feel still need an in-game check; these tests do not emulate
the native camera builder or render a stage.

Local debug vision modes (2026-10-10): a final host override in
d3d8_states_apply covers the five draw entry points, including CPU XYZRHW and
GPU NV2A fixed-function vertices. d3d8_debug_views.c supplies wireframe,
vertex COLOR0, UV0 checker and view-distance PS modes plus optional cull-none.
It operates only on main-target, Z-enabled RGB draws; render-to-texture and
alpha/depth-only mask draws are excluded. Debug PS output is opaque, respects
the guest color write mask, and retains depth/stencil/scissor settings. Raster
and blend objects are cached and released with the normal state subsystem.
Every normal draw preparation rebinds its shaders and states, preventing the
override from leaking into skipped draws or persisting after Normal is selected.
Settings are temporary, host-only, and independent of free camera and saves.

The Distance mode uses D3D11 pixel SV_Position.w directly (clip W for this
perspective projection), not its reciprocal. tests/debug_views renders with
the production module on D3D11 WARP and checks exact vertex-color, UV and
distance pixels; wireframe coverage; offscreen/Z-disabled/alpha-mask exclusions;
backface state; and restoration to normal. Release build and this test pass.
The new modes have not yet been visually checked on a running game stage.

Local performance review (2026-10-10): opt-in DOA3_PERF_CAPTURE writes
performance_frames.csv, limited to 18,000 presented frames. It records frame
interval, mode, movie ownership, foreground state and free camera; normal runs
do not perform timer/file work. The review run used the existing DOA3_PROFILE
CPU sampler and the user's normal 2562x1440 guest target. Filtering to mode 7
(Practice), foreground, no movie, and excluding the first two seconds of that
mode left 2,229 frames: 60.00 FPS average, median 16.6384 ms, p95 18.2228 ms,
p99 24.3183 ms, maximum 46.656 ms. There were 18 frames above 25 ms, one above
33.333 ms, and none above 50 ms. These are instrumented presentation intervals,
not GPU timings or proof that every stage is equally fast. The window was closed
normally before the 90-second sampler dump, so no native CPU hot-spot ranking
was obtained. Only 16 combiner shaders compiled across boot/menus/fight; no
evidence of repeated shader-cache eviction appeared in that run.

The review found that the older call profiler defaulted ON and wrote
doa3_prof.txt every five seconds even with regular logging disabled (about
209 KB in this run). It is now explicitly opt-in with DOA3_PROF=1 and returns
before its per-frame GetTickCount when disabled. Debug view constants are now
uploaded only when mode/range changes, and non-wireframe color views avoid
querying/rebinding an unchanged rasterizer. WARP checks cover changing the
distance range, repeated draws and resource recreation with unchanged settings.

An optional performance_benchmark target in tests/debug_views measures CPU-only
draw preparation on hardware D3D11, without drawing or GPU timestamp queries.
Before cleanup, median time for 10,000 preparations was 0.432 ms unchanged
Normal, 3.346 ms alternating blend/depth/cull states, 0.866 ms vertex colors,
and 0.992 ms wireframe. After cleanup those medians were 0.222/3.299/0.413/0.626
ms respectively. The unchanged Normal control also varied, so these are small
microbenchmark costs, not a claimed in-game FPS improvement. General remaining
candidates are repeated blend/depth/raster state creation on state changes,
and lighting/transform CB preparation before the NV2A FF path replaces those
bindings. Their game-wide impact is unmeasured; no broad renderer rewrite was
made. The isolated long frame has not been attributed to any of these costs.

Local FOV control (2026-10-10): Escape > Debug > Camera controls exposes a
15-120 degree vertical FOV slider independent of free-camera movement. The
existing 00029E30 activation hook temporarily sets camera+D8, rebuilds using
00027720, activates, and restores the source camera. The original builder
uses tan(D8 * [250ED0]), where the 3.1 XBE constant is 0.5: D8 is radians.
This keeps native projection, combined matrices and activation-time culling
values consistent. FOV-only changes preserve the scripted eye, target and roll.
Reset resumes the game's current scripted zoom; settings are session-only and
the override is inactive online. Auxiliary slots retain their FOV while flying.
The native-hook test covers independent FOV, FOV with free camera, reset,
limits/invalid values, source-camera and ABI restoration, and online gating.

Local combat diagnostics (2026-10-10): Escape > Debug > Combat diagnostics has
independent hitbox, hurtbox, and observed strike-frame toggles. The read-only
0007F1A0 wrapper samples after the native collision pass. 0007F220/0007F270
test attack spheres (593040, 16-byte entries, 15 per side; radii at 5A2728)
against body spheres (598C80, 16-byte entries, 16 per side; radii at 5969A0)
and cylinders (596590, two padded XYZ endpoints per 32-byte entry; radii at
59CE78). 0007EDE0 rejects projections beyond either endpoint; these are open
cylinders, not capsules. Type arrays are 5A2638 (attacks, stride 30 per side)
and 5A2818 (body, stride 32 per side). 5A2618 chooses the 30E404/30E41C stance
mapping for type lookup; transformed geometry stays in collision order.
00090FD0, 000910C0 and 00091150 build these world-space volumes from asset
data. No visual skeleton bounds or guessed radii are used. Live reads with
Helena/Ayane confirmed populated types, endpoints and world-unit radii.

59CF58 + side*68 is the player state: +2E motion ID, +33 category, +39 strike
collision enable, +3E contact result. 5A28E4 suppresses further contact after a
hit; its volumes are amber while the move's enable flag remains set. Frame
tracking uses 9B28A0 + side*2, incremented/reset by 000B9980. Categories 2/3
are observed strikes. Duplicate frames are ignored, missing/start-in-progress
samples marked partial, and a transition into another attack/reaction marked
interrupted. Recovery is elapsed time since the final observed active frame,
not a static move-table value, cancel window, or frame advantage. Throw range
and the boss's separate projectile collision path are outside this viewer.

ImGui draws outlines through the scene using the activated 598C30 combined
matrix, homogeneous frustum clipping, and the presentation path's centered
4:3/16:9 rectangle. This also follows independent FOV/free-camera changes.
The capture leaves guest memory/registers untouched, returns immediately when
disabled, expires stale data, and is disabled online. Host overlays are outside
the clean screenshot capture. tests/combat_debug covers timing transitions,
partial/interrupted/multi-window observations, clipping/invalid floats, both
collision shape types, mirrored stance indexing, spent/inactive attacks, and
off/online gating. Release build and the focused tests pass; visual alignment
in a live fight still needs player confirmation.

Hitbox fill update: strike spheres now use translucent triangle surfaces,
clipped in homogeneous space before perspective division. Per-triangle fill
antialiasing is disabled to avoid internal seams. Hurtboxes retain their
wireframes and draw after the hitbox fills to remain legible at intersections.
Contact/spent coloring is preserved. Release build passes.

Performance overlay: a collapsed Performance section in Debug toggles a
top-right host overlay. QPC samples consecutive doa3_ui_render calls at the
presentation boundary, including host-only netplay hold frames. Metrics are
therefore explicitly presentation cadence, not guest simulation or GPU time.
A fixed 240-sample ring supplies a chronological graph, aggregate FPS
(1000/mean interval), mean, nearest-rank P99 and maximum frame interval.
Statistics refresh at 4 Hz; sampling/graph update each presentation. Enabling,
disabling and resetting clear history/timestamps. Disabled mode does no timer,
sorting or file work. No guest state or existing capture/profiler is changed.

Training recording: Escape > Training is gated by offline modes 7/B; the
recorder additionally requires solo Sparring, no Exercise/native pause/free
camera, and a human/CPU pair. Native guard (C58E42), counter-action (C58E1B)
and counter-hit (C58E43) controls are deliberately not duplicated or modified.
11980's post-hook runs once per input-fiber iteration, after native maintenance
and before input history/command decoding. It temporarily routes the CPU side
through an isolated controller port with the recorded button bindings and raw
stick axes. 1EF20 supplies native stick filtering and edges; its virtual-port
history is kept separately. 12B60's post-hook restores routing, controller
classification, port inputs and bindings after command commit, before the
fiber yields. 1894B0 clears the three session-only 600-frame recordings.
Escape, leaving the supported mode, native pause, or free camera stops the
operation. Tests cover routing/restoration, edges, loop/end/capacity, both
player sides, native-setting preservation, and mode/online gates. Release
build and focused tests pass; live dummy playback needs player validation.

Stage hazard overlay: `Combat diagnostics` adds a session-only orange fill
for stage collision boundaries and objects. DC7D0 reads X/Z vertices at
9B80F8 and eight-byte edge records at 9B8104 (count 9B810C); types below 8
and FE are excluded by the native boundary query. The overlay uses those
same edges, including ordinary walls, with a 3m display height because the
records do not specify a vertical ceiling. Bases follow DBE20's packed
height field at 9B8110, X/Z origin 9B80E0/9B80FC, spacing 9B80E8,
dimensions 9B80F4/9B8100 and base height 9B80EC.

E8E60's post-hook observes the native stage-object output (base at +0,
X/Z radii +10/+18, height +14, leaning axis +28/+2C/+30). E8F60's
post-hook removes disabled/broken objects. No extra guest calls are made:
some stage helpers update state. Geometry is keyed by stage/layer and
expires after 200ms without a native query. The viewer shows the shared
wall and object collision paths, not every stage-specific scripted effect.
Disabled/online modes skip capture; counts, pointers and floating-point
values are bounded before use. Focused fixtures cover boundary filtering,
packed terrain heights, tilted objects, invalid geometry and stage changes.

Slippery ground is filled blue using A9AD0's actual hit-reaction predicate:
DC6E0 reads `(cell >> 12) & 7`; material 6 is slippery, and stage 44 also
treats material 0 as slippery. B7871 uses this to select alternate hit
reactions. Beach (47) was checked live: its 316x153 grid contains 1458
material-6 cells and 131 high-bit variants (E), on the water side of the
arena. The overlay follows the packed height samples, with a 2cm display
offset. Coplanar runs merge, nonplanar cells remain separate; the cache
refreshes every 250ms and immediately after stage/grid changes or toggling.
There is a 4096-quad limit with a visible truncation warning. The finite
map is displayed; stage 44's out-of-map default material is not an infinite
highlight. Tests cover dry/slippery separation, the stage exception,
high-bit masking, merging, nonplanar heights, and invalid grids.

Purple distinguishes special boundary classes C/D, 10/11, 14/15, and 18
from the remaining orange boundaries. AB240/AB2E0 treat these separately
with stage/layer exceptions. Purple means a special reaction boundary,
not a guarantee that any particular attack causes a fall. Stage-specific
scripted transitions outside the shared edge/object data are not yet
visualized. Frame-data bottom-right inset is now 204px (48px higher).
Live Danger Zone (3D), layer 0, has four straight type-C edges at X/Z
+/-10 (endpoints +/-6), four diagonal type-A corner edges, and four FE
helper edges. The shared overlay therefore marks the straight walls
purple, corners orange, and omits the FE helpers. Its floor materials
are 0/8, so the slippery-ground overlay correctly remains empty.

The Stage hazards submenu now selects slippery ground, special boundaries,
ordinary walls, and collidable objects independently. Disabled categories
skip their capture work; object and floor caches reset only when their own
selection changes. Tests cover each category alone, combined selections,
and preserving object observations when toggling another category.

Hazards now render with D3D11 scene-depth testing before the final window
scaling pass, rather than as ImGui screen fills. `combat_world.cpp` transforms
the same world geometry with the activated camera matrix, keeps homogeneous
coordinates for hardware clipping, and uses LESS_EQUAL with depth writes and
stencil disabled. Characters and foreground scenery therefore occlude the
highlights. Hit/hurt volumes retain their existing screen-overlay behavior.
A private color copy avoids modifying the guest frame, reflection/crossfade
history, or accumulating tint on repeated presentations. All changed pipeline
bindings are restored; resources resize with the scene and release on UI
shutdown. The WARP graphics test verifies hidden/visible geometry, unchanged
color/depth/stencil, state restoration, near clipping, resize, category gating,
and repeated presentation. Live stage appearance still needs user validation.

### Observed hit/block recovery

Read-only wrappers at `B7180` and `B9980` now measure strike recovery gaps.
The former saves attacker contact bits/move IDs before resolving collisions,
then accepts new defender reactions in category 7 (block) or 8/9 (hit). The
latter samples after both fighter category handlers and motion counters have
updated. Measurements count native logic steps, not presents or milliseconds.
Each side's last result remains visible beside its strike timing; positive
means the attacker returned to neutral first.

This measures natural return to category 0. The UI labels hit results
**Hit recovery gap**, since defensive holds and stun escapes may be possible
earlier. It does not claim these are guaranteed punishment windows. Attack
strings, same-ID restarts, changing reactions/falls, early guard-cancel states
(`player+48`), trades, KOs, and measurements exceeding 300 ticks are rejected.
Tag/netplay, match phase changes, character/stage/mode changes, long capture
gaps, and toggling frame data reset the measurements. Existing S/A/R timing
and volume capture remain at `7F1A0`.

The read-only Helena/Ayane Practice capture showed 008A reaction starting at
attack frame 13, defender neutral at attack frame 24, and attacker neutral
after frame 26: -3 frames on hit and block. The rebuilt hooks confirmed -3
for repeated single jabs on hit and block. Completed logic-step traces for
008D measured 26 ticks to attacker neutral, 33 on hit and 18 on block (+7/-8);
0122 measured 20/41 on hit (+21) and 20/15 on block (-5). External polling
occasionally mixed phases of the same tick, which is why the completed
logic-step observation is used in the overlay and kick test fixtures.
These samples are deterministic test fixtures, alongside interruption,
trade, mirrored-player, reset, and render-independence coverage. Set
`DOA3_FRAME_DATA_TRACE=1` to log resolved contacts and completed measurements
for live verification without modifying game state.

### Input latency audit

Gameplay input already polls XInput/GetAsyncKeyState at each guest read;
there is no host-side frame cache to remove. The guest frame completion path
presents before its 60Hz pacing wait. Preserve that ordering and the native
simulation/input semantics. A local 40-call XInput check on the connected
wireless controller measured approximately 0.0015ms median API time (0.41ms
maximum in that sample); this measures API overhead, not radio or button-to-screen
latency.

Device enumeration now reuses connection status from fresh gameplay/raw
reads. Idle connected slots are checked every 250ms and empty slots every
second. Actual button/stick reads remain unthrottled. Initial discovery also
works before one second of system uptime. The deterministic input test covers
fresh packet reads, no repeated discovery polling, idle discovery, immediate
disconnect through guest/raw reads, reconnect, clock wrap, port routing,
keyboard fallback, input capture, and rumble.

The existing bitblt D3D11 swap chain now requests a device-level maximum
frame latency of 1 through IDXGIDevice1, then reads the value back and logs
it. DXGI's default maximum is 3. Failure logs a diagnostic and leaves the
device usable. Present(0,0), the swap-chain model, game speed, and netplay
input delay remain unchanged. This bounds avoidable render queueing; it is
not a claim of a fixed millisecond saving or a complete display-latency
measurement. API reference:
https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgidevice1-setmaximumframelatency

The live build read back a queue limit of 1; Practice remained at approximately
16.67ms average frame time. The user reported normal controls, rumble, and
smoothness. This is a pacing/regression check, not an external input-to-display
measurement or a controlled same-scene comparison with the older build.

### Flip presentation and input timing follow-up

The host now prefers `DXGI_SWAP_EFFECT_FLIP_DISCARD` with two buffers and
`DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING` when windowed and the DXGI factory
reports tearing support. This avoids the legacy compositor copy and enables
lower-latency presentation on supported desktops. Present remains interval 0
and the device queue limit remains 1. This is not a waitable swap chain, so
the device-level latency setting still applies. Exclusive fullscreen omits
the present tearing flag; automatic DXGI Alt+Enter is disabled in favor of
the existing windowed/borderless settings. ResizeBuffers preserves swap flags.

Unsupported systems or failed flip creation retry the compatibility blit
path. `DOA3_LEGACY_PRESENT=1` explicitly selects that path for comparisons.
Both paths are implemented in `d3d8_presentation.c`. The hidden-window WARP
test exercised both actual presentation models, queue readback, repeated
presentation, and resizing. The live hardware build selected flip discard;
the user confirmed Practice and windowed/borderless switching appeared normal.
Reference: https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/for-best-performance--use-dxgi-flip-model

`DOA3_PERF_CAPTURE=1` additionally logs port-0 input-read age at present
return, read-to-logic-completion time, time spent in the host Present path,
age of the latest input available at logic completion, and reads per frame.
Negative timings mean no applicable sample. These are CPU timestamps, not
controller-radio, GPU-completion, scanout, or button-to-photon measurements.
The last input timestamp survives a present so a delayed logic sample cannot
hide a frame-boundary wait. Logging remains opt-in and bounded to 18000 frames.

In 1247 foreground Practice samples after warm-up, mean frame time was
16.667ms, median read-to-logic completion 0.071ms, and median input age at
present return 9.847ms. There was exactly one port-0 read per frame, and the
logic-associated input age matched the latest read's age: no additional
whole-frame input buffer appeared in that capture. Host Present's median
duration was 0.059ms. These results locate most remaining CPU-side delay
after game logic; they do not establish a measured end-to-end latency saving
over the older presentation path.

### CPU render-path follow-up

A live Beach Practice thread sample found substantial time in texture
signatures, combiner shader lookup, and the old function-return tracing.
The trace was still enabled by default for 3.1 startup diagnostics, polling
15 guest words and maintaining a return ring throughout gameplay. It is now
opt-in through `DOA3_TRACE_FN` for all versions; the generated wrappers skip
the trace after its first environment check when unset. To restore the old
intro trace, use `DOA3_TRACE_FN=19e3c0,19e230,a4680,a4ab0,1a81e0,1a8220`.

Combiner lookup canonicalizes its key once and stores that canonical key.
It hashes four bytes per iteration instead of one and still compares the
entire key on a hash match. Animated constant colors remain excluded from
the shader key and are uploaded as before. Texture signatures, palette
animation checks, shader generation, and the 60 Hz game clock are unchanged.
The cache test covers constant changes, every structural byte, forced hash
collisions, eviction, and failed compilation without losing a live entry.

The rebuilt release passed the cache test and the user's Beach Practice
visual/audio check. Before the second sampling profiler ran, 1501 foreground
Practice frames after warm-up had median input-to-Present-return 8.480ms,
versus 9.227ms in 513 foreground Beach samples immediately before these
edits (about 0.75ms / 8% lower). The earlier 9.847ms capture used for the
initial discussion is a separate run. Mean frame time remained 16.680ms.
The p95 input age increased from 10.817ms to 12.543ms in these short runs;
there is no demonstrated improvement to worst-case latency. They are not
deterministic replay or button-to-photon measurements. A subsequent thread
sample no longer showed the default function trace, and combiner lookup
dropped from roughly 5% to 1% of wall-time samples. Texture validation is
the next significant renderer cost, but its animation checks are preserved.

### 0.0.75: four local controller configurations

Escape > Controls now selects Player 1–4, with independent bindings, an
input-device dropdown (Automatic, None, Keyboard, Controller 1–4), and an
Xbox controller-type dropdown (Duke, Controller S, Fight Stick). Save writes
all four players into `[Player1]` through `[Player4]` in `doa3_input.ini`.
Reset affects only the selected player's bindings. Old unsectioned mapping
files populate all four mappings to preserve their former shared behavior.

Explicit devices reserve their Windows XInput slot before Automatic ports
are assigned. Selecting a device already explicitly assigned elsewhere moves
it to the selected player; no physical pad drives two local ports. Explicit
slots remain reserved across unplug/reconnect. Windows can renumber XInput
devices after reconnect/restart, so these are slot assignments, not persistent
hardware identities. The controller capture listens only to the selected
player's assigned device. Keyboard may be selected on any port; Player 1's
Automatic setting retains the legacy bound-key overlay/fallback.

Type changes report original Xbox subtypes 0x01/0x02/0x20 through the guest
capabilities wrapper, and changed local ports reopen to refresh capabilities.
Bindings remain intact when type changes. These values describe the emulated
Xbox device, not Windows XInput's different subtype numbering. Reference:
https://xboxdevwiki.net/Xbox_Input_Devices

Tests exercise four simultaneous simulated pads, per-player input and packet
routing, independent bindings, keyboard isolation and menu suppression,
explicit/automatic assignment, duplicate prevention, disconnection/reconnection,
rumble shutdown/routing, all three subtypes, persistence, invalid settings,
and legacy mapping migration. The existing fresh-polling and rumble tests pass.

### 0.0.75: Tao statue polygon offset

A Tao Practice capture showed a blended, depth-writing statue-area pass
enabling polygon offset with slope -0.5 and bias -2. The translator stored
those registers but never applied them; the D3D11 rasterizer always used
zero bias. Successive captures showed speckled changes on the statues even
with the fighters standing still. This is evidence of a missing separation
between nearly coplanar surfaces; live confirmation of the fix is pending.

Both CPU and GPU draws now translate the guest solid-polygon offset to the
D3D11 rasterizer. Disabling it restores zero bias, and independent points
and lines do not inherit polygon bias. The rasterizer compares its complete
descriptor so offset changes cannot reuse a stale state. Constant bias is
converted from the guest D16/D24 range to the host D24 buffer; slope is
scaled by render height to compensate for increased resolution. A single
slope factor approximates anisotropic widescreen stretching. Guest point/
line polygon fill modes remain outside the currently supported solid path.

The standalone `tests/polygon_offset` WARP test extracts the production
translation and rasterizer code. It renders nearly coplanar surfaces at
native and 3x resolution, checking positive/negative constant and slope
bias, Tao's values, and reset on the following draw. It also checks D16
conversion, non-finite input handling, and primitive/enable gating.
Reference: https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias

### Direct AFS archive loading

The PC build already stores the source archives in `HDD/D`. The original
Xbox startup nevertheless validates and sometimes copies them into `HDD/Z`.
`tools/recomp/direct_afs.py` now skips the 3.1 cache validator A4300 and
clears the copy-request flag at 73C0A4. The existing 874BE boot check then
skips spawning the 1C67C0 copying thread. This also avoids the validator's
cache-file deletion and timestamp/size checks.

The CRI initialization branch at 1C7A86 always takes its native direct-source
path (1C7B3B): it excludes the Z: archive names and mounts D:\loadfile.afs,
D:\bgm.afs and D:\voice.afs. Merely clearing the flag would instead select
the cached-file mounting branch, so both changes are required. Native
streaming, buffers, missing-source errors, save paths and host I/O remain
unchanged. Existing Z: copies are left on disk but are no longer loaded.
The patch is version-gated and reapplied by the postprocess pipeline.

`tests/direct_afs` executes the native initialization and installer gate
with a simulated guest memory buffer and CRI call stubs. It verifies three
D: mounts, exclusion of the cached paths, no installer start, guest stack
preservation, fresh-generation patching and idempotence.

The release startup check reached the intro and subsequent asset/music
loads using the D: archive handles, without starting the copy worker.
Existing Z: archive sizes and modification times stayed unchanged. CRI
still enumerates its registered drive backends and opens Z: files to query
metadata during initialization; these are not archive-content loads or
cache copies.

### 0.0.76: automatic diagnostic captures are opt-in

Normal launches no longer generate boot-glitch, periodic scene, draw-count,
movie, character-select or framebuffer-effect screenshots and method traces.
Set `DOA3_AUTO_CAPTURE=1` before launching to enable those developer diagnostics.
The disabled frame monitor returns before any GPU readback or allocation.
Manual PNG screenshots (the assigned hotkey and Debug menu button) remain
available. Explicit `DOA3_CAPTURE_FRAMES`, `DOA3_MTRACE` and `DOA3_PMSHOTS`
diagnostic requests still work independently. Existing captures are retained.

### 0.0.76: native USB DualShock 4 support

`src/input/ds4_device.c` enumerates Sony DS4 devices through DirectInput 8,
alongside the existing XInput path. Host slots 0-3 keep their previous XInput
meaning; slots 4-7 are PS4 devices. All eight are eligible for the four guest
players, including explicit assignments, automatic routing, saved bindings,
reconnect handling, free camera and input capture. The controls menu labels
the two device families separately. Overlay navigation now reads player 1's
assigned device instead of ImGui's hardcoded XInput slot 0.

Cross/Circle/Square/Triangle map to A/B/X/Y, Share/Options to Back/Start,
and L1/R1 to White/Black. Both sticks, stick clicks, diagonal d-pad states
and independent analog triggers are supported. This mapping is limited to
Sony product IDs 054C:05C4, 054C:09CC and 054C:0BA0; it does not guess mappings
for unrelated DirectInput devices. Device identities remain stable during
the process lifetime, including reconnection of the same instance.

`ds4_rumble.c` opens the device's HID path for USB motor output. It checks
for the 32-byte USB report format and sends report 05 with only the motor
validity bit set, preserving lightbar and audio state. Low/high-frequency
Xbox motors map to the DS4's large/small motors. A worker coalesces updates
and performs overlapped USB writes so gameplay never waits on a motor write.
Focus loss, controller reassignment, UI capture and normal device teardown
stop vibration. Bluetooth rumble is not implemented; input remains usable
when the HID output handle is unavailable. No additional driver or downloaded
runtime is required for supported USB hardware.

The input tests cover DS4 button/axis/hat conversion, USB motor report bytes,
mixed Xbox/PS4 routing, persistence, disconnect/reconnect and vibration stops.
Live hardware validation remains pending. The optional `ds4_controller_check`
target reports detection and button/axis values without generating inputs
or requesting vibration.
Protocol references: [SDL's DS4 implementation](https://github.com/libsdl-org/SDL/blob/main/src/joystick/hidapi/SDL_hidapi_ps4.c)
and [Windows controller mappings](https://github.com/libsdl-org/SDL/blob/main/src/joystick/SDL_gamepad_db.h).

### 0.0.76: Japanese 3.1 console-save signatures

The kernel now initializes the title's signature key from the loaded XBE
certificate instead of leaving the export zeroed. New saves use the retail
title-derived HMAC-SHA1 key. The native 3.1 loader retains its format checks
and accepts an alternate signature only after verifying all 20 digest bytes.
Supported alternatives are the zero-root title-derived scheme found in the
supplied console save and the plain-zero key used by older PC builds.
Imported title-derived saves retain their signing scheme during the session;
legacy PC saves migrate to retail signing when next saved. No input file is
rewritten by validation, and corrupt or truncated saves remain rejected.

`tests/save_compat` checks certificate bounds, title-key derivation against
independent Python HMAC vectors, all three signatures, corruption rejection,
and regeneration patch boundaries/idempotence. The optional test executable
arguments are an XBE path and a save path; it validates and re-signs the save
in memory without writing to either file. Both the supplied Japanese 3.1
console save and the existing PC save passed this check. Release compilation
passed; loading the supplied save through the game UI remains unconfirmed.

### Controller discovery and periodic frame hitches

DirectInput device enumeration formerly ran on the game thread once per
second. A standalone measurement on the development machine took 48-64 ms
for each recurring scan, even without a detected DS4; the four XInput slot
checks took about 0.2 ms. Discovery now runs at startup and in response to
Windows `WM_DEVICECHANGE` / `DBT_DEVNODES_CHANGED` notifications. Notifications
only queue a scan, with a 250 ms minimum interval to coalesce bursts. Ordinary
input reads stay fresh; a completed scan immediately refreshes host-slot
connection state. Controller attachment can still incur an enumeration cost,
but elapsed time alone no longer triggers it.

The same benchmark after the change measured 0.000-0.001 ms for idle discovery
checks after the initial scan. This is discovery timing, not total game frame
time. Tests cover idle discovery, notification bursts, timer wrap, failed-scan
recovery and immediate routing after reconnection. Live gameplay confirmation
of the performance graph remains pending.

## Castle ember colors and fading (2026-10-12)

The 3.1 point-sprite path retained the 3.0 port's texture-stage marker
fallback. Those markers are not emitted by 3.1, so it selected the sprite's
RGB and alpha alone. The burning-castle cutscene in Ryu's Story instead emits
shader program `00008000`, one combiner, RGB input `04200000`, alpha input
`1B140000`, both outputs `00000C00`, and final words
`0000000C / 00001C80`. This is RGB = vertex color and alpha = T3 alpha times
vertex alpha, with SRCALPHA/ONE framebuffer blending. Captured vertex colors
include `33FF6D02` (orange with 20% opacity), not opaque white. The temporary
particle logger also observed this shader on courtyard torch embers.

`nv_apply_point_state` now selects the real combiner path for T3-only point
sprite programs with valid combiner counts. `nv_draw_point_sprites` submits
the expanded quads with four texture coordinates and the generated UV on T3,
binds the sprite there, and uses the existing register-combiner translator.
It restores direct-shader mode and unbinds T3 after each draw. This is shared
by inline and array points; it neither changes global brightness nor names a
specific stage/texture address. Multi-texture point programs and legacy TSS
markers keep the prior path. The additional vertex fields are local to
sprites; general CPU vertex size is unchanged. The temporary logger was
removed, and normal play does not gain any automatic captures.

`tests/point_sprites` extracts the production point routing, vertex packing
and NV2A decoder, then renders with the production combiner shaders on WARP.
It checks the captured orange/fade result against a deliberately green sprite
with four different mask alpha values, complete fade-out, full opacity,
texture-colored particles, missing textures, cleanup, and legacy guards.
Release build, point-sprite WARP and combiner-cache tests pass. In-game comparison of the corrected cutscene is still pending.

## Omega entrance framebuffer feedback (2026-10-12)

An Ayane Story diagnostic recorded the entrance's inline, depth-disabled
framebuffer reads swapping source/destination `01058000` and `011C0000`.
The entrance uses SRCALPHA/INVSRCALPHA with T0*V0 color and alpha; later the
fight uses CONSTANT_ALPHA/ONE_MINUS_CONSTANT_ALPHA, commonly `95FFFFFF`.
The reference clip `2026-10-12_00-29-18.mp4` shows accumulated trails during
the snowy entrance and transition, and during combat, including guest HUD/KO
trails. This confirms that the earlier pre-overlay-only history policy was
suppressing part of the intended effect.

The first-2D-draw call to `d3d8_SnapshotSceneForAfterImage` overwrote the image
that the same frame's blur then sampled. A WARP test reproduced this by
observing the current red scene instead of the previous green frame. Keeping
a separate pending scene fixed that ordering error but still discarded the
blur feedback, so the final change removes the pre-overlay capture entirely.
`d3d8_keep_prev_frame` retains the completed guest frame at each native present,
including guest effects/text, before host composition. It continues updating
while effects are idle, preserving the Sparring crossfade fix. Guest blend factors, zoom offsets
and timing remain unchanged; there is no artificial strength multiplier.

The frame-history WARP regression exercises actual GPU blends across several
frames: an older red after-image remains in a later blue/black scene with the
expected decay, and history does not change mid-frame. It also retains the
freshness check after more than 120 idle presents. The temporary opt-in blur
logger was removed; no automatic screenshots or logging were added to normal
play. Visual confirmation of the updated executable is still pending.

## Timer shadow appears before the HUD (2026-10-12)

A captured Practice round start (`mtrace_17233`) submits the two timer-shadow
quads with `00000000` vertex colors, followed by the frame artwork with
`00FFFFFF`. Both request TEX0 * V0 for RGB and alpha: combiner inputs
`08040000` / `18140000`, outputs `00000C00`, final `0000000C` / `00001C80`,
one stage, SRCALPHA / INVSRCALPHA. Subsequent shadow alpha values rise through
`11`, `50`, and `80`. The renderer's all-zero-diffuse heuristic selected the
texture directly only at the zero endpoint, displaying the opaque black
backing before the intended fade. The capture visibly reproduces this during
GET READY; no framebuffer-history sampling is involved in these HUD draws.

`nv_use_vertex_diffuse` now honors transparent black for this explicit inline
modulation program in both ordinary texture and render-target texture paths.
Other programs retain their existing fallback. The `hud_fade` regression
uses the captured registers and fade values and checks fallback isolation.
The regression and Release build pass. The user confirmed the corrected
round-start appearance in game.

## Ice Cave near-plane clipping precision (2026-10-12)

A native-60-Hz movement capture shows a dark wedge in frame 61372, between
clean frames 61371/61373. The main-target reflection draw samples `020FF200`;
its minimum screen Y unexpectedly becomes zero. A subsequent opt-in coordinate
trace reproduced this on multiple triangles in Ice Cave: clip-space positions
around 1e13 and negative depths around 1e9 adjoin visible vertices around 1e5.
The old float interpolation parameter rounded to 1, and `A + (B-A)*t`
cancelled X/Y/Z to zero while retaining positive W. Reprojection stretched
the reflection triangle to the screen origin. This is independent of render
interpolation and is not a single faulty mesh.

`nv_clip_plane` now computes signed distances and independent endpoint weights
in double precision, blends all position/color/UV fields with those weights,
and sets the selected coordinate exactly to the clipping boundary. Computing
both weights directly avoids losing the smaller weight through `1-t`.
No stage-specific filtering, geometry removal or blend adjustment is used.

`tests/clipping` retains 12 exact captured input triangles, with expected
intersections calculated offline using 70-digit decimal arithmetic. It checks
all cyclic vertex orders and both windings, projected output, both UV sets,
color interpolation, unchanged inside triangles, rejected outside triangles,
and the positive-W boundary. The old code failed 180 checks; the correction
passes, as does the existing reflection-mask WARP test. The temporary logger
has been removed. The user confirmed that the rebuilt Ice Cave scene looks
correct after the fix.

## Aspect ratio projection and display shape (2026-10-12)

The user identified the high bytes of the projection float at XBE offset
`0x244C22`. The retail Japanese 3.1 file contains `00 00 10 3F` beginning at
`0x244C20`: the full little-endian float 0.5625 (9/16). The .rdata section
maps it to guest VA `0x251620`. Seven recompiled reads use `MEMF` at that
address, including `sub_00027720`; changing mapped data therefore affects
the native recompiled camera routines without patching generated code.

The camera chooses that constant when `0xA48CDC` bit 0 is set; otherwise it
reads 0.75 at `0x250EF0`. D3D initialization `sub_0017B8D0` caches the video
flags in `0xA48CDC`, raw video flags `0xA48C78`, and present flags
`0xA48C58` (widescreen bit 0x10), with guest present width at `0xA48C30`.
The new 3.1 runtime synchronization updates those bits after initialization
and writes the full projection float before startup and at frame boundaries.
It preserves unrelated flags and never writes to the XBE file on disk.

The five selections separate display ratio from projection height/width:
Full Screen 4:3 uses 3/4; Anamorphic 4:3 and 16:9 use 9/16; 21:9 uses 9/21;
32:9 uses 9/32. The base targets are 640x480 for either 4:3 mode, 854x480 for
16:9 (unchanged), 1120x480 for 21:9, and 1707x480 for 32:9, multiplied by the
internal-resolution scale. The final presentation uses the exact requested
aspect despite integer target-size rounding, as do combat debug overlays.
Guest HUD/movie layout remains game-authored and is presented in that area.

Switching between the two 4:3 modes must synchronize even when target
dimensions do not change. A new Video/AspectRatio key stores the five modes;
legacy Widescreen=0 migrates to Anamorphic 4:3 and Widescreen=1 to 16:9.
The video_settings regression exercises startup before D3D initialization,
exact float bits, unrelated-memory preservation, both 4:3 modes, all scales,
save/load round-trips, old settings, and invalid selections. Visual checking
of the new ultrawide modes is still pending.

## Player visibility and controller tabs (2026-10-13)

Controls now uses four player tabs. Player 1 retains its Automatic default;
unconfigured Players 2–4 default to None. Explicit saved assignments and
optional Automatic routing still work.
The routing tests cover disconnected defaults, saved selections, and the
existing four-port XInput/DS4 input and rumble paths.

The screenshot section exposes separate render-only fighter visibility flags.
The 3.1 player record byte at `0x59CF58 + side*0x68 + 3` selects one of four
resource slots. `8AA50` allocates `0x498000` bytes per slot and stores its base
at `0x5A0CF0 + slot*0x1C`. The indexed-draw path filters the position stream
against the selected player's arena before either CPU or GPU submission.
A paused Beach trace with two Helenas places both body and shadow streams in
these arenas, distinct from stage geometry. Guest updates and collision data
are untouched; phase 1 and offline play gate the filter. Flags reset on launch.
Tests cover separate sides, stage exclusion, invalid slots, online gating,
and reset behavior. The user confirmed native Start-pause collision overlays
already work in 0.0.78, so that implementation remains unchanged.

The global `Input/CaptureMenu` setting defaults to enabled. With the Escape
menu open, `pad_mapping_apply_player` returns a neutral guest gamepad for each
port, while raw device reads remain available to ImGui navigation and binding
capture. No controller disconnect or gameplay pause is synthesized. Disabling
the option restores controller passthrough; keyboard bindings retain their
existing menu suppression. Tests exercise all four ports, raw input, immediate
toggle changes, menu close, persistence, and malformed/legacy settings.

## Ultrawide character-select previews (2026-10-13)

The centered 16:9 HUD left character-select portrait models and their dark
backings in full-width coordinates. A 21:9 Sparring capture showed the left
preview scissor at 94,154–701,625 on the 1440x960 supersampled menu surface,
with viewport origin 94.53125,154.53125. A 16:9 reference confirmed that the
model projection already responds to the aspect constant.

Recognize bounded preview viewports on the main 2x2 menu surface and translate
their centres into the HUD canvas, preserving model width. Apply the same
translation to CPU output and GPU fixed-function clip constants; map their
scissors into the centered canvas. Flat far-depth panel backings follow the
HUD scale. Ordinary world draws, offscreen targets, fullscreen fades, and
native main-menu pixel offsets retain their existing behavior.

The aspect/HUD layout tests cover both portrait sides, preserved projected
width, backing alignment, legacy aspect ratios, and offscreen/world isolation.
All four video/HUD/timer regression tests and the Release build pass. The user
confirmed character select looks correct in both 21:9 and 32:9.

### 0.0.123: automatic diagnostics switch in the settings file

The 0.0.76 opt-in for automatic diagnostic captures needed the
`DOA3_AUTO_CAPTURE` environment variable, i.e. a console launch. It is now
also `[Debug] AutoCapture=0/1` in `doa3_settings.ini` and a checkbox under
Escape > Debug > Diagnostics (saved with the Debug section's Save, cleared
by its Reset). The environment variable still wins when present (`1` on,
`0` off), so one-off developer launches need no ini edit; the checkbox is
greyed out in that case. `doa3_auto_capture_enabled()` moved from a static
header function to debug_settings.c and reads the live setting, so toggling
it in the menu starts or stops the periodic scene/method-trace and
framebuffer-effect captures immediately. The boot glitch finder only runs
during the first frames after launch, so it needs the setting saved (or the
variable set) before starting. Default remains off. tests/debug_settings
covers the default, persistence round-trip and the ini key.

### 0.0.124: game volume sliders, streamer mode, free-camera speed

The game keeps three audio settings in its save block (0x59D088, written by
SaveGame 0x20C80): Music 0x59D094, Sound effects 0x59D095, Voice 0x59D096,
each a u8 percent 0-100 with default 80 (clamp table 0x3F4528, applied by
the loader). The Options menu edits them in steps of 5 (0xC3A40 slider,
step 5) and applies music through 0xA61C0 (eax = level: clamp, store the
live level 0x2D3E9C, call 0x1C6EF0, which sets every live music channel's
ADX output volume). The boot path applies SFX and voice through 0x8A3F0
(0x2D3EA0 = SFX, 0x2D3EA4 = voice * 0.85, then 0x1C6AE0).

Escape > Audio gains Music / Sound effects / Voice sliders bound to those
bytes. The UI queues a change; the game thread applies it at the next
frame (d3d4134_frame_done) through those same routines, with caller-saved
guest registers preserved. Nothing is stored in doa3_settings.ini: the
game saves the values itself when leaving Options or after the credits.
The Master slider is unchanged (a final output attenuator, 100 = the
game's own level; it never boosts) and its text now says so.

Streamer mode ([Audio] StreamerMode, Audio tab) silences the licensed
songs through the game's own music volume. Every song starts through
ADXT_StartAfs (0x191DD0) with partition 2 (bgm.afs); the hook records each
ADX handle's file and logs "[BGM] ... bgm.afs file N started". The song
list at 0x29CE48 pairs "nine lives" with file 0x18 and "home tonight" with
0x17 (to be confirmed from those log lines). While a licensed file is on a
live music channel (0x5CF418 + n*0x45A00, kind 3) and still sounding (ADXT
status at handle+1 in 1..4), the music byte and live level are set to 0;
when it ends, another song starts, or the option is turned off, the
player's level is restored. A change made in the game's Options while
muted becomes the level to restore. The SaveGame hook puts the real level
back for the save and re-zeroes it afterwards, so a 0 can never be stored.

Free camera speed: default 10, range 1-20 (was 300, 10-3000).
Also: the [VTHIT] diagnostic printed one register more than it passed
(ebp has no global here; dropped), and the Diagnostics checkbox reads the
environment through GetEnvironmentVariableA (no C4996 warning).

### 0.0.125: Diagnostics folder; boot glitch finder removed

Developer diagnostic files now go to a `Diagnostics` folder next to the
game (created on first use) through `doa3_diag_fopen()` in
debug_settings.c: method traces (mtrace_*.txt), automatic capture
screenshots (scene_*, aa_*, mvend_*, frame_*, movie_*, frame_at*,
frame_post*, pm*), doa3_prof.txt and performance_frames.csv. The log
(doa3_log.txt), xbox_kernel.log, netplay digests/replays, manual PNG
screenshots and DOA3_WAVCAP (an explicit path) are unchanged.

The boot-screen glitch finder is gone: it read every frame back for the
first 1500 frames to catch the TECMO-logo flash fixed in the 0.0.20s,
writing doa3_frames.csv and glitch_NNNN.bmp (and DOA3_GLITCH_UNTIL). The
same hook now only takes the periodic half-size scene snapshot (every 180
frames, at most 80, from the start of the run), reading the frame back
only on those frames.

### 0.0.126: periodic scene snapshots removed

The automatic scene_NNNNN.bmp snapshots (every 180 frames, capped at 80, so
the last was always scene_15840) are removed with doa3_frame_monitor.
Targeted captures stay: aa_*.bmp with its mtrace_aa / mtrace_fb trace,
mvend_*, and the method traces, all under Diagnostics.

### 0.0.127: streamer mode covers the credits movie and "Amazing"; menu input

The credits do not start "Home Tonight" through ADXT_StartAfs: it is the
audio of the movie d:\mv_st.sfd (entry 17 of the movie table at 0x2D4DD8).
The movie start routine 0xA47E0 (eax = movie number) creates the Sofdec
player at 0x73C308 and sets its output volume once, from the live music
level 0x2D3E9C (0 -> -999). Hooking 0xA47E0 lets streamer mode mute before
that read; the credits count as licensed until the player is torn down
(0xA4C10 zeroes 0x73C308) or, if it never appears, after 600 frames.
"[MOVIE] frame N: movie M starting" is logged for every movie.

"Amazing" (song list entry 22, file 0x16, sound test 23) joins Nine Lives
(0x18) and Home Tonight (0x17) as licensed.

"Capture gameplay input in Escape menu" off previously only returned the
controllers to the game; the menu still read the same controller, so both
moved at once. Off now also withholds the controller from ImGui
navigation (mouse and keyboard still work; rebinding reads raw input).

## Additional character-select portrait layouts (2026-10-13)

A live 21:9 Tag Battle capture reproduced the uncentered portrait/backing in
four-slot character select. Its native surface is 1440x480 (2x1 supersampling),
with first portrait scissor 92,77-389,312 and origin 92.53125,77.53125. The
previous 1440x960-only gate excluded this layout entirely. Its dark backing
is still in 720x480 display coordinates: 46,77-196,313.

Accept both 2x1 and 2x2 main menu surfaces. Validate backing bounds against
actual display dimensions instead of halving the surface height. The existing
viewport translation and centered scissor then apply to these additional
character-select layouts without narrowing the already aspect-correct model.
Regressions cover all four slots at both ultrawide ratios, all three internal
scales, legacy ratios, and offscreen/full-screen isolation.

The Release build and both video/layout tests pass. A second live Tag Battle
capture at 21:9 verified that the portrait and all four dark panels align
with their centered borders. Survival/Time Attack visual confirmation is pending.

## Story-mode hitch investigation and persistent shaders (2026-10-13)

Hayabusa Story profiling confirmed individual presentation gaps despite an
average near 60 FPS. A native overlay capture contained a 118.74 ms frame.
A separate foreground Story timing run recorded 61 frames over 25 ms and
9 over 50 ms in 16,746 samples (74.68 ms maximum). These are presentation
intervals, including CPU work and waits, not GPU timings. Slow-frame thread
samples caught D3DCompile during multiple stalls, as well as guest audio
voice-retirement waits and texture/render work. Sampling overhead was below
0.4 ms per sample; each stalled frame received at most five samples.

Cache generated combiner pixel shader and programmable vertex shader DXBC
under `%LOCALAPPDATA%\DOA3\shader-cache-v1`. SHA-256 keys include the exact
generated source, entry point, target profile, and compiler flags; changing
generated code invalidates old entries automatically. Records carry the key,
bounded bytecode length and payload digest. Invalid/truncated records or
unavailable cache directories fall back to normal compilation. Temporary
files are renamed after complete writes. Existing GPU shader-object caches
and shader optimization flags remain unchanged. `DOA3_SHADER_CACHE_DIR`
overrides the location for isolated tests/profiling.

This removes repeat compilation after a shader has been encountered, including
across launches. It does not eliminate first-encounter compilation, texture
uploads, or audio waits. `DOA3_PERF_CAPTURE` additionally logs cache-hit/compile
time and voice-retirement total/wait time for follow-up attribution; ordinary
launches do not emit these profiling records. A follow-up cold-cache run
measured 21.86 ms retiring four voices on frame 5237; most of that time was
in polling sleeps, with individual `Sleep(1)` calls taking about 2 ms.

Signal an auto-reset host event when the APU raises a pending interrupt.
The retirement loop waits for this event with the original 1 ms timeout,
so an IRQ can wake it immediately. Waking never acknowledges the interrupt:
the existing ISR/DPC service and guest retirement flags remain authoritative.
Deterministic APU stepping retains its existing branch. Event creation failure
falls back to sleeping, and shutdown closes the event after joining the APU
worker. A stale event can cause one harmless early retry. This avoids removing
the retirement check, which previously allowed use-after-free crashes.

The disk-cache regression covers byte-identical reuse, creation of cached
pixel/vertex shaders on WARP, key invalidation, damaged header/payload recovery,
compiler errors, and an unavailable cache location. Disk-cache, combiner-cache
and point-sprite WARP tests pass. APU IRQ tests cover pre-wait and concurrent
signals, acknowledgement, coalescing, stale wakes, lifecycle reset and the
unavailable-event fallback. The Release build succeeds. A warm-cache launch
verified 14 disk hits at 0.232-0.795 ms with no new shader compilations in the
initial sample (prior cold compilations were roughly 6-15 ms each). The user
still reported visible hitches with these changes.

### Audio retirement between output batches

The follow-up trace attributed 35.253 ms of a 62.200 ms frame (12969) to
retiring seven voices, each taking about 4-6 ms. Frame 22391 spent another
47.195 ms retiring ten voices. An event alone did not resolve this: the audio
worker waits between 256-sample output batches, so serial guest destruction
can repeatedly wait for the next batch to raise each stopped voice's idle trap.

While the guest is explicitly waiting for retirement, perform a control-only
voice-list walk under the APU lock. For the first already-inactive, unlocked
voice, emit the normal SE2FE_IDLE_VOICE trap and publish the interrupt. The
guest ISR/DPC still acknowledges and unlinks it. Preserve outstanding traps;
skip halted/disabled notification paths; bound malformed lists. This poll does
not mix samples, advance chip time/envelopes, or mark an active voice stopped.
The deterministic netplay branch continues using its original chip stepping.

Idle-walk regressions cover all three lists, active/locked voices, trap
preservation, guest acknowledgement, malformed links and disabled traps.
Idle-walk and audio-mix tests pass. The Release build links successfully.
The next live run observed 55 logged retirements with a 0.026 ms median and
1.201 ms maximum; the largest multi-voice total was 0.642 ms. This removes
the observed serial output-batch waits, but the user still sees hitches.
An initial 9,852 foreground Story frames still included 11 frames over 25 ms,
with a 48.949 ms maximum (33.173 ms inside presentation). These are different
playthroughs, not a matched before/after benchmark. Later samples also caught
new shader compilation and waits acquiring the audio voice lock.

Extend opt-in frame capture to 72,000 presentations (about 20 minutes at
60 Hz). Log slow frame boundaries and operations with QPC timestamps to
separate GPU shader creation, texture allocation/upload, DXGI presentation,
and APU voice-lock acquisition. Ordinary launches leave these timers and logs
disabled. The targeted diagnostic Release build links and the point-sprite
WARP test passes. The live run still showed foreground intro frames of
35.371 and 36.519 ms without any individual shader creation, texture upload,
or audio-lock operation exceeding the reporting threshold in those frames.
Thread samples elsewhere in the run repeatedly landed in the generic shader
preparation underneath `d3d8_DrawNv2aFF`; samples also include unrelated
background/focus-switch frames and are not a complete CPU profile.

### Avoid discarded generic shader uploads in NV2A draws

`d3d8_DrawNv2aFF` previously prepared all generic fixed-function shaders and
uploaded VS transform, VS lighting and PS constants, then replaced the vertex
shader/layout/VS b0 with the NV2A implementation and PS b0 with the combiner.
Try combiner preparation first and use generic preparation only on fallback.
The NV2A shader does not read generic VS lighting b1. This eliminates three
unused constant-buffer uploads per successful NV2A/combiner draw, preserving
the generic PS when combiners are unavailable.

The WARP regression executes the production draw and shaders, alternates
combiner/fallback paths, changes constant colors and vertex colors, and moves
geometry out of view with a changed transform. Pixel checks pass and the
Release executable links successfully. Measuring the same intro in the
installed build still showed a 39.004 ms foreground intro frame; the user
confirmed the hitch remains. Shader-cache reads in that frame totaled about
1.8 ms and retirement took 1.968 ms, so neither explains its full duration.

### Story texture cache pressure

The next trace sampled a 32.063 ms foreground intro frame in texture creation
and driver allocation. A read-only inspection found all 1024 cache entries
occupied at 65,931,928 accounted bytes, well below the 192 MiB budget. The user
also reported that replaying without restarting still hitches, somewhat less.

Allow 4096 entries and a proportionally sized hash table within the same
192 MiB budget. Under entry or byte pressure, reuse the oldest compatible
allocation not used this frame (exact dimensions, format, mip count and byte
size), removing its old hash identity and fully uploading the new contents.
Incompatible allocations retain normal LRU eviction. This avoids avoidable
free/create driver work, but first encounters still require texture uploads.

Production cache-policy tests cover full-table lookups and deletion, reuse,
current-frame protection, mip/size/format incompatibility, and byte-pressure
handling. Tests pass and the Release build links. Opt-in per-frame texture
totals report allocation and upload costs for the next live comparison;
effectiveness against the intro hitch remains unverified.

The next live run still hitched: foreground frame 5645 took 38.090 ms,
with the preceding renderer flush reporting 55 texture allocations/uploads
totalling 6.948 ms. Frame 14723 took 40.007 ms, with 119 allocations/uploads
totalling 12.624 ms. Texture setup explains only part of these intervals.
The user identifies the recurring spike as the first frame of each intro.

Add opt-in `[PERF-PHASE]` attribution at the existing 3.1 frame boundary:
previous-frame pacing (including worker laps), work before the renderer flush,
pushbuffer translation as an inclusive subset, final flush, auxiliary work,
and presentation. Worker total and longest lap distinguish a pacing sleep
from a worker consuming the remaining deadline. Timers do not change pacing
or gameplay, and remain disabled without `DOA3_PERF_CAPTURE`. The diagnostic
Release build links successfully; live first-intro attribution is pending.

Focused replay then captured frame 6466 at 32.435 ms (32.719 ms in the
phase reporter, which includes logging overhead): 14.566 ms previous pacing,
17.214 ms before flush (9.625 ms pushbuffer subset), 0.389 ms final flush,
0.002 ms auxiliary work, and 0.547 ms presentation. The 59 new textures in
that frame consumed 5.092 ms within the pushbuffer work. Thread samples
across the startup interval include texture allocation, guest D3D state
setup and CPU vertex-program execution; they do not identify a single
blocking call. The 33 ms pre-render burst in the earlier frame 1525 occurred
unfocused and is not sufficient evidence of the focused intro's cause.

There are also separate foreground presentation stalls: frame 7088 was
48.712 ms, including 31.612 ms in DXGI Present. Do not conflate these with
texture preparation or claim the intro hitch is fixed. Moving the existing
post-present pacing before presentation could absorb variable render costs
but would change input-to-display latency; no such timing change is applied.
Saved capture: build/hayabusa-frame-phases.csv, story-frame-phases-stderr.log
and story-frame-phases-stacks-20261013-195504.json. The sampler is stopped.

### Reduce first-use texture setup without moving frame pacing

Keep the existing 60 Hz and post-present pacing after the user chose to
preserve low input latency. NV2A now fills the complete mip chain in staging
before creating its D3D11 texture with initial subresource data. This replaces
an empty GPU allocation followed by one UpdateSubresource per mip. Existing
cached textures keep ordered per-level updates; ordinary D3D8 CreateTexture
still allocates its GPU resource immediately. Do not publish the NV2A cache
fingerprint until every level and the final GPU resource succeed, allowing
failed initialization to retry. Deferred publication rejects incomplete or
locked staging data.

Allocate one aligned staging block per texture rather than one heap block
per mip, and decode ordinary swizzled mips directly into matching staging
rows. Palette expansion and padded-row fallback retain their conversion path.

Production-resource tests read back every mip for BGRA8, RGB565, A8 and
DXT1/3/5 on WARP and hardware, compare exact bytes, and verify that subsequent
single-level updates preserve the other levels. They also cover incomplete
publication, repeated finish calls and release before publication. Texture
cache, NV2A draw and point-sprite regression suites pass; Release links.
Paired 64-texture hardware batches show modest, variable upload savings, not
evidence that the full intro hitch is solved. Live comparison is pending in
the story-batched-upload diagnostic build. GPU creation time is now included
in the upload total, so compare allocation plus upload across builds.

The live upload-only run still showed foreground scene-start spikes (33.423 ms
at frame 1462; 38.262 ms at frame 5212). The latter loaded 55 textures with
7.723 ms total allocation/upload, versus 6.948 ms for 55 textures in an earlier
run; these are not matched A/B scenes and do not establish a live win. Keep
the remaining hitch explicitly unresolved.

CPU vertex-program execution also appeared in startup samples. Avoid fetching
unused source operands: MOV/ARL need A, ADD needs A/C, dot products need A/B,
and ILU needs C. All required operands are still fetched before either paired
operation writes registers, preserving simultaneous-source semantics. No
arithmetic, constants, masks, register aliasing or vertex counts change.
Differential tests against unconditional operand fetching cover 32,768
opcode/operand combinations, paired MAC/ILU register dependencies and program
start bounds. Results match bit for bit. A paired 250,000-vertex DP4/MOV
microbenchmark takes roughly 20–22 ms versus 24–26 ms for unconditional
fetches; this is a component benchmark, not an overall frame-time claim.
Release links; live verification of the combined build is pending.

Combined live build (story-setup-optimized, saved CSV
build/hayabusa-setup-optimized.csv) still records foreground startup frames
1502=37.502 ms, 3150=30.660 ms and 4770=36.628 ms. Frame 4770 reports
15.394 ms previous pacing, 20.948 ms pre-flush work (16.782 ms pushbuffer
subset), and only 0.282 ms presentation. Its 55 texture preparations total
7.225 ms. These different runs do not demonstrate an overall hitch fix;
first-use preparation and rendering remain concentrated on the first frame.
An additional foreground frame 5611=42.676 ms has 26.739 ms in presentation,
a separate stall from the startup work. User completed the test; visual
regression confirmation was not supplied. Input/pacing behavior is unchanged.
### 0.0.129: voice event trace for the Genra/Omega KO audio; Diagnostics cleared at boot

Comparing the user's recordings of the same KO (PC 4.80 s, hardware 6.02 s,
first K.O. frame to the black screen before the ending):
- Hardware's "K.O." has a ~1.2 s decaying tail (-36 to -71 dB) and a
  regular ~22 ms envelope repetition decaying 0.72, 0.63, 0.55, 0.45...;
  the scream also fades out over ~1.2 s at the end. PC has neither. This
  is the effects-processor (reverb/echo) output the PC build never makes:
  sub_000A5C60 skips the dsstdfx.bin download and apu_dsp.c bypasses the
  GP/EP DSP.
- PC has repeated runs of exact digital silence: 36 ms at 0.18 s, then
  5.3 ms (256 samples) and 8 ms holes about 38-40 ms apart from 1.81 s to
  2.21 s and around 0.65-0.85 s. Hardware has one 13 ms gap. The log shows
  no XAudio2 starvation then, so the holes come from the guest mix itself.
  The scream's voice data was read in one 137 KB block (frame 28445), so
  this is not disc streaming.
- The PC sequence is ~20% shorter, consistent with the user's observation
  that the KO slow motion differs from hardware.

The game's main loop presents once per iteration at 60 Hz and never changes
the presentation interval, so the slow motion is game logic. To see what it
does to the voices, apu_vp.c now writes Diagnostics\apu_voices.txt while
automatic captures are on: per-voice pause/resume and pitch changes, and
"hole" lines when a playing voice ran out of sample data and later sounded
again (length in APU frames / ms), each with host ms and game frame.
Capped at 20000 lines.

Diagnostics are per run: main() calls doa3_diag_clear() right after it
settles the exe folder and before the log or any diagnostic file opens. It
deletes the plain files directly inside <exe folder>\Diagnostics, using an
absolute path built from the exe's location (never the working directory),
and leaves subfolders, links and the folder itself alone; a Diagnostics
that is missing, not a folder, or a reparse point is not touched. The log
notes how many files it removed. (Numbered 0.0.129: Jay's v0.0.128, "fix
ultrawide tag portraits and optimize scene setup", landed first.)


### 0.0.130: KO audio, round 2 -- what the trace showed, mix recording

The 0.0.129 trace of a full Hayate Story run (KO on Omega, ending movie 13
at frame 22138) had no voice holes, no pauses and no pitch change through
the KO: the game does not slow its voices down in slow motion, and no voice
ran dry and recovered. The routine pitch lines were 3D wobble around -4596
(22.05 kHz sources at 48 kHz). So the silent runs in the recording lie
between voices or across the whole mix -- 5.3 ms is exactly one 256-sample
output batch.

The trace now also records:
- voice start (pitch, mono/stereo, buffer/stream), restarted-while-playing
  and end; pitch changes only when >= 64 units (no positional wobble);
- "mix batch: N-sample silent run" when a submitted batch holds >= 96
  consecutive zero frames while voices are active;
- "output STARVED" (queue empty with voices active) and "mix batch
  DROPPED": xa2_submit_samples discarded a finished batch when the queue was
  full (return 0 without submitting), which the meter never counted --
  [XA2] now reports dropped batches too.
The submitted mix of the last 30 s is kept in memory (captures on only) and
written as Diagnostics\apu_mix_movieNN_fFFFFF.wav at each movie start and
apu_mix_exit.wav at exit (at most 8 per run), for direct comparison with
the hardware recording.

### 0.0.131: CRI worker fibers get every elapsed vblank (Omega KO stutter)

The 0.0.130 run (Music and SFX at 0, voice only) settled where the KO
holes come from. Our own submitted mix (apu_mix_movie08_f22242.wav, the 30 s
before the ending) contains them -- 10-12 ms silent runs from 24.2 s to
26.2 s -- so they are made by the game's audio, not by XAudio2 or the
recorder; no batch was dropped and the queue never starved there. The
voice trace shows no voice paused, ran dry or changed pitch. What changes
is the frame time: from frame 22025 (the KO voice is read that frame)
to ~22150 frames take 30-51 ms instead of 16.7 ms, and the holes recur
once per frame.

The ADX player decodes into a looping DirectSound buffer from CRI's vsync
server thread, which the console's vblank interrupt wakes every 1/60 s
regardless of the game thread. Here worker fibers that wait on the D3D
vblank event park on XBOX_FIB_VBLANK_KEY, and xbox_fiber_vblank_tick (run
at every scheduling decision) released them once when a vblank was due --
once per slow frame, however many vblanks had passed. ADX then decoded ~25
times a second, fell behind playback, and the voice played cleared buffer.

xbox_fiber_vblank_tick now counts the vblanks that elapsed (up to 6 = 100
ms, re-anchoring after longer stalls) into a sequence number, and the new
xbox_fiber_vblank_wait (used by the worker branch of the vblank wait in
bridge_KeWaitForSingleObject) returns at once while that fiber still has
elapsed vblanks to consume. Workers therefore get exactly one pass per real
vblank -- the hardware's 60 Hz, never more (the earlier "answer at once"
attempt ran them millions of times a second and tripped ADX's stall
check). Netplay's deterministic vblank path is unchanged.

Still open: why the KO slow-motion frames take 30-51 ms on PC. That is a
rendering/CPU cost in those frames (no diagnostic capture fired there) and
is the next thing to measure; it may also explain the KO sequence running
~1.2 s shorter than on hardware.

### 0.0.132: texture freshness check once per frame (low-end frame rate)

A tester's low-end PC idling on X Octagon at 32:9 averaged 37 fps (47 s,
apu_voices.txt timestamps) with spikes past 300 ms, and its method traces
(frames 3000-3003) show why the CPU side is heavy: 2073 draws a frame,
2005 of them on the GPU fixed-function path, median 12 indices (87,611
indices in total -- trivial for the GPU). 1556 draws repeat the previous
draw's state; between draws the game mostly sends new matrices (1742 of
2072 gaps), vertex array offsets and re-sent render states. So the frame
cost is per-draw host overhead, not geometry or resolution.

One per-draw cost was a regression from 0.0.74: get_dynamic_texture
fingerprinted the source of every swizzled/DXT texture on every bind, and
since 0.0.74 hashed textures up to 16 KB in full -- a serial FNV chain.
At ~4000 binds a frame (two stages) that is up to ~21 ms per frame on a
fast server CPU (measured: 4146 x 16 KB = 21.1 ms full, 1.3 ms sampled).
The Beach shoreline it was meant for was a stage-0 texture matrix after
all. Now each cache slot records the frame it was verified in
(sig_frame), an uploaded swizzled/DXT texture already verified this frame
is used without hashing, and only images up to 1 KB are hashed whole
(256 samples above that). Load-time address reuse (Hitomi/Kasumi) is
still caught on the first bind of the next frame.

### 0.0.133: the 10-second freeze was our own trace writer; per-draw timing

0.0.132 brought X Octagon at 16:9 to a steady 60 fps on the tester's
low-end PC and the Steam Deck; at 32:9 it measures ~55 in the centre and
~44 at a wall with the whole stage in view. The traces show why: the
wider view makes the game draw more of the stage -- 2073 draws (centre)
vs 2717-2734 (whole stage) -- and the difference costs about 4.5 ms, ~7 us
of host work per draw on that machine. Resolution does not matter.

The ~10 s freeze the tester saw is visible in his log's 5-second [DRAW]
lines: frames 2971 -> 3000 -> 3002 -> 3155, i.e. ~30 frames in ~10 s,
exactly where the periodic automatic traces wrote frames 3000-3003
(~3 MB of text each on X Octagon, ~130k small fprintf calls). Those
periodic traces (every 1500 frames, a development aid) are removed;
event-triggered traces (multisampled screens, frame-buffer effects, movie
ends) and DOA3_MTRACE ranges remain, and every trace file now gets a 4 MB
stdio buffer (doa3_mtrace_open).

Timing for the next round, all switched on together by the diagnostics
setting (main.c sets DOA3_PERF_CAPTURE=1 through _putenv_s when it is on,
so testers need no environment variable):
- Jay's existing [PERF-PHASE] (frames >= 25 ms split into pacing,
  workers, push-buffer translation and the rest) and [PERF-FRAME];
- new [DRAWTIME] every 5 s: GPU fixed-function draws per frame and the
  average microseconds per draw in vertex fetch, nv_apply_draw_state,
  constant building, combiner setup and stage textures, d3d8_DrawNv2aFF,
  and cleanup, plus the resulting ms per frame.

### 0.0.134: timing follows the Diagnostics switch live

The tester's 0.0.133 logs (centre ~55 fps, wall ~44 fps from the 5-second
[DRAW] lines; no freeze any more) had no [DRAWTIME]/[PERF-PHASE] lines:
diagnostics were switched on from the menu after launch, while 0.0.133
only exported DOA3_PERF_CAPTURE at startup and doa3_perf_begin cached the
switch forever. doa3_auto_capture_set now sets/clears DOA3_PERF_CAPTURE
(unless the user set it before launch), and doa3_perf_begin re-reads it
about once a second, so the timing lines start and stop with the checkbox.

### 0.0.135: fewer D3D11 calls per draw, probes out of ordinary builds

From the tester's 0.0.134 [DRAWTIME]/[PERF-PHASE] lines on X Octagon at
32:9 (wall view, ~2860 GPU draws a frame, 25.7 ms frames): ~9 ms is the
draws themselves, of which d3d8_DrawNv2aFF alone is ~1.7 us a draw, and
~12 ms is game code. Two changes aimed at those:

Bind cache (d3d8_internal.h: g_d3d8_bind_epoch / d3d8_BindInvalidate).
The NV2A draw path no longer re-issues D3D11 calls for what is already
bound:
- combiners: the same combiner state as the last draw reuses its shader
  without the key/hash/probe; PSSetShader only when the shader changes;
  the PS constants are built every draw but only mapped and uploaded when
  they differ from what the buffer holds; PSSetConstantBuffers only after
  an invalidation. The LRU counter now advances per lookup instead of per
  draw, which keeps the same eviction order.
- render states: blend, depth-stencil and rasterizer objects are kept by
  their full descriptor (64 of each) instead of released and recreated at
  every change, and OMSetBlendState / OMSetDepthStencilState / RSSetState
  run only when the object, blend factor or stencil ref changed. The old
  dirty check hashed a few render states with XOR, so two different
  states could collide (and STENCILWRITEMASK and the stencil ops were not
  in the hash at all); comparing the whole descriptor removes that.
- samplers: already cached; they are now also rebound after an
  invalidation (the movie draw sets sampler 0 and did not restore it).
- NV2A VS: the per-draw constants are still uploaded; VSSetShader,
  IASetInputLayout and VSSetConstantBuffers only after an invalidation.
Everything else that binds shaders or OM/RS state calls
d3d8_BindInvalidate: the generic D3D8 shader path, the programmable VS
path, the debug views (when active), the movie draw, and the end of the
Present blit (overlay, combat view). Comparing object pointers is safe
because an object that is still bound is kept alive by the context.

Probes. Every recompiled function's wrapper (gen/recomp_probes.c) did a
stack-balance check, a first-call registration for the call profile and a
trace test on every call. These are development aids and are now built
only with the CMake option DOA3_PROBES=ON (esp_probes.py emits both
forms); ordinary builds keep only the fiber scheduling point, which every
build needs. With probes off, DOA3_PROF and DOA3_TRACE_FN have nothing to
report.

### 0.0.136: one upload per NV2A draw; fewer diagnostic screenshots

The tester's 0.0.135 run (X Octagon 32:9, diagnostics on): centre 57.4 fps
(17.4 ms frames, was ~55), wall 46.1 fps (21.7 ms, was ~44); draw cost per
frame 6.7 -> 5.3 ms centre and 9.0 -> 6.8 ms wall, d3d-draw 1.68 -> 1.02 us
per draw. Frame times are steady (p99 21.3 / 23.4 ms); the one slow frame
every 300 is the diagnostics CSV flush. The frame gain (~1 ms) is smaller
than the draw saving, so part of the cost moved into the driver.

d3d8_DrawNv2aFF still mapped two rings per draw (vertices into the shared
DrawPrimitiveUP ring, indices into their own) and rebound both buffers at
the new offsets every draw, plus the VS constants:
- one ring now holds a draw's vertices and indices (a D3D11 buffer can be
  a vertex and an index buffer at once): one Map per draw;
- it stays bound at offset 0 and each draw addresses its data through
  DrawIndexed's first-index and base-vertex arguments (vertices are placed
  on a multiple of the 40-byte vertex), so IASetVertexBuffers /
  IASetIndexBuffer / IASetPrimitiveTopology run only after an
  invalidation, a ring rebuild or a topology change. SetStreamSource and
  SetIndices now invalidate too;
- the NV2A VS constants are uploaded only when they differ from the last
  upload (consecutive draws of one object share them).
tests/nv2aff_draw stubs the new upload with a non-zero base vertex and
first index.

Diagnostic screenshots cut back: the draw-numbered boot captures
(frame_at5/15/30/400/1200/2400.bmp), the post-movie frame_postN.bmp and
the movie-end captures (mvend_*.bmp plus mtrace_mvend_*.txt, five per
movie for the first four movies, ~22 MB per screenshot at 32:9) are gone.
aa_*.bmp with its trace, movie_030/200.bmp and explicit DOA3_MTRACE ranges
remain.

### 0.0.137: vertex fetch decodes each attribute format once per draw

The tester's 0.0.136 run: centre 58.5 fps (median frame 16.9 ms), wall
45.3 fps at ~6% more draws than the 0.0.135 spot; d3d-draw down to
0.91/0.86 us per draw. Vertex fetch (0.83 us per draw, ~1.7 ms per centre
frame) is now the largest draw phase.

nv_gpu_ff_submit read every attribute of every unique vertex through
nv_fetch_attr, which decodes SET_VERTEX_DATA_ARRAY_FORMAT, re-checks the
slot and its bounds, defaults all four components and switches on the
type on each call (four to five calls per vertex). NvAttrReader now
decodes position, normal, both texcoord sets and colour once per draw
(host pointer, stride, type, count and the highest index whose 16 bytes
stay inside guest RAM, the same bound nv_fetch_attr applies), and the
loop reads with fixed-size copies for float32 and the colour word
directly. A format nv_fetch_attr cannot read still sends the batch to the
CPU path. Results are identical to nv_fetch_attr (checked against it over
1.6M random formats, strides, bases and indices; the only difference is
that an index whose address would wrap past 4 GB is rejected instead of
read). Measured on this machine, 20-23 ns -> 6-12 ns per vertex for a
typical position + normal + colour + texcoord layout.

### 0.0.138: texture-cache diagnostics for the Beach flicker; no movie WAVs

After a long session (~47 min, 32:9) the Beach (Sunset) stage-select
background lost its water and sand detail for two frames at a time,
repeatedly (captured on video; frames 2-3 of 4 smooth, 1 and 4 normal).
It did not happen right after a restart, and the run's log was
overwritten. Suspects: the palette-animated shoreline texture (the cache
must notice the in-place palette rewrite; 0.0.132 verifies once per
frame), the 192 MB texture budget that only long sessions reach (then
every new texture evicts or recycles a slot), or the guest rewriting the
palette after the kick-off. Diagnostics to tell them apart, all in the
log:
- [TEXCACHE] every 5 s with [DRAW]: resident MB / slots, textures
  created and recycled, failed creations, bindings that resolved to no
  texture, palettised checks, palette changes, bad / all-zero palettes,
  bind gaps;
- [TEXDIAG] events (at most 200 per kind): a binding that resolved to no
  texture (offset, format, palette, rect), a palette register outside RAM,
  an all-zero palette, and a texture bound on every frame for 120+ frames
  that then misses one to three frames (get_dynamic_texture is now a thin
  wrapper over get_dynamic_texture_impl; the texture_cache test's
  extraction boundary is unchanged);
- performance_frames.csv runs for two hours instead of 20 minutes.

The per-movie audio dumps (apu_mix_movieNN_fNNNNN.wav, ~5.7 MB each) are
gone: they served the KO-before-ending stutter, since fixed. The
single apu_mix_exit.wav at shutdown remains.

Zack's black character-select portrait in two aa_*.bmp shots was the
model still loading (no costume list yet; the frame had 418 draws instead
of 562); not a bug.

### 0.0.139: README, debug guide, LICENSE and .recomp.json

- README.md rewritten for this fork: the Japanese release (TC-001, XBE
  certificate version 1, "v3.1"), current features (aspect ratios up to
  32:9, game volume, streamer mode, training), first-run setup, the files
  the port keeps next to DOA3.exe, every player-facing Escape-menu tab,
  bug-report instructions, credits (including parkerallan/doa3-recomp and
  the bundled libraries), a legal notice and the license summary. The
  animated image and the outdated menu screenshot are gone.
- docs/DEBUG.md: the Debug tab (unlockables, camera, combat diagnostics
  and stage hazards, performance, vision modes, screenshots/UI,
  diagnostics) and Online > Developer Settings, kept apart so tools can be
  dropped before v1.0 without touching the README.
- LICENSE: MIT text (the project had only the README statement), noting
  that xemu-derived code stays LGPL-2.1-or-later (src/nv2a/nv2a_core.c
  says so) and third_party libraries keep their own licenses.
- .recomp.json at the repository root for recomp.fyi (see
  parkerallan/doa3-recomp issue #4): Japan, revision 1 (the XBE
  certificate version), status playable.

### 0.0.140: Jay-Collins in LICENSE and .recomp.json

The LICENSE copyright line and the .recomp.json maintainers now include
Jay-Collins (https://github.com/Jay-Collins).

### 0.0.141: README marks online play as experimental

Online play has not been tested on the 3.1 build. It is removed from the
README feature list, and the Online section says it is experimental and
expected not to work yet.

### 0.0.142: credit SkillerCMP/xemu-Cheats

The README credits SkillerCMP/xemu-Cheats, the xemu build used as the
debugging reference for this fork. Cxbx-Reloaded stays listed as the
reference used for the original port.

### 0.0.143: reduce rendering and diagnostic overhead

Skip redundant pixel-shader resource bindings with render-target and
external-renderer invalidation. Share the diagnostic enable flag so disabled
timing checks avoid repeated clock and locked operations. Reuse the SHA-256
provider and avoid full texture-cache scans when a free slot fits the budget.

Frame CSV formatting and writes now run on a bounded worker queue; live
capture toggles reset timestamps and preserve the two-hour recording limit.
Audio capture uses bulk ring copies and writes snapshots outside the mixer
lock. Added lifecycle, concurrency, SRV binding and real streaming-ring
regression coverage, and repaired standalone test integration.

Measured CPU component costs improved, but matched 3D demo presentation
tests on the v0.0.136 baseline showed no meaningful FPS improvement: both
builds held approximately 60 FPS, with mixed 1% lows. These measurements
predate integration of upstream v0.0.137-v0.0.142; see OPTIMIZATION_REVIEW.md
for exact results and limits.

### 0.0.144: texture-cache experiments for the long-session glitches

A ~66-minute session (0.0.143, Story with several characters, the Nine
Lives demo) ended with Zack's first Story cutscene (vs Gen Fu) black from
start to finish while the game kept drawing 360-560 draws a frame. The
0.0.138 counters show the texture cache filled its 192 MB budget at frame
~56,000 (~16 min) and stayed full; from then on every new texture recycles
or evicts a slot, and the cutscene's start recycled 40 slots at once. The
Beach flicker also appeared only after long sessions. Both point at the
cache's full-budget path, which short sessions never reach.

Experiments, in doa3_settings.ini [Debug] (read at the first texture
binding, logged as "[TEXCACHE] budget N MB, recycling on/off"):
- TextureCacheMB=N (16-2048, default 192): a small budget (e.g. 32)
  reaches the full-cache state within minutes instead of an hour;
- TextureCacheRecycle=0: under pressure, free the oldest texture and
  create a new one instead of reusing a same-sized allocation.
TEXCACHE_BUDGET is now the variable g_texcache_budget
(TEXCACHE_BUDGET_DEFAULT keeps the 192 MB value; the texture_cache test
maps TEXCACHE_BUDGET to it and defines g_texcache_recycle).

Separately, the same log had 200 "binding resolved to no texture" events
for one 128x128 DXT1 texture (offset 020D2E00, format 07750C2A) in
stretches around frames 53,000-104,000, with no failed creations. Each
NULL return in get_dynamic_texture_impl now records its source line, and
the [TEXDIAG] line prints it (once per texture and failure point) with
control0.

### 0.0.145: the black Zack cutscene is not the texture cache; trace on demand

Run 1 of the 0.0.144 experiment (TextureCacheMB=32): the cache was full
from frame ~7,000 on, yet Zack's first Story cutscene (vs Gen Fu) went black
only at frame ~255,000 (~71 min), at the same point in the session as the
0.0.143 run, and F12 screenshots (the guest frame, before presentation) are
black until the fight starts. So the game's own image is black, and the
cache's full-budget path is not the trigger. Both sessions played several
Story runs before Zack's, so the next test is Zack's Story first thing after
launch.

Debug > Diagnostics > "Trace the next frame" (g_doa3_trace_request,
recomp_manual.c) writes mtrace_manual_NNNNN.txt and aa_NNNNN.bmp for the
next frame, with or without automatic captures: a method trace of the black
cutscene shows what the game draws there.

### 0.0.146: offscreen render targets replaced least-recently-used (black Zack cutscene)

The black cutscene happens only late in a session (a fresh launch straight
into Zack's Story plays it fine) and the guest frame itself is black. The
host keeps one colour+depth target per guest surface the game renders into
(d3d8_device.c, offrt_slot). There were 8 slots for the whole session, never
released, and once full every new surface went into slot 7. Each stage and
cutscene renders into surfaces at its own heap addresses (Run 1 logged 15
distinct ones before Zack's cutscene), so after enough Story runs the table
was full of stale targets, and a scene that renders into two new surfaces
per frame had both share slot 7: rendering the second freed the first, the
fullscreen pass that samples the first found nothing, and the frame came out
black until the fight (which uses neither) started.

Now there are 16 slots, each stamped with a use counter on every render into
it and every sample of it, and a full table replaces the least recently used
target that is not currently bound for rendering. Each replacement logs
"[RTT] host targets full: surface X replaces Y" (first 100). Worst case is 16
live targets instead of 8; full-size ones are back-buffer sized.

### 0.0.147: not the slot table; log stale offscreen targets substituted for textures

0.0.146 did not fix the black Zack cutscene: the run logged 13 guest
render-target surfaces and no "[RTT] host targets full" line, so the 16-slot
table never filled and slot thrashing was not the cause (the LRU stays; it
is still better than overwriting slot 7).

Next suspect: a draw whose texture address matches a guest surface the game
rendered into earlier gets the HOST target for that surface instead of the
texture in memory (nv_apply_draw_state). Host targets are never dropped, so
after many stages the guest heap can place an ordinary cutscene texture at
an address that was a render target in an earlier scene, and the draw
samples that old, unrelated (often cleared) render instead. A fresh launch
has no such stale targets. Each host target now records the presented
frame it was last rendered into (d3d8_OffscreenTextureAge), and a draw that
samples one last rendered more than 30 frames earlier logs "[RTT] frame N:
draw samples surface X, whose host target was last rendered N frames ago"
with the texture's format, control0 and rect (once per surface per 10 s,
at most 200). The reflection targets are rendered every frame and stay
silent.

### 0.0.148: never substitute a render target for a DXT or palettised texture

From the 0.0.147 session with ten "Trace the next frame" captures of the
black Zack cutscene (costume 3, the metallic snowboard suit) and one of the
fight that follows:
- The black frames are fully black in the guest frame (aa_*.bmp max 0) while
  the game issues ~550 GPU draws, all to the default target (off=0), with
  sane composite matrices and the usual frame-start clear; there are no
  inline/2D draws in them. The fight frame (705 draws) renders normally.
- 0x028D4600 is a real render target, Zack costume 3's environment map
  (rendered every few hundred frames, sampled as stage 0 by his model with
  the 1440x960 rect), in both the black frames and the fight. It is not the
  cause by itself.
- The stale-target log did catch a real bug: two 64x64 DXT1 textures
  (0x0190FC00, 0x0200FC00, format 06610C2A) were drawn with the host target
  of a surface the game had rendered into 40,000-97,000 frames earlier.
  A render target is never block-compressed or palettised, so the
  host-target substitution now requires a texture colour format other than
  DXT1/3/5 (0x0C/0x0E/0x0F) and I8 (0x0B).
Still needed: a trace of the same cutscene rendering correctly (fresh
launch) to diff against the black one.

### 0.0.149: GPU draws in method traces show where they land

The black Zack cutscene is not tied to session length: on 0.0.148 it
appeared on the first attempt after a fresh launch, and played correctly on
another fresh attempt. So far it has only been seen with costume 3 (the
metallic snowboard suit, whose environment map is the render target
0x028D4600). Traces of black frames (04969-05621) and good ones from the
same session (fight 05799, fight-2 intro/outro 08666 and 10761):
- no register value appears only in the black frames;
- the black frames draw only lit geometry (col=0 lit=1, 262-480 draws); the
  good cutscene frames also draw ~65-75 prelit stage draws (col=1 lit=0)
  and the stencil-shadow passes; yet even the lit draws leave nothing in a
  frame that is entirely 0.
To see whether those draws are on screen at all, a traced GPU draw now
appends "verts N box(x0,y0)-(x1,y1) w[min,max] behind K": its vertices
through the composite matrix in guest pixels, the w range and the number
behind the camera (computed only while a trace is open).

### 0.0.150: traced GPU draws report the pixels they wrote and where they went

0.0.149 session (Zack costume 3, five Story attempts on one boot; black on
the first, correct on the other four), traces of black frames 04974-05099
and good ones 10616-10795 of the same cutscene:
- the projected geometry is the same in both (e.g. the 107-vertex draw at
  box(-22104,485)-(3844,3841) w[-12.8,15.6] vs (-8585,494)-(4964,1780)
  w[-12.6,15.3]; the same draw sizes, 53 vertices behind the camera in
  both), so the camera is fine and the draws are on screen;
- the register state of the black frames is a subset of the good ones;
- the costume-3 environment map (0x028D4600) was rendered ~330 frames
  before both, at the cutscene start;
- yet the black frames end entirely 0, with no trace of the fogged
  (grey-tinted) colour every one of those draws would produce: the draws
  write no pixels to the guest frame at all.
To tell which (depth/stencil/scissor rejection, or the wrong render target
or viewport), a traced GPU draw now also reports "pixels N rt R vp(x,y WxH)
sc(l,t)-(r,b)": the samples it actually wrote (a D3D11 occlusion query,
waited on; traces only), the bound render target (0 guest frame,
1 offscreen target, 2 other), the viewport and the scissor rectangle.
The probe code sits before nv2aff_ring_upload, outside the nv2aff_draw
test's extraction; the test stubs d3d8_probe_begin/end.

### 0.0.151: Zack costume 3's environment map in reflections and character select

On 0.0.150 neither the user nor the testers can reproduce the black Zack
cutscene any more. Of the changes since it was last seen, 0.0.148 (no
render-target substitution for DXT/palettised textures) is the one that
changes what those frames sample; most likely that was it.

Zack's costume 3 (metallic suit) samples its environment map from the
surface the game renders it into, 0x028D4600. Two remaining differences
from hardware:
1. His low-poly reflection in the floor showed the default environment map
   (Beach day) instead of the stage's. The floor reflection is itself an
   offscreen pass, and render-target substitution was disabled while any
   offscreen target was bound, so that pass sampled the guest memory
   (still the default map). d3d8_HasOffscreenTexture already refuses the
   target being rendered into, so the blanket condition is gone: a draw
   inside one offscreen pass now samples another pass's target.
2. At character select he kept the last stage's environment map; hardware
   shows the default one there, because the game loads the default map back
   into 0x028D4600 and on hardware the latest writer of that memory wins.
   Here the host target always won. Now each surface's guest memory is
   fingerprinted (nv_tex_signature over guest pitch x height) whenever the
   game binds it for rendering, and a draw samples the host target only
   while that fingerprint still matches; once the game has written the
   memory, the draw samples the memory, until the game renders into the
   surface again. This also covers the heap handing a render target's old
   address to an unrelated texture (the cause the 0.0.148 format rule
   addressed), for any format.

### 0.0.152: save the textures of a frame as .dds

Debug > Diagnostics > "Save the next frame's textures" (g_doa3_texdump_request)
writes each texture bound during the next pgraph frame once, as
Textures\doa3_<address>_<w>x<h>_f<NV format>.dds next to the exe (not under
Diagnostics, so it survives the launch-time clear). d3d8_TextureWriteDDS
(d3d8_resources.c) writes the D3D8 texture's staged levels: the
translator's host rows (unswizzled, I8 palettes expanded to A8R8G8B8, DXT1/3/5
blocks as is) with a matching DDS pixel format, every uploaded mip level.
Render-target substitutions are not textures and are not saved. Asked for to
extract the default environment map of Zack's costume 3 (Beach, day) at
character select for Blender.

### 0.0.153: costume 3 at character select; counting what one-off captures draw

User test of 0.0.151: the low-poly floor reflection of Zack's costume 3 now
uses the stage's environment map like the main model. Two issues remain.

1. Character select still shows the last stage's map. The 0.0.151
   fingerprint cannot see the reload: in the port the render never reached
   guest memory, so the memory still holds the default map, and the game
   writing the same default map back leaves it byte-for-byte unchanged. On
   hardware the memory held the render and the reload overwrote it. Now a
   one-off render (a target not rendered for more than 60 frames, i.e. not
   the per-frame reflections) writes a marker into the fingerprinted words
   of its guest memory when it starts, so any later reload changes them and
   the draw samples the memory again. The per-frame reflection targets are
   not marked (a stage-1 read of one, which has no host substitution, would
   otherwise show the marks).
2. In Zack's Story cutscene before Fight 3 (vs Gen Fu), the suit is black on
   one attempt and shows a partial capture (the base environment without the
   full stage) on another; the fight's own capture at the end of the
   cutscene is correct. The game captures the map once at the cutscene start
   (~330 frames before the traced frames) and samples it for the whole
   scene, so whatever that one pass produced is what shows. To see what it
   produced, every one-off pass now logs one line when the game switches
   away from it: "[RTT] pass into surface X at frame N: D GPU draws, W wrote
   pixels (P pixels); first draw rt/vp/sc", using the 0.0.150 occlusion
   probe for that pass only (at most 100 lines).

## 0.0.154

1. Zack costume 3, results of the 0.0.153 pass log (five Story runs on one
   boot). Character select now shows the default map. The cutscene capture
   before Fight 3 happens about 6 frames before the same three reads
   (loadfile.afs offsets 19BFF000, 19529800, 06D5E000) in every run, and
   the stage's own reads finish about 300 frames earlier in all runs, so
   the stage data is in memory each time. What the pass wrote varies:
   - first run (black suit): 106 draws, none wrote a pixel;
   - a partial run: 135 draws, 2 wrote (128523 pixels, about two full
     256x256 layers, likely the sky);
   - good runs: 177 to 331 draws, 65 to 154 wrote.
   Draws that write nothing while full-screen layers do points at the
   depth or stencil test, or the camera, rather than missing data. To see
   which, the first 24 one-off passes now also write every draw and clear
   to Diagnostics\rtpass_<surface>_<frame>.txt in the method trace's @DRAW
   format (depth/stencil/blend state, screen box, w and new z/w ranges,
   pixels written), plus @CLEAR lines and a closing @END total.
2. Debug > Camera controls: the Field of view slider and its reset button
   were removed (they caused more false bug reports than they helped). The
   free camera is unchanged; docs/DEBUG.md and README.md updated.

## 0.0.155

Zack costume 3, results of the 0.0.154 pass files (five Story runs, no
black suit this time). In every environment-map pass the draws that write
nothing are simply outside the 256x256 map: their screen boxes lie past its
edges (for example x 271..468), with ordinary depth (LEQUAL against a 1.0
clear), stencil ALWAYS and z/w near the far plane. Nothing is being rejected
by depth or stencil. So the black capture (106 draws, none wrote) means the
whole scene was out of view of the capture's camera, and the partial ones
mean only the sky was in view: the camera the game uses for that one pass
is wrong on those attempts, not the rendering. Both black captures so far
were the first Fight 3 cutscene after a fresh launch, which points at timing
(the first visit's longer frames) moving the game's camera state.
- The 24 files were used up by frame 15328, mostly by other surfaces and by
  the empty 03C24000 passes. Now up to 64, and passes with no draws delete
  their file and do not count.
- Each file records the composite matrix (the fixed-function camera and
  projection) as @CAMERA lines whenever it changes (up to 16 per pass), so a
  bad capture's camera can be compared with a good one.

## 0.0.156

Zack costume 3, five fresh-launch runs with the 0.0.155 camera lines. Every
environment-map pass draws the scene several times with the camera turned in
90-degree steps around the vertical axis (the composite matrix's w row
rotates between, for example, (0.94, 0.02, 0.34), (-0.94, 0.02, -0.34) and
(0.34, 0.02, -0.94)), so the map is built from the capture point looking
around the stage. The character-select capture is identical in every run
(level camera, w row (-1, 0, 0)). The in-fight and cutscene captures differ
from run to run and their tilt follows the game's current camera: the worst
one this time (run 5, frame 3908, the capture at the start of fight 2, 58
draws of which 3 wrote, each covering the whole map) was taken with the
camera pointing straight up or down (w row (0.03, -0.999, 0.03)), so only
the sky or floor layers landed in the map. That matches the partial maps
seen on other stages too, and the black one is the same thing with nothing
in view. The open question is which camera the game should be using at
that moment; if it is the previous scene's last camera (the win pose or
results screen), the order of the capture and the camera update differs
from hardware.
- Each pass file now starts with @BEFORE (the matrix left by the last draw
  before the pass, normally the previous frame) and ends with @AFTER lines
  for the next six draws on the main target, so the capture's camera can be
  compared with the cameras either side of it.

## 0.0.157

1. The shader disk cache moved from `%LOCALAPPDATA%\DOA3\shader-cache-v1`
   to a `ShaderCache` folder next to DOA3.exe (found from the executable's
   own path), so the port keeps nothing in AppData. `DOA3_SHADER_CACHE_DIR`
   still overrides it. The old AppData folder is not touched; it can be
   deleted by hand. README's file table lists the new folder.
2. Zack costume 3: the second set of five runs was on 0.0.155, so the
   pass files have no @BEFORE/@AFTER lines yet. Notable in them: two
   fight-start captures that wrote little (run 1 frame 2983, 21 of 271
   draws; run 5 frame 1551, 29 of 256) draw everything after their first
   object with the w row exactly (0, 0, -1), an unrotated camera looking
   down -Z, unlike every full capture. The character-select capture is
   still identical in all ten runs.

## 0.0.158

Zack costume 3, five runs on 0.0.157 including one black suit (run 2, the
capture at frame 5067 before Fight 3: 107 draws, none wrote a pixel). In
that capture every draw used a camera looking straight down (w row
(-0.01, -1.00, -0.02)); the last draw before it was already steeply down
(-0.69, -0.72, 0.10). The draws right after each capture use the same matrix
as the capture's first draw, and every capture's look direction tilts with
the game's camera of the moment, while the character-select capture is level
and identical in all fifteen runs. Good fight-start captures (700+ draws
writing) are nearly level too. So the environment camera takes its pitch
from a game camera that, on the bad attempts, is still pointing somewhere
unexpected (straight down, or the exact unrotated -Z of the earlier weak
captures).
- The camera-activation hook (sub_00029E30, already used by the free
  camera) now records the last 32 activations: frame, slot, eye, target,
  fov and the slot's +ABC flag. Each one-off pass file lists them as
  @CAMSLOT lines, plus @SLOT lines with all five slots' current eye,
  target, fov and view matrix, so a bad capture shows which slot supplied
  the camera and whether its eye or target was stale.
- tests/free_camera stubs the new diagnostics call.

## 0.0.159

Zack costume 3, conclusion: the environment map's partial captures and the
fully black one both happen on real hardware (recorded by the user). The
game builds the map from the current camera turned in 90-degree steps, and
on some attempts that camera starts at floor height looking straight down,
so nothing is in view. That is the original game's behaviour and the port
matches it. The real port bugs found on the way stay fixed: character select
showing the last stage's map instead of the default (0.0.153 guest-memory
marks) and the low-poly floor reflection (0.0.151).

Diagnostics added for the hunt are removed:
- the per-pass Diagnostics\rtpass_*.txt files (@PASS/@DRAW/@CLEAR/@CAMERA/
  @BEFORE/@AFTER/@SLOT/@CAMSLOT lines) from 0.0.154-0.0.158, which were
  written for every player whether or not diagnostics were on;
- the camera-activation ring in the sub_00029E30 hook and
  src/nv2a/doa3_camlog.h (recomp_manual.c and tests/free_camera are back to
  their 0.0.153 state).
Kept: the one-line "[RTT] pass into surface ..." summary, now only with
Debug > Diagnostics > Automatic diagnostic captures on, so the occlusion
probe no longer runs in normal play; the z/w range in method-trace @DRAW
lines; and the texture-dump button.

## 0.0.160

Movie display option (16:9 movies). On hardware set to widescreen the game's
options add a movie display setting, stored at 0x0059FB4C (0 = 4:3,
1 = 16:9); with 16:9 the console zooms each Sofdec movie until it fills the
width of the 16:9 picture, cropping the top and bottom. The port always drew
movies as a centred 4:3 picture. movie_draw_to_guest now reads the setting:
with 16:9 the movie is drawn as wide as the 16:9 picture (the whole target
at 4:3 Anamorphic and 16:9, the centred 16:9 area at 21:9 and 32:9, not the
whole ultrawide frame) and 4/3 of its height, so an eighth is cropped at the
top and at the bottom. 4:3 Full Screen always shows the 4:3 movie (the
console has no option there), and the 4:3 setting is unchanged.

## 0.0.161

Timer backing shows as a black rounded box for about 7 frames during the
GET READY fade of Christie's Story Fight 3 intro (vs Bayman); the round-start
case was fixed on 2026-10-12 ("Timer shadow appears before the HUD") by
honouring transparent black for one exact inline TEX0 * V0 combiner set in
nv_use_vertex_diffuse. This scene most likely uses a different combiner set
(fog, a second stage or another final combiner), so the zero colour falls
back to the opaque texture again. No trace of the frame exists, so every
distinct combiner set that takes that fallback for an inline draw with all
vertex colours 00000000 is now logged once as "[HUDZERO] frame N: ...
1E60/0AC0/0260/1E40/0AA0/0288/028C | stage1 ..." (at most 40 lines), to
widen the rule to exactly the timer's variant.

## 0.0.162

The 0.0.161 log of Christie's Fight 3 intro shows the timer draw (mode 7,
the 07710B2A timer atlas) with the round-start TEX0 * V0 program
(0AC0 08040000, 0260 18140000, outputs 0C00) but final combiner
CW0 0288 = 0000000E instead of 0000000C: D = spare0 + specular rather than
spare0. These 2D draws supply no specular colour, so the result is the same
TEX0 * V0, and the zero vertex colour is the transparent start of the fade.
nv_use_vertex_diffuse accepts both final-combiner forms, so the backing fades
in instead of showing black (and the timer-body edge path applies there
too). tests/hud_fade checks the 0E form; a different final combiner still
keeps the fallback. The [HUDZERO] logging is removed.

## 0.0.163

Life bars a pixel off (user, 1x and 3x; Kasumi vs Hayate trace
mtrace_manual_08583). The HUD block runs with SET_TRANSFORM_EXECUTION_MODE
= program (1E94 = 6) and loads the Xbox D3D passthrough program for XYZRHW
vertices with constants (1, 1, 2^24-1, 1) and (0.53125, 0.53125, 0, 0): on
hardware every pre-transformed vertex gets the 0.53125 screen-space offset,
and the NV2A samples pixel centres at +0.5, so the game's sprites at x.5
(bar frame and dark backing at 46.5/32.5, 1:1 with their 128x128 atlases)
land on texel centres. The inline path never ran that program, so all inline
2D was drawn half a pixel up and left: 1:1 HUD textures were sampled
halfway between texel rows (the light 1-pixel row under the bar's top edge,
the uneven top line), and the bars' stencil mask (submitted at 47.53/33.53)
started two pixels inside the frame instead of one.

The inline path now measures the loaded program's effect on two positions
and, when it is exactly unit scale plus a sub-pixel offset (the
passthrough), adds that offset to every inline vertex before the usual
window fit, so it scales with the internal resolution like the rest of the
position. nv_hud_geometry's full-screen test allows 2 pixels at the edges
(was 1) so full-screen quads at 0.53125 plus the offset are still treated as
full-screen. This affects every inline 2D draw (HUD, menus, fades) by the
same half pixel the hardware applies.

## 0.0.164

2D snapped to the console's pixel grid at 2x and 3x. At 480 lines (1x)
0.0.163 matches hardware; at 2x and 3x the life bars and the character-
select panel borders still showed 1-pixel slivers and steps. The NV2A draws
2D at 480 lines, where each edge fills whole pixels (pixel k when its centre
k + 0.5 is inside), so pieces placed a fraction of a pixel apart (the bars'
mask at 33.53 against the frame at 32.5) meet on a row boundary; scaled, the
exact fractional edges do not. nv_snap_2d now runs after the window fit for
screen-space 2D (every vertex rhw = 1, not offscreen, target at least 1.5x
the guest height): each vertex moves to the guest pixel edge the console
fills to, ceil(e - 0.5) in guest pixels, and its texture coordinates and
colour are re-evaluated from the draw's own linear mapping at the moved
position, so the covered pixels sample what they did before. Draws whose
vertices do not share one linear mapping are snapped per quad (lists of
6), or left alone. Wide lines (character select's panel borders) are
snapped per segment quad. 1x is unchanged. The window fit records its
mapping (s_fit_*) for this.

## 0.0.165

Screenshot resolution. Debug > Screenshots and UI has a "Screenshot
resolution" setting ([Debug] ScreenshotScale: 0 = same as the game, 1..5).
When it differs from the internal resolution, d3d8_screenshot_request sets a
one-frame override (video_set_scale_override; video_guest_target_size now
uses video_get_effective_scale), the existing frame-boundary target rebuild
switches the guest target to that size, the first frame rendered at it is
captured, and the override is cleared so the next boundary restores the
game's scale (gives up after 30 frames). The game's own Internal resolution
stays 1x-3x; only screenshots go to 4x and 5x. The capture's pixel cap is
64 MP (32:9 at 5x is 8535x2400). Reset to defaults sets it back to "same as
the game". tests/debug_views/screenshot_test stubs the new calls;
docs/DEBUG.md and README updated.

## 0.0.166

Reverts 0.0.164 and 0.0.165: the code is back to 0.0.163 (the passthrough
screen-space offset stays). User testing: the 2x/3x pixel-grid snapping did
not make the life bars look closer to hardware and introduced new problems
on character select at higher resolutions, and F12 saved no screenshots on
0.0.165 (with Screenshot resolution at "same as the game"). Both may be
revisited separately.

## 0.0.167

Internal resolution 4x (1920 lines): Video > Internal resolution offers
"4x (1920p)", and doa3_settings.ini InternalResolution accepts 4 (load, save
and video_set_scale clamp to 1..4; unknown values still fall back to 3x).
Guest targets at 4x: 2560x1920 (4:3), 3416x1920 (16:9), 4480x1920 (21:9),
6828x1920 (32:9). tests/video_settings checks sizes up to 4x; README lists
the option. Screenshots are unchanged (they save at the render resolution).

## 0.0.168

Genra's flame overlay at 21:9 and 32:9. The traces (mtrace_manual_24191,
24467, 24676; 32:9 at 4x) show it is not 2D: a GPU-path triangle strip of 31
vertices with an animated texture (0181B780 / 0185B780), additive
(SRCALPHA, ONE), every vertex at w = 0.35 (locked to the camera), covering
guest x 152..568 and y -89..569. It is sized to overfill a 16:9 picture;
the ultrawide projection widens the view around it, so it covered only the
middle (about 18.5:9). nv_gpu_ff_submit now checks blended GPU draws of up
to 256 vertices at 21:9 and 32:9: when every vertex has the same w and the
draw covers the whole centred 16:9 area and the full height, the clip row
for x is scaled by (display aspect)/(16:9) (2 at 32:9, 21/16 at 21:9), which
stretches it about the screen centre to the width it covers at 16:9. World
geometry never shares a single w across its vertices, so it is unaffected.

## 0.0.169

Genra's flames stretched only on some frames (user). In every new trace the
flame sheet is the same 31-vertex draw covering guest 152..568 x -89..569,
which passes every 0.0.168 test except, most likely, the single-w test: the
trace prints w to three digits (0.35), while the check required all
vertices to agree within 1e-4 relative, and a camera-locked sheet picks up
small differences as the camera moves. The tolerance is now 1%, and a draw
that already spans the full width is never stretched (so a wide backdrop
facing the camera cannot be widened). With a method trace open, each
candidate (blended, w under 4, wider than a quarter of the surface) writes
an @OVERLAY line with its w range, box, surface size and decision, so a
frame that still is not stretched shows why.

## 0.0.170

Room reverb (first pass). The user's Cmp recordings (Ayane's taunt on 15
stages and several areas, hardware against port) show a reverb tail on
every hardware recording and none on the port: on open stages (Beach, Snow,
Forest, Lorelei areas 0/1/3) the tail starts about 17 dB under the voice
peak and fades over ~0.5 s; in DOATECHK area 1, Danger Zone, Lorelei areas
2/4, Aquarium, Iron Hell, Pancratium and X-Octagon it is much stronger (up
to about the voice's own level) and longer.

Cause: on the console the room reverb is the I3DL2 effect in the GP DSP
program (dsstdfx.bin), fed by the I3DL2 send mixbin. The port stubs the
effects-image download (sub_000A5C60) and apu_dsp.c passed only the stereo
and positional bins through, so voices played dry. The game selects the
room with IDirectSound_SetI3DL2Listener (sub_001F2A27, stdcall 12) from a
table of DSI3DL2LISTENER structs at 0x2D3EB8: the 23 standard I3DL2 presets,
"no reverb" at index 23 (0x2D4308), then 27 rooms of its own from 0x2D4338
(index 24 on; decay 0.14-0.82 s, reflections -603 mB, reverb -159..-2318
mB). The index is kept at 0x7FE2F0.

- sub_001F2A27 is overridden (overrides_3.1.txt; its probe wrapper
  removed): it hands the twelve parameters to the host reverb and logs
  "[I3DL2] params <addr> (table index N): ..." (first 200 calls).
- apu_dsp.c has a host I3DL2 reverb on mixbin 10 (DSMIXBIN_I3DL2): input
  high-cut from lRoomHF at flHFReference, six early-reflection taps after
  flReflectionsDelay at lReflections, and an 8-line feedback delay network
  after flReverbDelay with flDecayTime / flDecayHFRatio damping, all-pass
  diffusion from flDiffusion and line lengths from flDensity, at lReverb;
  both scaled by lRoom. The wet signal goes to the front pair. Until the
  game sets a listener the reverb is off, so tests/audio_mix is unchanged.
- The first time each mixbin from 10 up carries audio, "[MIX] mixbin N
  carries audio" is logged, to confirm the send bin.
The send level and the exact DSP response are not known yet; the next step
is to compare new port recordings against the hardware ones with the logged
table index per area and calibrate.

## 0.0.171 - room reverb decays at the console's rate

Compared the 0.0.170 recordings (release/Cmp, Ayane's taunt in 25 stage
areas) with the hardware ones. The port's reverb level was right, but its
tails died far too early: in the light rooms (index 30, flDecayTime 0.2 s)
the console's tail rings for about 0.8 s, and in Ice Cave (index 25,
0.359 s) for about 1.4 s.

Method: a Python copy of the apu_dsp.c reverb simulated each area's room
(identified by matching the port recording) on the dry voice, with the
decay time scaled 1x-6x and a free level. 4x gave the best fit in almost
every area (about 2.2-2.6 dB rms over the 700 ms after the voice in the
strong rooms, ~3.2 dB in the light ones, with the capture's noise floor
included), and the level that fit at 4x was the one 0.0.170 already uses.

- apu_dsp.c: RV_DECAY_SCALE 4.0 multiplies flDecayTime before the delay
  line gains are derived (still clamped to 0.1-20 s). Levels unchanged.
Why the DSP's decay runs 4x longer than the parameter says is not known;
it is a measured factor. The rooms at indexes 37/38 (0.70/0.82 s) were not
in the recordings, so their tails (~3 s) are extrapolated.

## 0.0.172 - spot lights have a cone (Iron Hell flash)

Iron Hell's periodic flash lit the fighters far brighter than on hardware.
The method trace of a lit frame against an unlit one: the game switches
SET_LIGHT_ENABLE_MASK (0x03BC) from 0x09 to 0x9F, turning lights 0 and 1
into spot lights (type 3) with diffuse colours up to ~2.4, range 12 and
narrow cones (SET_LIGHT_SPOT_DIRECTION e.g. (174.2, 180.8, -86.6, -262.3),
falloff (0, -0.495, 1.494): full inside ~7.6 deg, nothing past ~9.2 deg).
The fixed-function lighting (CPU nv_light_vertex and the GPU NvFF vertex
shader) treated spot lights as plain local lights, so the whole cone-less
light hit everyone in range.

- Spot factor as xemu's vsh-ff computes it (US6417851B1): s =
  dot(spot direction, L) + direction.w; 0 when s <= 0, 1 when s >= 1, else
  (s + L) / (M s + N) clamped to 0..1, multiplying the attenuation (ambient,
  diffuse and specular).
- Nv2aFFConstants gains light_spot_dir / light_spot_fall (appended; the
  shader cbuffer LSpotDir / LSpotFall), and light_dir.w carries type 3.

## 0.0.173 - 3D voices through their HRTF filters (left/right placement)

All 25 port recordings of Ayane's taunt were centred to within 0.1 dB; on
hardware she sits 0.5-2 dB left, varying with position. DirectSound (the
game's own recompiled code) positions 3D voices by writing an HRTF entry
(31-tap FIR per ear + interaural delay, NV1BA0_PIO_SET_HRIR / _HRIR_X) and
pointing the voice at it (SET_VOICE_TAR_HRTF); the VP filters the voice and
mixes the ears into the four HRTF submixes (DSMIXBIN 6-9). The port had the
filter switched off (g_config.audio.hrtf = false), and even with it on the
xemu mix sent a mono voice's channel 0 into all four bins.

- g_config.audio.hrtf on; DOA3_HRTF=0 in the environment turns it off.
- 3D voice mix: bins 0-3 take the left ear (even) / right ear (odd) filter
  output; bins 4-7 (I3DL2 and effect sends) the unfiltered voice. A mono
  voice feeds both ears.
- hrtf_filter_set_target_params no longer scales each ear to unity abs-sum
  (that erased the level difference between the ears); one common factor,
  only when the louder ear could exceed unity.
- Diagnostics: "[HRTF] vN entry H itd ... L sum/rms R sum/rms (L-R dB)"
  when a voice is pointed at an entry (first 300), "[V3D] vN hrtf=H
  bins/vol(dB)" when a 3D voice's routing or volumes change (first 600).
The FIR form, ITD sign and parameter smoothing are xemu's; whether the
hardware applies the coefficients raw is to be checked against the new
Beach (Day) left/right, near/far recordings.

## 0.0.174 - training vignette full width; Beach flicker diagnostics

Training mode darkens the top and bottom of the screen with two untextured,
blended, black bands (alpha 0x99 at the screen edge fading to 0). They are
2D draws, so at 21:9 and 32:9 they landed in the centred 16:9 HUD canvas
(trace: box 538..3951 of 4480, 1712..5126 of 6828) and their ends showed
as hard vertical edges. The inline draw path now spots a band like that --
blended, no texture, no depth test, every vertex black, spanning the full
guest width and less than 3/4 of its height -- and after the HUD fit
stretches it back over the whole target width. With a trace open, such
candidates write "@VIGNETTE ... -> full width / left".

Beach stage-select flicker (long sessions): the 0.0.173 tester log has the
texture cache full (192 MB) and "bindings without a texture" at 16-20 per
5 s whenever the Beach stage select is up (frames ~100,500-103,500 and
~289,000-290,500), all from the LockRect failure in the upload loop
(nv2a_pgraph_d3d11.c, the DXT textures at 0186AA80 / 019D2E00 / 01A33E00 /
020D2E00 are the logged ones). The 18 traces of the stage select did not
show which draw drops. Now:
- "[TEXFAIL] frame N: LockRect level L of N failed for <offset> (fmt, slot,
  first upload / re-upload): <size, levels, refcount, locked flag and
  level, staging memory, initialised levels, GPU copy>" at most once a
  second (400 lines), with the count of failures in between
  (d3d8_TextureLockDiag in d3d8_resources.c);
- when the failure is the first level of a re-upload of a texture that is
  already on the GPU, the draw keeps the previous copy instead of drawing
  untextured; the slot stays marked not uploaded, so the next binding
  retries.

## 0.0.175 - training vignette: no HUD scissor

0.0.174 widened the vignette bands (trace: "@VIGNETTE ... -> full width",
boxes 10..6838 of 6828 at 32:9), but the screen still showed them only in
the middle: the bands carry the guest's full-screen window clip, and an
inline draw judged HUD gets its scissor mapped into the HUD canvas
(nv_apply_window_clip(hud)), which cut the widened band back to the centred
16:9 area. The frame measured 0.63-0.67 darker just inside the 16:9 edge
than just outside it over the bottom band. A vignette band now takes the
scissor as a full-screen draw. Its ends are also pinned to the target
edges (the guest band starts at x 0.53, which left a few pixels open on
the left).

## 0.0.176 - HRTF filtering off again (muffled voices)

With 0.0.173's HRTF path the voices sounded muffled / "under water" even on
Beach (Day). The 0.0.175 log's [HRTF] lines show why the filter cannot be
right as decoded: a centred entry (ITD 0, both ears equal) has a per-ear
coefficient sum of -2.88 and an RMS of 2.71 over 31 taps -- the taps
average about +/-0.5 of full scale, nothing like an impulse response whose
energy sits in a few taps -- and off-centre entries reach 3.3. Run as a
FIR (and scaled down by the 10+ abs-sum) that is a heavy comb/low-pass, i.e.
the muffling. The coefficient format (or how the hardware uses it) is not
what xemu's filter assumes.

- The HRTF filter runs only with DOA3_HRTF=1 in the environment; by default
  3D voices mix as before 0.0.173 (centred).
- The [HRTF] lines now also print the first entries' 31 raw taps per ear
  (signed bytes) so the format can be worked out offline.
The [V3D] lines show DirectSound gives a 3D voice equal front-left/right
volumes (bins 6/7) and silences the rear pair (8/9), so the left/right
placement is entirely in the HRTF entry.

## 0.0.177 - the black Zack cutscenes: overlay left the target flag wrong

Puppy (0.0.176) got both of Zack's Story cutscenes black, and changing the
internal resolution during the black screen brought the picture back. His
traces settle it: in every black frame (21528, 27348, 39017, 48995, 62759)
every GPU draw lands in a 256x256 offscreen target ("rt 1 vp(0,0 256x256)")
while the layer reports no offscreen target bound ("off=0"), and the guest
is rendering into its frame buffer (SET_SURFACE_COLOR_OFFSET 011C0000 /
01058000). After the resolution change (49328, 63179) the same scene goes
to the guest frame ("rt 0", full-size viewport) and writes millions of
pixels.

How the layer and D3D11 fell out of step: the frame was presented while an
offscreen target was bound (Zack costume 3's environment-map pass). The
presenter saves the bound targets, draws, and puts them back -- but in
between, the Escape-menu / performance-overlay / training-banner pass
(doa3_ui_render) ended with d3d8_RestoreDefaultTarget(), which clears
g_off_active. The presenter then re-bound the offscreen target, the flag
said "guest frame", and nv_sync_render_target only restores the guest frame
when the flag says an offscreen target is active, so the switch was skipped
from then on. Changing the internal resolution rebuilds and rebinds the
guest target, which is why that cleared it. It needs the overlay drawn
(Puppy runs with the performance overlay), which is why it came and went.

- doa3_ui_render saves the output-merger targets and puts the same ones
  back instead of calling d3d8_RestoreDefaultTarget.
- After the presenter restores the targets it re-derives g_off_active /
  g_off_cur from the view actually bound (d3d8_sync_target_flag), so no
  other path can leave them out of step this way.

## 0.0.178 - per-frame CPU phase split in performance_frames.csv

Puppy's 0.0.177 run: both Zack Story cutscenes render (05307, 11991,
12387, 14784; all draws to the guest frame). Four Ice Cave traces at his
slow spot (17750-22198) have ~900-1000 draws a frame, about 400 of them the
floor reflection drawn again into its 256x256 target. They do not say
where his CPU time goes: the frame capture only had the interval and
present, and [PERF-PHASE] lines are written for frames of 25 ms or more
(his slow frames are ~18-20 ms).

performance_frames.csv (Debug > Automatic diagnostic captures) now has, per
frame: game_ms (guest code between frames outside push-buffer translation),
pb_ms (push-buffer translation, which includes building and submitting the
draws), flush_ms, aux_ms (capture and audio bookkeeping before present),
prev_present_ms (the previous frame's compose + overlay + DXGI Present),
prev_pace_ms (the previous frame's 60 Hz wait) and prev_workers_ms (CRI
worker fibers run inside that wait), draws and kick-offs. The values come
from the counters recomp_manual.c already kept for [PERF-PHASE]
(g_doa3_phase_ms). The first 11 columns are unchanged. The APU mixes on its
own thread outside netplay, so it is not in the host thread's split.

## 0.0.179 - releases named by disc (USA, JAPAN, EUROPE, KOREA)

The XBE identifiers used the competitive balance numbers (3.0/3.1/3.2),
which name patches, not discs: the Korean disc has its own default.xbe but
the 3.2 balance. The runtime now names releases the way Redump names the
discs. Certificates of the four default.xbe files (user-supplied):

| Release | Title ID | Region | Cert version | Built | Size | FNV-1a 64 |
|---|---|---|---|---|---|---|
| USA    | 54430001 | 1 (North America) | 1       | 2001-10-27 | 3928064 | A2F3BDCB2A5B02D4 |
| JAPAN  | 54430001 | 2 (Japan)         | 1       | 2002-02-08 | 4218880 | 8219CF3EC3C08681 |
| EUROPE | 4D53002D | 4 (rest of world) | 1       | 2002-02-15 | 4218880 | 7611143500851317 |
| KOREA  | 54430001 | 2 (Japan)         | 1180166 | 2002-12-03 | 4198400 | 03AA09F2F806C45C |

USA and JAPAN share Tecmo's title ID and certificate version and differ in
the region flag; EUROPE was published by Microsoft under its own title ID;
KOREA is a separate December 2002 build (its .text is 0x1C7F40 bytes
against JAPAN's 0x1CB330 and EUROPE's 0x1CBC70). A region-free copy of the
Korean XBE differs from the original only in the certificate's media and
region fields (region 7, media C00001FF).

- tools/xbe_layout.py keeps its pipeline keys (3.0, 3.1, 3.2, kr, 3pp-1.48;
  overrides_3.1.txt etc. are unchanged) and maps them to a release for the
  generated header: DOA3_XBE_RELEASE / DOA3_XBE_REGION ("JAPAN") and
  DOA3_XBE_ID_JAPAN (USA, EUROPE, KOREA, 3PP_1_48 for the others).
  DOA3_XBE_VERSION and DOA3_XBE_ID_3_x are gone from the runtime.
- xiso_extract.c identifies the disc it is given: the exact JAPAN file is
  accepted; a known other release is named ("This is the USA release of Dead
  or Alive 3. This port runs only the JAPAN release."); a modified or unknown
  file whose title ID is either of the game's or whose title is "Dead or
  Alive 3" is described from its certificate (Korean version number first,
  then Microsoft's title ID, then the region flag); anything else is still
  "not Dead or Alive 3". Previously a European disc was reported as not
  being Dead or Alive 3 at all.
- Setup screen, log line and the online/replay "not ported" messages say
  "JAPAN release".

## 0.0.180 - Proton 10 / Sarek shader compiler fix (from upstream v1.0.6)

parkerallan/doa3-recomp issue #9: black screen with sound under Proton 10 /
Proton Sarek (and Proton CachyOS on the Steam Deck). Wine's vkd3d-based
d3dcompiler rejects two constructs in the port's own HLSL and the renderer
never starts; the errors only show in Wine's output (WINEDEBUG=err+d3dcompiler):
  vs_ffp:    E5005: Function "isfinite" is not defined.
  vs_nv2aff: E5013: Invalid semantic 'SV_ClipDistance'.
This fork has both, in the same places. Ported upstream commit 90776ac:
- vs_ffp: isfinite(w) replaced with w <= FLT_MAX (NaN already fails w > 0).
- vs_nv2aff: the SV_ClipDistance output is inside #ifndef NO_CLIPDIST; if the
  first compile fails, it is compiled once more with NO_CLIPDIST (logged).
  DepthClipEnable already clips the same z < 0 / w < z geometry. On Windows
  the first compile succeeds and nothing changes.
- d3d8_Nv2aFF_Init failing is no longer fatal to CreateDevice: the NV2A
  batches fall back to the CPU vertex path (d3d8_Nv2aFF_Available gates
  nv_gpu_ff_submit).
The fork's NvFF shader has grown since upstream's (lighting, fog, texgen,
spot lights), so only these lines were taken; no other upstream change since
v1.0.1 is merged (they target the USA release's addresses).

## 0.0.181 - always-on 5-second CPU phase summary in the log

Puppy's Ice Cave run on 0.0.178 (traces 02994 and 04371 slow, 05401 fine,
all 32:9) came back without performance_frames.csv or any [PERF-PHASE]
line: diagnostics were not on in that run, so the 0.0.178 split was never
recorded. The traces alone: 943 / 1021 draws in the slow views against 817
in the good one, about 45% of them in every view the floor reflection
drawn again into its 256x256 target (393 / 447 / 341); 94-103k indices
against 78k. The slow views have 15-25% more draws, not a different kind
of work.

So the split no longer depends on a switch:
- the push-buffer translation, CRI worker and frame-end timers always run
  (a few QPC reads per frame);
- every 5 s the log gets "[PERF-5S] frames N (M over 17.5 ms, worst W ms) |
  all: interval, game, pb, flush+aux, present, pace (workers), draws |
  slow: the same averaged over the frames that missed 60 fps", at most 720
  lines (an hour);
- [PERF-PHASE] lines and performance_frames.csv still need diagnostics.

## 0.0.182 - HRTF taps as s.9; hardware placement measured (still opt-in)

Hardware recordings of Ayane's taunt on Beach at five positions (Cmp/
ayane_taunt1_Beach_<pos>_hw.wav), level over the voice, left-right level
difference (broadband and per band 100-300 / 300-700 / 700-1500 /
1500-3000 / 3000-6000 / 6000-12000 Hz) and the inter-channel delay from
the cross-correlation:

| Position | Level | L-R | per band | delay |
|---|---|---|---|---|
| Far left     | -30.7 dBFS | +1.9 | +1.6 +2.0 +1.9 +2.0 +3.9 +4.0 | left leads 6 smp |
| Centre left  | -30.3 | +1.8 | +1.4 +1.9 +1.8 +1.7 +3.4 +2.9 | left leads 4 |
| Centre       | -30.3 | +0.8 | +0.9 +1.2 +0.6 +0.4 +1.3 +0.7 | left leads 1 |
| Centre right | -30.2 | -1.4 | -0.6 -0.5 -2.2 -1.7 -2.9 -2.7 | right leads 5 |
| Far right    | -30.6 | -1.4 | -0.7 -0.5 -2.0 -2.0 -3.4 -3.8 | right leads 6 |

So the console places her with a small head-shadow ILD (larger above
3 kHz) and an interaural delay of up to ~0.13 ms, and the level does not
change with distance across the stage (0.5 dB). The voice spectrum on
hardware against the port's dry voice (0.0.170 recordings, normalised at
0.8-1.6 kHz) has a notch of 7-12 dB at 5-7 kHz in every stage; the logged
centred HRTF entry (ITD 0, both ears equal), read as two's complement
taps, has that notch (its response: flat-ish to 1.5 kHz, -16 dB at
5-7 kHz, back up by 10 kHz). With the taps as int8/128 that entry's gain is
+12 dB; as s.9 (value/512, the same fixed point as the ITD word) it is
about unity, so the taps are now value/512 and used as written (no
per-ear or common rescaling).

- HRTF stays opt-in (DOA3_HRTF=1) until a port recording at the same five
  positions matches the table.
- [HRTF] lines carry the frame, and the raw taps are printed once for
  every distinct entry (up to 400) instead of only the first 24.

## 0.0.183 - HRTF entries read live (the taunt was always centred)

Port recordings of the five Beach positions with DOA3_HRTF=1 (made on
0.0.181, so the tap scale differs from 0.0.182 by a common factor only):
left-right difference 0.0 to +0.3 dB in every band and no inter-channel
delay at any position, against the hardware's +-1.4 to +-1.9 dB (up to
+-4 dB above 3 kHz) and +-4 to 6 samples. The voice spectrum did pick up
the hardware's 5-7 kHz notch (hw-port no longer shows it), so the filter
ran -- with the centred entry (ITD 0, identical ears) every time.

The [V3D] lines show why: DirectSound double-buffers each 3D voice's HRTF
entry (voice 1 alternates entries 2 and 3, voice 2 4 and 5, ...), and every
even entry logged at SET_VOICE_TAR_HRTF time holds the centred filter. The
VP copied an entry's coefficients into the voice's filter only when that
method arrived (xemu does the same); the hardware filters from the entry
memory. If DirectSound fills the entry after pointing the voice at it, the
copy is the previous contents.

- 3D voices now take their filter from the entry live, every frame (the
  per-sample parameter smoothing covers the change);
- "[HRTF] frame N vV entry E changed after it was targeted: itd ..." (first
  60) shows whether that is what happens.

## 0.0.184 - HRTF ears by mixbin, not by slot

0.0.183's port recordings (Beach, five positions, DOA3_HRTF=1) were still
exactly centred: left-right within 0.1 dB in every band, no delay, while
the log shows the taunt's voices on positioned entries (ITD -5 to +29
samples, ears up to 10 dB apart). No "changed after it was targeted" line
appeared, so the double-buffered entries were not the problem (the live
read stays; it is what the hardware does).

The mix was: a 3D voice's slots 0-3 are the HRTF submixes, and DOA3 sets
them (SET_HRTF_SUBMIXES, [V3D] lines) to mixbins 6, 8, 7, 9 -- front left,
rear left, front right, rear right. 0.0.173 picked the ear by slot parity,
so slots 0 and 2, the two front channels, both got the LEFT ear: the
output was mono, and for a voice on the right it was the far, darkened ear
-- the "under water" sound of 0.0.173. The ear now follows the mixbin:
even (6, 8) left, odd (7, 9) right, the same pairing apu_dsp.c's mixdown
uses.

## 0.0.185 - HRTF decoding parked; per-draw timing always on

0.0.184 port recordings: the inter-channel delay now matches hardware
(port -4/-4/-2/+4/+4 samples against hardware -6/-4/-1/+5/+6), so the ITD
word is read right. Matching each port recording to the HRTF entry the game
sent (about 1 dB rms) and checking those entries against the hardware
recordings, the ear levels are not: the low bands come out 2-5 dB the wrong
way and the upper bands swing -8..+4 dB where hardware rises smoothly from
~2 to ~4 dB toward the near ear, and the tone is 5-9 dB darker above
1.6 kHz. Two's complement, sign-magnitude, offset-128, ones' complement,
reversed taps and swapped ears all miss (best: sign-magnitude, 3.5 dB rms
ILD error). The 31 values per ear are not a plain FIR in the format xemu
assumes; the next step is DirectSound's routine that builds them (parked).
HRTF stays opt-in (DOA3_HRTF=1).

Performance (Puppy, Ice Cave): the per-draw phase timing of the GPU path
(fetch / state / constants / combiners / d3d-draw / cleanup, the
5-second [DRAWTIME] line) now runs without the diagnostics switch, like
0.0.181's [PERF-5S], so one ordinary log shows where both the frame and
the draws spend their time.

## 0.0.186 - transform programs run a batch at a time

Puppy's 0.0.185 log, Ice Cave slow angle: the pushbuffer phase is ~11 ms
of a ~19 ms frame, but the ~830 GPU-path draws account for only ~3.5 ms
of it. The rest of the draws are 39 array draws per frame (~8,700
vertices) of the reflective ice floor, whose transform program -- 25
instructions, projecting the mirrored-scene render target onto the floor
-- ran on the CPU interpreter one vertex at a time, re-extracting ~35 bit
fields per instruction per vertex.

- The program is decoded once (rebuilt when an instruction is written or
  the entry point moves).
- An array draw that takes the program path now runs the program for all
  of the batch's distinct vertices up front (nv2a_vp_execute_batch): one
  instruction across 64 vertices at a time, so the operation, operand
  source, swizzle and write-mask branches are taken once per instruction
  per chunk and the per-vertex loops are straight-line.
- Only the input registers the program reads are fetched (v0, v2, v9 for
  the floor instead of all 16).

Replaying the program uploads and draws of Puppy's Ice Cave trace
(mtrace_manual_04371) against the old interpreter: every output identical
over 8,696 vertices of random inputs (with zeros, denormals, infinities and
NaNs mixed in, the only difference is the sign bit of a NaN made from two
NaNs), and 2.4-2.6x faster.

New 5-second [DRAWCPU] line: ms per frame in GPU-path array draws,
CPU-built array draws (with the program prepass and the vertex build
separately) and inline draws, so the next log shows how much of the
pushbuffer time is draws at all and how much is method handling.

## 0.0.187 - a profiler that can find the rest

Puppy's 0.0.186 log. Ice Cave, slow angle: pushbuffer 11.0 -> 8.4-8.9 ms,
frame interval ~19 -> 16.7-17.3 ms (about 58-60 fps). The floor's
program prepass is ~1.1 ms for ~4,350 distinct vertices.

The other stage he flagged runs ~40-44 fps for a different reason:
~3,050 GPU-path draws a frame (three times Ice Cave's), interval 22.7 ms =
game 11.4 + pushbuffer 11.1 (draws 7.0, method handling ~3.8). Nothing
in 0.0.186 changes that path, and the draw timers cost well under 1 ms at
that count. Neither the game's 11 ms nor the method handling is broken
down by the timers we have.

DOA3_PROFILE=1 (DOA3_PROFILE.bat) now runs a real sampling profiler on the
thread that finishes frames, for the whole session: ~1,000 samples a
second, each with up to 16 frames of call stack from the x64 unwind data,
reported every 10 s as [PROF] self (the function the thread was in) and
total (functions on the stack) lines, with RVAs that resolve against
DOA3.map from the same build. It replaces the old fixed 45 s + 90 s,
256-byte-bucket sampler. Opt-in, because it stops the thread a thousand
times a second.

## 0.0.188 - the profiler's first targets

Puppy's 0.0.187 [PROF] log on the ~3,100-draw stage (~44 fps), resolved
against DOA3.map, share of the frame thread's samples:

- 13.5% pgraph_d3d11_method itself: per-method dispatch, called for every
  dword of the pushbuffer -- in the Ice Cave trace 30% of all methods are
  light registers (64 a draw) and 33% inline vertex data.
- 9.1% + 4.3% sub_001E1C70 and sub_00190064: the XDK's SSE 4x4 matrix
  multiply (two copies), run tens of thousands of times a frame.
- 6.0% sub_001E1F50: the XDK's x87 matrix inverse (not changed here).
- 4.6% + 2.2% nv_gpu_ff_submit, and ~14% inside system DLLs (D3D11 and
  the driver).

Changes:
- xmm_load (every movaps/movups/mulps-from-memory in the recompiled code)
  resolves its address once and reads the 16 bytes as one copy instead of
  four volatile dword reads; stores stay volatile. Same values, so the
  results are unchanged; the matrix multiply is 2.6x faster in a
  standalone test.
- The pushbuffer walker hands each packet to pgraph_d3d11_method_run,
  which applies light registers, the model-view and composite matrices
  (with their transform-constant mirror) and inline vertex data directly
  -- the same stores, shadow register, counters and return value as the
  general dispatch -- and sends every other method through
  pgraph_d3d11_method as before. Method tracing still goes the general way.

## 0.0.189 - method fast path learns; profiler gets addresses

Puppy, 0.0.188: Ice Cave holds 59-60 fps. The ~3,150-draw stage went from
22.7 to 19.95 ms a frame (~50 fps): game 11.4 -> 9.6 ms, pushbuffer
11.1 -> 10.2 ms. In its profile the matrix multiply fell from 13.4% to
about 6%; still on top are the general method dispatch (7.3%, plus 3.5%
in the new run loop), the x87 matrix inverse sub_001E1F50 (6.2%),
nv_gpu_ff_submit (~7%) and the driver (~17%).

- A method with no flag-driven handling that reaches the switch default is
  marked the first time; pgraph_d3d11_method_run then applies it directly
  (shadow register and counters, same return value). The default is the
  last label and no case falls into it, so a method that reaches it once
  always does.
- [PROF] hot lines: the 40 hottest exact addresses (8-byte buckets) in the
  exe, so a hot function can be taken apart instruction by instruction
  against the same build's DOA3.exe (the Release PDB has public symbols
  only, no line tables).

## 0.0.190 - guest registers in locals (build-time pass)

Puppy, 0.0.189: no single hot spot is left in our own code; the frame on
the ~3,100-draw stage (~19.8 ms) is about half recompiled game code
(~9.5 ms) and half our pushbuffer/draw handling (~10 ms).

The game half pays a tax on nearly every instruction: in the generated
code eax..edi, esp and the x87 stack are globals, and every guest memory
access is a volatile access through a computed pointer, which the compiler
must assume can alias any global -- so the register file is reloaded from
memory after almost every access and stored after every change.

tools/recomp/localize_regs.py (run by CMake at build time, Python 3,
option DOA3_LOCAL_REGS, on by default) writes a copy of each generated
chunk into the build tree in which a function's body works on locals:

- loaded from the globals on entry; around every direct call,
  RECOMP_ICALL_SAFE and RECOMP_ITAIL, the registers the body writes are
  stored and every register it uses is reloaded; stored again at every
  return. Callees see exactly the globals they saw before, so the calling
  convention (implicit parameters in registers included) is unchanged.
- the x87 stack is copied rotated, so the body's stack indices are
  constants and the slots can live in host registers; the physical slots
  and g_fp_top are restored exactly.
- only functions whose bodies use nothing but the known-pure helpers plus
  calls are rewritten, and only where there are at least 12 statements per
  call (fewer and the synchronisation costs more than it saves): 6,034 of
  11,025 functions, about 64% of the generated statements.
- in the whole copy, imul's (int32_t)a * (int32_t)b is written as the
  unsigned product and idiv's edx:eax is assembled unsigned: the same bits,
  without the signed overflow C leaves undefined (x86 wraps). With the
  operands in locals clang was seen exploiting that in sub_0018C288.

Without Python the original chunks are built (CMake warns). The log's
second line says which: "guest registers in locals" or "in globals".

Checked here: a standalone test of the XDK matrix routines (identical
results; inverse 1.8x, multiply 1.1-1.3x faster); every rewritten function
without calls (3,027) run against its original from 4 random starting
states each, and every rewritten function with calls (6,664 -- all
eligible, regardless of the threshold) from 3 each with every callee
replaced by one stand-in that reads and changes all registers, the x87
stack and memory. Built without optimisation, original and rewritten
agreed in every case; with optimisation, the handful that differed also
differ between the original's own optimised and unoptimised builds
(undefined behaviour on random inputs), apart from the imul case above,
which is fixed. All 16 rewritten chunks pass a syntax check.

## 0.0.191 - one map per GPU draw

Puppy, 0.0.190 (registers in locals): the ~3,100-draw stage went from
19.8 to 18.75 ms a frame (~53 fps); the game half 9.5 -> 8.7 ms, the
pushbuffer half unchanged at ~9.9 ms. Ice Cave holds 60. Matt played the
whole story mode as Hayate on it without a problem.

The draw path was already de-duplicated (shader, layout, blend, depth,
raster and both constant buffers are only re-sent when they change; ring
buffer at a fixed binding), and in the Ice Cave trace 719 of 909 GPU draws
differ in state from the one before, so merging draws would not pay. What
every draw still paid was two buffer maps: the vertex/index ring and the
VS constant buffer (880 bytes, nearly always new -- each object has its
own matrices).

- The draw's Nv2aFFConstants now go into the same ring, in the same Map,
  on a 64-byte boundary (or not at all when they equal the previous
  draw's and the ring has not been discarded since). The VS variant
  compiled with NV_RAWCB reads them through a raw view of the ring at
  VS t0, at the slot given by an instance-rate CBIDX attribute: IA slot 1
  holds an immutable identity buffer, and DrawIndexedInstanced's
  StartInstanceLocation is the slot. Same shader code, same values.
- Falls back to the constant buffer per draw if the variant does not
  compile or the ring cannot carry a raw view; the log says which
  ("nv2aff VS: constants from the vertex ring" / "[NV2AFF] draw
  constants ...").
- [PROF] module lines: the samples outside the exe by module (the driver,
  d3d11, ntdll, ...), so the next profile shows where that ~17% goes.

## 0.0.192 - fewer clock reads; who calls out of the exe

Puppy, 0.0.191: the ring-constants VS compiled and runs ("constants from
the vertex ring"); the ~3,100-draw stage went from 18.75 to 18.32 ms
(~54.6 fps). The new module lines split the ~18% of samples outside the
exe: ntdll 6.6%, VCRUNTIME140 (memcpy/memset/memcmp) 6.1%, d3d11 2.0%,
the NVIDIA driver only 1.6%, ucrtbase 1.4%. So the driver is not where
the time goes.

- ntdll is where QueryPerformanceCounter lands (RtlQueryPerformanceCounter):
  the per-draw [DRAWTIME] phases read it seven times per GPU draw and the
  [DRAWCPU] split twice per draw -- about 27,000 reads a frame here. Both
  now time one draw in 16 (DRAWCPU scales its times by 16); the averages
  they report are the same, and the draw counts stay exact.
- [PROF] calls-out lines: for samples outside the exe, the exe function
  that made the call, so the memcpy/memset share and the rest of ntdll
  can be traced to their callers.

## 0.0.194 - GPU vertex cache removed

0.0.193's GPU vertex cache (reverted here) worked as designed -- on the
~3,100-draw stage about 2,650 draws a frame were drawn from vertices kept
on the GPU, 75 a frame went the per-draw way because their data kept
changing, and Puppy saw no visual problems -- but the frame did not get
faster (18.4 ms at ~3,075 draws). The per-draw vertex phase went from
0.57 to 0.66 us: the cost was never the conversion but reading the
game's vertex data, and deciding whether it changed means reading it
all again. The upload saving (d3d-draw 0.90 -> 0.83 us) did not pay for
the extra code, so it is gone again; 0.0.191's one-map-per-draw and
0.0.192's sampled timers stay.

## 0.0.195 - asynchronous GPU thread (opt-in: DOA3_ASYNC_GPU=1)

The frame on the ~3,100-draw stage is ~8.6 ms of recompiled game code
followed by ~9.6 ms translating its pushbuffer, one after the other on one
thread. The console's GPU ran in parallel with the CPU, and the game only
reuses memory the GPU has finished with (DMA_GET for the ring, the fence
semaphore for everything else), so the translation can run on a thread of
its own.

With DOA3_ASYNC_GPU=1 (DOA3_ASYNC.bat):
- KickOff queues each span (with the wrap split as before) instead of
  translating it, and no longer marks it consumed itself. A render thread
  translates the queue in order and only then writes DMA_PUT/GET and the
  fence semaphore (every fence inserted up to that kick), as the GPU did.
- The game thread finds the frame end (FLIP_STALL) with a header-only walk
  of the span (data runs are skipped by their count).
- At a frame end the render thread flushes pgraph, keeps the
  previous-frame copy the guest samples, captures/traces as before, and
  copies the finished frame into one of two buffers. The game thread, at
  its next frame end, waits until the render thread has finished the
  previous frame (so it is never more than one frame ahead), presents that
  copy with the overlay, then paces and runs the frame callback as before.
  One frame more latency than before, like the console's GPU queue.
- One lock (d3d8_Lock) serialises the immediate context between the two
  threads; every window, overlay and present call stays on the game
  thread, which pumps messages while it waits.
- Off before the first movie hand-over, while the host movie presenter owns
  the screen and in netplay; switching drains the queue first. [ASYNC]
  lines: when it takes over, and every 5 s the render thread's busy time
  per frame.

## 0.0.196 - the asynchronous GPU thread actually switches on

Puppy's 0.0.195 run with DOA3_ASYNC_GPU=1: "[ASYNC] ... on", but every
5-second line said "inactive" and the frames were the synchronous ones
(18.5 ms). The switch waited for g_doa3_post_movie, which only the USA
build's movie presenter sets. It now takes over after the first 600
frames (boot stays synchronous) whenever no movie owns the screen and
netplay is off.

## 0.0.197 - asynchronous GPU thread on by default

Puppy, 0.0.196 with DOA3_ASYNC_GPU=1: the render thread took over at
frame 621 and the ~3,050-draw stage held 16.67 ms (60 fps) with 3.5-5.5
ms of pacing slack every 5 s (game thread up to ~12.8 ms including its
waits, render thread busy 10.5-13.4 ms), on a CPU capped at 60-80% --
synchronous, the same stage needed ~88% to hold 60. No hangs, a clean
exit; Matt saw no new problems either.

Now the default; DOA3_ASYNC_GPU=0 goes back to translating at kick-off on
the game thread. Netplay keeps the synchronous path (it switches back
while a session is active), untested as netplay does not work yet.

## 0.1.0 - milestone: full speed

No code change from 0.0.197; the version marks the milestone. From here
the update number counts from 0 again under minor version 1.

On 0.0.197 (asynchronous GPU thread on by default) Puppy finished Time
Attack and Matt finished Story Mode with no new problems; the render
thread handed back to the synchronous path for the movies and took over
again after them as designed. Where tonight's work started, Puppy's
laptop (Ryzen 7 5800H, RTX 3060) ran the Ice Cave at ~50 fps and the
~3,100-draw X Octagon at ~44 fps; it now holds 60 on both with the CPU
capped at 60%. The steps:

- 0.0.186  transform programs decoded once and run a batch at a time
- 0.0.188  SSE loads in recompiled code as one copy; fast path for the
           light, matrix and inline-data methods
- 0.0.190  guest registers in locals (build-time pass)
- 0.0.191  draw constants in the vertex ring, one map per draw
- 0.0.192  per-draw timers sample one draw in 16
- 0.0.195-0.0.197  the pushbuffer translated on a render thread, in
           parallel with the game, the way the console's GPU ran

Known graphical issues predate this work and are unchanged.

## 0.1.1 - overlay history around the REPLAY -> WINNER cross-fade

Matt: at the REPLAY -> WINNER transition the REPLAY lettering loses its
fill (only the outline is left) for 1-2 frames on hardware but 1-7
frames on the port. The automatic [FBFX] captures already land on that
transition -- its cross-fade is a full-screen quad (alpha F6) sampling
surface 03C24000, which the game fills in a render pass two frames
earlier ([RTT] "pass into surface 03C24000 ... 0 GPU draws") -- but each
is one trace of the first cross-fade frame, after the frames in
question.

Every 2D draw (no depth test) of the last 32 frames is now kept in
memory: box, colour, both stage textures with format/control/palette,
enabled stages, blend, render target, combiner program and a fingerprint
of stage 0's first 4 KB. When [FBFX] fires, overlay_NNNNN.txt in
Diagnostics gets them, oldest first, so the frames where the fill goes
missing can be compared draw by draw with the ones before.

## 0.1.2 - overlay history: whole texture and palette fingerprints

Matt's 0.1.1 run (5 REPLAY -> WINNER transitions, skipped with START):
the 30 frames before each cross-fade draw exactly the same six REPLAY
letter quads every frame -- texture 03B5A400 (512x256, 8-bit palettised,
palette 03B7A400), same state, same first-4 KB fingerprint, the colour
surface alternating 01058000/011C0000. The frame before the fade copies
the previous frame (011C0000) into 03C24000 and draws it back with the
letters on top; the fade frame draws it at alpha 7F under WINNER. So the
lost fill is not a missing or different draw: it has to be in the bytes
-- the letters' palette (the earlier blink is likely palette animation)
or the texture beyond its first 4 KB.

The history now fingerprints stage 0's whole level 0 and its whole
palette (once per texture per frame), and counts the palette's fully
transparent and fully opaque entries.

## 0.1.3 - REPLAY -> WINNER: the fill drop was the capture

Matt's hardware recordings (two Story runs, 12 skipped replays): the
REPLAY letters lose their fill for exactly one frame every time -- the
cross-fade's first frame, the one where the copied previous frame is
drawn back one pixel off (the top/left margin). The port draws the same
single frame. It stayed up for 1-7 refreshes because the automatic
frame-buffer-effect capture ([FBFX]: a full trace plus a 19 MB screenshot)
fired on the next frame: performance_frames.csv shows 73-91 ms there and
~17 ms around it. With automatic captures off Matt sees one frame, as on
hardware.

The [FBFX] captures are removed: they were added for Omega's after-image,
fixed long ago, and fired at every match end. 0.1.1/0.1.2's overlay
history (which hashed every 2D texture and palette each frame) is
removed with them; it did its job.

## 0.1.4 - menu header bar no longer widened at 21:9 / 32:9

Since 0.0.174 a black bar ran across the full top of the screen on mode,
character and stage select at 21:9 and 32:9. These screens draw a
solid black band (uniform alpha, guest lines ~1-44) behind their header.
It matched the training-vignette test (untextured, blended, black,
full guest width, under 3/4 of the height), so it was stretched to the
full target width. The vignette is a gradient: alpha 0x99 at the edge
and 0 inward. The test now also requires the vertex alpha to vary, so
the menu band stays in the centred 16:9 HUD canvas as before. The
@VIGNETTE trace line now prints the alpha range.

## 0.1.5 - Dip Switches (General tab); left stick navigates menus

The General tab has a new Dip Switches section for quality-of-life
toggles (fixes for the original game's limitations and bugs). Every
switch is off by default, which gives the original behaviour. They are
saved in [DipSwitches] in doa3_settings.ini (src/game/dip_switches.c;
adding a switch takes one table row and one enum entry).

First switch: "Left stick navigates menus" (LeftStickMenus).
- Menus test only the directional-pad bits of the per-frame pad
  aggregate (0x73C8D0 + pad*0x2C: held +0, pressed +8, auto-repeat
  +0x10). The builder 0xA4E70 derives those bits from wButtons.
- The fight reads the sticks separately (+0x1C, with its own 0x5000
  threshold and hysteresis, 0x1EF20).
- With the switch on, XInputGetState (0x211A96) folds the left stick
  into wButtons as directional-pad bits. The game's own pressed and
  auto-repeat logic then handles the stick exactly like the pad.
- The stick is read in 8-way sectors. An axis switches on at 0x5000
  and off below 0x3000.
- The stick is not folded during a fight (phase 0x596A37 = 1 or 2)
  unless the pause menu is up (0x5A27A0 = 1), or in netplay. Fights
  are unchanged.
- The first 12 changes are logged as [DIP] with phase, pause and mode,
  to confirm the gating.

## 0.1.6 - left-stick menus: fight detection from the fight's stick reader

Matt: 0.1.5's switch worked on the in-fight pause menus but not on
any menu outside a battle (mode select, options, character, costume
and stage select).

The [DIP] log showed every menu press as "phase 1 ... fight, not
folded". The phase byte 0x596A37 is 1 on the menus as well, so it
can't tell a fight from a menu.

"In a fight" now means the fight's own stick reader ran within the
last 3 frames. That reader is 0x1EF20, called per player each logic
frame; it is now an override that stamps the frame, and its esp-probe
wrapper is removed from recomp_probes.c. The pause-menu exception and
the netplay exclusion are unchanged. The [DIP] line now also shows how
many frames ago the reader ran, and up to 24 changes are logged.

## 0.1.7 - Input device list shows only connected controllers

Controls > Input device listed all four Xbox and all four PS4 slots
whether or not a pad was plugged in. It now lists Automatic, None,
Keyboard and only the connected controllers (connection is polled
every 250 ms for connected slots and every second for empty ones). The
device currently assigned to the player stays listed even when it is
unplugged, marked "(disconnected)", so the assignment can still be
seen and changed. "No controllers connected" shows when there are none.

## 0.1.8 - DualSense (PS5) controller support

The native PlayStation path (src/input/ds4_device.c, DirectInput) now
also accepts the DualSense (054C:0CE6) and the DualSense Edge
(054C:0DF2), in the same four slots as the DualShock 4. Through
DirectInput the DualSense reports the DS4's layout:
- buttons: Square Cross Circle Triangle L1 R1 L2 R2 Create Options L3 R3
- d-pad: the hat
- sticks: X/Y left, Z/Rz right
- triggers: Rx/Ry
So the existing translation and its tests cover it unchanged.

Rumble (USB): the rumble opener picks the report by the HID output
report length.
- DS4: 32 bytes, report 0x05, motors at bytes 4 and 5.
- DualSense: 48 bytes, report 0x02, valid flags 0x03 (compatible
  vibration + haptics select), right/left motors at bytes 3 and 4, and
  0x04 at byte 39 (improved rumble emulation on firmware 2.24+).
Bluetooth rumble is still unsupported for both (different report, CRC).

The Controls tab and the [INPUT] log name the pad "PS5 controller N"
or "PS4 controller N" by model. ds4_mapping_test checks the DualSense
report.

## 0.1.9 - per-model controller numbering; depth-func diagnostic

Input device list: no "(connected)" suffix (only connected pads are
listed). Numbers count per model in slot order: Xbox controller 1,
PS4 controller 1, PS5 controller 1, PS5 controller 2. An unplugged pad
that is still the player's choice shows as "<model> (disconnected)".
The "Using ..." line uses the same name.

Matt reports the 3D rendering broken on 0.1.8 during the Nine Lives
demonstration (FMVs fine): characters partly see-through, earlier shots
showing through later ones. His four traces of it (frames 14729-15028)
have SET_DEPTH_FUNC = 0x0CBCDBEB on every draw. That is not a GL compare
(0x200..0x207); the same run's boot traces show 0x203. The D3D path maps
an out-of-range value to LESSEQUAL, so the value alone draws nothing
wrong, but it means something sent a bad word. Nothing in 0.1.4..0.1.8
touches the push-buffer path. pgraph now keeps the last 48 methods and,
when an out-of-range depth func arrives, logs them as [BADZFUNC] with
the frame and thread (first 6 times). A data word read as a method
points at the push-buffer walk; a sane run, at the guest. To tell an
asynchronous-GPU problem from anything else, run with
DOA3_ASYNC_GPU=0 (DOA3_SYNC.bat).

## 0.1.10 - Beach ocean flicker: texture cache ignored the mip count

Matt: after long sessions the Beach's ocean flickers on stage select;
it is gone after a fresh boot. Logs of it show [TEXFAIL] "LockRect level
1 of 5 failed for 020D2E00 ... 128x128 levels 1".

The texture cache key is address, size, format and palette, without the
level count. Heap addresses are recycled between loads, so eventually
a texture declared with 5 levels lands where a same-size, same-format
1-level texture was cached. The hit served the 1-level copy, and every
binding's upload failed on level 1 and drew with no texture.

A hit whose level count differs now evicts the stale copy and creates
the texture anew, logged as [TEXCACHE] ... replaced (first 40).

Matt saw it only with the performance overlay on. That is likely about
what lands where in a long session rather than the overlay itself.

## 0.1.11 - stage-select camera loop: memory snapshots

Matt (0.1.9, around frame 129000, about 36 min in, after three Story
runs and two or three Nine Lives viewings): the stage-select preview
camera stopped at the end of its path instead of looping. Re-entering
the preview animated it again. He took five traces frozen
(129035-129457) and five animating (129676-129951).

The vertex constants confirm it. Every frozen trace has the same view
matrix (c96 onward), 420 frames apart, while the animating traces
change it each time. So the camera position itself stopped advancing;
it is not a stale picture. The preview's state is rebuilt on entry,
which is why a refresh animates. What fails is the step that should
loop the path back to its start.

The game's clocks don't explain it:
- KeTickCount runs from 0 at boot.
- The XAPI QPC (rdtsc, 733 MHz) is boot-relative.
- The frame-time meter (0xA65B0/0xA6680) works on small deltas.

The code alone does not show which variable fails, so Debug >
Diagnostics has a new "Save a game memory snapshot" button. It writes
guest RAM (64 MB) to Snapshots\ram_<frame>.bin, a folder kept between
launches (Diagnostics is cleared at each launch). Two snapshots frozen
and two animating separate the camera-path state from everything else:
the frozen view matrix is known from the traces, so the camera struct
can be found by value and the loop test read from its code.

## 0.1.12 - Training: controller-driven dummy; Stagger Escape hold

Dummy control. "Controller controls the dummy" plus a port choice
(Player 1-4 as in Controls). During the input window (0x11980 ..
0x12B60), the window the recorder already uses, the dummy is marked
human (0x59CF58 + side*0x68 = 1) and given that port (0x30E3F4 + side).
The game then decodes its commands from that port's own pad aggregate
and bindings, and both bytes are restored when the window closes. On
the first frame of control the CPU's pending command history is
cleared, as recording does.

The dummy stays on the CPU whenever any of these holds:
- the port's pad is not in the game's open-device mask (0x73C8C8),
  i.e. it is unplugged
- the port is the human's own
- a recording or playback is running
- the overlay is open
- the fight is not in a recordable state (same test as recording)

Stagger Escape. The value is the float array 0x4DA2D4[side]; 0x4DA2DD
is the side being processed and 0x4DA1C4 that fighter. The stun
handlers (0x14D70 for the human, 0x16840 for the CPU's random mash,
0x1BAE0) do the following each frame:
- mash: add 0.03 (0.0432 when 0xC59867 is set), clamp at 2.00
- no input: subtract 0.01 (0.0144), floor at 1.00
- copy the value into the fighter's animation speed (+0x10)
- reset it to 1.00 out of the stun

The Training tab shows both live values and has a Hold toggle and a
1.00-2.00 slider per fighter. A held value is written at the start of
each input window, before the fighters update, so the game still
applies that frame's mash or decay and does the speed copy itself; the
port never writes the animation speed.

## 0.1.13 - Stagger Escape shown as a percentage

The Training tab shows Stagger Escape as 0-100 % (game value 1.00-2.00:
(v - 1) * 100), for the live readout and the hold slider. Mash +3 %,
decay -1 % per idle frame. The game's value is unchanged.

## 0.1.14 - dummy control and Stagger Escape hold with the menu open / CPU dummy

Matt (0.1.12/0.1.13):
- A controller drove the dummy only while the overlay was closed.
- A held Stagger Escape took effect only when the menu was closed AND
  a player drove the dummy; a CPU dummy ignored it.

Overlay gate. Dummy control copied the recorder's overlay check, but
only the recorder needs it (it reads the player's own pad). Control
now ignores whether the overlay is open.

CPU dummy. The CPU stun handler 0x16840 first resets 0x4DA2D4[side] to
1.00 when the fighter's +0x59 is 1 (a CPU fighter), then applies its
random mash or decay and copies the result to the fighter's animation
speed (+0x10) on every path. The value seeded at the input window was
therefore lost. Opening the overlay handed the dummy back to the CPU,
which explains the first condition too.

0x16840 is now an override (its esp-probe wrapper removed):
- Before the call it notes the base the handler starts from (1.00 on a
  reset, otherwise the slot).
- After the call, for a held side, it writes the held value plus that
  frame's change (clamped to 1.00-2.00) to both the slot and +0x10.

Player-driven fighters keep the input-window seed, since their handler
(0x14D70) does not reset.

## 0.1.15 - Dip switch: Random includes alternate costume colours

Player record (0x59CF58 + side*0x68): +1 character, +2 costume ID (Matt).

How a costume ID is chosen:
- 0xCD870(al = character, dl = costume slot, [esp+4] = alternate) maps
  slot and alternate to the costume ID. Slots below
  min(unlocked 0x59FB16[0x2FF868[char]], 0x29C70C[char]) index
  0x29C728[char][sum of earlier slots' variant counts + alternate].
  The variant counts are 0x29C790[char][slot]; slots past the unlocked
  count are 0x40+ extras.
- Character select takes the alternate from the held buttons (0xCD110:
  X/Y, with LT for the upper ones), limited only by the variant count;
  there is no unlock test per alternate.
- The random picker 0xCC980 handles the player's Random (character
  0xFF) and the CPU's random opponents (modes 2, 3, 4, 5, 9, 10 via
  0x5A28F0). It picks a character and a slot from the random pools
  0x404990/0x404998, then calls 0xCD870 with alternate 0, every time.
- Scripted opponents come from 0xCC760, which passes their own
  alternate.

With DIP_RANDOM_ALT_COSTUMES on, 0xCC980 and 0xCD870 are overrides
(their esp-probe wrappers removed). While the picker runs, an alternate
of 0 for a base slot with more than one variant is replaced by
(next byte of the 0x404998 pool + slot*7 + char*3) mod variants. That
is deterministic: no host randomness, so replays and netplay agree.
Logged as [RANDALT] (first 16).

## 0.1.16 - costume diagnostics (random alternates for CPU opponents)

Matt (0.1.15): the player's Random now gets alternate colours, but
Survival's CPU opponents never wear one, though they do get costumes
other than the first. His log has [RANDALT] at each new Survival
opponent, three of them alternate 1, so a later step replaces the CPU's
costume. A candidate is 0x76B30(char), a CPU-opponent costume function:
- default: slot 0, alternate 0
- mode 2: the Story table 0x5B1454
- modes 3/5: compares with the player's costume through 0xCD900, the
  Story easter eggs (Hayate/Hayabusa costume 3; Ayane C3 -> Kasumi's
  hair-down alternates); it must not be touched

The fix waits for evidence. [COSTUME] lines now log every slot +
alternate -> costume mapping (first 200: character, slot, alternate,
result, whether the random picker made it, mode bytes 0x5A2858/0x596A33)
and, from the input hook, each change of the fighters' character and
costume (first 100).

## 0.1.17 - random costumes and colours for Survival / Time Attack opponents

The 0.1.16 log of Matt's four tag-Survival fights shows the random
picker running only for his Random team. Each CPU opponent's costume
comes from 0x76B30(character [esp+4]) -> costume, which by game mode
(0x5A2858) does:
- mode 2, Story: the Story table 0x5B1454[fight], which carries the
  Fight 3/6 outfit easter eggs
- modes 3 and 5 (Time Attack, Survival; his log is mode 5): costume 1,
  or costume 2 against the same character in costume 1 (mirror rule,
  via 0xCD900)
- otherwise: costume 1

The tables confirm slot = costume and alternate = colour, e.g. Leifang
(0x0C) has 4 costumes and the first has 4 colours.

0x76B30 is now an override (its esp-probe wrapper removed). With
DIP_RANDOM_ALT_COSTUMES on and the mode 3 or 5, it replaces the result
with a random unlocked costume and a random colour of it, mapped the
way 0xCD870 does:
- A pick equal to a human player's character and costume is re-rolled
  (up to 4 tries; otherwise the game's choice stands).
- The answer per character is kept for 120 frames, because the game
  asks several times while setting up one fight.
- Random bytes: the game's pool 0x404998 stirred by a call counter.
- Logged as "[RANDALT] CPU opponent ..." (first 32).
Story (mode 2) and every other mode are untouched. The [COSTUME]
diagnostics from 0.1.16 stay in for now.

## 0.1.18 - Dip switch: Random includes costumes from storage

3.0 could load extra costumes from TDATA next to the save (the Booster
Pack outfits; fans use it for custom XPR/CAT pairs). Character Select
offers them, but Random never picks them. In the costume mapper
0xCD870, a slot past the unlocked count
min(0x59FB16[0x2FF868[c]], 0x29C70C[c]) becomes 0x40 + n for the n-th
storage costume, if n < 0x5B3174[0x2FF868[c]] (the per-group storage
count Character Select fills). The random picker 0xCC980 draws its slot
from the unlocked count only.

DIP_RANDOM_HDD_COSTUMES (RandomStorageCostumes):
- Player's Random and other random-picker draws: while 0xCC980 runs,
  the slot given to 0xCD870 is redrawn over unlocked + storage. A
  storage pick has no colour options.
- Survival / Time Attack opponents (0x76B30, needs
  DIP_RANDOM_ALT_COSTUMES too): the random slot is drawn over the same
  range.
Random bytes come from the game's pool 0x404998. Logged as [RANDHDD]
(first 16).

## 0.1.19 - Dip switch: costume list scrolls past nine (Stage 1)

Character Select shows at most nine costume buttons.
- Count: 0xCD7C0(character) = unlocked disc costumes
  (min(0x59FB16[0x2FF868[c]], 0x29C70C[c])) + min(storage
  0x5B3174[group], 9 - 0x29C70C[c]).
- Cursor: 0xCCB80 wraps within that count.
- Draw: 0xD18A0 gets the four players' cursor block in edx; each
  player's record is base + 0x10 + p*0x38 (+0x13 active, +0x34 == 1,
  +0x16 grid index, selected slot at record + (int8)index, character =
  0x29CA60[0x9B76B0[index]*10]). Per costume it draws a box 20 units
  apart, highlighted when the row equals the slot, then through 0x6DF00
  "COS." (descriptor 0x29BC04, sprite 0x2E5, 37 px, x 45) and one
  digit (descriptor 0x29BC0C, sprite 0x2E6 + (row+1) % 10, 10 px,
  x 82). Digits 0-9 are sprites 0x2E6-0x2EF (sprite table 0x3049B8,
  12 bytes each).

DIP_COSTUME_CAROUSEL (CostumeCarousel). Three overrides (esp-probe
wrappers removed):
- 0xCD7C0 returns unlocked + all storage costumes, so the cursor
  reaches every one.
- 0xD18A0 keeps a scroll window per player that follows the selected
  slot (window = slot when above it, slot - 8 when below), writes the
  slot relative to the window for the draw only and restores it after.
  During the draw the count is capped at nine; the player order of its
  0xCD7C0 calls tells the hooks whose row is being drawn.
- 0x6DF00 relabels the rows: window + row + 1. From 10 up, "COS."
  moves 3 px left and two digit sprites are drawn (tens 3 px left of
  the usual place, units 10 px after).
The buttons never move; only labels and the costume behind them change
(Matt's design). Storage is still limited to 7 per character (scan
table 0x5B1E58, 7 entries per group); lifting that is Stage 2.

## 0.1.20 - storage costumes: 64 per character, 2048 files (Stage 2)

The TDATA scan runs once at boot (0x87420 -> 0x86AD0/0x86B20).
- Per character group (19; prefixes at 0x302C58: zac tin jan kte ryu
  kas gen hik bay bas lei aya, sam nin kor bni sui gok), it lists
  T:\<prefix>??.* and copies each name (16 bytes) into 0x5B2140. The
  counter 0x5B212C is capped at 0x100 files in total; that is the
  "100".
- 0x86C20 reads the two characters after the prefix as hex ("%2X",
  kas14 = 0x14; 256 numbers per character) and files .xpr/.cat under
  that number in a 7-entry working list 0x5B3148.
- 0x86CC0 (unrolled seven times) copies complete pairs into the pair
  table 0x5B1E58 (19 x 7 x {cat|0x8000, xpr|0x8000}) and the count
  into 0x5B3174[group].
- A storage costume is 0x40 + n: 0x840D0 and 0x86060 read
  pairs[group*7 + n], and 0x85C40 reads the name.
So: 7 per character and 256 files.

Now, at the start of the scan (doa3_tdata_alloc), the name table,
working list and pair table move to guest heap allocations sized for
2048 files and 64 costumes per character (costumes 0x40-0x7F: clear of
0xFF and the high bit). The scan and the three readers use variables
in place of the fixed addresses, the 7 and the 0x100 (hand edits in
gen/recomp_0002.c, marked "DOA3 0.1.20"). The variables start at the
original addresses and limits, which also remain if the allocation
fails. 0x86CC0 is now an override: the same step as a loop. Logged as
[TDATA] (capacity, then the count for each group that has storage
costumes).

With the carousel switch off, Character Select still shows at most
nine buttons. The costumes past that are reachable with "Costume list
scrolls past nine" or picked by "Random includes costumes from
storage".

## 0.1.21 - storage costumes: at most 99 per character

Matt: cap a single character at 99 storage costumes (was 64 in 0.1.20).
Storage costumes are now 0x40-0xA2. Nothing reads the costume byte as
signed: the uses are equality tests, and the pair-table readers
(0x840D0, 0x86060) test it with an unsigned < 0x40. The working list
takes the first 99 distinct numbers a character's files use; the
2048-file total is unchanged.

## 0.1.22 - costume list ends at COS.99

Matt: labels never go past two digits. With the carousel switch the
costume count (0xCD7C0) is capped at 99, so Down on COS.99 wraps to
COS.1 (the cursor's own wrap) even when more storage costumes exist.
Random from storage, for both the player and Survival/Time Attack
opponents, draws from the same first 99, so it never picks a costume
the list cannot reach.

## 0.1.23 - storage costume options (aya01.y.xpr)

A storage file named <prefix><two hex digits>.<suffix>.<xpr|cat> is an
extra option for that disc costume (number counted from 0, so aya01 is
Costume 2).

Suffix -> button -> option (the game's own option numbers in 0xCD110,
where A / Start is 0):
- x -> X -> 1
- y -> Y -> 2
- l -> LT+A -> 3
- lx -> LT+X -> 4
- ly -> LT+Y -> 5

Confirm-button word 0x9B7C0E + port*8, high byte: 1 A, 4 X, 8 Y; LT
held = 0x4000 in the pad aggregate. A costume's disc options are
0x29C790[c][slot] (Leifang's first costume: 4 = A X Y LT+A; Kasumi's:
3).

The original game takes the text after the first dot as the extension
(0x86EF0: strstr "."), so "y.xpr" is neither xpr nor cat and these
files are ignored on hardware. (They do use up one of the character's 7
storage numbers there unless a plain file already uses that number.)

Port (always on; nothing changes without such files):
- 0x86C20 (scan, one file): option files go to a per-character option
  table (up to 64 in total), not to the numbered working list.
- 0x86CC0: complete option pairs are appended to the pair table after
  the numbered costumes. Each gets a storage costume id 0x40 + n but
  no COS.n entry (0x5B3174 still counts only the numbered ones).
  Logged as "[TDATA] group g: costume c option o from storage".
- 0xCD110: when the confirm button means an option the disc costume
  lacks and a storage file provides it, that option is returned. A
  button the disc costume already uses is never taken over.
- 0xCD870: (costume, option) maps to the storage id.
- 0xCD900: maps that id back to (costume, option).

## 0.1.24 - storage options: x, y, l only

Matt: LT+A is the game's only LT option, so the lx/ly suffixes are
dropped. Suffixes are now x (X, option 1), y (Y, 2) and l (LT+A, 3).
In 0xCD110, X or Y with LT held means X or Y.

## 0.1.25 - storage "l" option: L held with A, X or Y

Matt: Leifang's first costume has four options, A, X, Y and L (held
while pressing A, X or Y). That matches 0xCD110, which for a costume
with 4 options returns 3 for L + A, X or Y. A storage "l" option is
now taken with L held and any of the three; previously only L+A. With
no storage L option the game's own choice stands, so L+X on a two-option
costume still gives X.

Neither Black nor White confirms a costume in 3.1: Character Select's
confirm test (0xCB760) accepts 0xD10 = Start, A, X, Y. The game's
button bits are A 0x100, B 0x200, X 0x400, Y 0x800, Black 0x1000,
White 0x2000 (analog-button table 0x2D4C98). So there is no Black or
White suffix.

## 0.1.26 - Random can pick storage costume options

With both "Random includes alternate costume colours" and "Random
includes costumes from storage" on, a random pick for a disc costume
chooses among its disc options and its storage options (x/y/l files on
buttons the disc costume leaves free). This covers the player's Random
(0xCD870 during 0xCC980) and Survival/Time Attack opponents (0x76B30).
A storage option resolves directly to its costume id. [RANDALT] lines
now give the option, the disc and total counts, and "-> storage
option" when one is chosen.

## 0.1.27 - storage options keyed by the disc costume's file number

Matt (0.1.26): aya00.y / aya00.l / aya02.x / aya04.y were found and
recorded, but the option did not appear on the costume he expected.
The scan had read the file number as a costume position (aya00 ->
Costume 1). He means the disc costume's own file number, which is its
costume id: disc costume ids (0x29C728, grouped by option counts
0x29C790) are the aya<NN> numbers.

Ayane (character 0x0D):
- COS.1 = 02
- COS.2 = 00, and 01 for X
- COS.3 = 03, and 04 for X

So aya00.y is COS.2's Y, aya00.l COS.2's L, aya02.x COS.1's X and
aya04.y COS.3's Y. (His list had 01 and 02 swapped; his files match
the table.)

An option file now attaches to the costume slot whose disc ids include
its number. That is resolved per character, so the API takes the
character (doa3_opt_costume / doa3_opt_lookup). The [TDATA] option line
names the file number.

## 0.1.28 - Dip switch: same costume, different option

With both sides on the same character, Character Select will not let a
player highlight a costume the other side has. Cursor-move check
0xCCC70(al = slot, edi = cursor record):
- an opposing player's cursor record active on that slot and the same
  character (the 0x9B79D0 records, 0x38 apart, side = +0x1A bit 0);
- then 0xCCD40, the opposing side's confirmed picks: slot
  0x9B7C00[side*5+i] (0xFF = none), character 0x588838[..], costume
  id 0x5A26F8[..].
The cursor skips a blocked costume. Confirming (0xCC760) writes the
costume id to record +0x15 and the side's pick lists; +0x34 = 2 when
the player's selection is complete.

DIP_SHARED_COSTUMES (SharedCostumes, off by default):
- 0xCCC70 is an override. When the game blocks, the costume is still
  allowed if it has two or more options (disc options plus storage
  options) and at least one is not confirmed by the other side. A
  one-option costume keeps the original block.
- The confirm test in 0xCB760 (buttons 0xD10) calls
  doa3_costume_confirm_mask (hand edit in gen/recomp_0003.c). It works
  out the costume id the press would give (0xCD110's button rules plus
  storage options) and returns 0, ignoring the press, when the other
  side confirmed that exact costume + option on that character.
  Otherwise 0xD10 as before.

## 0.1.29 - same costume, different option: the right side's picks

Matt (0.1.28): P2 could confirm the very costume + option P1 had
confirmed. The duplicate test read the other side's pick list using the
record's +0x1A bit, but 0xCC760 files picks at side*5 + member with
side = 0xCCEF0(record) & 1, which depends on the mode. The test was
looking at the wrong side's list. Both the highlight rule and the
confirm test now call the game's 0xCCEF0 (guest registers saved and
restored around it). A refused confirm is logged as [SHARED] (first
16). The only other confirm caller, 0xCB370, handles a Random
character pick, not the costume press.

## 0.1.30 - costume list past nine is always on

Matt: the costume carousel (0.1.19) need not be a switch. A player of
the original sees no difference, since it only shows costumes that
could not be reached before. DIP_COSTUME_CAROUSEL and its table entry
are removed, and the 0xCD7C0 / 0xD18A0 / 0x6DF00 hooks are
unconditional. A CostumeCarousel= line left in doa3_settings.ini is
ignored.

## 0.1.31 - Random is default stage option; label renames

New dip switch RandomStageDefault (second in the list): Stage Select
opens on Random instead of Beach (Day).
- Cursor byte 0x9B7BAA indexes the order list 0x9B76C4 (count
  0x29CA5A); each entry is a row of the 16-byte stage table 0x29C960.
  Init 0xD8470 builds the list, appends 0x0E (Random) at the end, and
  stores its stack argument as the cursor. Found by diffing Matt's
  three snapshots: 2 (Beach Day), 1, then 14 (Random).
- All callers of 0xD8470 pass the constant 2 except 0xCADAC, which
  passes the remembered stage at state +0x1EA. Hand edits in
  gen/recomp_0003.c: the three copies of loc_000D84BD pass the
  argument through doa3_stage_initial_cursor, and the 0xCADAC caller
  sets g_stage_cursor_keep so a remembered stage is left alone.
- With the switch on and the argument 2, the cursor is set to the
  list position holding 0x0E.

Labels: SharedCostumes is now "Allow players to select alternate
options of the same costume" and LeftStickMenus is "Menu navigation
with Left Stick". The RandomAltCostumes help no longer mentions
Black (3.1 confirms with A, X, Y or L).

## 0.1.32 - Random without Ein; a sound for a refused costume press

Matt: Random only appears on Character Select once Ein is unlocked.
- 0xCD9E0 builds the grid. With Random allowed (state +0x1D8; 0 on
  Story's select) and Ein unlocked (0x5A0B47) it lists all 18 cells of
  0x29CA60 (10 bytes per cell, byte 0 = character: 3 Ein, 0xFF Random)
  and sets the count 0x29CB14 to 0x12. Otherwise it lists 16, leaving
  out Ein and Random.
- The select code already supports a locked cell, character 0xFE:
  0xCF8E0 draws it with sprite 0x313 (texture 7 of the chr_sel sprite
  file, the 60x48 lock; the same texture as sprite#0D's eighth),
  0xCB5F9 refuses it with sound 0x29, and Random (0xCCA3F) skips it.
- With Random allowed and Ein locked, doa3_ein_grid_gate (hand edit at
  0xCD9E6) sets Ein's cell (0x29CAB0) to 0xFE and takes the 18-cell
  path. Every other path puts the 3 back (doa3_ein_grid_restore at
  0xCDA12), so Story and an unlocked Ein are as before. Not a switch.

The same-costume switch now plays sound 0x29 when it refuses a confirm
press (Start, A, X or Y pressed this frame), the sound the game plays
for a press on a locked slot.

## 0.1.33 - dip switch: Correct offset of menu layout; README

Matt: the menu frame leaves a strip of the scene along the left and top
edges (hidden by CRT overscan in 2001). Cause: every 2D drawer of the
sprite system (0x6E6C0, 0x6EAD0, 0x6F9B0, 0x71730, 0xCF660 and their
entry copies) adds the float constant 0x251244 = 0.53125 to its
positions, and the pre-transformed passthrough program adds the
screen-space offset 0.53125 again, so the layer lands at about +1.06
pixels: an old trace has the menu header band at 1.1,1.1-721.1,154.1.
New switch MenuOffset (last in the list): the frame hook sub_00011980
sets the constant to -0.53125 while it is on (total offset 0) and puts
0.53125 back when off. Every 2D element moves together, so the HUD
shifts by the same pixel.

README: dip switch table, always-on changes (costume list past nine,
storage limits, Random before Ein), storage costume and option file
naming, Training dummy control and Stagger Escape, DualSense, the HDD\D
and TDATA folders, Snapshots. DEBUG.md: memory snapshot button.

## 0.1.34 - menu offset switch applies from boot

Matt (0.1.33): with MenuOffset saved on, the Press Start fade, Main Menu,
Character Select and Stage Select kept the gap until the first fight.
sub_00011980 is the fight loop's frame hook and does not run on menus.
doa3_menu_offset_apply is now also called at every frame end
(d3d4134_frame_done), so the constant is set from the first frame.

## 0.1.35 - movie display option defaults to 16:9

Matt: with no save the movie option starts at 4:3. Checked: the default
is the XBE's own .data byte 0x3F456C = 0 (not the console's widescreen
flag), read by the new-save init sub_000204B0 and the Options Default
buttons (0xC2590, 0xC2786, 0xC35FA, 0xC37AA); it also picks the movie
scale 0x2D4CE8 (0.75 for 4:3, 1.0 for 16:9). video_sync_guest_widescreen
(startup and frame boundaries) now writes 1 there. A saved setting is
untouched. README notes it under Changes to the game.

## 0.1.36 - README: storage costume details removed

Matt: keep the storage costume limits and the option file naming out of
the README. Removed the Features bullet, the TDATA row in the files
table, the "Storage costume limits" bullet and the Storage costumes
section; the costume list bullet no longer names COS.99. The Dip
Switches table is unchanged. "Changes to the game" moved out of the
Settings section to sit before Building.

## 0.2.0 - milestone version

Release of the 0.1.x work: DualSense support and per-model controller
numbering, the Training tab's dummy control and Stagger Escape, the Dip
Switches (left stick in menus, Random as the default stage, Random with
alternate and storage costumes, shared costumes, menu layout offset),
the costume list past nine, Random before Ein is unlocked, 16:9 as the
movie default, rendering fixes (menu header at ultrawide, Beach ocean
flicker), and the updated README. No code change from 0.1.36.

## 0.2.1 - storage random switch works on its own; README wording

Matt: "Random includes costumes from storage" should not need the colour
switch. Before, Survival/Time Attack opponents (0x76B30 hook) only ran
with the colour switch on, and storage options (x/y/l files) only joined
a random pick through the colour switch's option draw. Now:
- random_option_set: option 0, disc options 1..v-1 with the colour
  switch, storage options with the storage switch.
- The Random option draw in 0xCD870 and the opponent hook in 0x76B30 run
  when either switch is on. Storage only: disc costumes in their first
  colour plus storage costumes and options; colour only: disc costumes
  and colours; both: everything.
Help text and README updated. README: costume-list bullet removed; the
movie default bullet reworded as Matt wrote it.

## 0.2.2 - Stagger Escape hold for the CPU-run Sparring dummy

Matt: the hold only worked with a second controller on the dummy. The
0.1.14 hook wraps the CPU stun handler 0x16840, but the Sparring dummy
under the CPU runs through the scripted-input player 0x14D70 (script
0x4DA5BC): it resets the slot 0x4DA2D4[side] to 1.00 at a stun's first
frame (0x1642D, +8 == 0) and, on later frames (0x16449), adds 0.03 when
the script mashes or takes 0.01, clamps, and copies the slot to the
fighter's +0x10. Hand edits in gen/recomp_0000.c call
training_stagger_script_before at 0x16449 and _after before both copies
to +0x10: the held value replaces the slot with that frame's change on
top. Logged as [STAGGER] (first 8).

## 0.2.3 - Stagger Escape hold: the player stun handler too

Matt (0.2.2): still no effect on the CPU-run dummy, and the log had no
[STAGGER] line, so the scripted path is not the one it takes. The player
stun handler 0x1BAE0 has the same reset as 0x16840: while the fighter's
+0x59 is 1 it sets the slot to 1.00 before mashing or decaying, which
throws the seeded hold away every frame for a fighter flagged that way.
0x1BAE0 is now an override (probe removed, overrides list) wrapped with
the same before/after as 0x16840. Both log as [STAGGER] stun handler
(first 12, with +0x59), so the next log shows which path the dummy takes.

## 0.2.4 - Stagger Escape diagnostic

Matt (0.2.3): still no effect, and the log (version 0.2.3, Sparring at
frame 1418) has no [STAGGER] line at all, so with the hold on neither
wrapped handler ran for the dummy. [STAGGER-TRACE] now logs every call
of the three stun routines (0x16840 CPU, 0x1BAE0 player, 0x16449 script),
first 40 each, hold or not: side, human flag, +0x59, slot before/after,
animation speed, hold state, training_available, mode.

## 0.2.5 - Stagger Escape hold for the Sparring dummy (0x1817E0)

The 0.2.4 trace: with a second controller on the dummy, 0x1BAE0 runs
(human 1) and the hold works; with the CPU on it, no stun routine runs at
all. In Sparring (mode 7/11) the fighter loop (0x12991) sends a fighter
that is not human to 0x1817E0, not to the CPU code 0x12EC0, so nothing
mashes or decays its value and its stun plays at the speed it started
with. After each of the three 0x1817E0 calls (hand edits in
gen/recomp_0000.c) training_stagger_dummy puts the held value in the slot
and in the record's animation speed +0x10 while +0x33 == 8 (the stun
state 0x14D70 tests), and puts a speed still equal to it back to 1.00
when the stun ends. [STAGGER-TRACE] dummy lines log its state changes
while a hold is set.

## 0.2.6 - Stagger Escape clean-up; American spelling

Matt: 0.2.5 works. Removed what turned out not to be the dummy's path:
the 0.2.2 scripted-input hooks (0x16449 hand edits and functions), the
0.2.3 0x1BAE0 override (probe and overrides list restored; the frame-start
seed already covers a human fighter), and the 0.2.4 [STAGGER-TRACE] /
[STAGGER] stun handler logs. Kept: the 0x1817E0 dummy hold (with its one
[STAGGER] Sparring dummy line) and the 0.1.14 0x16840 CPU wrap.

The colour switch is now "Random includes alternate costume colors from
disc"; user-facing text (dip switch help, README, DEBUG.md) uses American
spelling. The ini key RandomAltCostumes is unchanged.

## 0.2.7 - Random stage in Tag Battle; replay / winner pose on the final round only

Random stage in Tag Battle: Matt reported that "Random is default stage
option" did not apply on Tag Battle's Stage Select. That screen is always
entered from the caller that passes the remembered stage (0xCADAC, after
the four-fighter loop), and the hook kept a remembered stage. Until a
stage has been chosen in the mode, though, that value is only the default
the mode setup writes (0xD65A0 sets select state +0x1EA to 2, clamped to
the list). 0xD65A0 is now an override that marks "no Stage Select since
mode setup", and the first Stage Select after it opens on Random whichever
caller it comes from. Later ones keep the stage that was played. The
`[STAGE]` log lines (first 16 only) show the mode, the remembered flag,
the fresh flag, and the cursor before and after.

New dip switches, both off by default:
- `ReplayFinalRoundOnly`, "Only show round's replay on final round".
  When the KO phase ends (0x73F5D), a round that does not decide the
  match takes the game's own no-replay path (B3950, phase 4 at 0x73F70),
  the one Team Battle, Survival and draws already use.
- `WinnerFinalRoundOnly`, "Only show round's winner on final round".
  Phase 4's pose loop (0x75311..0x75584) counts ebp frames down and exits
  to 0x7558A early when 0x7DC00 sees a skip press. A round that does not
  decide the match now exits there before the loop's first frame.
"Decides the match": a side's rounds won (side record +0x60, raised at
the KO in 0x737F5..0x73857) has reached the rounds to win (0x588837).
Both hooks are hand edits in gen/recomp_0001.c (three copies each) and
log a `[ROUND]` line with the score and the decision.
