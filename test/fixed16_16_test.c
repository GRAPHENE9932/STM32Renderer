#include "fixed16_16_test.h"
#include "fixed16_16.h"

#include <unity.h>

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

void test_fixed16_16_add(void) {
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(1, 5, 0), fixed16_16_add(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(0, 5, 0))); // 1.0 + 0.5 = 1.5
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(3, 0, 0), fixed16_16_add(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(2, 0, 0))); // 1.0 + 2.0 = 3.0
    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-26967, 4102020263671875, 0),
        fixed16_16_add(FIXED16_16_CONST(5032, 9119873046875, 0), FIXED16_16_CONST(-32000, 3221893310546875, 0))
    ); // 5032.9119873046875 + (-32000.3221893310546875) = -26967.4102020263671875
    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(14739, 63128662109375, 0),
        fixed16_16_add(FIXED16_16_CONST(2348, 341, 0), FIXED16_16_CONST(12391, 2903, 0))
    ); // 2348.3409881591796875 + 12391.2902984619140625 = 14739.63128662109375
}

void test_fixed16_16_sub(void) { 
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(0, 5, 0), fixed16_16_sub(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(0, 5, 0))); // 1.0 - 0.5 = 0.5
    TEST_ASSERT_EQUAL(FIXED16_16_CONST(-1, 0, 0), fixed16_16_sub(FIXED16_16_CONST(1, 0, 0), FIXED16_16_CONST(2, 0, 0))); // 1.0 - 2.0 = -1.0
    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(36033, 2341766357421875, 0),
        fixed16_16_sub(FIXED16_16_CONST(4032, 9119873046875, 0), FIXED16_16_CONST(-32000, 3221893310546875, 0))
    ); // 4032.9119873046875 - (-32000.3221893310546875) = 36033.2341766357421875
    TEST_ASSERT_EQUAL(
        FIXED16_16_CONST(-10042, 949310302734375, 0),
        fixed16_16_sub(FIXED16_16_CONST(2348, 341, 0), FIXED16_16_CONST(12391, 2903, 0))
    ); // 2348.3409881591796875 - 12391.2902984619140625 = −10042.949310302734375
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
}

void test_fixed16_16_sin(void) {

}

void test_fixed16_16_cos(void) {
    
}
