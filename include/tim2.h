#ifndef TIM2_H
#define TIM2_H

#include <stdint.h>

void tim2_initialize(void);
void tim2_reset(void);

// Returns a number of hundreds of microseconds passed since the last reset.
uint32_t tim2_get_measurement_100us(void);

#endif // TIM2_H
