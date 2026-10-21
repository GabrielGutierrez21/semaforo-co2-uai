# Semáforo de CO₂ para salas UAI

## Estado del primer modelo para Avance #2 (21-10-2026)

El equipo dispone físicamente de **NodeMCU ESP8266, OLED, módulo RGB, pulsador, Arduino Uno y USB**. El DEC confirmó que no quedan sensores disponibles. La implementación inmediata pasa a ser un **indicador de cambios de calidad del aire con CCS811** (TVOC y **eCO₂ estimado**), pantalla OLED, tres LED separados y página local opcional servida por la propia NodeMCU. **El CCS811 no mide CO₂ real; la medición NDIR permanece como etapa futura.**

- [Lista de compra, conexiones y demostración actual](docs/modelo_ccs811_avance_02.md).
- [Sketch para NodeMCU ESP8266 y CCS811](firmware/SemaforoAireCCS811/SemaforoAireCCS811.ino).
- Estado: el nuevo programa está preparado **sin compilación ni prueba física todavía**. Confirmar serigrafía y tensión del módulo comprado, soldaduras, controlador OLED y respuesta durante cinco minutos antes de afirmar funcionamiento.

La sección siguiente conserva el proyecto inicial con SCD40/SCD41 y ESP32 como referencia de diseño; sus conexiones, umbrales y carcasa no describen el primer modelo CCS811.

## Diseño original con sensor NDIR (etapa futura)

Prototipo alfa autónomo: un módulo SCD40 o SCD41 mide CO₂ real, el ESP32 enciende una de tres luces y, si se instala una pantalla compatible, se podrán ver los ppm en el propio aparato. Una página servida **por el propio ESP32** permite verificar la transmisión por Wi-Fi local durante la demostración. No necesita Internet, nube, aplicación instalada ni cuenta. La OLED y otras alternativas de sensor se documentan como **mejoras pendientes de integración**; el firmware actual usa SCD40/SCD41 y tres LED individuales.

> Estado de este paquete: firmware y carcasa generados; lógica comprobada por prueba automática; STL exportados. **No se ha probado el firmware en un ESP32 ni el encaje con las placas físicas.** La evidencia presencial debe producirla el equipo.

**Contingencia Avance #2:** si no se consigue sensor CO₂ a tiempo, ver [plan B de humedad de aula](docs/plan_b_avance_02.md) y el sketch separado `firmware/SemaforoAulaDHT22/`. Esta variante **no mide CO₂**; requiere préstamo de DHT22 #651 y prueba física. No mezclar sus lecturas con el programa SCD40/SCD41.

## Orden de ejecución

1. Revisar [compras.md](docs/compras.md). Primero preguntar por préstamo de un sensor de CO₂ real en el DEC; su catálogo público no lista ninguno. Si se compra, elegir un **SCD40 o SCD41 montado en breakout I²C** y verificar chip, conectores y entrega; nunca el chip SMD suelto. MH-Z19C/E o STCC4 también miden CO₂, pero **requieren adaptar código y/o conexión antes de usarlos**. El MQ-7 del DEC mide CO, no CO₂.
2. En la mesa, cablear conforme a [conexiones.md](docs/conexiones.md) con USB desconectado. Verificar 3V3, GND y continuidad. Probar primero en protoboard.
3. Instalar Arduino IDE, paquete ESP32 y biblioteca **Sensirion I2C SCD4x** con sus dependencias. Abrir `firmware/SemaforoCO2/SemaforoCO2.ino`, elegir placa y puerto, cargar y abrir monitor serie a 115200 baudios.
4. Esperar la primera lectura (modo periódico, aproximadamente cada 5 s). Verificar CO₂ en ppm, temperatura y humedad; una cifra fija, 0 ppm o ausencia de datos señala un problema de montaje o sensor.
5. Si desean comprobar la página opcional, ver en el monitor serie la **clave temporal generada al encender**, conectar un teléfono a `Semaforo-CO2-UAI` y abrir `http://192.168.4.1`. La clave cambia al reiniciar. El dispositivo y sus luces funcionan sin teléfono ni Wi-Fi externo.
6. Verificar el pulsador: cada pulsación cambia **REAL → PRUEBA VERDE → PRUEBA AMARILLO → PRUEBA ROJO → REAL**. En prueba se simulan solo las luces; la página sigue mostrando ppm reales y etiqueta claramente el modo.
7. Medir las placas reales. Abrir `cad/carcasa.scad` en OpenSCAD; ajustar USB, orificios LED, ranuras y, si se instala OLED, diseñar su ventana tras medir el módulo. Cada pieza conceptual actual mide **96 × 76 mm** de planta y cabe en el volumen **sugerido** por DEC de 100 × 100 × 100 mm. Imprimir base/tapa solo tras verificar el encaje y el laminador con el DEC. Los STL adjuntos siguen siendo **conceptuales**.
8. Guardar tres ensayos de cinco minutos siguiendo [pruebas_y_demo.md](docs/pruebas_y_demo.md), fotografías, lecturas y decisiones de mejora. En una sala ocupada, documentar comparación antes/después de ventilar; no asegurar un cambio de estado que no ocurrió.
9. Registrar cada cambio y sus pruebas en este repositorio siguiendo [github.md](docs/github.md). Incluir fotos y registros **reales** solo después de obtenerlos. Mostrar el repositorio en la demo.

## Árbol de archivos

- `firmware/SemaforoCO2/`: programa ESP32 y lógica de umbrales.
- `cad/carcasa.scad`: modelo 3D paramétrico y STL conceptuales.
- `docs/compras.md`: materiales, cantidades y alternativas.
- `docs/conexiones.md`: cableado y montaje.
- `docs/dec.md`: catálogo, solicitud de componentes y fabricación en el DEC.
- `docs/pruebas_y_demo.md`: registro de iteración y guion.
- `docs/github.md`: repositorio y entregables.
- `tests/test_logic.cpp`: comprobación de límites e histéresis.

## Comportamiento

Inicio: amarillo intermitente. Fallo o falta de lectura durante más de 20 s: rojo y amarillo intermitentes; nunca se anuncia aire normal sin datos. Primera lectura: verde <700 ppm, amarillo 700–1300 ppm inclusive, rojo >1300 ppm. Luego se aplica una histéresis de 50 ppm. Son umbrales del **prototipo para recomendar ventilación**, no límites de daño sanitario ni una medición general de todos los contaminantes.

## Referencias técnicas y de evaluación

- Guía Avance 02 TEI201 entregada por el curso: funcionamiento, conectividad, modelo 3D, iteración y documentación. El PDF original no se redistribuye dentro de este repositorio.
- [Sensirion SCD4x y controlador Arduino](https://developer.sensirion.com/product-support/scd4x-co2-sensor).
- [Guía de cableado y librería SCD40 de Adafruit](https://learn.adafruit.com/adafruit-scd-40-and-scd-41/arduino).
- [Wi-Fi AP del ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html).
- [CDC sobre interpretar CO₂ como indicador contextual de ventilación](https://www.cdc.gov/niosh/ventilation/faq/index.html).
