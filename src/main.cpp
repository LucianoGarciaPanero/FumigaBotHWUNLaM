/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
#include "config.h"
#include "sensores.h"

/* ------------------ ESTADOS ------------------ */

#define ST_INACTIVO             0
#define ST_ESPERANDO_DETECCION  1
#define ST_OBJETO_DETECTADO     2   

/* ------------------ EVENTOS ------------------ */

#define EVT_CONTINUE            0
#define EVT_OBJETO_NO_DETECTADO 1
#define EVT_OBJETO_DETECTADO    2

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);
int generarEvento(void);
void maquinaEstados(int);

/* ------------------ VARIABLES GLOBALES ------------------ */
int estado;
int evento;

/* ------------------ CÓDIGO ------------------ */

void setup() {
  doInit();
}

void loop() {

  
}

/* ------------------ DEFINICIÓN FUNCIONES ------------------ */

/*
* Inicializa los pines en los modos correctos y las variables necesarias
*/

void doInit(){
  
  // Inicializar pines sensor ultrasonido
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Inicializar pin LED
  pinMode(PIN_LED, OUTPUT);
  
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);

  // Inicializar estado
  estado = ST_INACTIVO;

  // Inicializar evento
  evento = EVT_CONTINUE;
}


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