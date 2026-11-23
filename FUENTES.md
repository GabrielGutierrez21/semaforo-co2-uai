# Fuentes y uso de herramientas de IA

Declaración del código y la documentación del proyecto. Actualizado el **22 de noviembre de 2026** con servidor web local e historial LittleFS. Añadir aquí cualquier fuente externa que se incorpore posteriormente.

## Librerías y dependencias del firmware vigente

Las versiones siguientes son **versiones de referencia para reproducir la compilación**, verificadas en los repositorios públicos; no hay registro del entorno exacto usado por el equipo al cargar la placa. Arduino IDE instala también dependencias transitivas.

| Paquete | Versión de referencia | Uso | Fuente |
|---|---:|---|---|
| ESP8266 Arduino core | 3.1.2 | Pines `D1`–`D7`, Wire, Serie y ejecución en NodeMCU | https://github.com/esp8266/Arduino/releases/tag/3.1.2 |
| Adafruit CCS811 Library | 1.1.3 | Inicialización y lecturas TVOC/eCO₂ del CCS811 | https://github.com/adafruit/Adafruit_CCS811 |
| Adafruit SSD1306 | 2.5.17 | Controlador OLED I²C 128 × 64 | https://github.com/adafruit/Adafruit_SSD1306/releases |
| Adafruit GFX Library | 1.12.6 | Texto y gráficos de la pantalla | https://github.com/adafruit/Adafruit-GFX-Library/releases |
| Adafruit BusIO | 1.17.4 | Dependencia de bibliotecas Adafruit para I²C | https://github.com/adafruit/Adafruit_BusIO/releases |

`Arduino.h` y `Wire.h` provienen del núcleo ESP8266. No se ha incluido código de terceros copiado literalmente en el repositorio según el trabajo disponible para esta revisión. La web local está escrita para este proyecto y no carga bibliotecas externas ni servicios de terceros.

## Referencias técnicas y pauta

- [Guía Adafruit CCS811](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview): distingue TVOC y eCO₂ equivalente, y recoge el acondicionamiento inicial y el periodo de 20 minutos tras encender.
- [Hoja de datos CCS811 alojada por Adafruit](https://cdn-shop.adafruit.com/product-files/3566/3566_datasheet.pdf): funcionamiento del sensor y condición de sus salidas estimadas.
- [Espressif, hoja de datos ESP8266EX](https://documentation.espressif.com/0a-esp8266ex_datasheet_en.html) y [recomendaciones de alimentación](https://docs.espressif.com/projects/esp-techpedia/en/latest/esp-friends/get-started/try-firmware/try-firmware-hardware/esp8266.html): voltaje del chip y margen de corriente de la fuente para Wi‑Fi.
- Documento de evaluación **Avance #3: Prototipo IoT Completo**, TEI201 DEC UAI, aportado por el equipo en esta conversación (19 de noviembre de 2026).

El archivo [`diseno-3d/SEMAFORO_AMBIENTAL.zip`](diseno-3d/SEMAFORO_AMBIENTAL.zip) fue aportado por el equipo. Los STEP, STL, PDF y script Fusion de [`diseno-3d/`](diseno-3d/) son su contenido extraído y organizado; la carcasa anterior está conservada en [`iteraciones/diseno-3d-inicial/`](iteraciones/diseno-3d-inicial/).

## Uso de inteligencia artificial y adaptación

Se empleó **ChatGPT de OpenAI** en 2026 para proponer y revisar el firmware ESP8266 + CCS811, las funciones de pantalla OLED, la estructura de carpetas, las guías de montaje, el diagrama de conexiones y el protocolo de pruebas. El encargo incluyó adaptar los pines reales **D1/D2 (I²C), D4 (botón), D5/D6/D7 (LED)**, conectar WAKE a GND, quitar las caritas de la pantalla y documentar las exigencias de EV3. El código resultante se encuentra en [`firmware/SemaforoAmbiental/SemaforoAmbiental.ino`](firmware/SemaforoAmbiental/SemaforoAmbiental.ino).

Para explicar y defender lo generado, el equipo debe conocer `estado()` (clasifica calentamiento, referencia y diferencia de TVOC), `leerSensor()` (obtiene lecturas y calcula la referencia de 30 muestras), `senal()` (activa LED o prueba del pulsador), `actualizarPantalla()` (muestra números y estado) y `reportar()` (salida serie). **No se ha verificado físicamente en esta revisión** el programa modificado ni la compatibilidad exacta del breakout que posee el equipo; deben compilar, cargar y registrar una prueba real antes de presentarlo.

También se usaron imágenes conceptuales con IA en una presentación anterior. No son fotos del prototipo ni renders de un gemelo Fusion 360, y por esa razón no se incluyen como evidencia física de EV3.

## Fuentes de la web local y textos informativos

- [ESP8266 Arduino Core, documentación de LittleFS](https://arduino-esp8266.readthedocs.io/en/stable/filesystem.html): montaje, escritura y rotación de archivos locales.
- [ESP8266WebServer, API del núcleo ESP8266](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h): respuesta HTTP y página HTML en flash.
- [Adafruit CCS811, guía de conexión y uso](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/arduino-wiring-test): salidas TVOC y eCO₂ estimado, acondicionamiento inicial y de cada sesión.
- [EPA, compuestos orgánicos volátiles en interiores](https://www.epa.gov/indoor-air-quality-iaq/volatile-organic-compounds-impact-indoor-air-quality): variedad de VOC y efectos que dependen del compuesto y de la exposición. Texto resumido y adaptado en las tarjetas de la web.

La interfaz HTML/CSS/JavaScript y las funciones de punto de acceso, API y registro CSV fueron generadas y revisadas con asistencia de ChatGPT para este proyecto. No se copiaron plantillas web externas ni se emplearon datos experimentales sintéticos. Las pruebas físicas y los resultados deben ser aportados por el equipo.
