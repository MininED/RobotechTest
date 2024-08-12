#include "../include/modbus_session.h"

ModbusSession::ModbusSession(tcp::socket socket, SharedData& shared_data)
    : socket_(std::move(socket)), shared_data_(shared_data) {
    modbus_ctx_ = modbus_new_tcp_pi(nullptr, nullptr);
    modbus_set_socket(modbus_ctx_, socket_.native_handle());

    modbus_mapping_ = modbus_mapping_new(0,0,0,0);

    std::lock_guard<std::mutex> lock(shared_data_.mutex);

    //Setting pointers to shared_data
    modbus_mapping_->tab_input_bits = shared_data_.digital_inputs.data();
    modbus_mapping_->nb_input_bits = shared_data_.digital_inputs.size();

    modbus_mapping_->tab_bits = shared_data_.digital_outputs.data();
    modbus_mapping_->nb_bits = shared_data_.digital_outputs.size();

    modbus_mapping_->tab_input_registers = shared_data_.input_registers.data();
    modbus_mapping_->nb_input_registers = shared_data_.input_registers.size();

    modbus_mapping_->tab_registers = shared_data_.output_registers.data();
    modbus_mapping_->nb_registers = shared_data_.output_registers.size();
}



//Moving session to separated thread
void ModbusSession::start() {
    auto self = shared_from_this();
    std::thread([self]() {
        self->do_session();
    }).detach();
}

//Handling session
void ModbusSession::do_session() {
    try {
        std::cout << "Modbus client connected from " << socket_.remote_endpoint().address().to_string()
                  << ":" << socket_.remote_endpoint().port() << std::endl;

        while (true) {
            uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
            int rc = modbus_receive(modbus_ctx_, query);

            if (rc > 0) {
                //Log input
                std::ostringstream received;
                received << "Modbus input from: " << socket_.remote_endpoint().address().to_string()
                            << ":" << socket_.remote_endpoint().port() << " - ";
                for (int i=0; i < rc; ++i) {
                    received << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(query[i]) << " ";
                }
                std::cout << received.str() << std::endl;

                //Send reply
                std::lock_guard<std::mutex> lock(shared_data_.mutex);
                rc = modbus_reply(modbus_ctx_, query, rc, modbus_mapping_);
                
                //Log reply
                std::ostringstream sent;
                sent << "Modbus output to: " << socket_.remote_endpoint().address().to_string()
                            << ":" << socket_.remote_endpoint().port() << " - ";
                for (int i=0; i < rc; ++i) {
                    sent << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(query[i]) << " ";
                }
                std::cout << sent.str() << std::endl;
            } else if (rc == -1) {
                std::cerr << "Modbus client disconnected from " << socket_.remote_endpoint().address().to_string()
                            << ":" << socket_.remote_endpoint().port() << std::endl;
                break;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in Modbus session thread: " << e.what() << std::endl;
    }
}