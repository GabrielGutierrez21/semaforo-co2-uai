#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Montaje fotografiado: NodeMCU ESP8266 + CCS811 + OLED I2C + modulo RGB R/G/B/GND.
// TVOC es una lectura del sensor; eCO2 es ESTIMADO y no mide CO2 real.
// Confirmar antes de energizar: pines y 3,3 V del CCS811, pantalla SSD1306,
// resistencias integradas R1/R2/R3 del modulo RGB y cabeceras soldadas.
constexpr uint8_t PIN_SDA = D2;  // GPIO4
constexpr uint8_t PIN_SCL = D1;  // GPIO5
constexpr uint8_t PIN_ROJO = D5; // GPIO14 -> R del RGB con resistencia
constexpr uint8_t PIN_VERDE = D6;// GPIO12 -> G del RGB con resistencia
constexpr uint8_t OLED_ANCHO = 128;
constexpr uint8_t OLED_ALTO = 64; // Cambiar a 32 si la pantalla fisica es 128x32.
constexpr uint32_t CALENTAMIENTO_MS = 20UL * 60UL * 1000UL;
constexpr uint8_t MUESTRAS_BASE = 30;
constexpr uint32_t EDAD_MAXIMA_MS = 10000;
constexpr uint32_t REINTENTO_MS = 5000;

Adafruit_CCS811 ccs;
Adafruit_SSD1306 oled(OLED_ANCHO, OLED_ALTO, &Wire, -1);
bool sensorListo = false;
bool oledLista = false;
bool datoValido = false;
uint16_t tvoc = 0;
uint16_t eco2 = 0;
uint16_t baseTvoc = 0;
uint32_t sumaBase = 0;
uint8_t muestrasBase = 0;
uint32_t inicioMs = 0;
uint32_t ultimoDatoMs = 0;
uint32_t ultimoIntentoMs = 0;
uint32_t ultimaPantallaMs = 0;

enum class Estado { SIN_SENSOR, CALENTANDO, REFERENCIA, VERDE, AMARILLO, ROJO, ERROR };

bool i2cPresente(uint8_t direccion) {
  Wire.beginTransmission(direccion);
  return Wire.endTransmission() == 0;
}

bool lecturaVigente(uint32_t ahora) {
  return datoValido && (uint32_t)(ahora - ultimoDatoMs) <= EDAD_MAXIMA_MS;
}

Estado estado(uint32_t ahora) {
  if (!sensorListo) return Estado::SIN_SENSOR;
  if (!lecturaVigente(ahora)) {
    if ((uint32_t)(ahora - inicioMs) <= EDAD_MAXIMA_MS) return Estado::CALENTANDO;
    return Estado::ERROR;
  }
  if ((uint32_t)(ahora - inicioMs) < CALENTAMIENTO_MS) return Estado::CALENTANDO;
  if (muestrasBase < MUESTRAS_BASE) return Estado::REFERENCIA;
  int32_t cambio = (int32_t)tvoc - baseTvoc;
  if (cambio < 50) return Estado::VERDE;
  if (cambio < 150) return Estado::AMARILLO;
  return Estado::ROJO;
}

const char *etiqueta(Estado e) {
  switch (e) {
    case Estado::CALENTANDO: return "ESTABILIZANDO";
    case Estado::REFERENCIA: return "TOMANDO BASE";
    case Estado::VERDE: return "CAMBIO PEQUENO";
    case Estado::AMARILLO: return "CAMBIO MODERADO";
    case Estado::ROJO: return "CAMBIO ALTO";
    case Estado::ERROR: return "LECTURA PERDIDA";
    default: return "SIN SENSOR";
  }
}

