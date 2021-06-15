/* ------------------ BIBLIOTECAS ------------------ */
#include <Arduino.h>
#include "config.h"
#include "sensores.h"

/* ------------------ VARIABLES GLOBALES ------------------ */
int ledEstado;

/* ------------------ ESTADOS ------------------ */
#define ST_INACTIVO             0
#define ST_ESPERANDO_DETECCION  1
#define ST_OBJETO_DETECTADO     2   

/* ------------------ EVENTOS ------------------ */
#define EVT_CONTINUE            0
#define EVT_OBJETO_NO_DETECTADO 1
#define EVT_OBJETO_DETECTADO    2

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */
int generarEvento();

/* ------------------ CÓDIGO ------------------ */
void setup() {
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);
  
  // Inicializar pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Inicializar variables globales
  ledEstado = LOW;
}

void loop() {
  // Obtenemos la distancia del objeto
  long distancia = obtenerDistancia(PIN_TRIG, PIN_ECHO);

  // Verificación de la distancia del objeto
  if(distancia <= UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM) {
    ledEstado = HIGH;
  } else {
    ledEstado = LOW;
  }
  digitalWrite(PIN_LED, ledEstado);
  
  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

