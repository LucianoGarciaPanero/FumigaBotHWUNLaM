#include <Arduino.h>
#include <WiFi.h>
#include <sensores.h>
#include <utilitarias.h>
#include <config.h>

/* ------------------ ESTADOS ------------------ */

// Estados MdE general
#define ST_INACTIVO                     0
#define ST_REALIZANDO_CONEXION_WIFI     1
#define ST_CONECTADO_WIFI               2
#define ST_REALIZANDO_CONEXION_FB       3
#define ST_CONECTADO_FB                 4
#define ST_DETECTANDO_OBJETO            5

// Estados MdE sensores distancia
#define ST_OBJETO_NO_DETECTADO          6
#define ST_OBJETO_DETECTADO             7

/* ------------------ EVENTOS ------------------ */

// Eventos MdE general
#define EVT_CONTINUAR                   0
#define EVT_ACABA_TIEMPO_WIFI           1
#define EVT_CONEXION_EXITOSA_WIFI       2
#define EVT_DESCONEXION_WIFI            3
#define EVT_ACABA_TIEMPO_FB             4
#define EVT_CONEXION_EXITOSA_FB         5
#define EVT_DESCONEXION_FB              6
#define EVT_COMENZAR_DETECCION          7

// Eventos MdE sensores distancia
#define EVT_OBJETO_FUERA_RANGO          8
#define EVT_OBJETO_DENTRO_RANGO         9

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);
void conectarWifi(void);

// MdE general
void generarEventoMdEGeneral(void);
void doInitMdEGeneral(void);
void maquinaEstadosGeneral(void);

void stInactivo(void);
void stRealizandoConexionWifi(void);
void stConectandoWifi(void);
void stRealizandoConexionFB(void);
void stConectandoFB(void);


// MdE sensores
int generarEventoMdESensorDistancia(int, int);
void doInitMdESesonres(void);
void maquinaEstadosSensoresDistancia(int);

void stObjetoNoDetectado(int);
void stObjetoDetectado(int);

/* ------------------ ESTRUCTURAS ------------------ */

typedef struct {
    int pinTrig;
    int pinEcho;
    int estado;
    int evento;
} stSensorDistancia;

/* ------------------ VARIABLES GLOBALES ------------------ */

stSensorDistancia sensores[CANT_SENSORES_DISTANCIA];
int pinesEcho[CANT_SENSORES_DISTANCIA] = PINES_ECHO;
int pinesTrig[CANT_SENSORES_DISTANCIA] = PINES_TRIG;

int glbEstado;
int glbEvento;

bool primeraConexion;