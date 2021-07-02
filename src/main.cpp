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
     digitalWrite(PIN_LED_DISTANCIA, LOW);
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
  pinMode(PIN_LED_DISTANCIA, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);

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

/* ------------------ SECCIÓN MdE SENSORES ------------------ */

/*
* Inicializa los estados correspondientes con la MdE.
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
* Implementación de cada uno de los estados de la MdE.
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
      digitalWrite(PIN_LED_DISTANCIA, LOW);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      // Encendemos el LED
      digitalWrite(PIN_LED_DISTANCIA, HIGH);

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
      digitalWrite(PIN_LED_DISTANCIA, HIGH);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      // Encendemos el LED
      digitalWrite(PIN_LED_DISTANCIA, HIGH);

      // Cambiamos de estado
      sensores[nro].estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE CONEXIONES ------------------ */

/*
* Inicializa los estados correspondientes con la MdE.
*/

void doInitMdEConexiones(void) {
  
  // Inicializamos estados generales
  stConexiones = ST_INACTIVO;
  evtConexiones = EVT_CONTINUAR;

}

/*
* Genera los eventos para la MdE.
*/

void generarEventoMdEConexiones(void) {

  if(WiFi.status() != WL_CONNECTED) {

    // Realizamos la conexión a wifi
    conectarWifi();

    // Verificamos si se logra conectar
    if(WiFi.status() != WL_CONNECTED) {
      evtConexiones = EVT_DESCONEXION_WIFI;
    } else {
      evtConexiones = EVT_CONEXION_EXITOSA_WIFI;
    }

  } else if(!conectadoFB) {
   
    // Realizmaos la coneixón al Firebase
    bool exito = conectarFB();
  
    // Verificamos que logro la conexión
    if(exito) {

      evtConexiones = EVT_CONEXION_EXITOSA_FB;
      conectadoFB = true;

    } else {

      evtConexiones = EVT_CONEXION_RECHAZADA_FB;
      conectadoFB = false;

    }
  } else {  
    
    evtConexiones = EVT_CONEXION_EXITOSA_FB;

  }
} 

/*
* Implementación de cada uno de los estados de MdE.
*/

