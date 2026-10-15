# DOA3 optimization implementation

Delivered as **v0.0.143**, rebased onto upstream **v0.0.142** (`e2096fa42944b49fa414b76ac5a5de1f35278ce7`) on 15 October 2026. The inspection and FPS measurements below were made against **v0.0.136**, `b12e78e49c66829da38f27e4f04d6ac648b91d88`, plus the optimization changes; they are not measurements of the subsequent upstream changes. Native simulation/pacing and online synchronization code were preserved.

## Changes delivered

- **Texture bindings:** skip redundant D3D11 pixel-shader resource binds, including repeated null binds. Preserve texture-stage activation semantics. Render-target changes invalidate the cache because D3D11 can automatically unbind conflicting resources; movies and external renderers invalidate through the existing binding epoch. Offscreen and frame-history binding paths share the cache.
- **Timing overhead:** replace per-call locked operations and clock polling with one shared enable flag. Refresh it when the Debug setting changes. Separate raw end timestamps from the enable check, so disabling capture during an operation cannot create a negative duration. Shader timing also follows live changes.
- **Frame capture:** move CSV formatting, writes and periodic flushes to a worker. A fixed 4,096-row queue drops diagnostic samples when full or contended, with a dropped-row count at shutdown. Capture can start, stop and restart from the Debug menu; restart appends and resets input/frame timing history. Each enabled session retains upstream v0.0.138's 432,000-frame limit (two hours at 60 FPS). Turning capture off or exiting drains the writer.
- **Audio capture:** replace sample-by-sample ring writes with at most two bulk copies. Snapshot chronological PCM under a short lock and write it on a worker; disk I/O no longer owns the mixer lock. One outstanding dump bounds memory, and disabling diagnostics prevents new dumps. Exit drains outstanding capture work. Audio playback buffers and scheduling are unchanged.
- **Resource preparation:** reuse the SHA-256 algorithm provider while keeping per-digest state independent. Return immediately when a free texture-cache slot fits the existing budget, preserving pressure/reuse/LRU behavior. Gate the combiner census behind diagnostics.
- **Regression coverage:** repair point-sprite and texgen test integration. Exercise the real NV2A streaming uploader, including append/wrap and external binding invalidation, instead of a test-only immutable-buffer substitute. Add capture lifecycle, audio snapshot/concurrency, SRV binding and concurrent SHA-256 coverage.

## Measurements

### Actual presentation FPS: no demonstrated gain

Measured on 15 October 2026 using separately built baseline and optimized **v0.0.136 Release** executables on the i5-12600K / RTX 3080. Both use **32:9, 5121x1440 internal resolution**, windowed output, native 60 Hz pacing, probes off, logging off and automatic capture off. Windows power plan is High performance. The original settings and saves were preserved in separate benchmark copies.

| Final matched comparison | Baseline | Optimized | Change |
| --- | ---: | ---: | ---: |
| Average presentation FPS | 59.9953 | 59.9967 | +0.0022%; effectively unchanged |
| 1% low FPS | 50.4694 | 50.2618 | -0.41% |
| P99 frame interval | 19.2865 ms | 19.2647 ms | -0.0218 ms |
| Frames above 20 ms | 6 / 2,160 | 8 / 2,160 | Two more |
| Frames above 25 ms | 0 / 2,160 | 0 / 2,160 | Unchanged |

The final comparison uses **six matched automatic 3D demo scene segments**, 360 frames per scene per executable (2,160 frames, approximately 36 seconds per build). Match mode, active flag, fighter IDs, stage ID, stage slot and demo phase. Discard incomplete segments at capture endpoints, require at least 480 frames per eligible segment, skip its first 60 frames and measure frames 60 through 419. This fixed rule gives every scene equal weight and removes scene-boundary attribution uncertainty; it was selected before the final capture and does not select scenes by performance. Automatic demos are not deterministic gameplay input replays.

The earlier optimized capture shares four eligible scenes with the same baseline recordings: **59.9945 -> 59.9970 average FPS**, **50.4444 -> 50.4246 FPS 1% low**, and three frames above 20 ms in each build. The optimized run was repeated; baseline scene windows are reused for the comparison, not independent repeated baseline trials. Individual scenes improved or worsened, and one early 8.6% improvement in a scene's 1% low shrank to 2.2% on repetition. There is no evidence here for a consistent slow-frame improvement or an average-FPS gain. Small differences have not been established as statistically significant regressions either.

