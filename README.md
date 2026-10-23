# Semáforo de aire: ESP8266 + CCS811 + OLED

**Versión actual del prototipo:** NodeMCU ESP8266, CCS811, OLED I²C 0,96″ 128×64 SSD1306, LED verde/amarillo/rojo y pulsador. El CCS811 entrega **TVOC y eCO₂ estimado**, no CO₂ medido directamente. El semáforo y las caritas representan cambios de TVOC desde la referencia local. No usa Wi-Fi ni aplicación.

- **[Código NUEVO con OLED](firmware/NUEVO_ESP8266_CCS811_OLED/NUEVO_ESP8266_CCS811_OLED.ino)**: versión que deben cargar para este montaje.
- **[Guía NUEVA de conexiones y prueba](docs/NUEVO_ESP8266_CCS811_OLED_ARMADO.md)**: cableado, bibliotecas y significado de la pantalla.

| Señal | ESP8266 |
|---|---|
| CCS811 SDA y OLED SDA | D2 |
| CCS811 SCL y OLED SCL | D1 |
| CCS811 WAKE | GND |
| CCS811 GND y OLED GND | GND |
| CCS811 y OLED alimentación | 3V3, si ambos módulos aceptan 3,3 V |
| LED verde, amarillo, rojo | D5, D6, D7 (cada uno con resistencia) |
| Pulsador | D4 a GND, solo al presionar |

**Antes de conectar USB:** comprobar que ningún cable de GND esté en a15 ni a23, filas usadas por D7/D6. La pantalla supone controlador SSD1306 y resolución 128×64; confirmarlo en el módulo. El tiempo de espera está en **1 minuto para prueba rápida** y debe volver a 20 minutos si se desean lecturas más estables. Sensor nuevo: el fabricante recomienda alrededor de 48 horas de uso inicial. El código aún no se ha compilado ni probado físicamente con esta OLED concreta.

La documentación y firmware SCD40 que puedan aparecer en el historial fueron una alternativa anterior; **no cargarlos con el CCS811**.

