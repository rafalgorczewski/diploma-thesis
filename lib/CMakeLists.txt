cmake_minimum_required(VERSION 3.16)
project(thesis_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")

set(THL_SRC
        buffer.cpp buffer.hpp
        sample.hpp
        spectrum.hpp
        stream.cpp stream.hpp
        streamreader.cpp streamreader.hpp)

set(THL_INCLUDE_DIRS "ext/include")
set(THL_LINK_DIRS "ext/lib")

include_directories(${THL_INCLUDE_DIRS})
link_directories(${THL_LINK_DIRS})

add_library(thesis_lib ${THL_SRC})
target_link_libraries(thesis_lib m lsl)
