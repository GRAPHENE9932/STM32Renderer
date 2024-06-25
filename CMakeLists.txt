cmake_minimum_required(VERSION 3.11)

project("stm32renderer" C ASM)

set(CPU cortex-m0)
add_definitions(-DSTM32F042x6)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/linker_script.ld")

add_compile_options(
    -mcpu=${CPU}
    -std=gnu11
    -Os
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

add_link_options(
    -nostdlib
    -mthumb
    -T ${LINKER_SCRIPT}
)

add_executable(
    stm32renderer

    submodules/stm32f0xx_hal_driver/Src/stm32f0xx_ll_gpio.c
    submodules/stm32f0xx_hal_driver/Src/stm32f0xx_ll_i2c.c

    src/startup.s
    src/main.c
    src/i2c.c
    src/sh1106.c
)

add_definitions(-DUSE_FULL_LL_DRIVER)

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
