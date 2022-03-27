cmake_minimum_required(VERSION 3.16)

project(SpectrumProvider LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE PROJECT_SOURCE FILES Src/*.h Src/*.cpp Src/*.hpp)

add_subdirectory(3rdParty/Kfr)

add_library(SpectrumProvider STATIC
    ${PROJECT_SOURCE}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})

target_link_libraries(SpectrumProvider PRIVATE kfr kfr_dft)

target_include_directories(SpectrumProvider PUBLIC Src )

