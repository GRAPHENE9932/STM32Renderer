#include "rendering/frametime_text.h"
#include "rendering/text_drawing.h"
#include "tim2.h"

#include <stdint.h>

#define DIGITS_COUNT 6

// The maximum is 99999.9ms (9 characters)
static char frametime_text_buf[DIGITS_COUNT + 3] = {'0', '0', '0', '0', '0', ',', '0', 'm', 's'};
static uint8_t frametime_text_offset;

static void generate_frametime_text(uint32_t hundreds_of_us) {
    uint8_t log2 = 0;

    uint32_t cur_power_of_10 = 10;
    for (uint_fast8_t i = 0; i < DIGITS_COUNT; i++) {
        uint32_t digit = hundreds_of_us;
        digit %= cur_power_of_10;
        digit *= 10;
        digit /= cur_power_of_10;
        cur_power_of_10 *= 10;

        if (digit != 0) {
            log2 = i;
        }

        frametime_text_buf[DIGITS_COUNT - (i == 0 ? 0 : i + 1)] = '0' + digit;
    }

    frametime_text_offset = DIGITS_COUNT - 1 - log2;
}

void draw_frametime_text(uint8_t* color_buffer) {
    generate_frametime_text(tim2_get_measurement_100us());
    tim2_reset();

    draw_text(color_buffer, 0, 0, frametime_text_buf + frametime_text_offset, sizeof(frametime_text_buf) - frametime_text_offset);
}
