# NUEVO — Semáforo de CO₂ con ESP8266 y SCD40

**Montaje solicitado:** NodeMCU ESP8266, sensor SCD40 I²C, tres LED individuales con resistencias y pulsador. Sin Wi‑Fi ni frontend. El SCD40 mide **CO₂ real en ppm**; un sensor marcado **CCS811 no sirve con este firmware** y solo produce eCO₂ estimado. Confirmar la etiqueta del sensor físico.

- [Guía NUEVO_ESP8266_SCD40_ARMADO.md](docs/NUEVO_ESP8266_SCD40_ARMADO.md): conexiones por pin y columna, revisión de posibles cortos, carga y prueba.
- [Programa NUEVO_ESP8266_SCD40.ino](firmware/NUEVO_ESP8266_SCD40/NUEVO_ESP8266_SCD40.ino): único firmware vigente para la configuración **SCD40**.

| Señal | Pin ESP8266 |
|---|---|
| SCD40 SDA / SCL | D2 / D1 |
| LED verde / amarillo / rojo | D5 / D6 / D7 |
| Pulsador | D4 a GND, solo al presionar |

**Antes de conectar USB:** si los cables grises de tierra están realmente en **a15 y a23**, comparten columnas con D7 y D6 y producen cortos. Deben ir a las columnas de los cátodos **12, 20 y 25**, según la guía. Revisar con multímetro. La imagen de referencia rotula D3 para el pulsador; la conexión comunicada usa **D4**.

**Semáforo:** verde <700 ppm; amarillo 700–1300 ppm; rojo >1300 ppm. Rojo intermitente = sensor ausente/falla. El botón prueba los tres LED después del arranque. Estos valores son umbrales del prototipo.

**Estado:** código preparado a partir del montaje descrito y de la API oficial de Sensirion; pendiente de compilar en Arduino IDE y probar con el **SCD40 físico** y la placa conectada. Una foto del chip y su serigrafía permitiría validar la alimentación y el orden exacto de pines.

