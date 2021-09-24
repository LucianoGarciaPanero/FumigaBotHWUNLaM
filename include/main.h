// Bibliotecas externas
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Bibliotecas propias
#include <sensores.h>
#include <utilitarias.h>
#include <config.h>

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