# Semáforo ambiental · Aire UAI

Dispositivo autónomo para observar **cambios relativos de TVOC** en una sala. Un **NodeMCU ESP8266** lee un **CCS811**, enciende tres LED, muestra las lecturas en una OLED y publica una **web local con aspecto de aplicación móvil**. El celular se conecta directamente a la red Wi‑Fi del dispositivo: no se requiere Internet, app instalada ni servidor externo.

> El CCS811 informa TVOC y **eCO₂ estimado**, no una medición directa de CO₂. El semáforo usa únicamente la diferencia de TVOC respecto de una referencia tomada tras el calentamiento. Sus colores son orientativos para esta demostración, no límites de salud ni certificados de ventilación.

## Equipo

Gabriel Gutiérrez · Hai Hu · Bastián Cabrera · Lionel Bravo · Samuel Acevedo.

## Problema y solución

En salas compartidas es difícil observar cómo cambian los compuestos volátiles durante el uso diario. Este primer prototipo muestra una tendencia local fácil de interpretar mediante LED, OLED y una interfaz de celular con carita feliz, neutra o triste. Cada tarjeta explica qué mide el sensor y el alcance de esa lectura.

## Arquitectura

`CCS811 (I²C) → ESP8266 → LED + OLED + red Wi‑Fi local → navegador del celular`

El ESP8266 guarda un registro CSV en **LittleFS** cada 60 segundos cuando hay lectura válida. La web consulta `/api/estado` cada dos segundos y dibuja los últimos 60 registros. El CSV incluye número de sesión y segundos desde el encendido; **no inventa hora de calendario** porque el prototipo no tiene reloj ni sincronización externa.

## Estructura

| Ruta | Contenido |
|---|---|
| [`firmware/SemaforoAmbiental/`](firmware/SemaforoAmbiental/) | Código vigente: sensor, LED, OLED, servidor local, historial y página integrada. |
| [`firmware/historico/`](firmware/historico/) | Firmware del primer prototipo, conservado como iteración. |
| [`iteraciones/`](iteraciones/) | Primer prototipo y carcasa 3D anterior; recorrido hasta la versión actual. |
| [`software/`](software/) | Arquitectura, API y funcionamiento de la web local. |
| [`hardware/`](hardware/) | BOM, conexiones, esquema y fotografías disponibles. |
| [`diseno-3d/`](diseno-3d/) | Carcasa actual: plano, STEP, STL, script para Fusion 360 y paquete original. |
| [`testing/`](testing/) | Protocolo de comprobación con datos obtenidos de la placa. |
| [`docs/`](docs/) | Documentos de avance y pauta para la presentación EV3. |
| [`FUENTES.md`](FUENTES.md) | Librerías, documentación técnica y atribuciones. |

## Montaje

1. Cablear según [hardware/conexiones.md](hardware/conexiones.md): **D1 = SCL**, **D2 = SDA** compartidos por CCS811 y OLED; **D4 = botón a GND**; **D5/D6/D7 = LED verde/amarillo/rojo**, cada uno con resistencia serie y cátodo a GND; **WAKE del CCS811 a GND**. Usar 3,3 V en los módulos compatibles y tierra común.
2. Instalar el núcleo **ESP8266 Arduino** y las bibliotecas **Adafruit CCS811**, **Adafruit SSD1306**, **Adafruit GFX** y su dependencia **Adafruit BusIO**; véase [FUENTES.md](FUENTES.md).
3. En Arduino IDE seleccionar **NodeMCU 1.0 (ESP‑12E Module)** y una configuración de flash con espacio para LittleFS. Abrir [firmware/SemaforoAmbiental/SemaforoAmbiental.ino](firmware/SemaforoAmbiental/SemaforoAmbiental.ino) y cargarlo por USB. `WebUI.h` debe permanecer en la misma carpeta. No se necesita cargar archivos web por separado.
4. Soltar el pulsador D4 durante el reinicio. Abrir Monitor Serie a **115200 baudios** y comprobar que aparecen el CCS811, la OLED y la dirección de la web. Si LittleFS no monta, la pantalla y los LED continúan, pero la web indicará que no se guarda el historial.

## Uso de la aplicación local

1. Alimentar el equipo por USB para programarlo o mediante el portapilas **2 × AA**, cuyo positivo está conectado a `3V3` y negativo a `GND` en el montaje reportado. Usar una fuente a la vez y [medir la tensión bajo carga](hardware/conexiones.md#alimentacion-con-dos-pilas-aa) para documentar la autonomía. En el celular, conectarse a **Aire-UAI** con la clave configurada en el dispositivo. El aviso «Sin Internet» en el teléfono es normal: mantener esa red.
2. Abrir **http://192.168.4.1** en el navegador. La página se adapta a la pantalla del teléfono. Los valores proceden de la placa y muestran `--` si no hay lectura; nunca usa datos de muestra.
3. Esperar **20 minutos** de acondicionamiento del CCS811 en cada uso y luego **30 muestras** para establecer la referencia de TVOC. Para la exposición, encender el prototipo con suficiente anticipación. Durante la espera la web muestra los números disponibles y la cuenta restante, sin clasificar el aire.
4. La carita feliz/neutra/triste y los LED reflejan **bajo / medio / alto cambio relativo de TVOC**. Tocar TVOC o eCO₂ abre su explicación. La pantalla OLED muestra números y estado, sin caritas.
5. Pulsar D4 para recorrer los LED como prueba eléctrica. Descargar `historico.csv` desde la web para revisar los registros; una rotación mantiene el archivo anterior en `/historico-anterior.csv`.

**Umbrales exploratorios:** verde por debajo de referencia + 50 ppb; amarillo entre +50 y +149 ppb; rojo desde +150 ppb. La concentración absoluta de TVOC, la salud de las personas y la ventilación real no se pueden determinar mediante estos colores.

## Reproducibilidad y evidencias

La [guía de software](software/README.md) describe los endpoints y el registro. El [protocolo](testing/protocolo.md) indica cómo documentar una prueba real con fotos, capturas, CSV y reinicio. El diseño físico se mantiene en [diseno-3d/](diseno-3d/) para continuar allí con los archivos de fabricación. No se incluyen registros experimentales fabricados.
