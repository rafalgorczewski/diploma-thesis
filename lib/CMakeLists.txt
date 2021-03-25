cmake_minimum_required(VERSION 3.16)
project(thesis_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")

set(THL_SRC
        buffer.cpp buffer.hpp sample.hpp spectrum.hpp)

add_library(thesis_lib ${THL_SRC})
