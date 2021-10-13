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

// Inicialización estados, variables, etc
void doInit(void);

// Conexión WiFI
void conectarWifi(void);

// Conexión Firebase
bool conectarFB(void);
void streamCallback(FirebaseStream);
void streamTimeoutCallback(bool);

// Actualizacion valores en Firebase
void escribirEstadoRobotFB(void);

// Ejecutar tareas en paralelo
void codigoTaskCero(void*);

// Movimiento
int determinarDireccion(float, float, float);

/* ------------------ VARIABLES GLOBALES ------------------ */

// Banderas
bool conectadoFB;
bool senialFumigar;


// Movimiento
int cantGiros;
bool giro;

// Variables para Firebase
FirebaseData fbConection;
FirebaseData fbWrite;
FirebaseConfig config;
FirebaseAuth auth;

const String pathHojaFumigar = String("/robots/0/fumigando");
const String pathHojaBateria = String("/robots/0/bateria");
const String pathHojaEncendido = String("/robots/0/encendido");
const String pathHojaQuimico = String("/robots/0/nivelQuimico");
const String pathHojaCantidadQuimicoPorArea = String("/robots/0/cantidadQuimicoPorArea");

// Para ejecutar en paralelo
TaskHandle_t task0;