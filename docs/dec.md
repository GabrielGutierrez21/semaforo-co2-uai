# Qué hacer en el DEC

La guía de Avance 02 indica que el modelo 3D conceptual es obligatorio y que **no es necesario tener la carcasa impresa** para ese avance. También menciona acceso al laboratorio DEC para impresión si deciden avanzar. El sitio UAI sitúa el DEC de Peñalolén en edificio D y el de Viña del Mar en edificio F. Confirmar directamente disponibilidad, procedimientos y horarios; no asumir que se puede llegar e imprimir sin reserva.

## Antes de ir

- Llevar ESP32, breakout SCD40 real, cables, LEDs y USB.
- Llevar `cad/carcasa.scad`, STL, fotos y medidas con regla o pie de metro.
- Llevar el diagrama de `conexiones.md` y explicar que la alimentación es solo USB de baja tensión.
- Consultar si pueden usar multímetro, protoboard, cautín, impresora 3D y filamento; preguntar condiciones de préstamo o capacitación.

## En la mesa de electrónica

1. Identificar el modelo exacto de ESP32 y breakout. Confirmar pines, tensión de entrada y conectores.
2. Probar SCD40 solo, luego LEDs y pulsador. Solicitar revisión de cableado antes de energizar.
3. Mostrar al encargado cinco minutos de lecturas continuas y la página local, o registrar el fallo que hay que corregir.
4. Si todo funciona, fijar en placa perforada o asegurar los cables de protoboard para la demo. Evitar soldar antes de comprobar cada función.

## En la mesa de fabricación

1. Medir largo, ancho, alto y ubicación USB del ESP32; largo, ancho, alto y abertura sensible del breakout.
2. Editar medidas y posiciones de `cad/carcasa.scad`. El tamaño 120 × 76 × 30,4 mm del modelo es una hipótesis, no una medida del prototipo real.
3. Abrir `parte="base"` y `parte="tapa"` por separado. Revisar en el laminador paredes, huecos, orientación, soportes y tiempo de impresión con el personal del DEC.
4. Hacer un primer montaje en seco. Ajustar ranuras de ventilación, salida USB, orificios de LED, botón y postes. Imprimir la versión corregida solo si cabe en el tiempo disponible.
5. Tomar fotos del modelo, dimensiones, primer montaje, problema encontrado y corrección. Eso es evidencia de iteración.

Si no imprimen, presenten el archivo OpenSCAD manipulable, sus vistas, cotas y distribución interna; el requisito de esa pauta es **conceptualizar** el encapsulado.

Fuentes: [Infraestructura UAI](https://www.uai.cl/ingenieria-y-ciencias/facultad/infraestructura-y-laboratorios) y guía Avance 02 entregada por el curso.
