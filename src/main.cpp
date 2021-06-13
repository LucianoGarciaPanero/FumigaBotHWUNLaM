#include <Arduino.h>

#define PIN_LED 4
#define TIEMPO_MILIS 1000

int led_estado = LOW;

void setup() {
  // Poner el pin en modo salida
  pinMode(PIN_LED, OUTPUT);

  // Establecer el pin en low
  led_estado = LOW;
}

void loop() {
  // Cambiar el estado del led
  led_estado = !led_estado;

  // Actualizar el pin
  digitalWrite(PIN_LED, led_estado);

  // Esperamos un tiempo
  delay(TIEMPO_MILIS);
}