cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)

set(PROJECT_NAME SKPokerEval)
project(${PROJECT_NAME})

# Versioning.
set(SK_POKER_EVAL_VERSION_MAJOR 1)
set(SK_POKER_EVAL_VERSION_MINOR 9)
set(SK_POKER_EVAL_VERSION_PATCH 0)

# Get the current commit.
execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE SK_POKER_EVAL_GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/SKConfig.h.in"
  "${PROJECT_BINARY_DIR}/generated/SKConfig.h"
)

# Includes.
include_directories("${PROJECT_SOURCE_DIR}/generated")

# Output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 11)

# Build
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(lib/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(eval_tests tests/five_eval_tests.cpp tests/seven_eval_tests.cpp)

target_link_libraries(eval_tests gtest_main skpokereval)
add_test(NAME GTests COMMAND eval_tests)

