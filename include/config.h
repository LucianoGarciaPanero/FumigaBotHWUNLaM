/* -------- PINES --------  */

// LED
#define PIN_LED 4

// Sensor Ultrasonido
#define PINES_TRIG {13}
#define PINES_ECHO {14}
#define CANT_SENSORES_DISTANCIA 1

// Batería
#define PIN_VIN_BATERIA 33

/* -------- OTROS --------  */

// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM 20
#define UMBRAL_MINIMA_DISTANCIA_OBJETO_CM 1

// WiFi
#define WIFI_RED "MICASA"
#define WIFI_CONTRASENIA "99agua_Azul"
#define WIFI_TIMEOUT_MS 20000

// FireBase
#define FIREBASE_URL "fumigabot-default-rtdb.firebaseio.com"
#define FIREBASE_SECRETO "mWGc6K24cfGsvWsJ73HkFMPrGq852SpK2g3yqMP2"

// Batería
#define VOLTAJE_MAX_BATERIA 6.6
#define TIEMPO_VERIFICACION_BATERIA_MS 10000