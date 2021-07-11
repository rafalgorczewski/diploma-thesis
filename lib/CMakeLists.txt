cmake_minimum_required(VERSION 3.16)
project(thesis_lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")

include(FetchContent)

FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
)
FetchContent_MakeAvailable(eigen)

FetchContent_Declare(minidnn
    GIT_REPOSITORY https://github.com/yixuan/MiniDNN.git
)
FetchContent_MakeAvailable(minidnn)

set(THL_SRC
        buffer.cpp buffer.hpp
        spectrum.hpp
        stream.cpp stream.hpp
        streamreader.cpp streamreader.hpp
        bodypart.hpp
        classifier.hpp classifier.cpp)

set(THL_INCLUDE_DIRS "ext/include" ${minidnn_SOURCE_DIR}/include ${eigen_SOURCE_DIR})
set(THL_LINK_DIRS "ext/lib")

include_directories(${THL_INCLUDE_DIRS})
link_directories(${THL_LINK_DIRS})

add_library(thesis_lib ${THL_SRC})
target_link_libraries(thesis_lib fftw3-3 m lsl eigen)
