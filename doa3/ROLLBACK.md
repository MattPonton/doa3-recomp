# Rollback implementation status

This experimental branch has a tested rollback event driver and opt-in local
fight rewind/replay, not playable online rollback DOA3. As of October 23, 2026,
live combat diagnostics have passed one-, two-, four- and eight-frame windows
with matching captured state across all ten checkpoint domains and the
renderer. These are bounded local samples, not proof of continuous rollback
or determinism across computers, stages and the full roster.

The intermediate host-rendering preparation skip passed the recorded
four/eight-frame comparisons. More aggressive guest model-render skips remain
experimental: the latest narrowed vertex-deformation skip matches the GPU
pixels and logical renderer state but still fails RAM/command-buffer and GPU
frontend checks. It stops safely and restores the original endpoint. The
latest revision builds in Release and passes 40 targeted CTest entries; its
live mismatch and timings are documented at the end of this file. Do not
interpret the rejected optimization timings as validated rollback performance.
The Online carousel remains a menu prototype. No new network listener, router
mapping, background connection or automatic diagnostic capture is enabled.

## Implemented

- `src/online/rollback.c` consumes GekkoNet Save/Load/Advance events synchronously,
  in order. It saves the state after frame N (initial state is frame -1), restores
  the frame preceding a correction, then replays through the current frame.
- Two 18-byte `np_pad` inputs per frame, host first. The config enables a bounded
  prediction window (1–16 frames), full state storage and desync checks. It does
  not enable runahead or change the 60 Hz simulation contract.
- Snapshots have a versioned, frame-tagged envelope, length checks and a local
  integrity hash. Gekko receives a separate canonical game-state checksum, so
  local pointer addresses need not cause false peer desyncs.
- Callback failures, corrupt snapshots, invalid ordering, an excessive rewind,
  incomplete replay, peer disconnects and detected desyncs stop the driver. A
  failed session cannot silently resume with partially restored state.
- Replay steps are explicitly marked for the game adapter. Presentation is
  called once after a completed batch, never for each intermediate replay step.
  The eventual game adapter must suppress/reconcile audio, rumble and other
  external effects; the event driver alone does not intercept those systems.
- `xbox_det_capture` / `xbox_det_restore` checkpoint the actual deterministic
  clock module: frame count, polling boosts, per-fiber kernel-call counters,
  pending worker laps, slice-due flag and exported tick. Checkpoints are local
  to an active session/epoch. Restoration does not wake workers or rewind host
  wall time, host offsets, or the IO diagnostic counter.

## Validation

Build and run the standalone suite (no game assets or external network needed):

```powershell
cmake -S doa3/tests/rollback -B doa3/build/tests/rollback -A x64
cmake --build doa3/build/tests/rollback --config Release --parallel 4
ctest --test-dir doa3/build/tests/rollback -C Release --output-on-failure
```

The tests use the production driver and the bundled, unmodified GekkoNet with a
small deterministic simulation fixture. They do **not** establish determinism
of the actual DOA3 simulation.

- Stress mode: 1,100 forward frames and 7,637 replayed frames, comparing every
  completed frame with an offline reference. Omitting RNG restoration on purpose
  causes Gekko's desync detection to fail the session.
- Two independent Gekko sessions with an in-memory transport: 2–6 delivery ticks
  of delay, packet loss, duplication, reordering and a 40-tick outage. Both peers
  recover and match the offline reference after 1,100 frames, with zero and two
  frames of local input delay. The eight-frame prediction limit is respected.
- Snapshot header/payload corruption and truncation are rejected before the
  simulation's load callback. Invalid input sizes, frame ordering, unsupported
  runahead/portable snapshots and callback failures are rejected.
- Fixture replay steps produce no repeated audio events; intermediate replay
  frames are not presented. These validate the callback contract, not DOA3's
  eventual sound/render integration.
- The production clock is advanced, restored and replayed; all checkpoint bytes
  match, per-fiber time/counters match, and stale-session restores are refused.

## Integration contract

1. Establish a compatible build, match configuration and initial state with the
   peer **before** creating the match session. Both sides must use the same
   prediction window and snapshot schema. A local snapshot is not a portable
   peer bootstrap or a spectator save.
2. Initialize `np_rollback` with the complete simulation callbacks. Use
   `np_rollback_config` to create GekkoGameSession; register host as actor 0 and
   joiner as actor 1. Supply the existing transport adapter at that point.
3. At the fixed 60 Hz simulation boundary, sample and submit local input once
   for the next forward frame. If Gekko stalls, retain that input; do not submit
   a different input under the same frame number. Network polling must continue.
4. Call `gekko_update_session`; process its session events first, then its entire
   game-event batch through the driver before another update. Never keep Gekko
   Save pointers for completion on a later game frame (the old lockstep provider
   does exactly that and cannot simply have its prediction window increased).
5. A callback must not recursively update the session. `advance` must return
   after precisely one complete game step, without polling hardware or pacing.
   `save` must cover every state item read by later steps; `load` must validate
   its payload before modifying anything. On failure, stop and recreate from a
   known safe match boundary, not from the failed partially advanced state.
6. Keep presentation and effect reconciliation separate from simulation replay.
   Forward frames can also contain predicted inputs; marking replay steps does
   not by itself solve effects that differ after a prediction is corrected.

## DOA3 3.1 work still required

### Opt-in fight-task continuations

`DOA3_ROLLBACK_TASKS=1` selects explicit continuations for fourteen 3.1 task
bodies at `87420`, `72070`, `54110`, `BE4A0`, `11110`, `B4720`, `A00D0`, `98DB0`, `7F470`,
`235B0`, `1C60AB`, `E8030`, `691F0`, and `BB500`, in dispatcher order. The generator copies
their current translated instructions and saves every declared root-scope C
local across their frame yields. `87420` and `72070` use the nested call stack described
below; the other twelve use single-yield root continuations. Unsupported declarations or yield shapes
fail generation. Other XBE versions return unsupported.

The game's fibers all enter through the shared `1C6690` wrapper. Manual
overrides therefore select the experimental path at the actual body entry,
preserving the wrapper and guest stack. The original bodies remain the default.
The first live build incorrectly selected by the shared fiber entry address;
its normal-play check did **not** validate the explicit continuations. Confirm
fourteen `[ROLLBACK] task ... uses an explicit continuation` messages and advancing
`g_rollback_task_steps` counters before testing this path. `tools/rollback_probe.py`
reads those counters and the guest task table without modifying the process.

The corrected live build on 2026-10-16 logged all five task entries in Practice;
a two-second sample advanced each 120 times with no nested-yield warning. This
confirms execution of the experimental path, not actual-fight restore/replay.

`xbox_fiber_task_capture/restore` checkpoint only these continuations and their
existing saved guest registers/x87 stack, at yielded boundaries on the primary
fiber. They reject task creation/deletion/reuse and unexpected nested scheduler
handoffs. These process-local checkpoints do not copy guest RAM/stacks, rewind
unconverted tasks or workers, or restore audio, renderer, heap and kernel state.
They must not be used alone as a complete game snapshot or peer digest.

Tests compare a generated continuation with an original Windows-fiber fixture,
then rewind/replay it. A separate test uses the production scheduler and a
shared-entry wrapper, restores two task continuations, and rejects stale or
nested-yield checkpoints. These are component tests, not real-fight replay tests.

### Actual-task capture and isolated replay checker

`DOA3_ROLLBACK_CAPTURE=<absolute .rbc path>` together with
`DOA3_ROLLBACK_TASKS=1` arms a **bounded diagnostic**, only in solo Practice
(mode 7, active fight phase 1) with training recording/playback idle. It records
84 task pairs, rotating through the fourteen explicit continuations, then closes
the file and stops. No screenshots or live rewinds occur. Recording is off
unless explicitly armed and may cause diagnostic hitches.

Each before/after sample contains the complete 128 MB base guest mapping as
4 KB page deltas, all recompiler register/flag/string/x87 globals and the current
task's explicit locals/resume point. A SHA-256 source fingerprint prevents the
offline checker from using another translation. These samples are **not** a
complete runnable whole-game checkpoint: they omit other task continuations,
host heap metadata, kernel objects, full audio execution and rendering state.
The opt-in v3 command snapshot described below adds part of the APU state.

The first live capture exposed asynchronous writes to `KeTickCount` and APU
voice cursors/notifiers. The controlled-capture revision temporarily uses the
existing deterministic clock/APU APIs around each sampled task, releasing them
after the sample. It refuses to take over an existing deterministic session.
This isolates task execution for diagnosis; it is not a substitute for restoring
or reconciling audio during real rollback.

Build the standalone checker:

```powershell
cmake -S doa3/tests/rollback_shadow -B doa3/build/rollback-shadow -A x64
cmake --build doa3/build/rollback-shadow --config Release --parallel 4
ctest --test-dir doa3/build/rollback-shadow -C Release --output-on-failure
doa3/build/rollback-shadow/Release/rollback_shadow.exe doa3/build/rollback-practice.rbc
```

The checker links no live game, device-output or networking runtime. It executes generated guest code in a
separate memory buffer, restores its pre-task image and state, then executes it
again. It compares both runs byte-for-byte and compares their result with the
captured live result. No guest-memory regions are masked out to manufacture a
pass. Unsupported indirect calls, host overrides, guest clocks, out-of-bounds
accesses and execution-budget exhaustion report `BLOCKED`, not success. Missing,
out-of-order, truncated or mismatched-source captures are rejected.

Observer-only combat hooks use their unchanged guest bodies; the recorder must
be idle. The CRT memmove override has its exact cdecl ABI reproduced with bounded
replay-buffer pointers. The checker omits only unused `_fsN` bookkeeping reads
inside helpers, whose destination is never consumed; the lifter can emit these
after overwriting the address register. Task-root locals are retained verbatim.
This normalization does not exempt any live guest memory from comparison.

Controlled live Practice validation (2026-10-16): all 30 samples matched the
captured live result, six per task, with zero blocked or divergent samples.
Each replay also matched its repeated execution after restoring the pre-task
image. Comparison covered all 128 MB of guest RAM, captured registers/x87 state
and the sampled task's explicit continuation; no memory regions were ignored.
The captured animation callback `0x9DCC0` was audited and added to the isolated
checker: it computes a scalar interpolation using only guest memory and x87
registers. The checker format/error-path tests also passed.
Local evidence: `build/rollback-practice-controlled.rbc` and
`build/rollback-controlled-report.log` (diagnostic artifacts, not source assets).

A passing task sample establishes local task replay under captured conditions.
It does not establish a complete 60 Hz fight step, multiframe rollback, a
portable canonical checksum, or peer determinism. Those remain integration work.

### Consecutive task-group checkpoint checker

The v2 task capture records presentation counters at every boundary. Setting
`DOA3_ROLLBACK_CAPTURE_CONTIGUOUS=1` alongside the normal task-capture variables
records six consecutive cycles after the initial Practice warmup. It refuses
unexpected task order, interrupted eligibility or a skipped presentation instead
of silently joining samples from separate intervals.

```powershell
doa3/build/rollback-shadow/Release/rollback_shadow.exe doa3/build/rollback-practice-chain.rbc --native-camera --chain
```

The chain checker carries the seven tasks' own registers, x87 stacks and explicit
continuations forward instead of replacing them with each recorded pre-state.
It retains the replay RAM between tasks and compares the entire result with each
captured post-state. Midway through a successful forward pass, it checkpoints
this isolated model; it then restores that checkpoint and repeats the remaining
steps, comparing every post-state and the final image/task states again.

**Scope:** work from unconverted tasks, the dispatcher and asynchronous writers
is supplied as a recorded external-write journal between samples. Shared flag
and string globals/x87 control also come from the recorded boundary; the native
scheduler does not save those per fiber. The checker reports the external byte
count and refuses carried-task state gaps. It does not simulate those external
writes, establish whole-frame determinism, restore live game/host state, or
validate a complete 60 Hz fight loop. Its independent fixture tests exercise a
42-step chain, a checkpoint after step 21 and 21 restored steps; omitted task
state, skipped frame counters, corrupt outputs and incomplete files are rejected.
Live consecutive capture completed on 2026-10-16, but its chain validation is
**not passed**. The initial verifier build matched 41/42 individual updates,
with pair 18, task `98DB0`, blocked at DirectSound/APU MMIO `FE820010`. Adding
guest-block tracing and recompiling exposed a second issue: that verifier
matched 36/42 updates, blocked pair 18, and found four differing stack bytes in
each of the other five `98DB0` updates. That chain stopped at pair 4, before its
midpoint checkpoint, so there is no successful live rewind result. Evidence:
`build/rollback-chain-audited-report.log` and
`build/rollback-chain-audited-individual.log`.

The stack difference is at `0515FF74`: a temporary address watch traced the
last access to `17C600`, a matrix-transform helper whose translated prologue
declares `uint32_t ebp;` and pushes that uninitialized C local before assigning
`ebp = esp`. The old compiler output happened to match the live stack bytes;
the additional tracing changes the value written. Repeated execution within
one build still matches, demonstrating why that check alone was insufficient.
The watch has been removed; block-site diagnostics remain. No bytes are masked,
zeroed, or replaced from the expected result. The translated frame-pointer
model therefore needed a defined initialization/inheritance contract in both
the game and checker. See the correction below; a new live capture is required.
The prior passing samples did not establish compiler-independent determinism.

The checker now reproduces these audited host-bridge paths in isolated memory:

- Critical-section initialize/enter/leave (ordinals 291/277/294): the current
  production implementations do not mutate the section. Preserve the bridge's
  EAX result and stdcall stack cleanup; do not run live worker scheduling.
- IRQL raise/lower/DPC level (160/161/129): reproduce the fastcall/no-argument
  ABI and KPCR byte `0x24`. All production IRQL setters mirror their host value
  there; this representation is part of the restored guest image. This relies
  on the current `kernel_hal.c` mirroring contract, not generic Xbox behavior.
- Buffer page lock/unlock (175): the current 3.1 kernel table explicitly has
  no bridge and consumes 12 argument bytes, returning EAX zero without touching
  the buffer. The isolated adapter reproduces that existing behavior; this
  does not model physical page locking on an Xbox. Physical-address query
  (173) returns its guest address argument unchanged, exactly as the production
  identity-mapping bridge does. ABI tests cover null, ordinary and high-bit
  addresses, preserved memory/registers, and truncated argument stacks.
- Voice-retirement wait `1F5157`: reproduce only its inactive path, retaining
  guest registers and returning without a wait. Active retirement is rejected
  because it requires interrupt delivery and APU advancement.

Thunk ordinals come from the generated XBE layout, with alignment and bounds
checks. Other kernel calls remain unsupported. Tests check memory/register
preservation, stack cleanup, IRQL changes, invalid thunks, out-of-bounds stacks,
and rejection of active retirement and uncaptured APU accesses. Block reports
include the last translated guest block address. These adapters must be
re-audited if their production kernel/manual implementations change. The new
fingerprint includes the kernel implementations and APU command/snapshot sources,
using newline-normalized bytes rather than platform-dependent text decoding.

### Defined frame locals and APU command snapshots

The translator now initializes each local EBP from `g_seh_ebp`, the existing
guest frame bridge, before its prologue can save it. The same mechanical change
is applied to the current generated 3.1 sources (5,665 declarations). Existing
explicit FPO inheritance and SEH readbacks remain in place. This defines the
current local-EBP translation contract; it does not replace it with a complete
architectural EBP register model. The continuation generator accepts this
specific initializer and still saves/restores task-owned locals. The real
`17C600` helper is compiled with both `/Od` and `/O2` in a regression test:
three different frame seeds, matrix results, stack cleanup and all 128 MB of
guest RAM agree. `tools/recomp/init_frame_pointer.py` reapplies the correction
to an existing generated tree; future translation emits it directly.

Set `DOA3_ROLLBACK_CAPTURE_APU=1` together with task capture to include an APU
command snapshot at every boundary. This originally introduced version 3,
whose header word 6 is the combined task-state plus APU-payload size. Current
v5 also permits a USER queue payload, described below. Each record stores the
task state, optional full APU payload, optional queue, then guest page deltas. Capture remains bounded
and explicitly armed, and takes the APU image only while deterministic clock
ownership is held. There is no live audio restore.

`src/apu/apu_rollback.c` exports registers, voice locks/SSL parameters, filter and
pitch history, HRTF parameters, pending IRQ and deterministic-clock fields.
The snapshot contains no host pointers, locks, threads or audio-device handles.
Filter output pointers become validated tags and are rebound to the private
destination on restore. The current shim resampler is stateless; its pointer
is reconstructed locally while the actual pitch history is retained. This is
a versioned local build format, not a network packet or general savestate.

The checker imports that state into a private APU instance and runs the actual
production VP command backend and MMIO instruction decoder against isolated
RAM. It opens no audio output or worker thread. It supports VP commands,
register reads and the captured sample-clock value. Main control-register
writes, interrupt servicing, chip advancement and active retirement remain
blocked. Unsupported effects must be implemented and verified, not silently
skipped. Software-mixer output, GP/EP execution and host output reconciliation
remain outside this command-only snapshot.

APU bytes are included in every comparison, the between-task external journal,
and the midpoint checkpoint/restore. They are not replaced with expected
post-state data during replay. Tests cover pointer rebinding, voice start/stop
and notifier writes, repeated restore, production MMIO access, rejection of
unsupported control writes, v3 record ordering/serialization and malformed or
different APU payloads. All 10 component tests and 5 checker tests passed before
live validation. The corrected game requires a new capture: old captures
are deliberately rejected by the updated source fingerprint.

Live validation (2026-10-16): `build/rollback-practice-apu.rbc` completed all
42 pairs with the v3 APU payload. The first checker pass matched 38 samples
and blocked four on indirect animation callback `9DC20`, with no divergence.
Auditing that leaf confirmed it only computes cubic interpolation coefficients
using guest memory/registers and x87 arithmetic. Adding the original translated
helper to the checker's indirect-call allowlist required no game change and
did not change the capture's source fingerprint or expected results.

After that addition, all 42 individual samples matched on repeated execution,
with zero blocked or divergent samples. The carried-state chain matched six
cycles of seven tasks, then restored the checkpoint after task 21 and replayed
the final 21 steps to the identical final memory, APU image and task states.
Evidence: `build/rollback-apu-individual-audited.log` and
`build/rollback-apu-chain-audited.log`. The chain consumed 3,765,459 bytes of
recorded external changes from work outside the converted tasks. This is a
successful isolated task-group rewind with recorded external input, not a
whole-fight rollback test or proof of every sound-command path. No live game
state was restored; full APU service and unconverted task integration remain.

### Complete dispatcher-boundary audit

The next conversion adds `54110` (per-frame counter/state and a stage-specific
update) and `BE4A0` (match-state updates, with mode-dependent result/voice
requests). Both roots have one direct frame yield; their statically followed
guest call graphs contain no additional frame-yield call. Indirect calls and
host services still require the checker's existing fail-closed runtime audit.
The original native-fiber bodies remain the default without the opt-in flag.

`rollback_added_tasks` compiles the actual two root bodies and their generated
continuations, varying the mode/phase/stage branches for 60 advances. It compares
registers and all 128 MB of guest RAM at each yield, restores step 21 and replays
39 advances to the same final state. Helper calls use deterministic fixtures
to isolate root conversion; this test does not validate real helper or audio
behavior. All 11 component tests and five checker tests pass with the nine-task
configuration. The game executable has been rebuilt successfully.

Expanded live validation (2026-10-16): all nine experimental task entries were
logged, and `build/rollback-practice-nine.rbc` completed 54 task pairs without
a nested-yield warning. All 54 individual samples matched their captured result
and repeated execution, including the v3 APU payload, with zero blocked or
divergent samples. The chain matched six consecutive cycles, restored after
step 27, and replayed the remaining 27 steps to identical final RAM, captured
APU state and continuations. Evidence: `build/rollback-nine-individual.log` and
`build/rollback-nine-chain.log`. The chain consumed 3,798,184 bytes of recorded
external changes; work outside these nine tasks is still supplied from the
capture rather than simulated. No live state was restored. This validates the
captured Practice paths, not all mode-dependent branches or complete netplay.

At the nine-task milestone, six observed dispatcher tasks remained unconverted: `87420`, `72070`,
`1C60AB`, `E8030`, `49790`, and `691F0`. The first two reach nested frame-yield
calls and cannot be made fully restorable by saving just their root locals.
The lexical call audit also found a yield in `496D0`, reached from `49790`;
inspection shows that yield is in a copied tail after the entry paths return.
It is not evidence of a reachable nested wait by itself. The remaining tasks
also reach rendering/input or device operations whose host state and side
effects need separate replay handling. Full audio-chip advancement, interrupt servicing and
output reconciliation also remain required. None of these paths is being
silently omitted from the definition of a complete fight step.

### Nested continuation integration

`src/online/rollback_nested.c/.h` and `tools/recomp/rollback_nested.py` implement
an experimental explicit call stack, now used by the live match-flow task.
Each of up to 16 frames stores its guest routine, resume point and up to 64
root-scope locals.
Direct calls save the caller and push a child; tail calls
replace the current frame; returns resume the caller; frame yields unwind to
the host driver without retaining a native C stack. The generator accepts
multiple yield points and callers with no direct yield of their own.

Every call must be selected for transformation or listed as synchronous.
Unknown indirect transfers, calls suspended inside temporary scopes,
unsupported local declarations and unexpected call syntax are rejected.
The driver rejects invalid resume points, malformed checkpoint headers,
noncanonical unused frames, overflow and transition-budget exhaustion.
Restore validates a local copy before changing the destination. This stack
does not itself include guest RAM/registers, kernel objects or device state,
and uses the surrounding capture's source fingerprint to identify its layout.

The `rollback_nested` test compiles the actual `72FE0` match-setup routine and
its `85820` wait helper alongside a fixture parent/tail-call bridge. Other
setup, rendering and audio helpers are explicit deterministic fixtures, not
production emulation. Original Windows-fiber and converted executions match
all 128 MB of RAM and registers/x87 state at 60 yields. A checkpoint taken
three calls deep restores and replays 50 advances to identical final state.
The test also covers multiple yield sites, tail calls, child return into the
parent, corrupt checkpoint rejection without destination mutation, overflow,
and sticky failure after an exhausted transition budget. The generator's
negative fixtures reject unsafe suspension patterns.

The native-fiber test passed on 2026-10-16. The next revision now integrates
`72070` with 44 selected routines listed in `tools/recomp/rollback_match.py`.
Generation recomputes the direct suspension graph and rejects changes to that
set. Conditional tail branches transfer the explicit frame, and `72F50`'s
local jump-table fallback is restricted to selected targets. The source
fingerprint includes the nested generator, graph, runtime and frame layout.
Unconverted helper calls still use their production native implementations;
an unexpected scheduler handoff invalidates prior checkpoints and cancels an
in-flight capture without writing a success footer. Once that native step
fully returns, a new checkpoint can be made in the new scheduler epoch.
This does not declare arbitrary native or indirect helper calls replay-safe.

The nested stack is embedded in `Doa3RollbackTask`, so production scheduler
capture/restore includes it and validates its resume points. Capture format v4
includes the expanded task state, followed by optional APU data and page deltas.
The checker executes the same generated nested routines against private RAM,
includes all nested frames/locals in comparisons and midpoint restore, and
still blocks unaudited host services (including debug-unlock reads). Old format
fixtures remain readable, but old live captures fail the changed fingerprint.

Tests now include an actual resumed `72070 -> 72F50 -> 72070` path through the
v4 reader, invalid nested state, nested-state mismatches, production scheduler
restoration of nested locals, and capture cancellation releasing both clocks.
A v4 chain fixture carries nested state through 60 steps and a midpoint rewind,
and rejects changed nested results or gaps in a caller's saved continuation.
The ten-task live capture passed on 2026-10-16. The v4 file
`build/rollback-practice-nested.rbc` contains 60 task pairs across six consecutive
presentation frames (1975 through 1980), with APU snapshots and a complete
success footer. Its source fingerprint is
`c188f430baf008d7ab1b652e06574a1e5e941926d3137e17b99bff543df3df9d`.
Individual replay with `--native-camera` matched all 60 pairs, including repeat
execution, with zero blocked or diverged pairs. The chained run matched all
60 steps and restored the checkpoint after step 30 to reproduce the remaining
30 steps exactly. It applied 3,768,029 bytes of recorded external changes;
those changes remain journal input rather than replayed simulation.
Results are in `build/rollback-nested-individual.log` and
`build/rollback-nested-chain.log`.

All twelve captured boundaries for `72070` were at stack depth one, resume
point one. This validates the integrated match task's steady Practice path,
not deeper suspended production calls during loading or transitions. The
depth-three restore evidence above still comes from the native-fiber fixture
with deterministic external helpers. A native-handoff warning occurred during
loading, before frame 1854 and before the capture began; its old wording says
checkpoints are disabled for the task, but the implementation invalidates the
old epoch and permits fresh checkpoints once the native step fully returns.
An in-capture handoff would cancel the capture without a success footer.

This remains a ten-task isolated replay result, not whole-fight rollback.
The nine-task live result above remains historical evidence for that revision.

### Dispatcher capture evidence

#### Two display/state continuations (2026-10-17)

The next revision adds `E8030` and `691F0`, in their observed dispatcher order
between `1C60AB` and `BB500`. Both roots have one direct frame yield and no
other wait in their audited direct-call suspension graph. Generation rejects
changes to that graph; indirect or native helper suspensions retain the
scheduler's checkpoint-invalidation and capture-cancellation guards.

The original instruction bodies and all production graphics/helper calls are
preserved. `E8030` maintains cached stage/display flags, temporarily changes a
shared status flag, and consumes a callback; `691F0` selects scene callbacks
and runs display/state updates. Their command-buffer writes and host rendering
calls have not been declared presentation-only or safe to skip during replay.

The new `rollback_display` fixture compares both actual roots with original
Windows fibers for 60 yields each, across changing stage/cached-tick values,
status flags, callback pointers and optional scene branches. It also executes
the actual `E8100` initialization and `E8830` cached-state update bodies.
Other helpers, including graphics and indirect callbacks, are explicit
deterministic fixtures. All 128 MB of guest RAM and integer registers matched
at every yield. Restoring after yield 21 reproduced the remaining 39 steps and
final continuation bytes exactly. This test validates the root conversions,
not the fixture-replaced helper behavior or host graphics restoration.

All 21 automated tests passed (15 component and six checker tests), and the
game/checker builds completed. Their
source fingerprints agree:
`1426ae74ad56004ae2b5e6627f2d7607bf791f708fd7c7736e7bd9acfd88a36c`.
Build logs: `build/rollback-display-tests-build.log`,
`build/rollback-display-checker-build.log`, and
`build/rollback-display-game-build.log`.

The fourteen-task live capture completed on 2026-10-17, but its replay is
**not passed**. `build/rollback-practice-display.rbc` contains all 168 records
and the completion footer (209,822,976 bytes, presentation frames 1637–1642).
Runtime log: `build/rollback-display-runtime.log`. A subsequent two-second
read-only probe showed all fourteen converted tasks advancing 122–123 times;
the capture stopped automatically and no live state was restored.

The checker matched 72/84 individual pairs, blocked twelve, and found zero
divergences among completed pairs. Those 72 matches cover the previously
validated twelve tasks. All six `E8030` pairs first reached indirect callback
`EA070`; after auditing its actual leaf body (two guest-byte writes, no host
effect) and including it unchanged, they stop at scene callback `EA080` from
`EA930`. That larger callback updates guest scene state and dispatches model
helpers/virtual calls; it remains outside the audited indirect entry set.
All six `691F0` pairs reach GPU register `FD800044` from `1E2A89` inside
`1E2A50`. This is channel DMA_GET, the consumed command-buffer cursor; the
current capture contains no GPU aperture state. The checker now reports
`graphics_state_requires_snapshot` explicitly, with a rejection test, rather
than mapping live graphics memory or returning a guessed cursor.

The chain blocks at pair 12 (`E8030`) before its midpoint checkpoint. There
is no successful fourteen-task restore/replay result. Evidence:
`build/rollback-display-audited-individual.log` and
`build/rollback-display-audited-chain.log`. Checker-only callback/guard changes
do not change the capture fingerprint. The last complete live replay result
remains the twelve-task result below. `49790` is the one remaining unconverted
task from the fifteen-task audit. Its conservative suspension graph includes
seven routines; `496D0` includes an appended wait loop whose reachability must
be checked before deciding how much nested conversion is actually necessary.
No whole-fight or live game restore is enabled by this revision.

The next graphics-boundary work must cover DMA_GET/DMA_PUT and the production
`1E27C0` submission bookkeeping (guest cursor and fence updates, host parsed
position, command translation, and possible presentation). Capturing a register
alone would not make that host path replay-safe. Scene callbacks must be audited
and executed with unknown indirect/native services still rejected, not skipped
to manufacture matching task outputs.

#### Captured GPU queue and isolated register replay (2026-10-17)

After adding the actual `EA080` scene callback and its direct-call closure,
the saved v4 capture still has 72/84 matches, twelve blocked samples and zero
completed divergences. Both new tasks now reach `FD800044` (DMA_GET) in the
individual checks. The chain instead reaches the indirect `EA630` wrapper
from `EA123`. That wrapper forwards the guest arguments to `EA380`'s visibility
test; its translated body/closure is now included too. Unknown indirect calls
and native overrides continue to block. Evidence from before the `EA630`
addition: `build/rollback-scene-individual.log`, `build/rollback-scene-chain.log`.
The corresponding old checker is preserved as `build/rollback-scene-v4-checker.exe`.

