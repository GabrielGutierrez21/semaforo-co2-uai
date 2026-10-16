#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SensirionI2cScd4x.h>
#include "logic.h"

// ESP32 DevKit clásico. Confirmar la serigrafía antes de conectar.
constexpr uint8_t PIN_SDA = 19;
constexpr uint8_t PIN_SCL = 18;
constexpr uint8_t PIN_VERDE = 25;
constexpr uint8_t PIN_AMARILLO = 26;
constexpr uint8_t PIN_ROJO = 27;
constexpr uint8_t PIN_BUZZER = 32; // Base de transistor NPN, nunca buzzer directo de potencia.
constexpr uint8_t PIN_PRUEBA = 33; // Pulsador a GND; INPUT_PULLUP.
constexpr bool USAR_BUZZER = true; // false si se omite el buzzer.

// Red local: el ESP32 funciona sin Internet y sin servidor externo.
const char* SSID_LOCAL = "Semaforo-CO2-UAI";
const char* CLAVE_LOCAL = "AulaCO2-2026"; // Cambiar antes de una demo pública.

SensirionI2cScd4x sensor;
WebServer servidor(80);
Estado estadoReal = Estado::INICIANDO;
Estado ultimoEstadoSenal = Estado::INICIANDO;
uint16_t ppm = 0;
float temperatura = 0;
float humedad = 0;
bool huboLectura = false;
bool sensorIniciado = false;
bool redLocal = false;
uint8_t pasoPrueba = 0; // 0=real; 1=verde; 2=amarillo; 3=rojo.
uint32_t ultimoDatoMs = 0;
uint32_t ultimoPollMs = 0;
uint32_t ultimoIntentoMs = 0;
uint32_t inicioRojoMs = 0;
uint32_t ultimoCambioBotonMs = 0;
bool botonAnterior = HIGH;

