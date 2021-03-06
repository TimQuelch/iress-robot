cmake_minimum_required(VERSION 3.20)

# Main project
project(iress-robot
  LANGUAGES CXX
  VERSION 1.0.0
  )

# Dependencies
include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 8.1.1
  GIT_SHALLOW ON
  )

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.1
  GIT_SHALLOW ON
  )

FetchContent_MakeAvailable(fmt Catch2)

include(${CMAKE_SOURCE_DIR}/cmake/set_build_options.cmake)

# Main lib
add_library(robot src/robot.cpp)
target_include_directories(robot PUBLIC src)
target_link_libraries(robot PRIVATE fmt::fmt)
robot_set_build_options(robot)

# Main executable
add_executable(main src/main.cpp)
target_link_libraries(main robot)
robot_set_build_options(main)

# Tests
add_executable(tests test/test.cpp)
target_link_libraries(tests PRIVATE robot Catch2::Catch2WithMain fmt::fmt)
robot_set_build_options(tests)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