`DOA3_ROLLBACK_CAPTURE_GPU=1` now includes a 264-byte snapshot of the production
USER queue: schema/reserved fields and all 32 channels' DMA_PUT/DMA_GET cursors.
This reads the actual host queue, not the inaccessible guest MMIO aperture.
The live USER handlers and private replay use the same extracted operations;
PUT still immediately advances GET exactly as this synchronous port already
does. Normal production width handling, channel aliases and unrecognized
register behavior are unchanged.

Capture v5 stores two explicit u32 payload sizes after its source digest,
then task state, optional APU state, optional queue state and guest page deltas
in each record. Legacy v1–v4 readers remain supported with matching source
fingerprints. The fingerprint now includes the production queue, core and
MMIO decoder sources. The new checker cannot use an old capture's missing
queue state or bypass its source mismatch.

The isolated backend reserves a private inaccessible MMIO aperture and runs
the production instruction decoder against the captured queue. It accepts
only 32-bit USER cursor accesses; other widths/registers stop replay. The
decoder-only build excludes live GPU initialization and VRAM allocation.
The queue is restored before each replay and compared after it, including
in carried chains, their external-write journal and midpoint rewind.

This snapshot excludes the push-buffer parser cursor/flip flag, PGRAPH state,
D3D resources, render targets and presentation. In particular `1E27C0` remains
unsupported; its submission, guest fence writes and possible presentation
must not be treated as an inert call. The fresh live queue capture described
below advances past USER access, but does not pass fourteen-task replay.

Validation: all 24 tests passed (16 component tests, eight checker tests).
The production writer test checks every queue record alongside the APU and
the bounded capture lifecycle. The MMIO test covers read/write, restore,
channel aliases, schema/size rejection and unsupported accesses. The v5 chain
fixture performs queue reads/writes over 84 steps and restores/replays 42;
bad queue results, bad lengths/schema and truncation fail. The real translated
paused-task reader test carries both APU and GPU state and detects a changed
queue byte. These are component/fixture results, not a live fourteen-task pass.
Game and checker builds succeeded with matching fingerprint
`24e856ec5adc99f3f1aee2b948ff945b8e3adc20ee5fa80b61b4b9d94963a3eb`.
Build logs: `build/rollback-queue-tests-build.log`,
`build/rollback-queue-checker-build.log`, `build/rollback-queue-game-build.log`.
The opt-in live run completed: `build/rollback-practice-queue.rbc`
(209,768,936 bytes, 168 records and success footer), with
`build/rollback-queue-runtime.log`. The user completed the requested test;
no explicit visual assessment was supplied. A post-capture two-second probe
shows every converted task advancing 120 times. No live state was restored.

The fresh replay matches 72/84 individual samples, blocks twelve, and finds
zero completed divergences. Both `E8030` and `691F0` now get past the USER
DMA_GET read and block at `FD00324C` from `1E2AB8`. The carried chain reaches
that same register at pair 12, so it has not reached its midpoint restore.
Evidence: `build/rollback-queue-individual.log`, `build/rollback-queue-chain.log`,
`build/rollback-queue-after-probe.json`. Checker diagnostics now report the
actual GPU register address instead of the host access-violation code.

`FD00324C` maps to `NV_PFIFO_CACHE1_DMA_SUBROUTINE` (PFIFO offset `124C`).
The production PFIFO reader returns `d->pfifo.regs[124C]` here. It is a separate
host register, absent from the USER queue snapshot; do not assume zero or
substitute a USER cursor. The next capture revision must include the relevant
PFIFO state and preserve its production read/write semantics, then investigate
any subsequent submission/parser boundary. The last fully matched live group
remains twelve tasks; playable whole-fight rollback is still incomplete.

#### PFIFO register snapshot (2026-10-17)

Queue image schema 2 expands the GPU payload from 264 to 33,040 bytes. It
includes all 8,192 entries of the production PFIFO register array (the port
indexes this array by byte offset), plus pending/enabled interrupt bits and
the USER cursors. The live export copies these fields from the actual NV2A
instance at the cooperative task boundary. No GPU locks, threads, handles,
parser state or render resources are serialized. Capture framing remains v5;
payload sizes, schema and source fingerprint distinguish this revision.

The production PFIFO handlers and isolated checker share the extracted
register operations. Existing empty-cache status, busy-bit masks, GET/PUT
relationship, ordinary register stores and IRQ update requests are preserved.
The checker accepts aligned 32-bit PFIFO reads/stores, but rejects writes to
interrupt acknowledgment/enable registers: interrupt delivery is not yet
captured. Unknown widths/register blocks/instructions still stop the sample.
The former USER-only checker is retained as `build/rollback-queue-v1-checker.exe`
for its matching older capture; missing PFIFO data is never filled with zeros.

The expanded fixtures exercise a nonzero subroutine cursor, idle/status masks,
USER aliases, PFIFO read/write persistence, all register bytes in serialization,
and rejection of IRQ writes. The chain fixture carries and restores both queue
domains; deliberate PFIFO result corruption must fail. Its MMIO load is kept
separate from arithmetic to match the captured guest path: the production
decoder does not support a compiler-folded IMUL memory operand. The initial
fixture exposed that limit and was blocked, rather than accepting guessed data.
The fresh live result below clears the PFIFO-read boundary but still does not
establish that either rendering task or the fourteen-task chain passes.

Builds succeeded and all 24 tests passed after separating the fixture's MMIO
load from its arithmetic. Logs: `build/rollback-pfifo-component-tests.log`
(16 passed), `build/rollback-pfifo-checker-tests.log` (six passed, initial
IMUL fixture failure), `build/rollback-pfifo-chain-tests.log` (corrected GPU
chain passed), and `build/rollback-pfifo-format-tests.log` (format passed).
The new game/checker fingerprint is
`d655cb6e27b47fb451b1fefa0f2e4dc279a9cfd2aca277c4f00fa6366205a1b1`.
`build/rollback-practice-pfifo.rbc` completed (216,046,104 bytes, 168 records,
presentation frames 3541–3546, success footer), with runtime log
`build/rollback-pfifo-runtime.log`. Its records include 1,005,552 APU bytes and
33,040 GPU bytes each. The user completed the requested Practice test; there
was no explicit visual assessment. No live state was restored.

The checker still reports 72/84 individual matches, twelve blocked and zero
completed divergences. Both rendering tasks now get past `FD00324C` and reach
the native KickOff override `1E27C0` from `1E2B92`. The carried chain stops at
that same call in pair 12, before its midpoint checkpoint. Evidence:
`build/rollback-pfifo-individual.log` and `build/rollback-pfifo-chain.log`.
No sample is declared successful just because its register access is supported.

The next integration boundary is command submission, not another missing
register. `1E27C0` consumes the ring using host `s_pb4134_parsed`, ring bounds
and `s_pb4134_flip`; it translates methods, updates USER cursors, the guest
last-kicked cursor and fence semaphore, and may call `d3d4134_frame_done`.
That path presents, runs pacing/worker service and the guest vblank callback.
PGRAPH method dispatch also changes host render state. A replay adapter must
capture/parser-replay the relevant submission state, reproduce guest-visible
cursor/fence effects, and explicitly separate or reject presentation/worker
effects. Calling the live renderer or merely returning success would not
validate rollback. Parser metadata and PGRAPH state remain absent from this
capture, so they cannot be reconstructed by assuming zero.

#### Shared submission parser and stream diagnostic (2026-10-17)

`nv2a_submission.h` now owns the port's push-buffer walker and submission
cursor/fence operations. The live override invokes it with the existing guest
memory and PGRAPH callbacks. It retains increasing/non-increasing methods,
old/new jumps, the single call/return slot, ring wrap, recording cursor,
USER PUT/GET updates, last-kicked cursor and fence completion. Production uses
the previous malformed-stream stop rules; isolated replay rejects malformed
or unsupported streams. Presentation/vblank remains outside this module.

GPU snapshot schema 3 adds the parser position, ring bounds, pending flip,
translation/tracing flags, command count and ordered 64-bit FNV method digest.
The payload is 33,080 bytes. The live capture reads actual parser state from
the override; it is not initialized from guessed values in the checker.
Tracing is enabled only while the bounded task capture is armed, and turns off
on the next submission after completion. Ordinary play does not hash commands.
The prior checker is preserved as `build/rollback-pfifo-v2-checker.exe`.

Default replay still rejects `1E27C0`. The explicit `--submission-stream`
profile runs the shared parser against private guest RAM/register state,
reproduces cursor/fence/ESP effects, and compares the captured command count
and digest along with all existing guest/task/APU/GPU bytes. It does **not**
execute or restore PGRAPH, resources or pixels. A matching digest is evidence
for the ordered command stream, not proof of complete graphics state. Pending
or encountered flips are rejected because presentation, pacing, worker service
and guest vblank have not been made replayable. Missing traces, unsupported
no-channel paths and unknown services also stop the sample.

The new parser fixture checks exact emitted command sequences, ring wrap,
calls/returns, recording, fences, checkpoint restore, malformed streams and
disabled tracing. The runtime profile fixture computes its expected command
digest independently in Python and checks the real adapter's guest effects,
strict default rejection, altered-digest detection, missing-trace rejection
and refusal to present a frame. The profile is diagnostic infrastructure;
it is not enabled for live rewinds or exposed as playable netcode.

Validation: the Release game and checker build successfully; all 17 component
tests and all 9 checker tests pass. Their generated source fingerprints agree:
`93193d7ac9b83e8c35d95b218e1a06ca5cf99e09d7e308e19c4b5bafe8cba8cf`.
Results are recorded in `build/rollback-submission-component-tests.log`,
`build/rollback-submission-checker-tests.log` and
`build/rollback-submission-format-tests.log`.

Live capture `build/rollback-practice-submission.rbc` completed all 84 task
pairs (216,011,824 bytes), without restoring live state. The first replay
matched 71 pairs, blocked 13 and found no completed-task divergences. Strict
replay retained the submission guard. With the explicit submission profile,
the six `E8030` samples reached indirect scene callback `E9710`, the six
`691F0` samples reached scene-effects callback `EAF70`, and one `98DB0` sample
reached audio-object AddRef callback `1F12D7`. Chained replay reached `E9710`
at pair 12. Logs: `build/rollback-submission-strict.log`,
`build/rollback-submission-individual.log`, `build/rollback-submission-chain.log`.
This advances past submission but does not yet establish agreement for the
two full rendering tasks or the complete frame chain.

The checker now includes audited indirect bodies `E9710`, `EAF70` and
`1F12D7`, retaining all guest operations and the existing native-call guards.
Rechecking the same capture still gives 71 matches, 13 blocked and zero
completed-task divergences. The six `E8030` samples now reach `1DC8E0`
(SetRenderTarget, including host offscreen-target tracking); the six `691F0`
samples reach `E69E0`; the audio sample reaches `1F1816`. The chain stops at
`1DC8E0` in pair 12. These are unresolved boundaries, not ignored effects.
Logs are `build/rollback-submission-callback-individual.log` and
`build/rollback-submission-callback-chain.log`. The updated checker builds
4,427 guest functions; the reader/translated-task test and both GPU/submission
adapter tests pass again. Only the offline checker changed, so this step
requires neither a new game executable nor another live capture.

#### Render-target routing snapshot (2026-10-17)

`nv2a_targets.h` now shares the production SetRenderTarget hook's host routing
table and update rules with isolated replay. The original guest function still
runs first. Its incoming surface/device are preserved; a null surface resolves
through the post-call guest device state. Display-buffer offsets are removed
from the reflection table, duplicate offsets are ignored, and the original
64-entry bound and swap-removal order (including inactive slots) are retained.
The renderer reads the same shared live table; no D3D objects are serialized.

GPU schema 4 appends the actual 264-byte target table, giving a 33,344-byte
payload. The capture reads it on the game thread at each task boundary. Import
rejects old schemas, excess entry counts and nonzero reserved fields. The
schema-3 checker is preserved as `build/rollback-submission-v3-checker.exe`;
old captures cannot establish target state and are not padded with guesses.

The submission-stream profile now wraps translated `1DC8E0` with the shared
target update against its private captured table. Default replay still blocks
this host hook, and missing GPU state is rejected. All target bytes join the
existing comparisons. This covers routing metadata, not texture resources,
PGRAPH state, pixels, presentation or vblank. Other native effects remain
blocked. `E69E0` and `1F1816` from the prior run remain unaudited boundaries.

New fixtures cover default targets, aliases, duplicate removal, table capacity,
snapshot restoration and invalid-state rejection. The real runtime wrapper is
also checked against an independent guest ABI fixture that clobbers registers
and changes the fallback target during the guest body; it verifies correct
ordering, stack effects, repeated replay and detection of altered routing.
The game and checker build with matching source fingerprint
`505a11e63a372f5a4740b3320d8961a428c3cee12a53e5465cfc9d31d6a3c0ab`.
All 18 component tests and all 10 checker tests pass (28 total).

`build/rollback-practice-targets.rbc` completed all 84 task pairs over frames
1610–1615 (168 records, 221,730,576 bytes). It contains the actual two-entry
target table; the table remained unchanged throughout these six frames.
This capture therefore checks preservation of an existing table, while the
fixtures exercise registration/removal. Initial replay matched 71 pairs,
blocked 13 and found no completed-task divergences: the rendering tasks now
stop at indirect guest callbacks `E97F0` and `E69E0`, and the first audio task
stops at `1F1816` (also the chain's first stop at pair 8). Logs use the
`build/rollback-targets-` prefix. These intermediate stops do not prove the
complete rendering tasks match.

Offline callback coverage now includes the actual guest bodies for `E97F0`
(intersection), `E69E0` (effect-list iteration) and `1F1816` (guarded AddRef).
That pass retained 71 matches/13 blocked/zero completed divergences, reaching
`F5B90`, `51830` and `1F4A7D` instead. The latter three bodies are now included
as well: a guest float return, position/height projection, and voice-format
setup through the existing private captured APU backend. These additions do
not change the game binary, source fingerprint or host-service guards.
The second pass matches 72/84 pairs, blocks 12 and has zero completed-task
divergences; the formerly blocked `98DB0` audio sample now matches including
the private APU image. Only `E8030` and `691F0` remain blocked, at `36C00` and
`118FB0`. Their tiny predicate/tail-wrapper bodies have also been added for
the next replay pass. Intermediate logs retain the `callback`/`callback2`
suffixes, including the passing reader and submission/target fixture tests.
The third pass matches 78/84 pairs with six blocked and no divergences:
all six `E8030` samples now match in full, including command digest and target
routing. Chained replay passes that task and stops at pair 13 (`691F0`), at
effect iterator `E7360`. Its audited guest-only leaf body is now included too.
At that stage, the pass for this capture still matched 78/84 with six blocked and zero
divergences, now stopping solely at `691F0`'s indirect callback `EF190` (site
`1805BA`). Chain replay stops at that same callback in pair 13, before the
midpoint checkpoint. `EF190` forwards guest constants to `DDEE0`; it remains
outside the audited callback set. The checker builds 4,444 guest functions.
Final logs are `build/rollback-targets-callback4-individual.log` and
`build/rollback-targets-callback4-chain.log`. This establishes full task-level
agreement for `E8030` in this six-frame capture, not complete-frame rollback,
GPU resource/pixel restoration or independence from recorded external writes.
The final reader/translated-task, submission and target-profile tests all
pass again (`build/rollback-targets-callback4-tests.log`). No game binary or
live state was changed during these offline callback extensions.

#### Ordinary sprite replay boundary (2026-10-17)

The checker now executes `EF190`'s original draw wrapper and direct translated
helpers. The existing schema-4 capture then reaches the manual `6DF00` sprite
hook. That hook delegates ordinary sprite IDs unchanged to the guest body;
only `ONLINE_MENU_ICON`/`ONLINE_MENU_LABEL` use host menu assets and staging.
The offline adapter reads the actual incoming sprite ID and executes the
translated native body for ordinary sprites, rejecting both custom IDs with
`online_sprite_requires_host_state`. It includes the production menu header
for those IDs and retains checked guest-pointer access. No menu state, pixels
or return values are invented. A fixture covers native delegation, both
custom-asset rejections and an out-of-range sprite pointer. This is an exact
conditional branch of the host override, not a blanket observer exemption.
All 11 checker tests pass, including the new guard fixture. Replay of the
same capture gets past ordinary sprite drawing and stops at the guest-only
UI lookup `5AF60`, with 78 matches/six blocked/zero divergences. That lookup's
actual leaf body is now included as well. Intermediate results are retained
in `build/rollback-sprite-{individual,chain}.log`; validation logs are
`build/rollback-sprite-fixture-tests.log` and
`build/rollback-sprite-format-tests.log`.
The lookup pass next reaches sprite Begin callback `1EE9DD`. Its original
body captures/applies guest D3D state blocks through `1E982D`/`1E96A5` and
marks the guest object active. Replay then reaches End (`1EEAE3`), which
applies the saved block and clears that flag. Both actual bodies are included,
as is the associated `1EEA0C` transform wrapper used by the same caller; its
indirect draw method still requires independent coverage. Intermediate logs
use `rollback-ui-lookup-` and `rollback-sprite-state-`. The captured method
count advances by 180,612 commands in `E8030` and 253,695 in `691F0` over the
six frames; command counts/digests remain diagnostic evidence, not pixels.

The next passes add the actual `1EEB46` Draw body (quad construction,
optional transform, texture/blend state and DrawPrimitiveUP submission) and
`1EEB25` Release body (reference decrement and conditional destruction).
Release retains the original state-block/device/allocator calls and all
unsupported native boundaries; it does not fake successful resource cleanup.
The resulting checker contains 4,466 isolated translated guest functions.

Validation against `build/rollback-practice-targets.rbc` now passes **84/84
task samples**, with zero blocked or divergent results and identical repeat
executions. This covers six samples of each of the fourteen converted tasks,
including both `E8030` and `691F0`. Chained replay also matches all six cycles,
restores its checkpoint after step 42, and reproduces all remaining 42 steps.
Evidence: `build/rollback-sprite-release-individual.log` and
`build/rollback-sprite-release-chain.log`.
All eleven checker tests pass after the final callback additions, including
format validation, checkpoint restore, APU/GPU chains, kernel ABI, submission,
target routing and custom sprite rejection
(`build/rollback-sprite-release-tests.log`).

The chain still imports **1,959,387 bytes of recorded external changes**.
`49790` remains the unconverted task from the fifteen-task dispatcher audit.
The submission profile compares synchronization, target routing and ordered
command digests, not PGRAPH execution, host resources or rendered pixels;
flip/vblank remain blocked. Thus this establishes the captured fourteen-task
group, not complete-frame or live rollback. These checker changes did not
modify the game executable or restore live state.

#### Fifteenth observed task: `49790` (2026-10-17)

The explicit task list now includes `49790` between `E8030` and `691F0`,
matching slots 13/14/15 of the original dispatcher audit. This covers all
fifteen tasks observed in that Practice session, not every possible task in
other modes or transitions. Capture remains opt-in, bounded to six cycles
(90 task pairs), with the existing unexpected-suspension cancellation guard.

The direct suspension graph initially appeared to require seven nested
routines. Its only extra yield was an unreachable copy of the `49790` loop
appended to leaf `496D0` by the lifter. The graph builder now checks that the
leaf starts at its original entry, every explicit jump stays within the leaf,
there are no calls/indirect transfers, and its final instruction returns before
the appended loop. Only then is that tail excluded from graph analysis. The
original translated bodies remain unchanged. Mutations introducing a jump to
the tail, a call, an indirect transfer or a missing final return are rejected.
With that proof, `49790` has only its own direct frame yield and uses the
existing single-yield continuation representation; no SIMD-local schema change
or invented helper result is required in the game/checker.

The display continuation fixture now includes the actual `49790` root and
checks initialization once, per-frame helper ordering, register/RAM equality
against a native fiber for 60 yields, then 39 steps after restoring a
checkpoint. Its external draw helpers are synthetic; it is a continuation
test, not evidence of actual renderer replay. The audit reader fixture also
checks the newly converted task plus an explicitly unknown task identity.
All eighteen component tests pass, including the expanded display fixture and
production capture/reader tests (`build/rollback-final-task-component-tests.log`).
The game and checker both rebuild successfully; the checker includes 4,468
translated guest functions. The live diagnostic build is armed for
`build/rollback-practice-fifteen.rbc`; replay validation is pending that capture.

The first live attempt stopped after twelve pairs: `49790` had been added to
the generator but its manual live-entry hook was missing, so its experimental
step counter stayed at zero. No live state was restored and that incomplete
capture is not a pass. The hook now follows the same opt-in trampoline/native
fallback as the other roots, with its generated body renamed through the
override manifest. Generation now rejects any listed task missing its exact
live wrapper, renamed original body or manifest entry; negative tests cover
all three omissions. The corrected run uses
`build/rollback-practice-fifteen-hook.rbc`.
The corrected game/checker builds pass. The targeted display, frame-audit and
task tests pass again, as does the rebuilt checker's reader/translated-task
test (`build/rollback-fifteen-hook-tests.log` and
`build/rollback-fifteen-hook-reader-tests.log`). The full eleven-test checker
suite also passed before the entry-hook correction
(`build/rollback-final-task-checker-tests.log`). Live validation of the
corrected hook remains pending the new Practice run.

The corrected live run completed **90 task pairs** with `49790` explicitly
active (fiber 19), and no live state restored. Capture:
`build/rollback-practice-fifteen-hook.rbc` (228,675,568 bytes); fingerprint
`227f07f6fabe0f4cc2417f76438d142601d225d8b9cf2708c9fcdbdba0fef4d0`.
The initial replay matches the other 84 samples with no divergences and stops
at `49790`'s indirect `4CD00` callback (site `4C69D`). Chained replay stops at
the same call in pair 13 (`build/rollback-fifteen-verified-{individual,chain}.log`).

The checker now includes the original update/predicate/draw callback bodies
from captured table `31C43C..31C53F`, plus the three descriptor callbacks
installed by those draw routines. These operate on guest animation arrays,
matrices and draw descriptors; native services and unknown indirect targets
remain guarded. This is an offline-only extension; it does not require another
game build or invalidate this capture.

With those callbacks included, **90/90 samples match**, with zero blocked or
divergent results and identical repeat execution. All fifteen observed task
types match in frames 2403–2408. Chained replay matches all six cycles, restores
after step 45 and reproduces the remaining 45 steps exactly. Evidence:
`build/rollback-player-callbacks-individual.log` and
`build/rollback-player-callbacks-chain.log`. The checker builds 4,504 isolated
guest functions. These are actual captured task comparisons, not synthetic
draw-helper results.
All eleven checker regression tests also pass after these additions
(`build/rollback-player-callbacks-tests.log`).

The chain still consumes **100,648 changed bytes** of recorded external state:
38,764 guest-RAM bytes, 61,805 APU-snapshot bytes and 79 GPU-snapshot bytes
(changes summed across boundaries, not unique addresses). A read-only delta
audit reproduces the checker's total in
`build/rollback-fifteen-external-writes.json`. Of the RAM changes, 37,392 occur
between the last task and the next frame's first task; smaller gaps include
the dispatcher slot/stack state. The largest RAM pages include `03D26000`
through `03D2B000`, `005CF000` and `005D3000`; page locations alone do not
establish their ownership or simulation relevance. Nothing is masked out.

The next milestone is to account for and reproduce that frame-boundary work,
including scheduler, input/worker and audio progression, instead of importing
it from the recording. Ordered GPU command agreement still does not reproduce
PGRAPH/resources/pixels; presentation/flip and real live restoration remain
outside this test. Covering all fifteen observed tasks is therefore not yet
complete-frame or playable rollback.

The new task list changes the source fingerprint. The old successful 84-pair
capture remains evidence for its original build only; its compatible checker
and fingerprint are retained as `build/rollback-fourteen-tasks-checker.exe`
and `build/rollback-fourteen-tasks-digest.bin`. A new live capture is required
to validate the fifteen-task group. It will still record external changes and
will not by itself establish complete-frame or live rollback.

#### Outer-loop gap audit (2026-10-17)

The bounded frame audit now uses schema 2 and records twenty ordered markers
in `A6450`, between the first dispatcher's return and the second dispatcher's
entry. These split the gap across guest post-dispatch work, presentation,
frame-start servicing, clearing and pre-dispatch work. The original guest
instructions remain intact. A missing, duplicated or out-of-order marker
invalidates the capture rather than silently producing an incomplete audit.

Use `DOA3_ROLLBACK_TASKS=1` and `DOA3_ROLLBACK_FRAME_CAPTURE=<path.rbf>` with
`DOA3_ROLLBACK_CAPTURE` unset. After 120 eligible solo-Practice passes, it
records two passes and their intervening gap, then disables itself. Clocks,
APU and asynchronous workers remain live. The bounded memory copies may hitch;
the reported deltas are observations, not exclusive attribution to a call,
timing measurements, replay validation or atomic restorable snapshots.

`tools/rollback_frame_report.py` reports each marker's preceding interval,
changed RAM pages, kernel-call deltas and presentations. Production writer and
reader tests pass for complete marker ordering, injected changes, missing and
invalid markers, and malformed captures. The generator is checked for
idempotence and preservation of all non-marker text. Compatible binaries for
the earlier 90-pair result are retained as
`build/rollback-fifteen-tasks-checker.exe` and
`build/rollback-fifteen-tasks-digest.bin`.

Live validation completed on 2026-10-17. `build/rollback-frame-gap.rbf`
(36,787,084 bytes, source fingerprint
`3b7230003aec5dc0d639d70592e9b2e0d10e6f2c573387004135ccd3a166dcd9`)
passes the strict reader: two complete dispatcher passes, all fifteen observed
tasks in each, all twenty gap markers, and one presentation. The report is
`build/rollback-frame-gap-report.json`. Across the gap's intervals it observes
295,173 changed RAM bytes and 1,638 kernel calls; these are summed changes,
not unique addresses or ordinary-frame workload measurements.

The largest interval, `A6450 -> A6455`, contains `A65B0` and its wrapper's
scheduling point: 68,989 changed bytes, including one `NtReadFile` (ordinal
219) and one `KeDelayExecutionThread` (99). The captured 38,912-byte buffer at
`005FD5C0..00606DBF` exactly matches `HDD/D/bgm.afs` at file offset `09E00000`.
Earlier runtime log entries also identify that buffer as a fiber-2 read from
handle `66000024`, opened on `bgm.afs`. This establishes a concrete streaming
write, not merely an address-based guess. It does not establish that every
other changed page is audio-owned or safe to exclude from a snapshot.

Most other intervals include repeated worker priority/resume/suspend and
object-reference calls. Generated `recomp_probes.c` wrappers call
`xbox_fiber_timeslice()` when due, including after the dispatcher's after-hook.
`xbox_fiber.c` catches up worker laps from elapsed host time, capped at 64;
the diagnostic's RAM copies therefore perturb worker scheduling. Even the
dispatcher-end-to-first-marker interval contains worker activity. Do not
attribute all interval writes exclusively to the intervening guest routine,
subtract them as noise, or treat this audit as a replay or performance test.

The next implementation target is the outer-loop/worker contract: deterministic
input sampling and frame servicing, plus restorable worker/streaming/audio
progress or a proven separation that prevents presentation workers from
altering replayed simulation. Existing task snapshots deliberately cover only
dormant converted tasks; they do not restore native worker continuations,
file positions/completions or full APU/DSP processing. Existing deterministic
clock checkpoints alone do not close this gap. Whole-frame validation must
eventually advance without importing recorded inter-task/boundary deltas.

#### Private audio progression and worker invalidation (2026-10-17)

`src/apu/apu_replay.c` adds a private, single-threaded processing backend. It
executes the production voice processor and the current passthrough DSP, with
shared chip gating and IRQ latching from `apu_chip.h`. Its own checkpoints
include command/filter history, clock/debt/IRQ state, the partial 256-sample
output period and its phase. The caller checkpoints the private 128 MiB guest
RAM separately. A game frame advances 25 chip ticks (800 samples at 48 kHz);
retire-wait ticks advance immediately and are repaid from later frame budgets.
Completed PCM periods are returned as data and never submitted to a device.

This backend requires `DOA3_APU_OFFLINE` and is not linked into the game. The
production physical-memory shim binds RAM globally, so it must run in an
isolated process. Seeding from an old command capture deliberately starts a
fresh output period: those captures lack live monitor-buffer state. This is
not a live audio restore API, full DSP56300 emulation, host mixer/device rewind,
or a comparison with the captured session's subsequent audio progression.
It does not run the guest DirectSound interrupt handler or CRI/file workers.

The production executable uses the shared gating/IRQ helpers with the same
existing host interrupt delivery; it still owns normal audio output. No audio
queue is replayed into speakers by the new offline backend.

