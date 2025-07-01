cmake_minimum_required(VERSION 3.31)
project(benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(benchmark
        benchmark.cc
        hilbert_curve_sort.cc
        hilbert_curve_sort.h)
