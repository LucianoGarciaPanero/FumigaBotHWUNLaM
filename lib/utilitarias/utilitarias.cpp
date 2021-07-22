#include "./utilitarias.h"
#include <stdio.h>

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
* Ordena un vector de tipo float de forma ascendente mediante burbujeo
*/

void ordenarVector(float *values, int len) {

  // Ordenar vector
  for(int i = 0; i < len - 1; i++) {
      for(int j = i + 1; j < len; j++) {
        if(values[i] > values[j]) {
          float swap = values[i];
          values[i] = values[j];
          values[j] = swap;
        }
      }
    }
}


/*
* Dado un array con los valores leidos, lo ordena, filtra las primeras y últimas
* n observaciones porque se las trata como ruido y calcula el promedio con
* los restantes.
*/

float calcularPromedio(float *values, int len) {

  // Verificar tener por lo menos 11 observaciones
  if(len <= 10) {
    return ERROR_PROMEDIO;
  }
  
  // Inicializar variables
  float acum = 0;
  float vecAux[len];

  // Copiar values en aux
  for(int i = 0; i < len; i++) {
    vecAux[i] = values[i];
  }

  // Ordenar el vector de forma ascendente
  ordenarVector(vecAux, len);

  // Calcular el promedio excluyendo las primeras y últimas 5 observaciones,
  // se consideran que son ruido.
  for(int i = 5; i < len - 5; i++) {
    acum += vecAux[i];
  }

  return acum / (len - 10);
}