Four recordings contain **48,547 raw presentation intervals** over 810 seconds; most are intentionally excluded because they are startup movies, menus, unmatched or incomplete scenes, or outside the fixed comparison windows. Do not compare unfiltered whole-capture averages: their scene composition differs. A 1% low is 1,000 divided by the mean of the slowest ceil(1% of frame count) intervals, not the reciprocal of P99.

Timing comes from the game's existing QPC history immediately before host presentation, read externally at 20 Hz. The existing performance overlay is enabled equally in both builds; no automatic screenshots, frame CSV writer or build runs during recording. The observer writes its data after collection and checks the running PE timestamp against its own executable/map. Presentation-counter rates independently agree with the recorded history. Both later captures recorded foreground state and all their samples remained foreground. The first two captures did not record that field. Existing shader disk caches were retained, other user applications remained running, and this was not an isolated machine or cold-cache experiment. These numbers include frame pacing and overlay cost; they do not measure GPU time or uncapped rendering headroom.

**Conclusion:** the component savings below are real measured CPU savings, but this test does not demonstrate a player-visible FPS improvement. Both builds already sustain the native 60 FPS cap in the selected scenes. The full gameplay/stage/online matrix and CPU/GPU profiling remain necessary for further optimization decisions.

Reproduction artifacts: [environment and executable hashes](build/optimization-audit/fps-ab/environment.json), [native history collector](build/optimization-audit/fps-ab/measure.py), [scene selection and statistics](build/optimization-audit/fps-ab/analyze.py), [final comparison with exact sample ranges](build/optimization-audit/fps-ab/baseline-demo-1,baseline-demo-2-vs-optimized-demo-2.json), [earlier comparison](build/optimization-audit/fps-ab/baseline-demo-1,baseline-demo-2-vs-optimized-demo-1.json). Raw CSVs: [baseline 1](build/optimization-audit/fps-ab/baseline-demo-1.csv), [baseline 2](build/optimization-audit/fps-ab/baseline-demo-2.csv), [optimized 1](build/optimization-audit/fps-ab/optimized-demo-1.csv), [optimized 2](build/optimization-audit/fps-ab/optimized-demo-2.csv). These are local ignored build artifacts. Recalculate using `python doa3/build/optimization-audit/fps-ab/analyze.py baseline-demo-1,baseline-demo-2 optimized-demo-2` from the repository root. The benchmark processes were closed after recording.

### Component CPU costs

Release component benchmarks on **Intel Core i5-12600K / NVIDIA GeForce RTX 3080**, driver **32.0.16.1742**. Each figure is the median of seven runs; baseline and optimized order alternate. The SRV benchmark explicitly reports the RTX 3080 adapter and includes two warmup pairs. These measure the targeted CPU operations, **not gameplay FPS or GPU frame time**.

| Workload | Baseline | Optimized | Result |
| --- | ---: | ---: | --- |
| 2,000,000 disabled timing checks | 11.102 ms | 1.021 ms | 10.87x faster |
| 100,000 stereo capture batches, 1,024 frames each | 284.349 ms | 10.213 ms | 27.84x faster |
| 200,000 unchanged SRV bindings | 1.860 ms | 0.223 ms | 8.34x faster |
| 200,000 alternating texture/null bindings | 5.228 ms | 5.255 ms | 0.5% higher median cost |

The SRV result depends on reuse: cache hits save work, while changing resources still requires a driver call and pays a small cache-check cost. Diagnostic ring gains apply when automatic capture is enabled. The normal-run improvements are reduced timing-check overhead, redundant binding elimination, disabled census work and resource-preparation changes. Shader-provider and free-slot changes have correctness coverage but no separately measured gameplay benefit.

Raw results: [timing](build/optimization-audit/perf_capture-benchmark.log), [audio capture](build/optimization-audit/audio_capture-benchmark.log), [hardware SRV bindings](build/optimization-audit/srv_bindings-benchmark.log). Benchmark sources are in `tests/perf_capture`, `tests/audio_capture` and `tests/srv_bindings`; build their benchmark targets in Release. Run `srv_bindings_test --benchmark` for the hardware binding comparison.

## Validation and remaining limits

