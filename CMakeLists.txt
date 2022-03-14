cmake_minimum_required(VERSION 3.5)

project(QEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3rdParty)
add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(Example)