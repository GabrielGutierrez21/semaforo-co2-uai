#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Primer modelo: TVOC y eCO2 estimado por CCS811. NO mide CO2 real.
// Placa: NodeMCU ESP8266; LED independientes con una resistencia por LED.
constexpr uint8_t PIN_SDA = D2;
constexpr uint8_t PIN_SCL = D1;
constexpr uint8_t PIN_VERDE = D5;
constexpr uint8_t PIN_AMARILLO = D6;
constexpr uint8_t PIN_ROJO = D7;
constexpr uint8_t OLED_ANCHO = 128;
constexpr uint8_t OLED_ALTO = 64; // Ajustar tras confirmar el modelo fisico.
constexpr uint32_t CALENTAMIENTO_MS = 20UL * 60UL * 1000UL;
constexpr uint16_t MUESTRAS_REFERENCIA = 30;
constexpr uint32_t EDAD_MAXIMA_MS = 10000;
constexpr uint32_t INTERVALO_PANTALLA_MS = 500;
constexpr uint32_t INTERVALO_REINTENTO_MS = 5000;
constexpr uint8_t OLED_DIRECCION_PRIMARIA = 0x3C;
constexpr uint8_t OLED_DIRECCION_ALTERNA = 0x3D;
const char *RED_LOCAL = "Semaforo-Aula-UAI";
const char *CLAVE_LOCAL = "AulaUAI2026"; // Solo red local de demostracion.

Adafruit_CCS811 sensor;
Adafruit_SSD1306 pantalla(OLED_ANCHO, OLED_ALTO, &Wire, -1);
ESP8266WebServer web(80);
bool sensorListo = false;
bool pantallaLista = false;
bool redLista = false;
bool datoValido = false;
uint16_t tvoc = 0;
uint16_t eco2 = 0;
uint16_t referenciaTvoc = 0;
uint32_t sumaReferencia = 0;
uint16_t muestras = 0;
uint32_t ultimoDatoMs = 0;
uint32_t ultimoIntentoMs = 0;
uint32_t ultimaPantallaMs = 0;
uint32_t inicioSensorMs = 0;

enum class Estado { SIN_SENSOR, CALENTANDO, REFERENCIA, VERDE, AMARILLO, ROJO, ERROR };

bool vigente(uint32_t ahora) {
  return datoValido && (uint32_t)(ahora - ultimoDatoMs) <= EDAD_MAXIMA_MS;
}

Estado estado(uint32_t ahora) {
  if (!sensorListo) return Estado::SIN_SENSOR;
  if (!vigente(ahora)) {
    if ((uint32_t)(ahora - inicioSensorMs) < EDAD_MAXIMA_MS) return Estado::CALENTANDO;
    return Estado::ERROR;
  }
  if ((uint32_t)(ahora - inicioSensorMs) < CALENTAMIENTO_MS) return Estado::CALENTANDO;
  if (muestras < MUESTRAS_REFERENCIA) return Estado::REFERENCIA;
  int32_t diferencia = (int32_t)tvoc - referenciaTvoc;
  if (diferencia < 50) return Estado::VERDE;
  if (diferencia < 150) return Estado::AMARILLO;
  return Estado::ROJO;
}

const char *nombre(Estado e) {
  switch (e) {
    case Estado::CALENTANDO: return "ESTABILIZANDO";
    case Estado::REFERENCIA: return "TOMANDO REFERENCIA";
    case Estado::VERDE: return "SIN CAMBIO ALTO";
    case Estado::AMARILLO: return "CAMBIO MODERADO";
    case Estado::ROJO: return "CAMBIO ALTO";
    case Estado::ERROR: return "LECTURA PERDIDA";
    default: return "SIN SENSOR";
  }
}

void luces(uint32_t ahora) {
  Estado e = estado(ahora);
  bool parpadeo = (ahora / 500) % 2;
  digitalWrite(PIN_VERDE, e == Estado::VERDE);
  digitalWrite(PIN_AMARILLO, e == Estado::AMARILLO ||
                                 ((e == Estado::CALENTANDO || e == Estado::REFERENCIA ||
                                   e == Estado::SIN_SENSOR || e == Estado::ERROR) && parpadeo));
  digitalWrite(PIN_ROJO, e == Estado::ROJO || (e == Estado::ERROR && parpadeo));
}

bool direccionPresente(uint8_t direccion) {
  Wire.beginTransmission(direccion);
  return Wire.endTransmission() == 0;
}

