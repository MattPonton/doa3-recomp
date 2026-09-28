# Online VS play (delay-based lockstep)

add_library(doa3_online STATIC
    netplay_session.c
    xbox_det.c
)

target_include_directories(doa3_online
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/kernel
)

if(MSVC)
    target_compile_options(doa3_online PRIVATE /W3 /WX-)
    target_compile_definitions(doa3_online PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()
