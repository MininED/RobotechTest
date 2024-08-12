#include "../include/tcp_client.h"
#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

TCPClient::TCPClient(boost::asio::io_context& io_context)
    : socket_(io_context), is_connected_(false), is_cyclic_reading_(false), io_context_(io_context) {}

void TCPClient::connect(const std::string& host, const std::string& port) {
    if (is_connected_) {
        std::cout << "Already connected to a server.\n";
        return;
    }

    try {
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::connect(socket_, resolver.resolve(host, port));
        is_connected_ = true;
        current_host_ = host;
        current_port_ = port;
        std::cout << "Connected to " << host << ":" << port << "\n";
    } catch (std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << "\n";
    }
}

void TCPClient::disconnect() {
    if (is_connected_) {
        socket_.close();
        is_connected_ = false;
        std::cout << "Disconnected from server.\n";
    } else {
        std::cout << "Not connected to any server.\n";
    }
}

void TCPClient::send(const std::string& message) {
    if (is_connected_) {
        try {
            boost::asio::write(socket_, boost::asio::buffer(message + "\n"));
        } catch (std::exception& e) {
            std::cerr << "Send failed: " << e.what() << "\n";
            is_connected_ = false;
        }
    } else {
        std::cout << "Not connected to any server.\n";
    }
}

std::string TCPClient::receive() {
    if (is_connected_) {
        try {
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket_, buffer, "\n");
            return boost::asio::buffer_cast<const char*>(buffer.data());
        } catch (std::exception& e) {
            std::cerr << "Receive failed: " << e.what() << "\n";
            is_connected_ = false;
            return "Error: Not connected to any server.\n";
        }
    } else {
        return "Error: Not connected to any server.\n";
    }
}

void TCPClient::start_cyclic_reading() {
    if (is_cyclic_reading_) {
        std::cout << "Cyclic reading is already started.\n";
        return;
    }
    is_cyclic_reading_ = true;
    cyclic_read_thread_ = std::thread([this]() {
        while (is_cyclic_reading_) {
            if (is_connected_) {
                send("read_all");
                std::string reply = receive();
                std::cout << reply << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void TCPClient::stop_cyclic_reading() {
    if (is_cyclic_reading_) {
        is_cyclic_reading_ = false;
        if (cyclic_read_thread_.joinable()) {
            cyclic_read_thread_.join();
        }
        std::cout << "Stopped cyclic reading.\n";
    } else {
        std::cout << "Cyclic reading is not running.\n";
    }
}

void TCPClient::check_connection() {
    if (!is_connected_) {
        std::cout << "Connection lost. Please reconnect.\n";
    }
}
