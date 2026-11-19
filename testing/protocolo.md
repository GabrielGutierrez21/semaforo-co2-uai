# Protocolo para la demostración y validación

**Objetivo:** comprobar que el ESP8266 obtiene valores reales del CCS811, los comunica por OLED y Serie, y activa las señales esperadas sin interrupción. La variación de TVOC orienta una conversación sobre fuentes de compuestos volátiles en la sala; eCO₂ es una estimación **que no valida CO₂ ni la ventilación por sí sola**.

**Montaje:** NodeMCU y componentes según [`hardware/conexiones.md`](../hardware/conexiones.md), alimentación USB estable, lugar y hora registrados, OLED visible y Monitor Serie a 115200 baudios. Mantener constante la ubicación del sensor y evitar tocar el elemento sensible. Tras encender, dejar 20 minutos de acondicionamiento del modo usado; en la primera puesta en uso, atender además la guía del fabricante para el periodo inicial.

## Procedimiento

| Paso | Acción | Evidencia a conservar | Criterio de éxito |
|---|---|---|---|
| 1 | Encender sin pulsar D4, identificar el sensor en Serie y la OLED. | Foto con cableado, foto OLED y primeras líneas del log. | Direcciones I²C detectadas y no aparece `SIN SENSOR`. |
| 2 | Mantener encendido 20 min y dejar tomar las 30 muestras de referencia. | Log con inicio y mensaje `Referencia TVOC`. | Aparece referencia y termina `CALENTANDO`/`BASE TVOC`. |
| 3 | Registrar 5 min de ambiente sin intervención, conservando hora y lecturas a 1 s. | CSV y log sin recortes selectivos. | Sin reinicio, sin ausencia de datos por más de 10 s y con estados coherentes. |
| 4 | Pulsar D4 mientras se observa el equipo, luego soltar. | Vídeo corto y anotación de hora. | Los LED ciclan al pulsar y vuelven al estado ambiental al soltar. Esta secuencia no es una medición. |
| 5 | Exponer brevemente al sensor a una fuente controlada de VOC adecuada al laboratorio, evitando líquidos, contacto y vapores nocivos; volver al aire de referencia. | Log y gráfica TVOC frente al tiempo. | TVOC responde y el LED coincide con la diferencia respecto de la referencia. Si no cruza umbrales, registrar el resultado sin forzarlo. |
| 6 | Desconectar el sensor solo con el sistema apagado; reiniciar y observar el diagnóstico. | Foto OLED, log de error y comportamiento LED. | El equipo señala `SIN SENSOR` en vez de presentar un valor previo como vigente. |

**Análisis solicitado:** calcular número de muestras esperadas y recibidas en la ventana de 5 min, máximo tiempo sin dato, latencia entre evento y cambio de estado, media y rango TVOC en referencia y exposición; registrar al menos un fallo real, causa comprobada y modificación. Si no hay instrumento de referencia de CO₂, no informar precisión de CO₂ ni afirmar que se validó ventilación.

**Integridad de datos:** cualquier captura usada en la presentación debe corresponder a estas pruebas reales. Al implementar la web local, repetir los pasos 2–5 con navegador conectado y verificar que el histórico persiste tras reiniciar el ESP8266.
