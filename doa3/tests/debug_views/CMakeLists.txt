cmake_minimum_required(VERSION 3.20)
project(debug_views_test C)
enable_testing()
add_executable(debug_views_test debug_views_test.c ../../src/d3d/d3d8_debug_views.c)
target_include_directories(debug_views_test PRIVATE ../../src/d3d)
target_link_libraries(debug_views_test PRIVATE d3d11 d3dcompiler)
add_test(NAME debug_views COMMAND debug_views_test)
add_executable(screenshot_test screenshot_test.c ../../src/d3d/d3d8_screenshot.c)
target_include_directories(screenshot_test PRIVATE ../../src/d3d)
target_link_libraries(screenshot_test PRIVATE d3d11)
add_test(NAME screenshot COMMAND screenshot_test)

# Explicit manual benchmark; never part of CTest.
add_executable(performance_benchmark EXCLUDE_FROM_ALL performance_benchmark.c
    ../../src/d3d/d3d8_states.c ../../src/d3d/d3d8_debug_views.c)
target_include_directories(performance_benchmark PRIVATE ../../src/d3d)
target_link_libraries(performance_benchmark PRIVATE d3d11 d3dcompiler)
