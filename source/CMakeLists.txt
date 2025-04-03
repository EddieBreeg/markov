add_library(Markov 
	Markov.cpp
	Alloca.hpp
	Markov.hpp
)

if(${MSVC})
	target_sources(Markov PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/View.natvis)
endif(${MSVC})


target_include_directories(Markov PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Markov PUBLIC cxx_std_17)
set_target_properties(Markov PROPERTIES
	COMPILE_WARNING_AS_ERROR ON
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER Markov.hpp
)

include(GNUInstallDirs)

if (${MARKOV_INSTALL})
	install(TARGETS Markov PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif (${MARKOV_INSTALL})

add_executable(Example example.cpp)
target_link_libraries(Example PRIVATE Markov)