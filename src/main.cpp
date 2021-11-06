#include <main.h>

/* ------------------ VARIABLES GLOBALES ------------------ */

// Banderas
bool escribirEstadoRobot;
bool escribirEncendidoRobot;
bool fumigar;

// Movimiento
int cantGiros;
bool giro;
float distanciaDerechaActual;
float distanciaDerechaPrevia;
float distanciaAdelante;
int direccion;
int velocidad;
float tiempoDelay;

// Variables para Firebase
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

// Para ejecutar en paralelo
TaskHandle_t task0;

// WiFi
int estadoLed;
unsigned long startTimeWifiTimeout;
unsigned long startTimeFirebaseFumigar;
unsigned long startTimeFirebaseEstadoRobot;
int contador;

// Quimico
float nivelQuimicoPrevio;

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  //Serial.begin(VEL_TRANSMISION);

  // Setup de cada proceso
  setupUno();
  setupCero();  
  
  // Inicialización de tareas en core cero
  xTaskCreatePinnedToCore(
    codigoTaskCero,           // Código a ejecutar
    NOMBRE_TASK_CERO,         // Un identificador
    TAMANIO_STACK_TASK_CERO,  // Tamaño del stack
    NULL,                     // Parámetros que recibe el código
    PRIORIDAD_TASK_CERO,      // Prioridad
    &task0,                   // Objeto para manejar la task
    PROCESADOR_CERO           // Número de procesador
  );  
  
}

/*
* Esta función siempre se ejecuta en el core 1
*/

void loop() {

  
  /* ESCRITURA/LECTURA EN FIREBASE */

  if(conexionesCorrectas() && escribirEstadoRobot && millis() - startTimeFirebaseEstadoRobot > FIREBASE_ESTADO_ROBOT_TIMEOUT_MS) {

    escribirEstadoRobotEnFirebase();
    escribirEstadoRobot = false;
    startTimeFirebaseEstadoRobot = millis();

  }

  if(conexionesCorrectas() && escribirEncendidoRobot) {

    Firebase.RTDB.setBool(&fbdo, PATH_ENCENDIDO, true);
    escribirEncendidoRobot = false;

  }

  if(conexionesCorrectas() && millis() - startTimeFirebaseFumigar > FIREBASE_FUMIGAR_TIMEOUT_MS) {

    
    if(Firebase.RTDB.getBool(&fbdo, PATH_FUMIGAR)) {

      fumigar = fbdo.boolData();

    }
    
    startTimeFirebaseFumigar = millis();

  }

  /* ALGORITMO MOVIMIENTO */
  
  if(fumigar) {

    distanciaDerechaPrevia = distanciaDerechaActual;
    
    // Obtener distancias
    distanciaDerechaActual = calcularDistanciaPromedio(PIN_TRIG_DERECHA, PIN_ECHO_DERECHA);
    Firebase.RTDB.setFloat(&fbdo, "/robots/0/distanciaD", distanciaDerechaActual);
    distanciaAdelante = calcularDistanciaPromedio(PIN_TRIG_ADELANTE, PIN_ECHO_ADELANTE);
    Firebase.RTDB.setFloat(&fbdo, "/robots/0/distanciaA", distanciaAdelante);

    // Calcular dirección y tiempo
    direccion = determinarDireccion(distanciaAdelante, distanciaDerechaActual, distanciaDerechaPrevia);
    tiempoDelay = determinarTiempoDelay(direccion, distanciaAdelante, distanciaDerechaActual);
    Firebase.RTDB.setInt(&fbdo, "/robots/0/direccion", direccion);

    mover(
      PIN_MOTOR_IZQUIERDA_IN1,
      PIN_MOTOR_IZQUIERDA_IN2,
      PIN_MOTOR_DERECHA_IN3,
      PIN_MOTOR_DERECHA_IN4,
      IZQUIERDA,
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

      fumigar = false;
      reiniciarVariablesTaskUno();

      // Si nos encontramos conectados a firebase avisamos que finalizamos
      if(conexionesCorrectas()) {

        Firebase.RTDB.setBool(&fbdo, PATH_FUMIGAR, fumigar);

      } 
    }
  } else { 

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

  }
}

