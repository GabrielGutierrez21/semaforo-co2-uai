# Iteraciones del prototipo

| Etapa | Evidencia y cambio |
|---|---|
| Primer montaje | [`firmware/historico/SemaforoLED/SemaforoLED.ino`](../firmware/historico/SemaforoLED/SemaforoLED.ino): ESP8266 + CCS811 + LED + botón; sin pantalla. |
| Lectura local | [`firmware/SemaforoAmbiental/`](../firmware/SemaforoAmbiental/): OLED numérica y señal visual; D1/D2, D4 y D5/D6/D7 según el montaje real. |
| Web de celular | Misma carpeta de firmware: punto de acceso Wi‑Fi local, API, caritas en el navegador e historial CSV en LittleFS. La OLED mantiene su presentación numérica. |

Las fotografías del hardware están en [`hardware/fotos/`](../hardware/fotos/). El protocolo para documentar una prueba cuantitativa y reproducible está en [`testing/`](../testing/).
