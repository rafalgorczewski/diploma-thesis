cmake_minimum_required(VERSION 3.16)
project(thesis_lib)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O2")

FetchContent_Declare(opencv
    GIT_REPOSITORY https://github.com/huihut/OpenCV-MinGW-Build.git
    GIT_TAG OpenCV-4.5.2-x64
)
FetchContent_MakeAvailable(opencv)

set(THL_SRC
        buffer.cpp buffer.hpp
        spectrum.hpp
        stream.cpp stream.hpp
        streamreader.cpp streamreader.hpp
        bodypart.hpp
        classifier.hpp classifier.cpp)

set(THL_INCLUDE_DIRS "ext/include")
set(THL_LINK_DIRS "ext/lib")

include_directories(${THL_INCLUDE_DIRS} ${opencv_SOURCE_DIR}/include)
link_directories(${THL_LINK_DIRS} ${opencv_SOURCE_DIR}/x64/mingw/lib)

add_library(thesis_lib ${THL_SRC})
target_link_libraries(thesis_lib fftw3-3 m lsl opencv_core452)
