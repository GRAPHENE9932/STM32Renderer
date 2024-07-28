#include "constants.h"
#include "sh1106.h"

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_rcc.h"
#include "tim2.h"

static uint8_t color_buffer[BUFFERS_HEIGHT * BUFFERS_WIDTH / 8];

void draw_frame(uint8_t* color_buffer);

static void switch_to_hsi48(void) {
    LL_RCC_HSI48_Enable();
    while (!LL_RCC_HSI48_IsReady());
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI48);
}

int main(void) {
    switch_to_hsi48();

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_InitTypeDef blinky_port;
    blinky_port.Pin = LL_GPIO_PIN_3;
    blinky_port.Mode = LL_GPIO_MODE_OUTPUT;
    blinky_port.Speed = LL_GPIO_SPEED_FREQ_LOW;
    blinky_port.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    blinky_port.Pull = LL_GPIO_PULL_NO;
    blinky_port.Alternate = LL_GPIO_AF_0;

    LL_GPIO_Init(GPIOB, &blinky_port);

    sh1106_initialize();
    tim2_initialize();

    while (1) {
        draw_frame(color_buffer);
        sh1106_send_display_data(color_buffer);
    }
}
