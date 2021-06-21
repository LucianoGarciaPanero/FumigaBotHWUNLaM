#include "maquinaEstados.h"

#include <Arduino.h>
#include <sensores.h>
#include <utilitarias.h>


/* -------- PINES --------  */
// LED
#define PIN_LED 4

// Sensor Ultrasonido
#define PINES_TRIG {13}
#define PINES_ECHO {14}
#define CANT_SENSORES_DISTANCIA 1

/* -------- OTROS --------  */

// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM 20
#define UMBRAL_MINIMA_DISTANCIA_OBJETO_CM 1

/* ------------------ VARIABLES GLOBALES ------------------ */

stSensorDistancia sensores[CANT_SENSORES_DISTANCIA];
int pinesEcho[CANT_SENSORES_DISTANCIA] = PINES_ECHO;
int pinesTrig[CANT_SENSORES_DISTANCIA] = PINES_TRIG;

int glbEstado;
int glbEvento;


void doInitMdEGeneral(void){
    
  // Inicializamos estados generales
  glbEstado = ST_INACTIVO;
  glbEstado = EVT_CONTINUAR;

  // Inicialización MdE sensores
  doInitMdESesonres();
}

/*
* Genera un evento a partir de analizar los sensores.
* Si un objeto se encuentra den intervalo genera un evento
* objeto detectado, en caso contraro objeto no detectado.
*/

void generarEventoMdEGeneral(void) {

  switch(glbEstado) {
    case ST_REALIZANDO_CONEXION_WIFI:
      glbEvento = EVT_CONEXION_EXITOSA;
      break;
    case ST_CONECTADO_WIFI:
      glbEvento = EVT_COMENZAR_DETECCION;
      break;

    default:
      break;
  }
  
}

/*
* Es la implementación de una máquina de estados. 
* Dado un estado y un evento, establece el nuevo estado.
*/

void maquinaEstadosGeneral() {
    
    // Segun el estado en el que nos encontramos llamamos a una función
    switch(glbEstado) {
        
        case ST_INACTIVO:
            Serial.println("ESTADO INACTIVO");
            stInactivo();
            break;

        case ST_REALIZANDO_CONEXION_WIFI:
            Serial.println("ESTADO REALIZANDO CONEXIÓN WIFI");
            stRealizandoConexionWifi();
            break;

        case ST_CONECTADO_WIFI:
            Serial.println("CONECTANDO WIFI");
            stConectandoWifi();
            break;

        case ST_DETECTANDO_OBJETO:
            Serial.println("DETECTANDO OBJETO");
            for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++){
              maquinaEstadosSensoresDistancia(i);
            }
            break;

        default:
            break;
    }

  // Generamos el evento para la siguiente pasada
  generarEventoMdEGeneral();
}

/*
* Implementación de cada uno de los estados de la máquina de estados.
*/

void stInactivo(){
  switch(glbEvento){

    case EVT_CONTINUAR:
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;

    default:
      break;
  }
}

void stRealizandoConexionWifi(void) {
  switch(glbEvento){
    
    case EVT_ACABA_TIEMPO:
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;

    case EVT_CONEXION_EXITOSA:
      glbEstado = ST_CONECTADO_WIFI;
      break;
    
    default:
      break;
  }
}

void stConectandoWifi(void) {

  switch(glbEvento) {

  case EVT_DESCONEXION:
    glbEstado = ST_REALIZANDO_CONEXION_WIFI;
    break;

  case EVT_COMENZAR_DETECCION:
    glbEstado = ST_DETECTANDO_OBJETO;
    break;

  default:
    break;
  }
}

void doInitMdESesonres() {
  
    // Inicializamos los sensores de distancia
    for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
      // Para MdE
      sensores[i].estado = ST_OBJETO_DETECTADO;
      sensores[i].evento = EVT_OBJETO_FUERA_RANGO;
    
      // Inicialización pines
      sensores[i].pinEcho = pinesEcho[i];
      sensores[i].pinTrig = pinesTrig[i];
      
      pinMode(sensores[i].pinEcho, INPUT);
      pinMode(sensores[i].pinTrig, OUTPUT);
    }
}


/*
*  Implementación MdE para sensores de distancia
*/

void maquinaEstadosSensoresDistancia(int nro) {
    sensores[nro].evento = generarEventoMdESensorDistancia(sensores[nro].pinTrig, sensores[nro].pinEcho);

    switch(sensores[nro].estado) {
      case ST_OBJETO_NO_DETECTADO:
        Serial.println("OBJETO NO DETECTADO");
        stObjetoNoDetectado(nro);
        break;

      case ST_OBJETO_DETECTADO:
        Serial.println("OBJETO DETECTADO");
        stObjetoDetectado(nro);
        break;
    
      default:
        break;
      }
  }


void stObjetoNoDetectado(int nro){
  switch(sensores[nro].evento){

    case EVT_OBJETO_FUERA_RANGO:
      // Apagamos LED
      digitalWrite(PIN_LED, LOW);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}

void stObjetoDetectado(int nro){
  switch(sensores[nro].evento){

    case EVT_OBJETO_FUERA_RANGO:
      // Apagamos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      // Encendemos el LED
      digitalWrite(PIN_LED, HIGH);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}


int generarEventoMdESensorDistancia(int pinTrig, int pinEcho) {
  
  long distancia = obtenerDistancia(pinTrig, pinEcho);
  Serial.println(distancia);
  
  // Verificamos en que parte del rango esta
  bool resultado = estaDentroRango(
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM, 
    distancia
    );

  // De acuerdo al valor devolvemos el evento correspondiente
  if(resultado) {
    return EVT_OBJETO_DENTRO_RANGO;
  } else {
    return EVT_OBJETO_FUERA_RANGO;
  }
}