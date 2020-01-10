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

add_library(my_bsp
    player.cpp
    player.h
    polygon.cpp
    polygon.h)
target_link_libraries(my_bsp PUBLIC Qt5::Widgets)

add_executable(bsp_tool
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  ut_bsp_tool.qrc
)
target_link_libraries(bsp_tool PRIVATE my_bsp Qt5::Widgets)

add_executable(create_world
    create_world.cpp
    player.cpp
    player.h
)
target_link_libraries(create_world PRIVATE my_bsp Qt5::Widgets)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)
add_test(NAME tests COMMAND tests)
