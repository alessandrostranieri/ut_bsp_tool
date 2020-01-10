cmake_minimum_required(VERSION 3.10)

project(ut_bsp_tool LANGUAGES CXX)

include(CTest)
enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(GTest MODULE REQUIRED)

add_subdirectory(bsp)
add_subdirectory(apps)
add_subdirectory(tests)
