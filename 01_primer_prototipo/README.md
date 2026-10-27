# Primer prototipo — semáforo de aire

**Montaje implementado:** NodeMCU ESP8266, CCS811, tres LED con resistencias y pulsador. Señal visible autónoma, sin Internet ni aplicación. El sensor lee **TVOC (ppb)** y calcula **eCO₂ estimado (ppm)**; no mide CO₂ real.

**Conexiones:** CCS811 SDA→D2, SCL→D1, alimentación→3V3, GND→GND y WAKE→GND; LED verde→D5, amarillo→D6 y rojo→D7, cada uno con resistencia y retorno a GND; pulsador D4→GND.

**Funcionamiento:** espera inicial, toma 30 lecturas para una referencia local de TVOC y luego compara la diferencia: verde <+50 ppb, amarillo de +50 a +149 ppb, rojo ≥+150 ppb. El botón recorre los LED para mostrar el funcionamiento eléctrico. El monitor serie informa valores y estado a 115200 baudios.

- [Código del primer prototipo](NUEVO_ESP8266_CCS811.ino)

