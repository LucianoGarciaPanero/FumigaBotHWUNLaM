#include <ArduinoFake.h>
#include <unity.h>
#include <Sensores.h>

using namespace fakeit;

void testObtenerDistancia(void) {
    
    //ARRANGE
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
    When(Method(ArduinoFake(), pulseIn)).AlwaysReturn(1000);
    long expectedDistance = 17;

    //ACT
    long currentDistance = obtenerDistancia(1,2);

    //ASSERT
    Verify(Method(ArduinoFake(), digitalWrite)).Exactly(3_Times);
    Verify(Method(ArduinoFake(), delayMicroseconds)).Exactly(2_Times);

    TEST_ASSERT_EQUAL(expectedDistance, currentDistance);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(testObtenerDistancia);

    // Finalizan los test
    UNITY_END();
}