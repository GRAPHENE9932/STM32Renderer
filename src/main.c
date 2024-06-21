#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"

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

    while (1) {
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_11);
        for (volatile int i = 0; i < 100000; i++) {
            
        }
    }
}
