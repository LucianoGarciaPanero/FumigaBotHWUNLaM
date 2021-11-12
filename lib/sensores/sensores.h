/* -------- BIBLIOTECAS --------  */

// Propias
#include <utilitarias.h>

// Archivos de configuración
#include "../../include/bomba_agua.h"
#include "../../include/movimiento.h"
#include "../../include/bateria.h"
#include "../../include/sensor_ultrasonido.h"

/* -------- SENSOR ULTRASONIDO --------  */
float calcularDistanciaPromedio(int, int);

/* -------- BATERÍA --------  */
float calcularNivelBateriaPromedio(int, float, float, float);

/* -------- MOVIMIENTO --------  */
float determinarTiempoDelay(int, float, float);
void mover(int, int, int, int, int, int, int, int);

/* -------- QUIMICO --------  */
float calcularNivelQuimicoPromedio(int, int);
void liberarQuimico(int, long);
