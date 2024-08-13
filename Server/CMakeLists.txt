set(SOURCES
        src/main.cpp
        src/modbus_session.cpp
        src/tcp_session.cpp
        src/server.cpp
)

set(HEADERS
        include/modbus_session.h
        include/tcp_session.h
        include/server.h
        include/shared_data.h
)

add_executable(Server ${SOURCES} ${HEADERS})

target_link_libraries(Server PRIVATE Boost::system Threads::Threads ${LIBMODBUS_LIBRARIES})

install(TARGETS Server RUNTIME DESTINATION bin)

target_compile_options(Server PRIVATE -Wall -Wextra -pedantic)
