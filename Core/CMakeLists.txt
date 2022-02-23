cmake_minimum_required(VERSION 3.5)

project(Core VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets ShaderTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core  Widgets ShaderTools REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE FILES *.h  *.cpp)

add_executable(Core
    ${PROJECT_SOURCE}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})

target_link_libraries(Core PRIVATE Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::GuiPrivate Qt${QT_VERSION_MAJOR}::ShaderToolsPrivate)