const char PAGINA[] PROGMEM = R"HTML(
<!doctype html><html lang="es"><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Semáforo CO₂ UAI</title>
<style>body{font:18px system-ui;margin:2rem;max-width:34rem;background:#f5f7f8;color:#15202a}
.tarjeta{background:white;padding:1.5rem;border-radius:16px;box-shadow:0 3px 14px #0002}
.valor{font-size:3.2rem;font-weight:700}.estado{font-size:1.4rem;font-weight:700}
.nota{font-size:.9rem;color:#52606d}#punto{font-size:3rem}</style>
<div class="tarjeta"><h1>Semáforo de CO₂</h1>
<div><span id="punto">●</span> <span id="estado" class="estado">Iniciando</span></div>
<div class="valor"><span id="ppm">--</span> <small>ppm</small></div>
<p id="modo"></p><p id="otros" class="nota"></p>
<p class="nota">Datos transmitidos directamente por el ESP32. Red local, sin Internet.</p></div>
<script>
async function actualizar(){try{let r=await fetch('/api',{cache:'no-store'});let d=await r.json();
document.getElementById('ppm').textContent=d.ppm===null?'--':d.ppm;
document.getElementById('estado').textContent=d.senal;
document.getElementById('modo').textContent=d.prueba?'MODO PRUEBA: luces simuladas; ppm real indicado arriba.':'MODO REAL: luces según lectura de CO₂.';
document.getElementById('otros').textContent='Temperatura: '+(d.temperatura===null?'--':d.temperatura)+' °C · Humedad: '+(d.humedad===null?'--':d.humedad)+' % · Lectura: '+d.real;
document.getElementById('punto').style.color=({VERDE:'#15803d',AMARILLO:'#eab308',ROJO:'#dc2626'}[d.senal]||'#64748b');
}catch(e){document.getElementById('estado').textContent='Sin comunicación';}}
actualizar();setInterval(actualizar,2000);
</script></html>
)HTML";

Estado estadoSenal() {
  if (pasoPrueba == 1) return Estado::VERDE;
  if (pasoPrueba == 2) return Estado::AMARILLO;
  if (pasoPrueba == 3) return Estado::ROJO;
  return estadoReal;
}

void responderApi() {
  // El modo prueba nunca modifica ni inventa los ppm reales.
  bool datoVigente = huboLectura && (uint32_t)(millis() - ultimoDatoMs) <= 20000;
  String json = "{\"ppm\":";
  json += datoVigente ? String(ppm) : "null";
  json += ",\"temperatura\":";
  json += datoVigente ? String(temperatura, 1) : "null";
  json += ",\"humedad\":";
  json += datoVigente ? String(humedad, 1) : "null";
  json += ",\"real\":\"";
  json += nombreEstado(estadoReal);
  json += "\",\"senal\":\"";
  json += nombreEstado(estadoSenal());
  json += "\",\"prueba\":";
  json += pasoPrueba ? "true}" : "false}";
  servidor.send(200, "application/json; charset=utf-8", json);
}

void iniciarSensor() {
  ultimoIntentoMs = millis();
  // Al reiniciar solamente el ESP32, el SCD40 puede seguir midiendo.
  sensor.stopPeriodicMeasurement();
  delay(500);
  int16_t error = sensor.startPeriodicMeasurement();
  sensorIniciado = (error == 0);
  if (!sensorIniciado) Serial.printf("No se pudo iniciar SCD40: %d\n", error);
}

void leerSensor() {
  uint32_t ahora = millis();
  if ((uint32_t)(ahora - ultimoPollMs) < 1000) return;
  ultimoPollMs = ahora;
  if (!sensorIniciado) {
    if ((uint32_t)(ahora - ultimoIntentoMs) >= 10000) iniciarSensor();
    return;
  }
  bool listo = false;
  int16_t error = sensor.getDataReadyStatus(listo);
  if (error != 0) {
    Serial.printf("Error I2C SCD40: %d\n", error);
    sensorIniciado = false;
    ultimoIntentoMs = ahora;
    return;
  }
  if (!listo) return;
  uint16_t nuevoPpm = 0;
  float nuevaTemperatura = 0, nuevaHumedad = 0;
  error = sensor.readMeasurement(nuevoPpm, nuevaTemperatura, nuevaHumedad);
  if (error != 0 || nuevoPpm == 0) {
    Serial.printf("Lectura invalida SCD40: %d\n", error);
    return;
  }
  ppm = nuevoPpm;
  temperatura = nuevaTemperatura;
  humedad = nuevaHumedad;
  huboLectura = true;
  ultimoDatoMs = ahora;
  estadoReal = clasificar(estadoReal, ppm);
  Serial.printf("%lu,%u,%.1f,%.1f,%s,%s\n", (unsigned long)ahora,
                ppm, temperatura, humedad, nombreEstado(estadoReal),
                pasoPrueba ? "PRUEBA" : "REAL");
}

void leerPulsador() {
  bool boton = digitalRead(PIN_PRUEBA);
  uint32_t ahora = millis();
  if (botonAnterior == HIGH && boton == LOW &&
      (uint32_t)(ahora - ultimoCambioBotonMs) > 200) {
    pasoPrueba = (pasoPrueba + 1) % 4;
    ultimoCambioBotonMs = ahora;
    Serial.printf("Modo: %s (%u)\n", pasoPrueba ? "PRUEBA" : "REAL", pasoPrueba);
  }
  botonAnterior = boton;
}

void actualizarSenal() {
  uint32_t ahora = millis();
  if (!huboLectura || (uint32_t)(ahora - ultimoDatoMs) > 20000) {
    estadoReal = huboLectura ? Estado::ERROR : (sensorIniciado ? Estado::INICIANDO : Estado::ERROR);
    // Si el sensor dejó de producir datos, intentar reiniciar su modo periódico.
    if (sensorIniciado && (uint32_t)(ahora - ultimoIntentoMs) > 20000 &&
        (!huboLectura || (uint32_t)(ahora - ultimoDatoMs) > 20000)) {
      sensorIniciado = false;
      ultimoIntentoMs = ahora;
    }
  }
  Estado senal = estadoSenal();
  if (senal != ultimoEstadoSenal) {
    if (senal == Estado::ROJO) inicioRojoMs = ahora;
    ultimoEstadoSenal = senal;
  }
  bool verde = senal == Estado::VERDE;
  bool amarillo = senal == Estado::AMARILLO;
  bool rojo = senal == Estado::ROJO;
  if (senal == Estado::INICIANDO) amarillo = (ahora / 500) % 2;
  if (senal == Estado::ERROR) rojo = amarillo = (ahora / 500) % 2;
  digitalWrite(PIN_VERDE, verde);
  digitalWrite(PIN_AMARILLO, amarillo);
  digitalWrite(PIN_ROJO, rojo);
  bool sonido = USAR_BUZZER && senal == Estado::ROJO &&
                 (uint32_t)(ahora - inicioRojoMs) >= 10000 &&
                 ((ahora - inicioRojoMs - 10000) % 15000) < 200;
  digitalWrite(PIN_BUZZER, sonido);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PRUEBA, INPUT_PULLUP);
  digitalWrite(PIN_BUZZER, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  sensor.begin(Wire, 0x62); // SCD40/SCD41: misma dirección I2C.
  iniciarSensor();
  WiFi.mode(WIFI_AP);
  redLocal = WiFi.softAP(SSID_LOCAL, CLAVE_LOCAL);
  if (redLocal) {
    servidor.on("/", []() { servidor.send_P(200, "text/html; charset=utf-8", PAGINA); });
    servidor.on("/api", responderApi);
    servidor.begin();
    Serial.print("Pagina local: http://");
    Serial.println(WiFi.softAPIP());
  } else Serial.println("Red local no disponible; semaforo autonomo activo.");
  Serial.println("ms,ppm,temperatura_C,humedad_pct,estado,modo");
}

void loop() {
  if (redLocal) servidor.handleClient();
  leerPulsador();
  leerSensor();
  actualizarSenal();
}
