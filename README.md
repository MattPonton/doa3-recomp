# Dead or Alive 3 — Static Recompilation for Windows

Translation of the original Xbox binary of Dead or Alive 3 into native C
that compiles and runs on Windows as a native x86-64 executable.

**Status:** All game modes are fully playable

**This branch: `codex/120hz`** — experimental interpolated rendering based on
Jay's branch. Gameplay remains at 60 Hz. The verified high-refresh setup is
120 Hz at 1× resolution in Beach Practice; broader stage/mode coverage, higher
resolutions and 240 Hz still need validation. See the
[branch changelog](doa3/CHANGELOG.md) for changes and measured results.

## Features
- New online mode added for single and tag matches
- 60 Hz gameplay with optional experimental 120/240 Hz interpolated rendering
- Windowed/Borderless and 4:3/16:9 options
- Internal resolution scaling to 1440p (3x)

<img width="640" height="480" alt="izuna" src="https://github.com/user-attachments/assets/cb91f3eb-0e5c-4043-b23a-96ab06497ac8" />

## Building

Needs Visual Studio 2022 (C11) and CMake 3.20+.

```bash
cmake -S doa3 -B doa3/build
cmake --build doa3/build --config Release --target doa3
```

The binary lands in `doa3/build/release/DOA3.exe`.

## Running

Download the latest release from the releases tab

Game files are **not** included. This branch targets **Japanese Dead or Alive 3
3.1 (NTSC-J)**; see [the porting notes](doa3/PORTING_3.1.md) for the required
XBE fingerprint. Use your legally obtained game files with the asset importer.

Press the **Escape** key to open the overlay menu. This has online, controller mapping, and other settings.

Under **Audio**, adjust **Master volume** from 0% (mute) to 100%. It affects all
game audio, including movies. Changes apply immediately; click **Save** to keep
the level between launches. **Reset to defaults** restores 100%.

### Experimental high-refresh rendering

Under **Escape → Video**, set **Internal resolution** to **1×** and **Render
rate** to **120 Hz (experimental)**. Choose **Save** to keep the settings.
The default remains **60 Hz (native)**. Extra frames interpolate geometry;
they do not advance attacks, inputs or game logic faster.

Lighting, reflections and texture animation retain their native cadence.
Hitbox, hurtbox and hazard overlays use native rendering. Interpolation can
add up to one logic frame of visual delay; it is not an input-latency reduction.
Unsupported draws and scene changes may use native geometry or native frames.

If rendering falls behind, the game falls back to 60 Hz and shows a message
in Video settings. Lower the resolution or reselect a render rate to retry.
Switching away suspends interpolation; returning to the game automatically
rearms it. Sustained high refresh at 2×/3× or 240 Hz is not yet verified.

<img width="642" height="512" alt="Screenshot 2026-10-05 172951" src="https://github.com/user-attachments/assets/a3f0d351-4e54-43b4-bb78-0d2f4bedec8b" />

## Credits

This project stands on the amazing work of **Ned Heller (sp00nz)**:

