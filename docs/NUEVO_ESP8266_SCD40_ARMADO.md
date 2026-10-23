# NUEVO — ESP8266 + SCD40 + tres LED

Este programa sirve **solo con un sensor SCD40/SCD41 real**. La foto del dibujo identifica un SCD40, pero **no permite comprobar qué chip hay físicamente**. Si la pieza comprada dice CCS811, no mide CO₂ real y este firmware no la detectará. Confirmar la serigrafía antes de conectar o presentar el prototipo como medidor de CO₂.

## Conexiones

| ESP8266 NodeMCU | Conectar a | Función |
|---|---|---|
| `3V3` | `VIN`/`VDD` del **breakout SCD40 compatible con 3,3 V** | Alimentación; consultar serigrafía de la placa específica |
| `GND` | `GND` SCD40 y riel negativo protoboard | Tierra común |
| `D2` (GPIO4) | `SDA` SCD40 | Datos I²C, dirección `0x62` |
| `D1` (GPIO5) | `SCL` SCD40 | Reloj I²C |
| `D5` (GPIO14) | Cable morado a **a29** → resistencia **c29–c26** → ánodo LED verde **e26** | Cátodo LED **e25** a GND en columna **25** |
| `D6` (GPIO12) | Cable amarillo a **a23** → resistencia **c23–c21** → ánodo LED amarillo **e21** | Cátodo LED **e20** a GND en columna **20** |
| `D7` (GPIO13) | Cable café a **a15** → resistencia **e15–c13** → ánodo LED rojo **e13** | Cátodo LED **e12** a GND en columna **12** |
| `D4` (GPIO2) | Pulsador, contacto opuesto a GND | Opcional: prueba de los LED al mantenerlo pulsado **después de arrancar** |

**ALTO ANTES DE CONECTAR USB:** según la descripción había cables grises de GND en **a15 y a23**. Esas posiciones comparten conexión con D7 y D6: si realmente son tierra, producen cortocircuito. Moverlos a las columnas **12** y **20** de los cátodos; añadir tierra a **25** para el verde. Con el circuito apagado, medir continuidad entre cada cátodo y GND, y ausencia de continuidad entre D5/D6/D7 y GND. Medir que cada resistencia es de aproximadamente **220–330 Ω** y confirmar la polaridad de cada LED.

El dibujo recibido todavía rotula pulsador **D3**, pero la conexión descrita es **D4** y el programa usa D4. En D4/GPIO2 no se puede mantener el pulsador a tierra mientras se enciende o reinicia el ESP8266. Confirmar que al soltarlo no une D4 con GND.

En la placa SCD40 se requieren **cuatro conexiones reales**: alimentación, GND, SDA y SCL. No extrapolar posiciones físicas de la foto del diagrama a un breakout distinto. Si el breakout necesita más corriente de la que entrega el regulador 3V3 de su NodeMCU, usar una fuente 3,3 V estable con **GND común**, respetando su ficha técnica; no llevar 5 V a los GPIO del ESP8266.

## Código y prueba

1. Instalar ESP8266 Boards en Arduino IDE y seleccionar **NodeMCU 1.0 (ESP-12E Module)**. Instalar **Sensirion I2C SCD4x** y sus dependencias desde el gestor de bibliotecas.
2. Abrir [NUEVO_ESP8266_SCD40.ino](../firmware/NUEVO_ESP8266_SCD40/NUEVO_ESP8266_SCD40.ino), verificar, cargar por USB y abrir Monitor Serie a **115200 baudios**. No presionar D4 durante encendido o carga.
3. Sin SCD40 conectado, debe parpadear el LED rojo y aparecer «SCD40 ausente en 0x62». Mantener D4 presionado después de que arranque: el programa enciende verde, amarillo y rojo por turnos para probar las salidas.
4. Con USB retirado y cableado revisado, conectar SCD40 y encender. Mientras se espera la primera medición parpadea amarillo; después, el monitor serie mostrará `CO2=... ppm` aproximadamente cada **5 segundos**. Si no aparece `0x62`, revisar VIN/GND y D2/SDA/D1/SCL.
5. **Verde:** menos de 700 ppm. **Amarillo:** 700–1300 ppm inclusive. **Rojo fijo:** más de 1300 ppm. **Rojo intermitente:** falla o lectura vencida. Estos umbrales son decisiones de presentación para el prototipo, no un certificado de seguridad ambiental.
6. Registrar fotos de cableado, lectura en Monitor Serie y colores. No cerrar el sensor en una caja hermética; necesita intercambio de aire. La foto del sensor físico y una prueba en la placa siguen pendientes.

Fuentes: [biblioteca Sensirion SCD4x](https://github.com/Sensirion/arduino-i2c-scd4x), [ejemplo oficial](https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/examples/exampleUsage/exampleUsage.ino), [guía de alimentación y cableado SCD40](https://learn.adafruit.com/adafruit-scd-40-and-scd-41/arduino).
