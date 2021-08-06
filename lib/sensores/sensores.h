#include <utilitarias.h>

/* -------- SENSOR ULTRASONIDO --------  */

// Valores dominio 
#define TIEMPO_ASEGURAR_NIVEL_BAJO_US 2
#define TIEMPO_PULSO_US 10
#define VALOR_CORRECCION_ONDA 2
#define VELOCIDAD_SONIDO_CM 0.0344
#define ERROR_NIVEL_QUIMICO -1

/* -------- BATERÍA --------  */

// Valores dominio
#define CONSTANTE_CORRECCION_VOLTAJE_BATERIA 2 * 3.3 / 4095 // 1.11814
#define VOLTAJE_MIN_ESP 5
#define ERROR_BATERIA -1
#define DELTA_CARGA_BATERIA 2

/* -------- FUNCIONES --------  */
long calcularDistancia(int, int);
float calcularDistanciaPromedio(int, int);
float calcularNivelBateriaPromedio(int, float, float, float);
void liberarQuimico(int, float);
