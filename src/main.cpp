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
int generarEvento(void);

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
  
  
  
  
  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

/* ------------------ DEFINICIÓN FUNCIONES ------------------ */

/*
* Genera un evento a partir de analizar los sensores.
* Si un objeto es menor al umbral de distancia entonces genera un evento
* objeto detectado, en caso contraro objeto no detectado.
*/
int generarEvento(void){

  // Inicializamos las variables a usar
  int evento = EVT_CONTINUE;
 
  // Obtenemos la distancia del objeto
  long distancia = obtenerDistancia(PIN_TRIG, PIN_ECHO);

  // Verificación de la distancia del objeto
  if(distancia <= UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM) {
    evento = EVT_OBJETO_DETECTADO;
  } else {
    evento = EVT_OBJETO_NO_DETECTADO;
  }

  return evento;
}