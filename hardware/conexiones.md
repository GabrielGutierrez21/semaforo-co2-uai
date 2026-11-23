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
| 3V3 | VCC del CCS811 y de OLED; positivo del portapilas 2 × AA en el montaje informado | Línea de alimentación |
| GND | Negativo del portapilas, GND del sensor/OLED, WAKE del CCS811, cátodos LED y pulsador | Referencia común |
| USB | Puerto USB del NodeMCU | Carga del programa y alimentación de prueba, con las pilas desconectadas |

La [carcasa actual](../diseno-3d/) incluye una cuna para **portapilas de 2 AA**. El montaje informado conecta **positivo a `3V3` y negativo a `GND`**; el [esquema](esquematico.pdf) muestra esa conexión junto con los demás componentes.

## Alimentación con dos pilas AA

**Conexión reportada:** portapilas `+` → `3V3` y portapilas `−` → `GND`. La polaridad debe comprobarse en los cables reales. Dos AA entregan aproximadamente 3 V nominales si son alcalinas (2,4 V si son recargables NiMH), y el voltaje disminuye con el uso. Al entrar por `3V3` se evita el regulador de la placa: esta conexión **no garantiza 3,3 V estables** ni ausencia de reinicios cuando el Wi‑Fi transmite. No conectar el portapilas a un GPIO ni a `VIN` como si fueran 5 V.

Para documentar el resultado real, medir el voltaje entre `3V3` y `GND` **con el dispositivo encendido y el punto de acceso Wi‑Fi funcionando**, registrar el tipo de pilas y observar si se reinicia durante una prueba continua. Desconectar las pilas antes de alimentar o programar por USB, salvo que se haya comprobado expresamente el circuito de selección de fuentes de esta versión de NodeMCU. Las pilas alcalinas no se recargan desde el puerto USB.

En la prueba del equipo anotar: tipo de pilas, voltaje en reposo y con Wi‑Fi activo, presencia de interruptor y duración sin reinicios. El firmware no cambia según la fuente de alimentación; el registro eléctrico permite evaluar la autonomía de este montaje.

Cada LED lleva **una resistencia propia** (valor objetivo 330 Ω, verificar el valor de las resistencias del montaje real), en serie entre el pin y el ánodo. El LED conduce desde pin → resistencia → ánodo → cátodo → GND. No conectar LEDs directamente a los pines. Si el módulo CCS811 tiene pin `nWAKE/WAKE`, mantenerlo a GND para permitir la comunicación I²C; no se asigna un GPIO a WAKE en este firmware.

**Direcciones detectadas:** CCS811 `0x5A` o `0x5B` y OLED `0x3C` o `0x3D`. Antes de unir la alimentación, comprobar con la serigrafía y ficha de **los módulos comprados** que aceptan 3,3 V en el pin usado y confirmar que los pines I²C tengan los pull-up necesarios en sus breakouts. No se deben conectar salidas de 5 V a GPIO del ESP8266.

## Secuencia de conexión

1. Desconectar USB. Instalar NodeMCU y protoboard, identificando claramente los rieles 3V3 y GND; algunas protoboards tienen rieles interrumpidos.
2. Montar cada LED con resistencia serie y retorno a GND. Comprobar polaridad de cada LED.
3. Cablear D4 al pulsador y el contacto opuesto a GND. No mantenerlo pulsado mientras inicia la placa.
4. Conectar OLED y CCS811 en paralelo a D1/D2, 3V3 y GND; conectar WAKE del sensor a GND.
5. Verificar continuidad y ausencia de corto entre 3V3 y GND antes de conectar USB. Abrir Serial a 115200 y comprobar los mensajes de detección.

El botón recorre los LED para probar el cableado; mientras está pulsado las luces **no representan** calidad del aire. El CCS811 informa TVOC y **eCO₂ estimado**, no CO₂ directo.
