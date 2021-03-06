#include "./utilitarias.h"

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
Cambiado Por: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
Fecha Cambió: 02/11/2021 
Referencia: Cambiamos algoritmo de burbujeo por uno que busca el mínimo y 
lo ponga al principio
*****************************************************************/

void ordenarVector(float *values, int len) {

  int pos = 0;

  for(int i = 0; i < len - 1 ; i ++) {

    float min = 20000;
      for (int j = i; j < len; j++){
        
        if( values[j] < min) {
              
              min = values[j];
              pos = j;

          }
      }

    float aux = values[i];
    values[i] = min;
    values[pos] = aux;
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
  float promedio;

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

  promedio = acum / (len - 10);
  return promedio;
}