**The final v0.0.143 Release build passed after integrating upstream v0.0.142. All 31 standalone CMake suites rebuilt and all 40 C/C++ tests passed.** The executable's file and product versions were verified as 0.0.143. Tests were built in Debug so assertions remain enabled. `git diff --check` passed. Detailed results: [initial implementation summary](build/optimization-audit/implementation-summary.json), [final build log](build/optimization-audit/game-release.log), [final checks](build/optimization-audit/checks.json).

The new tests verify capture off/on/off/on, append without duplicate headers, timestamp reset, file-open failure recovery, queue drain and accepted-row ordering; audio wraparound, oversized batches, chronological snapshot bytes and mixer-lock availability during a stalled disk write; all four texture stages, repeated bindings, external changes and render-target hazards; concurrent first-use SHA-256; and production streamed rendering through buffer wraps. Existing frame-history, texture-budget/reuse, shader-cache corruption, particle, texgen, audio and presentation tests also pass.

The subsequent FPS pass launched separate baseline and optimized Release builds and read actual native presentation intervals during the automatic 3D demo. Desktop screenshot capture timed out, but the game's own F12 screenshots allowed limited visual inspection outside timed recording. This does not establish full audiovisual correctness or stage/online coverage. The stage/online matrix below remains necessary before claiming improved whole-game frame-time tails. No claim is made that every remaining bottleneck has been eliminated. Further dispatch, combiner decoding, history-buffer and scheduling changes should be selected from current gameplay profiles rather than inferred from the component timings.

---

# Original inspection, before implementation

Inspected 15 October 2026 against **v0.0.136**, commit `b12e78e49c66829da38f27e4f04d6ac648b91d88`. The checkout was fast-forwarded nine commits from v0.0.128 after fetching `origin/doa3.1`. Existing local files were preserved. The Release executable was rebuilt and its Windows file version verified as `0.0.136`; `DOA3_PROBES=OFF` and `/O2 /Ob2` are active.

The strongest next optimization candidates are CPU work repeated for each draw, guest indirect dispatch, and first-use scene preparation. Recent upstream changes already remove many expensive D3D11 calls. Further gains need measurements of the current build before choosing between those remaining costs. Two broken regression suites and inconsistent profiling controls should be addressed first so those measurements and changes can be trusted.

This covers the game host, generated dispatch, NV2A translation, D3D11 resources and presentation, textures and shaders, audio, fiber scheduling, input, file I/O, online synchronization, debug UI, and build/test setup. Fresh evidence consists of the Release build, component tests, and a hardware-device draw-state microbenchmark. The attempted live game launch stopped at a computer-use approval timeout. Gameplay numbers below are explicitly historical; current full-game FPS, stage coverage, online behavior, and audiovisual correctness were not revalidated live.

**Validation**

| Check | Result |
| --- | --- |
| Current Release build | Passed; executable version 0.0.136 |
| Standalone CMake test suites | 26 of 28 built; all 33 tests in those suites passed |
| Python debug code-generation tests | 2 passed |
| Point-sprite suite | Link failure: missing `g_d3d8_bind_epoch` |
| Texture-generation suite | Compile failure: undeclared `g_d3d8_bind_epoch` in extracted implementation |
| Hardware draw-state benchmark | Completed; CPU preparation only |
| Live gameplay profiling | Not completed because launch approval timed out |

Passing suites cover announcer policy, APU idle and IRQ behavior, audio mixing/settings, clipping, combat diagnostics, combiner cache, debug settings/views, screenshots, direct AFS, frame history, camera, HUD/timer, controller mapping/discovery/rumble, NV2A draws, polygon offset, presentation, reflection masks, save compatibility, shader cache, texture cache/upload, training, vertex-program operands, and video/HUD layout.

The initial build failures involving duplicate `Path`/`PATH` environment entries were resolved by giving MSBuild a canonical `Path` entry and disabling node reuse. They are distinct from the two remaining source/test integration failures.

Detailed results: [summary.json](build/optimization-audit/summary.json), [build log](build/optimization-audit/game-release-canonical-env.log), [benchmark log](build/optimization-audit/benchmark.log). These local build artifacts are ignored by Git.

**What the latest version already fixes**

