cmake_minimum_required (VERSION 2.8.11)

set(PROJECT_NAME SpecialKEval)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")

### Build
add_library(specialkeval src/FiveEval.cpp src/SevenEval.cpp)

### Testing
enable_testing()
add_subdirectory(lib/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(eval_tests tests/seven_eval_tests.cpp)
target_link_libraries(eval_tests gtest_main specialkeval)
add_test(NAME GTests COMMAND eval_tests)
