#include <main.h>

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  //Serial.begin(VEL_TRANSMISION);

  // Inicialización pines, variables, etc.
  doInit();
  
  // Inicialización de tareas en cada core
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

/******************************************************************* 
Nombre: codigoTaskCero
Entradas: -
Salida: -
Proceso: código a ejecutar en el procesador cero, se encarga de realizar la 
fumigación
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void codigoTaskCero(void *param) {
  
  /* SETUP */
  
  // Inicialización MdE
  doInitMdESesonres();
  doInitMdECoreCero();

  /* LOOP */
  while(true) {

    //  Solo se ejecuta la MdE de fumgar si se recibe la señal del Firebase.
    if(senialFumigar) {

      maquinaEstadosCoreCero();
    
    } else {

      // Apagar la bomba
      digitalWrite(PIN_BOMBA, LOW);

    }

    // Le damos tiempo a las tareas en background a ejecutarse
    delay(10);
  }
}

/******************************************************************* 
Nombre: codigoTaskUno
Entradas: -
Salida: -
Proceso: código a ejecutar en el procesador uno, se encarga de realizar la 
conexión al WiFi y Firebase
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void codigoTaskUno(void *param) {
  
  /* SETUP */
  
  doInitMdECoreUno();
  

  /* LOOP */

  while(true) {
    
    maquinaEstadosCoreUno();

    
    // Le damos tiempo a las tareas en background a ejecutarse
    delay(10);
  }
}

/* ------------------ SECCIÓN INIT ------------------ */

/******************************************************************* 
Nombre: doInit
Entradas: -
Salida: -
Proceso: inicializa las variables globales y los pines
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: Tomás Sánchez Grigioni
Fecha Cambió: 24/09/2021
Referencia: Agregamos inicilización de pines encargados del movimiento del
robot
*****************************************************************/

void doInit(){

  // Inicialización pines sensores distancia
  pinMode(PIN_TRIG_ADELANTE, OUTPUT);
  pinMode(PIN_ECHO_ADELANTE, INPUT);

  pinMode(PIN_TRIG_DERECHA, OUTPUT);
  pinMode(PIN_ECHO_DERECHA, INPUT);

  // Inicialización pines motores
  pinMode(PIN_MOTOR_IZQUIERDA_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IZQUIERDA_IN2, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_0, FREQ, RESOLUTION);
  ledcAttachPin(PIN_MOTOR_IZQUIERDA_EN, PWM_CHANNEL_0);
  
  pinMode(PIN_MOTOR_DERECHA_IN3, OUTPUT);
  pinMode(PIN_MOTOR_DERECHA_IN4, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_1, FREQ, RESOLUTION);
  ledcAttachPin(PIN_MOTOR_DERECHA_EN, PWM_CHANNEL_1);


  // Inicialización variables
  conectadoFB = false;
  senialFumigar = false;
}

/* ------------------ SECCIÓN MdE CONEXIONES ------------------ */

void doInitMdEConexiones(void) {
  
  // Inicializamos estados generales
  stConexiones = ST_INACTIVO;
  evtConexiones = EVT_CONTINUAR;

}

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

/* ------------------ SECCIÓN CONEXIÓN WIFI ------------------ */

/******************************************************************* 
Nombre: conectarWifi
Entradas: -
Salida: -
Proceso: conecta al wifi, y lo reintenta durante 20 ms
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

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

/******************************************************************* 
Nombre: conectarFB
Entradas: -
Salida: -
Proceso: conecta con Firebase, y genera la suscripción a la hoja de fumigar
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

bool conectarFB(void) {

  // Asignamos los datos de la base de datos en tiempo real
  config.database_url = FIREBASE_URL;
  config.signer.tokens.legacy_token = FIREBASE_SECRETO;
  
  // Hacemos la conexión
  Firebase.begin(&config, &auth);

  // Nos suscribimos a la hoja de fumigar
  bool resultado =  Firebase.RTDB.beginStream(&fbConection, pathHojaFumigar.c_str());

  if(resultado) {
    // Establece las acciones cuando ocurre una actualización
    Firebase.RTDB.setStreamCallback(&fbConection, streamCallback, streamTimeoutCallback);
  }

  return resultado;
}

/******************************************************************* 
Nombre: streamCallback
Entradas: -
Salida: -
Proceso: se llama a la función en el momento que se detecta un cambio
en la hoja fumigación. Si el valor es true reinicia la MdE del core cero
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void streamCallback(FirebaseStream data) {
  
  senialFumigar = data.boolData();

}

/******************************************************************* 
Nombre: streamTimeoutCallback
Entradas: -
Salida: -
Proceso: se llama a la función en el momento que se detecta un timeout
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void streamTimeoutCallback(bool timeout) {}