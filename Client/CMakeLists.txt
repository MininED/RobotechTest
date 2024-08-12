set(SOURCES
        src/main.cpp
        src/tcp_client.cpp
)

set(HEADERS
        include/tcp_client.h
)

add_executable(Client ${SOURCES} ${HEADERS})

target_link_libraries(Client PRIVATE Boost::system)

install(TARGETS Client RUNTIME DESTINATION bin)