cmake_minimum_required(VERSION 3.20)
project(debug_settings_test C)
enable_testing()
add_executable(debug_settings_test debug_settings_test.c ../../src/game/debug_settings.c ../../src/game/dip_switches.c)
target_include_directories(debug_settings_test PRIVATE ../../src/game ../../src)
add_test(NAME debug_settings COMMAND debug_settings_test)

target_sources(debug_settings_test PRIVATE ../../src/perf_capture.c)
