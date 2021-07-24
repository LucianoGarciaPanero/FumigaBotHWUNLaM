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
* Calcula la distancia promedio leyendo por los pines indicados
*/ 

float calcularDistanciaPromedio(int pinTrig, int pinEcho) {
  
  // Inicializar variables
  int n = 25;
  float values[n];

  // Leer n valores
  for(int i = 0; i < n; i++) {
    values[i]= obtenerDistancia(pinTrig, pinEcho);

    // Para que las mediciones no interfieran entre si
    delay(30);
  }
  
  // Calcular promedio
  return calcularPromedio(values, n);
}

/*
* Lee la tensión que entra por el pin especificado. Calcula el nivel de 
* batería de acuerdo a la tensión mínima y la máxima
*/

float obtenerNivelBateria(int pin, float vMin, float vMax, float constCorr) {

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

/*
* Libera químico por el pin especificado durante x milisegundos
*/

void liberarQuimico(int pinBomba, float tiempoMs) {

  // Inicializar variable
  long currentTime = 0;
  
  // Prender bomba
  digitalWrite(pinBomba, HIGH);

  // Dejarla prendida durante x ms
  currentTime = millis();
  while(millis() - currentTime < tiempoMs ) {
    // DO NOTHING
  }

  // Apagar bomba
  digitalWrite(pinBomba, LOW);


}