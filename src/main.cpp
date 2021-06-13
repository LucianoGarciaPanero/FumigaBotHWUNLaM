#include <Arduino.h>

// CONSTANTES
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
// VARIABLES GLOBALES
int ledEstado = LOW;

//FUNCIONES
long calcularDistancia();

void setup() {
  // Inicializar serial
  Serial.begin(VEL_TRANSMISION);
  
  // Inicializar pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Establecer el pin en low
  ledEstado = LOW;
}

void loop() {
  // Cambiar el estado del led
  ledEstado = !ledEstado;

  // Actualizar el pin
  digitalWrite(PIN_LED, ledEstado);

  // Obtenemos la distancia del objeto
  long distancia = calcularDistancia();
 Serial.println("La distancia es");
  Serial.println(distancia);

  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}

long calcularDistancia() {
  
  // Inicializando el pin del trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(TIEMPO_ASEGURAR_NIVEL_BAJO_US);
  
  // Comenzar a transmitir
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(TIEMPO_PULSO_US);
  digitalWrite(TRIG_PIN, LOW);

  // Lectura del tiempo que estuvo en high
  long duracion = pulseIn(ECHO_PIN, HIGH);
  Serial.println("La duracion es");
  Serial.println(duracion);

  // Calculamos la distancia del objeto
  return (duracion / VALOR_CORRECCION_ONDA) * VELOCIDAD_SONIDO_CM;
}