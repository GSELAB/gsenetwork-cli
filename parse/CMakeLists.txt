SET(sources
    commandParser.cpp)

SET(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -w")


ADD_LIBRARY(commandParser ${sources})

target_link_libraries(commandParser PUBLIC Beast::Beast ${GSE_BOOST_LIBS} client)