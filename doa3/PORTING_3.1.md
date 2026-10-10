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