The current implementation includes persistent validated shader bytecode, a 4,096-entry texture cache within a 192 MiB GPU-plus-staging budget, sampled source verification once per texture per frame, batched initial mip uploads, allocation reuse, and reduced CPU vertex-program operand fetching. It also caches blend/depth/rasterizer objects and bindings, skips unchanged shader constant uploads, combines NV2A vertices and indices into one ring upload, disables guest probes by default, removes periodic automatic traces, and catches up CRI workers for elapsed vblanks. Controller discovery is already throttled/event driven.

Those are existing improvements, not new recommendations. In particular, adding another generic shader cache or repeating the two-ring-to-one-ring change would duplicate completed work.

**Prioritized findings**

| Order | Work | Evidence and expected scope | Risk |
| --- | --- | --- | --- |
| 1 | Repair profiling controls and regression coverage | Confirmed defects; enables reliable further optimization | Low to moderate |
| 2 | Reduce repeated texture bindings and state construction | Confirmed repeated work in current draw path; current frame-time saving unmeasured | Moderate |
| 3 | Profile and accelerate guest dispatch/hot functions | Binary searches and tracing remain; likely relevant to CPU-heavy views | Moderate to high |
| 4 | Spread first-use resource work across loading | Historical scene-entry spikes; current residual requires reproduction | Moderate |
| 5 | Remove diagnostic I/O from real-time paths | Confirmed synchronous flushes and audio dump lock | Low to moderate |
| 6 | Measure memory bandwidth, cache pressure and presentation separately | Plausible costs, not established current bottlenecks | Moderate |

**1. Profiling controls and test coverage**

`doa3_perf_frame()` in [main.c](src/game/main.c), line 322, reads `DOA3_PERF_CAPTURE` only when `s_perf_enabled < 0`. If diagnostics are off at the first frame, enabling them later never starts the CSV recorder or its input/logic timestamps. If enabled at startup, turning them off does not stop that recorder; it runs until the 72,000-frame limit. By contrast, [perf_capture.h](src/perf_capture.h), line 9, polls the environment about once a second. The menu therefore controls phase timings and CSV capture differently.

Use a shared capture lifecycle, opening/closing a session on transitions and resetting all timestamps. Separate the enable check from the raw timestamp function: some callers use `doa3_perf_begin()` as an end timestamp, so an off transition can return zero and corrupt a duration. The current helper also executes an interlocked operation and `GetTickCount64()` while disabled, including on the per-draw path. Snapshot the switch at a safe frame boundary instead of checking wall time for each draw.

The CSV `present_ms` includes the host Present wrapper and composition work; it is not GPU duration. No D3D11 GPU timestamp/disjoint query instrumentation was found. Add delayed, nonblocking GPU timestamps and distinct CPU timers for guest logic, pushbuffer translation, resource preparation, composition, DXGI Present, and pacing. Do not sum `pb-subset` into `before-flush`, or worker time into pacing: these are nested measurements.

Fresh failures are recorded in [point sprites](build/optimization-audit/point_sprites-retry-build.log) and [texgen](build/optimization-audit/texgen-retry-build.log). Both standalone harnesses need the bind-epoch dependency introduced upstream. Also, [nv2aff_draw_test.c](tests/nv2aff_draw/nv2aff_draw_test.c) substitutes an immutable-buffer uploader for the real ring. Its passing pixel check validates nonzero base-vertex/index addressing, but does not exercise the new dynamic ring's append, wrap, DISCARD, or reuse behavior. Add that coverage before extending batching or ring reuse. The root CMake project does not aggregate these standalone suites, so an ordinary game build cannot reveal these test failures.

**2. Repeated draw preparation remains**

[d3d8_device.c](src/d3d/d3d8_device.c), line 1306, still issues `PSSetShaderResources` on every `SetTexture`, even if the same shader-resource view is already bound. The GPU path in [nv2a_pgraph_d3d11.c](src/nv2a/nv2a_pgraph_d3d11.c), around lines 3280–3315, binds secondary stages and clears them again after each draw. That defeats simple consecutive-draw reuse.

Track the actual resource view bound to each stage and bind only changes. First count redundant calls and bindings caused by cleanup. Preserve explicit unbinding for render-target hazards, framebuffer/history sampling, movies, overlays, and resource destruction; a texture-pointer comparison alone is insufficient.

