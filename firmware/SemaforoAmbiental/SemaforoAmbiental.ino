#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "WebUI.h"

// Prototipo: CCS811 mide TVOC y estima eCO2; NO mide CO2 real.
// CCS811 WAKE se conecta directamente a GND, no a un pin digital.
constexpr uint8_t PIN_SDA = D2;
constexpr uint8_t PIN_SCL = D1;
constexpr uint8_t PIN_BOTON = D4; // Pulsador hacia GND. Soltar al encender.
constexpr uint8_t PIN_VERDE = D5;
constexpr uint8_t PIN_AMARILLO = D6;
constexpr uint8_t PIN_ROJO = D7;

// Acondicionamiento recomendado antes de interpretar cambios de TVOC.
constexpr uint32_t CALENTAMIENTO_MS = 20UL * 60UL * 1000UL;
constexpr uint32_t DATO_MAX_EDAD_MS = 10000UL;
constexpr uint32_t REINTENTO_MS = 5000UL;
constexpr uint8_t MUESTRAS_BASE = 30;
constexpr uint16_t OLED_ANCHO = 128;
constexpr uint16_t OLED_ALTO = 64;

const char* WIFI_NOMBRE = "Aire-UAI";
const char* WIFI_CLAVE = "AireUAI2026";
constexpr uint32_t REGISTRO_MS = 60000UL;
constexpr size_t HISTORICO_LIMITE = 64000;
ESP8266WebServer servidor(80);
bool almacenamientoListo = false;
bool registroInicial = false;
uint32_t ultimaGrabacionMs = 0;
uint32_t sesion = 1;

Adafruit_CCS811 ccs;
Adafruit_SSD1306 pantalla(OLED_ANCHO, OLED_ALTO, &Wire, -1);
bool pantallaLista = false;
bool sensorListo = false;
bool datoValido = false;
bool botonCrudo = HIGH;
bool botonEstable = HIGH;
uint16_t tvoc = 0, eco2 = 0, baseTvoc = 0;
uint32_t sumaBase = 0, inicioSensorMs = 0, ultimoDatoMs = 0;
uint32_t ultimoIntentoMs = 0, ultimoReporteMs = 0, ultimaPantallaMs = 0;
uint32_t cambioBotonMs = 0;
uint8_t muestrasBase = 0;

enum class Estado { SIN_SENSOR, CALENTANDO, REFERENCIA, VERDE, AMARILLO, ROJO, ERROR };

