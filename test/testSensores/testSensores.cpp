#include <ArduinoFake.h>
#include <unity.h>
#include <sensores.h>

using namespace fakeit;

// Variables globales que necesitamos
int vMin = 5;
int vMax = 6;

void testObtenerDistancia(void) {
    
    // Arrange
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
    When(Method(ArduinoFake(), pulseIn)).AlwaysReturn(1000);
    long expectedDistance = 17;

    // Act
    long currentDistance = obtenerDistancia(1,2);

    // Assert
    Verify(Method(ArduinoFake(), digitalWrite)).Exactly(3_Times);
    Verify(Method(ArduinoFake(), delayMicroseconds)).Exactly(2_Times);

    TEST_ASSERT_EQUAL(expectedDistance, currentDistance);
}

void testObtenerNivelBateriaMinimo(void){

    // Arrange
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3103.030303);
    float expectedNivel = 0;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax);

    // Assert
    Verify(Method(ArduinoFake(), analogRead)).Exactly(1_Times);
    TEST_ASSERT_TRUE(abs(expectedNivel - actualNivel) < 0.01);
}

void testObtenerNivelBateriaMaximo(void){

    // Arrange
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(4096);
    float expectedNivel = 100;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax);

    // Assert
    Verify(Method(ArduinoFake(), analogRead)).Exactly(1_Times);
    TEST_ASSERT_TRUE(abs(expectedNivel - actualNivel) < 0.01);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(testObtenerDistancia);
    RUN_TEST(testObtenerNivelBateriaMinimo);
    RUN_TEST(testObtenerNivelBateriaMaximo);

    // Finalizan los test
    UNITY_END();
}