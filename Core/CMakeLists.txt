cmake_minimum_required(VERSION 3.5)

project(Core VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets ShaderTools Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Core5Compat Widgets OpenGL ShaderTools Network REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE FILES *.h *.cpp)

add_library(Core STATIC
    ${PROJECT_SOURCE}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})

target_link_libraries(Core PUBLIC 
    Qt${QT_VERSION_MAJOR}::Core5Compat 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::WidgetsPrivate 
    Qt${QT_VERSION_MAJOR}::CorePrivate 
    Qt${QT_VERSION_MAJOR}::OpenGLPrivate 
    Qt${QT_VERSION_MAJOR}::ShaderToolsPrivate
    Qt${QT_VERSION_MAJOR}::Network
    assimp::assimp
)
target_include_directories(Core PUBLIC  .)

