cmake_minimum_required(VERSION 3.5)

project(moc VERSION 0.1 LANGUAGES CXX)

include_directories(tinycbor)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

#_RECURSE
file(GLOB PROJECT_SOURCES FILES *.h *.cpp *.hpp)
add_executable(moc  ${PROJECT_SOURCES})

get_target_property(MOC_PATH Qt${QT_VERSION_MAJOR}::moc IMPORTED_LOCATION)
get_filename_component(MOC_DIR ${MOC_PATH} DIRECTORY)

set_target_properties(moc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MOC_DIR})
set_target_properties(moc PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${MOC_DIR})
set_target_properties(moc PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MOC_DIR})

add_subdirectory(genkey)

target_link_libraries(moc PRIVATE  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate )
