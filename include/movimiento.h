// Configurar el PWM
#define FREQ 30000
#define RESOLUTION 8
#define PWM_CHANNEL_MOTOR 14

// Valores de dominio
#define MAXIMA_CANTIDAD_GIROS 6

// Tiempos delays
#define TIEMPO_DELAY_PARAR_MS 250
#define TIEMPO_DELAY_RETROCEDER_MS 100 // Lo realiza en IZQUIERDA o DERECHA
#define TIEMPO_DELAY_GIRO_CORTO_MS 30 // Lo realiza en ADELANTE
#define TIEMPO_DELAY_GIRO_MS 750 // Lo realiza en IZQUIERDA o DERECHA
#define TIEMPO_DELAY_ADELANTE_MS 200

// Velocidades
#define VELOCIDAD_ROBOT 175

// Codificacion direccion
#define PARAR 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4