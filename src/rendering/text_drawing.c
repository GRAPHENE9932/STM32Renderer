#include "rendering/text_drawing.h"
#include "constants.h"

// 6x6 font. A byte represents a row.
static const uint8_t FONT_NUMBERS[10 * 6] = {
    0b00011111, // 0
    0b00010001,
    0b00010101,
    0b00010101,
    0b00010001,
    0b00011111,

    0b00011100, // 1
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00011111,

    0b00011111, // 2
    0b00000001,
    0b00000001,
    0b00011111,
    0b00010000,
    0b00011111,

    0b00011111, // 3
    0b00000001,
    0b00011111,
    0b00000001,
    0b00000001,
    0b00011111,

    0b00000110, // 4
    0b00001010,
    0b00010010,
    0b00011111,
    0b00000010,
    0b00000010,

    0b00011111, // 5
    0b00010000,
    0b00011111,
    0b00000001,
    0b00000001,
    0b00011111,

    0b00011111, // 6
    0b00010000,
    0b00011111,
    0b00010001,
    0b00010001,
    0b00011111,

    0b00011111, // 7
    0b00010001,
    0b00000001,
    0b00000001,
    0b00000001,
    0b00000001,

    0b00011111, // 8
    0b00010001,
    0b00011111,
    0b00010001,
    0b00010001,
    0b00011111,

    0b00011111, // 9
    0b00010001,
    0b00010001,
    0b00011111,
    0b00000001,
    0b00011111
};

static const uint8_t FONT_COMMA[6] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000100
};

static const uint8_t FONT_LETTER_m[6] = {
    0b00000000,
    0b00011010,
    0b00010101,
    0b00010101,
    0b00010101,
    0b00010101
};

static const uint8_t FONT_LETTER_s[6] = {
    0b00000000,
    0b00001111,
    0b00010000,
    0b00001110,
    0b00000001,
    0b00011110
};

static const uint8_t* get_font_char_ptr(char c) {
    if (c >= '0' && c <= '9') return FONT_NUMBERS + (c - '0') * 6;
    if (c == ',') return FONT_COMMA;
    if (c == 'm') return FONT_LETTER_m;
    if (c == 's') return FONT_LETTER_s;

    return FONT_NUMBERS;
}

static void draw_char(uint8_t* color_buffer, uint32_t x, uint32_t y, char c) {
    const uint8_t* font_char = get_font_char_ptr(c);

    uint_fast8_t first_byte_shift_right = x & 0x00000007; // x % 8
    uint_fast8_t second_byte_shift_left = 8 - first_byte_shift_right;

    for (uint_fast8_t local_y = 0; local_y < 6; local_y++) {
        uint8_t font_row = font_char[local_y];
        uint8_t* first_byte_ptr = color_buffer + (y + local_y) * (BUFFERS_WIDTH / 8) + (x >> 3);
        uint8_t* second_byte_ptr = first_byte_ptr + 1;

        *first_byte_ptr ^= font_row >> first_byte_shift_right;
        *second_byte_ptr ^= font_row << second_byte_shift_left;
    }
}

void draw_text(uint8_t* color_buffer, uint32_t x, uint32_t y, const char* text_buffer, uint32_t text_size) {
    for (uint32_t i = 0; i < text_size; i++) {
        draw_char(color_buffer, x + 6 * i, y, text_buffer[i]);
    }
}
