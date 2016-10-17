cmake_minimum_required(VERSION 3.6)
project(mag_tolstikov_transposotion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES transpose.cpp)
add_executable(mag_tolstikov_transposotion ${SOURCE_FILES})

set(SOURCE_FILES1 generate.cpp)
add_executable(generate ${SOURCE_FILES1})

set(SOURCE_FILES2 print_matrix.cpp)
add_executable(print ${SOURCE_FILES2})

set(SOURCE_FILES3 mul.cpp)
add_executable(mul ${SOURCE_FILES3})