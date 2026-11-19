# Firmware vigente

Cargar `SemaforoAmbiental.ino` en NodeMCU ESP8266. CCS811 y OLED comparten I²C: D1=SCL, D2=SDA. D5/D6/D7 controlan los LED verde/amarillo/rojo y D4 recibe el pulsador a GND. WAKE del CCS811 va a GND. La OLED muestra TVOC y eCO₂ estimado, sin caritas. Véanse las instrucciones de la raíz y `hardware/conexiones.md`.
