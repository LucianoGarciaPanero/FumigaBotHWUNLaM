// Bibliotecas externas
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Bibliotecas propias
#include <sensores.h>
#include <utilitarias.h>
#include <config.h>

/* ------------------ MdE SENSORES ------------------ */
// Estados
#define ST_OBJETO_NO_DETECTADO          100
#define ST_OBJETO_DETECTADO             125

// Eventos
#define EVT_OBJETO_FUERA_RANGO          1000
#define EVT_OBJETO_DENTRO_RANGO         1025

// Funciones
void doInitMdESesonres(void);
int generarEventoMdESensorDistancia(int, int);
void maquinaEstadosSensoresDistancia(int);
void stObjetoNoDetectado(int);
void stObjetoDetectado(int);

/* ------------------ MdE CONEXIONES ------------------ */
// Estados
#define ST_REALIZANDO_CONEXION_WIFI     100
#define ST_REALIZANDO_CONEXION_FB       125
#define ST_CONECTADO_FB                 150

// Eventos
#define EVT_CONTINUAR                   1000
#define EVT_CONEXION_EXITOSA_WIFI       1025
#define EVT_DESCONEXION_WIFI            1050
#define EVT_CONEXION_RECHAZADA_FB       1075
#define EVT_CONEXION_EXITOSA_FB         1100
#define EVT_DESCONEXION_FB              1125

// Funciones
void doInitMdEConexiones(void);
void generarEventoMdEConexiones(void);
void maquinaEstadosConexiones(void);
void stRealizandoConexionWiFi(void);
void stRealizandoConexionFB(void);
void stConectadoFB(void);

// Variables globales
int stConexiones;
int evtConexiones;

/* ------------------ MdE BATERÍA ------------------ */
// Estados
#define ST_CALCULANDO_NIVEL_BATERIA     100
#define ST_COMUNICANDO_CARGA_FB         125

// Eventos
#define EVT_COMUNICAR_CARGA             1000
#define EVT_DETECTAR_CARGA              1025

// Funciones
void doInitMdEBateria(void);
void generarEventoMdEBateria(void);
void maquinaEstadosBateria(void);
void stCalculandoNivelBateria(void);

// Variables globales
int stBateria;
int evtBateria;
float cargaBateria;

/* ------------------ MdE CORE 1 ------------------ */
// Estados
#define ST_INACTIVO                     100
#define ST_REALIZANDO_CONEXIONES        125
#define ST_DETECTANDO_CARGA_BATERIA     150

// Eventos
#define EVT_CONTINUAR                   1000
#define EVT_FIN_TIMER                   1025

// MdE general
void generarEventoMdECoreUno(void);
void doInitMdECoreUno(void);
void maquinaEstadosCoreUno(void);
void stInactivo(void);
void stRealizandoConexiones(void);
void stDetectandoCargaBateria(void);

// Variables globales
int stCoreUno;
int evtCoreUno;
unsigned long lastTime;

/* ------------------ MdE CORE 0 ------------------ */
// Estados
#define ST_VERIFICANDO_SENSORES_DISTANCIA   100
#define ST_LIBERAR_QUIMICO                  125
#define ST_SIN_QUIMICO                      150

// Eventos
#define EVT_LIBERAR_QUIMICO                   1000
#define EVT_NIVEL_BAJO_QUIMICO                1025
#define EVT_FIN_LIBERAR_QUIMICO               1050
#define EVT_FINALIZAR_FUMIGACION              1075

// MdE Core Cero
void generarEventoMdECoreCero(void);
void doInitMdECoreCero(void);
void maquinaEstadosCoreCero(void);
void stVerificandoSensoresDistancia(void);
void stLiberarQuimico(void);
void stSinQuimico(void);

// Variables globales
int stCoreCero;
int evtCoreCero;
int tiempoLiberarQuimicoMs;

// Función auxiliar
void finalizarFumigacionSinQuimico(void);

/* ------------------ DECLARACIÓN FUNCIONES GENERALES ------------------ */

void doInit(void);

// Conexión WiFI
void conectarWifi(void);

// Conexión Firebase
bool conectarFB(void);
void streamCallback(FirebaseStream);
void streamTimeoutCallback(bool);

// Ejecutar tareas en paralelo
void codigoTaskCero(void*);
void codigoTaskUno(void*);

/* ------------------ ESTRUCTURAS ------------------ */

typedef struct {
    int pinTrig;
    int pinEcho;
    int estado;
    int evento;
} stSensorDistancia;

/* ------------------ VARIABLES GLOBALES ------------------ */

stSensorDistancia sensores[CANT_SENSORES_DISTANCIA];
int pinesEcho[] = PINES_ECHO;
int pinesTrig[] = PINES_TRIG;

// Banderas
bool conectadoFB;
bool senialFumigar;

// Variables para Firebase
FirebaseData fbConection;
FirebaseData fbWrite;
FirebaseConfig config;
FirebaseAuth auth;

const String pathHojaFumigar = "/robots/0/fumigando";
const String pathHojaBateria = "/robots/0/bateria";
const String pathHojaEncendido = "/robots/0/encendido";
const String pathHojaQuimico = "/robots/0/nivelQuimico";
const String pathHojaCantidadQuimicoPorArea = "/robots/0/cantidadQuimicoPorArea";

// Para ejecutar en paralelo
TaskHandle_t task0;
TaskHandle_t task1;