cmake_minimum_required(VERSION 3.8)
project("sfdb")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("external/fmt")

# Simple File Database
add_library("sfdb")

target_include_directories("sfdb" PUBLIC "${CMAKE_SOURCE_DIR}/src/")
target_link_libraries("sfdb" PUBLIC fmt::fmt)

file(GLOB_RECURSE SFDB_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/**.cpp")
target_sources("sfdb" PRIVATE "${SFDB_SOURCES}")

# Example
add_executable("sfdb_example")

target_link_libraries("sfdb_example" PRIVATE "sfdb")

file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/example/**.cpp")
target_sources("sfdb_example" PRIVATE "${EXAMPLE_SOURCES}")