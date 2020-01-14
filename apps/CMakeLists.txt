add_executable(bsp_tool
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ../ut_bsp_tool.qrc
        )
target_link_libraries(bsp_tool PUBLIC my_bsp Qt5::Widgets)
target_include_directories(bsp_tool PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(create_world
        create_world.cpp
        )
target_link_libraries(create_world PUBLIC my_bsp Qt5::Widgets)
target_include_directories(create_world PRIVATE ${CMAKE_SOURCE_DIR})
