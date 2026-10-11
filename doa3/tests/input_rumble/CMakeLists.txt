cmake_minimum_required(VERSION 3.20)
project(input_rumble_test C)
enable_testing()
add_executable(input_rumble_test input_rumble_test.c input_device_test.c)
target_include_directories(input_rumble_test PRIVATE ../../src/input)
add_test(NAME input_rumble COMMAND input_rumble_test)
add_executable(player_mapping_test player_mapping_test.c input_device_test.c player_mapping_device.c)
target_include_directories(player_mapping_test PRIVATE ../../src/input)
target_link_libraries(player_mapping_test PRIVATE user32)
add_test(NAME player_mapping COMMAND player_mapping_test)

# Explicit manual check: briefly pulses each motor on the first real pad.
# Never run by CTest.
add_executable(rumble_controller_check EXCLUDE_FROM_ALL
    rumble_controller_check.c ../../src/input/xinput_device.c ../../src/input/pad_mapping.c)
target_include_directories(rumble_controller_check PRIVATE ../../src/input)
target_link_libraries(rumble_controller_check PRIVATE xinput user32)
