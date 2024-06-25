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

void sh1106_initialize(void) {
    struct i2c_tx_handle handle = i2c_begin_master_transmission(SH1106_ADDRESS, sizeof(INIT_COMMANDS) * 2);

    for (size_t i = 0; i < sizeof(INIT_COMMANDS); i++) {
        i2c_send_next_byte(&handle, i == sizeof(INIT_COMMANDS) - 1 ? SH1106_LAST_COMMAND : SH1106_COMMAND);
        i2c_send_next_byte(&handle, INIT_COMMANDS[i]);
    }
}

#define SH1106_SET_CURSOR_TX_SIZE 6
static void sh1106_set_cursor_position(struct i2c_tx_handle* handle, uint8_t x, uint8_t page, bool last_command) {
    x += 2;

    i2c_send_next_byte(handle, SH1106_COMMAND);
    i2c_send_next_byte(handle, 0xB0 + (page & 0x0F)); // Set page address
    i2c_send_next_byte(handle, SH1106_COMMAND);
    i2c_send_next_byte(handle, 0x00 + (x & 0x0F)); // Set lower column address
    i2c_send_next_byte(handle, last_command ? SH1106_LAST_COMMAND : SH1106_COMMAND);
    i2c_send_next_byte(handle, 0x10 + ((x >> 4) & 0x0F)); // Set higher column address
}

// Converts the data to the format that the display expects (a byte - is a vertical line, 8 pixels high).
static uint8_t get_line_byte(const uint8_t* data, uint8_t col, uint8_t page) {
    uint8_t result = 0x00;

    for (uint_fast8_t bit = 0; bit < 8; bit++) {
        result |= data[(page * 8 + bit) * (SH1106_WIDTH / 8) + col / 8] & (1 << (7 - (col % 8))) ? 1 << bit : 0x00;
    }

    return result;
}

#define SH1106_SEND_DISPLAY_DATA_TX_SIZE ((SH1106_WIDTH * 2 + SH1106_SET_CURSOR_TX_SIZE) * SH1106_HEIGHT / 8)
void sh1106_send_display_data(const uint8_t* data) {
    struct i2c_tx_handle handle = i2c_begin_master_transmission(SH1106_ADDRESS, SH1106_SEND_DISPLAY_DATA_TX_SIZE);

    for (uint_fast8_t page = 0; page < SH1106_HEIGHT / 8; page++) {
        sh1106_set_cursor_position(&handle, 0, page, false);
        
        for (uint_fast8_t col = 0; col < SH1106_WIDTH; col++) {
            uint8_t byte_to_send = get_line_byte(data, col, page);

            i2c_send_next_byte(&handle, (col == SH1106_WIDTH - 1 && page == SH1106_HEIGHT / 8 - 1) ? SH1106_LAST_DATA : SH1106_DATA);
            i2c_send_next_byte(&handle, byte_to_send);
        }
    }
}
