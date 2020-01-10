add_library(my_bsp STATIC
    player.cpp
    player.h
    polygon.cpp
    polygon.h)
target_link_libraries(my_bsp PUBLIC Qt5::Widgets)

