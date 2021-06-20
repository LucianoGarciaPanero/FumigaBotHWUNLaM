#include <WiFi.h>

#include <maquinaEstados.h>
#include <config.h>
#include <sensores.h>
#include <utilitarias.h>

/*
* Inicializa las variables globales 
*/

void doInitMdE(void) {
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

    case ST_REALIZANDO_CONEXION_WIFI:
      stRealizandoConexionWifi();
      break;

    case ST_CONECTADO_WIFI:
      stConectandoWifi();
      break;

    case ST_DETECTANDO_OBJETO:
      
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