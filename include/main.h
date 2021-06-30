// Bibliotecas externas
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Bibliotecas propias
#include <sensores.h>
#include <utilitarias.h>
#include <config.h>

/* ------------------ ESTADOS ------------------ */

// Estados MdE general
#define ST_INACTIVO                     0
#define ST_REALIZANDO_CONEXION_WIFI     1
#define ST_REALIZANDO_CONEXION_FB       3
#define ST_CONECTADO_FB                 4
#define ST_DETECTANDO_OBJETO            5

// Estados MdE sensores distancia
#define ST_OBJETO_NO_DETECTADO          6
#define ST_OBJETO_DETECTADO             7

/* ------------------ EVENTOS ------------------ */

// Eventos MdE general
#define EVT_CONTINUAR                   0
#define EVT_CONEXION_EXITOSA_WIFI       100
#define EVT_DESCONEXION_WIFI            200
#define EVT_CONEXION_RECHAZADA_FB       300
#define EVT_CONEXION_EXITOSA_FB         400
#define EVT_DESCONEXION_FB              600
#define EVT_COMENZAR_DETECCION          700
#define EVT_DETENER_DETECCION           800

// Eventos MdE sensores distancia
#define EVT_OBJETO_FUERA_RANGO          900
#define EVT_OBJETO_DENTRO_RANGO         10000

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);

// Conexión WiFI
void conectarWifi(void);

// Conexión Firebase
bool conectarFB(void);
void streamCallback(FirebaseStream);
void streamTimeoutCallback(bool);

// MdE general
void generarEventoMdEGeneral(void);
void doInitMdEGeneral(void);
void maquinaEstadosGeneral(void);

void stInactivo(void);
void stRealizandoConexionWifi(void);
void stRealizandoConexionFB(void);
void stConectadoFB(void);

// MdE sensores
int generarEventoMdESensorDistancia(int, int);
void doInitMdESesonres(void);
void maquinaEstadosSensoresDistancia(int);

void stObjetoNoDetectado(int);
void stObjetoDetectado(int);

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

int glbEstado;
int glbEvento;


// Banderas
bool conectadoFB;
bool senialFumigar;

// Variables para Firebase
FirebaseData fbConection;
FirebaseConfig config;
FirebaseAuth auth;

const String pathHojaFumigar = "/robots/0/fumigando";

// Para ejecutar en paralelo
TaskHandle_t task0;
TaskHandle_t task1;