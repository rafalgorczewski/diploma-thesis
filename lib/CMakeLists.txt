cmake_minimum_required(VERSION 3.16)
project(thesis_lib)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")

set(THL_INCLUDE_DIRS "include")

set(THL_INCLUDE
  include/streamer.hpp
)

set(THL_SRC
  src/streamer.cpp
)

include_directories(${THL_INCLUDE_DIRS})
add_library(thesis_lib ${THL_SRC} ${THL_INCLUDE})
