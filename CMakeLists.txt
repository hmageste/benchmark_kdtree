cmake_minimum_required(VERSION 3.15)
project(KDTreeBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GoogleBenchmark
include(FetchContent)
FetchContent_Declare(benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable gtest")
FetchContent_MakeAvailable(benchmark)

# Add nlohmann/json for output
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Main executable
add_executable(benchmark_kdtree
  src/main.cpp
)

target_link_libraries(benchmark_kdtree
  PRIVATE nlohmann_json::nlohmann_json m
)

target_include_directories(benchmark_kdtree
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set compiler flags for optimization
if(MSVC)
  target_compile_options(benchmark_kdtree PRIVATE /O2)
else()
  target_compile_options(benchmark_kdtree PRIVATE -O2)
endif()
