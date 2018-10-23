SET(sources
    main.cpp)

SET(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -w")


add_executable( gsecli ${sources})

target_link_libraries(gsecli  PUBLIC Beast::Beast ${GSE_BOOST_LIBS} commandParser)
