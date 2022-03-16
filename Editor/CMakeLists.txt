cmake_minimum_required(VERSION 3.5)

project(Editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE FILES *.h *.cpp)

qt6_add_resources(QRC_FILES resources.qrc)

add_executable(Editor
    ${PROJECT_SOURCE}
    ${QRC_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})
target_link_libraries(Editor 
    PUBLIC 
    Core
    kddockwidgets
    QScintilla
    )

add_custom_command(TARGET Editor PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp> $<TARGET_FILE_DIR:Editor>
    COMMAND_EXPAND_LISTS
)