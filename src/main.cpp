
#include <Arduino.h>
#include "config.h"
#include "sensores.h"

/* ------------------ VARIABLES GLOBALES ------------------ */
int ledEstado = LOW;

void setup() {
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);
  
  // Inicializar pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Establecer el estado del led en low
  ledEstado = LOW;
}

void loop() {
  // Obtenemos la distancia del objeto
  long distancia = obtenerDistancia(PIN_TRIG, PIN_ECHO);

  // Verificación de la distancia del objeto
  if(distancia <= UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM){
    ledEstado = HIGH;
  } else {
    ledEstado = LOW;
  }
  digitalWrite(PIN_LED, ledEstado);
  
  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

