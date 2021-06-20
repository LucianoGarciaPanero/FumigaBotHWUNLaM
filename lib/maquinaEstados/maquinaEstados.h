/* ------------------ ESTADOS ------------------ */

// Estados MdE general
#define ST_INACTIVO                     0
#define ST_REALIZANDO_CONEXIÓN_WIFI     1
#define ST_CONECTADO_WIFI               2
#define ST_DETECTANDO_OBJETO            3

// Estados MdE sensores distancia
#define ST_OBJETO_NO_DETECTADO          4
#define ST_OBJETO_DETECTADO             5   
                                                                                                                            
/* ------------------ EVENTOS ------------------ */

// Eventos MdE general
#define EVT_CONTINUAR                   0
#define EVT_ACABA_TIEMPO                1
#define EVT_CONEXION_EXITOSA            2
#define EVT_DESCONEXION                 3
#define EVT_COMENZAR_DETECCIÓN          4

// Eventos MdE sensores distancia
#define EVT_OBJETO_FUERA_RANGO          5
#define EVT_OBJETO_DENTRO_RANGO         6

/* ------------------ DECLARACIÓN FUNCIONES ------------------ */

void generarEvento(void);
void maquinaEstados(void);

// Estados MdE general
void stInactivo(void);
void stRealizandoConexionWifi(void);
void stConectandoWifi(void);
void stDetectandoObjeto(void);

// Estados MdE sensores distancia
void stObjetoNoDetectado(void);
void stObjetoDetectado(void);
