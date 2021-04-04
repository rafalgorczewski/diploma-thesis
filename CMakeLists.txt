cmake_minimum_required(VERSION 3.16)
project(thesis)

add_subdirectory(lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -g")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets Charts REQUIRED)

set(TH_SRC
        window.cpp window.hpp)

set(TH_INCLUDE_DIRS "lib")
set(TH_LINK_DIRS "lib")

include_directories(${TH_INCLUDE_DIRS})
link_directories(${TH_LINK_DIRS})

add_executable(thesis main.cpp ${TH_SRC})

target_link_libraries(thesis PRIVATE Qt5::Widgets Qt5::Charts thesis_lib lsl)
