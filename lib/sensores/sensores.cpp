#include "./sensores.h"
#include <Arduino.h>

/*
* Esta función se encarga de obtener la distancia que se encuentra un objeto en
* frente del sensor de ultrasonido identificado, pasado los pines donde se 
* ubican el trigger y echo
*/

long obtenerDistancia(int trigPin, int echoPin) {
  
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

/*
* Lee la tensión que entra por el pin especificado. Calcula el nivel de 
* batería de acuerdo a la tensión mínima y la máxima
*/

float obtenerNivelBateria(int pin, float vMin, float vMax, float constCorr) {

  // Inicializar variables
  float acum = 0;
  float vIn = -1;
  float porcentaje = -1;

  int nLecturasDescartadas = 10;
  int nLecturasTotales = 100;
  
  // Realizar pasadas iniciales para desechar valores basura
  for(int i = 0; i < nLecturasDescartadas; i++) {
    analogRead(pin);

    // Para que las mediciones no interfieran entre si
    delay(30);
  }

  // Leer n valores seguidos
  for(int i = 0; i < nLecturasTotales; i++) {
    acum += analogRead(pin) * constCorr;
  }
  
  // Calcular el promedio leido
  vIn =  acum / nLecturasTotales;

  // Calcular el porcentaje de la carga
  porcentaje = 100 * (vIn - vMin) / (vMax - vMin);

  // Verificar que el resultado no tenga errores
  if(porcentaje < 0 - DELTA_CARGA_BATERIA || porcentaje > 100 + DELTA_CARGA_BATERIA) {
    return ERROR_BATERIA;
  } else {
    return porcentaje;
  }
}
