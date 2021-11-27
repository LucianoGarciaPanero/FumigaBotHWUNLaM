// Configurar el PWM
#define FREQ 3000
#define RESOLUTION 8
#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1

// Umbral
#define UMBRAL_CM 0.5

// Valores de dominio
#define MAXIMA_CANTIDAD_GIROS 4

#define DISTANCIA_DERECHA_MAXIMA_CM 70
#define DISTANCIA_DERECHA_MINIMA_CM 10
#define DISTANCIA_ADELANTE_MINIMA_CM 30

#define TIEMPO_DELAY_GIRO_MUY_CORTO_MS 0.1 * 1000
#define TIEMPO_DELAY_GIRO_CORTO_MS 0.2 * 1000
#define TIEMPO_DELAY_GIRO_LARGO_DERECHA_MS 0.3 * 1000
#define TIEMPO_DELAY_GIRO_LARGO_IZQUIERDA_MS 0.6 * 1000
#define TIEMPO_DELAY_ADELANTE_MS 0.6 * 1000
#define TIEMPO_DELAY_ADELANTE_GIRO_MS 1 * 1000
#define TIEMPO_DELAY_RETROCEDER_MS 0.1 * 1000
#define TIEMPO_DELAY_PARAR_MS 0.5 * 1000

// Velocidades
#define VELOCIDAD_ROBOT 200

// Codificacion direccion
#define PARAR 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4
#define ADELANTE_GIRO 5
#define ADELANTE_DERECHA 6
#define ADELANTE_IZQUIERDA 7