/* ------------------ SECCIÓN TAREAS ------------------ */
/******************************************************************* 
Nombre: setupUno
Entradas: -
Salida: -
Proceso: inicializa las variables correspondiente con el código del proceso
del core uno.
Fecha Creación: 26/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void setupUno(void) {
  
  // Inicialización pines sensores distancia
  pinMode(PIN_TRIG_ADELANTE, OUTPUT);
  pinMode(PIN_ECHO_ADELANTE, INPUT);

  pinMode(PIN_TRIG_DERECHA, OUTPUT);
  pinMode(PIN_ECHO_DERECHA, INPUT);

  pinMode(PIN_TRIG_QUIMICO, OUTPUT);
  pinMode(PIN_ECHO_QUIMICO, INPUT);

  // Inicialización pines motores
  pinMode(PIN_MOTOR_IZQUIERDA_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IZQUIERDA_IN2, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_0, FREQ, RESOLUTION);
  ledcAttachPin(PIN_MOTOR_IZQUIERDA_EN, PWM_CHANNEL_0);
  
  pinMode(PIN_MOTOR_DERECHA_IN3, OUTPUT);
  pinMode(PIN_MOTOR_DERECHA_IN4, OUTPUT);
  
  ledcSetup(PWM_CHANNEL_1, FREQ, RESOLUTION);
  ledcAttachPin(PIN_MOTOR_DERECHA_EN, PWM_CHANNEL_1);

  // Inicialización pin WiFi
  pinMode(PIN_LED_WIFI, OUTPUT);

  reiniciarVariablesTaskUno();

}

/******************************************************************* 
Nombre: setupCero
Entradas: -
Salida: -
Proceso: inicializa las variables correspondiente con la máquina de estados de
conexión.
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void setupCero(void) {

  // Inicializamos estados generales
  stConexiones = ST_REALIZANDO_CONEXION_WIFI;
  evtConexiones = EVT_DESCONEXION_WIFI;

  // Inicialización variables
  reiniciarVariablesTaskCero();
  
  digitalWrite(PIN_LED_WIFI, estadoLed);
  inicializarWifi();

}

/******************************************************************* 
Nombre: loopCero
Entradas: -
Salida: -
Proceso: instrucciones a ejecutar en el core cero
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void loopCero(void) {

  maquinaEstadosConexiones();

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

void codigoTaskCero(void *param) {

  for(;;) {
    
    loopCero();  
    vTaskDelay(DELAY_TASK_END_MS / portTICK_PERIOD_MS);

  }
}

/* ------------------ SECCIÓN MdE CONEXIONES ------------------ */

void generarEventoMdEConexiones(void) {

  // Verificación de que estemos conectados al WIFI
  if(WiFi.status() != WL_CONNECTED) {

    evtConexiones = EVT_DESCONEXION_WIFI;

  // Verificamos conexión a Firebase
  } else if(!Firebase.ready()) {
   
    evtConexiones = EVT_CONEXION_RECHAZADA_FB;
  
  // Evento por defecto, se realizaron las conexiones correctamente
  } else {  
  
    evtConexiones = EVT_CONEXION_EXITOSA_FB;
    estadoLed = HIGH;
    digitalWrite(PIN_LED_WIFI, estadoLed);

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
      conectarWifi();
      stConexiones = ST_REALIZANDO_CONEXION_WIFI;
      break;

    case EVT_CONEXION_RECHAZADA_FB:
      conectarFirebase();
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;
    
    default:
      break;
  }
}

void stRealizandoConexionFB() {
  switch(evtConexiones){
    
    case EVT_CONEXION_RECHAZADA_FB:
      conectarFirebase();
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_CONEXION_EXITOSA_FB:
      if(!escribirEstadoRobot) {

        escribirEstadoRobot = true;

      }      
      stConexiones = ST_CONECTADO_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      conectarWifi();
      stConexiones = ST_REALIZANDO_CONEXION_WIFI;
      break;
    
    default:
      break;
  }
}

void stConectadoFB() {
  switch(evtConexiones) {
    
    case EVT_CONEXION_RECHAZADA_FB:
      conectarFirebase();
      stConexiones = ST_REALIZANDO_CONEXION_FB;
      break;

    case EVT_DESCONEXION_WIFI:
      ESP.restart();
      break;
    
    case EVT_CONEXION_EXITOSA_FB:
      if(!escribirEstadoRobot) {

        escribirEstadoRobot = true;

      }
      stConexiones = ST_CONECTADO_FB;
      break;
    
    default:
      break;
  }
}

/* ------------------ SECCIÓN AUXILIARES ------------------ */

