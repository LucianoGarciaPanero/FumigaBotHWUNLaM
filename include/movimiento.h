// Configurar el PWM
#define FREQ 30000
#define RESOLUTION 8
#define PWM_CHANNEL_MOTOR 14

// Valores de dominio
#define MAXIMA_CANTIDAD_GIROS 4

// Tiempos delays
#define TIEMPO_DELAY_PARAR_MS 500
#define TIEMPO_DELAY_RETROCEDER_MS 100
#define TIEMPO_DELAY_GIRO_CORTO_MS 50 // Lo realiza en ADELANTE_IZQUIERDA
#define TIEMPO_DELAY_GIRO_MS 900 // Lo realiza en IZQUIERDA o DERECHA
#define TIEMPO_DELAY_ADELANTE_MS 250

// Velocidades
#define VELOCIDAD_ROBOT 175

// Codificacion direccion
#define PARAR 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4
#define ADELANTE_IZQUIERDA 7