#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Plan B para Avance #2: mide humedad y temperatura, NO CO2.
// ESP32 DevKit clásico; confirmar serigrafía de la placa prestada.
constexpr uint8_t PIN_DHT = 23;
constexpr uint8_t PIN_VERDE = 25;
constexpr uint8_t PIN_AMARILLO = 26;
constexpr uint8_t PIN_ROJO = 27;
constexpr uint8_t PIN_BOTON = 33; // Contacto a GND, INPUT_PULLUP.
constexpr uint32_t INTERVALO_LECTURA_MS = 2500;
constexpr uint32_t MAX_EDAD_DATO_MS = 10000;

DHT dht(PIN_DHT, DHT22);
WebServer servidor(80);
const char* SSID_LOCAL = "Semaforo-Aula-UAI";
char claveLocal[17]; // Temporal; cambia en cada reinicio.
bool redLocal = false;

enum class Estado { INICIANDO, VERDE, AMARILLO, ROJO, ERROR };
Estado estadoReal = Estado::INICIANDO;
float humedad = NAN;
float temperatura = NAN;
bool datoValido = false;
uint32_t ultimoDatoMs = 0;
uint32_t ultimaLecturaMs = 0;
uint32_t ultimoBotonMs = 0;
bool botonAnterior = HIGH;
uint8_t modoPrueba = 0; // 0=real; 1=verde; 2=amarillo; 3=rojo.

// Rangos ILUSTRATIVOS para el prototipo, ajustables con mediciones reales.
// Los colores describen humedad, no seguridad sanitaria ni ventilación/CO2.
Estado clasificarHumedad(float rh) {
  if (rh >= 40.0f && rh <= 60.0f) return Estado::VERDE;
  if (rh >= 30.0f && rh <= 70.0f) return Estado::AMARILLO;
  return Estado::ROJO;
}

const char* nombre(Estado e) {
  switch (e) {
    case Estado::VERDE: return "VERDE";
    case Estado::AMARILLO: return "AMARILLO";
    case Estado::ROJO: return "ROJO";
    case Estado::ERROR: return "ERROR";
    default: return "INICIANDO";
  }
}

Estado senalVisible() {
  if (modoPrueba == 1) return Estado::VERDE;
  if (modoPrueba == 2) return Estado::AMARILLO;
  if (modoPrueba == 3) return Estado::ROJO;
  return estadoReal;
}

void responderApi() {
  bool vigente = datoValido && (uint32_t)(millis() - ultimoDatoMs) <= MAX_EDAD_DATO_MS;
  String json = "{\"humedad\":";
  json += vigente ? String(humedad, 1) : "null";
  json += ",\"temperatura\":";
  json += vigente ? String(temperatura, 1) : "null";
  json += ",\"estadoReal\":\"";
  json += nombre(estadoReal);
  json += "\",\"senal\":\"";
  json += nombre(senalVisible());
  json += "\",\"modoPrueba\":";
  json += modoPrueba ? "true}" : "false}";
  servidor.send(200, "application/json; charset=utf-8", json);
}

void leerDht() {
  uint32_t ahora = millis();
  if ((uint32_t)(ahora - ultimaLecturaMs) < INTERVALO_LECTURA_MS) return;
  ultimaLecturaMs = ahora;
  float rh = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(rh) || isnan(t) || rh < 0 || rh > 100) {
    Serial.println("Lectura DHT22 invalida; revisar alimentacion, DATA y pull-up.");
    return;
  }
  humedad = rh;
  temperatura = t;
  ultimoDatoMs = ahora;
  datoValido = true;
  estadoReal = clasificarHumedad(rh);
  Serial.printf("%lu,%.1f,%.1f,%s,%s\n", (unsigned long)ahora,
                humedad, temperatura, nombre(estadoReal),
                modoPrueba ? "PRUEBA" : "REAL");
}

void leerBoton() {
  bool boton = digitalRead(PIN_BOTON);
  uint32_t ahora = millis();
  if (botonAnterior == HIGH && boton == LOW &&
      (uint32_t)(ahora - ultimoBotonMs) >= 250) {
    modoPrueba = (modoPrueba + 1) % 4;
    ultimoBotonMs = ahora;
    Serial.printf("Modo: %s (%u)\n", modoPrueba ? "PRUEBA" : "REAL", modoPrueba);
  }
  botonAnterior = boton;
}

void actualizarLuces() {
  uint32_t ahora = millis();
  if (!datoValido) estadoReal = ahora > MAX_EDAD_DATO_MS ? Estado::ERROR : Estado::INICIANDO;
  else if ((uint32_t)(ahora - ultimoDatoMs) > MAX_EDAD_DATO_MS) estadoReal = Estado::ERROR;
  Estado visible = senalVisible();
  bool verde = visible == Estado::VERDE;
  bool amarillo = visible == Estado::AMARILLO;
  bool rojo = visible == Estado::ROJO;
  if (visible == Estado::INICIANDO) amarillo = (ahora / 500) % 2;
  if (visible == Estado::ERROR) rojo = amarillo = (ahora / 500) % 2;
  digitalWrite(PIN_VERDE, verde);
  digitalWrite(PIN_AMARILLO, amarillo);
  digitalWrite(PIN_ROJO, rojo);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  dht.begin();
  WiFi.mode(WIFI_AP);
  snprintf(claveLocal, sizeof(claveLocal), "%08lX%08lX",
           (unsigned long)esp_random(), (unsigned long)esp_random());
  redLocal = WiFi.softAP(SSID_LOCAL, claveLocal);
  if (redLocal) {
    servidor.on("/", []() {
      servidor.send(200, "text/html; charset=utf-8",
        "<!doctype html><html lang='es'><meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>Semaforo de humedad UAI</title>"
        "<body style='font:20px system-ui;max-width:35rem;margin:2rem'>"
        "<h1>Humedad en sala</h1><p id='dato'>Esperando sensor...</p>"
        "<p id='estado'></p><small>Prototipo de humedad/temperatura. No mide CO2.</small>"
        "<script>async function ver(){try{const r=await fetch('/api',{cache:'no-store'});"
        "const d=await r.json();document.getElementById('dato').textContent="
        "(d.humedad===null?'--':d.humedad)+' % HR; '+"
        "(d.temperatura===null?'--':d.temperatura)+' C';"
        "document.getElementById('estado').textContent='Lectura: '+d.estadoReal+"
        "'; luz: '+d.senal+(d.modoPrueba?' (MODO PRUEBA)':' (MODO REAL)');"
        "}catch(e){document.getElementById('estado').textContent='Sin comunicacion';}}"
        "ver();setInterval(ver,2500)</script></body></html>");
    });
    servidor.on("/api", responderApi);
    servidor.begin();
    Serial.print("Pagina local: http://");
    Serial.println(WiFi.softAPIP());
    Serial.print("Clave temporal: ");
    Serial.println(claveLocal);
  } else Serial.println("Red local no disponible; luces autonomas activas.");
  Serial.println("ms,humedad_pct,temperatura_C,estado,modo");
}

void loop() {
  if (redLocal) servidor.handleClient();
  leerBoton();
  leerDht();
  actualizarLuces();
}
