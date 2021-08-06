#include "./sensores.h"
#include <Arduino.h>

/******************************************************************* 
Nombre: calcularDistancia
Entradas:
          + trigPin: int
          + echoPin: int
Salida:
          + distancia: long
Proceso: calcula la distancia a la que se encuentra un objeto en frente
del sensor de ultrasonido, identificado por trigPin y echoPin
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

long calcularDistancia(int trigPin, int echoPin) {
  
  // Inicializando el pin del trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(TIEMPO_ASEGURAR_NIVEL_BAJO_US);
  
  // Comenzar a transmitir
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(TIEMPO_PULSO_US);
  digitalWrite(trigPin, LOW);

  // Lectura del tiempo que estuvo en high
  long duracion = pulseIn(echoPin, HIGH);

  // Calculamos la distancia del objeto
  return (duracion / VALOR_CORRECCION_ONDA) * VELOCIDAD_SONIDO_CM;
}

/******************************************************************* 
Nombre: calcularDistanciaPromedio
Entradas:
          + pinTrig: int
          + pinEcho: int
Salida:
          + distanciaPromedio: float
Proceso: obtiene n mediciones del sensor de ultrasonido, especificado por
pinTrig y pinEcho. Luego, calcular el promedio usando la función 
calcularPromedio.
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

float calcularDistanciaPromedio(int pinTrig, int pinEcho) {
  
  // Inicializar variables
  int n = 25;
  float values[n];

  // Leer n valores
  for(int i = 0; i < n; i++) {
    values[i]= calcularDistancia(pinTrig, pinEcho);

    // Para que las mediciones no interfieran entre si
    delay(30);
  }
  
  // Calcular promedio
  return calcularPromedio(values, n);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Entradas:
          + pin: int
          + vMin: float
          + vMax: float
          + constCorr: float
Salida:
          + porcentajeBateria: float
Proceso: obtiene n mediciones de la bateria. Luego, calcula el promedio usando
calcularPromedio. Calcula y devuelve el porcentaje de bateria restante,
teniendo en cuenta el vMin y vMax. Si el porcentaje de batería es menor
a 0 o mayor a 100, entonces devuelve error
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

float calcularNivelBateriaPromedio(int pin, float vMin, float vMax, float constCorr) {

  // Inicializar variables
  int n = 100;
  
  float vIn = -1;
  float porcentaje = -1;

  float values[n];  

  // Leer n valores seguidos
  for(int i = 0; i < n; i++) {
    values[i] = analogRead(pin) * constCorr;

    // Para que las mediciones no interfieran entre si
    delay(30);
  }
  
  // Calcular el promedio leido
  vIn =  calcularPromedio(values, n);

  // Calcular el porcentaje de la carga
  porcentaje = 100 * (vIn - vMin) / (vMax - vMin);

  // Verificar que el resultado no tenga errores
  if(porcentaje < 0 - DELTA_CARGA_BATERIA || porcentaje > 100 + DELTA_CARGA_BATERIA) {
    return ERROR_BATERIA;
  } else {
    return porcentaje;
  }
}

/******************************************************************* 
Nombre: liberarQuimico
Entradas:
          + pinBomba: int
          + tiempoMs: float
Salida: -
Proceso: activa la bomba durante tiempMs, identificandola por pinBomba
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void liberarQuimico(int pinBomba, float tiempoMs) {

  // Inicializar variable
  long currentTime = 0;
  
  // Prender bomba
  digitalWrite(pinBomba, HIGH);

  // Dejarla prendida durante x ms
  currentTime = millis();
  while(millis() - currentTime < tiempoMs ) {}

  // Apagar bomba
  digitalWrite(pinBomba, LOW);
}