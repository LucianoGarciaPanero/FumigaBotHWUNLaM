#include <unity.h>
#include <config.h>
#include <utilitarias.h>

// Variables globales que necesitamos
float delta = 0.05;

/* ------------------ TEST MÉTODO estaDentroRango() ------------------ */

void testEstaDentroRangoJustoPorEncima(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM + 1
        ));
}

void testEstaDentroRangoJustoPorDebajo(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM - 1
        ));
}

void testEstaDentroRangoJustoEnLimiteSuperior(void){
    TEST_ASSERT_TRUE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM
        ));
}

void testEstaDentroRangoJustoEnLimiteInferior(void){
     TEST_ASSERT_TRUE(estaDentroRango(
         UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
         UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM ,
         UMBRAL_MINIMA_DISTANCIA_OBJETO_CM
         ));
}

void testEstaDentroRangoNegativo(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        - 1
        ));
}

/* ------------------ TEST MÉTODO calcularPromedio() ------------------ */


void testCalcularPromedioDiezObservaciones(void) {
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float expectedPromedio = ERROR_PROMEDIO;
    float actualPromedio;
    int len = 10;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

void testCalcularPromedioOnceObservaciones(void) {
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float expectedPromedio = 5;
    float actualPromedio;
    int len = 11;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

void testCalcularPromedioOrdenadoAscendente(void) {
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    float expectedPromedio = 7;
    float actualPromedio;
    int len = 15;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

void testCalcularPromedioOrdenadoDescendente(void) {
    float values[] = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    float expectedPromedio = 7;
    float actualPromedio;
    int len = 15;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

void testCalcularPromedioTodosValoresIguales(void) {
    float values[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
    float expectedPromedio = 7;
    float actualPromedio;
    int len = 15;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    // estaDentroRango()
    RUN_TEST(testEstaDentroRangoJustoPorEncima);
    RUN_TEST(testEstaDentroRangoJustoPorDebajo);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteSuperior);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteInferior);
    RUN_TEST(testEstaDentroRangoNegativo);

    // calcularPromedio
    RUN_TEST(testCalcularPromedioDiezObservaciones);
    RUN_TEST(testCalcularPromedioOnceObservaciones);
    RUN_TEST(testCalcularPromedioOrdenadoAscendente);
    RUN_TEST(testCalcularPromedioOrdenadoDescendente);
    RUN_TEST(testCalcularPromedioTodosValoresIguales);
    
    // Finalizan los test
    UNITY_END();
}