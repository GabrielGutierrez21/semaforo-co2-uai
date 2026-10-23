# Semáforo ambiental de aula

Prototipo autónomo de señal visible para cambios en la calidad del aire de una sala. Un **ESP8266** lee un **CCS811**, controla tres LED y muestra datos y caritas en una **OLED de 0,96″**. Funciona localmente, sin Internet ni aplicación. El equipo reportó el circuito de sensor y LED funcional.

**Alcance medido:** TVOC en ppb y eCO₂ **estimado** en ppm. El CCS811 no mide CO₂ real. Las caritas y los LED comunican cambios de TVOC respecto de una referencia tomada al encender; son umbrales demostrativos del prototipo.

## Recorrido del proyecto

| Carpeta | Contenido |
|---|---|
| [01 · Primer prototipo](01_primer_prototipo/) | ESP8266, CCS811, tres LED y botón; código y descripción del funcionamiento. |
| [02 · Prototipo final](02_prototipo_final/) | Código vigente con OLED y caritas; conexiones, bibliotecas y uso. |
| [03 · Modelado 3D](03_modelado_3d/) | Carcasa paramétrica, base y tapa en STL; medidas y proceso de ajuste. |
| [04 · Entregable 2](04_entregable_2/) | Mapa de evidencias, registro de pruebas y guion de demostración. |

**Código que deben cargar:** [NUEVO_ESP8266_CCS811_OLED.ino](02_prototipo_final/NUEVO_ESP8266_CCS811_OLED.ino). El primer prototipo queda disponible para mostrar la evolución. En la versión actual, SDA de sensor y OLED comparten D2; SCL comparte D1; LED verde/amarillo/rojo usan D5/D6/D7; botón D4; WAKE del CCS811 a GND.

## Estado del Entregable 2

El montaje electrónico y el programa se presentan con la evolución del prototipo y un modelo 3D editable. La carpeta de Entregable 2 indica las evidencias observables y deja espacios para incorporar fotografías, video y resultados medidos por el equipo. La carcasa 3D es un diseño para ajustar a las dimensiones del montaje físico antes de imprimir.

### Bibliotecas y fuentes

- Firmware con **Adafruit CCS811**, **Adafruit SSD1306**, **Adafruit GFX** y el núcleo **ESP8266 Arduino**.
- Bibliotecas oficiales: [CCS811](https://github.com/adafruit/Adafruit_CCS811), [SSD1306](https://github.com/adafruit/Adafruit_SSD1306), [GFX](https://github.com/adafruit/Adafruit-GFX-Library).
- Este material fue organizado y el código/modelo 3D se apoyaron en asistencia de IA; el equipo debe validar físicamente el montaje y sus mediciones.

