#include <unity.h>
#include <config.h>
#include <utilitarias.h>

// Variables globales que necesitamos
float delta = 0.05;

/******************************************************************* 
Nombre: estaDentroRango
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica el caso por encima del límite superior 
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testEstaDentroRangoJustoPorEncima(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM + 1
        ));
}

/******************************************************************* 
Nombre: estaDentroRango
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica el caso por debajo del límite inferior 
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testEstaDentroRangoJustoPorDebajo(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM - 1
        ));
}

/******************************************************************* 
Nombre: estaDentroRango
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica el caso del límite superior 
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testEstaDentroRangoJustoEnLimiteSuperior(void){
    TEST_ASSERT_TRUE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM
        ));
}

/******************************************************************* 
Nombre: estaDentroRango
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica el caso del límite inferior 
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testEstaDentroRangoJustoEnLimiteInferior(void){
     TEST_ASSERT_TRUE(estaDentroRango(
         UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
         UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM ,
         UMBRAL_MINIMA_DISTANCIA_OBJETO_CM
         ));
}

/******************************************************************* 
Nombre: estaDentroRango
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica el caso que sea un número negativo
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testEstaDentroRangoNegativo(void) {
    TEST_ASSERT_FALSE(estaDentroRango(
        UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
        UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM , 
        - 1
        ));
}

/******************************************************************* 
Nombre: ordenarVector
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que la entrada este ordenado de forma 
ascendente
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testOrdenarVectorOrdenadoAscendente(void) {
    
    int len = 15;
    float expectedVec[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    float actualVec[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    
    ordenarVector(actualVec, len);

    for(int i = 0; i < len; i++) {
        TEST_ASSERT_FLOAT_WITHIN(delta, expectedVec[i], actualVec[i]);
    }    
}

/******************************************************************* 
Nombre: ordenarVector
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que la entrada este ordenado de forma 
descendente
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testOrdenarVectorOrdenadoDescendente(void) {
    
    int len = 15;
    float expectedVec[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    float actualVec[] = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    
    ordenarVector(actualVec, len);

    for(int i = 0; i < len; i++) {
        TEST_ASSERT_FLOAT_WITHIN(delta, expectedVec[i], actualVec[i]);
    }
}

/******************************************************************* 
Nombre: ordenarVector
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que la entrada sean todos valores iguales
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testOrdenarVectorTodosValoresIguales(void) {
    
    int len = 15;
    float expectedVec[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
    float actualVec[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
    
    ordenarVector(actualVec, len);

    for(int i = 0; i < len; i++) {
        TEST_ASSERT_FLOAT_WITHIN(delta, expectedVec[i], actualVec[i]);
    }
}

/******************************************************************* 
Nombre: calcularPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que sea una entrada de exactamente 10 
observaciones
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testCalcularPromedioDiezObservaciones(void) {
 
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float expectedPromedio = ERROR_PROMEDIO;
    float actualPromedio;
    int len = 10;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

/******************************************************************* 
Nombre: calcularPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que sea una entrada de exactamente 11 
observaciones
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testCalcularPromedioOnceObservaciones(void) {
    
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float expectedPromedio = 5;
    float actualPromedio;
    int len = 11;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

/******************************************************************* 
Nombre: calcularPromedio
Método Prueba: Caja Negra
Técnica Prueba: AVL
Proceso: se verifica en el caso que sea una entrada de exactamente 15 
observaciones distintas
Fecha Creación: 01/07/2021
Creador:
        + Luciano Garcia Panero
        + Tomás Sánchez Grigioni
*****************************************************************/

void testCalcularPromedioExitoso(void) {
   
    float values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    float expectedPromedio = 6;
    float actualPromedio;
    int len = 13;

    actualPromedio = calcularPromedio(values, len);

    TEST_ASSERT_FLOAT_WITHIN(delta, expectedPromedio, actualPromedio);
}

int main(int argc, char **argv) {

    UNITY_BEGIN();

    RUN_TEST(testEstaDentroRangoJustoPorEncima);
    RUN_TEST(testEstaDentroRangoJustoPorDebajo);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteSuperior);
    RUN_TEST(testEstaDentroRangoJustoEnLimiteInferior);
    RUN_TEST(testEstaDentroRangoNegativo);

    RUN_TEST(testOrdenarVectorOrdenadoAscendente);
    RUN_TEST(testOrdenarVectorOrdenadoDescendente);
    RUN_TEST(testOrdenarVectorTodosValoresIguales);

    RUN_TEST(testCalcularPromedioDiezObservaciones);
    RUN_TEST(testCalcularPromedioOnceObservaciones);
    RUN_TEST(testCalcularPromedioExitoso);
    
    UNITY_END();
}