- [xboxrecomp](https://github.com/sp00nznet/xboxrecomp) — the static recompilation toolkit this is
  built with: the x86→C lifter, XBE parser, kernel layer, D3D8→D3D11 compat layer, and NV2A
  translator.
- [burnout3](https://github.com/sp00nznet/burnout3) — the first static recompilation of a retail
  original Xbox game, and the reference this port follows. The NV2A→D3D11 push-buffer translator
  comes from it directly.

Also relies on:

- [Cxbx-Reloaded](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded) — used as a debugging oracle;
  Breakpointing the emulator and diffing its behaviour against ours found most of the bugs.
- [xemu](https://github.com/xemu-project/xemu) — NV2A documentation and the texture unswizzling
  algorithm.
- [Xbox Dev Wiki](https://xboxdevwiki.net) — XBE format, kernel exports, NV2A registers.

## License

Project code is MIT, following xboxrecomp. Dead or Alive 3 and its assets are property of Tecmo /
Koei Tecmo; nothing from the game is distributed here. You need your own copy.

## Bugs & Contributing

- Come across any issues? Be sure to open an issue ticket with reproduction steps. Error logs written to `doa3_log.txt` are also helpful.
- Pull requests are welcome.

## Debug unlocks

Open **Escape > Debug > Unlockables** and tick **Unlock all unlockables** to access characters,
costumes and Theater movies. Re-enter the selection screen to refresh its list.
Unticking restores normal unlock checks; earned progress and the game save are
preserved. Choose **Save** to remember the toggle in `doa3_settings.ini`.

## Debug free camera

Enter a stage, open **Escape > Debug > Camera controls**, enable **Free camera**, then close the
overlay. **Reset to game camera** moves it back to the current normal view;
unticking **Free camera** returns control to the game's camera.

| Action | Keyboard / mouse | Controller |
| --- | --- | --- |
| Move forward/back and strafe | WASD | Left stick |
| Look around | Mouse | Right stick |
| Up / down | E / Q | RT / LT |
| Faster / slower | Shift / Ctrl | LB / RB |

Adjust **Camera speed** in Debug for close inspection or travelling across a
stage. Escape opens the settings and releases the mouse; switching to another
application also releases it. Fighter input is suppressed while free camera is
enabled, but the fight continues, so Practice is useful for inspection.
Camera mode and speed are temporary and do not change the save file. Free camera
is unavailable during online play. The camera can pass through scenery; stages
may have missing backsides or distant geometry outside the normal playable view.

**Field of view** adjusts the vertical viewing angle from 15 to 120 degrees.
It works with the normal game camera as well as free camera: lower values zoom
in, higher values show more of the scene. **Reset field of view** restores the
game's scripted zoom. FOV is temporary and does not apply during online play.

## Debug vision modes

### Combat diagnostics

In **Escape > Debug > Combat diagnostics**, independently enable **Show
hitboxes**, **Show hurtboxes**, and **Show attack frame data**. The overlays
work with both the normal camera and free camera. Translucent solid red/pink
spheres show strike volumes; green/cyan wireframe spheres and cylinders show
body collision volumes. Amber
indicates an attack that already registered contact. These outlines draw through
scenery and characters and use the game's actual collision geometry, including
stance mirroring. Throw ranges and special projectile collision are not shown.

Frame data shows both players' move IDs and native move-frame counters, plus
current and previous observed strike timing. Startup is the first active frame
(1-based); active is the number of active frames observed, and recovery is the
time observed after the final active frame. Partial captures and interruptions
are marked. This is a live measurement, not a move-list database: cancels can
shorten recovery, and hit/block advantage is not calculated. These settings last
for the session and are disabled during online play. Like the Escape menu, these
host overlays are not included in the game's clean PNG screenshots.

### Performance

**Escape > Debug > Performance > Show performance overlay** displays a compact
panel at the top right with presentation FPS, current/average frame interval,
99th-percentile and worst frame time, a rolling 240-frame graph, and render
resolution. **Reset performance history** clears the samples. Timing includes
pacing and presentation waits; it does not measure GPU render time or simulation
FPS. The toggle is session-only and writes no profiling files.

### Training

**Escape > Training** is available only in offline Sparring. It adds three
dummy input recording slots (up to 600 frames / ten seconds each), single
playback and looped playback. Record temporarily controls the dummy using your
normal keyboard/controller bindings. Press **Escape** to save the recording
or stop playback. Directions replay as recorded, without resetting positions.
Slots last for the current Sparring session. Recording/playback requires a
solo fight with Exercise, the native pause menu and free camera disabled.

Guard, counter-action and counter-hit settings remain in the original
Sparring pause menu; the new tab does not duplicate those controls.

### Rendering

In **Escape > Debug > Vision modes**, choose a **Render view**:

- **Normal:** original game rendering.
- **Wireframe:** triangle edges for inspecting meshes and intersections.
- **Vertex colors:** vertex lighting/color without textures or fog.
- **UV checker:** the first texture coordinate set, with U in red, V in green,
  and a repeating blue checker to inspect texture mapping.
- **Distance:** view-axis distance in grayscale, near black to far white.
  Adjust **Distance range** for close character inspection or a whole stage.

**Show backfaces** disables GPU face culling for eligible draws. It can reveal
the reverse side of submitted surfaces, but cannot restore geometry the game
omits. These views affect depth-tested color draws; offscreen reflection/mask
targets and draws with depth testing disabled retain normal rendering. Surface
alpha tests and blending are replaced, so transparency and cutouts may look
solid. The Escape overlay remains readable. Use **Restore normal rendering**
to clear both the view mode and backface override. Options reset on launch and
work independently of the free camera.

## Screenshots and hidden UI

Debug sections start collapsed; click a section heading to expand its controls.
Open **Escape > Debug > Screenshots and UI** to assign a screenshot hotkey
(default **F12**). Key changes save automatically. Close the settings and press
the assigned key, or use **Take screenshot** inside the menu. PNGs save in the
**Screenshots** folder beside `DOA3.exe`, at the game's render resolution and
without the Escape overlay. Use a key not already assigned to game controls.

Enable **Hide UI** for unobstructed stage and character views. This hides game
screen overlays, including HUD and menus; some fades may also disappear.
**Escape** still opens settings so you can untick it. Hide UI resets on launch.
