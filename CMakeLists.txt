cmake_minimum_required(VERSION 3.20)
project(orderbook CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_library(orderbook src/orderbook.cpp)
target_include_directories(orderbook PUBLIC include)
target_compile_options(orderbook PRIVATE -Wall -Wextra -Wpedantic)

# Tests (Catch2 is downloaded automatically the first time you configure)
include(FetchContent)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4)
FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/test_orderbook.cpp)
target_link_libraries(tests PRIVATE orderbook Catch2::Catch2WithMain)

add_executable(bench bench/bench.cpp)
target_link_libraries(bench PRIVATE orderbook)

enable_testing()
add_test(NAME tests COMMAND tests)
