cmake_minimum_required(VERSION 3.15...4.0)
project(general-allocator LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build tests" OFF)
include(CTest)

add_subdirectory(src)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()