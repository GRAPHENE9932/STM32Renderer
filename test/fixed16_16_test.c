#include "fixed16_16_test.h"
#include "fixed16_16.h"

#include <unity.h>

#include <stdio.h>
#include <math.h>

static double fixed16_16_to_double(fixed16_16 f) {
    return (int32_t)f / 65536.0;
}

static double dabs(double num) {
    return num < 0.0 ? -num : num;
}

void test_fixed16_16_fixed16_16_const(void) {
    TEST_ASSERT_EQUAL((fixed16_16)0x00000000, FIXED16_16_CONST(0, 0, 0)); // 0.0
    TEST_ASSERT_EQUAL((fixed16_16)0x00008000, FIXED16_16_CONST(0, 5, 0)); // 0.5
    TEST_ASSERT_EQUAL((fixed16_16)0x00010000, FIXED16_16_CONST(1, 0, 0)); // 1.0
    TEST_ASSERT_EQUAL((fixed16_16)0x00020000, FIXED16_16_CONST(2, 0, 0)); // 2.0
    TEST_ASSERT_EQUAL((fixed16_16)0x00028000, FIXED16_16_CONST(2, 5, 0)); // 2.5
    TEST_ASSERT_EQUAL((fixed16_16)0x13A8E978, FIXED16_16_CONST(5032, 9119873046875, 0)); // 5032.9119873046875
    TEST_ASSERT_EQUAL((fixed16_16)0x82FFAD85, FIXED16_16_CONST(-32000, 3221893310546875, 0)); // -32000.3221893310546875
    TEST_ASSERT_EQUAL((fixed16_16)0x00000009, FIXED16_16_CONST(0, 1373291015625, 3)); // 0.0001373291015625
    TEST_ASSERT_EQUAL((fixed16_16)0xFFFFF94D, FIXED16_16_CONST(0, -261688232421875, 1)); // -0.0261688232421875
    TEST_ASSERT_EQUAL((fixed16_16)0x0000FFF9, FIXED16_16_CONST(0, 9999000000000000, 0)); // ~0.9999
}

void test_fixed16_16_mul(void) {
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 0, 0), fixed16_16_mul(FIXED16_16_CONST(0, 0, 0), FIXED16_16_CONST(0, 0, 0))); // 0.0 * 0.0 = 0.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 0, 0), fixed16_16_mul(FIXED16_16_CONST(0, 0, 0), FIXED16_16_CONST(1, 0, 0))); // 0.0 * 1.0 = 0.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 0, 0), fixed16_16_mul(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(0, 0, 0))); // 1.0 * 0.0 = 0.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(1, 0, 0), fixed16_16_mul(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(1, 0, 0))); // 1.0 * 1.0 = 1.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(2, 0, 0), fixed16_16_mul(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(2, 0, 0))); // 1.0 * 2.0 = 2.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 5, 0), fixed16_16_mul(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(0, 5, 0))); // 1.0 * 0.5 = 0.5
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 5, 0), fixed16_16_mul(FIXED16_16_CONST(0, 5, 0), FIXED16_16_CONST(1, 0, 0))); // 0.5 * 1.0 = 0.5
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(-2, 0, 0), fixed16_16_mul(FIXED16_16_CONST(-1, 0, 0), FIXED16_16_CONST(2, 0, 0))); // -1.0 * 2.0 = -2.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(-2, 0, 0), fixed16_16_mul(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(-2, 0, 0))); // 1.0 * (-2.0) = -2.0
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(2, 0, 0), fixed16_16_mul(FIXED16_16_CONST(-1, 0, 0), FIXED16_16_CONST(-2, 0, 0))); // -1.0 * (-2.0) = 2.0

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(9, 8695526123046875, 0),
        fixed16_16_mul(FIXED16_16_CONST(3, 1415928, 0), FIXED16_16_CONST(3, 1415928, 0))
    ); // ~3.1415928 * ~3.1415928 = 9.8695526123046875

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(30, 818939208984375, 0),
        fixed16_16_mul(FIXED16_16_CONST(3, 1415928, 0), FIXED16_16_CONST(9, 81, 0))
    ); // ~3.1415928 * ~9.81 = 30.818939208984375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-30, 818939208984375, 0),
        fixed16_16_mul(FIXED16_16_CONST(-3, 1415928, 0), FIXED16_16_CONST(9, 81, 0))
    ); // ~-3.1415928 * ~9.81 = -30.818939208984375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-30, 818939208984375, 0),
        fixed16_16_mul(FIXED16_16_CONST(3, 1415928, 0), FIXED16_16_CONST(-9, 81, 0))
    ); // ~3.1415928 * ~-9.81 = -30.818939208984375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(30, 818939208984375, 0),
        fixed16_16_mul(FIXED16_16_CONST(-3, 1415928, 0), FIXED16_16_CONST(-9, 81, 0))
    ); // ~-3.1415928 * ~-9.81 = 30.818939208984375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(0, 54779052734375, 1),
        fixed16_16_mul(FIXED16_16_CONST(3, 1415928, 0), FIXED16_16_CONST(0, 174524064372835, 1))
    ); // ~3.1415928 * ~0.0174524064372835 = 0.054779052734375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(0, -54779052734375, 1),
        fixed16_16_mul(FIXED16_16_CONST(-3, 1415928, 0), FIXED16_16_CONST(0, 174524064372835, 1))
    ); // ~-3.1415928 * ~0.0174524064372835 = -0.054779052734375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(0, -54779052734375, 1),
        fixed16_16_mul(FIXED16_16_CONST(3, 1415928, 0), FIXED16_16_CONST(0, -174524064372835, 1))
    ); // ~3.1415928 * ~-0.0174524064372835 = -0.054779052734375

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(0, 54779052734375, 1),
        fixed16_16_mul(FIXED16_16_CONST(-3, 1415928, 0), FIXED16_16_CONST(0, -174524064372835, 1))
    ); // ~-3.1415928 * ~-0.0174524064372835 = 0.054779052734375
}

