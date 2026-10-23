# NUEVO — Conexión real: ESP8266, CCS811, tres LED y pulsador

## Inventario actual

| Pieza | Estado |
|---|---|
| NodeMCU ESP8266, cable USB A–microUSB | Fotografiados |
| CCS811, 120 Dupont (10 cm), tira de 40 pines macho 2,54 mm | Compra confirmada; **sin foto legible del sensor físico** |
| Protoboard, LED verde/amarillo/rojo, resistencias en serie, pulsador y cables | Montaje parcial en la foto recibida; **valor de las resistencias y continuidad aún por medir** |
| OLED y módulo RGB de fotos antiguas; Arduino Uno | Disponibles pero **no conectarlos** en esta versión |

## Conexionado según tu descripción

| NodeMCU | Protoboard / pieza | Función |
|---|---|---|
| `3V3` | Riel positivo (+), quinto agujero indicado | Alimentación **solo a 3,3 V** para CCS811 compatible |
| `GND` | Riel negativo (−). Según tu relato hay puentes grises en a15, a12, a20 y a23 | **Conflicto: a15 comparte columna con D7 y a23 con D6. No alimentar hasta aclararlo y retirar los puentes a GND que caigan en a15/a23.** Los cátodos deben ir a columnas distintas: e12, e20 y e25. |
| `D2` (GPIO4) | `SDA` CCS811 | Datos I²C |
| `D1` (GPIO5) | `SCL` CCS811 | Reloj I²C |
| `D5` (GPIO14) | Cable morado a **a29** → resistencia **c29–c26** → LED **e26–e25** | Verde. Confirmar que **e25** es cátodo y llega a GND |
| `D6` (GPIO12) | Cable amarillo a **a23** → resistencia **c23–c21** → LED **e21–e20** | Amarillo. Confirmar que **e20** es cátodo y llega a GND |
| `D7` (GPIO13) | Cable café a **a15** → resistencia **e15–c13** → LED **e13–e12** | Rojo. Confirmar que **e12** es cátodo y llega a GND |
| `D4` (GPIO2) | Pulsador; otro contacto al riel negativo (mencionaste «menos, fila 8») | `INPUT_PULLUP`: libre = HIGH; presionado = LOW |

**CCS811:** además de D1 y D2 necesita alimentación y GND. Confirmar en la **serigrafía de su placa** qué pin es `VIN`/`3V3`, `GND`, `SDA`, `SCL` y `WAKE`. Si el breakout expone `WAKE` y exige bajo para medir, unirlo a GND. No aplicar 5 V ni seguir el dibujo de **SCD40** como pinout del CCS811. El sensor no aparece conectado en la foto, así que ese tramo no está comprobado.

**ALTO, no conectar USB todavía:** los cinco agujeros a–e de la columna **15** tienen continuidad entre sí, igual que los de la **23**. Si el cable gris realmente termina en **a15**, hace corto de D7 a GND; si termina en **a23**, hace corto de D6 a GND. Mover esos grises a los cátodos **e12** y **e20** (por ejemplo a12 y a20) y comprobar que el tercero esté en **e25** (por ejemplo a25). No usar a15, a23 ni a29 como tierra de esos LED. Verificar con multímetro continuidad de cada cátodo al riel negativo y **ausencia** de continuidad de D5/D6/D7 al riel negativo antes de alimentar.

**Protoboard:** a–e de la misma columna numérica están unidos entre sí; el canal central separa a–e de f–j. Los rieles +/− pueden estar cortados a la mitad en ciertas protoboards. Comprobar con multímetro. Para cada LED debe existir una sola cadena `Dx → resistencia → ánodo LED → cátodo LED → GND`. El esquema dibujado indica 330 Ω; medir las resistencias reales (220–330 Ω es adecuado para la primera prueba con 3,3 V). Si falta resistencia o el LED está invertido, corregir antes de alimentar. El color gris no demuestra que un agujero sea tierra: comprobar que llegue al riel negativo.

**Pulsador D4:** verificar con continuidad que *solo al presionarlo* se una D4 a GND y nunca a 3V3. El esquema viejo dice D3; el código usa tu cableado **D4**. D4/GPIO2 debe estar en alto al arrancar el ESP8266: **soltar el botón al conectar USB, cargar el programa o reiniciar**. Si no arranca, desconectar el botón, arrancar y revisar sus contactos.

## Cargar y comprobar, en este orden

1. Sin USB, medir con multímetro las resistencias, los cátodos de LED a GND, ausencia de cortocircuito entre +/− y continuidad del botón únicamente al pulsar. Comprobar que los tres LED no toman 3V3 directamente como ánodo si el código pretende manejarlos desde D5–D7.
2. En Arduino IDE instalar el soporte **ESP8266 Boards** y seleccionar **NodeMCU 1.0 (ESP-12E Module)**. Instalar **Adafruit CCS811** y las dependencias que solicite el gestor (por ejemplo Adafruit BusIO). Abrir [solo este sketch](../firmware/NUEVO_ESP8266_CCS811/NUEVO_ESP8266_CCS811.ino), verificar/compilar y cargar por USB.
3. Abrir Monitor Serie a **115200**. Sin CCS811, debe mostrar `CCS811 no detectado` y parpadear el LED rojo. Con botón mantenido **después del arranque**, los tres LED se encienden de uno en uno: **esto solo prueba el cableado**, no la calidad del aire. Si no ocurre, apagar USB y revisar primero polaridad/filas/cables/resistencias.
4. Con USB desconectado, conectar CCS811 a 3V3, GND, D2/SDA y D1/SCL conforme a su serigrafía; `WAKE` a GND si requiere bajo. Encender **sin presionar D4**. El Serial debe indicar dirección `0x5A` o `0x5B`, TVOC en ppb y eCO2 **estimado** en ppm. Si no detecta el sensor, apagar y corregir pines y soldaduras.
5. Esperar **20 minutos** de estabilización por encendido (amarillo intermitente), luego 30 lecturas para referencia (amarillo intermitente). Después: verde por cambio TVOC < +50 ppb; amarillo +50 a +149; rojo ≥ +150. Son **umbrales ilustrativos**, no criterios sanitarios. Sin dato reciente o con falla: rojo intermitente. Un sensor nuevo puede necesitar un rodaje inicial de alrededor de 48 h para mayor estabilidad.
6. Registrar fotos y cinco minutos de lecturas/colores reales. No atribuir el color a un valor medido de CO2. La carcasa 3D se diseña después de medir el circuito que efectivamente funciona.

No hay Wi-Fi, frontend, buzzer ni OLED en el programa. El pulsador solo prueba los LED mientras se mantiene presionado.

Fuentes: [Adafruit CCS811](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview), [cableado CCS811](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/arduino-wiring-test), [pines y arranque ESP8266](https://github.com/esp8266/Arduino/blob/master/doc/boards.rst).
