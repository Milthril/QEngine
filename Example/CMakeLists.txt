cmake_minimum_required(VERSION 3.5)

project(Example VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE PROJECT_SOURCE FILES *.h *.cpp)

add_executable(Example
    ${PROJECT_SOURCE}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})

target_link_libraries(Example PUBLIC Core)

add_custom_command(TARGET Example PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp> $<TARGET_FILE_DIR:Example>
    COMMAND_EXPAND_LISTS
)

target_compile_definitions(Example PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

target_compile_definitions(Example PRIVATE ASSET_DIR="${ASSET_DIR}")