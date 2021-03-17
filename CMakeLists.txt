cmake_minimum_required(VERSION 3.16)
project(thesis)

add_subdirectory(lib)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")

set(TH_INCLUDE

)

set(TH_SRC

)

set(TH_INCLUDE_DIRS "include lib/include")
set(TH_LINK_DIRS "lib")

link_directories(${TH_LINK_DIRS})
include_directories(${TH_INCLUDE_DIRS})

add_executable(thesis main.cpp ${TH_SRC} ${TH_INCLUDE})

target_link_libraries(thesis thesis_lib lsl)
