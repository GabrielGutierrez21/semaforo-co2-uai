# Pruebas, registro y demostración

## Tres ensayos completos

Para cada ensayo, guardar fecha, sala, número aproximado de personas, posición del sensor, alimentación, duración, registro Serial CSV, si la red local funcionó y fallas. Registrar los resultados reales en una tabla como esta:

| Ensayo | Duración | Rango ppm real | LED observado | Página local | Incidente | Cambio realizado |
|---|---|---|---|---|---|---|
| 1 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| 2 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| 3 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |

No rellenar con datos inventados. Para observar una tendencia, medir unos minutos en una sala ocupada y luego ventilar; anotar horario de apertura de puerta/ventana y número de personas. No esperen alcanzar 1300 ppm durante una demo corta: usen el pulsador de **modo prueba** para enseñar colores, dejando visibles la etiqueta y la lectura real.

## Casos que deben verificar

- Arranque: amarillo parpadea hasta primera lectura real.
- Verde: lectura inicial menor que 700 ppm; amarillo: de 700 a 1300 ppm; rojo: mayor que 1300 ppm.
- Histéresis: cambios ±50 ppm alrededor de los cortes no hacen parpadear el semáforo.
- Desconexión del sensor: tras 20 s sin dato válido, rojo y amarillo parpadean; página indica error; al reconectar se recupera.
- Teléfono fuera de la red: las luces siguen respondiendo al sensor; al reconectar, la página vuelve a mostrar datos.
- Cinco minutos: sin reinicios, cables flojos ni lectura en blanco. Repetir tres veces.
- Modo prueba: las luces cambian pero el sitio dice claramente **MODO PRUEBA** y jamás llama real a un valor simulado.

## Guion para 5–7 minutos

| Tiempo | Persona | Demostración |
|---|---|---|
| 0:00–0:40 | 1 | Problema de ventilación en salas y usuarios; explicar que el CO₂ indica cómo cambia la ventilación en contexto, no mide todos los contaminantes. |
| 0:40–2:10 | 2 | Mostrar ppm reales en Serial y luz real; seguir el cable sensor → ESP32 → LEDs. |
| 2:10–3:00 | 3 | Teléfono en red local y página en 192.168.4.1; señalar transmisión sin Internet. |
| 3:00–4:00 | 2 | Pulsador con estado verde/amarillo/rojo en **modo prueba**. Volver a modo real. |
| 4:00–5:10 | 4 | Abrir modelo 3D, dimensiones, ventilación, USB, fijaciones, ubicación del sensor. |
| 5:10–6:20 | 1 y 3 | Mostrar registro de ensayos e iteraciones verdaderas; comentar límites y próxima mejora. |

Durante toda la explicación, dejar conectado y midiendo el sistema. Llevar cable USB, powerbank, cables Dupont y LEDs de repuesto. Registrar fotos del prototipo ensamblado y una captura de las lecturas reales.

La guía del curso pondera funcionamiento 36 %, integración 21 %, encapsulado 14 %, iteración/testing 14 %, documentación 7 % y comunicación 7 % (redondeo de porcentajes de la pauta).
