/* ------------------ BIBLIOTECAS ------------------ */
// Bibliotecas externas
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Bibliotecas propias
#include <sensores.h>
#include <utilitarias.h>

// Archivos de configuración
#include <pines.h>
#include <bateria.h>
#include <bomba_agua.h>
#include <config.h>
#include <firebase_settings.h>
#include <wifi_settings.h>
#include <maquina_estados_conexiones.h>


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
int determinarDireccion(int, int, int);

// Auxiliar
void reiniciarVariablesTaskCero(void);
void reiniciarVariablesTaskUno(void);
bool conexionesCorrectas(void);
void finalizarFumigacion(int);

/* ------------------ VARIABLES GLOBALES ------------------ */

// Banderas
bool escribirEstadoRobot;
bool escribirEncendidoRobot;
bool fumigar;

// Movimiento
int cantGiros;
bool giro;
int distanciaDerechaActual;
int distanciaDerechaPrevia;
int distanciaAdelante;
int direccion;
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
int nivelQuimicoPrevio;

// Bateria
int nivelBateriaPrevio;