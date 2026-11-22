# Firmware vigente

Abrir `SemaforoAmbiental.ino` en Arduino IDE con `WebUI.h` en esta misma carpeta. El programa integra CCS811, LED, OLED, servidor web local y registro CSV LittleFS. El [README principal](../../README.md) contiene instalación y uso; [software/README.md](../../software/README.md) explica los endpoints.

Pines: D1=SCL, D2=SDA para CCS811 y OLED; D4=pulsador a GND; D5/D6/D7=LED verde/amarillo/rojo con resistencias propias; WAKE a GND. La OLED muestra números. Las caritas se dibujan **solo en la web**. Encienda el equipo 20 minutos antes de interpretar el estado y espere las 30 muestras de referencia. No pulsar D4 mientras inicia.
