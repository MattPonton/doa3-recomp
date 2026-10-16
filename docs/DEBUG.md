# Debug tools

The **Debug** tab of the Escape menu holds developer and inspection tools.
They are not needed to play, some may change or be removed before v1.0, and
most are unavailable during online play. Sections start collapsed; click a
heading to expand it.

## Unlockables

Open **Escape > Debug > Unlockables** and tick **Unlock all unlockables** to access characters,
costumes and Theater movies. Re-enter the selection screen to refresh its list.
Unticking restores normal unlock checks; earned progress and the game save are
preserved. Choose **Save** to remember the toggle in `doa3_settings.ini`.

## Camera controls

Enter a stage, open **Escape > Debug > Camera controls**, enable **Free camera**, then close the
overlay. **Reset to game camera** moves it back to the current normal view;
unticking **Free camera** returns control to the game's camera.

| Action | Keyboard / mouse | Controller |
| --- | --- | --- |
| Move forward/back and strafe | WASD | Left stick |
| Look around | Mouse | Right stick |
| Up / down | E / Q | RT / LT |
| Faster / slower | Shift / Ctrl | LB / RB |

Adjust **Camera speed** (1–20, default 10) for close inspection or travelling across a
stage. Escape opens the settings and releases the mouse; switching to another
application also releases it. Fighter input is suppressed while free camera is
enabled, but the fight continues, so Practice is useful for inspection.
Camera mode and speed are temporary and do not change the save file. Free camera
is unavailable during online play. The camera can pass through scenery; stages
may have missing backsides or distant geometry outside the normal playable view.

## Combat diagnostics

Independently enable **Show hitboxes**, **Show hurtboxes** and **Show attack
frame data**. The overlays work with the normal camera and the free camera,
draw through characters and scenery, and use the game's own collision
geometry, including stance mirroring.

- Red/pink solid spheres: hitboxes (strike volumes).
- Green/cyan wireframe spheres and cylinders: hurtboxes (body volumes).
- Amber: an attack that has already registered contact.

Frame data shows both players' move IDs, the move's base startup (first
active frame), active duration and recovery, next to the strike frames
actually observed. The timeline uses blue for startup, red for active, yellow
for recovery and gray for missing samples. Cancels shorten the observation,
not the base timing; hit/block advantage is not measured yet. Throw ranges
and projectile collision are not shown.

**Stage hazards** adds the stage's collision data:

- **Slippery ground** (blue).
- **Special boundaries** (purple): ledges, breakable barriers and other
  stage reactions; their behavior depends on the stage and floor level.
- **Ordinary walls** and **Collidable objects** (orange).

These settings last for the session.

## Performance

**Escape > Debug > Performance > Show performance overlay** displays a compact
panel at the top right with presentation FPS, current/average frame interval,
99th-percentile and worst frame time, a rolling 240-frame graph, and render
resolution. **Reset performance history** clears the samples. Timing includes
pacing and presentation waits; it does not measure GPU render time or simulation
FPS. The toggle is session-only and writes no profiling files.

## Vision modes

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

## Screenshots and UI

Open **Escape > Debug > Screenshots and UI** to assign a screenshot hotkey
(default **F12**). Key changes save automatically. Close the settings and press
the assigned key, or use **Take screenshot** inside the menu. PNGs save in the
**Screenshots** folder beside `DOA3.exe`, at the game's render resolution and
without the Escape overlay. Use a key not already assigned to game controls.

Enable **Hide UI** for unobstructed stage and character views. This hides game
screen overlays, including HUD and menus; some fades may also disappear.
**Escape** still opens settings so you can untick it. Hide UI resets on launch.

**Hide Player 1** and **Hide Player 2** independently hide the fighters during
battle, including their model shadows and reflections. These session-only
options do not stop attacks, movement, or collision diagnostics, and reset on
launch or when resetting Debug settings. They are unavailable online.
Hitboxes and hurtboxes also remain visible while the native Start menu pauses
a fight, including when enabled after pausing.

## Diagnostics

**Automatic diagnostic captures** writes developer traces and screenshots to
the `Diagnostics` folder next to `DOA3.exe` when specific events happen
(multisampled screens, frame-buffer effects), and adds frame-timing and per-
draw timing lines to the log plus `performance_frames.csv`. The folder is
emptied at every launch. Leave this off for normal play; turn it on, together
with **General > Write log files**, when collecting data for a bug report.

**Trace the next frame** records one frame's graphics commands
(`mtrace_manual_*.txt`) and a screenshot (`aa_*.bmp`) in the `Diagnostics`
folder, whether or not automatic captures are on. Open the menu while a visual
bug is on screen and press it; the game keeps running behind the menu.

**Save the next frame's textures** writes every texture drawn in the next
frame as a `.dds` file in a `Textures` folder next to `DOA3.exe` (kept between
launches), named `doa3_<memory address>_<width>x<height>_f<format>.dds`. The
data is what the port uploads: unswizzled, palettised textures expanded to
32-bit colour, DXT textures kept compressed. Images rendered by the game at
run time (reflections, environment maps captured from the stage) are not
included, only textures loaded from the game's files.

## Online developer settings

**Online > Developer Settings** runs a local two-player session on one PC
(host or joiner path, optionally with Player 2 mirroring Player 1), writes
diagnostic logs, and records and replays sessions to check that both sides
stay in sync. It is meant for netcode development.
