#include <iostream>
#include "../include/server.h"

int main() {
    try {
        boost::asio::io_context io_context;
        SharedData shared_data;
        initialize_shared_data(shared_data);
        Server server(io_context, 1502, 1501, shared_data); // Ports for Modbus and custom TCP
        std::cout << "MODBUS on port 1502 and TCP on port 1501 server started, waiting for connections..." << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << std::endl;
    }

    return 0;
}