void test_fixed16_16_div(void) {
    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(285, 8692626953125, 0),
        fixed16_16_div(FIXED16_16_CONST(28329, 6421, 0), FIXED16_16_CONST(99, 1, 0))
    ); // ~28329.6421 / ~99.1 = 285.8692626953125

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-285, 8692626953125, 0),
        fixed16_16_div(FIXED16_16_CONST(-28329, 6421, 0), FIXED16_16_CONST(99, 1, 0))
    ); // ~-28329.6421 / ~99.1 = -285.8692626953125

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-285, 8692626953125, 0),
        fixed16_16_div(FIXED16_16_CONST(28329, 6421, 0), FIXED16_16_CONST(-99, 1, 0))
    ); // ~28329.6421 / ~-99.1 = -285.8692626953125

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(285, 8692626953125, 0),
        fixed16_16_div(FIXED16_16_CONST(-28329, 6421, 0), FIXED16_16_CONST(-99, 1, 0))
    ); // ~-28329.6421 / ~-99.1 = 285.8692626953125

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(478, 1600799560546875, 0),
        fixed16_16_div(FIXED16_16_CONST(92, 24515, 0), FIXED16_16_CONST(0, 19292, 0))
    ); // ~92.24515 / ~0.19292 = 478.1600799560546875

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-478, 1600799560546875, 0),
        fixed16_16_div(FIXED16_16_CONST(92, 24515, 0), FIXED16_16_CONST(0, -19292, 0))
    ); // ~92.24515 / ~-0.19292 = -478.1600799560546875

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-478, 1600799560546875, 0),
        fixed16_16_div(FIXED16_16_CONST(-92, 24515, 0), FIXED16_16_CONST(0, 19292, 0))
    ); // ~-92.24515 / ~0.19292 = -478.1600799560546875

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(478, 1600799560546875, 0),
        fixed16_16_div(FIXED16_16_CONST(-92, 24515, 0), FIXED16_16_CONST(0, -19292, 0))
    ); // ~-92.24515 / ~-0.19292 = 478.1600799560546875

    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(32767, 3776092529296875, 0),
        fixed16_16_div(FIXED16_16_CONST(32767, 3776092529296875, 0), FIXED16_16_ONE)
    ); // 32767.3776092529296875 / 1.0 = 32767.3776092529296875
}

