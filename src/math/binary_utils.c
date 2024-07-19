#include "math/binary_utils.h"

uint32_t trailing_zeroes(uint32_t x) {
    uint32_t count = 0;
    uint32_t cur_mask = 0xFFFFFFFF;
    uint32_t cur_power = 32;

    while (cur_power != 0) {
        cur_power >>= 1;
        cur_mask <<= cur_power;
        if (!(x & cur_mask)) {
            count += cur_power;
            x <<= cur_power;
        }
    }

    return count;
}
