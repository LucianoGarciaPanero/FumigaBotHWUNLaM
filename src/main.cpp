/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>

#include <config.h>
#include <maquinaEstados.h>

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Init pines
  doInit();
  
  // Init MdE
  doInitMdE();
}

void loop() {
  maquinaEstados();
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
}