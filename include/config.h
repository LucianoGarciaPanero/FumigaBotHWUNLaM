// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMA_DISTANCIA_OBJETO_CM 150
#define UMBRAL_MINIMA_DISTANCIA_OBJETO_CM 15

#define UMBRAL_MINIMO_NIVEL_BATERIA_PORCENTAJE 5
#define UMBRAL_MINIMO_NIVEL_QUIMICO_PORCENTAJE 5

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