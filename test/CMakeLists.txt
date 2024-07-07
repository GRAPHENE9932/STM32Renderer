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

# The stm32renderer and stm32renderer_test are two independent CMake projects built for different CPU architectures
# (however, stm32renderer hooks the stm32renderer_test build with ExternalProject_Add).
# So, there are two independent compile_commands.json that are both necessary if you need clangd to work
# with both the test and the main project at the same time. But clangd does not support multiple
# compile commands files (https://github.com/clangd/clangd/issues/116).
# The best solution I see is to merge these compile_commands files together here, using the jq command.
if (${CMAKE_EXPORT_COMPILE_COMMANDS})
    set(TEST_COMPILE_COMMANDS "${CMAKE_BINARY_DIR}/compile_commands.json")
    set(MAIN_COMPILE_COMMANDS "${CMAKE_BINARY_DIR}/../compile_commands.json")
    set(MERGED_COMPILE_COMMANDS "${CMAKE_BINARY_DIR}/../compile_commands_merged.json")
    add_custom_command(
        TARGET stm32renderer_test POST_BUILD
        COMMAND jq -s '.[0] + .[1]' "${MAIN_COMPILE_COMMANDS}" "${TEST_COMPILE_COMMANDS}" > "${MERGED_COMPILE_COMMANDS}"
        COMMAND ${CMAKE_COMMAND} -E copy ${MERGED_COMPILE_COMMANDS} ${MAIN_COMPILE_COMMANDS}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/.."
    )
endif()
