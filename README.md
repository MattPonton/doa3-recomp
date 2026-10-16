# Dead or Alive 3 — Static Recompilation for Windows

A native Windows port of the original Xbox **Dead or Alive 3**, made by
translating the game's executable into C and running it on a purpose-built
runtime (kernel, Direct3D 11 renderer, XAudio2 audio, input and networking).
No emulator is involved.

This fork targets the **Japanese release** (title ID TC-001, XBE certificate
version 1), the version the community calls **v3.1**. The goal for v1.0 is a
faithful 1:1 port of that version; PC additions are kept optional.

**Status:** every game mode is playable. Work continues on accuracy and on
performance for low-end PCs.

## Features

- Native x86-64 Windows executable; also runs on Steam Deck through Proton
  (Proton Experimental recommended)
- Windowed or borderless fullscreen
- Aspect ratios: 4:3 (full screen or anamorphic), 16:9, 21:9 and 32:9
- Internal resolution 1x (480p), 2x (960p) or 3x (1440p)
- Keyboard, Xbox and PS4 controllers, up to four players, with per-player
  bindings
- Master volume plus the game's own Music / Sound effects / Voice levels
- Streamer mode: mutes the licensed Aerosmith songs wherever they play
- Training: record and replay the Sparring dummy's inputs
- Screenshots (PNG) at render resolution, without the overlay

## Getting started

1. Download the latest release from the **Releases** tab and extract it.
2. Run `DOA3.exe`.
3. On first launch, choose your own disc image of Dead or Alive 3 (Japanese
   release) in `.xiso` or `.iso` format. The game files are extracted into an
   `assets` folder next to `DOA3.exe` and save data is created; after that the
   game starts normally.

Game files are **not** included and are never downloaded. You need your own
legally obtained copy.

Files the port keeps next to `DOA3.exe`:

| File / folder | Contents |
| --- | --- |
| `doa3_settings.ini` | Video, audio and general settings |
| `doa3_input.ini` | Controller and keyboard bindings |
| `HDD\E\UDATA\54430001\` | The game's save data |
| `Screenshots\` | Screenshots |
| `doa3_log.txt` | Log, only when **Write log files** is on |
| `Diagnostics\` | Developer captures, only when diagnostics are on; cleared at each launch |

## Settings (Escape menu)

Press **Escape** at any time to open the settings overlay. Most tabs have
**Reset to defaults** and **Save** buttons; changes apply immediately, and
**Save** keeps them for the next launch.

### Online

**Experimental and untested on this version.** The Online tab comes from the
original USA-based port and has not been tested with the Japanese release;
expect it not to work yet. It lets you host a match and share **Your code**,
or join with a friend's code, with an automatic or manual input delay.

### Controls

- **Input only when focused**: ignore input while another window is active.
- **Capture gameplay input in Escape menu** (on by default): while the menu is
  open, controllers drive the menu and the game sees no input. Turn it off to
  keep playing behind the menu.
- **Player 1–4** tabs: choose a device (Automatic, keyboard or a specific
  controller) and the Xbox controller type, then click a row to rebind it.
  Right-click a binding to clear it. Player 1 defaults to Automatic; Players
  2–4 default to None until assigned.

### Video

**Window type** (windowed / borderless fullscreen), **Aspect ratio** and
**Internal resolution**.

### Audio

- **Master volume**: the PC port's overall output level, applied after the
  game's own mix (including movies).
- **Game volume**: the game's own **Music**, **Sound effects** and **Voice**
  settings, the same as Options > Audio Setting, in steps of 5. They are
  stored in the game's save the next time the game saves.
- **Streamer mode**: silences "Nine Lives", "Amazing" and "Home Tonight"
  wherever they play (opening, credits, sound test) and restores your music
  level afterwards. Your saved music volume is never changed.

### General

**Write log files** writes `doa3_log.txt`, which is useful when reporting a
bug.

### Training

Available in offline Sparring only. Three slots record up to ten seconds of
dummy input each; play a slot once or on a loop. **Record dummy** temporarily
gives you control of the dummy with your normal bindings; **Escape** finishes
the recording or stops playback. Recordings last for the current Sparring
session. Guard, counter-action and counter-hit settings stay in the game's own
Sparring pause menu.

### Debug

Developer and inspection tools: unlock-all, free camera, hitbox and
frame-data overlays, render views, a performance overlay, hiding the HUD or
fighters, and diagnostic captures. Some of these may change or be
removed before v1.0. See [docs/DEBUG.md](docs/DEBUG.md).

## Building

Requires Visual Studio 2022 (C11) and CMake 3.20 or newer.

```bash
cmake -S doa3 -B doa3/build
cmake --build doa3/build --config Release --target doa3
```

The executable is written to `doa3/build/release/DOA3.exe`.

## Reporting bugs and contributing

Please open an issue with steps to reproduce. Turn on **General > Write log
files** first and attach `doa3_log.txt`; for graphics or timing problems, also
enable **Debug > Diagnostics > Automatic diagnostic captures** and attach the
`Diagnostics` folder. Copy both before relaunching, since a new launch
replaces them. Pull requests are welcome.

## Credits

- [parkerallan/doa3-recomp](https://github.com/parkerallan/doa3-recomp) — the
  original Dead or Alive 3 recompilation this fork is based on.
- **Ned Heller (sp00nz)** — [xboxrecomp](https://github.com/sp00nznet/xboxrecomp),
  the static recompilation toolkit the project started from (x86→C lifter,
  XBE parser, kernel layer, D3D8→D3D11 layer, NV2A translator), and
  [burnout3](https://github.com/sp00nznet/burnout3), the first static
  recompilation of a retail Xbox game, from which the NV2A push-buffer
  translator comes.
- [SkillerCMP/xemu-Cheats](https://github.com/SkillerCMP/xemu-Cheats) — the
  xemu build used as the debugging reference for this fork, running the game
  side by side with the port to compare behaviour.
- [Cxbx-Reloaded](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded) — used as a
  debugging reference for the original port.
- [xemu](https://github.com/xemu-project/xemu) — NV2A documentation and code
  for texture unswizzling.
- [Xbox Dev Wiki](https://xboxdevwiki.net) — XBE format, kernel exports, NV2A
  registers.
- Third-party libraries: [Dear ImGui](https://github.com/ocornut/imgui),
  [GekkoNet](https://github.com/HeatXD/GekkoNet),
  [miniupnpc](https://github.com/miniupnp/miniupnp),
  [pl_mpeg](https://github.com/phoboslab/pl_mpeg) and
  [stb_image_write](https://github.com/nothings/stb).

## Legal

This project is not affiliated with, endorsed by or sponsored by Koei Tecmo,
Team Ninja or Microsoft. *Dead or Alive* and all related names, characters,
artwork, music and other game content are the property of their respective
owners. This repository contains no game assets, data files or disc images;
you must supply your own legally obtained copy of the game. The project's
license grants no rights to the game or its content.

## License

The original source code in this repository is released under the
[MIT License](LICENSE). Code derived from xemu keeps its original license,
LGPL-2.1-or-later, as noted in those files' headers. Third-party libraries in
`doa3/third_party` remain under their own licenses, included with each
library.
