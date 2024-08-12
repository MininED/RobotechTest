#include "../include/server.h"

Server::Server(boost::asio::io_context& io_context, unsigned short modbus_port, unsigned short tcp_port, SharedData& shared_data)
    : modbus_acceptor_(io_context, tcp::endpoint(tcp::v4(), modbus_port)),
      tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), tcp_port)),
      shared_data_(shared_data) {
    do_accept_modbus();
    do_accept_tcp();
}

void Server::do_accept_modbus() {
    modbus_acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<ModbusSession>(std::move(socket), shared_data_)->start();
        }
        do_accept_modbus();
    });
}

void Server::do_accept_tcp() {
    tcp_acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<TCPSession>(std::move(socket), shared_data_)->start();
        }
        do_accept_tcp();
    });
}