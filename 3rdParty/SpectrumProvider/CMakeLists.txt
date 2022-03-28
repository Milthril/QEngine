cmake_minimum_required(VERSION 3.16)

project(SpectrumProvider LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB PROJECT_SOURCE FILES Src/*.h Src/*.cpp)

add_library(SpectrumProvider STATIC
    ${PROJECT_SOURCE}
)

add_library(fftw3 STATIC IMPORTED)
set_target_properties(fftw3 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/fftw3/lib/fftw3.lib )

target_link_libraries(SpectrumProvider PRIVATE fftw3)

target_include_directories(
    SpectrumProvider PUBLIC 
    Src
)

target_include_directories(
    SpectrumProvider PRIVATE
    fftw3/api
)
