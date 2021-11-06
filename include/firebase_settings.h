// Datos
#define FIREBASE_URL "fumigabot-default-rtdb.firebaseio.com"
#define FIREBASE_SECRETO "mWGc6K24cfGsvWsJ73HkFMPrGq852SpK2g3yqMP2"

// Paths
const char* PATH_FUMIGAR = "/robots/0/fumigando";
const char* PATH_BATERIA = "/robots/0/bateria";
const char* PATH_ENCENDIDO = "/robots/0/encendido";
const char* PATH_QUIMICO = "/robots/0/nivelQuimico";
const char* PATH_CANTIDAD_QUIMICO_POR_AREA = "/robots/0/cantidadQuimicoPorArea";
const char* PATH_CONTADOR = "/robots/0/contador";

// Timeouts
#define FIREBASE_FUMIGAR_TIMEOUT_MS 500
#define FIREBASE_ESTADO_ROBOT_TIMEOUT_MS 500 