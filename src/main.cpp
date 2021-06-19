/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
#include <config.h>
#include <WiFi.h>

#include <sensores.h>
#include <utilitarias.h>

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
void generarEvento(void);
void maquinaEstados(void);
void stInactivo(void);
void stEsperandoDeteccion(void);
void stObjetoDetectado(void);

/* ------------------ VARIABLES GLOBALES ------------------ */

int glbEstado;
int glbEvento;

/* ------------------ CÓDIGO ------------------ */

void setup() {
  doInit();
}

void loop() {
  maquinaEstados();

  // Este sleep es solo para visualizar mejor el LED.
  // Hay que borrarlo
  sleep(2);
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
  glbEstado = ST_INACTIVO;

  // Inicializar evento
  glbEvento = EVT_CONTINUE;
}


/*
* Genera un evento a partir de analizar los sensores.
* Si un objeto se encuentra den intervalo genera un evento
* objeto detectado, en caso contraro objeto no detectado.
*/

void generarEvento(void) {
  
  // Verificamos en que parte del rango esta
  bool resultado = estaDentroRango(
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM, 
    obtenerDistancia(PIN_TRIG, PIN_ECHO)
    );

  // De acuerdo al valor devolvemos el evento correspondiente
  if(resultado) {
    glbEvento = EVT_OBJETO_DETECTADO;
  } else {
    glbEvento = EVT_OBJETO_NO_DETECTADO;
  }
}

/*
* Es la implementación de una máquina de estados. 
* Dado un estado y un evento, establece el nuevo estado.
*/

void maquinaEstados(){
  
  // Segun el estado en el que nos encontramos llamamos a una función
  switch(glbEstado) {

    case ST_INACTIVO:
      stInactivo();
      break;

    case ST_ESPERANDO_DETECCION:
      stEsperandoDeteccion();
      break;
    
    case ST_OBJETO_DETECTADO:
      stObjetoDetectado();
      break;
    
    default:
      break;
  }

  // Generamos el evento para la siguiente pasada
  generarEvento();
}

/*
* Implementación de cada uno de lso estados de la máquina de estados.
*/

void stInactivo(){
  switch(glbEvento){

    case EVT_CONTINUE:
      glbEstado = ST_ESPERANDO_DETECCION;
      break;

    default:
      break;
  }
}

void stEsperandoDeteccion(){
  switch(glbEvento){

    case EVT_OBJETO_NO_DETECTADO:
      // Apagamos LED
      digitalWrite(PIN_LED, LOW);

      // Cambiamos de estado
      glbEstado = ST_ESPERANDO_DETECCION;
      break;

    case EVT_OBJETO_DETECTADO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      glbEstado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}

void stObjetoDetectado(){
  switch(glbEvento){

    case EVT_OBJETO_NO_DETECTADO:
      // Apagamos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      glbEstado = ST_ESPERANDO_DETECCION;
      break;

    case EVT_OBJETO_DETECTADO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      glbEstado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}