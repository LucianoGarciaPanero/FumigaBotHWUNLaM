#include <ArduinoFake.h>
#include <unity.h>
#include <sensores.h>

using namespace fakeit;

// Variables globales que necesitamos
float vMin = 5;
float vMax = 6;
float delta = 0.05;
float constCorreccion = 0.00161172;

/* ------------------ TEST MÉTODO obtenerDistancia() ------------------ */

void testObtenerDistancia(void) {
    
    // Arrange
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
    When(Method(ArduinoFake(), pulseIn)).AlwaysReturn(1000);
    float expectedDistance = 17;

    // Act
    float currentDistance = obtenerDistancia(1,2);

    // Assert
    Verify(Method(ArduinoFake(), digitalWrite)).Exactly(3_Times);
    Verify(Method(ArduinoFake(), delayMicroseconds)).Exactly(2_Times);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedDistance, currentDistance);
}

/* ------------------ TEST MÉTODO obtenerNivelBateria() ------------------ */

/*
* Testeamos que el límite inferior (0%).
*/

void testObtenerNivelBateriaMinimo(void) {

    // Arrange
    // Simulamos una entrada de 2.5V
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3102);
    float expectedNivel = 0;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax, constCorreccion);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/*
* Testeamos el límite superior(100%).
*/

void testObtenerNivelBateriaMaximo(void){

    // Arrange
    // Simulamos una entrada de 3V
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3723);
    float expectedNivel = 100;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax, constCorreccion);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/*
* Testeamos por debajo del límite inferior (< 0%)
*/

void testObtenerNivelBateriaNegativo(void) {
    
    // Arrange
    // Simulamos una entrada de 2.4V
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(2978);
    float expectedNivel = ERROR_BATERIA;

    // Act
    float actualNivel = obtenerNivelBateria(1, vMin, vMax, constCorreccion);

    // Assert
    TEST_ASSERT_EQUAL(expectedNivel, actualNivel);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    // obtenerDistancia()
    RUN_TEST(testObtenerDistancia);

    // obtenerNivelBateria
    RUN_TEST(testObtenerNivelBateriaMinimo);
    RUN_TEST(testObtenerNivelBateriaMaximo);
    RUN_TEST(testObtenerNivelBateriaNegativo);

    // Finalizan los test
    UNITY_END();
}