# Semáforo de CO₂ para salas UAI

Prototipo alfa autónomo: un SCD40 mide CO₂ real, el ESP32 enciende una de tres luces y, si se instala, emite un aviso breve. Una página servida **por el propio ESP32** permite verificar la transmisión por Wi-Fi local durante la demostración. No necesita Internet, nube, aplicación instalada ni cuenta.

> Estado de este paquete: firmware y carcasa generados; lógica comprobada por prueba automática; STL exportados. **No se ha probado el firmware en un ESP32 ni el encaje con las placas físicas.** La evidencia presencial debe producirla el equipo.

## Orden de ejecución

1. Revisar [compras.md](docs/compras.md) y comprar un **módulo/breakout SCD40 montado**, nunca el chip SMD suelto. Mantener el ESP32 que ya tienen. No hace falta HDC1080 para medir CO₂.
2. En la mesa, cablear conforme a [conexiones.md](docs/conexiones.md) con USB desconectado. Verificar 3V3, GND y continuidad. Probar primero en protoboard.
3. Instalar Arduino IDE, paquete ESP32 y biblioteca **Sensirion I2C SCD4x** con sus dependencias. Abrir `firmware/SemaforoCO2/SemaforoCO2.ino`, elegir placa y puerto, cargar y abrir monitor serie a 115200 baudios.
4. Esperar la primera lectura (modo periódico, aproximadamente cada 5 s). Verificar CO₂ en ppm, temperatura y humedad; una cifra fija, 0 ppm o ausencia de datos señala un problema de montaje o sensor.
5. Conectar un teléfono a `Semaforo-CO2-UAI`, clave `AulaCO2-2026`; abrir `http://192.168.4.1`. El valor de ppm debe cambiar con las lecturas del monitor serie. El dispositivo funciona aun sin teléfono.
6. Verificar el pulsador: cada pulsación cambia **REAL → PRUEBA VERDE → PRUEBA AMARILLO → PRUEBA ROJO → REAL**. En prueba se simulan solo las luces; la página sigue mostrando ppm reales y etiqueta claramente el modo.
7. Medir las placas reales. Abrir `cad/carcasa.scad` en OpenSCAD; ajustar `ancho`, `alto`, USB, orificios LED y ranuras. Imprimir base/tapa después de revisar el archivo en el laminador con personal del DEC. Los STL adjuntos son únicamente una **versión conceptual**.
8. Guardar tres ensayos de cinco minutos siguiendo [pruebas_y_demo.md](docs/pruebas_y_demo.md), fotografías, lecturas y decisiones de mejora. En una sala ocupada, documentar comparación antes/después de ventilar; no asegurar un cambio de estado que no ocurrió.
9. Registrar cada cambio y sus pruebas en este repositorio privado siguiendo [github.md](docs/github.md). Incluir fotos y registros **reales** solo después de obtenerlos. Mostrar el repositorio en la demo.

## Árbol de archivos

- `firmware/SemaforoCO2/`: programa ESP32 y lógica de umbrales.
- `cad/carcasa.scad`: modelo 3D paramétrico y STL conceptuales.
- `docs/compras.md`: materiales, cantidades y alternativas.
- `docs/conexiones.md`: cableado y montaje.
- `docs/dec.md`: tareas para la visita al DEC.
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
