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
#define EVT_DESCONEXION_WIFI            1050
#define EVT_CONEXION_RECHAZADA_FB       1075
#define EVT_CONEXION_EXITOSA_FB         1100

// Funciones
void generarEventoMdEConexiones(void);
void maquinaEstadosConexiones(void);
void stRealizandoConexionWiFi(void);
void stRealizandoConexionFB(void);
void stConectadoFB(void);

// Variables globales
int stConexiones;
int evtConexiones;

/* ------------------ DECLARACIÓN FUNCIONES GENERALES ------------------ */

// WiFI
void inicializarWifi(void);
void conectarWifi(void);

// Firebase
void conectarFirebase(void);
void streamCallback(FirebaseStream);
void streamTimeoutCallback(bool);
void escribirEstadoRobotEnFirebase(void);

// Ejecutar tareas en paralelo
void codigoTaskCero(void*);
void loopCero(void);
void setupCero(void);
void setupUno(void);

// Movimiento
int determinarDireccion(float, float, float);

// Auxiliar
void reiniciarVariablesTaskCero(void);
void reiniciarVariablesTaskUno(void);
bool conexionesCorrectas(void);