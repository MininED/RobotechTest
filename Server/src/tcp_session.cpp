#include "../include/tcp_session.h"

TCPSession::TCPSession(tcp::socket socket, SharedData& shared_data)
    : socket_(std::move(socket)), shared_data_(shared_data) {}

void TCPSession::start() {
    std::thread([self = shared_from_this()]() {
        self->do_session();
    }).detach();
}

void TCPSession::do_session() {
    try {
        std::cout << "TCP client connected from " << socket_.remote_endpoint().address().to_string()
          << ":" << socket_.remote_endpoint().port() << std::endl;

        while (true) {
            std::vector<char> data(1024);
            boost::system::error_code ec;
            size_t length = socket_.read_some(boost::asio::buffer(data), ec);

            std::string command(data.data(), length);
            //Log input
            if (!command.empty())
                std::cout << "TCP input from: " << socket_.remote_endpoint().address().to_string()
                            << ":" << socket_.remote_endpoint().port() << " - " << command;

            if (ec == boost::asio::error::eof) {
                std::cout << "TCP client disconnected from " << socket_.remote_endpoint().address().to_string()
                          << ":" << socket_.remote_endpoint().port() << std::endl;
                break;
            } else if (ec) {
                std::cerr << "TCP read error: " << ec.message() << std::endl;
                break;
            }

            std::string response = handle_tcp_command(command);
            response += '\n';
            //Log reply
            std::cout << "TCP output to: " << socket_.remote_endpoint().address().to_string()
              << ":" << socket_.remote_endpoint().port() << " - " << response;

            boost::asio::write(socket_, boost::asio::buffer(response));
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in TCP session thread: " << e.what() << std::endl;
    }
}

std::string TCPSession::handle_tcp_command(const std::string& command) {
    std::lock_guard<std::mutex> lock(shared_data_.mutex);

    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "read_input") {
        int address;
        iss >> address;
        address--;
        if (address >= 0 && address < shared_data_.digital_inputs.size()) {
            return shared_data_.digital_inputs[address] ? "1" : "0";
        } else {
            return "Error: Invalid input address";
        }
    }

    else if (cmd == "read_output") {
        int address;
        iss >> address;
        address--;
        if (address >= 0 && address < shared_data_.digital_outputs.size()) {
            return shared_data_.digital_outputs[address] ? "1" : "0";
        } else {
            return "Error: Invalid output address";
        }
    }

    else if (cmd == "read_input_register") {
        int address;
        iss >> address;
        address--;
        if (address >= 0 && address < shared_data_.input_registers.size()) {
            return std::to_string(shared_data_.input_registers[address]);
        } else {
            return "Error: Invalid input register address";
        }
    }

    else if (cmd == "read_output_register") {
        int address;
        iss >> address;
        address--;
        if (address >= 0 && address < shared_data_.output_registers.size()) {
            return std::to_string(shared_data_.output_registers[address]);
        } else {
            return "Error: Invalid output register address";
        }
    }

    else if (cmd == "read_all_inputs") {
        std::string response = "Inputs: ";
        for (bool input : shared_data_.digital_inputs) {
            response += input ? "1 " : "0 ";
        }
        return response;
    }

    else if (cmd == "read_all_outputs") {
        std::string response = "Outputs: ";
        for (bool output : shared_data_.digital_outputs) {
            response += output ? "1 " : "0 ";
        }
        return response;
    }

    else if (cmd == "read_all_input_registers") {
        std::string response = "Input Registers: ";
        for (int reg : shared_data_.input_registers) {
            response += std::to_string(reg) + " ";
        }
        return response;
    }

    else if (cmd == "read_all_output_registers") {
        std::string response = "Output Registers: ";
        for (int reg : shared_data_.output_registers) {
            response += std::to_string(reg) + " ";
        }
        return response;
    }

    else if (cmd == "read_all") {
        std::string response = "Inputs: ";
        for (bool input : shared_data_.digital_inputs) {
            response += input ? "1 " : "0 ";
        }
        response += "\nOutputs: ";
        for (bool output : shared_data_.digital_outputs) {
            response += output ? "1 " : "0 ";
        }
        response += "\nInput Registers: ";
        for (int reg : shared_data_.input_registers) {
            response += std::to_string(reg) + " ";
        }
        response += "\nOutput Registers: ";
        for (int reg : shared_data_.output_registers) {
            response += std::to_string(reg) + " ";
        }
        return response;
    }

    else {
        return "Error: Unknown command";
    }
}