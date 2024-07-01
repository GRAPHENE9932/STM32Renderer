#include "fixed16_16.h"

#include <stdbool.h>

static uint32_t fixed16_16_mul_unsigned(uint32_t a, uint32_t b) {
    return (((a >> 16) * (b >> 16)) << 16) +
        ((a >> 16) * (b & 0x0000FFFF)) +
        ((b >> 16) * (a & 0x0000FFFF)) +
        (((a & 0x0000FFFF) * (b & 0x0000FFFF)) >> 16);
}

fixed16_16 fixed16_16_mul(fixed16_16 a, fixed16_16 b) {
    if (a < 0 && b >= 0) {
        return -fixed16_16_mul_unsigned(-a, b);
    }

    if (a >= 0 && b < 0) {
        return -fixed16_16_mul_unsigned(a, -b);
    }

    if (a < 0 && b < 0) {
        return fixed16_16_mul_unsigned(-a, -b);
    }

    return fixed16_16_mul_unsigned(a, b);
}

static uint32_t trailing_zeroes(uint32_t x) {
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

static uint32_t div_48_bit_int_by_32_bit_int(uint64_t a, uint64_t b) {
    uint32_t result = 0;
    uint32_t cur_power = trailing_zeroes(b) + 16;

    while (cur_power != 0) {
        if ((b << cur_power) <= a) {
            result |= 1 << cur_power;
            a -= b << cur_power;
        }

        cur_power--;
    }

    return result;
}

fixed16_16 fixed16_16_div(fixed16_16 a, fixed16_16 b) {
    uint32_t mod_a = a < 0 ? -a : a;
    uint64_t shifted_a = ((uint64_t)mod_a) << 16;

    if (a < 0 && b >= 0) {
        return -div_48_bit_int_by_32_bit_int(shifted_a, b);
    }

    if (a >= 0 && b < 0) {
        return -div_48_bit_int_by_32_bit_int(shifted_a, -b);
    }

    if (a < 0 && b < 0) {
        return div_48_bit_int_by_32_bit_int(shifted_a, -b);
    }

    return div_48_bit_int_by_32_bit_int(shifted_a, b);
}
