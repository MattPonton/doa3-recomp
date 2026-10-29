@echo off
rem Builds dspdis.exe and fxrun.exe against xemu's DSP core (fetch_xemu.py first).
rem Needs the MSVC x64 tools: run from a Developer Command Prompt, or this
rem finds them through vswhere.
setlocal
cd /d "%~dp0"
if not exist xemu\interp\dsp_cpu.c (
    echo xemu sources missing: py -3 tools\dsstdfx\dsp\fetch_xemu.py
    exit /b 1
)
if not defined VCToolsInstallDir (
    for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do call "%%i\VC\Auxiliary\Build\vcvars64.bat" >nul
)
if not exist out mkdir out
set INC=/I shim /I xemu\interp /I xemu
cl /nologo /O2 /w %INC% dspdis.c /Foout\ /Fe:out\dspdis.exe || exit /b 1
cl /nologo /O2 /w %INC% fxrun.c xemu\dsp.c xemu\dsp_c.c xemu\dsp_dma.c xemu\interp\dsp_cpu.c /Foout\ /Fe:out\fxrun.exe || exit /b 1
