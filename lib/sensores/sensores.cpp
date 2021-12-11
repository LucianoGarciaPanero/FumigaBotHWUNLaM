#include "./sensores.h"
#include <Arduino.h>

float calcularDistancia(int, int);

/******************************************************************* 
Nombre: calcularDistancia
Entradas:
          + trigPin: int
          + echoPin: int
Salida:
          + distancia: long
Proceso: calcula la distancia a la que se encuentra un objeto en frente
del sensor de ultrasonido, identificado por trigPin y echoPin
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

float calcularDistancia(int trigPin, int echoPin) {
  
  // Inicializando el pin del trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(TIEMPO_ASEGURAR_NIVEL_BAJO_US);
  
  // Comenzar a transmitir
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(TIEMPO_PULSO_US);
  digitalWrite(trigPin, LOW);

  // Lectura del tiempo que estuvo en high
  long duracion = pulseIn(echoPin, HIGH);

  // Calculamos la distancia del objeto
  return (duracion / VALOR_CORRECCION_ONDA) * VELOCIDAD_SONIDO_CM;
}

/******************************************************************* 
Nombre: calcularDistanciaPromedio
Entradas:
          + pinTrig: int
          + pinEcho: int
Salida:
          + distanciaPromedio: float
Proceso: obtiene n mediciones del sensor de ultrasonido, especificado por
pinTrig y pinEcho. Luego, calcular el promedio usando la función 
calcularPromedio.
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

float calcularDistanciaPromedio(int pinTrig, int pinEcho) {
  
  // Inicializar variables
  int n = 15;
  float values[n];

  // Leer n valores
  for(int i = 0; i < n; i++) {
    
    values[i]= calcularDistancia(pinTrig, pinEcho);

  }
  
  // Calcular promedio
  return calcularPromedio(values, n);
}

/******************************************************************* 
Nombre: calcularNivelBateriaPromedio
Entradas:
          + pin: int
          + vMin: float
          + vMax: float
          + constCorr: float
Salida:
          + porcentajeBateria: float
Proceso: obtiene n mediciones de la bateria. Luego, calcula el promedio usando
calcularPromedio. Calcula y devuelve el porcentaje de bateria restante,
teniendo en cuenta el vMin y vMax. Si el porcentaje de batería es menor
a 0 o mayor a 100, entonces devuelve error
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

int calcularNivelBateriaPromedio(int pin) {

  // Inicializar variables
  int n = 100;
  
  float vIn = -1;
  int porcentaje = -1;

  float values[n];  

  // Leer n valores seguidos
  for(int i = 0; i < n; i++) {

    // Convertimos a valores de tensión
    values[i] = analogRead(pin) * CONSTANTE_CORRECCION_VOLTAJE_BATERIA;

  }
  
  // Calcular el promedio leido
  vIn =  calcularPromedio(values, n);

  // Calcular el porcentaje de la carga
  if(vIn > 2.9) {

    porcentaje = PORCENTAJE_MAX_BATERIA;
  } else if(vIn > 2.75) {

    porcentaje = 85;

  } else if(vIn > 2.5) {

    porcentaje = 75;

  } else if(vIn > 2.3) {

    porcentaje = 50;

  } else if(vIn > 2.1) {

    porcentaje = 35;

  } else if(vIn > 1.75) {

    porcentaje = 25;

  } else if(vIn > 1.5) {

    porcentaje = 10;

  } else if(vIn > 1.25) {

    porcentaje = 5; 
  
  } else {

    porcentaje = MIN_NIVEL_QUIMICO_PORCENTAJE;

  }

  return porcentaje;

}

/******************************************************************* 
Nombre: liberarQuimico
Entradas:
          + pinBomba: int
          + tiempoMs: float
Salida: -
Proceso: activa la bomba durante tiempMs, identificandola por pinBomba
Fecha Creación: 01/07/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void liberarQuimico(int pinBomba, long tiempoMs) {

  // Inicializar variable
  long currentTime = 0;
  
  // Prender bomba
  digitalWrite(pinBomba, HIGH);

  // Dejarla prendida durante x ms
  currentTime = millis();
  while(millis() - currentTime < tiempoMs ) {}

  // Apagar bomba
  digitalWrite(pinBomba, LOW);
}

/******************************************************************* 
Nombre: calcularNivelQuimicoPromedio
Entradas:
          + pinTrig: int
          + pinEcho: int
Salida: El nivel de quimico promedio en tanque
Proceso: Teniendo en cuenta las dimensiones del tanque calcula el porcentaje
de quimico en el mismo
Fecha Creación: 06/11/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

int calcularNivelQuimicoPromedio(int pintTrig, int pinEcho) {

  // Obtener la distancia al quimico desde el sensor
  float distancia = calcularDistanciaPromedio(pintTrig, pinEcho);

  // Calcular la distancia del quimico al maximo
  //float nivelQuimicoCm = distancia - DISTANCIA_SENSOR_DESDE_ARRIBA_CM;
  float nivelQuimicoCm = MAXIMA_ALTURA_LIQUIDO_CM - distancia - DISTANCIA_SENSOR_DESDE_ARRIBA_CM;  
  
  // Regla de tres para obtener el porcentaje
  int nivelQuimicoPorcetnaje = nivelQuimicoCm * 100 / MAXIMA_ALTURA_LIQUIDO_CM;
  
  // Verificacion de que el nivel no se encuentra fuera de los limites
  if(nivelQuimicoPorcetnaje < MIN_NIVEL_QUIMICO_PORCENTAJE) {

    return MIN_NIVEL_QUIMICO_PORCENTAJE;

  } else if(nivelQuimicoPorcetnaje > MAX_NIVEL_QUIMICO_PORCENTAJE) {

    return MAX_NIVEL_QUIMICO_PORCENTAJE;

  } else {

    return nivelQuimicoPorcetnaje;
  
  }
}

/******************************************************************* 
Nombre: determinarTiempoDelay
Entradas:
          + distanciaAdelante: float
          + distanciaDerechaActual: float
          + direccion: int
Salida: tiempo_delay
Proceso: evalua las distancias y la direccion del movimiento para determinar
durante cuanto tiempo debe ejecutarlo.
Fecha Creación: 24/09/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

float determinarTiempoDelay(int direccion, int objetoDerecha) {


  if(direccion == DERECHA || direccion == IZQUIERDA) {
      
    return TIEMPO_DELAY_GIRO_MS;

  }
  
  return TIEMPO_DELAY_ADELANTE_MS;
}

/******************************************************************* 
Nombre: mover
Entradas:
          + pIN_MOTOR_IZQUIERDA_IN1: float
          + pIN_MOTOR_IZQUIERDA_IN2: float
          + pIN_MOTOR_DERECHA_IN3: int
          + pIN_MOTOR_DERECHA_IN4: int
          + direccion: int
          + canalIzq: int
          + canalDer: int
          + cicloTrabajo: int
Salida: -
Proceso: según la dirección del movimiento manda las señales a los pines corres
pondientes
Fecha Creación: 24/09/2021
Creador: 
        + Luciano Garcia Panero 
        + Tomás Sánchez Grigioni
—————————————————————– 
Cambiado Por: -
Fecha Cambió: - 
Referencia: -
*****************************************************************/

void mover(int direccion) {

  // Setear la velocidad de movimiento
  ledcWrite(PWM_CHANNEL_MOTOR, VELOCIDAD_ROBOT);
  
  // Realizamos el movimiento
  switch(direccion) {

    case ADELANTE:
    
      // Parar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_PARAR_MS);
      
      // Girar a la izquierda muy poco
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_GIRO_CORTO_MS);

      // Parar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_PARAR_MS);
      
      // Adelante
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      break;

    case ATRAS:
      
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, HIGH);

      break;
    
    case PARAR:

      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);  

      break;

    case IZQUIERDA:

      // Parar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_PARAR_MS);

      // Girar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      break;

    case DERECHA:

      // Parar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_PARAR_MS);

      // Avanzar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, HIGH);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_ADELANTE_MS * 3);

      // Parar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      delay(TIEMPO_DELAY_PARAR_MS);

      // Girar
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, HIGH);

      break;
    
    default:

      digitalWrite(PIN_MOTOR_IZQUIERDA_IN1, LOW);
      digitalWrite(PIN_MOTOR_IZQUIERDA_IN2, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN3, LOW);
      digitalWrite(PIN_MOTOR_DERECHA_IN4, LOW);

      break;
  }
}