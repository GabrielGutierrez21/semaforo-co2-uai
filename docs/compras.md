# Lista de compra para una unidad

> **Compra vigente para Avance #2:** ver [modelo CCS811 con NodeMCU ESP8266](modelo_ccs811_avance_02.md). Este documento conserva el presupuesto del diseño futuro con SCD40/SCD41 y ESP32. Las piezas y conexiones de abajo no corresponden a las placas fotografiadas el 21-10-2026.

**Comprobar primero qué componentes ya están en el Circuito UAI y en el equipo.** La compra indispensable para convertir la prueba de temperatura en el proyecto declarado es un módulo de CO₂ real. Precios y stock cambian: confirmar antes de pagar, especialmente plazo de entrega a Chile.

El [catálogo público DEC](https://dec.uai.cl/componentes/) consultado el 16-10-2026 no mostró sensores CO₂ al buscarlos; el inventario publicado no garantiza stock. Preguntar presencialmente si hay uno fuera de catálogo. Un módulo genérico SCD40/SCD41 montado se anunciaba en Chile alrededor de $13.500–$16.000 CLP, sujeto a verificar vendedor, autenticidad y entrega. La placa Adafruit 5187 en Mouser rondaba $51.210 CLP antes de otros cargos, pero no es obligatoria.

| Prioridad | Cantidad | Componente y especificación | Uso |
|---|---:|---|---|
| Imprescindible | 1 | **Breakout SCD40 o SCD41 I²C ya montado**, prestado o comprado; revisar chip, alimentación, pinout y fecha de entrega. | Medición de CO₂ real en ppm. El chip SMD suelto no sirve para protoboard. |
| Imprescindible si no lo tienen | 1 | ESP32 DevKit **DEC #787** con USB y GPIO18, 19, 25, 26, 27, 33. | Control y Wi-Fi local. Confirmar variante física. |
| Imprescindible | 1 de cada color | LED difuso verde, amarillo y rojo de 5 mm. | Señal visible desde el equipo. |
| Imprescindible | 3 | Resistencias de **330 Ω**, 1/4 W. | Una en serie con cada LED; no conectar LEDs desnudos al GPIO. |
| Imprescindible | 1 | Pulsador normalmente abierto, **DEC #726 KY-004**; verificar qué dos pines forman el contacto. | Cambiar entre lectura real y demostración identificada. |
| Imprescindible | 1 | Protoboard **DEC #802**; 10–15 cables Dupont cortos (preguntar si prestan). | Montaje. Usar placa perforada solo después de que funcione la protoboard. |
| Imprescindible | 1 | Cable USB de datos, **DEC #844/#845 MicroUSB** si coincide con su ESP32; fuente USB 5 V o powerbank ≥1 A. | Alimentación y programación. Verificar conector. |
| Según módulo | 1 | Cable STEMMA QT/Qwiic a extremos libres **o** tira de pines macho y cables Dupont; comprobar conectores de la placa elegida. | Unir SCD40 al ESP32 sin adivinar el pinout por colores. |
| Opcional | 1 | Buzzer **activo** de 3,3–5 V y bajo consumo, si hay. El **DEC #708 KY-006 es pasivo** y requiere otro control por software. | Aviso corto después de 10 s en rojo; por defecto está desactivado en firmware. |
| Opcional, pendiente de integrar | 1 | Pantalla **DEC #778 OLED 0,96 pulgadas**. Confirmar I²C, tensión, dirección, pines y medidas. | Mostrar ppm en el propio equipo tras agregar código y una ventana a la carcasa. |
| Opcional con buzzer | 1 + 1 | Transistor NPN 2N2222 o equivalente y resistencia 1 kΩ. | Accionar buzzer sin cargar directamente el GPIO32. |
| Carcasa | ~80 g | Filamento PLA o PETG del laboratorio; consultar disponibilidad. | Base y tapa de escritorio. |
| Carcasa | 4 | Tornillos M3 de longitud apropiada para tapa y postes; personal del DEC debe validar rosca y largo. | Cierre desmontable. |
| Carcasa | 4 | Bridas pequeñas y, opcionalmente, cuatro patas adhesivas. | Fijar placas y estabilizar base. |

**No sustituir CO₂:** el **DEC #659 MQ-7** mide monóxido de carbono (CO), no CO₂; el **#662 MQ-135** responde a varios gases y no mide CO₂ de forma selectiva en ppm. El **#750 LED RGB KY-016** podría reemplazar las tres luces, pero requeriría adaptar cableado, resistencias, firmware y carcasa. Preguntar por tres LEDs separados, resistencias de 330 Ω y Dupont, que no aparecieron individualmente en el catálogo consultado.

Otras alternativas **no compatibles aún con el firmware**: MH-Z19C/E mide CO₂ real, pero requiere 5 V, UART/PWM, verificar niveles lógicos y adaptar código. STCC4 con SHT4x mide CO₂ con menor precisión y también requiere módulo/código distintos. Mantener OLED y buzzer como mejoras opcionales hasta demostrar lectura real y luces. No comprar un sensor eCO₂/VOC para afirmar medición directa de CO₂.
