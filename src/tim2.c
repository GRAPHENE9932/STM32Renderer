#include "tim2.h"
#include "constants.h"

#include <stm32f0xx_ll_tim.h>
#include <stm32f0xx_ll_bus.h>

void tim2_initialize(void) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    LL_TIM_InitTypeDef init_struct;
    init_struct.Prescaler = CK_INT_FREQ / 10000; // CK_CNT increments every 100 us.
    init_struct.CounterMode = LL_TIM_COUNTERMODE_UP;
    init_struct.Autoreload = 0xFFFFFFFF;
    init_struct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    init_struct.RepetitionCounter = 0;

    LL_TIM_Init(TIM2, &init_struct);
    LL_TIM_EnableCounter(TIM2);
}

void tim2_reset(void) {
    LL_TIM_SetCounter(TIM2, 0);
}

uint32_t tim2_get_measurement_100us(void) {
    return LL_TIM_GetCounter(TIM2);
}
