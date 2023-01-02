cmake_minimum_required(VERSION 3.23)
project(
    cmakeprogen
    VERSION 0.1.0
    DESCRIPTION "A cmake project generated for C/C++"
    LANGUAGES CXX)

link_directories(
    ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)

file(COPY include DESTINATION ${CMAKE_BINARY_DIR}/linux/include)

add_subdirectory(config)
add_subdirectory(src)
