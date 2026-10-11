cmake_minimum_required(VERSION 3.20)
project(direct_afs_test C)
enable_testing()
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(GAME "${CMAKE_CURRENT_SOURCE_DIR}/../../src/game")
execute_process(COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/extract.py"
    "${GAME}/recomp/gen" "${CMAKE_CURRENT_BINARY_DIR}/guest.h" RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Direct AFS extraction failed")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${GAME}/recomp/gen/recomp_0002.c" "${GAME}/recomp/gen/recomp_0009.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/extract.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/direct_afs.py")
add_executable(direct_afs_test direct_afs_test.c)
target_include_directories(direct_afs_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${GAME}")
add_test(NAME direct_afs COMMAND direct_afs_test)
