# Prototipo final — semáforo de aire con pantalla

**Componentes:** NodeMCU ESP8266, CCS811, OLED I²C 0,96″ 128×64 SSD1306, LED verde/amarillo/rojo, resistencias y pulsador. Toda la electrónica cabe en una protoboard. No necesita Internet ni aplicación.

La pantalla presenta TVOC en ppb, eCO₂ **estimado** en ppm, estado y una carita. Verde/feliz: cambio TVOC menor que +50 ppb respecto de la referencia; amarillo/inconforme: +50 a +149 ppb; rojo/triste: ≥+150 ppb. En calentamiento o toma de referencia aparece cara neutra. Un sensor sin datos muestra ojos en X y LED rojo intermitente. El CCS811 **no mide CO₂ real**.

| Componente | ESP8266 |
|---|---|
| CCS811 SDA y OLED SDA | D2 |
| CCS811 SCL y OLED SCL | D1 |
| CCS811 WAKE | GND |
| CCS811 VCC y OLED VCC | 3V3 |
| CCS811 GND y OLED GND | GND |
| LED verde, amarillo, rojo | D5, D6, D7; resistencia individual |
| Pulsador | D4 a GND al presionar |

1. Instalar soporte **ESP8266 Boards** y bibliotecas **Adafruit CCS811**, **Adafruit SSD1306**, **Adafruit GFX** y sus dependencias en Arduino IDE.
2. Abrir y cargar [NUEVO_ESP8266_CCS811_OLED.ino](NUEVO_ESP8266_CCS811_OLED.ino) seleccionando **NodeMCU 1.0 (ESP-12E Module)**.
3. Verificar la lectura, estado y carita en la OLED; el monitor serie usa 115200 baudios.
4. La espera del programa está en **1 minuto para la demo** y luego toma 30 lecturas para referencia. Para mayor estabilidad, cambiar `CALENTAMIENTO_MS` a `20UL * 60UL * 1000UL` y encender con anticipación.

**Validación del equipo:** el equipo informó que el circuito CCS811 + LED quedó funcional. Adjuntar en esta carpeta fotografías o video reales de la versión con OLED después de probarla; no se atribuyen pruebas no realizadas.
