#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <thread>
#include <boost/asio.hpp>

class TCPClient {
public:
    TCPClient(boost::asio::io_context& io_context);

    void connect(const std::string& host, const std::string& port);
    void disconnect();
    void send(const std::string& message);
    std::string receive();
    void start_cyclic_reading();
    void stop_cyclic_reading();
    void check_connection();

    bool is_cyclic_reading_;
private:
    boost::asio::ip::tcp::socket socket_;
    bool is_connected_;
    boost::asio::io_context& io_context_;
    std::thread cyclic_read_thread_;
    std::string current_host_;
    std::string current_port_;
};

#endif //TCP_CLIENT_H
