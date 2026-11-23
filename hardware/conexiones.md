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

La [carcasa actual](../diseno-3d/) incluye una cuna para **portapilas de 2 AA**. El equipo informa que conectó las pilas directamente a la placa; esa conexión no aparece en el esquema USB y se debe identificar y medir antes de declararla como alimentación autónoma reproducible.

## Alimentación con dos pilas AA

**No asumir que la carcasa de pilas es un circuito de alimentación.** Dos AA entregan aproximadamente 3 V nominales si son alcalinas (2,4 V si son recargables NiMH), y el voltaje baja con la descarga. Esa salida no equivale a 5 V en `VIN` o en el conector USB; tampoco es una fuente regulada de 3,3 V para la línea `3V3`, especialmente cuando el Wi‑Fi demanda corriente. No conectar las dos AA a un pin GPIO. Desconectar las pilas para medir sin alimentación USB.

Para una versión autónoma reproducible con este NodeMCU, usar **portapilas 2 AA → convertidor elevador adecuado para dos AA → salida regulada de 5 V (al menos 500 mA, preferible mayor margen) → entrada de alimentación de 5 V confirmada en la placa o cable micro USB**, y unir el negativo con GND. Comprobar polaridad y 5 V reales con multímetro antes de conectar el NodeMCU. No unir a ciegas la salida del convertidor con `3V3`, ni alimentar por USB y por un pin externo simultáneamente sin verificar el esquema eléctrico específico de la placa. Esta ruta es una **recomendación técnica**, no se atribuye al montaje ya realizado.

En la prueba del equipo anotar: tipo y número de pilas, voltaje del portapilas en reposo y con Wi‑Fi activo, pin exacto usado para positivo y negativo, presencia de convertidor/interruptor y duración sin reinicios. Hasta verificar esos datos, el [esquema](esquematico.pdf) y el procedimiento siguiente describen la referencia USB.

Cada LED lleva **una resistencia propia** (valor objetivo 330 Ω, verificar el valor de las resistencias del montaje real), en serie entre el pin y el ánodo. El LED conduce desde pin → resistencia → ánodo → cátodo → GND. No conectar LEDs directamente a los pines. Si el módulo CCS811 tiene pin `nWAKE/WAKE`, mantenerlo a GND para permitir la comunicación I²C; no se asigna un GPIO a WAKE en este firmware.

**Direcciones detectadas:** CCS811 `0x5A` o `0x5B` y OLED `0x3C` o `0x3D`. Antes de unir la alimentación, comprobar con la serigrafía y ficha de **los módulos comprados** que aceptan 3,3 V en el pin usado y confirmar que los pines I²C tengan los pull-up necesarios en sus breakouts. No se deben conectar salidas de 5 V a GPIO del ESP8266.

## Secuencia de conexión

1. Desconectar USB. Instalar NodeMCU y protoboard, identificando claramente los rieles 3V3 y GND; algunas protoboards tienen rieles interrumpidos.
2. Montar cada LED con resistencia serie y retorno a GND. Comprobar polaridad de cada LED.
3. Cablear D4 al pulsador y el contacto opuesto a GND. No mantenerlo pulsado mientras inicia la placa.
4. Conectar OLED y CCS811 en paralelo a D1/D2, 3V3 y GND; conectar WAKE del sensor a GND.
5. Verificar continuidad y ausencia de corto entre 3V3 y GND antes de conectar USB. Abrir Serial a 115200 y comprobar los mensajes de detección.

El botón recorre los LED para probar el cableado; mientras está pulsado las luces **no representan** calidad del aire. El CCS811 informa TVOC y **eCO₂ estimado**, no CO₂ directo.