The same GPU path reconstructs vertex constants and calls `nv_build_combiner_state()` each draw. The newer caches avoid many uploads, but still pay for decoding registers, constructing structs, comparing them, converting colors, and deriving specular power. Introduce separate revision counters for transforms, lighting, texgen, combiner structure, and constants, then rebuild only affected data. Guest writes that repeat a value need not advance a revision. Keep guest per-draw markers and texture availability in the dependency model.

Residual diagnostic work also remains unconditional: the 64-entry distinct-combiner scan at line 3265, method histogram writes at line 4338, and several draw counters. Gate diagnostic-only census work while retaining state needed for rendering and essential fault reporting.

The fresh state-preparation benchmark measured **0.959 ms per 10,000 unchanged-state preparations** and **0.885 ms per 10,000 alternating-state preparations**. Vertex-color and wireframe cases measured 0.943 and 1.105 ms respectively. These are CPU microbenchmarks with a hardware D3D11 device, not FPS or GPU measurements. State-object preparation alone is therefore a smaller target than the entire draw translation path on this machine.

**3. Guest CPU execution and dispatch**

The generated [dispatch table](src/game/recomp/gen/recomp_dispatch.c), line 11057, binary-searches 11,032 entries for each resolved indirect lookup. [recomp_types.h](src/game/recomp/recomp_types.h), around lines 503–594, also writes the indirect-call flight recorder regardless of `DOA3_PROBES`. Turning probes off removes wrapper checks, not all dispatch overhead. The manual lookup table currently contains only a sentinel; its apparent linear search is not a large-table bottleneck.

Collect sampling profiles with probes off, resolving stacks using this build's symbols/map. If dispatch is hot, benchmark a small VA-to-function cache or direct page-indexed table while preserving manual/generated/kernel precedence and failure behavior. If lifted math, animation or scene traversal dominates, target those measured functions instead. Preserve scheduling points: removing wrappers or changing guest execution order can starve CRI workers or alter online determinism. Do not enable global fast-math as a blanket optimization.

Whole-program optimization or profile-guided optimization is an experiment after sampling, not an assumed improvement. Current build options already enable ordinary Release optimization. Record executable size, compile cost, replay digests and frame tails for any compiler experiment.

**4. First-use texture and shader work**

Shader disk hits still perform synchronous hashing, file reads and validation; misses compile and save in the requesting path. [d3d8_shader_cache.c](src/d3d/d3d8_shader_cache.c) also opens/closes a BCrypt algorithm provider for each digest. Scene prewarming and a reusable hash-provider handle are candidates, with prewarming likely the more consequential change. Count actual misses and per-operation times first.

Texture cache hits are hashed, but `texcache_make_room()` in [nv2a_pgraph_d3d11.c](src/nv2a/nv2a_pgraph_d3d11.c), line 728, scans all 4,096 slots even when a free slot exists and the byte budget is comfortable; multiple evictions repeat the scan. Maintain a free-slot list and an eviction structure if allocation bursts show this is material. Avoid merely enlarging the cache. Source identity must still detect recycled guest addresses, palette changes and failed uploads; further weakening freshness checks could restore stale character textures.

Historical local capture `hayabusa-setup-optimized.csv`, filtered to mode 2, foreground, no movie, contains 3,866 frames: mean 16.667 ms, p99 18.940 ms, worst 42.677 ms, four frames over 25 ms. Its development-build log identifies scene-entry frames around 31–38 ms and a separate 26.739 ms presentation interval. This supports separate investigations of resource preparation and presentation, but does not establish that those spikes persist in v0.0.136.

**5. Audio, scheduling and diagnostics**

The real-time mixer takes an exclusive `s_ring_lock` in `mix_ring_push()`. `apu_mix_dump()` in [apu_xaudio2.c](src/apu/apu_xaudio2.c), line 156, holds that same lock shared while synchronously writing up to 30 seconds of stereo PCM, about 5.76 MB. A slow dump can block the mixer producer. Copy or swap a bounded snapshot under a short lock, then write outside the lock on a worker. The dump function also lacks a current capture-enabled check, so previously collected audio can still be dumped after diagnostics are disabled.

The frame CSV calls `fflush()` every 300 frames on the game thread. Upstream notes already attribute periodic slow frames to that flush. Use bounded asynchronous recording or a memory ring exported after the measurement. Keep timing-only recording separate from screenshots, method traces and mix dumps.

