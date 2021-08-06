#include <main.h>

/* ------------------ CÓDIGO ------------------ */

void setup() {
  
  // Borrar
  Serial.begin(VEL_TRANSMISION);

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
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void doInit(){

  // Inicialización pin LED
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);

  // Inicialización pines sensores distancia de posición
  for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
    
    // Inicialización pines
    sensores[i].pinEcho = pinesEcho[i];
    sensores[i].pinTrig = pinesTrig[i];
      
    pinMode(sensores[i].pinEcho, INPUT);
    pinMode(sensores[i].pinTrig, OUTPUT);
  }

  // Inicialización de pines sensor distancia de liquido
  pinMode(PIN_ECHO_LIQUIDO, INPUT);
  pinMode(PIN_TRIG_LIQUIDO, OUTPUT);

  // Inicialización pin bomba
  pinMode(PIN_BOMBA, OUTPUT);

  // Inicialización variables
  conectadoFB = false;
  senialFumigar = false;
}

/* ------------------ SECCIÓN MdE SENSORES ------------------ */

void doInitMdESesonres() {
  
    // Inicializamos los estados
    for(int i = 0; i < CANT_SENSORES_DISTANCIA; i++) {
      sensores[i].estado = ST_OBJETO_DETECTADO;
      sensores[i].evento = EVT_OBJETO_FUERA_RANGO;
    }
}

int generarEventoMdESensorDistancia(int pinTrig, int pinEcho) {
  
  // Inicializar variable
  float promedio = -1;
  bool resultado = false;

  // Calcular la distancia promedio
  promedio = calcularDistanciaPromedio(pinTrig, pinEcho);
  
  // Verificar en que parte del rango esta
  resultado = estaDentroRango(
    UMBRAL_MINIMA_DISTANCIA_OBJETO_CM, 
    UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM, 
    promedio
    );

  // De acuerdo al valor devolvemos el evento correspondiente
  if(resultado) {
    return EVT_OBJETO_DENTRO_RANGO;
  } else {
    return EVT_OBJETO_FUERA_RANGO;
  }
}

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
      sensores[nro].pinEcho
      );
  }

