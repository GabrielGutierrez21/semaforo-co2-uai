#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_CCS811.h>

// CCS811: TVOC y eCO2 estimado. Este montaje NO mide CO2 real.
// Tres LED independientes, con una resistencia en serie para cada uno.
// Boton D4-GND: no mantener presionado durante encendido/reset.
constexpr uint8_t PIN_SDA = D2;     // GPIO4
constexpr uint8_t PIN_SCL = D1;     // GPIO5
constexpr uint8_t PIN_BOTON = D4;   // GPIO2, INPUT_PULLUP
constexpr uint8_t PIN_VERDE = D5;   // GPIO14
constexpr uint8_t PIN_AMARILLO = D6;// GPIO12
constexpr uint8_t PIN_ROJO = D7;    // GPIO13
constexpr uint32_t CALENTAMIENTO_MS = 20UL * 60UL * 1000UL;
constexpr uint32_t DATO_MAX_EDAD_MS = 10000UL;
constexpr uint32_t REINTENTO_MS = 5000UL;
constexpr uint8_t MUESTRAS_BASE = 30;

Adafruit_CCS811 ccs;
bool sensorListo = false, datoValido = false;
uint16_t tvoc = 0, eco2 = 0, baseTvoc = 0;
uint32_t sumaBase = 0, inicioSensorMs = 0, ultimoDatoMs = 0;
uint32_t ultimoIntentoMs = 0, ultimoReporteMs = 0;
uint8_t muestrasBase = 0;
bool botonCrudo = HIGH, botonEstable = HIGH;
uint32_t cambioBotonMs = 0;

enum class Estado { SIN_SENSOR, CALENTANDO, REFERENCIA, VERDE, AMARILLO, ROJO, ERROR };

bool i2cPresente(uint8_t dir) {
  Wire.beginTransmission(dir);
  return Wire.endTransmission() == 0;
}
bool datoReciente(uint32_t ahora) {
  return datoValido && (uint32_t)(ahora - ultimoDatoMs) <= DATO_MAX_EDAD_MS;
}
Estado estado(uint32_t ahora) {
  if (!sensorListo) return Estado::SIN_SENSOR;
  if (!datoReciente(ahora)) {
    if ((uint32_t)(ahora - inicioSensorMs) <= DATO_MAX_EDAD_MS)
      return Estado::CALENTANDO;
    return Estado::ERROR;
  }
  if ((uint32_t)(ahora - inicioSensorMs) < CALENTAMIENTO_MS)
    return Estado::CALENTANDO;
  if (muestrasBase < MUESTRAS_BASE) return Estado::REFERENCIA;
  int32_t diferencia = (int32_t)tvoc - baseTvoc;
  if (diferencia < 50) return Estado::VERDE;
  if (diferencia < 150) return Estado::AMARILLO;
  return Estado::ROJO;
}
const char *nombre(Estado e) {
  switch (e) {
    case Estado::SIN_SENSOR: return "SIN_SENSOR";
    case Estado::CALENTANDO: return "ESTABILIZANDO";
    case Estado::REFERENCIA: return "TOMANDO_BASE";
    case Estado::VERDE: return "CAMBIO_PEQUENO";
    case Estado::AMARILLO: return "CAMBIO_MODERADO";
    case Estado::ROJO: return "CAMBIO_ALTO";
    default: return "ERROR_LECTURA";
  }
}
void leds(bool verde, bool amarillo, bool rojo) {
  digitalWrite(PIN_VERDE, verde ? HIGH : LOW);
  digitalWrite(PIN_AMARILLO, amarillo ? HIGH : LOW);
  digitalWrite(PIN_ROJO, rojo ? HIGH : LOW);
}
void leerBoton(uint32_t ahora) {
  bool lectura = digitalRead(PIN_BOTON);
  if (lectura != botonCrudo) cambioBotonMs = ahora;
  botonCrudo = lectura;
  if ((uint32_t)(ahora - cambioBotonMs) >= 35) botonEstable = lectura;
}
void senal(uint32_t ahora) {
  if (botonEstable == LOW) {
    // Solo prueba electrica mientras se mantiene pulsado: NO indica aire seguro.
    uint8_t paso = (ahora / 600) % 3;
    leds(paso == 0, paso == 1, paso == 2);
    return;
  }
  Estado e = estado(ahora);
  bool pulso = (ahora / 500) % 2 == 0;
  if (e == Estado::SIN_SENSOR || e == Estado::ERROR) leds(false, false, pulso);
  else if (e == Estado::CALENTANDO || e == Estado::REFERENCIA) leds(false, pulso, false);
  else leds(e == Estado::VERDE, e == Estado::AMARILLO, e == Estado::ROJO);
}
void iniciarSensor(uint32_t ahora) {
  if (sensorListo || (uint32_t)(ahora - ultimoIntentoMs) < REINTENTO_MS) return;
  ultimoIntentoMs = ahora;
  uint8_t dir = i2cPresente(0x5A) ? 0x5A : 0x5B;
  if (!i2cPresente(dir) || !ccs.begin(dir, &Wire)) {
    Serial.println(F("CCS811 no detectado: revisar 3V3/GND, D2/SDA, D1/SCL y WAKE"));
    return;
  }
  sensorListo = true;
  inicioSensorMs = ahora;
  Serial.printf("CCS811 conectado en 0x%02X. Estabilizar 20 min.\n", dir);
}
void leerSensor(uint32_t ahora) {
  if (!sensorListo || !ccs.available()) return;
  if (ccs.readData() != 0) {
    datoValido = false;
    Serial.println(F("Error de lectura CCS811"));
    return;
  }
  tvoc = ccs.getTVOC();
  eco2 = ccs.geteCO2();
  ultimoDatoMs = ahora;
  datoValido = true;
  if ((uint32_t)(ahora - inicioSensorMs) >= CALENTAMIENTO_MS &&
      muestrasBase < MUESTRAS_BASE) {
    sumaBase += tvoc;
    ++muestrasBase;
    if (muestrasBase == MUESTRAS_BASE) {
      baseTvoc = sumaBase / MUESTRAS_BASE;
      Serial.printf("Referencia TVOC: %u ppb\n", baseTvoc);
    }
  }
}
void reportar(uint32_t ahora) {
  if ((uint32_t)(ahora - ultimoReporteMs) < 1000) return;
  ultimoReporteMs = ahora;
  Estado e = estado(ahora);
  if (datoReciente(ahora))
    Serial.printf("TVOC=%u ppb, eCO2_ESTIMADO=%u ppm, base=%u ppb, estado=%s%s\n",
                  tvoc, eco2, baseTvoc, nombre(e),
                  botonEstable == LOW ? ", PRUEBA_LED" : "");
  else
    Serial.printf("Sin dato valido, estado=%s%s\n", nombre(e),
                  botonEstable == LOW ? ", PRUEBA_LED" : "");
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
  Serial.println(F("NUEVO ESP8266 / CCS811 / 3 LED / boton D4"));
  Serial.println(F("eCO2 estimado; NO mide CO2 real. No pulsar D4 al reiniciar."));
  ultimoIntentoMs = millis() - REINTENTO_MS;
}
void loop() {
  uint32_t ahora = millis();
  iniciarSensor(ahora);
  leerSensor(ahora);
  leerBoton(ahora);
  senal(ahora);
  reportar(ahora);
  delay(10);
}
