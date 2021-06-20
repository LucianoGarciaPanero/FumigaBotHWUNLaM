/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
#include <config.h>
#include <WiFi.h>

#include <sensores.h>
#include <utilitarias.h>

/* ------------------ ESTADOS ------------------ */

// Estados MdE general
#define ST_INACTIVO                     0
#define ST_REALIZANDO_CONEXIÓN_WIFI     1
#define ST_CONECTADO_WIFI               2
#define ST_DETECTANDO_OBJETO            3

// Estados MdE sensores distancia
#define ST_OBJETO_NO_DETECTADO          4
#define ST_OBJETO_DETECTADO             5   
                                                                                                                            
/* ------------------ EVENTOS ------------------ */

// Eventos MdE general
#define EVT_CONTINUAR                   0
#define EVT_ACABA_TIEMPO                1
#define EVT_CONEXION_EXITOSA            2
#define EVT_DESCONEXION                 3
#define EVT_COMENZAR_DETECCIÓN          4

// Eventos MdE sensores distancia
#define EVT_OBJETO_FUERA_RANGO          5
#define EVT_OBJETO_DENTRO_RANGO         6

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);
void generarEvento(void);
void maquinaEstados(void);

// Estados MdE general
void stInactivo(void);
void stRealizandoConexionWifi(void);
void stConectandoWifi(void);
void stDetectandoObjeto(void);

// Estados MdE sensores distancia
void stObjetoNoDetectado(void);
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
  glbEvento = EVT_CONTINUAR;
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
    glbEvento = EVT_OBJETO_DENTRO_RANGO;
  } else {
    glbEvento = EVT_OBJETO_FUERA_RANGO;
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

    case ST_OBJETO_NO_DETECTADO:
      stObjetoNoDetectado();
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
* Implementación de cada uno de los estados de la máquina de estados.
*/

void stInactivo(){
  switch(glbEvento){

    case EVT_CONTINUAR:
      glbEstado = ST_OBJETO_NO_DETECTADO;
      break;

    default:
      break;
  }
}

void stObjetoNoDetectado(){
  switch(glbEvento){

    case EVT_OBJETO_FUERA_RANGO:
      // Apagamos LED
      digitalWrite(PIN_LED, LOW);

      // Cambiamos de estado
      glbEstado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
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

    case EVT_OBJETO_FUERA_RANGO:
      // Apagamos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      glbEstado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      glbEstado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}