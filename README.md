# NUEVO — Indicador de cambios de aire de aula (ESP8266 + CCS811)

**Montaje vigente para el Avance 2.** La placa principal es la **NodeMCU ESP8266** de las fotografías. El sensor **CCS811 comprado** entrega TVOC (ppb) y un cálculo de **eCO₂ estimado** (ppm). El indicador se ve en la OLED y en el módulo RGB ya disponible: verde = cambio pequeño, amarillo = cambio moderado, rojo = cambio alto respecto de una referencia tomada al iniciar. **No es un medidor de CO₂ real ni un semáforo de seguridad sanitaria o de ventilación.**

## Abrir solo estos dos archivos

1. [NUEVO_ESP8266_CCS811_ARMADO.md](docs/NUEVO_ESP8266_CCS811_ARMADO.md): inventario conocido, condiciones que hay que comprobar, conexiones y prueba paso a paso.
2. [NUEVO_ESP8266_CCS811.ino](firmware/NUEVO_ESP8266_CCS811/NUEVO_ESP8266_CCS811.ino): único programa del montaje vigente; seleccionar **NodeMCU 1.0 (ESP-12E Module)** en Arduino IDE.

**Estado real:** documentación y código adaptados a ESP8266 y al módulo RGB fotografiado; no se ha compilado en Arduino IDE ni ensayado con estas placas. Se necesita comprobar la serigrafía/tensión del CCS811, controlador y altura de la OLED, resistencias del RGB y soldar los pines que faltan. La tira de pines comprada no queda conectada por sí sola. Una protoboard facilita repartir 3V3/GND/SDA/SCL; el equipo indicó disponibilidad en DEC, pero no hay fotografía que confirme que ya la retiró.

El Arduino Uno y el pulsador fotografiados quedan como repuestos sin conectar en este primer circuito. No se usa Internet, aplicación, buzzer ni tres LED separados. **La antigua carcasa para ESP32 no corresponde a este montaje:** medir las placas ensambladas y diseñar la caja después de que la electrónica funcione.

## Qué mostrar

Encender con anticipación: el CCS811 requiere un período inicial de funcionamiento y unos **20 minutos de estabilización por encendido**. Durante la espera el módulo parpadea en amarillo. Luego se registra la referencia TVOC y se ve la lectura en OLED/Serial. Registrar una prueba continua de cinco minutos y explicar que eCO₂ es estimado. En falla se indica rojo intermitente; nunca se anuncia verde sin datos.

Fuentes: [Adafruit: CCS811 y estabilización](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview), [conexión CCS811](https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/arduino-wiring-test), [pines NodeMCU](https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h).
