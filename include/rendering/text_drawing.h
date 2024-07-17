#ifndef TEXT_DRAWING_H
#define TEXT_DRAWING_H

#include <stdint.h>

void draw_text(uint8_t* color_buffer, uint32_t x, uint32_t y, const char* text_buffer, uint32_t text_size);

#endif //TEXT_DRAWING_H
