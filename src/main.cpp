#include <Arduino.h>

/* ------------------ CONSTANTES ------------------ */

// Led
#define PIN_LED 4

// Sensor distancia
#define TRIG_PIN 13
#define ECHO_PIN 14
#define TIEMPO_ASEGURAR_NIVEL_BAJO_US 2
#define TIEMPO_PULSO_US 10
#define VALOR_CORRECCION_ONDA 2
#define VELOCIDAD_SONIDO_CM 0.0344

// Delay
#define TIEMPO_MILIS 1000

// Comunicacion con el serial
#define VEL_TRANSMISION 9600

// Dominio del problema
#define UMBRAL_MAXIMO_DETECCION_CM 20

/* ------------------ VARIABLES GLOBALES ------------------ */
int ledEstado = LOW;

/* ------------------ FUNCIONES ------------------ */
long calcularDistancia(int, int);

void setup() {
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);
  
  // Inicializar pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Establecer el estado del led en low
  ledEstado = LOW;
}

void loop() {
  // Obtenemos la distancia del objeto
  long distancia = calcularDistancia(TRIG_PIN, ECHO_PIN);

  // Verificación de la distancia del objeto
  if(distancia <= UMBRAL_MAXIMO_DETECCION_CM){
    ledEstado = HIGH;
  } else {
    ledEstado = LOW;
  }
  digitalWrite(PIN_LED, ledEstado);
  
  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

long calcularDistancia(int trig_pin, int echo_pin) {
  
  // Inicializando el pin del trigger
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(TIEMPO_ASEGURAR_NIVEL_BAJO_US);
  
  // Comenzar a transmitir
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(TIEMPO_PULSO_US);
  digitalWrite(trig_pin, LOW);

  // Lectura del tiempo que estuvo en high
  long duracion = pulseIn(echo_pin, HIGH);

  // Calculamos la distancia del objeto
  return (duracion / VALOR_CORRECCION_ONDA) * VELOCIDAD_SONIDO_CM;
}