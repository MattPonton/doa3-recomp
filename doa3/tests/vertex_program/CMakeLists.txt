cmake_minimum_required(VERSION 3.20)
project(vertex_program_test C)
enable_testing()
set(NV "${CMAKE_CURRENT_SOURCE_DIR}/../../src/nv2a")
# Differential oracle: the previous interpreter fetched all three operands
# unconditionally. Keep its arithmetic identical, removing only the three
# optimization guards so randomized tests catch incorrectly skipped reads.
file(READ "${NV}/nv2a_vertex_program.c" reference)
foreach(guard IN ITEMS
    "if (mac >= MAC_MOV && mac <= MAC_ARL)"
    "if (mac == MAC_MUL || (mac >= MAC_MAD && mac <= MAC_SGE))"
    "if (mac == MAC_ADD || mac == MAC_MAD || ilu != ILU_NOP)")
    string(FIND "${reference}" "${guard}" found)
    if(found LESS 0)
        message(FATAL_ERROR "Operand-fetch reference guard changed")
    endif()
    string(REPLACE "${guard}" "" reference "${reference}")
endforeach()
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/reference.c" "${reference}")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${NV}/nv2a_vertex_program.c")
add_library(vp_reference OBJECT "${CMAKE_CURRENT_BINARY_DIR}/reference.c")
target_include_directories(vp_reference PRIVATE "${NV}")
foreach(name reset write_program write_constant execute dump)
    target_compile_definitions(vp_reference PRIVATE "nv2a_vp_${name}=reference_${name}")
endforeach()
add_executable(vertex_program_test vertex_program_test.c
    "${NV}/nv2a_vertex_program.c" $<TARGET_OBJECTS:vp_reference>)
target_include_directories(vertex_program_test PRIVATE "${NV}")
add_test(NAME vertex_program_operands COMMAND vertex_program_test)
