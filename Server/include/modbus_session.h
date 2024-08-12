#ifndef MODBUS_SESSION_H
#define MODBUS_SESSION_H

#include <boost/asio.hpp>
#include <modbus/modbus.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include "shared_data.h"


using boost::asio::ip::tcp;

class ModbusSession : public std::enable_shared_from_this<ModbusSession> {
public:
    ModbusSession(tcp::socket socket, SharedData& shared_data);
    //~ModbusSession();

    void start();

private:
    void do_session();

    tcp::socket socket_;
    SharedData& shared_data_;
    modbus_t* modbus_ctx_;
    modbus_mapping_t* modbus_mapping_;
};

#endif //MODBUS_SESSION_H
