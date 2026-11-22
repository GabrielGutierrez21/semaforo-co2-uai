# Software · web local en ESP8266

La interfaz de celular está integrada en [`firmware/SemaforoAmbiental/WebUI.h`](../firmware/SemaforoAmbiental/WebUI.h) y se sirve directamente desde la memoria flash del ESP8266. El servidor y el sensor están en [`SemaforoAmbiental.ino`](../firmware/SemaforoAmbiental/SemaforoAmbiental.ino). No hay Node.js, hosting, Internet, claves de servicios ni un segundo circuito.

## Flujo de datos

1. El CCS811 entrega TVOC (ppb) y eCO₂ estimado (ppm) por I²C.
2. El programa toma una referencia de 30 muestras de TVOC tras 20 minutos de acondicionamiento.
3. Estado y LED dependen de la diferencia de TVOC frente a esa referencia: menos de +50 ppb, +50 a +149 ppb, o +150 ppb en adelante.
4. El servidor HTTP local responde cada consulta con valores vigentes o `null` si la lectura está ausente/desactualizada. La web consulta cada 2 s y muestra `--` sin inventar cifras.
5. LittleFS registra una lectura válida cada 60 s, con límite de 64 kB por archivo. Al alcanzar ese límite rota el CSV anterior; ambos sobreviven a reinicios normales.

## API local

| Ruta | Respuesta |
|---|---|
| `GET /` | HTML/CSS/JS adaptable a celular, incrustado en el firmware. |
| `GET /api/estado` | JSON con estado, TVOC, eCO₂, referencia, sesión, segundos desde arranque, muestras de referencia y estado de almacenamiento. |
| `GET /historico.csv` | CSV actual con `sesion,segundos,tvoc_ppb,eco2_estimado_ppm,base_tvoc_ppb,estado`. |
| `GET /historico-anterior.csv` | CSV de la rotación anterior. |

No existe reloj en el circuito: `segundos` es tiempo desde el encendido y `sesion` cambia en cada arranque si LittleFS está disponible. La web grafica los últimos 60 registros del archivo actual; el navegador no guarda lecturas ficticias.

## Instalación

Usar el procedimiento del [README principal](../README.md). Elegir espacio de sistema de archivos para la placa en Arduino IDE y cargar ambos archivos del sketch en la misma carpeta. No hace falta herramienta adicional de carga LittleFS porque la página está compilada en `WebUI.h`; LittleFS guarda únicamente CSV y contador de sesión.

Si el teléfono insiste en usar datos móviles, desactivar temporalmente esa preferencia o escribir `http://192.168.4.1` explícitamente. El punto de acceso local tiene nombre `Aire-UAI` y clave `AireUAI2026`, modificables en el firmware. La red es solo para la demostración y no debe tratarse como un sistema de seguridad o monitoreo clínico.

## Alcance de las caritas

Las caritas, los LED y la OLED representan la **variación de TVOC** respecto de la referencia. La tarjeta eCO₂ se muestra por transparencia técnica, pero no clasifica el estado. Pulsar las tarjetas abre texto explicativo sobre el sensor y el posible efecto de algunos VOC; no se ofrecen diagnósticos ni recomendaciones basadas en umbrales sanitarios.

Código de interfaz y firmware elaborado para este proyecto; bibliotecas y fuentes documentadas en [FUENTES.md](../FUENTES.md).
