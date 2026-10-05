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

# ── Compatibility layers (static libraries) — mirrors burnout3 ──
add_subdirectory(src/kernel)   # Xbox kernel imports -> Win32, memory layout, dispatch
add_subdirectory(src/d3d)      # D3D8 -> D3D11 graphics abstraction
add_subdirectory(src/audio)    # DirectSound -> XAudio2
add_subdirectory(src/input)    # Xbox input -> XInput
add_subdirectory(src/apu)      # MCPX APU emulation (from xemu)
add_subdirectory(src/online)   # Online VS play
add_subdirectory(src/ui)       # Overlay Menu (Esc Key)

# ── Recompiled game executable ─────────────────────────────────
add_subdirectory(src/game)
