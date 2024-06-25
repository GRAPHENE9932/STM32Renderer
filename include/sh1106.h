#ifndef SH1106_H
#define SH1106_H

#include <stdint.h>

void sh1106_initialize(void);
void sh1106_send_display_data(const uint8_t* data);

#endif // SH1106_H
