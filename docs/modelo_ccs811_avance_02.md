# Primer modelo para el Avance #2: CCS811, ESP8266 y señal local

**Alcance:** el CCS811 detecta compuestos orgánicos volátiles totales (TVOC) y calcula un valor de CO₂ equivalente (**eCO₂**). **No mide CO₂ real ni permite certificar la ventilación de la sala.** La hipótesis demostrable es que el aparato registra cambios relativos en su entorno y avisa en el mismo dispositivo. El proyecto original con SCD40/SCD41 NDIR queda como siguiente etapa.

**Estado a 21-10-2026:** programa preparado y publicado; aún no compilado en Arduino IDE ni validado con las placas reales. El prototipo físico, su pinout y su respuesta de cinco minutos deben comprobarse en mesa. El modelo 3D previo es conceptual para otro montaje: medir antes de diseñar o imprimir la nueva caja.

## Piezas confirmadas por fotografías y compras pendientes

En las fotos se distingue Arduino Uno (respaldo), **NodeMCU ESP8266** (placa principal), OLED I²C de cuatro contactos (modelo/controlador y alto por confirmar), LED RGB, pulsador, cable USB A–microUSB y dos resistencias de valor no confirmado. No se ve CCS811, protoboard ni juego de cables Dupont. No contamos las resistencias fotografiadas como las tres de los LED.