Validation: all nineteen rollback component tests pass, including chip IRQ
latching, 60 Hz budgets, retire-wait debt spanning a frame, partial PCM-period
restore, malformed checkpoint rejection and unchanged state on rejection.
The audio replay test also ran against the six original main-task samples in
`rollback-practice-fifteen-hook.rbc`, frames 2403–2408. For each seed, two
restores reproduce identical private chip state, every byte of guest RAM and
all output PCM. Each sequence produces 8,704 nonzero stereo frames; observed
RAM changes range from 8 to 14 bytes. These are captured-state **self-replay**
checks, not full-frame live comparisons. Evidence:
`build/rollback-audio-tests.log`, `build/rollback-audio-captured-replay.log`.
Reproduce the captured-state checks with:

```powershell
python tests/rollback/audio_capture_replay.py build/rollback-practice-fifteen-hook.rbc build/tests/rollback/Release/apu_replay_test.exe --source-digest 227f07f6fabe0f4cc2417f76438d142601d225d8b9cf2708c9fcdbdba0fef4d0
```

Task checkpoint restoration now also checks an external-execution epoch. Any
native worker or unconverted coroutine execution invalidates older task
checkpoints, even if it returns to the exact same wait key. Worker wake paths,
including deadlock recovery, also invalidate them. The scheduler test executes
a real Windows worker fiber, verifies atomic rejection after its wake/run,
and verifies that task-only restore still works while that worker stays parked.
This detects unsafe partial restoration; it neither freezes nor restores
worker continuations. Scheduler sources now participate in capture fingerprints.

The executable rebuild succeeds (`build/rollback-audio-main-build.log`). The
remaining integration work is to make worker/streaming continuation and file
completion state restorable, join private audio progression to the full frame
step, and reconcile presentation effects before attempting live rewind.

#### First restorable worker: CRI watchdog (2026-10-17)

`DOA3_ROLLBACK_WORKERS=1` together with `DOA3_ROLLBACK_TASKS=1` enables an
explicit park boundary for the 3.1 watchdog `1926B0`, inside its original
`18C690` thread-start wrapper. It preserves the existing manual override's
counter increment (`40C700`), stop flag (`40C718`), exit acknowledgement
(`40C71C`) and cooperative yield. No file or audio operation occurs in this
worker's iteration. Its surviving native local is an immutable fiber pointer;
no native stack is copied or restored. Shutdown invalidates the checkpoint
before the enclosing startup wrapper unwinds. The default path is unchanged.

The new process-local `xbox_fiber_converted_capture/restore` API includes the
converted tasks and this worker, saved guest registers/x87 state, the worker
round-robin cursor and idle-yield count. Capture and restore require the primary
fiber, all included continuations parked at their audited boundaries, no
pending directed handoff, and an active deterministic clock session. Restore
validates all records before changing any. The caller still owns guest RAM,
clock contents, kernel objects, file state and the APU; this is not a complete
frame snapshot or an automatic live rewind.

A separate worker-progress epoch keeps the old task-only API conservative:
running the converted watchdog still invalidates task-only checkpoints.
The combined API permits that progress, but rejects an intervening unconverted
worker execution or wake, task topology change, clock-session change, or
watchdog shutdown. The other three observed CRI workers (`1926F0`, `192760`,
`1927D0`) remain native and are **not** restorable. Host-time scheduling and
streaming/completion state are still outside this checkpoint.

The production Windows-fiber test captures one converted task and the watchdog,
advances them for 30 steps, then restores and repeats twice. Each replay
matches the complete checkpoint and every byte of its 8 MiB test guest RAM.
It also tests task-only invalidation, a missing deterministic session, changed
session, unconverted-worker advancement, wrong buffer size and shutdown.
This fixture uses the production watchdog body and scheduler with a synthetic
task and startup wrapper; it is not a full-game replay comparison.

All nineteen component tests pass (77.89 seconds), and the Release executable
build succeeds. The opt-in executable reaches the menu and logs the real
watchdog entry on fiber 1. The read-only probe observes 1,871 watchdog
iterations over two seconds, alongside 120 main-task updates. This verifies
that the hook is active, not live rewind correctness. Evidence:
`build/rollback-watchdog-tests.log`, `build/rollback-watchdog-main-build.log`,
`build/rollback-watchdog-runtime.log`, `build/rollback-watchdog-live-probe.json`.
This run has neither task nor full-frame memory capture enabled.
The user confirmed normal movement, stage rendering and audio in solo Practice
with Helena and Ayane. A subsequent read-only sample observed all fifteen
converted tasks advancing 120 times over two seconds and 1,866 watchdog
iterations (`build/rollback-watchdog-practice-probe.json`). This completes the
normal-execution smoke check, not a live restore test. `tools/rollback_probe.py`
now includes the watchdog counter when the running build exports it.

#### Streaming completion queue (2026-10-17)

The 3.1 streaming workers depend on host-owned completion metadata in
`kernel_bridge.c`. Successful asynchronous reads queue a guest completion
routine, guest IO-status pointer and transferred-byte count. An alertable
`KeDelayExecutionThread` delivers them later. Restoring only guest RAM loses
the pending callbacks and their order.

`kernel_apc.c` now owns that bounded 16-entry FIFO. Its process-local checkpoint
stores pending entries, queue identity and reset epoch. Capture/restore refuse
an active delivery batch, invalid/overflowed state, a different queue/reset
session, malformed entries or incorrect buffer sizes. Rejections leave the
queue unchanged. Overflow retains the existing bounded behavior but now emits
a warning and prevents checkpoints from certifying lost completions. Snapshot
functions execute no callback and touch no host file.

Delivery detaches its current batch before calling guest code. Previously,
delivery cleared the count but continued iterating the same array; callbacks
that queued new work could overwrite the remaining original callbacks. New
work now waits for the next alertable delivery, or an explicit nested alertable
delivery, while the original batch retains its order. The in-flight depth
stays nonzero across nested delivery, so checkpoints cannot hide a suspended
callback's native C stack.

The production `xbox_stream_completion_capture/restore` wrappers additionally
require the primary fiber and an active deterministic-clock session. This API
is not yet called by a live rewind driver. File handles/cursors, read buffers,
worker continuations, clock state and guest callback effects remain separate
responsibilities; the three streaming/audio workers are still native.

The new `rollback_apc` test uses the production queue with synthetic callbacks.
It covers pending and empty queues, callback order with reentrant enqueue and
nested delivery, three identical executions after restore, callback effects on
fixture guest memory, in-flight rejection, overflow, reset/instance mismatch
and atomic malformed-checkpoint rejection. These are component checks, not
evidence of replaying actual file reads or full streaming workers. Queue source
and headers now participate in capture fingerprints and build dependencies.
The read-only runtime probe reports queued/delivered completion counters.

The Release executable rebuild succeeds (`build/rollback-apc-main-build.log`).
The user confirmed normal loading, music, voices and controls during solo
Practice with Helena and Ayane. A read-only sample found all fifteen converted
tasks advancing 120 times over two seconds, 1,816 watchdog iterations, and
three queued and delivered completions (180 total of each). No APC queue warning
was logged. Evidence: `build/rollback-apc-queue-runtime.log` and
`build/rollback-apc-queue-practice-probe.json`. This is a normal-execution check;
no production queue or guest state was rewound in this session.
All twenty rollback component tests pass (105.24 seconds), including the new
completion-queue test; see `build/rollback-apc-tests.log`.

#### Read-only streaming file positions (2026-10-17)

`kernel_stream.c` adds a process-local file-position checkpoint. A session
binds an ordered set of guest IDs to their existing Win32 read handles. Before
capture is allowed, it opens read-only leases on the same files with write
and delete sharing denied, checking volume/file identity, size and last-write
time. Writable bindings, active external writers, directories, duplicate IDs
or handles, missing files and identity mismatches are rejected. A failed begin
releases all leases already opened. No file content is written or restored.

Capture stores each original handle's read position, including positions used
by sequential and explicit-offset reads. Restore validates all identities,
nonnegative offsets, session ownership, generation, table epoch and
buffer size before changing any cursor. It keeps the original positions for
compensation if an OS seek fails. Return 0 means no change or successful
compensation; return -1 means compensation failed and poisons the session.
A future driver must abandon replay on that result. Snapshots cannot cross
session end/begin, and poisoned sessions cannot capture or restore again.

The bridge exposes `xbox_stream_file_checkpoint_begin/end/size/capture/restore`.
Begin requires primary-fiber execution and an active deterministic clock,
rejects untracked file handles, and binds every currently open tracked file.
Capture/restore also reject active/tainted completion delivery and a changed
clock session. Opens, closes, stale-handle reopens, writes, directory queries,
deletes and mutating file-information calls invalidate the table epoch and
release the pins before the ordinary operation proceeds. Position-only seeks
are restorable. The begin API is **not automatically called during gameplay**;
this is infrastructure for the future controlled frame driver, not live rewind.
All file checkpoint sources participate in capture fingerprints.

The `rollback_stream_files` test uses real temporary Windows files and the
production cursor and completion-queue implementations. Three executions after
restore produce identical bytes from sequential/explicit-offset reads, final
positions and pending completion metadata. It covers writer/delete exclusion,
partial-begin cleanup, directories, mismatched identity, duplicate IDs, changed
epochs, closed handles, malformed offsets, unchanged capture output on failure,
empty sets and session replacement. Test-only seek fault injection exercises
both successful compensation and poisoned-session failure. The mutation and
failure tests target only temporary files created by the test itself.

All twenty-one rollback component tests pass (73.88 seconds), and the Release
executable builds successfully. A separate read-only run against the actual
229,093,376-byte `HDD/D/bgm.afs` passes: three restores reproduce identical
32,768-byte outputs and both independent cursors, using offsets 4,096 and
`09E00000`. This mode never writes, truncates, renames or deletes its input.
It runs on private read handles in the test process, not the running game's
handles. Evidence: `build/rollback-stream-files-tests.log`,
`build/rollback-stream-files-main-build.log`, `build/rollback-stream-files-asset.log`.
No live game rewind has been attempted.

Remaining work includes native streaming-worker continuations, combined atomic
restoration of file positions/completions/RAM/clocks/APU/kernel state, a policy
for file opens during rewind, and live whole-frame replay. These file-position
tests do not establish full-worker or whole-game determinism.

#### Main game-flow continuation (2026-10-17)

The next revision adds `87420`, first in dispatcher order. Its direct suspension
graph contains 38 selected routines; the two nested roots share five routines,
giving 77 generated nested bodies in total. Generation checks the exact graph
sets rather than silently accepting new suspendable paths. The local switches
in `87420`, `20DE0` and `18A430` retain their enumerated guest-label targets;
their table expressions and target sets are checked at generation, and unknown
runtime targets fail instead of escaping into native tail calls.

`A47E0` (movie initialization with CRI virtual calls and a frame wait) is an
explicit native boundary. It and its native callers continue normally in the
game, but any native suspension invalidates old scheduler checkpoints and
cancels an active diagnostic capture. Fresh captures require the native step
to return completely. The isolated checker blocks this movie boundary, save
compatibility calls requiring uncaptured host keys, and carousel routines that
depend on the host-owned `g_online_menu_descriptors` base. These are blocked
paths, not successful stubs, and no loading/movie rollback is claimed.

`rollback_flow` compares the actual `87420 -> 87620 -> 873A0` bodies against
original Windows fibers with explicit deterministic fixtures for external
IO, rendering, audio, task creation and mode helpers. All 60 yields matched
the entire 128 MB of RAM and registers/x87. Twelve yields occurred three calls
deep; the wait then returned to the main loop for 48 yields. Restoring the
checkpoint after yield six reproduced all 54 remaining steps and the final
nested continuation exactly. The test exercises all five known main-loop
switch cases and verifies that a corrupt target fails. These fixture results
validate stack removal and local control flow, not the omitted host helpers.

All 20 automated tests passed (14 component and six checker tests), and the
game executable was rebuilt. The launch log confirms that `87420` enters the
explicit continuation. The source fingerprint is
`e42f3be66026b00d7879967abde4be26b219646b7016ffa099ef248bf41a579b`.
The runtime log is `build/rollback-flow-runtime.log`; the completed capture is
`build/rollback-practice-flow.rbc` (185,126,148 bytes, presentation frames
2102–2107, all 144 records and the successful footer).

Twelve-task live validation passed on 2026-10-17. The checker matched all 72
individual pre/post pairs, including repeat execution, with zero blocked calls
or divergences. The consecutive chain matched all six cycles, restored after
step 36 and reproduced the remaining 36 steps and final state exactly. It
compares all 128 MB of guest RAM, task registers/x87 and continuations, plus
the captured APU command state. Evidence:
`build/rollback-flow-final-individual.log` and
`build/rollback-flow-final-chain.log`.

The initial checker matched 64/72 and blocked eight calls without divergence.
Auditing the logging-only Start/join and pause wrappers (`88910`, `8A2E0`)
allowed their unchanged generated bodies. The HRTF selector `1F3660` and
seven indirect spatial-audio callbacks installed at `20BE88..20BEA0` were
audited as guest-memory/math operations and added as actual generated bodies,
not replacement results. Two exact current kernel ABI adapters (175 page-lock
stub and 173 identity physical-address query) completed the audio path; their
stack, memory and return-value tests also check invalid argument stacks.
These changes affect only the offline checker. The live executable and its
capture fingerprint did not change, and no expected output bytes were masked.

All twelve captured boundaries for `87420` were depth one, resume five;
deep loading waits remain covered by the native/converted fixture above,
not this live sample. The chain supplied 3,652,348 external journal bytes
from work outside the converted subset. The three remaining unconverted
tasks from the earlier audit are `E8030`, `49790` and `691F0`; their effects
are still recorded input, not replayed simulation. This is isolated task-group
checkpoint/replay evidence, not complete fight rollback, live rewinding, or
peer determinism. Rendering/host-resource reconciliation and complete dispatcher
coverage remain necessary before enabling a rollback match.

#### Player status/history continuation (2026-10-17)

The 2026-10-17 revision adds `1C60AB` in dispatcher order, between `235B0`
and `BB500` in the converted subset. Its original body maintains per-player
previous values and status flags, and calls model/render helpers as well as
guest-only updates. The conversion preserves its EBP local and guest stack
across its one direct frame yield. An unexpected helper suspension still
invalidates checkpoints; this addition does not declare its graphics calls
or host resources replay-safe. The original body remains the default unless
the rollback experiment is enabled at startup.

The root-equivalence fixture now includes this task and the actual translated
`1C1705` and `1C5F54` helpers. Initialization, model/render update, and `BB660`
helpers are explicit deterministic fixtures. Inputs alternate the helper's
single/tag paths, history-copy gate and pause-related status. On 2026-10-17,
all three task-root fixtures matched native Windows fibers for 60 yields each,
comparing all 128 MB of RAM and guest registers; restoring after yield 21
reproduced the remaining 39 yields and final continuation state exactly.
Production helper execution is checked separately by the isolated live
capture/checker. The eleven-task capture records 66 pairs, and the reader's
header limit now follows the writer's task-derived bound rather than the
previous fixed 64-pair ceiling. Empty and oversized counts remain rejected.

Live validation passed on 2026-10-17. `build/rollback-practice-status.rbc`
(172,410,784 bytes) completed all 66 pairs with a success footer, and the runtime
logged all eleven explicit task entries. With `--native-camera`, individual
replay matched 66/66 pairs, including all six `1C60AB` samples with production
helpers, with zero blocked or diverged results. Repeated execution also matched.
The chained replay matched six cycles and restored after step 33 to reproduce
the remaining 33 steps exactly. Logs: `build/rollback-status-individual.log`
and `build/rollback-status-chain.log`. Source fingerprint:
`0878434483390238f1bc0987c8997bbc26af02812ffbfd75a53180cf57aea73a`.

The chain still consumed 3,649,144 bytes of recorded external changes. Four
tasks from the earlier fifteen-task dispatcher audit remain unconverted:
`87420`, `E8030`, `49790`, and `691F0`. This establishes the captured eleven-task
Practice path, not whole-fight rollback, loading/transition replay, or complete
audio/render-state restoration. No live state was restored during capture.

`DOA3_ROLLBACK_FRAME_CAPTURE=<absolute .rbf path>` with
`DOA3_ROLLBACK_TASKS=1` enables a separate bounded diagnostic. Do not also set
`DOA3_ROLLBACK_CAPTURE`; simultaneous captures are refused. After 120 eligible
solo Practice passes, it records two consecutive complete dispatcher passes
and stops. Training recording/playback must be idle. Capturing may hitch.

The 3.1 dispatcher at `1C650C` visits 18 task slots in order, decrementing their
delays and invoking eligible tasks through `1C64C7`. The audit hooks the existing
stack-probe wrappers around both functions, retaining their normal scheduler
and trace behavior. It records all 128 MB as page deltas at dispatcher entry,
each task entry/return, and dispatcher exit. The second entry also exposes
changes made outside the dispatcher between passes. Kernel ordinal counters
and presentation counts help identify external work; they do not enumerate
every possible host side effect.

At the time of this audit, the live Practice task table contained 15 active tasks,
including the five original explicit continuations. The ten other routines were `87420`,
`72070`, `54110`, `BE4A0`, `235B0`, `1C60AB`, `E8030`, `49790`, `691F0`, and
`BB500`. These cannot be assumed irrelevant to rollback just because the five
isolated task tests pass.

```powershell
python doa3/tools/rollback_frame_report.py doa3/build/rollback-frame.rbf --output doa3/build/rollback-frame-report.json
```

This audit leaves clocks, APU, inputs and task ordering unchanged. Asynchronous
writers remain active, so its memory copies are **observed boundary deltas**,
not atomic snapshots or proof that the named task caused every changed byte.
The reader reports all bytes without masking and lists page overlap with the
five converted tasks; overlap alone does not establish read dependencies.
It rejects stale source fingerprints, invalid ordering/slots/pages, truncated
streams and missing completion footers. Production-writer/reader tests cover
complete passes, an unconverted task's writes, kernel/presentation effects,
inter-pass changes, invalid slots and malformed files. Live validation of this
full-dispatcher capture passed its live format/ordering validation on 2026-10-16.

The two recorded passes each ran all 15 active tasks. There was one presentation
and 13,883 changed bytes between the passes. The largest observed task deltas
were under `E8030`, `49790`, and `691F0` (roughly 285 KB, 582 KB, and 354 KB summed
across both passes), including guest graphics-command buffers. These numbers
describe changed bytes in the instrumented capture, not normal frame costs.
Task `98DB0` also coincided with kernel synchronization/memory calls, so its
earlier isolated task success does not cover all host interactions on all paths.
Evidence: `build/rollback-frame.rbf` and `build/rollback-frame-report.json`.

Based on that audit, `235B0` (camera state/matrices) and `BB500` (guest rumble
request consumption/feedback state) now use the same explicit-continuation
mechanism. The camera still executes its original calculations; the rumble task
still commits its original feedback requests. This does not suppress host motors
during replay or make rendering replay-safe.

Expanded live validation (2026-10-16): 42/42 task samples matched, six per each
of the seven continuations, including repeated executions and all guest-memory
bytes/registers/continuation state. The user reported normal camera, controls,
and rumble after the bounded diagnostic hitches. Capture completed and disabled
itself. Evidence: `build/rollback-practice-expanded.rbc` and
`build/rollback-expanded-camera-report.log`.

The camera comparison uses the checker's explicit `--native-camera` profile:
`29E30` reproduces the original camera activation path, while free-camera and
custom FOV overrides are outside that profile. The default checker blocks that
hook. The profile still compares every byte and every captured register/local
against the actual live result; a differing override result fails comparison.
This establishes the captured native-camera path, not debug-camera replay or a
general host-state snapshot. No game executable change was needed to analyze
these saved samples.

#### Two streaming-worker outer continuations (2026-10-17)

`DOA3_ROLLBACK_STREAM_WORKERS=1` together with `DOA3_ROLLBACK_TASKS=1`
converts the outer loops at `1926F0` and `192760`. Their original service calls,
callbacks and shutdown paths remain in place. The generated continuations save
root locals around the audited `1DC660` D3D vblank wait; a small adapter preserves
its guest stack writes, event clearing, counter increment and return registers,
including the kernel thunk's controlled-clock call accounting.
Generation rejects a changed D3D wait body or missing entry overrides.

The adapter retains the existing kernel policy: ordinary workers block on host
vblank, while controlled-clock workers yield cooperatively. It does not introduce
a new deterministic worker rate. Combined local scheduler checkpoints now include
these workers' continuations and READY/WAITING state. Capture is refused inside a
native helper; a nested helper yield permanently invalidates older checkpoints,
even after the helper unwinds. Restore also rejects a changed park kind.

The worker test compares both original translated roots running on Windows fibers
with their generated continuations over 40 waits in each clock mode. All 128 MB
of guest RAM and all eight saved integer registers match, including two replays
from the midpoint. Service and callback helpers in this comparison are fixtures;
this is not a full CRI streaming replay. The production scheduler test separately
checks repeated worker/task replay, wake-state restoration and rejection across
nested native waits.

Together with the watchdog, three of the four observed worker outer loops now
have explicit boundaries. `1927D0` remains native; inner helper continuations,
combined atomic RAM/clock/APU/file/completion snapshots, full-frame replay and
effect reconciliation remain unfinished. No live rewind or network play is enabled.
Runtime diagnostics remain opt-in, with no large memory capture required.

Validation: all 22 component tests passed (117.31 seconds). After adding the
clock-accounting assertion, worker, scheduler and clock tests passed again.
The game and isolated replay checker build successfully. The startup probe
observed 119 iterations of each new worker over two seconds, zero nested worker
yields, and ten queued/delivered completions. This confirms the live hooks are
active, not that a whole live frame can rewind.
The subsequent fight sample recorded all 15 converted tasks and both streaming
workers advancing 120 times over two seconds, with zero nested worker waits and
three queued/delivered completions (177 total, equal). Visual/audio confirmation
from the user subsequently passed: they reported that it works well and asked
to continue. See `build/rollback-stream-workers-followup-probe.json`.
Evidence: `build/rollback-stream-workers-tests.log`,
`build/rollback-stream-workers-accounting-tests.log`,
`build/rollback-stream-workers-main-build.log`,
`build/rollback-stream-workers-runtime.log` and
`build/rollback-stream-workers-live-probe.json`.

#### File-worker self-suspend continuation (2026-10-17)

The same streaming-worker opt-in now also converts `1927D0`, the fourth observed
worker including the watchdog. Its service, priority-change, callback and exit
instructions remain the original translated code. The explicit boundary is the
outer `18C522` SuspendThread call, with a separately audited adapter for the
3.1 `NtSuspendThread` bridge. Generation checks the wrapper's exact body and
the worker entry hook before building.

This boundary preserves the guest stack, the previous-suspend-count write into
the caller's handle argument, the kernel-call counter, and the original
self-suspend semantics. Zero and current-thread pseudo-handles map to the
worker's own handle. Suspending another thread remains an immediate no-op with
no added yield; a true self-suspend parks on its own handle for normal resume.
No resume credits or new worker timing policy are introduced.

The expanded original-versus-converted test executes all three streaming roots
for 40 waits in both clock modes and repeats each second half twice from a
checkpoint. All guest RAM, integer registers and kernel-call accounting match.
It exercises zero, pseudo, own and other-thread handles, callback/no-callback,
priority-change and repeat-without-suspend branches. Service/callback helpers
remain explicit fixtures, so this is not certification of actual file IO replay.
The production scheduler test additionally combines a task and two streaming
workers, repeats 30 directed-resume/wake cycles three times, restores WAITING
state, and rejects checkpoints after nested native waits or termination.

All four observed worker **outer boundaries** are now represented. Inner native
helper suspensions still invalidate checkpoints. Atomic whole-frame state,
kernel object restoration, IO session integration and effect reconciliation
remain necessary before live match rewind or playable rollback can be enabled.
The read-only probe reports each streaming worker separately. No large capture
is enabled for this validation build.

All 22 component tests passed in 81.26 seconds
(`build/rollback-file-worker-tests.log`). The game build succeeded, and startup logged the fourth worker's explicit entry
on fiber 4. The first read-only probe measured 117 iterations of each streaming
worker over two seconds, no nested worker yields, and seven queued/delivered
completions (22 total, equal). The user confirmed normal Practice/audio behavior
with the four-worker build. A follow-up two-second sample recorded all 15 fight
tasks advancing 120 times, each streaming worker advancing 119 times, zero nested
worker yields, and three queued/delivered completions (250 total, equal). This
validates ordinary forward execution of the enabled paths, not live rewind.
Evidence: `build/rollback-file-worker-practice-probe.json`,
`build/rollback-file-worker-main-build.log`,
`build/rollback-file-worker-runtime.log`, and
`build/rollback-file-worker-live-probe.json`.

#### Coordinated checkpoint transaction (2026-10-17)

`src/online/rollback_checkpoint.c` coordinates six explicitly bound domains:
read-only file cursors, guest RAM, converted fibers, deterministic clock,
completion queue and audio. This is a process-local checkpoint transaction,
not a complete game adapter. Creation requires all six implementations and an
owner-thread acquire/release gate that pins every writer for the entire operation.
There is no automatic caller in the game and no new capture environment switch.

Checkpoints carry a session identity/generation, epoch, frame, component sizes
and integrity hash. Capture assembles a private image and publishes only after
all components validate. Restore copies its input, checks all component state
without mutation, and captures/validates a complete before-image before applying
any target state. Files apply first, then RAM, with the clock afterward so its
exported tick can be republished. If an apply fails, every component is restored
from the before-image in dependency order. Failed compensation or an unexpected
epoch change poisons the session and forbids more captures/restores. The owner
must stop simulation on poison; creating another session does not repair state.

Read-only validation APIs were added to the real fiber, clock, APC, file and APU
snapshot implementations. Audio import uses the same validation logic as before;
its private replay backend now validates without temporarily importing into a
throwaway device. Live hardware-audio restoration is still unsupported.

The transaction test injects capture, validation and post-mutation restore
failures at each domain, rejects corruption of every byte of a small bundle,
checks size/session/epoch/ownership/reentrancy rejection, and exercises poisoned
sessions. The integration harness binds the production Windows-fiber scheduler
(one task and four workers), clock, APC queue, read-only temporary file cursors,
all 128 MiB guest RAM, and the private production APU/VP/passthrough processing.
The task and stream-service bodies in this harness are fixtures, not DOA3's full
fight execution. Heap allocation occurs only during setup; kernel objects, GPU
state/resources, real input and presentation are outside this harness.

Three 16-frame replays from one coordinated checkpoint match the complete bundle
and PCM. The checkpoint includes two undelivered completions and a partial PCM
period. Failures injected after each real domain has restored recover exactly
the previous combined state; a completion-queue epoch reset rejects the target
before files or RAM change. A separate run seeded with the existing captured
game RAM/APU image also passed, with 12,800 nonzero stereo samples reproduced.
This is repeatability of the private mixed fixture, not comparison against a
live whole-fight result. Evidence: `build/rollback-checkpoint-targeted-tests.log`
and `build/rollback-checkpoint-audio-seed-test.log`.

The affected production libraries compile. No live rewind, audio-device restore,
or replacement of the running game is needed for this step. The remaining work
is a live ownership/boundary adapter with complete host-state coverage, followed
by a full-frame replay comparison and safe effect reconciliation. The large
transaction buffers and integrity hashing are correctness scaffolding; no claim
of a 60 Hz checkpoint budget is made yet.

Validation completed: all 24 component tests passed in 85.29 seconds, and the
strengthened valid-size reentrant-call test passed separately afterward.
Evidence: `build/rollback-checkpoint-full-tests.log`,
`build/rollback-checkpoint-reentrancy-test.log`, and
`build/rollback-checkpoint-production-build.log`. Source fingerprints now also
include the coordinator and deterministic clock implementations so later changes
cannot silently reuse incompatible captures.

#### Allocator checkpoint and seventh domain (2026-10-17)

The coordinated transaction now also requires heap bookkeeping (`DOA3_CP_HEAP`),
after RAM and before fibers; its local bundle format is version 2. The allocator
was moved from `xbox_memory_layout.c` to `xbox_heap.c` without changing allocation,
free/coalescing, rounding, zero-fill or top/high reservation policy. Its former
private mapping offset is accessed through the existing getter. Normal allocation
and freeing do not perform any checkpoint work.

The opt-in owner-thread heap session saves the low bump pointer, reserved ceiling,
high-heap pointer, complete live-allocation hash table (including tombstones),
free list and accounting/diagnostic counters. Capture validates before publishing;
restore validates before changing bookkeeping. Validation checks bounds, sizes,
counts, accounting, disjoint live/free spans and reachable hash probe chains.
Session generation, mapping generation and host offset reject stale snapshots;
memory layout initialization/shutdown invalidate sessions even on same-address
remapping. Tracker exhaustion disables capture and restore rather than silently
continuing with incomplete allocation state. Scratch storage and sorting are
confined to explicit checkpoint calls.

The integration fixture now uses the production allocator for fiber stacks and
for low-heap allocate/free, high allocations and top reservations during each
replayed frame. Allocation references live in the fixture's guest RAM. The clock
also publishes its tick into that RAM, instead of a separate test variable.
Three 16-frame replays match the complete seven-domain bundle and PCM; failures
after each real domain applies still compensate to the identical before-image.
The captured RAM/APU seed run reproduced 12,800 nonzero stereo samples. This seed
supplies audio data, not a captured live heap: the harness creates fresh allocator
bookkeeping and still uses fixture task/service bodies.

