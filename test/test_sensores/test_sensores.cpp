#include <unity.h>
#include "sensores.h"

void test_siempre_verdadero(void){
    TEST_ASSERT_EQUAL(0, 0);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(test_siempre_verdadero);

    // Finalizan los test
    UNITY_END();
}