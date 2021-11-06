#include <utilitarias.h>

/* -------- SENSOR ULTRASONIDO --------  */
#define TIEMPO_ASEGURAR_NIVEL_BAJO_US 2
#define TIEMPO_PULSO_US 10
#define VALOR_CORRECCION_ONDA 2
#define VELOCIDAD_SONIDO_CM 0.0344
#define ERROR_NIVEL_QUIMICO -1

float calcularDistanciaPromedio(int, int);

/* -------- BATERÍA --------  */
#define CONSTANTE_CORRECCION_VOLTAJE_BATERIA 2 * 3.3 / 4095 // 1.11814
#define VOLTAJE_MIN_ESP 5
#define ERROR_BATERIA -1
#define DELTA_CARGA_BATERIA 2

float calcularNivelBateriaPromedio(int, float, float, float);

/* -------- MOVIMIENTO --------  */
#define UMBRAL_CM 0.5

#define MAXIMA_CANTIDAD_GIROS 4

#define VELOCIDAD_ALTA 240
#define VELOCIDAD_MEDIA 255
#define VELOCIDAD_BAJA 220

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

#define PARAR 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4
#define ADELANTE_GIRO 5
#define ADELANTE_DERECHA 6
#define ADELANTE_IZQUIERDA 7

float determinarTiempoDelay(int, float, float);
void mover(int, int, int, int, int, int, int, int);

/* -------- QUIMICO --------  */

#define DISTANCIA_SENSOR_DESDE_ARRIBA_CM 1
#define ALTURA_RECIPIENTE_LIQUIDO_CM 10
#define LIMITE_INFERIOR_CONTENIDO 0.2
#define DISTANCIA_MAX_CM 7

float calcularNivelQuimicoPromedio(int, int);
void liberarQuimico(int, float);