The allocator-specific test checks three repeatable allocation sequences and
their entire 128 MiB RAM contents, fragmentation/coalescing, aligned reuse,
zero-filled allocations, high/top addresses and allocation-failure accounting.
Malformed spans, overlaps, unreachable hash entries, wrong-thread calls, stale
session/mapping and real tracker exhaustion are rejected. The affected production
libraries compile. Evidence: `build/rollback-heap-targeted-tests.log`,
`build/rollback-heap-audio-seed-test.log`, and
`build/rollback-heap-production-build.log`.

All 25 component tests passed in 87.71 seconds
(`build/rollback-heap-full-tests.log`). Task-hook regeneration and `git diff
--check` also passed. The extracted allocation/free implementation was compared
against the original: only its block typedef and memory-offset accessor differ.

This restores allocation metadata, not host object lifetimes, mappings or VM
protections. The eventual live adapter must pin all allocator users and validate
every host object retaining guest heap addresses before committing RAM and heap
together. GPU resources/queues, kernel objects, input ownership and effect
reconciliation remain outside this transaction's complete-game coverage. No live
rewind is enabled, and the current executable need not be replaced to run these
private tests. Full-RAM buffers, structural validation and hashing remain
correctness scaffolding, not a measured per-frame rollback implementation.

#### Interrupt handlers and deferred callbacks (2026-10-17)

`kernel_interrupt.c` now owns the registered guest ISR metadata and pending DPC
queue previously private to `kernel_bridge.c`. The bridge routes initialization,
enqueue, enumeration and FIFO removal through this shared implementation. The
four-handler/sixteen-DPC limits, duplicate rejection, FIFO order and historical
guest result on a full queue are preserved. Overflow or invalid entries taint
checkpointing rather than certifying lost work. Normal delivery can continue.

The existing `doa3_apu_deliver_irq` body now brackets delivery through this
component, replacing its private reentry flag. Capture/restore/reset refuse
while an ISR or DPC is running. Newly queued DPCs remain eligible in the same
drain, as before. The production checkpoint wrappers additionally require the
deterministic session, primary fiber and passive IRQL; the clock epoch is part
of the snapshot identity. Queue reset establishes a new generation and owner
thread. Snapshot validation checks counts, active entries, duplicate DPCs and
zero inactive slots before mutation. No callbacks execute during restore.

The coordinated transaction now has eight required domains, adding interrupt
metadata before audio, with local bundle format version 3. The mixed fixture
saves two pending DPCs and two pending file completions. Three 16-frame replays
match all eight domains and PCM; post-apply failures at every domain compensate
exactly. An active interrupt-delivery scope rejects the entire restore without
changing the combined state. The captured RAM/APU seed again reproduces 12,800
nonzero stereo samples. Callback bodies in the mixed fixture remain fixtures.

An additional test compiles the actual production `doa3_apu_deliver_irq` body,
extracted at build time, with private callback functions and an IRQ/IRQL fixture.
It verifies interrupt masking, ISR-before-DPC order, DPCs queued by callbacks,
stack/IRQL restoration, reentry rejection and refusal to checkpoint during
callbacks. Component tests cover replay of pending work, malformed/stale
snapshots, wrong-thread access, duplicates and both capacity limits. Evidence:
`build/rollback-interrupt-targeted-tests.log`,
`build/rollback-interrupt-audio-seed-test.log`, and
`build/rollback-interrupt-production-build.log` (affected libraries compile).

All 26 component tests passed in 83.55 seconds
(`build/rollback-interrupt-full-tests.log`). Task-hook generation and
`git diff --check` passed as well. The running game was not replaced for this
offline validation step.

This step does not restore the live hardware IRQ latch or audio device. The
private APU replay owns its own latch; live audio ownership still needs an
adapter. It also does not make arbitrary native callback stacks restorable:
checkpoints are excluded while delivery is active.

Audit notes for the remaining host state:

- Guest `KeSetEvent`/`KeWaitForSingleObject` signaling in the bridge uses guest
  RAM and fiber waits. `NtCreateEvent` and semaphore wrappers instead use Win32
  objects, whose signal/count state and lifetimes are not covered here.
- `kernel_hal.c` keeps IRQL in host TLS and mirrors it into guest RAM. The new
  production wrapper permits only passive-IRQL checkpoints, rather than
  pretending RAM restoration rewinds TLS at arbitrary instruction boundaries.
- `Nv2aQueueSnapshot` covers synchronous USER/PFIFO registers, push-buffer parser
  state and target-routing metadata. Its import helper is not a whole-renderer
  restore: PGRAPH state, D3D resources, texture contents and presentation still
  require separate ownership and reconciliation work.

No live whole-fight rewind is enabled by these changes.

#### Coordinated GPU command replay (2026-10-17)

`nv2a_frontend.c` adds a private, owner-thread GPU command component built from
the production USER/PFIFO register helpers, push-buffer walker and render-target
routing helper. It owns the queue registers, parser cursor/ring state, ordered
method digest and target offsets. It creates no window, D3D device or textures.
PGRAPH methods are hashed in order, not executed: this verifies command replay,
not pixels or complete GPU effects.

Local snapshots bind the instance/generation and deterministic session epoch.
Read-only preflight rejects malformed queue payloads, invalid ring/cursor values,
unsupported parser modes and duplicate/out-of-range active target offsets.
Capture/restore refuse wrong-thread and reentrant calls, including attempts
from the guest-memory callbacks during submission. The existing raw queue
schema remains 4; its import validation was split into a read-only helper
without changing its accepted format or normal production register behavior.

Supported operations include USER channel aliases, PFIFO status masking, ordered
commands, called push buffers, ring wrap, guest semaphore writes and target
routing updates. Guest memory accesses are bounded to 128 MiB; device/ring
arithmetic and cursor alignment are checked before command processing. Flips,
PFIFO interrupt writes, unhandled MMIO regions/widths, uninitialized channels
and malformed streams latch a fault and refuse further checkpointing/restores.
A failed operation may have changed private RAM: callers must stop/discard the
run, not clear the fault and continue. This is not a live renderer restore API.

The coordinated checkpoint now requires this ninth domain before audio (local
transaction bundle version 4). The integration fixture submits two commands per
frame, wraps the ring every eight frames, checks USER progress and guest fence
values, and changes target offsets. Three 16-frame replays reproduce the entire
nine-domain bundle, all 128 MiB RAM and PCM. Failures injected after each domain
has applied still compensate to the identical before-image. The RAM/APU-seeded
run again reproduces 12,800 nonzero stereo samples; GPU commands and task bodies
in this harness remain fixtures, not a recorded whole fight.

The new component test covers called buffers, ring wrap, pending parser state,
target routing, MMIO aliases/status, exact repeatability, rejected/stale images,
wrong-thread/reentrant access and latched unsupported-operation failures. The
existing native MMIO decoder test also passes against the shared queue validator.
Evidence: `build/rollback-gpu-frontend-targeted-tests.log`,
`build/rollback-gpu-frontend-mmio-test.log`,
`build/rollback-gpu-frontend-audio-seed-test.log`, and
`build/rollback-gpu-frontend-production-build.log` (the updated online library
compiles). No replacement of the running game or live rewind is enabled.

All 27 component tests passed in 83.54 seconds
(`build/rollback-gpu-frontend-full-tests.log`), plus the separate native GPU
MMIO test. Task-hook generation and `git diff --check` also passed.

Renderer audit for the next stage: `NV2AState.pgraph` is still a register stub;
the translator's real draw state is in `nv2a_pgraph_d3d11.c` (`g_pg`, `g_nvreg`,
surface/blend/framebuffer-offset globals and related state). `g_pg` mixes logical
render state, vertex-program/constants, inline/index accumulators and lighting
with native texture pointers, texture caches and statistics. Copying that
structure wholesale would not restore D3D resources or rendered pixel contents.
Logical state must be separated from resource ownership, with explicit handling
for resource caches, render-target/depth contents, frame flips and presentation.

### Pointer-free renderer state (2026-10-17)

The translator now stores its draw registers, vertex program/constants,
lighting and inline/index accumulators in `Nv2aPgraphDrawState`. Native texture
pointers, cache entries and upload metadata, statistics and initialization stay
in `g_pg_host`. Former logical globals (raw register shadow, surface routing,
framebuffer offsets and layout flags) live in `Nv2aPgraphAuxState`. This also
includes the last valid source/destination blend factors: these were previously
function-local statics whose history affected later draws with invalid factors.
The externally shared model-view matrix is copied explicitly, not relocated.

The split preserves existing initialization lifetimes and rendering statements.
A one-off comparison against the pre-edit source confirms that executable
statements changed only in state locations and the split zero-initialization.
No texture invalidation, synchronization, rendering policy or cache eviction
behavior was changed. The production executable was rebuilt successfully.

`nv2a_pgraph_state.c` provides a versioned, pointer-free **private** logical
image with accumulator-bound validation and rejection before destination writes.
Raw register bit patterns, including NaNs and out-of-range VP load pointers,
remain intact. This low-level copy requires owned/quiescent, non-overlapping
buffers; it is not an authenticated/session-bound checkpoint or a live restore
API. There is no tenth coordinated domain and no live rewind activation.

`rollback_pgraph_state_test` compiles the actual production initialization,
method dispatcher, vertex-program writers, flush and blend-application code.
Draw submission and device Clear/SetRenderState calls use private test sinks.
Three replays from a partially submitted batch reproduce the complete logical
image and submitted batch exactly while leaving native cache/pointer ownership
untouched. Coverage includes lighting, all four texture stages, viewport/clip
registers, matrices/program/constants, layout/combiner markers, clears, full
accumulators, dropped indices, remembered blend fallback, invalid images and
every dispatch-table entry's state and return classification. This proves
command-state replay, **not** restored pixels or a recorded whole fight.

All 28 rollback component tests passed (251.26 seconds in this run).
The clipping, reflection-mask, point-sprite WARP, HUD fade/timer edge and video
settings/HUD layout suites also pass (seven tests across five suites). Logs:
`build/rollback-pgraph-state-tests.log`, `build/rollback-pgraph-*-tests.log`
and `build/rollback-pgraph-production-build.log`. The capture source fingerprint
now includes the translator, logical image and vertex-program implementation.

Next: reconcile texture uploads/cache validity and D3D bindings after RAM
restore, and define which render-target/depth contents must be preserved or
recreated before replay. Frame flips, presentation, debug/configuration state
and host D3D resource lifetimes remain outside this private image. Only after
that work can the real renderer join the coordinated restore/replay adapter.

### GPU image preservation and guest texture reload (2026-10-17)

`d3d8_replay_images.c` adds opaque, owner-thread GPU image checkpoints. The
production inventory adapter in `d3d8_render_checkpoint.inc` includes the guest
color/depth targets, all eight offscreen color/depth pairs, the framebuffer
sampling copy, and previous-frame history (20 possible images). Swap-chain and
host UI images are excluded. Restore reinstates the captured target routing,
viewport and history-valid flag, and clears the four sampled texture bindings
so PGRAPH can rebuild them from restored state before drawing.

The copies retain their original resources and context, preventing pointer
reuse from disguising a replacement. Validation checks owner thread, nonzero
caller-supplied session epoch, immediate context, resource identity, occupied
slots and guest target keys before any destination mutation. Resizes, resource
replacement/creation, malformed inventories and unsupported image formats are
rejected; resources are not recreated. Capture allocation failures clean up
partial copies. A detected device failure after restore copies begin returns
an irrecoverable result and poisons that image checkpoint. GPU work remains
ordered on the same immediate context; this is not a disk/wire format.

`pgraph_d3d11_invalidate_guest_uploads` is the separate restore barrier after
RAM restoration. It preserves native cache allocations but forces complete
guest mip/palette reuploads on subsequent binds, bypassing the sampled base-
level hash. The normal uploader now marks a cache entry valid only after all
levels upload successfully; failed locks, unlocks or unswizzle allocations
leave it invalid for retry. A different mip count at the same cache key now
replaces the incompatible native allocation.

The WARP test compiles production offscreen allocation, binding, feedback and
checkpoint adapter code. It fills all 20 images with distinct colors/depth/
stencil values, changes every image, and restores three times with matching
full readback hashes. It checks restored viewport/target, cleared sampled
bindings, absent/later-created history, wrong-thread/session/key/resource
rejection, unsupported formats/deferred contexts, partial allocation cleanup
and final COM reference cleanup. The guest-upload test compiles the real format,
cache, signature and upload code with memory-backed texture sinks. It checks
changed lower mips with identical level zero, changes missed by the sampled
hash, palettised data, changed mip count and failed-upload retries. The latter
test validates uploaded bytes, not GPU shader sampling.

All 30 rollback tests passed in 231.51 seconds; the separate production frame-
history WARP test also passed. The executable rebuilt successfully, task-hook
generation passed, and `git diff --check` found no whitespace errors.
Evidence: `build/rollback-resource-tests.log`, `build/rollback-resource-history-tests.log`,
`build/rollback-resource-production-build.log`. These APIs have **no live game
caller**, and allocate no checkpoints in normal play. All renderer writers
must be stopped by the future adapter before capture/restore. Immutable shader
objects can remain cached, but D3D logical state, ownership boundaries and
binding reconstruction still need integration with the PGRAPH image. These GPU
handles are not yet a domain in the nine-domain coordinator. No whole-fight
rewind or network rollback is enabled, and copy memory/time is not budgeted for
per-frame use. Next: combine logical state, RAM/cache invalidation and GPU image
ownership into a coordinated renderer restore with compensation and actual
draw/replay validation; then handle resource-inventory changes explicitly.

### Owned renderer checkpoints in coordinated restore (2026-10-17)

The coordinator now accepts an optional renderer resource alongside its nine
byte domains. `Doa3CheckpointImage` owns both the byte image and an opaque GPU
sidecar; discard and session destruction release the associated resources.
The byte-only APIs refuse operation when a renderer is configured, so callers
cannot accidentally restore RAM while omitting GPU images. The internal byte
bundle version is now 5. These are local, session-owned handles, not serialized
or transferable snapshots.

All capture, validation, restore and cleanup callbacks share the coordinator's
owner fence. Before committing, the coordinator validates every domain and
captures a renderer before-image. If any byte domain or the renderer fails
after mutation, it compensates all nine byte domains in dependency order,
then restores the renderer before-image. Failed compensation, an irrecoverable
restore or an epoch change poisons the session. Temporary images are released
on both success and failure. Discard remains available after poison or epoch
changes, but still requires the owner fence; destroy leaves the session alive
if that fence refuses acquisition.

`nv2a_renderer_checkpoint.c` packages the private PGRAPH logical state,
auxiliary registers and model-view matrix with the real D3D image inventory.
It rejects an unfinished draw batch, a changed source descriptor, wrong owner
or epoch, and incompatible GPU inventory before applying state. A successful
restore reinstates images and logical state, then invalidates guest uploads
so restored RAM supplies all mip levels and palettes on the next bind.

The new WARP transaction test uses actual D3D11 draws, production texture
creation/upload/cache code and production render-target/history checkpoint
code. Three replays reproduce exact image readback hashes. Injected failures
after each byte domain and after the renderer reproduce the previous RAM,
GPU images and next draw. A negative control deliberately skips upload
invalidation: a changed lower mip with unchanged level zero then produces
different pixels, while the normal restore reproduces the reference. Resource
counts and final COM cleanup are checked, including poisoned-session cleanup.
Coordinator tests also cover cross-session handles, reentrancy, denied cleanup,
failed capture and repeated capture/restore/discard.

All 31 rollback tests passed in 250.77 seconds, and the production executable
rebuilt successfully with regenerated task hooks. `git diff --check` passed.
Evidence: `build/rollback-owned-tests.log`,
`build/rollback-owned-full-build.log`, and
`build/rollback-owned-production-build.log`.

This test supplies fixture byte domains and rebuilds its own D3D pipeline for
each draw. The separate CPU/APU integration test exercises the genuine nine
domains. Neither test is a recorded fight, and the new adapter has **no live
game caller**. D3D backend shader/FVF state, stream bindings and pipeline-cache
reconstruction are not captured by this adapter. Resource-inventory changes
still reject restore rather than recreating targets. Per-frame copy cost and
memory remain unbudgeted. Next: restore or reconstruct the production backend
state and verify replay through the actual PGRAPH draw submission path before
connecting a bounded live fight rewind.

### Backend pipeline restore and production draw replay (2026-10-17)

The private renderer checkpoint now also owns a pointer-free D3D backend image:
render/texture-stage states, transforms, viewport, material/lights, scene state,
shader/FVF selections, scissor rectangles, polygon offset and blend factors,
register-combiner state, and the programmable vertex-shader registry/constants.
The image validates its layout, scissor count, combiner stage counts and vertex
program slot bounds before mutation. It is fenced and authenticated by the
existing renderer/coordinator handle, not a separate wire or disk format.

Restore clears native D3D11 bindings, restores the logical backend state, then
restores GPU targets/viewport and PGRAPH state and invalidates guest uploads.
Sampler binding keys are invalidated so an unchanged sampler descriptor cannot
skip rebinding after ClearState. Programmable vertex constants are marked dirty
for reupload. Immutable shader/state caches remain allocated; draw preparation
rebinds shaders, layouts, constants and render states. Dynamic vertex/index
upload rings keep advancing, avoiding overwrites of GPU work still in flight.
All new restore functions remain unused during normal gameplay.

Bound guest D3D8 vertex/index buffers are explicitly **unsupported**: capture
and validation reject them rather than retain unrestored mutable buffers.
PGRAPH's UP and GPU fixed-function paths upload their own vertices and indices
and do not require such guest bindings. Target-inventory/size changes remain
rejected by the existing GPU image adapter. A future live adapter must enforce
these boundaries or extend resource preservation before enabling restore.

`rollback_pgraph_draw_replay_test` compiles the complete production PGRAPH
translator plus unchanged device getters/setters, target management, draw and
ring-upload functions. It links production resource, shader, sampler/state,
register-combiner and vertex-program code against a WARP device. Only window,
movie, debug-overlay and game services are fixture replacements. Synthetic
method sequences execute inline textured draws, render-to-texture sampling,
and indexed geometry through the GPU fixed-function and register-combiner
shaders. Draw counters and pixel changes confirm these paths actually execute.

Three restore/replay runs reproduce the image hashes after changing backend
state, adding a vertex program, changing constants and clearing native bindings.
The complete backend image also matches immediately after restore. Leaving the
future wireframe setting in place deliberately changes the output; the normal
restore reproduces the reference again. Unsupported buffer bindings and malformed
vertex-program state reject restore, and final COM references return to zero.
The earlier coordinated failure/compensation test now includes this backend
image while retaining its separate fixture shader for the mip-invalidation test.

All 32 rollback tests passed in 251.43 seconds. The final stricter indexed-pixel
check was rebuilt and passed separately after adding the fixture's missing
fixed-function shader initialization. The point-sprite WARP and combiner-cache
regression suites also pass. The production executable rebuilt successfully
with regenerated task hooks, and `git diff --check` passed. Logs:
`build/rollback-pipeline-tests.log`, `build/rollback-pgraph-draw-test.log`,
`build/rollback-pipeline-full-build.log` and
`build/rollback-pipeline-production-build.log`.

This verifies production draw execution with synthetic commands, **not a
recorded fight or live rewind**. The programmable vertex-shader registry is
round-tripped, but programmable vertex draws are not covered by this new pixel
test. Next: expand command coverage and audit resource-inventory changes and
renderer ownership at the real frame boundary before connecting a bounded
whole-fight restore/replay test. Rendering time and snapshot memory still need
measurement before per-frame rollback can be enabled.

### Live renderer boundary audit (2026-10-17)

`DOA3_ROLLBACK_RENDER_AUDIT=<absolute .csv path>` now observes the real
`0x1C650C` dispatcher entry/return independently of the large memory captures.
It waits for 120 eligible solo-Practice passes, buffers 240 observations
(120 complete passes), writes one CSV, and disables itself. Leaving eligible
Practice after sampling begins terminates with a partial report. Unpaired
entry calls and owner-thread mismatches also terminate the run.

Each observation reports the unsupported-condition bitmask, number of GPU
images, estimated uncompressed bytes per image set, and a process-local
inventory fingerprint. Flags: owner=1, device=2, bound guest VB/IB=4,
incomplete inventory=8, unexpected target bindings=16, viewport count=32,
unsupported image format/layout=64, unfinished PGRAPH draw batch=128.
The fingerprint includes device/context, all 20 resource slots, and offscreen
keys. It is only a diagnostic for observed changes, not an authenticated
identity, generation counter, or restore token. Image bytes exclude staging,
driver overhead and other checkpoint domains.

The owner is recorded at PGRAPH initialization; the probe refuses to query
the immediate context on a different thread. It reads descriptors/bindings
without copying images, waiting for the GPU, or changing gameplay state.
Zero flags do **not** establish exclusive ownership or fence asynchronous
writers. This audit does not enable any live restore. Run with the existing
experimental task/worker switches when investigating the converted paths.

Validation: the production PGRAPH/WARP test checks wrong-thread rejection,
inventory-key changes, missing depth surfaces, unbound targets, missing
viewports, bound vertex buffers, unfinished draw batches, eligibility gating,
bounded completion and partial termination. Pixel hashes before/after the
audit match, and COM cleanup still passes. All six affected renderer/frame
tests passed; the production executable rebuilt successfully.
Build logs: `build/rollback-boundary-test-build.log` and
`build/rollback-boundary-production-build.log`.

Live Practice audit completed on 2026-10-17 at 21:43:54 local time:
`build/rollback-render-live-boundary.csv` contains 240 observations (120 entry,
120 return), all with zero flags and one unchanged inventory fingerprint.
All samples contained seven GPU images totaling 45,844,224 uncompressed bytes
(43.72 MiB). The runtime log confirms completion with zero blocked samples
and zero inventory changes, along with the converted task and worker paths
active. This is evidence for this short Practice window only; it does not
cover transitions or establish a writer fence or deterministic live replay.

At this observed size, eight independently copied image sets would require
about 349.76 MiB before guest RAM, other host state, staging and driver costs.
This is a storage estimate, not measured allocation or copy time. Next is
explicit ownership/quiescence integration across all snapshot domains and a
bounded live capture/validation check before enabling whole-fight rewind.

### Clock/APU transaction pins and audio handoff (2026-10-17)

The APU output path temporarily releases its mutex while waiting for XAudio2
or waveOut. Holding that mutex therefore did not prove that the previous
chip frame had finished. Deterministic ownership now requests a handoff,
waits for the complete frame to unwind, and prevents another background
frame from starting during that handoff. Host-only frame/pending markers
remain outside the serialized command image.

New nonblocking, nonrecursive APIs pin the deterministic clock and APU
command backend: `xbox_det_checkpoint_acquire/release` and
`mcpx_apu_rollback_acquire/release`. Acquire clock first, then APU; release in
reverse order, including failure cleanup. Clock acquisition requires the
session owner on the primary fiber. Audio acquisition requires its session
owner and a completed chip frame. Captures can run while the pins are held;
stepping and owner-side session changes are rejected until release. The
clock pin blocks both host timer writers. These pins do not authorize
yielding or establish ownership of every other game subsystem.

`DOA3_ROLLBACK_OWNERSHIP_AUDIT=1`, together with the experimental task and
worker switches, enables a single live diagnostic after 120 eligible
solo-Practice passes. It temporarily starts deterministic clock/APU ownership,
acquires both pins, compares two clock/APU-command/converted-fiber captures,
validates their images, and releases ownership before returning to gameplay.
It logs component results, the captured fiber count, byte sizes and elapsed
time. It neither copies all guest RAM nor restores state. Other memory
captures cannot be armed simultaneously. A rejected acquisition terminates
this diagnostic rather than repeatedly interrupting the game.

The concurrency test holds a simulated audio-device wait open inside the
production frame body, verifies ownership cannot transfer until that frame
finishes, then checks wrong-thread/nested pin rejection, writer exclusion,
stable double captures, lock-contention cleanup and resumed stepping. The
clock test verifies a real host-timer thread stays blocked until release.
The audit harness uses the real clock/APU functions and a small parked-fiber
fixture; real scheduler tests remain separate. Ten affected tests passed;
the final audit harness and frame-hook tests were checked separately. The
production executable rebuilt successfully.

The live Practice run passed: both pins acquired, both clock and audio-command
captures matched, and all 19 captured continuations were stable and validated.
The audio-command image was 1,005,552 bytes and the converted-fiber image was
301,624 bytes. The one-time handoff/capture/check/release interval measured
1.335 ms (allocation and subsequent frees excluded); this is not a whole-frame
snapshot or replay-cost measurement. The user reported normal play. The log
subsequently records a user-initiated window close, not a crash. Evidence:
`build/rollback-ownership-live-stderr.log`, `[ROLLBACK-OWNERSHIP] passed`.

This still does **not** provide live audio restore: the host mixer, pending
output samples and device queue require separate treatment. No complete
live fight rewind or deterministic fight replay is claimed. Logs:
`build/rollback-audio-ownership-tests.log`,
`build/rollback-ownership-audit-tests.log`, and
`build/rollback-audio-ownership-production.log`.

### Expanded live state audit (2026-10-18)

Set `DOA3_ROLLBACK_OWNERSHIP_AUDIT=2` to extend the same one-time Practice
check to guest RAM (128 MiB), allocator metadata, leased read-only file
positions, pending stream completions, interrupt/DPC metadata and the real
USER/PFIFO/submission queue export. Mode 1 retains the smaller clock/APU/fiber
check. Both remain opt-in and perform no restore or gameplay replay.

The larger audit preallocates two sets of memory/metadata storage before
entering the clock/APU ownership window. It pins read-only files after starting
the deterministic clock, then captures two full passes while holding the
clock and APU guards. Original clock/audio/fiber comparisons surround this
additional work. Each domain must capture twice, validate both images and
compare byte-for-byte. File/completion bridge adapters now expose read-only
validation as well as capture/restore. A refused file session does not prevent
observing other components, but the overall audit cannot pass.

`[ROLLBACK-STATE]` reports each result and a six-bit stable mask: RAM=1,
heap=2, files=4, completions=8, interrupts=16, GPU queue=32; all pass is `3F`.
The original `[ROLLBACK-OWNERSHIP]` pass additionally requires those six
results. Buffers and any acquired file leases are released after the check.
No memory images are written to disk. There may be one hitch from allocation,
copying and checking roughly 256 MiB of RAM snapshots. Reported elapsed time
excludes initial allocation and final cleanup and is not a rollback cost.

The new test uses real allocator, file-lease, APC, interrupt and queue code
with private bridge bindings. It verifies stable capture, wrong-thread
rejection, individual RAM/file/APC/interrupt/GPU mutations, failed exports,
malformed GPU images, refused writable-file bindings and lease cleanup.
All ten affected tests pass, and the production executable rebuilt. Logs:
`build/rollback-state-audit-tests.log`,
`build/rollback-state-audit-fixture-build.log`, and
`build/rollback-state-audit-production.log`.

The live Practice run captured stable RAM, allocator metadata, stream
completions, interrupts and GPU queue state (`mask=3B`). Clock and audio
guards were acquired, both remained stable, and all 19 converted
continuations matched. The file checkpoint session refused to start
(`files_ready=0`), so the overall audit correctly rejected; no restore ran.
That initial log did not identify which file-session precondition failed.
The measured ownership/capture/check/release
interval was 57.923 ms, excluding allocation and cleanup; this diagnostic
timing does not establish a usable rollback frame budget. The game later
closed normally at the user's request. Evidence:
`build/rollback-state-live-stderr.log`.

File-session refusal diagnostics now identify the first failed binding, guest
handle, static reason and immediate Win32 error. The bridge reports the
associated path/live count, or which clock/thread/APC/untracked-handle
precondition blocked setup. Untracked-handle provenance distinguishes table
capacity from resolution of a raw handle. These diagnostics run only when an
explicit checkpoint session is requested; normal file operations do not log
additional output. No admission checks were relaxed and no file state is
restored. A failed setup still unwinds every partial lease, while a repeated
begin leaves an existing session intact.

The stream tests verify directory, writable, duplicate and changed-identity
refusals, the actual sharing-violation code, partial-lease cleanup, active
session retention, and clearing diagnostics on success. All three affected
tests (stream files, state audit, checkpoint integration) pass; the production
executable rebuilt. Logs: `build/rollback-file-reasons-tests.log` and
`build/rollback-file-reasons-production.log`.

The diagnostic rerun identified `directory-handle`, guest `660000AC`, entry
25 of 26 live handles: the read-only `HDD/E/TDATA/54430001/` save directory.
All eight other domains remained stable (`mask=3B`, 19 continuations); the
audit took 60.367 ms and performed no restore. The game closed normally.
Evidence: `build/rollback-file-reasons-live-stderr.log`.

The file component now supports explicitly guarded idle directory handles.
It leases and validates their identity/attributes without querying or restoring
a file position. Snapshot schema 2 records directory versus regular-file
entries and requires a zero directory position. Writable directories still
reject. The production bridge refuses capture when a directory enumeration is
active; `NtQueryDirectoryFile` already invalidates leases/epoch before querying.
Closing a guest directory now releases its associated find cursor, avoiding
leaks and stale state when Windows recycles the directory handle. Directory
contents and enumeration positions are NOT restored; queries during a replay
window remain unsupported and invalidate that window.