void mostrar(uint32_t ahora) {
  if (!pantallaLista || (uint32_t)(ahora - ultimaPantallaMs) < INTERVALO_PANTALLA_MS) return;
  ultimaPantallaMs = ahora;
  pantalla.clearDisplay();
  pantalla.setTextColor(SSD1306_WHITE);
  pantalla.setTextSize(1);
  pantalla.setCursor(0, 0);
  pantalla.println(F("AIRE AULA - PROTOTIPO"));
  pantalla.print(F("TVOC: "));
  if (vigente(ahora)) pantalla.print(tvoc); else pantalla.print(F("--"));
  pantalla.println(F(" ppb"));
  pantalla.print(F("eCO2: "));
  if (vigente(ahora)) pantalla.print(eco2); else pantalla.print(F("--"));
  pantalla.println(F(" ppm estimados"));
  pantalla.print(F("Ref TVOC: "));
  if (muestras == MUESTRAS_REFERENCIA) pantalla.println(referenciaTvoc);
  else pantalla.println(F("pendiente"));
  pantalla.println(nombre(estado(ahora)));
  if (redLista) pantalla.println(F("WiFi: 192.168.4.1"));
  pantalla.println(F("NO mide CO2 real"));
  pantalla.display();
}

void pagina() {
  uint32_t ahora = millis();
  String html = F("<!doctype html><html lang='es'><meta charset='utf-8'>"
                  "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                  "<meta http-equiv='refresh' content='3'>"
                  "<title>Semaforo aula</title><body style='font:20px system-ui;max-width:36rem;margin:2rem'>"
                  "<h1>Calidad del aire - prototipo</h1><p>Estado: ");
  html += nombre(estado(ahora));
  html += F("</p><p>TVOC: ");
  html += vigente(ahora) ? String(tvoc) : String("--");
  html += F(" ppb</p><p>eCO2 calculado: ");
  html += vigente(ahora) ? String(eco2) : String("--");
  html += F(" ppm</p><p>Referencia TVOC: ");
  html += muestras == MUESTRAS_REFERENCIA ? String(referenciaTvoc) : String("pendiente");
  html += F(" ppb</p><small>El CCS811 estima eCO2 a partir de gases volatiles. "
            "No mide CO2 real ni valida ventilacion.</small></body></html>");
  web.send(200, "text/html; charset=utf-8", html);
}

void intentarSensor(uint32_t ahora) {
  if (sensorListo || (uint32_t)(ahora - ultimoIntentoMs) < INTERVALO_REINTENTO_MS) return;
  ultimoIntentoMs = ahora;
  if (!sensor.begin(0x5A, &Wire)) {
    Serial.println(F("CCS811 no responde. Revisar 3V3/GND/SDA/SCL/WAKE y pinout."));
    return;
  }
  sensorListo = true;
  inicioSensorMs = ahora;
  Serial.println(F("CCS811 conectado; esperar estabilizacion antes de fijar referencia."));
}

void leerSensor(uint32_t ahora) {
  if (!sensorListo || !sensor.available()) return;
  if (sensor.readData() != 0) {
    Serial.println(F("Error de lectura CCS811; no anunciar estado verde."));
    datoValido = false;
    return;
  }
  tvoc = sensor.getTVOC();
  eco2 = sensor.geteCO2();
  ultimoDatoMs = ahora;
  datoValido = true;
  if ((uint32_t)(ahora - inicioSensorMs) >= CALENTAMIENTO_MS &&
      muestras < MUESTRAS_REFERENCIA) {
    sumaReferencia += tvoc;
    ++muestras;
    if (muestras == MUESTRAS_REFERENCIA) {
      referenciaTvoc = sumaReferencia / MUESTRAS_REFERENCIA;
      Serial.printf("Referencia TVOC: %u ppb\n", referenciaTvoc);
    }
  }
  Serial.printf("%lu,%u,%u,%s\n", (unsigned long)ahora, tvoc, eco2, nombre(estado(ahora)));
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_AMARILLO, LOW);
  digitalWrite(PIN_ROJO, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);

  uint8_t addr = direccionPresente(OLED_DIRECCION_PRIMARIA)
                     ? OLED_DIRECCION_PRIMARIA : OLED_DIRECCION_ALTERNA;
  if (direccionPresente(addr)) pantallaLista = pantalla.begin(SSD1306_SWITCHCAPVCC, addr);
  if (!pantallaLista) Serial.println(F("OLED no detectada; seguir con LED, Serial y pagina local."));

  WiFi.mode(WIFI_AP);
  redLista = WiFi.softAP(RED_LOCAL, CLAVE_LOCAL);
  if (redLista) {
    web.on("/", pagina);
    web.begin();
    Serial.printf("Red: %s / clave: %s / http://%s\n",
                  RED_LOCAL, CLAVE_LOCAL, WiFi.softAPIP().toString().c_str());
  }
  Serial.println(F("ms,TVOC_ppb,eCO2_estimado_ppm,estado"));
  // Primer intento inmediato; en caso de fallo se reintenta cada 5 s.
  ultimoIntentoMs = millis() - INTERVALO_REINTENTO_MS;
}

void loop() {
  uint32_t ahora = millis();
  if (redLista) web.handleClient();
  intentarSensor(ahora);
  leerSensor(ahora);
  luces(ahora);
  mostrar(ahora);
  delay(10);
}
