#include "math/binary_utils.h"

#include <stdbool.h>
#include <stdint.h>

__attribute__((used)) uint64_t __aeabi_llsl(uint64_t a, uint32_t b) {
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

__attribute__((used)) uint64_t __aeabi_uidivmod(uint32_t numerator, uint32_t denominator) {
    uint32_t quot = 0;
    uint32_t cur_power = trailing_zeroes(denominator);
    uint32_t rem = numerator;

    while (true) {
        if ((denominator << cur_power) <= rem) {
            quot |= 1 << cur_power;
            rem -= denominator << cur_power;
        }

        if (cur_power-- == 0) {
            break;
        }
    }

    // Run-time ABI for Arm32 requires the __aeabi_uidivmod function to
    // output the quotient to the R0 and the remainder to the R1 register.
    // Procedure Call Standard for the Arm Architecture says:
    // "A double-word sized Fundamental Data Type (e.g., long long,
    // double and 64-bit containerized vectors) is returned in r0 and r1."
    // So, return an uint64 with a quotient as a lower half and a remainder as a higher half.
    uint64_t result = (uint64_t)quot | ((uint64_t)rem << 32);
    return result;
}

__attribute__((used)) uint32_t __aeabi_uidiv(uint32_t numerator, uint32_t denominator) {
    return __aeabi_uidivmod(numerator, denominator);
}
