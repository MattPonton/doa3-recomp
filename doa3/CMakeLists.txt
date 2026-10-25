cmake_minimum_required(VERSION 3.20)
if(POLICY CMP0184)
    cmake_policy(SET CMP0184 NEW)
endif()
project(doa3_recomp VERSION 1.0 LANGUAGES C CXX)

if(MSVC AND POLICY CMP0184)
    foreach(language C CXX)
        string(REPLACE "/RTC1" "" debug_flags "${CMAKE_${language}_FLAGS_DEBUG}")
        set(CMAKE_${language}_FLAGS_DEBUG "${debug_flags}" CACHE STRING
            "Flags used by the ${language} compiler during Debug builds" FORCE)
    endforeach()
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Common definitions
add_compile_definitions(
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# Version-specific XBE layout (xbe_layout.h), generated next to the recompiled
# code by `py -3 -m tools.xbe_layout --header src/game/recomp/gen/xbe_layout.h`.
include_directories(${CMAKE_SOURCE_DIR}/src/game/recomp/gen)

# ── OpenAL Soft (LGPL-2.1): the game's sound, linked static into DOA3.exe ──
# Only the library: no utilities, examples, install rules or legacy EAX.
# Backends limited to Windows' own (WASAPI, then DirectSound / WinMM) so a
# JACK or PortAudio install on the build machine cannot change the build.
set(LIBTYPE STATIC)
foreach(option ALSOFT_UTILS ALSOFT_EXAMPLES ALSOFT_TESTS ALSOFT_INSTALL
        ALSOFT_INSTALL_CONFIG ALSOFT_INSTALL_HRTF_DATA ALSOFT_INSTALL_AMBDEC_PRESETS
        ALSOFT_INSTALL_EXAMPLES ALSOFT_INSTALL_UTILS ALSOFT_UPDATE_BUILD_VERSION
        ALSOFT_EAX ALSOFT_BACKEND_PIPEWIRE ALSOFT_BACKEND_PULSEAUDIO
        ALSOFT_BACKEND_JACK ALSOFT_BACKEND_PORTAUDIO ALSOFT_BACKEND_SDL2
        ALSOFT_BACKEND_SDL3 ALSOFT_BACKEND_WAVE)
    set(${option} OFF)
endforeach()
add_subdirectory(third_party/openal-soft EXCLUDE_FROM_ALL)

# ── Compatibility layers (static libraries) — mirrors burnout3 ──
add_subdirectory(src/kernel)   # Xbox kernel imports -> Win32, memory layout, dispatch
add_subdirectory(src/d3d)      # D3D8 -> D3D11 graphics abstraction
add_subdirectory(src/audio)    # Xbox DirectSound, high level, on OpenAL Soft
add_subdirectory(src/input)    # Xbox input -> XInput
add_subdirectory(src/online)   # Online VS play
add_subdirectory(src/ui)       # Overlay Menu (Esc Key)

# ── Recompiled game executable ─────────────────────────────────
add_subdirectory(src/game)
