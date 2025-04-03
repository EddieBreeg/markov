set(CMAKE_FOLDER Tests)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE Markov)


add_test(NAME MarkovTests COMMAND tests)