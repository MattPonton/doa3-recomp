cmake_minimum_required(VERSION 3.20)
project(texture_upload_test C)
enable_testing()
set(D3D "${CMAKE_CURRENT_SOURCE_DIR}/../../src/d3d")
add_executable(texture_upload_test texture_upload_test.c
    "${D3D}/d3d8_resources.c")
target_include_directories(texture_upload_test PRIVATE "${D3D}")
target_link_libraries(texture_upload_test PRIVATE d3d11 dxguid)
add_test(NAME texture_upload_warp COMMAND texture_upload_test)
