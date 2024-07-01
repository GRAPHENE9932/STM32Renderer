#include <stdint.h>

uint64_t __aeabi_llsl(uint64_t a, uint32_t b) {
    uint32_t* low = (uint32_t*)&a;
    uint32_t* high = low + 1;

    if (b < 32) {
        *high <<= b;
        *high |= *low >> (32 - b);
        *low <<= b;
    }
    else {
        *low = 0;
        *high <<= b - 32;
    }

    return a;
}
