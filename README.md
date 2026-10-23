# NUEVO — Prototipo ESP8266 + CCS811 + tres LED + pulsador

**Montaje vigente según las conexiones comunicadas el 23-10-2026.**

- [NUEVO_ESP8266_CCS811_ARMADO.md](docs/NUEVO_ESP8266_CCS811_ARMADO.md): inventario, pin a pin, columnas de protoboard, precauciones y pruebas.
- [NUEVO_ESP8266_CCS811.ino](firmware/NUEVO_ESP8266_CCS811/NUEVO_ESP8266_CCS811.ino): único programa vigente para NodeMCU ESP8266. Los LED verde, amarillo y rojo usan D5, D6 y D7; el pulsador usa D4; el CCS811 usa D2/SDA y D1/SCL.

La imagen del esquema recibido rotula **SCD40** y **D3** para el botón. Esos rótulos son anteriores y **no representan este circuito**: el sensor comprado es **CCS811** y el pulsador informado está en **D4**. La OLED y el módulo RGB de fotografías previas **no se conectan** en este montaje.

El CCS811 entrega **TVOC y eCO2 estimado**, no medición directa de CO2. Los colores representan una variación relativa de TVOC respecto de la referencia local; no son umbrales de seguridad ni de ventilación.

**Estado:** código y guía actualizados según la descripción y foto del montaje parcial. No se ha compilado en Arduino IDE ni se ha probado con las placas del equipo. Antes de alimentar: verificar la serigrafía y 3,3 V del CCS811, valores de las tres resistencias, polaridad de los LED, rieles de la protoboard y conexiones del pulsador. No imprimir la carcasa antigua; medir el montaje final primero.
