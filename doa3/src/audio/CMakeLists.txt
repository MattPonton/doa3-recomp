# Xbox DirectSound, high level, on OpenAL Soft (third_party/openal-soft)

add_library(dsound_compat STATIC
    dsound_hle.c     # buffers, voices, 3D, I3DL2 reverb; loopback mix
    oal_output.c     # the output device the mix plays on
)

target_include_directories(dsound_compat PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dsound_compat PRIVATE OpenAL::OpenAL winmm)

if(MSVC)
    target_compile_options(dsound_compat PRIVATE
        /W4
        $<$<NOT:$<CONFIG:Debug>>:/O2>
    )
    target_compile_definitions(dsound_compat PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(dsound_compat PRIVATE -Wall -Wextra -O2)
endif()
