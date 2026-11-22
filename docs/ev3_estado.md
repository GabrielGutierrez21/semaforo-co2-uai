# Guía para la presentación EV3

La evaluación combina **Presentación 3** y **revisión final de GitHub** (50 % cada una), según la indicación del docente. Durante la exposición se puede omitir la navegación por GitHub y la exhibición de planos, pues se revisan aparte. La pauta aportada por el equipo fija una presentación presencial de **8–10 minutos** y **3 minutos de preguntas**, con demostración del prototipo.

## Secuencia sugerida

1. Problema concreto de monitorear cambios ambientales en una sala; alcance real del CCS811.
2. Montaje físico: ESP8266, CCS811, OLED numérica, tres LED y pulsador. Indicar que WAKE va a GND y que sensor y OLED comparten I²C.
3. Abrir la web desde un celular conectado a `Aire-UAI`, en `http://192.168.4.1`. Mostrar TVOC y eCO₂ estimado, carita y LED correspondientes, y abrir ambas tarjetas de explicación.
4. Mostrar historial CSV que persiste tras reiniciar la placa. Señalar sesiones y segundos desde arranque, sin llamarlos hora real.
5. Explicar el protocolo, resultados reales que haya documentado el equipo y sus límites. No presentar eCO₂ como CO₂ real ni los colores como umbrales sanitarios.

Para que la carita ya clasifique durante la demo, encender el sensor con **20 minutos de anticipación** y esperar la referencia de 30 muestras. Si se presenta antes, mostrar honestamente el estado de calentamiento y las lecturas disponibles. Llevar un registro CSV real y capturas de la interfaz como respaldo por si el celular se desconecta.

## Documentos del repositorio

- [Montaje y esquema](../hardware/README.md).
- [Firmware y uso](../firmware/SemaforoAmbiental/README.md).
- [Arquitectura web y API](../software/README.md).
- [Modelo físico](../diseno-3d/README.md).
- [Protocolo de pruebas](../testing/protocolo.md).
- [Fuentes y atribución](../FUENTES.md).
