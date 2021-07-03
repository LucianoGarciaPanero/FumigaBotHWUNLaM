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
* batería de acuerdo a la tensión mínima de esta y la max
*/

float obtenerNivelBateria(int pin, float vMin, float vMax) {

  // Leemos el voltaje de entrada por el pin
  float vIn = analogRead(pin) * CONSTANTE_CORRECCION_VOLTAJE_BATERIA;

  // Calculamos el porcentaje de la carga y lo retornamos
  float porcentaje = 100 * (vIn - vMin) / (vMax - vMin);

  // Si llega a ocurrir que leemos valores no previstos devolvemos error  
  // Como estamos tratando con float y hay un error en los décimales, damos un 
  // margen de DELTA_CARGA_BATERIA
  
  if(porcentaje < 0 - DELTA_CARGA_BATERIA || porcentaje > 100 + DELTA_CARGA_BATERIA) {
    return ERROR_BATERIA;
  } else {
    return porcentaje;
  }
}