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
    
    // Finalizan los test
    UNITY_END();
}