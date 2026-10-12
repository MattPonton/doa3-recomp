# Experimental 120 Hz branch

## 2026-10-12 — Changes since `17b8974`

This update branches from `codex/jays-branch` at `17b8974` (the last pushed
controller-discovery hitch fix). It introduces experimental interpolated
presentation while retaining the original 60 Hz gameplay schedule. The
existing build version is 0.0.76; this branch update is not a tagged release.

### Rendering and settings

- Add a persisted Video render-rate selection: native 60 Hz (default),
  experimental 120 Hz and experimental 240 Hz. Reset restores native 60 Hz.
- Capture a three-frame history of backbuffer geometry, indices, constants,
  texture versions and graphics state. Replay interpolated geometry into
  private color/depth targets using a D3D11 deferred context, restoring the
  immediate context afterward.
- Interpolate projected CPU vertices in homogeneous coordinates and match
  fixed-function meshes using stable geometry/material information, even
  when effects change draw order. Reject ambiguous matches, large jumps and
  incompatible geometry instead of interpolating unrelated objects.
- Keep current lighting and material data during geometry interpolation.
  Render-to-texture effects, reflections, texture animation and HUD data
  retain native timing. This is geometry interpolation, not image blending.
- Schedule additional presentations during rendering and pacing checkpoints,
  anchored to the first actual presentation. Copy the completed native image
  for the final subdivision instead of replaying the entire scene again.
  Extra presentations never display a partially built guest frame.
- Keep guest input/game updates, native vblank callbacks and audio callbacks
  on the existing native frame path. Do not alter PAL timing constants or
  animation speed. The PAL path adjusts gameplay timing and is not an
  independent 60 Hz simulation/50 Hz renderer suitable for this feature.

### Reliability and performance

- Invalidate history for scene changes, unsupported framebuffer feedback and
  unsupported draw paths. Collision/hazard overlays use native rendering.
  Recreate interpolation targets after internal-resolution changes and
  release resources when interpolation shuts down.
- Snapshot mutable textures at the version sampled by each draw. Track
  texture unlocks and offscreen render writes so subsequent draws capture
  fresh contents without changing earlier snapshots.
- Capture shader constants from CPU copies rather than reading mapped
  write-combined GPU memory. Keep a GPU-copy fallback for unknown buffers.
- Reuse geometry buffer capacity with discard uploads, reuse identical
  consecutive constants, skip redundant graphics-state commands and avoid
  uploading both native and interpolated constants for the same draw.
- Protect gameplay timing with a 60 Hz fallback after more than 50 ms of
  accumulated tick delay following warmup. Complete native tick intervals
  include pacing and catch-up; short recoverable workload spikes do not
  automatically count as sustained slowdown.
- Suspend interpolation while unfocused and reset its timing guard on focus
  transitions. Background compositor stalls no longer permanently disable
  high refresh after switching windows.

### Diagnostics and tests

- Add independent counters for native logic/input ticks, host presentations,
  replayed/interpolated draws, capture cost, replay preparation/execution cost
  and replay buffer allocations.
- Add an explicit `DOA3_INTERPOLATION_CHECK=1` developer capture that compares
  an alpha=1 replay with the native frame and saves a midpoint image. These
  screenshots remain disabled in ordinary runs. Extra presentations do not
  duplicate normal screenshot capture or update guest crossfade history.
- Add the standalone `tests/interpolation` WARP suite. It exercises actual
  rendered CPU/GPU midpoints, perspective endpoints, camera transforms,
  texture/constant isolation, graphics-state restoration, draw-order changes,
  shared and changed material constants, scene changes, invalidation,
  disable behavior, catch-up and sustained-overload fallback.

Run the tests from the repository root on Windows:

```powershell
cmake -S doa3/tests/interpolation -B doa3/build/interpolation-tests
cmake --build doa3/build/interpolation-tests --config Release
ctest --test-dir doa3/build/interpolation-tests -C Release --output-on-failure
```

### Validation and remaining limits

- Release build and the WARP suite pass.
- Native versus alpha=1 replay comparisons matched every pixel in sampled
  frames at 1× and 3×. This verifies those replay endpoints, not every
  interpolated frame or stage.
- At 1× in Beach Practice, the user reported no flashing or slowdown while
  moving and attacking with the optimized renderer.
- A 39-second combat sample measured 60.01 logic/input ticks and 120.07 host
  presentations per second before a background-focus stall exposed the old
  guard behavior.
- After the focus fix, switching away and back resumed interpolation. The
  next 29.02-second sample measured 60.03 logic ticks and 119.81 host
  presentations per second, with interpolation active throughout and a
  median replay CPU cost of 2.82 ms.
- These are host counters, not monitor scanout or end-to-end latency
  measurements. Interpolation can add up to one logic frame of visual delay.
  Higher resolutions, 240 Hz, longer sessions, other stages/modes and online
  behavior still need validation. This remains an experimental feature.

The existing hurtbox implementation was investigated during this work;
the native 3.1 collision routines use finite cylinders and separate spheres.
No capsule conversion or other collision/gameplay behavior change is included.
See [PORTING_3.1.md](PORTING_3.1.md) for implementation details.