Tests cover mixed directory/file capture and restore, no OS directory seek,
identity pinning, malformed directory entries, epoch/session invalidation,
writable rejection and closed handles. A new fixture extracts the actual
production table/checkpoint/query code and verifies query invalidation, active
enumeration refusal, end-of-enumeration readiness and close cleanup with real
temporary directories. All four affected tests pass (stream files, stream
bridge, state audit and checkpoint integration), and the production executable
rebuilt. Logs: `build/rollback-directory-tests.log` and
`build/rollback-directory-production.log`.

The next live run reached the newly added enumeration guard and rejected
`active-directory-enumeration`. The other eight domains again matched, with
19 continuations, `mask=3B`, and 53.652 ms in the diagnostic interval; no
restore occurred. Evidence: `build/rollback-directory-live-stderr.log`.

The current bridge now retains an existing directory enumeration only inside
an unchanged file session. Every active find cursor must belong to a tracked
guest handle, with no duplicate find handles. After file leases are acquired,
the bridge retains the exact directory/find table and checks it before each
capture/validate/restore. This is process-local retained state, not serialized
enumeration state. A query, continuation, restart or guest close still ends
the guard and leases before touching the OS cursor; the epoch/session checks
then reject the old snapshot even if Windows recycles the same handle values.
The guard never advances, reopens or rewinds a find cursor. External directory
contents are not frozen; replay windows containing directory queries remain
unsupported. The low-level opt-in binding field is now `directory_guarded`.

The production bridge fixture verifies two-entry enumeration order across a
restore, foreign/duplicate find-handle rejection, retained guard preservation
on repeated begin, table-change rejection, and invalidation on continuation,
exhaustion, restart and close. All four affected tests pass. Logs:
`build/rollback-directory-guard-tests.log` and
`build/rollback-directory-guard-production.log`.

The retained-cursor revision passed its live Practice audit. All 26 file
handles were pinned (`bytes=448`, epoch 243), with one enumeration retained
unchanged. Both clock/APU guards were acquired; clock, audio and all 19
converted continuations matched. RAM, heap, files, completions, interrupts
and GPU queue also matched (`files_ready=1`, `mask=3F`). Both audit summaries
reported passed. The measured interval was 62.339 ms; this remains a one-time
double-capture diagnostic, not a demonstrated rollback-frame cost. No state
was restored and no gameplay replay ran. Evidence:
`build/rollback-directory-guard-live-stderr.log`.

Mode 2 does not include renderer image/pipeline snapshots. Neither mode yet
establishes input ownership, all kernel objects, host audio processing/output state, or a
whole-game restore transaction. Stable double captures are evidence for the
sampled interval, not proof of complete state coverage or deterministic replay.

### Combined live renderer capture audit (2026-10-18)

`DOA3_ROLLBACK_OWNERSHIP_AUDIT=3` adds the production PGRAPH/backend/surface
checkpoint to the one-time Practice audit. Modes 1 and 2 retain their existing
scope. The live source is accepted only on the initialized PGRAPH owner thread,
outside a draw batch, with the supported guest target/buffer/viewport inventory.
An initialization generation prevents images surviving renderer restart.
This binds capture and comparison only: no live restore or replay call is made.

The first renderer capture precedes the six-component state sample; the second
follows it, both inside the existing clock/APU guards. It compares PGRAPH
logical state, model-view, D3D pipeline state, target routing/keys/viewport,
and every captured color/depth/history image. GPU comparison uses staging
copies and exact row bytes (excluding pitch padding), including D24S8 and
D16 depth. Copies/readbacks are ordered on the same immediate context. They
do not change live pixels, bindings or presentation. A failed capture,
validation, allocation, readback or comparison rejects the overall audit.
The `[ROLLBACK-RENDER]` lines report boundary flags, image count/bytes and the
two capture/equality results. No screenshots or RAM images are saved to disk.

The extra GPU allocations/readbacks can cause a one-time hitch and are not a
production rollback cost measurement. Snapshot clones and temporary staging
resources are released; the live renderer keeps its original resources.
Thread/boundary checks and stable captures do not prove coverage of every
renderer writer or solve input, host audio/output or whole-fight replay.

WARP tests exercise the actual live-source binding and production renderer,
checking equal images, color/depth changes (including a seven-pixel-wide D16
surface), logical/pipeline changes, wrong thread, draw/buffer rejection and
generation changes. Comparison leaves live pixels unchanged. Additional
orchestration fixtures use the real clock/APU guards with stub renderer/state
components to check capture ordering, balanced cleanup and overall rejection
on a renderer mismatch. All nine affected tests pass across
`build/rollback-render-capture-tests.log` and
`build/rollback-render-capture-final-tests.log`. Production build log:
`build/rollback-render-capture-production-final.log`.

The live Practice run passed the combined audit. Renderer boundary flags were
zero, with seven images totaling 45,844,224 bytes per capture. Both renderer
captures validated and matched exactly in logical state, pipeline, target
metadata and pixel bytes (`ready=1 first=1 second=1 equal=1`). All nine prior
state domains also passed: 26 pinned file handles with one retained enumeration,
19 matching continuations, clock/audio stable and six-component mask `3F`.
The measured guarded diagnostic interval was 89.708 ms, including synchronous
GPU comparison; it is not a production rollback cost. No restore or replay ran.
The game subsequently closed normally at the user's request. Evidence:
`build/rollback-render-capture-live-stderr.log`.

### Guest XAPI input bookkeeping (2026-10-18)

Input tracing found two guest-visible mutable fields outside guest RAM:
`XGetDeviceChanges`' prior-device mask and four synthetic packet counters in
the 3.1 `XInputGetState` override. They now live in `rollback_input.c`, with
the same initial packet values, per-port increment behavior and device-change
semantics. The normal overrides call this module; physical polling/mapping,
focus handling, host packet caches, rumble and device configuration are unchanged.

The pointer-free 32-byte XPP snapshot stores schema/size, clock-session epoch,
prior mask and four packet counters. Capture/validation/restore require the
original owner thread, primary guest context and active deterministic session.
Invalid schema/size/mask/epoch/thread rejects without mutation. Component
restore exists for tests but has no live caller. This is bookkeeping support,
not a physical input tape, prediction system or complete input checkpoint.

Modes 2/3 now double-capture this component as bit 6 (`xpp=1`); all seven
state-audit bits are `7F`. Earlier live `3F` results above predate this addition.
The renderer/clock/audio/continuation audit still surrounds the state sample.
The live snapshot includes ten CPU-side domains plus the renderer when mode 3
is used; full game restore remains disabled.

Tests cover three repeated restores, four independent packet streams, device
insertion/removal, packet wrap, malformed/stale/wrong-owner rejection and a
mutation between captures. An additional fixture extracts the actual 3.1 XPP
overrides and checks identical guest-memory results after bookkeeping restore,
null output pointers, forced-device baseline and configured-device reopening.
All six affected tests pass: `build/rollback-input-tests.log`. The production
build succeeds: `build/rollback-input-production.log` and
`build/rollback-input-production-final.log`.

The live Practice audit passed with `xpp=1` and all seven state bits set
(`mask=7F`). Clock/audio and all 19 continuations also matched; 26 file handles
and one retained enumeration remained valid. The renderer passed with nine
images totaling 75,358,464 bytes per capture and exact logical/pipeline/target/
pixel equality. The diagnostic interval was 139.425 ms, including GPU readback;
this is not a rollback-frame budget measurement. The user reported that play
felt good, and the game later closed normally. No live restore or recorded-input
playback occurred. Evidence: `build/rollback-input-live-stderr.log`.

### Bounded guest input recording/replay (2026-10-18)

`rollback_input_tape.c` records the ordered input API calls in each dispatcher
interval, including repeated reads of the same port. The 3.1 overrides now
route device masks, reconfiguration notices, controller subtypes, mapped pad
states and rumble through this adapter. Outside an explicitly active tape it
passes through to the existing input backend. Recording also retains fresh
reads, preserving normal polling, mapping and focus behavior.

The process-local tape has at most 600 intervals and 128 calls per interval.
It is not a prediction buffer or network input format. A replay caller must
restore guest RAM and XPP bookkeeping separately. Replay checks call order,
port, pointer presence and motor arguments, returns the saved response and
never polls a device or repeats physical rumble. It requires the owner thread,
primary context for begin/end and an active deterministic clock session.
Under/over-consumption, overflow, epoch change or a mismatched call poisons the
tape. A failed replay stays gated until explicit abort/destruction; it cannot
silently fall back to live input. Recording overflow leaves ordinary physical
input working and rejects the diagnostic result.

The opt-in `DOA3_ROLLBACK_INPUT_AUDIT=1` records 600 dispatcher-start to next
dispatcher-start intervals after 120 eligible solo Practice intervals. It
requires `DOA3_ROLLBACK_TASKS=1`, excludes training recording/playback and other
capture/audit modes, logs counts and frees the tape after completion or exit.
This live audit only records; it does not rewind or replay the fight. It makes
no large RAM/GPU snapshots or per-frame disk writes. UI/free-camera physical
polling remains outside the guest adapter.

Seven affected tests pass (`build/rollback-input-tape-tests.log`). The actual
3.1 input overrides reproduce identical guest bytes and XPP packet counters
over repeated fixture replay after physical devices are changed/disconnected,
with no additional hardware polling or rumble calls. Tests also cover failed
reads, metadata, repeated fresh reads, wrong owner/context/session, malformed
call sequences, overflow, and automatic live-recorder cleanup/gating. The
production executable builds (`build/rollback-input-tape-production.log`).
The live Practice recorder completed all 600 dispatcher intervals with 2,400
events: 600 pad-state reads on port 1 (`ports=1`, a bitmask), 600 rumble calls,
and 1,200 other input API events. It reported `failed=0` and stopped recording
automatically; the game remained running and responsive afterward. Evidence:
`build/rollback-input-tape-live-stderr.log`. This confirms the bounded recording
path in this session, not live replay determinism. There is still no live fight
replay caller, input prediction/correction, or playable 3.1 rollback networking.

### Input integrated with coordinated restore (2026-10-18)

The local transaction now requires XPP bookkeeping as its tenth byte domain
(`DOA3_CP_INPUT`, local bundle schema 6). Construction rejects a configuration
that omits it. Packet counters and the previous-device mask therefore restore
and compensate with RAM, heap, fibers, clock, files, completions, interrupts,
GPU frontend and private audio. Old local bundle layouts are rejected.

The mixed integration fixture now extracts the actual 3.1 XPP overrides and
uses them to drive its task/worker state. It records 16 intervals of four-port
input, including repeated reads, device/reconfiguration masks, capabilities
and rumble. It then disconnects its fake physical backend and performs three
16-frame restores/replays. Every replayed frame's complete ten-domain checkpoint
hash matches its original; final bytes and PCM match exactly. There are no
physical backend reads, metadata queries or rumble calls during replay.

Failure injection after each of the ten real domain restores still recovers
the original before-image. An intentionally mismatched rumble request after
simulation mutation poisons the input tape and prevents live-input fallback;
restoring the original end checkpoint recovers all ten domains exactly before
the failed tape is discarded. Missing input-domain configurations, corrupted
bundles, stale epochs and the separate renderer-sidecar transaction are also
covered by the affected tests.

All six affected tests pass (`build/rollback-input-coordinated-tests-final.log`),
including per-frame comparisons in the mixed integration test. The production
online library compiles (`build/rollback-input-coordinated-library.log`). The
running executable has not been replaced by this test-only integration step.

A second run seeded from the existing captured game RAM/APU image also passes
the per-frame and final comparisons, reproducing 12,800 nonzero stereo samples.
Evidence: `build/rollback-input-coordinated-audio-seed.log`. This uses the
capture as initial data for private processing, not a replay of the captured
live fight's subsequent frames.

This is coordinated replay of the private mixed fixture, not a live fight:
its scheduler, input overrides and checkpoint implementations are production
code, but task/stream bodies and GPU commands are fixtures. Audio uses the
private `apu_replay` backend. Production currently exposes APU capture and
ownership pins but no live APU processing/output restore adapter; implementing
that safely remains a prerequisite for live rewind. Kernel object coverage,
whole-fight stepping and presentation/effect reconciliation also remain.

### Production audio processing checkpoint and replay output gate (2026-10-18)

`apu_live_checkpoint.inc` adds a process-local processing snapshot to the
production APU backend. It contains command/filter state, chip clock/debt and
pending IRQ, full period phase, pause state, GP/EP register arrays, partial
256-sample PCM, and the logical XAudio2 staging buffer/fill count. Staging moved
from function-local statics to file-private state without changing ordinary
monitor behavior. Snapshots require the deterministic APU owner, its held pin,
primary guest context, matching device/RAM identity and session epoch. A new
APU session invalidates earlier processing snapshots even with the same base.

Restore is available only inside an explicit local replay output gate. Begin
retains the original processing end-state. While replay runs, the real monitor
advances its PCM/staging state but does not query/wait on the physical queue,
submit samples or advance host-mixer voices. End requires an exact return to
the retained end-state. A mismatched or faulted trial stays gated; explicit
cancellation permits closing only after the original state has been restored.
Deterministic-session exit is blocked while this gate remains active.

This gate supports unchanged-input round-trip validation, not corrected-audio
reconciliation. Device buffers already submitted may drain while it runs;
their samples cannot be unplayed. The adapter rejects waveOut, movie audio,
software voices, test tones, debug monitoring/muting and unsupported output
sizes. The eventual whole-fight caller must also exclude configuration/device/
mixer/movie writers, restore guest RAM and IRQ delivery state, and handle a
failed trial as a whole transaction. Host device objects, DSP interpreter state
(the current DSP is passthrough), diagnostic counters and wall-clock pacing
are not serialized. There is still no live game caller of restore or replay.

The source fixture compiles the production monitor, stepping, ownership and
checkpoint functions directly. Three six-frame round trips reproduce processing
bytes and all guest RAM, including a partial PCM period and staging buffer.
Replay makes no device submissions, queue queries or host-mixer calls; ordinary
submission resumes after the gate closes. Tests cover wrong owner/context,
stale sessions, malformed images, unsupported paths, backend loss and recovery.
Seven affected tests pass (`build/rollback-live-audio-tests-final.log`). A run
seeded from captured game RAM/APU state also passes, with 4,864 nonzero output
samples (`build/rollback-live-audio-seed-final.log`). These are component tests
using fake transport endpoints, not a live whole-fight rewind.

Ownership audit mode `DOA3_ROLLBACK_OWNERSHIP_AUDIT=4` double-captures and validates
this processing image under the clock/APU pins in solo Practice. It checks
clock and parked continuations too, but skips large RAM/GPU captures and never
opens the output gate or restores state. Live mode-4 validation is pending.
The production executable builds successfully (`build/rollback-live-audio-production.log`).
The launch approval was declined, so this build's live audit has not been started.

### Production audio in the coordinated checkpoint fixture

The ten-domain integration now has a second executable,
`rollback_checkpoint_live_audio_test`. It compiles the production APU monitor,
stepping, ownership and processing-checkpoint implementation directly, using
the real deterministic clock/APU pins. Physical audio transport remains fake;
game task bodies and stream services remain fixtures. It does not launch DOA3.

This exposed a distinction between a valid captured image and permission to
restore it: audio images are capturable with the replay gate closed, but cannot
be restored then. The coordinator now supports an optional read-only
`can_restore` preflight on each byte domain, under the common fence and before
any domain mutates. A closed audio gate rejects the complete restore without
changing RAM or requiring compensation. The checkpoint blob remains schema 6.

Three 16-interval replays match each interval's complete checkpoint hash, final
bytes and generated PCM across all ten domains. Audio staging and partial PCM
are included. Replays issue no additional device submissions, queue queries,
hardware input reads or rumble. Injected failures after each domain restores
recover the complete original state; mismatched recorded input also recovers
before the audio gate closes. Tests additionally cover a denied APU pin (with
clock-pin cleanup), an in-flight interrupt and a stale completion queue.

All nine affected tests pass (`build/rollback-combined-audio-tests.log`), and
the production `apu_emu` and `doa3_online` libraries build successfully
(`build/rollback-combined-audio-production.log`). The captured RAM/APU-seeded
variant also passes, reproducing 12,800 nonzero PCM sample frames exactly
(`build/rollback-combined-audio-seed.log`). The game executable was not
relinked or launched for this fixture-only validation. Live mode-4 validation
and actual fight rewind remain pending; these tests do not establish either.

### Bounded local replay trial controller (2026-10-19)

`src/game/rollback_trial.c` now owns the sequencing previously performed by
the combined fixture. It accepts retained checkpoint images and an immutable
input tape, with a maximum of 16 dispatcher intervals. It verifies the current
end-state, captures a recovery image, opens the supplied output gate, restores
and verifies the earlier checkpoint, then replays and verifies each interval.
It uses the owned-image API so an eventual renderer sidecar can participate;
the combined CPU/audio fixture does not yet include that sidecar.

The tape now exposes a read-only range preflight. Missing frames, overflowing
ranges, active tapes, wrong owners, non-primary boundaries and failed tapes are
rejected before mutation. Reentrant trials are rejected. The deterministic
session epoch must remain unchanged throughout execution and recovery.

If restore, input replay, advancement, verification or output completion fails,
the controller restores the original end-state and verifies it before aborting
the tape and closing the output gate. Failed recovery, a poisoned checkpoint,
changed epoch, failed gate cancellation or cleanup failure returns a sticky
fatal result: the caller must stop simulation. It retains its recovery image,
trial-wide exclusion and any still-active gates. A new trial cannot bypass
that failure. Fatal-process teardown is intentionally separate from ordinary
successful cleanup; there is no unsafe "resume anyway" path.

The caller must still supply real whole-fight advancement, complete state
verification, trial-wide exclusion of external/configuration writers, and
suppression of irreversible effects. The short per-checkpoint clock/APU pins
are distinct from that longer exclusion and are released while advancing.
No game hook invokes this controller, and no environment switch enables it.
This remains unchanged-input validation, not corrected-input/network play.

The automated controller test uses the real coordinator and input tape with
small fixture state. It covers bounded/invalid ranges, wrong thread/context,
reentrancy, failures at each stage, successful recovery and fatal recovery
paths in isolated processes. The mixed production-audio fixture now runs its
three 16-interval replays through this controller and injects a verification
failure after three replayed intervals. Recovery reproduces the original
ten-domain checkpoint exactly with no additional hardware input, rumble,
audio submissions or device queue queries. Task bodies and audio transport
are still fixtures; this is not a live fight rewind.

All 12 affected tests pass. A separate captured RAM/APU-seeded run also passes,
matching 12,800 nonzero PCM sample frames (`build/rollback-trial-audio-seed.log`).
Build/test evidence is recorded in `build/rollback-trial-build-final.log`,
`build/rollback-trial-tests-final.log` and `build/rollback-trial-production.log`.
The production online library includes the inactive controller. The running
game executable has not been replaced or launched for this work.

### QPC polling history restored with the clock (2026-10-19)

The host-state audit found a concrete rewind divergence in
`bridge_KeQueryPerformanceCounter`: its deterministic per-fiber consecutive
poll counts, fallback counts and previous kernel-call numbers were static
bridge state, outside `xbox_det_checkpoint`. Restoring time and the per-fiber
kernel-call counts left those poll histories in the future. The next replay
could therefore boost QPC and service workers at a different point.

An automated test compiles that actual bridge body with the production clock.
It failed before the fix on a replayed QPC sample comparison
(`build/rollback-qpc-before.log`, exit 1). The history now belongs to the clock
component, resets at session start, and is captured/restored for all 64 fiber
slots. `xbox_det_qpc_poll` retains the existing >150 consecutive-poll and 2048
fallback-poll rules, the 733333 boost, and primary-only worker service in the
bridge. The non-session QPC path is unchanged.

The local clock image is version 2. Version-1 images and invalid poll-history
fields are rejected before mutation. This is not a game save-format change;
the larger clock domain also changes the local coordinated snapshot size.
Poll updates are refused while pinned, outside a deterministic session, or
from another host thread.

Coverage includes three replays across each boost threshold on both primary
and worker fibers, exact returned QPC values and worker-service decisions,
fiber isolation, session reset, malformed images and ownership/pin guards.
The combined ten-domain replay fixture now crosses the QPC threshold too and
makes guest memory depend on the returned time. Its task bodies remain fixtures.

All eight affected tests pass, including the previously failing bridge test.
The production online and kernel libraries build successfully.
Evidence: `build/rollback-qpc-build.log`, `build/rollback-qpc-tests.log`,
`build/rollback-qpc-production.log`. The game executable has not been relinked
or launched for this step. Host synchronization objects, unsupported kernel
effects and trial-wide writer exclusion still need integration; the proposed
kernel replay guard was deferred to fix this reproduced clock-state defect.

### Kernel dispatch guard for local trials (2026-10-19)

`kernel_replay.c` adds a trial-scoped guard around the actual synthetic kernel
dispatcher. The trial controller opens it after retaining the original state
and before opening replay output. Begin briefly uses the clock pin to verify
the deterministic owner; the guard itself is not a writer-exclusion lock.
Ordinary dispatch remains enabled when no local trial is active.

The initial 3.1 allowlist is deliberately small: pool allocation/free/query,
guest DPC/interrupt setup and queuing, guest event signaling, deterministic
QPC/frequency/system-time queries, and fixed protection/heap-size queries.
Each entry checks both the ordinal and its exact resolved bridge function.
A remapped ordinal, missing bridge, old-build mapping or unreviewed call is
rejected. File operations (including reads until a production cursor adapter
is integrated), native synchronization objects, thread lifecycle, VM changes,
unreviewed waits and the host interrupt-time query are not approved.

Rejection happens before kernel-call accounting, scheduling, logging or bridge
execution. Guest stdcall stack cleanup is retained and result registers are
zeroed as placeholders; this is NOT ordinary guest success/failure emulation.
If a scheduler invocation faults the guard through another worker, dispatch
checks the guard again before executing the original bridge. The first fault
reason/ordinal and accepted/rejected counts are retained in the trial report.
After a fault all further dispatch is denied, including otherwise approved
calls. A 100,000-call trial budget also faults the guard. Wrong-thread access
and session changes are rejected; only the owner at a primary boundary can
end the guard, and cancellation requires the caller to have verified recovery.

The trial controller checks the guard after advance/verification and restores
the original checkpoint before cancelling it. Fatal recovery leaves it active.
This does not unwind arbitrary guest/native stacks or stop an unbounded guest
loop: a future production advance callback must return promptly on the guard
fault and have its own bounded execution mechanism. Direct native host calls
outside the synthetic dispatcher, renderer/audio lifecycle, external writers,
and actual host-object restoration remain separate integration requirements.
No live game hook or user-facing option activates a trial.

The dispatch regression compiles the production dispatch and binding policy
with counted bridge endpoints and the real clock/guard. It checks forbidden
calls, exact binding matches, stack cleanup, absence of dispatch/scheduler/
clock side effects on initial rejection, nested worker faults, owner/epoch/
budget checks and unchanged dispatch outside trials. The controller fixture
injects an unsupported write ordinal and verifies full state recovery with no
extra input or rumble. The mixed production-audio replay now runs with this
guard; its game task bodies and physical audio transport remain fixtures.

Eight controller/integration tests pass in `build/rollback-kernel-guard-tests.log`.
The dispatcher test also passes in `build/rollback-kernel-guard-dispatch-test.log`
after giving its fake endpoints distinct code so MSVC identical-code folding
cannot erase the binding-identity check. All nine affected tests pass, and the
production online/kernel libraries build successfully.
Build evidence: `build/rollback-kernel-guard-build.log`,
`build/rollback-kernel-guard-dispatch-build.log`, `build/rollback-kernel-guard-production.log`.
The game executable has not been relinked or launched.

### Combined CPU, production audio and renderer replay fixture

`rollback_checkpoint_render_audio` now runs the local trial controller with
all ten production checkpoint domains and the production renderer sidecar in
the same transaction. It shares the existing CPU/audio integration harness
and a reusable headless WARP scene with the renderer transaction regression.
No window, presentation loop or game process is launched.

The fixture records 16 intervals after a two-frame warmup, then replays the
same inputs three times. Every interval must reproduce all ten byte-domain
observations, PCM output and hashes of the renderer's color, depth/stencil,
offscreen, framebuffer-copy and previous-frame images. Texture lower mips and
shader constants depend on the fixture CPU task result; the drawing also
samples the preceding GPU frame. All restores use owned checkpoint images,
so the CPU/audio domains and GPU images share the same fence, restore order
and compensation path. Raw coordinator APIs remain rejected with a renderer
attached. The independent observation helper reads state under that same
fence; it is test-only and never restores state.

The same trial also checks:

- Deliberately corrupting only GPU pixels during replay is detected by the
  normal verifier, and restores the original CPU/audio/pixel end-state.
- Failure injected after the renderer actually restores its image is
  compensated, and the trial recovers without leaving any output gate held.
- A failure after each of the ten byte-domain restores compensates the full
  bundle, including the renderer. Input-call mismatch, audio-pin rejection,
  active interrupt delivery and stale completion epochs remain covered.
- Replay/recovery performs no physical input, rumble, audio submission or
  audio-queue queries. Owned renderer images are released; the final D3D11
  device reference count returns to zero.

All four affected regression tests pass in `build/rollback-combined-tests.log`;
build evidence is in `build/rollback-combined-build.log`. A separate seeded
run also passes (`build/rollback-combined-seeded.log`), reproducing 12,800
nonzero PCM samples through each replay alongside the state/pixel checks.
This remains a
private harness: task bodies and physical audio endpoints are fixtures, and
the small drawing pipeline is not a DOA3 guest draw stream. It does not prove
live fight determinism, host-writer exclusion or acceptable replay cost.
Production runtime behavior and the game executable are unchanged.

### Resumable 3.1 outer-loop control flow

The next fight-adapter execution primitive is generated from the actual
`sub_000A6450` body by `tools/recomp/rollback_loop.py`. It covers one complete
dispatcher-to-dispatcher interval, including the work after the dispatcher
returns and before its next call. Its boundary is `A64B0` **before** the guest
store/return-address push. This is not interchangeable with the existing audit
hook inside `1C650C`, which already has a native dispatcher stack active.

`Doa3LoopState` retains the outer loop's local flag operands, continuation,
pending helper, bounded call count and interval count. `doa3_loop_next` runs
only the lifted arithmetic, branches, argument/return-address pushes and x87
operations up to the next direct call. It returns that helper as a request;
it does not invoke it. The caller must execute the admitted helper with the
guest ABI and acknowledge that exact routine before proceeding. Invalid state,
a pending call, exhausted call budget and interval overflow refuse advancement.
The reviewed source digest pins all 21 call sites and the surrounding control
flow; changes require an explicit review before generation succeeds.

The regression compares this continuation against the original lifted loop
running on a private native fiber, with the same deterministic helper fixtures.
Across 20 intervals, registers, x87 state, stack effects and all 16 MiB of
private guest memory match after every helper and at each boundary. It covers
the optional call and both fade branches, then restores an earlier snapshot
and repeats 15 intervals three times with identical state. It also checks
pending/wrong/duplicate acknowledgement, malformed state, budget/overflow
rejection, and rejects source mutations to calls, branches, stores and cleanup.
Evidence: `build/rollback-loop-tests.log`, `build/rollback-loop-build.log`,
`build/rollback-loop-production.log`.

The generated primitive is compiled into `doa3_online`; no native loop has
been replaced, no live trial is enabled and the game executable is unchanged.
This proves control-flow equivalence only. A production advance adapter still
must implement and bound the helper calls, handle presentation/pacing, own
external writers, bind complete host state, include `Doa3LoopState` in the
checkpoint and enter at the correct native boundary. An interval is not yet
proven to equal one gameplay tick in all modes. Merely invoking every returned
routine through `recomp_lookup` would not constitute a safe replay adapter.

### First replay helper group: controller polling and feedback

The outer-loop adapter now admits the actual 3.1 `A4DD0` (device changes and
four-port polling) and `A50A0` (four-port feedback countdown/submission) bodies.
`rollback_loop_input.py` extracts them from the lift, pins their complete
source and the six production XPP endpoint bodies, and compiles the result
into `doa3_online`. Their loops have fixed four-port bounds; no guest-selected
routine is dispatched. `doa3_loop_input_run` requires an exact pending outer-
loop request, the exact healthy active replay tape, owner/primary context,
unchanged deterministic epoch, matching RAM mapping and a valid guest stack.
All other helpers refuse before mutation; there is no native-lookup fallback.

The helpers use the production XPP overrides and recorded input events,
including device-change masks, subtype queries, synthetic packet counters,
close-controller rumble stops and feedback status writes. Replay suppresses
XPP diagnostic logging and performance-sample callbacks (also while a failed
tape awaits recovery), so those host observers do not run twice. Normal input
and recording keep their previous diagnostic behavior. If input replay or
guest ABI checks fail after a helper runs, it returns failure without
acknowledging the pending call. The caller must restore the enclosing full
checkpoint; this helper API cannot repair a partially advanced frame itself.

`rollback_loop_input` records 12 outer-loop intervals and replays them three
times. Actual polling/feedback code is exercised for all four controllers,
device removal/reinsertion/reconfiguration, pending feedback and expiring
rumble timers. Guest RAM, registers, XPP state and loop continuations match at
every boundary. Physical controllers are disconnected during replay, while
input/rumble/metadata, diagnostic log and performance-observer call counts
remain unchanged. Injected rumble mismatch leaves the request pending and
the tape gated. Recording, wrong epoch, non-primary execution, invalid stack,
missing tape and all unapproved loop helpers refuse without state changes.
Source-guard tests reject changed code in either helper or any XPP endpoint.

