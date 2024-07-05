#include "fixed16_16_test.h"

#include <unity.h>

void setUp(void) {
    
}

void tearDown(void) {
    
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fixed16_16_fixed16_16_const);
    RUN_TEST(test_fixed16_16_mul);
    RUN_TEST(test_fixed16_16_div);
    RUN_TEST(test_fixed16_16_sin);
    RUN_TEST(test_fixed16_16_cos);
    return UNITY_END();
}
