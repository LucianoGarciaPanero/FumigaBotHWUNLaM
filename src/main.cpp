#include <main.h>

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  Serial.begin(VEL_TRANSMISION);
  
  doInit();
}

void loop() {
  maquinaEstadosGeneral();
}

/* ------------------ SECCIÓN INIT ------------------ */

/*
* Inicializa los pines en los modos correctos y las variables necesarias
*/

void doInit(){

  // Inicializamos pin LED
  pinMode(PIN_LED, OUTPUT);

  // Inicialización pines sensores distancia
  for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
    
    // Inicialización pines
    sensores[i].pinEcho = pinesEcho[i];
    sensores[i].pinTrig = pinesTrig[i];
      
    pinMode(sensores[i].pinEcho, INPUT);
    pinMode(sensores[i].pinTrig, OUTPUT);
  }

  // Inicialización MdE
  doInitMdEGeneral();

  // Inicialización variables
  conectadoWifi = false;
  conectadoFB = false;
}

/*
* Inicializa los estados correspondientes con la MdE general.
*/

void doInitMdEGeneral(void){
    
  // Inicializamos estados generales
  glbEstado = ST_INACTIVO;
  glbEvento = EVT_CONTINUAR;

  // Inicialización MdE sensores
  doInitMdESesonres();
}

/*
* Inicializa los estados correspondientes con la MdE de los sensores.
*/

void doInitMdESesonres() {
  
    // Inicializamos los estados
    for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
      sensores[i].estado = ST_OBJETO_DETECTADO;
      sensores[i].evento = EVT_OBJETO_FUERA_RANGO;
    }
}

/* ------------------ SECCIÓN GENERAR EVENTO ------------------ */

/*
* Genera los eventos para la MdE general en base al estado de la conexión WiFi.
*/

void generarEventoMdEGeneral(void) {

  if(!conectadoWifi) {

    // Medimos el punto de inicio
    unsigned long startTime = millis();
    
    // Hacemos que intente conectarse durante 20 ms
    while(WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
    }

    // Verificamos si se logra conectar
    if(WiFi.status() != WL_CONNECTED) {
      
      glbEvento = EVT_ACABA_TIEMPO_WIFI;
      return;
    } else {

      // Marcamos que logro conectarse a WiFI
      conectadoWifi = true;
      glbEvento = EVT_CONEXION_EXITOSA_WIFI;
      return;
    }
  }

  // Si ocurre una desconexión del wifi debemos volver a conectarnos nuevamente
  if(WiFi.status() != WL_CONNECTED) {
    glbEvento = EVT_DESCONEXION_WIFI;
    return;
  }

  glbEvento = EVT_COMENZAR_DETECCION;
  
}

/*
* Genera un evento a partir de analizar los sensores.
* Si un objeto se encuentra den intervalo genera un evento
* objeto detectado, en caso contraro objeto no detectado.
*/

int generarEventoMdESensorDistancia(int pinTrig, int pinEcho) {
  
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

/* ------------------ SECCIÓN MdE GENERAL ------------------ */

void maquinaEstadosGeneral() {
    
    // Segun el estado en el que nos encontramos llamamos a una función
    switch(glbEstado) {
        
        case ST_INACTIVO:
          stInactivo();
          break;

        case ST_REALIZANDO_CONEXION_WIFI:
          stRealizandoConexionWifi();
          break;

        case ST_CONECTADO_WIFI:
          stConectadoWifi();
          break;

        case ST_REALIZANDO_CONEXION_FB:
          stRealizandoConexionFB();
          break;
        
        case ST_CONECTADO_FB:
          stConectadoFB();
          break;

        case ST_DETECTANDO_OBJETO:
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
      conectarWifi();
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;

    default:
      break;
  }
}

void stRealizandoConexionWifi(void) {
  switch(glbEvento){
    
    case EVT_ACABA_TIEMPO_WIFI:
      conectarWifi();
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;

    case EVT_CONEXION_EXITOSA_WIFI:
      glbEstado = ST_CONECTADO_WIFI;
      break;
    
    default:
      break;
  }
}

void stConectadoWifi(void) {

  switch(glbEvento) {

  case EVT_DESCONEXION_WIFI:
    conectarWifi();
    glbEstado = ST_REALIZANDO_CONEXION_WIFI;
    break;

  case EVT_CONTINUAR:
    glbEstado = ST_REALIZANDO_CONEXION_FB;
    break;

  default:
    break;
  }
}

void stRealizandoConexionFB() {
  switch(glbEvento){
    
    case EVT_ACABA_TIEMPO_FB:
      glbEstado = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_CONEXION_EXITOSA_FB:
      glbEstado = ST_CONECTADO_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      conectarWifi();
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    default:
      break;
  }
}

void stConectadoFB() {
  switch(glbEvento) {
    
    case EVT_DESCONEXION_FB:
      glbEstado = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      conectarWifi();
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    case EVT_COMENZAR_DETECCION:
      glbEstado = ST_DETECTANDO_OBJETO;
      break;
    
    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE SENSORES DISTANCIA ------------------ */

void maquinaEstadosSensoresDistancia(int nro) {

    switch(sensores[nro].estado) {
      case ST_OBJETO_NO_DETECTADO:
        stObjetoNoDetectado(nro);
        break;

      case ST_OBJETO_DETECTADO:
        stObjetoDetectado(nro);
        break;
    
      default:
        break;
      }
    
    // Generación evento para próxima pasada
    sensores[nro].evento = generarEventoMdESensorDistancia(sensores[nro].pinTrig, sensores[nro].pinEcho);
  }

/*
* Implementación de cada uno de los estados de la máquina de estados.
*/

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

/* ------------------ SECCIÓN CONEXIONES ------------------ */

void conectarWifi() {
  
  // Modo para que el ESP se conecte a una red
  WiFi.mode(WIFI_STA);

  // Comenzamos conexión
  WiFi.begin(WIFI_RED, WIFI_CONTRASENIA);

  // Marcamos que es la primera conexión que se realiza
  conectadoWifi = false;
}

/*
void conectarFB(void) {

  // Iniciar conexión
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  conectadoFB = false;
}
*/