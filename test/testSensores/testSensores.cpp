#include <ArduinoFake.h>
#include <unity.h>
#include <sensores.h>

using namespace fakeit;

// Variables globales que necesitamos
float vMin = 5;
float vMax = 6.6;
float delta = 0.05;

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

/*
* Testeamos que el límite inferior (0%).
*/

void testObtenerNivelBateriaMinimo(void) {

    // Arrange
    // Para un Vin de 2.5V le corresponde un valor de 3102.2727...
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3102);
    float expectedNivel = 0;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/*
* Testeamos el límite superior(100%).
*/

void testObtenerNivelBateriaMaximo(void){

    // Arrange
    // Para un Vin de 3.3V le corresponde un valor de 4095
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(4095);
    float expectedNivel = 100;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/*
* Testeamos por debajo del límite inferior (< 0%)
*/

void testObtenerNivelBateriaNegativo(void) {
    
    // Arrange
    // Para un Vin de 2.4V le corresponde un valor de 2978.181818
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(2978.181818);
    float expectedNivel = ERROR_BATERIA;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax);

    // Assert
    TEST_ASSERT_EQUAL(expectedNivel, actualNivel);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(testObtenerDistancia);
    RUN_TEST(testObtenerNivelBateriaMinimo);
    RUN_TEST(testObtenerNivelBateriaMaximo);
    RUN_TEST(testObtenerNivelBateriaNegativo);

    // Finalizan los test
    UNITY_END();
}