Six affected regressions pass across `build/rollback-loop-input-tests.log`,
`build/rollback-loop-input-final-tests.log` and the combined test log below.
Build evidence:
`build/rollback-loop-input-combined-tests.log`,
`build/rollback-loop-input-production.log` and
`build/rollback-loop-input-manual-compile.log`. The production library and
modified manual override compile; no game launch or executable relink occurs.
The fixture still substitutes every other outer-loop helper and physical
controller endpoint. This initial group excluded `A4E70`; the decoder is now
admitted as described below, followed by timestamp bookkeeping. Audio,
dispatcher and presentation helpers remain unadmitted. No live whole-fight advance is enabled, and
whole-trial configuration/writer exclusion remains the future adapter's
responsibility.

### Replay helper group: guest input decoding

The same adapter now admits the real 3.1 `A4E70` decoder and its internal
`A4E99`/`A4EA0` tail calls. All three complete lifted bodies join the source
digest; their internal calls are redirected to private extracted functions.
Only the outer `A4E70` request can enter the decoder. No native lookup or
public admission of a mid-function tail is available.

Before any decoder write, the adapter checks the three mapping tables for
zero-mask terminators within their 3.1 extents and restricts analog indices
to the eight pressure bytes. Inspection of the local 3.1 XBE confirms eight
digital mappings at `2D4C50`, six low-threshold mappings at `2D4C98`, and two
high-threshold mappings at `2D4CD0`, each followed by a sentinel. The decoder's
guest stack scratch/return range must not overlap these tables or the pad,
decoded-output and aggregate regions. These checks bound the table walks and
four-port tail chain without changing the lifted decoding behavior. The
enclosing trial must still exclude concurrent guest-memory writers.

The expanded `rollback_loop_input` test records 48 outer-loop intervals and
replays them three times with actual polling, decoding and feedback bodies.
It independently checks digital/pressure-derived buttons, pressed/released
edges, disconnected-port clearing, aggregate masks, the initial 30-tick
held-button repeat and subsequent five-tick repeat. Inputs include pressure
values 0/31/32/127/128/255 and stick values around both deadzone boundaries;
full RAM/register/XPP/continuation comparisons verify equivalence with the
current lifted code, not original-console deadzone accuracy. Malformed
sentinels, out-of-range/wrapping analog indices, overlapping stacks and
internal-tail entry requests all refuse without changing state. All 11
helper/endpoint source mutations are rejected by the source guard. Physical
controller, feedback and diagnostic observer counts remain unchanged during
replay; the existing failed-rumble recovery checks still pass.

Five affected tests pass in `build/rollback-loop-decode-tests.log`. Build
evidence: `build/rollback-loop-decode-build.log` and
`build/rollback-loop-decode-production.log`. The production library compiles;
no executable relink, game launch or live rewind was performed. Other frame
helpers remain test stand-ins, so this is input-decoder coverage rather than
proof of whole-fight determinism or corrected-input resimulation.

### Replay helper group: timestamp bookkeeping

The real 3.1 `A65B0` and `A6680` timing/statistics routines and their XAPI
`18B744` timestamp helper are now admitted through `doa3_loop_time_run`.
Inspection found that `doa3_guest_rdtsc` still sampled host QPC even during a
deterministic session. It now delegates to `xbox_det_rdtsc`: a shared CPU
timestamp base plus exact 733,333,333-Hz cycles at 60 logic frames per second,
with the calling fiber's QPC boost converted to CPU cycles. Session reads do
not poll the host, alter clock state or schedule workers. Outside a session,
the previous elapsed-host-QPC behavior remains, with an offset to prevent
time moving backward when leaving a deterministic session. The shared base
is immutable during a session; existing frame/boost checkpoints already
contain all mutable state needed to rewind these timestamps.

Session parameter blobs now use `DOA3PRM2`, and replay files use version 3,
to serialize the new CPU timestamp base. Older parameter/replay formats
are deliberately rejected rather than silently loading with a missing base.
This is an internal format revision, not a game release-version bump. The
3.1 online-play gate remains closed; no networking functionality is enabled.

`rollback_loop_time.py` pins all three lifted routines plus the production
timestamp bridge. The adapter requires a valid pending timing request,
healthy owner/primary input tape, mapped guest RAM and valid stack/x87
position. It rejects stack overlap with timing state/constants and invalid
eight-slot timing-history indices before writes. It pins the deterministic
clock while running these bounded, read-only timestamp consumers. No native
dispatch fallback, pacing, clock advancement or presentation is admitted.
The future trial still must exclude other guest RAM writers and restore the
whole checkpoint after any partial execution failure.

`rollback_loop_time` compares two 72-interval replays with native extracted
timing code, including frequency-disabled/negative-high-word branches,
low-word timestamp rollover, FPS refresh, history wrap and x87 state. RAM,
registers, clock and loop continuations match after every interval. Separate
assertions verify exact 60-Hz CPU progression, per-fiber boost conversion and
isolation, restored timestamps, session-base propagation and monotonic exit.
Host QPC/frequency counters stay unchanged throughout session/replay reads;
helpers leave clock and scheduler state untouched. Missing tape, wrong
thread/fiber, already-pinned clock, bad stack/index/x87 position and other
helpers refuse. Source mutations invalidate admission.

The actual session-blob and replay-file codecs are extracted into
`rollback_session_codec` with synthetic guest regions. They round-trip every
clock base and following payload, reject truncated/old-format blobs, and
reject old replay versions. Timing, source-guard, clock, QPC bridge, kernel
guard, codec and combined production audio/WARP checkpoint tests all pass:
`build/rollback-loop-time-tests.log` and
`build/rollback-time-integration-tests.log`. Production-library and manual
override compilation pass in `build/rollback-loop-time-production.log` and
`build/rollback-loop-time-manual-compile.log`. No executable relink or launch
was performed. Other outer-loop helpers are still test stand-ins; actual
fight determinism and corrected-input effect reconciliation remain unproven.

### Audio service queue continuation

`rollback_audio_service.py` now extracts and pins the eight real 3.1 routines
`1F1F01`, `1F1358`, `1F1526`, `1F42B6`, `1F3C45`, `1F3C78`, `1F3C1B` and
`1F1260`. They implement the DirectSound service entry, synchronization
wrappers and pending-callback list processing. The generated continuation
keeps the nested guest locals and return points in a process-local structure
instead of leaving them on an unrewindable native C stack.

`doa3_audio_service_next` runs the internal guest control flow and returns
before every external synchronization/callback request, reporting its raw
target, calling routine/resume point and expected post-call stack pointer.
The caller must separately admit and execute that operation, then acknowledge
the exact target with correct guest stack cleanup. No native lookup, callback
invocation, kernel synchronization, worker scheduling, host audio submission
or fallback occurs in this primitive. The native indirect-call macro's
scheduling/tracing/stack-repair behavior is not reproduced here; scheduler
coordination and external effects still need explicit replay implementations.
The outer loop is acknowledged only after the entire service returns with
the original guest stack balanced.

All generated guest reads/writes use checked mapped-RAM accesses. A fixed
4,096-step budget covers basic blocks and nested transitions across all
external-call resumptions; malformed cyclic queues cannot spin indefinitely.
Faults stop further requests and leave the outer call pending for full-state
recovery. The state also validates continuation links, owner, session epoch,
primary replay tape and outer-loop interval. The caller must retain exclusive
ownership of guest state and include this continuation in any active-service
checkpoint. Reinitialization after a fault is not a substitute for restoring
the enclosing checkpoint.

`rollback_audio_service` compares the generated continuation with the actual
native lifted routines, substituting deterministic test handlers for external
calls. It checks full guest RAM and registers at every external request and
return, including multiple callbacks, retaining/removing queue entries,
empty queues, disabled servicing, absent device and elevated guest IRQL.
Three runs per scenario match. Restoring while a real queue callback is
pending reproduces the rest of the service, including its nested locals.
Invalid context/state, duplicate advance, wrong acknowledgment, bad callback
stack cleanup, out-of-range queue pointers and a cyclic queue are covered.
All eight source-body mutations invalidate generation.

Both new tests pass in `build/rollback-audio-service-tests.log`; the production
library compiles in `build/rollback-audio-service-production.log`. No live
caller, executable relink or launch was added. These tests establish queue
control-flow/continuation equivalence with fixture external handlers; they
do not establish real synchronization, actual callback/APU replay, corrected
sound reconciliation or whole-fight rewind. The next audio integration step
is explicit admission of those synchronization operations and callback targets
under the existing checkpoint/output guards.

### Audio synchronization and deferred callback admission

The next offline step adds `kernel_audio_sync.inc` and two explicit service
adapters. The queue continuation is schema 2. `doa3_audio_service_sync` admits
only the actual 3.1 bindings for critical-section enter/leave (277/294) and
IRQL raise/lower (160/161). It checks the source site, IAT, aligned synthetic
target, resolved ordinal, exact bridge, argument-byte count, stack range,
return/argument values and matching host/guest IRQL before executing. The
actual HAL/RTL and bridge bodies are source-pinned, not rewritten stubs.
The critical-section endpoints currently do nothing; IRQL changes update
both the host TLS value and guest byte `0x24`. Kernel clock accounting is
preserved. These four calls are NOT added to the generic dispatch whitelist.

`XboxAudioSync` begins only at passive IRQL with an active owner-thread,
primary-fiber kernel replay guard. A unique pin prevents ending that guard
while host synchronization state needs recovery. On failure, quiesce to
passive IRQL, restore and verify the entire entry checkpoint, then finish the
cancelled scope and release the other guards. Quiescing is not a RAM, clock,
register, audio or whole-game restore. It remains available after a sticky
kernel replay failure; wrong thread/fiber/session and stale pins are refused.
This scope does not yet support mid-service checkpoint restoration.

The actual buffer constructor `1F634C` installs vtable `0x21811C`, whose
slot `+0x18` in the Japanese 3.1 XBE is `1F688C`. The callback adapter checks
this exact binding and enters its source-pinned dispatcher as a saved child
frame. Commands 0–3 request `1F659E`, `1F6403`, `1F66FE`, or `1F64A8` with
their original arguments and stack cleanup. Other command values follow the
original no-op return path. Unknown vtables/callbacks remain pending. The
four voice-operation bodies still require separate admission: none is
silently run through native lookup, and no APU/output reconciliation is
claimed by this dispatcher conversion.

`rollback_audio_sync` uses actual lifted queue/callback code, actual HAL/RTL
and bridge bodies, and the production deterministic clock/replay guard. It
compares full RAM/register/clock results with native execution, exercises
all four command routes plus the default, retains/removes queue entries,
and cancels at every external request, including raised-IRQL boundaries.
Negative cases cover bindings, ABI, memory range, mirror mismatch, wrong
owner/fiber, duplicate scope, failed guard and stale pin. The voice-operation
effects and scheduler are fixtures. Production library builds and focused
tests are recorded in `build/rollback-audio-sync-production.log` and
`build/rollback-audio-sync-tests.log`.

No live hook, executable relink or launch was added. Scheduling at kernel
call boundaries must still be integrated explicitly (the bounded bridge
adapter never runs workers), along with the remaining voice operations and
other outer-loop requests, before actual fight rewind can be tested.

### Loaded-fight checkpoint binding and first in-game restore test (2026-10-19)

The immediate milestone is now a **local unchanged-input fight rewind**, not
further broad audio conversion. Frame advancement remains missing; do not
describe the following restore-only test as working rollback.

`rollback_fight_checkpoint.c` binds the existing transaction to production
RAM, allocator, converted fibers, clock, file cursors, completions, interrupts,
GPU frontend, APU processing, XPP and renderer resources. The RAM domain also
includes the primary guest integer registers, string/flag operands, x87 stack,
control word, and outer-loop continuation. The old fiber snapshot covered
converted non-primary fibers and did not include those primary registers.
The binding owns file leases and per-snapshot clock/APU pins. The caller still
owns full-window exclusion of configuration/lifecycle and other writers.

Production USER/PFIFO register restore now joins the manual submission parser
and render-target routing state. A changed PFIFO interrupt configuration or
changed translation/tracing configuration is refused before mutation. This
does not claim to rewind the PMC/PCRTC interrupt controller, arbitrary MMIO,
resource creation/destruction, or loading. Renderer images remain a mandatory
sidecar. Restore requires both the kernel replay guard and audio output gate.

`doa3_checkpoint_image_matches` captures and compares the included state under
the same fence without restoring it. It reports a domain mismatch mask and
first differing byte per domain, plus an exact renderer-resource comparison
(including GPU pixels). It distinguishes mismatches from invalid images or
failed captures. The production binding can recover at a parked pending
outer-loop helper; it does not incorrectly require a completed frame before
restoring an earlier boundary. It cannot unwind an executing native helper.

The explicit `DOA3_ROLLBACK_CHECKPOINT_TEST=1` probe runs **once** after 120
eligible solo Practice boundaries, at A64B0 before the dispatcher call. Enable
the existing `DOA3_ROLLBACK_TASKS=1`, `DOA3_ROLLBACK_WORKERS=1` and
`DOA3_ROLLBACK_STREAM_WORKERS=1` options before launching; disable other rollback
capture/audit options. The probe starts temporary deterministic clock/APU
ownership, captures the current parked state, gates audio output, restores
the same image, and requires an exact comparison before releasing guards.
No guest frame is executed or rewound through time. Its temporary outer-loop
token is valid only for this identity restore; live frame replay will need
the actual running continuation. A refused capture/gate leaves the game
running; an unverifiable restore stops the process instead of resuming with
partial state. The normal path remains disabled without that environment
option. It does not take screenshots or create recurring captures.

Nine focused tests pass across `build/rollback-fight-checkpoint-tests.log`
and `build/rollback-fight-probe-tests.log`. They cover the actual binding's
register/RAM routing and pending-helper recovery with controlled domain
endpoints, actual production GPU frontend adapter bodies, comparison failure
reporting, guard retention after a fatal restore, probe eligibility/one-shot
behavior, and the combined production audio/renderer replay fixture. Those
fixtures still use stand-ins for game simulation. The executable was rebuilt
successfully (`build/rollback-fight-probe-production.log`).

Live testing on 2026-10-19 reached solo Practice with Helena and Ayane. The
first attempt stopped at the deliberate fatal-verification guard; its original
log did not identify the failing stage. After adding stage/result and domain
difference reporting, the second attempt returned restore=1, frame=0,
poisoned=0 and comparison=1, then resumed normally. The user confirmed no
crash. Evidence: `build/rollback-live-checkpoint-20261019-190820.stderr.log`.
This is one live identity-restore pass, not a replay-determinism result.

Inspection also found that separate try-lock acquisitions between restore,
comparison and audio-gate cleanup could turn transient host-thread contention
into a fatal verification failure. The restore-only test now opens the kernel
guard first, then retains the clock/APU pins through capture, restore,
comparison and cleanup. Coordinator callbacks remain nonrecursive. Ordinary
checkpoint users still release pins between operations, as actual frame
advancement requires. The binding test simulates denied reacquisition after
the first capture and verifies successful cleanup without another physical
acquisition. Fatal restoration retains the pins with the other guards until
process teardown. This removes a possible cause of the first failure; that
failure's precise stage was not recorded and remains unconfirmed.

The continuously pinned revision also passed in a live fight:
`build/rollback-live-checkpoint-20261019-191409.stderr.log` records restore=1,
frame=0, poisoned=0 and comparison=1, followed by successful cleanup. Its two
binding tests pass, including simulated contention and fatal guard retention;
the production executable rebuilt successfully
(`build/rollback-checkpoint-pins-build.log`). There are now two successful
live identity restores across the diagnostic and pinned revisions. Advancing
and replaying even one actual fight frame remains unverified.

Priority from here: run the loaded-fight restore test; connect a bounded real
frame advance to the existing trial/input tape; retain the expected original
frame states and compare replay results; only then expand supported effects
and corrected-input/network behavior. Do not assume skipping guest audio
service is harmless: muting physical output is separate from proving that
guest-visible audio callbacks may be omitted.

### One native interval and rewind/return check (2026-10-19)

`DOA3_ROLLBACK_WINDOW_TEST=1` is a separate, mutually exclusive opt-in probe.
With the existing task/worker flags enabled, it captures a loaded solo Practice
boundary, records one native A64B0-to-A64B0 interval and its input calls, then
captures the current endpoint. The new binding operation visits the earlier
image, compares it, and restores/compares the current endpoint before returning.
It keeps clock/APU pins through the restore transaction and gates audio output.
A rejected earlier image or mismatched earlier state recovers to the verified
current endpoint. Unverifiable recovery stops the process with guards retained.
The recording interval runs normally; no guest instruction executes while
temporarily visiting the earlier image. Native caller locals are only resumed
at the current endpoint. This is **not recorded-input frame replay**.

The first live interval froze in CRI's `199DB0` voice-stop/status wait. Its
`199E20` loop uses the XAPI CPU timestamp (`18B744`), not the QPC bridge, so
the existing QPC hot-poll logic never advanced its deadline. Evidence is in
`build/rollback-live-window-20261019-192602.stderr.log`, with the watchdog
stack mapping through `199DB0`, `1F1E79`, `1F19A0` and `1F6149` on worker 2.
The wait now calls `xbox_det_audio_wait_poll` only at that reviewed loop site.
During a deterministic session this advances the calling fiber's already
checkpointed time boost by a fixed 100 microseconds per iteration. It does not
advance gameplay ticks, yield, force a voice status, or affect other fibers.
Outside a session it does nothing. Source hashing guards the patch against a
changed guest wait body, and `apply_overrides.py` preserves it on regeneration.

The actual lifted wait/XAPI timestamp helper are exercised with the production
clock in `rollback_audio_wait`: an always-active voice reaches its original
deadline, early voice-stop status still wins, and restoring the clock repeats
the exact polling count and returned timestamp. Owner/pin guards and isolation
from the primary fiber are also covered. The checkpoint binding tests cover
two distinct endpoints, stale earlier-image rejection, incorrect earlier
restore recovery, and fatal guard retention. Seven probe cases cover opt-in,
conflicts, input failure, endpoint capture failure, mode changes and presentation
count checks.

After the wait correction, the live interval completed with one presentation,
one recorded input frame, four input events, one state read and one rumble call
without a tape failure. The earlier image was rejected by fiber validation;
the current endpoint was restored and matched, and the user confirmed normal
gameplay/audio. Evidence: `build/rollback-live-window-20261019-193648.stderr.log`.
Fiber refusal diagnostics now identify header/identity/wait-key differences.
The subsequent diagnostic run (`build/rollback-live-window-20261019-194244.stderr.log`)
identified worker 2 (`1926F0`): its earlier wait key was `B1A4B1A4`, but its
current wait key was zero. Enabling the controlled clock changes that worker
from its normal vblank block to a cooperative yield. The probe now allows one
uncaptured native interval for this handoff before capturing the start image;
the scheduler's native-park validation remains intact. The seven probe tests
pass with this extra settling interval.

Live validation passed in `build/rollback-live-window-20261019-195031.stderr.log`:
one presentation and one input frame (four events, one state read, one rumble
call, no tape failure), earlier restore=1 at frame=0, earlier comparison=1
with domain mismatch mask=000 and renderer mismatch=0, then current endpoint
restored and matched at frame=1. The user reported normal movement and audio.
This verifies restoring both endpoints after one actual native interval. No
frame was replayed; advancing from the restored start with the recorded inputs
and comparing its result against the original endpoint remains the next step.

### First native recorded-input replay diagnostic (2026-10-21)

`DOA3_ROLLBACK_REPLAY_TEST=1`, together with `DOA3_ROLLBACK_WINDOW_TEST=1`
and the task/worker flags, now extends the one-interval capture into a local
replay attempt. The binding restores/verifies the start image, releases its
physical clock/APU pins while retaining the audio output gate and kernel
guard, and invokes `rollback_native_frame.c`. This driver uses the source-pinned
A6450 continuation and an explicit list of its 21 native helper symbols. It
advances the clock/APU once and feeds the recorded input calls back through
the production input tape. It requires exactly one completed interval, one
guest flip and full input-tape consumption. Dead outer-loop flag temporaries
are normalized at the boundary; guest registers and RAM are not normalized.

The entire resulting checkpoint, including renderer resources, is compared
with the recorded endpoint. Regardless of success or mismatch, the original
endpoint is restored and independently verified before returning to the native
caller. A mismatch is diagnostic evidence, not a successful rollback test.

The replay flip still flushes drawing, delivers the pending APU IRQ, maintains
the previous-frame texture and runs the guest vblank callback. It omits window
presentation, message processing, screenshots, UI overlays and presentation
pacing. Rumble is suppressed by the input tape and physical audio by the APU
gate. Controlled-clock sessions no longer execute wall-time-dependent worker
laps inside `d3d4134_pace`; normal pacing remains unchanged.

This is an experimental native diagnostic, separate from the fully bounded
helper adapters. Its outer loop has a call budget, but individual native
helpers are not preemptible. The kernel guard therefore terminates immediately
on unsupported calls instead of returning placeholder registers into native
code. The newly admitted CS operations are the existing no-op bridges;
KfRaise/LowerIrql only change the host level and guest mirror. Checkpoint
operations still require both levels to be passive. File IO and other
unsupported operations remain rejected. No network transport is enabled.

Component coverage includes actual lifted outer-loop equivalence, native-driver
dispatch/clock/flip/input failure handling, successful and mismatching replay
callbacks with verified endpoint recovery, and the actual production flip
helper with presentation suppressed. All 22 relevant checks pass, including
audio synchronization, prior trial recovery cases and a subprocess test that
rejects an unsupported kernel call before its native continuation can resume.
The production executable built successfully (`build/rollback-native-game-build.log`).
The first live attempt (`build/rollback-live-replay-20261021-180240.stderr.log`)
recorded one interval but refused the earlier restore: worker 2 still had the
old `B1A4B1A4` park at capture and zero at the endpoint. The endpoint recovered
and matched, and the user reported normal gameplay. No replay helper executed.
Removing the wall-time presentation wait exposed that one uncaptured settling
interval is not sufficient evidence of a completed worker handoff.

The probe now uses the read-only `xbox_fiber_converted_clock_ready` check:
converted tasks/workers must be checkpoint-ready, and every live vblank worker
must be parked at its deterministic yield with zero wait key. READY status
alone does not qualify an old native park. It advances uncaptured intervals
until ready, or refuses after 120 intervals without retaining a checkpoint.
All ten scheduler/probe tests pass, including an actual Windows-fiber handoff,
delayed readiness and timeout. The executable rebuilt successfully in
`build/rollback-worker-ready-game-build.log`. The repeat live check in
`build/rollback-live-replay-20261021-180844.stderr.log` verified worker readiness
after two intervals, restored the earlier checkpoint, and matched it with
mask `000`. Replay entered helper `001C650C`, then deliberately terminated at
unsupported kernel ordinal 246 (`ObReferenceObjectByHandle`). This was the
reported crash; it was not a completed replay or an access violation.

The next revision admits only the port's non-coroutine object-reference stub
(bounded, aligned guest output, written as null), its no-op dereference, and
null-object single waits with bounded timeout pointers. Real event waits,
blocking, and coroutine reference calls that yield remain excluded. The tests
execute these three production bridge bodies and verify their guest-memory
effects and lack of scheduling, as well as binding and argument rejection.

A read-only kernel audit now spans the original recorded interval. It counts
every unsupported ordinal without changing normal dispatch. Unsupported calls
or invalid audit ownership/session cause a refusal before rewind and normal
cleanup, avoiding repeated fail-stops just to discover original-path calls.
The enforcing replay guard still applies: an original-path audit does not
prove the replay cannot diverge into another unsupported operation.

All 26 relevant tests pass, including audit ownership/epoch and overlap gates,
pre-rewind refusal/cleanup, kernel fail-stop, checkpoint recovery, native frame
and presentation paths, audio synchronization, and prior trial cases. Build
logs are `build/rollback-kernel-audit-tests-build.log` and
`build/rollback-kernel-audit-game-build.log`. The executable builds successfully;
a complete matching live replay remains unverified.

The corrected build was launched for the next live check on October 23, with
window/replay, converted-task and worker flags enabled. Its log is
`build/rollback-live-replay-20261023-170251.stderr.log`. The user reported normal
gameplay. The original-frame audit completed with 18,657 accepted calls and
17,476 unsupported calls, no ownership/session errors, and no unknown ordinals.
It safely refused BEFORE rewind: 124 (3,084 calls), 143 (6,168), 224 (5,140),
and 231 (3,084). This run does not demonstrate successful replay.

The next revision admits the 3.1 null-bound priority query/set slots only as
their existing zero-result dispatch behavior. Named resume/suspend calls require
valid guest output pointers and scheduler admission: no coroutine path, no
unconverted caller, no native worker self-suspend, and every affected wake or
directed-resume target must be a checkpoint-ready converted worker. Null and
pseudo handles are excluded. The production thread bridge bodies are exercised
in the dispatch fixture; the actual Windows-fiber fixture checks admission at
converted parks, nested waits, task callers and worker callers. Kernel replay,
fail-stop and fiber tests pass. The game build succeeded in
`build/rollback-thread-admission-game-build.log`. The prior session exited via
user window close, and the revised test was launched with log
`build/rollback-live-replay-20261023-170637.stderr.log`. The user reported normal
play. The audit accepted 26,904 calls but still refused before rewind: 124
(3,084), 143 (6,168), and newly observed 153 (two calls). Resume/suspend admission
passed. The priority policy was incorrect: `kx_resolve` replaces null entries
with `bridge_generic_stub`, whereas the first fixture assumed null dispatch.

The corrected policy matches that exact generic stub. Its test now extracts
the actual priority table entries and resolver and dispatches their returned
bindings. Ordinal 153 is admitted only for the existing DirectSound completion
callbacks `1F3FFA` and `1F8C51`, with bounded context/stack and exact resolved
callback identity checks. Their lifted bodies consume status bits and move two
byte counters in guest RAM; arbitrary callbacks and manual remaps are refused.
The fixture executes the production synchronization bridge and both lifted
callback bodies, checking completion results, stack cleanup and register/memory
effects. Kernel replay, fail-stop and scheduler tests pass. A complete matching
live replay remains unverified.

After the user closed the previous session, the corrected executable linked
successfully (`build/rollback-kernel-callback-game-build.log`) and was launched
with the same replay flags. The next live log is
`build/rollback-live-replay-20261023-171107.stderr.log`. This run PASSED the first
live one-frame recorded-input rewind/replay check. Workers settled after two
intervals; the original audit accepted all 36,148 calls, with no unsupported
calls or session errors. One presentation and one input frame were recorded
(four tape events, one state read, one port, one rumble call).

The earlier checkpoint restored and matched (`mask=000`, `renderer=0`). The
native driver completed the recorded interval with exactly one replay flip.
Its endpoint matched the original endpoint (`compare=1`, `mask=000`,
`renderer=0`). The original endpoint was then restored and independently
matched before normal gameplay resumed. Relevant log lines are 2045–2078.
The session later ended through user window close, with no fatal replay error.

The user reported a slight freeze near the start and otherwise normal play.
That is consistent with this one-shot diagnostic copying/checking full guest
and renderer state (seven renderer images, 45,844,224 image bytes), but this
run did not measure pause duration. This proves one sampled native interval,
not a sustained rollback session, every attack/character/stage, a longer rewind,
or network play. Next validation should repeat sampled intervals during combat
and then extend recorded input and replay to multiple consecutive frames while
retaining full endpoint comparison and recovery.

### Staged combat and two-frame diagnostic (2026-10-23)

`DOA3_ROLLBACK_COMBAT_TEST=1`, together with the existing window/replay and
converted task/worker flags, enables six bounded trials in solo Practice.
Trials 1–3 replay one frame; only three matching trials unlock the two-frame
trials 4–6. Each waits at least 480 eligible dispatcher intervals and requires
an attack category (2 or 3) at capture. Worker handoff is rechecked each time.
Move, category, attack-frame counter and health are logged for both fighters
at start/end. This samples combat state; it does not guarantee every contact,
throw, cancel or stage interaction has been covered.

The original path records each consecutive interval into its own input-tape
frame and validates eligibility/presentation count at each boundary. A single
kernel audit spans the batch; unsupported operations or a batch exceeding the
unchanged 100,000-call replay budget refuse before rewind. Native replay is
limited to two frames, advances the clock/APU once per frame, consumes each
frame's input in order, checks one flip per frame and restores the visible
frame counter on exit. Full captured state is compared at the final endpoint,
which is restored/verified even after a failed replay. Any refusal or mismatch
stops the sequence. No automatic retries or network session are enabled.

All 22 targeted checks pass: 17 native-driver/probe cases and five loop,
presentation, input-tape and checkpoint cases. New cases cover the attack gate,
three singles before doubles, six-trial limit, stop on mismatch, mode/input
failure in the second recorded frame, second replay-frame failure, frame-index
ordering, and recovery to the saved frame-two endpoint. Build logs:
`build/rollback-combat-sequence-tests-build.log`,
`build/rollback-combat-recovery-tests-build.log`, and
`build/rollback-combat-sequence-game-build.log`. The executable builds; live
combat/multi-frame results are pending. Diagnostic duration reporting includes
capture, the recorded interval(s), verification and recovery, not replay CPU
time alone.

