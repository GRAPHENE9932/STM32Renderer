#include "constants.h"
#include "i2c.h"
#include "sh1106.h"

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "tim2.h"

static uint8_t color_buffer[BUFFERS_HEIGHT * BUFFERS_WIDTH / 8];

void draw_frame(uint8_t* color_buffer);

int main(void) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    LL_GPIO_InitTypeDef blinky_port;
    blinky_port.Pin = LL_GPIO_PIN_11;
    blinky_port.Mode = LL_GPIO_MODE_OUTPUT;
    blinky_port.Speed = LL_GPIO_SPEED_FREQ_LOW;
    blinky_port.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    blinky_port.Pull = LL_GPIO_PULL_NO;
    blinky_port.Alternate = LL_GPIO_AF_0;

    LL_GPIO_Init(GPIOA, &blinky_port);

    i2c_initialize();
    sh1106_initialize();
    tim2_initialize();

    while (1) {
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_11);
        draw_frame(color_buffer);
        sh1106_send_display_data(color_buffer);
    }
}
