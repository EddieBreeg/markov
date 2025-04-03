cmake_minimum_required(VERSION 3.10.0)
project(Markov VERSION 0.1.0 LANGUAGES C CXX)

option(MARKOV_INSTALL "Install Markov targets" ON)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} IsTopLevel)

if(${IsTopLevel})
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(${IsTopLevel})


include(CPack)
include(CTest)
enable_testing()

add_subdirectory(source)
add_subdirectory(tests)
