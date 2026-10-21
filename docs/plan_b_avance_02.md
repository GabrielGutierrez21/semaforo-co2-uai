# Plan B para Avance #2: humedad de una sala

**Cambio de alcance explícito:** esta variante mide **humedad relativa y temperatura**, no CO₂ ni calidad de ventilación. Es un prototipo parcial de monitoreo ambiental de aulas con señal local. No presentar sus valores como ppm de CO₂. Mantener el diseño SCD40/SCD41 original como objetivo posterior.

El [catálogo DEC](https://dec.uai.cl/componentes/) consultado el 21-10-2026 lista **#651 AM2302 (DHT22)** y, como alternativa de menor resolución, **#679 KY-015 (DHT11)**. El catálogo no confirma existencias: pedir prestado **#651** presencialmente y fotografiar su pinout. Si solo existe otro sensor, este programa no se debe cargar sin adaptarlo.

## Pedir al DEC

- ESP32 #787, DHT22/AM2302 #651, protoboard #802, tres LED independientes, tres resistencias 330 Ω, pulsador #726 y cables Dupont. Cable USB de datos compatible (#844/#845 si la placa es MicroUSB).
- **Una resistencia de 10 kΩ para DATA a 3V3** si el DHT22 es de cuatro pines sin pull-up incorporada; verificar el módulo concreto. El módulo de tres pines puede incluirla.
- No usar MQ-7 para afirmar CO₂. El sensor #662 MQ-135 tampoco mide CO₂ de forma selectiva. No conectar una salida de 5 V directamente a los GPIO del ESP32.

## Conexiones previstas (confirmar placa antes de energizar)

| ESP32 | Componente | Nota |
|---|---|---|
| 3V3, GND, GPIO23 | VCC, GND, DATA de DHT22 | Verificar disposición real; pull-up 10 kΩ de DATA a 3V3 si no está integrado. |
| GPIO25 | Resistencia 330 Ω → ánodo LED verde | Cátodo a GND. |
| GPIO26 | Resistencia 330 Ω → ánodo LED amarillo | Cátodo a GND. |
| GPIO27 | Resistencia 330 Ω → ánodo LED rojo | Cátodo a GND. |
| GPIO33 | Pulsador a GND | `INPUT_PULLUP`; confirmar contactos del módulo KY-004. |

## Programa y demostración

1. Instalar paquete ESP32 y las bibliotecas **DHT sensor library** y **Adafruit Unified Sensor** desde Arduino IDE. Abrir `firmware/SemaforoAulaDHT22/SemaforoAulaDHT22.ino` y cargarlo. Este sketch es independiente del de CO₂.
2. A 115200 baudios, verificar humedad %HR y temperatura °C aproximadamente cada 2,5 s. Error o dato ausente: luces roja y amarilla intermitentes. Los límites de color (40–60 % verde; 30–40/60–70 % amarillo; extremos rojo) son **rangos ilustrativos del prototipo** y no umbrales sanitarios.
3. La red `Semaforo-Aula-UAI` se crea por el ESP32; la clave temporal sale por Serial al encender. Teléfono opcional: abrir `http://192.168.4.1` y cotejar lectura real. No hay Internet ni servidor externo.
4. En la demo mostrar la lectura real y el LED correspondiente. El botón recorre colores **en MODO PRUEBA** para explicar la interfaz; no usarlo como evidencia de cambios reales de humedad. Registrar al menos tres ensayos completos de 5 min, lecturas, fallas y correcciones.
5. Mostrar el modelo 3D existente como **conceptual**: la abertura de sensor y los soportes deben rediseñarse para el DHT22 físico. No afirmar que la caja actual está validada para este sensor.

### Si el DEC no tiene #651

Preguntar por **#679 DHT11** (requiere cambiar `DHT22` por `DHT11` en el sketch) y confirmar su módulo. Si tampoco hay, el **#683 LDR+LM393** permite demostrar rápidamente lectura de luz y LEDs, pero eso cambia el problema a iluminación de aula y exige otro código/justificación. Sin sensor real, solo podrán demostrar electrónica y conectividad parcial: explicarlo al evaluador antes de la demo, sin inventar CO₂.

Fuente técnica de DHT22: [Adafruit: conexión](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor) y [bibliotecas Arduino](https://learn.adafruit.com/dht/using-a-dhtxx-sensor-with-arduino). La disponibilidad se confirma presencialmente.
