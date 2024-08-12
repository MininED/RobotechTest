#include "../include/tcp_client.h"
#include <iostream>
#include <string>
#include <sstream>

void print_help() {
    std::cout << "Available commands:\n"
              << "  connect <address> <port>       Connect to server\n"
              << "  disconnect                     Disconnect from server\n"
              << "  read_input <number>            Read digital input\n"
              << "  read_output <number>           Read digital output\n"
              << "  read_input_register <number>   Read input register\n"
              << "  read_output_register <number>  Read output register\n"
              << "  read_all_inputs                Read all digital inputs\n"
              << "  read_all_outputs               Read all digital outputs\n"
              << "  read_all_input_registers       Read all input registers\n"
              << "  read_all_output_registers      Read all output registers\n"
              << "  read_all                       Read all data\n"
              << "  start cyclic_read              Start cyclic reading\n"
              << "  stop cyclic_read               Stop cyclic reading\n"
              << "  help                           Show this help message\n"
              << "  exit                           Exit the application\n";
}

int main() {
    try {
        boost::asio::io_context io_context;
        TCPClient client(io_context);

        std::string command;
        while (true) {
            std::cout << "Enter command (type 'help' for list of commands): ";
            std::getline(std::cin, command);

            if (command == "exit") {
                client.disconnect();
                break;
            } else if (command == "help") {
                print_help();
            } else if (command.substr(0, 8) == "connect ") {
                std::istringstream iss(command.substr(8));
                std::string host;
                std::string port;
                iss >> host >> port;
                client.connect(host, port);
            } else if (command == "disconnect") {
                client.disconnect();
            } else if (command == "start_cyclic_read") {
                client.start_cyclic_reading();
            } else if (client.is_cyclic_reading_ && !command.empty()) {
                client.stop_cyclic_reading();
            } else {
                client.send(command);
                std::cout << client.receive() << std::endl;
            }

            client.check_connection();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}