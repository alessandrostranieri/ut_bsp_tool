add_executable(bsp_tool
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  ../ut_bsp_tool.qrc
)
target_link_libraries(bsp_tool PUBLIC my_bsp Qt5::Widgets)
target_include_directories(bsp_tool PUBLIC my_bsp)

add_executable(create_world
    create_world.cpp
)
target_link_libraries(create_world PRIVATE my_bsp Qt5::Widgets)
