#include <Arduino.h>
#include <config.h>
#include <maquinaEstados.h>

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);

/* ------------------ CÓDIGO ------------------ */

void setup() {
  doInit();
}

void loop() {
  maquinaEstadosGeneral();
}

/* ------------------ DEFINICIÓN FUNCIONES ------------------ */

/*
* Inicializa los pines en los modos correctos y las variables necesarias
*/

void doInit(){

  // Inicializamos pin LED
  pinMode(PIN_LED, OUTPUT);

  // Inicialización MdE general.
  doInitMdEGeneral();

  Serial.begin(VEL_TRANSMISION);
}