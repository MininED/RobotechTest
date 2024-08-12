#ifndef SERVER_H
#define SERVER_H

#include "modbus_session.h"
#include "tcp_session.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short modbus_port, unsigned short tcp_port, SharedData& shared_data);

private:
    void do_accept_modbus();
    void do_accept_tcp();

    tcp::acceptor modbus_acceptor_;
    tcp::acceptor tcp_acceptor_;
    SharedData& shared_data_;
};

#endif //SERVER_H