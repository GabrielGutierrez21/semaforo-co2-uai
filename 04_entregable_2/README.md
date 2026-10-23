# Entregable 2 — evidencia del avance

## Qué presentar

| Criterio de la pauta | Peso | Evidencia en este repositorio | Evidencia presencial |
|---|---|---|
| Funcionamiento del alfa | 36 % | [Código final](../02_prototipo_final/NUEVO_ESP8266_CCS811_OLED.ino) | Encender y demostrar sensor, actuadores y operación continua **al menos 5 minutos**. |
| Integración | 21 % | [Conexiones](../02_prototipo_final/README.md) | Mostrar ESP8266, CCS811, OLED, botón y LED funcionando juntos. |
| Diseño de encapsulado conceptual | 14 % | [Modelo 3D](../03_modelado_3d/) | Mostrar dimensiones, base/tapa, ventilación, acceso USB y mantenimiento. No se exige impresión física. |
| Iteración y pruebas | 14 % | [Primer prototipo](../01_primer_prototipo/) → [final](../02_prototipo_final/) | Mostrar evidencia fechada de cambios, prueba continua y relación con el problema/ODS. |
| Documentación técnica | 7 % | Código comentado y tablas de pines en las guías | Aportar esquema eléctrico completo y lista de componentes. |
| Comunicación | 7 % | Esta guía | Exposición de **5–7 minutos** con demostración en vivo, participación de todo el equipo y **2 minutos** de retroalimentación. |

**Conectividad de la pauta:** el montaje documentado comunica CCS811 y OLED mediante I²C, sin internet ni aplicación. La pauta pide evidenciar conexión y transmisión de datos (menciona Wi-Fi/Bluetooth). Confirmen con el equipo docente si la demostración I²C cumple ese criterio; no presenten el prototipo como conectado a Wi-Fi si no lo está.

## Registro de prueba para completar con datos reales

| Fecha y lugar | Duración | TVOC inicio/fin (ppb) | eCO₂ estimado (ppm) | LED/carita observados | Observaciones |
|---|---|---|---|---|---|
| Por registrar | Por registrar | Por registrar | Por registrar | Por registrar | Por registrar |

**Demostración sugerida:** mostrar encendido y espera, lectura y pantalla, transición de LED/carita si se observa durante la medición, y pulsador como prueba de salidas. Explicar que la respuesta se interpreta como **cambio relativo de TVOC**, no como medición directa de CO₂ ni diagnóstico de ventilación. No presentar la secuencia del botón como lectura ambiental.

**Antes de entregar:** agregar fotos reales del montaje y una captura/video de funcionamiento, completar la tabla con los datos medidos, consignar integrantes/roles y verificar que el enlace público del repositorio abra sin iniciar sesión.
