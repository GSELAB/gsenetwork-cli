SET(client_source
    JsonRequest.cpp
    Http.cpp)

ADD_LIBRARY(client ${client_source})
TARGET_LINK_LIBRARIES(client
    utils crypto trie jsoncpp_lib_static Beast::Beast ${GSE_BOOST_LIBS})