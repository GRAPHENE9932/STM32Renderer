#include "sh1106.h"
#include "i2c.h"

#include <stddef.h>

#define SH1106_ADDRESS 0x3C
#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64

#define SH1106_COMMAND 0x80
#define SH1106_LAST_COMMAND 0x00
#define SH1106_DATA 0xC0
#define SH1106_LAST_DATA 0x40

static const uint8_t INIT_COMMANDS[] = {
    0xAE, // Display OFF
    0x40, // Set display start line to 0
    0xC8, // Set common output scan direction to reverse (vertical flip)
    0xA1, // Set segment re-map to reverse direction (horizontal flip)
    0x81, // The contrast control register to 0xFF (double byte command)
    0xFF,
    0xAF, // Display ON
};

#define SH1106_SET_CURSOR_TX_SIZE 6
#define SH1106_SEND_DISPLAY_DATA_TX_SIZE ((SH1106_WIDTH * 2 + SH1106_SET_CURSOR_TX_SIZE) * SH1106_HEIGHT / 8)
static uint8_t tx_buffer[SH1106_SEND_DISPLAY_DATA_TX_SIZE];

void sh1106_initialize(void) {
    i2c_dma_initialize();

    for (size_t i = 0; i < sizeof(INIT_COMMANDS); i++) {
        tx_buffer[i * 2] = i == sizeof(INIT_COMMANDS) - 1 ? SH1106_LAST_COMMAND : SH1106_COMMAND;
        tx_buffer[i * 2 + 1] = INIT_COMMANDS[i];
    }

    i2c_dma_send(SH1106_ADDRESS, tx_buffer, sizeof(INIT_COMMANDS) * 2);
}

static uint32_t sh1106_set_cursor_position(uint8_t* buffer, uint8_t x, uint8_t page, bool last_command) {
    x += 2;

    uint32_t i = 0;
    buffer[i++] = SH1106_COMMAND;
    buffer[i++] = 0xB0 + (page & 0x0F);
    buffer[i++] = SH1106_COMMAND;
    buffer[i++] = 0x00 + (x & 0x0F);
    buffer[i++] = last_command ? SH1106_LAST_COMMAND : SH1106_COMMAND;
    buffer[i++] = 0x10 + ((x >> 4) & 0x0F);

    return i;
}

// Converts the data to the format that the display expects (a byte - is a vertical line, 8 pixels high).
static uint8_t get_line_byte(const uint8_t* data, uint8_t col, uint8_t page) {
    uint8_t result = 0x00;

    for (uint_fast8_t bit = 0; bit < 8; bit++) {
        result |= data[(page * 8 + bit) * (SH1106_WIDTH / 8) + col / 8] & (1 << (7 - (col % 8))) ? 1 << bit : 0x00;
    }

    return result;
}

void sh1106_send_display_data(const uint8_t* data) {
    uint32_t offset_in_buffer = 0;

    for (uint_fast8_t page = 0; page < SH1106_HEIGHT / 8; page++) {
        offset_in_buffer += sh1106_set_cursor_position(tx_buffer + offset_in_buffer, 0, page, false);

        for (uint_fast8_t col = 0; col < SH1106_WIDTH; col++) {
            uint8_t byte_to_send = get_line_byte(data, col, page);

            tx_buffer[offset_in_buffer++] = (col == SH1106_WIDTH - 1 && page == SH1106_HEIGHT / 8 - 1) ? SH1106_LAST_DATA : SH1106_DATA;
            tx_buffer[offset_in_buffer++] = byte_to_send;
        }
    }

    i2c_dma_send(SH1106_ADDRESS, tx_buffer, sizeof(tx_buffer));
}
