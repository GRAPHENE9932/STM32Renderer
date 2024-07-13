#include "fixed32_test.h"

#include <unity.h>

void setUp(void) {
    
}

void tearDown(void) {
    
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fixed32_fixed32_const);
    RUN_TEST(test_fixed32_mul);
    RUN_TEST(test_fixed32_div);
    RUN_TEST(test_fixed32_sqrt);
    RUN_TEST(test_fixed32_inv_sqrt);
    RUN_TEST(test_fixed32_sin);
    RUN_TEST(test_fixed32_cos);
    return UNITY_END();
}
