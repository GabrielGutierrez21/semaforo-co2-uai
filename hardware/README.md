# Hardware

Los archivos de esta carpeta describen el montaje con **una protoboard** y un portapilas de **2 AA**, con positivo conectado a `3V3` y negativo a `GND`. El cable USB se utiliza para programar y probar con las pilas desconectadas. La [guía de conexiones](conexiones.md) explica cómo medir la tensión bajo carga:

- [`conexiones.md`](conexiones.md): tabla de pines, alimentación, precauciones para 2 AA y procedimiento de comprobación.
- [`esquematico.pdf`](esquematico.pdf): diagrama funcional de las interconexiones; su fuente editable es [`esquematico.dot`](esquematico.dot).
- [`BOM.csv`](BOM.csv): componentes, portapilas y referencias de costo. No se ha recibido una boleta completa del equipo; los costos indicados son **precios públicos de referencia**, no pagos efectivamente realizados.
- [`fotos/montaje_led_protoboard.jpeg`](fotos/montaje_led_protoboard.jpeg): fotografía real del montaje con LED y ESP8266 en protoboard aportada por el equipo.
- [`fotos/componentes_esp8266_oled.jpeg`](fotos/componentes_esp8266_oled.jpeg): fotografía real del ESP8266, OLED y controles antes del ensamble final.

Las dos imágenes muestran etapas distintas: **no acreditan** la integración final del CCS811, la OLED encendida ni tres vistas del ensamble. Para EV3 faltan tres fotografías del dispositivo final desde ángulos distintos y una verificación eléctrica del circuito real.
