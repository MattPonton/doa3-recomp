# Dead or Alive 3 — Static Recompilation for Windows

Translation of the original Xbox binary of Dead or Alive 3 into native C
that compiles and runs on Windows as a native x86-64 executable.

**Status:** All game modes are fully playable

## Features
- New online mode added for single and tag matches
- Capped at 60 FPS
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

Game files are **not** included. On first launch of DOA3.exe, select your legally obtained copy of Dead or Alive 3 (USA) in `.xiso` or
`.xiso` format. Asset files are automatically extracted, save data is created, and then the game will be ready to play once this completes.

Press the **Escape** key to open the overlay menu. This has online, controller mapping, and other settings.

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
