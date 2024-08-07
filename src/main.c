#include "constants.h"
#include "sh1106.h"

#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "tim2.h"

static uint8_t color_buffer[BUFFERS_HEIGHT * BUFFERS_WIDTH / 8];

void draw_frame(uint8_t* color_buffer);

static void switch_to_hsi48(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

    LL_RCC_HSI48_Enable();
    while (!LL_RCC_HSI48_IsReady());
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI48);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI48);
}

int main(void) {
    switch_to_hsi48();

    sh1106_initialize();
    tim2_initialize();

    while (1) {
        draw_frame(color_buffer);
        sh1106_send_display_data(color_buffer);
    }
}