Audio currently submits 1,024 samples at 48 kHz and permits six queued buffers: approximately 21.3 ms per buffer and 128 ms of queue capacity. This is capacity, not a measured end-to-end latency. The APU waits using `Sleep(1)` with the mixer lock released. An audio-buffer completion event can reduce polling and wakeup jitter; measure underruns and queue depth before reducing buffering. Preserve the newly added elapsed-vblank catch-up and deterministic online path.

**6. Presentation, memory, input and online play**

Presentation already uses flip-discard when available, a one-frame queue limit and native-rate pacing. [recomp_manual.c](src/game/recomp/recomp_manual.c), line 1009, rechecks workers roughly every millisecond and spins over the final approximately 0.5 ms. That final spin can consume about 3% of one core at 60 Hz. Longer waits could save power but must retain worker service and input latency. Pacing time is intentional; treating it as CPU rendering cost would misdirect optimization.

[d3d8_keep_prev_frame()](src/d3d/d3d8_device.c), line 1184, copies the guest target every frame. At the actual 32:9, 3x target of **5121×1440**, one RGBA8 surface is about 28.1 MiB; a 60 Hz copy moves about 1.77 GB/s of payload before counting both reads and writes. This is a bandwidth estimate, not a measured bottleneck. GPU timing is needed, especially on integrated graphics. History is required even while effects are inactive; do not simply skip it. A render-target rotation design must preserve Omega after-images, crossfades, HUD feedback and clean screenshots.

The 192 MiB texture budget excludes other allocations such as the 16 MiB NV2A ring, render/depth/history surfaces, offscreen targets, guest RAM and driver allocations. Record total committed memory, GPU memory and resource counts through repeated stage changes; a bounded texture cache alone does not establish bounded process memory. State-object caches clear all 64 entries when full, so instrument occupancy/reset counts before considering bounded LRU replacement.

Input discovery already avoids repeated empty-slot polling and PS4 time-based enumeration. Keep fresh gameplay input reads; there is no evidence here that reducing their frequency would help. The disabled performance overlay avoids its timer/sort work, and combat geometry capture exits when disabled, so these are lower priorities than per-draw work.

File reads remain synchronous host operations from cooperative guest execution. Add byte counts and timing at the file bridge before changing streaming; keep direct-AFS and save compatibility semantics. The active 3.1 movie path uploads guest-decoded frames. The separate 3.0 host MPEG decoder should not be mistaken for the current version's movie bottleneck.

Online play is lockstep: `pv_exchange()` in [netplay.c](src/online/netplay.c), line 1240, polls for remote input and sleeps 1 ms. Separate network wait from simulation/render cost in online metrics. Measure packet-arrival-to-consumption time before replacing polling with an event wait. Preserve deterministic clocks, worker budgets and checksums; don't remove synchronization or digest checks to improve an FPS number. A two-peer test is required for online conclusions.

**Next measurement pass**

Use this exact Release revision with probes and automatic captures off for the baseline. After repairing capture controls, collect timing-only runs separately and quantify instrumentation overhead. Record CPU/GPU models, driver, power mode, aspect ratio, internal resolution, focus, cache temperature and commit for every run.

| Scenario | Purpose |
| --- | --- |
| X Octagon centre and wall, 16:9 and 32:9, 1x and 3x | Separate wider-view draw/guest costs from pixel costs |
| Hayabusa Story entry, repeated warm and cold shader-cache runs | Identify first-use preparation and presentation spikes |
| Omega entrance and KO | Check frame-history effects and audio catch-up |
| Beach, Ice Cave and dense particle scenes | Protect animated textures, reflections, clipping and sprites |
| Repeated character/stage changes and Tag | Check resource residency, address reuse and cache pressure |
| Movie start, skip and ending | Check streaming, transition stalls and capture overhead |
| Two-peer online single and tag matches | Separate network waits and verify deterministic behavior |

Report p50/p95/p99/max, frames over 20/25/33.3 ms, draw counts, CPU phase costs, GPU time, allocations/uploads/evictions, audio queue/underruns and memory trends. Compare matched scenes with repeated runs. Accept an optimization only when the targeted cost and full-frame tails improve without changing native timing, visuals, audio or replay behavior.

The initial inspection did not modify runtime source. The implementation above supersedes the repaired findings and initial validation counts; the remaining measurement scenarios still apply.