/******************************************************************* 
Nombre: reiniciarVariablesTaskCero
Entradas: -
Salida: -
Proceso: asignar los valores iniciales a las variables de la task cero.
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void reiniciarVariablesTaskCero(void) {

  fumigar = false;
  escribirEstadoRobot = false;
  escribirEncendidoRobot = false;
  startTimeWifiTimeout = millis();
  startTimeFirebaseFumigar = millis();
  startTimeFirebaseEstadoRobot = millis();
  contador = 0;
  estadoLed = LOW;

}

/******************************************************************* 
Nombre: reiniciarVariablesTaskUno
Entradas: -
Salida: -
Proceso: asignar los valores iniciales a las variables de la task uno.
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void reiniciarVariablesTaskUno(void) {

  cantGiros = 0;
  giro = false;
  distanciaDerechaActual = 0;
  distanciaDerechaPrevia = 0;
  distanciaAdelante = 0;
  direccion = 0;
  velocidad = 255;
  tiempoDelay = 0;
  
  // Para mejorar la primera medición del quimico
  nivelQuimicoPrevio = calcularNivelQuimicoPromedio(PIN_TRIG_QUIMICO, PIN_ECHO_QUIMICO);

  if(nivelQuimicoPrevio < 10) {

    nivelQuimicoPrevio = 4;

  } else if(nivelQuimicoPrevio < 20) {

    nivelQuimicoPrevio = 17;

  } else if(nivelQuimicoPrevio < 50) {

    nivelQuimicoPrevio = 43;

  } else if(nivelQuimicoPrevio < 75) {

    nivelQuimicoPrevio = 68;

  } else {

    nivelQuimicoPrevio = 89;

  }
}

/******************************************************************* 
Nombre: conexionesCorrectas
Entradas: -
Salida: -
Proceso: validar que el ESP este conectado a internet y Firebase
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

bool conexionesCorrectas(void) {

  return WiFi.status() == WL_CONNECTED && Firebase.ready();

}

/* ------------------ SECCIÓN CONEXIÓN WIFI ------------------ */

/******************************************************************* 
Nombre: inicializarWifi
Entradas: -
Salida: -
Proceso: establece el modo correcto del ESP e inicia la conexió al wifi
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void inicializarWifi() {

  // Modo para que el ESP se conecte a una red
  WiFi.mode(WIFI_STA);

  // Desconectamos por si estuviera previamente conectado a otra red
  WiFi.disconnect();

  // Comenzamos conexión
  WiFi.begin(WIFI_RED, WIFI_CONTRASENIA);

}


/******************************************************************* 
Nombre: conectarWifi
Entradas: -
Salida: -
Proceso: intenta la conexió al wifi, si ocurre un timeout se reconecta a la red
nuevamente
Fecha Creación: 13/10/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void conectarWifi() {
  
  if(millis() - startTimeWifiTimeout > WIFI_TIMEOUT_MS) {

    reiniciarVariablesTaskCero();
    digitalWrite(PIN_LED_WIFI, estadoLed);
    inicializarWifi();

  } else {

    estadoLed = !estadoLed;
    digitalWrite(PIN_LED_WIFI, estadoLed);
    delay(WIFI_TIEMPO_ESPERA_MS);

  }
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

void conectarFirebase(void) {

  // Parpadeo de LED
  estadoLed = LOW;
  digitalWrite(PIN_LED_WIFI, estadoLed);
  
  // Asignamos los datos de la base de datos en tiempo real
  config.database_url = FIREBASE_URL;
  config.signer.tokens.legacy_token = FIREBASE_SECRETO;

  // Hacemos la conexión
  Firebase.begin(&config, &auth);

  escribirEncendidoRobot = true;

}

/* ------------------ SECCIÓN ACTUALIZAR VALORES FIREBASE ------------------ */

/******************************************************************* 
Nombre: escribirEstadoRobotFB
Entradas: -
Salida: -
Proceso: informa en Firebase del valore de batería, químico y que esta encendido 
el robot
Fecha Creación: 24/09/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void escribirEstadoRobotEnFirebase(void) {

  float nivelQuimicoActual = calcularNivelQuimicoPromedio(PIN_TRIG_QUIMICO, PIN_ECHO_QUIMICO);
  float nivelBateria = 100;
  /*
  // Obtener valores de sensores
  nivelBateria = calcularNivelBateriaPromedio();
  */  

  // Para evitar para tener variaciones intensas en el nivel de quimico
  if(nivelQuimicoActual <= nivelQuimicoPrevio) {

    if(nivelQuimicoPrevio - nivelQuimicoActual > 3) {

      nivelQuimicoActual = nivelQuimicoPrevio - 3;

    }       

    Firebase.RTDB.setFloat(&fbdo, PATH_QUIMICO, nivelQuimicoActual);
    nivelQuimicoPrevio = nivelQuimicoActual;

  }
  
  // Actualizar valor de bateria y nivel quimico
  Firebase.RTDB.setFloat(&fbdo, PATH_BATERIA, nivelBateria); 
  Firebase.RTDB.setInt(&fbdo, PATH_CONTADOR, ++contador);



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