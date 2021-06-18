#include <unity.h>
#include <ArduinoFake.h>
#include "sensores.h"

using namespace fakeit;

void test_siempre_verdadero(void){
    TEST_ASSERT_EQUAL(0, 0);
    When(Method(ArduinoFake(), delay)).AlwaysReturn();
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(test_siempre_verdadero);

    // Finalizan los test
    UNITY_END();
}