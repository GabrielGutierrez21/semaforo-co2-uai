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

## Verificación de web e historial

| Paso | Acción | Evidencia | Criterio |
|---|---|---|---|
| 7 | Conectar el celular a `Aire-UAI` y abrir `http://192.168.4.1` sin Internet. | Captura de red, web y OLED en el mismo instante. | TVOC/eCO₂ coinciden con OLED o Serie y cambian al recibir nuevas lecturas; sin sensor aparece `--`. |
| 8 | Tras dos registros separados al menos 60 s, descargar `/historico.csv`. | CSV original y captura de la gráfica. | CSV contiene sesión, segundos, TVOC, eCO₂, referencia y estado; gráfica usa esas filas. |
| 9 | Reiniciar el ESP8266, volver a la web y descargar CSV. | CSV antes/después y log del arranque. | Persisten filas anteriores y aumenta el número de sesión. |
| 10 | Desconectar temporalmente el celular de la red local. | Captura del aviso en la interfaz. | La web muestra `Sin conexión` y deja de enseñar un valor viejo como vigente. |

**Análisis:** comparar al menos tres pares de valores Serie/OLED/web, medir el intervalo real entre registros, contar filas antes y después del reinicio y explicar discrepancias. Si LittleFS no monta, registrar el error y verificar la partición de flash antes de afirmar persistencia.

**Integridad:** cualquier dato, gráfico o captura presentado debe provenir del circuito real. No completar CSV con números simulados.

**Prueba de autonomía con 2 AA:** realizarla solamente después de confirmar el pin de entrada, polaridad y regulación de voltaje según [`hardware/conexiones.md`](../hardware/conexiones.md#alimentacion-con-dos-pilas-aa). Registrar el voltaje en vacío y mientras funciona el Wi‑Fi, el número de reinicios y el tiempo efectivo de operación. El portapilas modelado por sí solo no valida la autonomía.