The staged build is running with live log
`build/rollback-combat-20261023-171833.stderr.log`. Trial 1 PASSED during an
attack: side 1 move 143/category 3 advanced from attack frame 3 to 4. All
36,160 original kernel calls were admitted; the replay endpoint and renderer
matched, and recovery matched. The measured diagnostic span was 1,500 ms,
including 1,125 ms for verification/replay/recovery (not replay alone).

Trial 2 began with side 1 move 278/category 3 and side 2 category 9. The audit
safely stopped the sequence before rewind: ordinal 173 occurred three times
and 175 twice. Therefore the run did NOT reach two-frame replay. The user
reported normal gameplay. The next revision admits only the actual
`MmGetPhysicalAddress` bridge (returns the supplied guest address without
translation/dereference) and the exact generic stub resolved for
`MmLockUnlockBufferPages` (no-op in this port). Tests exercise the actual table,
resolver and bridge on zero, ordinary and all-bits-set addresses, unchanged
guest memory, stack cleanup, and rejection of remapped bindings or invalid
stack ranges. Kernel dispatch and fail-stop tests pass. Live validation is
pending; no additional combat/multi-frame result is claimed.

The revision rebuilt successfully (`build/rollback-combat-memory-game-build.log`)
and was launched after the prior session's user window close. Repeat staged
validation in `build/rollback-combat-20261023-172356.stderr.log` PASSED all six
trials (lines 2431–2818):

| Trial | Frames replayed | Side 1 move/category | Attack frame progression | Result |
| --- | --- | --- | --- | --- |
| 1 | 1 | 143 / 3 | 3 → 4 | All domains and renderer matched |
| 2 | 1 | 358 / 3 | 4 → 5 | All domains and renderer matched |
| 3 | 1 | 141 / 3 | 2 → 3 | All domains and renderer matched |
| 4 | 2 | 272 / 3 → 0 / 0 | 46 → idle frame 1 | All domains and renderer matched |
| 5 | 2 | 153 / 2 | 4 → 6 | All domains and renderer matched |
| 6 | 2 | 290 / 3 | 12 → 14 | All domains and renderer matched |

Trials 2 and 4 also sampled the defender in category 9. Every audit had zero
unsupported calls and no ownership/session errors. Each replay produced exactly
its requested interval/flip count, matched the original final endpoint with
`mask=000` and `renderer=0`, and restored/independently matched that endpoint.
The sequence ended automatically after six trials. The user reported normal
play, and the process later exited through user window close.

Diagnostic spans were 1,390–1,469 ms; verification/replay/recovery portions were
1,047–1,125 ms. These include full snapshots and comparisons and do not measure
replay CPU time in isolation. This establishes nine replayed frames across six
sampled combat windows, not correctness for every frame or move. Intermediate
frame-one state inside a two-frame batch was not independently compared.
Next work is longer bounded windows (4–8 frames), broader combat scenarios,
separate replay/capture/restore timings, and reducing snapshot cost before
attempting continuous rollback or network integration.

### Four/eight-frame combat probe (six live samples passed)

Add `DOA3_ROLLBACK_LONG_TEST=1` to the existing WINDOW_TEST, REPLAY_TEST and
COMBAT_TEST launch flags to run three four-frame samples followed by three
eight-frame samples. Eight-frame trials unlock only after all three four-frame
endpoints match. The attack gate, 480-interval spacing, six-trial limit and
stop-on-refusal/mismatch behavior remain in place. Without LONG_TEST, the
existing one/two-frame sequence is unchanged. This is a bounded diagnostic,
not continuous rollback or network play.

The native driver now accepts at most eight frames. Each interval consumes its
own recorded input and advances the deterministic clock/APU once, with exactly
one flip required. The original kernel audit and enforcing replay guard have a
total batch allowance of 100,000 calls times the requested frame count. The
guard allowance can be configured only once, by its owner before any replay
calls, without a pin or fail-stop mode active, in the same deterministic epoch.
It cannot be widened during replay; callers that do not configure it retain the
100,000-call default. Unsupported calls still refuse the original audit or stop
replay independently of this allowance.

`[ROLLBACK-TIMING]` reports host high-resolution durations for restoring/checking
the start, the replay callback, comparing the replay endpoint, and restoring/
checking the original endpoint. These are diagnostic costs; the replay value
still includes instrumented helper/render work. Initial capture, preflight and
APU setup/teardown are not all included in those individual fields, so their
sum is not the overall diagnostic duration.

The production executable builds and all 25 targeted native-frame,
fight-window, checkpoint, kernel-replay and fail-stop tests pass. Tests include
four/eight-frame input/clock/presentation ordering, final-frame failures,
stopping progression on mismatch, exact call limits and rejected attempts to
change the budget after replay starts. Build logs:
`build/rollback-long-window-tests-build.log` and
`build/rollback-long-window-game-build.log`. As with two-frame batches, only the final replay endpoint is compared
against a full original checkpoint; intermediate endpoints are not independently
compared.

The staged long-window build was launched with all four diagnostic flags and
converted task/worker flags enabled. Live log:
`build/rollback-long-20261023-173342.stderr.log`. All six combat trials PASSED
(lines 2146–3027):

| Trial | Frames | Side 1 move/category | Attack frame progression | Replay callback ms |
| --- | --- | --- | --- | --- |
| 1 | 4 | 143 / 3 | 1 → 5 | 55.963 |
| 2 | 4 | 292 / 3 | 45 → 49 | 54.121 |
| 3 | 4 | 272 / 3 | 11 → 15 | 53.834 |
| 4 | 8 | 324 / 3 | 13 → 21 | 103.159 |
| 5 | 8 | 295 / 3 → 296 / 3 | 16 → next move frame 4 | 110.719 |
| 6 | 8 | 153 / 2 → 358 / 3 | 36 → next move frame 1 | 108.606 |

All original audits reported zero unsupported, invalid or unknown calls.
Four-frame batches admitted 144,579–144,754 calls; eight-frame batches admitted
289,441–289,786. Every batch produced exactly its requested intervals and flips,
matched the full final state and renderer (`compare=1 mask=000 renderer=0`),
then restored and independently matched the original endpoint. Trials 5 and 6
crossed attacker move transitions; trial 5 also crossed the defender from
category 0 to category 9. The sequence stopped successfully after six trials.
The user reported normal feel, and the game remained running/responding.

This validates 36 replayed frames across six sampled windows, not all combat
scenarios. Replay callbacks took 53.8–56.0 ms for four frames and 103.2–110.7 ms
for eight. Restoring/checking the start took 330.7–357.3 ms, endpoint comparison
179.9–193.9 ms, and original endpoint recovery/check 336.1–357.6 ms. Overall
diagnostic spans were 1,484–1,641 ms. These instrumented full-snapshot checks are
not yet fast enough for continuous real-time rollback. Next priorities are
reducing snapshot/restore/compare and replay cost, plus broader combat coverage
(throws, counters, walls and stage interactions) before network integration.

### Checkpoint checksum performance (live comparison pending)

The local checkpoint integrity checksum now uses pinned xxHash 0.8.3 XXH3-64
instead of a serial byte-at-a-time FNV-1a pass. Schema 7 rejects older checksum
formats; this changes only ephemeral diagnostic checkpoints, not game saves.
All header, padding and domain bytes remain covered, with only the checksum
field normalized to zero. Neither checksum is an authentication mechanism.
Full byte-by-byte endpoint comparisons, renderer comparisons, preflight
validation and failed-restore compensation are unchanged.

`rollback_checkpoint_bench` measures the production capture/restore/compare APIs
with 128 MiB of patterned RAM and nine small fixture domains. It forces a RAM
restore and verifies a mismatch at the last RAM byte. It excludes the actual
renderer/APU/fiber workload and is not an in-game rollback timing. On this host,
warm runs (1–3) measured:

| Operation | Before (ms) | After (ms) |
| --- | --- | --- |
| Capture | 157.790–168.696 | 49.388–52.898 |
| Restore | 145.152–171.713 | 40.090–44.023 |
| Compare | 138.398–151.526 | 30.997–31.103 |

Logs: `build/rollback-perf-before.log`, `build/rollback-perf-after.log`.
The checksum test now flips every bit in every serialized byte (including
header, checksum and padding), requiring refusal before any restore callback.
The benchmark has no timing assertion, because host load affects results.
The production build succeeded (`build/rollback-perf-game-build.log`). Live
four/eight-frame matching and timing comparison remain pending.

All 19 targeted checkpoint, fatal recovery, native replay, long-window, trial,
renderer transaction and combined live audio/render checkpoint tests passed
(`build/rollback-perf-tests.log`); build log:
`build/rollback-perf-all-tests-build.log`. The optimized build was launched with
the same staged four/eight-frame flags; live log:
`build/rollback-perf-20261023-174013.stderr.log`.

That live run matched trial 1 (four frames, move 272/category 3, attack frame
29 to 33) with all domains and renderer equal, exact interval/flip counts and
verified original endpoint recovery. Its overall diagnostic was 625 ms
(previous four-frame samples: 1,484–1,516 ms); restore/start check 136.894 ms,
replay 57.655 ms, endpoint compare 55.550 ms, and recovery/check 106.396 ms.
Replay CPU work itself is not faster in this sample. Different combat windows
were sampled, so this is not a frame-identical performance comparison.

Trial 2 stopped safely before any rewind (157 ms capture span, 0 ms verification,
no restore/timing output); its original audit had no rejected calls. The
existing logging cannot distinguish the early context, guard or physical-pin
refusal. No eight-frame trials ran. The user reported normal feel and the game
remained responsive. Added explicit early-refusal and clock/audio-pin logging
to identify this on the next run; no retry policy or guard relaxation added.
Checkpoint success/fatal tests pass after the reporting change.
The user then closed the previous game (log line 2346). The reporting revision
built successfully (`build/rollback-perf-refusal-game-build.log`) and was opened
for another staged run:
`build/rollback-perf-refusal-20261023-174309.stderr.log`. All six trials PASSED
(lines 2069–2974), each with exact interval/flip counts, all-domain and renderer
endpoint matches (`mask=000 renderer=0`), and independently verified recovery.
No early refusal recurred in this run; the previous refusal's cause remains
unidentified. All original audits had zero unsupported/invalid/unknown calls.

| Trial | Frames | Side 1 move/category | Attack frames | Overall diagnostic ms | Replay callback ms |
| --- | --- | --- | --- | --- | --- |
| 1 | 4 | 301 / 3 | 28 → 32 | 594 | 53.898 |
| 2 | 4 | 369 / 3 | 15 → 19 | 609 | 55.005 |
| 3 | 4 | 322 / 3 | 8 → 12 | 625 | 57.033 |
| 4 | 8 | 322 / 3 | 23 → 31 | 657 | 105.095 |
| 5 | 8 | 272 / 3 | 20 → 28 | 703 | 99.662 |
| 6 | 8 | 301 / 3 | 23 → 31 | 703 | 126.348 |

Compared with the prior successful six-trial baseline, overall four-frame
diagnostics fell from 1,484–1,516 ms to 594–625 ms, and eight-frame diagnostics
from 1,593–1,641 ms to 657–703 ms (roughly 58–59% lower group averages).
Restore/start checks were 112.498–135.628 ms, endpoint comparisons
57.683–78.412 ms, and recovery/checks 101.503–122.960 ms. Different combat
windows were sampled. This demonstrates reduced checkpoint overhead, not a
proven improvement in replay execution: eight-frame callbacks still cost
99.662–126.348 ms. The game remained responsive after the sequence completed.
Continuous real-time rollback still requires reducing replay work and snapshot
cost; these diagnostic durations are not input-latency measurements.

### Replay execution profiling (live results pending)

Native replay no longer writes a stderr line before every helper by default.
`DOA3_ROLLBACK_REPLAY_TRACE=1` retains the original trace for a helper that
hangs/crashes; failures still report the last helper without this flag.
This removes up to 144 synchronous helper-log writes from an eight-frame batch.
Its actual effect on replay time has not yet been measured.

`DOA3_ROLLBACK_REPLAY_PROFILE=1` records host QPC time per source-pinned helper,
the deterministic clock/APU step, and the remaining driver work. Helper totals
include all work nested below that helper (including guest tasks or rendering).
Timing summaries are emitted after guest replay has ended and fail-stop mode is
off; they do not alter guest state or kernel admission. `execution_ms` excludes
the final log output, whereas the outer checkpoint `replay_ms` includes it.
Both profiling and verbose trace are opt-in and do not skip simulation work,
audio processing, draws, or endpoint comparisons.

The normal, profiled and traced native-driver tests pass, including four/eight
frame success/failure, input consumption, clock/flip counts and cleanup. Eight
targeted tests passed in `build/rollback-replay-profile-tests.log`; the game
build succeeded in `build/rollback-replay-profile-game-build.log`. The staged
combat probe was launched with profiling enabled and verbose trace disabled:
`build/rollback-replay-profile-20261023-174759.stderr.log`. All six trials passed
with exact endpoint/renderer matches and recovered endpoints. Four-frame
execution (excluding summary output) took 45.119–51.065 ms, eight-frame execution
98.634–101.491 ms. Dispatcher helper `001C650C` accounted for 43.531–48.975 ms
and 92.401–98.729 ms respectively (about 94–97% of execution); clock/APU steps
cost only 0.290–1.421 ms. Removing helper trace writes did not eliminate the
main cost. The complete helper breakdown is retained in the live log.

A separate 15-second main-thread sampler of the running ordinary fight
(`build/render-samples.json`) found `get_dynamic_texture` the largest named
game hotspot (641 of 9,797 PC samples), followed by `pgraph_d3d11_method` (589).
4,566 samples were in ntdll, mostly accompanied by frame pacing on the stack.
This sample was NOT restricted to rollback and is not a measurement of the
dispatcher's exclusive subtask costs; it identifies renderer work to optimize
and then validate with the bounded replay probe.

### Small texture fingerprint optimization (live results pending)

The production texture cache now uses XXH3 for complete fingerprints of sources
up to 16 KiB, on every lookup. This replaces the serial dword FNV pass without
skipping checks or relying on once-per-frame freshness. Larger sources retain
their exact previous sampled fingerprint policy. Palette changes still affect
the cache signature; restore still invalidates all guest uploads and refreshes
every mip level. Signatures are host-only cache hints, not snapshot data.

A same-process benchmark of 20,000 16 KiB lookups measured 73.2–74.9 ms for the
old function versus 10.5–10.9 ms for the new function
(`build/rollback-texture-signature-benchmark.log`). This is the isolated hash
cost, not an overall replay speedup. Tests check individual byte mutations,
unaligned sources, small/tail sizes, unchanged large-image fingerprints,
warm-cache palette animation and small-source refresh, restored mip data,
upload failures, renderer transactions and combined audio/render checkpoints.
All four targeted tests passed (`build/rollback-texture-perf-tests.log`). The
game compiled but linking hit LNK1104 because DOA3.exe was running
(`build/rollback-texture-perf-game-build.log`). Awaiting user close to relink
and rerun live four/eight-frame comparison; no overall speedup claimed yet.
After the user closed the game, linking succeeded (same build log). The
optimized build was opened with the same long-window and profiling flags:
`build/rollback-texture-perf-20261023-175402.stderr.log`. All six trials passed
(lines 2149–2485), with exact final state/renderer matches, independent endpoint
recovery and no unsupported/invalid/unknown original kernel calls. The process
remained responsive after the sequence.

| Frames | Previous execution ms | Optimized execution ms |
| --- | --- | --- |
| 4 | 51.065, 45.119, 45.990 | 50.955, 45.372, 47.140 |
| 8 | 99.755, 98.634, 101.491 | 87.331, 83.186, 89.857 |

The eight-frame mean decreased from 99.960 to 86.791 ms (about 13.2%); four-frame
execution showed no clear improvement (47.391 vs 47.822 ms means). Samples are
different combat windows rather than identical replay tapes. Dispatcher totals
for eight frames fell from 92.401–98.729 to 80.363–87.064 ms, consistent with
reduced rendering work, but controlled repeated A/B runs remain necessary to
isolate the change from scene/host variance. Overall diagnostic spans were
609–656 ms for four frames and 672–719 ms for eight, so this run does not show
a reduction in total diagnostic pause. Snapshot/compare overhead remains large,
and even the reduced replay cost is not sufficient for real-time rollback.

### Per-task replay timing (live results pending)

With `DOA3_ROLLBACK_REPLAY_PROFILE=1`, the native replay driver now brackets a
host-only scheduler profile around each batch. The scheduler accumulates QPC
duration and step counts per converted fight-task fiber, and prints
`[ROLLBACK-TASK-PROFILE]` totals after replay/fail-stop mode ends. These are
inclusive step durations: nested worker work is included and overlapping
durations must not be summed as exclusive CPU costs. Timing state is excluded
from checkpoints. A generation check excludes an unfinished step that crosses
profile sessions. Disabled profiling performs no QPC calls in task steps.

The actual Windows-fiber test verifies that profiled replay produces the same
guest results as its unprofiled baseline. Native-driver tests cover profiler
cleanup on success and failed input/flip paths. All four targeted tests pass,
and the game builds (`build/rollback-task-profile-tests-build.log`,
`build/rollback-task-profile-game-build.log`). Live staged combat log:
`build/rollback-task-profile-20261023-175924.stderr.log`. This change adds
measurement, not another claimed replay speedup.

The run stopped on trial 1 before rewind: `acquire refused: audio pin`, followed
by `refused before rewind: physical pins` (lines 2098–2103). No task replay or
timing completed. The original audit had no unsupported/invalid/unknown calls.
This identifies the refused pin in this run; it does not establish the precise
cause of the older uninstrumented refusal.

The initial pre-rewind physical-pin acquisition now permits up to eight attempts
with seven 1 ms host sleeps. Actual wall time may exceed that under scheduling
delays. Every failed audio acquisition releases the clock pin before waiting;
context is checked each time. No guest steps, output-gate changes or restores
occur while waiting. Full endpoint preflight still runs after acquisition, and
persistent refusal still stops the sequence. This does not retry failed replay,
change other checkpoint acquisitions, or weaken validation.

Checkpoint tests pass for persistent refusal without capture/restore, transient
contention followed by verified replay, and a changed endpoint during the wait
being rejected before restore. The game builds
(`build/rollback-pin-wait-tests-build.log`, `build/rollback-pin-wait-game-build.log`).
New live task-profile run:
`build/rollback-task-profile-wait-20261023-180246.stderr.log`. All six trials
passed, with `compare=1 mask=000 renderer=0` at every endpoint. Four-frame
execution times were 56.107, 45.632 and 41.183 ms; eight-frame times were
82.431, 74.758 and 90.184 ms. These remain separate live samples, not a
controlled performance comparison. No pin retry was reported in this run.

The largest inclusive task durations across the eight-frame trials were
`000691F0` (14.814–24.706 ms), `00049790` (21.770–22.169 ms), and `000E8030`
(9.707–14.773 ms). Input task `00011110` took 0.111–0.145 ms and combat task
`000B4720` took 0.170–0.190 ms. Inclusive task timing may include nested worker
execution; it does not establish exclusive ownership of that cost.

### Avoid unused generic vertex setup on NV2A fixed-function draws

`d3d8_DrawNv2aFF` previously ran the generic XYZRHW shader preparation before
replacing its vertex shader, layout and b0 buffer with the NV2A equivalents.
It now calls the shared pixel-only preparation. This removes two unused
constant-buffer Map/Unmap uploads and generic VS/layout/buffer bindings per
affected draw. Pixel constants, shader binding, combiners, render/sampler state,
the NV2A vertex/lighting constants and all draw calls still run normally.
Ordinary D3D8 draws use the complete generic preparation. No cache or new
checkpoint state is introduced.

The production PGRAPH/WARP test compares the old full setup against the new
pixel-only setup, with mixed inline/indexed draws, offscreen images and
checkpoint restoration after hostile future state/ClearState. It verifies
both paths were exercised and their image hashes match. Renderer transaction
and CPU/audio/renderer checkpoint integration tests also exercise the change.
The removal of redundant work is not a demonstrated live speedup. All three targeted
tests pass and the Release game builds (`build/rollback-pixel-setup-tests-build.log`,
`build/rollback-pixel-setup-game-build.log`). New staged live log:
`build/rollback-pixel-setup-20261023-181147.stderr.log`: all six trials passed,
with all earlier/endpoint comparisons reporting `compare=1 mask=000 renderer=0`
and each original endpoint restored and matched. Four-frame execution times
were 45.120, 45.299 and 47.467 ms (mean 45.962 ms); eight-frame times were
83.224, 92.881 and 95.011 ms (mean 90.372 ms). Previous run means were 47.641
and 82.458 ms respectively. Four frames averaged 3.5% less time and eight
frames 9.6% more; different combat windows and host scheduling prevent a
controlled A/B conclusion. This run does not demonstrate a live speedup.
Total diagnostic pauses remained 625–735 ms, including captures, restoration
and comparisons; those are separate from the execution figures above.

Task `000E8030` reached 23.540 ms in the final eight-frame batch, compared with
14.773 ms in the previous run's final batch. This is an inclusive task duration,
not proof of a renderer regression. Further performance work needs finer
measurements within the expensive tasks rather than extrapolating the removed
buffer uploads into a claimed overall improvement.

### Repeat with optional diagnostics removed

At the user's request, removed the native replay's per-helper/per-task timers,
profiling arrays, trace switches and reports, including the fiber profiling API.
Only two QPC reads bracket the full execution, with a single
`ROLLBACK-EXECUTION` result emitted after timing stops. Old PROFILE/TRACE
environment variables no longer enable instrumentation in this driver.

Removed recurring renderer diagnostic work: filter/mip/address/depth/culling/
stencil/blend summaries, projective/clip statistics, combiner reports,
post-movie vertex census/scans, target-state dumps and method histograms.
Removed the audio output's per-sample diagnostic peak/clipping meter. These
changes preserve rendering decisions, audio samples and volume processing.
Kernel flight-recorder writes and periodic summary clock polls/log flushing
now require `DOA3_KERNEL_DIAGNOSTICS=1` and are off by default. Failure reports
and replay admission/ownership/budget guards remain active.

The repeat run clears optional capture, trace, performance and watch flags.
The staged test still captures start/end state, verifies replay and restores
the original endpoint: those operations happen outside `execution_ms` and
still cause the bounded test pauses. This is not a production rollback-cost
measurement or removal of correctness checks. Dormant diagnostic code and
lightweight counters elsewhere are not claimed to have been deleted.

Six targeted tests pass (native frame, fibers, kernel replay and fatal-stop,
production PGRAPH rendering, CPU/audio/renderer checkpoint integration).
Kernel tests additionally verify that the disabled flight recorder remains
untouched. Release builds successfully; logs:
`build/rollback-lean-tests-final.log`, `build/rollback-lean-game-build.log`.
Live run: `build/rollback-lean-20261023-182310.stderr.log`: all six checks passed
with matching state and renderer endpoints and verified endpoint recovery.
Four-frame execution: 51.517, 43.156, 43.464 ms (mean 46.046); eight-frame:
80.182, 87.506, 94.509 ms (mean 87.399). Previous means were 45.962 and
90.372 ms. The difference is negligible for four frames and about 3.3% lower
for eight; these different combat windows do not isolate diagnostic overhead.
Snapshot/verification pauses were 704–844 ms; renderer inventories here reached
75,882,752 bytes, so total diagnostic cost is also sensitive to scene resources.

Review caught one remaining `[DRAW]` summary inside trial 1's timed execution.
The older push-buffer kickoff path still polled wall time for this summary and
periodic heap statistics. Removed those blocks and the periodic presented-frame
log as a follow-up. The run above is therefore not claimed to be entirely free
of routine logging. The follow-up presentation test passes and Release linked
successfully after closing the locked executable. New live run:
`build/rollback-lean-present-20261023-182753.stderr.log`: all six trials passed
with matching state/images and recovered endpoints. Four-frame execution was
45.615, 52.978, 53.846 ms (mean 50.813); eight-frame execution was 98.835,
95.102, 100.925 ms (mean 98.287). There is no demonstrated speedup compared
with the preceding 46.046/87.399 ms means; these are different combat samples.
Total diagnostic pauses were 609–719 ms.

Removed summaries and the detailed profilers did not reappear. Trial 1 still
contained four `[DSVOICE]` observations from the old bounded DirectSound list
observer. Trials 2–6 had no log lines between the earlier-state comparison and
the completed-batch report, yet eight-frame execution remained 95–101 ms.
This does not measure the residual cost of guards/counters, but synchronous
logging alone cannot explain those five execution windows.

The DirectSound observer is now removed too: `sub_001F3FA0` directly invokes
the original service, preserving its guest behavior without an extra four-list
walk or diagnostic writes. This follow-up compiled; installation initially
blocked by the running executable (`build/rollback-lean-voice-game-build.log`).
After the user closed the game, Release linked successfully. Audio-service,
audio-sync and CPU/audio/renderer integration tests all pass. New staged run:
`build/rollback-lean-voice-20261023-183330.stderr.log`: all six trials passed,
with `compare=1 mask=000 renderer=0` at every replay endpoint and all original
endpoints restored and matched. No log lines occurred between the verified
earlier-state comparison and completed-batch report in any of the six trials.
Four-frame execution was 48.408, 45.243, 51.751 ms (mean 48.467); eight-frame
execution was 89.463, 87.291, 97.199 ms (mean 91.318). Total diagnostic pauses
were 672–781 ms, including snapshot/verification work outside execution.

Compared with the immediately preceding 50.813/98.287 ms averages, these means
are lower, but the first pre-cleanup pixel-setup run averaged 45.962/90.372 ms.
The results overlap prior variability and do not demonstrate a material replay
speedup from removing optional diagnostics. These are separate combat windows,
not repeated execution of an identical saved input sequence. Admission guards,
input validation and normal simulation/rendering remain in the execution path;
their individual costs have not been isolated by this comparison.

### Avoid duplicate guard locking and repeat identical recorded windows

The native samples admit about 289,000 guest kernel calls per eight frames.
Dispatch previously took a second replay-health lock after every admitted call,
even when no worker scheduling occurred. It now repeats that check only when
`xbox_fiber_timeslice` was invoked. Initial binding/argument/owner/epoch/budget
admission remains unconditional. The intervening non-scheduling path only
updates call counters; scheduled worker faults still prevent bridge execution.

The production dispatcher fixture includes a benchmark-only reference retaining
the old unconditional second check (never linked into the game). Five alternating
300,000-call samples for the same supported critical-section binding measured
reference 9.667, 9.543, 9.357, 8.953, 9.304 ms versus optimized 6.289, 6.061,
6.219, 5.801, 5.960 ms. Both paths executed exactly 300,000 accepted calls with
identical endpoint and stack-cleanup assertions. This isolates dispatcher cost
with stub bridges; it does not establish a whole-fight speedup.

`DOA3_ROLLBACK_REPEAT_TEST=1` repeats each staged four/eight-frame window three
times against the same start/end images and recorded tape. Each iteration
independently restores start, executes, compares the endpoint, restores the
original endpoint and verifies recovery. Any refusal/mismatch stops repetitions
and the staged sequence; this is not a retry mechanism. Per-helper/task profiling
remains removed and only total execution timing is collected. Repetitions extend
the bounded snapshot/verification pause. First versus subsequent iterations may
still differ due to host cache/scheduling effects, which is what this test measures.

All 24 staged-window and kernel tests pass, including a failure on the second
repeat stopping the sequence, reuse of exactly two images and the same tape,
worker faults before bridge execution and fatal rejection. Build/test logs:
`build/rollback-repeat-tests-build.log`, `build/rollback-dispatch-tests-build.log`.
Release builds successfully (`build/rollback-repeat-dispatch-game-build.log`).
Live validation completed in
`build/rollback-repeat-dispatch-20261023-184148.stderr.log`. All 18 start,
replayed-endpoint and original-endpoint recovery comparisons matched, including
renderer comparisons. All six combat trials passed.

| Trial | Frames | First execution ms | Second ms | Third ms |
|---|---:|---:|---:|---:|
| 1 | 4 | 41.657 | 39.643 | 38.886 |
| 2 | 4 | 46.953 | 47.651 | 43.953 |
| 3 | 4 | 44.327 | 44.346 | 41.459 |
| 4 | 8 | 75.428 | 72.500 | 76.184 |
| 5 | 8 | 79.318 | 73.475 | 72.313 |
| 6 | 8 | 79.771 | 69.347 | 74.406 |

First-execution means are 44.312/78.172 ms for four/eight frames; all-repetition
means are 43.208/74.749 ms. The prior clean build measured 48.467/91.318 ms,
but it used different combat windows and larger captured renderer images.
This run reports 46,368,512 image bytes. Consequently, the lower whole-fight
times are not a controlled measurement of the dispatcher optimization. Only
the isolated identical-call benchmark above establishes that path's speedup.
Repeated execution still varies for identical inputs, so single samples are
insufficient. Total diagnostic pauses were 1,437–1,718 ms, including all three
restore/compare/recovery passes outside the execution timer. These costs still
exceed a 16.67 ms frame budget; this is correctness/performance evidence for
bounded local replay, not readiness for live rollback netplay.

### Retain verified guest texture uploads across rewind