void maquinaEstadosConexiones() {
    
    // Segun el estado en el que nos encontramos llamamos a una función
    switch(stConexiones) {

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
  switch(evtConexiones){
    
    case EVT_DESCONEXION_WIFI:
      stConexiones = ST_REALIZANDO_CONEXION_WIFI;
      break;

    case EVT_CONEXION_EXITOSA_WIFI:
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;
    
    default:
      break;
  }
}

void stRealizandoConexionFB() {
  switch(evtConexiones){
    
    case EVT_CONEXION_RECHAZADA_FB:
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_CONEXION_EXITOSA_FB:
      stConexiones = ST_CONECTADO_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      stConexiones = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    default:
      break;
  }
}

void stConectadoFB() {
  switch(evtConexiones) {
    
    case EVT_DESCONEXION_FB:
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      stConexiones = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    case EVT_CONEXION_EXITOSA_FB:
      stConexiones = ST_CONECTADO_FB;
      break;
    
    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE BATERÍA ------------------ */

/*
* Inicializa los estados correspondientes con la MdE.
*/

void doInitMdEBateria(void) {
  
  // Inicialización estados y eventos
  stBateria = ST_CALCULANDO_NIVEL_BATERIA;
  evtBateria = EVT_COMUNICAR_CARGA;

  // Inicialización variables
  cargaBateria = 0;
}

/*
* Genera los eventos para la MdE.
*/

void generarEventoMdEBateria(void) {
  
  if(cargaBateria != 0) {
    evtBateria = EVT_COMUNICAR_CARGA;
  } else {
    evtBateria = EVT_DETECTAR_CARGA;
  }
}

/*
* Implementación de cada uno de los estados de la MdE.
*/

void maquinaEstadosBateria(void) {
  switch(stBateria)  {
  
  case ST_CALCULANDO_NIVEL_BATERIA:
    stCalculandoNivelBateria();
    break;
  
  case ST_COMUNICANDO_CARGA_FB:
    //  DO NOTHING
    break;

  default:
    break;
  }

  generarEventoMdEBateria();
}

void stCalculandoNivelBateria(void) {

  // Calculamos la carga de la batería  
  cargaBateria = obtenerNivelBateria(
    PIN_VIN_BATERIA, 
    VOLTAJE_MIN_ESP, 
    VOLTAJE_MAX_BATERIA);

  switch(evtBateria) {

    case EVT_COMUNICAR_CARGA:

      // Comunicar la carga de batería en Firebase
      Firebase.RTDB.setFloat(&fbWrite, pathHojaBateria.c_str(), cargaBateria);

      // Prendemos el LED segun el nivel de batería
      if(cargaBateria  < MAX_VALOR_NIVEL_BAJO) {

        // Prendemos solo el LED rojo
        digitalWrite(PIN_LED_VERDE, LOW);
        digitalWrite(PIN_LED_AMARILLO, LOW);
        digitalWrite(PIN_LED_ROJO, HIGH);

      } else if(cargaBateria < MAX_VALOR_NIVEL_MEDIO) {

        // Prendemos solo el LED amarillo
        digitalWrite(PIN_LED_VERDE, LOW);
        digitalWrite(PIN_LED_AMARILLO, HIGH);
        digitalWrite(PIN_LED_ROJO, LOW);

      } else {

        // Prendemos solo el LED verde
        digitalWrite(PIN_LED_VERDE, HIGH);
        digitalWrite(PIN_LED_AMARILLO, LOW);
        digitalWrite(PIN_LED_ROJO, LOW);

      }

      stBateria =  ST_COMUNICANDO_CARGA_FB;
      break;

    case EVT_DETECTAR_CARGA:
      stBateria = ST_CALCULANDO_NIVEL_BATERIA;
      break;

    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE GENERAL ------------------ */

/*
* Inicializa los estados correspondientes con la MdE.
*/

void doInitMdEGeneral(void) {
  
  // Inicializaicón estado y evento
  stGeneral = ST_INACTIVO;
  evtGeneral = EVT_CONTINUAR;

  // Inicialización del a variable a usar
  lastTime = millis();
}

/*
* Genera los eventos para la MdE.
*/

void generarEventoMdEGeneral(void) {

  // Verificación de que haya ocurrido timeout en el timer
  if(millis() - lastTime >=  TIEMPO_VERIFICACION_BATERIA_MS) {
    
    // Volvemos a tomar el tiempo
    lastTime = millis();

    // Asignamos el evento
    evtGeneral = EVT_FIN_TIMER;
  } else {
    evtGeneral = EVT_CONTINUAR;
  }

}

/*
* Implementación de cada uno de los estados de la MdE.
*/

void maquinaEstadosGeneral() {
  switch(stGeneral) {

    case ST_INACTIVO:
      stInactivo();
      break;
   
    case ST_REALIZANDO_CONEXIONES:
      stRealizandoConexiones();
      break;
    
    case ST_DETECTANDO_CARGA_BATERIA:
      stDetectandoCargaBateria();
      break;
    
    default:
      break;
  }

  generarEventoMdEGeneral();
}

void stInactivo() {
  switch(evtGeneral) {

    case EVT_CONTINUAR:
      
      doInitMdEConexiones();
      maquinaEstadosConexiones();

      stGeneral = ST_REALIZANDO_CONEXIONES;
      break;

    default:
      break;
  }
}

void stRealizandoConexiones() {
  switch(evtGeneral) {

    case EVT_FIN_TIMER:

      // Informamos que el micro sigue prendido
      Firebase.RTDB.setBool(&fbWrite, pathHojaEncendido.c_str(), true);

      // Realizamos la acción para cambiar de MdE
      doInitMdEBateria();
      maquinaEstadosBateria();

      stGeneral = ST_DETECTANDO_CARGA_BATERIA;
      break;

    case EVT_CONTINUAR:

      maquinaEstadosConexiones();

      stGeneral = ST_REALIZANDO_CONEXIONES;
      break;

    default:
      break;
  }
}

void stDetectandoCargaBateria() {
  switch(evtGeneral) {

    case EVT_FIN_TIMER:

      // Informamos que el micro sigue prendido
      Firebase.RTDB.setBool(&fbWrite, pathHojaEncendido.c_str(), true);

      // Realizamos la acción para cambiar de MdE
      doInitMdEConexiones();
      maquinaEstadosConexiones();

      stGeneral = ST_REALIZANDO_CONEXIONES;
      break;

    case EVT_CONTINUAR:
      maquinaEstadosBateria();

      stGeneral = ST_DETECTANDO_CARGA_BATERIA;
      break;

    default:
      break;
  }
}

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