#include <main.h>

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  Serial.begin(VEL_TRANSMISION);

  // Inicialización pines, variables, etc.
  doInit();
  
  // Inicialización de tareas en cada core
  xTaskCreatePinnedToCore(
    codigoTaskUno,  // Código a ejecutar
    "Task1",        // Un identificador
    10000,          // Tamaño del stack
    NULL,           // Parámetros que recibe el código
    1,              // Prioridad
    &task1,         // Objeto para manejar la task
    1               // Número de procesador
  );            
}

void loop() {
  
  // Declaración e inicialización variables
  cantGiros = 0;
  giro = false;
  float distanciaDerechaActual = 0;
  float distanciaDerechaPrevia = 0;
  float distanciaAdelante = 0;
  int direccion = 0;
  int velocidad = 250;
  float tiempoDelay = 0;

  while(senialFumigar) {

    distanciaDerechaPrevia = distanciaDerechaActual;
    
    // Obtener distancias
    distanciaDerechaActual = calcularDistanciaPromedio(PIN_TRIG_DERECHA, PIN_ECHO_DERECHA);
    delay(0.1 * 1000);
    distanciaAdelante = calcularDistanciaPromedio(PIN_TRIG_ADELANTE, PIN_ECHO_ADELANTE);
      
    // Calcular dirección y tiempo
    direccion = determinarDireccion(distanciaAdelante, distanciaDerechaActual, distanciaDerechaPrevia);
    tiempoDelay = determinarTiempoDelay(direccion, distanciaAdelante, distanciaDerechaActual);

    mover(
      PIN_MOTOR_IZQUIERDA_IN1,
      PIN_MOTOR_IZQUIERDA_IN2,
      PIN_MOTOR_DERECHA_IN3,
      PIN_MOTOR_DERECHA_IN4,
      direccion,
      PWM_CHANNEL_0,
      PWM_CHANNEL_1,
      velocidad
    );

    // Para darle tiempo al robot a que realice la acción
    delay(tiempoDelay);

    // Parar el movimiento para que no moleste en la siguiente acción
    mover(
      PIN_MOTOR_IZQUIERDA_IN1,
      PIN_MOTOR_IZQUIERDA_IN2,
      PIN_MOTOR_DERECHA_IN3,
      PIN_MOTOR_DERECHA_IN4,
      PARAR,
      PWM_CHANNEL_0,
      PWM_CHANNEL_1,
      velocidad
    );

    // Si superamos la maxima cantidad de giros significa que termino la fumigacion
    if(cantGiros >= MAXIMA_CANTIDAD_GIROS) {

      senialFumigar = false;

      // Si nos encontramos conectados a firebase avisamos que finalizamos
      if(WiFi.status() == WL_CONNECTED && conectadoFB) {

        Firebase.RTDB.setBool(&fbWrite, pathHojaFumigar.c_str(), false);

      }
    }

    // Le damos tiempo a las tareas en background a ejecutarse
    delay(10);
  }
}

/* ------------------ SECCIÓN TAREAS ------------------ */

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
  
  doInitMdEConexiones()  ;

  /* LOOP */

  while(true) {
    
    maquinaEstadosConexiones();

    
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
  stConexiones = ST_REALIZANDO_CONEXION_WIFI;
  evtConexiones = EVT_DESCONEXION_WIFI;

  // Inicialización variables
  conectadoFB = false;
  senialFumigar = false;

}

void generarEventoMdEConexiones(void) {

  // Verificación de que estemos conectados al WIFI
  if(WiFi.status() != WL_CONNECTED) {

    // Realizamos la conexión a wifi
    conectarWifi();

    // Verificamos si se logra conectar
    if(WiFi.status() != WL_CONNECTED) {

      evtConexiones = EVT_DESCONEXION_WIFI;
    
    } else {

      evtConexiones = EVT_CONEXION_EXITOSA_WIFI;

    }

  // Verificamos conexión a Firebase
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

  // Evento por defecto
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

      // Informamos que estamos encendidos en Firebas
      escribirEncendidoRobotFB();

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

      // Informamos que estamos encendidos en Firebas
      escribirEncendidoRobotFB();

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

/******************************************************************* 
Nombre: escribirEncendidoRobotFB
Entradas: -
Salida: -
Proceso: informa en Firebase que el robot se encuentra encendido.
Fecha Creación: 24/09/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/


void escribirEncendidoRobotFB(void) {

  Firebase.RTDB.setBool(&fbWrite, pathHojaEncendido.c_str(), true);
  
}

/* ------------------ SECCIÓN MOVIMIENTO ------------------ */

/******************************************************************* 
Nombre: determinarDireccion
Entradas:
          + distanciaAdelante: float
          + distanciaDerechaActual: float
          + ditanciaDerechaPrevia: float
Salida: dirección
Proceso: evalua las distancia para encontrar la dirección a la cual el robot debe
dirigirse.
Fecha Creación: 24/09/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

int determinarDireccion(float distanciaAdelante, float distanciaDerechaActual, float ditanciaDerechaPrevia) {

  // Caso inmediatamente despues de realizar un giro, para que solo avance si hay espacio
  if(giro) {

    giro = false;
    if(distanciaAdelante > DISTANCIA_ADELANTE_MINIMA_CM + 10) {

      // ADELANTE_GIRO realiza 2 veces adelante
      return ADELANTE_GIRO;

    } else if(distanciaDerechaActual < DISTANCIA_DERECHA_MINIMA_CM) {

      return IZQUIERDA;

    } else {

      return ATRAS;

    }

  }
  
  // Caso que nos encontremos muy pegado a la pared derecha
  if(distanciaDerechaActual <  DISTANCIA_DERECHA_MINIMA_CM) {
    
    giro = true;
    return IZQUIERDA;

  }
  
  // Caso que estemos muy cerca de una pared en frente
  if(distanciaAdelante < DISTANCIA_ADELANTE_MINIMA_CM) {

    giro = true;
    cantGiros++;
    return IZQUIERDA;
    
    
  } 
  
  // Caso que nos encontremos muy lejos de una pared derecha
  if(distanciaDerechaActual > DISTANCIA_DERECHA_MAXIMA_CM) {
    
    giro = true;
    cantGiros--;
    return DERECHA;

  } 
  
  
  // Caso que nos encontremos con suficiente espacio como para avanzar
  if(distanciaAdelante > DISTANCIA_ADELANTE_MINIMA_CM) {

    // Verificamos si el robot se encuentra en dirección diagonal derecha o 
    // izquierda y corregimos
    if(ditanciaDerechaPrevia != 0 && distanciaDerechaActual - ditanciaDerechaPrevia > UMBRAL_CM) {

      return ADELANTE_DERECHA;
 
    } else if(ditanciaDerechaPrevia != 0 && distanciaDerechaActual - ditanciaDerechaPrevia < -UMBRAL_CM) {

      return ADELANTE_IZQUIERDA;

    }

    // Caso que este avanzando perfectamente paralelo a la pared
    return ADELANTE;

  } 

  // Caso por defecto
  return PARAR;
}