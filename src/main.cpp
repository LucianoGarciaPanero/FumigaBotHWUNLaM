#include <main.h>

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  Serial.begin(VEL_TRANSMISION);

  // Inicialización pines, variables, etc.
  doInit();
  
  // Inicialización de tareas
  xTaskCreatePinnedToCore(
    codigoTaskCero, // Código a ejecutar
    "Task0",        // Un identificador
    10000,          // Tamaño del stack
    NULL,           // Parámetros que recibe el código
    1,              // Prioridad
    &task0,         // Objeto para manejar la task
    0);             // Número de procesador

  xTaskCreatePinnedToCore(
    codigoTaskUno,  // Código a ejecutar
    "Task1",        // Un identificador
    10000,          // Tamaño del stack
    NULL,           // Parámetros que recibe el código
    1,              // Prioridad
    &task1,         // Objeto para manejar la task
    1);             // Número de procesador
}

void loop() {vTaskDelete(NULL);}

/* ------------------ SECCIÓN TAREAS ------------------ */

/*
* Código a ejecutar en el core 0.
* Máquina de estados para hacer la detección de objetos
*/

void codigoTaskCero(void *param) {
  
  /* SETUP */
  
  // Inicialización MdE sensores
  doInitMdESesonres();

  /* LOOP */
  while(true) {

    //  Solo se ejecuta la MdE de fumgar si se recibe la señal del Firebase.
    if(senialFumigar) {

      // Recorremos todos los sensores y ejecutamos sus MdE
     for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++){
            maquinaEstadosSensoresDistancia(i);
      }

    } else {
     digitalWrite(PIN_LED, LOW);
    }
   
    // Le damos tiempo a las tareas en background a ejecutarse
    delay(10);
  }
}

/*
* Código a ejecutar en el core 1.
* Máquina de estados para la conexión a internet y conectarse a Wifi y FB.
*/

void codigoTaskUno(void *param) {
  
  /* SETUP */
  
  // Inicialización MdE
  doInitMdEGeneral();

  /* LOOP */
  while(true) {
    
    maquinaEstadosGeneral();

    
    // Le damos tiempo a las tareas en background a ejecutarse
    delay(10);
  }
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

  // Inicialización variables
  conectadoFB = false;
  senialFumigar = false;
}

/*
* Inicializa los estados correspondientes con la MdE general.
*/

void doInitMdEGeneral(void){}

/* ------------------ SECCIÓN GENERAR EVENTO ------------------ */

/*
* Genera los eventos para la MdE general en base al estado de la conexión WiFi.
*/

void generarEventoMdEGeneral(void) {

  if(WiFi.status() != WL_CONNECTED) {

    // Realizamos la conexión a wifi
    conectarWifi();

    // Verificamos si se logra conectar
    if(WiFi.status() != WL_CONNECTED) {
      glbEvento = EVT_DESCONEXION_WIFI;
    } else {
      glbEvento = EVT_CONEXION_EXITOSA_WIFI;
    }

  } else if(!conectadoFB) {
   
    // Realizmaos la coneixón al Firebase
    bool exito = conectarFB();
  
    // Verificamos que logro la conexión
    if(exito) {

      glbEvento = EVT_CONEXION_EXITOSA_FB;
      conectadoFB = true;

    } else {

      glbEvento = EVT_CONEXION_RECHAZADA_FB;
      conectadoFB = false;

    }
  } else {  
    
    glbEvento = EVT_CONTINUAR;

  } 
  
}
/* ------------------ SECCIÓN MdE SENSORES ------------------ */

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

/*
* Implementación de cada uno de los estados de la máquina de estados.
*/

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
    sensores[nro].evento = generarEventoMdESensorDistancia(
      sensores[nro].pinTrig, 
      sensores[nro].pinEcho);
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

/* ------------------ SECCIÓN MdE CONEXIONES ------------------ */

/*
* Inicializa los estados correspondientes con la MdE de los sensores.
*/

void doInitMdEConexiones(void) {
  
  // Inicializamos estados generales
  glbEstado = ST_INACTIVO;
  glbEvento = EVT_CONTINUAR;

}

/*
* Genera los eventos para la MdE general en base al estado de la conexión WiFi.
*/

