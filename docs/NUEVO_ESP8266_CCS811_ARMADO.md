# NUEVO — Armado real ESP8266 + CCS811 + OLED + módulo RGB

## 1. Inventario comprobado

| Material | Estado | Función |
|---|---|---|
| NodeMCU **ESP8266** | Visible en fotos anteriores; placa principal | Lee sensor y controla la señal. Sus bordes parecen sin pines soldados: comprobar. |
| CCS811 | **Compra confirmada en la captura nueva**, sin foto del módulo físico | TVOC y eCO₂ calculado. Confirmar por foto chip, pines y alimentación antes de energizar. |
| OLED I²C de cuatro contactos | Visible, sin cabecera en la foto | Mostrar TVOC, eCO₂ estimado y estado. Controlador SSD1306 y altura 128×64 aún por verificar. |
| Módulo RGB de cuatro terminales `R G B GND` | Visible | Un solo foco: rojo, verde y amarillo mezclando R+G. Verificar tres resistencias integradas en la placa; no alimentar LED sin limitación de corriente. |
| Pack **120 Dupont de 10 cm** | **Compra confirmada**: 40 M-M, 40 H-H, 40 H-M | Unir pines y protoboard. |
| Tira recta macho de **40 pines, paso 2,54 mm** | **Compra confirmada** | Cortar y soldar cabeceras faltantes de OLED/sensor/NodeMCU; la soldadura y herramientas se hacen en el DEC. |
| USB A–microUSB, Arduino Uno, pulsador, dos resistencias sueltas | Visibles en fotos | USB para programación/alimentación; el resto no se conecta en este modelo. No se conoce el valor de las dos resistencias. |

**Protoboard:** habían señalado que el DEC tiene protoboards, pero no aparece una en las fotos ni en la compra nueva. **Llévense una prestada** si aún no la retiraron: permite compartir 3V3, GND, SDA y SCL con dos módulos sin empalmar cables. **No se compró el kit de accesorios** propuesto antes, así que no contamos con tres LED independientes ni resistencias de 220 Ω verificadas. Con el módulo RGB no hacen falta esos tres LED.

## 2. Cuatro verificaciones antes del USB

1. Foto legible de **ambos lados del CCS811**. Identificar pin de alimentación (`VCC`, `VIN` o `3V3`), `GND`, `SDA`, `SCL`, `WAKE`/`WAK` si está expuesto y `ADDR` si existe. **No conectar un pin desconocido a 5 V**. El montaje usa lógica de 3,3 V. El breakout genérico puede diferir del de Adafruit.
2. Leer en la OLED sus cuatro rótulos y comprobar si es **SSD1306 128×64**; si es 128×32 cambiar `OLED_ALTO` a `32` antes de cargar. Si usa SH1106, hay que cambiar la biblioteca/controlador del código. Sus contactos fotografiados parecen no tener pines soldados.
3. Mirar de cerca el RGB: confirmar que los componentes `R1`, `R2`, `R3` están **montados**, no solo dibujados en la PCB, y que el cuarto pin es `GND`. Si faltan resistencias, adquirir **dos de 220–330 Ω** para las líneas R y G antes de conectarlo. Si el módulo resulta ánodo común, invertir el circuito y el programa con ayuda del DEC.
4. Contar agujeros que requieren cabecera y soldar tramos de la tira con ayuda del DEC. La NodeMCU debe poder apoyarse firmemente en la protoboard; no mantener el cable solo contra una almohadilla. Usar multímetro para descartar cortos entre 3V3 y GND.

## 3. Cableado previsto (USB desconectado)

