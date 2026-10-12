# DOA3 3.1 rendering fixes

These fixes retain native 60 Hz rendering and gameplay. Experimental 120/240 Hz
presentation remains on its separate branch. The Release build and six suites
(clipping, HUD fade, point sprites, frame history, reflection mask and combiner
cache) pass on this native-rate version.

## Unreleased — Ice Cave reflection triangles

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

## Unreleased — Round-start timer shadow

- Preserve transparent black vertex colors in the HUD's texture-times-diffuse
  program. The timer shadow previously appeared fully opaque before the timer
  faded in because zero color was mistaken for a missing diffuse attribute.
- Add a regression for the captured shadow fade and isolation of the existing
  fallback for other rendering programs.

## Unreleased — Omega entrance blur

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

## Unreleased — Castle fire particles

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