void generarEventoMdEConexiones(void) {

  if(WiFi.status() != WL_CONNECTED) {

    // Realizamos la conexión a wifi
    conectarWifi();

    // Verificamos si se logra conectar
    if(WiFi.status() != WL_CONNECTED) {
      glbEvento = EVT_DESCONEXION_WIFI;
    } else {
      glbEvento = EVT_CONEXION_EXITOSA_WIFI;
    }

  } else if(!conectadoFB) {
   
    // Realizmaos la coneixón al Firebase
    bool exito = conectarFB();
  
    // Verificamos que logro la conexión
    if(exito) {

      glbEvento = EVT_CONEXION_EXITOSA_FB;
      conectadoFB = true;

    } else {

      glbEvento = EVT_CONEXION_RECHAZADA_FB;
      conectadoFB = false;

    }
  } else {  
    
    glbEvento = EVT_CONEXION_EXITOSA_FB;

  }
} 

/*
* Implementación de cada uno de los estados de la máquina de estados.
*/

void maquinaEstadosBateria() {
    
    // Segun el estado en el que nos encontramos llamamos a una función
    switch(glbEstado) {

        case ST_REALIZANDO_CONEXION_WIFI:
          stRealizandoConexionWiFi();
          break;

        case ST_REALIZANDO_CONEXION_FB:
          stRealizandoConexionFB();
          break;
        
        case ST_CONECTADO_FB:
          stConectadoFB();
          break;

        default:
            break;
    }

  // Generamos el evento para la siguiente pasada
  generarEventoMdEConexiones();
}

void stRealizandoConexionWiFi(void) {
  switch(glbEvento){
    
    case EVT_DESCONEXION_WIFI:
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;

    case EVT_CONEXION_EXITOSA_WIFI:
      glbEstado = ST_REALIZANDO_CONEXION_FB;
      break;
    
    default:
      break;
  }
}

void stRealizandoConexionFB() {
  switch(glbEvento){
    
    case EVT_CONEXION_RECHAZADA_FB:
      glbEstado = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_CONEXION_EXITOSA_FB:
      glbEstado = ST_CONECTADO_FB;
      break;

    case EVT_DESCONEXION_WIFI:
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
      glbEstado = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    case EVT_CONEXION_EXITOSA_FB:
      glbEstado = ST_CONECTADO_FB;
      break;
    
    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE GENERAL ------------------ */

/*
* Implementación de cada uno de los estados de la máquina de estados.
*/

void maquinaEstadosGeneral() {}

void stInactivo(){}

/* ------------------ SECCIÓN CONEXIÓN WIFI ------------------ */

void conectarWifi() {
  
  // Modo para que el ESP se conecte a una red
  WiFi.mode(WIFI_STA);

  // Comenzamos conexión
  WiFi.begin(WIFI_RED, WIFI_CONTRASENIA);

  // Medimos el punto de inicio
  unsigned long startTime = millis();
    
  // Hacemos que intente conectarse durante 20 ms
  while(WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {}

}

/* ------------------ SECCIÓN CONEXIÓN FIREBASE ------------------ */

bool conectarFB(void) {

  // Asignamos los datos de la base de datos en tiempo real
  config.database_url = FIREBASE_URL;
  config.signer.tokens.legacy_token = FIREBASE_SECRETO;
  
  // Hacemos la conexión
  Firebase.begin(&config, &auth);

  // Nos suscribimos a la hoja de fumigar
  bool resultado =  Firebase.RTDB.beginStream(&fbConection, pathHojaFumigar.c_str());

  // Establece las acciones cuando ocurre una actualización
  Firebase.RTDB.setStreamCallback(&fbConection, streamCallback, streamTimeoutCallback);

  return resultado;
}


/*
* Función que se ejecuta cada vez que se recibe una actualización de Firebase.
* Genera el evento para comenzar la detección.
*/

void streamCallback(FirebaseStream data) {
  senialFumigar = data.boolData();
}

/*
* Esta función se ejecuta en caso de que se reciba un timeout.
*/

void streamTimeoutCallback(bool timeout) {
  if(timeout){

    // TODO: ACCIÓN POR UN TIMEOUT
  
  }
}