// Datos
#define FIREBASE_URL "fumigabot-default-rtdb.firebaseio.com"
#define FIREBASE_SECRETO "mWGc6K24cfGsvWsJ73HkFMPrGq852SpK2g3yqMP2"

// Paths
const char* PATH_FUMIGAR = "/robots/0/fumigando";
const char* PATH_BATERIA = "/robots/0/bateria";
const char* PATH_ENCENDIDO = "/robots/0/encendido";
const char* PATH_QUIMICO = "/robots/0/nivelQuimico";
const char* PATH_CANTIDAD_QUIMICO_POR_AREA = "/robots/0/fumigacionActual/cantidadQuimicoPorArea";
const char* PATH_CONTADOR = "/robots/0/contador";
const char* PATH_RAZON_FINALIZACION = "/robots/0/razonFinalizacion";
const char* PATH_DETENCION_AUTOMATICA = "/robots/0/detencionAutomatica";

// Timeouts
#define FIREBASE_FUMIGAR_TIMEOUT_MS 500
#define FIREBASE_ESTADO_ROBOT_TIMEOUT_MS 500 

// Codificacion razon de finalizacion
#define RAZON_FINALIZACION_OK "OK"
#define RAZON_FINALIZACION_FALTA_QUIMICO "FDQ"
#define RAZON_FINALIZACION_FALTA_BATERIA "FDB"

#define NRO_RAZON_FINALIZACION_OK 0
#define NRO_RAZON_FINALIZACION_FALTA_QUIMICO 1
#define NRO_RAZON_FINALIZACION_FALTA_BATERIA 2