bool i2cPresente(uint8_t direccion) {
  Wire.beginTransmission(direccion);
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

const char* nombre(Estado e) {
  switch (e) {
    case Estado::SIN_SENSOR: return "SIN SENSOR";
    case Estado::CALENTANDO: return "CALENTANDO";
    case Estado::REFERENCIA: return "BASE TVOC";
    case Estado::VERDE: return "CAMBIO BAJO";
    case Estado::AMARILLO: return "CAMBIO MEDIO";
    case Estado::ROJO: return "CAMBIO ALTO";
    default: return "ERROR SENSOR";
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
    uint8_t paso = (ahora / 600) % 3;
    leds(paso == 0, paso == 1, paso == 2);
    return;
  }
  Estado e = estado(ahora);
  bool pulso = (ahora / 500) % 2 == 0;
  if (e == Estado::SIN_SENSOR || e == Estado::ERROR)
    leds(false, false, pulso);
  else if (e == Estado::CALENTANDO || e == Estado::REFERENCIA)
    leds(false, pulso, false);
  else
    leds(e == Estado::VERDE, e == Estado::AMARILLO, e == Estado::ROJO);
}

void iniciarPantalla() {
  uint8_t direccion = i2cPresente(0x3C) ? 0x3C : 0x3D;
  if (!i2cPresente(direccion) ||
      !pantalla.begin(SSD1306_SWITCHCAPVCC, direccion)) {
    Serial.println(F("OLED no detectada: revisar VCC/GND, SDA/D2 y SCL/D1"));
    return; // El semaforo sigue funcionando sin pantalla.
  }
  pantallaLista = true;
  pantalla.setTextColor(SSD1306_WHITE);
  Serial.printf("OLED detectada en 0x%02X\n", direccion);
}

void actualizarPantalla(uint32_t ahora) {
  if (!pantallaLista || (uint32_t)(ahora - ultimaPantallaMs) < 500) return;
  ultimaPantallaMs = ahora;
  Estado e = estado(ahora);
  pantalla.clearDisplay();
  pantalla.setTextSize(1);
  pantalla.setCursor(0, 0);
  pantalla.print(F("AIRE / CCS811"));
  pantalla.setCursor(0, 12);
  pantalla.print(F("TVOC: "));
  if (datoReciente(ahora)) pantalla.print(tvoc);
  else pantalla.print(F("--"));
  pantalla.print(F(" ppb"));
  pantalla.setCursor(0, 23);
  pantalla.print(F("eCO2: "));
  if (datoReciente(ahora)) pantalla.print(eco2);
  else pantalla.print(F("--"));
  pantalla.print(F(" ppm"));
  pantalla.setCursor(0, 35);
  pantalla.print(nombre(e));

  if (e == Estado::CALENTANDO && sensorListo) {
    uint32_t transcurrido = (uint32_t)(ahora - inicioSensorMs);
    if (transcurrido < CALENTAMIENTO_MS) {
      pantalla.setCursor(0, 46);
      pantalla.print((CALENTAMIENTO_MS - transcurrido + 999) / 1000);
      pantalla.print(F(" s restantes"));
    }
  } else if (e == Estado::REFERENCIA) {
    pantalla.setCursor(0, 46);
    pantalla.print(muestrasBase);
    pantalla.print(F("/30 muestras"));
  } else if (botonEstable == LOW) {
    pantalla.setCursor(0, 46);
    pantalla.print(F("PRUEBA LED"));
  }

  pantalla.drawFastHLine(0, 55, OLED_ANCHO, SSD1306_WHITE);
  pantalla.setCursor(0, 56);
  pantalla.print(F("eCO2 ESTIMADO"));
  pantalla.display();
}

void iniciarSensor(uint32_t ahora) {
  if (sensorListo || (uint32_t)(ahora - ultimoIntentoMs) < REINTENTO_MS) return;
  ultimoIntentoMs = ahora;
  uint8_t direccion = i2cPresente(0x5A) ? 0x5A : 0x5B;
  if (!i2cPresente(direccion) || !ccs.begin(direccion, &Wire)) {
    Serial.println(F("CCS811 no detectado: revisar 3V3/GND, WAKE/GND, D2/SDA, D1/SCL"));
    return;
  }
  sensorListo = true;
  inicioSensorMs = ahora;
  Serial.printf("CCS811 conectado en 0x%02X. Espera de prueba: %lu s.\n",
                direccion, (unsigned long)(CALENTAMIENTO_MS / 1000));
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
  if (datoReciente(ahora))
    Serial.printf("TVOC=%u ppb, eCO2_ESTIMADO=%u ppm, base=%u ppb, estado=%s%s\n",
                  tvoc, eco2, baseTvoc, nombre(estado(ahora)),
                  botonEstable == LOW ? ", PRUEBA_LED" : "");
  else
    Serial.printf("Sin dato valido, estado=%s%s\n", nombre(estado(ahora)),
                  botonEstable == LOW ? ", PRUEBA_LED" : "");
}


const char* codigoEstado(Estado e) {
  switch (e) {
    case Estado::SIN_SENSOR: return "SIN_SENSOR";
    case Estado::CALENTANDO: return "CALENTANDO";
    case Estado::REFERENCIA: return "REFERENCIA";
    case Estado::VERDE: return "VERDE";
    case Estado::AMARILLO: return "AMARILLO";
    case Estado::ROJO: return "ROJO";
    default: return "ERROR";
  }
}

void iniciarAlmacenamiento() {
  almacenamientoListo = LittleFS.begin();
  if (!almacenamientoListo) {
    Serial.println(F("LittleFS no disponible: verificar tamaño de flash y partición FS"));
    return;
  }
  File anterior = LittleFS.open("/sesion.txt", "r");
  if (anterior) {
    sesion = (uint32_t)anterior.parseInt() + 1;
    anterior.close();
  }
  File actual = LittleFS.open("/sesion.txt", "w");
  if (actual) {
    actual.print(sesion);
    actual.close();
  }
  Serial.printf("Histórico activo. Sesión %lu\n", (unsigned long)sesion);
}

void guardarLectura(uint32_t ahora) {
  if (!almacenamientoListo || !datoReciente(ahora)) return;
  if (registroInicial && (uint32_t)(ahora - ultimaGrabacionMs) < REGISTRO_MS) return;
  const char* ruta = "/historico.csv";
  if (LittleFS.exists(ruta)) {
    File previo = LittleFS.open(ruta, "r");
    size_t bytes = previo ? previo.size() : 0;
    if (previo) previo.close();
    if (bytes >= HISTORICO_LIMITE) {
      LittleFS.remove("/historico_anterior.csv");
      if (!LittleFS.rename(ruta, "/historico_anterior.csv")) {
        Serial.println(F("No se pudo rotar el histórico; se omite esta muestra"));
        return;
      }
    }
  }
  bool nuevo = !LittleFS.exists(ruta);
  File f = LittleFS.open(ruta, "a");
  if (!f) {
    Serial.println(F("Error de escritura LittleFS"));
    return;
  }
  if (nuevo) f.println(F("sesion,segundos,tvoc_ppb,eco2_estimado_ppm,base_tvoc_ppb,estado"));
  f.printf("%lu,%lu,%u,%u,%u,%s\n",
           (unsigned long)sesion, (unsigned long)(ahora / 1000),
           tvoc, eco2, muestrasBase == MUESTRAS_BASE ? baseTvoc : 0,
           codigoEstado(estado(ahora)));
  f.close();
  registroInicial = true;
  ultimaGrabacionMs = ahora;
}

void responderEstado() {
  uint32_t ahora = millis();
  Estado e = estado(ahora);
  bool valido = datoReciente(ahora);
  uint32_t restante = 0;
  if (sensorListo && (uint32_t)(ahora - inicioSensorMs) < CALENTAMIENTO_MS)
    restante = (CALENTAMIENTO_MS - (uint32_t)(ahora - inicioSensorMs) + 999) / 1000;
  String json;
  json.reserve(270);
  json += F("{\"estado\":\"");
  json += codigoEstado(e);
  json += F("\",\"tvoc\":");
  json += valido ? String(tvoc) : "null";
  json += F(",\"eco2\":");
  json += valido ? String(eco2) : "null";
  json += F(",\"base_tvoc\":");
  json += muestrasBase == MUESTRAS_BASE ? String(baseTvoc) : "null";
  json += F(",\"sesion\":");
  json += sesion;
  json += F(",\"uptime_s\":");
  json += ahora / 1000;
  json += F(",\"restante_s\":");
  json += restante;
  json += F(",\"muestras_base\":");
  json += muestrasBase;
  json += F(",\"registrando\":");
  json += almacenamientoListo ? F("true}") : F("false}");
  servidor.sendHeader(F("Cache-Control"), F("no-store"));
  servidor.send(200, F("application/json; charset=utf-8"), json);
}

void responderHistorico(const char* ruta) {
  if (!almacenamientoListo) {
    servidor.send(503, F("text/plain; charset=utf-8"), F("Almacenamiento no disponible"));
    return;
  }
  File f = LittleFS.open(ruta, "r");
  if (!f) {
    servidor.send(200, F("text/csv; charset=utf-8"),
                  F("sesion,segundos,tvoc_ppb,eco2_estimado_ppm,base_tvoc_ppb,estado\n"));
    return;
  }
  servidor.sendHeader(F("Cache-Control"), F("no-store"));
  servidor.streamFile(f, F("text/csv; charset=utf-8"));
  f.close();
}

void iniciarWeb() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(WIFI_NOMBRE, WIFI_CLAVE))
    Serial.println(F("No se pudo crear la red Wi-Fi local"));
  servidor.on("/", HTTP_GET, []() {
    servidor.sendHeader(F("Cache-Control"), F("no-store"));
    servidor.send_P(200, PSTR("text/html; charset=utf-8"), WEB_UI);
  });
  servidor.on("/api/estado", HTTP_GET, responderEstado);
  servidor.on("/historico.csv", HTTP_GET, []() { responderHistorico("/historico.csv"); });
  servidor.on("/historico-anterior.csv", HTTP_GET, []() { responderHistorico("/historico_anterior.csv"); });
  servidor.onNotFound([]() { servidor.send(404, F("text/plain"), F("Ruta no encontrada")); });
  servidor.begin();
  Serial.printf("Web local: Wi-Fi %s / clave %s / http://%s\n",
                WIFI_NOMBRE, WIFI_CLAVE, WiFi.softAPIP().toString().c_str());
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
  iniciarPantalla();
  iniciarAlmacenamiento();
  iniciarWeb();
  ultimoIntentoMs = millis() - REINTENTO_MS;
  Serial.println(F("ESP8266 + CCS811 + OLED SSD1306; eCO2 estimado"));
}

void loop() {
  uint32_t ahora = millis();
  iniciarSensor(ahora);
  leerSensor(ahora);
  leerBoton(ahora);
  senal(ahora);
  actualizarPantalla(ahora);
  reportar(ahora);
  guardarLectura(ahora);
  servidor.handleClient();
  delay(5);
}
