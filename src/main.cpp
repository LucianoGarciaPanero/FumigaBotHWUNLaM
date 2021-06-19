/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
#include <config.h>
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
int generarEvento(long);
void maquinaEstados(int, int);
void stInactivo(int);
void stEsperandoDeteccion(int);
void stObjetoDetectado(int);

/* ------------------ VARIABLES GLOBALES ------------------ */

int estado;
int evento;

/* ------------------ CÓDIGO ------------------ */

void setup() {
  doInit();
}

void loop() {
  maquinaEstados(estado, evento);
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

int generarEvento(long distancia) {

  // Verificamos en que parte del rango esta
  int resultado = estaDentroRango(UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM, distancia);

  if(resultado == DENTRO_RANGO) {
    return EVT_OBJETO_DETECTADO;
  } else if(resultado == FUERA_RANGO) {
    return EVT_OBJETO_NO_DETECTADO;
  }
}

/*
* Es la implementación de una máquina de estados. 
* Dado un estado y un evento, establece el nuevo estado.
*/

void maquinaEstados(int estado, int evento){
  
  // Segun el estado en el que nos encontramos llamamos a una función
  switch(estado) {

    case ST_INACTIVO:
      stInactivo(evento);
      break;

    case ST_ESPERANDO_DETECCION:
      stEsperandoDeteccion(evento);
      break;
    
    case ST_OBJETO_DETECTADO:
      stObjetoDetectado(evento);
      break;
    
    default:
      break;
  }

  // Generamos el evento para la siguiente pasada
  evento = generarEvento(obtenerDistancia(PIN_TRIG, PIN_ECHO));
}

/*
* Implementación de cada uno de lso estados de la máquina de estados.
*/

void stInactivo(int evento){
  switch(evento){

    case EVT_CONTINUE:
      estado = ST_ESPERANDO_DETECCION;
      break;

    default:
      break;
  }
}

void stEsperandoDeteccion(int evento){
  switch(evento){

    case EVT_OBJETO_NO_DETECTADO:
      // Apagamos LED
      digitalWrite(PIN_LED, LOW);

      // Cambiamos de estado
      estado = ST_ESPERANDO_DETECCION;
      break;

    case EVT_OBJETO_DETECTADO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}

void stObjetoDetectado(int evento){
  switch(evento){

    case EVT_OBJETO_NO_DETECTADO:
      // Apagamos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      estado = ST_ESPERANDO_DETECCION;
      break;

    case EVT_OBJETO_DETECTADO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}