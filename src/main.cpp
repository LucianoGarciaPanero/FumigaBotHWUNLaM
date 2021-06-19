#include <main.h>

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

int generarEvento(long distancia){

  // Inicializamos las variables a usar
  int evento = EVT_CONTINUE;
 
  // Verificación de la distancia del objeto
  if(distancia > UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM || distancia <= 0) {
    evento = EVT_OBJETO_NO_DETECTADO;
  } else if(distancia > 0 && distancia <= UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM) {
    evento = EVT_OBJETO_NO_DETECTADO;
  }

  return evento;
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