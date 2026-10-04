# Online VS play (delay-based lockstep)

# ── GekkoNet (BSD-2): the lockstep input exchange ─────────────────────
# Built static, without its ASIO adapter: the port supplies its own UDP
# adapter (netplay.c) so lobby traffic and lockstep traffic share one socket.
set(GEKKO_DIR ${CMAKE_SOURCE_DIR}/third_party/GekkoNet)
file(GLOB GEKKO_SOURCES ${GEKKO_DIR}/src/*.cpp)
add_library(gekkonet STATIC ${GEKKO_SOURCES})
target_include_directories(gekkonet
    PUBLIC  ${GEKKO_DIR}/include
    PRIVATE ${GEKKO_DIR}/include/private
            ${GEKKO_DIR}/thirdparty)
target_compile_definitions(gekkonet PUBLIC GEKKONET_STATIC GEKKONET_NO_ASIO)
set_target_properties(gekkonet PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
if(MSVC)
    target_compile_options(gekkonet PRIVATE /W0 /WX- /EHsc)
    set_property(TARGET gekkonet PROPERTY MSVC_RUNTIME_CHECKS "")
endif()

# ── miniupnpc (BSD-3): opens the host's UDP port on the router ────────
set(UPNP_DIR ${CMAKE_SOURCE_DIR}/third_party/miniupnpc)
add_library(miniupnpc STATIC
    ${UPNP_DIR}/src/igd_desc_parse.c
    ${UPNP_DIR}/src/miniupnpc.c
    ${UPNP_DIR}/src/minixml.c
    ${UPNP_DIR}/src/minisoap.c
    ${UPNP_DIR}/src/minissdpc.c
    ${UPNP_DIR}/src/miniwget.c
    ${UPNP_DIR}/src/upnpcommands.c
    ${UPNP_DIR}/src/upnpdev.c
    ${UPNP_DIR}/src/upnpreplyparse.c
    ${UPNP_DIR}/src/upnperrors.c
    ${UPNP_DIR}/src/connecthostport.c
    ${UPNP_DIR}/src/portlistingparse.c
    ${UPNP_DIR}/src/receivedata.c
    ${UPNP_DIR}/src/addr_is_reserved.c
)
target_include_directories(miniupnpc
    PUBLIC  ${UPNP_DIR}/include
    PRIVATE ${UPNP_DIR} ${UPNP_DIR}/src)
target_compile_definitions(miniupnpc
    PUBLIC  MINIUPNP_STATICLIB
    PRIVATE _WIN32_WINNT=0x0601 _CRT_SECURE_NO_WARNINGS)
target_link_libraries(miniupnpc PUBLIC ws2_32 iphlpapi)
if(MSVC)
    target_compile_options(miniupnpc PRIVATE /W0 /WX-)
    set_property(TARGET miniupnpc PROPERTY MSVC_RUNTIME_CHECKS "")
endif()

# ── the port's online code ────────────────────────────────────────────
add_library(doa3_online STATIC
    netplay_session.c
    xbox_det.c
    netplay.c
    net_upnp.c
    join_code.c
)

target_include_directories(doa3_online
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/kernel
)

target_link_libraries(doa3_online PUBLIC gekkonet miniupnpc ws2_32)

if(MSVC)
    target_compile_options(doa3_online PRIVATE /W3 /WX-)
    set_property(TARGET doa3_online PROPERTY MSVC_RUNTIME_CHECKS "")
    target_compile_definitions(doa3_online PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()
