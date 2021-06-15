
#include <Arduino.h>
#include <sensores.h>
#include "config.h"

/* ------------------ VARIABLES GLOBALES ------------------ */
int ledEstado = LOW;

void setup() {
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);
  
  // Inicializar pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Establecer el estado del led en low
  ledEstado = LOW;
}

void loop() {
  // Obtenemos la distancia del objeto
  long distancia = calcularDistancia(PIN_TRIG, PIN_ECHO);

  // Verificación de la distancia del objeto
  if(distancia <= UMBRAL_MAXIMO_DETECCION_CM){
    ledEstado = HIGH;
  } else {
    ledEstado = LOW;
  }
  digitalWrite(PIN_LED, ledEstado);
  
  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

