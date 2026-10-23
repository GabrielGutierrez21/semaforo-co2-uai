#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>

// Version para SCD40 real. Un CCS811 NO sirve con este programa.
// NodeMCU ESP8266: SDA D2, SCL D1; LED verde D5, amarillo D6, rojo D7.
// D4 es un pulsador a GND para probar los LED; soltarlo al arrancar.
constexpr uint8_t PIN_SDA = D2;
constexpr uint8_t PIN_SCL = D1;
constexpr uint8_t PIN_BOTON = D4;
constexpr uint8_t PIN_VERDE = D5;
constexpr uint8_t PIN_AMARILLO = D6;
constexpr uint8_t PIN_ROJO = D7;
constexpr uint8_t DIRECCION_SCD40 = 0x62;
constexpr uint16_t UMBRAL_VERDE_PPM = 700;
constexpr uint16_t UMBRAL_ROJO_PPM = 1300;
constexpr uint32_t REINTENTO_MS = 5000;
constexpr uint32_t CONSULTA_MS = 1000;
constexpr uint32_t DATO_MAX_EDAD_MS = 20000;

SensirionI2cScd4x sensor;
bool sensorIniciado = false;
bool datoValido = false;
uint16_t co2Ppm = 0;
uint32_t ultimoIntentoMs = 0;
uint32_t ultimaConsultaMs = 0;
uint32_t ultimoDatoMs = 0;
uint32_t ultimoAvisoMs = 0;
bool botonCrudo = HIGH;
bool botonEstable = HIGH;
uint32_t cambioBotonMs = 0;

void leds(bool verde, bool amarillo, bool rojo) {
  digitalWrite(PIN_VERDE, verde ? HIGH : LOW);
  digitalWrite(PIN_AMARILLO, amarillo ? HIGH : LOW);
  digitalWrite(PIN_ROJO, rojo ? HIGH : LOW);
}

void leerBoton(uint32_t ahora) {
  const bool lectura = digitalRead(PIN_BOTON);
  if (lectura != botonCrudo) cambioBotonMs = ahora;
  botonCrudo = lectura;
  if (static_cast<uint32_t>(ahora - cambioBotonMs) >= 35) {
    botonEstable = lectura;
  }
}

void iniciarSensor(uint32_t ahora) {
  if (sensorIniciado || static_cast<uint32_t>(ahora - ultimoIntentoMs) < REINTENTO_MS) return;
  ultimoIntentoMs = ahora;

  Wire.beginTransmission(DIRECCION_SCD40);
  if (Wire.endTransmission() != 0) {
    Serial.println(F("SCD40 ausente en 0x62: comprobar VIN, GND, SDA/D2 y SCL/D1"));
    return;
  }

  sensor.begin(Wire, DIRECCION_SCD40);
  // Un reinicio del ESP8266 puede dejar al sensor midiendo; detenerlo primero.
  sensor.stopPeriodicMeasurement();
  delay(500);
  uint64_t serie = 0;
  if (sensor.getSerialNumber(serie) != 0 || sensor.startPeriodicMeasurement() != 0) {
    Serial.println(F("SCD40 responde, pero no se pudo iniciar la medicion"));
    return;
  }
  sensorIniciado = true;
  datoValido = false;
  Serial.println(F("SCD40 iniciado. Primera medicion en unos 5 segundos."));
}

void leerSensor(uint32_t ahora) {
  if (!sensorIniciado || static_cast<uint32_t>(ahora - ultimaConsultaMs) < CONSULTA_MS) return;
  ultimaConsultaMs = ahora;
  bool disponible = false;
  if (sensor.getDataReadyStatus(disponible) != 0) {
    Serial.println(F("Error consultando SCD40"));
    return;
  }
  if (!disponible) return;

  uint16_t nuevaLectura = 0;
  float temperatura = 0.0f;
  float humedad = 0.0f;
  if (sensor.readMeasurement(nuevaLectura, temperatura, humedad) != 0 || nuevaLectura == 0) {
    Serial.println(F("Lectura SCD40 invalida"));
    return;
  }
  co2Ppm = nuevaLectura;
  ultimoDatoMs = ahora;
  datoValido = true;
  Serial.printf("CO2=%u ppm; temperatura=%.1f C; humedad=%.1f %%\n",
                co2Ppm, temperatura, humedad);
}

void senal(uint32_t ahora) {
  if (botonEstable == LOW) {
    // Prueba electrica: no representa una medicion de CO2.
    const uint8_t paso = (ahora / 600) % 3;
    leds(paso == 0, paso == 1, paso == 2);
    return;
  }
  const bool pulso = (ahora / 500) % 2 == 0;
  if (!sensorIniciado || (sensorIniciado && datoValido &&
      static_cast<uint32_t>(ahora - ultimoDatoMs) > DATO_MAX_EDAD_MS)) {
    leds(false, false, pulso);  // Falla: rojo intermitente.
  } else if (!datoValido) {
    leds(false, pulso, false);  // Esperando primera lectura: amarillo intermitente.
  } else if (co2Ppm < UMBRAL_VERDE_PPM) {
    leds(true, false, false);
  } else if (co2Ppm <= UMBRAL_ROJO_PPM) {
    leds(false, true, false);
  } else {
    leds(false, false, true);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);
  leds(false, false, false);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);
  Serial.println(F("ESP8266 + SCD40: CO2 real en ppm"));
  Serial.println(F("No mantener pulsado D4 al encender o reiniciar."));
  ultimoIntentoMs = millis() - REINTENTO_MS;
}

void loop() {
  const uint32_t ahora = millis();
  iniciarSensor(ahora);
  leerSensor(ahora);
  leerBoton(ahora);
  senal(ahora);
  if (!sensorIniciado && static_cast<uint32_t>(ahora - ultimoAvisoMs) >= 10000) {
    ultimoAvisoMs = ahora;
    Serial.println(F("Sin sensor SCD40: rojo intermitente. Revisar conexiones."));
  }
  delay(10);
}