The restore barrier previously forced every guest texture to upload again.
Static swizzled/DXT uploads now retain an exact copy of their complete source
chain and palette. After a restore they are marked for validation: the next bind
compares all source bytes and the source layout before reusing the GPU texture.
This avoids repeated unswizzling, palette expansion, per-mip locks and GPU
uploads for unchanged assets. Changed lower mips, unsampled base-level bytes,
palettes and layout changes cannot use the level-zero signature shortcut at
that barrier. Linear/movie uploads retain their unconditional upload behavior.

Source-copy capacity is charged to the existing 192 MiB texture LRU budget,
freed on eviction/shutdown, and is host-only derived state. Allocation failure
falls back to uploading again. Failed or incomplete uploads remain invalid
through later restore barriers. Guest simulation, rendering commands, feedback
images and full endpoint comparison are unchanged. Rendering intermediate
frames cannot simply be dropped: later draws may sample their image history.

All 29 targeted tests pass (`build/rollback-texture-reuse-tests.log`), including
the production texture upload path, full PGRAPH/WARP pixels, coordinated
renderer restore/compensation, and CPU/audio/renderer integration. Tests cover
unchanged chains across repeated barriers with zero additional uploads,
mutation of every byte in a four-level DXT chain and every palette byte,
sampled-signature misses, allocation/lock/unlock failures, and linear uploads.
Release builds successfully (`build/rollback-texture-reuse-game-build.log`).

`DOA3_ROLLBACK_TEXTURE_AB_TEST=1` selects four fully verified executions of each
saved input window: forced uploads / exact reuse / exact reuse / forced uploads
on odd trials, reversed on even trials. Both paths use the same executable,
source-copy bookkeeping, snapshots, resolution and recorded inputs; the sole
switch is restore-time upload reuse. The mode label is printed outside timed
execution, and the prior mode is restored on success or a rejected comparison.
Any failed comparison stops the trial/sequence. Tests verify ordering, tape and
image reuse, cleanup and stopping on a second-sample mismatch. Detailed
per-helper profiling remains disabled. Live performance gains are not yet
measured; each four-repeat diagnostic pause will be longer than a single test.
Live comparison completed with detailed diagnostics disabled:
`build/rollback-texture-reuse-ab-20261023-185546.stderr.log`. All 24 executions,
start-state comparisons, replayed endpoint comparisons (including renderer),
and original-endpoint recovery comparisons passed. All six combat trials
completed. The game remained running and responsive after the test.

| Trial | Frames | Forced upload samples ms | Exact reuse samples ms |
|---|---:|---|---|
| 1 | 4 | 46.610, 46.460 | 40.559, 37.602 |
| 2 | 4 | 42.480, 41.232 | 41.377, 41.715 |
| 3 | 4 | 39.501, 40.681 | 35.518, 40.254 |
| 4 | 8 | 77.171, 81.188 | 75.199, 75.205 |
| 5 | 8 | 78.596, 68.951 | 67.187, 65.102 |
| 6 | 8 | 88.936, 91.664 | 82.199, 86.303 |

Across six samples per path and length, four-frame means were 42.827 ms forced
versus 39.504 ms reused (3.323 ms / 7.8% less execution time). Eight-frame means
were 81.084 versus 75.199 ms (5.885 ms / 7.3% less). Each pair uses the same
captured combat window and resolution. Individual timings still vary; this is
a modest measured saving in this run, not a large enough improvement for live
netplay. Four-frame trial images were 45,844,224 bytes and eight-frame images
46,368,512 bytes; comparisons across different trials are not identical scenes.
The six total diagnostic pauses were 1,829–2,157 ms, encompassing four verified
replays and restore/compare/recovery work outside execution timing. Those pauses
are not representative of a production rollback implementation.

### Bulk vertex/index packet submission (2026-10-23)

The synchronous 3.1 push-buffer walker can submit non-incrementing INLINE_ARRAY,
ARRAY_ELEMENT16 and ARRAY_ELEMENT32 payloads directly to PGRAPH. Eligible
packets bypass the full method dispatcher for each individual word. Inline and
32-bit index payloads use bounded copies; 16-bit indices are unpacked in order.
Final register shadows, logical buffers (including unused tails), counters,
overflow/drop behavior and draw ordering match scalar processing. State/draw
commands, traced streams, unsupported packets and declined callbacks retain
the existing scalar path. Guest payload bounds and alignment are checked first.

All 33 targeted tests pass (`build/rollback-bulk-packets-tests.log`), including
packet ordering, nested call/return handling, malformed and declined packets,
full logical-state equivalence at capacity boundaries, and actual scalar versus
bulk rendered pixels through the production walker. Renderer tests include
offscreen targets, prior-frame history and hostile-state restoration. Release
build succeeds (`build/rollback-bulk-packets-game-build.log`).

The isolated INLINE_ARRAY benchmark processes 10,240,000 words per sample:
scalar 25.915–27.157 ms versus bulk 0.217–0.290 ms across four alternating
samples (`build/rollback-packet-bench.log`). This measures hot packet dispatch
only, not full replay execution or GPU work; it does not establish a live gain.

`DOA3_ROLLBACK_PACKET_AB_TEST=1` selects four verified replays of each recorded
window, scalar/bulk/bulk/scalar on odd trials and the reverse on even trials.
Texture reuse stays enabled in both paths. The packet and texture A/B modes
cannot run together. The previous dispatch mode is restored even on a refused
comparison; a failure stops the sequence. One host-only word counter increment
per accepted packet confirms actual use, with reports outside replay timing.
No per-helper timing or recurring profiler is added.

Live comparison completed in
`build/rollback-bulk-packets-ab-20261023-195044.stderr.log` (PID 7280).
All 24 initial-state, replayed endpoint (including renderer), and recovered
original-endpoint comparisons matched. All six combat trials passed, with no
fatal error; the process remained responsive after the run. Bulk executions
processed 189,847–256,334 words per four-frame window and 363,387–381,916 per
eight-frame window. Every scalar execution reported zero bulk words.

| Trial | Frames | Scalar execution ms | Bulk execution ms |
|---|---:|---|---|
| 1 | 4 | 42.105, 44.381 | 43.942, 42.362 |
| 2 | 4 | 38.797, 42.735 | 37.596, 38.932 |
| 3 | 4 | 34.158, 37.534 | 33.015, 36.221 |
| 4 | 8 | 76.706, 78.327 | 71.114, 72.528 |
| 5 | 8 | 69.163, 73.828 | 71.519, 72.322 |
| 6 | 8 | 73.435, 75.800 | 70.959, 72.627 |

Six samples per path/length averaged 39.952 -> 38.678 ms for four frames
(1.274 ms / 3.2% reduction), and 74.543 -> 71.845 ms for eight frames
(2.698 ms / 3.6%). These are execution-only measurements on identical saved
inputs within each trial, with texture reuse enabled for both paths. Timing
variation remains, and trial 5 showed no gain. The isolated packet benchmark
does not translate into a similarly large whole-replay improvement. Full
four-repeat diagnostic pauses were 1,750–2,078 ms, including snapshot,
comparison and recovery work outside execution timing. This remains well
above a usable live rollback budget; substantial render/guest-work reductions
are still required.

### Reuse packed geometry and resident buffers (2026-10-23)

The NV2A fixed-function array path now caches complete object-space mesh
preparation: unique-vertex conversion, index remapping, fan/quad expansion,
and the input-color summary. On an exact second use it creates immutable
D3D11 vertex/index buffers, avoiding both dynamic ring uploads on subsequent
draws. One-off geometry uses the existing rings. Programmed/CPU/inline paths
remain unchanged. Every draw still runs current transforms, lighting, fog,
materials, textures, target/scissor setup and rasterization; no intermediate
render targets, frame history, or guest tasks are skipped.

The cache key includes primitive/layout, enabled source attributes and the
floating-point conversion control. Index bytes and all source bytes in merged
attribute spans are compared exactly on every hit. Hashes only select candidate
entries. A restored guest memory image is therefore independently validated;
no invalidation exemption or probabilistic content match is used. Invalid,
overflowing or excessively sparse spans fall back to normal preparation.
Up to 1,024 entries use a 64 MiB budget covering retained source, CPU output and
reserved GPU copies. Four-way sets and global LRU budget eviction release both
CPU/native resources. Cache state is host-only derived data, outside snapshots.
Allocation failure retains the original draw path; device mismatch rejects the
old native buffer and returns to ring uploads. Init/shutdown clear the cache.

All 33 targeted tests pass (`build/rollback-geometry-tests.log`). Coverage
includes every retained source/index/key byte, deliberate hash collisions,
old-source restoration, each CPU allocation failure, set and byte-budget
eviction, overlapping/constant attribute spans, range overflow, and real WARP
rendered pixels. The latter compares rebuild/reuse with changes to geometry,
color, camera constants and triangle/list/strip/fan/quad topology, allocation
fallback, hostile future state, offscreen/history targets and coordinated
checkpoint restore. Native resource references are checked at teardown.
Release builds successfully (`build/rollback-geometry-game-build.log`).

`DOA3_ROLLBACK_GEOMETRY_AB_TEST=1` runs the same six four/eight-frame combat
windows with four verified repetitions each: rebuild/reuse/reuse/rebuild,
reversed on alternate trials. Texture reuse and bulk packet processing remain
enabled in both paths. Other A/B modes are rejected when selected together.
Host counters report hits, misses, reused vertices and resident draws outside
execution timing; no per-draw timers or logging are added. Each comparison
must pass before the next executes, and the prior reuse setting is restored
on refusal as well as success.

Live comparison completed in
`build/rollback-geometry-ab-20261023-200636.stderr.log` (PID 35600, subsequently
closed). All 24 initial-state, replayed endpoint including renderer, and
original-endpoint recovery comparisons passed. All six combat trials passed.

| Trial | Frames | Rebuild execution ms | Reuse execution ms |
|---|---:|---|---|
| 1 | 4 | 45.242, 39.640 | 42.058, 45.153 |
| 2 | 4 | 39.606, 39.959 | 44.179, 41.388 |
| 3 | 4 | 36.567, 36.630 | 43.542, 41.359 |
| 4 | 8 | 73.958, 78.004 | 81.671, 83.281 |
| 5 | 8 | 75.436, 73.197 | 84.066, 77.892 |
| 6 | 8 | 76.903, 78.576 | 76.205, 74.752 |

Six samples per path/length averaged 39.607 ms rebuild versus 42.946 ms reuse
for four frames (8.4% slower), and 76.012 versus 79.644 ms for eight frames
(4.8% slower). Texture reuse and bulk packet handling were enabled in both
paths; inputs and snapshots were identical within each trial. These are
execution-only timings, excluding snapshot/verification/restore overhead.
Reuse was active: 1,752–2,288 resident draws and 117,825–183,790 reused vertices
per four-frame sample, 3,262–3,338 draws and 267,428–275,325 vertices per
eight-frame sample. There were 592–685 and 1,175–1,215 misses respectively.
Each pair of reuse repetitions had identical counters. Diagnostic pauses were
1,859–2,313 ms. This experiment therefore demonstrated correctness but no
performance benefit; total cache costs outweighed the work avoided in this run.
No individual overhead source has been timed or isolated.

Geometry reuse is now DISABLED BY DEFAULT (`force_geometry_rebuild=1`). The
bounded A/B harness can still opt in for future experiments, but ordinary
gameplay and replay use the faster existing pack/upload path, without cache
lookup or source-copy overhead. The earlier validated texture and packet
optimizations remain enabled. Next work should investigate reducing repeated
draw/state setup or rendering with explicit dependency tracking, rather than
claiming this cache as an optimization.
The default-disabled revision passes the rendering and both bounded geometry
A/B control tests (`build/rollback-geometry-default-tests.log`) and the game
has been rebuilt (`build/rollback-geometry-disabled-game-build.log`). It has
not been relaunched or put through another live combat run after this switch.

### Host drawing disabled: bounded replay A/B diagnostic (2026-10-23)

`DOA3_ROLLBACK_RENDER_AB_TEST=1`, with the existing tasks/workers/stream-workers,
window/replay/combat/long-test options, compares full rendering against omitted
host draws on the SAME captured four/eight-frame windows. Four repetitions
alternate ABBA/BAAB across six attack-gated trials. Other A/B modes conflict.
Geometry reuse remains disabled; exact texture reuse and bulk packets remain on.

This first experiment suppresses the six D3D8/NV2A draw entry paths and their
transient geometry uploads, scoped only to `doa3_native_frame_run`. It retains
guest instructions, command parsing, vertex transformation/packing in PGRAPH,
texture/target routing, clears, framebuffer/history copies, shader preparation
that consumes logical dirty state, and all emulated completion notifications.
It is NOT yet a simulation-only engine or elimination of all rendering work.
The prior draw mode is restored on success and failed execution, before endpoint
comparison/recovery. No per-draw timer or log is added; a total skipped-draw
counter is printed outside `execution_ms`.

Every endpoint still receives the ordinary exact comparison. A SEPARATE
diagnostic entry can return 2 for differing GPU pixels ONLY when every byte
domain and the PGRAPH/pipeline/target logical state match. Unsupported logical
comparison, failed capture, RAM/audio/input/worker changes, or other mismatches
stop the sequence. This is explicitly reported as state-only diagnostic success,
never full rendered equivalence. The original endpoint is always restored and
must match INCLUDING GPU pixels before resuming ordinary play. No skipped-draw
endpoint is committed to the live game, and no production replay check is relaxed.

All 35 targeted checks pass (`build/rollback-render-ab-tests.log`), including
production PGRAPH/D3D WARP draws with unchanged upload-ring offsets and changed
pixels, matching logical renderer state, a logical-state negative control,
native gate cleanup on failure, ABBA/BAAB scheduling, mismatch termination, and
full endpoint recovery. Release builds successfully
(`build/rollback-render-ab-game-build.log`).

Live combat results, `build/rollback-render-ab-20261023-202421.stderr.log`
(PID 39716, no longer running when results were read): all six windows and
24 repetitions completed. All 24 start restores and original-endpoint recoveries
matched exactly, including GPU images. The 12 full-render replays matched their
entire endpoints. All 12 draws-off replays matched every CPU byte domain and
logical renderer state; only GPU image contents differed, as expected. This is
not rendered equivalence or evidence that omitted framebuffer history is safe
to commit during netplay.

Execution-only timings (ms; two samples per mode on each identical window):

| Trial | Frames | Full rendering | Host draws off |
| --- | --- | --- | --- |
| 1 | 4 | 36.538, 38.120 | 34.651, 37.109 |
| 2 | 4 | 39.196, 35.590 | 41.714, 34.591 |
| 3 | 4 | 37.474, 36.410 | 34.946, 35.250 |
| 4 | 8 | 78.632, 83.409 | 73.751, 71.410 |
| 5 | 8 | 81.903, 69.668 | 74.886, 74.894 |
| 6 | 8 | 77.279, 77.533 | 84.074, 72.757 |

Four-frame means: 37.221 ms full versus 36.377 ms draws-off, saving 0.844 ms
(2.3%). Eight-frame means: 78.071 versus 75.295 ms, saving 2.775 ms (3.6%).
There are six samples per mode per window length, but only three independent
captured windows per length. The draws-off pair was slower in trials 2 and 6;
the small averages are not a stable performance guarantee. The gate omitted
2,419–2,615 draws per four-frame replay and 4,918–6,361 per eight-frame replay.
Diagnostic pauses were 1,829–2,015 ms; capture/restore/comparison are outside
the execution timer and these timings do not represent total rollback cost.

Conclusion: suppressing host draws/uploads alone does not provide the required
large reduction. Most measured execution time remains in retained work; this
test does not attribute it among guest simulation, guest rendering, PGRAPH
processing, state/texture preparation, or clears/copies. The next larger
experiment should isolate render preparation higher in the pipeline while
preserving gameplay dependencies. Host drawing remains enabled by default;
the opt-in gate is diagnostic-only and no default change was made after the run.

### Intermediate-frame renderer preparation A/B (2026-10-23)

`DOA3_ROLLBACK_PREP_AB_TEST=1` is a separate mutually exclusive A/B mode,
using the existing six four/eight-frame combat windows and four ABBA/BAAB
repetitions per window. It compares full replay against skipping PGRAPH END
preparation for the first N-1 replay frames, then rendering the final frame
normally. This is a larger cut than the earlier D3D draw/upload gate: it omits
vertex fetch/vertex-program execution, geometry clipping/packing, texture
binding and material/pipeline setup as well as host drawing on hidden frames.
It still executes every guest task, consumes every graphics command/payload,
routes render targets, processes clears and existing frame-copy paths, advances
the original fixed-step clocks, and retains GPU frontend completion semantics.
This does NOT yet skip guest-side draw-command generation or its CPU skinning.

The PGRAPH gate is scoped per replay frame and restored on all returning exits.
The final frame revalidates exact texture source bytes because hidden frames
can modify texture RAM without binding it. One-frame replay never skips a frame.
The D3D draw-only gate and preparation gate cannot be selected together. Host
batch/index/inline-payload counters are logged outside the execution timer;
there are no new per-draw timers or trace records.

No saved renderer state is injected to fabricate an equal replay endpoint.
The normal final frame must independently reproduce all CPU byte domains and
the COMPLETE existing logical PGRAPH/pipeline/target checkpoint. The same
diagnostic-only pixel-difference result from the draw-only test remains
available for framebuffer-history differences. Any CPU/logical mismatch stops
the sequence; every trial still restores and verifies the original endpoint,
including all GPU images, before the live game resumes. Skipped-frame images
are never committed to ordinary play, and this mode remains off by default.

All 37 targeted tests pass (`build/rollback-prep-ab-tests.log`). Coverage includes
one/two/four/eight-frame scheduling, early/final-frame failure cleanup, mode
conflict rejection in the driver, final-frame texture invalidation, ABBA/BAAB
control and stop-on-mismatch. The production PGRAPH/D3D WARP test skips two
frames with both inline and indexed geometry, checks that geometry upload
rings do not advance, changes texture RAM during the hidden interval, then
renders the final frame and requires exact logical state AND GPU pixels against
full rendering for that synthetic scene. This is not proof that history-based
effects in real stages match. Release built successfully
(`build/rollback-prep-ab-game-build.log`) and launched as PID 32612 with log
`build/rollback-prep-ab-20261023-203754.stderr.log`.

The completed live run passed all six windows and 24 repetitions. All 24
earlier-state checks, replay endpoints and original-endpoint recoveries matched
exactly INCLUDING GPU images. All 12 optimized samples completed the final
render normally and matched full logical renderer state. The diagnostic-only
pixel-difference exception was not needed in this run; no fatal report occurred.
The game remained running/responding when results were read.

Execution-only timings (ms; paired samples use identical saved state/input):

| Trial | Frames | Full rendering | Skip intermediate preparation |
| --- | --- | --- | --- |
| 1 | 4 | 38.705, 39.352 | 31.518, 30.763 |
| 2 | 4 | 38.692, 35.413 | 29.674, 29.038 |
| 3 | 4 | 33.831, 32.986 | 28.235, 25.575 |
| 4 | 8 | 92.263, 86.661 | 62.784, 67.714 |
| 5 | 8 | 89.180, 83.759 | 66.838, 68.344 |
| 6 | 8 | 104.089, 96.120 | 73.777, 73.444 |

Four-frame means: 36.497 ms full versus 29.134 ms optimized, saving 7.363 ms
(20.2%). Eight-frame means: 92.012 versus 68.817 ms, saving 23.195 ms (25.2%).
All six optimized window means improved. These are six samples per mode per
length from three independent combat windows each, not a broad performance
guarantee. Compare against the SAME run's baseline: the eight-frame baseline
and submitted geometry workload differ from the preceding draw-only run.

The gate skipped 1,853–2,402 nonempty batches and 223,137–329,535 indices per
four-frame replay; 6,424–8,495 batches and 594,356–977,013 indices per eight-frame
replay. Counter pairs matched for repeated inputs. Diagnostic pauses were
1,703–2,078 ms, including four replay repetitions and snapshot verification;
snapshot capture/restoration/comparison remain outside execution_ms.

This is a materially larger execution improvement, but 68.817 ms for eight
frames is still over a 16.67 ms display-frame budget BEFORE checkpoint cost.
It does not establish equivalence for other stages, effects, camera changes or
framebuffer-history-dependent scenes. Keep this opt-in pending that coverage.
Next larger work should investigate guest-side render-command construction and
render-only model processing, which still execute for every replayed frame;
their individual cost and safe separation from collision/animation must be
measured rather than inferred from the remaining total.

### Remaining whole-fight integration

The existing `netplay_session.c` is explicitly gated to `DOA3_XBE_ID_3_0`.
Its `k_sync` ranges are 3.0 start-sync/digest regions, **not** a full restorable
3.1 snapshot. `netplay_poll` and `netplay_input_tick` are currently called only
by the reference 3.0 manual override. Do not remove the version gate or copy
those addresses to enable 3.1 online play.

Evidence from the current generated 3.1 code:

- `sub_000B9980` updates per-side attack counters (including `0x9B28A0` and
  `0x9B272C`) and is called at several points in `sub_000B4720`. The performance
  and frame-data hooks there are observers, not a replayable complete frame.
- The existing training-input hooks at `sub_00011980` and `sub_00012B60` bound
  part of command processing. They are not proven to contain animation,
  collision, stage state, RNG, match progression and every other frame update.
- `xbox_fiber.c` uses Windows fibers with live native C stacks, saved guest
  registers/x87 state, wait states and scheduler metadata. Copying guest RAM
  does not rewind those native continuations. Identify a complete step boundary
  that avoids rewinding continuations, or redesign the affected continuations.
- The deterministic clock checkpoint deliberately does not claim to restore
  that scheduler, the APU, heap allocation state, kernel objects or GPU resources.

Next implementation milestones:

1. Identify and instrument the complete 3.1 fight-frame boundary and its side
   effects. Start with two-player single fights after assets have loaded; keep
   loading, movies, selection, transitions and tag play out of the initial scope.
2. Implement the complete game snapshot and restore callback, including RNG,
   input history, animation, collision, stage hazards, counters, mutable heaps,
   necessary guest register/stack state and host-owned simulation state.
   Document which state is excluded and why it cannot affect future simulation.
3. Run an opt-in local rewind/replay check on the **actual game**. Validate
   restored-state hashes before advance and identical replay hashes afterward,
   including attacks, throws, throws escaped, counters, wall hits, stage breaks,
   round ends, pauses and input transitions. Detect unexpected IO/worker effects.
4. Implement safe presentation/effect reconciliation, measure replay cost, then
   connect the validated adapter to lobby transport and the Online menu.
5. Test two real clients with latency/jitter/loss and fixed 60 Hz game timing
   before exposing this as playable rollback. The fixture results above are not
   a substitute for those checks.

### Intermediate guest model-render skip experiment (2026-10-23)

`DOA3_ROLLBACK_GUEST_RENDER_AB_TEST=1` adds a bounded local experiment on top
of the intermediate PGRAPH preparation skip. It remains disabled in ordinary
play and cannot be combined with another A/B experiment. Both A/B arms skip
host preparation on intermediate frames; the new arm additionally skips:

- `17D900`: model traversal, material setup, draw-command construction and
  deferred-model submissions.
- `17DF00`: the alpha model traversal/material/draw-command path.
- `52046`: deformation/copying of rendered model vertex positions and normals.

These are candidate rendering-only boundaries, not proven disposable state.
Their writes include renderer scratch, guest vertex buffers, draw counters
and queue cursors. No such memory is excluded from verification. Skipping
these routines can therefore fail the exact comparison even if the visible
final frame looks correct. A mismatch stops the sequence after restoring and
verifying the original endpoint; it is not accepted as a successful replay.

The animation/skeleton updates (including `4CD00`), combat and collision tasks,
camera updates, scene callbacks, render-queue cleanup and frame completion
still execute. The broader display tasks are deliberately retained because
they mix simulation and rendering. `17F400` is not skipped: it has an indirect
callback whose effects need separate review. The final replay frame executes
all original model helpers and host rendering, with exact texture-source
validation re-enabled before that frame. No reference-state data is injected
into the replay result.

The three wrappers preserve guest registers and consume their original return
and argument slots (4, 8 and 4 bytes respectively). The gate is set only inside
the native replay driver, only before the final frame, and cleared on every
returning exit. It cannot skip arbitrary helper addresses. Aggregate helper
counts are logged after the execution timer; no per-call timers were added.
Generation pins all three reviewed guest bodies, wrapper ABIs, live overrides
and absence of competing stack-probe wrappers. The source digest includes the
new gates and wrappers. Regeneration uses the ordinary override/probe pipeline.

Validation before the first live run: Release executable built; 39 targeted
checkpoint, replay, A/B sequencing and actual PGRAPH rendering tests passed;
four source-guard tests passed. Driver tests exercise 1/2/4/8-frame batches,
first/final-frame failures, default pass-through, exact guest stack cleanup,
retained registers, final-frame pass-through and post-replay gate cleanup.
Sequence tests cover alternating A/B order and stopping on mismatch. These
component tests do not establish game-state equivalence or a performance gain.
Live run `build/rollback-guest-render-ab-20261023-205348.stderr.log` stopped
safely on trial 1 (four frames). The host-preparation-only arm took 57.964 ms
and matched the complete endpoint, including GPU pixels. The guest-skip arm
took 24.280 ms, bypassing 766 model draws, 144 alpha-model draws and 84 vertex
deformations. This is a 58.1% reduction for ONE same-input sample, not a
validated speedup: the candidate failed endpoint equivalence. No further
repetitions or eight-frame trials ran.

The mismatch mask was `082`: RAM/CPU and GPU frontend, plus a renderer
mismatch with `logical_renderer_equal=0`. RAM first differed at checkpoint
offset `1EB459`; subtracting the 184-byte FightCpu header locates guest byte
`1EB3A1`, inside the command write cursor at `1EB3A0`. The original `1DDEB0`
advances that cursor and writes command/value pairs. Frontend byte `9` is
inside USER channel 0 PUT (`Nv2aQueueSnapshot` header is eight bytes). These
first differences are consistent with constructing fewer commands, but do not
identify every mismatch or prove that gameplay state is unaffected. Renderer
pixel equivalence was not established independently of its logical mismatch.

Both start restores and both original-endpoint recoveries matched exactly.
The game resumed from the independently restored original endpoint; the
candidate state was not kept. The experiment remains opt-in and stopped for
this process. Next work must separate disposable command-buffer history from
state required by later frames, identify the remaining renderer/RAM changes,
and verify future execution before this can become a usable optimization.
Do not copy expected endpoint bytes into a candidate or broadly ignore RAM
or renderer differences to turn this rejection into a pass.

### State-preserving revision of the guest-render experiment

The next revision retains the original `17D900` and `17DF00` bodies on EVERY
frame. That keeps material state, deferred model submissions, draw counters,
command payloads, ring allocation/advancement and GPU frontend consumption in
their original order. It does not copy any renderer state from the reference
endpoint or attempt to correct just the first differing cursor. The native
skip allowlist now rejects both model-command helpers; source guards require
their wrappers to delegate unconditionally.

The additional skip is restricted to `52046` vertex deformation on hidden
frames. The already validated PGRAPH preparation skip remains in both A/B
arms; the final frame regenerates the vertices and renders normally. This
trades some of the rejected whole-model skip for preserving its stateful
work. How much of the measured saving remains is unknown until a matching
live replay is measured. The previous 58.1% sample does not apply to this
revision, and the deformation skip remains experimental.

Release build succeeded and all 40 targeted CTest entries passed, including
the source-guard suite. Driver tests now explicitly require both command
builders to execute on hidden frames while only deformation is bypassed;
1/2/4/8-frame final-frame rendering and error cleanup coverage remains.
Full RAM/CPU, frontend and renderer comparisons and exact endpoint recovery
are unchanged.

Live run `build/rollback-preserve-state-20261023-210018.stderr.log` completed
one four-frame A/B pair, then stopped on mismatch. The baseline took 29.645 ms
and matched exactly. Preserving model commands while skipping 84 deformation
calls took 28.758 ms (0.887 ms / 3.0% less in this single pair). Both arms
processed the same 2,330 skipped host-preparation batches and 314,373 indices.
The optimized arm now matched the COMPLETE renderer sidecar, including GPU
pixels and logical state (`renderer=0`, `logical_renderer_equal=1`). It still
failed RAM/CPU and GPU frontend comparison (`mask=082`). The first RAM byte
was checkpoint offset `1EB458`, guest address `1EB3A0` after the FightCpu
header: the command write cursor. Frontend byte `8` is channel-zero PUT.
Both original-endpoint recoveries matched; no eight-frame checks ran. This
is not a validated optimization and does not retain the earlier 58.1% saving.

Source inspection identifies a concrete remaining command side effect:
`52046` calls buffer-lock helper `1DDDC0` before its deformation loops. Unless
lock flag bit `0x10` is set, `1DDDC0` emits the pair `0x41710, 0`, advances the
guest command cursor by eight bytes, and may reserve/flush the ring. It also
retains conditional resource-wait and resource-address calls (`1E2CD0`,
`1E0740`). Skipping the whole deformation helper removed these operations.
The next narrower candidate must retain the original lock calls and their
ordering/flags while bypassing only the vertex arithmetic/copy loops. This
explains a cursor difference, not every potentially differing byte; exact
comparison must remain the acceptance criterion.
