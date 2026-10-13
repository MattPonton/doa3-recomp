# DOA3 3.1 changelog

## 0.0.77 — 2026-10-12

Adds five aspect-ratio choices and the input-focus toggle, and includes the
castle-fire, Omega entrance, timer-shadow and Ice Cave rendering corrections
delivered since 0.0.76. Release builds and the aspect-ratio and input regression
tests pass. The new ultrawide modes still need in-game visual confirmation.

These fixes retain native 60 Hz rendering and gameplay. Experimental 120/240 Hz
presentation remains on its separate branch. The Release build and six suites
(clipping, HUD fade, point sprites, frame history, reflection mask and combiner
cache) pass on this native-rate version.

### Input focus setting

- Add **Input only when focused** to Escape > Controls. Enabled by default;
  disable it to allow background gameplay input. The setting applies to
  keyboard, Xbox and PS4 controllers across all four players and is persisted
  by Controls > Save in `doa3_input.ini`.
- Keep controllers connected while filtering unfocused input to neutral.
  Report focus-driven releases/restores as changed input packets even when
  the physical controller's packet has not changed. Cancel pending rebinding
  when focus is lost; existing menu, free-camera and rumble focus rules remain.
- Extend input regressions for focus transitions, background input, live
  setting changes, keyboard/PS4 routing, persistence and older settings files.

### Aspect ratios

- Expand Video > Aspect ratio to 4:3 (Full Screen), 4:3 (Anamorphic), 16:9,
  21:9 and 32:9. Full Screen uses a 3/4 projection ratio; Anamorphic and 16:9
  use 9/16; ultrawide modes use 9/21 and 9/32. Both 4:3 modes use the same
  4:3 display area.
- Update the mapped 3.1 projection float and cached video flags at startup
  and frame boundaries, including switches between the two 4:3 modes without
  resizing. Fit presentation and combat overlays to each display ratio.
- Save the selection under Video/AspectRatio and migrate legacy Widescreen=0
  to 4:3 (Anamorphic). Leave the original XBE on disk unchanged.
- Add regression coverage for all five modes, exact projection bits, 1x/2x/3x
  target sizes, cached flags, persistence and legacy settings migration.

### Ice Cave reflection triangles

- Fix precision loss when clipping large reflection vertices against the near
  plane. Some intersections collapsed to the screen origin, producing dark
  translucent triangles across Ice Cave during movement even at native 60 Hz.
- Calculate independent endpoint weights in double precision and pin each
  intersection to its clip plane, preserving colors and both texture UV sets.
  The correction applies to the shared clipper rather than specific meshes.
- Add 12 captured-triangle regression fixtures, checking both winding orders,
  projection, ordinary attribute interpolation and eye/near-plane clipping.
  Clipping and reflection-mask tests pass; the user confirmed the corrected
  Ice Cave appearance in game.

### Round-start timer shadow

- Preserve transparent black vertex colors in the HUD's texture-times-diffuse
  program. The timer shadow previously appeared fully opaque before the timer
  faded in because zero color was mistaken for a missing diffuse attribute.
- Add a regression for the captured shadow fade and isolation of the existing
  fallback for other rendering programs.

### Omega entrance blur

- Retain the completed guest frame for framebuffer-sampling effects. The
  previous pre-overlay capture replaced history during rendering and removed
  accumulated after-images, weakening Omega's Story entrance blur.
- Preserve the game's blend strength and UV offsets; no global blur multiplier
  is added. Native HUD/text can appear in trails where the game requests it,
  as seen in the supplied original-game footage. Host overlays remain
  outside the guest frame history.
- Extend the frame-history WARP test to check history stability during drawing,
  after-image persistence across multiple frames, and fresh captures after
  inactivity. In-game comparison of the corrected entrance remains pending.

### Castle fire particles

- Preserve the guest's particle colors and fading for T3-only point sprites.
  Ryu's burning-castle scene supplies orange embers with changing opacity;
  the legacy texture-only fallback discarded both and made them bright white.
- Use the captured NV2A register-combiner state for inline and array point
  sprites, with the generated sprite coordinates supplied to texture unit 3.
  Other texture programs and the older marker-based path retain their behavior.
- Add a WARP rendering regression test for the castle's shader, sprite alpha
  mask, particle tint/fade, texture-colored particles, and shader cleanup.
  Release compilation and the point-sprite and combiner-cache tests pass;
  the corrected cutscene still needs in-game visual confirmation.
