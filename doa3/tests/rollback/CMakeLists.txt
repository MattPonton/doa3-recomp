cmake_minimum_required(VERSION 3.20)
project(rollback_test C CXX)
enable_testing()
set(GEKKO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/GekkoNet)
file(GLOB GEKKO_SOURCES ${GEKKO_DIR}/src/*.cpp)
add_library(gekkonet STATIC ${GEKKO_SOURCES})
target_include_directories(gekkonet PUBLIC ${GEKKO_DIR}/include
    PRIVATE ${GEKKO_DIR}/include/private ${GEKKO_DIR}/thirdparty)
target_compile_definitions(gekkonet PUBLIC GEKKONET_STATIC GEKKONET_NO_ASIO)
target_compile_features(gekkonet PRIVATE cxx_std_20)
add_executable(rollback_test rollback_test.cpp ../../src/online/rollback.c)
target_include_directories(rollback_test PRIVATE ../../src/online)
target_link_libraries(rollback_test PRIVATE gekkonet)
target_compile_features(rollback_test PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(gekkonet PRIVATE /W0 /EHsc)
    target_compile_options(rollback_test PRIVATE /W4 /WX /EHsc)
endif()
add_test(NAME rollback COMMAND rollback_test)
add_executable(rollback_checkpoint_test checkpoint_test.c ../../src/online/rollback_checkpoint.c)
target_include_directories(rollback_checkpoint_test PRIVATE ../../src/online)
if(MSVC)
    target_compile_options(rollback_checkpoint_test PRIVATE /W4 /WX)
endif()
add_test(NAME rollback_checkpoint COMMAND rollback_checkpoint_test)
set_tests_properties(rollback PROPERTIES TIMEOUT 30)

if(WIN32)
    add_executable(rollback_geometry_cache_test geometry_cache_test.c)
    target_include_directories(rollback_geometry_cache_test PRIVATE ../../src)
    add_test(NAME rollback_geometry_cache COMMAND rollback_geometry_cache_test)
    add_executable(rollback_texture_signature_test texture_signature_test.c)
    target_include_directories(rollback_texture_signature_test PRIVATE ../../src)
    add_test(NAME rollback_texture_signature COMMAND rollback_texture_signature_test)
    add_executable(rollback_checkpoint_bench checkpoint_bench.c ../../src/online/rollback_checkpoint.c)
    target_include_directories(rollback_checkpoint_bench PRIVATE ../../src/online)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_loop.py"
            "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        DEPENDS ../../tools/recomp/rollback_loop.py ../../src/game/recomp/gen/recomp_0002.c VERBATIM)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/loop_native.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_loop_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/loop_native.generated.inc"
        DEPENDS generate_loop_fixture.py ../../tools/recomp/rollback_loop.py
            ../../src/game/recomp/gen/recomp_0002.c VERBATIM)
    add_executable(rollback_loop_test loop_test.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c" "${CMAKE_CURRENT_BINARY_DIR}/loop_native.generated.inc")
    target_include_directories(rollback_loop_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_loop COMMAND rollback_loop_test)
    add_executable(rollback_native_frame_test native_frame_test.c ../../src/game/rollback_native_frame.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c")
    target_include_directories(rollback_native_frame_test PRIVATE ../../src)
    add_test(NAME rollback_native_frame COMMAND rollback_native_frame_test)
    add_test(NAME rollback_guest_render_source_guard COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/guest_render_generator_test.py")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/native_present.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_native_present_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/native_present.generated.inc"
        DEPENDS generate_native_present_fixture.py ../../tools/recomp/rollback_loop_input.py
            ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_executable(rollback_native_present_test native_present_test.c "${CMAKE_CURRENT_BINARY_DIR}/native_present.generated.inc")
    target_include_directories(rollback_native_present_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_native_present COMMAND rollback_native_present_test)
    set_tests_properties(rollback_loop PROPERTIES TIMEOUT 30)
    add_executable(rollback_fight_checkpoint_test fight_checkpoint_test.c
        ../../src/game/rollback_fight_checkpoint.c ../../src/online/rollback_checkpoint.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c")
    target_include_directories(rollback_fight_checkpoint_test PRIVATE ../../src ../../src/game/recomp/gen)
    add_test(NAME rollback_fight_checkpoint COMMAND rollback_fight_checkpoint_test)
    add_test(NAME rollback_fight_checkpoint_fatal COMMAND rollback_fight_checkpoint_test fatal)
    set_tests_properties(rollback_fight_checkpoint PROPERTIES TIMEOUT 60)
    add_executable(rollback_fight_probe_test fight_probe_test.c ../../src/game/rollback_fight_probe.c)
    add_executable(rollback_fight_window_test fight_window_test.c ../../src/game/rollback_fight_window.c)
    target_include_directories(rollback_fight_window_test PRIVATE ../../src ../../src/game/recomp/gen)
    add_test(NAME rollback_fight_window_long-geometry-ab-success COMMAND rollback_fight_window_test long-geometry-ab-success)
    add_test(NAME rollback_fight_window_long-render-ab-success COMMAND rollback_fight_window_test long-render-ab-success)
    add_test(NAME rollback_fight_window_long-prep-ab-success COMMAND rollback_fight_window_test long-prep-ab-success)
    add_test(NAME rollback_fight_window_long-prep-ab-mismatch COMMAND rollback_fight_window_test long-prep-ab-mismatch)
    add_test(NAME rollback_fight_window_long-guest-ab-success COMMAND rollback_fight_window_test long-guest-ab-success)
    add_test(NAME rollback_fight_window_long-guest-ab-mismatch COMMAND rollback_fight_window_test long-guest-ab-mismatch)
    add_test(NAME rollback_fight_window_long-render-ab-mismatch COMMAND rollback_fight_window_test long-render-ab-mismatch)
    add_test(NAME rollback_fight_window_long-geometry-ab-mismatch COMMAND rollback_fight_window_test long-geometry-ab-mismatch)
    foreach(window_mode IN ITEMS enabled disabled conflict input-failure endpoint-failure transition presentation late-worker settle-timeout replay kernel-refusal kernel-invalid combat-success combat-mismatch combat-transition combat-input-failure long-success long-mismatch long-transition long-input-failure long-repeat-success long-repeat-mismatch long-texture-ab-success long-texture-ab-mismatch long-packet-ab-success long-packet-ab-mismatch)
        add_test(NAME rollback_fight_window_${window_mode} COMMAND rollback_fight_window_test ${window_mode})
    endforeach()
    target_include_directories(rollback_fight_probe_test PRIVATE ../../src ../../src/game/recomp/gen)
    foreach(probe_mode IN ITEMS enabled disabled conflict)
        add_test(NAME rollback_fight_probe_${probe_mode} COMMAND rollback_fight_probe_test ${probe_mode})
    endforeach()
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/live_frontend.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_live_frontend_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/live_frontend.generated.inc"
        DEPENDS generate_live_frontend_fixture.py ../../src/nv2a/nv2a_core.c
            ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_executable(rollback_live_frontend_test live_frontend_test.c "${CMAKE_CURRENT_BINARY_DIR}/live_frontend.generated.inc")
    target_include_directories(rollback_live_frontend_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_live_frontend COMMAND rollback_live_frontend_test)
    foreach(kind IN ITEMS production reference)
        if(kind STREQUAL "production")
            set(time_output "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop_time.generated.c")
            set(time_args)
        else()
            set(time_output "${CMAKE_CURRENT_BINARY_DIR}/loop_time_reference.generated.inc")
            set(time_args --fixture)
        endif()
        add_custom_command(OUTPUT "${time_output}"
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_loop_time.py"
                "${time_output}" ${time_args}
            DEPENDS ../../tools/recomp/rollback_loop_time.py ../../tools/recomp/rollback_loop_input.py
                ../../src/game/recomp/gen/recomp_0002.c ../../src/game/recomp/gen/recomp_0006.c
                ../../src/game/recomp/recomp_manual.c VERBATIM)
    endforeach()
    add_executable(rollback_loop_time_test loop_time_test.c ../../src/game/rollback_input_tape.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop_time.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/loop_time_reference.generated.inc")
    target_include_directories(rollback_loop_time_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_loop_time COMMAND rollback_loop_time_test)
    add_test(NAME rollback_loop_time_source_guard COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/loop_time_generator_test.py")
    set_tests_properties(rollback_loop_time PROPERTIES TIMEOUT 45)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/session_codec_types.generated.inc" "${CMAKE_CURRENT_BINARY_DIR}/session_codec.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_session_codec_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/session_codec_types.generated.inc" "${CMAKE_CURRENT_BINARY_DIR}/session_codec.generated.inc"
        DEPENDS generate_session_codec_fixture.py ../../src/online/netplay_session.c VERBATIM)
    add_executable(rollback_session_codec_test session_codec_test.c
        "${CMAKE_CURRENT_BINARY_DIR}/session_codec_types.generated.inc" "${CMAKE_CURRENT_BINARY_DIR}/session_codec.generated.inc")
    target_include_directories(rollback_session_codec_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_session_codec COMMAND rollback_session_codec_test)
    foreach(kind IN ITEMS production reference)
        if(kind STREQUAL "production")
            set(audio_output "${CMAKE_CURRENT_BINARY_DIR}/rollback_audio_service.generated.c")
            set(audio_args)
        else()
            set(audio_output "${CMAKE_CURRENT_BINARY_DIR}/audio_service_reference.generated.inc")
            set(audio_args --fixture)
        endif()
        add_custom_command(OUTPUT "${audio_output}"
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_audio_service.py"
                "${audio_output}" ${audio_args}
            DEPENDS ../../tools/recomp/rollback_audio_service.py ../../tools/recomp/rollback_loop_input.py
                ../../tools/recomp/rollback_audio_sync.py ../../src/kernel/kernel_bridge.c
                ../../src/kernel/kernel_hal.c ../../src/kernel/kernel_rtl.c
                ../../src/game/recomp/gen/recomp_0010.c ../../src/game/rollback_audio_service.inc VERBATIM)
    endforeach()
    add_executable(rollback_audio_service_test audio_service_test.c ../../src/game/rollback_input_tape.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_audio_service.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/audio_service_reference.generated.inc")
    target_include_directories(rollback_audio_service_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_audio_service COMMAND rollback_audio_service_test)
    add_test(NAME rollback_audio_service_source_guard COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/audio_service_generator_test.py")
    set_tests_properties(rollback_audio_service PROPERTIES TIMEOUT 45)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/audio_sync_endpoints.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_audio_sync.py"
            "${CMAKE_CURRENT_BINARY_DIR}/audio_sync_endpoints.generated.inc"
        DEPENDS ../../tools/recomp/rollback_audio_sync.py ../../src/kernel/kernel_bridge.c
            ../../src/kernel/kernel_hal.c ../../src/kernel/kernel_rtl.c VERBATIM)
    add_executable(rollback_audio_sync_test audio_sync_test.c ../../src/game/rollback_input_tape.c
        ../../src/kernel/kernel_replay.c ../../src/online/xbox_det.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_audio_service.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/audio_service_reference.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/audio_sync_endpoints.generated.inc")
    target_include_directories(rollback_audio_sync_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_audio_sync COMMAND rollback_audio_sync_test)
    set_tests_properties(rollback_audio_sync PROPERTIES TIMEOUT 180)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/pipeline.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_pipeline_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/pipeline.generated.inc"
        DEPENDS generate_pipeline_fixture.py ../../src/d3d/d3d8_device.c VERBATIM)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/texture_restore.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_texture_restore_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/texture_restore.generated.inc"
        DEPENDS generate_texture_restore_fixture.py ../../src/nv2a/nv2a_pgraph_d3d11.c VERBATIM)
    add_executable(rollback_texture_restore_test texture_restore_test.c
        "${CMAKE_CURRENT_BINARY_DIR}/texture_restore.generated.inc")
    target_include_directories(rollback_texture_restore_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_texture_restore COMMAND rollback_texture_restore_test)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/render_images.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_render_images_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/render_images.generated.inc"
        DEPENDS generate_render_images_fixture.py ../../src/d3d/d3d8_device.c VERBATIM)
    add_executable(rollback_render_images_test render_images_test.c
        ../../src/d3d/d3d8_replay_images.c "${CMAKE_CURRENT_BINARY_DIR}/render_images.generated.inc")
    target_include_directories(rollback_render_images_test PRIVATE ../../src/d3d "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(rollback_render_images_test PRIVATE d3d11 dxguid)
    add_test(NAME rollback_render_images COMMAND rollback_render_images_test)
    add_executable(rollback_renderer_transaction_test renderer_transaction_test.c
        ../../src/online/rollback_checkpoint.c ../../src/nv2a/nv2a_pgraph_state.c
        ../../src/nv2a/nv2a_renderer_checkpoint.c ../../src/d3d/d3d8_replay_images.c
        ../../src/d3d/d3d8_resources.c "${CMAKE_CURRENT_BINARY_DIR}/render_images.generated.inc"
        ../../src/d3d/d3d8_shaders.c ../../src/d3d/d3d8_states.c
        ../../src/d3d/d3d8_combiners.c ../../src/d3d/d3d8_vsh.c
        "${CMAKE_CURRENT_BINARY_DIR}/pipeline.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/texture_restore.generated.inc")
    target_include_directories(rollback_renderer_transaction_test PRIVATE ../../src ../../src/d3d "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(rollback_renderer_transaction_test PRIVATE d3d11 d3dcompiler dxguid)
    add_test(NAME rollback_renderer_transaction COMMAND rollback_renderer_transaction_test)
    add_executable(rollback_pgraph_draw_replay_test pgraph_draw_replay_test.c
        ../../src/nv2a/nv2a_renderer_checkpoint.c ../../src/nv2a/nv2a_pgraph_state.c
        ../../src/nv2a/nv2a_vertex_program.c ../../src/d3d/d3d8_replay_images.c
        ../../src/d3d/d3d8_resources.c ../../src/d3d/d3d8_shaders.c
        ../../src/d3d/d3d8_states.c ../../src/d3d/d3d8_combiners.c ../../src/d3d/d3d8_vsh.c
        "${CMAKE_CURRENT_BINARY_DIR}/pipeline.generated.inc")
    target_include_directories(rollback_pgraph_draw_replay_test PRIVATE ../../src ../../src/d3d "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(rollback_pgraph_draw_replay_test PRIVATE d3d11 d3dcompiler dxguid)
    if(MSVC)
        target_compile_options(rollback_pgraph_draw_replay_test PRIVATE /wd4005)
    endif()
    add_test(NAME rollback_pgraph_draw_replay COMMAND rollback_pgraph_draw_replay_test)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/pgraph.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_pgraph_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/pgraph.generated.inc"
        DEPENDS generate_pgraph_fixture.py ../../src/nv2a/nv2a_pgraph_d3d11.c VERBATIM)
    add_executable(rollback_pgraph_state_test pgraph_state_test.c
        ../../src/nv2a/nv2a_pgraph_state.c ../../src/nv2a/nv2a_vertex_program.c
        "${CMAKE_CURRENT_BINARY_DIR}/pgraph.generated.inc")
    target_include_directories(rollback_pgraph_state_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(rollback_pgraph_state_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        target_compile_options(rollback_pgraph_state_test PRIVATE /wd4005)
    endif()
    add_test(NAME rollback_pgraph_state COMMAND rollback_pgraph_state_test)
    add_executable(rollback_gpu_frontend_test gpu_frontend_test.c)
    target_include_directories(rollback_gpu_frontend_test PRIVATE ../../src/nv2a)
    if(MSVC)
        target_compile_options(rollback_gpu_frontend_test PRIVATE /W4 /WX /wd4505)
    endif()
    add_test(NAME rollback_gpu_frontend COMMAND rollback_gpu_frontend_test)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/interrupt.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_interrupt_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/interrupt.generated.inc"
        DEPENDS generate_interrupt_fixture.py ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_executable(rollback_interrupt_test interrupt_test.c ../../src/kernel/kernel_interrupt.c
        "${CMAKE_CURRENT_BINARY_DIR}/interrupt.generated.inc")
    target_include_directories(rollback_interrupt_test PRIVATE ../../src/kernel ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    if(MSVC)
        target_compile_options(rollback_interrupt_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME rollback_interrupt COMMAND rollback_interrupt_test)
    add_executable(rollback_heap_test heap_test.c)
    target_include_directories(rollback_heap_test PRIVATE ../../src/kernel ../../src/game/recomp/gen)
    if(MSVC)
        target_compile_options(rollback_heap_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME rollback_heap COMMAND rollback_heap_test)
    add_executable(rollback_state_audit_test state_audit_test.c ../../src/game/rollback_state_audit.c
        ../../src/game/rollback_input.c
        ../../src/kernel/xbox_heap.c ../../src/kernel/kernel_stream.c
        ../../src/kernel/kernel_apc.c ../../src/kernel/kernel_interrupt.c)
    target_include_directories(rollback_state_audit_test PRIVATE ../../src ../../src/game/recomp/gen)
    target_compile_definitions(rollback_state_audit_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    add_test(NAME rollback_state_audit COMMAND rollback_state_audit_test)
    set_tests_properties(rollback_state_audit PROPERTIES TIMEOUT 60)
    add_executable(rollback_input_test input_test.c ../../src/game/rollback_input.c)
    target_include_directories(rollback_input_test PRIVATE ../../src)
    add_test(NAME rollback_input COMMAND rollback_input_test)
    add_executable(rollback_input_tape_test input_tape_test.c ../../src/game/rollback_input_tape.c)
    target_include_directories(rollback_input_tape_test PRIVATE ../../src)
    add_test(NAME rollback_input_tape COMMAND rollback_input_tape_test)
    add_executable(rollback_trial_test trial_test.c ../../src/game/rollback_trial.c
        ../../src/kernel/kernel_replay.c
        ../../src/game/rollback_input_tape.c ../../src/online/rollback_checkpoint.c)
    target_include_directories(rollback_trial_test PRIVATE ../../src)
    if(MSVC)
        target_compile_options(rollback_trial_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME rollback_trial COMMAND rollback_trial_test)
    add_test(NAME rollback_trial_poisoned_checkpoint COMMAND rollback_trial_test poison)
    foreach(fault RANGE 12 16)
        add_test(NAME rollback_trial_fatal_${fault} COMMAND rollback_trial_test ${fault})
    endforeach()
    add_executable(rollback_input_audit_test input_audit_test.c ../../src/game/rollback_input_audit.c
        ../../src/game/rollback_input_tape.c)
    target_include_directories(rollback_input_audit_test PRIVATE ../../src ../../src/game/recomp/gen)
    target_compile_definitions(rollback_input_audit_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    add_test(NAME rollback_input_audit COMMAND rollback_input_audit_test complete)
    set_tests_properties(rollback_input_audit PROPERTIES PASS_REGULAR_EXPRESSION "complete: frames=600 events=2400 state_reads=2400 ports=F rumble_calls=0 failed=0")
    add_test(NAME rollback_input_audit_exit COMMAND rollback_input_audit_test leave)
    set_tests_properties(rollback_input_audit_exit PROPERTIES PASS_REGULAR_EXPRESSION "stopped: left eligible Practice: frames=5")
    add_test(NAME rollback_input_audit_conflict COMMAND rollback_input_audit_test conflict)
    set_tests_properties(rollback_input_audit_conflict PROPERTIES PASS_REGULAR_EXPRESSION "refused: another audit is armed")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_input_bridge_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc"
        DEPENDS generate_input_bridge_fixture.py ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_executable(rollback_input_bridge_test input_bridge_test.c ../../src/game/rollback_input.c
        ../../src/game/rollback_input_tape.c
        "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc")
    target_include_directories(rollback_input_bridge_test PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_input_bridge COMMAND rollback_input_bridge_test)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop_input.generated.c"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_loop_input.py"
            "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop_input.generated.c"
        DEPENDS ../../tools/recomp/rollback_loop_input.py ../../src/game/recomp/gen/recomp_0002.c
            ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/loop_input_reference.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/recomp/rollback_loop_input.py"
            "${CMAKE_CURRENT_BINARY_DIR}/loop_input_reference.generated.inc" --fixture
        DEPENDS ../../tools/recomp/rollback_loop_input.py ../../src/game/recomp/gen/recomp_0002.c
            ../../src/game/recomp/recomp_manual.c VERBATIM)
    add_executable(rollback_loop_input_test loop_input_test.c
        ../../src/game/rollback_input.c ../../src/game/rollback_input_tape.c
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/rollback_loop_input.generated.c"
        "${CMAKE_CURRENT_BINARY_DIR}/loop_input_reference.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc")
    target_include_directories(rollback_loop_input_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_loop_input COMMAND rollback_loop_input_test)
    add_test(NAME rollback_loop_input_source_guard COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/loop_input_generator_test.py")
    set_tests_properties(rollback_loop_input PROPERTIES TIMEOUT 30)
    add_executable(rollback_stream_files_test stream_files_test.c ../../src/kernel/kernel_stream.c ../../src/kernel/kernel_apc.c)
    target_include_directories(rollback_stream_files_test PRIVATE ../../src/kernel)
    target_compile_definitions(rollback_stream_files_test PRIVATE XBOX_STREAM_TEST_SEEK)
    if(MSVC)
        target_compile_options(rollback_stream_files_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME rollback_stream_files COMMAND rollback_stream_files_test)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stream_bridge.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_stream_bridge_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/stream_bridge.generated.inc"
        DEPENDS generate_stream_bridge_fixture.py ../../src/kernel/kernel_bridge.c VERBATIM)
    add_executable(rollback_stream_bridge_test stream_bridge_test.c ../../src/kernel/kernel_stream.c
        "${CMAKE_CURRENT_BINARY_DIR}/stream_bridge.generated.inc")
    target_include_directories(rollback_stream_bridge_test PRIVATE ../../src/kernel "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(rollback_stream_bridge_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    add_test(NAME rollback_stream_bridge COMMAND rollback_stream_bridge_test)
    add_executable(rollback_apc_test apc_test.c ../../src/kernel/kernel_apc.c)
    target_include_directories(rollback_apc_test PRIVATE ../../src/kernel)
    if(MSVC)
        target_compile_options(rollback_apc_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME rollback_apc COMMAND rollback_apc_test)
    add_executable(rollback_targets_test targets_test.c)
    target_include_directories(rollback_targets_test PRIVATE ../../src/nv2a)
    add_test(NAME rollback_targets COMMAND rollback_targets_test)
    add_executable(rollback_submission_test submission_test.c)
    target_include_directories(rollback_submission_test PRIVATE ../../src/nv2a)
    add_test(NAME rollback_submission COMMAND rollback_submission_test)
    add_library(apu_command_fixture STATIC ../../src/apu/apu_rollback.c ../../src/apu/apu_vp.c)
    target_include_directories(apu_command_fixture PUBLIC ../../src/apu ../../src/nv2a)
    target_compile_definitions(apu_command_fixture PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    if(MSVC)
        target_compile_options(apu_command_fixture PRIVATE /Gy)
    endif()
    add_executable(apu_snapshot_test apu_snapshot_test.c)
    target_link_libraries(apu_snapshot_test PRIVATE apu_command_fixture)
    target_compile_definitions(apu_snapshot_test PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_apu_snapshot COMMAND apu_snapshot_test)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/apu_live.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_apu_live_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/apu_live.generated.inc"
        DEPENDS generate_apu_live_fixture.py ../../src/apu/apu_core.c VERBATIM)
    add_executable(apu_live_test apu_live_test.c ../../src/apu/apu_dsp.c
        "${CMAKE_CURRENT_BINARY_DIR}/apu_live.generated.inc")
    target_include_directories(apu_live_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(apu_live_test PRIVATE apu_command_fixture winmm)
    target_compile_definitions(apu_live_test PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_apu_live_processing COMMAND apu_live_test)
    set_tests_properties(rollback_apu_live_processing PROPERTIES TIMEOUT 30)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/apu_ownership.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_apu_ownership_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/apu_ownership.generated.inc"
        DEPENDS generate_apu_ownership_fixture.py ../../src/apu/apu_core.c VERBATIM)
    add_executable(apu_ownership_test apu_ownership_test.c ../../src/online/xbox_det.c
        ../../src/game/rollback_ownership.c "${CMAKE_CURRENT_BINARY_DIR}/apu_ownership.generated.inc")
    target_include_directories(apu_ownership_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(apu_ownership_test PRIVATE apu_command_fixture)
    target_compile_definitions(apu_ownership_test PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_apu_ownership COMMAND apu_ownership_test)
    add_test(NAME rollback_apu_processing_audit COMMAND apu_ownership_test processing)
    set_tests_properties(rollback_apu_processing_audit PROPERTIES TIMEOUT 20
        PASS_REGULAR_EXPRESSION "ROLLBACK-AUDIO-PROCESSING.*passed")
    set_tests_properties(rollback_apu_ownership PROPERTIES TIMEOUT 20)
    add_test(NAME rollback_render_capture_ownership COMMAND apu_ownership_test renderer)
    add_test(NAME rollback_render_capture_mismatch COMMAND apu_ownership_test renderer-mismatch)
    set_tests_properties(rollback_render_capture_ownership PROPERTIES TIMEOUT 20
        PASS_REGULAR_EXPRESSION "ROLLBACK-OWNERSHIP.*passed")
    set_tests_properties(rollback_render_capture_mismatch PROPERTIES TIMEOUT 20
        PASS_REGULAR_EXPRESSION "ROLLBACK-OWNERSHIP.*rejected")
    add_executable(apu_replay_test apu_replay_test.c ../../src/apu/apu_replay.c ../../src/apu/apu_dsp.c)
    target_link_libraries(apu_replay_test PRIVATE apu_command_fixture)
    target_compile_definitions(apu_replay_test PRIVATE DOA3_APU_OFFLINE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_apu_replay COMMAND apu_replay_test)
    set_tests_properties(rollback_apu_replay PROPERTIES TIMEOUT 60)
    add_executable(rollback_checkpoint_integration_test checkpoint_integration_test.c
        ../../src/game/rollback_input.c ../../src/game/rollback_input_tape.c
        "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc"
        ../../src/online/rollback_checkpoint.c ../../src/online/xbox_det.c
        ../../src/kernel/xbox_fiber.c ../../src/kernel/kernel_apc.c ../../src/kernel/kernel_stream.c ../../src/kernel/xbox_heap.c ../../src/kernel/kernel_interrupt.c
        ../../src/apu/apu_replay.c ../../src/apu/apu_dsp.c ../../src/nv2a/nv2a_frontend.c)
    target_link_libraries(rollback_checkpoint_integration_test PRIVATE apu_command_fixture)
    target_include_directories(rollback_checkpoint_integration_test PRIVATE ../../src ../../src/online ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(rollback_checkpoint_integration_test PRIVATE DOA3_APU_OFFLINE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_checkpoint_integration COMMAND rollback_checkpoint_integration_test)
    set_tests_properties(rollback_checkpoint_integration PROPERTIES TIMEOUT 90)
    add_executable(rollback_checkpoint_live_audio_test checkpoint_integration_test.c checkpoint_live_audio.c
        ../../src/game/rollback_trial.c
        ../../src/kernel/kernel_replay.c
        "${CMAKE_CURRENT_BINARY_DIR}/apu_live.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc"
        ../../src/game/rollback_input.c ../../src/game/rollback_input_tape.c
        ../../src/online/rollback_checkpoint.c ../../src/online/xbox_det.c
        ../../src/kernel/xbox_fiber.c ../../src/kernel/kernel_apc.c ../../src/kernel/kernel_stream.c ../../src/kernel/xbox_heap.c ../../src/kernel/kernel_interrupt.c
        ../../src/apu/apu_dsp.c ../../src/nv2a/nv2a_frontend.c)
    target_link_libraries(rollback_checkpoint_live_audio_test PRIVATE apu_command_fixture winmm)
    target_include_directories(rollback_checkpoint_live_audio_test PRIVATE ../../src ../../src/online ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(rollback_checkpoint_live_audio_test PRIVATE DOA3_CP_LIVE_AUDIO _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_checkpoint_live_audio COMMAND rollback_checkpoint_live_audio_test)
    set_tests_properties(rollback_checkpoint_live_audio PROPERTIES TIMEOUT 120)
    add_executable(rollback_checkpoint_render_audio_test checkpoint_integration_test.c checkpoint_live_audio.c checkpoint_renderer.c
        ../../src/nv2a/nv2a_pgraph_state.c ../../src/nv2a/nv2a_renderer_checkpoint.c
        ../../src/d3d/d3d8_replay_images.c ../../src/d3d/d3d8_resources.c
        ../../src/d3d/d3d8_shaders.c ../../src/d3d/d3d8_states.c
        ../../src/d3d/d3d8_combiners.c ../../src/d3d/d3d8_vsh.c
        "${CMAKE_CURRENT_BINARY_DIR}/pipeline.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/texture_restore.generated.inc"
        ../../src/game/rollback_trial.c
        ../../src/kernel/kernel_replay.c
        "${CMAKE_CURRENT_BINARY_DIR}/apu_live.generated.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/input_bridge.generated.inc"
        ../../src/game/rollback_input.c ../../src/game/rollback_input_tape.c
        ../../src/online/rollback_checkpoint.c ../../src/online/xbox_det.c
        ../../src/kernel/xbox_fiber.c ../../src/kernel/kernel_apc.c ../../src/kernel/kernel_stream.c ../../src/kernel/xbox_heap.c ../../src/kernel/kernel_interrupt.c
        ../../src/apu/apu_dsp.c ../../src/nv2a/nv2a_frontend.c)
    target_link_libraries(rollback_checkpoint_render_audio_test PRIVATE apu_command_fixture winmm d3d11 d3dcompiler dxguid)
    target_include_directories(rollback_checkpoint_render_audio_test PRIVATE ../../src ../../src/d3d ../../src/online ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(rollback_checkpoint_render_audio_test PRIVATE DOA3_CP_LIVE_AUDIO DOA3_CP_RENDERER _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    add_test(NAME rollback_checkpoint_render_audio COMMAND rollback_checkpoint_render_audio_test)
    set_tests_properties(rollback_checkpoint_render_audio PROPERTIES TIMEOUT 180)
    add_executable(rollback_capture_test rollback_capture_test.c ../../src/game/rollback_capture.c)
    target_include_directories(rollback_capture_test PRIVATE ../../src ../../src/online
        ../../src/game ../../src/game/recomp/gen)
    target_compile_definitions(rollback_capture_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    add_test(NAME rollback_capture COMMAND rollback_capture_test "${CMAKE_CURRENT_BINARY_DIR}/capture-test.rbc")
    set_tests_properties(rollback_capture PROPERTIES TIMEOUT 60)
    add_test(NAME rollback_capture_contiguous COMMAND rollback_capture_test "${CMAKE_CURRENT_BINARY_DIR}/capture-contiguous-test.rbc" contiguous)
    set_tests_properties(rollback_capture_contiguous PROPERTIES TIMEOUT 60)
    add_test(NAME rollback_capture_apu COMMAND rollback_capture_test "${CMAKE_CURRENT_BINARY_DIR}/capture-apu-test.rbc" apu)
    set_tests_properties(rollback_capture_apu PROPERTIES TIMEOUT 60)
    add_test(NAME rollback_capture_gpu COMMAND rollback_capture_test "${CMAKE_CURRENT_BINARY_DIR}/capture-gpu-test.rbc" gpu)
    set_tests_properties(rollback_capture_gpu PROPERTIES TIMEOUT 60)
    add_test(NAME rollback_capture_cancel COMMAND rollback_capture_test "${CMAKE_CURRENT_BINARY_DIR}/capture-cancel-test.rbc" cancel)
    set_tests_properties(rollback_capture_cancel PROPERTIES TIMEOUT 60)
    add_executable(rollback_fiber_test rollback_fiber_test.c ../../src/kernel/xbox_fiber.c)
    target_include_directories(rollback_fiber_test PRIVATE ../../src ../../src/online
        ../../src/game/recomp/gen)
    add_test(NAME rollback_fiber COMMAND rollback_fiber_test)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/workers.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_worker_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/workers.generated.inc"
        DEPENDS generate_worker_fixture.py ../../tools/recomp/rollback_workers.py ../../tools/recomp/rollback_tasks.py
            ../../src/game/recomp/gen/recomp_0007.c ../../src/game/recomp/gen/recomp_0009.c VERBATIM)
    add_executable(rollback_worker_test worker_test.c ../../src/game/rollback_workers.c "${CMAKE_CURRENT_BINARY_DIR}/workers.generated.inc")
    target_include_directories(rollback_worker_test PRIVATE ../../src ../../src/game/recomp ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_workers COMMAND rollback_worker_test)
    set_tests_properties(rollback_workers PROPERTIES TIMEOUT 90)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/nested.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_nested_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/nested.generated.inc"
        DEPENDS generate_nested_fixture.py ../../tools/recomp/rollback_nested.py
            ../../src/game/recomp/gen/recomp_0001.c ../../src/game/recomp/gen/recomp_0002.c VERBATIM)
    add_executable(nested_test nested_test.c ../../src/online/rollback_nested.c
        "${CMAKE_CURRENT_BINARY_DIR}/nested.generated.inc")
    target_include_directories(nested_test PRIVATE ../../src/game/recomp ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_nested COMMAND nested_test)
    set_tests_properties(rollback_nested PROPERTIES TIMEOUT 90)
    file(GLOB FLOW_GRAPH_INPUTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../../src/game/recomp/gen/recomp_*.c")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/flow.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_flow_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/flow.generated.inc"
        DEPENDS generate_flow_fixture.py ../../tools/recomp/rollback_nested.py
            ../../tools/recomp/rollback_match.py ../../src/game/recomp/recomp_manual.c
            ${FLOW_GRAPH_INPUTS} VERBATIM)
    add_executable(flow_test flow_test.c ../../src/online/rollback_nested.c
        "${CMAKE_CURRENT_BINARY_DIR}/flow.generated.inc")
    target_include_directories(flow_test PRIVATE ../../src/game/recomp ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_flow COMMAND flow_test)
    set_tests_properties(rollback_flow PROPERTIES TIMEOUT 90)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/display.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_display_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/display.generated.inc"
        DEPENDS generate_display_fixture.py ../../tools/recomp/rollback_tasks.py
            ../../tools/recomp/rollback_match.py ../../src/game/recomp/recomp_manual.c
            ${FLOW_GRAPH_INPUTS} VERBATIM)
    add_executable(display_test display_test.c "${CMAKE_CURRENT_BINARY_DIR}/display.generated.inc")
    target_include_directories(display_test PRIVATE ../../src/game/recomp ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_display COMMAND display_test)
    set_tests_properties(rollback_display PROPERTIES TIMEOUT 90)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/added_tasks.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_added_tasks_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/added_tasks.generated.inc"
        DEPENDS generate_added_tasks_fixture.py ../../tools/recomp/rollback_tasks.py
            ../../src/game/recomp/gen/recomp_0001.c ../../src/game/recomp/gen/recomp_0003.c
            ../../src/game/recomp/gen/recomp_0009.c VERBATIM)
    add_executable(added_tasks_test added_tasks_test.c "${CMAKE_CURRENT_BINARY_DIR}/added_tasks.generated.inc")
    target_include_directories(added_tasks_test PRIVATE ../../src/game/recomp ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_added_tasks COMMAND added_tasks_test)
    set_tests_properties(rollback_added_tasks PROPERTIES TIMEOUT 90)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/frame_fixture.c"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_frame_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/frame_fixture.c"
        DEPENDS generate_frame_fixture.py ../../src/game/recomp/gen/recomp_0006.c
            ../../tools/recomp/init_frame_pointer.py ../../tools/recomp/rollback_tasks.py VERBATIM)
    add_custom_target(frame_fixture_gen DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/frame_fixture.c")
    foreach(variant IN ITEMS fast slow)
        add_library(frame_${variant} OBJECT "${CMAKE_CURRENT_BINARY_DIR}/frame_fixture.c")
        add_dependencies(frame_${variant} frame_fixture_gen)
        target_include_directories(frame_${variant} PRIVATE ../../src/game/recomp)
        target_compile_definitions(frame_${variant} PRIVATE RECOMP_GENERATED_CODE sub_0017C600_gen=frame_${variant})
    endforeach()
    if(MSVC)
        target_compile_options(frame_fast PRIVATE /O2)
        target_compile_options(frame_slow PRIVATE /Od)
    endif()
    add_executable(frame_pointer_test frame_pointer_test.c $<TARGET_OBJECTS:frame_fast> $<TARGET_OBJECTS:frame_slow>)
    target_include_directories(frame_pointer_test PRIVATE ../../src/game/recomp)
    add_test(NAME rollback_frame_pointer COMMAND frame_pointer_test)
    add_executable(rollback_frame_test rollback_frame_test.c ../../src/game/rollback_frame.c)
    target_include_directories(rollback_frame_test PRIVATE ../../src ../../src/online
        ../../src/game ../../src/game/recomp/gen)
    target_compile_definitions(rollback_frame_test PRIVATE _CRT_SECURE_NO_WARNINGS)
    add_test(NAME rollback_frame COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/rollback_frame_test.py" "$<TARGET_FILE:rollback_frame_test>")
    set_tests_properties(rollback_frame PROPERTIES TIMEOUT 120)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/task_fixture.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_task_fixture.py"
                "${CMAKE_CURRENT_BINARY_DIR}/task_fixture.generated.inc"
        DEPENDS generate_task_fixture.py task_fixture.inc ../../tools/recomp/rollback_tasks.py
            ../../tools/recomp/rollback_match.py ../../tools/recomp/rollback_nested.py
            ../../tools/recomp/overrides_3.1.txt ../../src/game/recomp/recomp_manual.c ${FLOW_GRAPH_INPUTS}
            ../../src/online/rollback_tasks.h
        VERBATIM)
    add_executable(rollback_task_test rollback_task_test.c
        "${CMAKE_CURRENT_BINARY_DIR}/task_fixture.generated.inc")
    target_include_directories(rollback_task_test PRIVATE ../../src/online "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_task COMMAND rollback_task_test)
    add_executable(rollback_clock_test rollback_clock_test.c ../../src/online/xbox_det.c)
    target_include_directories(rollback_clock_test PRIVATE ../../src ../../src/online
        ../../src/game/recomp/gen)
    if(MSVC)
        target_compile_options(rollback_clock_test PRIVATE /W3 /WX)
    endif()
    add_test(NAME rollback_clock COMMAND rollback_clock_test)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/audio_wait.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_audio_wait_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/audio_wait.generated.inc"
        DEPENDS generate_audio_wait_fixture.py ../../tools/recomp/rollback_boundaries.py
            ../../src/game/recomp/gen/recomp_0007.c ../../src/game/recomp/gen/recomp_0006.c VERBATIM)
    add_executable(rollback_audio_wait_test audio_wait_test.c ../../src/online/xbox_det.c
        "${CMAKE_CURRENT_BINARY_DIR}/audio_wait.generated.inc")
    target_include_directories(rollback_audio_wait_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_audio_wait COMMAND rollback_audio_wait_test)
    set_tests_properties(rollback_audio_wait PROPERTIES TIMEOUT 10)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/qpc_bridge.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_qpc_bridge_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/qpc_bridge.generated.inc"
        DEPENDS generate_qpc_bridge_fixture.py ../../src/kernel/kernel_bridge.c VERBATIM)
    add_executable(rollback_qpc_bridge_test qpc_bridge_test.c ../../src/online/xbox_det.c
        "${CMAKE_CURRENT_BINARY_DIR}/qpc_bridge.generated.inc")
    target_include_directories(rollback_qpc_bridge_test PRIVATE ../../src ../../src/online ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_qpc_bridge COMMAND rollback_qpc_bridge_test)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kernel_dispatch.generated.inc"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_kernel_dispatch_fixture.py"
            "${CMAKE_CURRENT_BINARY_DIR}/kernel_dispatch.generated.inc"
        DEPENDS generate_kernel_dispatch_fixture.py ../../src/kernel/kernel_bridge.c
            ../../src/game/recomp/gen/recomp_0010.c VERBATIM)
    add_executable(rollback_kernel_replay_test kernel_replay_test.c ../../src/kernel/kernel_replay.c
        ../../src/online/xbox_det.c "${CMAKE_CURRENT_BINARY_DIR}/kernel_dispatch.generated.inc")
    target_include_directories(rollback_kernel_replay_test PRIVATE ../../src ../../src/game/recomp/gen "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME rollback_kernel_replay COMMAND rollback_kernel_replay_test)
    add_test(NAME rollback_kernel_fail_stop COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/kernel_fail_stop_test.py" "$<TARGET_FILE:rollback_kernel_replay_test>")
endif()
