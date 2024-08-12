#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "shared_data.h"

using boost::asio::ip::tcp;

class TCPSession : public std::enable_shared_from_this<TCPSession> {
public:
    TCPSession(tcp::socket socket, SharedData& shared_data);

    void start();

private:
    void do_session();
    std::string handle_tcp_command(const std::string& command);

    tcp::socket socket_;
    SharedData& shared_data_;
};

#endif //TCP_SESSION_H
