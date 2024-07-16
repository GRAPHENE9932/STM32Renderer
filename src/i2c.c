#include "i2c.h"

#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_i2c.h>

void i2c_initialize(void) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    
    LL_GPIO_InitTypeDef i2c1_sda;
    i2c1_sda.Pin = LL_GPIO_PIN_10;
    i2c1_sda.Mode = LL_GPIO_MODE_ALTERNATE;
    i2c1_sda.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    i2c1_sda.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    i2c1_sda.Pull = LL_GPIO_PULL_UP;
    i2c1_sda.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &i2c1_sda);

    LL_GPIO_InitTypeDef i2c1_scl;
    i2c1_scl.Pin = LL_GPIO_PIN_9;
    i2c1_scl.Mode = LL_GPIO_MODE_ALTERNATE;
    i2c1_scl.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    i2c1_scl.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    i2c1_scl.Pull = LL_GPIO_PULL_UP;
    i2c1_scl.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &i2c1_scl);

    LL_I2C_InitTypeDef i2c1_init;
    i2c1_init.PeripheralMode = LL_I2C_MODE_I2C;
    i2c1_init.Timing = 0x2000090E;
    i2c1_init.AnalogFilter = LL_I2C_ANALOGFILTER_DISABLE;
    i2c1_init.DigitalFilter = 0x00;
    i2c1_init.OwnAddress1 = 0x00;
    i2c1_init.TypeAcknowledge = LL_I2C_ACK;
    i2c1_init.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;

    LL_I2C_Init(I2C1, &i2c1_init);
    LL_I2C_SetMasterAddressingMode(I2C1, LL_I2C_ADDRESSING_MODE_7BIT);
    LL_I2C_Enable(I2C1);
}

struct i2c_tx_handle i2c_begin_master_transmission(uint8_t address, uint32_t size) {
    while (LL_I2C_IsActiveFlag_BUSY(I2C1));

    LL_I2C_HandleTransfer(
        I2C1,
        address << 1,
        LL_I2C_ADDRSLAVE_7BIT,
        size <= 255 ? size : 255,
        size <= 255 ? LL_I2C_MODE_AUTOEND : LL_I2C_MODE_RELOAD,
        LL_I2C_GENERATE_START_WRITE
    );

    struct i2c_tx_handle handle = {
        .bytes_left = size,
        .bytes_left_in_batch = size <= 255 ? size : 255,
        .with_reload = size <= 255
    };

    return handle;
}

void i2c_send_next_byte(struct i2c_tx_handle* handle, uint8_t byte) {
    if (handle->bytes_left_in_batch == 0) {
        while (!handle->with_reload && !LL_I2C_IsActiveFlag_TCR(I2C1));

        handle->bytes_left_in_batch = handle->bytes_left <= 255 ? handle->bytes_left : 255;
        LL_I2C_SetTransferSize(I2C1, handle->bytes_left_in_batch);

        if (handle->bytes_left <= 255) {
            LL_I2C_DisableReloadMode(I2C1);
            LL_I2C_EnableAutoEndMode(I2C1);
        }
        else {
            LL_I2C_EnableReloadMode(I2C1);
        }
    }

    while (!LL_I2C_IsActiveFlag_TXIS(I2C1));

    LL_I2C_TransmitData8(I2C1, byte);

    handle->bytes_left--;
    handle->bytes_left_in_batch--;
}
