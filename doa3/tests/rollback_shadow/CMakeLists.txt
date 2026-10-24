cmake_minimum_required(VERSION 3.20)
project(rollback_shadow C)
enable_testing()
find_package(Python3 COMPONENTS Interpreter REQUIRED)
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
file(GLOB INPUTS CONFIGURE_DEPENDS "${ROOT}/src/game/recomp/gen/recomp_*.c")
file(GLOB HOST_MODEL CONFIGURE_DEPENDS "${ROOT}/src/apu/*.[ch]")
list(APPEND HOST_MODEL "${ROOT}/src/apu/apu_live_checkpoint.inc")
list(APPEND HOST_MODEL "${ROOT}/src/kernel/kernel_rtl.c" "${ROOT}/src/kernel/kernel_hal.c"
    "${ROOT}/src/nv2a/nv2a_queue.h" "${ROOT}/src/nv2a/nv2a_core.c"
    "${ROOT}/src/nv2a/nv2a_submission.h"
    "${ROOT}/src/nv2a/nv2a_targets.h"
    "${ROOT}/src/nv2a/nv2a_pfifo_registers.h" "${ROOT}/src/nv2a/nv2a_regs.h"
    "${ROOT}/src/nv2a/nv2a_mmio_hook.c" "${ROOT}/src/nv2a/nv2a_mmio_hook.h"
    "${ROOT}/src/kernel/kernel_bridge.c" "${ROOT}/src/online/rollback_capture.h"
    "${ROOT}/src/kernel/xbox_fiber.c" "${ROOT}/src/kernel/xbox_fiber.h"
    "${ROOT}/src/kernel/kernel_apc.c" "${ROOT}/src/kernel/kernel_apc.h"
    "${ROOT}/src/kernel/kernel_stream.c" "${ROOT}/src/kernel/kernel_stream.h"
    "${ROOT}/src/game/rollback_workers.c" "${ROOT}/src/online/rollback_workers.h"
    "${ROOT}/src/nv2a/nv2a_frontend.c"
    "${ROOT}/src/nv2a/nv2a_frontend.h"
    "${ROOT}/src/nv2a/nv2a_pgraph_d3d11.c"
    "${ROOT}/src/nv2a/nv2a_pgraph_d3d11.h"
    "${ROOT}/src/nv2a/nv2a_pgraph_state.c"
    "${ROOT}/src/nv2a/nv2a_pgraph_state.h"
    "${ROOT}/src/nv2a/nv2a_renderer_checkpoint.c"
    "${ROOT}/src/nv2a/nv2a_renderer_checkpoint.h"
    "${ROOT}/src/d3d/d3d8_device.c"
    "${ROOT}/src/d3d/d3d8_resources.c"
    "${ROOT}/src/d3d/d3d8_swizzle.h"
    "${ROOT}/src/d3d/d3d8_replay_images.c"
    "${ROOT}/src/d3d/d3d8_replay_images.h"
    "${ROOT}/src/d3d/d3d8_render_checkpoint.inc"
    "${ROOT}/src/d3d/d3d8_pipeline_checkpoint.h" "${ROOT}/src/d3d/d3d8_pipeline_checkpoint.inc"
    "${ROOT}/src/d3d/d3d8_internal.h"
    "${ROOT}/src/d3d/d3d8_shaders.c" "${ROOT}/src/d3d/d3d8_states.c"
    "${ROOT}/src/d3d/d3d8_combiners.c" "${ROOT}/src/d3d/d3d8_combiners.h"
    "${ROOT}/src/d3d/d3d8_vsh.c" "${ROOT}/src/d3d/d3d8_vsh.h"
    "${ROOT}/src/nv2a/nv2a_vertex_program.c"
    "${ROOT}/src/nv2a/nv2a_vertex_program.h"
    "${ROOT}/src/online/rollback_checkpoint.c" "${ROOT}/src/online/rollback_checkpoint.h"
    "${ROOT}/src/kernel/kernel_interrupt.c"
    "${ROOT}/src/kernel/kernel_interrupt.h"
    "${ROOT}/src/kernel/xbox_heap.c"
    "${ROOT}/src/kernel/xbox_heap.h"
    "${ROOT}/src/kernel/xbox_memory_layout.c"
    "${ROOT}/src/kernel/xbox_memory_layout.h"
    "${ROOT}/src/online/xbox_det.c" "${ROOT}/src/online/xbox_det.h"
    "${ROOT}/tools/recomp/rollback_workers.py"
    "${ROOT}/src/online/rollback_frame.h" "${ROOT}/src/game/rollback_frame.c"
    "${ROOT}/src/game/rollback_ownership.c"
    "${ROOT}/src/game/rollback_state_audit.c" "${ROOT}/src/game/rollback_state_audit.h"
    "${ROOT}/src/game/rollback_input.c" "${ROOT}/src/game/rollback_input.h"
    "${ROOT}/src/game/rollback_input_tape.c" "${ROOT}/src/game/rollback_input_tape.h"
    "${ROOT}/src/game/rollback_input_audit.c"
    "${ROOT}/tools/recomp/rollback_boundaries.py"
    "${ROOT}/src/online/rollback_tasks.h" "${ROOT}/src/online/rollback_nested.h"
    "${ROOT}/src/online/rollback_nested.c" "${ROOT}/tools/recomp/rollback_nested.py"
    "${ROOT}/tools/recomp/rollback_match.py")
