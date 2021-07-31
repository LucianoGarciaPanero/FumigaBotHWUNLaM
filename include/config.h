/* -------- PINES --------  */

// Sensor Ultrasonido de posición
#define PINES_TRIG {13}
#define PINES_ECHO {14}
#define CANT_SENSORES_DISTANCIA 1

// Sensor Ultrasonido de químico
#define PIN_TRIG_LIQUIDO 25
#define PIN_ECHO_LIQUIDO 26

// Batería
#define PIN_LED_VERDE 5
#define PIN_LED_AMARILLO 15
#define PIN_LED_ROJO 18
#define PIN_VIN_BATERIA 33

// Bomba Agua
#define PIN_BOMBA 32

/* -------- OTROS --------  */

// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM 150
#define UMBRAL_MINIMA_DISTANCIA_OBJETO_CM 15

// WiFi
#define WIFI_RED "MICASA"
#define WIFI_CONTRASENIA "99agua_Azul"
#define WIFI_TIMEOUT_MS 20000

// FireBase
#define FIREBASE_URL "fumigabot-default-rtdb.firebaseio.com"
#define FIREBASE_SECRETO "mWGc6K24cfGsvWsJ73HkFMPrGq852SpK2g3yqMP2"

// Batería
#define VOLTAJE_MAX_BATERIA 6
#define TIEMPO_VERIFICACION_BATERIA_MS 10 * 1000
#define MAX_VALOR_NIVEL_BAJO 15
#define MAX_VALOR_NIVEL_MEDIO 40

// Recipiente líquido
#define DISTANCIA_MAX_CM 7
#define DISTANCIA_SENSOR_CM 11
#define LIMITE_INFERIOR_CONTENIDO 0.2

// Bomba de agua
#define LIBERAR_CANTIDAD_BAJA_QUIMICO 1
#define LIBERAR_CANTIDAD_MEDIA_QUIMICO 2
#define LIBERAR_CANTIDAD_ALTA_QUIMICO 3
#define TIEMPO_LIBERAR_QUIMICO_BAJA_MS 0.5 * 1000
#define TIEMPO_LIBERAR_QUIMICO_MEDIA_MS 1 * 1000
#define TIEMPO_LIBERAR_QUIMICO_ALTA_MS 2 * 1000