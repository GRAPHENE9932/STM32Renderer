#ifndef FIXED32
#define FIXED32

#include <stdint.h>

// 16.16 signed fixed-point number.
typedef int32_t fixed32;

#define FIXED32_INTERNAL_LOG10(x) (\
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

#define FIXED32_INTERNAL_POW(x, p) (\
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

#define FIXED32_INTERNAL_POSITIVE_CONST(integer, fraction, zeroes_preceding_fraction) (\
    (((uint32_t)(integer)) << 16) |\
    ((((uint64_t)(fraction)) << (16U - (FIXED32_INTERNAL_LOG10(fraction) + zeroes_preceding_fraction))) / \
    FIXED32_INTERNAL_POW(5, (FIXED32_INTERNAL_LOG10(fraction) + zeroes_preceding_fraction)))\
)

/**
 * @brief Create a fixed32 constant from the human-readable (almost) representation.
 *
 * If the integer OR fraction part is negative, the constant will be negative.
 * If the described number cannot be represented with fixed32 exactly, the result will be rounded down.
 * Examples:
 * - FIXED32_CONST(31, 125, 0) // 31.125
 * - FIXED32_CONST(-31, 125, 0) // -31.125
 * - FIXED32_CONST(0, 1373291015625, 3) // 0.0001373291015625
 * - FIXED32_CONST(0, -261688232421875, 1) // -0.0261688232421875
 * - FIXED32_CONST(0, 9999000000000000, 0) // 0.9998931884765625
 * 
 * @param integer The integer part of the number.
 * @param fraction The fraction part of the number (without the preceding zeroes).
 * @param zeroes_preceding_fraction The number of zeroes preceding the fraction part.
 */
#define FIXED32_CONST(integer, fraction, zeroes_preceding_fraction) ((fixed32)(\
    ((integer) < 0) ? -FIXED32_INTERNAL_POSITIVE_CONST(-(integer), fraction, zeroes_preceding_fraction) :\
    ((fraction) < 0) ? -FIXED32_INTERNAL_POSITIVE_CONST(integer, -(fraction), zeroes_preceding_fraction) :\
    FIXED32_INTERNAL_POSITIVE_CONST(integer, fraction, zeroes_preceding_fraction)\
))

#define FIXED32_ZERO ((fixed32)0x00000000)
#define FIXED32_ONE ((fixed32)0x00010000)
#define FIXED32_MINUS_ONE ((fixed32)0xFFFEFFFF)
#define FIXED32_MAX ((fixed32)0x7FFFFFFF)
#define FIXED32_MIN ((fixed32)0x80000000)

fixed32 fixed32_mul(fixed32 a, fixed32 b);
fixed32 fixed32_div(fixed32 a, fixed32 b);

fixed32 fixed32_sqrt(fixed32 num);
fixed32 fixed32_inv_sqrt(fixed32 num);

static inline fixed32 fixed32_from_uint32(uint32_t num) {
    return num << 16;
}

static inline fixed32 fixed32_from_int32(int32_t num) {
    return num << 16;
}

static inline fixed32 fixed32_abs(fixed32 num) {
    return num < FIXED32_ZERO ? -num : num;
}

#endif // FIXED32