set(GENERATED "${CMAKE_CURRENT_BINARY_DIR}/generated")
add_custom_command(OUTPUT "${GENERATED}/shadow.generated.c" "${GENERATED}/shadow_types.h"
    BYPRODUCTS "${GENERATED}/source_digest.bin"
    COMMAND "${Python3_EXECUTABLE}" "${ROOT}/tools/recomp/rollback_shadow.py"
        --gen "${ROOT}/src/game/recomp/gen" --out "${GENERATED}"
    DEPENDS ${INPUTS} ${HOST_MODEL} "${ROOT}/src/game/recomp/recomp_types.h"
        "${ROOT}/src/game/recomp/gen/xbe_layout.h"
        "${ROOT}/src/game/recomp/recomp_manual.c" "${ROOT}/tools/recomp/rollback_shadow.py"
        "${ROOT}/tools/recomp/rollback_tasks.py" VERBATIM)
add_executable(rollback_shadow shadow_runtime.c shadow_apu.c shadow_gpu.c "${ROOT}/src/nv2a/nv2a_mmio_hook.c" "${GENERATED}/shadow.generated.c"
    "${ROOT}/src/online/rollback_nested.c"
    "${ROOT}/src/apu/apu_rollback.c" "${ROOT}/src/apu/apu_vp.c" "${ROOT}/src/apu/apu_mmio_hook.c")
target_include_directories(rollback_shadow PRIVATE . "${GENERATED}" "${ROOT}/src/online" "${ROOT}/src/apu" "${ROOT}/src/nv2a")
target_compile_definitions(rollback_shadow PRIVATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES DOA3_SHADOW_APU DOA3_SHADOW_GPU DOA3_NV2A_DECODER_ONLY)
if(MSVC)
    target_compile_options(rollback_shadow PRIVATE /Gy)
    set_source_files_properties("${GENERATED}/shadow.generated.c" PROPERTIES COMPILE_FLAGS "/w /bigobj")
    target_link_options(rollback_shadow PRIVATE /STACK:33554432)
endif()
add_test(NAME rollback_shadow_format COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/shadow_test.py" "$<TARGET_FILE:rollback_shadow>"
    "${GENERATED}/source_digest.bin" "$<TARGET_FILE:rollback_apu_mmio_test>")