| Comprar o pedir prestado | Cantidad | Observación |
|---|---:|---|
| [CCS811/CJMCU-811](https://afel.cl/products/sensor-monoxido-de-carbono-co-cjmcu-811) | 1 | Verificar chip, tensión admitida, pinout y pines de WAKE/ADDR en la placa concreta. La ficha de Afel mezcla CO con TVOC/eCO₂: **no presentarlo como medidor selectivo de CO ni CO₂**. Precio publicado consultado: oferta alrededor de $14.400, habitual $18.000; confirmar en caja. |
| [Kit de accesorios Arduino](https://afel.cl/products/kit-de-accesorios-de-iniciacion-para-arduino) | 1 | $5.000 publicados; protoboard de 400 puntos, LED separados rojo/amarillo/verde, resistencias de 220 Ω. |
| [Pack 60 Dupont](https://afel.cl/products/pack-60-cables-de-conexion) | 1 | $2.500 publicados; macho–macho, macho–hembra y hembra–hembra. |
| [Tira de pines rectos de 2,54 mm](https://afel.cl/products/regleta-macho-macho-pin-header) | 1, condicional | $250 publicados **solo si** la NodeMCU, OLED o sensor carecen de cabeceras. Requiere soldadura; consultar servicio o taller. |

**Total orientativo:** $7.750 si se presta el CCS811 y se compra la tira; $22.150 si se compra a $14.400. Hay fichas que muestran tanto «Agotado» como unidades disponibles: confirmar stock presencial. No comprar buzzer, otro microcontrolador ni pantalla nueva para este avance. El DHT22 sería una ampliación útil de temperatura/humedad y compensación del CCS811, no requisito de este primer montaje.

## Cableado previsto, con USB desconectado

Estas conexiones son para un **módulo CCS811 compatible con lógica de 3,3 V**. Verificar serigrafía y circuito del módulo recibido; el breakout de Adafruit y un CJMCU genérico no comparten necesariamente regulación o pinout. No conectar señales de 5 V a GPIO de la NodeMCU.

| NodeMCU ESP8266 | Módulo | Nota |
|---|---|---|
| 3V3 y GND | alimentación OLED y CCS811 | Tierra común; comprobar que el CCS811 concreto acepta 3,3 V en su pin rotulado VCC/VIN. |
| D2 / GPIO4 | SDA de OLED y CCS811 | Bus I²C compartido. |
| D1 / GPIO5 | SCL de OLED y CCS811 | Bus I²C compartido. |
| GND | WAKE del CCS811, **si existe y la hoja del módulo exige nivel bajo** | Dejar otros pines del sensor sin conectar por ahora; confirmar fotos. |
| D5 / GPIO14 | 220 Ω → ánodo LED verde | Cátodo a GND. |
| D6 / GPIO12 | 220 Ω → ánodo LED amarillo | Cátodo a GND. |
| D7 / GPIO13 | 220 Ω → ánodo LED rojo | Cátodo a GND. |

La OLED fotografiada muestra cuatro contactos sin cabecera visible; confirmar altura **128 × 64** o **128 × 32**, controlador SSD1306 y dirección **0x3C/0x3D**. El código supone 128 × 64 y detecta esas dos direcciones, pero no detecta la altura ni valida otro controlador. Si no coincide, adaptar `OLED_ALTO` y biblioteca. Si el sensor no responde, el programa intenta reconectar y la señal de error parpadea. El cable USB alimenta y programa la NodeMCU; el Arduino Uno no se necesita.

## Carga y prueba, en orden

1. Confirmar y soldar cabeceras si faltan; identificar todos los pines por la placa física. Montar sin tensión. **Probar primero CCS811 y NodeMCU**, luego tres LED, después OLED. Compartir SDA/SCL y GND; colocar resistencia individual en cada LED.
2. En Arduino IDE instalar paquete **ESP8266 Boards** y bibliotecas **Adafruit CCS811**, **Adafruit SSD1306**, **Adafruit GFX** y dependencias solicitadas por el gestor (incluida Adafruit BusIO). Elegir **NodeMCU 1.0 (ESP-12E Module)** y puerto correcto. Abrir `firmware/SemaforoAireCCS811/SemaforoAireCCS811.ino`.
3. Cargar y abrir Serial a **115200 baudios**. Verificar respuesta CCS811 (I²C **0x5A**), TVOC **ppb** y eCO₂ **ppm estimados**. Si falla, revisar alimentación/WAKE/pinout; no interpretar `400 ppm` inicial como una medición fiable de CO₂ real.
4. Encender al menos **20 min antes** de la demostración. Mientras estabiliza, LED amarillo intermitente y pantalla `ESTABILIZANDO`. Después toma 30 lecturas para fijar una referencia TVOC y usa diferencias **ilustrativas**: menor a +50 ppb verde, +50 a +149 ppb amarillo, +150 ppb o más rojo. Son valores de prototipo, **no límites sanitarios**; si una luz no cambia con el ambiente, mostrar la variación numérica y no inventar el cambio.
5. La NodeMCU crea la red `Semaforo-Aula-UAI`, clave de demostración `AulaUAI2026`. Opcionalmente, conectar teléfono y abrir `http://192.168.4.1`; la página es servida **localmente** por la placa, sin Internet ni aplicación externa. La pantalla y LED funcionan aunque no se conecte ningún teléfono. Cambiar la clave del sketch para uso fuera de la demo.
6. Registrar al menos una prueba continua de **5 min** con lectura, estado, LED, pantalla/página, errores y correcciones; idealmente repetirla tres veces. Fotografiar el montaje y explicar el cambio de alcance al evaluador. La comparación es relativa a la referencia adquirida en ese arranque; no presentar eCO₂ como indicador validado de ocupación o ventilación.

## Presentación honesta

> «Primer modelo autónomo de monitoreo relativo de calidad del aire de aula. El CCS811 registra TVOC y calcula eCO₂; la NodeMCU procesa las lecturas, muestra datos en la OLED y da una señal visible. La medición directa de CO₂ con NDIR es la siguiente iteración».

Fuentes: [ams OSRAM sobre TVOC/eCO₂](https://ams-osram.com/news/press-releases/new-environmental-sensor-kits-from-ams-enable-easy-prototyping-of-indoor-air-quality-monitoring-systems), [Adafruit CCS811, conexión y estabilización](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview), [ESP8266 Arduino NodeMCU](https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h).
