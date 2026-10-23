# NUEVO — ESP8266 + CCS811 + OLED 0,96 pulgadas + LED

Versión de prueba con pantalla **I²C SSD1306 de 128×64 píxeles y cuatro pines**. Confirmar en la serigrafía de la OLED `GND`, `VCC`, `SCL` y `SDA`. Si el controlador real es SH1106 o la resolución no es 128×64, esta versión de pantalla requiere otro controlador o dimensiones.

## Conectar con el USB desconectado

| Pieza | ESP8266 NodeMCU | Conexión |
|---|---|---|
| OLED VCC | 3V3 | Al mismo riel positivo que el CCS811, solo si el módulo OLED admite 3,3 V. |
| OLED GND | GND | Al mismo riel negativo que el CCS811. |
| OLED SDA | D2 (GPIO4) | Comparte línea de datos con SDA del CCS811. |
| OLED SCL | D1 (GPIO5) | Comparte línea de reloj con SCL del CCS811. |
| CCS811 alimentación, GND | 3V3, GND | Mantener conexiones funcionales actuales. |
| CCS811 WAKE | GND | Debe permanecer en bajo; no necesita pin digital. |
| LED verde, amarillo, rojo | D5, D6, D7 | Cada LED con su propia resistencia en serie hacia GND. |
| Pulsador | D4 y GND | Soltarlo durante encendido y reinicio. |

No conectar 5 V a SDA/SCL del ESP8266. La OLED suele responder en `0x3C` o `0x3D`; el CCS811, en `0x5A` o `0x5B`. El programa detecta ambas direcciones habituales. Los cables grises de tierra no deben tocar las filas a15/a23 que reciben D7/D6. Antes de alimentar, comprobar continuidad y ausencia de cortos.

## Cargar y probar

1. En Arduino IDE seleccionar **NodeMCU 1.0 (ESP-12E Module)**. Instalar **Adafruit CCS811**, **Adafruit SSD1306** y **Adafruit GFX Library** con las dependencias que solicite el gestor.
2. Abrir [`NUEVO_ESP8266_CCS811_OLED.ino`](../firmware/NUEVO_ESP8266_CCS811_OLED/NUEVO_ESP8266_CCS811_OLED.ino), verificar y cargar; abrir Monitor Serie a **115200**. El programa debe informar las direcciones del CCS811 y la OLED.
3. La OLED muestra TVOC en ppb, eCO₂ **estimado**, estado y carita. Verde = feliz (variación TVOC menor que +50 ppb respecto de la referencia), amarillo = neutral (+50 a +149 ppb), rojo = triste (≥+150 ppb). Al inicio indica calentamiento y después toma 30 lecturas de referencia. Sensor ausente/falla: cara con ojos en X y rojo intermitente. Si falla la OLED, los LED y el monitor serie siguen funcionando.
4. Para esta prueba el calentamiento del programa es **1 minuto**. Al quedar encendido toma 30 muestras de referencia antes de clasificar. Para presentar lecturas más estables, cambiar `CALENTAMIENTO_MS` a `20UL * 60UL * 1000UL`, cargar el código de nuevo y encender con anticipación. El fabricante recomienda alrededor de 48 horas de funcionamiento inicial cuando el sensor es nuevo.

**Límite de interpretación:** el CCS811 no mide CO₂ real. La pantalla dice `eCO2 SOLO ESTIMADO`; el gesto y los LED expresan cambios en TVOC frente a la referencia local, no un umbral sanitario de CO₂. No usar los antiguos umbrales de 700/1300 ppm con esta configuración.
