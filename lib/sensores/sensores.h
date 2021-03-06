/* -------- BIBLIOTECAS --------  */

// Propias
#include <utilitarias.h>

// Archivos de configuración
#include "../../include/bomba_agua.h"
#include "../../include/movimiento.h"
#include "../../include/bateria.h"
#include "../../include/sensor_ultrasonido.h"
#include "../../include/pines.h"

/* -------- SENSOR ULTRASONIDO --------  */
float calcularDistanciaPromedio(int, int);

/* -------- BATERÍA --------  */
int calcularNivelBateriaPromedio(int);

/* -------- MOVIMIENTO --------  */
float determinarTiempoDelay(int, int);
void mover(int);

/* -------- QUIMICO --------  */
int calcularNivelQuimicoPromedio(int, int);
void liberarQuimico(int, long);
