#include "./utilitarias.h"
#include <stdio.h>

/******************************************************************* 
Nombre: estaDentroRango
Entradas:
          + limiteInferior: int
          + limiteSuperior: int
          + valor: long
Salida: 
          + true/false: bool 
Proceso: verifica que valor se encuentre dentro del intervalo [limiteInferior;
limiteSuperior]
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

bool estaDentroRango(int limiteInferior, int limiteSuperior, long valor) {
    if(valor < limiteInferior || valor > limiteSuperior) {
        return false;
    } else {
        return true;
    }
}

/******************************************************************* 
Nombre: ordenarVector
Entradas:
          + values: float*
          + len: int
Salida: -
Proceso: ordena el vector in situ de forma ascendente mediante burbujeo
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

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

/******************************************************************* 
Nombre: calcularPromedio
Entradas:
          + values: float*
          + len: int
Salida:
          + promedio: float
Proceso: ordena el vector dado como entrada, descarta las primeras 5
observaciones y las 5 últimas observaciones. Luego, calcula el promedio con
el resto de observaciones. Si la longitud del vector es menor o igual a 10
retorna error.
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

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