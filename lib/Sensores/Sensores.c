#include "./sensores.h"

long obtenerDistancia(int trig_pin, int echo_pin) {
  
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