void test_fixed16_16_sqrt(void) {
    TEST_ASSERT_EQUAL(
        FIXED16_16_ZERO,
        fixed16_16_sqrt(FIXED16_16_ZERO)
    ); // sqrt(0.0) = 0.0

    TEST_ASSERT_EQUAL(
        FIXED16_16_ONE,
        fixed16_16_sqrt(FIXED16_16_ONE)
    ); // sqrt(1.0) = 1.0

    // Check all the numbers from 0 to 32767.9999847412109375 with interval of 0.1352386474609375
    // (the interval was chosen arbitrarily).
    const fixed16_16 INTERVAL = FIXED16_16_CONST(0, 1352386474609375, 0);
    double absolute_error_sum = 0.0;
    double relative_error_sum = 0.0;
    double max_relative_error = -1.0;
    double max_relative_error_source;
    uint32_t count = 0;
    for (fixed16_16 num = INTERVAL; num < FIXED16_16_MAX - INTERVAL; num += INTERVAL) {
        double actual_sqrt = fixed16_16_to_double(fixed16_16_sqrt(num));
        double expected_sqrt = sqrt(fixed16_16_to_double(num));

        double absolute_error = dabs(expected_sqrt - actual_sqrt);
        double relative_error = absolute_error / expected_sqrt;
        absolute_error_sum += absolute_error;
        relative_error_sum += relative_error;

        if (max_relative_error < relative_error) {
            max_relative_error = relative_error;
            max_relative_error_source = fixed16_16_to_double(num);
        }

        count++;
    }

    absolute_error_sum /= count;
    relative_error_sum /= count;

    printf(
        "fixed16_16_sqrt: average absolute error: %f\n"
        "fixed16_16_sqrt: average relative error: %f%%\n"
        "fixed16_16_sqrt: max relative error: %f%% for sqrt(%f)\n",
        absolute_error_sum, relative_error_sum * 100.0, max_relative_error * 100.0, max_relative_error_source
    );

    TEST_ASSERT_LESS_THAN_DOUBLE(0.02, max_relative_error); // Expect 2% of precision.
}

void test_fixed16_16_inv_sqrt(void) {
    TEST_ASSERT_EQUAL(
        FIXED16_16_ONE,
        fixed16_16_sqrt(FIXED16_16_ONE)
    ); // sqrt(1.0) = 1.0

    // Check all the numbers from 0 to 32767.9999847412109375 with interval of 0.1352386474609375
    // (the interval was chosen arbitrarily).
    const fixed16_16 INTERVAL = FIXED16_16_CONST(0, 1352386474609375, 0);
    double absolute_error_sum = 0.0;
    double relative_error_sum = 0.0;
    double max_relative_error = -1.0;
    double max_relative_error_source;
    uint32_t count = 0;
    for (fixed16_16 num = INTERVAL; num < FIXED16_16_MAX - INTERVAL; num += INTERVAL) {
        double actual_sqrt = fixed16_16_to_double(fixed16_16_inv_sqrt(num));
        double expected_sqrt = 1.0 / sqrt(fixed16_16_to_double(num));

        double absolute_error = dabs(expected_sqrt - actual_sqrt);
        double relative_error = absolute_error / expected_sqrt;
        absolute_error_sum += absolute_error;
        relative_error_sum += relative_error;

        if (max_relative_error < relative_error) {
            max_relative_error = relative_error;
            max_relative_error_source = fixed16_16_to_double(num);
        }
        //TEST_ASSERT_INT32_WITHIN(DELTA, num, sqrt_squared);
        count++;
    }

    absolute_error_sum /= count;
    relative_error_sum /= count;

    printf(
        "fixed16_16_inv_sqrt: average absolute error: %f\n"
        "fixed16_16_inv_sqrt: average relative error: %f%%\n"
        "fixed16_16_inv_sqrt: max relative error: %f%% for sqrt(%f)\n",
        absolute_error_sum, relative_error_sum * 100.0, max_relative_error * 100.0, max_relative_error_source
    );

    TEST_ASSERT_LESS_THAN_DOUBLE(0.02, max_relative_error); // Expect 2% of precision.
}

void test_fixed16_16_sin(void) {

}

void test_fixed16_16_cos(void) {
    
}
