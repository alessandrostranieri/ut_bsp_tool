add_library(my_bsp STATIC
    player.cpp
    player.h
    polygon.cpp
    polygon.h
    worldline.cpp
    worldline.h
    bsp.cpp
    bsp.h)
target_link_libraries(my_bsp PUBLIC Qt5::Widgets)
