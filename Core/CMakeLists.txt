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

set(LUA_API_DIR $ENV{USERPROFILE}/QEngineCoreApplication/LuaAPI)
string(REPLACE "\\" "/" LUA_API_DIR ${LUA_API_DIR})

set(CMAKE_AUTOMOC_MOC_OPTIONS --output-lua-api ${LUA_API_DIR})        #开启moc的json生成

add_library(Core STATIC
    ${PROJECT_SOURCE}
)

add_dependencies(Core QMocWithLua)                  #使Core依赖于QMocWithLua

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
    ImGuizmo
    sol2
    Lua
    SpectrumProvider
)

target_compile_definitions(Core PUBLIC LUA_API_DIR="${LUA_API_DIR}")

function(target_setup_dll TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp> $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND_EXPAND_LISTS
    )
    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Lua> $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND_EXPAND_LISTS
    )
endfunction()

target_include_directories(Core PUBLIC  .)

set(QEngineCoreApplication_ASSET_DIR "${CMAKE_SOURCE_DIR}/Asset")

target_compile_definitions(Core PUBLIC QEngineCoreApplication_ASSET_DIR="${QEngineCoreApplication_ASSET_DIR}")

