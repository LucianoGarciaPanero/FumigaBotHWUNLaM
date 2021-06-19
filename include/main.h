/* ------------------ BIBLIOTECAS ------------------ */

#include <Arduino.h>
#include <config.h>
#include <sensores.h>

/* ------------------ ESTADOS ------------------ */

#define ST_INACTIVO             0
#define ST_ESPERANDO_DETECCION  1
#define ST_OBJETO_DETECTADO     2   
                                                                                                                            
/* ------------------ EVENTOS ------------------ */

#define EVT_CONTINUE            0
#define EVT_OBJETO_NO_DETECTADO 1
#define EVT_OBJETO_DETECTADO    2

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void doInit(void);
int generarEvento(int);
void maquinaEstados(int, int);
void stInactivo(int);
void stEsperandoDeteccion(int);
void stObjetoDetectado(int);