add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE my_bsp GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME tests COMMAND tests)
