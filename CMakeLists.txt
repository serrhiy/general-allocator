cmake_minimum_required(VERSION 3.15...4.0)
project(general-allocator LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs)
add_subdirectory(src)