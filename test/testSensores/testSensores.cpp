#include <ArduinoFake.h>
#include <unity.h>
#include <sensores.h>

using namespace fakeit;

// Variables globales que necesitamos
float vMin = 5;
float vMax = 6;
float delta = 0.05;
float constCorreccion = 0.00161172;

/******************************************************************* 
Nombre: calcularDistancia
Método Prueba: Caja Blanca
Técnica Prueba: McCabe
Proceso: se verifica que con un pulseIn de 1000 se corresponda una distancia
de 17 cm
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testObtenerDistancia(void) {
    
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
    When(Method(ArduinoFake(), pulseIn)).AlwaysReturn(1000);
    
    float expectedDistance = 17;
    float currentDistance = calcularDistancia(1,2);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedDistance, currentDistance);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: límite inferior, se verifica que con un nivel de 5v (3102) el nivel 
de bateria sea 0
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testNivelBateriaMinimo(void) {

    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3102);
    When(Method(ArduinoFake(), delay)).AlwaysReturn();

    float expectedNivel = 0;
    float actualNivel = calcularNivelBateriaPromedio(
        1, 
        vMin, 
        vMax, 
        constCorreccion
    );

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: límite superior, se verifica que con un nivel de 6v (3723) el nivel 
de bateria sea 100
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testNivelBateriaMaximo(void){

    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3723);
    When(Method(ArduinoFake(), delay)).AlwaysReturn();

    float expectedNivel = 100;
    float actualNivel = calcularNivelBateriaPromedio(
        1, 
        vMin, 
        vMax, 
        constCorreccion
    );

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: por debajo del limite inferior, se verifica que con un nivel de 4.8v 
(2978) se retorne error
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testNivelBateriaNegativo(void) {

    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(2978);
    When(Method(ArduinoFake(), delay)).AlwaysReturn();
    
    float expectedNivel = ERROR_BATERIA;
    float actualNivel = calcularNivelBateriaPromedio(
        1, 
        vMin, 
        vMax, 
        constCorreccion
    );

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedNivel, actualNivel);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: por sobre el limite superior, se verifica que con un nivel de 6.2v 
(3847) se retorne error
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testNivelBateriaExcedido(void) {

    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(3847);
    When(Method(ArduinoFake(), delay)).AlwaysReturn();
    
    float expectedNivel = ERROR_BATERIA;
    float actualNivel = calcularNivelBateriaPromedio(
        1, 
        vMin, 
        vMax, 
        constCorreccion
    );

    TEST_ASSERT_EQUAL(expectedNivel, actualNivel);
}

int main(int argc, char **argv) {

    UNITY_BEGIN();

    RUN_TEST(testObtenerDistancia);

    RUN_TEST(testNivelBateriaMinimo);
    RUN_TEST(testNivelBateriaMaximo);
    RUN_TEST(testNivelBateriaNegativo);
    RUN_TEST(testNivelBateriaExcedido);

    UNITY_END();
}