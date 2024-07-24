#include "i2c.h"

#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_i2c.h>
#include <stm32f0xx_ll_dma.h>

static void initialize_i2c(void) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
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
    LL_I2C_DisableClockStretching(I2C1);
    LL_I2C_EnableDMAReq_TX(I2C1);

    LL_I2C_EnableIT_TC(I2C1);
    NVIC_EnableIRQ(I2C1_IRQn);

    LL_I2C_Enable(I2C1);
}

static void initialize_dma() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    LL_DMA_InitTypeDef dma_init_struct;
    dma_init_struct.PeriphOrM2MSrcAddress = LL_I2C_DMA_GetRegAddr(I2C1, LL_I2C_DMA_REG_DATA_TRANSMIT);
    dma_init_struct.MemoryOrM2MDstAddress = 0;
    dma_init_struct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma_init_struct.Mode = LL_DMA_MODE_NORMAL;
    dma_init_struct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma_init_struct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma_init_struct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    dma_init_struct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    dma_init_struct.NbData = 0;
    dma_init_struct.Priority = LL_DMA_PRIORITY_LOW;
    LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &dma_init_struct);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
}

void i2c_dma_initialize(void) {
    initialize_dma();
    initialize_i2c();
}

static uint8_t* cur_data_ptr = 0;
static uint16_t bytes_left = 0;
static uint8_t cur_i2c_address = 0;

static void send_batch(void) {
    if (bytes_left == 0) {
        return;
    }

    bool reload = bytes_left > 255;

    uint8_t batch_size = reload ? 255 : bytes_left;
    bytes_left -= batch_size;

    LL_I2C_DisableDMAReq_TX(I2C1);

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, batch_size);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)cur_data_ptr);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

    LL_I2C_SetTransferSize(I2C1, batch_size);

    if (reload) {
        LL_I2C_EnableReloadMode(I2C1);
    }
    else {
        LL_I2C_DisableReloadMode(I2C1);
        LL_I2C_EnableAutoEndMode(I2C1);
    }

    LL_I2C_EnableDMAReq_TX(I2C1);

    cur_data_ptr += batch_size;
}

void i2c_dma_send(uint8_t address, uint8_t* data_ptr, uint16_t size) {
    while (bytes_left != 0);

    cur_data_ptr = data_ptr;
    bytes_left = size;
    cur_i2c_address = address;

    LL_I2C_HandleTransfer(
        I2C1,
        address << 1,
        LL_I2C_ADDRSLAVE_7BIT,
        size > 255 ? 255 : size,
        size > 255 ? LL_I2C_MODE_RELOAD : LL_I2C_MODE_AUTOEND,
        LL_I2C_GENERATE_START_WRITE
    );

    send_batch();
}

void dma1_ch2_3_handler(void) {
    LL_DMA_ClearFlag_TC2(DMA1);
}

void i2c1_handler(void) {
    if (!LL_I2C_IsActiveFlag_TCR(I2C1)) {
        return;
    }

    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_3);
    send_batch();
}
