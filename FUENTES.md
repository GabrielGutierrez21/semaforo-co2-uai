# Fuentes y uso de herramientas de IA

Declaración del código y de la documentación disponibles en este repositorio al **19 de noviembre de 2026**. El archivo es parte de la entrega EV3 y debe actualizarse cuando se añadan la web, nuevas librerías, archivos Fusion o mediciones.

## Librerías y dependencias del firmware vigente

Las versiones siguientes son **versiones de referencia para reproducir la compilación**, verificadas en los repositorios públicos; no hay registro del entorno exacto usado por el equipo al cargar la placa. Arduino IDE instala también dependencias transitivas.

| Paquete | Versión de referencia | Uso | Fuente |
|---|---:|---|---|
| ESP8266 Arduino core | 3.1.2 | Pines `D1`–`D7`, Wire, Serie y ejecución en NodeMCU | https://github.com/esp8266/Arduino/releases/tag/3.1.2 |
| Adafruit CCS811 Library | 1.1.3 | Inicialización y lecturas TVOC/eCO₂ del CCS811 | https://github.com/adafruit/Adafruit_CCS811 |
| Adafruit SSD1306 | 2.5.17 | Controlador OLED I²C 128 × 64 | https://github.com/adafruit/Adafruit_SSD1306/releases |
| Adafruit GFX Library | 1.12.6 | Texto y gráficos de la pantalla | https://github.com/adafruit/Adafruit-GFX-Library/releases |
| Adafruit BusIO | 1.17.4 | Dependencia de bibliotecas Adafruit para I²C | https://github.com/adafruit/Adafruit_BusIO/releases |

`Arduino.h` y `Wire.h` provienen del núcleo ESP8266. No se ha incluido código de terceros copiado literalmente en el repositorio según el trabajo disponible para esta revisión. Si el equipo incorpora fragmentos externos al hacer la web, debe agregar aquí su URL, función y adaptación antes de la entrega.

## Referencias técnicas y pauta

- [Guía Adafruit CCS811](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview): distingue TVOC y eCO₂ equivalente, y recoge el acondicionamiento inicial y el periodo de 20 minutos tras encender.
- [Hoja de datos CCS811 alojada por Adafruit](https://cdn-shop.adafruit.com/product-files/3566/3566_datasheet.pdf): funcionamiento del sensor y condición de sus salidas estimadas.
- Documento de evaluación **Avance #3: Prototipo IoT Completo**, TEI201 DEC UAI, aportado por el equipo en esta conversación (19 de noviembre de 2026).

## Uso de inteligencia artificial y adaptación

Se empleó **ChatGPT de OpenAI** en 2026 para proponer y revisar el firmware ESP8266 + CCS811, las funciones de pantalla OLED, la estructura de carpetas, las guías de montaje, el diagrama de conexiones y el protocolo de pruebas. El encargo incluyó adaptar los pines reales **D1/D2 (I²C), D4 (botón), D5/D6/D7 (LED)**, conectar WAKE a GND, quitar las caritas de la pantalla y documentar las exigencias de EV3. El código resultante se encuentra en [`firmware/SemaforoAmbiental/SemaforoAmbiental.ino`](firmware/SemaforoAmbiental/SemaforoAmbiental.ino).

Para explicar y defender lo generado, el equipo debe conocer `estado()` (clasifica calentamiento, referencia y diferencia de TVOC), `leerSensor()` (obtiene lecturas y calcula la referencia de 30 muestras), `senal()` (activa LED o prueba del pulsador), `actualizarPantalla()` (muestra números y estado) y `reportar()` (salida serie). **No se ha verificado físicamente en esta revisión** el programa modificado ni la compatibilidad exacta del breakout que posee el equipo; deben compilar, cargar y registrar una prueba real antes de presentarlo.

También se usaron imágenes conceptuales con IA en una presentación anterior. No son fotos del prototipo ni renders de un gemelo Fusion 360, y por esa razón no se incluyen como evidencia física de EV3.
