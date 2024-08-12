#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <vector>
#include <mutex>

struct SharedData {
    std::vector<uint8_t> digital_inputs;
    std::vector<uint8_t> digital_outputs;
    std::vector<uint16_t> input_registers;
    std::vector<uint16_t> output_registers;

    mutable std::mutex mutex; // For thread-safe access
};

//Random initialization
inline void initialize_shared_data(SharedData& shared_data) {
    std::srand(static_cast<unsigned>(std::time(0)));

    size_t num_digital_inputs = 10;
    size_t num_digital_outputs = 10;
    size_t num_input_registers = 5;
    size_t num_output_registers = 5;

    shared_data.digital_inputs.resize(num_digital_inputs);
    shared_data.digital_outputs.resize(num_digital_outputs);
    shared_data.input_registers.resize(num_input_registers);
    shared_data.output_registers.resize(num_output_registers);

    for (int i = 0; i < num_digital_inputs; ++i) {
        shared_data.digital_inputs[i] = std::rand() % 2;
    }
    for (int i = 0; i < num_digital_outputs; ++i) {
        shared_data.digital_outputs[i] = std::rand() % 2;
    }
    for (int i = 0; i < num_input_registers; ++i) {
        shared_data.input_registers[i] = std::rand() % 65536;
    }
    for (int i = 0; i < num_output_registers; ++i) {
        shared_data.output_registers[i] = std::rand() % 65536;
    }
}

#endif //SHARED_DATA_H