void actualizarLuz(uint32_t ahora) {
  Estado e = estado(ahora);
  bool pulso = (ahora / 500) % 2;
  // RGB de catodo comun: HIGH en R/G enciende. R+G produce amarillo.
  // El pin B queda desconectado. No usar sin resistencias de corriente.
  bool rojo = e == Estado::ROJO ||
               ((e == Estado::SIN_SENSOR || e == Estado::ERROR) && pulso) ||
               ((e == Estado::CALENTANDO || e == Estado::REFERENCIA) && pulso);
  bool verde = e == Estado::VERDE || e == Estado::AMARILLO ||
                ((e == Estado::CALENTANDO || e == Estado::REFERENCIA) && pulso);
  if (e == Estado::AMARILLO) rojo = true;
  digitalWrite(PIN_ROJO, rojo ? HIGH : LOW);
  digitalWrite(PIN_VERDE, verde ? HIGH : LOW);
}

void iniciarSensor(uint32_t ahora) {
  if (sensorListo || (uint32_t)(ahora - ultimoIntentoMs) < REINTENTO_MS) return;
  ultimoIntentoMs = ahora;
  uint8_t addr = i2cPresente(0x5A) ? 0x5A : 0x5B;
  if (!i2cPresente(addr) || !ccs.begin(addr, &Wire)) {
    Serial.println(F("CCS811 no responde: revisar 3V3, GND, SDA, SCL, WAKE y soldaduras"));
    return;
  }
  sensorListo = true;
  inicioMs = ahora;
  Serial.printf("CCS811 conectado en 0x%02X; esperar 20 min\n", addr);
}

void leerSensor(uint32_t ahora) {
  if (!sensorListo || !ccs.available()) return;
  if (ccs.readData() != 0) {
    datoValido = false;
    Serial.println(F("Error CCS811: no anunciar verde"));
    return;
  }
  tvoc = ccs.getTVOC();
  eco2 = ccs.geteCO2();
  ultimoDatoMs = ahora;
  datoValido = true;
  if ((uint32_t)(ahora - inicioMs) >= CALENTAMIENTO_MS && muestrasBase < MUESTRAS_BASE) {
    sumaBase += tvoc;
    ++muestrasBase;
    if (muestrasBase == MUESTRAS_BASE) {
      baseTvoc = sumaBase / MUESTRAS_BASE;
      Serial.printf("Base TVOC: %u ppb\n", baseTvoc);
    }
  }
  Serial.printf("%lu,TVOC=%u ppb,eCO2_estimado=%u ppm,base=%u,estado=%s\n",
                (unsigned long)ahora, tvoc, eco2, baseTvoc, etiqueta(estado(ahora)));
}

void mostrar(uint32_t ahora) {
  if (!oledLista || (uint32_t)(ahora - ultimaPantallaMs) < 1000) return;
  ultimaPantallaMs = ahora;
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println(F("AIRE AULA / PROTOTIPO"));
  oled.print(F("TVOC: "));
  if (lecturaVigente(ahora)) oled.print(tvoc); else oled.print(F("--"));
  oled.println(F(" ppb"));
  oled.print(F("eCO2 est: "));
  if (lecturaVigente(ahora)) oled.print(eco2); else oled.print(F("--"));
  oled.println(F(" ppm"));
  oled.print(F("Base: "));
  if (muestrasBase == MUESTRAS_BASE) oled.println(baseTvoc);
  else oled.println(F("pendiente"));
  oled.println(etiqueta(estado(ahora)));
  oled.println(F("No mide CO2 real"));
  oled.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  digitalWrite(PIN_ROJO, LOW);
  digitalWrite(PIN_VERDE, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);

  uint8_t addrOled = i2cPresente(0x3C) ? 0x3C : 0x3D;
  if (i2cPresente(addrOled)) oledLista = oled.begin(SSD1306_SWITCHCAPVCC, addrOled);
  if (!oledLista) Serial.println(F("OLED no detectada; continuar con RGB y Serial"));
  Serial.println(F("NUEVO ESP8266 + CCS811 | TVOC y eCO2 estimado, no CO2 real"));
  ultimoIntentoMs = millis() - REINTENTO_MS; // Primer intento inmediato.
}

void loop() {
  uint32_t ahora = millis();
  iniciarSensor(ahora);
  leerSensor(ahora);
  actualizarLuz(ahora);
  mostrar(ahora);
  delay(10);
}
