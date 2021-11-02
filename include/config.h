/* -------- PINES --------  */

// Sensores distancia
#define PIN_TRIG_ADELANTE 12
#define PIN_ECHO_ADELANTE 13

#define PIN_TRIG_DERECHA 23
#define PIN_ECHO_DERECHA 22

// Motores
#define PIN_MOTOR_IZQUIERDA_EN 32
#define PIN_MOTOR_IZQUIERDA_IN1 33
#define PIN_MOTOR_IZQUIERDA_IN2 25
#define PIN_MOTOR_DERECHA_IN3 26
#define PIN_MOTOR_DERECHA_IN4 27
#define PIN_MOTOR_DERECHA_EN 14

// LED
#define PIN_LED_WIFI 2

/* -------- OTROS --------  */

// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM 150
#define UMBRAL_MINIMA_DISTANCIA_OBJETO_CM 15

// WiFi
#define WIFI_RED "MICASA"
#define WIFI_CONTRASENIA "99agua_Azul"
#define WIFI_TIMEOUT_MS 5000
#define WIFI_TIEMPO_ESPERA_MS 10

// FireBase
#define FIREBASE_URL "fumigabot-default-rtdb.firebaseio.com"
#define FIREBASE_SECRETO "mWGc6K24cfGsvWsJ73HkFMPrGq852SpK2g3yqMP2"
const char* PATH_FUMIGAR = "/robots/0/fumigando";
const char* PATH_BATERIA = "/robots/0/bateria";
const char* PATH_ENCENDIDO = "/robots/0/encendido";
const char* PATH_QUIMICO = "/robots/0/nivelQuimico";
const char* PATH_CANTIDAD_QUIMICO_POR_AREA = "/robots/0/cantidadQuimicoPorArea";
const char* PATH_CONTADOR = "/robots/0/contador";
#define FIREBASE_FUMIGAR_TIMEOUT_MS 500
#define FIREBASE_ESTADO_ROBOT_TIMEOUT_MS 500 

// Batería
#define VOLTAJE_MAX_BATERIA 6
#define TIEMPO_VERIFICACION_BATERIA_MS 10000
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
#define TIEMPO_LIBERAR_QUIMICO_BAJA_MS 500
#define TIEMPO_LIBERAR_QUIMICO_MEDIA_MS 1000
#define TIEMPO_LIBERAR_QUIMICO_ALTA_MS 2000

// PWM
#define FREQ 3000
#define RESOLUTION 8
#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1

// Procesamient en Paralelo
#define NOMBRE_TASK_CERO "Task0"
#define TAMANIO_STACK_TASK_CERO 10000
#define PRIORIDAD_TASK_CERO 0
#define PROCESADOR_CERO 0
#define DELAY_TASK_END_MS 40