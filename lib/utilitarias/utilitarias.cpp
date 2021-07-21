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


/*
* Dado un array con los valores leidos, lo ordena, filtra las primeras y últimas
* n observaciones porque se las trata como ruido y calcula el promedio con
* los restantes.
*/

float calcularPromedio(float *valores, int len) {

  // Verificar tener por lo menos 11 observaciones
  if(len <= 10) {
    return ERROR_PROMEDIO;
  }
  
  // Inicializar variables
  float acum = 0;
  
  // Ordenar el vector de forma ascendente
  for(int i = 0; i < len; i++) {
    for(int j = i + 1; j < len - 1; j++) {
      if(valores[i] > valores[j]) {
        float swap = valores[i];
        valores[i] = valores[j];
        valores[j] = swap;
      }
    }

    // Calcular el promedio excluyendo las primeras y últimas 5 observaciones,
    // se consideran que son ruido.
    for(int i = 5; i < len - 5; i++) {
      acum += valores[i];
    }

    return acum / len - 10;

  }

}