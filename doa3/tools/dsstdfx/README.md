# dsstdfx.bin tools

Offline tools for the XDK effects image the game downloads to the MCPX GP
DSP (`D:\dsstdfx.bin`, XDK 4134). None of this is built into the game.

| step | command (from `doa3/`) | what it gives |
| --- | --- | --- |
| graph | `py -3 -m tools.dsstdfx info` | the ten effects, their mixbins and state blocks |
| decrypt | `py -3 -m tools.dsstdfx decrypt out.bin` | the image with its DSP code decrypted |
| rooms | `py -3 -m tools.dsstdfx rooms dir` | each I3DL2 room's reverb state, from DSOUND's own code (`pip install unicorn`) |
| disassemble | `tools\dsstdfx\dsp\out\dspdis.exe out.bin [fx]` | DSP56300 listing per effect |
| run | `tools\dsstdfx\dsp\out\fxrun.exe out.bin dir\room30.state 10 3000 0x4bb ir.raw 6 7 8 9` | an effect's response (here: impulse into the I3DL2 bin, reverb only, bins 6-9, float32) |

The two executables need `py -3 tools/dsstdfx/dsp/fetch_xemu.py` and then
`tools\dsstdfx\dsp\build.bat` (MSVC x64).

## Credits and licences

- **xemu**'s MCPX DSP emulation does the disassembly and execution:
  `hw/xbox/mcpx/apu/dsp` (DSP56300 interpreter, GP DMA, disassembler), by
  espes and Matt Borgerson, adapted from the **Hatari** DSP56001 emulation
  (ARAnyM developer team, Thomas Huth). GPL-2.0-or-later.
  <https://github.com/xemu-project/xemu>, used unchanged at commit
  `67cc79e663038d1f55448c0f566b37dde016adf6`. `fetch_xemu.py` downloads those
  files into `dsp/xemu/` (not committed).
- Because they compile against that code, `dsp/dspdis.c`, `dsp/fxrun.c` and
  the stand-in headers in `dsp/shim/` are GPL-2.0-or-later too, unlike the
  rest of this repository (MIT). They stay standalone programs, outside the
  game build.
- `i3dl2.py` runs DSOUND's code from the XBE on **Unicorn**
  (<https://www.unicorn-engine.org/>, GPL-2.0), installed with pip, not
  bundled.
- The Python in this directory (`image.py`, `i3dl2.py`, `__main__.py`) is
  this repository's own (MIT).
