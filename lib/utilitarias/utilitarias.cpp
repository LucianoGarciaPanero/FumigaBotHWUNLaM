#include "./utilitarias.h"

/*
* Verifica que un valor se encuentre en el intervalo [Li; Ls]
*/

int estaDentroRango(int limiteInferior, int limiteSuperior, long valor) {
    if(valor < limiteInferior || valor > limiteSuperior) {
        return FUERA_RANGO;
    } else {
        return DENTRO_RANGO;
    }
}