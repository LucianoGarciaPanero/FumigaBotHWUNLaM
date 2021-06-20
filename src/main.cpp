/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
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
  //maquinaEstadosSensoresDistancia();
}

/* ------------------ DEFINICIÓN FUNCIONES ------------------ */

/*
* Inicializa los pines en los modos correctos y las variables necesarias
*/

void doInit(){

  pinesEcho[0] = PIN_ECHO_0;
  pinesTrig[0] = PIN_TRIG_0;

  // Inicializamos los sensores de distancia
  for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
    
    sensores[i].distancia = 0;

    // Para MdE
    sensores[i].estado = ST_OBJETO_DETECTADO;
    sensores[i].evento = EVT_OBJETO_FUERA_RANGO;
    
    // Inicialización pines
    sensores[i].pinEcho = pinesEcho[i];
    sensores[i].pinTrig = pinesTrig[i];
    pinMode(sensores[i].pinEcho, INPUT);
    pinMode(sensores[i].pinEcho, OUTPUT);
  }

  // Inicializamos pin LED
  pinMode(PIN_LED, OUTPUT);
}


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
      maquinaEstadosSensoresDistancia();
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


/*
*  Implementación MdE para sensores de distancia
*/

void maquinaEstadosSensoresDistancia(void) {

  for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++){
    sensores[i].evento = generarEventoSensorDistancia(sensores[i].pinTrig, sensores[i].pinEcho);

    switch(sensores[i].estado) {
    case ST_OBJETO_NO_DETECTADO:
      stObjetoNoDetectado(i);
      break;

    case ST_OBJETO_DETECTADO:
      stObjetoDetectado(i);
      break;
    
    default:
      break;
    }
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


int generarEventoSensorDistancia(int pinTrig, int pinEcho) {
  
  // Verificamos en que parte del rango esta
  bool resultado = estaDentroRango(
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM, 
    obtenerDistancia(pinTrig, pinEcho)
    );

  // De acuerdo al valor devolvemos el evento correspondiente
  if(resultado) {
    return EVT_OBJETO_DENTRO_RANGO;
  } else {
    return EVT_OBJETO_FUERA_RANGO;
  }
}