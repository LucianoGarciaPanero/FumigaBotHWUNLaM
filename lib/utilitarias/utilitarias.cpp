#include "./utilitarias.h"

/*
* Verifica que un valor se encuentre en el intervalo [Li; Ls]
*/

bool estaDentroRango(int limiteInferior, int limiteSuperior, long valor) {
    if(valor < limiteInferior || valor > limiteSuperior) {
        return false;
    } else {
        return true;
    }
}