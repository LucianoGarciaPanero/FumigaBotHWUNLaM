/* -------- SENSOR ULTRASONIDO --------  */
// Valores dominio
#define TIEMPO_ASEGURAR_NIVEL_BAJO_US 2
#define TIEMPO_PULSO_US 10
#define VALOR_CORRECCION_ONDA 2
#define VELOCIDAD_SONIDO_CM 0.0344

/* -------- FUNCIONES --------  */
long obtenerDistancia(int, int);