add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)
add_test(NAME tests COMMAND tests)
