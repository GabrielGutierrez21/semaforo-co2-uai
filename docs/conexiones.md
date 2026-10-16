# Cableado y puesta en marcha

## Esquema eléctrico

**Desconectar USB durante el cableado.** Compartir GND entre todos los módulos. La tabla asume un módulo SCD40 con entrada lógica compatible con **3,3 V**; revisar la ficha del breakout exacto antes de conectarlo. El SCD40 puede exigir picos de corriente relevantes: confirmar con el DEC que el regulador de 3V3 de *su* ESP32 soporta el sensor y el ESP32 con Wi-Fi simultáneamente. Si aparece un reinicio por alimentación y usan el **Adafruit 5187**, su VIN acepta 5 V y tiene regulador propio; consultar el pin 5V/VIN del ESP32 antes de recablear. La alternativa SparkFun puede tener otro circuito de alimentación.

| Origen | Destino | Nota |
|---|---|---|
| ESP32 `3V3` | SCD40 `VIN` o `3V3` según breakout | Para Adafruit 5187, VIN acepta 3,3 V; otras placas pueden variar. No poner 5 V en un pin marcado solo `3V3`. |
| ESP32 `GND` | SCD40 `GND` | Masa común. |
| ESP32 `GPIO19` | SCD40 `SDA` | Datos I²C. |
| ESP32 `GPIO18` | SCD40 `SCL` | Reloj I²C. |
| ESP32 `GPIO25` | 330 Ω → ánodo LED verde | Cátodo (pata corta) → GND. |
| ESP32 `GPIO26` | 330 Ω → ánodo LED amarillo | Cátodo → GND. |
| ESP32 `GPIO27` | 330 Ω → ánodo LED rojo | Cátodo → GND. |
| ESP32 `GPIO33` | Pulsador → GND | El firmware habilita resistencia interna `INPUT_PULLUP`. |
| ESP32 `GPIO32` | 1 kΩ → base NPN | Solo si se instala el buzzer. |
| NPN emisor / colector | GND / negativo buzzer | Positivo buzzer a 3V3 si su ficha acepta 3,3 V. Comprobar disposición E-B-C del transistor físico. |

En sensores Adafruit 5187, los pines expuestos son VIN/GND/SCL/SDA y el SCD40 trabaja por I²C. No unir el positivo del buzzer directamente al GPIO. Si el buzzer es magnético, solicitar al DEC una etapa de manejo con protección adecuada; el montaje propuesto supone un buzzer **activo piezoeléctrico** de baja corriente. `USAR_BUZZER` está en `false` por defecto; el **DEC #708 KY-006 es pasivo** y necesita generación de tono y nuevo circuito validado. El pulsador DEC #726 KY-004 debe revisarse por serigrafía/continuidad para identificar los contactos del interruptor.

La pantalla OLED **DEC #778 es una ampliación pendiente**, no una función implementada. Si el módulo concreto es I²C compatible con 3,3 V, podría compartir GPIO19 SDA y GPIO18 SCL con el SCD40; comprobar dirección, alimentación, pull-ups, biblioteca y montaje antes de modificar el firmware y la tapa. No conectar una pantalla sin verificar su interfaz.

## Secuencia de depuración

1. Con multímetro, comprobar ausencia de corto entre 3V3 y GND antes de conectar USB.
2. Conectar solo ESP32 y SCD40. Cargar primero el ejemplo `exampleUsage` de la biblioteca de Sensirion. Ver en Serial a 115200 una lectura de ppm aproximadamente cada 5 s.
3. Si no aparecen datos, comprobar 3V3/GND y SDA=19, SCL=18. Revisar la serigrafía de la placa, no fiarse del color de cable. Si se usa un ESP32 distinto, cambiar pines en el código.
4. Agregar cada LED con su resistencia. Subir el firmware del proyecto y comprobar arranque, error y modo prueba.
5. Agregar pulsador. Si más adelante se dispone de buzzer **activo** y etapa validada, activar `USAR_BUZZER`; el código ya viene con `false`.
6. Encender la red local del ESP32; el celular se conecta a `Semaforo-CO2-UAI`. Su aviso «sin acceso a Internet» es normal. Abrir **http://192.168.4.1** y verificar que el valor coincide con el monitor serie.

El sensor debe recibir aire de la sala a través de ranuras; ubicarlo separado del calor del ESP32, lejos de una ventana abierta directamente, de personas soplándole encima y de una fuente de aire que no represente la sala.
