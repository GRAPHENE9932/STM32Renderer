#ifndef FIXED16_16
#define FIXED16_16

#include <stdint.h>

// 16.16 signed fixed-point number.
typedef int32_t fixed16_16;

#define FIXED16_16_INTERNAL_LOG10(x) (\
    ((uint64_t)(x)) == 0U ? 0 :\
    ((uint64_t)(x)) < 10U ? 1 :\
    ((uint64_t)(x)) < 100U ? 2 :\
    ((uint64_t)(x)) < 1000U ? 3 :\
    ((uint64_t)(x)) < 10000U ? 4 :\
    ((uint64_t)(x)) < 100000U ? 5 :\
    ((uint64_t)(x)) < 1000000U ? 6 :\
    ((uint64_t)(x)) < 10000000U ? 7 :\
    ((uint64_t)(x)) < 100000000U ? 8 :\
    ((uint64_t)(x)) < 1000000000U ? 9 :\
    ((uint64_t)(x)) < 10000000000U ? 10 :\
    ((uint64_t)(x)) < 100000000000U ? 11 :\
    ((uint64_t)(x)) < 1000000000000U ? 12 :\
    ((uint64_t)(x)) < 10000000000000U ? 13 :\
    ((uint64_t)(x)) < 100000000000000U ? 14 :\
    ((uint64_t)(x)) < 1000000000000000U ? 15 :\
    ((uint64_t)(x)) < 10000000000000000U ? 16 :\
    ((uint64_t)(x)) < 100000000000000000U ? 17 :\
    ((uint64_t)(x)) < 1000000000000000000U ? 18 :\
    ((uint64_t)(x)) < 10000000000000000000U ? 19 : -1\
)

#define FIXED16_16_INTERNAL_POW(x, p) (\
    ((p) == 0) ? 1 :\
    ((p) == 1) ? ((uint64_t)(x)) :\
    ((p) == 2) ? ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 3) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 4) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 5) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) :\
    ((p) == 6) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 7) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 8) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 9) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) :\
    ((p) == 10) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 11) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 12) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 13) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) :\
    ((p) == 14) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 15) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) :\
    ((p) == 16) ? ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) *\
    ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) * ((uint64_t)(x)) : 0xFFFFFFFFFFFFFFFF\
)

#define FIXED16_16_INTERNAL_POSITIVE_CONST(integer, fraction, zeroes_preceding_fraction) (\
    (((uint32_t)(integer)) << 16) |\
    ((((uint64_t)(fraction)) << (16U - (FIXED16_16_INTERNAL_LOG10(fraction) + zeroes_preceding_fraction))) / \
    FIXED16_16_INTERNAL_POW(5, (FIXED16_16_INTERNAL_LOG10(fraction) + zeroes_preceding_fraction)))\
)

/**
 * @brief Create a fixed16_16 constant from the human-readable (almost) representation.
 *
 * If the integer OR fraction part is negative, the constant will be negative.
 * If the described number cannot be represented with fixed16_16 exactly, the result will be rounded down.
 * Examples:
 * - FIXED16_16_CONST(31, 125, 0) // 31.125
 * - FIXED16_16_CONST(-31, 125, 0) // -31.125
 * - FIXED16_16_CONST(0, 1373291015625, 3) // 0.0001373291015625
 * - FIXED16_16_CONST(0, -261688232421875, 1) // -0.0261688232421875
 * - FIXED16_16_CONST(0, 9999000000000000, 0) // 0.9998931884765625
 * 
 * @param integer The integer part of the number.
 * @param fraction The fraction part of the number (without the preceding zeroes).
 * @param zeroes_preceding_fraction The number of zeroes preceding the fraction part.
 */
#define FIXED16_16_CONST(integer, fraction, zeroes_preceding_fraction) ((fixed16_16)(\
    ((integer) < 0) ? -FIXED16_16_INTERNAL_POSITIVE_CONST(-(integer), fraction, zeroes_preceding_fraction) :\
    ((fraction) < 0) ? -FIXED16_16_INTERNAL_POSITIVE_CONST(integer, -(fraction), zeroes_preceding_fraction) :\
    FIXED16_16_INTERNAL_POSITIVE_CONST(integer, fraction, zeroes_preceding_fraction)\
))

#define FIXED16_16_ZERO ((fixed16_16)0x00000000)
#define FIXED16_16_ONE ((fixed16_16)0x00010000)
#define FIXED16_16_MINUS_ONE ((fixed16_16)0xFFFEFFFF)
#define FIXED16_16_MAX ((fixed16_16)0x7FFFFFFF)
#define FIXED16_16_MIN ((fixed16_16)0x80000000)

fixed16_16 fixed16_16_mul(fixed16_16 a, fixed16_16 b);
fixed16_16 fixed16_16_div(fixed16_16 a, fixed16_16 b);

fixed16_16 fixed16_16_sqrt(fixed16_16 num);
fixed16_16 fixed16_16_inv_sqrt(fixed16_16 num);

fixed16_16 fixed16_16_sin(fixed16_16 rad);
fixed16_16 fixed16_16_cos(fixed16_16 rad);

static inline fixed16_16 fixed16_16_from_uint32(uint32_t num) {
    return num << 16;
}

static inline fixed16_16 fixed16_16_from_int32(int32_t num) {
    return num << 16;
}

static inline fixed16_16 fixed16_16_abs(fixed16_16 num) {
    return num < FIXED16_16_ZERO ? -num : num;
}

#endif // FIXED16_16
