# Lista de compra para una unidad

**Comprobar primero qué componentes ya están en el Circuito UAI y en el equipo.** La compra indispensable para convertir la prueba de temperatura en el proyecto declarado es un módulo de CO₂ real. Precios y stock cambian: confirmar antes de pagar, especialmente plazo de entrega a Chile.

Referencia consultada el 16-10-2026: Mouser Chile mostraba el **Adafruit 5187 a $51.210 CLP**, con stock indicado, antes de los cargos de entrega, aduana e impuestos que correspondan. El pedido de un solo sensor puede salir bastante más caro al incluir envío; preguntar primero en el DEC si lo prestan.

| Prioridad | Cantidad | Componente y especificación | Uso |
|---|---:|---|---|
| Imprescindible | 1 | **Breakout SCD40 I²C ya montado**, preferido [Adafruit 5187 en Mouser Chile](https://www.mouser.cl/es/ProductDetail/Adafruit/5187) o [fabricante](https://www.adafruit.com/product/5187); alternativa [SparkFun SEN-22395 en Mouser Chile](https://www.mouser.cl/en/ProductDetail/SparkFun/SEN-22395). Confirmar cuál llega antes. | Medición de CO₂ real en ppm. El componente SCD40-D-R1 suelto requiere montaje SMD y no sirve para una protoboard. |
| Imprescindible si no lo tienen | 1 | ESP32 DevKit con USB y pines GPIO18, 19, 25, 26, 27, 33. | Control y Wi-Fi local. |
| Imprescindible | 1 de cada color | LED difuso verde, amarillo y rojo de 5 mm. | Señal visible desde el equipo. |
| Imprescindible | 3 | Resistencias de **330 Ω**, 1/4 W. | Una en serie con cada LED; no conectar LEDs desnudos al GPIO. |
| Imprescindible | 1 | Pulsador normalmente abierto para protoboard. | Cambiar entre lectura real y demostración identificada. |
| Imprescindible | 1 | Protoboard mediana o placa perforada; 10–15 cables Dupont cortos. | Montaje. Usar placa perforada solo después de que funcione la protoboard. |
| Imprescindible | 1 | Cable USB de datos compatible con **su ESP32** y fuente USB 5 V o powerbank de al menos 1 A. | Alimentación y programación. Verificar tipo USB antes de comprar. |
| Según módulo | 1 | Cable STEMMA QT/Qwiic a extremos libres **o** tira de pines macho y cables Dupont; comprobar conectores de la placa elegida. | Unir SCD40 al ESP32 sin adivinar el pinout por colores. |
| Opcional | 1 | Buzzer **activo** de 3,3–5 V, de bajo consumo. | Aviso corto después de 10 s en rojo. |
| Opcional con buzzer | 1 + 1 | Transistor NPN 2N2222 o equivalente y resistencia 1 kΩ. | Accionar buzzer sin cargar directamente el GPIO32. |
| Carcasa | ~80 g | Filamento PLA o PETG del laboratorio; consultar disponibilidad. | Base y tapa de escritorio. |
| Carcasa | 4 | Tornillos M3 de longitud apropiada para tapa y postes; personal del DEC debe validar rosca y largo. | Cierre desmontable. |
| Carcasa | 4 | Bridas pequeñas y, opcionalmente, cuatro patas adhesivas. | Fijar placas y estabilizar base. |

**No comprar para esta versión:** otro sensor de temperatura HDC1080, Raspberry Pi Pico, sensor MQ-135 para afirmar ppm de CO₂, convertidor de 220 V ni clavijas para enchufe. El SCD40 ya incorpora medición auxiliar de temperatura y humedad; el prototipo se alimenta por USB.

Alternativa de compra: si el SCD40 no llega a tiempo, SCD41 **en breakout** usa el mismo bus y el programa, pero comprobar su módulo y su precio. No sustituirlo por un sensor eCO₂/VOC si el objetivo es medir CO₂ directamente.
