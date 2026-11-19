# Conexiones del dispositivo local

La tabla sigue los nombres **D1...D7 impresos en la placa NodeMCU** y coincide con [`SemaforoAmbiental.ino`](../firmware/SemaforoAmbiental/SemaforoAmbiental.ino). Todas las tierras comparten GND.

| NodeMCU ESP8266 | Destino | Función |
|---|---|---|
| D1, GPIO5 | SCL de CCS811 y SCL de OLED | Reloj I²C compartido |
| D2, GPIO4 | SDA de CCS811 y SDA de OLED | Datos I²C compartidos |
| D4, GPIO2 | Una pata del pulsador; la otra a GND | Prueba de LED con `INPUT_PULLUP` |
| D5, GPIO14 | Resistencia serie → ánodo LED verde | Estado verde |
| D6, GPIO12 | Resistencia serie → ánodo LED amarillo | Estado amarillo |
| D7, GPIO13 | Resistencia serie → ánodo LED rojo | Estado rojo |
| 3V3 | VCC del CCS811 y de OLED | Alimentación de 3,3 V |
| GND | GND del sensor/OLED, WAKE del CCS811, cátodos LED, pulsador | Referencia común |
| USB | Puerto USB del NodeMCU | Alimentación y carga del programa |

Cada LED lleva **una resistencia propia** (valor objetivo 330 Ω, verificar el valor de las resistencias del montaje real), en serie entre el pin y el ánodo. El LED conduce desde pin → resistencia → ánodo → cátodo → GND. No conectar LEDs directamente a los pines. Si el módulo CCS811 tiene pin `nWAKE/WAKE`, mantenerlo a GND para permitir la comunicación I²C; no se asigna un GPIO a WAKE en este firmware.

**Direcciones detectadas:** CCS811 `0x5A` o `0x5B` y OLED `0x3C` o `0x3D`. Antes de unir la alimentación, comprobar con la serigrafía y ficha de **los módulos comprados** que aceptan 3,3 V en el pin usado y confirmar que los pines I²C tengan los pull-up necesarios en sus breakouts. No se deben conectar salidas de 5 V a GPIO del ESP8266.

## Secuencia de conexión

1. Desconectar USB. Instalar NodeMCU y protoboard, identificando claramente los rieles 3V3 y GND; algunas protoboards tienen rieles interrumpidos.
2. Montar cada LED con resistencia serie y retorno a GND. Comprobar polaridad de cada LED.
3. Cablear D4 al pulsador y el contacto opuesto a GND. No mantenerlo pulsado mientras inicia la placa.
4. Conectar OLED y CCS811 en paralelo a D1/D2, 3V3 y GND; conectar WAKE del sensor a GND.
5. Verificar continuidad y ausencia de corto entre 3V3 y GND antes de conectar USB. Abrir Serial a 115200 y comprobar los mensajes de detección.

El botón recorre los LED para probar el cableado; mientras está pulsado las luces **no representan** calidad del aire. El CCS811 informa TVOC y **eCO₂ estimado**, no CO₂ directo.