void stObjetoNoDetectado(int nro){
  switch(sensores[nro].evento){

    case EVT_OBJETO_FUERA_RANGO:
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      sensores[nro].estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
}

void stObjetoDetectado(int nro){
  switch(sensores[nro].evento){

    case EVT_OBJETO_FUERA_RANGO:
      sensores[nro].estado = ST_OBJETO_NO_DETECTADO;
      break;

    case EVT_OBJETO_DENTRO_RANGO:
      sensores[nro].estado = ST_OBJETO_DETECTADO;
      break;
    
    default:
      break;
  }
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

/* ------------------ SECCIÓN MdE BATERÍA ------------------ */

void doInitMdEBateria(void) {
  
  // Inicialización estados y eventos
  stBateria = ST_CALCULANDO_NIVEL_BATERIA;
  evtBateria = EVT_COMUNICAR_CARGA;

  // Inicialización variables
  cargaBateria = 0;
}

void generarEventoMdEBateria(void) {
  
  if(cargaBateria != 0) {
    evtBateria = EVT_COMUNICAR_CARGA;
  } else {
    evtBateria = EVT_DETECTAR_CARGA;
  }
}

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
  cargaBateria = calcularNivelBateriaPromedio(
    PIN_VIN_BATERIA, 
    VOLTAJE_MIN_ESP, 
    VOLTAJE_MAX_BATERIA,
    CONSTANTE_CORRECCION_VOLTAJE_BATERIA);

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

/* ------------------ SECCIÓN MdE CORE UNO ------------------ */

void doInitMdECoreUno(void) {
  
  // Inicializaicón estado y evento
  stCoreUno = ST_INACTIVO;
  evtCoreUno = EVT_CONTINUAR;

  // Inicialización del a variable a usar
  lastTime = millis();
}

void generarEventoMdECoreUno(void) {

  // Verificación de que haya ocurrido timeout en el timer
  if(millis() - lastTime >=  TIEMPO_VERIFICACION_BATERIA_MS) {
    
    // Volvemos a tomar el tiempo
    lastTime = millis();

    // Asignamos el evento
    evtCoreUno = EVT_FIN_TIMER;
  } else {
    evtCoreUno = EVT_CONTINUAR;
  }

}

void maquinaEstadosCoreUno() {
  switch(stCoreUno) {

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

  generarEventoMdECoreUno();
}

void stInactivo() {
  switch(evtCoreUno) {

    case EVT_CONTINUAR:
      
      doInitMdEConexiones();
      maquinaEstadosConexiones();

      stCoreUno = ST_REALIZANDO_CONEXIONES;
      break;

    default:
      break;
  }
}

void stRealizandoConexiones() {
  switch(evtCoreUno) {

    case EVT_FIN_TIMER:

      // Informamos que el micro sigue prendido
      Firebase.RTDB.setBool(&fbWrite, pathHojaEncendido.c_str(), true);

      // Realizamos la acción para cambiar de MdE
      doInitMdEBateria();
      maquinaEstadosBateria();

      stCoreUno = ST_DETECTANDO_CARGA_BATERIA;
      break;

    case EVT_CONTINUAR:

      maquinaEstadosConexiones();

      stCoreUno = ST_REALIZANDO_CONEXIONES;
      break;

    default:
      break;
  }
}

void stDetectandoCargaBateria() {
  switch(evtCoreUno) {

    case EVT_FIN_TIMER:

      // Informamos que el micro sigue prendido
      Firebase.RTDB.setBool(&fbWrite, pathHojaEncendido.c_str(), true);

      // Realizamos la acción para cambiar de MdE
      maquinaEstadosConexiones();

      stCoreUno = ST_REALIZANDO_CONEXIONES;
      break;

    case EVT_CONTINUAR:
      maquinaEstadosBateria();

      stCoreUno = ST_DETECTANDO_CARGA_BATERIA;
      break;

    default:
      break;
  }
}

/* ------------------ SECCIÓN MdE CORE CERO ------------------ */

void doInitMdECoreCero(void) {

  // Inicializar estado y evento
  stCoreCero = ST_VERIFICANDO_SENSORES_DISTANCIA;
  evtCoreCero = EVT_FIN_LIBERAR_QUIMICO;

  // Leer de Firebase la cantidad de químico a depositar
  int cantidadQuimicoLiberar = 0;
  
  Firebase.RTDB.getInt(
    &fbWrite, 
    pathHojaCantidadQuimicoPorArea.c_str(),
    &cantidadQuimicoLiberar);

  // Setear el tiempo que libera el quimico
  switch(cantidadQuimicoLiberar) {

    case LIBERAR_CANTIDAD_BAJA_QUIMICO:
    tiempoLiberarQuimicoMs = TIEMPO_LIBERAR_QUIMICO_BAJA_MS;
    break;

    case LIBERAR_CANTIDAD_MEDIA_QUIMICO:
    tiempoLiberarQuimicoMs = TIEMPO_LIBERAR_QUIMICO_MEDIA_MS;
    break;

    case LIBERAR_CANTIDAD_ALTA_QUIMICO:
    tiempoLiberarQuimicoMs = TIEMPO_LIBERAR_QUIMICO_ALTA_MS;
    break;

    default:
    tiempoLiberarQuimicoMs = TIEMPO_LIBERAR_QUIMICO_MEDIA_MS;
    break;
  }
}

void generarEventoMdECoreCero(void) {

  // Inicializar el evento por defecto
  evtCoreCero = EVT_FIN_LIBERAR_QUIMICO;

  // Inicializar variables auxiliares
  int evtAux = -1;
  float distanciaQuimico = -1;
  float porcentajeQuimico = -1;
  float nivelQuimico = -1;

  // Hacer una pasada de la MdE del pin 0
  maquinaEstadosSensoresDistancia(0);

  // Verificar que se detecte un objeto
  evtAux = sensores[0].evento;

  if(evtAux == EVT_OBJETO_DENTRO_RANGO) {
    evtCoreCero = EVT_LIBERAR_QUIMICO;
  } else if(evtAux == EVT_OBJETO_FUERA_RANGO) {
    evtCoreCero = EVT_FIN_LIBERAR_QUIMICO;
  }

  // Verificar que se tenga suficiente quiímico
  distanciaQuimico = calcularDistanciaPromedio(
    PIN_TRIG_LIQUIDO, 
    PIN_ECHO_LIQUIDO
    );

  nivelQuimico = 1 - ((distanciaQuimico - (DISTANCIA_SENSOR_CM-DISTANCIA_MAX_CM)) / DISTANCIA_MAX_CM);

  if(nivelQuimico < LIMITE_INFERIOR_CONTENIDO) {
    evtCoreCero = EVT_NIVEL_BAJO_QUIMICO;
  }

  porcentajeQuimico = nivelQuimico * 100;

  // Escribir en firebase el nivel de quimico
  Firebase.RTDB.setFloat(&fbWrite, pathHojaQuimico.c_str(), porcentajeQuimico);
}

void maquinaEstadosCoreCero() {

  // Mover entre estados
  switch(stCoreCero) {

    case ST_VERIFICANDO_SENSORES_DISTANCIA:
      stVerificandoSensoresDistancia();
      break;

    case ST_LIBERAR_QUIMICO:
      stLiberarQuimico();
      break;

    case ST_SIN_QUIMICO:
      stSinQuimico();
      break;

    default:
      break;
  }

  // Generar un nuevo evento
  generarEventoMdECoreCero();
}

void stVerificandoSensoresDistancia(void) {

  switch(evtCoreCero) {

    case EVT_FIN_LIBERAR_QUIMICO:
      stCoreCero = ST_VERIFICANDO_SENSORES_DISTANCIA;
      break;


    case EVT_NIVEL_BAJO_QUIMICO:
      stCoreCero = ST_SIN_QUIMICO;
      finalizarFumigacionSinQuimico();
      break;

    case EVT_LIBERAR_QUIMICO:
      liberarQuimico(PIN_BOMBA, tiempoLiberarQuimicoMs);
      stCoreCero = ST_LIBERAR_QUIMICO;
      break;

    default:
      break;
  }

}
void stLiberarQuimico(void) {

  switch(evtCoreCero) {

    case EVT_FIN_LIBERAR_QUIMICO:
      stCoreCero = ST_VERIFICANDO_SENSORES_DISTANCIA;
      break;
    
    case EVT_LIBERAR_QUIMICO:
      liberarQuimico(PIN_BOMBA, tiempoLiberarQuimicoMs);
      stCoreCero = ST_LIBERAR_QUIMICO;
      break;
    
    case EVT_NIVEL_BAJO_QUIMICO:
      stCoreCero = ST_SIN_QUIMICO;
      finalizarFumigacionSinQuimico();
      break;

    default:
      break;
  }

}
void stSinQuimico(void) {

  switch(evtCoreCero) {
    
    default:
      break;
  }
}

/******************************************************************* 
Nombre: finalizarFumigacionSinQuimico
Entradas: -
Salida: -
Proceso: comunica en firebase que finaliza la fumigación e informa error por
falta de quimico
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void finalizarFumigacionSinQuimico(void) {
  // Comunicar a firebase el error
  Firebase.RTDB.setFloat(&fbWrite, pathHojaQuimico.c_str(), ERROR_NIVEL_QUIMICO);

  // Apagar la fumigacion en firebase
  Firebase.RTDB.setBool(&fbWrite, pathHojaFumigar.c_str(), false);
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

  if(senialFumigar) {
    doInitMdECoreCero();
  }
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