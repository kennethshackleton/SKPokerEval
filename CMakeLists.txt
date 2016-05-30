cmake_minimum_required (VERSION 2.8.7)

set(PROJECT_NAME SKPokerEval)
project(${PROJECT_NAME})

# Versioning.
set (SK_POKER_EVAL_VERSION_MAJOR 1)
set (SK_POKER_EVAL_VERSION_MINOR 3)
configure_file (
  "${PROJECT_SOURCE_DIR}/src/SKConfig.h.in"
  "${PROJECT_BINARY_DIR}/src/SKConfig.h"
)

set(CMAKE_CXX_FLAGS "-g -Wall")

# Build
add_library(skpokereval src/FiveEval.cpp src/SevenEval.cpp)

# Testing
enable_testing()
add_subdirectory(lib/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(eval_tests tests/seven_eval_tests.cpp)
target_link_libraries(eval_tests gtest_main skpokereval)
add_test(NAME GTests COMMAND eval_tests)
