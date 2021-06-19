#include <unity.h>
#include <config.h>
#include <utilitarias.h>

void testEstaDentroRangoJustoPorEncima(void) {
    TEST_ASSERT_EQUAL(FUERA_RANGO, estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM + 1));
}

void testEstaDentroRangoJustoPorDebajo(void) {
    TEST_ASSERT_EQUAL(FUERA_RANGO, estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM - 1));
}

void testEstaDentroRangoJustoEnLimiteSuperior(void){
    TEST_ASSERT_EQUAL(DENTRO_RANGO, estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM));
}

void testEstaDentroRangoJustoEnLimiteInferior(void){
     TEST_ASSERT_EQUAL(DENTRO_RANGO, estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM));
}

void testEstaDentroRangoNegativo(void) {
    TEST_ASSERT_EQUAL(FUERA_RANGO, estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , - 1));
}


int main(int argc, char **argv) {

    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(testEstaDentroRangoJustoPorEncima);
    RUN_TEST(testEstaDentroRangoJustoPorDebajo);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteSuperior);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteInferior);
    RUN_TEST(testEstaDentroRangoNegativo);
    
    // Finalizan los test
    UNITY_END();
}