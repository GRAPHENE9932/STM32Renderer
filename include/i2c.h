#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

void i2c_dma_initialize(void);
void i2c_dma_send(uint8_t address, uint8_t* data_ptr, uint16_t size);
void i2c_dma_wait_for_transfer_to_complete(void);

#endif // I2C_H
