cmake_minimum_required(VERSION 3.14)

project(thesis VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

add_subdirectory(lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O2")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Quick Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Quick Charts REQUIRED)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(json)

set(TH_INCLUDE_DIRS "lib" "lib/ext/include" ${json_SOURCE_DIR}/include)
set(TH_LINK_DIRS "lib" "lib/ext/lib")

include_directories(${TH_INCLUDE_DIRS})
link_directories(${TH_LINK_DIRS})

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        recorderbackend.hpp recorderbackend.cpp
        calibrationbackend.hpp calibrationbackend.cpp
        configurator.hpp configurator.cpp
        plotter.h plotter.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(thesis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(thesis SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(thesis
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(thesis
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(thesis
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts thesis_lib lsl)

set_target_properties(thesis PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(thesis)
    qt_finalize_executable(thesis)
endif()

add_executable(test test.cpp)
target_link_libraries(test thesis_lib lsl)