| NodeMCU ESP8266 | Componente | Conexión |
|---|---|---|
| `3V3` | positivo OLED y CCS811 | Compartir en riel de protoboard **solo tras verificar que ambos módulos admiten 3,3 V**. |
| `GND` | GND OLED, CCS811 y RGB | Tierra común en riel. |
| `D2` = GPIO4 | SDA OLED y CCS811 | Dos cables al mismo riel de datos I²C. |
| `D1` = GPIO5 | SCL OLED y CCS811 | Dos cables al mismo riel de reloj I²C. |
| `GND` | `WAKE`/`WAK` del CCS811 si el módulo lo expone y requiere nivel bajo | Revisar serigrafía; no confundir con `ADDR`. |
| `D5` = GPIO14 | `R` del módulo RGB | **Solo si tiene resistencia integrada**; de otro modo poner 220–330 Ω en serie. |
| `D6` = GPIO12 | `G` del módulo RGB | Misma condición. |
| Sin conexión | `B` del RGB, pulsador, Arduino Uno, dos resistencias sueltas | No participan en el circuito inicial. |

La OLED suele tener dirección `0x3C` o `0x3D`, el CCS811 `0x5A` o `0x5B`. El programa prueba esas cuatro posibilidades. Ningún pin del ESP8266 debe recibir señales de 5 V. **No guiarse por el color del cable, sino por la serigrafía.**

## 4. Arduino IDE y primera prueba

1. Instalar **ESP8266 Boards** por el gestor de placas y seleccionar `NodeMCU 1.0 (ESP-12E Module)`. Instalar bibliotecas `Adafruit CCS811`, `Adafruit SSD1306`, `Adafruit GFX Library` y las dependencias que solicite el gestor, entre ellas `Adafruit BusIO`.
2. Abrir **solo** [`firmware/NUEVO_ESP8266_CCS811/NUEVO_ESP8266_CCS811.ino`](../firmware/NUEVO_ESP8266_CCS811/NUEVO_ESP8266_CCS811.ino). Cargar con el USB de datos y abrir Monitor Serie a `115200`.
3. Primero conectar **CCS811 + NodeMCU**, sin OLED/RGB, y ver `CCS811 conectado`, TVOC y eCO₂. El código continúa por Serial aunque falte OLED. Si no responde, apagar USB y revisar alimentación, SDA/SCL, WAKE, dirección y soldaduras.
4. Apagar USB; añadir el módulo RGB con sus resistencias verificadas. En el arranque debe parpadear amarillo; si el sensor falla, parpadea rojo. Apagar USB; añadir OLED y ver los datos. Si permanece negra, revisar dirección/controlador/altura y soldadura.
5. Dejar funcionar por **20 min** cada vez; para un sensor nuevo, la recomendación del fabricante citada por Adafruit es un rodaje inicial de unas **48 h** para lecturas más estables. Luego el sketch toma 30 muestras para una referencia local. Mostrar TVOC y diferencias, no atribuir las luces a niveles validados de CO₂ o salud.
6. Registrar un ensayo de **5 min**: hora, lectura TVOC, eCO₂ estimado, referencia, color, foto y cualquier corrección. La falta de cambio de color es también un resultado válido; no simularlo como una medición real.

## 5. Indicador y alcance

La comparación ilustrativa toma TVOC actual menos promedio de referencia: **menos de +50 ppb** verde, **+50 a +149 ppb** amarillo, **+150 ppb o más** rojo. En calentamiento/referencia parpadean R y G juntos; en falla parpadea R. Son umbrales de la **demostración relativa**, no límites de ventilación ni recomendaciones clínicas. El CCS811 no mide CO₂ directo. La OLED y el RGB funcionan sin Wi-Fi, aplicación, Internet ni buzzer.

La caja anterior se diseñó para ESP32 y otro sensor: **no imprimirla**. Tras confirmar funcionamiento, medir NodeMCU, OLED, CCS811, módulo RGB y posición del microUSB; hacer la ventana y ventilación de la caja con esas medidas. Sin foto y medidas del sensor comprado no existe todavía un modelo 3D validado.

Fuentes: [CCS811 de Adafruit](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview), [cableado y dirección](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/arduino-wiring-test), [pines de NodeMCU](https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h).
