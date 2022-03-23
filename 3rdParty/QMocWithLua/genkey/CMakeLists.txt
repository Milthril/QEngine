cmake_minimum_required(VERSION 3.5)

project(moc VERSION 0.1 LANGUAGES CXX)

add_executable(QMocKeywordGen  generate_keywords.cpp)

target_compile_definitions(QMocKeywordGen PRIVATE KEYWORDS_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../keywords.inl")

target_link_libraries(QMocKeywordGen PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate )
