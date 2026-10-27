# DOA3 3.1 changelog

## 1.0.0 — 2026-10-27

First release of the Dead or Alive 3 (JAPAN, 3.1) recompilation. Since 0.0.128:

**Game**
- Dip Switches (General tab), all off by default so the game plays as on
  hardware: left stick in menus; Random as the default stage option (Tag and
  Team Battle included); Random picking alternate and storage costumes;
  shared costumes with a different option; corrected menu pixel offset; the
  KO replay and the winner pose only after a match's final round; Beach's
  and Aqua Palace's places on the Stage Select world map; and a fix for
  Lorelei's double-hit ledge falls (a throw's damage lost on the fall's
  first hit, or the landing damage dealt twice).
- Storage costumes (TDATA): up to 99 per character, with x / y / l options;
  the costume list scrolls past nine.
- Options > Video > Regulator (gamma, brightness, contrast) now changes the
  picture.

**Sparring**
- Record and play back the dummy's inputs (three slots, loop); playback
  follows the dummy's facing, so forward stays forward after a side switch.
- A second controller can take the dummy, or in Tag Sparring your partner.
- Hold the Stagger Escape value for either fighter.
- Show Move Info (frame data).

**Graphics**
- Menu and HUD line work matched to hardware at 16:9, 21:9 and 32:9: line
  widths and positions, box corners, panel edges, the supersampled menus'
  half-pixel offset and averaging, and atlas sprites no longer bleeding into
  their neighbors (character names, Options buttons).

**Escape menu and app**
- Tabs reorganized: General first (check for updates at boot, logging, dip
  switches); Video (menu size, performance overlay); Photo Mode (free camera,
  hide either fighter or the HUD, screenshots); Sparring; developer tools
  moved out of view.
- Check for updates at boot (announces a newer GitHub release; never
  downloads anything).
- Keyboard layout on Reset to defaults when the input device is Keyboard.
- First-run setup starts the game in a fresh process after copying the disc
  image (it could close instead of starting).
- The executable carries a plain placeholder icon; the window uses the
  title image from the user's own disc.

## 0.0.128 — 2026-10-13

- Extend the centered ultrawide character-select portrait and backing-panel
  correction to 2×1/four-slot layouts, including Tag Battle. Preserve the
  existing solo layout, 4:3/16:9 behavior and main-menu pixel offsets.
- Cache generated shader bytecode between launches with validated cache
  records and normal compilation fallback for missing or damaged entries.
- Reduce audio voice-retirement waits using IRQ wakeups and control-only
  idle notifications, preserving guest acknowledgement and deterministic
  audio behavior.
- Remove redundant shader constant uploads and unused CPU vertex-operand
  fetches. Expand texture-cache capacity within the existing byte budget,
  reuse compatible allocations, batch initial mip uploads, consolidate
  staging allocations and avoid redundant texture decode copies.
- Add opt-in frame-phase, texture, shader and audio timing diagnostics and
  regression tests. Retain native 60 Hz logic and low-latency frame pacing.
- Preserve upstream v0.0.123–v0.0.127 settings, audio/streamer controls and
  diagnostic changes. The separate experimental Online carousel and
  120/240 Hz work are not part of this update.
- **Known issue:** Hayabusa Story intro startup spikes remain around
  31–38 ms in live testing; separate presentation stalls also remain.
  Component benchmark gains do not establish an overall hitch fix.
- Validation: Release builds as v0.0.128, and all 12 selected regression
  suites pass after integration with the latest 3.1 changes.

## 0.0.122 — 2026-10-13

- Align character-select previews and their dark backing panels with the
  centered 16:9 interface at 21:9 and 32:9. Move portrait viewport centres
  and clipping together while preserving the models' projected proportions
  in both CPU and GPU rendering paths.
- Preserve 4:3/16:9 behavior, ordinary world rendering, offscreen targets,
  full-screen fades, and the existing main-menu pixel offsets.
- Extend HUD layout regressions for both portrait sides, model width, panel
  alignment, and target isolation. All four aspect/HUD/timer tests and the
  Release build pass; the user confirmed both ultrawide ratios look correct.

## 0.0.121 — 2026-10-13

- Replace the player selector in Controls with Player 1–4 tabs. Keep Player 1's
  Automatic device default; default Players 2–4 to None. Existing saved device
  assignments, bindings, and Xbox controller types remain supported.
- Add **Capture gameplay input in Escape menu**, enabled by default. While
  settings are open, all four players send neutral gameplay input while raw
  controller input remains available for menu navigation and rebinding. Turn
  the option off for controller passthrough; Controls > Save persists it.
- Add independent **Hide Player 1** and **Hide Player 2** options under Debug >
  Screenshots and UI. Hide fighter model draws, including their model shadows
  and reflections, without altering simulation or collision diagnostics.
  Visibility resets on launch or Debug reset and is unavailable online.
- Preserve native Start-pause hitbox/hurtbox rendering, confirmed working by
  the user. Keep the existing main-menu pixel offsets and native 60 Hz build.
- Input routing/rumble, native PS4 support, visibility filtering/settings, and
  combat overlay regressions pass (seven tests).

## 0.0.78 — 2026-10-12

- Center the HUD in a 16:9 area for 21:9 and 32:9, including its clipping
  rectangles. Retain full-width scaling in both 4:3 modes and 16:9, and
  preserve native menu pixel offsets. World geometry, collision overlays,
  offscreen rendering and full-screen fades keep the full render area.
- Keep collision overlays visible during native pause in the same fight,
  including when first enabled while paused. Do not advance attack timing
  while reading frozen geometry. Invalidate snapshots on stage, mode or
  fighter changes; unpaused stale snapshots still expire normally.
- Aspect/HUD layout, combat geometry/paused snapshots, and timer/HUD fade
  regressions pass (six tests), and the Release build succeeds. The new
  ultrawide HUD and paused collision behavior still need in-game confirmation.
- Correct the persistent horizontal seam between the timer frame's upper
  rim and lower body. Clamp the body's top half-texel strip to its first
  artwork row, preventing bilinear filtering from sampling transparent
  atlas padding. Apply the same correction to its shadow without moving
  the frame, stretching the remaining artwork, or changing other textures.
- Add timer-edge regressions for 1x–4x scaling, all five aspect widths,
  shadow fading, unchanged interior UVs, and unrelated geometry. These and
  the existing HUD fade test pass. The timer seam fix is confirmed in-game
  at 3x resolution.

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
