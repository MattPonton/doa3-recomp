cmake_minimum_required(VERSION 3.20)
project(save_compat_test C)
enable_testing()
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/fixtures.py"
    "${CMAKE_CURRENT_BINARY_DIR}/fixtures.h" RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Save compatibility fixture/patch check failed")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    fixtures.py ../../tools/recomp/save_compat.py ../../src/game/recomp/gen/recomp_0000.c)
add_executable(save_compat_test save_compat_test.c ../../src/game/save_compat.c ../../src/kernel/kernel_crypto.c)
target_include_directories(save_compat_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    ../../src/game ../../src/kernel ../../src/game/recomp/gen)
target_compile_definitions(save_compat_test PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(save_compat_test PRIVATE bcrypt)
add_test(NAME save_compat COMMAND save_compat_test)
