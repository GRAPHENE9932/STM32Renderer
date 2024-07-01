cmake_minimum_required(VERSION 3.11)

project("stm32renderer_test" C ASM)

add_executable(stm32renderer_test
    main.c
    fixed16_16_test.c
    ../src/fixed16_16.c
    ../submodules/Unity/src/unity.c
)

target_include_directories(
    stm32renderer_test PUBLIC
    ../include/
    ../submodules/Unity/src
)

set_target_properties(stm32renderer_test PROPERTIES OUTPUT_NAME stm32renderer_test)
