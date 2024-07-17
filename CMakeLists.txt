cmake_minimum_required(VERSION 3.11)
include(ExternalProject)

project("stm32renderer" C ASM)
include(CTest)

set(CPU cortex-m0)
add_definitions(-DSTM32F042x6)
add_definitions(-DUSE_FULL_LL_DRIVER)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/linker_script.ld")

add_executable(
    stm32renderer

    submodules/stm32f0xx_hal_driver/Src/stm32f0xx_ll_gpio.c
    submodules/stm32f0xx_hal_driver/Src/stm32f0xx_ll_i2c.c

    src/startup.s
    src/main.c
    src/frame_drawing.c
    src/i2c.c
    src/sh1106.c
    src/math/fixed32.c
    src/runtime_abi.c
    src/rendering/rasterization.c
    src/rendering/vertex_processing.c
    src/rendering/camera.c
    src/rendering/model.c
    src/rendering/text_drawing.c
    src/math/quat.c
    src/math/vec3.c
    src/math/mat4.c
)

target_compile_options(
    stm32renderer PRIVATE
    -mcpu=${CPU}
    -std=gnu11
    $<$<CONFIG:Release>:-Os>
    -Wall
    -Wno-main
    -Wundef
    -Werror
    -Wfatal-errors
    --param=min-pagesize=0
    -Wl,--relax,--gc-sections
    -nostdlib
    -mthumb
    -T ${LINKER_SCRIPT}
)

target_link_options(
    stm32renderer PRIVATE
    -nostdlib
    -mthumb
    -T ${LINKER_SCRIPT}
)

target_include_directories(
    stm32renderer PUBLIC
    include/
    submodules/stm32f0xx_hal_driver/Inc
    submodules/cmsis_device_f0/Include
    submodules/CMSIS_5/CMSIS/Core/Include
)

set_target_properties(stm32renderer PROPERTIES OUTPUT_NAME stm32renderer.elf)

add_custom_target(
    upload
    COMMAND arm-none-eabi-objcopy -O binary stm32renderer.elf firmware.bin
    COMMAND st-flash write firmware.bin 0x08000000
)

ExternalProject_Add(
    stm32renderer_test
    PREFIX ${CMAKE_BINARY_DIR}/test
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/test
    BINARY_DIR ${CMAKE_BINARY_DIR}/test
    INSTALL_COMMAND ""
    BUILD_ALWAYS TRUE
    CMAKE_ARGS
        -DCMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

add_test(NAME stm32renderer_test COMMAND ./test/stm32renderer_test)
enable_testing()
