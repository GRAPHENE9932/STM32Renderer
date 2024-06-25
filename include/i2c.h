#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

void i2c_initialize(void);

struct i2c_tx_handle {
    uint32_t bytes_left;
    uint8_t bytes_left_in_batch; // A batch has at most 255 bytes.
    bool with_reload;
};

/**
 * @brief Begins the I2C communication and sends the address to the bus.
 *
 * Must be called before all the i2c_send_next_byte calls. After the call
 * i2c_send_next_byte should be called to send a byte. i2c_send_next_byte must be
 * called exactly <size> times.
 *
 * Returns a struct that should be passed to every i2c_send_next_byte call.
 * 
 * @param address Address of the slave.
 * @param size Total amount of data that will be sent.
 * @return struct i2c_tx_handle 
 */
struct i2c_tx_handle i2c_begin_master_transmission(uint8_t address, uint32_t size);
void i2c_send_next_byte(struct i2c_tx_handle* handle, uint8_t byte);

#endif // I2C_H