set_tests_properties(rollback_shadow_format PROPERTIES TIMEOUT 180)
add_executable(rollback_chain_test shadow_runtime.c chain_fixture/guest.c)
target_include_directories(rollback_chain_test PRIVATE chain_fixture . "${ROOT}/src/online")
target_compile_definitions(rollback_chain_test PRIVATE _CRT_SECURE_NO_WARNINGS)
add_test(NAME rollback_chain COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/chain_test.py" "$<TARGET_FILE:rollback_chain_test>")
set_tests_properties(rollback_chain PROPERTIES TIMEOUT 180)
add_test(NAME rollback_chain_nested COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/chain_test.py" "$<TARGET_FILE:rollback_chain_test>" --nested)
set_tests_properties(rollback_chain_nested PROPERTIES TIMEOUT 180)
add_executable(rollback_apu_mmio_test apu_mmio_test.c shadow_apu.c
    "${ROOT}/src/apu/apu_rollback.c" "${ROOT}/src/apu/apu_vp.c" "${ROOT}/src/apu/apu_mmio_hook.c")
target_include_directories(rollback_apu_mmio_test PRIVATE . "${ROOT}/src/apu" "${ROOT}/src/nv2a")
target_compile_definitions(rollback_apu_mmio_test PRIVATE DOA3_SHADOW_APU _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
if(MSVC)
    target_compile_options(rollback_apu_mmio_test PRIVATE /Gy)
endif()
add_test(NAME rollback_apu_mmio COMMAND rollback_apu_mmio_test)
add_executable(rollback_chain_apu_test shadow_runtime.c chain_fixture/guest.c shadow_apu.c
    "${ROOT}/src/apu/apu_rollback.c" "${ROOT}/src/apu/apu_vp.c" "${ROOT}/src/apu/apu_mmio_hook.c")
target_include_directories(rollback_chain_apu_test PRIVATE chain_fixture . "${ROOT}/src/online"
    "${ROOT}/src/apu" "${ROOT}/src/nv2a")
target_compile_definitions(rollback_chain_apu_test PRIVATE DOA3_SHADOW_APU _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
if(MSVC)
    target_compile_options(rollback_chain_apu_test PRIVATE /Gy)
endif()
add_test(NAME rollback_chain_apu COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/chain_test.py" "$<TARGET_FILE:rollback_chain_apu_test>"
    "$<TARGET_FILE:rollback_apu_mmio_test>")
set_tests_properties(rollback_chain_apu PROPERTIES TIMEOUT 180)
add_test(NAME rollback_kernel_abi COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel_test.py" "$<TARGET_FILE:rollback_chain_test>")

add_executable(rollback_gpu_mmio_test gpu_mmio_test.c shadow_gpu.c "${ROOT}/src/nv2a/nv2a_mmio_hook.c")
target_include_directories(rollback_gpu_mmio_test PRIVATE . "${ROOT}/src/nv2a")
target_compile_definitions(rollback_gpu_mmio_test PRIVATE DOA3_SHADOW_GPU DOA3_NV2A_DECODER_ONLY _CRT_SECURE_NO_WARNINGS)
add_executable(rollback_chain_gpu_test shadow_runtime.c chain_fixture/guest.c shadow_gpu.c "${ROOT}/src/nv2a/nv2a_mmio_hook.c")
target_include_directories(rollback_chain_gpu_test PRIVATE chain_fixture . "${ROOT}/src/online" "${ROOT}/src/nv2a")
target_compile_definitions(rollback_chain_gpu_test PRIVATE DOA3_SHADOW_GPU DOA3_NV2A_DECODER_ONLY _CRT_SECURE_NO_WARNINGS)
if(MSVC)
    target_compile_options(rollback_gpu_mmio_test PRIVATE /Gy)
    target_compile_options(rollback_chain_gpu_test PRIVATE /Gy)
endif()
add_test(NAME rollback_gpu_mmio COMMAND rollback_gpu_mmio_test)
add_test(NAME rollback_chain_gpu COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/chain_test.py" "$<TARGET_FILE:rollback_chain_gpu_test>" --gpu)
set_tests_properties(rollback_chain_gpu PROPERTIES TIMEOUT 180)
add_test(NAME rollback_submission_profile COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/submission_test.py" "$<TARGET_FILE:rollback_chain_gpu_test>")

add_test(NAME rollback_targets_profile COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/targets_test.py" "$<TARGET_FILE:rollback_chain_gpu_test>")
add_test(NAME rollback_sprite_guard COMMAND "${Python3_EXECUTABLE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/sprite_test.py" "$<TARGET_FILE:rollback_chain_test>")
