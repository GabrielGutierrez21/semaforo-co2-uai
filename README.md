# Semáforo ambiental para una sala

Prototipo local con **NodeMCU ESP8266**, sensor **CCS811**, tres LED, pulsador y pantalla **OLED I²C 128 × 64**. Muestra TVOC (ppb), eCO₂ **estimado** (ppm) y la variación de TVOC respecto de una referencia inicial. El CCS811 **no mide la concentración real de CO₂**, por lo que el equipo no debe presentarlo como medidor de CO₂ ni usar eCO₂ para certificar ventilación.

Equipo: Gabriel Gutiérrez, Hai Hu, Bastián Cabrera, Lionel Bravo y Samuel Acevedo.

## Estado al 19 de noviembre de 2026

El firmware local y la carcasa conceptual están disponibles. Se conserva una fotografía del montaje de LED en protoboard y otra de componentes. **La web, la persistencia de datos, el gemelo 3D en Fusion 360, los planos y las mediciones de validación todavía no están implementados o documentados**. La [pauta EV3](docs/ev3_estado.md) distingue los entregables presentes de los pendientes. No se simulan resultados ni se afirma que una pantalla conceptual sea una prueba de funcionamiento.

## Rutas del repositorio

| Carpeta o archivo | Contenido |
|---|---|
| [`firmware/SemaforoAmbiental/`](firmware/SemaforoAmbiental/) | Programa vigente ESP8266 + CCS811 + OLED numérica + LED + botón. |
| [`firmware/historico/`](firmware/historico/) | Primera versión sin OLED, conservada para mostrar la iteración. No cargar en la demo EV3. |
| [`hardware/`](hardware/) | Conexiones, lista de materiales, diagrama y fotografías disponibles. |
| [`diseno-3d/`](diseno-3d/) | Fuente OpenSCAD y STL de base y tapa, con medidas y limitaciones. |
| [`testing/`](testing/) | Protocolo reproducible y criterios de éxito. No contiene resultados inventados. |
| [`docs/`](docs/) | Estado frente a EV3 y registro del avance anterior. |
| [`FUENTES.md`](FUENTES.md) | Librerías, referencias, código externo y asistencia de IA. |

## Montaje y ejecución

1. Reunir los componentes de [`hardware/BOM.csv`](hardware/BOM.csv) y cablear según [`hardware/conexiones.md`](hardware/conexiones.md) y [`hardware/esquematico.pdf`](hardware/esquematico.pdf). Alimentar los módulos con **3,3 V**, verificar GND común y el voltaje admitido por el breakout CCS811 adquirido.
2. En Arduino IDE 2.x instalar el núcleo [ESP8266 Arduino](https://github.com/esp8266/Arduino) y las bibliotecas detalladas en [`FUENTES.md`](FUENTES.md). Seleccionar **NodeMCU 1.0 (ESP-12E Module)** y el puerto serie correspondiente.
3. Abrir [`firmware/SemaforoAmbiental/SemaforoAmbiental.ino`](firmware/SemaforoAmbiental/SemaforoAmbiental.ino), compilar y cargar. Soltar el botón conectado a D4 durante el reinicio, pues ese pin influye en el arranque.
4. Abrir Monitor Serie a **115200 baudios**. La OLED muestra lecturas y estado; los LED marcan cambio relativo de TVOC. El botón recorre los LED **solo como prueba eléctrica** mientras se mantiene presionado.
5. Dejar el CCS811 en funcionamiento **20 minutos** antes de interpretar las variaciones y esperar que el equipo capture sus **30 muestras de referencia**. En la primera puesta en uso, seguir además el acondicionamiento inicial indicado por el fabricante. Registrar la prueba según [`testing/protocolo.md`](testing/protocolo.md).

La pantalla **no dibuja caritas** en el programa vigente. El dispositivo funciona sin internet. Para EV3, una futura **web local servida por el ESP8266**, accesible desde su punto de acceso Wi-Fi y con almacenamiento persistente, sería el siguiente desarrollo; esta versión aún no contiene esa función.

## Alcance técnico

Verde: TVOC menor que referencia + 50 ppb; amarillo: +50 a +149 ppb; rojo: +150 ppb o más. Estos umbrales son **reglas exploratorias del prototipo**, no límites sanitarios ni equivalentes a ppm de CO₂. El programa avisa calentamiento, error y ausencia de sensor por separado. La frecuencia de visualización es 0,5 s; la de reporte serie, 1 s; las lecturas nuevas dependen de `ccs.available()`.

El modelo físico publicado mide nominalmente **100 × 80 × 33,5 mm**, compuesto por base de 31 mm y tapa de 2,5 mm; antes de imprimir hay que contrastar el tamaño real de la electrónica y sus conectores. Las imágenes de producto generadas previamente son conceptuales y no sustituyen fotos, planos ni un archivo `.f3d`.
