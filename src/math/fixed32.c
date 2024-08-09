#include "math/fixed32.h"
#include "math/binary_utils.h"

#include <stdbool.h>

static uint32_t fixed32_mul_unsigned(uint32_t a, uint32_t b) {
    return (((a >> 16) * (b >> 16)) << 16) +
        ((a >> 16) * (b & 0x0000FFFF)) +
        ((b >> 16) * (a & 0x0000FFFF)) +
        (((a & 0x0000FFFF) * (b & 0x0000FFFF)) >> 16);
}

fixed32 fixed32_mul(fixed32 a, fixed32 b) {
    if (a < 0 && b >= 0) {
        return -fixed32_mul_unsigned(-a, b);
    }

    if (a >= 0 && b < 0) {
        return -fixed32_mul_unsigned(a, -b);
    }

    if (a < 0 && b < 0) {
        return fixed32_mul_unsigned(-a, -b);
    }

    return fixed32_mul_unsigned(a, b);
}

static uint32_t div_48_bit_int_by_32_bit_int(uint64_t a, uint64_t b) {
    uint32_t result = 0;
    uint32_t cur_power = trailing_zeroes(b) + 16;

    while (true) {
        if ((b << cur_power) <= a) {
            result |= 1 << cur_power;
            a -= b << cur_power;
        }

        if (cur_power-- == 0) {
            break;
        }
    }

    return result;
}

fixed32 fixed32_div(fixed32 a, fixed32 b) {
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

static const fixed32 INV_SQRT_TABLE[] = {
    0x7FFFFFFF, // 1 / sqrt(0)
    0x01000000, // 1 / sqrt(1/65536)
    0x00B504F3, // 1 / sqrt(2/65536)
    0x00800000, // 1 / sqrt(4/65536)
    0x005A8279, // 1 / sqrt(8/65536)
    0x00400000, // And so on
    0x002D413C,
    0x00200000,
    0x0016A09E,
    0x00100000,
    0x000B504F,
    0x00080000,
    0x0005A827,
    0x00040000,
    0x0002D413,
    0x00020000,
    0x00016A09,
    0x00010000,
    0x0000B504,
    0x00008000,
    0x00005A82,
    0x00004000,
    0x00002D41,
    0x00002000,
    0x000016A0,
    0x00001000,
    0x00000B50,
    0x00000800,
    0x000005A8,
    0x00000400,
    0x000002D4,
    0x00000200,
    0x0000016A // 1 / sqrt(32768)
};

static fixed32 fixed32_estimate_inv_sqrt(fixed32 num) {
    // Special case, that can break the further logic.
    if (num == 0) {
        return 0;
    }

    uint32_t i_begin = 32 - trailing_zeroes(num);
    uint32_t i_end = i_begin + 1;

    return INV_SQRT_TABLE[i_begin] + fixed32_mul((num - (1 << (i_begin - 1))) >> (i_begin - 1), INV_SQRT_TABLE[i_end]);
}

fixed32 fixed32_sqrt(fixed32 num) {
    fixed32 y = fixed32_estimate_inv_sqrt(num);

    fixed32 b = num;
    fixed32 res = fixed32_mul_unsigned(num, y);

    for (uint32_t i = 0; i < 4; i++) { // 4 iterations are tested to be enough for 2% of relative precision.
        b = fixed32_mul_unsigned(fixed32_mul_unsigned(b, y), y);
        y = (FIXED32_CONST(3, 0, 0) - b) >> 1;
        res = fixed32_mul_unsigned(res, y);
    }

    return res;
}

fixed32 fixed32_inv_sqrt(fixed32 num) {
    fixed32 y = fixed32_estimate_inv_sqrt(num);

    fixed32 b = num;
    fixed32 res = y;

    for (uint32_t i = 0; i < 4; i++) { // 4 iterations are tested to be enough for 2% of relative precision.
        b = fixed32_mul_unsigned(fixed32_mul_unsigned(b, y), y);
        y = (FIXED32_CONST(3, 0, 0) - b) >> 1;
        res = fixed32_mul_unsigned(res, y);
    }

    return res;
}
